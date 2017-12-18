#include "stdafx.h"

#include "TraderSpi.hpp"
#include "PlatFormService.hpp"
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"

#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CTraderSpi", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)

CTraderSpi::CTraderSpi(CPlatFormService& RefPlatFormService)
:
m_RefTraderApi(*(RefPlatFormService.m_pTradeApi)),
m_RefPackageCache(*(RefPlatFormService.m_PCache)),
m_PlatformParam(RefPlatFormService.m_PlatformParam),
m_RefPlatFormService(RefPlatFormService)
{
    m_PlatformParam.TradeStatus = CPlatFormParam::CONNSTATUS_Disconnected;

    m_pWriteLog=new zqWriteLog();
    LOG_INFO("CTraderSpi::CTraderSpi(构造CTraderSpi对象)");
}

CTraderSpi::~CTraderSpi()
{
    LOG_INFO("CTraderSpi::~CTraderSpi(析构CTraderSpi对象)");
    delete m_pWriteLog;
}
#ifndef _NEED_AUTHENTICATE
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CTraderSpi::OnFrontConnected()
{
    //GetLocalTime(&m_ConnectTime);//获得连接时间
	DataFrontConnected tmp;
	tmp.Head.BID=BID_FrontConnected;
	tmp.Head.Size=sizeof(DataFrontConnected);
	tmp.bTrader=true;
	m_RefPackageCache.SaveDataTo(BID_FrontConnected, 0, 0, 0, sizeof(tmp), &tmp);
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
    strncpy(req.Password, m_PlatformParam.UserPW.c_str(),sizeof(req.Password)-1);
	strncpy(req.UserProductInfo, "中期信息FastTrader",sizeof(req.UserProductInfo)-1);
	strncpy(req.OneTimePassword,m_PlatformParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

    m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_Logining;

    int iRequestID=m_PlatformParam.RequestID++;
    int iRlt = m_RefTraderApi.ReqUserLogin(&req, iRequestID);

    LOG_INFO("OnFrontConnected(与交易所成功建立TCP连接)\n"
                     "\t\t\t --->ReqUserLogin(用户登录请求):ret=[%d],iRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
                     iRlt, iRequestID,
                     req.BrokerID, req.UserID,req.UserProductInfo);
}
#else
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CTraderSpi::OnFrontConnected()
{
    GetLocalTime(&m_ConnectTime);//获得连接时间
	DataFrontConnected tmp;
	tmp.Head.BID=BID_FrontConnected;
	tmp.Head.Size=sizeof(DataFrontConnected);
	tmp.bTrader=true;
	m_RefPackageCache.SaveDataTo(BID_FrontConnected, 0, 0, 0, sizeof(tmp), &tmp);
    CThostFtdcReqAuthenticateField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
	strncpy(req.UserProductInfo, "中期信息FastTrader",sizeof(req.UserProductInfo)-1);
	strcpy(req.AuthCode,"");
	m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_Logining;

    int iRequestID=m_PlatformParam.RequestID++;

    int iRlt = m_RefTraderApi.ReqAuthenticate(&req, iRequestID);

    LOG_INFO("OnFrontConnected(与交易所成功建立TCP连接)\n"
                     "\t\t\t --->ReqAuthenticate(客户端认证请求):ret=[%d],iRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
                     iRlt, iRequestID,
                     req.BrokerID, req.UserID,req.UserProductInfo);
}
//客户端认证响应
void CTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(0==pRspInfo->ErrorID)//登入成功
	{
		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
		strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
		strncpy(req.Password, m_PlatformParam.UserPW.c_str(),sizeof(req.Password)-1);
	    strncpy(req.UserProductInfo, "中期信息FastTrader",sizeof(req.UserProductInfo)-1);
		strncpy(req.OneTimePassword,m_PlatformParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

		int iRequestID=m_PlatformParam.RequestID++;

		int iRlt = m_RefTraderApi.ReqUserLogin(&req, iRequestID);

		LOG_INFO("OnRspAuthenticate(客户端认证响应)\n"
			"\t\t\t --->ReqUserLogin(用户登录请求):ret=[%d],iRequestID=[%d],\n"
			"\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
			iRlt, iRequestID,
			req.BrokerID, req.UserID,req.UserProductInfo);
	}
	else
	{
        m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_LoginFailure;
		if(pRspInfo) memcpy(&m_RefPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
	}
}
#endif
///当客户端与交易后台通信连接断开时，该方法被调用。
///当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CTraderSpi::OnFrontDisconnected(int nReason)
{
    LOG_INFO("OnFrontDisconnected(与交易所失去TCP连接), Reason=[%#x]", nReason);
    DataFrontDisconnected tmp;
    tmp.Head.BID=BID_FrontDisconnected;
    tmp.Head.Size=sizeof(DataFrontDisconnected);
	tmp.bTrader=true;
    tmp.nReason=nReason;
    m_RefPackageCache.SaveDataTo(BID_FrontDisconnected, 0, 0, 0, sizeof(tmp), &tmp);
    m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_Disconnected;
}
    
///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
///综合交易平台的API已经屏蔽此接口
void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
    LOG_INFO("OnHeartBeatWarning:nTimeLapse=[%d]", nTimeLapse);
}


///登录请求响应
void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pRspUserLogin)
    {
		m_RefPlatFormService.SetOrderRef(atoi(pRspUserLogin->MaxOrderRef));
        DataRspUserLogin tmp;
        tmp.bTrader       = true;
        tmp.nRequestID = nRequestID;
        tmp.bIsLast    = bIsLast;
        tmp.CurTickCount = GetTickCount();
        tmp.UserLoginField = *pRspUserLogin;
        if(pRspInfo)
            tmp.RspInfoField = *pRspInfo;

        m_RefPackageCache.SaveDataTo(BID_RspUserLogin, 0, 0, 0, sizeof(tmp), &tmp);
        
        LOG_INFO("OnRspUserLogin(登录请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]\n"
                        "\t\t\t TradingDay=[%s],\t LoginTime=[%s],\t BrokerID=[%s],\t UserID=[%s],\t SystemName=[%s]\n"
                        "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t MaxOrderRef[%s],\t SHFETime=[%s],\t DCETime=[%s],\n"
                        "\t\t\t CZCETime[%s],\t FFEXTime=[%s],\t CurTickCount=[%ul]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pRspUserLogin->TradingDay,pRspUserLogin->LoginTime,pRspUserLogin->BrokerID   , pRspUserLogin->UserID,pRspUserLogin->SystemName, 
                        pRspUserLogin->FrontID  , pRspUserLogin->SessionID ,pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime , pRspUserLogin->DCETime, 
                        pRspUserLogin->CZCETime   , pRspUserLogin->FFEXTime,tmp.CurTickCount);
    }
    else
    {
        LOG_INFO("OnRspUserLogin(登录请求响应),pRspUserLogin is NULL");
    }
    
    if(0==pRspInfo->ErrorID)//登入成功
    {
		m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirming;
        PlatformStru_QrySettlementInfoConfirm req;
        memset(&req, 0, sizeof(req));
        strcpy(req.Thost.BrokerID, m_PlatformParam.BrokerID.c_str());
        strcpy(req.Thost.InvestorID, m_PlatformParam.InvestorID.c_str());

        int iRequestID=m_PlatformParam.RequestID++;
        int iRlt = m_RefTraderApi.ReqQrySettlementInfoConfirm((CThostFtdcQrySettlementInfoConfirmField *)&req.Thost, iRequestID);

        LOG_INFO("OnRspUserLogin(登录请求响应)\n"
                        "\t\t\t --->ReqSettlementInfoConfirm(投资者结算结果确认):ret=[%d],iRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                        iRlt, iRequestID, req.Thost.BrokerID, req.Thost.InvestorID); 

    }
    else
    {
        m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_LoginFailure;
		if(pRspInfo) memcpy(&m_RefPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
    }

}


///投资者结算结果确认响应
void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG_INFO("OnRspSettlementInfoConfirm(投资者结算结果确认响应),ErrorID=[%d],ErrorMsg=[%s]", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
  /*  
    if(pRspInfo->ErrorID==0)
    {
        #if 0//合约列表查询交给GUI登入模块处理
        CThostFtdcQryInstrumentField req;
        memset(&req, 0, sizeof(req));
        
        int iRequestID=m_PlatformParam.RequestID++;
        int iRlt = m_RefTraderApi.ReqQryInstrument(&req, iRequestID);

        LOG_INFO("OnRspSettlementInfoConfirm--->ReqQryInstrument(return code:%d) : iRequestID=[%d]",
				iRlt, iRequestID);    
        #endif
        
        m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_Connected;
        
    }
    else
    {
        m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_TradeSettlementInfoConfirmFailure;
    }*/
}


///登出请求响应
void CTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pUserLogout)
    {
        LOG_INFO("OnRspUserLogout(登出请求响应),pUserLogout is NULL");
        return;
    }

    DataRspUserLogout tmp;
    tmp.Type       = 0;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.UserLogoutField = *pUserLogout;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspUserLogout, 0, 0, 0, sizeof(tmp), &tmp);

    LOG_INFO("OnRspUserLogout(登出请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                    "\t\t\t BrokerID=[%s],\t UserID=[%s],",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                    pUserLogout->BrokerID, pUserLogout->UserID);
}

///用户口令更新请求响应
void CTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pUserPasswordUpdate)
    {
        LOG_INFO("OnRspUserPasswordUpdate(用户口令更新请求响应),pUserPasswordUpdate is NULL");
        return;
    }

    DataRspUserPasswordUpdate tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.UserPasswordUpdateField = *pUserPasswordUpdate;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

	if(tmp.RspInfoField.ErrorID==0)
	{
		m_PlatformParam.InvestorPW=pUserPasswordUpdate->NewPassword;
	}

    m_RefPackageCache.SaveDataTo(BID_RspUserPasswordUpdate, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspUserPasswordUpdate(用户口令更新请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                    "\t\t\t BrokerID=[%s],\t UserID=[%s],\t OldPassword=[%s],\t NewPassword=[%s]",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                    pUserPasswordUpdate->BrokerID, pUserPasswordUpdate->UserID,pUserPasswordUpdate->OldPassword,pUserPasswordUpdate->NewPassword);
}

///资金账户口令更新请求响应
void CTraderSpi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pTradingAccountPasswordUpdate)
    {
        LOG_INFO("OnRspTradingAccountPasswordUpdate(资金账户口令更新请求响应),pTradingAccountPasswordUpdate is NULL");
        return;
    }

    DataRspTradingAccountPasswordUpdate tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.TradingAccountPasswordUpdateField = *pTradingAccountPasswordUpdate;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspTradingAccountPasswordUpdate, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspTradingAccountPasswordUpdate(资金账户口令更新请求响应) : ErrorID=[%d],ErrorMsg=[%s],BrokerID=[%s] AccountID=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
			pTradingAccountPasswordUpdate->BrokerID, pTradingAccountPasswordUpdate->AccountID, nRequestID,bIsLast);
}

///报单录入请求响应
void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pInputOrder)
    {
        LOG_INFO("OnRspOrderInsert(报单录入请求响应),pInputOrder is NULL");
        return;
    }

    DataRspOrderInsert1 tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.InputOrderField = *pInputOrder;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspOrderInsert1, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspOrderInsert(报单录入请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
             "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
             pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
             pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
             pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
             pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
             pInputOrder->RequestID, pInputOrder->UserForceClose);
}

///报单操作请求响应
void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pInputOrderAction)
    {
        LOG_INFO("OnRspOrderAction(报单操作请求响应),pInputOrderAction is NULL");
        return;
    }

    DataRspOrderAction1 tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.InputOrderActionField = *pInputOrderAction;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspOrderAction1, 0, 0, 0, sizeof(tmp), &tmp);  
    LOG_INFO("OnRspOrderAction(报单操作请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                     pInputOrderAction->BrokerID, pInputOrderAction->InvestorID, pInputOrderAction->OrderActionRef, pInputOrderAction->OrderRef,pInputOrderAction->RequestID,
                     pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->ExchangeID, pInputOrderAction->OrderSysID,pInputOrderAction->ActionFlag,
                     pInputOrderAction->LimitPrice, pInputOrderAction->VolumeChange, pInputOrderAction->UserID, pInputOrderAction->InstrumentID);
}

