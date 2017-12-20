#include "stdafx.h"

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/GLobalFunc.h"
#define CHECK_LOGIN_STATUS()    \
	if(!m_isLogin)  return -999
const static string __UNKOWN__ = "未知";
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CServiceProxy", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)

///启动平台
int CServiceProxy::Start()
{
    m_pWriteLog=new zqWriteLog();
	m_PlatFormService=new CPlatFormService(*this);
	m_pQryQueue->SetPlatFormService(m_PlatFormService);
    return 0;
}

///关闭平台
int CServiceProxy::Stop()
{
	int ret=Logout();
	SAFE_DELETE(m_PlatFormService);
	m_pQryQueue->SetPlatFormService(m_PlatFormService);
    SAFE_DELETE(m_pQryQueue);
    return ret;
}
int CServiceProxy::StopThread()
{
	m_PlatFormService->StopThreads();
	m_pQryQueue->StopThread();
    
    return 0;
}
///密码是否是当前密码
///1：是，其他：不是
int CServiceProxy::IsValidPW(const std::string& yourPW)
{
	return m_PlatFormService->IsValidPW(yourPW);
}
int CServiceProxy::GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts)
{
	return m_PlatFormService->GetBrokerIDUserID(UserID,BrokerID,accounts);
}
string CServiceProxy::GetAccount()
{
	std::string UserID,BrokerID;
	GetBrokerIDUserID(UserID,BrokerID);
	return UserID;
}
///设置交易前置机地址
int CServiceProxy::SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)
{
    return m_PlatFormService->SetTradeFrontAddrs(TradeAddr);
}

///设置行情前置机地址
int CServiceProxy::SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)
{
    return m_PlatFormService->SetQuotFrontAddrs(QuotAddr);
}

///设置经纪公司参数信息
int CServiceProxy::SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)
{
    return m_PlatFormService->SetParticipantInfo(BrokerID, UserID, UserPW, OneTimePassword);
}
//设置代理服务器字符串
int CServiceProxy::SetProxyConnectStr(const std::string& str)
{
	return m_PlatFormService->SetProxyConnectStr(str);
}


///登入
int CServiceProxy::Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPWD)
{
    if(m_isLogin)
        return 0;
    
    int ret=m_PlatFormService->LoginTrade(InvestorID, InvestorPW, InvestorNewPWD);
    if(ret<0)
        return ret;

    ret=m_PlatFormService->LoginQuot(InvestorID, InvestorPW);
    if(ret<0)
    {
        m_PlatFormService->LogoutTrade();
        return ret;
    }

    m_isLogin=true; 
    return ret;
}

///登出
int CServiceProxy::Logout()
{
    m_PlatFormService->LogoutTrade();
    m_PlatFormService->LogoutQuot();
    m_isLogin = false;
    return 0;
}

///定制业务数据
int CServiceProxy::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
    return m_PlatFormService->SubscribeBusinessData(BID, GID, callback, sendCurrentInfo);
}

///取消业务数据的定制
int CServiceProxy::UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)
{
    return m_PlatFormService->UnSubscribeBusinessData(BID, GID);
}

