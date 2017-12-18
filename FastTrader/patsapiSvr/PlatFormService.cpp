#include "stdafx.h"

#include "PlatFormService.hpp"
#include "DataCenter.hpp"
#include "wx/thread.h"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"

#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatFormService", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif
//仅限于CPlatFormService的成员函数使用
#define CHECK_TRADE_STATUS() \
    if(!m_pTradeApi || m_PlatformParam.TradeStatus!=CPlatFormParam::CONNSTATUS_Connected)\
    {\
        LOG_INFO("交易API还未准备就绪，可能登入未成功或正在重连");\
        return ret;\
    }

///用户口令更新请求
/***********************************************************
*   return  :   0:成功
*               -1:     网络连接失败
*               -2:     未处理请求超过许可数
*               -3:     每秒发送请求数超过许可数
*               -999:   其它原因失败
***********************************************************/
int CPlatFormService::ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate *pUserPasswordUpdate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pUserPasswordUpdate->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pUserPasswordUpdate->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pUserPasswordUpdate->Thost.UserID, m_PlatformParam.UserID.c_str(), sizeof(pUserPasswordUpdate->Thost.UserID)-1);  ///用户代码,等于投资者代码
    ret = m_pTradeApi->ReqUserPasswordUpdate((CThostFtdcUserPasswordUpdateField *)&(pUserPasswordUpdate->Thost), nRequestID); 
    LOG_INFO("ReqUserPasswordUpdate(用户口令更新请求):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],UserID=[%s],OldPassword=[******],NewPassword=[******]",
                     ret, nRequestID,
                     pUserPasswordUpdate->Thost.BrokerID,
                     pUserPasswordUpdate->Thost.UserID);
    return ret;
}

///资金账户口令更新请求
int CPlatFormService::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pTradingAccountPasswordUpdate->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pTradingAccountPasswordUpdate->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pTradingAccountPasswordUpdate->Thost.AccountID, m_PlatformParam.UserID.c_str(), sizeof(pTradingAccountPasswordUpdate->Thost.AccountID)-1);  ///资金账号,等于用户代码,也等于投资者代码
    ret = m_pTradeApi->ReqTradingAccountPasswordUpdate((CThostFtdcTradingAccountPasswordUpdateField *)&pTradingAccountPasswordUpdate->Thost, nRequestID);  
    LOG_INFO("ReqTradingAccountPasswordUpdate(资金账户口令更新请求):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],AccountID=[%s],OldPassword=[******],NewPassword=[******]",
                     ret, nRequestID,
                     pTradingAccountPasswordUpdate->Thost.BrokerID,
                     pTradingAccountPasswordUpdate->Thost.AccountID);
    return ret;
}

///报单录入请求
int CPlatFormService::ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    pInputOrder->RequestID = nRequestID;
    strncpy(pInputOrder->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pInputOrder->BrokerID)-1); ///经纪公司代码       
    strncpy(pInputOrder->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pInputOrder->InvestorID)-1);  ///投资者代码
    strncpy(pInputOrder->UserID, m_PlatformParam.UserID.c_str(), sizeof(pInputOrder->UserID)-1);  ///用户代码
    //pInputOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	sprintf(pInputOrder->OrderRef,"%12d",++m_CurOrderRef);
    ret = m_pTradeApi->ReqOrderInsert(pInputOrder, nRequestID);  
    LOG_INFO("ReqOrderInsert(报单录入请求):ret=[%d],nRequestID=[%d]\n"
             "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             ret, nRequestID,
             pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
             pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
             pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
             pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
             pInputOrder->RequestID, pInputOrder->UserForceClose);
    return ret;
}


///预埋单录入请求
int CPlatFormService::ReqParkedOrderInsert(PlatformStru_ParkedOrder *pParkedOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    pParkedOrder->RequestID = nRequestID;
    strncpy(pParkedOrder->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pParkedOrder->BrokerID)-1); ///经纪公司代码       
    strncpy(pParkedOrder->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pParkedOrder->InvestorID)-1);  ///投资者代码
    strncpy(pParkedOrder->UserID, m_PlatformParam.UserID.c_str(), sizeof(pParkedOrder->UserID)-1);  ///用户代码
    //pInputOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	sprintf(pParkedOrder->OrderRef,"%12d",++m_CurOrderRef);

	///预埋单
	CThostFtdcParkedOrderField req={0};
	
	strncpy(req.BrokerID, pParkedOrder->BrokerID, sizeof(req.BrokerID));///经纪公司代码
	strncpy(req.InvestorID, pParkedOrder->InvestorID, sizeof(req.InvestorID));///投资者代码
	strncpy(req.InstrumentID, pParkedOrder->InstrumentID, sizeof(req.InstrumentID));///合约代码
	strncpy(req.OrderRef, pParkedOrder->OrderRef, sizeof(req.OrderRef));///报单引用
	strncpy(req.UserID, pParkedOrder->UserID, sizeof(req.UserID));///用户代码
	req.OrderPriceType = pParkedOrder->OrderPriceType;///报单价格条件
	req.Direction = pParkedOrder->Direction;///买卖方向
	strncpy(req.CombOffsetFlag, pParkedOrder->CombOffsetFlag, sizeof(req.CombOffsetFlag));///组合开平标志
	strncpy(req.CombHedgeFlag, pParkedOrder->CombHedgeFlag, sizeof(req.CombHedgeFlag));///组合投机套保标志
	req.LimitPrice = pParkedOrder->LimitPrice;///价格
	req.VolumeTotalOriginal = pParkedOrder->VolumeTotalOriginal;///数量
	req.TimeCondition = pParkedOrder->TimeCondition;///有效期类型
	strncpy(req.GTDDate, pParkedOrder->GTDDate, sizeof(req.GTDDate));///GTD日期
	req.VolumeCondition = pParkedOrder->VolumeCondition;///成交量类型
	req.MinVolume = pParkedOrder->MinVolume;///最小成交量
	req.ContingentCondition = pParkedOrder->ContingentCondition;///触发条件
	req.StopPrice = pParkedOrder->StopPrice;///止损价
	req.ForceCloseReason = pParkedOrder->ForceCloseReason;///强平原因
	req.IsAutoSuspend = pParkedOrder->IsAutoSuspend;///自动挂起标志
	strncpy(req.BusinessUnit, pParkedOrder->BusinessUnit, sizeof(req.BusinessUnit));///业务单元
	req.RequestID = pParkedOrder->RequestID;///请求编号
	req.UserForceClose = pParkedOrder->UserForceClose;///用户强评标志
	strncpy(req.ExchangeID, pParkedOrder->ExchangeID, sizeof(req.ExchangeID));///交易所代码
	strncpy(req.ParkedOrderID, pParkedOrder->ParkedOrderID, sizeof(req.ParkedOrderID));///预埋报单编号
	req.UserType = pParkedOrder->UserType;///用户类型
	req.Status = pParkedOrder->Status;///预埋单状态
	req.ErrorID = pParkedOrder->ErrorID;///错误代码
	strncpy(req.ErrorMsg, pParkedOrder->ErrorMsg, sizeof(req.ErrorMsg));///错误信息

    ret = m_pTradeApi->ReqParkedOrderInsert(&req, nRequestID);  
    LOG_INFO("ReqOrderInsert(预埋报单录入请求):ret=[%d],nRequestID=[%d]\n"
             "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             ret, nRequestID,
             pParkedOrder->BrokerID, pParkedOrder->InvestorID, pParkedOrder->InstrumentID, pParkedOrder->OrderRef,pParkedOrder->UserID,
             pParkedOrder->OrderPriceType, pParkedOrder->Direction, pParkedOrder->CombOffsetFlag, pParkedOrder->CombHedgeFlag,pParkedOrder->LimitPrice,
             pParkedOrder->VolumeTotalOriginal, pParkedOrder->TimeCondition, pParkedOrder->GTDDate, pParkedOrder->VolumeCondition,pParkedOrder->MinVolume,
             pParkedOrder->ContingentCondition, pParkedOrder->StopPrice, pParkedOrder->ForceCloseReason, pParkedOrder->IsAutoSuspend,pParkedOrder->BusinessUnit,
             pParkedOrder->RequestID, pParkedOrder->UserForceClose);
    return ret;
}


