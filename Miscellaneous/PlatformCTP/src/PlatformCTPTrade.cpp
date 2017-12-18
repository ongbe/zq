//#include "stdafx.h"
//
#include <windows.h>
#include "PlatformCTPTrade.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"


#pragma unmanaged


#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPTrade", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__)
#endif

#define strncpy_ctptrade(dest,sour)	{ memset((dest),0,sizeof(dest));  strncpy((dest),(sour),sizeof(dest)-1); }


//*************************************************************************
//  CQryQueue的实现
//*************************************************************************

CPlatformCTPTrade::CQryQueue::CQryQueue(CPlatformCTPTrade& RefTrade)
:	m_RefTrade(RefTrade),
	m_ReqQueue(20*1024*1024,0),
	m_bInQry(false),
	m_QryTime(0),
	m_bNeedExit(false),
	m_hThread(0)
{
    DWORD threadId;
    m_hThread=(int)CreateThread(NULL,2*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&threadId);
}
CPlatformCTPTrade::CQryQueue::~CQryQueue(void)
{
	m_bNeedExit=true;
	CTools_Win32::StopThread((HANDLE)m_hThread);
	m_hThread=NULL;
}
//是否到达查询时刻。ctp规定每秒只能有一次查询 
bool CPlatformCTPTrade::CQryQueue::IsQryTime(void)
{
    DWORD CurTickCount=GetTickCount();
    if(!m_bInQry)
    {                                                       //没有在途查询
        if(CurTickCount>=m_QryTime && CurTickCount-m_QryTime > 1000 ||
            CurTickCount<m_QryTime && CurTickCount+((DWORD)0xffffffff-m_QryTime) > 1000)
            return true;
    }
    else
    {                                                       //有在途查询，超时为8s
        if(CurTickCount>=m_QryTime && CurTickCount-m_QryTime > 8000 ||
            CurTickCount<m_QryTime && CurTickCount+((DWORD)0xffffffff-m_QryTime) > 8000)
            return true;
    }
    return false;
}
bool CPlatformCTPTrade::CQryQueue::IsCacheEmpty(void)
{
	return m_ReqQueue.IsEmpty();
}
//发送查询请求，返回值表示是否成功发送请求
//ReqRlt表示如果进行了查询，查询返回值，<0表示查询失败
bool CPlatformCTPTrade::CQryQueue::SendQryCmd(int& ReqRlt)
{
	if(m_RefTrade.GetConnStatus()!=CTPCONNSTATUS_Connected) return false;
	if(!m_RefTrade.m_pTradeApi) return false;

	char Buf[10240];
	CVariablePackage::HeadStru PackageHead;

	ReqRlt=0;
	bool bProcessQueue=false;

	while(!m_ReqQueue.IsEmpty())
	{
		if(m_ReqQueue.GetCurPackageDataLen()>sizeof(Buf))
		{
			m_ReqQueue.DeleteCurPackage();
			continue;
		}

		PackageHead=m_ReqQueue.PeekFrom(Buf,sizeof(Buf));

		switch(PackageHead.CmdID)
		{
			case QryID_ReqQryNotice:						//请求查询客户通知
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryNoticeField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryNotice((CThostFtdcQryNoticeField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询客户通知)ReqQryNotice RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQrySettlementInfoConfirm:         //请求查询结算信息确认
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQrySettlementInfoConfirmField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQrySettlementInfoConfirm((CThostFtdcQrySettlementInfoConfirmField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询结算信息确认)ReqQrySettlementInfoConfirm RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQrySettlementInfo:				//查询投资者结算结果
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQrySettlementInfoField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQrySettlementInfo((CThostFtdcQrySettlementInfoField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询投资者结算结果)ReqQrySettlementInfo RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInstrument:                    //查询合约信息
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInstrumentField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInstrument((CThostFtdcQryInstrumentField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询合约信息)ReqQryInstrument : InstrumentID=%s RequestID=%d Rlt=%d",((CThostFtdcQryInstrumentField*)Buf)->InstrumentID,PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryOrder:                         //查询报单
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryOrderField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryOrder((CThostFtdcQryOrderField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询报单信息)QryID_ReqQryOrder RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryTrade:                         //查询成交
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryTradeField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryTrade((CThostFtdcQryTradeField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询成交信息)ReqQryTrade RequestID=%d Rlt=%d ",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInvestorPosition:              //查询持仓
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInvestorPositionField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInvestorPosition((CThostFtdcQryInvestorPositionField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询持仓汇总)ReqQryInvestorPosition RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInvestorPositionDetail:        //查询持仓明细
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInvestorPositionDetailField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInvestorPositionDetail((CThostFtdcQryInvestorPositionDetailField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询持仓明细)ReqQryInvestorPositionDetail RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInvestorPositionCombineDetail:              //请求查询投资者持仓明细(组合)
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInvestorPositionCombineDetailField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInvestorPositionCombineDetail((CThostFtdcQryInvestorPositionCombineDetailField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询投资者持仓明细(组合))ReqQryInvestorPositionCombineDetail RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryTradingAccount:                //查询资金
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryTradingAccountField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryTradingAccount((CThostFtdcQryTradingAccountField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询资金信息)ReqQryTradingAccount RequestID=%d Rlt=%d",PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInstrumentCommissionRate:      //查询合约手续费率

				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInstrumentCommissionRateField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInstrumentCommissionRate((CThostFtdcQryInstrumentCommissionRateField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询合约手续费率)ReqQryInstrumentCommissionRate，InstrumentID=%s RequestID=%d Rlt=%d",((CThostFtdcQryInstrumentCommissionRateField*)Buf)->InstrumentID,PackageHead.UserData1,ReqRlt);
				break;
			case QryID_ReqQryInstrumentMarginRate:          //请求查询合约保证金率
				if(PackageHead.DataLen1!=sizeof(CThostFtdcQryInstrumentMarginRateField))  break;
				ReqRlt=m_RefTrade.m_pTradeApi->ReqQryInstrumentMarginRate((CThostFtdcQryInstrumentMarginRateField*)Buf,(int)PackageHead.UserData1);
				bProcessQueue=true;
				LOG_INFO("CQryQueue::SendQryCmd : (查询合约保证金率)ReqQryInstrumentCommissionRate，InstrumentID=%s RequestID=%d Rlt=%d",((CThostFtdcQryInstrumentMarginRateField*)Buf)->InstrumentID,PackageHead.UserData1,ReqRlt);
				break;
		}

		if(!bProcessQueue)
		{
			m_ReqQueue.DeleteCurPackage();
			continue;
		}

        if(ReqRlt==0) 
        { 
            //成功发出查询请求
            m_bInQry=true;
			m_QryTime=GetTickCount();
			m_ReqQueue.DeleteCurPackage();
            return true; 
        }                                                               
        else if(ReqRlt==-2)
        {
            //发送请求失败，未处理请求超过许可数，也就是上一次的请求还没处理完。
            //设置m_bInQry，并返回true，以更新发送请求的时间，避免频繁发送
            m_bInQry=true;
			m_QryTime=GetTickCount();
            return true; 
        }
        else
        {
            return false;         
        }
    }
    return false;
}
//线程函数
unsigned int CPlatformCTPTrade::CQryQueue::ThreadProc(void *arg)
{
    CPlatformCTPTrade::CQryQueue   *pQryQueue=(CQryQueue*)arg;
    int ReqRlt;

    while(!pQryQueue->m_bNeedExit)
    {
		int sleepms=50;

        if(pQryQueue->m_RefTrade.GetConnStatus()!=CTPCONNSTATUS_Connected) 
			sleepms=1000;
        else if( !pQryQueue->IsQryTime()) 
			sleepms=50;
        else 
        {
			CeasymutexGuard guard(pQryQueue->m_RefTrade.m_mutex);

            if(pQryQueue->SendQryCmd(ReqRlt)) sleepms=1000;
            else
            {
                if(pQryQueue->IsCacheEmpty()) sleepms=100;
                else if(ReqRlt<0) sleepms=200;
            }
        }
		if(sleepms>0) Sleep(sleepms);
    }
    return 0;
}




//*************************************************************************
//  CPlatformCTPTrade的实现
//*************************************************************************
CPlatformCTPTrade::CPlatformCTPTrade(const Stru_CTPConnectionParam& ConnParam,
                                     bool bPushCache,bool bUseComb,bool bCalcuCombQuotOnLegQuot,
                                     const string& Ver_String,
									 bool bAutoReqSettlementInfoConfirmed,
									 bool bAutoConfirmSettlement,
									 bool bAutoQryInstrumentInfo)
:	CPlatformCTP(ConnParam,bPushCache,bUseComb,bCalcuCombQuotOnLegQuot),
	m_Ver_String(Ver_String),
	m_bAutoReqSettlementInfoConfirmed(bAutoReqSettlementInfoConfirmed),
    m_bAutoConfirmSettlement(bAutoConfirmSettlement),
	m_bAutoQryInstrumentInfo(bAutoQryInstrumentInfo),
    m_CurOrderRef(0),
    m_pTradeApi(NULL),
	m_pQryQueue(NULL),
	m_bSettlementInfoConfirmed(false)
{
    memset(&m_LoginInfo,0,sizeof(m_LoginInfo));
	m_pQryQueue=new CQryQueue(*this);
}

CPlatformCTPTrade::~CPlatformCTPTrade()
{
    CeasymutexGuard guard(m_mutex);

	if(m_pQryQueue)
	{
		delete m_pQryQueue;
		m_pQryQueue=NULL;
	}

    if(m_pTradeApi)
        Logout();
}
bool CPlatformCTPTrade::Login(const CThostFtdcTraderSpi* pSpi)
{
    CeasymutexGuard guard(m_mutex);

    if(m_pTradeApi) 
    {
        LOG_INFO("Login()成功：已经有有效m_pTradeApi(%x)",m_pTradeApi);
        return true;
    }
    if(m_ConnParam.BrokerID.empty()||m_ConnParam.UserID.empty()||m_ConnParam.UserPW.empty())
    {
        LOG_INFO("Login()失败：登录参数(BrokerID、UserID、UserPW)为空");
        return false;
    }

	SetConnStatus(CTPCONNSTATUS_Connecting,false);
 
	memset(&m_LoginInfo,0,sizeof(m_LoginInfo));

    //创建Api对象
    m_pTradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi();

    //注册回调
    m_pTradeApi->RegisterSpi(static_cast<CThostFtdcTraderSpi*>(pSpi?const_cast<CThostFtdcTraderSpi*>(pSpi):this));

    //订阅公有流和私有流
    m_pTradeApi->SubscribePublicTopic(THOST_TERT_QUICK);
    m_pTradeApi->SubscribePrivateTopic(THOST_TERT_QUICK);

    //注册前置机地址
    string addrstr;
    for(vector<string>::iterator it=m_ConnParam.FrontAddrs.begin();it!=m_ConnParam.FrontAddrs.end();++it)
    {
        addrstr=MakeConnectStr(*it,m_ConnParam.ProxyConnectStr);
		m_pTradeApi->RegisterFront(const_cast<char*>(addrstr.c_str()));
    }

    //初始化并启动前置机接口
	m_pTradeApi->Init();     


    LOG_INFO("Login()成功：m_pTradeApi(%x)",m_pTradeApi);
    return true;
}
void CPlatformCTPTrade::Logout(void)
{
	m_mutex.lock();
    if(!m_pTradeApi) 
    {
        LOG_INFO("Logout()成功：m_pTradeApi为NULL");
		m_mutex.unlock();
        return;
    }
	SetConnStatus(CTPCONNSTATUS_Disconnecting,false);
	m_mutex.unlock();

	//注意，Release() 会将剩余未处理的事务让回调处理，如果这个地方加锁的话，回调的锁会产生死锁。
	m_pTradeApi->RegisterSpi(NULL);
    m_pTradeApi->Release();
	m_pTradeApi=NULL;

	m_mutex.lock();
    m_pTradeApi=NULL;
	SetConnStatus(CTPCONNSTATUS_Disconnected,false);
	m_Ver_String=string("");
	m_bSettlementInfoConfirmed=false;
    LOG_INFO("Logout()成功");
	m_mutex.unlock();
}



///****************************************************************************
/// 下面是ctp指令执行函数。
///	一般情况下   return  :   0:		成功
///							-1:     网络连接失败
///							-2:     未处理请求超过许可数
///							-3:     每秒发送请求数超过许可数
///							-999:   其它原因失败
///****************************************************************************

//仅限于CPlatformCTPTrade的成员函数使用
#define CHECK_TRADE_STATUS() \
    if(!m_pTradeApi || GetConnStatus()!=CTPCONNSTATUS_Connected)\
    {\
        LOG_INFO("交易API还未准备就绪，可能登入未成功或正在重连");\
        return -999;\
    }


///用户口令更新请求
int CPlatformCTPTrade::ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();
    strncpy_ctptrade(pUserPasswordUpdate->BrokerID, m_ConnParam.BrokerID.c_str());	///经纪公司代码       
    strncpy_ctptrade(pUserPasswordUpdate->UserID, m_ConnParam.UserID.c_str());		///用户代码,等于投资者代码
    int ret = m_pTradeApi->ReqUserPasswordUpdate(pUserPasswordUpdate,++m_RequestID); 
    LOG_INFO("ReqUserPasswordUpdate():ret=[%d],RequestID=[%d]  \t BrokerID=[%s],UserID=[%s]",
		ret, m_RequestID,pUserPasswordUpdate->BrokerID,pUserPasswordUpdate->UserID);
    return ret;
}
///资金账户口令更新请求
int CPlatformCTPTrade::ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();
    strncpy_ctptrade(pTradingAccountPasswordUpdate->BrokerID, m_ConnParam.BrokerID.c_str()); ///经纪公司代码       
    strncpy_ctptrade(pTradingAccountPasswordUpdate->AccountID, m_ConnParam.UserID.c_str());  ///资金账号,等于用户代码,也等于投资者代码
    int ret = m_pTradeApi->ReqTradingAccountPasswordUpdate(pTradingAccountPasswordUpdate, ++m_RequestID);  
    LOG_INFO("ReqTradingAccountPasswordUpdate():ret=[%d],RequestID=[%d], \t BrokerID=[%s],AccountID=[%s]",
		ret, m_RequestID,pTradingAccountPasswordUpdate->BrokerID,pTradingAccountPasswordUpdate->AccountID);
    return ret;
}

///报单录入请求. 返回时pInputOrder中的RrequestID、BrokerID、InvestorID、UserID、OrderRef的值被修改为底层确定的值
int CPlatformCTPTrade::ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    pInputOrder->RequestID = ++m_RequestID;
    strncpy_ctptrade(pInputOrder->BrokerID, m_ConnParam.BrokerID.c_str()); ///经纪公司代码       
    strncpy_ctptrade(pInputOrder->InvestorID, m_ConnParam.InvestorID.c_str());  ///投资者代码
    strncpy_ctptrade(pInputOrder->UserID, m_ConnParam.UserID.c_str());  ///用户代码
    pInputOrder->ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	sprintf(pInputOrder->OrderRef,"%12d",++m_CurOrderRef);

    int ret = m_pTradeApi->ReqOrderInsert(pInputOrder, m_RequestID);  

    LOG_INFO("ReqOrderInsert(报单录入请求):ret=[%d],RequestID=[%d]\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
             "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
             "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
             "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
             "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
             ret, m_CurOrderRef,
             pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
             pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
             pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
             pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
             pInputOrder->RequestID, pInputOrder->UserForceClose);
    return ret;
}
///报单操作请求
///
int CPlatformCTPTrade::ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    pInputOrderAction->RequestID = ++m_RequestID;
    strncpy_ctptrade(pInputOrderAction->BrokerID, m_ConnParam.BrokerID.c_str()); ///经纪公司代码       
    strncpy_ctptrade(pInputOrderAction->InvestorID, m_ConnParam.InvestorID.c_str());  ///投资者代码
    pInputOrderAction->ActionFlag = THOST_FTDC_AF_Delete;

    int ret = m_pTradeApi->ReqOrderAction(pInputOrderAction, m_RequestID); 

    LOG_INFO("ReqOrderAction(报单操作请求):ret=[%d],RequestID=[%d]\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                     "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                     "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                     ret, m_RequestID,
                     pInputOrderAction->BrokerID, pInputOrderAction->InvestorID, pInputOrderAction->OrderActionRef, pInputOrderAction->OrderRef,pInputOrderAction->RequestID,
                     pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->ExchangeID, pInputOrderAction->OrderSysID,pInputOrderAction->ActionFlag,
                     pInputOrderAction->LimitPrice, pInputOrderAction->VolumeChange, pInputOrderAction->UserID, pInputOrderAction->InstrumentID);
    return ret;
}




///投资者结算结果确认
int CPlatformCTPTrade::ReqSettlementInfoConfirm()
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strncpy_ctptrade(req.BrokerID, m_ConnParam.BrokerID.c_str());
	strncpy_ctptrade(req.InvestorID, m_ConnParam.InvestorID.c_str());

    int ret = m_pTradeApi->ReqSettlementInfoConfirm(&req, ++m_RequestID);

    LOG_INFO("ReqSettlementInfoConfirm(投资者结算结果确认):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ConfirmDate=[%s],\t ConfirmTime=[%s]",
                     ret, m_RequestID, 
                     req.BrokerID,
                     req.InvestorID,
                     req.ConfirmDate,
                     req.ConfirmTime);
    return ret;
}

///请求查询投资者结算结果
int CPlatformCTPTrade::ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQrySettlementInfo->BrokerID, m_ConnParam.BrokerID.c_str());			///经纪公司代码       
    strncpy_ctptrade(pQrySettlementInfo->InvestorID, m_ConnParam.InvestorID.c_str());		///投资者代码

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQrySettlementInfo,pQrySettlementInfo,sizeof(*pQrySettlementInfo),NULL,0,++m_RequestID);

	LOG_INFO("Push into QryQueue: ReqQrySettlementInfo(请求查询投资者结算结果):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t TradingDay=[%s]",
                     brlt, m_RequestID, 
                     pQrySettlementInfo->BrokerID,
                     pQrySettlementInfo->InvestorID,
                     pQrySettlementInfo->TradingDay);
    return 0;
}
///请求查询结算信息确认
int CPlatformCTPTrade::ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	strncpy_ctptrade(pQrySettlementInfoConfirm->BrokerID, m_ConnParam.BrokerID.c_str());		///经纪公司代码    
	strncpy_ctptrade(pQrySettlementInfoConfirm->InvestorID, m_ConnParam.InvestorID.c_str());    ///投资者代码

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQrySettlementInfoConfirm,pQrySettlementInfoConfirm,sizeof(*pQrySettlementInfoConfirm),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQrySettlementInfoConfirm(请求查询结算信息确认):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     brlt, m_RequestID,
                     pQrySettlementInfoConfirm->BrokerID,
                     pQrySettlementInfoConfirm->InvestorID);
    return 0;
}
///请求查询客户通知
int CPlatformCTPTrade::ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	strncpy_ctptrade(pQryNotice->BrokerID, m_ConnParam.BrokerID.c_str()); ///经纪公司代码    

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryNotice,pQryNotice,sizeof(*pQryNotice),NULL,0,++m_RequestID);
    return 0;
}

