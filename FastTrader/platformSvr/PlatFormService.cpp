#include "stdafx.h"

#include "PlatFormService.hpp"
#include "DataCenter.hpp"
#include "wx/thread.h"
#include "../inc/Module-Misc2/packagecache.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"
#include "FileOpr.h"
#include "tmpstackbuf.h"

using std::string;

#ifndef LOG_INFO
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
#endif

//extern std::map<std::string, int> g_allPlatformID;

#include "TcpPortMap.h"


//****************************************************************************
//  代理服务器的处理: 
//  1. socks4/socks4a/socks5: 直接连接socks5:// asp-sim2-front1.financial-trading-platform.com:26205/user:pass@127.0.0.1:10001接口
//  2. http :   (1) 首先创建端口映射对象CTcpPortMap 将127.0.0.1:5739映射到http proxy server
//              (2) 让ctp连接127.0.0.1:5739
//              (3) release ctp时, 注销CTcpPortMap对象
//****************************************************************************
std::string MakeConnectStr(const std::string& addr,const Stru_ProxyServerParam& ProxyParam)
{
	string rltstr;

    //去掉左右空白和"tcp://"
    string deststr=addr;
	CTools_Ansi::mytrim(deststr);
    int pos=deststr.find("tcp://");
    if(pos!=string::npos) deststr=deststr.substr(pos+6);

	//代理格式: "socks5://asp-sim2-front1.financial-trading-platform.com:26205/user:pass@127.0.0.1:10001"
    //ConnectStr格式: "socks4://10.11.112.172:8888"

    //if(!g_pTcpPortMap4HttpProxy)
    //    g_pTcpPortMap4HttpProxy=new CTcpPortMap();
    if(ProxyParam.ProxyType==string("socks4")||
       ProxyParam.ProxyType==string("socks4a")||
       ProxyParam.ProxyType==string("socks5"))
    {
        rltstr=
            ProxyParam.ProxyType+string("://")+
            deststr+string("/")+
            ProxyParam.ProxyUser+string(":")+
            ProxyParam.ProxyPassword+string("@")+
            ProxyParam.ProxyIP+string(":")+
            ProxyParam.ProxyPort;
    }
    else if(ProxyParam.ProxyType==string("http"))
    {
        CTools_Ansi::PerformWSAStartup();

        Stru_IPAddress DestAddr(deststr);
        if(!CTcpPortMap::getObj().IsDestAddrExist(DestAddr))
        {
            Stru_IPAddress BindAddr(string("127.0.0.1:0"));
            Stru_IPAddress ProxySvrAddr(ProxyParam.dwProxyIP,ProxyParam.usProxyPort);
            CTcpPortMap::Stru_PortMapParam PortMapParam(BindAddr,DestAddr,CTcpPortMap::Map_BypassHttpProxySvr,ProxySvrAddr);
            //创建一个端口映射, 绑定地址为127.0.0.1:ProxyParam.PortMapParam.Port
            CTcpPortMap::getObj().NewPortMap(PortMapParam);
            //让ctp指向127.0.0.1:bindport
            char buf[256];
            _snprintf(buf,sizeof(buf)-1,"tcp://127.0.0.1:%d",PortMapParam.BindAddr.Port);
            rltstr=string(buf);
        }

        CTools_Ansi::PerformWSACleanup();
    }
    else rltstr=string("tcp://")+deststr;

    return rltstr;
}


class CTradeThread : public wxThread
{
public:
    CTradeThread(CPlatFormService& RefPlatFormService)
    :wxThread(),
    m_PlatFormService(RefPlatFormService),
	m_FrontAddrs(RefPlatFormService.m_PlatformParam.TradeFrontAddrs),
	m_ProxyParam(RefPlatFormService.m_PlatformParam.ProxyParam)
    {

    }

    // thread execution starts here
    virtual void *Entry()
    {
        if(GlobalConfigManager::m_Test_bLogThreadID)
            CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCTradeThread::Entry",GetCurrentThreadId());

        m_PlatFormService.m_pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);
        m_PlatFormService.m_pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);
		std::string addrstr;
        for(std::vector<std::string>::iterator it=m_FrontAddrs.begin();it!=m_FrontAddrs.end();++it)
        {
			addrstr=MakeConnectStr(*it,m_ProxyParam);
//            addrstr=string("socks4://180.168.102.193:41205/:@180.169.125.49:8888");
//            addrstr=string("http://180.168.102.193:41205/:@61.181.131.102:9999");
            m_PlatFormService.m_pTradeApi->RegisterFront(const_cast<char*>(addrstr.c_str()));// 连接前置机
        }
    	m_PlatFormService.m_pTradeApi->Init();
    	m_PlatFormService.m_pTradeApi->Join();
        
        while(!TestDestroy())//等待CPlatFormService::LogoutTrade主动杀掉此线程
        {
            wxThread::Sleep(1);
        }
        
        return NULL;
    }

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit()
    {
        m_PlatFormService.m_pTradeWorkThread = NULL;
    }

private:
    CPlatFormService&       m_PlatFormService;
    Stru_ProxyServerParam   m_ProxyParam;
    std::vector<std::string> m_FrontAddrs;
};

class CQuotThread : public wxThread
{
public:
    CQuotThread(CPlatFormService& RefPlatFormService)
    :wxThread(),
    m_PlatFormService(RefPlatFormService),
	m_FrontAddrs(RefPlatFormService.m_PlatformParam.QuotFrontAddrs),
	m_ProxyParam(RefPlatFormService.m_PlatformParam.ProxyParam)
    {
    }

    // thread execution starts here
    virtual void *Entry()
    {
        if(GlobalConfigManager::m_Test_bLogThreadID)
            CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCQuotThread::Entry",GetCurrentThreadId());

		std::string addrstr;
        for(std::vector<std::string>::iterator it=m_FrontAddrs.begin();it!=m_FrontAddrs.end();++it)
        {
			addrstr=MakeConnectStr(*it,m_ProxyParam);
			m_PlatFormService.m_pQuotApi->RegisterFront(const_cast<char*>(addrstr.c_str()));// 连接前置机
        }
    	m_PlatFormService.m_pQuotApi->Init();
    	m_PlatFormService.m_pQuotApi->Join();

        while(!TestDestroy())//等待CPlatFormService::LogoutQuot主动杀掉此线程
        {
            wxThread::Sleep(1);
        }
        
        return NULL;
    }

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit()
    {
        m_PlatFormService.m_pQuotWorkThread = NULL;
    }

private:
    CPlatFormService&       m_PlatFormService;
    Stru_ProxyServerParam   m_ProxyParam;
    std::vector<std::string> m_FrontAddrs;

};

//仅限于CPlatFormService的成员函数使用
#define CHECK_TRADE_STATUS() \
    if(!m_pTradeApi || m_PlatformParam.TradeStatus!=CTPCONNSTATUS_Connected)\
    {\
        LOG_INFO("交易API还未准备就绪, 可能登入未成功或正在重连");\
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

    CThostFtdcUserPasswordUpdateField req={0};
    strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_PlatformParam.UserID.c_str(), sizeof(req.UserID)-1);
    strncpy(req.NewPassword, pUserPasswordUpdate->NewPassword, sizeof(req.NewPassword)-1);
    strncpy(req.OldPassword, pUserPasswordUpdate->OldPassword, sizeof(req.OldPassword)-1);
    ret = m_pTradeApi->ReqUserPasswordUpdate(&req, nRequestID); 
    LOG_INFO("ReqUserPasswordUpdate(用户口令更新请求):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],UserID=[%s],OldPassword=[******],NewPassword=[******]",
                     ret, nRequestID,
                     req.BrokerID,
                     req.UserID);
    return ret;
}