///预埋撤单录入请求
int CPlatFormService::ReqParkedOrderAction(PlatformStru_ParkedOrderAction *pParkedOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    pParkedOrderAction->Thost.RequestID = nRequestID;
    strncpy(pParkedOrderAction->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pParkedOrderAction->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pParkedOrderAction->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pParkedOrderAction->Thost.InvestorID)-1);  ///投资者代码
    pParkedOrderAction->Thost.ActionFlag = THOST_FTDC_AF_Delete;
    ret = m_pTradeApi->ReqParkedOrderAction((CThostFtdcParkedOrderActionField *)&pParkedOrderAction->Thost, nRequestID);  
    LOG_INFO("ReqParkedOrderAction(预埋撤单录入请求):ret=[%d],nRequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pParkedOrderAction->Thost.BrokerID, pParkedOrderAction->Thost.InvestorID, pParkedOrderAction->Thost.OrderActionRef, pParkedOrderAction->Thost.OrderRef,pParkedOrderAction->Thost.RequestID,
                     pParkedOrderAction->Thost.FrontID, pParkedOrderAction->Thost.SessionID, pParkedOrderAction->Thost.ExchangeID, pParkedOrderAction->Thost.OrderSysID,pParkedOrderAction->Thost.ActionFlag,
                     pParkedOrderAction->Thost.LimitPrice, pParkedOrderAction->Thost.VolumeChange, pParkedOrderAction->Thost.UserID, pParkedOrderAction->Thost.InstrumentID);
    return ret;
}

///报单操作请求
///
int CPlatFormService::ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    pInputOrderAction->Thost.RequestID = nRequestID;
    strncpy(pInputOrderAction->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pInputOrderAction->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pInputOrderAction->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pInputOrderAction->Thost.InvestorID)-1);  ///投资者代码
    pInputOrderAction->Thost.ActionFlag = THOST_FTDC_AF_Delete;
    ret = m_pTradeApi->ReqOrderAction((CThostFtdcInputOrderActionField *)&pInputOrderAction->Thost, nRequestID); 
    LOG_INFO("ReqOrderAction(报单操作请求):ret=[%d],nRequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pInputOrderAction->Thost.BrokerID, pInputOrderAction->Thost.InvestorID, pInputOrderAction->Thost.OrderActionRef, pInputOrderAction->Thost.OrderRef,pInputOrderAction->Thost.RequestID,
                     pInputOrderAction->Thost.FrontID, pInputOrderAction->Thost.SessionID, pInputOrderAction->Thost.ExchangeID, pInputOrderAction->Thost.OrderSysID,pInputOrderAction->Thost.ActionFlag,
                     pInputOrderAction->Thost.LimitPrice, pInputOrderAction->Thost.VolumeChange, pInputOrderAction->Thost.UserID, pInputOrderAction->Thost.InstrumentID);
    return ret;
}

///查询最大报单数量请求
int CPlatFormService::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume *pQueryMaxOrderVolume, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQueryMaxOrderVolume((CThostFtdcQueryMaxOrderVolumeField *)&pQueryMaxOrderVolume->Thost, nRequestID);  
    return ret;
}

///投资者结算结果确认
int CPlatFormService::ReqSettlementInfoConfirm()
{
    int ret = -999;
	CHECK_TRADE_STATUS();
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, m_PlatformParam.BrokerID.c_str());
	strcpy(req.InvestorID, m_PlatformParam.InvestorID.c_str());
	int nRequestID=m_PlatformParam.RequestID++;
    ret = m_pTradeApi->ReqSettlementInfoConfirm(&req, nRequestID);
    LOG_INFO("ReqSettlementInfoConfirm(投资者结算结果确认):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ConfirmDate=[%s],\t ConfirmTime=[%s]",
                     ret, nRequestID, 
                     req.BrokerID,
                     req.InvestorID,
                     req.ConfirmDate,
                     req.ConfirmTime);
    return ret;
}


///请求删除预埋单
int CPlatFormService::ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder *pRemoveParkedOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pRemoveParkedOrder->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pRemoveParkedOrder->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pRemoveParkedOrder->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pRemoveParkedOrder->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqRemoveParkedOrder((CThostFtdcRemoveParkedOrderField *)&pRemoveParkedOrder->Thost, nRequestID);  
    LOG_INFO("ReqRemoveParkedOrder(请求删除预埋单):ret=[%d],nRequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderID=[%s]",
                     ret, nRequestID,
                     pRemoveParkedOrder->Thost.BrokerID, pRemoveParkedOrder->Thost.InvestorID, pRemoveParkedOrder->Thost.ParkedOrderID);
    return ret;
}

///请求删除预埋撤单
int CPlatFormService::ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction *pRemoveParkedOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pRemoveParkedOrderAction->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pRemoveParkedOrderAction->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pRemoveParkedOrderAction->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pRemoveParkedOrderAction->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqRemoveParkedOrderAction((CThostFtdcRemoveParkedOrderActionField *)&pRemoveParkedOrderAction->Thost, nRequestID);  
    LOG_INFO("ReqRemoveParkedOrderAction(请求删除预埋撤单):ret=[%d],nRequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderActionID=[%s]",
                     ret, nRequestID,
                     pRemoveParkedOrderAction->Thost.BrokerID, pRemoveParkedOrderAction->Thost.InvestorID, pRemoveParkedOrderAction->Thost.ParkedOrderActionID);
    return ret;
}


///请求查询报单
int CPlatFormService::ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    m_pDataCenter->ClearMapQryRltOrders();
    ret = m_pTradeApi->ReqQryOrder((CThostFtdcQryOrderField *)&pQryOrder->Thost, nRequestID);


    LOG_INFO("ReqQryOrder(请求查询报单):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t OrderSysID=[%s],\n"
                     "\t\t\t InsertTimeStart=[%s],\t InsertTimeEnd=[%s]",
                     ret, nRequestID,
                     pQryOrder->Thost.BrokerID,
                     pQryOrder->Thost.InvestorID,
                     pQryOrder->Thost.InstrumentID,
                     pQryOrder->Thost.ExchangeID,
                     pQryOrder->Thost.OrderSysID,
                     pQryOrder->Thost.InsertTimeStart,
                     pQryOrder->Thost.InsertTimeEnd);

    return ret;
}

///请求查询成交
int CPlatFormService::ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();
	m_pDataCenter->ClearMapQryRltTrades();
    ret = m_pTradeApi->ReqQryTrade((CThostFtdcQryTradeField *)&pQryTrade->Thost, nRequestID);

    LOG_INFO("ReqQryTrade(请求查询成交):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t TradeID=[%s],\n"
                     "\t\t\t TradeTimeStart=[%s],\t TradeTimeEnd=[%s]",
                     ret, nRequestID,
                     pQryTrade->Thost.BrokerID,
                     pQryTrade->Thost.InvestorID,
                     pQryTrade->Thost.InstrumentID,
                     pQryTrade->Thost.ExchangeID,
                     pQryTrade->Thost.TradeID,
                     pQryTrade->Thost.TradeTimeStart,
                     pQryTrade->Thost.TradeTimeEnd);
    return ret;
}

///请求查询投资者持仓
int CPlatFormService::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

	PlatformStru_QryInvestorPosition field;
	memset( &field, 0, sizeof(field));
	strcpy_s(field.BrokerID, pQryInvestorPosition->BrokerID);
	strcpy_s(field.InstrumentID, pQryInvestorPosition->InstrumentID);
	strcpy_s(field.InvestorID, pQryInvestorPosition->InvestorID);
    m_pDataCenter->ClearMapQryRltPositions(std::string(pQryInvestorPosition->InstrumentID));
	ret = m_pTradeApi->ReqQryInvestorPosition(&field, nRequestID);  

    LOG_INFO("ReqQryInvestorPosition(请求查询投资者持仓):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pQryInvestorPosition->BrokerID,
                     pQryInvestorPosition->InvestorID,
                     pQryInvestorPosition->InstrumentID);
    return ret;
}

