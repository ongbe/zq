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
void CQuotSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    if(!pDepthMarketData)
        return;

    //pDepthMarketData->SettlementPrice=util::GetDoubleInvalidValue();
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
	DataRtnDepthMarketData tmpSend;
    tmpSend.DepthMarketDataField=PlatformStru_DepthMarketData(*pDepthMarketData);

//#ifdef _USE_COMBOINSTRUMENT
    if(GlobalConfigManager::m_bUseComb&&
        GlobalFunc::IsCombInstrument2(tmpSend.DepthMarketDataField.InstrumentID))
    {
        if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LastPrice||
            GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_Volume||
            GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_AskBid||
            GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreSettlementPrice||
            GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreClosePrice||
            GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LimitPrice)
        {
            //自行计算最新价、现量、买卖价买卖量、昨结算价、最新价、涨跌停板
            string Leg1InstrumentID,Leg2InstrumentID;
            PlatformStru_DepthMarketData Leg1Quot,Leg2Quot;
            if(GlobalFunc::IsCombInstrument(tmpSend.DepthMarketDataField.InstrumentID,Leg1InstrumentID,Leg2InstrumentID)&&
                GetQuotInfo_Internal(Leg1InstrumentID,Leg1Quot)&&
                GetQuotInfo_Internal(Leg2InstrumentID,Leg2Quot))
            {
                if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LastPrice)
                {
                    if(!util::isInvalidValue(Leg1Quot.LastPrice)&&!util::isInvalidValue(Leg2Quot.LastPrice))
                        tmpSend.DepthMarketDataField.LastPrice=Leg1Quot.LastPrice-Leg2Quot.LastPrice;
                    else tmpSend.DepthMarketDataField.LastPrice=util::GetDoubleInvalidValue();
                }
                if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_Volume)
                {
                    if(!util::isInvalidValue(Leg1Quot.Volume)&&!util::isInvalidValue(Leg2Quot.Volume))
                        tmpSend.DepthMarketDataField.Volume = Leg1Quot.Volume <= Leg2Quot.Volume ? Leg1Quot.Volume : Leg2Quot.Volume;
                    else tmpSend.DepthMarketDataField.Volume = util::GetIntInvalidValue();
                }
                if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_AskBid)
                {
                    if(!util::isInvalidValue(Leg1Quot.AskPrice1)&&!util::isInvalidValue(Leg2Quot.AskPrice1))
                        tmpSend.DepthMarketDataField.AskPrice1=Leg1Quot.AskPrice1-Leg2Quot.BidPrice1;
                    else tmpSend.DepthMarketDataField.AskPrice1=util::GetDoubleInvalidValue();
                    if(!util::isInvalidValue(Leg1Quot.AskVolume1)&&!util::isInvalidValue(Leg2Quot.BidVolume1))
                        tmpSend.DepthMarketDataField.AskVolume1 = Leg1Quot.AskVolume1 <= Leg2Quot.BidVolume1 ? Leg1Quot.AskVolume1 : Leg2Quot.BidVolume1;
                    else tmpSend.DepthMarketDataField.AskVolume1=util::GetIntInvalidValue();
                    if(!util::isInvalidValue(Leg1Quot.BidPrice1)&&!util::isInvalidValue(Leg2Quot.BidPrice1))
                        tmpSend.DepthMarketDataField.BidPrice1=Leg1Quot.BidPrice1-Leg2Quot.AskPrice1;
                    else tmpSend.DepthMarketDataField.BidPrice1=util::GetDoubleInvalidValue();
                    if(!util::isInvalidValue(Leg1Quot.BidVolume1)&&!util::isInvalidValue(Leg2Quot.AskVolume1))
                        tmpSend.DepthMarketDataField.BidVolume1 = Leg1Quot.BidVolume1 <= Leg2Quot.AskVolume1 ? Leg1Quot.BidVolume1 : Leg2Quot.AskVolume1;
                    else tmpSend.DepthMarketDataField.BidVolume1=util::GetIntInvalidValue();
                }
                if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LimitPrice)
                {
                    if(!util::isInvalidValue(Leg1Quot.LowerLimitPrice)&&!util::isInvalidValue(Leg2Quot.UpperLimitPrice))
                        tmpSend.DepthMarketDataField.LowerLimitPrice=Leg1Quot.LowerLimitPrice-Leg2Quot.UpperLimitPrice;
                    else tmpSend.DepthMarketDataField.LowerLimitPrice=util::GetDoubleInvalidValue();
                    if(!util::isInvalidValue(Leg1Quot.UpperLimitPrice)&&!util::isInvalidValue(Leg2Quot.LowerLimitPrice))
                        tmpSend.DepthMarketDataField.UpperLimitPrice=Leg1Quot.UpperLimitPrice-Leg2Quot.LowerLimitPrice;
                    else tmpSend.DepthMarketDataField.UpperLimitPrice=util::GetDoubleInvalidValue();
                }
                if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreSettlementPrice)
                {
                    if(!util::isInvalidValue(Leg1Quot.PreSettlementPrice)&&!util::isInvalidValue(Leg2Quot.PreSettlementPrice))
                        tmpSend.DepthMarketDataField.PreSettlementPrice=Leg1Quot.PreSettlementPrice-Leg2Quot.PreSettlementPrice;
                    else tmpSend.DepthMarketDataField.PreSettlementPrice=util::GetDoubleInvalidValue();
                }
                if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreClosePrice)
                {
                    if(!util::isInvalidValue(Leg1Quot.PreClosePrice)&&!util::isInvalidValue(Leg2Quot.PreClosePrice))
                        tmpSend.DepthMarketDataField.PreClosePrice=Leg1Quot.PreSettlementPrice-Leg2Quot.PreClosePrice;
                    else tmpSend.DepthMarketDataField.PreClosePrice=util::GetDoubleInvalidValue();
                }
            }
        }
    }