///资金账户口令更新请求
int CPlatFormService::ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    CThostFtdcTradingAccountPasswordUpdateField req={0};
    strncpy(req.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(req.BrokerID)-1); 
    strncpy(req.AccountID, m_PlatformParam.UserID.c_str(), sizeof(req.AccountID)-1);
    strncpy(req.OldPassword, pTradingAccountPasswordUpdate->OldPassword, sizeof(req.OldPassword)-1);
    strncpy(req.NewPassword, pTradingAccountPasswordUpdate->NewPassword, sizeof(req.NewPassword)-1);
    strncpy(req.CurrencyID, pTradingAccountPasswordUpdate->CurrencyID, sizeof(req.CurrencyID)-1);

    ret = m_pTradeApi->ReqTradingAccountPasswordUpdate(&req, nRequestID);  
    LOG_INFO("ReqTradingAccountPasswordUpdate(资金账户口令更新请求):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],AccountID=[%s],OldPassword=[******],NewPassword=[******],CurrencyID=[%s]",
                     ret, nRequestID,
                     req.BrokerID,
                     req.AccountID,
                     req.CurrencyID);
    return ret;
}

///报单录入请求. 返回时pInputOrder中的RequestID, BrokerID, InvestorID, UserID, OrderRef的值被修改为底层确定的值
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

	CThostFtdcInputOrderField tField = {0};

	strcpy(tField.BrokerID, pInputOrder->BrokerID);					///经纪公司代码
	strcpy(tField.InvestorID, pInputOrder->InvestorID);				///投资者代码
	strcpy(tField.InstrumentID, pInputOrder->InstrumentID);			///合约代码
	strcpy(tField.OrderRef, pInputOrder->OrderRef);					///报单引用
	strcpy(tField.UserID, pInputOrder->UserID);						///用户代码
	tField.OrderPriceType = pInputOrder->OrderPriceType;			///报单价格条件
	tField.Direction = pInputOrder->Direction;						///买卖方向
	strcpy(tField.CombOffsetFlag, pInputOrder->CombOffsetFlag);		///组合开平标志
	strcpy(tField.CombHedgeFlag, pInputOrder->CombHedgeFlag);		///组合投机套保标志
	tField.LimitPrice = pInputOrder->LimitPrice;					///价格
	tField.VolumeTotalOriginal = pInputOrder->VolumeTotalOriginal;	///数量
	tField.TimeCondition = pInputOrder->TimeCondition;				///有效期类型
	strcpy(tField.GTDDate, pInputOrder->GTDDate);					///GTD日期
	tField.VolumeCondition = pInputOrder->VolumeCondition;			///成交量类型
	tField.MinVolume = pInputOrder->MinVolume;						///最小成交量
	tField.ContingentCondition = pInputOrder->ContingentCondition;	///触发条件
	tField.StopPrice = pInputOrder->StopPrice;						///止损价
	tField.ForceCloseReason = pInputOrder->ForceCloseReason;		///强平原因
	tField.IsAutoSuspend = pInputOrder->IsAutoSuspend;				///自动挂起标志
	strcpy(tField.BusinessUnit, pInputOrder->BusinessUnit);			///业务单元
	tField.RequestID = pInputOrder->RequestID;						///请求编号
	tField.UserForceClose = pInputOrder->UserForceClose;			///用户强评标志

    ////对于中金所的组合合约，把HedgeFlag设置为套利
    //PlatformStru_InstrumentInfo InstrumentInfo;
    //if(pInputOrder->CombHedgeFlag[0]==THOST_FTDC_HF_Speculation&&
    //    GetInstrumentInfo(string(pInputOrder->InstrumentID),InstrumentInfo)==0&&
    //    string(InstrumentInfo.ExchangeID)==string("CFFEX")&&
    //    InstrumentInfo.ProductClass==THOST_FTDC_PC_Combination)
    //    tField.CombHedgeFlag[0]=THOST_FTDC_HF_Arbitrage;


    ret = m_pTradeApi->ReqOrderInsert(&tField, nRequestID);  
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


///预埋单录入请求. 返回时pInputOrder中的RrequestID, BrokerID, InvestorID, UserID, OrderRef的值被修改为底层确定的值
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
    ret = m_pTradeApi->ReqParkedOrderAction(&pParkedOrderAction->Thost, nRequestID);  
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
    ret = m_pTradeApi->ReqOrderAction(&pInputOrderAction->Thost, nRequestID); 
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
///报单操作请求(撤单)
int CPlatFormService::ReqCancelOrder(const OrderKey& orderkey, int nRequestID)
{
	PlatformStru_OrderInfo OrderInfo;
    if(!m_pDataCenter->GetOrder(orderkey,OrderInfo)) 
		return -1;

	PlatformStru_InputOrderAction ReqData;
    ReqData.Thost.FrontID=OrderInfo.FrontID;
    ReqData.Thost.SessionID=OrderInfo.SessionID;
    memcpy(ReqData.Thost.OrderRef, OrderInfo.OrderRef, sizeof(OrderInfo.OrderRef));
    memcpy(ReqData.Thost.OrderSysID, OrderInfo.OrderSysID, sizeof(OrderInfo.OrderSysID));
    memcpy(ReqData.Thost.ExchangeID, OrderInfo.ExchangeID, sizeof(OrderInfo.ExchangeID));
	memcpy(ReqData.Thost.InstrumentID, OrderInfo.InstrumentID, sizeof(OrderInfo.InstrumentID));
	return ReqOrderAction(&ReqData,nRequestID);
}

///查询最大报单数量请求
int CPlatFormService::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume *pQueryMaxOrderVolume, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQueryMaxOrderVolume(&pQueryMaxOrderVolume->Thost, nRequestID);  
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
    ret = m_pTradeApi->ReqRemoveParkedOrder(&pRemoveParkedOrder->Thost, nRequestID);  
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
    ret = m_pTradeApi->ReqRemoveParkedOrderAction(&pRemoveParkedOrderAction->Thost, nRequestID);  
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


    ret = m_pTradeApi->ReqQryOrder(&pQryOrder->Thost, nRequestID);
    if(ret==0)
        m_pDataCenter->ClearMapQryRltOrders();


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
    ret = m_pTradeApi->ReqQryTrade(&pQryTrade->Thost, nRequestID);


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

	CThostFtdcQryInvestorPositionField field;
	memset( &field, 0, sizeof(field));
	strncpy(field.BrokerID, pQryInvestorPosition->BrokerID,sizeof(field.BrokerID)-1);
	strncpy(field.InstrumentID, pQryInvestorPosition->InstrumentID,sizeof(field.InstrumentID)-1);
	strncpy(field.InvestorID, pQryInvestorPosition->InvestorID,sizeof(field.InvestorID)-1);
    ret = m_pTradeApi->ReqQryInvestorPosition(&field, nRequestID);  
    if(ret==0)
        m_pDataCenter->ClearMapQryRltPositions(std::string(pQryInvestorPosition->InstrumentID));

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
    ret = m_pTradeApi->ReqQryInvestor(&pQryInvestor->Thost, nRequestID);  
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

#ifdef CTP060300

///请求查询期权合约手续费
int CPlatFormService::ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField *pQryOptionInstrCommRate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryOptionInstrCommRate->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryOptionInstrCommRate->BrokerID)-1); ///经纪公司代码    
	strncpy(pQryOptionInstrCommRate->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryOptionInstrCommRate->InvestorID)-1);  ///投资者代码

    ret = m_pTradeApi->ReqQryOptionInstrCommRate(pQryOptionInstrCommRate, nRequestID);  
    LOG_INFO("ReqQryOptionInstrCommRate(请求查询期权合约手续费率):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     ret, nRequestID,
                     pQryOptionInstrCommRate->BrokerID,
                     pQryOptionInstrCommRate->InvestorID,
                     pQryOptionInstrCommRate->InstrumentID);
    return ret;
}