///获取底层服务的当前状态
ServiceStatus CServiceProxy::GetCurrentStatus(int* pQuotStatus,int* pTradeStatus)
{
    if(!m_isLogin)
        return SS_NOLOGIN;
    
    CPlatFormParam& RefPlatformParam=m_PlatFormService->m_PlatformParam;
    if(pQuotStatus) *pQuotStatus=RefPlatformParam.QuotStatus;
    if(pTradeStatus) *pTradeStatus=RefPlatformParam.TradeStatus;
    if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Connecting ||
       RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Logining   ||
       RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirming /*  ||
       RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Connecting ||
       RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Logining*/)
        return SS_LOGINING;
    else if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_LoginFailure /*||*/
            /*RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirmFailure ||*/
            /*RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_LoginFailure*/)
        return SS_LOGINFAILED;
    else if(RefPlatformParam.TradeStatus==CPlatFormParam::CONNSTATUS_Connected /*&&
       RefPlatformParam.QuotStatus ==CPlatFormParam::CONNSTATUS_Connected*/)
        return SS_LOGINED;
    else
        return SS_LOSTCONN;

}
///获取合约列表
int CServiceProxy::GetInstrumentList(std::vector<GroupInfo> &outData)
{
	int ret=m_PlatFormService->GetInstrumentList(outData);
	for(int i=0;i<(int)outData.size();i++)
	{
		outData[i].GroupName=ExchangeID2String(outData[i].GroupName);
	}
	return ret;
}
///获取合约列表
int CServiceProxy::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    return m_PlatFormService->GetInstrumentList(outData,ExchangeID);
}
//添加主力合约列表
int CServiceProxy::AddMainInstrumentList(std::string instrument)
{
	return m_PlatFormService->AddMainInstrumentList(instrument);
}
///获取主力合约列表
int CServiceProxy::GetMainInstrumentList(std::set<std::string> &outData)
{
    return m_PlatFormService->GetMainInstrumentList(outData);
}
///设置合约容差列表
void CServiceProxy::SetInstrumentVarietyMap(map<string, string>& inData)
{
    m_PlatFormService->SetInstrumentVarietyMap(inData);
}

// 获取合约容差列表
int CServiceProxy::GetInstrumentVarietyMap(map<string, string>& outData)
{
    return m_PlatFormService->GetInstrumentVarietyMap(outData);
}

///获取指定合约信息
int CServiceProxy::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    return m_PlatFormService->GetInstrumentInfo(InstrumentID, outData);
}
    //设置合约订阅状态
void CServiceProxy::SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper)
{
   m_PlatFormService->SetSubscribeStatus(InstrumentID,status,oper);
}
///设置指定合约信息
void CServiceProxy::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    m_PlatFormService->SetInstrumentInfo(InstrumentID, outData);
}
///获取合约的产品类型，失败返回-1
char CServiceProxy::GetProductClassType(const std::string& InstrumentID)
{
    return m_PlatFormService->GetProductClassType(InstrumentID);
}

///获取合约品种代码
int CServiceProxy::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    return m_PlatFormService->GetProductID(outData,ExchangeID);
}

///获取合约手续费率
int CServiceProxy::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_PlatFormService->GetCommissionRate(InstrumentID, outData);
}

///获取合约手续费率，如果没有，则触发查询
int CServiceProxy::GetCommissionRate_TriggerReq(const std::string& InstrumentID)
{
    PlatformStru_InstrumentCommissionRate RateData;
    int ret;

    ret=m_PlatFormService->GetCommissionRate(InstrumentID, RateData);

    if(ret==0) return 0;
    else 
    {                                                       //如果当前没有该费率，则放一条到查询队列中
        CThostFtdcQryInstrumentCommissionRateField ReqData;
        memset(&ReqData,0,sizeof(ReqData));
        strncpy(ReqData.BrokerID,m_PlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
        strncpy(ReqData.InvestorID,m_PlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;
        strncpy(ReqData.InstrumentID,InstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);

        if(m_pQryQueue)
            m_pQryQueue->ReqQryInstrumentCommissionRate(ReqData,0);
        return ret;
    }
}

///获取合约保证金率
int CServiceProxy::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_PlatFormService->GetMarginRate(InstrumentID, outData);
}

///获取合约保证金率，如果没有，则触发查询
int CServiceProxy::GetMarginRate_TriggerReq(const std::string& InstrumentID)
{
    PlatformStru_InstrumentMarginRate RateData;
    int ret;
    
    if(InstrumentID.empty())
        return -1;
    ret=m_PlatFormService->GetMarginRate(InstrumentID, RateData);

    if(ret==0) return 0;
    else 
    {                                                       //如果当前没有该费率，则放一条到查询队列中
        CThostFtdcQryInstrumentMarginRateField ReqData;
        memset(&ReqData,0,sizeof(ReqData));
        strncpy(ReqData.BrokerID,m_PlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
        strncpy(ReqData.InvestorID,m_PlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
        strncpy(ReqData.InstrumentID,InstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);
        ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

        if(m_pQryQueue)
            m_pQryQueue->ReqQryInstrumentMarginRate(ReqData,0);
        return ret;
    }
}

///设置合约手续费率
int CServiceProxy::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_PlatFormService->SetCommissionRate(InstrumentID, outData);
}

///设置合约保证金率
int CServiceProxy::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_PlatFormService->SetMarginRate(InstrumentID, outData);
}

///获取合约乘数，成功返回合约乘数，失败返回-1
int CServiceProxy::GetInstrumentMultiple(const std::string& InstrumentID)
{
    return m_PlatFormService->GetInstrumentMultiple(InstrumentID);
}
///获取指定品种的合约列表,返回合约个数
int CServiceProxy::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
	return m_PlatFormService->GetInstrumentListByProductID(ProductID,outData);
}
///获取指定合约行情
int CServiceProxy::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_PlatFormService->GetQuotInfo(InstrumentID, outData);
}

