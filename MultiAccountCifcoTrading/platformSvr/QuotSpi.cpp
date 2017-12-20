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
#include "../inc/Module-Misc/orderCommonFunc.h"

#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CQuotSpi", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif
#ifdef DELAY_QUOT	
std::set<CQuotSpi*> CQuotSpi::m_pQuotSpiSet;
void CALLBACK CQuotSpi::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	std::set<CQuotSpi*>::const_iterator it;
	for (it=m_pQuotSpiSet.begin();it!=m_pQuotSpiSet.end();++it)
	{
		(*it)->OnTimer();
	}
}
void CQuotSpi::OnTimer()
{
	std::map<std::string,CThostFtdcDepthMarketDataField>::iterator it;
	EnterCriticalSection(&m_CS);
	for (it=m_mapQuot.begin();it!=m_mapQuot.end();++it)
	{
		DataRtnDepthMarketData tmpSend;	
		CopyDepthMarketData(tmpSend.DepthMarketDataField, it->second);
		m_RefPackageCache.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(tmpSend), &tmpSend); 
	}
	m_mapQuot.clear();
	LeaveCriticalSection(&m_CS);
}
#endif
CQuotSpi::CQuotSpi(CPlatFormService& RefCPlatFormService)
:m_RefMdApi(*(RefCPlatFormService.m_pQuotApi)), 
m_RefPackageCache(*(RefCPlatFormService.m_PCache)),
m_PlatformParam(RefCPlatFormService.m_PlatformParam),
m_RefCPlatFormService(RefCPlatFormService)
{
#ifdef DELAY_QUOT	
	InitializeCriticalSection(&m_CS);
	m_pQuotSpiSet.insert(this);
	SetTimer(NULL,0,1000,TimerProc);
#endif
    m_pWriteLog=new zqWriteLog();
    LOG_INFO("CQuotSpi::CQuotSpi(构造CQuotSpi对象)");
}

