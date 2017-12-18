#include "stdafx.h"

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
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


/*--------------------------------------------------------------------------------------------------------
           下面业务请求都是异步的，需定制具体业务数据，才能收到请求结果
  ---------------------------------------------------------------------------------------------------------*/
///用户口令更新请求
int CServiceProxy::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate &UserPasswordUpdate, int nRequestID)
{
    CHECK_LOGIN_STATUS();
        
    return m_PlatFormService->ReqUserPasswordUpdate(&UserPasswordUpdate, nRequestID);
}

///资金账户口令更新请求
int CServiceProxy::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate &TradingAccountPasswordUpdate, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqTradingAccountPasswordUpdate(&TradingAccountPasswordUpdate, nRequestID);
}

///报单录入请求. 返回时pInputOrder中的RrequestID、BrokerID、InvestorID、UserID、OrderRef的值被修改为底层确定的值
int CServiceProxy::ReqOrderInsert(PlatformStru_InputOrder &InputOrder, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqOrderInsert(&InputOrder, nRequestID);
}

///预埋单录入请求. 返回时pInputOrder中的RrequestID、BrokerID、InvestorID、UserID、OrderRef的值被修改为底层确定的值
int CServiceProxy::ReqParkedOrderInsert(PlatformStru_ParkedOrder &ParkedOrder, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqParkedOrderInsert(&ParkedOrder, nRequestID);
}

///预埋撤单录入请求
int CServiceProxy::ReqParkedOrderAction(PlatformStru_ParkedOrderAction &ParkedOrderAction, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqParkedOrderAction(&ParkedOrderAction, nRequestID);
}

///报单操作请求
int CServiceProxy::ReqOrderAction(PlatformStru_InputOrderAction &InputOrderAction, int nRequestID)
{
    CHECK_LOGIN_STATUS();

    return m_PlatFormService->ReqOrderAction(&InputOrderAction, nRequestID);
}

///报单操作请求(撤单)
int CServiceProxy::ReqCancelOrder(const OrderKey& orderkey, int nRequestID)
{
    CHECK_LOGIN_STATUS();

    return m_PlatFormService->ReqCancelOrder(orderkey, nRequestID);
}

///查询最大报单数量请求
int CServiceProxy::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &QueryMaxOrderVolume, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQueryMaxOrderVolume(QueryMaxOrderVolume,nRequestID);
    return 0;
}

///投资者结算结果确认
int CServiceProxy::ReqSettlementInfoConfirm() 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqSettlementInfoConfirm();
}

///请求删除预埋单
int CServiceProxy::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &RemoveParkedOrder, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqRemoveParkedOrder(&RemoveParkedOrder, nRequestID);
}

///请求删除预埋撤单
int CServiceProxy::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &RemoveParkedOrderAction, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqRemoveParkedOrderAction(&RemoveParkedOrderAction, nRequestID);
}

///请求查询报单
int CServiceProxy::ReqQryOrder(PlatformStru_QryOrder &QryOrder, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    if(m_pQryQueue)
        m_pQryQueue->ReqQryOrder(QryOrder,nRequestID);
    return 0;
}

///请求查询成交
int CServiceProxy::ReqQryTrade(PlatformStru_QryTrade &QryTrade, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryTrade(QryTrade,nRequestID);
    return 0;
}

///请求查询投资者持仓
int CServiceProxy::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &QryInvestorPosition, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryInvestorPosition(QryInvestorPosition,nRequestID);
    return 0;
}

///请求查询资金账户
int CServiceProxy::ReqQryTradingAccount(PlatformStru_QryTradingAccount &QryTradingAccount, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryTradingAccount(QryTradingAccount,nRequestID);
    return 0;
}

//清除查询请求队列中指定类型的查询请求，用于TCP重连后的重查前的处理
//ReqCmdID:1:资金查询；2:成交查询；4:报单查询；8:持仓明细查询；16:持仓查询；32:组合持仓明细查询
//ReqCmdID可以或处理
void CServiceProxy::ClearReqInQryQueue(int ReqCmdID)
{
    if(!m_isLogin)  return;
    
    if(m_pQryQueue)
        m_pQryQueue->ClearReqInQryQueue(ReqCmdID);
}

///请求查询投资者
int CServiceProxy::ReqQryInvestor(PlatformStru_QryInvestor &QryInvestor, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqQryInvestor(&QryInvestor, nRequestID);
}

///请求查询经纪公司交易参数
int CServiceProxy::ReqQryBrokerTradingParams(int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryBrokerTradingParams(nRequestID);

    return 0;
}

///请求查询品种
int CServiceProxy::ReqQryProduct(PlatformStru_QryProduct &QryProduct, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryProduct(QryProduct,nRequestID);

    return 0;
}

///请求查询合约
int CServiceProxy::ReqQryInstrument(PlatformStru_QryInstrument &QryInstrument, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryInstrument(QryInstrument,nRequestID);

    return 0;
}