///请求查询资金账户
int CPlatFormService::ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    CThostFtdcQryTradingAccountField QryTradingAccount;
    memset(&QryTradingAccount,0,sizeof(QryTradingAccount));
    strncpy(QryTradingAccount.BrokerID,pQryTradingAccount->BrokerID,sizeof(QryTradingAccount.BrokerID)-1);
    strncpy(QryTradingAccount.InvestorID,pQryTradingAccount->InvestorID,sizeof(QryTradingAccount.InvestorID)-1);

    ret = m_pTradeApi->ReqQryTradingAccount(&QryTradingAccount, nRequestID);  
    LOG_INFO("ReqQryTradingAccount(请求查询资金账户):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     ret, nRequestID,
                     pQryTradingAccount->BrokerID,
                     pQryTradingAccount->InvestorID);
    return ret;
}

///请求查询投资者
int CPlatFormService::ReqQryInvestor(PlatformStru_QryInvestor *pQryInvestor, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryInvestor->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryInvestor->Thost.BrokerID)-1); ///经纪公司代码    
    strncpy(pQryInvestor->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryInvestor->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQryInvestor((CThostFtdcQryInvestorField *)&pQryInvestor->Thost, nRequestID);  
    LOG_INFO("ReqQryInvestor(请求查询投资者):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     ret, nRequestID,
                     pQryInvestor->Thost.BrokerID,
                     pQryInvestor->Thost.InvestorID);
    return ret;
}

///请求查询交易编码
int CPlatFormService::ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryTradingCode(pQryTradingCode, nRequestID);  
    return ret;
}

///请求查询合约保证金率
int CPlatFormService::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryInstrumentMarginRate->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryInstrumentMarginRate->BrokerID)-1); ///经纪公司代码    
	strncpy(pQryInstrumentMarginRate->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryInstrumentMarginRate->InvestorID)-1);  ///投资者代码
    if(pQryInstrumentMarginRate->HedgeFlag==0) pQryInstrumentMarginRate->HedgeFlag=THOST_FTDC_HF_Speculation;
    ret = m_pTradeApi->ReqQryInstrumentMarginRate(pQryInstrumentMarginRate, nRequestID);  
    LOG_INFO("ReqQryInstrumentMarginRate(请求查询合约保证金率):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d]",
                     ret, nRequestID,
                     pQryInstrumentMarginRate->BrokerID,
                     pQryInstrumentMarginRate->InvestorID,
                     pQryInstrumentMarginRate->InstrumentID,
                     pQryInstrumentMarginRate->HedgeFlag);
    return ret;
}

///请求查询合约手续费率
int CPlatFormService::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryInstrumentCommissionRate->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryInstrumentCommissionRate->BrokerID)-1); ///经纪公司代码    
	strncpy(pQryInstrumentCommissionRate->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryInstrumentCommissionRate->InvestorID)-1);  ///投资者代码

    ret = m_pTradeApi->ReqQryInstrumentCommissionRate(pQryInstrumentCommissionRate, nRequestID);  
    LOG_INFO("ReqQryInstrumentCommissionRate(请求查询合约手续费率):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pQryInstrumentCommissionRate->BrokerID,
                     pQryInstrumentCommissionRate->InvestorID,
                     pQryInstrumentCommissionRate->InstrumentID);
    return ret;
}

///请求查询交易所
int CPlatFormService::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryExchange(pQryExchange, nRequestID);  
    return ret;
}

///请求查询合约
int CPlatFormService::ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryInstrument((CThostFtdcQryInstrumentField *)&pQryInstrument->Thost, nRequestID); 
    LOG_INFO("ReqQryInstrument(请求查询合约):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExchangeInstID=[%s],\tProductID=[%s]",
                     ret, nRequestID, 
                     pQryInstrument->Thost.InstrumentID,
                     pQryInstrument->Thost.ExchangeID,
                     pQryInstrument->Thost.ExchangeInstID,
                     pQryInstrument->Thost.ProductID);

    return ret;
}

///请求查询行情
int CPlatFormService::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryDepthMarketData((CThostFtdcQryDepthMarketDataField *)&pQryDepthMarketData->Thost, nRequestID);  
    return ret;
}

///请求查询投资者结算结果
int CPlatFormService::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo *pQrySettlementInfo, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQrySettlementInfo->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQrySettlementInfo->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pQrySettlementInfo->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQrySettlementInfo->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQrySettlementInfo((CThostFtdcQrySettlementInfoField *)&pQrySettlementInfo->Thost, nRequestID); 
    LOG_INFO("ReqQrySettlementInfo(请求查询投资者结算结果):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t TradingDay=[%s]",
                     ret, nRequestID, 
                     pQrySettlementInfo->Thost.BrokerID,
                     pQrySettlementInfo->Thost.InvestorID,
                     pQrySettlementInfo->Thost.TradingDay);
    return ret;
}

///请求查询保证金监管系统经纪公司资金账户密钥
int CPlatFormService::ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey *pQryCFMMCTradingAccountKey, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    memset(pQryCFMMCTradingAccountKey,0,sizeof(PlatformStru_QryCFMMCTradingAccountKey));
    strncpy(pQryCFMMCTradingAccountKey->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryCFMMCTradingAccountKey->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pQryCFMMCTradingAccountKey->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryCFMMCTradingAccountKey->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQryCFMMCTradingAccountKey((CThostFtdcQryCFMMCTradingAccountKeyField *)&pQryCFMMCTradingAccountKey->Thost, nRequestID); 
    LOG_INFO("ReqQryCFMMCTradingAccountKey(请求查询保证金监管系统经纪公司资金账户密钥):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     ret, nRequestID, 
                     pQryCFMMCTradingAccountKey->Thost.BrokerID,
                     pQryCFMMCTradingAccountKey->Thost.InvestorID);
    return ret;
}

///请求查询转帐银行
int CPlatFormService::ReqQryTransferBank(PlatformStru_QryTransferBank *pQryTransferBank, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryTransferBank((CThostFtdcQryTransferBankField *)&pQryTransferBank->Thost, nRequestID);  
    return ret;
}

///请求查询投资者持仓明细
int CPlatFormService::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    CThostFtdcQryInvestorPositionDetailField QryInvestorPositionDetail;
    memset(&QryInvestorPositionDetail,0,sizeof(QryInvestorPositionDetail));
    strncpy(QryInvestorPositionDetail.BrokerID,pQryInvestorPositionDetail->BrokerID,sizeof(QryInvestorPositionDetail.BrokerID)-1);
    strncpy(QryInvestorPositionDetail.InvestorID,pQryInvestorPositionDetail->InvestorID,sizeof(QryInvestorPositionDetail.InvestorID)-1);
    strncpy(QryInvestorPositionDetail.InstrumentID,pQryInvestorPositionDetail->InstrumentID,sizeof(QryInvestorPositionDetail.InstrumentID)-1);
    m_pDataCenter->ClearMapQryRltPositionDetails(std::string(pQryInvestorPositionDetail->InstrumentID));
    ret = m_pTradeApi->ReqQryInvestorPositionDetail(&QryInvestorPositionDetail, nRequestID);  
    LOG_INFO("ReqQryInvestorPositionDetail(请求查询投资者持仓明细):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pQryInvestorPositionDetail->BrokerID,
                     pQryInvestorPositionDetail->InvestorID,
                     pQryInvestorPositionDetail->InstrumentID);
    return ret;
}

///请求查询客户通知
int CPlatFormService::ReqQryNotice(PlatformStru_QryNotice *pQryNotice, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryNotice((CThostFtdcQryNoticeField *)&pQryNotice->Thost, nRequestID);  
    return ret;
}

///请求查询结算信息确认
int CPlatFormService::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm *pQrySettlementInfoConfirm, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQrySettlementInfoConfirm((CThostFtdcQrySettlementInfoConfirmField *)&pQrySettlementInfoConfirm->Thost, nRequestID);  
    LOG_INFO("ReqQrySettlementInfoConfirm(请求查询结算信息确认):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     ret, nRequestID,
                     pQrySettlementInfoConfirm->Thost.BrokerID,
                     pQrySettlementInfoConfirm->Thost.InvestorID);
    return ret;
}