///请求查询期权交易成本
int CPlatFormService::ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField *pQryOptionInstrTradeCost, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryOptionInstrTradeCost->BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryOptionInstrTradeCost->BrokerID)-1); ///经纪公司代码    
	strncpy(pQryOptionInstrTradeCost->InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryOptionInstrTradeCost->InvestorID)-1);  ///投资者代码

    ret = m_pTradeApi->ReqQryOptionInstrTradeCost(pQryOptionInstrTradeCost, nRequestID);  
    LOG_INFO("ReqQryOptionInstrTradeCost(请求查询期权交易成本):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d],\t InputPrice=[%g],\t UnderlyingPrice=[%g]",
                     ret, nRequestID,
                     pQryOptionInstrTradeCost->BrokerID,
                     pQryOptionInstrTradeCost->InvestorID,
                     pQryOptionInstrTradeCost->InstrumentID,
                     pQryOptionInstrTradeCost->HedgeFlag,
                     pQryOptionInstrTradeCost->InputPrice,
                     pQryOptionInstrTradeCost->UnderlyingPrice
                     );
    return ret;
}

#endif

///请求查询交易所
int CPlatFormService::ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryExchange(pQryExchange, nRequestID);  
    return ret;
}

///请求查询品种
int CPlatFormService::ReqQryProduct(PlatformStru_QryProduct *pQryProduct, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryProduct(&pQryProduct->Thost, nRequestID); 
    LOG_INFO("ReqQryProduct(请求查询品种):ret=[%d],nRequestID=[%d],\n"
                     "\t\t\t ProductID=[%s],\t ProductClass=[%d]",
                     ret, nRequestID, 
                     pQryProduct->Thost.ProductID,
                     pQryProduct->Thost.ProductClass);

    return ret;
}

///请求查询合约
int CPlatFormService::ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQryInstrument(&pQryInstrument->Thost, nRequestID); 
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

    ret = m_pTradeApi->ReqQryDepthMarketData(&(pQryDepthMarketData->Thost), nRequestID);  
    return ret;
}

///请求查询投资者结算结果
int CPlatFormService::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo *pQrySettlementInfo, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQrySettlementInfo->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQrySettlementInfo->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pQrySettlementInfo->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQrySettlementInfo->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQrySettlementInfo(&pQrySettlementInfo->Thost, nRequestID); 
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
    ret = m_pTradeApi->ReqQryCFMMCTradingAccountKey(&pQryCFMMCTradingAccountKey->Thost, nRequestID); 
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

    ret = m_pTradeApi->ReqQryTransferBank(&pQryTransferBank->Thost, nRequestID);  
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

    ret = m_pTradeApi->ReqQryInvestorPositionDetail(&QryInvestorPositionDetail, nRequestID);  
    if(ret==0)
        m_pDataCenter->ClearMapQryRltPositionDetails(std::string(pQryInvestorPositionDetail->InstrumentID));

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

    ret = m_pTradeApi->ReqQryNotice(&pQryNotice->Thost, nRequestID);  
    return ret;
}

///请求查询结算信息确认
int CPlatFormService::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm *pQrySettlementInfoConfirm, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    ret = m_pTradeApi->ReqQrySettlementInfoConfirm(&pQrySettlementInfoConfirm->Thost, nRequestID);  
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

    ret = m_pTradeApi->ReqQryInvestorPositionCombineDetail(&QryInvestorPositionCombineDetail, nRequestID); 
    if(ret==0)
        m_pDataCenter->ClearMapQryRltPositionDetailComb(std::string(pQryInvestorPositionCombineDetail->CombInstrumentID));

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
    ret = m_pTradeApi->ReqQryTransferSerial(&pQryTransferSerial->Thost, nRequestID); 
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

    ret = m_pTradeApi->ReqQryContractBank(&pQryContractBank->Thost, nRequestID);  
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

    ret = m_pTradeApi->ReqQryAccountregister(&pQryAccountregister->Thost, nRequestID);  
    LOG_INFO("ReqQryAccountregister(请求查询银期签约关系) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t BankID=[%s]",
                        ret,nRequestID,
                        pQryAccountregister->Thost.BrokerID,
                        pQryAccountregister->Thost.AccountID,
						pQryAccountregister->Thost.BankID);
    return ret;
}
///请求查询汇率
int CPlatFormService::ReqQryExchangeRate(PlatformStru_QryExchangeRate *pQryExchangeRate, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryExchangeRate->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryExchangeRate->Thost.BrokerID)-1); ///经纪公司代码    

    ret = m_pTradeApi->ReqQryExchangeRate((CThostFtdcQryExchangeRateField *)&pQryExchangeRate->Thost, nRequestID);  
    LOG_INFO("ReqQryExchangeRate(请求查询汇率) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t FromCurrencyID=[%s],\t ToCurrencyID=[%s]",
                        ret,nRequestID,
                        pQryExchangeRate->Thost.BrokerID,
                        pQryExchangeRate->Thost.FromCurrencyID,
						pQryExchangeRate->Thost.ToCurrencyID);
    return ret;
}


///请求查询预埋单
int CPlatFormService::ReqQryParkedOrder(PlatformStru_QryParkedOrder *pQryParkedOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

    strncpy(pQryParkedOrder->Thost.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(pQryParkedOrder->Thost.BrokerID)-1); ///经纪公司代码       
    strncpy(pQryParkedOrder->Thost.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(pQryParkedOrder->Thost.InvestorID)-1);  ///投资者代码
    ret = m_pTradeApi->ReqQryParkedOrder(&pQryParkedOrder->Thost, nRequestID);  
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
    ret = m_pTradeApi->ReqQryParkedOrderAction(&pQryParkedOrderAction->Thost, nRequestID);  
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
    ret = m_pTradeApi->ReqFromBankToFutureByFuture(&pReqTransfer->Thost, nRequestID);  
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
    ret = m_pTradeApi->ReqFromFutureToBankByFuture(&pReqTransfer->Thost, nRequestID);  
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
    ret = m_pTradeApi->ReqQueryBankAccountMoneyByFuture(&pReqQueryAccount->Thost, nRequestID); 
    LOG_INFO("ReqQueryBankAccountMoneyByFuture(期货发起查询银行余额请求) : ret=[%d],nRequestID=[%d],\n"
                        "\t\t\t TradeCode=[%s],\t BankID=[%s],\t BankBranchID=[%s],\t BrokerID=[%s],\t BrokerBranchID=[%s],\n"
                        "\t\t\t TradeDate=[%s],\t TradeTime=[%s],\t BankSerial=[%s],\t TradingDay=[%s],\t PlateSerial=[%d],\n"
                        "\t\t\t LastFragment=[%d],\t SessionID=[%#x],\t CustomerName=[%s],\t IdCardType=[%d],\t IdentifiedCardNo=[%s],\n"
                        "\t\t\t CustType=[%d],\t BankAccount=[%s],\t BankPassWord=[******],\t AccountID=[%s],\t Password=[******],\n"
                        "\t\t\t FutureSerial=[%d],\t InstallID=[%d],\t UserID=[%s],\t VerifyCertNoFlag=[%d],CurrencyID=[%s],\n"
                        "\t\t\t Digest=[%s],\t BankAccType=[%d],\t DeviceID=[%s],\t BankSecuAccType=[%d],BrokerIDByBank=[%s],\n"
                        "\t\t\t BankSecuAcc=[%s],\t BankPwdFlag=[%d],\t SecuPwdFlag=[%d],\t OperNo=[%s],RequestID=[%d],\n"
                        "\t\t\t TID=[%d]",
                        ret,nRequestID,
                        pReqQueryAccount->Thost.TradeCode, pReqQueryAccount->Thost.BankID,pReqQueryAccount->Thost.BankBranchID,pReqQueryAccount->Thost.BrokerID,pReqQueryAccount->Thost.BrokerBranchID,
                        pReqQueryAccount->Thost.TradeDate, pReqQueryAccount->Thost.TradeTime,pReqQueryAccount->Thost.BankSerial,pReqQueryAccount->Thost.TradingDay,pReqQueryAccount->Thost.PlateSerial,
                        pReqQueryAccount->Thost.LastFragment, pReqQueryAccount->Thost.SessionID,pReqQueryAccount->Thost.CustomerName,pReqQueryAccount->Thost.IdCardType,pReqQueryAccount->Thost.IdentifiedCardNo,
                        pReqQueryAccount->Thost.CustType, pReqQueryAccount->Thost.BankAccount,pReqQueryAccount->Thost.AccountID,
                        pReqQueryAccount->Thost.FutureSerial, pReqQueryAccount->Thost.InstallID,pReqQueryAccount->Thost.UserID,pReqQueryAccount->Thost.VerifyCertNoFlag,pReqQueryAccount->Thost.CurrencyID,
                        pReqQueryAccount->Thost.Digest, pReqQueryAccount->Thost.BankAccType,pReqQueryAccount->Thost.DeviceID,pReqQueryAccount->Thost.BankSecuAccType,pReqQueryAccount->Thost.BrokerIDByBank,
                        pReqQueryAccount->Thost.BankSecuAcc, pReqQueryAccount->Thost.BankPwdFlag,pReqQueryAccount->Thost.SecuPwdFlag,pReqQueryAccount->Thost.OperNo,pReqQueryAccount->Thost.RequestID,
                        pReqQueryAccount->Thost.TID
                        );
    return ret;
}

