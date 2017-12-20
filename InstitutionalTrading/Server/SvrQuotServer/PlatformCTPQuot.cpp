//#include "stdafx.h"
//
#include <windows.h>
#include "PlatformCTPQuot.h"
#include "FileOpr.h"
#include "tools_util.h"

using std::make_pair;

#pragma unmanaged


#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif




//*************************************************************************
//  CPlatformCTPQuot的实现
//*************************************************************************
CPlatformCTPQuot::CPlatformCTPQuot(const Stru_CTPConnectionParam& ConnParam,bool bPushCache,bool bUseComb,bool bCalcuCombQuotOnLegQuot)
:   CPlatformCTP(ConnParam,bPushCache,bUseComb,bCalcuCombQuotOnLegQuot),
	m_pQuotApi(NULL)
{    
}
CPlatformCTPQuot::~CPlatformCTPQuot()
{    
    Clear(); 

    if(m_pQuotApi)
        Logout();
}

void CPlatformCTPQuot::Clear()
{
    CeasymutexGuard guard(m_mutex);

    m_SubscribedInstrumentID.clear();
    m_LegInstrument2CombSubscribed.clear();
    m_lastQuot.clear();

    memset(m_TradeDay,0,sizeof(TThostFtdcDateType));
}

void CPlatformCTPQuot::GetTradeDay(TThostFtdcDateType szTradeDay)
{
    CeasymutexGuard guard(m_mutex);
    memcpy(szTradeDay,m_TradeDay,sizeof(TThostFtdcDateType));
}

bool CPlatformCTPQuot::Login(const CThostFtdcMdSpi* pSpi)
{
    CeasymutexGuard guard(m_mutex);

    if(m_pQuotApi) 
    {
        LOG_INFO("Login()成功：已经有有效m_pQuotApi(%x)",m_pQuotApi);
        return true;
    }
    if(m_ConnParam.BrokerID.empty()||m_ConnParam.UserID.empty()||m_ConnParam.UserPW.empty())
    {
        LOG_INFO("Login()失败：登录参数(BrokerID、UserID、UserPW)为空");
        return false;
    }

	SetConnStatus(CTPCONNSTATUS_Connecting,true);

    //创建Api对象
    m_pQuotApi = CThostFtdcMdApi::CreateFtdcMdApi();

    //注册回调
    m_pQuotApi->RegisterSpi(static_cast<CThostFtdcMdSpi*>(pSpi?const_cast<CThostFtdcMdSpi*>(pSpi):this));

    //注册前置机地址
    string addrstr;
    for(vector<string>::iterator it=m_ConnParam.FrontAddrs.begin();it!=m_ConnParam.FrontAddrs.end();++it)
    {
        addrstr=MakeConnectStr(*it,m_ConnParam.ProxyConnectStr);
		m_pQuotApi->RegisterFront(const_cast<char*>(addrstr.c_str()));
    }

    //初始化并启动前置机接口
	m_pQuotApi->Init();     

    LOG_INFO("Login CTP：m_pQuotApi(%x)",m_pQuotApi);

    return true;
}
void CPlatformCTPQuot::Logout(void)
{
	m_mutex.lock();
    if(!m_pQuotApi) 
    {
        LOG_INFO("Logout()成功：m_pQuotApi为NULL");
        return;
    }
	SetConnStatus(CTPCONNSTATUS_Disconnecting,true);
	m_mutex.unlock();

	//注意，Release() 会将剩余未处理的行情让回调处理，如果这个地方加锁的话，行情回调的锁会产生死锁。
    m_pQuotApi->Release();

	m_mutex.lock();
    m_pQuotApi=NULL;
	SetConnStatus(CTPCONNSTATUS_Disconnected,true);
    LOG_INFO("Logout()成功");
	m_mutex.unlock();
}