///获取指定合约旧行情
int CServiceProxy::GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_PlatFormService->GetOldQuotInfo(InstrumentID, outData);
}

///获取指定合约的现价，失败返回0.0
double CServiceProxy::GetCurPrice(const std::string& InstrumentID)
{
    return m_PlatFormService->GetCurPrice(InstrumentID);
}


int CServiceProxy::GetPositions2(const std::string& strInstrument,
                                 std::set<long>& setFTID,
                                 std::vector<PlatformStru_Position>& outData,
                                 long& lastseq)
{
    return m_PlatFormService->GetPositions2(strInstrument,setFTID,outData,lastseq);
}

int CServiceProxy::GetPositions3(const std::string& strInstrument,
                                 std::vector<PlatformStru_Position>& outData,
                                 long& lastseq)
{
    return m_PlatFormService->GetPositions3(strInstrument,outData,lastseq);
}

int CServiceProxy::GetPositionDetails3(const std::string& strInstrument,
                                          std::set<long>& setFTID,
                                          std::vector<PlatformStru_PositionDetail>& vecValue,
                                          long& lastseq)
{
    return m_PlatFormService->GetPositionDetails3(strInstrument,setFTID,vecValue,lastseq);
}

int CServiceProxy::GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData)
{
    return m_PlatFormService->GetPositionCombs2(strTradeID, strInstrument,outData);
}

int CServiceProxy::GetPositions(std::vector<PlatformStru_Position>& outData)
{
    return m_PlatFormService->GetPositions(outData);
}

int CServiceProxy::GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    return m_PlatFormService->GetPositionDetails(outData,lastseq);
}

int CServiceProxy::GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData)
{
    return m_PlatFormService->GetPositionCombs(outData);
}


bool CServiceProxy::HavePositionDetail(const std::string& strInstrumentID)
{
    return m_PlatFormService->HavePositionDetail(strInstrumentID);
}

//获取持仓记录键值列表,返回持仓记录的数量
int CServiceProxy::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_PlatFormService->GetPositionKeySet(PositionKeySet);
}

//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
int CServiceProxy::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    return m_PlatFormService->GetPositionData(strAccount,InstrumentID,Direction,HedgeFlag,PositionData);
}



int  CServiceProxy::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
	return m_PlatFormService->GetWaitOrderVolume(strAccount, strInstrumentID, Direction, CloseType,HedgeFlag);
}

//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
int CServiceProxy::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_PlatFormService->GetCloseOrderVolume(strAccount,strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);
}


///获取交易所时间
string CServiceProxy::GetTradingDay(const string& ExchangeID)
{
    return m_PlatFormService->GetTradingDay(ExchangeID);
}