///请求查询合约
int CPlatformCTPTrade::ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInstrument,pQryInstrument,sizeof(*pQryInstrument),NULL,0,++m_RequestID);

	LOG_INFO("Push into QryQueue: ReqQryInstrument(请求查询合约):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExchangeInstID=[%s],\tProductID=[%s]",
                     brlt, m_RequestID, 
                     pQryInstrument->InstrumentID,
                     pQryInstrument->ExchangeID,
                     pQryInstrument->ExchangeInstID,
                     pQryInstrument->ProductID);

    return 0;
}

///请求查询报单
int CPlatformCTPTrade::ReqQryOrder(CThostFtdcQryOrderField *pQryOrder)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryOrder->BrokerID, m_ConnParam.BrokerID.c_str());			///经纪公司代码       
    strncpy_ctptrade(pQryOrder->InvestorID, m_ConnParam.InvestorID.c_str());		///投资者代码

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryOrder,pQryOrder,sizeof(*pQryOrder),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryOrder(请求查询报单):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t OrderSysID=[%s],\n"
                     "\t\t\t InsertTimeStart=[%s],\t InsertTimeEnd=[%s]",
                     brlt, m_RequestID,
                     pQryOrder->BrokerID,
                     pQryOrder->InvestorID,
                     pQryOrder->InstrumentID,
                     pQryOrder->ExchangeID,
                     pQryOrder->OrderSysID,
                     pQryOrder->InsertTimeStart,
                     pQryOrder->InsertTimeEnd);

    return 0;
}