int CPlatformCTPQuot::SubscribeMarketData(const string& InstrumentID)
{
    CeasymutexGuard guard(m_mutex);

    int ret = -999;
	if(InstrumentID.empty()) return ret;

    map<string,int>::iterator it=m_SubscribedInstrumentID.find(InstrumentID);
    if(it==m_SubscribedInstrumentID.end())
    {
        m_SubscribedInstrumentID.insert(make_pair(InstrumentID,1));

        string Leg1InstrumentID,Leg2InstrumentID;
        if(IsCombInstrument(InstrumentID,Leg1InstrumentID,Leg2InstrumentID))
        {
            m_LegInstrument2CombSubscribed.insert(make_pair(Leg1InstrumentID,InstrumentID));
            m_LegInstrument2CombSubscribed.insert(make_pair(Leg2InstrumentID,InstrumentID));
        }

		it=m_SubscribedInstrumentID.find(InstrumentID);
    }
	else it->second++;

    if(it!=m_SubscribedInstrumentID.end())
	{
		if(it->second==1&&m_pQuotApi && GetConnStatus()==CTPCONNSTATUS_Connected)
		{
			char *pInstrumentID = const_cast<char*>(InstrumentID.c_str());
            //Sleep(1000);
			ret=m_pQuotApi->SubscribeMarketData(&pInstrumentID,1);
			LOG_INFO("SubscribeMarketData(定制指定合约行情):ret=[%d],InstrumentID=[%s]",ret, pInstrumentID);
		}
	}

    return 0;
}
int CPlatformCTPQuot::UnSubscribeMarketData(const string& InstrumentID)
{
    CeasymutexGuard guard(m_mutex);

    int ret = -999;

	if(InstrumentID.empty()) return ret;

    map<string,int>::iterator it=m_SubscribedInstrumentID.find(InstrumentID);
    if(it!=m_SubscribedInstrumentID.end())
    {		
		
/*		LOG_INFO("UnSubscribeMarketData(从行情订阅列表中删除合约):InstrumentID=[%s],合约订阅计数=[%d]",InstrumentID.c_str(),it->second);

        if(it->second==0)
        {
            if(m_pQuotApi && GetConnStatus()==CTPCONNSTATUS_Connected)
            {
                char *pInstrumentID = const_cast<char*>(it->first.c_str());
                ret=m_pQuotApi->UnSubscribeMarketData(&pInstrumentID,1);
                LOG_INFO("UnSubscribeMarketData(取消行情订阅):ret=[%d],InstrumentID=[%s]",ret, pInstrumentID);
            }

            if(IsCombInstrument2(it->first))
            {
                for(multimap<string,string>::iterator it2=m_LegInstrument2CombSubscribed.begin();it2!=m_LegInstrument2CombSubscribed.end();)
                {
                    if(it2->second==it->first) it2=m_LegInstrument2CombSubscribed.erase(it2);
                    else it2++;
                }
            }
            m_SubscribedInstrumentID.erase(it);
        }
        */ 
        
        //当合约计数为1时，不能再退订取消合约了，否则一旦断开ctp再次连接后就无法收到第三方订阅的行情了
        int nCount = it->second;
        it->second--;
        if(it->second==0)
        {
            if(IsCombInstrument2(it->first))
            {
                for(multimap<string,string>::iterator it2=m_LegInstrument2CombSubscribed.begin();it2!=m_LegInstrument2CombSubscribed.end();)
                {
                    if(it2->second==it->first) it2=m_LegInstrument2CombSubscribed.erase(it2);
                    else it2++;
                }
            }
        }
        
		if(nCount>1 && m_pQuotApi && GetConnStatus()==CTPCONNSTATUS_Connected)
		{
			char *pInstrumentID = const_cast<char*>(it->first.c_str());
			ret=m_pQuotApi->UnSubscribeMarketData(&pInstrumentID,1);
            LOG_INFO("UnSubscribeMarketData(退订行情):InstrumentID=[%s] 合约订阅计数=[%d]",pInstrumentID,--nCount);
            
            m_SubscribedInstrumentID.erase(it);
		}
    }
    return ret;
}
int CPlatformCTPQuot::UnSubscribeAllMarketData()
{
    CeasymutexGuard guard(m_mutex);

	LOG_INFO("UnSubscribeAllMarketData(删除全部行情订阅列表)");

    int ret = -999;

	if(m_pQuotApi && GetConnStatus()==CTPCONNSTATUS_Connected)
	{
		for(map<string,int>::iterator it=m_SubscribedInstrumentID.begin();it!=m_SubscribedInstrumentID.end();it++)
		{
			char *pInstrumentID = const_cast<char*>(it->first.c_str());
            ret=m_pQuotApi->UnSubscribeMarketData(&pInstrumentID,1);
            LOG_INFO("UnSubscribeAllMarketData(取消行情订阅):ret=[%d],InstrumentID=[%s]",ret, pInstrumentID);
		}
	}
	m_SubscribedInstrumentID.clear();
	m_LegInstrument2CombSubscribed.clear();
    return ret;
}