///查询最大报单数量响应
void CTraderSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

    LOG_INFO("OnRspQueryMaxOrderVolume(查询最大报单数量响应),Unimplemented");
}



///预埋单录入请求响应
void CTraderSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pParkedOrder)
    {
        LOG_INFO("OnRspParkedOrderInsert(预埋单录入请求响应),pParkedOrder is NULL");
        return;
    }

    DataRspParkedOrderInsert tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.ParkedOrder = *pParkedOrder;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspParkedOrderInsert, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspParkedOrderInsert(预埋单录入请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
             "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
             pParkedOrder->BrokerID, pParkedOrder->InvestorID, pParkedOrder->InstrumentID, pParkedOrder->OrderRef,pParkedOrder->UserID,
             pParkedOrder->OrderPriceType, pParkedOrder->Direction, pParkedOrder->CombOffsetFlag, pParkedOrder->CombHedgeFlag,pParkedOrder->LimitPrice,
             pParkedOrder->VolumeTotalOriginal, pParkedOrder->TimeCondition, pParkedOrder->GTDDate, pParkedOrder->VolumeCondition,pParkedOrder->MinVolume,
             pParkedOrder->ContingentCondition, pParkedOrder->StopPrice, pParkedOrder->ForceCloseReason, pParkedOrder->IsAutoSuspend,pParkedOrder->BusinessUnit,
             pParkedOrder->RequestID, pParkedOrder->UserForceClose);
}

///预埋撤单录入请求响应
void CTraderSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pParkedOrderAction)
    {
        LOG_INFO("OnRspParkedOrderAction(预埋撤单录入请求响应),pParkedOrderAction is NULL");
        return;
    }

    DataRspParkedOrderAction tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.ParkedOrderAction = *pParkedOrderAction;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspParkedOrderAction, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspParkedOrderAction(预埋撤单录入请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                     pParkedOrderAction->BrokerID, pParkedOrderAction->InvestorID, pParkedOrderAction->OrderActionRef, pParkedOrderAction->OrderRef,pParkedOrderAction->RequestID,
                     pParkedOrderAction->FrontID, pParkedOrderAction->SessionID, pParkedOrderAction->ExchangeID, pParkedOrderAction->OrderSysID,pParkedOrderAction->ActionFlag,
                     pParkedOrderAction->LimitPrice, pParkedOrderAction->VolumeChange, pParkedOrderAction->UserID, pParkedOrderAction->InstrumentID);
}

///删除预埋单响应
void CTraderSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pRemoveParkedOrder)
    {
        LOG_INFO("OnRspRemoveParkedOrder(删除预埋单响应),pRemoveParkedOrder is NULL");
        return;
    }

    DataRspRemoveParkedOrder tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.RemoveParkedOrder = *pRemoveParkedOrder;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspRemoveParkedOrder, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspRemoveParkedOrder(删除预埋单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderID=[%s]",
                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                     pRemoveParkedOrder->BrokerID, pRemoveParkedOrder->InvestorID, pRemoveParkedOrder->ParkedOrderID);
}

///删除预埋撤单响应
void CTraderSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(!pRemoveParkedOrderAction)
    {
        LOG_INFO("OnRspRemoveParkedOrderAction(删除预埋撤单响应),pRemoveParkedOrderAction is NULL");
        return;
    }

    DataRspRemoveParkedOrderAction tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.RemoveParkedOrderAction = *pRemoveParkedOrderAction;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspRemoveParkedOrderAction, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnRspRemoveParkedOrderAction(删除预埋撤单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ParkedOrderActionID=[%s]",
                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                     pRemoveParkedOrderAction->BrokerID, pRemoveParkedOrderAction->InvestorID, pRemoveParkedOrderAction->ParkedOrderActionID);
}

///请求查询预埋单响应
void CTraderSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryParkedOrder tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;
    
    if(!pParkedOrder)//查询结果为空时，pParkedOrder的值将为NULL
    {
        LOG_INFO("OnRspQryParkedOrder(请求查询预埋单响应),pParkedOrder is NULL,可能是无查询记录");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        tmp.ParkedOrder = *pParkedOrder;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryParkedOrder, 0, 0, 0, sizeof(tmp), &tmp); 

    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryParkedOrder(请求查询预埋单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        LOG_INFO("OnRspQryParkedOrder(请求查询预埋单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                        "\t\t\t RequestID=[%d],\t ParkedOrderID=[%s],\t ExchangeID=[%s],\n"
                        "\t\t\t UserForceClose=[%d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pParkedOrder->BrokerID,pParkedOrder->InvestorID,pParkedOrder->InstrumentID,pParkedOrder->OrderRef,pParkedOrder->UserID,
                        pParkedOrder->OrderPriceType,pParkedOrder->Direction,pParkedOrder->CombOffsetFlag,pParkedOrder->CombHedgeFlag,pParkedOrder->LimitPrice,
                        pParkedOrder->VolumeTotalOriginal,pParkedOrder->TimeCondition,pParkedOrder->GTDDate,pParkedOrder->VolumeCondition,pParkedOrder->MinVolume,
                        pParkedOrder->ContingentCondition,pParkedOrder->StopPrice,pParkedOrder->ForceCloseReason,pParkedOrder->IsAutoSuspend,pParkedOrder->BusinessUnit,
                        pParkedOrder->RequestID,pParkedOrder->ParkedOrderID,pParkedOrder->ExchangeID,
                        pParkedOrder->UserForceClose
                    );
    }
}

///请求查询预埋撤单响应
void CTraderSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryParkedOrderAction tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;
    
    if(!pParkedOrderAction)//查询结果为空时，pParkedOrderAction的值将为NULL
    {
        LOG_INFO("OnRspQryParkedOrderAction(请求查询预埋撤单响应),pParkedOrderAction is NULL,可能是无查询记录");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);
        }
    }
    else
    {
        tmp.ParkedOrderAction = *pParkedOrderAction;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryParkedOrderAction, 0, 0, 0, sizeof(tmp), &tmp); 

    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryParkedOrderAction(请求查询预埋撤单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        LOG_INFO("OnRspQryParkedOrderAction(请求查询预埋撤单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeChange=[%d],\n"
                        "\t\t\t RequestID=[%d],\t ExchangeID=[%s],\n"
                        "\t\t\t ActionFlag=[%d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pParkedOrderAction->BrokerID,pParkedOrderAction->InvestorID,pParkedOrderAction->InstrumentID,pParkedOrderAction->OrderRef,pParkedOrderAction->UserID,
                        pParkedOrderAction->LimitPrice,
                        pParkedOrderAction->VolumeChange,
                        pParkedOrderAction->RequestID,pParkedOrderAction->ExchangeID,
                        pParkedOrderAction->ActionFlag
                    );
    }
}




///请求查询报单响应
void CTraderSpi::OnRspQryOrder(PlatformStru_OrderInfo *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryOrder tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;
    
    if(!pOrder)//查询结果为空时，pOrder的值将为NULL
    {
        LOG_INFO("OnRspQryOrder(请求查询报单响应),pOrder is NULL,可能是无查询记录");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);
           
        }
    }
    else
    {
        tmp.OrderField = *pOrder;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryOrder, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryOrder(请求查询报单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pOrder= &tmp.OrderField;
        LOG_INFO("OnRspQryOrder(请求查询报单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                        "\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ClientID=[%s],\n"
                        "\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
                        "\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
                        "\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
                        "\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
                        "\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
                        pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
                        pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
                        pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
                        pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ClientID,
                        pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
                        pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
                        pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
                        pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
                        pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
                        pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
                    );
    }
}

//void CTraderSpi::OnRspQryOrderEx(PlatformStru_OrderInfo *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//{
//
//
//    DataRspQryOrder tmp;
//    tmp.nRequestID = nRequestID;
//    tmp.bIsLast    = bIsLast;
//    if(pRspInfo)
//        tmp.RspInfoField = *pRspInfo;
//    
//    if(!pOrder)//查询结果为空时，pOrder的值将为NULL
//    {
//        LOG_INFO("OnRspQryOrder(请求查询报单响应),pOrder is NULL,可能是无查询记录");
//        if(tmp.RspInfoField.ErrorID==0)
//        {
//            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
//            tmp.RspInfoField.ErrorID=CustomErrorID;
//            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);
//           
//        }
//    }
//    else
//    {
//        tmp.OrderField = *pOrder;
//    }
//
//    m_RefPackageCache.SaveDataTo(BID_RspQryOrder, 0, 0, 0, sizeof(tmp), &tmp); 
//
//    //log
//    if(tmp.RspInfoField.ErrorID!=0)
//    {
//        LOG_INFO("OnRspQryOrder(请求查询报单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
//                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
//                         );
//
//    }
//    else
//    {
//        pOrder= &tmp.OrderField;
//        LOG_INFO("OnRspQryOrder(请求查询报单响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
//                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
//                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
//                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
//                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
//                        "\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ClientID=[%s],\n"
//                        "\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
//                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
//                        "\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
//                        "\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
//                        "\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
//                        "\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
//                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
//                        pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
//                        pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
//                        pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
//                        pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
//                        pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ClientID,
//                        pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
//                        pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
//                        pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
//                        pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
//                        pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
//                        pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
//                    );
//    }
//}