int CPlatFormService::SubscribeMarketData(const std::string& InstrumentID)
{
    int ret = -999;
    if(InstrumentID.empty() ||
       !m_pQuotApi          ||
       m_PlatformParam.QuotStatus!=CTPCONNSTATUS_Connected)
    {
        LOG_INFO("行情API还未准备就绪, 可能登入未成功或正在重连");
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
       m_PlatformParam.QuotStatus!=CTPCONNSTATUS_Connected)
    {
        LOG_INFO("行情API还未准备就绪, 可能登入未成功或正在重连");
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
       m_PlatformParam.BrokerID.empty() ||
       m_PlatformParam.UserID.empty()/* ||
       m_PlatformParam.UserPW.empty()*/)
    {
        LOG_INFO("LoginTrade:参数未配置完整");
        return -1;
    }

    if(CTPCONNSTATUS_Connected==m_PlatformParam.TradeStatus)
    {
        LOG_INFO("LoginTrade:已经登入");
        return 0;//已经登入
    }
    if(CTPCONNSTATUS_Logining==m_PlatformParam.TradeStatus ||
       CTPCONNSTATUS_Connecting==m_PlatformParam.TradeStatus 
       ||CTPCONNSTATUS_TradeSettlementInfoConfirming==m_PlatformParam.TradeStatus)
    {
        LOG_INFO("LoginTrade:正在登入中");
        return -2;//正在登入中
    }

    LogoutTrade();

    m_PlatformParam.TradeStatus = CTPCONNSTATUS_Connecting;//正在连接
    m_PlatformParam.InvestorID = InvestorID;
    m_PlatformParam.InvestorPW = InvestorPW;
	//std::map<std::string, int>::iterator it = g_allPlatformID.find(m_PlatformParam.UserID);
	//if ( it != g_allPlatformID.end() )
	//{
	//	m_PlatformParam.PlatFormID = it->second;
	//}
	//else
	//{
	//	m_PlatformParam.PlatFormID = 0;
	//}

    m_pDataCenter->m_LoginInvestorID=InvestorID;
    m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
    m_pTradeSpi = new CTraderSpi(*this);
    m_pTradeApi->RegisterSpi(static_cast<CThostFtdcTraderSpi*>(m_pTradeSpi));
    
    m_pTradeWorkThread = new CTradeThread(*this);
    
    if(m_pTradeWorkThread->Create(512*1024) != wxTHREAD_NO_ERROR)
        LOG_INFO("LoginTrade:创建交易工作线程失败!");
    else
        LOG_INFO("LoginTrade:创建交易工作线程成功!");

    if(m_pTradeWorkThread->Run() != wxTHREAD_NO_ERROR)
    {
        LOG_INFO("LoginTrade:运行交易工作线程失败!");
        return -3;
    }
    else
    {
        LOG_INFO("LoginTrade:运行交易工作线程成功!");
    }
   
    return 0;
}

int CPlatFormService::LoginQuot(const std::string& InvestorID, const std::string& InvestorPW)
{
    LOG_INFO("LoginQuot:InvestorID=[%s],InvestorPW=[******]", InvestorID.c_str());
    
    if(InvestorID.empty() || 
       /*InvestorPW.empty() ||*/
       m_PlatformParam.BrokerID.empty() ||
       m_PlatformParam.UserID.empty()/* ||
       m_PlatformParam.UserPW.empty()*/)
    {
        LOG_INFO("LoginQuot:参数未配置完整");
        return -1;
    }


    if(CTPCONNSTATUS_Connected==m_PlatformParam.QuotStatus)
    {
        LOG_INFO("LoginQuot:已经登入");
        return 0;//已经登入
    }
    
    if(CTPCONNSTATUS_Logining==m_PlatformParam.QuotStatus ||
       CTPCONNSTATUS_Connecting==m_PlatformParam.QuotStatus )
    {
        LOG_INFO("LoginQuot:正在登入中");
        return -2;//正在登入中
    }

    LogoutQuot();

    m_PlatformParam.QuotStatus = CTPCONNSTATUS_Connecting;//正在连接
    //m_PlatformParam.InvestorID = InvestorID;
    //m_PlatformParam.InvestorPW = InvestorPW;

    m_pQuotApi = CThostFtdcMdApi::CreateFtdcMdApi();
    m_pQuotSpi = new CQuotSpi(*this);
    m_pQuotApi->RegisterSpi(static_cast<CThostFtdcMdSpi*>(m_pQuotSpi));
    
    m_pQuotWorkThread = new CQuotThread(*this);
    
    if(m_pQuotWorkThread->Create(512*1024) != wxTHREAD_NO_ERROR)
        LOG_INFO("LoginQuot:创建行情工作线程失败!");
    else
        LOG_INFO("LoginQuot:创建行情工作线程成功!");

    if(m_pQuotWorkThread->Run() != wxTHREAD_NO_ERROR)
    {
         LOG_INFO("LoginQuot:运行行情工作线程失败!");
         return -3;
    }
    else
    {
         LOG_INFO("LoginQuot:运行行情工作线程成功!");
    }
   
    return 0;
}

void CPlatFormService::StopThreads(void)
{
    if(m_pDataCenter)
        m_pDataCenter->Stop();
    if(m_pTradeApi)
    {
        LOG_INFO("开始关闭交易API");
        m_PlatformParam.TradeStatus = CTPCONNSTATUS_Disconnecting;
        m_pTradeApi->RegisterSpi(NULL);
        m_pTradeApi->Release();
        m_PlatformParam.TradeStatus = CTPCONNSTATUS_Disconnected;
        m_pTradeApi=NULL;
        LOG_INFO("完成关闭交易API");
    }
	if(m_pTradeWorkThread)
	{
        LOG_INFO("开始关闭交易工作线程");
		m_pTradeWorkThread->Delete();
        m_pTradeWorkThread=NULL;
        LOG_INFO("完成关闭交易工作线程");
	}
    if(m_pQuotApi)
    {
        LOG_INFO("开始关闭行情API");
        m_PlatformParam.QuotStatus = CTPCONNSTATUS_Disconnecting;
        m_pQuotApi->RegisterSpi(NULL);
        m_pQuotApi->Release();
        m_PlatformParam.QuotStatus = CTPCONNSTATUS_Disconnected;
        m_pQuotApi=NULL;
        LOG_INFO("完成关闭行情API");
    }

    if(m_pQuotWorkThread)
    {
        LOG_INFO("开始关闭行情工作线程");
        m_pQuotWorkThread->Delete();
        m_pQuotWorkThread=NULL; 
        LOG_INFO("完成关闭行情工作线程");
    }
}