///请求查询投资者持仓明细
int CPlatFormService::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    CThostFtdcQryInvestorPositionCombineDetailField QryInvestorPositionCombineDetail;
    memset(&QryInvestorPositionCombineDetail,0,sizeof(QryInvestorPositionCombineDetail));
    strncpy(QryInvestorPositionCombineDetail.BrokerID,pQryInvestorPositionCombineDetail->BrokerID,sizeof(QryInvestorPositionCombineDetail.BrokerID)-1);
    strncpy(QryInvestorPositionCombineDetail.InvestorID,pQryInvestorPositionCombineDetail->InvestorID,sizeof(QryInvestorPositionCombineDetail.InvestorID)-1);
    strncpy(QryInvestorPositionCombineDetail.CombInstrumentID,pQryInvestorPositionCombineDetail->CombInstrumentID,sizeof(QryInvestorPositionCombineDetail.CombInstrumentID)-1);
	
	m_pDataCenter->ClearMapQryRltPositionCombs(std::string(pQryInvestorPositionCombineDetail->CombInstrumentID));
    ret = m_pTradeApi->ReqQryInvestorPositionCombineDetail(&QryInvestorPositionCombineDetail, nRequestID); 
    LOG_INFO("ReqQryInvestorPositionCombineDetail(请求查询投资者组合持仓明细):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t CombInstrumentID=[%s]",
                     ret, nRequestID,
                     pQryInvestorPositionCombineDetail->BrokerID,
                     pQryInvestorPositionCombineDetail->InvestorID,
                     pQryInvestorPositionCombineDetail->CombInstrumentID);
    return ret;
}

///请求查询转帐流水
int CPlatFormService::ReqQryTransferSerial(PlatformStru_QryTransferSerial *pQryTransferSerial, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryTransferSerial->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryTransferSerial->Thost.BrokerID)-1); ///经纪公司代码    
    strncpy(pQryTransferSerial->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryTransferSerial->Thost.AccountID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQryTransferSerial((CThostFtdcQryTransferSerialField *)&pQryTransferSerial->Thost, nRequestID); 
    LOG_INFO("ReqQryTransferSerial(请求查询转帐流水) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
                        ret,nRequestID,
                        pQryTransferSerial->Thost.BrokerID,
                        pQryTransferSerial->Thost.AccountID,
						pQryTransferSerial->Thost.BankID);

    return ret;
}

///请求查询签约银行
int CPlatFormService::ReqQryContractBank(PlatformStru_QryContractBank *pQryContractBank, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryContractBank->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryContractBank->Thost.BrokerID)-1); ///经纪公司代码    

    ret = m_pTradeApi->ReqQryContractBank((CThostFtdcQryContractBankField *)&pQryContractBank->Thost, nRequestID);  
    LOG_INFO("ReqQryContractBank(请求查询签约银行) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t BankID=[%s],\t BankBrchID=[%s]",
                        ret,nRequestID,
                        pQryContractBank->Thost.BrokerID,
                        pQryContractBank->Thost.BankID,
						pQryContractBank->Thost.BankBrchID);
    return ret;
}
    
///请求查询银期签约关系
int CPlatFormService::ReqQryAccountregister(PlatformStru_QryAccountRegister *pQryAccountregister, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryAccountregister->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryAccountregister->Thost.BrokerID)-1); ///经纪公司代码    
    strncpy(pQryAccountregister->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryAccountregister->Thost.AccountID)-1);  ///投资者代码

    ret = m_pTradeApi->ReqQryAccountregister((CThostFtdcQryAccountregisterField *)&pQryAccountregister->Thost, nRequestID);  
    LOG_INFO("ReqQryAccountregister(请求查询银期签约关系) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
                        ret,nRequestID,
                        pQryAccountregister->Thost.BrokerID,
                        pQryAccountregister->Thost.AccountID,
						pQryAccountregister->Thost.BankID);
    return ret;
}

///请求查询预埋单
int CPlatFormService::ReqQryParkedOrder(PlatformStru_QryParkedOrder *pQryParkedOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryParkedOrder->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryParkedOrder->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pQryParkedOrder->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryParkedOrder->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQryParkedOrder((CThostFtdcQryParkedOrderField *)&pQryParkedOrder->Thost, nRequestID);  
    LOG_INFO("ReqQryParkedOrder(请求查询预埋单):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s]",
                     ret, nRequestID,
                     pQryParkedOrder->Thost.BrokerID,
                     pQryParkedOrder->Thost.InvestorID,
                     pQryParkedOrder->Thost.InstrumentID,
                     pQryParkedOrder->Thost.ExchangeID);
    return ret;
}

///请求查询预埋撤单
int CPlatFormService::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction *pQryParkedOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryParkedOrderAction->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryParkedOrderAction->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pQryParkedOrderAction->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryParkedOrderAction->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQryParkedOrderAction((CThostFtdcQryParkedOrderActionField *)&pQryParkedOrderAction->Thost, nRequestID);  
    LOG_INFO("ReqQryParkedOrderAction(请求查询预埋撤单):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s]",
                     ret, nRequestID,
                     pQryParkedOrderAction->Thost.BrokerID,
                     pQryParkedOrderAction->Thost.InvestorID,
                     pQryParkedOrderAction->Thost.InstrumentID,
                     pQryParkedOrderAction->Thost.ExchangeID);
    return ret;
}

///请求查询交易通知
int CPlatFormService::ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryTradingNotice(pQryTradingNotice, nRequestID);  
    return ret;
}

///请求查询经纪公司交易参数
int CPlatFormService::ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryBrokerTradingParams(pQryBrokerTradingParams, nRequestID);  
    return ret;
}

///请求查询经纪公司交易算法
int CPlatFormService::ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryBrokerTradingAlgos(pQryBrokerTradingAlgos, nRequestID);  
    return ret;
}

///期货发起银行资金转期货请求
int CPlatFormService::ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pReqTransfer->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pReqTransfer->Thost.BrokerID)-1); ///经纪公司代码    
    strncpy(pReqTransfer->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pReqTransfer->Thost.AccountID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqFromBankToFutureByFuture((CThostFtdcReqTransferField*)&pReqTransfer->Thost, nRequestID);  
    //LOG_INFO("ReqFromBankToFutureByFuture(期货发起银行资金转期货请求) : ret=[%d],nRequestID=[%d],\n"
    //                    "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
    //                    "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
    //                    "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
    //                    "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
    //                    "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
    //                    "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
    //                    "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
    //                    "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
    //                    "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
    //                    ret,nRequestID,
    //                    pReqTransfer->Thost.TradeCode, pReqTransfer->Thost.BankID,pReqTransfer->Thost.BankBranchID,pReqTransfer->Thost.BrokerID,pReqTransfer->Thost.BrokerBranchID,
    //                    pReqTransfer->Thost.TradeDate, pReqTransfer->Thost.TradeTime,pReqTransfer->Thost.BankSerial,pReqTransfer->Thost.TradingDay,pReqTransfer->Thost.PlateSerial,
    //                    pReqTransfer->Thost.LastFragment, pReqTransfer->Thost.SessionID,pReqTransfer->Thost.CustomerName,pReqTransfer->Thost.IdCardType,pReqTransfer->Thost.IdentifiedCardNo,
    //                    pReqTransfer->Thost.CustType, pReqTransfer->Thost.BankAccount,pReqTransfer->Thost.BankPassWord,pReqTransfer->Thost.AccountID,pReqTransfer->Thost.Password,
    //                    pReqTransfer->Thost.InstallID,pReqTransfer->Thost.FutureSerial, pReqTransfer->Thost.UserID,pReqTransfer->Thost.VerifyCertNoFlag,pReqTransfer->Thost.CurrencyID,
    //                    pReqTransfer->Thost.TradeAmount,pReqTransfer->Thost.FutureFetchAmount, pReqTransfer->Thost.FeePayFlag,pReqTransfer->Thost.CustFee,pReqTransfer->Thost.BrokerFee,
    //                    pReqTransfer->Thost.Message, pReqTransfer->Thost.Digest, pReqTransfer->Thost.BankAccType,pReqTransfer->Thost.DeviceID,pReqTransfer->Thost.BankSecuAccType,
    //                    pReqTransfer->Thost.BrokerIDByBank, pReqTransfer->Thost.BankSecuAcc, pReqTransfer->Thost.BankPwdFlag,pReqTransfer->Thost.SecuPwdFlag,pReqTransfer->Thost.OperNo,
    //                    pReqTransfer->Thost.RequestID, pReqTransfer->Thost.TID, pReqTransfer->Thost.TransferStatus
    //                    );
    return ret;
}