vector<string> CPlatformCTPQuot::GetSubscribedInstrumentIDs(void) const 
{ 
    CeasymutexGuard guard(m_mutex);  
    vector<string> rlt;
    for(map<string,int>::const_iterator it=m_SubscribedInstrumentID.begin();it!=m_SubscribedInstrumentID.end();it++)
        rlt.push_back(it->first);
    return rlt; 
}

bool CPlatformCTPQuot::GetLastQuotMap(map<string,CThostFtdcDepthMarketDataField>& mapLastQuot)
{
    mapLastQuot = m_lastQuot;
    return m_lastQuot.size()>0;
};
///****************************************************************************
/// 下面是ctp回调函数
///****************************************************************************

///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CPlatformCTPQuot::OnFrontConnected()
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Quot_FrontConnected, NULL, 0);

	SetConnStatus(CTPCONNSTATUS_Logining,true);

    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_ConnParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_ConnParam.UserID.c_str(),sizeof(req.UserID)-1);
    strncpy(req.Password, m_ConnParam.UserPW.c_str(),sizeof(req.Password)-1);
	strncpy(req.OneTimePassword,m_ConnParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);
    int RequestID=m_RequestID++;
    int iRlt = m_pQuotApi->ReqUserLogin(&req, RequestID);

    LOG_INFO("CPlatformCTPQuot::OnFrontConnected(与交易所成功建立TCP连接):\n"
        "\t\t\t\t\t\t\t\t\t --->ReqUserLogin(用户登录请求):ret=[%d],iRequestID=[%d],BrokerID=[%s],UserID=[%s]\n",
        iRlt, RequestID,req.BrokerID, req.UserID);
}

///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
///@param nReason 错误原因
///        0x1001 网络读失败
///        0x1002 网络写失败
///        0x2001 接收心跳超时
///        0x2002 发送心跳失败
///        0x2003 收到错误报文
void CPlatformCTPQuot::OnFrontDisconnected(int nReason)
{ 
    CeasymutexGuard guard(m_mutex);

	SetConnStatus(CTPCONNSTATUS_Disconnected,true);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Quot_FrontDisconnected,NULL,0,NULL,0,nReason);

    LOG_INFO("CPlatformCTPQuot::OnFrontDisconnected(与交易所失去TCP连接):nReason=[%#x]",nReason);
}
    
///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
void CPlatformCTPQuot::OnHeartBeatWarning(int nTimeLapse)
{
	LOG_INFO("CPlatformCTPQuot::OnHeartBeatWarning:nTimeLapse=[%x]",nTimeLapse);
}

