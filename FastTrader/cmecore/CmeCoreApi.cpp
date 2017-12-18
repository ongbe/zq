#include "StdAfx.h"
#include "CmeCoreApi.h"
#include <string>
#include "../inc/Module-Misc/tools_util.h"

#include "ISvr/BusinessData.hpp"
#define TRADE_TIMER_ID  3400
#define QUOTE_TIMER_ID  3401
#define ELAPSE_TIME     8000
#define OVERTIME_LIMIT  3
static CCmeCoreApi* g_CmeCoreApi=NULL;
void CALLBACK TradeTimerProc(HWND hWnd, UINT uMsg, UINT_PTR,DWORD)
{
    g_CmeCoreApi->SendHeartbeat(true);
}
void CALLBACK QuotTimerProc(HWND hWnd, UINT uMsg, UINT_PTR,DWORD)
{
    g_CmeCoreApi->SendHeartbeat(false);
}
CCmeCoreApi::CCmeCoreApi():
m_pTradeDataWorker(NULL),
m_pQuotDataWorker(NULL),
m_pMdSpi(NULL),
m_pTraderSpi(NULL),
SubscribeUnsubscribeID(10000),
m_TradeOverTimeCount(0),
m_QuotOverTimeCount(0)
{
    g_CmeCoreApi=this;
}
CCmeCoreApi::~CCmeCoreApi()
{
}
CCmeCoreApi *CCmeCoreApi::CreateFtdcApi(const char *pszFlowPath, const bool bIsUsingUdp)
{
    return new CCmeCoreApi();
}
void CCmeCoreApi::Release()
{
}

void CCmeCoreApi::Init()
{
}
int CCmeCoreApi::Join()
{
    return 0;
}
const char * CCmeCoreApi::GetTradingDay()
{
    return "";
}
//格式为TradeFrontAddr:TradeFrontPort;QuotFrontAddr:QuotFrontPort
void CCmeCoreApi::ParseAddress(char *pszFrontAddress,Stru_IPAddress& TradeFrontAddr,
                               Stru_IPAddress& QuotFrontAddr)
{
    std::string strHostAddr,strPriceAddr,strIp,strPort,strAddr(pszFrontAddress);
	int pos=strAddr.find(";");
	strHostAddr=strAddr.substr(0,pos);
    strPriceAddr=strAddr.substr(pos+1);
	pos=strHostAddr.rfind(":");
	strIp=strHostAddr.substr(0,pos);
	strPort=strHostAddr.substr(pos+1);
    TradeFrontAddr.IP=inet_addr(strIp.c_str());
    TradeFrontAddr.Port=atoi(strPort.c_str());

	pos=strPriceAddr.rfind(":");
	strIp=strPriceAddr.substr(0,pos);
	strPort=strPriceAddr.substr(pos+1);
    QuotFrontAddr.IP=inet_addr(strIp.c_str());
    QuotFrontAddr.Port=atoi(strPort.c_str());
}

void CCmeCoreApi::RegisterFront(char *pszFrontAddress)
{
    ParseAddress(pszFrontAddress,m_TradeFrontAddr,m_QuotFrontAddr);
    m_pTradeDataWorker=new CTradeDataWorker(Stru_IPAddress(),m_TradeFrontAddr,this);
    m_pQuotDataWorker=new CQuotDataWorker(Stru_IPAddress(),m_QuotFrontAddr,this);
}
void CCmeCoreApi::RegisterSpi(CPatsQuotSpi *pSpi)
{
    m_pMdSpi=pSpi;
}
void CCmeCoreApi::RegisterSpi(CPatsTraderSpi *pSpi)
{
    m_pTraderSpi=pSpi;
}
int CCmeCoreApi::SubscribeMarketData(char *ppInstrumentID[], int nCount)
{
    map<string,EPCID>::const_iterator iter;
    for(int i=0;i<nCount;++i)
    {
        iter=m_ContractName2ID.find(ppInstrumentID[i]);
        if(iter!=m_ContractName2ID.end())
        {
            m_SubscribeUnsubscribeMarketMap[SubscribeUnsubscribeID]=iter->second.ContractID;
            m_pQuotDataWorker->SubscribeMarketData(iter->second.ContractID,SubscribeUnsubscribeID++);
        }
    }
    return 0;
}
int CCmeCoreApi::UnSubscribeMarketData(char *ppInstrumentID[], int nCount)
{
    map<string,EPCID>::const_iterator iter;
    for(int i=0;i<nCount;++i)
    {
        iter=m_ContractName2ID.find(ppInstrumentID[i]);
        if(iter!=m_ContractName2ID.end())
        {
            m_SubscribeUnsubscribeMarketMap[SubscribeUnsubscribeID]=iter->second.ContractID;
             m_pQuotDataWorker->UnSubscribeMarketData(iter->second.ContractID,SubscribeUnsubscribeID++);
        }
    }
    return 0;
}
void CCmeCoreApi::SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType)
{
}

void CCmeCoreApi::SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType)
{
}
///客户端认证请求
int CCmeCoreApi::ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID) 
{
    return 0;
}


///用户登录请求
int CCmeCoreApi::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) 
{
    int ret=0;
    if(strlen(pReqUserLoginField->UserProductInfo)>0)
        ret=m_pTradeDataWorker->ReqUserLogin(pReqUserLoginField,nRequestID);
    else
        ret=m_pQuotDataWorker->ReqUserLogin(pReqUserLoginField,nRequestID);
    return ret;
}



///登出请求
int CCmeCoreApi::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) 
{
    int ret=0;
    //if(strlen(pReqUserLoginField->UserProductInfo)>0)
        ret=m_pTradeDataWorker->ReqUserLogout(pUserLogout,nRequestID);
    //else
    //    ret=m_pQuotDataWorker->ReqUserLogout(pUserLogout,nRequestID);
    return ret;
}