///请求查询成交响应
void CTraderSpi::OnRspQryTrade(PlatformStru_TradeInfo *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryTrade tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;
    
    if(!pTrade)
    {
        LOG_INFO("OnRspQryTrade(请求查询成交响应),pTrade is NULL,可能是无查询记录");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {    
        tmp.TradeField = *pTrade;
    }
    

    m_RefPackageCache.SaveDataTo(BID_RspQryTrade, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryTrade(请求查询成交响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pTrade=&tmp.TradeField;
        LOG_INFO("OnRspQryTrade(请求查询成交响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
                        "\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
                        "\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
                        "\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
                        pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
                        pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
                        pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
                        pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
                        pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
                        pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq
                        );
    }
}

///请求查询投资者持仓响应
void CTraderSpi::OnRspQryInvestorPosition(PlatformStru_Position *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
//void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryInvestorPosition tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pInvestorPosition)
    {
        LOG_INFO("OnRspQryInvestorPosition(请求查询投资者持仓响应),pInvestorPosition is NULL,可能是无查询记录");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {    
		tmp.InvestorPositionField = *pInvestorPosition;
        //CopyPositionField( tmp.InvestorPositionField, *pInvestorPosition );
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryInvestorPosition, 0, 0, 0, sizeof(tmp), &tmp);

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInvestorPosition(请求查询投资者持仓响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        LOG_INFO("OnRspQryInvestorPosition(请求查询投资者持仓响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t PosiDirection=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t PositionDate=[%d],\t YdPosition=[%d],\t Position=[%d],\t LongFrozen=[%d],\t ShortFrozen=[%d],\n"
                        "\t\t\t LongFrozenAmount=[%g],\t  ShortFrozenAmount=[%g],\t OpenVolume=[%d],\t CloseVolume=[%d],\t OpenAmount=[%g],\n"
                        "\t\t\t CloseAmount=[%g],\t PositionCost=[%g],\t PreMargin=[%g],\t UseMargin=[%g],\t FrozenMargin=[%g],\n"
                        "\t\t\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CashIn=[%g],\t Commission=[%g],CloseProfit=[%g],\n"
                        "\t\t\t PositionProfit=[%g],\t PreSettlementPrice=[%g],\t SettlementPrice=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n"
                        "\t\t\t OpenCost=[%g],\t ExchangeMargin=[%g],\t CombPosition=[%d],\t CombLongFrozen=[%d],\t CombShortFrozen=[%d],\n"
                        "\t\t\t CloseProfitByDate=[%g],\t CloseProfitByTrade=[%g],\t TodayPosition=[%d],\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInvestorPosition->InstrumentID, pInvestorPosition->BrokerID,pInvestorPosition->InvestorID,pInvestorPosition->PosiDirection,pInvestorPosition->HedgeFlag,
                        pInvestorPosition->PositionDate, pInvestorPosition->YdPosition,pInvestorPosition->Position,pInvestorPosition->LongFrozen,pInvestorPosition->ShortFrozen,
                        pInvestorPosition->LongFrozenAmount, pInvestorPosition->ShortFrozenAmount,pInvestorPosition->OpenVolume,pInvestorPosition->CloseVolume,pInvestorPosition->OpenAmount,
                        pInvestorPosition->CloseAmount, pInvestorPosition->PositionCost,pInvestorPosition->PreMargin,pInvestorPosition->UseMargin,pInvestorPosition->FrozenMargin,
                        pInvestorPosition->FrozenCash, pInvestorPosition->FrozenCommission,pInvestorPosition->CashIn,pInvestorPosition->Commission,pInvestorPosition->CloseProfit,
                        pInvestorPosition->PositionProfit, pInvestorPosition->PreSettlementPrice,pInvestorPosition->SettlementPrice,pInvestorPosition->TradingDay,pInvestorPosition->SettlementID,
                        pInvestorPosition->OpenCost, pInvestorPosition->ExchangeMargin,pInvestorPosition->CombPosition,pInvestorPosition->CombLongFrozen,pInvestorPosition->CombShortFrozen,
                        pInvestorPosition->CloseProfitByDate, pInvestorPosition->CloseProfitByTrade,pInvestorPosition->TodayPosition,pInvestorPosition->MarginRateByMoney,pInvestorPosition->MarginRateByVolume
                        );
    }


}

///请求查询资金账户响应
void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    if(!pTradingAccount)
    {
        LOG_INFO("OnRspQryTradingAccount(请求查询资金账户响应),pTradingAccount is NULL");
        return;
    }

    DataRspQryTradingAccount tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
   // tmp.TradingAccountField = *pTradingAccount;
	memset(&tmp.TradingAccountField,0,sizeof(PlatformStru_TradingAccountInfo));

     strcpy(tmp.TradingAccountField.AccountID, pTradingAccount->AccountID);
	 strcpy(tmp.TradingAccountField.BrokerID, pTradingAccount->BrokerID);
     tmp.TradingAccountField.PreMortgage=pTradingAccount->PreMortgage;
	 tmp.TradingAccountField.PreCredit=pTradingAccount->PreCredit;
	 tmp.TradingAccountField.PreDeposit=pTradingAccount->PreDeposit;
	 tmp.TradingAccountField.PreBalance=pTradingAccount->PreBalance;
	 tmp.TradingAccountField.PreMargin=pTradingAccount->PreMargin;
	 tmp.TradingAccountField.InterestBase=pTradingAccount->InterestBase;
	 tmp.TradingAccountField.Interest=pTradingAccount->Interest;
	 tmp.TradingAccountField.Deposit=pTradingAccount->Deposit;

	 tmp.TradingAccountField.Withdraw=pTradingAccount->Withdraw;
	 tmp.TradingAccountField.FrozenMargin=pTradingAccount->FrozenMargin;
	 //tmp.TradingAccountField.FrozenCash=pTradingAccount->FrozenCash;
	 tmp.TradingAccountField.FrozenCommission=pTradingAccount->FrozenCommission;
	 tmp.TradingAccountField.CurrMargin=pTradingAccount->CurrMargin;
	 tmp.TradingAccountField.CashIn=pTradingAccount->CashIn;
	 tmp.TradingAccountField.Commission=pTradingAccount->Commission;
	 tmp.TradingAccountField.CloseProfit=pTradingAccount->CloseProfit;

	 tmp.TradingAccountField.PositionProfit=pTradingAccount->PositionProfit;
	 tmp.TradingAccountField.Balance=pTradingAccount->Balance;
	// tmp.TradingAccountField.Available=pTradingAccount->Available;
	 tmp.TradingAccountField.WithdrawQuota=pTradingAccount->WithdrawQuota;
	 tmp.TradingAccountField.Reserve=pTradingAccount->Reserve;
     strcpy(tmp.TradingAccountField.TradingDay, pTradingAccount->TradingDay);
	 tmp.TradingAccountField.SettlementID=pTradingAccount->SettlementID;
	 tmp.TradingAccountField.Credit=pTradingAccount->Credit;
	 tmp.TradingAccountField.Mortgage=pTradingAccount->Mortgage;
	 tmp.TradingAccountField.ExchangeMargin=pTradingAccount->ExchangeMargin;
	 tmp.TradingAccountField.DeliveryMargin=pTradingAccount->DeliveryMargin;
	 tmp.TradingAccountField.ExchangeDeliveryMargin=pTradingAccount->ExchangeDeliveryMargin;

	 tmp.TradingAccountField.StaticProfit = tmp.TradingAccountField.PreBalance
			- tmp.TradingAccountField.PreCredit - tmp.TradingAccountField.PreMortgage
			+ tmp.TradingAccountField.Mortgage - tmp.TradingAccountField.Withdraw 
			+ tmp.TradingAccountField.Deposit;
		tmp.TradingAccountField.DynamicProfit = tmp.TradingAccountField.StaticProfit 
			+ tmp.TradingAccountField.CloseProfit + tmp.TradingAccountField.PositionProfit 
			- tmp.TradingAccountField.Commission;
		tmp.TradingAccountField.Available = tmp.TradingAccountField.DynamicProfit
			- tmp.TradingAccountField.CurrMargin - tmp.TradingAccountField.FrozenMargin
			- tmp.TradingAccountField.FrozenCommission 
			- tmp.TradingAccountField.DeliveryMargin + tmp.TradingAccountField.Credit;
		tmp.TradingAccountField.FrozenCash = tmp.TradingAccountField.FrozenMargin 
			+ tmp.TradingAccountField.FrozenCommission;
		if ( util::equals( tmp.TradingAccountField.DynamicProfit, 0) )
		{
			tmp.TradingAccountField.RiskDegree = 0;
		}
		else
		{
			tmp.TradingAccountField.RiskDegree = ( tmp.TradingAccountField.CurrMargin 
				+ tmp.TradingAccountField.DeliveryMargin ) 
				* 100 / tmp.TradingAccountField.DynamicProfit;
		}


    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspQryTradingAccount, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspQryTradingAccount(请求查询资金账户响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                    "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t PreMortgage=[%g],\t PreCredit=[%g],\t PreDeposit=[%g],\n"
                    "\t\t\t PreBalance=[%g],\t PreMargin=[%g],\t InterestBase=[%g],\t Interest=[%g],\t Deposit=[%g],\n "
                    "\t\t\t Withdraw=[%g],\t FrozenMargin=[%g],\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CurrMargin=[%g],\n "
                    "\t\t\t CashIn=[%g],\t Commission=[%g],\t CloseProfit=[%g],\t PositionProfit=[%g],\t Balance=[%g],\n "
                    "\t\t\t Available=[%g],\t WithdrawQuota=[%g],\t Reserve=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n "
                    "\t\t\t Credit=[%g],\t Mortgage=[%g],\t ExchangeMargin=[%g],\t DeliveryMargin=[%g]",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                    pTradingAccount->BrokerID,pTradingAccount->AccountID,pTradingAccount->PreMortgage,pTradingAccount->PreCredit,pTradingAccount->PreDeposit, 
                    pTradingAccount->PreBalance,pTradingAccount->PreMargin,pTradingAccount->InterestBase,pTradingAccount->Interest,pTradingAccount->Deposit, 
                    pTradingAccount->Withdraw,pTradingAccount->FrozenMargin,pTradingAccount->FrozenCash,pTradingAccount->FrozenCommission,pTradingAccount->CurrMargin, 
                    pTradingAccount->CashIn,pTradingAccount->Commission,pTradingAccount->CloseProfit,pTradingAccount->PositionProfit,pTradingAccount->Balance, 
                    pTradingAccount->Available,pTradingAccount->WithdrawQuota,pTradingAccount->Reserve,pTradingAccount->TradingDay,pTradingAccount->SettlementID, 
                    pTradingAccount->Credit,pTradingAccount->Mortgage,pTradingAccount->ExchangeMargin,pTradingAccount->DeliveryMargin
                    );
}

///请求查询投资者响应
void CTraderSpi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryInvestor tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pInvestor)
    {
        LOG_INFO("OnRspQryInvestor(请求查询投资者响应),pInvestor is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.InvestorField = *pInvestor;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryInvestor, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInvestor(请求查询投资者响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pInvestor= &tmp.InvestorField;
        LOG_INFO("OnRspQryInvestor(请求查询投资者响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InvestorID=[%s],\t BrokerID=[%s],\t InvestorGroupID=[%s],\t InvestorName=[%s],\t IdentifiedCardType=[%d],\n"
                        "\t\t\t IdentifiedCardNo=[%s],\t IsActive=[%d],\t Telephone=[%s],\t Address=[%s]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInvestor->InvestorID, pInvestor->BrokerID,pInvestor->InvestorGroupID,pInvestor->InvestorName,pInvestor->IdentifiedCardType,
                        pInvestor->IdentifiedCardNo, pInvestor->IsActive,pInvestor->Telephone,pInvestor->Address
                        );
    }
}

///请求查询交易编码响应
void CTraderSpi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    if(!pTradingCode)
    {
        LOG_INFO("OnRspQryTradingCode(请求查询交易编码响应),pTradingCode is NULL");
        return;
    }

    DataRspQryTradingCode tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.TradingCodeField = *pTradingCode;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspQryTradingCode, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspQryTradingCode(请求查询交易编码响应) : ErrorID=[%d],ErrorMsg=[%s],InvestorID=[%s],BrokerID=[%s],ClientID=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
			pTradingCode->InvestorID, pTradingCode->BrokerID,pTradingCode->ClientID,nRequestID,bIsLast);
}

///请求查询合约保证金率响应
void CTraderSpi::OnRspQryInstrumentMarginRate(PlatformStru_InstrumentMarginRate *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryInstrumentMarginRate tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pInstrumentMarginRate)
    {
        LOG_INFO("OnRspQryInstrumentMarginRate(请求查询合约保证金率响应),pInstrumentMarginRate is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.InstrumentMarginRateField = *pInstrumentMarginRate;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryInstrumentMarginRate, 0, 0, 0, sizeof(tmp), &tmp);

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInstrumentMarginRate(请求查询合约保证金率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pInstrumentMarginRate=&tmp.InstrumentMarginRateField;
        LOG_INFO("OnRspQryInstrumentMarginRate(请求查询合约保证金率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\n"
                        "\t\t\t LongMarginRatioByMoney=[%g],\t LongMarginRatioByVolume=[%g],\t ShortMarginRatioByMoney=[%g],\t ShortMarginRatioByVolume=[%g],\t IsRelative=[%d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInstrumentMarginRate->InstrumentID,
                        pInstrumentMarginRate->InvestorRange,
                        pInstrumentMarginRate->BrokerID,
                        pInstrumentMarginRate->InvestorID,
                        pInstrumentMarginRate->HedgeFlag,
                        pInstrumentMarginRate->LongMarginRatioByMoney,
                        pInstrumentMarginRate->LongMarginRatioByVolume,
                        pInstrumentMarginRate->ShortMarginRatioByMoney,
                        pInstrumentMarginRate->ShortMarginRatioByVolume,
                        pInstrumentMarginRate->IsRelative
                        );
        }
}

///请求查询合约手续费率响应
void CTraderSpi::OnRspQryInstrumentCommissionRate(PlatformStru_InstrumentCommissionRate *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryInstrumentCommissionRate tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pInstrumentCommissionRate)
    {
        LOG_INFO("OnRspQryInstrumentCommissionRate(请求查询合约手续费率响应),pInstrumentCommissionRate is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.InstrumentCommissionRateField = *pInstrumentCommissionRate;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryInstrumentCommissionRate, 0, 0, 0, sizeof(tmp), &tmp);

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInstrumentCommissionRate(请求查询合约手续费率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, 
                         tmp.RspInfoField.ErrorMsg,
                         nRequestID,bIsLast
                         );

    }
    else
    {
        pInstrumentCommissionRate=&tmp.InstrumentCommissionRateField;
        LOG_INFO("OnRspQryInstrumentCommissionRate(请求查询合约手续费率响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t OpenRatioByMoney=[%g],\n"
                        "\t\t\t OpenRatioByVolume=[%g],\t CloseRatioByMoney=[%g],\t CloseRatioByVolume=[%g],\t CloseTodayRatioByMoney=[%g],\t CloseTodayRatioByVolume=[%g]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pInstrumentCommissionRate->InstrumentID,
                        pInstrumentCommissionRate->InvestorRange,
                        pInstrumentCommissionRate->BrokerID,
                        pInstrumentCommissionRate->InvestorID,
                        pInstrumentCommissionRate->OpenRatioByMoney,
                        pInstrumentCommissionRate->OpenRatioByVolume,
                        pInstrumentCommissionRate->CloseRatioByMoney,
                        pInstrumentCommissionRate->CloseRatioByVolume,
                        pInstrumentCommissionRate->CloseTodayRatioByMoney,
                        pInstrumentCommissionRate->CloseTodayRatioByVolume
                        );
    }

}

///请求查询交易所响应
void CTraderSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    if(!pExchange)
    {
        LOG_INFO("OnRspQryExchange(请求查询交易所响应),pExchange is NULL");
        return;
    }

    DataRspQryExchange tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
    tmp.ExchangeField = *pExchange;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspQryExchange, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRspQryExchange(请求查询交易所响应) : ErrorID=[%d],ErrorMsg=[%s],ExchangeID=[%s],ExchangeName=[%s],ExchangeProperty=[%c],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
			pExchange->ExchangeID, pExchange->ExchangeName,pExchange->ExchangeProperty,nRequestID,bIsLast);
}