int CPlatFormService::LogoutTrade()
{
    //LOG_INFO("LogoutTrade");
    if(m_pTradeApi)
    {
        LOG_INFO("LogoutTrade:关闭交易API");
        m_PlatformParam.TradeStatus = CTPCONNSTATUS_Disconnecting;
        m_pTradeApi->Release();
        m_PlatformParam.TradeStatus = CTPCONNSTATUS_Disconnected;
        m_pTradeApi=NULL;
    }

    if(m_pTradeWorkThread)
    {
        m_pTradeWorkThread->Delete();
        //m_pTradeWorkThread=NULL;   //will be set NULL in calling CTradeThread::OnExit()
        LOG_INFO("LogoutTrade:关闭交易工作线程");
    }

    if(m_pTradeSpi)
    {
        delete m_pTradeSpi;
        m_pTradeSpi=NULL;
    }
    
    return 0;
}

int CPlatFormService::LogoutQuot()
{
    LOG_INFO("LogoutQuot");
    if(m_pQuotApi)
    {
        LOG_INFO("LogoutQuot:关闭行情API");
        m_PlatformParam.QuotStatus = CTPCONNSTATUS_Disconnecting;
        m_pQuotApi->Release();
        m_PlatformParam.QuotStatus = CTPCONNSTATUS_Disconnected;
        m_pQuotApi=NULL;
    }

    if(m_pQuotWorkThread)
    {
        m_pQuotWorkThread->Delete();
        //m_pQuotWorkThread=NULL;   //will be set NULL in calling CQuotThread::OnExit()
        LOG_INFO("LogoutQuot:关闭行情工作线程");
    }

    if(m_pQuotSpi)
    {
        delete m_pQuotSpi;
        m_pQuotSpi=NULL;
    }
    
    return 0;
}

///密码是否是当前密码
bool CPlatFormService::IsValidInvestorPW(const string& yourPW)
{
    return m_PlatformParam.InvestorPW==yourPW ? true : false;
}
bool CPlatFormService::IsValidUserPW(const string& yourPW)
{
    return m_PlatformParam.UserPW==yourPW ? true : false;
}
int CPlatFormService::GetBrokerIDUserID(std::string& UserID, std::string& BrokerID,set<string>* accounts)
{
	BrokerID=m_PlatformParam.BrokerID;
	UserID=m_PlatformParam.UserID;
	return 0;

}
int CPlatFormService::SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)
{
    m_PlatformParam.TradeFrontAddrs=TradeAddr;
    return 0;
}


int CPlatFormService::SetProxyParam(const Stru_ProxyServerParam& ProxyParam)
{
	m_PlatformParam.ProxyParam = ProxyParam;
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

CPlatFormService::CPlatFormService(CServiceProxy& rServiceProxy)
:   m_pWriteLog(NULL)
    ,m_pTradeApi(NULL)
    ,m_pTradeSpi(NULL)
    ,m_pQuotApi(NULL)
    ,m_pQuotSpi(NULL)
    ,m_pTradeWorkThread(NULL)
    ,m_pQuotWorkThread(NULL)
    ,m_PCache(NULL)
    ,m_pDataCenter(NULL)
    ,m_CurOrderRef(0)
    ,m_CurForQuoteRef(0)
    ,m_CurExecOrderRef(0)
    ,m_CurExecOrderActionRef(0)
    ,m_status(0)
    ,m_nOrderReqID(0)
    ,m_bSettlementInfoConfirmed(false)
    ,m_bInQry(false)
    ,m_rServiceProxy(rServiceProxy)
{
    InitializeCriticalSection(&m_CS);

    m_pWriteLog = new zqWriteLog();
	m_PCache   = new CPackageCache((!IS_MULTIACCOUNT_VERSION?(40*1024*1024):(20*1024*1024)), 1);
    m_pDataCenter = new CDataCenter(*m_PCache,this,rServiceProxy.m_PlatformID,rServiceProxy.m_MaxPlatformID);
    m_pDataCenter->Start();
    m_pTradeApi=NULL;
    m_pTradeSpi=NULL;
    m_pQuotApi=NULL;
    m_pQuotSpi=NULL;
    m_pTradeWorkThread=NULL;
    m_pQuotWorkThread=NULL;
	m_bSettlementInfoConfirmed=false;
	memset(&m_ErrorInfoField,0,sizeof(m_ErrorInfoField));

	m_nOrderReqID = 0;
}

CPlatFormService::~CPlatFormService()
{ 
    LogoutTrade();
    LogoutQuot();

    if(m_pDataCenter)
    {
        m_pDataCenter->Stop();
        delete m_pDataCenter;
        m_pDataCenter = NULL;
    }
    
    if(m_PCache)
    {
        delete m_PCache;
        m_PCache=NULL;
    }

    if(m_pWriteLog)
    {
        LOG_INFO("CPlatFormService::~CPlatFormService(析构CPlatFormService对象)");
        delete m_pWriteLog;
        m_pWriteLog=NULL;
    }

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
int CPlatFormService::GetInstrumentList(std::vector<InstrumentGroupInfo> &outData)
{
    int irlt=m_pDataCenter->GetInstrumentList(outData);
    return irlt==0?-1:irlt;
}
///获取合约列表
int CPlatFormService::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    int irlt=m_pDataCenter->GetInstrumentList(outData,ExchangeID);
    return irlt==0?-1:irlt;
}
int CPlatFormService::GetInstrumentList(vector<string> &outData,string ExchangeID)
{
    int irlt=m_pDataCenter->GetInstrumentList(outData,ExchangeID);
    return irlt==0?-1:irlt;
}
//添加主力合约列表
void CPlatFormService::AddMainInstrumentList(std::string instrument)
{
	m_pDataCenter->AddMainInstrumentList(instrument);
}
///获取主力合约列表
int CPlatFormService::GetMainInstrumentList(std::set<std::string> &outData)
{
    return m_pDataCenter->GetMainInstrumentList(outData);
}
///设置合约容差列表
void CPlatFormService::SetInstrumentVarietyMap(map<string, string>& inData)
{
    m_pDataCenter->SetInstrumentVarietyMap(inData);
}

// 获取合约容差列表
int CPlatFormService::GetInstrumentVarietyMap(map<string, string>& outData)
{
    return m_pDataCenter->GetInstrumentVarietyMap(outData);
}
///获取指定品种的合约列表,返回合约个数
int CPlatFormService::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
	return m_pDataCenter->GetInstrumentListByProductID(ProductID,outData);
}
int CPlatFormService::GetInstrumentListByProductID(const string& ProductID,vector<string> &outData)
{
	return m_pDataCenter->GetInstrumentListByProductID(ProductID,outData);
}
///获取指定合约信息
int CPlatFormService::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    return m_pDataCenter->GetInstrumentInfo(InstrumentID, outData);
}
bool CPlatFormService::GetLegInstrumentID(const string& strComInstrumentID,
                                          string& strLeg1InstrumentID,
                                          string& strLeg2InstrumentID)
{
    return m_pDataCenter->GetLegInstrumentID(strComInstrumentID,strLeg1InstrumentID,strLeg2InstrumentID);
}

///获取包含此单腿合约的组合合约列表
bool CPlatFormService::GetCombInstrumentIDs_IncludeLeg(const std::string& LegInstrument,std::vector<std::string>& vecCombInstruments)
{
    return m_pDataCenter->GetCombInstrumentIDs_IncludeLeg(LegInstrument,vecCombInstruments);
}