///请求查询成交
int CPlatformCTPTrade::ReqQryTrade(CThostFtdcQryTradeField *pQryTrade)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();
	
    strncpy_ctptrade(pQryTrade->BrokerID, m_ConnParam.BrokerID.c_str());		///经纪公司代码       
    strncpy_ctptrade(pQryTrade->InvestorID, m_ConnParam.InvestorID.c_str());  ///投资者代码

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryTrade,pQryTrade,sizeof(*pQryTrade),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryTrade(请求查询成交):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t TradeID=[%s],\n"
                     "\t\t\t TradeTimeStart=[%s],\t TradeTimeEnd=[%s]",
                     brlt, m_RequestID,
                     pQryTrade->BrokerID,
                     pQryTrade->InvestorID,
                     pQryTrade->InstrumentID,
                     pQryTrade->ExchangeID,
                     pQryTrade->TradeID,
                     pQryTrade->TradeTimeStart,
                     pQryTrade->TradeTimeEnd);
    return 0;
}

///请求查询投资者持仓
int CPlatformCTPTrade::ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

	strncpy_ctptrade(pQryInvestorPosition->BrokerID, m_ConnParam.BrokerID.c_str());
	strncpy_ctptrade(pQryInvestorPosition->InvestorID, m_ConnParam.InvestorID.c_str());

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInvestorPosition,pQryInvestorPosition,sizeof(*pQryInvestorPosition),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInvestorPosition(请求查询投资者持仓):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     brlt, m_RequestID,
                     pQryInvestorPosition->BrokerID,
                     pQryInvestorPosition->InvestorID,
                     pQryInvestorPosition->InstrumentID);
    return 0;
}
///请求查询投资者持仓明细
int CPlatformCTPTrade::ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryInvestorPositionDetail->BrokerID,m_ConnParam.BrokerID.c_str());
    strncpy_ctptrade(pQryInvestorPositionDetail->InvestorID,m_ConnParam.InvestorID.c_str());

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInvestorPositionDetail,pQryInvestorPositionDetail,sizeof(*pQryInvestorPositionDetail),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInvestorPositionDetail(请求查询投资者持仓明细):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     brlt, m_RequestID,
                     pQryInvestorPositionDetail->BrokerID,
                     pQryInvestorPositionDetail->InvestorID,
                     pQryInvestorPositionDetail->InstrumentID);
    return 0;
}
///请求查询投资者持仓明细
int CPlatformCTPTrade::ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryInvestorPositionCombineDetail->BrokerID,m_ConnParam.BrokerID.c_str());
    strncpy_ctptrade(pQryInvestorPositionCombineDetail->InvestorID,m_ConnParam.InvestorID.c_str());

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInvestorPositionCombineDetail,pQryInvestorPositionCombineDetail,sizeof(*pQryInvestorPositionCombineDetail),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInvestorPositionCombineDetail(请求查询投资者组合持仓明细):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t CombInstrumentID=[%s]",
                     brlt, m_RequestID,
                     pQryInvestorPositionCombineDetail->BrokerID,
                     pQryInvestorPositionCombineDetail->InvestorID,
                     pQryInvestorPositionCombineDetail->CombInstrumentID);
    return 0;
}



///请求查询资金账户
int CPlatformCTPTrade::ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryTradingAccount->BrokerID,m_ConnParam.BrokerID.c_str());
    strncpy_ctptrade(pQryTradingAccount->InvestorID,m_ConnParam.InvestorID.c_str());

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryTradingAccount,pQryTradingAccount,sizeof(*pQryTradingAccount),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryTradingAccount(请求查询资金账户):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s]",
                     brlt, m_RequestID,
                     pQryTradingAccount->BrokerID,
                     pQryTradingAccount->InvestorID);
    return 0;
}


///请求查询合约保证金率
int CPlatformCTPTrade::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryInstrumentMarginRate->BrokerID, m_ConnParam.BrokerID.c_str());			///经纪公司代码    
	strncpy_ctptrade(pQryInstrumentMarginRate->InvestorID, m_ConnParam.InvestorID.c_str());		///投资者代码
    if(pQryInstrumentMarginRate->HedgeFlag==0) pQryInstrumentMarginRate->HedgeFlag=THOST_FTDC_HF_Speculation;

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInstrumentMarginRate,pQryInstrumentMarginRate,sizeof(*pQryInstrumentMarginRate),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInstrumentMarginRate(请求查询合约保证金率):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%d]",
                     brlt, m_RequestID,
                     pQryInstrumentMarginRate->BrokerID,
                     pQryInstrumentMarginRate->InvestorID,
                     pQryInstrumentMarginRate->InstrumentID,
                     pQryInstrumentMarginRate->HedgeFlag);
    return 0;
}