///请求查询合约响应
void CTraderSpi::OnRspQryInstrument(PlatformStru_InstrumentInfo *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    if(!pInstrument)
    {
        LOG_INFO("OnRspQryInstrument(请求查询合约响应),pInstrument is NULL");
        return;
    }
//修复大商所 组合 合约乘数为0的问题
	if(strcmp(pInstrument->ExchangeID,"DCE")==0&&
		pInstrument->ProductClass==THOST_FTDC_PC_Combination&&
		pInstrument->VolumeMultiple==0)
	{
		pInstrument->VolumeMultiple=1;
	}
    DataRspQryInstrument tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;
	tmp.InstrumentField = *pInstrument;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    //暂时不处理组合合约。updated by l. 20110301
    if(pInstrument->ProductClass!=THOST_FTDC_PC_Futures)
    {
        memset(tmp.InstrumentField.InstrumentID,0,sizeof(tmp.InstrumentField.InstrumentID));
    }

    //如果已经获取到全部的合约信息，无需再发送给DataCenter
    if(!m_PlatformParam.HaveAllInstrumentIDs)
        m_RefPackageCache.SaveDataTo(BID_RspQryInstrument, 0, 0, 0, sizeof(tmp), &tmp); 

    //如果是最后一个合约信息，表示全部的合约信息已经获取成功
    if(tmp.RspInfoField.ErrorID==0 && bIsLast)
    {
        m_PlatformParam.HaveAllInstrumentIDs=true;
    }   

    //定制行情数据
    if(tmp.RspInfoField.ErrorID==0)
    {
        //if(m_RefPlatFormService.)
    }

    LOG_INFO("OnRspQryInstrument(请求查询合约响应) : ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],InstrumentName=[%s],nRequestID=[%d],bIsLast=[%d]",
			tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
			pInstrument->InstrumentID, pInstrument->InstrumentName,nRequestID,bIsLast);
}
///请求查询行情响应
void CTraderSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    LOG_INFO("OnRspQryDepthMarketData,Unimplemented");
}

///请求查询投资者结算结果响应
void CTraderSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQrySettlementInfo tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pSettlementInfo)
    {
        LOG_INFO("OnRspQrySettlementInfo(请求查询投资者结算结果响应),pSettlementInfo is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.SettlementInfoField = *pSettlementInfo;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQrySettlementInfo, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQrySettlementInfo(请求查询投资者结算结果响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );
		if(pRspInfo) memcpy(&m_RefPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
    }
    else
    {
        pSettlementInfo= &tmp.SettlementInfoField;
        LOG_INFO("OnRspQrySettlementInfo(请求查询投资者结算结果响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t SequenceNo=[%d],\n"
                        "\t\t\t Content=[%s]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pSettlementInfo->TradingDay, pSettlementInfo->SettlementID,pSettlementInfo->BrokerID,pSettlementInfo->InvestorID,pSettlementInfo->SequenceNo,
                        pSettlementInfo->Content
                        );
    }
}

///请求查询转帐银行响应
void CTraderSpi::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

   LOG_INFO("OnRspQryTransferBank,Unimplemented");
}

///请求查询投资者持仓明细响应
void CTraderSpi::OnRspQryInvestorPositionDetail(PlatformStru_Position *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
//void CTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryInvestorPositionDetail tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pInvestorPositionDetail)
    {
        LOG_INFO("OnRspQryInvestorPositionDetail(请求查询投资者持仓明细响应),pInvestorPositionDetail is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
		CopyPositionDetailField( tmp.InvestorPositionDetailField, *pInvestorPositionDetail);
    }


    m_RefPackageCache.SaveDataTo(BID_RspQryInvestorPositionDetail, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryInvestorPositionDetail(请求查询投资者持仓明细响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        //LOG_INFO("OnRspQryInvestorPositionDetail(请求查询投资者持仓明细响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
        //                "\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\t Direction=[%d],\n"
        //                "\t\t\t OpenDate=[%s],\t TradeID=[%s],\t Volume=[%d],\t OpenPrice=[%g],\t TradingDay=[%s],\n"
        //                "\t\t\t SettlementID=[%d],\t  TradeType=[%d],\t CombInstrumentID=[%s],\t ExchangeID=[%s],\t CloseProfitByDate=[%g],\n"
        //                "\t\t\t CloseProfitByTrade=[%g],\t PositionProfitByDate=[%g],\t PositionProfitByTrade=[%g],\t Margin=[%g],\t ExchMargin=[%g],\n"
        //                "\t\t\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g],\t LastSettlementPrice=[%g],\t SettlementPrice=[%g],CloseVolume=[%d],\n"
        //                "\t\t\t CloseAmount=[%g]",
        //                tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
        //                pInvestorPositionDetail->InstrumentID, pInvestorPositionDetail->BrokerID,pInvestorPositionDetail->InvestorID,pInvestorPositionDetail->HedgeFlag,pInvestorPositionDetail->Direction,
        //                pInvestorPositionDetail->OpenDate, pInvestorPositionDetail->TradeID,pInvestorPositionDetail->Volume,pInvestorPositionDetail->OpenPrice,pInvestorPositionDetail->TradingDay,
        //                pInvestorPositionDetail->SettlementID, pInvestorPositionDetail->TradeType,pInvestorPositionDetail->CombInstrumentID,pInvestorPositionDetail->ExchangeID,pInvestorPositionDetail->CloseProfitByDate,
        //                pInvestorPositionDetail->CloseProfitByTrade, pInvestorPositionDetail->PositionProfitByDate,pInvestorPositionDetail->PositionProfitByTrade,pInvestorPositionDetail->Margin,pInvestorPositionDetail->ExchMargin,
        //                pInvestorPositionDetail->MarginRateByMoney, pInvestorPositionDetail->MarginRateByVolume,pInvestorPositionDetail->LastSettlementPrice,pInvestorPositionDetail->SettlementPrice,pInvestorPositionDetail->CloseVolume,
        //                pInvestorPositionDetail->CloseAmount
        //                );
    }
}

///请求查询客户通知响应
void CTraderSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    LOG_INFO("OnRspQryNotice,Unimplemented");
}

///请求查询结算信息确认响应
void CTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	m_RefPlatFormService.m_bSettlementInfoConfirmed=pSettlementInfoConfirm?true:false;
	m_PlatformParam.TradeStatus=CPlatFormParam::CONNSTATUS_Connected;
	if(pRspInfo) memcpy(&m_RefPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
    LOG_INFO("OnRspQrySettlementInfoConfirm,Unimplemented");
}

///请求查询投资者持仓明细响应
void CTraderSpi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    LOG_INFO("OnRspQryInvestorPositionCombineDetail,Unimplemented");
}

///请求查询转帐流水响应
void CTraderSpi::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    DataRspQryTransferSerial tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pTransferSerial)
    {
        LOG_INFO("OnRspQryTransferSerial(请求查询转帐流水响应),pTransferSerial is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.TransferSerialField = *pTransferSerial;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryTransferSerial, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQryTransferSerial(请求查询转帐流水响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pTransferSerial=& tmp.TransferSerialField;
        LOG_INFO("OnRspQryTransferSerial(请求查询转帐流水响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t PlateSerial=[%d],\t TradeDate=[%s],\t TradingDay=[%s],\t TradeTime=[%s],\t TradeCode=[%s],\n"
                        "\t\t\t SessionID=[%#x],\t BankID=[%s],\t BankBranchID=[%s],\t BankAccType=[%d],\t BankAccount=[%s],\n"
                        "\t\t\t BankSerial=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\t FutureAccType=[%d],\t AccountID=[%s],\n"
                        "\t\t\t InvestorID=[%s],\t FutureSerial=[%d],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\t CurrencyID=[%s],\n"
                        "\t\t\t TradeAmount=[%g],\t CustFee=[%g],BrokerFee=[%g],\t AvailabilityFlag=[%d],\t OperatorCode=[%s],\n"
                        "\t\t\t BankNewAccount=[%s],\t --ErrorID=[%d],\t --ErrorMsg=[%d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pTransferSerial->PlateSerial, pTransferSerial->TradeDate,pTransferSerial->TradingDay,pTransferSerial->TradeTime,pTransferSerial->TradeCode,
                        pTransferSerial->SessionID, pTransferSerial->BankID,pTransferSerial->BankBranchID,pTransferSerial->BankAccType,pTransferSerial->BankAccount,
                        pTransferSerial->BankSerial, pTransferSerial->BrokerID,pTransferSerial->BrokerBranchID,pTransferSerial->FutureAccType,pTransferSerial->AccountID,
                        pTransferSerial->InvestorID, pTransferSerial->FutureSerial,pTransferSerial->IdCardType,pTransferSerial->IdentifiedCardNo,pTransferSerial->CurrencyID,
                        pTransferSerial->TradeAmount,pTransferSerial->CustFee, pTransferSerial->BrokerFee,pTransferSerial->AvailabilityFlag,pTransferSerial->OperatorCode,
                        pTransferSerial->BankNewAccount, pTransferSerial->ErrorID, pTransferSerial->ErrorMsg
                        );
    }
}

///错误应答
void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
LOG_INFO("CTraderSpi::OnRspError,ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
          pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, int(bIsLast));
}