//设置合约订阅状态
void CPlatFormService::SetSubscribeStatus(const std::string& InstrumentID,int GID,SubscribeMarketDataOper oper)
{
    //int oldstatus,newstatus;

    EnterCriticalSection(&m_CS);

    bool bOriSub=m_SubscribeMap.find(InstrumentID)!=m_SubscribeMap.end();
    //if(m_SubscribeMap.find(InstrumentID)!=m_SubscribeMap.end()) 
    //    oldstatus=m_SubscribeMap[InstrumentID];
    //else oldstatus=0;

	switch(oper)
	{
	    case eSubscribeMarketData:
            if(!CTools_Ansi::IsKeyValueInMultimap<string, int>(m_SubscribeMap,InstrumentID,GID))
                m_SubscribeMap.insert(pair<string, int>(InstrumentID,GID));
		    //m_SubscribeMap[InstrumentID]|=status;
		    break;
	    case eUnsubscribeMarketData:
            if(CTools_Ansi::IsKeyValueInMultimap<string, int>(m_SubscribeMap,InstrumentID,GID))
                CTools_Ansi::EraseKeyValueInMultimap<string, int>(m_SubscribeMap,InstrumentID,GID);
		    //m_SubscribeMap[InstrumentID]&=~status;
		    break;
	}

    bool bNewSub=m_SubscribeMap.find(InstrumentID)!=m_SubscribeMap.end();
    //if(m_SubscribeMap.find(InstrumentID)!=m_SubscribeMap.end()) 
    //    newstatus=m_SubscribeMap[InstrumentID];
    //else newstatus=0;

    LeaveCriticalSection(&m_CS);


    if(!bOriSub&&bNewSub)
        SubscribeMarketData(InstrumentID);
    else if(bOriSub&&!bNewSub)
        UnSubscribeMarketData(InstrumentID);
    //if(oldstatus==0&&newstatus!=0)
    //    SubscribeMarketData(InstrumentID);
    //else if(oldstatus!=0&&newstatus==0)
    //    UnSubscribeMarketData(InstrumentID);
}
//退订GID对应的全部合约行情
void CPlatFormService::UnsubscribeMarketDataOfGID(int GID)
{
    EnterCriticalSection(&m_CS);
	vector<string> InstrumentIDs;
	CTools_Ansi::GetKeysInMultimap_MatchValue<string,int>(m_SubscribeMap,GID,InstrumentIDs);
    LeaveCriticalSection(&m_CS);

	for(int i=0;i<(int)InstrumentIDs.size();i++)
		SetSubscribeStatus(InstrumentIDs[i],GID,eUnsubscribeMarketData);
}
//检查GID是否订阅了指定合约的行情
bool CPlatFormService::IsSubscribeMarketDataOfGID(const string& InstrumentID,int GID)
{
    EnterCriticalSection(&m_CS);
	bool brlt=CTools_Ansi::IsKeyValueInMultimap<string,int>(m_SubscribeMap,InstrumentID,GID);
    LeaveCriticalSection(&m_CS);
	return brlt;
}

///设置指定合约信息
void CPlatFormService::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData,bool bLast)
{
    CTools_Ansi::EraseKeyInMultimap<string, int>(m_SubscribeMap,InstrumentID);
    m_pDataCenter->SetInstrumentInfo(InstrumentID, outData, bLast);
}
///获取合约的产品类型, 失败返回-1
char CPlatFormService::GetProductClassType(const std::string& InstrumentID)
{
    return m_pDataCenter->GetProductClassType(InstrumentID);
}
///获取合约的ProductID
string CPlatFormService::GetProductID(const string& strInstrumentID)
{
    return m_pDataCenter->GetProductID(strInstrumentID);
}
string CPlatFormService::GetExchangeID(const string& strInstrumentID)
{
    return m_pDataCenter->GetExchangeID(strInstrumentID);
}
///设置指定品种信息
void CPlatFormService::SetProductInfo(const string& ProductID, PlatformStru_ProductInfo& outData)
{
    m_pDataCenter->SetProductInfo(ProductID, outData);
}
///获取品种列表
int CPlatFormService::GetProductList(vector<string> &outData,const string& ExchangeID)
{
    return m_pDataCenter->GetProductList(outData,ExchangeID);
}

///获取指定交易所的所有品种-合约
int CPlatFormService::GetProductID_InstrumentIDsByExchangeID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    return m_pDataCenter->GetProductID_InstrumentIDsByExchangeID(outData,ExchangeID);
}
///获取全部交易所的品种合约ID vector<pair<ExchangeID,vector<pair<ProductID,vector<InstrumentID>>>>>
void CPlatFormService::GetExchangeID_ProductID_InstrumentIDs(vector<pair<string,vector<pair<string,vector<string>>>>>& outData)
{
    m_pDataCenter->GetExchangeID_ProductID_InstrumentIDs(outData);
}
///获取全部交易所的品种信息 vector<pair<ExchangeID,vector<PlatformStru_ProductInfo>>>
void CPlatFormService::GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData)
{
    m_pDataCenter->GetExchangeID_ProductInfos(outData);
}

///根据合约ID获取品种信息
bool CPlatFormService::GetProductInfo(const string& strInstrumentID,PlatformStru_ProductInfo& outData)
{
    return m_pDataCenter->GetProductInfo(strInstrumentID,outData);
}
///根据品种ID获取品种信息
bool CPlatFormService::GetProductInfo2(const string& strProductID,PlatformStru_ProductInfo& outData)
{
    return m_pDataCenter->GetProductInfo2(strProductID,outData);
}
//获取指定交易所的期权标的物期货品种
int CPlatFormService::GetOptionProductIDsByExchangeID(const string& ExchangeID,vector<string>& outProductIDs)
{
    return m_pDataCenter->GetOptionProductIDsByExchangeID(ExchangeID,outProductIDs);
}
//获取指定标的物期货合约对应的期权合约ID，返回目标期权合约的数量。UnderlyingInstrumentID为空则返回全部期权合约
int CPlatFormService::GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outOptionInstrumentIDs)
{
    return m_pDataCenter->GetOptionInstrumentIDsByUnderlyingInstrumentID(UnderlyingInstrumentID,outOptionInstrumentIDs);
}
//获取指定标的物期货合约对应的看涨期权合约ID数组和看跌期权合约ID数组。UnderlyingInstrumentID为空则返回全部期权合约
void CPlatFormService::GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outCallOptionInstrumentIDs,vector<string>& outPutOptionInstrumentIDs)
{
    return m_pDataCenter->GetOptionInstrumentIDsByUnderlyingInstrumentID(UnderlyingInstrumentID,outCallOptionInstrumentIDs,outPutOptionInstrumentIDs);
}
//获取所有标的物期货合约
int CPlatFormService::GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs)
{
    return m_pDataCenter->GetAllOptionUnderlyingInstrumentIDs(outUnderlyingInstrumentIDs);
}
int CPlatFormService::GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs)
{
    return m_pDataCenter->GetAllOptionUnderlyingInstrumentIDs(outUnderlyingInstrumentIDs);
}
//判断是否是期权标的物期货合约
bool CPlatFormService::IsUnderlyingInstrumentID(const string& InstrumentID)
{
    return m_pDataCenter->IsUnderlyingInstrumentID(InstrumentID);
}

///获取合约手续费率
int CPlatFormService::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_pDataCenter->GetCommissionRate(InstrumentID, outData);
}
bool CPlatFormService::IsCommissionRateExist(const string& InstrumentID)
{
    return m_pDataCenter->IsCommissionRateExist(InstrumentID);
}

///获取合约保证金率
int CPlatFormService::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_pDataCenter->GetMarginRate(InstrumentID, outData);
}
bool CPlatFormService::IsMarginRateExist(const string& InstrumentID)
{
    return m_pDataCenter->IsMarginRateExist(InstrumentID);
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

///获取合约乘数, 成功返回合约乘数, 失败返回-1
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


///获取指定合约的现价, 失败返回无效值
double CPlatFormService::GetCurPrice(const std::string& InstrumentID)
{
    return m_pDataCenter->GetCurPrice(InstrumentID);
}
string CPlatFormService::GetTradingDay()
{
	return m_pDataCenter->GetTradingDay();
}

bool CPlatFormService::IsTodayPosition( const char* pOpenData )
{
	return m_pDataCenter->IsTodayPosition(pOpenData);
}

///获取交易所时间
unsigned long CPlatFormService::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataCenter->GetExchangeTime(ExchangeID,time);
}
///获取交易所日内交易时段, 返回交易时段起始时间对的数组，如{pair("0915","1130"),pair("1300","1515")}等
vector<pair<string,string>> CPlatFormService::GetTradingTimespan(const string& ExchangeID)
{
    vector<pair<string,string>> timespan;

    if(ExchangeID==string("CFFEX"))
    {
        timespan.push_back(make_pair("0915","1130"));
        timespan.push_back(make_pair("1300","1515"));
    }
    else if(ExchangeID==string("SHFE")||ExchangeID==string("DCE")||ExchangeID==string("CZCE"))
    {
        timespan.push_back(make_pair("0900","1015"));
        timespan.push_back(make_pair("1030","1130"));
        timespan.push_back(make_pair("1300","1500"));
    }

    return timespan;
}

