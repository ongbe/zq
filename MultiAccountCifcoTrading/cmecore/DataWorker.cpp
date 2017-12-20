#include "StdAfx.h"
#include "DataWorker.h"
#include "CmeCoreApi.h"
#include "../inc/tools/UniPkg.h"
#include "../inc/tools/client_stru.h"
#include "../inc/tools/client_cmdid.h"
#ifdef _DEBUG
	#pragma comment(lib, "ToolsD.lib")
#else
	#pragma comment(lib, "Tools.lib")
#endif
static CCmeCoreApi* g_pTradeAPI=NULL,*g_pQuotAPI=NULL;
CTradeDataWorker::CTradeDataWorker(Stru_IPAddress LocalAddr,Stru_IPAddress SvrAddr,
									 CCmeCoreApi* pAPI,
                                     int SOSNDBUFSize,int SORCVBUFSize):
CTcpClientWorker(LocalAddr,SvrAddr,rcvdata,disconn,startconnect,connsuccess,
                 connfailure,ontime,SOSNDBUFSize,SORCVBUFSize)
{
    g_pTradeAPI=pAPI;
}

CTradeDataWorker::~CTradeDataWorker(void)
{

}
//从tcp得到数据后，调用此回调函数进行处理
void CTradeDataWorker::rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
   if(g_pTradeAPI) g_pTradeAPI->trade_rcvdata(pData,DataLen, LocalAddr,SvrAddr,hSocket);
}
//断开tcp连接后，调用此回调函数进行处理
void CTradeDataWorker::disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{	
   if(g_pTradeAPI) g_pTradeAPI->trade_disconn(LocalAddr,SvrAddr,hSocket);
}
//开始调用connect()后，调用此函数通知应用层
void CTradeDataWorker::startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
   if(g_pTradeAPI) g_pTradeAPI->trade_startconnect(LocalAddr,SvrAddr,hSocket);
}
//连接成功后，调用此回调函数进行处理
void CTradeDataWorker::connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
   if(g_pTradeAPI) g_pTradeAPI->trade_connsuccess(LocalAddr,SvrAddr,hSocket);
}
//连接失败后，调用此回调函数进行处理
void CTradeDataWorker::connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
   if(g_pTradeAPI) g_pTradeAPI->trade_connfailure(LocalAddr,SvrAddr,hSocket);
}
void CTradeDataWorker::ontime(void)
{
   if(g_pTradeAPI) g_pTradeAPI->trade_ontime();
}
CQuotDataWorker::CQuotDataWorker(Stru_IPAddress LocalAddr,Stru_IPAddress SvrAddr,
									 CCmeCoreApi* pAPI,
                                     int SOSNDBUFSize,int SORCVBUFSize):
CTcpClientWorker(LocalAddr,SvrAddr,rcvdata,disconn,startconnect,connsuccess,
                 connfailure,ontime,SOSNDBUFSize,SORCVBUFSize)
{
    g_pQuotAPI=pAPI;
}