///请求查询合约手续费率
int CPlatformCTPTrade::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate)
{
    CeasymutexGuard guard(m_mutex);

	CHECK_TRADE_STATUS();

    strncpy_ctptrade(pQryInstrumentCommissionRate->BrokerID, m_ConnParam.BrokerID.c_str());			///经纪公司代码    
	strncpy_ctptrade(pQryInstrumentCommissionRate->InvestorID, m_ConnParam.InvestorID.c_str());		///投资者代码

	bool brlt=m_pQryQueue->m_ReqQueue.SaveDataTo2(CQryQueue::QryID_ReqQryInstrumentCommissionRate,pQryInstrumentCommissionRate,sizeof(*pQryInstrumentCommissionRate),NULL,0,++m_RequestID);

    LOG_INFO("Push into QryQueue: ReqQryInstrumentCommissionRate(请求查询合约手续费率):ret=[%d],RequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s]",
                     brlt, m_RequestID,
                     pQryInstrumentCommissionRate->BrokerID,
                     pQryInstrumentCommissionRate->InvestorID,
                     pQryInstrumentCommissionRate->InstrumentID);
    return 0;
}












///****************************************************************************
/// 下面是ctp回调函数
///****************************************************************************

#ifndef _NEED_AUTHENTICATE
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CPlatformCTPTrade::OnFrontConnected()
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2((unsigned int)CmdID_Trade_FrontConnected, NULL,0);

    //发起登录请求
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_ConnParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_ConnParam.UserID.c_str(),sizeof(req.UserID)-1);
    strncpy(req.Password, m_ConnParam.UserPW.c_str(),sizeof(req.Password)-1);
    vector<string> vervec;
	vector<int> vecpos=CTools_Ansi::GetCharPos(m_Ver_String.c_str(),',');
    if((int)vecpos.size()==3)
    {
        char tmp[256]={0};
        sprintf(tmp,"ft%02d%02d%02d%02d",
            atoi(m_Ver_String.c_str()),
            atoi(m_Ver_String.c_str()+vecpos[0]),
            atoi(m_Ver_String.c_str()+vecpos[1]),
            atoi(m_Ver_String.c_str()+vecpos[2]));
	    strncpy(req.UserProductInfo,tmp,sizeof(req.UserProductInfo)-1);
    }
    else
    {
	    strncpy(req.UserProductInfo, "ft00000000",sizeof(req.UserProductInfo)-1);
    }
	strncpy(req.OneTimePassword,m_ConnParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

	SetConnStatus(CTPCONNSTATUS_Logining,false);

    int RequestID=++m_RequestID;
    int iRlt = m_pTradeApi->ReqUserLogin(&req, RequestID);

    LOG_INFO("OnFrontConnected(与交易所成功建立TCP连接)\n"
                     "\t\t\t --->ReqUserLogin(用户登录请求):ret=[%d],iRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
                     iRlt, RequestID,
                     req.BrokerID, req.UserID,req.UserProductInfo);
}
#else
///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
void CPlatformCTPTrade::OnFrontConnected()
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_FrontConnected, NULL,0);

    //发起认证请求
    GetLocalTime(&m_ConnectTime);//获得连接时间

    CThostFtdcReqAuthenticateField req;
    memset(&req, 0, sizeof(req));
    strncpy(req.BrokerID, m_ConnParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
    strncpy(req.UserID, m_ConnParam.UserID.c_str(),sizeof(req.UserID)-1);
    vector<string> vervec;
    if(SplitString(m_Ver_String,vervec)==4)
    {
        char tmp[256]={0};
        sprintf(tmp,"ft%02d%02d%02d%02d",
            atoi(vervec[0].c_str()),
            atoi(vervec[1].c_str()),
            atoi(vervec[2].c_str()),
            atoi(vervec[3].c_str()));
	    strncpy(req.UserProductInfo,tmp,sizeof(req.UserProductInfo)-1);
    }
    else
    {
	    strncpy(req.UserProductInfo, "ft00000000",sizeof(req.UserProductInfo)-1);
    }

	m_ConnStatus=CTPCONNSTATUS_Logining;

    int RequestID=++m_RequestID;
    int iRlt = m_pTradeApi->ReqAuthenticate(&req, RequestID);

    LOG_INFO("OnFrontConnected(与交易所成功建立TCP连接)\n"
                     "\t\t\t --->ReqAuthenticate(客户端认证请求):ret=[%d],iRequestID=[%d],\n"
                     "\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
                     iRlt, RequestID,
                     req.BrokerID, req.UserID,req.UserProductInfo);
}
//客户端认证响应
void CPlatformCTPTrade::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(pRspInfo&&pRspInfo->ErrorID==0)//登入成功
	{
		CThostFtdcReqUserLoginField req;
		memset(&req, 0, sizeof(req));
		strncpy(req.BrokerID, m_ConnParam.BrokerID.c_str(),sizeof(req.BrokerID)-1);
		strncpy(req.UserID, m_ConnParam.UserID.c_str(),sizeof(req.UserID)-1);
		strncpy(req.Password, m_ConnParam.UserPW.c_str(),sizeof(req.Password)-1);
        vector<string> vervec;
        if(SplitString(m_Ver_String,vervec)==4)
        {
            char tmp[256]={0};
            sprintf(tmp,"ft%02d%02d%02d%02d",
                atoi(vervec[0].c_str()),
                atoi(vervec[1].c_str()),
                atoi(vervec[2].c_str()),
                atoi(vervec[3].c_str()));
	        strncpy(req.UserProductInfo,tmp,sizeof(req.UserProductInfo)-1);
        }
        else
        {
	        strncpy(req.UserProductInfo, "ft00000000",sizeof(req.UserProductInfo)-1);
        }
		strncpy(req.OneTimePassword,m_ConnParam.OneTimePassword.c_str(),sizeof(req.OneTimePassword)-1);

        int RequestID=++m_RequestID;
		int iRlt = m_pTradeApi->ReqUserLogin(&req, RequestID);

		LOG_INFO("OnRspAuthenticate(客户端认证响应)\n"
			"\t\t\t --->ReqUserLogin(用户登录请求):ret=[%d],RequestID=[%d],\n"
			"\t\t\t BrokerID=[%s],\t UserID=[%s],\t UserProductInfo=[%s]",
			iRlt, RequestID,
			req.BrokerID, req.UserID,req.UserProductInfo);
	}
	else
	{
        m_ConnStatus=CTPCONNSTATUS_LoginFailure;
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
void CPlatformCTPTrade::OnFrontDisconnected(int nReason)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_FrontDisconnected, NULL,0,NULL,0,(DWORD)nReason);
	SetConnStatus(CTPCONNSTATUS_Disconnected,false);

    LOG_INFO("OnFrontDisconnected(与交易所失去TCP连接), Reason=[%#x]", nReason);
}
    
///心跳超时警告。当长时间未收到报文时，该方法被调用。
///@param nTimeLapse 距离上次接收报文的时间
///综合交易平台的API已经屏蔽此接口
void CPlatformCTPTrade::OnHeartBeatWarning(int nTimeLapse)
{
    LOG_INFO("OnHeartBeatWarning:nTimeLapse=[%d]", nTimeLapse);
}