///获取全部交易所
int CPlatFormService::GetExchangeIDs(vector<string>& outData)
{
    return m_pDataCenter->GetExchangeIDs(outData);
}

///设置当前显示的持仓内容. 1:持仓; 2:持仓明细; 3:组合持仓
void CPlatFormService::SetCurrentPositionContent(int PositionContentMode)
{
    return m_pDataCenter->SetCurrentPositionContent(PositionContentMode);
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
bool CPlatFormService::GetOrder2(const string& strOrderSysID,PlatformStru_OrderInfo& outData)
{
    return m_pDataCenter->GetOrder2(strOrderSysID,outData);
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
int CPlatFormService::GetWaitOrders3(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataCenter->GetWaitOrders3(strInstrument,outData);
}
///获取指定合约相关的未成交报单, 不包含手工审核中的报单, 合约是单合约, 报单是对应报单, 或包含该合约的组合报单
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
int CPlatFormService::GetTradingAccountID(char* AccountID,int rltsize)
{
    return m_pDataCenter->GetTradingAccountID(AccountID,rltsize);
}
///获取交易资金账户信息
int CPlatFormService::GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)
{
    return m_pDataCenter->GetTradingAccount(outData);
}
///获取账户资金文本信息
int CPlatFormService::GetAccountText(std::string& outData,int language)
{
    return m_pDataCenter->GetAccountText( outData, language);
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
int CPlatFormService::GetTradeBriefsOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfoBrief>& outData)
{
    return m_pDataCenter->GetTradeBriefsOfInstrument(strInstrument,outData);
}

///获取全部成交统计记录
int CPlatFormService::GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_pDataCenter->GetAllTradeTotals(outData);
}

///获取指定合约的成交统计记录成功返回0, 失败返回-1
int CPlatFormService::GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_pDataCenter->GetTradeTotalOfInstrument(strInstrument,outData);
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
    return m_pDataCenter->GetPositionDetails3(strInstrument,setFTID,vecValue,lastseq);
}

int CPlatFormService::GetPositionCombs2(const std::string& strInstrument, std::vector<PlatformStru_Position>& outData)
{
    return m_pDataCenter->GetPositionCombs2(strInstrument,outData);
}


int CPlatFormService::GetPositions(std::vector<PlatformStru_Position>& outData)
{
    return m_pDataCenter->GetPositions(outData);
}

int CPlatFormService::GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    return m_pDataCenter->GetPositionDetails(outData,lastseq);
}


int CPlatFormService::GetPositionCombs(std::vector<PlatformStru_Position>& outData)
{
    return m_pDataCenter->GetPositionCombs(outData);
}


bool  CPlatFormService::HavePositionDetail(const std::string& strInstrumentID)
{
	return m_pDataCenter->HavePositionDetail(strInstrumentID);
}

bool  CPlatFormService::HaveCombPositionDetail()
{
	return m_pDataCenter->HaveCombPositionDetail();
}
void CPlatFormService::GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs)
{
    m_pDataCenter->GetDerivedInstrumentID_OnCloseTrade(InstrumentIDs);
}


//获取持仓记录键值列表,返回持仓记录的数量
int CPlatFormService::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_pDataCenter->GetPositionKeySet(PositionKeySet);
}

//获取指定合约, 方向, 投保的持仓数据, 成功返回0, 失败返回-1
int CPlatFormService::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    return m_pDataCenter->GetPositionData(strAccount,InstrumentID,Direction,HedgeFlag,PositionData);
}


int  CPlatFormService::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataCenter->GetWaitOrderVolume(strAccount,strInstrumentID, Direction, CloseType,HedgeFlag);
}

//获取指定合约指定方向的平仓挂单的平仓量, 返回0表示成功, <0表示失败. 成功时CloseVolume返回平仓单(含强平和本地强平)的手数, CloseTodayVolume返回平今单手数, CloseYesterdayVolume表示平昨单手数
int CPlatFormService::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataCenter->GetCloseOrderVolume(strAccount,strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);
}




CTPCONNSTATUS CPlatFormService::GetTradeStatus(void)
{
    return m_PlatformParam.TradeStatus;
};

//获取行情连接状态. 
CTPCONNSTATUS CPlatFormService::GetQuotStatus(void)
{
    return m_PlatformParam.QuotStatus;
};

//获取交易BrokerID. 
std::string CPlatFormService::GetTradeBrokerID(void)
{
    return m_PlatformParam.BrokerID;
};

//获取交易InvestorID. 
std::string CPlatFormService::GetTradeInvestorID(void)
{
    return m_PlatformParam.InvestorID;
};

//int CPlatFormService::GetPlatFormID()
//{
//	return m_PlatformParam.PlatFormID;
//}

//判断是否需要查询新合约的费率
bool CPlatFormService::IsNeedCheckCommissionRateAndMarginRate(void)
{
     return m_pDataCenter->IsNeedCheckCommissionRateAndMarginRate();
}


//清除需要查询新合约费率的标志
void CPlatFormService::SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck)
{
    m_pDataCenter->SetNeedCheckCommissionRateAndMarginRate(bNeedCheck);
}
bool CPlatFormService::IsSettlementInfoConfirmed()
{
	return m_bSettlementInfoConfirmed;
}
SERVER_PARAM_CFG&  CPlatFormService::GetServerParamCfg()
{
	return m_pDataCenter->GetServerParamCfg();
}

//获取客户签约银行信息
int CPlatFormService::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{
    return m_pDataCenter->GetContractBank(outData);
}

//UI层通知底层, 初始化查询开始或完成
void CPlatFormService::NotifyInitQryStart()
{
	m_pDataCenter->NotifyInitQryStart();
}
//UI层通知底层, 初始化查询开始或完成
void CPlatFormService::NotifyInitQrySucc()
{
	m_pDataCenter->NotifyInitQrySucc();
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

int CPlatFormService::GetPositionCombInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_pDataCenter->GetPositionCombInfo(lFTID, outData);
}


//获取交易连接的FrontID和SessionID. 这两个值在交易登录时由ctp返回
const PlatformStru_LoginInfo& CPlatFormService::GetTradingLoginInfo()
{
    return m_pDataCenter->GetTradingLoginInfo();
}

///DataCenter是否已经和底层同步上，可以开始初始化查询了
bool CPlatFormService::IsDataCenterReady(void)
{
    return m_pDataCenter ? m_pDataCenter->bRspQrySettlementInfoConfirm : false;
}

vector<string> CPlatFormService::GetCurrencys(const string& BankName,bool bAddUSD,bool bAddCNY)
{
    return m_pDataCenter->GetCurrencys(BankName,bAddUSD,bAddCNY);
}
vector<string> CPlatFormService::GetCurrencys(bool bAddUSD,bool bAddCNY)
{
    return m_pDataCenter->GetCurrencys(bAddUSD,bAddCNY);
}