CQuotSpi::~CQuotSpi()
{
#ifdef DELAY_QUOT	
	DeleteCriticalSection(&m_CS);
	m_pQuotSpiSet.erase(this);
#endif
LOG_INFO("CQuotSpi::~CQuotSpi(析构CQuotSpi对象)");
delete m_pWriteLog;
}

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CQuotSpi::OnFrontConnected()
{
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
        m_RefCPlatFormService.GetMarketDataInstrumentID(setMarketDataInstrumentID);
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
		if(pRspInfo) memcpy(&m_RefCPlatFormService.m_ErrorInfoField,pRspInfo,sizeof(CThostFtdcRspInfoField));
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

void ConvertDepthMarketData(CThostFtdcDepthMarketDataField& data, const PlatformStru_DepthMarketData& depthMarketData)
{
	strcpy(data.TradingDay, depthMarketData.TradingDay);
	strcpy(data.InstrumentID, depthMarketData.InstrumentID);
	strcpy(data.ExchangeID, depthMarketData.ExchangeID);
	strcpy(data.ExchangeInstID, depthMarketData.ExchangeInstID);
	data.LastPrice = depthMarketData.LastPrice;
	data.PreSettlementPrice = depthMarketData.PreSettlementPrice;
	data.PreClosePrice = depthMarketData.PreClosePrice;
	data.PreOpenInterest = depthMarketData.PreOpenInterest;
	data.OpenPrice = depthMarketData.OpenPrice;
	data.HighestPrice = depthMarketData.HighestPrice;
	data.LowestPrice = depthMarketData.LowestPrice;
	data.Volume = depthMarketData.Volume;
	data.Turnover = depthMarketData.Turnover;
	data.OpenInterest = depthMarketData.OpenInterest;
	data.ClosePrice = depthMarketData.ClosePrice;
	data.SettlementPrice = depthMarketData.SettlementPrice;
	data.UpperLimitPrice = depthMarketData.UpperLimitPrice;
	data.LowerLimitPrice = depthMarketData.LowerLimitPrice;
	data.PreDelta = depthMarketData.PreDelta;
	data.CurrDelta = depthMarketData.CurrDelta;
	strcpy(data.UpdateTime, depthMarketData.UpdateTime);
	data.UpdateMillisec = depthMarketData.UpdateMillisec;
	data.BidPrice1 = depthMarketData.BidPrice1;
	data.BidVolume1 = depthMarketData.BidVolume1;
	data.AskPrice1 = depthMarketData.AskPrice1;
	data.AskVolume1 = depthMarketData.AskVolume1;
	data.BidPrice2 = depthMarketData.BidPrice2;
	data.BidVolume2 = depthMarketData.BidVolume2;
	data.AskPrice2 = depthMarketData.AskPrice2;
	data.AskVolume2 = depthMarketData.AskVolume2;
	data.BidPrice3 = depthMarketData.BidPrice3;
	data.BidVolume3 = depthMarketData.BidVolume3;
	data.AskPrice3 = depthMarketData.AskPrice3;
	data.AskVolume3 = depthMarketData.AskVolume3;
	data.BidPrice4 = depthMarketData.BidPrice4;
	data.BidVolume4 = depthMarketData.BidVolume4;
	data.AskPrice4 = depthMarketData.AskPrice4;
	data.AskVolume4 = depthMarketData.AskVolume4;
	data.BidPrice5 = depthMarketData.BidPrice5;
	data.BidVolume5 = depthMarketData.BidVolume5;
	data.AskPrice5 = depthMarketData.AskPrice5;
	data.AskVolume5 = depthMarketData.AskVolume5;
	data.AveragePrice = depthMarketData.AveragePrice;
}

///深度行情通知
void CQuotSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    if(!pDepthMarketData)
        return;

	//if(strlen(pDepthMarketData->InstrumentID)>10) {
	//	char strText[1024];
	//	ZeroMemory(strText, 1024);
	//	sprintf_s(strText, 1023, "%s, Bid Price:=%f, Ask Price:=%f\n", 
	//			pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, pDepthMarketData->AskPrice1);
	//	TRACE(strText);
	//}

    static int Count=0;
    Count++;


    static bool blogID=false;
    if(blogID)
    {
        LOG_INFO("CQuotSpi::OnRtnDepthMarketData:InstrumentID=%s,LastPrice=%g,UpdateTime=%s",
            pDepthMarketData->InstrumentID,pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
    }
/*
	if(pDepthMarketData->AskVolume1==0) {
		pDepthMarketData->AskPrice1=util::GetDoubleInvalidValue();
	}
	if(pDepthMarketData->BidVolume1==0) {
		pDepthMarketData->BidPrice1=util::GetDoubleInvalidValue();
	}
	if(util::equals(pDepthMarketData->OpenPrice, 0.0)) {
		pDepthMarketData->OpenPrice=util::GetDoubleInvalidValue();
	}
	if(util::equals(pDepthMarketData->HighestPrice, 0.0)) {
		pDepthMarketData->HighestPrice=util::GetDoubleInvalidValue();
	}
	if(util::equals(pDepthMarketData->LowestPrice, 0.0)) {
		pDepthMarketData->LowestPrice=util::GetDoubleInvalidValue();
	}
	if(util::equals(pDepthMarketData->PreClosePrice, 0.0)) {
		pDepthMarketData->PreClosePrice=util::GetDoubleInvalidValue();
	}
	if(util::equals(pDepthMarketData->ClosePrice, 0.0)) {
		pDepthMarketData->ClosePrice=util::GetDoubleInvalidValue();
	}

	std::multimap<std::string,std::string> mapCombineRelation;

	std::string strMainCode;
	std::string strSubCode1, strSubCode2;
	BOOL bIsCombine = FALSE;
	double LastPrice1=0.0, Price1=0.0;
	double LastPrice2=0.0, Price2=0.0;
	double AskPrice=0.0;
	double BidPrice=0.0;

    PlatformStru_DepthMarketData tmp1, tmp2;
	ZeroMemory(&tmp1, sizeof(tmp1));
	ZeroMemory(&tmp2, sizeof(tmp2));

	strMainCode = pDepthMarketData->InstrumentID;
	bIsCombine = m_RefCPlatFormService.GetProductClassType(strMainCode)
			== THOST_FTDC_PC_Combination;
	if(bIsCombine) {
		ConvertComboInstrumentID(strMainCode, strSubCode1, strSubCode2);
		if(m_RefCPlatFormService.GetQuotInfo(strSubCode1, tmp1)==0 
				&& m_RefCPlatFormService.GetQuotInfo(strSubCode2, tmp2)==0) {

			LastPrice1=util::isInvalidValue(tmp1.LastPrice)?tmp1.PreSettlementPrice:tmp1.LastPrice;
			LastPrice2=util::isInvalidValue(tmp2.LastPrice)?tmp2.PreSettlementPrice:tmp2.LastPrice;
			pDepthMarketData->LastPrice=LastPrice1-LastPrice2;

			AskPrice=util::isInvalidValue(tmp1.AskPrice1)?tmp1.PreSettlementPrice:tmp1.AskPrice1;
			BidPrice=util::isInvalidValue(tmp2.BidPrice1)?tmp2.PreSettlementPrice:tmp2.BidPrice1;
			Price1=AskPrice-BidPrice;
			if(util::isInvalidValue(pDepthMarketData->AskPrice1)) {
				pDepthMarketData->AskPrice1=Price1;
				pDepthMarketData->AskVolume1=min(tmp1.AskVolume1, tmp2.BidVolume1);
			}
			else {
				if(util::equals(Price1, pDepthMarketData->AskPrice1)) {
					//pDepthMarketData->AskPrice1=Price1;
					pDepthMarketData->AskVolume1+=min(tmp1.AskVolume1, tmp2.BidVolume1);
				}
				else if(Price1>pDepthMarketData->AskPrice1) {
					//pDepthMarketData->AskPrice1=pDepthMarketData->AskPrice1;
					//pDepthMarketData->AskVolume1=pDepthMarketData->AskVolume1;
				}
				else {
					pDepthMarketData->AskPrice1=Price1;
					pDepthMarketData->AskVolume1=min(tmp1.AskVolume1, tmp2.BidVolume1);
				}
			}

			BidPrice=util::isInvalidValue(tmp1.BidPrice1)?tmp1.PreSettlementPrice:tmp1.BidPrice1;
			AskPrice=util::isInvalidValue(tmp2.AskPrice1)?tmp2.PreSettlementPrice:tmp2.AskPrice1;
			Price2=BidPrice-AskPrice;
			if(util::isInvalidValue(pDepthMarketData->BidPrice1)) {
				pDepthMarketData->BidPrice1=Price2;
				pDepthMarketData->BidVolume1=min(tmp1.BidVolume1, tmp2.AskVolume1);
			}
			else {
				if(util::equals(Price2, pDepthMarketData->BidPrice1)) {
					//pDepthMarketData->BidPrice1=Price2;
					pDepthMarketData->BidVolume1+=min(tmp1.BidVolume1, tmp2.AskVolume1);
				}
				else if(Price2>pDepthMarketData->BidPrice1) {
					pDepthMarketData->BidPrice1=Price2;
					pDepthMarketData->BidVolume1=min(tmp1.BidVolume1, tmp2.AskVolume1);
				}
				else {
					//pDepthMarketData->BidPrice1=pDepthMarketData->BidPrice1;
					//pDepthMarketData->BidVolume1=pDepthMarketData->BidVolume1;
				}
			}
		}
	}
	else {
		std::multimap<std::string,std::string>::iterator itlow, itupp;

		m_RefCPlatFormService.GetCombineRelation(mapCombineRelation);
		itlow = mapCombineRelation.lower_bound(strMainCode);
		itupp = mapCombineRelation.upper_bound(strMainCode);
		for(;itlow!=itupp;itlow++) {
			ConvertComboInstrumentID(itlow->second, strSubCode1, strSubCode2);
			BOOL bIsFound = FALSE;
			if(strMainCode.compare(strSubCode1)==0) {
				bIsFound = m_RefCPlatFormService.GetQuotInfo(strSubCode2, tmp2)==0;
				CopyDepthMarketData(tmp1, *pDepthMarketData);
			}
			else {
				bIsFound = m_RefCPlatFormService.GetQuotInfo(strSubCode1, tmp1)==0;
				CopyDepthMarketData(tmp2, *pDepthMarketData);
			}
			if(bIsFound) {
				CThostFtdcDepthMarketDataField newDepthMarketData;
				PlatformStru_DepthMarketData tmpCombineMarket;
				ZeroMemory(&tmpCombineMarket, sizeof(tmpCombineMarket));
				ZeroMemory(&newDepthMarketData, sizeof(newDepthMarketData));
				if(m_RefCPlatFormService.GetQuotInfo(itlow->second, tmpCombineMarket)==0) {

					if(tmpCombineMarket.BidVolume1==0)
						tmpCombineMarket.BidPrice1=1.7976931348623158e+308;
					if(tmpCombineMarket.AskVolume1==0)
						tmpCombineMarket.AskPrice1=1.7976931348623158e+308;

					ConvertDepthMarketData(newDepthMarketData, tmpCombineMarket);
					
					LastPrice1=util::isInvalidValue(tmp1.LastPrice)?tmp1.PreSettlementPrice:tmp1.LastPrice;
					LastPrice2=util::isInvalidValue(tmp2.LastPrice)?tmp2.PreSettlementPrice:tmp2.LastPrice;
					newDepthMarketData.LastPrice=LastPrice1-LastPrice2;

					AskPrice=util::isInvalidValue(tmp1.AskPrice1)?tmp1.PreSettlementPrice:tmp1.AskPrice1;
					BidPrice=util::isInvalidValue(tmp2.BidPrice1)?tmp2.PreSettlementPrice:tmp2.BidPrice1;
					Price1=AskPrice-BidPrice;
					if(util::isInvalidValue(newDepthMarketData.AskPrice1)) {
						newDepthMarketData.AskPrice1=Price1;
						newDepthMarketData.AskVolume1=min(tmp1.AskVolume1, tmp2.BidVolume1);
					}
					else {
						if(util::equals(Price1, newDepthMarketData.AskPrice1)) {
							//newDepthMarketData.AskPrice1=Price1;
							newDepthMarketData.AskVolume1+=min(tmp1.AskVolume1, tmp2.BidVolume1);
						}
						else if(Price1>newDepthMarketData.AskPrice1) {
							//newDepthMarketData.AskPrice1=pDepthMarketData->AskPrice1;
							//newDepthMarketData.AskVolume1=pDepthMarketData->AskVolume1;
						}
						else {
							newDepthMarketData.AskPrice1=Price1;
							newDepthMarketData.AskVolume1=min(tmp1.AskVolume1, tmp2.BidVolume1);
						}
					}

					BidPrice=util::isInvalidValue(tmp1.BidPrice1)?tmp1.PreSettlementPrice:tmp1.BidPrice1;
					AskPrice=util::isInvalidValue(tmp2.AskPrice1)?tmp2.PreSettlementPrice:tmp2.AskPrice1;
					Price2=BidPrice-AskPrice;
					if(util::isInvalidValue(tmpCombineMarket.BidPrice1)) {
						newDepthMarketData.BidPrice1=Price2;
						newDepthMarketData.BidVolume1=min(tmp1.BidVolume1, tmp2.AskVolume1);
					}
					else {
						if(util::equals(Price2, tmpCombineMarket.BidPrice1)) {
							//newDepthMarketData.BidPrice1=Price2;
							newDepthMarketData.BidVolume1+=min(tmp1.BidVolume1, tmp2.AskVolume1);
						}
						else if(Price2>tmpCombineMarket.BidPrice1) {
							newDepthMarketData.BidPrice1=Price2;
							newDepthMarketData.BidVolume1=min(tmp1.BidVolume1, tmp2.AskVolume1);
						}
						else {
							//newDepthMarketData.BidPrice1=pDepthMarketData->BidPrice1;
							//newDepthMarketData.BidVolume1=pDepthMarketData->BidVolume1;
						}
					}

					CopyDepthMarketData(tmpSend.DepthMarketDataField, newDepthMarketData);
					m_RefPackageCache.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(tmpSend), &tmpSend); 
				}
			}
		}
	}*/
#ifdef DELAY_QUOT		
	EnterCriticalSection(&m_CS);
	m_mapQuot[pDepthMarketData->InstrumentID]=*pDepthMarketData;
	LeaveCriticalSection(&m_CS);
#else
	DataRtnDepthMarketData tmpSend;	
	CopyDepthMarketData(tmpSend.DepthMarketDataField, *pDepthMarketData);
	m_RefPackageCache.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(tmpSend), &tmpSend); 
#endif	
}