///登录请求响应
void CPlatformCTPTrade::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(pRspUserLogin&&(!pRspInfo||pRspInfo->ErrorID==0))
    {
        memcpy(&m_LoginInfo,pRspUserLogin,sizeof(m_LoginInfo));
        m_CurOrderRef=atoi(pRspUserLogin->MaxOrderRef);
    }
    else
    {
		SetConnStatus(CTPCONNSTATUS_LoginFailure,false);
    }
    
    if(m_bPushCache) 
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspUserLogin,
            pRspUserLogin,pRspUserLogin?sizeof(*pRspUserLogin):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
            (unsigned int)nRequestID, (unsigned int)bIsLast,
			(unsigned int)GetConnStatus());
    }

        
    if(pRspUserLogin)
    {
        LOG_INFO("OnRspUserLogin(登录请求响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]\n"
                        "\t\t\t TradingDay=[%s],\t LoginTime=[%s],\t BrokerID=[%s],\t UserID=[%s],\t SystemName=[%s]\n"
                        "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t MaxOrderRef[%s],\t SHFETime=[%s],\t DCETime=[%s],\n"
                        "\t\t\t CZCETime[%s],\t FFEXTime=[%s]",
						pRspInfo,pRspInfo?pRspInfo->ErrorID:0,pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
                        pRspUserLogin->TradingDay,pRspUserLogin->LoginTime,pRspUserLogin->BrokerID   , pRspUserLogin->UserID,pRspUserLogin->SystemName, 
                        pRspUserLogin->FrontID  , pRspUserLogin->SessionID ,pRspUserLogin->MaxOrderRef, pRspUserLogin->SHFETime , pRspUserLogin->DCETime, 
                        pRspUserLogin->CZCETime   , pRspUserLogin->FFEXTime);
    }
    else 
    {
        LOG_INFO("OnRspUserLogin(登录请求响应) : pRspUserLogin=[%x] pRspInfo=[%x] ErrorID=%d",
            pRspUserLogin,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }

    if(pRspUserLogin&&(!pRspInfo||pRspInfo->ErrorID==0))
    {
		SetConnStatus(CTPCONNSTATUS_Connected,false);

        //查询结算结果确认信息
		if(m_bAutoReqSettlementInfoConfirmed)
		{
			CThostFtdcQrySettlementInfoConfirmField req;
			memset(&req, 0, sizeof(req));
			strncpy_ctptrade(req.BrokerID, m_ConnParam.BrokerID.c_str());
			strncpy_ctptrade(req.InvestorID, m_ConnParam.InvestorID.c_str());

			int iRlt = ReqQrySettlementInfoConfirm(&req);
			LOG_INFO("OnRspUserLogin--->ReqQrySettlementInfoConfirm(请求查询投资者结算结果确认):ret=[%d]",iRlt); 
		}

		//查询合约信息
		if(m_bAutoQryInstrumentInfo)
		{
			CThostFtdcQryInstrumentField req;
			memset(&req,0,sizeof(req));
			int iRlt=ReqQryInstrument(&req) ;
			LOG_INFO("OnRspUserLogin--->ReqQryInstrument(请求查询合约信息):ret=[%d]",iRlt); 
		}
    }
}

///请求查询结算信息确认响应，这里指查询“是否已经进行结算确认”
void CPlatformCTPTrade::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

	if(pSettlementInfoConfirm&&(!pRspInfo||pRspInfo->ErrorID==0))
		m_bSettlementInfoConfirmed=true;

    if(m_bPushCache) 
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspQrySettlementInfoConfirm,
            pSettlementInfoConfirm,pSettlementInfoConfirm?sizeof(*pSettlementInfoConfirm):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
            (unsigned int)nRequestID,(unsigned int)bIsLast,
			m_bSettlementInfoConfirmed);
    }

    LOG_INFO("OnRspQrySettlementInfoConfirm(请求查询投资者结算结果确认响应)：pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]\n"
		"\t\t\t pSettlementInfoConfirm=[%x],m_bSettlementInfoConfirmed=%d\n"
        "\t\t\t BrokerID=%s InvestorID=%s ConfirmDate=%s ConfirmTime=%s",
        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
        pSettlementInfoConfirm,m_bSettlementInfoConfirmed,
        pSettlementInfoConfirm?pSettlementInfoConfirm->BrokerID:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->InvestorID:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmDate:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmTime:"");

    //自动进行结算确认
    if(!m_bSettlementInfoConfirmed&&m_bAutoConfirmSettlement)
    {
	    CThostFtdcSettlementInfoConfirmField req;
	    memset(&req, 0, sizeof(req));
	    strncpy_ctptrade(req.BrokerID, m_ConnParam.BrokerID.c_str());
	    strncpy_ctptrade(req.InvestorID, m_ConnParam.InvestorID.c_str());

        int ret = m_pTradeApi->ReqSettlementInfoConfirm(&req, ++m_RequestID);

        LOG_INFO("ReqSettlementInfoConfirm(投资者结算结果确认):ret=[%d],RequestID=[%d],\n"
                         "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t ConfirmDate=[%s],\t ConfirmTime=[%s]",
                         ret, m_RequestID, 
                         req.BrokerID,
                         req.InvestorID,
                         req.ConfirmDate,
                         req.ConfirmTime);
    }
}

///投资者结算结果确认响应，这里指进行确认的相应
void CPlatformCTPTrade::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(pSettlementInfoConfirm&&(!pRspInfo||pRspInfo->ErrorID==0))
	    m_bSettlementInfoConfirmed=true;

    if(m_bPushCache) 
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspSettlementInfoConfirm,
            pSettlementInfoConfirm,pSettlementInfoConfirm?sizeof(*pSettlementInfoConfirm):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast,
            m_bSettlementInfoConfirmed);
    }

    LOG_INFO("OnRspSettlementInfoConfirm(投资者结算结果确认响应)：pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]\n"
		"\t\t\t pSettlementInfoConfirm=[%x] m_bSettlementInfoConfirmed=%d\n"
        "\t\t\t BrokerID=%s InvestorID=%s ConfirmDate=%s ConfirmTime=%s",
        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
        pSettlementInfoConfirm,m_bSettlementInfoConfirmed,
        pSettlementInfoConfirm?pSettlementInfoConfirm->BrokerID:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->InvestorID:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmDate:"",
        pSettlementInfoConfirm?pSettlementInfoConfirm->ConfirmTime:"");

}







///用户口令更新请求响应
void CPlatformCTPTrade::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(pUserPasswordUpdate&&(!pRspInfo||pRspInfo->ErrorID==0))
		m_ConnParam.UserPW=pUserPasswordUpdate->NewPassword;

    if(m_bPushCache) 
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspUserPasswordUpdate, 
            pUserPasswordUpdate,pUserPasswordUpdate?sizeof(*pUserPasswordUpdate):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
    }

    if(pUserPasswordUpdate)
    {
        LOG_INFO("OnRspUserPasswordUpdate(用户口令更新请求响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t UserID=[%s],\t OldPassword=[******],\t NewPassword=[******]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pUserPasswordUpdate->BrokerID, pUserPasswordUpdate->UserID);
    }
    else
    {
        LOG_INFO("OnRspUserPasswordUpdate(用户口令更新请求响应) : pUserPasswordUpdate=[%x] pRspInfo=[%x] ErrorID=%d",
            pUserPasswordUpdate,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }
}

///资金账户口令更新请求响应
void CPlatformCTPTrade::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache)
    {
        m_Cache.SaveDataTo2(CmdID_Trade_RspTradingAccountPasswordUpdate, 
            pTradingAccountPasswordUpdate,pTradingAccountPasswordUpdate?sizeof(*pTradingAccountPasswordUpdate):0,
            pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
    }

    if(pTradingAccountPasswordUpdate)
    {
        LOG_INFO("OnRspTradingAccountPasswordUpdate(资金账户口令更新请求响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],BrokerID=[%s] AccountID=[%s],RequestID=[%d],bIsLast=[%d]",
			    pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", 
			    pTradingAccountPasswordUpdate->BrokerID, pTradingAccountPasswordUpdate->AccountID, nRequestID,bIsLast);
    }
    else
    {
        LOG_INFO("OnRspTradingAccountPasswordUpdate(资金账户口令更新请求响应) : pTradingAccountPasswordUpdate=[%x] pRspInfo=[%x] ErrorID=%d",
            pTradingAccountPasswordUpdate,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }
}

///报单录入请求响应
void CPlatformCTPTrade::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspOrderInsert,
			pInputOrder,pInputOrder?sizeof(*pInputOrder):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast); 
	}

    if(pInputOrder)
    {
        LOG_INFO("OnRspOrderInsert(报单录入请求响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                 "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                 "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                 "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                 "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                 "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
                 pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                 pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
                 pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
                 pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
                 pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
                 pInputOrder->RequestID, pInputOrder->UserForceClose);
    }
    else
    {
        LOG_INFO("OnRspOrderInsert(报单录入请求响应) : pInputOrder=[%x] pRspInfo=[%x] ErrorID=%d",
            pInputOrder,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }
}

///报单操作请求响应
void CPlatformCTPTrade::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspOrderAction,
			pInputOrderAction,pInputOrderAction?sizeof(*pInputOrderAction):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);  
	}

    if(pInputOrderAction)
    {
        LOG_INFO("OnRspOrderAction(报单操作请求响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                         "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                         "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                         "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
                         pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                         pInputOrderAction->BrokerID, pInputOrderAction->InvestorID, pInputOrderAction->OrderActionRef, pInputOrderAction->OrderRef,pInputOrderAction->RequestID,
                         pInputOrderAction->FrontID, pInputOrderAction->SessionID, pInputOrderAction->ExchangeID, pInputOrderAction->OrderSysID,pInputOrderAction->ActionFlag,
                         pInputOrderAction->LimitPrice, pInputOrderAction->VolumeChange, pInputOrderAction->UserID, pInputOrderAction->InstrumentID);
    }
    else
    {
        LOG_INFO("OnRspOrderAction(报单操作请求响应) : pInputOrderAction=[%x] pRspInfo=[%x] ErrorID=%d",
            pInputOrderAction,pRspInfo,pRspInfo?pRspInfo->ErrorID:0);
    }
}