///期货发起期货资金转银行请求
int CPlatFormService::ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pReqTransfer->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pReqTransfer->Thost.BrokerID)-1); ///经纪公司代码    
    strncpy(pReqTransfer->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pReqTransfer->Thost.AccountID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqFromFutureToBankByFuture((CThostFtdcReqTransferField *)&pReqTransfer->Thost, nRequestID);  
    //LOG_INFO("ReqFromFutureToBankByFuture(期货发起期货资金转银行请求) : ret=[%d],nRequestID=[%d],\n"
    //                    "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
    //                    "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
    //                    "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
    //                    "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
    //                    "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
    //                    "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
    //                    "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
    //                    "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
    //                    "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
    //                    ret,nRequestID,
    //                    pReqTransfer->Thost.TradeCode, pReqTransfer->Thost.BankID,pReqTransfer->Thost.BankBranchID,pReqTransfer->Thost.BrokerID,pReqTransfer->Thost.BrokerBranchID,
    //                    pReqTransfer->Thost.TradeDate, pReqTransfer->Thost.TradeTime,pReqTransfer->Thost.BankSerial,pReqTransfer->Thost.TradingDay,pReqTransfer->Thost.PlateSerial,
    //                    pReqTransfer->Thost.LastFragment, pReqTransfer->Thost.SessionID,pReqTransfer->Thost.CustomerName,pReqTransfer->Thost.IdCardType,pReqTransfer->Thost.IdentifiedCardNo,
    //                    pReqTransfer->Thost.CustType, pReqTransfer->Thost.BankAccount,pReqTransfer->Thost.BankPassWord,pReqTransfer->Thost.AccountID,pReqTransfer->Thost.Password,
    //                    pReqTransfer->Thost.InstallID,pReqTransfer->Thost.FutureSerial, pReqTransfer->Thost.UserID,pReqTransfer->Thost.VerifyCertNoFlag,pReqTransfer->Thost.CurrencyID,
    //                    pReqTransfer->Thost.TradeAmount,pReqTransfer->Thost.FutureFetchAmount, pReqTransfer->Thost.FeePayFlag,pReqTransfer->Thost.CustFee,pReqTransfer->Thost.BrokerFee,
    //                    pReqTransfer->Thost.Message, pReqTransfer->Thost.Digest, pReqTransfer->Thost.BankAccType,pReqTransfer->Thost.DeviceID,pReqTransfer->Thost.BankSecuAccType,
    //                    pReqTransfer->Thost.BrokerIDByBank, pReqTransfer->Thost.BankSecuAcc, pReqTransfer->Thost.BankPwdFlag,pReqTransfer->Thost.SecuPwdFlag,pReqTransfer->Thost.OperNo,
    //                    pReqTransfer->Thost.RequestID, pReqTransfer->Thost.TID, pReqTransfer->Thost.TransferStatus
    //                    );
    return ret;
}