///报单通知
void CTraderSpi::OnRtnOrder(PlatformStru_OrderInfo *pOrder)
{
    if(!pOrder)
    {
        LOG_INFO("OnRtnOrder(报单通知),pOrder is NULL");
        return;
    }

    DataRtnOrder tmp;
    tmp.OrderField = *pOrder;

    m_RefPackageCache.SaveDataTo(BID_RtnOrder, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRtnOrder(报单通知) : \n"
                    "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                    "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                    "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                    "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                    "\t\t\t RequestID=[%d],\t OrderLocalID=[%s],\t ExchangeID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                    "\t\t\t ExchangeInstID=[%s],\t TraderID=[%s],\t InstallID=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\n"
                    "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t OrderSysID=[%s],\t OrderSource=[%d],\t OrderStatus=[%d],\n"
                    "\t\t\t OrderType=[%d],\t VolumeTraded=[%d],\t VolumeTotal=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\n"
                    "\t\t\t ActiveTime=[%s],\t SuspendTime=[%s],\t UpdateTime=[%s],\t CancelTime=[%s],\t ActiveTraderID=[%s]\n"
                    "\t\t\t ClearingPartID=[%s],\t SequenceNo=[%d],\t FrontID=[%d],\t SessionID=[%#x],\t UserProductInfo=[%s],\n"
                    "\t\t\t StatusMsg=[%s],\t UserForceClose=[%d],\t ActiveUserID=[%s],\t BrokerOrderSeq=[%d],RelativeOrderSysID=[%s]",
                    pOrder->BrokerID,pOrder->InvestorID,pOrder->InstrumentID,pOrder->OrderRef,pOrder->UserID,
                    pOrder->OrderPriceType,pOrder->Direction,pOrder->CombOffsetFlag,pOrder->CombHedgeFlag,pOrder->LimitPrice,
                    pOrder->VolumeTotalOriginal,pOrder->TimeCondition,pOrder->GTDDate,pOrder->VolumeCondition,pOrder->MinVolume,
                    pOrder->ContingentCondition,pOrder->StopPrice,pOrder->ForceCloseReason,pOrder->IsAutoSuspend,pOrder->BusinessUnit,
                    pOrder->RequestID,pOrder->OrderLocalID,pOrder->ExchangeID,pOrder->ParticipantID,pOrder->ClientID,
                    pOrder->ExchangeInstID,pOrder->TraderID,pOrder->InstallID,pOrder->OrderSubmitStatus,pOrder->NotifySequence,
                    pOrder->TradingDay,pOrder->SettlementID,pOrder->OrderSysID,pOrder->OrderSource,pOrder->OrderStatus,
                    pOrder->OrderType,pOrder->VolumeTraded,pOrder->VolumeTotal,pOrder->InsertDate,pOrder->InsertTime,
                    pOrder->ActiveTime,pOrder->SuspendTime,pOrder->UpdateTime,pOrder->CancelTime,pOrder->ActiveTraderID,
                    pOrder->ClearingPartID,pOrder->SequenceNo,pOrder->FrontID,pOrder->SessionID,pOrder->UserProductInfo,
                    pOrder->StatusMsg,pOrder->UserForceClose,pOrder->ActiveUserID,pOrder->BrokerOrderSeq,pOrder->RelativeOrderSysID
                    );
}

///成交通知
void CTraderSpi::OnRtnTrade(PlatformStru_TradeInfo *pTrade)
{
    if(!pTrade)
    {
        LOG_INFO("OnRtnTrade(成交通知),pTrade is NULL");
        return;
    }

    DataRtnTrade tmp;
    tmp.TradeField = *pTrade;

    m_RefPackageCache.SaveDataTo(BID_RtnTrade, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRtnTrade(成交通知) : \n"
                    "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                    "\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
                    "\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
                    "\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
                    "\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
                    "\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d]",
                    pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
                    pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
                    pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
                    pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
                    pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
                    pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq
                    );
}

///报单录入错误回报
void CTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    if(!pInputOrder)
    {
        LOG_INFO("OnRspOrderInsert(报单录入错误回报),pInputOrder is NULL");
        return;
    }

    DataRspOrderInsert2 tmp;
    tmp.nRequestID = -1;
    tmp.bIsLast    = true;
    tmp.InputOrderField = *pInputOrder;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspOrderInsert2, 0, 0, 0, sizeof(tmp), &tmp); 
    LOG_INFO("OnErrRtnOrderInsert(报单录入错误回报) : ErrorID=[%d],ErrorMsg=[%s],\n"
                    "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                    "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                    "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                    "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                    "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, 
                    pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
                    pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
                    pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
                    pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
                    pInputOrder->RequestID, pInputOrder->UserForceClose);
}

///报单操作错误回报
void CTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    if(!pOrderAction)
    {
        LOG_INFO("OnErrRtnOrderAction(报单操作错误回报),pOrderAction is NULL");
        return;
    }

    DataRspOrderAction2 tmp;
    tmp.OrderActionField = *pOrderAction;
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    m_RefPackageCache.SaveDataTo(BID_RspOrderAction2, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnErrRtnOrderAction(报单操作错误回报) : ErrorID=[%d],ErrorMsg=[%s],\n"
                    "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                    "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                    "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t ActionDate=[%s],\t ActionTime=[%s],\t TraderID=[%s],\n"
                    "\t\t\t InstallID=[%d],\t OrderLocalID=[%s],\t ActionLocalID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                    "\t\t\t BusinessUnit=[%s],\t OrderActionStatus=[%d],\t UserID=[%s],\t StatusMsg=[%s],\t InstrumentID=[%s]",
                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,
                    pOrderAction->BrokerID, pOrderAction->InvestorID, pOrderAction->OrderActionRef, pOrderAction->OrderRef,pOrderAction->RequestID,
                    pOrderAction->FrontID, pOrderAction->SessionID, pOrderAction->ExchangeID, pOrderAction->OrderSysID,pOrderAction->ActionFlag,
                    pOrderAction->LimitPrice, pOrderAction->VolumeChange, pOrderAction->ActionDate, pOrderAction->ActionTime,pOrderAction->TraderID,
                    pOrderAction->InstallID, pOrderAction->OrderLocalID, pOrderAction->ActionLocalID, pOrderAction->ParticipantID,pOrderAction->ClientID,
                    pOrderAction->BusinessUnit, pOrderAction->OrderActionStatus, pOrderAction->UserID, pOrderAction->StatusMsg,pOrderAction->InstrumentID
    			);
 }

///合约交易状态通知
void CTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
    if (!pInstrumentStatus)
    {
        LOG_INFO("OnRtnInstrumentStatus(合约交易状态通知),pInstrumentStatus is NULL");
        return;
    }
    DataInstrumentStatus tmp;
    tmp.InstrumentStatusField = *pInstrumentStatus;

    m_RefPackageCache.SaveDataTo(BID_RtnInstrumentStatus, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRtnInstrumentStatus(合约交易状态通知):\n"
                     "\t\t\t ExchangeID=[%s],\t ExchangeInstID=[%s],\t SettlementGroupID=[%s],\t InstrumentID=[%s],\t InstrumentStatus=[%d],\n"
                     "\t\t\t TradingSegmentSN=[%d],\t EnterTime=[%s],\t EnterReason=[%d]",
                     pInstrumentStatus->ExchangeID,
                     pInstrumentStatus->ExchangeInstID,
                     pInstrumentStatus->SettlementGroupID,
                     pInstrumentStatus->InstrumentID,
                     pInstrumentStatus->InstrumentStatus,
                     pInstrumentStatus->TradingSegmentSN,
                     pInstrumentStatus->EnterTime,
                     pInstrumentStatus->EnterReason);
}

///交易通知
void CTraderSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo)
{
    if(!pTradingNoticeInfo)
    {
        LOG_INFO("OnRtnTradingNotice(交易通知),pTradingNoticeInfo is NULL");
        return;
    }
    DataTradingNoticeInfoField tmp;
    tmp.TradingNoticeInfoField=*pTradingNoticeInfo;

    m_RefPackageCache.SaveDataTo(BID_RtnTradingNotice, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRtnTradingNotice(交易通知):\n"
           "\t\t\t BrokerID=[%s],\tInvestorID=[%s],SendTime=[%s],\n"
           "\t\t\t FieldContent=[%s],\n"
           "\t\t\t SequenceSeries=[%d],SequenceNo=[%d]",
             pTradingNoticeInfo->BrokerID,
             pTradingNoticeInfo->InvestorID,
             pTradingNoticeInfo->SendTime,
             pTradingNoticeInfo->FieldContent,
             pTradingNoticeInfo->SequenceSeries,
             pTradingNoticeInfo->SequenceNo);
}
void CTraderSpi::OnRtnAlertMessage(PlatformStru_MessageStruct *pMessage)
{
    if(!pMessage)
    {
        LOG_INFO("OnRtnAlertMessage(Pats Alert Message),pMessage is NULL");
        return;
    }
	DataMessageStruct tmp;
	tmp.MessageStructField=*pMessage;
    m_RefPackageCache.SaveDataTo(BID_AlertMessage, 0, 0, 0, sizeof(tmp), &tmp);
    LOG_INFO("OnRtnAlertMessage(Pats Alert Message):\n"
           "\t\t\t MsgID=[%s],\nMsgText=[%s],IsAlert=[%c],Status=[%c]\n",
		   pMessage->MsgID,
		   pMessage->MsgText,
		   pMessage->IsAlert,
		   pMessage->Status);

}
///请求查询签约银行响应
void CTraderSpi::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{


    if(!pContractBank)
    {
        LOG_INFO("OnRspQryContractBank(期货发起查询银行余额通知),pContractBank is NULL");
        return;
    }
    DataRspQryContractBank tmp;
    tmp.ContractBankField=*pContractBank;

    m_RefPackageCache.SaveDataTo(BID_RspQryContractBank, 0, 0, 0, sizeof(tmp), &tmp);

    LOG_INFO("OnRtnTradingNotice(请求查询签约银行响应):\n"
           "\t\t\t BrokerID=[%s],\tBankID=[%s],BankBrchID=[%s],\n"
           "\t\t\t BankName=[%s],\n",
             pContractBank->BrokerID,
             pContractBank->BankID,
             pContractBank->BankBrchID,
             pContractBank->BankName);
}

///请求查询交易通知响应
void CTraderSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	 if(!pTradingNotice)
    {
        LOG_INFO("OnRspQryContractBank(定时更新资金),pTradingNotice is NULL");
        return;
    }
    DataRspQryTradingNotice tmp;

    m_RefPackageCache.SaveDataTo(BID_RspQryTradingNotice, 0, 0, 0, sizeof(tmp), &tmp);

}

///请求查询经纪公司交易参数响应
void CTraderSpi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

    LOG_INFO("OnRspQryBrokerTradingParams,Unimplemented");
}

///请求查询经纪公司交易算法响应
void CTraderSpi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

    LOG_INFO("OnRspQryBrokerTradingAlgos,Unimplemented");
}

///银行发起银行资金转期货通知
void CTraderSpi::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer)
{
LOG_INFO("OnRtnFromBankToFutureByBank,Unimplemented");
}

///银行发起期货资金转银行通知
void CTraderSpi::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer)
{
LOG_INFO("OnRtnFromFutureToBankByBank,Unimplemented");
}

///银行发起冲正银行转期货通知
void CTraderSpi::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromBankToFutureByBank,Unimplemented");
}

///银行发起冲正期货转银行通知
void CTraderSpi::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromFutureToBankByBank,Unimplemented");
}

///期货发起银行资金转期货通知
void CTraderSpi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
if(!pRspTransfer)
    {
        LOG_INFO("OnRtnFromBankToFutureByFuture(期货发起银行资金转期货通知),pRspTransfer is NULL");
        return;
    }
    DataRtnFromBankToFutureByFuture tmp;
    tmp.RspTransferField=*pRspTransfer;

    m_RefPackageCache.SaveDataTo(BID_RtnFromBankToFutureByFuture, 0, 0, 0, sizeof(tmp), &tmp);

//LOG_INFO("OnRtnFromBankToFutureByFuture(期货发起银行资金转期货通知) : TradeCode=[%s],BankID=[%s],BankBranchID=[%d],BrokerID=[%d],\n"
//                        "\t\t\t BrokerBranchID=[%s],\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\n"
//                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t PlateSerial=[%d],\n"
//                        "\t\t\t LastFragment=[%s],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
//                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
//                        "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
//                        "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
//                        "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
//                        "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
//                        "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d],\t ErrorID=[%d],\t ErrorMsg=[%s]",
//                        pRspTransfer->TradeCode, pRspTransfer->BankID,pRspTransfer->BankBranchID,pRspTransfer->BrokerID,pRspTransfer->BrokerBranchID,
//                        pRspTransfer->TradeDate, pRspTransfer->TradeTime,pRspTransfer->BankSerial,pRspTransfer->TradingDay,pRspTransfer->TradeDate,
//                        pRspTransfer->TradeTime, pRspTransfer->BankSerial, pRspTransfer->PlateSerial,
//                        pRspTransfer->LastFragment, pRspTransfer->SessionID,pRspTransfer->CustomerName,pRspTransfer->IdCardType,pRspTransfer->IdentifiedCardNo,
//                        pRspTransfer->CustType, pRspTransfer->BankAccount,pRspTransfer->BankPassWord,pRspTransfer->AccountID,pRspTransfer->Password,
//                        pRspTransfer->InstallID,pRspTransfer->FutureSerial, pRspTransfer->UserID,pRspTransfer->VerifyCertNoFlag,pRspTransfer->CurrencyID,
//                        pRspTransfer->TradeAmount,pRspTransfer->FutureFetchAmount, pRspTransfer->FeePayFlag,pRspTransfer->CustFee,pRspTransfer->BrokerFee,
//                        pRspTransfer->Message, pRspTransfer->Digest, pRspTransfer->BankAccType,pRspTransfer->DeviceID,pRspTransfer->BankSecuAccType,
//                        pRspTransfer->BrokerIDByBank, pRspTransfer->BankSecuAcc, pRspTransfer->BankPwdFlag,pRspTransfer->SecuPwdFlag,pRspTransfer->OperNo,
//                        pRspTransfer->RequestID, pRspTransfer->TID, pRspTransfer->TransferStatus,pRspTransfer->ErrorID,pRspTransfer->ErrorMsg
//                        );
}