//#endif

	m_RefPackageCache.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(tmpSend), &tmpSend); 
    SetQuotInfo_Internal(tmpSend.DepthMarketDataField);

//#ifdef _USE_COMBOINSTRUMENT
    if(GlobalConfigManager::m_bUseComb&&
        (GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LastPrice||
         GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_Volume||
         GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_AskBid||
         GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreSettlementPrice||
         GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreClosePrice||
         GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LimitPrice))
    {
        std::string ThisInstrumentID(pDepthMarketData->InstrumentID);
        std::vector<std::string> CombInstrumentIDs;
        std::string Leg1InstrumentID,Leg2InstrumentID;
        PlatformStru_DepthMarketData Leg1Quot,Leg2Quot,CombQuot;

        //对于非组合单，并且是某些组合单的单腿的情况进行处理
        if(!GlobalFunc::IsCombInstrument2(ThisInstrumentID)&&
            m_RefCPlatFormService.GetCombInstrumentIDs_IncludeLeg(ThisInstrumentID,CombInstrumentIDs))
        {
            //此时CombInstrumentIDs存放着和单腿有关的组合合约
            //针对每一个组合合约，生成行情
            for(int i=0;i<(int)CombInstrumentIDs.size();i++)
            {
                if(GlobalFunc::IsCombInstrument(CombInstrumentIDs[i],Leg1InstrumentID,Leg2InstrumentID))
                {
                    //获取Leg1Quot和Leg2Quot
                    if(Leg1InstrumentID==ThisInstrumentID)
                    {
                        Leg1Quot=tmpSend.DepthMarketDataField;
                        if(!GetQuotInfo_Internal(Leg2InstrumentID, Leg2Quot))
                            continue;
                    }
                    else if(Leg2InstrumentID==ThisInstrumentID)
                    {
                        if(!GetQuotInfo_Internal(Leg1InstrumentID, Leg1Quot))
                            continue;
                        Leg2Quot=tmpSend.DepthMarketDataField;
                    }
                    else continue;

                    //获取CombQuot
                    if(!GetQuotInfo_Internal(CombInstrumentIDs[i], CombQuot))
                    {
                        CombQuot.clear();
                        strncpy(CombQuot.TradingDay,tmpSend.DepthMarketDataField.TradingDay,sizeof(CombQuot.TradingDay)-1);
                        strncpy(CombQuot.InstrumentID,CombInstrumentIDs[i].c_str(),sizeof(CombQuot.InstrumentID)-1);
                        strncpy(CombQuot.ExchangeID,tmpSend.DepthMarketDataField.ExchangeID,sizeof(CombQuot.ExchangeID)-1);
                        strncpy(CombQuot.ExchangeInstID,CombInstrumentIDs[i].c_str(),sizeof(CombQuot.ExchangeInstID)-1);
                        strncpy(CombQuot.UpdateTime,tmpSend.DepthMarketDataField.UpdateTime,sizeof(CombQuot.UpdateTime)-1);
                        CombQuot.UpdateMillisec=tmpSend.DepthMarketDataField.UpdateMillisec;
                        CombQuot.PreSettlementPrice=util::GetDoubleInvalidValue();
                    }

                    //下面用Leg1Quot和Leg2Quot来生成组合单的行情
                    //计算最新价、买卖价、买卖量、涨跌停价、行情更新时间
                    bool bUpdate=false;
                    double dval;
                    int ival;
                    if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LastPrice)
                    {
                        if(!util::isInvalidValue(Leg1Quot.LastPrice)&&!util::isInvalidValue(Leg2Quot.LastPrice))
                            dval=Leg1Quot.LastPrice-Leg2Quot.LastPrice;
                        else dval=util::GetDoubleInvalidValue();
                        if(dval!=CombQuot.LastPrice) { CombQuot.LastPrice=dval;  bUpdate=true; }
                    }
                    if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_Volume)
                    {
                        if(!util::isInvalidValue(Leg1Quot.Volume)&&!util::isInvalidValue(Leg2Quot.Volume))
                            ival=Leg1Quot.Volume<= Leg2Quot.Volume ? Leg1Quot.Volume : Leg2Quot.Volume;
                        else ival= util::GetIntInvalidValue();
                        if(ival!=CombQuot.Volume) { CombQuot.Volume=ival;  bUpdate=true; }
                    }
                    if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_AskBid)
                    {
                        if(!util::isInvalidValue(Leg1Quot.AskPrice1)&&!util::isInvalidValue(Leg2Quot.AskPrice1))
                            dval=Leg1Quot.AskPrice1-Leg2Quot.BidPrice1;
                        else dval=util::GetDoubleInvalidValue();
                        if(dval!=CombQuot.AskPrice1) { CombQuot.AskPrice1=dval;  bUpdate=true; }

                        if(!util::isInvalidValue(Leg1Quot.AskVolume1)&&!util::isInvalidValue(Leg2Quot.BidVolume1))
                            ival= Leg1Quot.AskVolume1 <= Leg2Quot.BidVolume1 ? Leg1Quot.AskVolume1 : Leg2Quot.BidVolume1;
                        else ival=util::GetIntInvalidValue();
                        if(ival!=CombQuot.AskVolume1) { CombQuot.AskVolume1=ival;  bUpdate=true; }

                        if(!util::isInvalidValue(Leg1Quot.BidPrice1)&&!util::isInvalidValue(Leg2Quot.BidPrice1))
                            dval=Leg1Quot.BidPrice1-Leg2Quot.AskPrice1;
                        else dval=util::GetDoubleInvalidValue();
                        if(dval!=CombQuot.BidPrice1) { CombQuot.BidPrice1=dval;  bUpdate=true; }

                        if(!util::isInvalidValue(Leg1Quot.BidVolume1)&&!util::isInvalidValue(Leg2Quot.AskVolume1))
                            ival= Leg1Quot.BidVolume1 <= Leg2Quot.AskVolume1 ? Leg1Quot.BidVolume1 : Leg2Quot.AskVolume1;
                        else ival=util::GetIntInvalidValue();
                        if(ival!=CombQuot.BidVolume1) { CombQuot.BidVolume1=ival;  bUpdate=true; }
                    }
                    if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LimitPrice)
                    {
                        if(!util::isInvalidValue(Leg1Quot.LowerLimitPrice)&&!util::isInvalidValue(Leg2Quot.UpperLimitPrice))
                            dval=Leg1Quot.LowerLimitPrice-Leg2Quot.UpperLimitPrice;
                        else dval=util::GetDoubleInvalidValue();
                        if(dval!=CombQuot.LowerLimitPrice) { CombQuot.LowerLimitPrice=dval;  bUpdate=true; }

                        if(!util::isInvalidValue(Leg1Quot.UpperLimitPrice)&&!util::isInvalidValue(Leg2Quot.LowerLimitPrice))
                            dval=Leg1Quot.UpperLimitPrice-Leg2Quot.LowerLimitPrice;
                        else dval=util::GetDoubleInvalidValue();
                        if(dval!=CombQuot.UpperLimitPrice) { CombQuot.UpperLimitPrice=dval;  bUpdate=true; }
                    }
                    if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreSettlementPrice)
                    {
                        if(!util::isInvalidValue(Leg1Quot.PreSettlementPrice)&&!util::isInvalidValue(Leg2Quot.PreSettlementPrice))
                            dval=Leg1Quot.PreSettlementPrice-Leg2Quot.PreSettlementPrice;
                        else dval=util::GetDoubleInvalidValue();
                        if(dval!=CombQuot.PreSettlementPrice) { CombQuot.PreSettlementPrice=dval;  bUpdate=true; }
                    }
                    if(GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreClosePrice)
                    {
                        if(!util::isInvalidValue(Leg1Quot.PreClosePrice)&&!util::isInvalidValue(Leg2Quot.PreClosePrice))
                            dval=Leg1Quot.PreClosePrice-Leg2Quot.PreClosePrice;
                        else dval=util::GetDoubleInvalidValue();
                        if(dval!=CombQuot.PreClosePrice) { CombQuot.PreClosePrice=dval;  bUpdate=true; }
                    }

                    if(bUpdate)
                    {
                        //更新了组合单的行情
                        int cmprlt=memcmp(tmpSend.DepthMarketDataField.UpdateTime,CombQuot.UpdateTime,sizeof(CombQuot.UpdateTime));
                        if(cmprlt>=0)
                        {
                            if(cmprlt>0)
                                strncpy(CombQuot.UpdateTime,tmpSend.DepthMarketDataField.UpdateTime,sizeof(CombQuot.UpdateTime)-1);
                            if(cmprlt==0&&tmpSend.DepthMarketDataField.UpdateMillisec>CombQuot.UpdateMillisec)
                                CombQuot.UpdateMillisec=tmpSend.DepthMarketDataField.UpdateMillisec;
                        }

	                    DataRtnDepthMarketData tmpCombQuot;
                        tmpCombQuot.DepthMarketDataField = CombQuot;
	                    m_RefPackageCache.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(tmpCombQuot), &tmpCombQuot); 
                        SetQuotInfo_Internal(CombQuot);
                    }
                }

            }
        }
    }