///期货发起查询银行余额请求
int CPlatFormService::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount *pReqQueryAccount, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pReqQueryAccount->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pReqQueryAccount->Thost.BrokerID)-1); ///经纪公司代码    
    strncpy(pReqQueryAccount->Thost.AccountID, m_PlatformParam.InvestorID.c_str(), sizeof(pReqQueryAccount->Thost.AccountID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQueryBankAccountMoneyByFuture((CThostFtdcReqQueryAccountField *)&pReqQueryAccount->Thost, nRequestID); 
    //LOG_INFO("ReqQueryBankAccountMoneyByFuture(期货发起查询银行余额请求) : ret=[%d],nRequestID=[%d],\n"
    //                    "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
    //                    "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
    //                    "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
    //                    "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
    //                    "\t\t\t FutureSerial=[%d],\t InstallID=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
    //                    "\t\t\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],BrokerIDByBank=[%s],\n"
    //                    "\t\t\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],RequestID=[%d],\n"
    //                    "\t\t\t TID=[%d]",
    //                    ret,nRequestID,
    //                    pReqQueryAccount->Thost.TradeCode, pReqQueryAccount->Thost.BankID,pReqQueryAccount->Thost.BankBranchID,pReqQueryAccount->Thost.BrokerID,pReqQueryAccount->Thost.BrokerBranchID,
    //                    pReqQueryAccount->Thost.TradeDate, pReqQueryAccount->Thost.TradeTime,pReqQueryAccount->Thost.BankSerial,pReqQueryAccount->Thost.TradingDay,pReqQueryAccount->Thost.PlateSerial,
    //                    pReqQueryAccount->Thost.LastFragment, pReqQueryAccount->Thost.SessionID,pReqQueryAccount->Thost.CustomerName,pReqQueryAccount->Thost.IdCardType,pReqQueryAccount->Thost.IdentifiedCardNo,
    //                    pReqQueryAccount->Thost.CustType, pReqQueryAccount->Thost.BankAccount,pReqQueryAccount->Thost.BankPassWord,pReqQueryAccount->Thost.AccountID,pReqQueryAccount->Thost.Password,
    //                    pReqQueryAccount->Thost.FutureSerial, pReqQueryAccount->Thost.InstallID,pReqQueryAccount->Thost.UserID,pReqQueryAccount->Thost.VerifyCertNoFlag,pReqQueryAccount->Thost.CurrencyID,
    //                    pReqQueryAccount->Thost.Digest, pReqQueryAccount->Thost.BankAccType,pReqQueryAccount->Thost.DeviceID,pReqQueryAccount->Thost.BankSecuAccType,pReqQueryAccount->Thost.BrokerIDByBank,
    //                    pReqQueryAccount->Thost.BankSecuAcc, pReqQueryAccount->Thost.BankPwdFlag,pReqQueryAccount->Thost.SecuPwdFlag,pReqQueryAccount->Thost.OperNo,pReqQueryAccount->Thost.RequestID,
    //                    pReqQueryAccount->Thost.TID
    //                    );
    return ret;
}

int CPlatFormService::SubscribeMarketData(const std::string& InstrumentID)
{
    int ret = -999;
    if(InstrumentID.empty() ||
       !m_pQuotApi          ||
       m_PlatformParam.QuotStatus!=CPlatFormParam::CONNSTATUS_Connected)
    {
        LOG_INFO("行情API还未准备就绪，可能登入未成功或正在重连");
        return ret;
    }

    bool bsub=false;

    EnterCriticalSection(&m_CS);
    if(m_MarketDataInstrumentID.find(InstrumentID)==m_MarketDataInstrumentID.end())
    {
        bsub=true;
        m_MarketDataInstrumentID.insert(InstrumentID);
    }
    LeaveCriticalSection(&m_CS);

    if(bsub)
    {
        char *pInstrumentID = const_cast<char*>(InstrumentID.c_str());
        ret=m_pQuotApi->SubscribeMarketData(&pInstrumentID, 1);
        LOG_INFO("SubscribeMarketData(定制指定合约行情):ret=[%d],InstrumentID=[%s]",
                         ret, pInstrumentID);
    }

    return ret;

}

int CPlatFormService::UnSubscribeMarketData(const std::string& InstrumentID)
{
    int ret = -999;
    if(InstrumentID.empty() ||
       !m_pQuotApi          ||
       m_PlatformParam.QuotStatus!=CPlatFormParam::CONNSTATUS_Connected)
    {
        LOG_INFO("行情API还未准备就绪，可能登入未成功或正在重连");
        return ret;
    }

    bool bunsub=false;
    EnterCriticalSection(&m_CS);
    std::set<std::string>::iterator iter=m_MarketDataInstrumentID.find(InstrumentID);
    if(iter!=m_MarketDataInstrumentID.end()) 
    {
        bunsub=true;
        m_MarketDataInstrumentID.erase(iter);
    }
    LeaveCriticalSection(&m_CS);

    if(bunsub)
    {
        char *pInstrumentID = const_cast<char*>(InstrumentID.c_str());
        ret=m_pQuotApi->UnSubscribeMarketData(&pInstrumentID, 1);
        LOG_INFO("UnSubscribeMarketData(取消指定合约行情):ret=[%d],InstrumentID=[%s]",
                     ret, pInstrumentID);
    }
    return ret;
}

void CPlatFormService::GetMarketDataInstrumentID(std::set<std::string>& setMarketDataInstrumentID)
{
    EnterCriticalSection(&m_CS);
    setMarketDataInstrumentID=m_MarketDataInstrumentID;
    LeaveCriticalSection(&m_CS);
} 

//admin  API
int CPlatFormService::LoginTrade(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd)
{
    LOG_INFO("LoginTrade:InvestorID=[%s],InvestorPW=[******]", InvestorID.c_str());
    if(InvestorID.empty() || 
       /*InvestorPW.empty() ||*/
       m_PlatformParam.TradeFrontAddr.empty() ||
       m_PlatformParam.BrokerID.empty() ||
       m_PlatformParam.UserID.empty()/* ||
       m_PlatformParam.UserPW.empty()*/)
    {
        LOG_INFO("LoginTrade:参数未配置完整");
        return -1;
    }

    if(CPlatFormParam::CONNSTATUS_Connected==m_PlatformParam.TradeStatus)
    {
        LOG_INFO("LoginTrade:已经登入");
        return 0;//已经登入
    }
    if(CPlatFormParam::CONNSTATUS_Logining==m_PlatformParam.TradeStatus ||
       CPlatFormParam::CONNSTATUS_Connecting==m_PlatformParam.TradeStatus 
       ||CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirming==m_PlatformParam.TradeStatus)
    {
        LOG_INFO("LoginTrade:正在登入中");
        return -2;//正在登入中
    }

    LogoutTrade();

    m_PlatformParam.TradeStatus = CPlatFormParam::CONNSTATUS_Connecting;//正在连接
    m_PlatformParam.InvestorID = InvestorID;
    m_PlatformParam.InvestorPW = InvestorPW;
	m_PlatformParam.m_strInvestorNewPWD = InvestorNewPwd;
    m_PlatformParam.QuotStatus = CPlatFormParam::CONNSTATUS_Connecting;//正在连接
	m_pTradeApi = CPatsystemApi::CreateFtdcApi(GetServerParamCfg().VersionStr.c_str()); 
	m_pQuotApi = m_pTradeApi;    m_pTradeSpi = new CTraderSpi(*this);
    m_pTradeApi->RegisterSpi(m_pTradeSpi);
    m_pQuotSpi = new CQuotSpi(*this);
    m_pQuotApi->RegisterSpi(m_pQuotSpi);   
	char FrontAddr[256];
	strcpy(FrontAddr,m_PlatformParam.TradeFrontAddr.c_str());
	strcat(FrontAddr,";");
	strcat(FrontAddr,m_PlatformParam.QuotFrontAddr.c_str());
	m_pTradeApi->SetNewPwd(InvestorNewPwd);
	m_pTradeApi->RegisterFront(FrontAddr);
	m_pTradeApi->Init();
  
    return 0;
}

int CPlatFormService::LoginQuot(const std::string& InvestorID, const std::string& InvestorPW)
{
    LOG_INFO("LoginQuot:InvestorID=[%s],InvestorPW=[%s]", InvestorID.c_str(), InvestorPW.c_str());
	//PATS无需登入行情
    return 0;
}

void CPlatFormService::StopThreads(void)
{
    if(m_pDataCenter)
        m_pDataCenter->Stop();
}

int CPlatFormService::LogoutTrade()
{
    //LOG_INFO("LogoutTrade");
    if(m_pTradeApi)
    {
        LOG_INFO("LogoutTrade:关闭交易API");
        m_PlatformParam.TradeStatus = CPlatFormParam::CONNSTATUS_Disconnecting;
        m_pTradeApi->Release();
		CPatsystemApi* tempApi=m_pTradeApi;
		m_pTradeApi=NULL;
		m_pQuotApi=NULL;		
		delete tempApi;
		tempApi=NULL;
        m_PlatformParam.TradeStatus = CPlatFormParam::CONNSTATUS_Disconnected;
    }

    if(m_pTradeSpi)
    {
        delete m_pTradeSpi;
        m_pTradeSpi=NULL;
        m_pQuotSpi=NULL;
    }
    
    return 0;
}

int CPlatFormService::LogoutQuot()
{
    LOG_INFO("LogoutQuot");
	//PATS无需登出行情
    return 0;
}

///密码是否是当前密码
int CPlatFormService::IsValidPW(const std::string& yourPW)
{
	if(m_PlatformParam.InvestorPW==yourPW)
		return 1;
	else
	    return -1;
}
int CPlatFormService::GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts)
{
	BrokerID=m_PlatformParam.BrokerID;
	UserID=m_PlatformParam.UserID;
	if(accounts) 
	{		
		*accounts = m_PlatformParam.Accounts;
	}	
	return 0;

}
int CPlatFormService::SetTradeFrontAddr(const std::string& addr)
{
    m_PlatformParam.TradeFrontAddr = addr;
	if(m_PlatformParam.TradeFrontAddr.find("tcp://")==0)
		m_PlatformParam.TradeFrontAddr=m_PlatformParam.TradeFrontAddr.substr(6);
    return 0;
}

int CPlatFormService::SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)
{
    m_PlatformParam.TradeFrontAddrs=TradeAddr;
    return 0;
}


int CPlatFormService::SetQuotFrontAddr(const std::string& addr)
{
    m_PlatformParam.QuotFrontAddr = addr;
	if(m_PlatformParam.QuotFrontAddr.find("tcp://")==0)
		m_PlatformParam.QuotFrontAddr=m_PlatformParam.QuotFrontAddr.substr(6);
    return 0;
}

int CPlatFormService::SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)
{
    m_PlatformParam.QuotFrontAddrs=QuotAddr;
    return 0;
}

int CPlatFormService::SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)
{
    m_PlatformParam.BrokerID = BrokerID;
    m_PlatformParam.UserID   = UserID;
    m_PlatformParam.UserPW   = UserPW;
	m_PlatformParam.OneTimePassword=OneTimePassword;
    return 0;
}


CPlatFormService::CPlatFormService()
{
    //m_status = PLATFORM_STATUS_UNINIT;

    InitializeCriticalSection(&m_CS);


    m_pWriteLog = new zqWriteLog();
    m_PCache   = new CPackageCache(40*1024*1024, 1);
    m_pDataCenter = new CDataCenter(*m_PCache,this);
    m_pDataCenter->Start();
    m_pTradeApi=NULL;
    m_pTradeSpi=NULL;
    m_pQuotApi=NULL;
    m_pQuotSpi=NULL;
	m_bSettlementInfoConfirmed=false;
	memset(&m_ErrorInfoField,0,sizeof(m_ErrorInfoField));

	m_nOrderReqID = 0;
}

CPlatFormService::~CPlatFormService()
{
    LogoutTrade();
    LogoutQuot();


	SAFE_DELETE(m_pDataCenter); 
    SAFE_DELETE(m_PCache);
    SAFE_DELETE(m_pWriteLog);

    DeleteCriticalSection(&m_CS);


}
   
///用户登录请求
int CPlatFormService::ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqUserLogin(pReqUserLoginField, nRequestID);  
    LOG_INFO("ReqUserLogin(用户登录请求):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t TradingDay=[%s],\t BrokerID=[%s],\t UserID=[%s],\t Password=[******],\t UserProductInfo=[%s],\n"
                     "\t\t\t InterfaceProductInfo=[%s],\t ProtocolInfo=[%s]",
                     ret, nRequestID,
                     pReqUserLoginField->TradingDay,
                     pReqUserLoginField->BrokerID,
                     pReqUserLoginField->UserID,
                     /*pReqUserLoginField->Password,*/
                     pReqUserLoginField->UserProductInfo,
                     pReqUserLoginField->InterfaceProductInfo,
                     pReqUserLoginField->ProtocolInfo);
    return ret;
}	