///登录请求响应
void CPlatformCTPQuot::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(pRspUserLogin)
    {
        DWORD tickcount=GetTickCount();
        if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Quot_RspUserLogin, pRspUserLogin,sizeof(*pRspUserLogin),pRspInfo,pRspInfo?sizeof(*pRspInfo):0,(unsigned int)nRequestID,(unsigned int)bIsLast,tickcount);

        LOG_INFO("CPlatformCTPQuot::OnRspUserLogin(登录请求响应):\n"
                        "\t\t\t\t\t\t\t\t\t ErrorID=[%d],ErrorMsg=[%s],nRequestID=[%d],bIsLast=[%d]\n"
                        "\t\t\t\t\t\t\t\t\t TradingDay=[%s],\t LoginTime=[%s],\t BrokerID=[%s],\t UserID=[%s],\t SystemName=[%s]\n"
                        "\t\t\t\t\t\t\t\t\t FrontID=[%d],\t SessionID=[%#x],\t MaxOrderRef[%s],\t SHFETime=[%s],\t DCETime=[%s],\n"
                        "\t\t\t\t\t\t\t\t\t CZCETime[%s],\t FFEXTime=[%s],\t CurTickCount=[%ul]",
                        pRspInfo?pRspInfo->ErrorID:-1, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
                        pRspUserLogin->TradingDay,pRspUserLogin->LoginTime,pRspUserLogin->BrokerID   , pRspUserLogin->UserID,pRspUserLogin->SystemName, 
                        pRspUserLogin->FrontID  , pRspUserLogin->SessionID ,pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime , pRspUserLogin->DCETime, 
                        pRspUserLogin->CZCETime   , pRspUserLogin->FFEXTime,tickcount);
    }
    else
    {
        LOG_INFO("CPlatformCTPQuot::OnRspUserLogin(登录请求响应),pRspUserLogin is NULL");
    }

    if(pRspUserLogin&&(!pRspInfo||pRspInfo->ErrorID==0))
    {
		SetConnStatus(CTPCONNSTATUS_Connected,true); 

        //todo 根据合约定制行情  网络断开后要重新订阅一下，否则可能收不到行情
        for(map<string,int>::const_iterator it=m_SubscribedInstrumentID.begin();it!=m_SubscribedInstrumentID.end();it++)
        {
            char *pInstrumentID = const_cast<char*>(it->first.c_str());
            m_pQuotApi->SubscribeMarketData(&pInstrumentID,1);
			LOG_INFO("CPlatformCTPQuot::OnRspUserLogin(登录请求响应),重新订阅行情，InstrumentID = %s",pInstrumentID);
        }
    }
    else
    {
		SetConnStatus(CTPCONNSTATUS_LoginFailure,true);
    }
    
}

///错误应答
void CPlatformCTPQuot::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pRspInfo)
		LOG_INFO("CPlatformCTPQuot::OnRspError : ErrCode=[%d] ErrMsg=[%s],RequestID=[%d],IsLast=[%d]",
				 pRspInfo->ErrorID,pRspInfo->ErrorMsg,nRequestID,(int)bIsLast);
}

///订阅行情应答
void CPlatformCTPQuot::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pSpecificInstrument&&(!pRspInfo||pRspInfo->ErrorID==0))
	{
		LOG_INFO("CPlatformCTPQuot::OnRspSubMarketData(订阅行情应答,Unimplemented) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],nRequestID=[%d],bIsLast=[%d]",
			pRspInfo,pRspInfo?pRspInfo->ErrorID:0,pRspInfo?pRspInfo->ErrorMsg:"",
			pSpecificInstrument->InstrumentID,nRequestID,(int)bIsLast);
	}
	else
	{
        LOG_INFO("CPlatformCTPQuot::OnRspSubMarketData(订阅行情应答,Unimplemented),pSpecificInstrument is NULL");
	}
}

///取消订阅行情应答
void CPlatformCTPQuot::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pSpecificInstrument&&(!pRspInfo||pRspInfo->ErrorID==0))
	{
		LOG_INFO("CPlatformCTPQuot::OnRspUnSubMarketData(取消订阅行情应答,Unimplemented) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],InstrumentID=[%s],nRequestID=[%d],bIsLast=[%d]",
			pRspInfo,pRspInfo?pRspInfo->ErrorID:0,pRspInfo?pRspInfo->ErrorMsg:"",
			pSpecificInstrument->InstrumentID,nRequestID,(int)bIsLast);
	}
    else
	{
        LOG_INFO("CPlatformCTPQuot::OnRspUnSubMarketData(取消订阅行情应答,Unimplemented),pSpecificInstrument is NULL");
	}
}