///请求查询报单响应
void CPlatformCTPTrade::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryOrder,
			pOrder,pOrder?sizeof(*pOrder):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);  
	}

    if(pOrder&&pRspInfo)
    {
        LOG_INFO("OnRspQryOrder(请求查询报单响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
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
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
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
    else
    {
        LOG_INFO("OnRspQryOrder(请求查询报单响应) : pOrder=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pOrder,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///请求查询成交响应
void CPlatformCTPTrade::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryTrade,
			pTrade,pTrade?sizeof(*pTrade):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pTrade)
    {
        LOG_INFO("OnRspQryTrade(请求查询成交响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t ExchangeID=[%s],\t TradeID=[%s],\t Direction=[%d],\t OrderSysID=[%s],\t ParticipantID=[%s],\n"
                        "\t\t\t ClientID=[%s],\t TradingRole=[%d],\t ExchangeInstID=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t Price=[%g],\t Volume=[%d],\t TradeDate=[%s],\t TradeTime=[%s],\t TradeType=[%d],\n"
                        "\t\t\t PriceSource=[%d],\t TraderID=[%s],\t OrderLocalID=[%s],\t ClearingPartID=[%s],\t BusinessUnit=[%s],\n"
                        "\t\t\t SequenceNo=[%d],\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerOrderSeq=[%d],\t TradeSource=[%c]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast,
                        pTrade->BrokerID,pTrade->InvestorID,pTrade->InstrumentID,pTrade->OrderRef,pTrade->UserID,
                        pTrade->ExchangeID,pTrade->TradeID,pTrade->Direction,pTrade->OrderSysID,pTrade->ParticipantID,
                        pTrade->ClientID,pTrade->TradingRole,pTrade->ExchangeInstID,pTrade->OffsetFlag,pTrade->HedgeFlag,
                        pTrade->Price,pTrade->Volume,pTrade->TradeDate,pTrade->TradeTime,pTrade->TradeType,
                        pTrade->PriceSource,pTrade->TraderID,pTrade->OrderLocalID,pTrade->ClearingPartID,pTrade->BusinessUnit,
                        pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq, pTrade->TradeSource);
    }
    else
    {
        LOG_INFO("OnRspQryTrade(请求查询成交响应) : pTrade=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pTrade,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///请求查询资金账户响应
void CPlatformCTPTrade::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryTradingAccount,
			pTradingAccount,pTradingAccount?sizeof(*pTradingAccount):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pTradingAccount)
    {
        LOG_INFO("OnRspQryTradingAccount(请求查询资金账户响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t BrokerID=[%s],\t AccountID=[%s],\t PreMortgage=[%g],\t PreCredit=[%g],\t PreDeposit=[%g],\n"
                        "\t\t\t PreBalance=[%g],\t PreMargin=[%g],\t InterestBase=[%g],\t Interest=[%g],\t Deposit=[%g],\n "
                        "\t\t\t Withdraw=[%g],\t FrozenMargin=[%g],\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CurrMargin=[%g],\n "
                        "\t\t\t CashIn=[%g],\t Commission=[%g],\t CloseProfit=[%g],\t PositionProfit=[%g],\t Balance=[%g],\n "
                        "\t\t\t Available=[%g],\t WithdrawQuota=[%g],\t Reserve=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n "
                        "\t\t\t Credit=[%g],\t Mortgage=[%g],\t ExchangeMargin=[%g],\t DeliveryMargin=[%g],\t ExchangeDeliveryMargin=[%g]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pTradingAccount->BrokerID,pTradingAccount->AccountID,pTradingAccount->PreMortgage,pTradingAccount->PreCredit,pTradingAccount->PreDeposit, 
                        pTradingAccount->PreBalance,pTradingAccount->PreMargin,pTradingAccount->InterestBase,pTradingAccount->Interest,pTradingAccount->Deposit, 
                        pTradingAccount->Withdraw,pTradingAccount->FrozenMargin,pTradingAccount->FrozenCash,pTradingAccount->FrozenCommission,pTradingAccount->CurrMargin, 
                        pTradingAccount->CashIn,pTradingAccount->Commission,pTradingAccount->CloseProfit,pTradingAccount->PositionProfit,pTradingAccount->Balance, 
                        pTradingAccount->Available,pTradingAccount->WithdrawQuota,pTradingAccount->Reserve,pTradingAccount->TradingDay,pTradingAccount->SettlementID, 
                        pTradingAccount->Credit,pTradingAccount->Mortgage,pTradingAccount->ExchangeMargin,pTradingAccount->DeliveryMargin,pTradingAccount->ExchangeDeliveryMargin
                        );
    }
    else
    {
        LOG_INFO("OnRspQryTrade(请求查询资金账户响应) : pTradingAccount=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pTradingAccount,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///请求查询投资者响应
void CPlatformCTPTrade::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInvestor,
			pInvestor,pInvestor?sizeof(*pInvestor):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInvestor)
    {
        LOG_INFO("OnRspQryInvestor(请求查询投资者响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InvestorID=[%s],\t BrokerID=[%s],\t InvestorGroupID=[%s],\t InvestorName=[%s],\t IdentifiedCardType=[%d],\n"
                        "\t\t\t IdentifiedCardNo=[%s],\t IsActive=[%d],\t Telephone=[%s],\t Address=[%s]",
                        pRspInfo,pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID,bIsLast,
                        pInvestor->InvestorID, pInvestor->BrokerID,pInvestor->InvestorGroupID,pInvestor->InvestorName,pInvestor->IdentifiedCardType,
                        pInvestor->IdentifiedCardNo, pInvestor->IsActive,pInvestor->Telephone,pInvestor->Address);
    }
    else
    {
        LOG_INFO("OnRspQryInvestor(请求查询投资者响应) : pInvestor=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
            pInvestor,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}


///请求查询合约保证金率响应
void CPlatformCTPTrade::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInstrumentMarginRate,
			pInstrumentMarginRate,pInstrumentMarginRate?sizeof(*pInstrumentMarginRate):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInstrumentMarginRate)
    {
        LOG_INFO("OnRspQryInstrumentMarginRate(请求查询合约保证金率响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\n"
                        "\t\t\t LongMarginRatioByMoney=[%g],\t LongMarginRatioByVolume=[%g],\t ShortMarginRatioByMoney=[%g],\t ShortMarginRatioByVolume=[%g],\t IsRelative=[%d]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pInstrumentMarginRate->InstrumentID,
                        pInstrumentMarginRate->InvestorRange,
                        pInstrumentMarginRate->BrokerID,
                        pInstrumentMarginRate->InvestorID,
                        pInstrumentMarginRate->HedgeFlag,
                        pInstrumentMarginRate->LongMarginRatioByMoney,
                        pInstrumentMarginRate->LongMarginRatioByVolume,
                        pInstrumentMarginRate->ShortMarginRatioByMoney,
                        pInstrumentMarginRate->ShortMarginRatioByVolume,
                        pInstrumentMarginRate->IsRelative);
    }
    else
    {
        LOG_INFO("OnRspQryInstrumentMarginRate(请求查询合约保证金率响应) : pInstrumentMarginRate=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],",
            pInstrumentMarginRate,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///请求查询合约手续费率响应
void CPlatformCTPTrade::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInstrumentCommissionRate,
			pInstrumentCommissionRate,pInstrumentCommissionRate?sizeof(*pInstrumentCommissionRate):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInstrumentCommissionRate)
    {
        LOG_INFO("OnRspQryInstrumentCommissionRate(请求查询合约手续费率响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t InvestorRange=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t OpenRatioByMoney=[%g],\n"
                        "\t\t\t OpenRatioByVolume=[%g],\t CloseRatioByMoney=[%g],\t CloseRatioByVolume=[%g],\t CloseTodayRatioByMoney=[%g],\t CloseTodayRatioByVolume=[%g]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pInstrumentCommissionRate->InstrumentID,
                        pInstrumentCommissionRate->InvestorRange,
                        pInstrumentCommissionRate->BrokerID,
                        pInstrumentCommissionRate->InvestorID,
                        pInstrumentCommissionRate->OpenRatioByMoney,
                        pInstrumentCommissionRate->OpenRatioByVolume,
                        pInstrumentCommissionRate->CloseRatioByMoney,
                        pInstrumentCommissionRate->CloseRatioByVolume,
                        pInstrumentCommissionRate->CloseTodayRatioByMoney,
                        pInstrumentCommissionRate->CloseTodayRatioByVolume);
    }
    else
    {
        LOG_INFO("OnRspQryInstrumentCommissionRate(请求查询合约手续费率响应) : pInstrumentCommissionRate=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pInstrumentCommissionRate,pRspInfo,
						 pRspInfo?pRspInfo->ErrorID:0, 
                         pRspInfo?pRspInfo->ErrorMsg:"",
                         nRequestID,bIsLast);
    }
}


///请求查询合约响应
void CPlatformCTPTrade::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    //修复大商所 组合 合约乘数为0的问题
	if(pInstrument&&
        strcmp(pInstrument->ExchangeID,"DCE")==0&&
		pInstrument->ProductClass==THOST_FTDC_PC_Combination&&
		pInstrument->VolumeMultiple==0)
	{
		pInstrument->VolumeMultiple=1;
	}

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInstrument,
			pInstrument,pInstrument?sizeof(*pInstrument):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInstrument)
    {
        LOG_INFO("OnRspQryInstrument(请求查询合约响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
			     "\t\t\t InstrumentID=[%s],\t ExchangeID=[%s],\t InstrumentName=[%s],\t ExchangeInstID=[%s],\t ProductID=[%s], \n"
			     "\t\t\t ProductClass=[%c],\t DeliveryYear=[%u],\t DeliveryMonth=[%u],\t MaxMarketOrderVolume=[%d],\t MinMarketOrderVolume=[%d], \n"
			     "\t\t\t MaxLimitOrderVolume=[%d],\t MinLimitOrderVolume=[%d],\t VolumeMultiple=[%d],\t PriceTick=[%g],\t CreateDate=[%s], \n"
			     "\t\t\t OpenDate=[%s],\t ExpireDate=[%s],\t StartDelivDate=[%s],\t EndDelivDate=[%s],\t InstLifePhase=[%c], \n"
			     "\t\t\t IsTrading=[%d],\t PositionType=[%c],\t PositionDateType=[%c],\t LongMarginRatio=[%g],\t ShortMarginRatio=[%g]\n", 
			     pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID, bIsLast,
			     pInstrument->InstrumentID, pInstrument->ExchangeID, pInstrument->InstrumentName, pInstrument->ExchangeInstID, pInstrument->ProductID, 
			     pInstrument->ProductClass, pInstrument->DeliveryYear, pInstrument->DeliveryMonth, pInstrument->MaxMarketOrderVolume, pInstrument->MinMarketOrderVolume, 
			     pInstrument->MaxLimitOrderVolume, pInstrument->MinLimitOrderVolume, pInstrument->VolumeMultiple, pInstrument->PriceTick, pInstrument->CreateDate, 
			     pInstrument->OpenDate, pInstrument->ExpireDate, pInstrument->StartDelivDate, pInstrument->EndDelivDate, pInstrument->InstLifePhase, 
			     pInstrument->IsTrading, pInstrument->PositionType, pInstrument->PositionDateType, pInstrument->LongMarginRatio, pInstrument->ShortMarginRatio
			     );
    }
    else
    {
        LOG_INFO("OnRspQryInstrument(请求查询合约响应) : pInstrument=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pInstrument,pRspInfo,
						 pRspInfo?pRspInfo->ErrorID:0, 
                         pRspInfo?pRspInfo->ErrorMsg:"",
                         nRequestID,bIsLast);
    }

}


///请求查询投资者结算结果响应
void CPlatformCTPTrade::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQrySettlementInfo,
			pSettlementInfo,pSettlementInfo?sizeof(*pSettlementInfo):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pSettlementInfo)
    {
        LOG_INFO("OnRspQrySettlementInfo(请求查询投资者结算结果响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t TradingDay=[%s],\t SettlementID=[%d],\t BrokerID=[%s],\t InvestorID=[%s],\t SequenceNo=[%d],\n"
                        "\t\t\t Content=[%s]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pSettlementInfo->TradingDay, pSettlementInfo->SettlementID,pSettlementInfo->BrokerID,pSettlementInfo->InvestorID,pSettlementInfo->SequenceNo,
                        pSettlementInfo->Content);
    }
    else
    {
        LOG_INFO("OnRspQrySettlementInfo(请求查询投资者结算结果响应) : pSettlementInfo=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],",
                         pSettlementInfo,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}


///请求查询投资者持仓明细响应
void CPlatformCTPTrade::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInvestorPositionDetail,
			pInvestorPositionDetail,pInvestorPositionDetail?sizeof(*pInvestorPositionDetail):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInvestorPositionDetail)
    {
        LOG_INFO("OnRspQryInvestorPositionDetail(请求查询投资者持仓明细响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t HedgeFlag=[%d],\t Direction=[%d],\n"
                        "\t\t\t OpenDate=[%s],\t TradeID=[%s],\t Volume=[%d],\t OpenPrice=[%g],\t TradingDay=[%s],\n"
                        "\t\t\t SettlementID=[%d],\t  TradeType=[%d],\t CombInstrumentID=[%s],\t ExchangeID=[%s],\t CloseProfitByDate=[%g],\n"
                        "\t\t\t CloseProfitByTrade=[%g],\t PositionProfitByDate=[%g],\t PositionProfitByTrade=[%g],\t Margin=[%g],\t ExchMargin=[%g],\n"
                        "\t\t\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g],\t LastSettlementPrice=[%g],\t SettlementPrice=[%g],CloseVolume=[%d],\n"
                        "\t\t\t CloseAmount=[%g]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                        pInvestorPositionDetail->InstrumentID, pInvestorPositionDetail->BrokerID,pInvestorPositionDetail->InvestorID,pInvestorPositionDetail->HedgeFlag,pInvestorPositionDetail->Direction,
                        pInvestorPositionDetail->OpenDate, pInvestorPositionDetail->TradeID,pInvestorPositionDetail->Volume,pInvestorPositionDetail->OpenPrice,pInvestorPositionDetail->TradingDay,
                        pInvestorPositionDetail->SettlementID, pInvestorPositionDetail->TradeType,pInvestorPositionDetail->CombInstrumentID,pInvestorPositionDetail->ExchangeID,pInvestorPositionDetail->CloseProfitByDate,
                        pInvestorPositionDetail->CloseProfitByTrade, pInvestorPositionDetail->PositionProfitByDate,pInvestorPositionDetail->PositionProfitByTrade,pInvestorPositionDetail->Margin,pInvestorPositionDetail->ExchMargin,
                        pInvestorPositionDetail->MarginRateByMoney, pInvestorPositionDetail->MarginRateByVolume,pInvestorPositionDetail->LastSettlementPrice,pInvestorPositionDetail->SettlementPrice,pInvestorPositionDetail->CloseVolume,
                        pInvestorPositionDetail->CloseAmount);
    }
    else
    {
        LOG_INFO("OnRspQryInvestorPositionDetail(请求查询投资者持仓明细响应) : pInvestorPositionDetail=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pInvestorPositionDetail,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}

///请求查询投资者持仓响应
void CPlatformCTPTrade::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInvestorPosition,
			pInvestorPosition,pInvestorPosition?sizeof(*pInvestorPosition):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pInvestorPosition)
    {
        LOG_INFO("OnRspQryInvestorPosition(请求查询投资者持仓响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
                        "\t\t\t InstrumentID=[%s],\t BrokerID=[%s],\t InvestorID=[%s],\t PosiDirection=[%d],\t HedgeFlag=[%d],\n"
                        "\t\t\t PositionDate=[%d],\t YdPosition=[%d],\t Position=[%d],\t LongFrozen=[%d],\t ShortFrozen=[%d],\n"
                        "\t\t\t LongFrozenAmount=[%g],\t  ShortFrozenAmount=[%g],\t OpenVolume=[%d],\t CloseVolume=[%d],\t OpenAmount=[%g],\n"
                        "\t\t\t CloseAmount=[%g],\t PositionCost=[%g],\t PreMargin=[%g],\t UseMargin=[%g],\t FrozenMargin=[%g],\n"
                        "\t\t\t FrozenCash=[%g],\t FrozenCommission=[%g],\t CashIn=[%g],\t Commission=[%g],CloseProfit=[%g],\n"
                        "\t\t\t PositionProfit=[%g],\t PreSettlementPrice=[%g],\t SettlementPrice=[%g],\t TradingDay=[%s],\t SettlementID=[%d],\n"
                        "\t\t\t OpenCost=[%g],\t ExchangeMargin=[%g],\t CombPosition=[%d],\t CombLongFrozen=[%d],\t CombShortFrozen=[%d],\n"
                        "\t\t\t CloseProfitByDate=[%g],\t CloseProfitByTrade=[%g],\t TodayPosition=[%d],\t MarginRateByMoney=[%g],\t MarginRateByVolume=[%g]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
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
    else
    {
        LOG_INFO("OnRspQryInvestorPosition(请求查询投资者持仓响应) : pInvestorPosition=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
                         pInvestorPosition,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }


}


///请求查询投资者组合持仓明细响应
void CPlatformCTPTrade::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);


	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryInvestorPositionCombDetail,
			pDetail,pDetail?sizeof(*pDetail):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pDetail)
    {
		LOG_INFO("OnRspQryInvestorPositionCombineDetail(请求查询投资者持仓组合合约明细响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
				 "\t\t\t TradingDay=[%s],\t OpenDate=[%s],\t ExchangeID=[%s],\t SettlementID=[%d],\t BrokerID=[%s], \n"
				 "\t\t\t InvestorID=[%s],\t ComTradeID=[%s],\t TradeID=[%s],\t InstrumentID=[%s],\t HedgeFlag=[%c], \n"
				 "\t\t\t Direction=[%c],\t TotalAmt=[%d],\t Margin=[%g],\t ExchMargin=[%g],\t MarginRateByMoney=[%g], \n"
				 "\t\t\t MarginRateByVolume=[%g],\t LegID=[%d],\t LegMultiple=[%d],\t CombInstrumentID=[%s]", 
				 pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
				 pDetail->TradingDay, pDetail->OpenDate, pDetail->ExchangeID, pDetail->SettlementID, pDetail->BrokerID, 
				 pDetail->InvestorID, pDetail->ComTradeID, pDetail->TradeID, pDetail->InstrumentID, pDetail->HedgeFlag, 
				 pDetail->Direction, pDetail->TotalAmt, pDetail->Margin, pDetail->ExchMargin, pDetail->MarginRateByMoney, 
				 pDetail->MarginRateByVolume, pDetail->LegID, pDetail->LegMultiple, pDetail->CombInstrumentID);
    }
    else
    {
        LOG_INFO("OnRspQryInvestorPositionCombineDetail(请求查询投资者持仓组合合约明细响应) : pDetail=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
			pDetail,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}



///请求查询客户通知响应
void CPlatformCTPTrade::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    CeasymutexGuard guard(m_mutex);

	if(bIsLast) m_pQryQueue->ClearbInQry();

    if(m_bPushCache) 
	{
		m_Cache.SaveDataTo2(CmdID_Trade_RspQryNotice,
			pNotice,pNotice?sizeof(*pNotice):0,
			pRspInfo,pRspInfo?sizeof(*pRspInfo):0,
			(unsigned int)nRequestID,(unsigned int)bIsLast);
	}

    if(pNotice)
    {
        LOG_INFO("OnRspQryNotice(请求查询客户通知响应) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d],\n"
				 "\t\t\t BrokerID=%s Content=%s SequenceLabel=%s",
				 pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID,bIsLast,
                 pNotice->BrokerID,pNotice->Content,pNotice->SequenceLabel);
    }
    else
    {
        LOG_INFO("OnRspQryNotice(请求查询客户通知响应) : pNotice=[%x],pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
			pNotice,pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",nRequestID,bIsLast);
    }
}


///错误应答
void CPlatformCTPTrade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    LOG_INFO("CPlatformCTPTrade::OnRspError,pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],RequestID=[%d],bIsLast=[%d]",
		pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", nRequestID, bIsLast);
}

///报单通知
void CPlatformCTPTrade::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_RtnOrder,pOrder,pOrder?sizeof(*pOrder):0);

    if(pOrder)
    {
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
    else 
    {
        LOG_INFO("OnRtnOrder(报单通知),pOrder is NULL");
    }
}

///成交通知
void CPlatformCTPTrade::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_RtnTrade,pTrade,pTrade?sizeof(*pTrade):0);

    if(pTrade)
    {
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
                        pTrade->SequenceNo,pTrade->TradingDay,pTrade->SettlementID,pTrade->BrokerOrderSeq,pTrade->TradeSource
                        );
    }
    else
    {
        LOG_INFO("OnRtnTrade(成交通知),pTrade is NULL");
    }
}

