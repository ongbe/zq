/***************************************************************************//**
*   @file          QuotSpi.cpp
*   @brief        综合交易平台(CTP)行情相关的回调对象
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/
#include "stdafx.h"

#include "QuotSpi.hpp"
#include "PlatFormService.hpp"
#include "../inc/ISvr/BusinessData.hpp"

#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"

#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CQuotSpi", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
namespace Pats
{
	CQuotSpi::CQuotSpi(CPlatFormService& RefCPatsPlatFormService)
		:m_RefMdApi(*(RefCPatsPlatFormService.m_pQuotApi)), 
		m_RefPackageCache(*(RefCPatsPlatFormService.m_PCache)),
		m_PlatformParam(RefCPatsPlatFormService.m_PlatformParam),
		m_RefCPatsPlatFormService(RefCPatsPlatFormService)
	{
		m_pWriteLog=new zqWriteLog();
		LOG_INFO("CQuotSpi::CQuotSpi(构造CQuotSpi对象)");
	}

	CQuotSpi::~CQuotSpi()
	{

		LOG_INFO("CQuotSpi::~CQuotSpi(析构CQuotSpi对象)");
		delete m_pWriteLog;
	}

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void CQuotSpi::OnFrontConnected()
	{
		//GetLocalTime(&m_ConnectTime);//获得连接时间

		DataFrontConnected tmp;
		tmp.Head.BID=BID_FrontConnected;
		tmp.Head.Size=sizeof(DataFrontConnected);
		tmp.bTrader=false;
		m_RefPackageCache.SaveDataTo(BID_FrontConnected, 0, 0, 0, sizeof(tmp), &tmp);

		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
		strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
		strncpy(req.Password, m_PlatformParam.UserPW.c_str(),sizeof(req.Password)-1);
		strncpy(req.OneTimePassword,m_PlatformParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

		m_PlatformParam.QuotStatus=CPlatFormParam::CONNSTATUS_Logining;

		int iRequestID=m_PlatformParam.RequestID++;
		int iRlt = m_RefMdApi.ReqUserLogin(&req, iRequestID);

		LOG_INFO("CQuotSpi::OnFrontConnected(与交易所成功建立TCP连接):\n"
			"\t\t\t --->ReqUserLogin(用户登录请求):ret=[%d],iRequestID=[%d],\n"
			"\t\t\t BrokerID=[%s],\t UserID=[%s],",
			iRlt, iRequestID,
			req.BrokerID, req.UserID);
	}

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	void CQuotSpi::OnFrontDisconnected(int nReason)
	{ 
		LOG_INFO("CQuotSpi::OnFrontDisconnected(与交易所失去TCP连接):nReason=[%#x]",nReason);
		DataFrontDisconnected tmp;
		tmp.Head.BID=BID_FrontDisconnected;
		tmp.Head.Size=sizeof(DataFrontDisconnected);
		tmp.bTrader=false;
		tmp.nReason=nReason;
		m_RefPackageCache.SaveDataTo(BID_FrontDisconnected, 0, 0, 0, sizeof(tmp), &tmp);
		m_PlatformParam.QuotStatus=CPlatFormParam::CONNSTATUS_Disconnected;
	}

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	void CQuotSpi::OnHeartBeatWarning(int nTimeLapse)
	{
		LOG_INFO("CQuotSpi::OnHeartBeatWarning:nTimeLapse=[%x]",nTimeLapse);
	}

	///登录请求响应
	void CQuotSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if(pRspUserLogin)
		{
			DataRspUserLogin tmp;
			tmp.bTrader       = false;
			tmp.nRequestID = nRequestID;
			tmp.bIsLast    = bIsLast;
			tmp.CurTickCount = GetTickCount();
			tmp.UserLoginField = *pRspUserLogin;
			if(pRspInfo)
				tmp.RspInfoField = *pRspInfo;

			m_RefPackageCache.SaveDataTo(BID_RspUserLogin, 0, 0, 0, sizeof(tmp), &tmp);
			LOG_INFO("CQuotSpi::OnRspUserLogin(登录请求响应) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]\n"
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
			LOG_INFO("CQuotSpi::OnRspUserLogin(登录请求响应),pRspUserLogin is NULL");
		}

		if(pRspInfo->ErrorID==0)
		{
			m_PlatformParam.QuotStatus=CPlatFormParam::CONNSTATUS_Connected;
			//todo 根据合约定制行情
			std::set<std::string> setMarketDataInstrumentID;
			m_RefCPatsPlatFormService.GetMarketDataInstrumentID(setMarketDataInstrumentID);
			std::set<std::string>::iterator iter=setMarketDataInstrumentID.begin();
			for(iter;iter!=setMarketDataInstrumentID.end();++iter)
			{
				char *pInstrumentID = const_cast<char*>(iter->c_str());
				m_RefMdApi.SubscribeMarketData(&pInstrumentID,1);
			}
		}
		else
		{
			m_PlatformParam.QuotStatus=CPlatFormParam::CONNSTATUS_LoginFailure;
			if(pRspInfo) memcpy(&m_RefCPatsPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
		}

	}

	///登出请求响应
	void CQuotSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if(!pUserLogout)
		{
			LOG_INFO("CQuotSpi::OnRspUserLogout(登出请求响应),pUserLogout is NULL");
			return;
		}

		DataRspUserLogout tmp;
		tmp.Type       = 1;
		tmp.nRequestID = nRequestID;
		tmp.bIsLast    = bIsLast;
		tmp.UserLogoutField = *pUserLogout;
		if(pRspInfo)
			tmp.RspInfoField = *pRspInfo;

		m_RefPackageCache.SaveDataTo(BID_RspUserLogout, 0, 0, 0, sizeof(tmp), &tmp);
		LOG_INFO("CQuotSpi::OnRspUserLogout(登出请求响应) : ErrCode=[%d] ErrMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
			"\t\t\t BrokerID=[%s],\t UserID=[%s]",
			tmp.RspInfoField.ErrorID,tmp.RspInfoField.ErrorMsg,nRequestID,bIsLast,
			pUserLogout->BrokerID,pUserLogout->UserID);
	}

	///错误应答
	void CQuotSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		LOG_INFO("CQuotSpi::OnRspError : ErrCode=[%d] ErrMsg=[%s],RequestID=[%d],IsLast=[%d]",
			pRspInfo->ErrorID,pRspInfo->ErrorMsg,nRequestID,(int)bIsLast);
	}

	///订阅行情应答
	void CQuotSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if(!pSpecificInstrument)
		{
			LOG_INFO("CQuotSpi::OnRspSubMarketData(订阅行情应答,Unimplemented),pSpecificInstrument is NULL");
			return;
		}

		LOG_INFO("CQuotSpi::OnRspSubMarketData(订阅行情应答,Unimplemented) : ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],nRequestID=[%d],bIsLast=[%d]",
			pRspInfo->ErrorID,pRspInfo->ErrorMsg,
			pSpecificInstrument->InstrumentID,nRequestID,(int)bIsLast);
	}

	///取消订阅行情应答
	void CQuotSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		if(!pSpecificInstrument)
		{
			LOG_INFO("CQuotSpi::OnRspUnSubMarketData(取消订阅行情应答,Unimplemented),pSpecificInstrument is NULL");
			return;
		}

		LOG_INFO("CQuotSpi::OnRspUnSubMarketData(取消订阅行情应答,Unimplemented) : ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],nRequestID=[%d],bIsLast=[%d]",
			pRspInfo->ErrorID,pRspInfo->ErrorMsg,   
			pSpecificInstrument->InstrumentID,nRequestID,(int)bIsLast);
	}

	///深度行情通知
	void CQuotSpi::OnRtnDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData)
	{
		if(!pDepthMarketData)
			return;

		static int Count=0;
		Count++;

		static bool blogID=false;
		if(blogID)
		{
			LOG_INFO("CQuotSpi::OnRtnDepthMarketData:InstrumentID=%s,LastPrice=%g,UpdateTime=%s",
				pDepthMarketData->InstrumentID,pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
		}

		bool ret;
		DataRtnDepthMarketData tmp;
		tmp.DepthMarketDataField = *pDepthMarketData;//原始行情
		ret=m_RefPackageCache.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(tmp), &tmp); 
		if(!ret)
		{
			LOG_INFO("CQuotSpi::OnRtnDepthMarketData,Failed to SaveDataTo m_RefPackageCache:"
				"InstrumentID=%s,LastPrice=%g",
				tmp.DepthMarketDataField.InstrumentID,tmp.DepthMarketDataField.LastPrice);
		}


	}
}