bool CServiceProxy::IsTodayPosition( const char* pOpenData )
{
	return m_PlatFormService->IsTodayPosition(pOpenData);
}

///获取交易所时间
unsigned long CServiceProxy::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_PlatFormService->GetExchangeTime(ExchangeID,time);
}
///获取全部交易所
int CServiceProxy::GetExchangeIDs(vector<string>& outData)
{
    return m_PlatFormService->GetExchangeIDs(outData);
}
int CServiceProxy::GetTradingAccountAvailable(double& fAvailable)
{
	CHECK_LOGIN_STATUS();
    return m_PlatFormService->GetTradingAccountAvailable(fAvailable);
}
int CServiceProxy::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    return m_PlatFormService->GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CServiceProxy::GetTradingAccountID(char* AccountID)
{
    return m_PlatFormService->GetTradingAccountID(AccountID);
}
///获取交易资金账户信息
int CServiceProxy::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)
{
    return m_PlatFormService->GetTradingAccount(strAccount,outData);
}
///获取账户资金文本信息
int CServiceProxy::GetAccountText(std::string& outData,int language)
{
    return m_PlatFormService->GetAccountText(outData,language);
}
///设置交易资金账户信息
void CServiceProxy::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    m_PlatFormService->SetTradingAccount(inData);
}

///请求查询投资者结算结果响应
int CServiceProxy::GetLastSettlementInfo(std::string& outData)
{
    return m_PlatFormService->GetLastSettlementInfo(outData);
}

///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
void CServiceProxy::SetCurrentPositionContent(int PositionContentMode)
{
    return m_PlatFormService->SetCurrentPositionContent(PositionContentMode);
}

///获取全部报单
int CServiceProxy::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetAllOrders(outData);
}
///获取全部报单
int CServiceProxy::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetAllOrders2(strInstrument,outData);
}

int CServiceProxy::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTriggerOrders(outData);
}

///获取合约相关的已触发的报单
int CServiceProxy::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTriggerOrders2(strInstrument,outData);
}

///获取指定已触发的报单
bool CServiceProxy::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetTriggerOrder(orderkey,outData);
}

///获取已成交报单
int CServiceProxy::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTradedOrders(outData);
}

///获取已成交报单
int CServiceProxy::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetTradedOrders2(strInstrument,outData);
}

///获取指定的已成交报单
bool CServiceProxy::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetTradedOrder(orderkey,outData);
}

///获取已撤单和错误报单
int CServiceProxy::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetCanceledOrders(outData);
}

///获取已撤单和错误报单
int CServiceProxy::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetCanceledOrders2(strInstrument,outData);
}

///获取指定的已撤单和错误报单
bool CServiceProxy::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetCanceledOrder(orderkey,outData);
}

///获取未成交报单
int CServiceProxy::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetWaitOrders(outData);
}

///获取未成交报单
int CServiceProxy::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_PlatFormService->GetWaitOrders2(strInstrument,outData);
}

///获取指定的未成交报单
bool CServiceProxy::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetWaitOrder(orderkey,outData);
}

///获取指定报单
bool CServiceProxy::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    return m_PlatFormService->GetOrder(inOrderKey,outData);
}


///获取全部成交单
int CServiceProxy::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_PlatFormService->GetAllTrades(outData);
}
///获取指定成交信息
bool CServiceProxy::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    return m_PlatFormService->GetTradeInfo(tradekey,outData);
}
///获取指定成交信息
int CServiceProxy::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_PlatFormService->GetTradesOfInstrument(strInstrument,outData);
}


///获取全部成交统计记录
int CServiceProxy::GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_PlatFormService->GetAllTradeTotals(outData);
}

///获取指定合约的成交统计记录成功返回0，失败返回-1
int CServiceProxy::GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_PlatFormService->GetTradeTotalOfInstrument(strInstrument,outData);
}