///用户口令更新请求
int CCmeCoreApi::ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) 
{

    return m_pTradeDataWorker->ReqUserPasswordUpdate(pUserPasswordUpdate,nRequestID);
}


///资金账户口令更新请求
int CCmeCoreApi::ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) 
{
    return 0;
}


///报单录入请求
int CCmeCoreApi::ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID) 
{
    Stru_Base_OrderCmdInfo cmd;
    FillOrderCmdInfo(*pInputOrder,cmd);
    m_RequestID2OrderCmdFlag[nRequestID]=OrderCmdFlag_New;
    return m_pTradeDataWorker->ReqOrderCmd(cmd,nRequestID);
}


///预埋单录入请求
int CCmeCoreApi::ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID) 
{
    Stru_Base_OrderCmdInfo cmd;
    FillOrderCmdInfo(*pParkedOrder,cmd);
    m_RequestID2OrderCmdFlag[nRequestID]=OrderCmdFlag_New_Park;
    return m_pTradeDataWorker->ReqOrderCmd(cmd,nRequestID);
}


///预埋撤单录入请求
int CCmeCoreApi::ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID) 
{
    Stru_Base_OrderCmdInfo cmd;
    FillOrderCmdInfo(*pParkedOrderAction,cmd);
    m_RequestID2OrderCmdFlag[nRequestID]=OrderCmdFlag_Cancel_Park;
    return m_pTradeDataWorker->ReqOrderCmd(cmd,nRequestID);
}


///报单操作请求
int CCmeCoreApi::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID) 
{
    Stru_Base_OrderCmdInfo cmd;
    FillOrderCmdInfo(*pInputOrderAction,cmd);
    m_RequestID2OrderCmdFlag[nRequestID]=OrderCmdFlag_Cancel;
    return m_pTradeDataWorker->ReqOrderCmd(cmd,nRequestID);
}


///查询最大报单数量请求
int CCmeCoreApi::ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID) 
{
    return 0;
}


///投资者结算结果确认
int CCmeCoreApi::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID) 
{
   return m_pTradeDataWorker->ReqSettlementInfoConfirm(pSettlementInfoConfirm,nRequestID);
}


///请求删除预埋单
int CCmeCoreApi::ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID) 
{
    return 0;
}


///请求删除预埋撤单
int CCmeCoreApi::ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID) 
{
    return 0;
}


///请求查询报单
int CCmeCoreApi::ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) 
{
    return  m_pTradeDataWorker->ReqQryOrder(pQryOrder,nRequestID);
}


///请求查询成交
int CCmeCoreApi::ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID) 
{
    return  m_pTradeDataWorker->ReqQryTrade(pQryTrade,nRequestID);
}


///请求查询投资者持仓
int CCmeCoreApi::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID) 
{
    return 0;
}


///请求查询资金账户
int CCmeCoreApi::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) 
{
   return m_pTradeDataWorker->ReqQryTradingAccount(pQryTradingAccount,nRequestID);
}
///请求查询投资者
int CCmeCoreApi::ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID) 
{
    return 0;
}


///请求查询交易编码
int CCmeCoreApi::ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) 
{
    return 0;
}


///请求查询合约保证金率
int CCmeCoreApi::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID) 
{
    int ExchangeID=m_ContractName2ID[pQryInstrumentMarginRate->InstrumentID].ExchangeID;
    if(m_ReqCommissionMarginInfo.find(ExchangeID)==m_ReqCommissionMarginInfo.end())
    {
        m_ReqCommissionMarginInfo.insert(ExchangeID);
        m_pTradeDataWorker->ReqQryInstrumentCommissionRateMarginRate(ExchangeID,nRequestID);
    }
    return 0;
}


///请求查询合约手续费率
int CCmeCoreApi::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) 
{
    int ExchangeID=m_ContractName2ID[pQryInstrumentCommissionRate->InstrumentID].ExchangeID;
    if(m_ReqCommissionMarginInfo.find(ExchangeID)==m_ReqCommissionMarginInfo.end())
    {
        m_ReqCommissionMarginInfo.insert(ExchangeID);
        m_pTradeDataWorker->ReqQryInstrumentCommissionRateMarginRate(ExchangeID,nRequestID);
    }
    return 0;
}


///请求查询交易所
int CCmeCoreApi::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID) 
{
    return 0;
}


///请求查询合约
int CCmeCoreApi::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID) 
{
    return m_pTradeDataWorker->ReqQryInstrument(pQryInstrument,nRequestID);
}


///请求查询行情
int CCmeCoreApi::ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) 
{
    int ContractID=0;
    map<string,EPCID>::const_iterator iter;
    if(pQryDepthMarketData)
        iter=m_ContractName2ID.find(pQryDepthMarketData->InstrumentID);
    if(iter!=m_ContractName2ID.end())
        ContractID=iter->second.ContractID;
    return m_pTradeDataWorker->ReqQryDepthMarketData(ContractID,nRequestID);
}


///请求查询投资者结算结果
int CCmeCoreApi::ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID) 
{
    return m_pTradeDataWorker->ReqQrySettlementInfo(pQrySettlementInfo,nRequestID);
}


///请求查询转帐银行
int CCmeCoreApi::ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID) 
{
    return 0;
}


///请求查询投资者持仓明细
int CCmeCoreApi::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) 
{
    return m_pTradeDataWorker->ReqQryInvestorPositionDetail(pQryInvestorPositionDetail,nRequestID);
}


///请求查询客户通知
int CCmeCoreApi::ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID) 
{
    return 0;
}


///请求查询结算信息确认
int CCmeCoreApi::ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID) 
{
    return m_pTradeDataWorker->ReqQrySettlementInfoConfirm(pQrySettlementInfoConfirm,nRequestID);
}


///请求查询投资者持仓明细
int CCmeCoreApi::ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID) 
{
    return 0;
}