///期货发起期货资金转银行通知
void CTraderSpi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
if(!pRspTransfer)
    {
        LOG_INFO("OnRtnFromFutureToBankByFuture(期货发起期货资金转银行通知),pRspTransfer is NULL");
        return;
    }
    DataRtnFromFutureToBankByFuture tmp;
    tmp.RspTransferField=*pRspTransfer;

    m_RefPackageCache.SaveDataTo(BID_RtnFromFutureToBankByFuture, 0, 0, 0, sizeof(tmp), &tmp);

//LOG_INFO("OnRtnFromFutureToBankByFuture(期货发起期货资金转银行通知) : TradeCode=[%s],BankID=[%s],BankBranchID=[%d],BrokerID=[%d],\n"
//                        "\t\t\t BrokerBranchID=[%s],\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\n"
//                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t PlateSerial=[%d],\n"
//                        "\t\t\t LastFragment=[%s],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
//                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
//                        "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
//                        "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
//                        "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
//                        "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
//                        "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d],\t ErrorID=[%d],\t ErrorMsg=[%s]",
//                        pRspTransfer->TradeCode, pRspTransfer->BankID,pRspTransfer->BankBranchID,pRspTransfer->BrokerID,pRspTransfer->BrokerBranchID,
//                        pRspTransfer->TradeDate, pRspTransfer->TradeTime,pRspTransfer->BankSerial,pRspTransfer->TradingDay,pRspTransfer->TradeDate,
//                        pRspTransfer->TradeTime, pRspTransfer->BankSerial, pRspTransfer->PlateSerial,
//                        pRspTransfer->LastFragment, pRspTransfer->SessionID,pRspTransfer->CustomerName,pRspTransfer->IdCardType,pRspTransfer->IdentifiedCardNo,
//                        pRspTransfer->CustType, pRspTransfer->BankAccount,pRspTransfer->BankPassWord,pRspTransfer->AccountID,pRspTransfer->Password,
//                        pRspTransfer->InstallID,pRspTransfer->FutureSerial, pRspTransfer->UserID,pRspTransfer->VerifyCertNoFlag,pRspTransfer->CurrencyID,
//                        pRspTransfer->TradeAmount,pRspTransfer->FutureFetchAmount, pRspTransfer->FeePayFlag,pRspTransfer->CustFee,pRspTransfer->BrokerFee,
//                        pRspTransfer->Message, pRspTransfer->Digest, pRspTransfer->BankAccType,pRspTransfer->DeviceID,pRspTransfer->BankSecuAccType,
//                        pRspTransfer->BrokerIDByBank, pRspTransfer->BankSecuAcc, pRspTransfer->BankPwdFlag,pRspTransfer->SecuPwdFlag,pRspTransfer->OperNo,
//                        pRspTransfer->RequestID, pRspTransfer->TID, pRspTransfer->TransferStatus,pRspTransfer->ErrorID,pRspTransfer->ErrorMsg
//                        );
}

///系统运行时期货端手工发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
void CTraderSpi::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromBankToFutureByFutureManual,Unimplemented");
}

///系统运行时期货端手工发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
void CTraderSpi::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromFutureToBankByFutureManual,Unimplemented");
}

///期货发起查询银行余额通知
void CTraderSpi::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount)
{
    if(!pNotifyQueryAccount)
    {
        LOG_INFO("OnRtnQueryBankBalanceByFuture(期货发起查询银行余额通知),pNotifyQueryAccount is NULL");
        return;
    }
    DataRtnQueryBankAccountMoneyByFuture tmp;
    tmp.NotifyQueryAccountField=*pNotifyQueryAccount;

    m_RefPackageCache.SaveDataTo(BID_RtnQueryBankAccountMoneyByFuture, 0, 0, 0, sizeof(tmp), &tmp);
    //LOG_INFO("OnRtnTradingNotice(交易通知):\n"
    //       "\t\t\t BrokerID=[%s],\tInvestorID=[%s],SendTime=[%s],\n"
    //       "\t\t\t FieldContent=[%s],\n"
    //       "\t\t\t SequenceSeries=[%d],SequenceNo=[%d]",
    //         pTradingNoticeInfo->BrokerID,
    //         pTradingNoticeInfo->InvestorID,
    //         pTradingNoticeInfo->SendTime,
    //         pTradingNoticeInfo->FieldContent,
    //         pTradingNoticeInfo->SequenceSeries,
    //         pTradingNoticeInfo->SequenceNo);
}

///请求查询银期签约关系响应
void CTraderSpi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    DataRspQryAccountregister tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;



    if(!pAccountregister)
    {
        LOG_INFO("OnRspQryAccountregister(请求查询银期签约关系响应),pAccountregister is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.Accountregister=*pAccountregister;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQryAccountregister, 0, 0, 0, sizeof(tmp), &tmp);

    ////log
    //if(tmp.RspInfoField.ErrorID!=0)
    //{
    //    LOG_INFO("OnRspQryTransferSerial(请求查询银期签约关系响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
    //                     tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
    //                     );

    //}
    //else
    //{
    //    pTransferSerial=& tmp.TransferSerialField;
    //    LOG_INFO("OnRspQryTransferSerial(请求查询银期签约关系响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
    //                    "\t\t\t PlateSerial=[%d],\t TradeDate=[%s],\t TradingDay=[%s],\t TradeTime=[%s],\t TradeCode=[%s],\n"
    //                    "\t\t\t SessionID=[%#x],\t BankID=[%s],\t BankBranchID=[%s],\t BankAccType=[%d],\t BankAccount=[%s],\n"
    //                    "\t\t\t BankSerial=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\t FutureAccType=[%d],\t AccountID=[%s],\n"
    //                    "\t\t\t InvestorID=[%s],\t FutureSerial=[%d],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\t CurrencyID=[%s],\n"
    //                    "\t\t\t TradeAmount=[%g],\t CustFee=[%g],BrokerFee=[%g],\t AvailabilityFlag=[%d],\t OperatorCode=[%s],\n"
    //                    "\t\t\t BankNewAccount=[%s],\t --ErrorID=[%d],\t --ErrorMsg=[%d]",
    //                    tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
    //                    pTransferSerial->PlateSerial, pTransferSerial->TradeDate,pTransferSerial->TradingDay,pTransferSerial->TradeTime,pTransferSerial->TradeCode,
    //                    pTransferSerial->SessionID, pTransferSerial->BankID,pTransferSerial->BankBranchID,pTransferSerial->BankAccType,pTransferSerial->BankAccount,
    //                    pTransferSerial->BankSerial, pTransferSerial->BrokerID,pTransferSerial->BrokerBranchID,pTransferSerial->FutureAccType,pTransferSerial->AccountID,
    //                    pTransferSerial->InvestorID, pTransferSerial->FutureSerial,pTransferSerial->IdCardType,pTransferSerial->IdentifiedCardNo,pTransferSerial->CurrencyID,
    //                    pTransferSerial->TradeAmount,pTransferSerial->CustFee, pTransferSerial->BrokerFee,pTransferSerial->AvailabilityFlag,pTransferSerial->OperatorCode,
    //                    pTransferSerial->BankNewAccount, pTransferSerial->ErrorID, pTransferSerial->ErrorMsg
    //                    );
    //}


}

///期货发起银行资金转期货错误回报
void CTraderSpi::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnBankToFutureByFuture,Unimplemented");
}

///期货发起期货资金转银行错误回报
void CTraderSpi::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnFutureToBankByFuture,Unimplemented");
}

///系统运行时期货端手工发起冲正银行转期货错误回报
void CTraderSpi::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnRepealBankToFutureByFutureManual,Unimplemented");
}

///系统运行时期货端手工发起冲正期货转银行错误回报
void CTraderSpi::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnRepealFutureToBankByFutureManual,Unimplemented");
}

///期货发起查询银行余额错误回报
void CTraderSpi::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo)
{
LOG_INFO("OnErrRtnQueryBankBalanceByFuture,Unimplemented");
}

///期货发起冲正银行转期货请求，银行处理完毕后报盘发回的通知
void CTraderSpi::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromBankToFutureByFuture,Unimplemented");
}

///期货发起冲正期货转银行请求，银行处理完毕后报盘发回的通知
void CTraderSpi::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
LOG_INFO("OnRtnRepealFromFutureToBankByFuture,Unimplemented");
}