///获取全部成交统计数据
int CServiceProxy::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return m_PlatFormService->GetAllTradeTotalDatas(outData);
}
///在成交统计查找参数rawData
int CServiceProxy::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return m_PlatFormService->FindIndexFromAllTradeTotalDatas(rawData);
}

bool CServiceProxy::IsSettlementInfoConfirmed()
{
	return m_PlatFormService->IsSettlementInfoConfirmed();
}
//获取客户签约银行信息
int CServiceProxy::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{
    return m_PlatFormService->GetContractBank(outData);
}
//查询队列是否为空
bool CServiceProxy::IsQryQueueEmpty(void)
{
	if(!m_isLogin)  return false;

	if(m_pQryQueue)
		return m_pQryQueue->IsCacheEmpty();

	return false;
}


void CServiceProxy::SetModuleDataQry(bool bQry)
{
    m_PlatFormService->SetModuleDataQry(bQry);
}
bool CServiceProxy::GetModuleDataQry()
{
    return m_PlatFormService->GetModuleDataQry();
}
void CServiceProxy::ReConnetServer()
{
	Logout();
	CPlatFormParam& RefPlatformParam=m_PlatFormService->m_PlatformParam;
	Login(RefPlatformParam.InvestorID,RefPlatformParam.InvestorPW);
}
CServiceProxy::~CServiceProxy()
{
    if(m_pQryQueue)
    {
        delete m_pQryQueue;
        m_pQryQueue=NULL;
    }
    LOG_INFO("CServiceProxy::~CServiceProxy(析构CServiceProxy对象)");
    SAFE_DELETE(m_pWriteLog);
}

CServiceProxy::CServiceProxy()
: m_pQryQueue( NULL )
, m_PlatFormService( NULL )
, m_isLogin( false )
, m_QryFinished(false),
m_bMACurrent(false)
{
    m_pQryQueue=new CQryQueue();
}
void CServiceProxy::GetErrorField(int& ErrorID)
{
	m_PlatFormService->GetErrorField(ErrorID);
}
void CServiceProxy::GetErrorField(string& ErrMsg)
{
	m_PlatFormService->GetErrorField(ErrMsg);
}
void CServiceProxy::SetReqID(int nOrderReqID) {
	return m_PlatFormService->SetReqID(nOrderReqID);
};

long CServiceProxy::GetReqID() {
	return m_PlatFormService->GetReqID();
};

long CServiceProxy::GetNextReqID()
{
	return m_PlatFormService->GetNextReqID();
}
int  CServiceProxy::GetAccountList(std::vector<string>& AcctList)
{
	string UserID,BrokerID;
	m_PlatFormService->GetBrokerIDUserID(UserID,BrokerID);
	AcctList.clear();
	AcctList.push_back(UserID);
	return 1;
}
void CServiceProxy::FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID,
								 std::vector<std::string>& vec )
{
}


///交易所ID
const string CServiceProxy::ExchangeID2String(const string& ExchangeID)
{
	const string SHFE="上期所";
	const string CZCE="郑商所";
	const string DCE="大商所";
	const string CFFEX="中金所";

	if(ExchangeID=="SHFE")
		return SHFE;
	else if(ExchangeID=="CZCE")
		return CZCE;
	else if(ExchangeID=="DCE")
		return DCE;
	else if(ExchangeID=="CFFEX")
		return CFFEX;
	else
		return __UNKOWN__;
}
const string CServiceProxy::ExchangeName2ID( const string& name)
{
	const static std::string strCZCE="郑商所合约";
	const static std::string strDCE="大商所合约";
	const static std::string strSHFE="上期所合约";
	const static std::string strCFFEX="中金所合约";
	const static std::string strDCECOMB="大连组合";   
	const static std::string strCZCECOMB="郑州组合";

	if(name==strCZCE)
		return std::string("CZCE");
	if(name==strDCE)
		return std::string("DCE");
	if(name==strSHFE)
		return std::string("SHFE");
	if(name==strCFFEX)
		return std::string("CFFEX");
	if(name==strCZCECOMB)
		return std::string("CZCE_COMB");
	if(name==strDCECOMB)
		return std::string("DCE_COMB");
	return "";
}
SERVER_PARAM_CFG&  CServiceProxy::GetServerParamCfg()
{
	return m_PlatFormService->GetServerParamCfg();
}