///请求查询保证金监管系统经纪公司资金账户密钥
int CCmeCoreApi::ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID) 
{
    return 0;
}


///请求查询仓单折抵信息
int CCmeCoreApi::ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID) 
{
    return 0;
}


///请求查询转帐流水
int CCmeCoreApi::ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID) 
{
    return 0;
}


///请求查询银期签约关系
int CCmeCoreApi::ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID) 
{
    return 0;
}


///请求查询签约银行
int CCmeCoreApi::ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID) 
{
    return 0;
}


///请求查询预埋单
int CCmeCoreApi::ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID) 
{
    return 0;
}


///请求查询预埋撤单
int CCmeCoreApi::ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID) 
{
    return 0;
}


///请求查询交易通知
int CCmeCoreApi::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID) 
{
    return 0;
}


///请求查询经纪公司交易参数
int CCmeCoreApi::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID) 
{
    return 0;
}


///请求查询经纪公司交易算法
int CCmeCoreApi::ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID) 
{
    return 0;
}


///期货发起银行资金转期货请求
int CCmeCoreApi::ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) 
{
    return 0;
}


///期货发起期货资金转银行请求
int CCmeCoreApi::ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) 
{
    return 0;
}


///期货发起查询银行余额请求
int CCmeCoreApi::ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) 
{
    return 0;
}
int CCmeCoreApi::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& Data)
{
    return 0;
}