///请求查询行情
int CServiceProxy::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &QryDepthMarketData, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryDepthMarketData(QryDepthMarketData,nRequestID);
	return 0;
}

///请求查询投资者结算结果
int CServiceProxy::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &QrySettlementInfo, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
   
    if(m_pQryQueue)
        m_pQryQueue->ReqQrySettlementInfo(QrySettlementInfo,nRequestID);

    return 0;
}

///请求查询保证金监管系统经纪公司资金账户密钥
int CServiceProxy::ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
   
    if(m_pQryQueue)
        m_pQryQueue->ReqQryCFMMCTradingAccountKey(QryCFMMCTradingAccountKey,nRequestID);

    return 0;
}


///请求查询转帐银行
int CServiceProxy::ReqQryTransferBank(PlatformStru_QryTransferBank &QryTransferBank, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryTransferBank(QryTransferBank,nRequestID);
    return 0;
}

///请求查询投资者持仓明细
int CServiceProxy::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &QryInvestorPositionDetail, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryInvestorPositionDetail(QryInvestorPositionDetail,nRequestID);
    return 0;
}

///请求查询客户通知
int CServiceProxy::ReqQryNotice(PlatformStru_QryNotice &QryNotice, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryNotice(QryNotice,nRequestID);
    return 0;
}

///请求查询结算信息确认
int CServiceProxy::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &QrySettlementInfoConfirm, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQrySettlementInfoConfirm(QrySettlementInfoConfirm,nRequestID);
    return 0;
}

///请求查询投资者持仓明细
int CServiceProxy::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &QryInvestorPositionCombineDetail, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryInvestorPositionCombineDetail(QryInvestorPositionCombineDetail,nRequestID);
    return 0;
}

///请求查询转帐流水
int CServiceProxy::ReqQryTransferSerial(PlatformStru_QryTransferSerial &QryTransferSerial, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryTransferSerial(QryTransferSerial,nRequestID);
    return 0;
}

///请求查询签约银行
int CServiceProxy::ReqQryContractBank(PlatformStru_QryContractBank &QryContractBank, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryContractBank(QryContractBank,nRequestID);
    return 0;
}

///请求查询银期签约关系
int CServiceProxy::ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryAccountregister(pQryAccountregister,nRequestID);
    return 0;
}

///请求查询汇率
int CServiceProxy::ReqQryExchangeRate(PlatformStru_QryExchangeRate &QryExchangeRate, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryExchangeRate(QryExchangeRate,nRequestID);
    return 0;
}

///请求查询预埋单
int CServiceProxy::ReqQryParkedOrder(PlatformStru_QryParkedOrder &QryParkedOrder, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryParkedOrder(QryParkedOrder,nRequestID);
    return 0;
}

///请求查询预埋撤单
int CServiceProxy::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &QryParkedOrderAction, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQryParkedOrderAction(QryParkedOrderAction,nRequestID);
    return 0;
}




///期货发起银行资金转期货请求
int CServiceProxy::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer &ReqTransfer, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqFromBankToFutureByFuture(&ReqTransfer, nRequestID);
}

///期货发起期货资金转银行请求
int CServiceProxy::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer &ReqTransfer, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    

    return m_PlatFormService->ReqFromFutureToBankByFuture(&ReqTransfer, nRequestID);
}

///期货发起查询银行余额请求
int CServiceProxy::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &ReqQueryAccount, int nRequestID) 
{
    CHECK_LOGIN_STATUS();
    
    if(m_pQryQueue)
        m_pQryQueue->ReqQueryBankAccountMoneyByFuture(ReqQueryAccount,nRequestID);
    return 0;
}

///订阅询价通知，OptionInstrumentIDs为空则订阅全部期权合约的询价通知
int  CServiceProxy::SubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->SubscribeForQuoteRsp(OptionInstrumentIDs);
}
///退订询价通知，OptionInstrumentIDs为空则退订全部合约的询价通知
int CServiceProxy::UnSubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->UnSubscribeForQuoteRsp(OptionInstrumentIDs);
}

///请求询价录入
int CServiceProxy::ReqForQuoteInsert(PlatformStru_InputForQuoteField& InputForQuote, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqForQuoteInsert(InputForQuote, nRequestID);
}
///执行宣告录入请求
int CServiceProxy::ReqExecOrderInsert(PlatformStru_InputExecOrderField& InputExecOrder, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqExecOrderInsert(InputExecOrder, nRequestID);
}
///执行宣告操作请求
int CServiceProxy::ReqExecOrderAction(PlatformStru_InputExecOrderActionField& InputExecOrderAction, int nRequestID)
{
    CHECK_LOGIN_STATUS();
    
    return m_PlatFormService->ReqExecOrderAction(InputExecOrderAction, nRequestID);
}
///请求查询执行宣告
int CServiceProxy::ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID)
{
    if(m_pQryQueue)
        m_pQryQueue->ReqQryExecOrder(QryExecOrder,nRequestID);
    return 0;
}