bool CServiceProxy::IsModuleValid( int nModuleId )
{
	switch( nModuleId )
	{
	case GID_ACCOUNT:
	case GID_QUOT_TABLE:
	case GID_INSTRUMENT:
	case GID_OPEN_ORDER:
	case GID_ALL_ORDER:
	case GID_ORDER_INSERT_PANEL:
	case GID_POSITIONALL:
	case GID_POSITION:
	case GID_POSITIONDETAIL:
	case GID_THRADE_RECORD:
	case GID_THRADE_STATISTICS:
	//case GID_QUERY_ORDER_BATCH:
	//case GID_QUERY_ORDER_CONDITION:
	//case GID_QUERY_ORDER_POSITION:
	//case GID_QUERY_ORDER_PARKED:
	//case GID_QUERYLOCAL_ORDER:
		return true;
	case GID_POSITIONCOMB:
#ifdef _USE_COMBOINSTRUMENT
        return true;
#else
        return false;
#endif
	default:
		return false;
	}
}

bool CServiceProxy::IsFieldValid( int nModuleId, int nFieldId )
{
	switch( nModuleId )
	{
	case GID_QUOT_TABLE:
		{
			if ( nFieldId <= QUOT_FIELD_ID_Turnover )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	case GID_INSTRUMENT:
		if ( nFieldId == INSTRUMENT_FIELD_ID_CURRENCY||
			nFieldId == INSTRUMENT_FIELD_ID_TICKSPERPOINT||
			nFieldId == INSTRUMENT_FIELD_ID_TICKSIZE)
		{
			return false;
		}
		else
		{
			return true;
		}
	case GID_POSITION:
		{
			if ( nFieldId == POSITION_FIELD_ID_CURRENCY )
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_ACCOUNT:
	{
			if ( nFieldId >= FUNDACCOUNT_MAX_COL_ID ||
				nFieldId == FUNDACCOUNT_EXPOSURE_COL_ID ||
				nFieldId == FUNDACCOUNT_BPUSED_COL_ID ||
				nFieldId == FUNDACCOUNT_BURNRATE_COL_ID||
				nFieldId == FUNDACCOUNT_BPREMAINING_COL_ID)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_ALL_ORDER:
		{
			if ( nFieldId == ALLORDER_d_avgPrice||
				 nFieldId == ALLORDER_UpdateTime)
				
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_OPEN_ORDER:
		{
			if ( nFieldId == OPENORDER_d_avgPrice||
				nFieldId == OPENORDER_UpdateTime)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	case GID_ORDER_INSERT_PANEL:
		{
			if ( nFieldId == ORDERNEWINPUT_INSTRUMENTID_ROW_ID ||
					nFieldId == ORDERNEWINPUT_BUYSELL_ROW_ID ||
					nFieldId == ORDERNEWINPUT_OPENCLOSE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_PRICE_ROW_ID ||
					nFieldId == ORDERNEWINPUT_VOLUME_ROW_ID) {
			//if ( nFieldId == ORDERNEWINPUT_INSTRUMENTID_ROW_ID ||
			//		nFieldId == ORDERNEWINPUT_BUYSELL_ROW_ID ||
			//		//nFieldId == ORDERNEWINPUT_OPENCLOSE_ROW_ID ||
			//		nFieldId == ORDERNEWINPUT_PRICE_ROW_ID ||
			//		nFieldId == ORDERNEWINPUT_VOLUME_ROW_ID || 
			//		ORDERNEWINPUT_TRADEACCOUNT_ROW_ID || 
			//		ORDERNEWINPUT_ORDERTYPE_ROW_ID || 
			//		ORDERNEWINPUT_LIMITPRICE_ROW_ID) {
				return true;
			}
			else {
				return false;
			}
		}
		break;
	default:
		return true;
	}
}

string	g_strShortNames;

void CServiceProxy::GetContractShortNames(string& strShortNames) 
{
	if(g_strShortNames.length()==0)
	{
		HANDLE hFile = NULL;

		std::string strFile;
		GlobalFunc::GetPath(strFile);
		strFile += "ProductSName.txt";

		hFile = ::CreateFile(strFile.c_str(), GENERIC_READ, 
                       FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, 
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 
					   NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			DWORD dwHighLen = 0;
			DWORD dwLen = ::GetFileSize(hFile, &dwHighLen);
			char* pBuff = new char[dwLen+8];
			ZeroMemory(pBuff, dwLen+8);
			::ReadFile(hFile, pBuff, dwLen, &dwLen, NULL);
			::CloseHandle(hFile);
			g_strShortNames = pBuff;
			delete[] pBuff;
		}
	}
	strShortNames = g_strShortNames;
//#ifdef _USE_COMBOINSTRUMENT
//	strShortNames = 
//		"cu,铜,t,al,铝,l,zn,锌,x,pb,铅,q,ru,橡胶,xj,fu,燃油,ry,au,黄金,hj,ag,白银,by,rb,螺纹钢,lwg,wr,线材,xc,IF,股指,gz,a,豆一,dy,b,豆二,de,m,豆粕,dp,y,豆油,dy,j,焦炭,jt,c,黄玉米,hym,l,聚乙烯,jyx,p,棕榈油,zly,v,聚氯乙烯,jlyx,SR,白糖,bt,CF,棉花,mh,RO,菜油,cy,TA,PTA,pta,ME,甲醇,jc,WS,强麦,qm,WT,硬麦,ym,PM,普麦,pm,ER,早籼稻,zxd,SPD,郑商跨期,zskq,SP,大商跨期,dskq,SPC,大商跨品种,dskpz";
//#else
//	strShortNames = 
//		"cu,铜,t,al,铝,l,zn,锌,x,pb,铅,q,ru,橡胶,xj,fu,燃油,ry,au,黄金,hj,ag,白银,by,rb,螺纹钢,lwg,wr,线材,xc,IF,股指,gz,a,豆一,dy,b,豆二,de,m,豆粕,dp,y,豆油,dy,j,焦炭,jt,c,黄玉米,hym,l,聚乙烯,jyx,p,棕榈油,zly,v,聚氯乙烯,jlyx,SR,白糖,bt,CF,棉花,mh,RO,菜油,cy,TA,PTA,pta,ME,甲醇,jc,WS,强麦,qm,WT,硬麦,ym,PM,普麦,pm,ER,早籼稻,zxd";
//#endif
}

string CServiceProxy::GetVersionIconString()
{
	std::string s="ULD_VERSTR_"+m_PlatFormService->GetServerParamCfg().VersionStr;
	std::transform(s.begin(), s.end(), s.begin(), toupper);
	return s;
}
	//是否支持插件
bool CServiceProxy::SupportPlugins()
{
	std::string s=m_PlatFormService->GetServerParamCfg().VersionStr;
	return s.find("plugins")!=string::npos;
}
//是否支持多账号
bool CServiceProxy::IsMultipleAccounts()
{
	std::string s=m_PlatFormService->GetServerParamCfg().VersionStr;
	return s.find("multi")!=string::npos;
}
bool CServiceProxy::HaveOrderType(UIOrderType type)
{
	if(IsMultipleAccounts())
	{
		if(type==UIOT_STANDARD)
			return true;
		else
			return false;
	}
	else
	{
		switch(type)
		{
		case UIOT_STANDARD:
			return true;
		case UIOT_BATCH:
			return true;
		case UIOT_CONDITION:
			return true;
		case UIOT_PROFIT:
			return true;
		case UIOT_HEDGE:
			return true;
		case UIOT_STOPLIMIT:
			return false;
		case UIOT_REPLACEORDER:
			return false;
		case UIOT_SIMULATEMARKET:
			//return true;
			return false;
		case UIOT_OPENCLOSE:
			return true;
		}
	}
	return false;
}
///获取账号信息
int CServiceProxy::GetTrader(std::vector<PlatformStru_Trader> &outData) 
{
	return 0;
}
///获取交易所信息
int CServiceProxy::GetExchange(std::vector<PlatformStru_Exchange> &outData)
{
	return 0;
}
///获取合约信息
int CServiceProxy::GetContract(std::vector<PlatformStru_Contract> &outData)
{
	return 0;
}
///获取商品信息
int CServiceProxy::GetCommodity(std::vector<PlatformStru_Commodity> &outData)
{
	return 0;
}
///获取报单类型信息
int CServiceProxy::GetOrderType(std::vector<PlatformStru_OrderType> &outData)
{
	return 0;
}
bool CServiceProxy::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
	return false;
}
bool CServiceProxy::GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,
									PlatformStru_AccountPosition* posi)
{
	return false;
}
bool CServiceProxy::GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,
										PlatformStru_AccountPosition* posi)
{
	return false;
}

std::string CServiceProxy::GetLogonReason()
{
	return "";
}
void CServiceProxy::GetLinkStatus(int * pHost,int * pPrice)
{
	if(pHost)  *pHost=-1;
	if(pPrice) *pPrice=-1;
}
int CServiceProxy::GetReports(map<DWORD, Report>& mapReport) 
{
	return 0;
}
double CServiceProxy::GetExchangeRate(char* currency)
{
	return 1.0;
}
void CServiceProxy::NotifyStartStopInitQry(bool bStart) 
{ 
	if(m_pQryQueue)
		m_pQryQueue->NotifyStartStopInitQry(bStart); 
}
int CServiceProxy::GetAllOrderFTIDList( std::vector<long>& vec )
{
	return m_PlatFormService->GetAllOrderFTIDList(vec);
}

int CServiceProxy::GetAllOrderInfo( long lFTID, PlatformStru_OrderInfo& outData )
{
	return m_PlatFormService->GetAllOrderInfo(lFTID, outData);
}

int CServiceProxy::GetAllOrderInfo( OrderKey key, PlatformStru_OrderInfo& outData )
{
	return m_PlatFormService->GetAllOrderInfo(key, outData);
}

int CServiceProxy::GetPositionFTIDList( std::vector<long>& vec )
{
	return m_PlatFormService->GetPositionFTIDList(vec);
}

int CServiceProxy::GetPositionInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_PlatFormService->GetPositionInfo(lFTID, outData);
}

int CServiceProxy::GetPositionInfo( PositionKey key, PlatformStru_Position& outData )
{
	return m_PlatFormService->GetPositionInfo(key, outData);
}

int CServiceProxy::GetPositionCombFTIDList( std::vector<long>& vec )
{
	return m_PlatFormService->GetPositionCombFTIDList(vec);
}

int CServiceProxy::GetPositionCombInfo( long lFTID, PlatformStru_PositionComb& outData )
{
	return m_PlatFormService->GetPositionCombInfo(lFTID, outData);
}
int CServiceProxy::GetGroupMaxContractNum()
{
	return 20;
}
bool CServiceProxy::ContractCountIsFit()
{
	return true;
}
bool CServiceProxy::GetMACurrentAccount()
{
	return m_bMACurrent;
}
void CServiceProxy::SetMACurrentAccount(bool bCur)
{
	m_bMACurrent=bCur;
}
bool CServiceProxy::GetQryFinished()
{
	return m_QryFinished;
}
void CServiceProxy::SetQryFinished(bool bFinished)
{
	m_QryFinished=bFinished;
}