///获取账户资金文本信息
int CCmeCoreApi::GetAccountText(std::string& outData,int language)
{
    return 0;
}
int CCmeCoreApi::GetAccountList(std::vector<string>& AcctList)
{
    return 0;
}
bool CCmeCoreApi::GetPositionInfo( const std::string& strAccount, 
                                  const std::string& strInstrumentID, PlatformStru_Position& field )
{
    return true;
}
void CCmeCoreApi::GetAllContractPosition(std::vector<PlatformStru_Position>& vecPosition)
{

}
void CCmeCoreApi::SetNewPwd( const std::string& strNewPwd )
{

}
int CCmeCoreApi::GetExchangeIDs(vector<string>& outData)
{
    return 0;
}
///获取账号信息
int CCmeCoreApi::GetTrader(std::vector<PlatformStru_Trader> &outData)
{
    return 0;
}
///获取交易所信息
int CCmeCoreApi::GetExchange(std::vector<PlatformStru_Exchange> &outData)
{
    return 0;
}
///获取合约信息
int CCmeCoreApi::GetContract(std::vector<PlatformStru_Contract> &outData)
{
    return 0;
}
///获取商品信息
int CCmeCoreApi::GetCommodity(std::vector<PlatformStru_Commodity> &outData)
{
    return 0;
}
///获取报单类型信息
int CCmeCoreApi::GetOrderType(std::vector<PlatformStru_OrderType>& outData)
{
    return 0;
}
// 获取报告
int CCmeCoreApi::GetReports(map<DWORD, Report>& mapReport)
{
    return 0;
}
bool CCmeCoreApi::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
    return false;
}
bool CCmeCoreApi::GetOpenPosition(PlatformStru_Contract InstrumentID,
                                  std::string account,PlatformStru_AccountPosition* posi)
{
    return false;
}
bool CCmeCoreApi::GetContractPosition(PlatformStru_Contract InstrumentID,
                                      std::string account,PlatformStru_AccountPosition* posi)
{
    return false;
}
// 获取登录状态原因
std::string CCmeCoreApi::GetLogonReason()
{
    return "";
}
void CCmeCoreApi::GetLinkStatus(int* pHost,int* pPrice)
{

}
double CCmeCoreApi::GetExchangeRate(char* currency)
{
    return 1.0;
}
//从tcp得到数据后，调用此回调函数进行处理
void CCmeCoreApi::trade_rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    ParseRcvData(pData,DataLen,true);
}
//断开tcp连接后，调用此回调函数进行处理
void CCmeCoreApi::trade_disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    m_TradeOverTimeCount=0;
    KillTimer(NULL,TRADE_TIMER_ID);
}
//开始调用connect()后，调用此函数通知应用层
void CCmeCoreApi::trade_startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
}
//连接成功后，调用此回调函数进行处理
void CCmeCoreApi::trade_connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    SetTimer(NULL, TRADE_TIMER_ID, ELAPSE_TIME, TradeTimerProc); 
    m_pTraderSpi->OnFrontConnected();
}
//连接失败后，调用此回调函数进行处理
void CCmeCoreApi::trade_connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
    m_pTraderSpi->OnFrontDisconnected(1);
}
void CCmeCoreApi::trade_ontime(void)
{
}
//从tcp得到数据后，调用此回调函数进行处理
void CCmeCoreApi::quot_rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    ParseRcvData(pData,DataLen,false);
}
//断开tcp连接后，调用此回调函数进行处理
void CCmeCoreApi::quot_disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{	
    m_QuotOverTimeCount=0;
    KillTimer(NULL,QUOTE_TIMER_ID);
}
//开始调用connect()后，调用此函数通知应用层
void CCmeCoreApi::quot_startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
}
//连接成功后，调用此回调函数进行处理
void CCmeCoreApi::quot_connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
    SetTimer(NULL, QUOTE_TIMER_ID, ELAPSE_TIME, QuotTimerProc); 
	m_pMdSpi->OnFrontConnected();
}
//连接失败后，调用此回调函数进行处理
void CCmeCoreApi::quot_connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
    m_pMdSpi->OnFrontDisconnected(1);
}
void CCmeCoreApi::quot_ontime(void)
{
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
}
////////////////////////////////////////////////////////////
//
//具体的业务逻辑处理
//
////////////////////////////////////////////////////////////////
void CCmeCoreApi::ParseRcvData(const char *pData,int DataLen,bool bTrader)
{
    CUniPkg datapkg;
    int LengthTakenup;
    if(datapkg.Parse(pData,DataLen,LengthTakenup))
    {
        Stru_UniPkgHead head=datapkg.GetPkgHead();
        switch(head.cmdid)
        {
        case CMDID_Client_HeartbeatRlt:
            if(bTrader)
                m_TradeOverTimeCount--;
            else
                m_QuotOverTimeCount--;
            break;
        case CMDID_Client_LoginRlt://用户登陆响应
            OnRspUserLogin(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_LogoutRlt://用户退出响应
        case CMDID_Client_ForceLogout:// 服务器端强制客户端登出
            OnRspUserLogout(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetIsSettltmentReportConfirmedRlt://查询结算信息是否确认
            OnRspQrySettlementInfoConfirm(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetSettltmentReportRlt://投资者结算结果
            OnRspQrySettlementInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ConfirmSettltmentReportRlt://服务器对客户端确认的返回
            OnRspSettlementInfoConfirm(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetCurrencyInfoRlt://货币信息
            OnRspGetCurrencyInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_CurrencyInfoUpdate://货币信息改变通知
            OnCurrencyInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetAllExchangeInfoRlt://请求查询合约信息步骤2.返回全部交易所信息
            OnRspGetAllExchangeInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ExchangeInfoUpdate://交易所信息更新
            OnExchangeInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ExchangeTradingModeUpdate:
            OnRtnExchangeStatus(datapkg.GetContent(),head.len,bTrader);
        case CMDID_Client_GetExchangeProductGroupInfoRlt://请求查询合约信息步骤4.返回交易所下产品组信息
            OnRspGetExchangeProductGroupInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ProductGroupInfoUpdate://产品组信息更新
            OnProductGroupInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetExchangeProductInfoRlt://请求查询合约信息步骤6.返回交易所下所有产品信息
            OnRspGetExchangeProductInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ProductInfoUpdate:
            OnProductInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ContractInfoUpdate:
            OnContractInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetInitialEquityInfoRlt://查询资金信息
            OnRspQryTradingAccount(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetDepositWithdrawInfoRlt://返回请求获取出入金信息
            OnRspGetDepositWithdrawInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_DepositWithdrawInfoUpdate://出入金信息改变通知
            OnDepositWithdrawInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetTradeInfoRlt://查询成交
            OnRspQryTrade(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_TradeArrived://成交改变通知
            OnRtnTrade(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetPositionInfoRlt://查询持仓明细
            OnRspQryInvestorPositionDetail(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_PositionInfoUpdate://持仓改变消息
            OnRtnPositionDetail(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetOrderInfoRlt://查询报单
            OnRspQryOrder(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_OrderCmdRlt://报单命令
            OnRspOrderCmd(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_OrderStatusUpdate://报单状态改变
            OnRspOrderStatusUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_SubscribeMarketDataRlt://订阅行情
            OnRspSubscribeMarketData(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_UnsubscribeMarketDataRlt: //退订行情  
            OnRspUnsubscribeMarketData(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetMarketDataSnapshotRlt://行情快照
            OnRspGetMarketDataSnapshot(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_NewMarketData://行情
            OnNewMarketData(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_ChangeLoginPWRlt://修改密码
            OnRspUserPasswordUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_GetExchangeCommissionMarginInfoRlt://获取指定交易所的保证金手续费费率
            OnRspGetExchangeCommissionMarginInfo(datapkg.GetContent(),head.len,bTrader);
            break;
        case CMDID_Client_CommissionMarginInfoUpdate:
            OnCommissionMarginInfoUpdate(datapkg.GetContent(),head.len,bTrader);
            break;
        default:
            break;
        }
    }
}
void CCmeCoreApi::OnRspUserLogin(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_LoginRlt))
    { 
        Stru_Client_LoginRlt& Rlt=*((Stru_Client_LoginRlt*)pData);
        CThostFtdcRspUserLoginField field={0};
        field.SessionID=Rlt.LoginSessionID;
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        if(bTrader)
        {
            m_pTraderSpi->OnRspUserLogin(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
        else
        {
            m_pMdSpi->OnRspUserLogin(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspUserLogout(const char *pData,int DataLen,bool bTrader)
{

    if(pData&&DataLen>=sizeof(Stru_Client_LogoutRlt))
    { 
        Stru_Client_LogoutRlt& Rlt=*((Stru_Client_LogoutRlt*)pData);
        CThostFtdcUserLogoutField field={0};
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        if(bTrader)
        {
            m_pTraderSpi->OnRspUserLogout(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
        else
        {
            m_pMdSpi->OnRspUserLogout(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspQrySettlementInfoConfirm(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetIsSettltmentReportConfirmedRlt))
    { 
        Stru_Client_GetIsSettltmentReportConfirmedRlt& Rlt=*((Stru_Client_GetIsSettltmentReportConfirmedRlt*)pData);
        CThostFtdcSettlementInfoConfirmField SettleInfo={0}; 
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQrySettlementInfoConfirm(Rlt.IsConfirmed!=0?(&SettleInfo):NULL, &pRspInfo,Rlt.RefRequestID,true);
    }

}
void CCmeCoreApi::OnRspQrySettlementInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetSettlementReportInfoRlt))
    { 
        Stru_Client_GetSettlementReportInfoRlt& Rlt=*((Stru_Client_GetSettlementReportInfoRlt*)pData);
        if(m_SettlementReportInfos.size()!=Rlt.InfoCount)   
            m_SettlementReportInfos.resize(Rlt.InfoCount); 
        m_SettlementReportInfos[Rlt.InfoOrd]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)
        {
            CThostFtdcSettlementInfoField SettleInfo={0};
            sprintf_s(SettleInfo.BrokerID,10,"%d",Rlt.Info.AccountID);
            SettleInfo.SettlementID=(int)Rlt.Info.SettlementReportID;
            string strConent;
            for(int i=0;i<(int)m_SettlementReportInfos.size();++i)
                strConent+=m_SettlementReportInfos[i].Content;
            strcpy_s(SettleInfo.Content,500,strConent.c_str());
            CThostFtdcRspInfoField pRspInfo={0};
            pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
            m_pTraderSpi->OnRspQrySettlementInfo(&SettleInfo,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspSettlementInfoConfirm(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ConfirmSettltmentReportRlt))
    {
        Stru_Client_ConfirmSettltmentReportRlt& Rlt=*((Stru_Client_ConfirmSettltmentReportRlt*)pData);
        CThostFtdcSettlementInfoConfirmField field={0};
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspSettlementInfoConfirm(&field,&pRspInfo,Rlt.RefRequestID,true);
    }
}
void CCmeCoreApi::OnRspGetCurrencyInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetCurrencyInfoRlt))
    {
        Stru_Client_GetCurrencyInfoRlt& Rlt=*((Stru_Client_GetCurrencyInfoRlt*)pData);
        m_CurrencyInfos[Rlt.Info.CurrencyID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)
        {
            m_pTradeDataWorker->GetAllExchangeInfo(Rlt.RefRequestID);//请求查询合约信息步骤1.获取全部交易所信息
        }
    }
}
void CCmeCoreApi::OnCurrencyInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_CurrencyInfoUpdate))
    {
        Stru_Client_CurrencyInfoUpdate& Rlt=*((Stru_Client_CurrencyInfoUpdate*)pData);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_CurrencyInfos[Rlt.Info.CurrencyID]=Rlt.Info;
            break;
        case UpdateFlag_remove:
            m_CurrencyInfos.erase(Rlt.Info.CurrencyID);
            break;
        }
    }

}
//交易所信息改变通知
void CCmeCoreApi::OnExchangeInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ExchangeInfoUpdate))
    {
        Stru_Client_ExchangeInfoUpdate& Rlt=*((Stru_Client_ExchangeInfoUpdate*)pData);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_ExchangeInfos[Rlt.Info.ExchangeID]=Rlt.Info;
            break;
        case UpdateFlag_remove:
            m_ExchangeInfos.erase(Rlt.Info.ExchangeID);
            break;
        }
    }
}
// 交易所交易状态改变通知
void CCmeCoreApi::OnRtnExchangeStatus(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ExchangeTradingModeUpdate))
    {
        Stru_Client_ExchangeTradingModeUpdate& Rlt=*((Stru_Client_ExchangeTradingModeUpdate*)pData);
        CThostFtdcInstrumentStatusField field={0};
        strcpy_s(field.ExchangeID,sizeof(field.ExchangeID)-1,m_ExchangeInfos[Rlt.ExchangeID].ExchangeCode);
        switch(Rlt.TradingMode){
            case TradingMode_NotTrading:    field.InstrumentStatus=THOST_FTDC_IS_NoTrading;break;
            case TradingMode_NomalTrading:  field.InstrumentStatus=THOST_FTDC_IS_Continous;break;
            case TradingMode_AggregateAuction:field.InstrumentStatus=THOST_FTDC_IS_AuctionOrdering;break;
            case TradingMode_AggregateMatching:field.InstrumentStatus=THOST_FTDC_IS_AuctionMatch;break;
            default:    field.InstrumentStatus=THOST_FTDC_IS_Continous;break;
        }
        m_pTraderSpi->OnRtnInstrumentStatus(&field);

    }
}
//请求查询合约信息步骤2.返回全部交易所信息
void CCmeCoreApi::OnRspGetAllExchangeInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetAllExchangeInfoRlt))
    { 
        Stru_Client_GetAllExchangeInfoRlt& Rlt=*((Stru_Client_GetAllExchangeInfoRlt*)pData);
        m_ExchangeInfos[Rlt.Info.ExchangeID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)//
        {
            //请求查询合约信息步骤3.获取交易所下产品组信息
            m_QueryExchangeInfoIterator=m_ExchangeInfos.begin();
            if(m_QueryExchangeInfoIterator!=m_ExchangeInfos.end())
            {
                m_pTradeDataWorker->GetExchangeProductGroupInfo(m_QueryExchangeInfoIterator->second.ExchangeID,Rlt.RefRequestID);
                m_QueryExchangeInfoIterator++;
            }
            else
                OnRspQryInstrument(Rlt.RefRequestID);    
        }
    }
}
//请求查询合约信息步骤4.返回交易所下产品组信息
void CCmeCoreApi::OnRspGetExchangeProductGroupInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetExchangeProductGroupInfoRlt))
    {
        Stru_Client_GetExchangeProductGroupInfoRlt& Rlt=*((Stru_Client_GetExchangeProductGroupInfoRlt*)pData);
        m_ProductGroupInfos[Rlt.ExchangeID][Rlt.Info.ProductGroupID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)//获取了产品信息
        {
            //请求查询合约信息步骤5.获取交易所下所有产品信息
            m_pTradeDataWorker->GetExchangeProductInfo(Rlt.Info.ExchangeID,Rlt.RefRequestID);
        }
    }
}
void CCmeCoreApi::OnProductGroupInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ProductGroupInfoUpdate))
    {
        Stru_Client_ProductGroupInfoUpdate& Rlt=*((Stru_Client_ProductGroupInfoUpdate*)pData);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_ProductGroupInfos[Rlt.Info.ExchangeID][Rlt.Info.ProductGroupID]=Rlt.Info;
            break;
        case UpdateFlag_remove:
            m_ProductGroupInfos[Rlt.Info.ExchangeID].erase(Rlt.Info.ProductGroupID);
            break;
        }
    }
}
//请求查询合约信息步骤6.返回交易所下所有产品信息
void CCmeCoreApi::OnRspGetExchangeProductInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetExchangeProductInfoRlt))
    {
        Stru_Client_GetExchangeProductInfoRlt& Rlt=*((Stru_Client_GetExchangeProductInfoRlt*)pData);
        m_ProductInfos[Rlt.ExchangeID][Rlt.Info.ProductID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)//获取了合约信息
        {
            //请求查询合约信息步骤7.获取交易所下所有合约信息
            m_pTradeDataWorker->GetExchangeContractInfo(Rlt.Info.ExchangeID,Rlt.RefRequestID);
        }
    }
}
void CCmeCoreApi::OnProductInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ProductInfoUpdate))
    {
        Stru_Client_ProductInfoUpdate& Rlt=*((Stru_Client_ProductInfoUpdate*)pData);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_ProductInfos[Rlt.Info.ExchangeID][Rlt.Info.ProductID]=Rlt.Info;
            break;
        case UpdateFlag_remove:
            m_ProductInfos[Rlt.Info.ExchangeID].erase(Rlt.Info.ProductID);
            break;
        }
    }
}
void CCmeCoreApi::OnContractInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ContractInfoUpdate))
    {
        Stru_Client_ContractInfoUpdate& Rlt=*((Stru_Client_ContractInfoUpdate*)pData);
        //获得ExchangeID
        int ExchangeID=0;
        map<int,map<int,Stru_Base_ProductInfo>>::const_iterator iter;
        map<int,Stru_Base_ProductInfo>::const_iterator inner_iter;
        for(iter=m_ProductInfos.begin();iter!=m_ProductInfos.end();++iter)
        {
            for(inner_iter=iter->second.begin();inner_iter!=iter->second.end();++inner_iter)
            {
                if(inner_iter->first==Rlt.Info.ProductID)
                {
                    ExchangeID=inner_iter->second.ExchangeID;
                    break;
                }
            }
        }
        if(ExchangeID==0) return;
        InstrumentIDType	InstrumentID={0};
        MakeGUIContractID(m_ExchangeInfos[ExchangeID],m_ProductInfos[ExchangeID][Rlt.Info.ProductID],
            Rlt.Info,InstrumentID);
        switch(Rlt.UpdateFlag)
        {
        case UpdateFlag_new:
        case UpdateFlag_modify:
            m_ContractInfos[ExchangeID][Rlt.Info.ContractID]=Rlt.Info;
            m_ID2ContractName[Rlt.Info.ContractID]=InstrumentID;
            m_ContractName2ID[InstrumentID]=EPCID(ExchangeID,Rlt.Info.ProductID,Rlt.Info.ContractID);
            break;
        case UpdateFlag_remove:
            m_ContractInfos[ExchangeID].erase(Rlt.Info.ContractID);
            m_ID2ContractName.erase(Rlt.Info.ContractID);
            m_ContractName2ID.erase(InstrumentID);
            break;
        }
    }
}
//请求查询合约信息步骤8.返回交易所下所有合约信息
void CCmeCoreApi::GetExchangeContractInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetExchangeContractInfoRlt))
    {
        Stru_Client_GetExchangeContractInfoRlt& Rlt=*((Stru_Client_GetExchangeContractInfoRlt*)pData);
        m_ContractInfos[Rlt.ExchangeID][Rlt.Info.ContractID]=Rlt.Info;
        if(Rlt.InfoOrd==Rlt.InfoCount-1)//获取了合约信息
        {
            //请求查询合约信息步骤9.完成一个交易所的查询,查询下一个交易所信息
            if(m_QueryExchangeInfoIterator!=m_ExchangeInfos.end())
            {
                m_pTradeDataWorker->GetExchangeProductGroupInfo(m_QueryExchangeInfoIterator->second.ExchangeID,Rlt.RefRequestID);
                m_QueryExchangeInfoIterator++;
            }
            else
                OnRspQryInstrument(Rlt.RefRequestID);    
        }
    }
}
//请求查询合约信息步骤10.全部完成
void CCmeCoreApi::OnRspQryInstrument(int RequestID)
{
    map<int,map<int,Stru_Base_ContractInfo>> ::const_iterator iterA;
    map<int,Stru_Base_ContractInfo>::const_iterator           iterB; 
    //获取合约总数
    int ContractCount=0,ContractIndex=0;
    for(iterA=m_ContractInfos.begin();iterA!=m_ContractInfos.end();++iterA)
    {
        ContractCount+=iterA->second.size();
    }
    for(iterA=m_ContractInfos.begin();iterA!=m_ContractInfos.end();++iterA)
    {
        const map<int,Stru_Base_ContractInfo>& contractmap=iterA->second;
        for(iterB=contractmap.begin();iterB!=contractmap.end();++iterB)
        {
            PlatformStru_InstrumentInfo Instrument;
            CThostFtdcRspInfoField pRspInfo={0};
            FillInstrumentInfo(iterA->first,iterB->second,Instrument);
            m_pTraderSpi->OnRspQryInstrument(&Instrument,&pRspInfo,RequestID,(++ContractIndex)==ContractCount);
        }
    }
}
void CCmeCoreApi::MakeGUIContractID(const Stru_Base_ExchangeInfo& ExchangeInfo,
                                    const Stru_Base_ProductInfo& ProductInfo,
                                    const Stru_Base_ContractInfo& ContractInfo,char* strContract)
{
    string str=string(ProductInfo.ProductName)+ContractInfo.ContractDate+'@'+ExchangeInfo.ExchangeCode;
    strcpy_s(strContract,sizeof(strContract)-1,str.c_str());
    m_ContractName2ID[strContract]=EPCID(ExchangeInfo.ExchangeID,ProductInfo.ProductID,ContractInfo.ContractID);
    m_ID2ContractName[ContractInfo.ContractID]=strContract;
}
void CCmeCoreApi::OnRspQryTradingAccount(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetInitialEquityInfoRlt))
    {
        Stru_Client_GetInitialEquityInfoRlt& Rlt=*((Stru_Client_GetInitialEquityInfoRlt*)pData);
        if(Rlt.Rlt==1)
        {
            FillTradingAccount(Rlt.Info,m_TradingAccount);
            m_pTradeDataWorker->GetDepositWithdrawInfo(Rlt.RefRequestID);
        }
    }
}
void CCmeCoreApi::OnRspGetDepositWithdrawInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetDepositWithdrawDynaRlt))
    {
        Stru_Client_GetDepositWithdrawDynaRlt& Rlt=*((Stru_Client_GetDepositWithdrawDynaRlt*)pData);
        FillTradingAccount(Rlt.Info,m_TradingAccount);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0; 
        m_pTraderSpi->OnRspQryTradingAccount(&m_TradingAccount,&pRspInfo,Rlt.RefRequestID,Rlt.InfoOrd==Rlt.InfoCount-1);
    }
}
void CCmeCoreApi::OnDepositWithdrawInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_DepositWithdrawDynaUpdate))
    {
        Stru_Client_DepositWithdrawDynaUpdate& Rlt=*((Stru_Client_DepositWithdrawDynaUpdate*)pData);
        FillTradingAccount(Rlt,m_TradingAccount);
        CThostFtdcRspInfoField pRspInfo={0};
        m_pTraderSpi->OnRspQryTradingAccount(&m_TradingAccount,&pRspInfo,0,true);
    }
}
void CCmeCoreApi::OnRspQryTrade(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetTradeInfoRlt))
    {
        Stru_Client_GetTradeInfoRlt& Rlt=*((Stru_Client_GetTradeInfoRlt*)pData);
        PlatformStru_TradeInfo Trade={0};
        FillTradeInfo(Rlt.Info,Trade);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQryTrade(&Trade,&pRspInfo,Rlt.RefRequestID,Rlt.InfoOrd==Rlt.InfoCount-1);
    }
}
void CCmeCoreApi::OnRtnTrade(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_TradeArrived))
    {
        Stru_Client_TradeArrived& Rlt=*((Stru_Client_TradeArrived*)pData);
        PlatformStru_TradeInfo Trade={0};
        FillTradeInfo(Rlt.Info,Trade);
        m_pTraderSpi->OnRtnTrade(&Trade);
    }
}
//持仓改变通知，暂时没有使用
void CCmeCoreApi::OnRtnPositionDetail(const char *pData,int DataLen,bool bTrader)
{

}
void CCmeCoreApi::OnRspQryInvestorPositionDetail(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetPositionInfoRlt))
    {
        Stru_Client_GetPositionInfoRlt& Rlt=*((Stru_Client_GetPositionInfoRlt*)pData);
        PlatformStru_Position  InvestorPositionDetail;
        FillPositionInfo(Rlt.Info,InvestorPositionDetail);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQryInvestorPositionDetail(&InvestorPositionDetail,&pRspInfo,Rlt.RefRequestID,Rlt.InfoOrd==Rlt.InfoCount-1);
    }
}
void CCmeCoreApi::OnRspQryOrder(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetOrderInfoRlt))
    {
        Stru_Client_GetOrderInfoRlt& Rlt=*((Stru_Client_GetOrderInfoRlt*)pData);
        PlatformStru_OrderInfo OrderInfo={0};
        FillOrderInfo(Rlt.Info,OrderInfo);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQryOrder(&OrderInfo,&pRspInfo,Rlt.RefRequestID,Rlt.InfoOrd==Rlt.InfoCount-1);
    }
}
void CCmeCoreApi::OnRspOrderCmd(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_OrderCmdRlt))
    {
        Stru_Client_OrderCmdRlt& Rlt=*((Stru_Client_OrderCmdRlt*)pData);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        map<int,EnumOrderCmdFlag>::const_iterator iter=m_RequestID2OrderCmdFlag.find(Rlt.RefRequestID);
        switch(iter->second)
        {
        case OrderCmdFlag_New:
            {
                CThostFtdcInputOrderField field={0};
                FillOrderCmdInfo(iter->second,Rlt.Info,field);
                m_pTraderSpi->OnRspOrderInsert(&field,&pRspInfo,Rlt.RefRequestID,true);
            }
            break;
        case OrderCmdFlag_Amend:
        case OrderCmdFlag_Cancel:
            {
                CThostFtdcInputOrderActionField field={0};
                FillOrderCmdInfo(iter->second,Rlt.Info,field);
                m_pTraderSpi->OnRspOrderAction(&field,&pRspInfo,Rlt.RefRequestID,true);
            }
            break;
        case OrderCmdFlag_New_Park:
            {
                CThostFtdcParkedOrderField field={0};
                FillOrderCmdInfo(iter->second,Rlt.Info,field);
                m_pTraderSpi->OnRspParkedOrderInsert(&field,&pRspInfo,Rlt.RefRequestID,true);
            }
            break;
        case OrderCmdFlag_Amend_Park:
        case OrderCmdFlag_Cancel_Park:
            {
                CThostFtdcParkedOrderActionField field={0};
                FillOrderCmdInfo(iter->second,Rlt.Info,field);
                m_pTraderSpi->OnRspParkedOrderAction(&field,&pRspInfo,Rlt.RefRequestID,true);
            }
            break;
        }
    }
}
void CCmeCoreApi::OnRspOrderStatusUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_OrderStatusUpdate))
    {
        Stru_Client_OrderStatusUpdate& Rlt=*((Stru_Client_OrderStatusUpdate*)pData);
        PlatformStru_OrderInfo info;
        FillRtnOrder(Rlt.Info,info);
        m_pTraderSpi->OnRtnOrder(&info);
    }
}
void CCmeCoreApi::OnRspSubscribeMarketData(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_SubscribeMarketDataRlt))
    {
        Stru_Client_SubscribeMarketDataRlt& Rlt=*((Stru_Client_SubscribeMarketDataRlt*)pData);
        map<int,string>::const_iterator iter=m_SubscribeUnsubscribeMarketMap.find(Rlt.RefRequestID);
        if(iter!=m_SubscribeUnsubscribeMarketMap.end())
        {
            CThostFtdcSpecificInstrumentField field={0};
            strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,iter->second.c_str());
            CThostFtdcRspInfoField pRspInfo={0};
            pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
            m_pMdSpi->OnRspSubMarketData(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspUnsubscribeMarketData(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_UnsubscribeMarketDataRlt))
    {
        Stru_Client_UnsubscribeMarketDataRlt& Rlt=*((Stru_Client_UnsubscribeMarketDataRlt*)pData);
        map<int,string>::const_iterator iter=m_SubscribeUnsubscribeMarketMap.find(Rlt.RefRequestID);
        if(iter!=m_SubscribeUnsubscribeMarketMap.end())
        {
            CThostFtdcSpecificInstrumentField field={0};
            strcpy_s(field.InstrumentID,sizeof(field.InstrumentID)-1,iter->second.c_str());
            CThostFtdcRspInfoField pRspInfo={0};
            pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
            m_pMdSpi->OnRspUnSubMarketData(&field,&pRspInfo,Rlt.RefRequestID,true);
        }
    }
}
void CCmeCoreApi::OnRspGetMarketDataSnapshot(const char *pData,int DataLen,bool bTrader)
{
    //只实现一次请求一个合约
    if(pData&&DataLen>=sizeof(Stru_Client_GetMarketDataSnapshotRlt))
    {
        Stru_Client_GetMarketDataSnapshotRlt& Rlt=*((Stru_Client_GetMarketDataSnapshotRlt*)pData);
        CThostFtdcDepthMarketDataField field={0};
        FillDepthMarketData(Rlt.Info,field);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspQryDepthMarketData(&field,&pRspInfo,Rlt.RefRequestID,true);
    }
}
void CCmeCoreApi::OnNewMarketData(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_NewMarketData))
    {
        Stru_Client_NewMarketData& Rlt=*((Stru_Client_NewMarketData*)pData);
        PlatformStru_DepthMarketData field={0};
        FillDepthMarketData(Rlt,field);
        m_pMdSpi->OnRtnDepthMarketData(&field);
    }
}
void CCmeCoreApi::OnRspUserPasswordUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_ChangeLoginPWRlt))
    {
        Stru_Client_ChangeLoginPWRlt& Rlt=*((Stru_Client_ChangeLoginPWRlt*)pData);
        CThostFtdcUserPasswordUpdateField  UserPasswordUpdate;
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0;
        m_pTraderSpi->OnRspUserPasswordUpdate(&UserPasswordUpdate,&pRspInfo,Rlt.RefRequestID,true);
    }
}
void CCmeCoreApi::SendHeartbeat(bool bTrader)
{
    if(bTrader)
    {
        if(m_TradeOverTimeCount>=OVERTIME_LIMIT)
        {
            Stru_IPAddress LocalAddr,SvrAddr;
            m_pTradeDataWorker->GetAddr(LocalAddr,SvrAddr);
            trade_disconn(LocalAddr,SvrAddr,INVALID_SOCKET);
        }
        else
        {
            m_TradeOverTimeCount++;
            m_pTradeDataWorker->SendHeartbeat();
        }
    }
    else
    {
        if(m_QuotOverTimeCount>=OVERTIME_LIMIT)
        {
            Stru_IPAddress LocalAddr,SvrAddr;
            m_pQuotDataWorker->GetAddr(LocalAddr,SvrAddr);
            quot_disconn(LocalAddr,SvrAddr,INVALID_SOCKET);            
        }
        else
        {
            m_QuotOverTimeCount++;
            m_pQuotDataWorker->SendHeartbeat();
        }
    }
}
void CCmeCoreApi::OnRspGetExchangeCommissionMarginInfo(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_GetExchangeCommissionMarginInfoRlt))
    {
        Stru_Client_GetExchangeCommissionMarginInfoRlt& Rlt=*((Stru_Client_GetExchangeCommissionMarginInfoRlt*)pData);
        PlatformStru_InstrumentMarginRate  mr={0};
        PlatformStru_InstrumentCommissionRate cr={0};
        FillCommissionMarginInfo(Rlt.CommissionInfo,cr);
        FillCommissionMarginInfo(Rlt.MarginInfo,mr);
        CThostFtdcRspInfoField pRspInfo={0};
        pRspInfo.ErrorID=Rlt.Rlt==0?Rlt.ErrNo:0; 
        m_pTraderSpi->OnRspQryInstrumentMarginRate(&mr,&pRspInfo,Rlt.RefRequestID,true) ;
        m_pTraderSpi->OnRspQryInstrumentCommissionRate(&cr,&pRspInfo,Rlt.RefRequestID,true) ;
    }
}
void CCmeCoreApi::OnCommissionMarginInfoUpdate(const char *pData,int DataLen,bool bTrader)
{
    if(pData&&DataLen>=sizeof(Stru_Client_CommissionMarginInfoUpdate))
    {
        Stru_Client_CommissionMarginInfoUpdate& Rlt=*((Stru_Client_CommissionMarginInfoUpdate*)pData);
        PlatformStru_InstrumentMarginRate  mr={0};
        PlatformStru_InstrumentCommissionRate cr={0};
        FillCommissionMarginInfo(Rlt.CommissionInfo,cr);
        FillCommissionMarginInfo(Rlt.MarginInfo,mr);
        CThostFtdcRspInfoField pRspInfo={0};
        m_pTraderSpi->OnRspQryInstrumentMarginRate(&mr,&pRspInfo,0,true) ;
        m_pTraderSpi->OnRspQryInstrumentCommissionRate(&cr,&pRspInfo,0,true) ;
    }
}