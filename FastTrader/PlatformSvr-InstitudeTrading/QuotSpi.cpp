/***************************************************************************//**
*   @file          QuotSpi.cpp
*   @brief        综合交易平台(CTP)行情相关的回调对象
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/14
********************************************************************************/
#include "stdafx.h"

#include "QuotSpi.h"
#include "PlatFormService.hpp"
#include "../inc/Module-Misc2/BusinessData.hpp"

#include "../inc/Module-Misc2/packagecache.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"
#include "../inc/Module-Misc2/GlobalFunc.h"

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


static CThostFtdcRspInfoField BlankRspInfo;

CQuotSpi::CQuotSpi(CPlatFormService& RefCPlatFormService)
:m_RefMdApi(*(RefCPlatFormService.m_pQuotApi)), 
m_RefPackageCache(*(RefCPlatFormService.m_PCache)),
m_PlatformParam(RefCPlatFormService.m_PlatformParam),
m_RefCPlatFormService(RefCPlatFormService)
{
    memset(&BlankRspInfo,0,sizeof(BlankRspInfo));
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
		DataFrontConnected tmp(false);
		m_RefPackageCache.SaveDataTo(BID_FrontConnected, 0, 0, 0, sizeof(tmp), &tmp);

        CThostFtdcReqUserLoginField req;
        memset(&req, 0, sizeof(req));
        strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
        strncpy(req.UserID, m_PlatformParam.UserID.c_str(),sizeof(req.UserID)-1);
        strncpy(req.Password, m_PlatformParam.UserPW.c_str(),sizeof(req.Password)-1);
		strncpy(req.OneTimePassword,m_PlatformParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

        m_PlatformParam.QuotStatus=CTPCONNSTATUS_Logining;

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
    DataFrontDisconnected tmp(false,nReason);
    m_RefPackageCache.SaveDataTo(BID_FrontDisconnected, 0, 0, 0, sizeof(tmp), &tmp);
    m_PlatformParam.QuotStatus=CTPCONNSTATUS_Disconnected;
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
		DataRspUserLogin tmp(false,nRequestID,bIsLast,GetTickCount(),*pRspUserLogin,pRspInfo?(*pRspInfo):BlankRspInfo);

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
        m_PlatformParam.QuotStatus=CTPCONNSTATUS_Connected;
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
        m_PlatformParam.QuotStatus=CTPCONNSTATUS_LoginFailure;
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
    
	DataRspUserLogout tmp(1,nRequestID,bIsLast,*pUserLogout,pRspInfo?(*pRspInfo):BlankRspInfo);

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
	DataRtnDepthMarketData tmpSend;
    //CopyDepthMarketData(tmpSend.DepthMarketDataField, *pDepthMarketData);
	tmpSend.DepthMarketDataField = *pDepthMarketData;

	m_RefPackageCache.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(tmpSend), &tmpSend); 
    SetQuotInfo_Internal(tmpSend.DepthMarketDataField);
	
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

//设置指定合约行情，设置的行情更新，则返回true；否则返回false（比原来的行情更老）
//仅用于OnRtnDepthMarketData()中
bool CQuotSpi::SetQuotInfo_Internal(const PlatformStru_DepthMarketData& inData)
{
    bool brlt=true;

    m_lastQuot[string(inData.InstrumentID)] = inData;

    return brlt;
}

//获取指定合约行情。仅用于OnRtnDepthMarketData()中
bool CQuotSpi::GetQuotInfo_Internal(const string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    bool brlt=false;

    map<string,PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
    if(it==m_lastQuot.end())
    {
        memset(&outData,0,sizeof(outData));
    }
    else
    {
        outData = it->second;
        brlt=true;
    }
    return brlt;
}