///订阅询价通知，OptionInstrumentIDs为空则订阅全部期权合约的询价通知
int  CPlatFormService::SubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs)
{
    int ret = -999;
    CHECK_TRADE_STATUS();

#ifdef CTP060300
    vector<string> InstrumentIDs=OptionInstrumentIDs;
    if(InstrumentIDs.empty())
        m_pDataCenter->GetOptionInstrumentIDsByUnderlyingInstrumentID(string(""),InstrumentIDs);

    for(vector<string>::iterator it=InstrumentIDs.begin();it!=InstrumentIDs.end();)
    {
        if(m_setOptionInstrumentIDs_SubscribeForQuoteRsp.find(*it)!=m_setOptionInstrumentIDs_SubscribeForQuoteRsp.end())
            it=InstrumentIDs.erase(it);
        else it++;
    }
    int cnt=InstrumentIDs.size();
    AllocTmpStackBuf(tmpInstrumentIDs,sizeof(char*)*cnt);
    for(int i=0;i<cnt;i++)
        ((char**)(tmpInstrumentIDs.m_pbuf))[i]=const_cast<char*>(InstrumentIDs[i].c_str());
    ret = m_pQuotApi->SubscribeForQuoteRsp((char**)(tmpInstrumentIDs.m_pbuf), cnt);  
    if(ret==0)
    {
        for(int i=0;i<cnt;i++)
            m_setOptionInstrumentIDs_SubscribeForQuoteRsp.insert(InstrumentIDs[i]);
    }
    LOG_INFO("SubscribeForQuoteRsp(订阅询价通知):ret=[%d],cnt=[%d]",
            ret, cnt);
#endif 

    return ret;
}
///退订询价通知，OptionInstrumentIDs为空则退订全部合约的询价通知
int CPlatFormService::UnSubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs)
{
    int ret = -999;
    CHECK_TRADE_STATUS();
    
#ifdef CTP060300
    vector<string> InstrumentIDs=OptionInstrumentIDs;
    if(InstrumentIDs.empty())
        CTools_Ansi::ConvertSet2Vector(m_setOptionInstrumentIDs_SubscribeForQuoteRsp,InstrumentIDs);
    else
    {
        for(vector<string>::iterator it=InstrumentIDs.begin();it!=InstrumentIDs.end();)
        {
            if(m_setOptionInstrumentIDs_SubscribeForQuoteRsp.find(*it)==m_setOptionInstrumentIDs_SubscribeForQuoteRsp.end())
                it=InstrumentIDs.erase(it);
            else it++;
        }
    }
    int cnt=InstrumentIDs.size();
    AllocTmpStackBuf(tmpInstrumentIDs,sizeof(char*)*cnt);
    for(int i=0;i<cnt;i++)
        ((char**)(tmpInstrumentIDs.m_pbuf))[i]=const_cast<char*>(InstrumentIDs[i].c_str());
    ret = m_pQuotApi->UnSubscribeForQuoteRsp((char**)(tmpInstrumentIDs.m_pbuf), cnt);  
    if(ret==0)
    {
        for(int i=0;i<cnt;i++)
		{
			if(m_setOptionInstrumentIDs_SubscribeForQuoteRsp.find(InstrumentIDs[i])!=m_setOptionInstrumentIDs_SubscribeForQuoteRsp.end())
				m_setOptionInstrumentIDs_SubscribeForQuoteRsp.erase(InstrumentIDs[i]);
		}
    }
    LOG_INFO("UnSubscribeForQuoteRsp(退订询价通知):ret=[%d],cnt=[%d]",
            ret, cnt);
#endif 

    return ret;
}

///请求询价录入
///调用者不用管InputForQuote中的BrokerID, InvestorID, UserID, ForQuoteRef这些值。底层自动确定，并返回
int CPlatFormService::ReqForQuoteInsert(PlatformStru_InputForQuoteField& InputForQuote, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();
#ifdef CTP060300

    strncpy(InputForQuote.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(InputForQuote.BrokerID)-1); ///经纪公司代码       
    strncpy(InputForQuote.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(InputForQuote.InvestorID)-1);  ///投资者代码
    strncpy(InputForQuote.UserID, m_PlatformParam.UserID.c_str(), sizeof(InputForQuote.UserID)-1);  ///用户代码
	sprintf(InputForQuote.ForQuoteRef,"%12d",++m_CurForQuoteRef);

    CThostFtdcInputForQuoteField thost;
    InputForQuote.ToThost(thost);

    ret = m_pTradeApi->ReqForQuoteInsert(&thost, nRequestID);  
    LOG_INFO("ReqForQuoteInsert(请求询价录入):ret=[%d],nRequestID=[%d]\n%s",
             ret, nRequestID,InputForQuote.tostring().c_str());
#endif
    return ret;
}

///执行宣告录入请求
///调用者不用管InputExecOrder中的BrokerID, InvestorID, UserID, ExecOrderRef这些值。底层自动确定，并返回
///ExecOrderRef暂时使用报单引用的值进行递增
int CPlatFormService::ReqExecOrderInsert(PlatformStru_InputExecOrderField& InputExecOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();
#ifdef CTP060300

    InputExecOrder.RequestID=nRequestID;
    strncpy(InputExecOrder.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(InputExecOrder.BrokerID)-1); ///经纪公司代码       
    strncpy(InputExecOrder.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(InputExecOrder.InvestorID)-1);  ///投资者代码
    strncpy(InputExecOrder.UserID, m_PlatformParam.UserID.c_str(), sizeof(InputExecOrder.UserID)-1);  ///用户代码
	sprintf(InputExecOrder.ExecOrderRef,"%12d",++m_CurExecOrderRef);

    CThostFtdcInputExecOrderField thost;
    InputExecOrder.ToThost(thost);

    ret = m_pTradeApi->ReqExecOrderInsert(&thost, nRequestID);  
    LOG_INFO("ReqExecOrderInsert(执行宣告录入请求):ret=[%d],nRequestID=[%d]\n%s",
             ret, nRequestID,InputExecOrder.tostring().c_str());
#endif
    return ret;
}

///执行宣告操作请求
///调用者不用管InputExecOrderAction中的BrokerID, InvestorID, UserID, ExecOrderActionRef这些值。底层自动确定，并返回
///ExecOrderActionRef暂时使用报单引用的值进行递增
int CPlatFormService::ReqExecOrderAction(PlatformStru_InputExecOrderActionField& InputExecOrderAction, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();
#ifdef CTP060300

    InputExecOrderAction.RequestID=nRequestID;
    strncpy(InputExecOrderAction.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(InputExecOrderAction.BrokerID)-1); ///经纪公司代码       
    strncpy(InputExecOrderAction.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(InputExecOrderAction.InvestorID)-1);  ///投资者代码
    strncpy(InputExecOrderAction.UserID, m_PlatformParam.UserID.c_str(), sizeof(InputExecOrderAction.UserID)-1);  ///用户代码
	InputExecOrderAction.ExecOrderActionRef=++m_CurExecOrderActionRef;

    CThostFtdcInputExecOrderActionField thost;
    InputExecOrderAction.ToThost(thost);

    ret = m_pTradeApi->ReqExecOrderAction(&thost, nRequestID);  
    LOG_INFO("ReqExecOrderAction(执行宣告操作请求):ret=[%d],nRequestID=[%d]\n%s",
             ret, nRequestID,InputExecOrderAction.tostring().c_str());
#endif
    return ret;
}

///请求查询执行宣告
///调用者不用管InputForQuote中的BrokerID, InvestorID这些值。底层自动确定，并返回
int CPlatFormService::ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID)
{
    int ret = -999;
    CHECK_TRADE_STATUS();
#ifdef CTP060300

    strncpy(QryExecOrder.BrokerID, m_PlatformParam.BrokerID.c_str(), sizeof(QryExecOrder.BrokerID)-1); ///经纪公司代码       
    strncpy(QryExecOrder.InvestorID, m_PlatformParam.InvestorID.c_str(), sizeof(QryExecOrder.InvestorID)-1);  ///投资者代码

    CThostFtdcQryExecOrderField thost;
    QryExecOrder.ToThost(thost);

    ret = m_pTradeApi->ReqQryExecOrder(&thost, nRequestID);  
    LOG_INFO("ReqQryExecOrder(请求查询执行宣告):ret=[%d],nRequestID=[%d]\n%s",
             ret, nRequestID,QryExecOrder.tostring().c_str());
#endif
    return ret;
}