///深度行情通知
void CPlatformCTPQuot::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
  
    CeasymutexGuard guard(m_mutex);
  
    if(!pDepthMarketData)
        return;

    static bool blogID=false;
    if(blogID)
    {
        LOG_INFO("CPlatformCTPQuot::OnRtnDepthMarketData:InstrumentID=%s,LastPrice=%g,UpdateTime=%s",
            pDepthMarketData->InstrumentID,pDepthMarketData->LastPrice,pDepthMarketData->UpdateTime);
    }

    CThostFtdcDepthMarketDataField MD=*pDepthMarketData;

    if(m_bUseComb&&
        IsCombInstrument2(MD.InstrumentID))
    {
        if(MD.PreSettlementPrice==util::GetDoubleInvalidValue()||
            MD.LastPrice==util::GetDoubleInvalidValue()||
            MD.LowerLimitPrice==0||
            MD.UpperLimitPrice==0)
        {
            if(m_bCalcuCombQuotOnLegQuot)
            {
                //自行计算昨结算价、最新价、涨跌停板
                string Leg1InstrumentID,Leg2InstrumentID;
                CThostFtdcDepthMarketDataField Leg1Quot,Leg2Quot;
                if(IsCombInstrument(MD.InstrumentID,Leg1InstrumentID,Leg2InstrumentID)&&
                    GetQuotInfo_Internal(Leg1InstrumentID,Leg1Quot)&&
                    GetQuotInfo_Internal(Leg2InstrumentID,Leg2Quot))
                {
                    if(Leg1Quot.LastPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.LastPrice!=util::GetDoubleInvalidValue())
                    {
                        MD.LastPrice=Leg1Quot.LastPrice-Leg2Quot.LastPrice;
                    }
                    if(Leg1Quot.AskPrice1!=util::GetDoubleInvalidValue()&&Leg2Quot.AskPrice1!=util::GetDoubleInvalidValue())
                    {
                        MD.AskPrice1=Leg1Quot.AskPrice1-Leg2Quot.BidPrice1;
                    }
                    if(Leg1Quot.AskVolume1!=util::GetIntInvalidValue()&&Leg2Quot.BidVolume1!=util::GetIntInvalidValue())
                    {
                        MD.AskVolume1 = Leg1Quot.AskVolume1 <= Leg2Quot.BidVolume1 ? Leg1Quot.AskVolume1 : Leg2Quot.BidVolume1;
                    }
                    if(Leg1Quot.BidPrice1!=util::GetDoubleInvalidValue()&&Leg2Quot.BidPrice1!=util::GetDoubleInvalidValue())
                    {
                        MD.BidPrice1=Leg1Quot.BidPrice1-Leg2Quot.AskPrice1;
                    }
                    if(Leg1Quot.BidVolume1!=util::GetIntInvalidValue()&&Leg2Quot.AskVolume1!=util::GetIntInvalidValue())
                    {
                        MD.BidVolume1 = Leg1Quot.BidVolume1 <= Leg2Quot.AskVolume1 ? Leg1Quot.BidVolume1 : Leg2Quot.AskVolume1;
                    }
                    if(MD.LowerLimitPrice==util::GetDoubleInvalidValue()&&Leg1Quot.LowerLimitPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.UpperLimitPrice!=util::GetDoubleInvalidValue())
                    {
                        MD.LowerLimitPrice=Leg1Quot.LowerLimitPrice-Leg2Quot.UpperLimitPrice;
                    }
                    if(MD.UpperLimitPrice==util::GetDoubleInvalidValue()&&Leg1Quot.UpperLimitPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.LowerLimitPrice!=util::GetDoubleInvalidValue())
                    {
                        MD.UpperLimitPrice=Leg1Quot.UpperLimitPrice-Leg2Quot.LowerLimitPrice;
                    }
                    if(MD.PreSettlementPrice==util::GetDoubleInvalidValue()&&
                        Leg1Quot.PreSettlementPrice!=util::GetDoubleInvalidValue()&&
                        Leg2Quot.PreSettlementPrice!=util::GetDoubleInvalidValue())
                    {
                        MD.PreSettlementPrice=Leg1Quot.PreSettlementPrice-Leg2Quot.PreSettlementPrice;
                    }
                    //现量，为两腿现量的最小值
                    if(Leg1Quot.Volume!=util::GetIntInvalidValue()&&Leg2Quot.Volume!=util::GetIntInvalidValue())
                    {
                        MD.Volume = Leg1Quot.Volume <= Leg2Quot.Volume ? Leg1Quot.Volume : Leg2Quot.Volume;
                    }

                }
            }
        }
        if(MD.HighestPrice==0) 
            MD.HighestPrice=util::GetDoubleInvalidValue();
        if(MD.LowestPrice==0)
            MD.LowestPrice=util::GetDoubleInvalidValue();
        if(MD.Volume==0)
            MD.Volume=util::GetIntInvalidValue();
        if(MD.OpenPrice==0)
            MD.OpenPrice=util::GetDoubleInvalidValue();
        if(MD.ClosePrice==0)
            MD.ClosePrice=util::GetDoubleInvalidValue();
        if(MD.PreClosePrice==0)
            MD.PreClosePrice=util::GetDoubleInvalidValue();
    }

	if(m_bPushCache)
		m_Cache.SaveDataTo2(CmdID_Quot_RtnDepthMarketData, &MD, sizeof(MD), NULL, 0, GetTickCount()); 
	PostPkg_Internal(CmdID_Quot_RtnDepthMarketData,&MD,sizeof(MD));

    SetQuotInfo_Internal(MD);

    if(m_bUseComb&&m_bCalcuCombQuotOnLegQuot)
    {
        string ThisInstrumentID(pDepthMarketData->InstrumentID);
        vector<string> CombInstrumentIDs;
        string Leg1InstrumentID,Leg2InstrumentID;
        CThostFtdcDepthMarketDataField Leg1Quot,Leg2Quot,CombQuot;

        //对于非组合单，并且是某些组合单的单腿的情况进行处理
        if(!IsCombInstrument2(ThisInstrumentID)&&
            GetCombInstrumentIDs_IncludeLeg_Subscribed_Internal(ThisInstrumentID,CombInstrumentIDs))
        {
            //此时CombInstrumentIDs存放着和单腿有关的组合合约
            //针对每一个组合合约，生成行情
            for(int i=0;i<(int)CombInstrumentIDs.size();i++)
            {
                if(IsCombInstrument(CombInstrumentIDs[i],Leg1InstrumentID,Leg2InstrumentID))
                {
                    //获取Leg1Quot和Leg2Quot
                    if(Leg1InstrumentID==ThisInstrumentID)
                    {
                        Leg1Quot=MD;
                        if(!GetQuotInfo_Internal(Leg2InstrumentID, Leg2Quot))
                            continue;
                    }
                    else if(Leg2InstrumentID==ThisInstrumentID)
                    {
                        if(!GetQuotInfo_Internal(Leg1InstrumentID, Leg1Quot))
                            continue;
                        Leg2Quot=MD;
                    }
                    else continue;

                    //获取CombQuot
                    if(!GetQuotInfo_Internal(CombInstrumentIDs[i], CombQuot))
                    {
                        memset(&CombQuot,0,sizeof(CombQuot));
                        strncpy(CombQuot.TradingDay,MD.TradingDay,sizeof(CombQuot.TradingDay)-1);
                        strncpy(CombQuot.InstrumentID,CombInstrumentIDs[i].c_str(),sizeof(CombQuot.InstrumentID)-1);
                        strncpy(CombQuot.ExchangeID,MD.ExchangeID,sizeof(CombQuot.ExchangeID)-1);
                        strncpy(CombQuot.ExchangeInstID,CombInstrumentIDs[i].c_str(),sizeof(CombQuot.ExchangeInstID)-1);
                        strncpy(CombQuot.UpdateTime,MD.UpdateTime,sizeof(CombQuot.UpdateTime)-1);
                        CombQuot.UpdateMillisec=MD.UpdateMillisec;
                        CombQuot.PreSettlementPrice=util::GetDoubleInvalidValue();
                    }

                    //下面用Leg1Quot和Leg2Quot来生成组合单的行情
                    //计算最新价、买卖价、买卖量、涨跌停价、行情更新时间
                    bool bUpdate=false;
                    if(Leg1Quot.LastPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.LastPrice!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.LastPrice=Leg1Quot.LastPrice-Leg2Quot.LastPrice;
                        bUpdate=true;
                    }
                    if(Leg1Quot.AskPrice1!=util::GetDoubleInvalidValue()&&Leg2Quot.AskPrice1!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.AskPrice1=Leg1Quot.AskPrice1-Leg2Quot.BidPrice1;
                        bUpdate=true;
                    }
                    if(Leg1Quot.AskVolume1!=util::GetIntInvalidValue()&&Leg2Quot.BidVolume1!=util::GetIntInvalidValue())
                    {
                        CombQuot.AskVolume1 = Leg1Quot.AskVolume1 <= Leg2Quot.BidVolume1 ? Leg1Quot.AskVolume1 : Leg2Quot.BidVolume1;
                        bUpdate=true;
                    }
                    if(Leg1Quot.BidPrice1!=util::GetDoubleInvalidValue()&&Leg2Quot.BidPrice1!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.BidPrice1=Leg1Quot.BidPrice1-Leg2Quot.AskPrice1;
                        bUpdate=true;
                    }
                    if(Leg1Quot.BidVolume1!=util::GetIntInvalidValue()&&Leg2Quot.AskVolume1!=util::GetIntInvalidValue())
                    {
                        CombQuot.BidVolume1 = Leg1Quot.BidVolume1 <= Leg2Quot.AskVolume1 ? Leg1Quot.BidVolume1 : Leg2Quot.AskVolume1;
                        bUpdate=true;
                    }
                    if(CombQuot.LowerLimitPrice==util::GetDoubleInvalidValue()&&Leg1Quot.LowerLimitPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.UpperLimitPrice!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.LowerLimitPrice=Leg1Quot.LowerLimitPrice-Leg2Quot.UpperLimitPrice;
                        bUpdate=true;
                    }
                    if(CombQuot.UpperLimitPrice==util::GetDoubleInvalidValue()&&Leg1Quot.UpperLimitPrice!=util::GetDoubleInvalidValue()&&Leg2Quot.LowerLimitPrice!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.UpperLimitPrice=Leg1Quot.UpperLimitPrice-Leg2Quot.LowerLimitPrice;
                        bUpdate=true;
                    }
                    if(CombQuot.PreSettlementPrice==util::GetDoubleInvalidValue()&&
                        Leg1Quot.PreSettlementPrice!=util::GetDoubleInvalidValue()&&
                        Leg2Quot.PreSettlementPrice!=util::GetDoubleInvalidValue())
                    {
                        CombQuot.PreSettlementPrice=Leg1Quot.PreSettlementPrice-Leg2Quot.PreSettlementPrice;
                        bUpdate=true;
                    }
                    //现量，为两腿现量的最小值
                    if(Leg1Quot.Volume!=util::GetIntInvalidValue()&&Leg2Quot.Volume!=util::GetIntInvalidValue())
                    {
                        CombQuot.Volume = Leg1Quot.Volume <= Leg2Quot.Volume ? Leg1Quot.Volume : Leg2Quot.Volume;
                        bUpdate=true;
                    }


                    if(bUpdate)
                    {
                        //更新了组合单的行情
                        int cmprlt=memcmp(MD.UpdateTime,CombQuot.UpdateTime,sizeof(CombQuot.UpdateTime));
                        if(cmprlt>=0)
                        {
                            if(cmprlt>0)
                                strncpy(CombQuot.UpdateTime,MD.UpdateTime,sizeof(CombQuot.UpdateTime)-1);
                            if(cmprlt==0&&MD.UpdateMillisec>CombQuot.UpdateMillisec)
                                CombQuot.UpdateMillisec=MD.UpdateMillisec;
                        }

                        SetQuotInfo_Internal(CombQuot);
						if(m_bPushCache)
							m_Cache.SaveDataTo2(CmdID_Quot_RtnDepthMarketData, &CombQuot, sizeof(CombQuot), NULL, 0, GetTickCount()); 
						PostPkg_Internal(CmdID_Quot_RtnDepthMarketData,&CombQuot,sizeof(CombQuot));
                    }
                }
            }
        }
    }
}
///获取包含此单腿合约的组合合约列表
bool CPlatformCTPQuot::GetCombInstrumentIDs_IncludeLeg_Subscribed_Internal(const string& LegInstrument,vector<string>& vecCombInstruments)
{
    bool ret=false;
    multimap<string,string>::iterator itlower=m_LegInstrument2CombSubscribed.lower_bound(LegInstrument);
    multimap<string,string>::iterator itupper=m_LegInstrument2CombSubscribed.upper_bound(LegInstrument);
    vecCombInstruments.clear();
    while(itlower!=itupper)
    {
        vecCombInstruments.push_back(itlower->second);
        itlower++;
    }
    return vecCombInstruments.size()==0?false:true;
}