///登出请求
int CPlatFormService::ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqUserLogout(pUserLogout, nRequestID);
    LOG_INFO("ReqUserLogout(登出请求):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],UserID=[%s]",
                     ret, nRequestID,
                     pUserLogout->BrokerID,
                     pUserLogout->UserID);
    return ret;
}


///定制业务数据
int CPlatFormService::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
    return m_pDataCenter->SubscribeBusinessData(BID, GID, callback, sendCurrentInfo);
}

///取消业务数据的定制
int CPlatFormService::UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)
{
    return m_pDataCenter->UnSubscribeBusinessData(BID, GID);
}
///获取合约列表
int CPlatFormService::GetInstrumentList(std::vector<GroupInfo> &outData)
{
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        return -1;

    return m_pDataCenter->GetInstrumentList(outData);
}
///获取合约列表
int CPlatFormService::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        return -1;

    return m_pDataCenter->GetInstrumentList(outData,ExchangeID);
}
//添加主力合约列表
int CPlatFormService::AddMainInstrumentList(std::string instrument)
{
	return m_pDataCenter->AddMainInstrumentList(instrument);
}
///获取主力合约列表
int CPlatFormService::GetMainInstrumentList(std::set<std::string> &outData)
{
    //if(!m_PlatformParam.HaveAllInstrumentIDs)
    //    return -1;

    return m_pDataCenter->GetMainInstrumentList(outData);
}
///设置合约容差列表
void CPlatFormService::SetInstrumentVarietyMap(map<string, string>& inData)
{
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        return;
    m_pDataCenter->SetInstrumentVarietyMap(inData);
}

// 获取合约容差列表
int CPlatFormService::GetInstrumentVarietyMap(map<string, string>& outData)
{
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        return -1;
    return m_pDataCenter->GetInstrumentVarietyMap(outData);
}
///获取指定品种的合约列表,返回合约个数
int CPlatFormService::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
	return m_pDataCenter->GetInstrumentListByProductID(ProductID,outData);
}
///获取指定合约信息
int CPlatFormService::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    return m_pDataCenter->GetInstrumentInfo(InstrumentID, outData);
}
//设置合约订阅状态
void CPlatFormService::SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper)
{
    int oldstatus,newstatus;

    EnterCriticalSection(&m_CS);

    if(m_SubscribeMap.find(InstrumentID)!=m_SubscribeMap.end()) 
        oldstatus=m_SubscribeMap[InstrumentID];
    else oldstatus=0;

	switch(oper)
	{
	    case S_STATUS_ADD:
		    m_SubscribeMap[InstrumentID]|=status;
		    break;
	    case S_STATUS_DEL:
		    m_SubscribeMap[InstrumentID]&=~status;
		    break;
	    case S_STATUS_EQU:
		    m_SubscribeMap[InstrumentID]=status;
		    break;
	}

    if(m_SubscribeMap.find(InstrumentID)!=m_SubscribeMap.end()) 
        newstatus=m_SubscribeMap[InstrumentID];
    else newstatus=0;

    LeaveCriticalSection(&m_CS);


    if(oldstatus==0&&newstatus!=0)
        SubscribeMarketData(InstrumentID);
    else if(oldstatus!=0&&newstatus==0)
        UnSubscribeMarketData(InstrumentID);
}
///设置指定合约信息
void CPlatFormService::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    m_PlatformParam.HaveAllInstrumentIDs=true;
	m_SubscribeMap[InstrumentID]=0;
    m_pDataCenter->SetInstrumentInfo(InstrumentID, outData);
}
///获取合约的产品类型，失败返回-1
char CPlatFormService::GetProductClassType(const std::string& InstrumentID)
{
    return m_pDataCenter->GetProductClassType(InstrumentID);
}

///获取合约品种代码
int CPlatFormService::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    return m_pDataCenter->GetProductID(outData,ExchangeID);
}

///获取合约手续费率
int CPlatFormService::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_pDataCenter->GetCommissionRate(InstrumentID, outData);
}

///获取合约保证金率
int CPlatFormService::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_pDataCenter->GetMarginRate(InstrumentID, outData);
}
///设置合约手续费率
int CPlatFormService::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_pDataCenter->SetCommissionRate(InstrumentID, outData);
}
///设置正在查询手续费率的合约
void CPlatFormService::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
{
    m_pDataCenter->SetReqCommissionRateInstrumentID(InstrumentID);
}

///设置合约保证金率
int CPlatFormService::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_pDataCenter->SetMarginRate(InstrumentID, outData);
}

///获取合约乘数，成功返回合约乘数，失败返回-1
int CPlatFormService::GetInstrumentMultiple(const std::string& InstrumentID)
{
    return m_pDataCenter->GetInstrumentMultiple(InstrumentID);
}


///获取指定合约行情
int CPlatFormService::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataCenter->GetQuotInfo(InstrumentID, outData);
}

///获取指定合约旧行情
int CPlatFormService::GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataCenter->GetOldQuotInfo(InstrumentID, outData);
}


///获取指定合约的现价，失败返回0.0
double CPlatFormService::GetCurPrice(const std::string& InstrumentID)
{
    return m_pDataCenter->GetCurPrice(InstrumentID);
}
string CPlatFormService::GetTradingDay(const string& ExchangeID)
{
	return m_pDataCenter->GetTradingDay(ExchangeID);
}

bool CPlatFormService::IsTodayPosition( const char* pOpenData )
{
	return false;
}

///获取交易所时间
unsigned long CPlatFormService::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataCenter->GetExchangeTime(ExchangeID,time);
}
    ///获取全部交易所
int CPlatFormService::GetExchangeIDs(vector<string>& outData)
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetExchangeIDs(outData);//m_pDataCenter->GetExchangeIDs(outData);
}

///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
void CPlatFormService::SetCurrentPositionContent(int PositionContentMode)
{

}


///获取全部报单
int CPlatFormService::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetAllOrders(outData);
}
///获取全部报单
int CPlatFormService::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetAllOrders2(strInstrument,outData);
}
///获取指定报单
bool CPlatFormService::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    return m_pDataCenter->GetOrder(inOrderKey,outData);
}

int CPlatFormService::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetTriggerOrders(outData);
}

///获取合约相关的已触发的报单
int CPlatFormService::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetTriggerOrders2(strInstrument,outData);
}

///获取指定已触发的报单
bool CPlatFormService::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataCenter->GetTriggerOrder(orderkey,outData);
}

///获取已成交报单
int CPlatFormService::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetTradedOrders(outData);
}

///获取已成交报单
int CPlatFormService::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetTradedOrders2(strInstrument,outData);
}

///获取指定已成交报单
bool CPlatFormService::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataCenter->GetTradedOrder(orderkey,outData);
}

///获取已撤单和错误报单
int CPlatFormService::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetCanceledOrders(outData);
}

///获取已撤单和错误报单
int CPlatFormService::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetCanceledOrders2(strInstrument,outData);
}

///获取指定已撤单和错误报单
bool CPlatFormService::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataCenter->GetCanceledOrder(orderkey,outData);
}

///获取未成交报单
int CPlatFormService::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetWaitOrders(outData);
}

///获取未成交报单
int CPlatFormService::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetWaitOrders2(strInstrument,outData);
}

///获取指定未成交报单
bool CPlatFormService::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataCenter->GetWaitOrder(orderkey,outData);
}



int CPlatFormService::GetTradingAccountAvailable(double& fAvailable)
{
    return m_pDataCenter->GetTradingAccountAvailable(fAvailable);
}
int CPlatFormService::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    return m_pDataCenter->GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CPlatFormService::GetTradingAccountID(char* AccountID)
{
    return m_pDataCenter->GetTradingAccountID(AccountID);
}
///获取交易资金账户信息
int CPlatFormService::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)
{
	if(m_pTradeApi==NULL)return -1;
	return m_pTradeApi->GetTradingAccount(strAccount, outData);
}
///获取账户资金文本信息
int CPlatFormService::GetAccountText(std::string& outData,int language)
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetAccountText(outData, language);
}
///设置交易资金账户信息
void CPlatFormService::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    m_pDataCenter->SetTradingAccount(inData);
}

///请求查询投资者结算结果响应
int CPlatFormService::GetLastSettlementInfo(std::string& outData)
{
    return m_pDataCenter->GetLastSettlementInfo(outData);
}