CQuotDataWorker::~CQuotDataWorker(void)
{

}
//从tcp得到数据后，调用此回调函数进行处理
void CQuotDataWorker::rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
   if(g_pQuotAPI) g_pQuotAPI->quot_rcvdata(pData,DataLen, LocalAddr,SvrAddr,hSocket);
}
//断开tcp连接后，调用此回调函数进行处理
void CQuotDataWorker::disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{	
   if(g_pQuotAPI) g_pQuotAPI->quot_disconn(LocalAddr,SvrAddr,hSocket);
}
//开始调用connect()后，调用此函数通知应用层
void CQuotDataWorker::startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
   if(g_pQuotAPI) g_pQuotAPI->quot_startconnect(LocalAddr,SvrAddr,hSocket);
}
//连接成功后，调用此回调函数进行处理
void CQuotDataWorker::connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
   if(g_pQuotAPI) g_pQuotAPI->quot_connsuccess(LocalAddr,SvrAddr,hSocket);
}
//连接失败后，调用此回调函数进行处理
void CQuotDataWorker::connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
   if(g_pQuotAPI) g_pQuotAPI->quot_connfailure(LocalAddr,SvrAddr,hSocket);
}
void CQuotDataWorker::ontime(void)
{
   if(g_pQuotAPI) g_pQuotAPI->quot_ontime();
}
int CTradeDataWorker::SendSingleData(int Cmdid,char* pData,int DataLen)
{
    int packetlength=sizeof(Stru_UniPkgHead)+DataLen+4;
    char* pbuffer=new char[packetlength];
    if(pbuffer==NULL) return 0;
    Stru_UniPkgHead head;
    head.len=DataLen;
    head.cmdid=Cmdid;
    head.moduleid=ModuleID_Client;
    memcpy(pbuffer,&head,sizeof(Stru_UniPkgHead));
    memcpy(pbuffer+sizeof(Stru_UniPkgHead),pData,DataLen);
    DWORD tail=TailID_UniPkg;
    memcpy(pbuffer+packetlength-4,(char*)&tail,sizeof(DWORD));
    int ret=SendData(pbuffer,packetlength);
    delete[] pbuffer;
    return ret;
}
int CQuotDataWorker::SendSingleData(int Cmdid,char* pData,int DataLen)
{
    int packetlength=sizeof(Stru_UniPkgHead)+DataLen+4;
    char* pbuffer=new char[packetlength];
    if(pbuffer==NULL) return 0;
    Stru_UniPkgHead head;
    head.len=DataLen;
    head.cmdid=Cmdid;
    head.moduleid=ModuleID_Client;
    memcpy(pbuffer,&head,sizeof(Stru_UniPkgHead));
    memcpy(pbuffer+sizeof(Stru_UniPkgHead),pData,DataLen);
    DWORD tail=TailID_UniPkg;
    memcpy(pbuffer+packetlength-4,(char*)&tail,sizeof(DWORD));
    int ret=SendData(pbuffer,packetlength);
    delete[] pbuffer;
    return ret;
}
////////////////////////////////////////////////////////////////////////////////
//
//具体的业务逻辑处理
//
////////////////////////////////////////////////////////////////////////////////
int CTradeDataWorker::SendHeartbeat()
{
    return SendSingleData(CMDID_Client_Heartbeat,NULL,0);
}
int CQuotDataWorker::SendHeartbeat()
{
    return SendSingleData(CMDID_Client_Heartbeat,NULL,0);
}
//发出交易用户登陆请求
int CTradeDataWorker::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
    Stru_Client_Login ClientLogin={0};
    ClientLogin.RequestID=nRequestID;
    if(pReqUserLoginField)
    {
        strcpy_s(ClientLogin.ClientProductName,31,pReqUserLoginField->UserProductInfo);
        strcpy_s(ClientLogin.LoginUserName,MAXLEN_LoginName-1,pReqUserLoginField->UserID);
        strcpy_s(ClientLogin.LoginUserPW,MAXLEN_LoginPW-1,pReqUserLoginField->Password);
    }
    SendSingleData(CMDID_Client_Login,(char*)&ClientLogin,sizeof(Stru_Client_Login));
    return 0;
}
//发出行情用户登陆请求
int CQuotDataWorker::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
    Stru_Client_Login ClientLogin={0};
    ClientLogin.RequestID=nRequestID;
    if(pReqUserLoginField)
    {
        strcpy_s(ClientLogin.ClientProductName,31,pReqUserLoginField->UserProductInfo);
        strcpy_s(ClientLogin.LoginUserName,MAXLEN_LoginName-1,pReqUserLoginField->UserID);
        strcpy_s(ClientLogin.LoginUserPW,MAXLEN_LoginPW-1,pReqUserLoginField->Password);
    }
    SendSingleData(CMDID_Client_Login,(char*)&ClientLogin,sizeof(Stru_Client_Login));
    return 0;
}
int CTradeDataWorker::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
    Stru_Client_Logout ClientLogout={0};
    ClientLogout.RequestID=nRequestID;
    SendSingleData(CMDID_Client_Logout,(char*)&ClientLogout,sizeof(Stru_Client_Logout));
    return 0;
}
int CQuotDataWorker::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
    Stru_Client_Logout ClientLogout={0};
    ClientLogout.RequestID=nRequestID;
    SendSingleData(CMDID_Client_Logout,(char*)&ClientLogout,sizeof(Stru_Client_Logout));
    return 0;
}
//发出查询结算信息是否确认
int CTradeDataWorker::ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField* pQrySettlementInfoConfirm, int nRequestID)
{
    Stru_Client_GetIsSettltmentReportConfirmed ReportConfirmed={0};
    ReportConfirmed.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetIsSettltmentReportConfirmed,(char*)&ReportConfirmed,sizeof(Stru_Client_GetIsSettltmentReportConfirmed));
    return 0;
}
//发出查询结算信息文本
int CTradeDataWorker::ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pSettlementInfoConfirm, int nRequestID)
{
    Stru_Client_GetSettlementReportInfo info={0};
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetSettltmentReport,(char*)&info,sizeof(Stru_Client_GetSettlementReportInfo));
    return 0;
}
//客户端确认投资者结算结果
int CTradeDataWorker::ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID)
{
    Stru_Client_ConfirmSettltmentReport info={0};
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_ConfirmSettltmentReport,(char*)&info,sizeof(Stru_Client_ConfirmSettltmentReport));
    return 0;
}
//请求查询合约信息
int CTradeDataWorker::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
    GetCurrencyInfo(nRequestID);
    return 0;
}
////请求查询货币信息
int CTradeDataWorker::GetCurrencyInfo(int nRequestID)
{
    Stru_Client_GetCurrencyInfo info={0};
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetCurrencyInfo,(char*)&info,sizeof(Stru_Client_GetCurrencyInfo));
    return 0;
}
int CTradeDataWorker::GetDepositWithdrawInfo(int nRequestID)
{
    Stru_Client_GetDepositWithdrawDyna dwi;
    dwi.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetDepositWithdrawInfo,(char*)&dwi,sizeof(Stru_Client_GetDepositWithdrawDyna));
    return 0;
}
////请求查询合约信息步骤1.获取全部交易所信息
int CTradeDataWorker::GetAllExchangeInfo(int nRequestID)
{
    Stru_Client_GetAllExchangeInfo info={0};
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetAllExchangeInfo,(char*)&info,sizeof(Stru_Client_GetAllExchangeInfo));
    return 0;
}
//请求查询合约信息步骤3.获取交易所下产品组信息
int CTradeDataWorker::GetExchangeProductGroupInfo(int ExchangeID,int nRequestID)
{
    Stru_Client_GetExchangeProductGroupInfo info={0};
    info.ExchangeID=ExchangeID;
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetExchangeProductGroupInfo,(char*)&info,
        sizeof(Stru_Client_GetExchangeProductGroupInfo));
    return 0;
}
//请求查询合约信息步骤5.获取交易所下所有产品信息
int CTradeDataWorker::GetExchangeProductInfo(int ExchangeID,int nRequestID)
{
    Stru_Client_GetExchangeProductInfo info={0};
    info.ExchangeID=ExchangeID;
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetExchangeProductInfo,(char*)&info,
        sizeof(Stru_Client_GetExchangeProductInfo));
    return 0;
}
//请求查询合约信息步骤7.获取交易所下所有合约信息
int CTradeDataWorker::GetExchangeContractInfo(int ExchangeID,int nRequestID)
{
    Stru_Client_GetExchangeContractInfoInfo info={0};
    info.ExchangeID=ExchangeID;
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetExchangeContractInfo,(char*)&info,
        sizeof(Stru_Client_GetExchangeContractInfoInfo));
    return 0;
}
//请求查询资金
int CTradeDataWorker::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID)
{
    Stru_Client_GetInitialEquityInfo info={0};
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetInitialEquityInfo,(char*)&info,
        sizeof(Stru_Client_GetInitialEquityInfo));
    return 0;
}
//请求查询成交
int CTradeDataWorker::ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID)
{
    Stru_Client_GetTradeInfo info={0};
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetTradeInfo,(char*)&info,sizeof(Stru_Client_GetTradeInfo));  
    return 0;
}
//请求查询持仓明细
int CTradeDataWorker::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID)
{
    Stru_Client_GetPositionInfo info={0};
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetPositionInfo,(char*)&info,sizeof(Stru_Client_GetPositionInfo));  
    return 0;
}
//请求查询报单
int CTradeDataWorker::ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID)
{
    Stru_Client_GetOrderInfo info={0};
    info.nRequestID=nRequestID;
    SendSingleData(CMDID_Client_GetOrderInfo,(char*)&info,sizeof(Stru_Client_GetOrderInfo));  
    return 0;
}
int CQuotDataWorker::SubscribeMarketData(int ContractID,int RequestID)
{
    Stru_Client_SubscribeMarketData smd={0};
    smd.nRequestID=RequestID;
    smd.SubscribeCount=1;
    smd.SubscribeContractID[0]=ContractID;
    SendSingleData(CMDID_Client_SubscribeMarketData,(char*)&smd,sizeof(Stru_Client_SubscribeMarketData));  
    return 0;
}
int CQuotDataWorker::UnSubscribeMarketData(int ContractID,int RequestID)
{
    Stru_Client_UnsubscribeMarketData smd={0};
    smd.nRequestID=RequestID;
    smd.UnsubscribeCount=1;
    smd.UnsubscribeContractID[0]=ContractID;
    SendSingleData(CMDID_Client_UnsubscribeMarketData,(char*)&smd,sizeof(Stru_Client_SubscribeMarketData));  
    return 0;
}
int CTradeDataWorker::ReqQryDepthMarketData(int ContractID,int nRequestID)
{
    Stru_Client_GetMarketDataSnapshot mds={0};
    mds.nRequestID=nRequestID;
    mds.GetCount=1;      
    mds.GetContractID[0]=ContractID;
    SendSingleData(CMDID_Client_GetMarketDataSnapshot,(char*)&mds,sizeof(Stru_Client_GetMarketDataSnapshot)); 
    return 0;
}
//发出报单命令
int CTradeDataWorker::ReqOrderCmd(Stru_Base_OrderCmdInfo& InputOrder, int nRequestID)
{
    Stru_Client_OrderCmd mds={0};
    mds.nRequestID=nRequestID;
    memcpy(&(mds.info),&InputOrder,sizeof(Stru_Base_OrderCmdInfo));
    SendSingleData(CMDID_Client_OrderCmd,(char*)&InputOrder,sizeof(Stru_Client_OrderCmd)); 
    return 0;
}
int CTradeDataWorker::ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID)
{
    Stru_Client_ChangeLoginPW smd={0};
    smd.RequestID=nRequestID;
    strcpy_s(smd.OldLoginUserPW,sizeof(smd.OldLoginUserPW)-1,pUserPasswordUpdate->OldPassword);
    strcpy_s(smd.NewLoginUserPW,sizeof(smd.NewLoginUserPW)-1,pUserPasswordUpdate->NewPassword);
    SendSingleData(CMDID_Client_ChangeLoginPW,(char*)&smd,sizeof(Stru_Client_ChangeLoginPW));  
    return 0;
}
int CTradeDataWorker::ReqQryInstrumentCommissionRateMarginRate(int ExchangeID, int nRequestID)
{
    Stru_Client_GetExchangeCommissionMarginInfo info={0};
    info.nRequestID=nRequestID;
    info.ExchangeID=ExchangeID;
    SendSingleData(CMDID_Client_GetExchangeCommissionMarginInfo,(char*)&info,sizeof(Stru_Client_GetExchangeCommissionMarginInfo));
    return 0;  
}