//设置指定合约行情，设置的行情更新，则返回true；否则返回false（比原来的行情更老）
//仅用于OnRtnDepthMarketData()中
bool CPlatformCTPQuot::SetQuotInfo_Internal(const CThostFtdcDepthMarketDataField& inData)
{
    bool brlt=true;

    map<string,CThostFtdcDepthMarketDataField>::iterator it = m_lastQuot.find(string(inData.InstrumentID));
    m_lastQuot[string(inData.InstrumentID)] = inData;

    return brlt;
}

//获取指定合约行情。仅用于OnRtnDepthMarketData()中
bool CPlatformCTPQuot::GetQuotInfo_Internal(const string& InstrumentID, CThostFtdcDepthMarketDataField& outData)
{
    bool brlt=false;

    map<string,CThostFtdcDepthMarketDataField>::iterator it =  m_lastQuot.find(InstrumentID);
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

//判断一个合约是否是组合单合约。如果是，返回单腿合约名称。
//目前仅能判断大商和郑商的组合合约
//判断依据是前有空格后有&，如SP c1305&c1309
bool CPlatformCTPQuot::IsCombInstrument(const string& strInstrument,string& LegInstrument1,string& LegInstrument2)
{
    int len=strInstrument.length();
    int pos1=strInstrument.find(" ");
    if(pos1==-1) return false;
    int pos2=strInstrument.find("&",pos1);
    if(pos2==-1) return false;
    LegInstrument1=strInstrument.substr(pos1+1,pos2-pos1-1);
    LegInstrument2=strInstrument.substr(pos2+1,len-pos2-1);
    return true;
}
//判断一个合约是否是组合单合约。
//目前仅能判断大商和郑商的组合合约
//判断依据是前有空格后有&，如SP c1305&c1309
bool CPlatformCTPQuot::IsCombInstrument2(const string& strInstrument)
{
    int len=strInstrument.length();
    int pos1=strInstrument.find(" ");
    if(pos1==-1) return false;
    int pos2=strInstrument.find("&",pos1);
    if(pos2==-1) return false;
    return true;
}