///期货发起银行资金转期货应答
void CTraderSpi::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    DataRspFromBankToFutureByFuture tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pReqTransfer)
    {
        LOG_INFO("OnRspFromBankToFutureByFuture(期货发起银行资金转期货应答),pReqTransfer is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.TransferField = *pReqTransfer;
    }

    m_RefPackageCache.SaveDataTo(BID_RspFromBankToFutureByFuture, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspFromBankToFutureByFuture(期货发起银行资金转期货应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pReqTransfer=& tmp.TransferField;
        LOG_INFO("OnRspFromBankToFutureByFuture(期货发起银行资金转期货应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
                        "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
                        "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
                        "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
                        "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
                        "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
                        "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pReqTransfer->TradeCode, pReqTransfer->BankID,pReqTransfer->BankBranchID,pReqTransfer->BrokerID,pReqTransfer->BrokerBranchID,
                        pReqTransfer->TradeDate, pReqTransfer->TradeTime,pReqTransfer->BankSerial,pReqTransfer->TradingDay,pReqTransfer->PlateSerial,
                        pReqTransfer->LastFragment, pReqTransfer->SessionID,pReqTransfer->CustomerName,pReqTransfer->IdCardType,pReqTransfer->IdentifiedCardNo,
                        pReqTransfer->CustType, pReqTransfer->BankAccount,pReqTransfer->BankPassWord,pReqTransfer->AccountID,pReqTransfer->Password,
                        pReqTransfer->InstallID,pReqTransfer->FutureSerial, pReqTransfer->UserID,pReqTransfer->VerifyCertNoFlag,pReqTransfer->CurrencyID,
                        pReqTransfer->TradeAmount,pReqTransfer->FutureFetchAmount, pReqTransfer->FeePayFlag,pReqTransfer->CustFee,pReqTransfer->BrokerFee,
                        pReqTransfer->Message, pReqTransfer->Digest, pReqTransfer->BankAccType,pReqTransfer->DeviceID,pReqTransfer->BankSecuAccType,
                        pReqTransfer->BrokerIDByBank, pReqTransfer->BankSecuAcc, pReqTransfer->BankPwdFlag,pReqTransfer->SecuPwdFlag,pReqTransfer->OperNo,
                        pReqTransfer->RequestID, pReqTransfer->TID, pReqTransfer->TransferStatus
                        );
    }
}

///期货发起期货资金转银行应答
void CTraderSpi::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    DataRspFromFutureToBankByFuture tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;

    if(!pReqTransfer)
    {
        LOG_INFO("OnRspFromFutureToBankByFuture(期货发起期货资金转银行应答),pReqTransfer is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.TransferField = *pReqTransfer;
    }

    m_RefPackageCache.SaveDataTo(BID_RspFromFutureToBankByFuture, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspFromFutureToBankByFuture(期货发起期货资金转银行应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pReqTransfer=& tmp.TransferField;
        LOG_INFO("OnRspFromFutureToBankByFuture(期货发起期货资金转银行应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
                        "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
                        "\t\t\t InstallID=[%d],\t FutureSerial=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
                        "\t\t\t TradeAmount=[%g],\t FutureFetchAmount=[%g],\t FeePayFlag=[%d],\t CustFee=[%g],BrokerFee=[%g],\n"
                        "\t\t\t Message=[%s],\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],\n"
                        "\t\t\t BrokerIDByBank=[%s],\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],\n"
                        "\t\t\t RequestID=[%d],\t TID=[%d],\t TransferStatus=[d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pReqTransfer->TradeCode, pReqTransfer->BankID,pReqTransfer->BankBranchID,pReqTransfer->BrokerID,pReqTransfer->BrokerBranchID,
                        pReqTransfer->TradeDate, pReqTransfer->TradeTime,pReqTransfer->BankSerial,pReqTransfer->TradingDay,pReqTransfer->PlateSerial,
                        pReqTransfer->LastFragment, pReqTransfer->SessionID,pReqTransfer->CustomerName,pReqTransfer->IdCardType,pReqTransfer->IdentifiedCardNo,
                        pReqTransfer->CustType, pReqTransfer->BankAccount,pReqTransfer->BankPassWord,pReqTransfer->AccountID,pReqTransfer->Password,
                        pReqTransfer->InstallID,pReqTransfer->FutureSerial, pReqTransfer->UserID,pReqTransfer->VerifyCertNoFlag,pReqTransfer->CurrencyID,
                        pReqTransfer->TradeAmount,pReqTransfer->FutureFetchAmount, pReqTransfer->FeePayFlag,pReqTransfer->CustFee,pReqTransfer->BrokerFee,
                        pReqTransfer->Message, pReqTransfer->Digest, pReqTransfer->BankAccType,pReqTransfer->DeviceID,pReqTransfer->BankSecuAccType,
                        pReqTransfer->BrokerIDByBank, pReqTransfer->BankSecuAcc, pReqTransfer->BankPwdFlag,pReqTransfer->SecuPwdFlag,pReqTransfer->OperNo,
                        pReqTransfer->RequestID, pReqTransfer->TID, pReqTransfer->TransferStatus
                        );
    }

}

///期货发起查询银行余额应答
void CTraderSpi::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    DataRspQueryBankAccountMoneyByFuture tmp;
    tmp.nRequestID = nRequestID;
    tmp.bIsLast    = bIsLast;    
    if(pRspInfo)
        tmp.RspInfoField = *pRspInfo;



    if(!pReqQueryAccount)
    {
        LOG_INFO("OnRspQueryBankAccountMoneyByFuture(期货发起查询银行余额应答),pReqQueryAccount is NULL");
        if(tmp.RspInfoField.ErrorID==0)
        {
            memset(&tmp.RspInfoField, 0, sizeof(tmp.RspInfoField));
            tmp.RspInfoField.ErrorID=CustomErrorID;
            strcpy(tmp.RspInfoField.ErrorMsg, CustomErrorMsg);           
        }
    }
    else
    {
        tmp.QueryAccountField = *pReqQueryAccount;
    }

    m_RefPackageCache.SaveDataTo(BID_RspQueryBankAccountMoneyByFuture, 0, 0, 0, sizeof(tmp), &tmp); 

    //log
    if(tmp.RspInfoField.ErrorID!=0)
    {
        LOG_INFO("OnRspQueryBankAccountMoneyByFuture(期货发起查询银行余额应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]",
                         tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast
                         );

    }
    else
    {
        pReqQueryAccount=& tmp.QueryAccountField;
 /*       LOG_INFO("OnRspQueryBankAccountMoneyByFuture(期货发起查询银行余额应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
                        "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[%s],\t AccountID=[%s],\t Password=[%s],\n"
                        "\t\t\t FutureSerial=[%d],\t InstallID=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
                        "\t\t\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],BrokerIDByBank=[%s],\n"
                        "\t\t\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],RequestID=[%d],\n"
                        "\t\t\t TID=[%d]",
                        tmp.RspInfoField.ErrorID, tmp.RspInfoField.ErrorMsg, nRequestID,bIsLast,
                        pReqQueryAccount->TradeCode, pReqQueryAccount->BankID,pReqQueryAccount->BankBranchID,pReqQueryAccount->BrokerID,pReqQueryAccount->BrokerBranchID,
                        pReqQueryAccount->TradeDate, pReqQueryAccount->TradeTime,pReqQueryAccount->BankSerial,pReqQueryAccount->TradingDay,pReqQueryAccount->PlateSerial,
                        pReqQueryAccount->LastFragment, pReqQueryAccount->SessionID,pReqQueryAccount->CustomerName,pReqQueryAccount->IdCardType,pReqQueryAccount->IdentifiedCardNo,
                        pReqQueryAccount->CustType, pReqQueryAccount->BankAccount,pReqQueryAccount->BankPassWord,pReqQueryAccount->AccountID,pReqQueryAccount->Password,
                        pReqQueryAccount->FutureSerial, pReqQueryAccount->InstallID,pReqQueryAccount->UserID,pReqQueryAccount->VerifyCertNoFlag,pReqQueryAccount->CurrencyID,
                        pReqQueryAccount->Digest, pReqQueryAccount->BankAccType,pReqQueryAccount->DeviceID,pReqQueryAccount->BankSecuAccType,pReqQueryAccount->BrokerIDByBank,
                        pReqQueryAccount->BankSecuAcc, pReqQueryAccount->BankPwdFlag,pReqQueryAccount->SecuPwdFlag,pReqQueryAccount->OperNo,pReqQueryAccount->RequestID,
                        pReqQueryAccount->TID
                        );*/
    }
}
void CTraderSpi::OnRtnConnectivityStatus(ConnectivityStatus *pChangeAccount)
{
    if(!pChangeAccount)
    {
        LOG_INFO("OnRtnConnectivityStatus(连接状态),pChangeAccount is NULL");
        return;
    }

    DataConnectivityStatus tmp;
	tmp.StatusField = *pChangeAccount;

    m_RefPackageCache.SaveDataTo(BID_ConnectivityStatus, 0, 0, 0, sizeof(tmp), &tmp);
}
void CTraderSpi::OnRtnExchangeRate(char* Currency)
{
    if(!Currency)
    {
        LOG_INFO("OnRtnExchangeRate(货币汇率),Currency is NULL");
        return;
    }

    DataExchangeRate tmp;
	strcpy(tmp.RateField,Currency);

    m_RefPackageCache.SaveDataTo(BID_ExchangeRate, 0, 0, 0, sizeof(tmp), &tmp);
}
void CTraderSpi::CopyInstrumentField( PlatformStru_InstrumentInfo& data, const CThostFtdcInstrumentField& instrumentField )
{
	strcpy(data.InstrumentID, instrumentField.InstrumentID);
	strcpy(data.ExchangeID, instrumentField.ExchangeID);
	strcpy(data.InstrumentName, instrumentField.InstrumentName);
	strcpy(data.ExchangeInstID, instrumentField.ExchangeInstID);
	strcpy(data.ProductID, instrumentField.ProductID);
	data.ProductClass = instrumentField.ProductClass;
	data.DeliveryYear = instrumentField.DeliveryYear;
	data.DeliveryMonth = instrumentField.DeliveryMonth;
	data.MaxMarketOrderVolume = instrumentField.MaxMarketOrderVolume;
	data.MinMarketOrderVolume = instrumentField.MinMarketOrderVolume;
	data.MaxLimitOrderVolume = instrumentField.MaxLimitOrderVolume;
	data.MinLimitOrderVolume = instrumentField.MinLimitOrderVolume;
	data.VolumeMultiple = instrumentField.VolumeMultiple;
	data.PriceTick = instrumentField.PriceTick;
	strcpy(data.CreateDate,instrumentField.CreateDate);
	strcpy(data.OpenDate,instrumentField.OpenDate);
	strcpy(data.ExpireDate, instrumentField.ExpireDate);
	strcpy(data.StartDelivDate, instrumentField.StartDelivDate);
	strcpy(data.EndDelivDate, instrumentField.EndDelivDate);
	data.InstLifePhase = instrumentField.InstLifePhase;
	data.IsTrading = instrumentField.IsTrading;
	data.PositionType = instrumentField.PositionType;
	data.PositionDateType = instrumentField.PositionType;
	data.LongMarginRatio = instrumentField.LongMarginRatio;
	data.ShortMarginRatio = instrumentField.ShortMarginRatio;
}

void CTraderSpi::CopyTradeRecordField( PlatformStru_TradeInfo& data, const CThostFtdcTradeField& tradeField )
{
	strcpy(data.BrokerID, tradeField.BrokerID);
	strcpy(data.InvestorID, tradeField.InvestorID);
	strcpy(data.InstrumentID, tradeField.InstrumentID);
	strcpy(data.OrderRef, tradeField.OrderRef);
	strcpy(data.UserID, tradeField.UserID);
	strcpy(data.ExchangeID, tradeField.ExchangeID);
	strcpy(data.TradeID, tradeField.TradeID);
	data.Direction = tradeField.Direction;
	strcpy(data.OrderSysID, tradeField.OrderSysID);
	strcpy(data.ParticipantID, tradeField.ParticipantID);
	strcpy(data.ClientID, tradeField.ClientID);
	data.TradingRole = tradeField.TradingRole;
	strcpy(data.ExchangeInstID, tradeField.ExchangeInstID);
	data.OffsetFlag = tradeField.OffsetFlag;
	data.HedgeFlag = tradeField.HedgeFlag;
	data.Price = tradeField.Price;
	data.Volume = tradeField.Volume;
	strcpy(data.TradeDate, tradeField.TradeDate);
	strcpy(data.TradeTime, tradeField.TradeTime);
	data.TradeType = tradeField.TradeType;
	data.PriceSource = tradeField.PriceSource;
	strcpy(data.TraderID, tradeField.TraderID);
	strcpy(data.OrderLocalID, tradeField.OrderLocalID);
	strcpy(data.ClearingPartID, tradeField.ClearingPartID);
	strcpy(data.BusinessUnit, data.BusinessUnit);
	data.SequenceNo = tradeField.SequenceNo;
	strcpy(data.TradingDay, tradeField.TradingDay);
	data.SettlementID = tradeField.SettlementID;
	data.BrokerOrderSeq = tradeField.BrokerOrderSeq;
	data.TradeSource = tradeField.TradeSource;

	strncpy(data.szAccount,tradeField.InvestorID,sizeof(data.szAccount)-1);
}

void CTraderSpi::CopyPositionField( PlatformStru_Position&data, const CThostFtdcInvestorPositionField& positionField )
{
	strncpy(data.InstrumentID,positionField.InstrumentID,sizeof(data.InstrumentID)-1);
	strncpy(data.BrokerID,positionField.BrokerID,sizeof(data.BrokerID)-1);
	strncpy(data.InvestorID,positionField.InvestorID,sizeof(data.InvestorID)-1);
	data.PosiDirection=positionField.PosiDirection;
	data.HedgeFlag=positionField.HedgeFlag;
	data.PositionDate=positionField.PositionDate;
	data.YdPosition=positionField.YdPosition;
	data.Position=positionField.Position;
	data.LongFrozen=positionField.LongFrozen;
	data.ShortFrozen=positionField.ShortFrozen;
	data.LongFrozenAmount=positionField.LongFrozenAmount;
	data.ShortFrozenAmount=positionField.ShortFrozenAmount;
	data.OpenVolume=positionField.OpenVolume;
	data.CloseVolume=positionField.CloseVolume;
	data.OpenAmount=positionField.OpenAmount;
	data.CloseAmount=positionField.CloseAmount;
	data.PositionCost=positionField.PositionCost;
	data.PreMargin=positionField.PreMargin;
	data.UseMargin=positionField.UseMargin;
	data.FrozenMargin=positionField.FrozenMargin;
	data.FrozenCash=positionField.FrozenCash;
	data.FrozenCommission=positionField.FrozenCommission;
	data.CashIn=positionField.CashIn;
	data.Commission=positionField.Commission;
	data.CloseProfit=positionField.CloseProfit;
	data.PositionProfit=positionField.PositionProfit;
	data.PreSettlementPrice=positionField.PreSettlementPrice;
	data.SettlementPrice=positionField.SettlementPrice;
	strncpy(data.TradingDay,positionField.TradingDay,sizeof(data.TradingDay)-1);
	data.SettlementID=positionField.SettlementID;
	data.OpenCost=positionField.OpenCost;
	data.ExchangeMargin=positionField.ExchangeMargin;
	data.CombPosition=positionField.CombPosition;
	data.CombLongFrozen=positionField.CombLongFrozen;
	data.CombShortFrozen=positionField.CombShortFrozen;
	data.CloseProfitByDate=positionField.CloseProfitByDate;
	data.CloseProfitByTrade=positionField.CloseProfitByTrade;
	data.TodayPosition=positionField.TodayPosition;
	data.MarginRateByMoney=positionField.MarginRateByMoney;
	data.MarginRateByVolume=positionField.MarginRateByVolume;

	strncpy(data.szAccount,positionField.InvestorID,sizeof(data.szAccount)-1);
}

void CTraderSpi::CopyPositionDetailField( PlatformStru_PositionDetail&data, const CThostFtdcInvestorPositionDetailField& positionField )
{
	strcpy( data.InstrumentID, positionField.InstrumentID);
	strcpy( data.BrokerID, positionField.BrokerID);
	strcpy( data.InvestorID, positionField.InvestorID);
	data.HedgeFlag = positionField.HedgeFlag;
	data.Direction = positionField.Direction;
	strcpy( data.OpenDate, positionField.OpenDate);
	strcpy( data.TradeID, positionField.TradeID);
	data.Volume = positionField.Volume;
	data.OpenPrice = positionField.OpenPrice;
	strcpy( data.TradingDay, positionField.TradingDay);
	data.SettlementID = positionField.SettlementID;
	data.TradeType = positionField.TradeType;
	strcpy( data.CombInstrumentID, positionField.CombInstrumentID);
	strcpy( data.ExchangeID, positionField.ExchangeID);
	data.CloseProfitByDate = positionField.CloseProfitByDate;
	data.CloseProfitByTrade = positionField.CloseProfitByTrade;
	data.PositionProfitByDate = positionField.PositionProfitByDate;
	data.PositionProfitByTrade = positionField.PositionProfitByTrade;
	data.Margin = positionField.Margin;
	data.ExchMargin = positionField.ExchMargin;
	data.MarginRateByMoney = positionField.MarginRateByMoney;
	data.MarginRateByVolume = positionField.MarginRateByVolume;
	data.LastSettlementPrice = positionField.LastSettlementPrice;
	data.SettlementPrice = positionField.SettlementPrice;
	data.CloseVolume = positionField.CloseVolume;
	data.CloseAmount = positionField.CloseAmount;

	strncpy(data.szAccount,positionField.InvestorID,sizeof(data.szAccount)-1);
}

void CTraderSpi::CopyPositionDetailField( PlatformStru_PositionDetail&data, const PlatformStru_Position& positionField )
{
	strcpy( data.InstrumentID, positionField.InstrumentID);
	strcpy( data.BrokerID, positionField.BrokerID);
	strcpy( data.InvestorID, positionField.InvestorID);
	data.HedgeFlag = positionField.HedgeFlag;
	data.Direction = positionField.PosiDirection==THOST_FTDC_PD_Long ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
	//strcpy( data.OpenDate, positionField.OpenDate);
	//strcpy( data.TradeID, positionField.TradeID);
	data.Volume = positionField.Position;
	data.OpenPrice = positionField.PositionCost;
	strcpy( data.TradingDay, positionField.TradingDay);
	data.SettlementID = positionField.SettlementID;
	data.TradeType = THOST_FTDC_TRDT_Common;//positionField.TradeType;
	//strcpy( data.CombInstrumentID, positionField.CombInstrumentID);
	strcpy( data.ExchangeID, positionField.ExchangeID);
	data.CloseProfitByDate = positionField.CloseProfitByDate;
	data.CloseProfitByTrade = positionField.CloseProfitByTrade;
	//data.PositionProfitByDate = positionField.PositionProfitByDate;
	data.PositionProfitByTrade = positionField.PositionProfitByTrade;
	//data.Margin = positionField.Margin;
	//data.ExchMargin = positionField.ExchMargin;
	data.MarginRateByMoney = positionField.MarginRateByMoney;
	data.MarginRateByVolume = positionField.MarginRateByVolume;
	//data.LastSettlementPrice = positionField.LastSettlementPrice;
	data.SettlementPrice = positionField.SettlementPrice;
	data.CloseVolume = positionField.CloseVolume;
	data.CloseAmount = positionField.CloseAmount;

	strncpy(data.szAccount,positionField.szAccount,sizeof(data.szAccount)-1);
}

void CTraderSpi::CopyPositionCombField( PlatformStru_PositionCombDetail& data, const CThostFtdcInvestorPositionCombineDetailField& positionCombField)
{
	strncpy(data.TradingDay,positionCombField.TradingDay,sizeof(data.TradingDay)-1);
	strncpy(data.OpenDate,positionCombField.OpenDate,sizeof(data.OpenDate)-1);
	strncpy(data.ExchangeID,positionCombField.ExchangeID,sizeof(data.ExchangeID)-1);
	data.SettlementID=positionCombField.SettlementID;
	strncpy(data.BrokerID,positionCombField.BrokerID,sizeof(data.BrokerID)-1);
	strncpy(data.InvestorID,positionCombField.InvestorID,sizeof(data.InvestorID)-1);
	strncpy(data.ComTradeID,positionCombField.ComTradeID,sizeof(data.ComTradeID)-1);

	strncpy(data.Leg1TradeID,positionCombField.TradeID,sizeof(data.Leg1TradeID)-1);
	strncpy(data.Leg1InstrumentID,positionCombField.InstrumentID,sizeof(data.Leg1InstrumentID)-1);
	data.HedgeFlag=positionCombField.HedgeFlag;
	data.Direction=positionCombField.Direction;
	data.TotalAmt=positionCombField.TotalAmt;
	data.Leg1Margin=positionCombField.Margin;
	data.Leg1ExchMargin=positionCombField.ExchMargin;
	data.Leg1MarginRateByMoney=positionCombField.MarginRateByMoney;
	data.Leg1MarginRateByVolume=positionCombField.MarginRateByVolume;
	data.Leg1ID=positionCombField.LegID;
	data.Leg1Multiple=positionCombField.LegMultiple;
	strncpy(data.InstrumentID,positionCombField.CombInstrumentID,sizeof(data.InstrumentID)-1);

	strncpy(data.szAccount,positionCombField.InvestorID,sizeof(data.szAccount)-1);
}


void CTraderSpi::CopyOrderField(PlatformStru_OrderInfo& data,const CThostFtdcOrderField& orderField)
{
	///经纪公司代码
	strcpy_s(data.BrokerID, sizeof(orderField.BrokerID), orderField.BrokerID);
	///投资者代码
	strcpy_s(data.InvestorID, sizeof(orderField.InvestorID), orderField.InvestorID);
	///合约代码
	strcpy_s(data.InstrumentID, sizeof(orderField.InstrumentID), orderField.InstrumentID);
	///报单引用
	strcpy_s(data.OrderRef, sizeof(orderField.OrderRef), orderField.OrderRef);
	///用户代码
	strcpy_s(data.UserID, sizeof(orderField.UserID), orderField.UserID);
	///报单价格条件
	data.OrderPriceType = orderField.OrderPriceType;
	///买卖方向
	data.Direction = orderField.Direction;
	///组合开平标志
	data.CombOffsetFlag[0] = orderField.CombOffsetFlag[0];
	///组合投机套保标志
	strcpy_s(data.CombHedgeFlag, sizeof(orderField.CombHedgeFlag), orderField.CombHedgeFlag);
	///价格
	data.LimitPrice = orderField.LimitPrice;
	///数量
	data.VolumeTotalOriginal = orderField.VolumeTotalOriginal;
	///有效期类型
	data.TimeCondition = orderField.TimeCondition;
	///GTD日期
	strcpy_s(data.GTDDate, sizeof(orderField.GTDDate), orderField.GTDDate);
	///成交量类型
	data.VolumeCondition = orderField.VolumeCondition;
	///最小成交量
	data.MinVolume = orderField.MinVolume;
	///触发条件
	data.ContingentCondition = orderField.ContingentCondition;
	///止损价
	data.StopPrice = orderField.StopPrice;
	///强平原因
	data.ForceCloseReason = orderField.ForceCloseReason;
	///自动挂起标志
	data.IsAutoSuspend = orderField.IsAutoSuspend;
	///业务单元
	strcpy_s(data.BusinessUnit, sizeof(orderField.BusinessUnit), orderField.BusinessUnit);
	///请求编号
	data.RequestID = orderField.RequestID;
	///本地报单编号
	strcpy_s(data.OrderLocalID, sizeof(orderField.OrderLocalID), orderField.OrderLocalID);
	///交易所代码
	strcpy_s(data.ExchangeID, sizeof(orderField.ExchangeID), orderField.ExchangeID);
	///会员代码
	strcpy_s(data.ParticipantID, sizeof(orderField.ParticipantID), orderField.ParticipantID);
	///客户代码
	strcpy_s(data.ClientID, sizeof(orderField.ClientID), orderField.ClientID);
	///合约在交易所的代码
	strcpy_s(data.ExchangeInstID, sizeof(orderField.ExchangeInstID), orderField.ExchangeInstID);
	///交易所交易员代码
	strcpy_s(data.TraderID, sizeof(orderField.TraderID), orderField.TraderID);
	///安装编号
	data.InstallID = orderField.InstallID;
	///报单提交状态
	data.OrderSubmitStatus = orderField.OrderSubmitStatus;
	///报单提示序号
	data.NotifySequence = orderField.NotifySequence;
	///交易日
	strcpy_s(data.TradingDay, sizeof(orderField.TradingDay), orderField.TradingDay);
	///结算编号
	data.SettlementID = orderField.SettlementID;
	///报单编号
	strcpy_s(data.OrderSysID, sizeof(orderField.OrderSysID), orderField.OrderSysID);
	///报单来源
	data.OrderSource = orderField.OrderSource;
	///报单状态
	data.OrderStatus = orderField.OrderStatus;
	///报单类型
	data.OrderType = orderField.OrderType;
	///今成交数量
	data.VolumeTraded = orderField.VolumeTraded;
	///剩余数量
	data.VolumeTotal = orderField.VolumeTotal;
	///报单日期
	strcpy_s(data.InsertDate, sizeof(orderField.InsertDate), orderField.InsertDate);
	///委托时间
	strcpy_s(data.InsertTime, sizeof(orderField.InsertTime), orderField.InsertTime);
	///激活时间
	strcpy_s(data.ActiveTime, sizeof(orderField.ActiveTime), orderField.ActiveTime);
	///挂起时间
	strcpy_s(data.SuspendTime, sizeof(orderField.SuspendTime), orderField.SuspendTime);
	///最后修改时间
	strcpy_s(data.UpdateTime, sizeof(orderField.UpdateTime), orderField.UpdateTime);
	///撤销时间
	strcpy_s(data.CancelTime, sizeof(orderField.CancelTime), orderField.CancelTime);
	///最后修改交易所交易员代码
	strcpy_s(data.ActiveTraderID, sizeof(orderField.ActiveTraderID), orderField.ActiveTraderID);
	///结算会员编号
	strcpy_s(data.ClearingPartID, sizeof(orderField.ClearingPartID), orderField.ClearingPartID);
	///序号
	data.SequenceNo = orderField.SequenceNo;
	///前置编号
	data.FrontID = orderField.FrontID;
	///会话编号
	data.SessionID = orderField.SessionID;
	///用户端产品信息
	strcpy_s(data.UserProductInfo, sizeof(orderField.UserProductInfo), orderField.UserProductInfo);
	///状态信息
	strcpy_s(data.StatusMsg, sizeof(orderField.StatusMsg), orderField.StatusMsg);
	///用户强评标志
	data.UserForceClose = orderField.UserForceClose;
	///操作用户代码
	strcpy_s(data.ActiveUserID, sizeof(orderField.ActiveUserID), orderField.ActiveUserID);
	///经纪公司报单编号
	data.BrokerOrderSeq = orderField.BrokerOrderSeq;
	///相关报单
	strcpy_s(data.RelativeOrderSysID, sizeof(orderField.RelativeOrderSysID), orderField.RelativeOrderSysID);


	strncpy(data.szAccount,orderField.InvestorID,sizeof(data.szAccount)-1);

}