//#endif
	
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

#ifdef CTP060300
///订阅询价应答
void CQuotSpi::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pSpecificInstrument&&pRspInfo)
    {
        DataRspSubForQuoteRsp tmpdata(pSpecificInstrument->InstrumentID,*pRspInfo);
	    m_RefPackageCache.SaveDataTo(BID_RspSubForQuoteRsp, 0, 0, 0, sizeof(tmpdata), &tmpdata); 
    }

    LOG_INFO("CQuotSpi::OnRspSubForQuoteRsp(订阅询价应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],pSpecificInstrument=[%x] pRspInfo=[%x] InstrumentID=[%s]",
        pRspInfo?pRspInfo->ErrorID:-1,pRspInfo?pRspInfo->ErrorMsg:"",
        nRequestID,(int)bIsLast,
        pSpecificInstrument,pRspInfo,pSpecificInstrument?pSpecificInstrument->InstrumentID:"");
}
///退订询价应答
void CQuotSpi::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if(pSpecificInstrument&&pRspInfo)
    {
        DataRspUnSubForQuoteRsp tmpdata(pSpecificInstrument->InstrumentID,*pRspInfo);
	    m_RefPackageCache.SaveDataTo(BID_RspUnSubForQuoteRsp, 0, 0, 0, sizeof(tmpdata), &tmpdata); 
    }

    LOG_INFO("CQuotSpi::OnRspUnSubForQuoteRsp(退订询价应答) : ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d],pSpecificInstrument=[%x] pRspInfo=[%x] InstrumentID=[%s]",
        pRspInfo?pRspInfo->ErrorID:-1,pRspInfo?pRspInfo->ErrorMsg:"",
        nRequestID,(int)bIsLast,
        pSpecificInstrument,pRspInfo,pSpecificInstrument?pSpecificInstrument->InstrumentID:"");
}
///询价通知
void CQuotSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
{
    if(!pForQuoteRsp)
        return;

    PlatformStru_ForQuoteRspField ForQuoteRsp(*pForQuoteRsp);
	DataRtnForQuoteRsp tmpdata(ForQuoteRsp);

	m_RefPackageCache.SaveDataTo(BID_RtnForQuoteRsp, 0, 0, 0, sizeof(tmpdata), &tmpdata); 

    LOG_INFO("CQuotSpi::OnRtnForQuoteRsp(询价通知) : ForQuoteRsp=[%s]",
              ForQuoteRsp.tostring().c_str());
}
#endif