///报单录入错误回报
void CPlatformCTPTrade::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_ErrRtnOrderInsert,pInputOrder,pInputOrder?sizeof(*pInputOrder):0,pRspInfo,pRspInfo?sizeof(*pRspInfo):0);  

    if(pInputOrder)
    {
        LOG_INFO("OnErrRtnOrderInsert(报单录入错误回报) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
                        "\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
                        "\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
                        "\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
                        "\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"", 
                        pInputOrder->BrokerID, pInputOrder->InvestorID, pInputOrder->InstrumentID, pInputOrder->OrderRef,pInputOrder->UserID,
                        pInputOrder->OrderPriceType, pInputOrder->Direction, pInputOrder->CombOffsetFlag, pInputOrder->CombHedgeFlag,pInputOrder->LimitPrice,
                        pInputOrder->VolumeTotalOriginal, pInputOrder->TimeCondition, pInputOrder->GTDDate, pInputOrder->VolumeCondition,pInputOrder->MinVolume,
                        pInputOrder->ContingentCondition, pInputOrder->StopPrice, pInputOrder->ForceCloseReason, pInputOrder->IsAutoSuspend,pInputOrder->BusinessUnit,
                        pInputOrder->RequestID, pInputOrder->UserForceClose);
    }
    else
    {
        LOG_INFO("OnRspOrderInsert(报单录入错误回报),pInputOrder=[%x] pRspInfo=[%x]",pInputOrder,pRspInfo);
    }
}

