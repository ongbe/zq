#pragma once
#include "winSock2.h"
#include "../inc/Tools/TcpClientWorker.h"
#include "../inc/tools/client_stru.h"
#include "../inc/tools/client_cmdid.h"
class CCmeCoreApi;
class CTradeDataWorker :public CTcpClientWorker
{
public:
    CTradeDataWorker(Stru_IPAddress LocalAddr,Stru_IPAddress SvrAddr,
        CCmeCoreApi* pAPI=NULL,int SOSNDBUFSize=655360,int SORCVBUFSize=655360);
    ~CTradeDataWorker();
public:
    static void	rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//从tcp得到数据后，调用此回调函数进行处理
    static void	disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);		//断开tcp连接后，调用此回调函数进行处理
    static void	startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);										//开始调用connect()后，调用此函数通知应用层
    static void	connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//连接成功后，调用此回调函数进行处理
    static void	connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//连接失败后，调用此回调函数进行处理
    static void ontime(void);
public:
    int SendSingleData(int Cmdid,char* pData,int DataLen);
public:
    int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);
    int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID);
    int ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField* pQrySettlementInfoConfirm, int nRequestID);
    int ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pSettlementInfoConfirm, int nRequestID);
    int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID);
    int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID);
    int GetCurrencyInfo(int nRequestID);
    int GetDepositWithdrawInfo(int nRequestID);
    int GetAllExchangeInfo(int nRequestID);
    int GetExchangeProductGroupInfo(int ExchangeID,int nRequestID);
    int GetExchangeProductInfo(int ExchangeID,int nRequestID);
    int GetExchangeContractInfo(int ExchangeID,int nRequestID);
    int ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID);
    int ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID);
    int ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID);
    int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID);
    int ReqOrderCmd(Stru_Base_OrderCmdInfo& InputOrder, int nRequestID);
    int ReqQryDepthMarketData(int ContractID, int nRequestID);
    int ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID);
    int SendHeartbeat();
    int ReqQryInstrumentCommissionRateMarginRate(int ExchangeID, int nRequestID); 
};
class CQuotDataWorker :public CTcpClientWorker
{
public:
    CQuotDataWorker(Stru_IPAddress LocalAddr,Stru_IPAddress SvrAddr,
        CCmeCoreApi* pAPI=NULL,int SOSNDBUFSize=655360,int SORCVBUFSize=655360);
    ~CQuotDataWorker();
public:
    static void	rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//从tcp得到数据后，调用此回调函数进行处理
    static void	disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);		//断开tcp连接后，调用此回调函数进行处理
    static void	startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);										//开始调用connect()后，调用此函数通知应用层
    static void	connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//连接成功后，调用此回调函数进行处理
    static void	connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//连接失败后，调用此回调函数进行处理
    static void ontime(void);
public:
    int SendSingleData(int Cmdid,char* pData,int DataLen);
public:
    int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);
    int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID);
    int SubscribeMarketData(int ContractID,int RequestID);
    int UnSubscribeMarketData(int ContractID,int RequestID);
    int SendHeartbeat();
};