void CQuotSpi::CopyDepthMarketData( PlatformStru_DepthMarketData& data, const CThostFtdcDepthMarketDataField& depthMarketData )
{
	strcpy(data.TradingDay, depthMarketData.TradingDay);
	strcpy(data.InstrumentID, depthMarketData.InstrumentID);
	strcpy(data.ExchangeID, depthMarketData.ExchangeID);
	strcpy(data.ExchangeInstID, depthMarketData.ExchangeInstID);
	data.LastPrice = depthMarketData.LastPrice;
	data.PreSettlementPrice = depthMarketData.PreSettlementPrice;
	data.PreClosePrice = depthMarketData.PreClosePrice;
	data.PreOpenInterest = depthMarketData.PreOpenInterest;
	data.OpenPrice = depthMarketData.OpenPrice;
	data.HighestPrice = depthMarketData.HighestPrice;
	data.LowestPrice = depthMarketData.LowestPrice;
	data.Volume = depthMarketData.Volume;
	data.Turnover = depthMarketData.Turnover;
	data.OpenInterest = depthMarketData.OpenInterest;
	data.ClosePrice = depthMarketData.ClosePrice;
	data.SettlementPrice = depthMarketData.SettlementPrice;
	data.UpperLimitPrice = depthMarketData.UpperLimitPrice;
	data.LowerLimitPrice = depthMarketData.LowerLimitPrice;
	data.PreDelta = depthMarketData.PreDelta;
	data.CurrDelta = depthMarketData.CurrDelta;
	strcpy(data.UpdateTime, depthMarketData.UpdateTime);
	data.UpdateMillisec = depthMarketData.UpdateMillisec;
	data.BidPrice1 = depthMarketData.BidPrice1;
	data.BidVolume1 = depthMarketData.BidVolume1;
	data.AskPrice1 = depthMarketData.AskPrice1;
	data.AskVolume1 = depthMarketData.AskVolume1;
	data.BidPrice2 = depthMarketData.BidPrice2;
	data.BidVolume2 = depthMarketData.BidVolume2;
	data.AskPrice2 = depthMarketData.AskPrice2;
	data.AskVolume2 = depthMarketData.AskVolume2;
	data.BidPrice3 = depthMarketData.BidPrice3;
	data.BidVolume3 = depthMarketData.BidVolume3;
	data.AskPrice3 = depthMarketData.AskPrice3;
	data.AskVolume3 = depthMarketData.AskVolume3;
	data.BidPrice4 = depthMarketData.BidPrice4;
	data.BidVolume4 = depthMarketData.BidVolume4;
	data.AskPrice4 = depthMarketData.AskPrice4;
	data.AskVolume4 = depthMarketData.AskVolume4;
	data.BidPrice5 = depthMarketData.BidPrice5;
	data.BidVolume5 = depthMarketData.BidVolume5;
	data.AskPrice5 = depthMarketData.AskPrice5;
	data.AskVolume5 = depthMarketData.AskVolume5;
	data.AveragePrice = depthMarketData.AveragePrice;
}