///报单操作错误回报
void CPlatformCTPTrade::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_ErrRtnOrderAction,pOrderAction,pOrderAction?sizeof(*pOrderAction):0,pRspInfo,pRspInfo?sizeof(*pRspInfo):0);  

    if(pOrderAction)
    {
        LOG_INFO("OnErrRtnOrderAction(报单操作错误回报) : pRspInfo=[%x],ErrorID=[%d],ErrorMsg=[%s],\n"
                        "\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
                        "\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
                        "\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t ActionDate=[%s],\t ActionTime=[%s],\t TraderID=[%s],\n"
                        "\t\t\t InstallID=[%d],\t OrderLocalID=[%s],\t ActionLocalID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\n"
                        "\t\t\t BusinessUnit=[%s],\t OrderActionStatus=[%d],\t UserID=[%s],\t StatusMsg=[%s],\t InstrumentID=[%s]",
                        pRspInfo,pRspInfo?pRspInfo->ErrorID:0, pRspInfo?pRspInfo->ErrorMsg:"",
                        pOrderAction->BrokerID, pOrderAction->InvestorID, pOrderAction->OrderActionRef, pOrderAction->OrderRef,pOrderAction->RequestID,
                        pOrderAction->FrontID, pOrderAction->SessionID, pOrderAction->ExchangeID, pOrderAction->OrderSysID,pOrderAction->ActionFlag,
                        pOrderAction->LimitPrice, pOrderAction->VolumeChange, pOrderAction->ActionDate, pOrderAction->ActionTime,pOrderAction->TraderID,
                        pOrderAction->InstallID, pOrderAction->OrderLocalID, pOrderAction->ActionLocalID, pOrderAction->ParticipantID,pOrderAction->ClientID,
                        pOrderAction->BusinessUnit, pOrderAction->OrderActionStatus, pOrderAction->UserID, pOrderAction->StatusMsg,pOrderAction->InstrumentID
    			    );
    }
    else
    {
        LOG_INFO("OnErrRtnOrderAction(报单操作错误回报) : pOrderAction=[%x] pRspInfo=[%x]",pOrderAction,pRspInfo);
    }
 }

///合约交易状态通知
void CPlatformCTPTrade::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_RtnInstrumentStatus,pInstrumentStatus,pInstrumentStatus?sizeof(*pInstrumentStatus):0);  

    if(pInstrumentStatus)
    {
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
    else
    {
        LOG_INFO("OnRtnInstrumentStatus(合约交易状态通知),pInstrumentStatus is NULL");
    }
}

///交易通知
void CPlatformCTPTrade::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo)
{
    CeasymutexGuard guard(m_mutex);

    if(m_bPushCache) m_Cache.SaveDataTo2(CmdID_Trade_RtnTradingNotice,pTradingNoticeInfo,pTradingNoticeInfo?sizeof(*pTradingNoticeInfo):0);  

    if(pTradingNoticeInfo)
    {
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
    else
    {
        LOG_INFO("OnRtnTradingNotice(交易通知),pTradingNoticeInfo is NULL");
    }
}