///获取全部成交单
int CPlatFormService::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataCenter->GetAllTrades(outData);
}
///获取指定成交信息
bool CPlatFormService::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    return m_pDataCenter->GetTradeInfo(tradekey,outData);
}
 ///获取指定成交信息
int CPlatFormService::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataCenter->GetTradesOfInstrument(strInstrument,outData);
}
///获取全部成交统计记录
int CPlatFormService::GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)
{
	return 0;
}
int CPlatFormService::GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)
{
	return 0;
}
///获取成交统计
int CPlatFormService::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return m_pDataCenter->GetAllTradeTotalDatas(outData);
} 
///在成交统计查找参数rawData
int CPlatFormService::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return m_pDataCenter->FindIndexFromAllTradeTotalDatas(rawData);
}    


int CPlatFormService::GetPositions2(const std::string& strInstrument,
                                    std::set<long>& setFTID,
                                    std::vector<PlatformStru_Position>& outData,
                                    long& lastseq)
{
    return m_pDataCenter->GetPositions2(strInstrument,setFTID,outData,lastseq);
}

int CPlatFormService::GetPositions3(const std::string& strInstrument,
                                    std::vector<PlatformStru_Position>& outData,
                                    long& lastseq)
{
    return m_pDataCenter->GetPositions3(strInstrument,outData,lastseq);
}

int CPlatFormService::GetPositionDetails3(const std::string& strInstrument,
                                          std::set<long>& setFTID,
                                          std::vector<PlatformStru_PositionDetail>& vecValue,
                                          long& lastseq)
{
	throw std::exception("The method is not implemented in pats.");
    return 0;
}

int CPlatFormService::GetPositionCombs2(const std::string& strInstrument, std::vector<PlatformStru_PositionComb>& outData)
{
	throw std::exception("The method is not implemented in pats.");
	return 0;
}

int CPlatFormService::GetPositions(std::vector<PlatformStru_Position>& outData)
{
    return m_pDataCenter->GetPositions(outData);
}

int CPlatFormService::GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
	throw std::exception("The method is not implemented in pats.");
	return 0;
}

int CPlatFormService::GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData)
{
	throw std::exception("The method is not implemented in pats.");
	return 0;
}


bool  CPlatFormService::HavePositionDetail(const std::string& strInstrumentID)
{
	return false;
}


//获取持仓记录键值列表,返回持仓记录的数量
int CPlatFormService::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_pDataCenter->GetPositionKeySet(PositionKeySet);
}

//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
int CPlatFormService::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    return m_pDataCenter->GetPositionData(strAccount,InstrumentID,Direction,HedgeFlag,PositionData);
}


int  CPlatFormService::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataCenter->GetWaitOrderVolume(strAccount,strInstrumentID, Direction, CloseType,HedgeFlag);
}

//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
int CPlatFormService::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataCenter->GetCloseOrderVolume(strAccount,strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);
}



//获取交易连接状态。静态方法，不会创建对象
CPlatFormParam::CONNSTATUS CPlatFormService::GetTradeStatus(void)
{
    return m_PlatformParam.TradeStatus;
};

//获取行情连接状态。静态方法，不会创建对象
CPlatFormParam::CONNSTATUS CPlatFormService::GetQuotStatus(void)
{
    return m_PlatformParam.QuotStatus;
};

//获取交易BrokerID。静态方法，不会创建对象
std::string CPlatFormService::GetTradeBrokerID(void)
{
    return m_PlatformParam.BrokerID;
};

//获取交易InvestorID。静态方法，不会创建对象
std::string CPlatFormService::GetTradeInvestorID(void)
{
    return m_PlatformParam.InvestorID;
};
bool CPlatFormService::IsSettlementInfoConfirmed()
{
	return m_bSettlementInfoConfirmed;
}
void CPlatFormService::FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec )
{

}
SERVER_PARAM_CFG&  CPlatFormService::GetServerParamCfg()
{
	return m_pDataCenter->GetServerParamCfg();
}

//获取客户签约银行信息
int CPlatFormService::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{	
	return 0;
}
void CPlatFormService::SetModuleDataQry(bool bQry)
{

}
bool CPlatFormService::GetModuleDataQry()
{
    return false;
}

int CPlatFormService::GetAllOrderFTIDList( std::vector<long>& vec )
{
	return m_pDataCenter->GetAllOrderFTIDList(vec);
}

int CPlatFormService::GetAllOrderInfo( long lFTID, PlatformStru_OrderInfo& outData )
{
	return m_pDataCenter->GetAllOrderInfo(lFTID, outData);
}

int CPlatFormService::GetAllOrderInfo( OrderKey key, PlatformStru_OrderInfo& outData )
{
	return m_pDataCenter->GetAllOrderInfo(key, outData);
}

int CPlatFormService::GetPositionFTIDList( std::vector<long>& vec )
{
	return m_pDataCenter->GetPositionFTIDList(vec);
}

int CPlatFormService::GetPositionInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_pDataCenter->GetPositionInfo(lFTID, outData);
}

int CPlatFormService::GetPositionInfo( PositionKey key, PlatformStru_Position& outData )
{
	return m_pDataCenter->GetPositionInfo(key, outData);
}

int CPlatFormService::GetPositionCombFTIDList( std::vector<long>& vec )
{
	return m_pDataCenter->GetPositionCombFTIDList(vec);
}

int CPlatFormService::GetPositionCombInfo( long lFTID, PlatformStru_PositionCombDetail& outData )
{
	return m_pDataCenter->GetPositionCombInfo(lFTID, outData);
}

int  CPlatFormService::GetAccountList(std::vector<string>& AcctList)
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetAccountList(AcctList);
}


int CPlatFormService::GetTrader(std::vector<PlatformStru_Trader> &outData) 
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetTrader(outData);
}
///请求查询交易所信息
int CPlatFormService::GetExchange(std::vector<PlatformStru_Exchange> &outData) {
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetExchange(outData);
}
///请求查询合约信息
int CPlatFormService::GetContract(std::vector<PlatformStru_Contract> &outData) {
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetContract(outData);
}
///请求商品信息
int CPlatFormService::GetCommodity(std::vector<PlatformStru_Commodity> &outData) 
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetCommodity(outData);
}
///获取报单类型信息
int CPlatFormService::GetOrderType(std::vector<PlatformStru_OrderType> &outData) 
{
	if(m_pTradeApi==NULL)return 0;
	return m_pTradeApi->GetOrderType(outData);
}
bool CPlatFormService::GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)
{
	if(m_pTradeApi==NULL)return false;	
	return m_pTradeApi->GetTotalPosition(account,posi);
}
bool CPlatFormService::GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
	if(m_pTradeApi==NULL)return false;	
	return m_pTradeApi->GetOpenPosition(InstrumentID,account,posi);
}
bool CPlatFormService::GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)
{
	if(m_pTradeApi==NULL)return false;	
	return m_pTradeApi->GetContractPosition(InstrumentID,account,posi);
}
std::string CPlatFormService::GetLogonReason()
{
	if(m_pTradeApi==NULL)return "";	
	return m_pTradeApi->GetLogonReason();
}
void CPlatFormService::GetLinkStatus(int* pHost,int* pPrice)
{
	if(m_pTradeApi)	
	{
		if(pHost)  *pHost=m_pTradeApi->m_HostLinkState;
		if(pPrice) *pPrice=m_pTradeApi->m_PriceLinkState;
	}
	else
	{
		if(pHost)  *pHost=ptLinkInvalid;		
		if(pPrice) *pPrice=ptLinkInvalid;
	}
}
int CPlatFormService::GetReports( map<DWORD, Report>& mapReport )
{
	if ( m_pTradeApi == NULL )
	{
		return -1;
	}	
	return m_pTradeApi->GetReports(mapReport);
}
double CPlatFormService::GetExchangeRate(char* currency)
{
	if ( m_pTradeApi == NULL )
	{
		return 1.0;	
	}	
	return m_pTradeApi->GetExchangeRate(currency);
}
void CPlatFormService::GetAllPosition(std::vector<PlatformStru_Position>& vecPosition)
{
	m_pTradeApi->GetAllContractPosition(vecPosition);
}