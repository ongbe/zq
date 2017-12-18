#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <set>
#include <vector>
#include <string>
#include <map>
using std::set;
using std::string;
using std::map;
using std::vector;

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#include "ReqOnRtnTrade.h"
#include "../../Miscellaneous/Tools/inc/FileOpr.h"
#include "../inc/Module-Misc2/GlobalConfigMgr.h"

#pragma warning(disable:4996)




#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("QryQueue", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif
#define REQQUEUE_DELETECURPACKAGE()\
do{\
    if(bCache)\
        m_pReqQueue->DeleteCurPackage();\
    else if(bCache2)\
        m_pReqQueue2->DeleteCurPackage();\
}while(0)

CQryQueue::CQryQueue(CPlatFormService& PlatFormService)
:    m_RefPlatFormService(PlatFormService)
    ,m_bStartQryRate(false)
{
    DWORD threadId;

    CReqOnRtnTrade::Init();

    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"QryQueue.log");

	if(!IS_MULTIACCOUNT_VERSION)
	{
		m_pReqQueue= new CPackageCache(20*1024*1024,3);
		m_pReqQueue2= new CPackageCache(20*1024*1024,3);
	}
	else
	{
		m_pReqQueue= new CPackageCache(10*1024*1024,3);
		m_pReqQueue2= new CPackageCache(10*1024*1024,3);
	}
    m_LastQryTime=0;
    m_bNeedExit=false;


    //在SendQryCmd()里要用到1M栈空间
    m_hThread=CreateThread(NULL,3*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&threadId);
    if(GlobalConfigManager::m_Test_bLogThreadID)
        CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCQryQueue::ThreadProc",threadId);
}
void CQryQueue::StopThread()
{
	DWORD			ExitCode;
    m_bNeedExit=true;
	if(m_hThread)
	{
		WaitForSingleObject((HANDLE)m_hThread,2000);
		if(GetExitCodeThread((HANDLE)m_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)m_hThread,0);
		CloseHandle(m_hThread);
		m_hThread=NULL;
	}
}
CQryQueue::~CQryQueue(void)
{
    StopThread();
    SAFE_DELETE(m_pReqQueue);
    SAFE_DELETE(m_pReqQueue2);
    LOG_INFO("CQryQueue::~CQryQueue(析构CQryQueue对象)");
    SAFE_DELETE(m_pWriteLog);

    CReqOnRtnTrade::Release();

}
//是否需要线程退出
bool CQryQueue::IsThreadNeedExit(void)
{
    return m_bNeedExit;
}



//是否到达查询时刻。ctp规定每秒只能有一次查询 
bool CQryQueue::IsQryTime(void)
{
    DWORD CurTickCount=GetTickCount();
    if(!m_RefPlatFormService.m_bInQry)
    {                                                       //没有在途查询
        if(CurTickCount>=m_LastQryTime && CurTickCount-m_LastQryTime > 1000 ||
            CurTickCount<m_LastQryTime && CurTickCount+((DWORD)0xffffffff-m_LastQryTime) > 1000)
            return true;
    }
    else
    {                                                       //有在途查询，超时为8s
        if(CurTickCount>=m_LastQryTime && CurTickCount-m_LastQryTime > 8000 ||
            CurTickCount<m_LastQryTime && CurTickCount+((DWORD)0xffffffff-m_LastQryTime) > 8000)
            return true;
    }
    return false;
}

//更新查询时刻
void CQryQueue::UpdateQryTime(void)
{
    m_LastQryTime = GetTickCount();
}


//请求查询经纪公司交易参数
void CQryQueue::ReqQryBrokerTradingParams(int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryBrokerTradingParams");

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqBrokerTradingParam,0,0,(DWORD)nRequestID,0,NULL);
}


//请求查询品种
int CQryQueue::ReqQryProduct(PlatformStru_QryProduct &QryProduct, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryProduct:ProductID=%s",QryProduct.Thost.ProductID);

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryProduct,0,0,(DWORD)nRequestID,sizeof(QryProduct),&QryProduct);

    return 0;
}

//请求查询合约
int CQryQueue::ReqQryInstrument(PlatformStru_QryInstrument &QryInstrument, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryInstrument:InstrumentID=%s",QryInstrument.Thost.InstrumentID);

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryInstrument,0,0,(DWORD)nRequestID,sizeof(QryInstrument),&QryInstrument);

    return 0;
}

//请求查询资金账户
int CQryQueue::ReqQryTradingAccount(PlatformStru_QryTradingAccount &QryTradingAccount, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryTradingAccount");

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryTradingAccount,0,0,(DWORD)nRequestID,sizeof(QryTradingAccount),&QryTradingAccount);
    return 0;
}

//请求查询成交
int CQryQueue::ReqQryTrade(PlatformStru_QryTrade &QryTrade, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryTrade");

    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryTrade,0,0,(DWORD)nRequestID,sizeof(QryTrade),&QryTrade);
    return 0;
}

//请求查询报单
int CQryQueue::ReqQryOrder(PlatformStru_QryOrder &QryOrder, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryOrder");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryOrder,0,0,(DWORD)nRequestID,sizeof(QryOrder),&QryOrder);
    return 0;
}

//请求查询投资者持仓明细
int CQryQueue::ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &QryInvestorPositionDetail, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryInvestorPositionDetail");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryInvestorPositionDetail,0,0,(DWORD)nRequestID,sizeof(QryInvestorPositionDetail),&QryInvestorPositionDetail);
    return 0;
}

//请求查询投资者持仓
int CQryQueue::ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &QryInvestorPosition, int nRequestID) 
{
    LOG_INFO("CQryQueue::ReqQryInvestorPosition");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryInvestorPosition,0,0,(DWORD)nRequestID,sizeof(QryInvestorPosition),&QryInvestorPosition);
    return 0;
}

//请求查询投资者结算结果
int CQryQueue::ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &QrySettlementInfo, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQrySettlementInfo");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQrySettlementInfo,0,0,(DWORD)nRequestID,sizeof(QrySettlementInfo),&QrySettlementInfo);
    return 0;
}

//请求查询保证金监管系统经纪公司资金账户密钥
int CQryQueue::ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryCFMMCTradingAccountKey");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryCFMMCTradingAccountKey,0,0,(DWORD)nRequestID,sizeof(QryCFMMCTradingAccountKey),&QryCFMMCTradingAccountKey);
    return 0;
}



//请求查询行情
int CQryQueue::ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &QryDepthMarketData, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryDepthMarketData");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryDepthMarketData,0,0,(DWORD)nRequestID,sizeof(QryDepthMarketData),&QryDepthMarketData);
    return 0;
}




///请求查询投资者持仓明细(组合)
int CQryQueue::ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &pQryInvestorPositionCombineDetail, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryInvestorPositionCombineDetail");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryInvestorPositionCombineDetail,0,0,(DWORD)nRequestID,sizeof(pQryInvestorPositionCombineDetail),&pQryInvestorPositionCombineDetail);
    return 0;
}

///请求查询客户通知
int CQryQueue::ReqQryNotice(PlatformStru_QryNotice &pQryNotice, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryNotice");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryNotice,0,0,(DWORD)nRequestID,sizeof(pQryNotice),&pQryNotice);
    return 0;
}

///请求查询结算信息确认
int CQryQueue::ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &pQrySettlementInfoConfirm, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQrySettlementInfoConfirm");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQrySettlementInfoConfirm,0,0,(DWORD)nRequestID,sizeof(pQrySettlementInfoConfirm),&pQrySettlementInfoConfirm);
    return 0;
}

///请求查询转帐银行
int CQryQueue::ReqQryTransferBank(PlatformStru_QryTransferBank &pQryTransferBank, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryTransferBank");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryTransferBank,0,0,(DWORD)nRequestID,sizeof(pQryTransferBank),&pQryTransferBank);
    return 0;
}

///请求查询转帐流水
int CQryQueue::ReqQryTransferSerial(PlatformStru_QryTransferSerial &pQryTransferSerial, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryTransferSerial");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryTransferSerial,0,0,(DWORD)nRequestID,sizeof(pQryTransferSerial),&pQryTransferSerial);
    return 0;
}

///请求查询签约银行
int CQryQueue::ReqQryContractBank(PlatformStru_QryContractBank &pQryContractBank, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryContractBank");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryContractBank,0,0,(DWORD)nRequestID,sizeof(pQryContractBank),&pQryContractBank);
    return 0;
}
    
///请求查询银期签约关系
int CQryQueue::ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryAccountregister");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryAccountregister,0,0,(DWORD)nRequestID,sizeof(pQryAccountregister),&pQryAccountregister);
    return 0;
}

///请求查询汇率
int CQryQueue::ReqQryExchangeRate(PlatformStru_QryExchangeRate &QryExchangeRate, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryExchangeRate");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryExchangeRate,0,0,(DWORD)nRequestID,sizeof(QryExchangeRate),&QryExchangeRate);
    return 0;
}
    
///请求查询预埋单
int CQryQueue::ReqQryParkedOrder(PlatformStru_QryParkedOrder &pQryParkedOrder, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryParkedOrder");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryParkedOrder,0,0,(DWORD)nRequestID,sizeof(pQryParkedOrder),&pQryParkedOrder);
    return 0;
}

///请求查询预埋撤单
int CQryQueue::ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &pQryParkedOrderAction, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryParkedOrderAction");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryParkedOrderAction,0,0,(DWORD)nRequestID,sizeof(pQryParkedOrderAction),&pQryParkedOrderAction);
    return 0;
}

///期货发起查询银行余额请求
int CQryQueue::ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &pReqQueryAccount, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQueryBankAccountMoneyByFuture");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQueryBankAccountMoneyByFuture,0,0,(DWORD)nRequestID,sizeof(pReqQueryAccount),&pReqQueryAccount);
    return 0;
}

//请求查询执行宣告
int CQryQueue::ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryExecOrder");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQryExecOrder,0,0,(DWORD)nRequestID,sizeof(QryExecOrder),&QryExecOrder);
    return 0;
}

//查询最大报单数量请求
int CQryQueue::ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &QueryMaxOrderVolume, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQueryMaxOrderVolume");
    if(m_pReqQueue)
        m_pReqQueue->SaveDataTo(QryID_ReqQueryMaxOrderVolume,0,0,(DWORD)nRequestID,sizeof(QueryMaxOrderVolume),&QueryMaxOrderVolume);
    return 0;
}



//请求查询合约手续费率
int CQryQueue::ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField &QryInstrumentCommissionRate, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryInstrumentCommissionRate : InstrumentID=%s",QryInstrumentCommissionRate.InstrumentID);
    if(m_pReqQueue2)
        m_pReqQueue2->SaveDataTo(QryID_ReqQryInstrumentCommissionRate,0,0,(DWORD)nRequestID,sizeof(QryInstrumentCommissionRate),&QryInstrumentCommissionRate);
    return 0;
}

//请求查询合约保证金率
int CQryQueue::ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField &QryInstrumentMarginRate, int nRequestID)
{
    LOG_INFO("CQryQueue::ReqQryInstrumentMarginRate : InstrumentID=%s",QryInstrumentMarginRate.InstrumentID);
    if(m_pReqQueue2)
        m_pReqQueue2->SaveDataTo(QryID_ReqQryInstrumentMarginRate,0,0,(DWORD)nRequestID,sizeof(QryInstrumentMarginRate),&QryInstrumentMarginRate);
    return 0;
}




bool CQryQueue::TradeStatusIsConnected(void)
{
	return m_RefPlatFormService.GetTradeStatus()==CTPCONNSTATUS_Connected;
}

//清除查询请求队列中指定类型的查询请求，用于TCP重连后的重查前的处理
//ReqCmdID:1:资金查询；2:成交查询；4:报单查询；8:持仓明细查询；16:持仓查询；32:组合持仓明细查询
//ReqCmdID可以或处理
void CQryQueue::ClearReqInQryQueue(int ReqCmdID)
{
    LOG_INFO("CQryQueue::ClearReqInQryQueue(%d)",ReqCmdID);
    if(!m_pReqQueue) return;
    if((ReqCmdID&1)==1) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryTradingAccount);
    if((ReqCmdID&2)==2) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryTrade);
    if((ReqCmdID&4)==4) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryOrder);
    if((ReqCmdID&8)==8) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryInvestorPositionDetail);
    if((ReqCmdID&16)==16) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryInvestorPosition);
    if((ReqCmdID&32)==32) m_pReqQueue->ClearCmdIDInPackageHead(QryID_ReqQryInvestorPositionCombineDetail);
}

//线程函数
DWORD CQryQueue::ThreadProc(void *arg)
{
    CQryQueue   *pQryQueue=(CQryQueue*)arg;

    while(!pQryQueue->IsThreadNeedExit())
    {
        if(!pQryQueue->TradeStatusIsConnected())
        {
            Sleep(200);
            continue;
        }
        else if( !pQryQueue->IsQryTime())
        {
            Sleep(50);
            continue;
        }
        else 
        {
            if(pQryQueue->SendQryCmd())
            {
                pQryQueue->UpdateQryTime();
                Sleep(1000);
            }
            else
            {
                Sleep(200);
            }
        }
        if(pQryQueue->IsCacheEmpty())
        {
		    CReqOnRtnTrade::CheckOnTime( pQryQueue->m_RefPlatFormService.m_rServiceProxy);
        }
    }
    return 0;
}




//检查ReqChache是否为空
bool CQryQueue::IsCacheEmpty(void)
{
    if(m_pReqQueue && !m_pReqQueue->IsEmpty()) return false;
    else return true;
}
bool CQryQueue::IsCache2Empty(void)
{
    if(m_pReqQueue2 && !m_pReqQueue2->IsEmpty()) return false;
    else return true;
}
//发送查询请求，返回值表示是否成功发送请求
//bTrySend返回是否尝试了发送
//ReqRlt表示如果进行了查询，查询返回值，0:代表成功；-1:网络连接失败；-2:未处理请求超过许可数；-3:每秒发送请求数超过许可数。
bool CQryQueue::SendQryCmd()
{
    char PkgDataBuf[1*1024*1024];              //线程栈空间为3M，这里用掉1M
    PackageHeadStru PackageHead;
    bool bHavePkg=false;

     
    //ReqRlt表示如果进行了查询，查询返回值，0:代表成功；-1:网络连接失败；-2:未处理请求超过许可数；-3:每秒发送请求数超过许可数；-999:未查询。
    int ReqRlt=-999;
    memset(&PackageHead,0,sizeof(PackageHead));

    //删除可能存在的无效包
    while(!IsCacheEmpty())
    {
        if(m_pReqQueue->GetCurPackageDataLen()>(int)sizeof(PkgDataBuf)) 
        {
            m_pReqQueue->DeleteCurPackage();
            continue;
        }
        if(!m_pReqQueue->PeekFrom(&PackageHead,PkgDataBuf,sizeof(PkgDataBuf))) break;
        if(!IsReqQryPackageInCacheValid(PackageHead,PkgDataBuf)) 
        {
            m_pReqQueue->DeleteCurPackage();
            continue;
        }
        break;
    }
    while(!IsCache2Empty())
    {
        if(m_pReqQueue2->GetCurPackageDataLen()>(int)sizeof(PkgDataBuf))
        {
            m_pReqQueue2->DeleteCurPackage();
            continue;
        }
        if(!m_pReqQueue2->PeekFrom(&PackageHead,PkgDataBuf,sizeof(PkgDataBuf))) break;
        if(!IsReqQryPackageInCacheValid(PackageHead,PkgDataBuf)) 
        {
            m_pReqQueue2->DeleteCurPackage();
            continue;
        }
        break;
    }

    bool bCache=!IsCacheEmpty();    //表示是否是发送Cache中的请求
    bool bCache2=!IsCache2Empty();  //表示是否是发送Cache2中的请求

    if(bCache2&&(bCache||!m_RefPlatFormService.m_rServiceProxy.m_bInitQrySucc))
        bCache2=false;              //如果正在初始化查询，或者Cache中有请求包，就不要查费率了。等初始化查询结束后再查。

    if(bCache||bCache2)
    {
        //发送队列里有待发送的查询请求
        (bCache?m_pReqQueue:m_pReqQueue2)->PeekFrom(&PackageHead,PkgDataBuf,sizeof(PkgDataBuf));
        bHavePkg=true;
    }
    else if(m_RefPlatFormService.m_rServiceProxy.m_bInitQrySucc&&m_RefPlatFormService.IsNeedCheckCommissionRateAndMarginRate())
    {                          
        //发送队列里没有待发送查询请求，则执行手续费率和保证金率的查询
        if(!m_bStartQryRate)
        {
            //当m_bInitQrySucc由false变为true时，开始查询费率。
            m_bStartQryRate=true;

            //查询费率的次数。头4次，用InstrumentID为空的方式查询手续费率、保证金率、期权手续费率、期权交易成本，以得到有持仓的费率数据
            m_QryRateCount=0;                 

            //确定查询顺序。将要查询的合约按顺序放到m_InstrumentIDsNeedQry中
            m_InstrumentIDsNeedQry.clear();
            m_InstrumentIDsNeedQry2.clear();

            //1. 首先用InstrumentID为空查4次，得到有持仓的合约的费率
            //2. RunMode==1/2: 先查询期货合约费率，再查询期权合约费率
            //   RunMode=3: 先查询标的物期货合约费率，再查询取期权费率，最后查非标的物期货合约费率
            //3. 如果某个合约查询无费率，将同品种的其它合约移到查询队列的后面去，最后再查。

            set<string> AllInstruments;
            vector<string> AllFutureInstruments,AllOptionInstruments;
            set<string> UnderlyingInstruments;

            m_RefPlatFormService.GetInstrumentList(AllInstruments);
            if(GlobalConfigManager::m_RunMode==2||GlobalConfigManager::m_RunMode==3)
                m_RefPlatFormService.GetAllOptionUnderlyingInstrumentIDs(UnderlyingInstruments);

            for(set<string>::iterator it_InstrumentID=AllInstruments.begin();it_InstrumentID!=AllInstruments.end();it_InstrumentID++)
            {
                if(it_InstrumentID->empty()) continue;
                char ProductClass=m_RefPlatFormService.GetProductClassType(*it_InstrumentID);
                if(ProductClass==THOST_FTDC_PC_Futures) AllFutureInstruments.push_back(*it_InstrumentID);
                else if(ProductClass==THOST_FTDC_PC_Options||ProductClass==THOST_FTDC_PC_SpotOption) AllOptionInstruments.push_back(*it_InstrumentID);
            }

            //qrystep: 查询步骤，0/1/2/3对应期权标的物期货合约(RunMode3)/期权合约(RunMode3)/其它期货合约(RunMode123)/期权合约(RunMode12)
            for(int qrystep=0;qrystep<4&&!bHavePkg;qrystep++)
            {
                //期权标的物期货合约
                if(qrystep==0&&GlobalConfigManager::m_RunMode==3)
                {
                    for(vector<string>::iterator it=AllFutureInstruments.begin();it!=AllFutureInstruments.end();)
                    {
                        if(UnderlyingInstruments.find(*it)!=UnderlyingInstruments.end())
                        {
                            m_InstrumentIDsNeedQry.push_back(*it);
                            it=AllFutureInstruments.erase(it);
                        }
                        else it++;
                    }
                }

                //期权合约
                if(qrystep==1&&GlobalConfigManager::m_RunMode==3||qrystep==3&&(GlobalConfigManager::m_RunMode==1||GlobalConfigManager::m_RunMode==2))
                {
                    m_InstrumentIDsNeedQry.insert(m_InstrumentIDsNeedQry.end(),AllOptionInstruments.begin(),AllOptionInstruments.end());
                    AllOptionInstruments.clear();
                }

                //期货合约
                if(qrystep==2)
                {
                    m_InstrumentIDsNeedQry.insert(m_InstrumentIDsNeedQry.end(),AllFutureInstruments.begin(),AllFutureInstruments.end());
                    AllFutureInstruments.clear();
                }
            }
        }


        if(m_QryRateCount==0||m_QryRateCount==1)
        {
            //以InstrumentID为空的方式查持仓的手续费率/期权手续费率
            CThostFtdcQryInstrumentCommissionRateField& ReqData=*((CThostFtdcQryInstrumentCommissionRateField*)PkgDataBuf);
            memset(&ReqData,0,sizeof(ReqData));
            strncpy(ReqData.BrokerID,m_RefPlatFormService.GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
            strncpy(ReqData.InvestorID,m_RefPlatFormService.GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;

            memset(&PackageHead,0,sizeof(PackageHead));
            PackageHead.CmdID=QryID_ReqQryInstrumentCommissionRate;
            bHavePkg=true;
        }
        else if(m_QryRateCount==2||m_QryRateCount==3)
        {
            //以InstrumentID为空的方式查持仓的保证金率/期权交易成本
            CThostFtdcQryInstrumentMarginRateField& ReqData=*((CThostFtdcQryInstrumentMarginRateField*)PkgDataBuf);
            memset(&ReqData,0,sizeof(ReqData));
            strncpy(ReqData.BrokerID,m_RefPlatFormService.GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
            strncpy(ReqData.InvestorID,m_RefPlatFormService.GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
            ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

            memset(&PackageHead,0,sizeof(PackageHead));
            PackageHead.CmdID=QryID_ReqQryInstrumentMarginRate;
            bHavePkg=true;
        }
        else
        {
            map<string,time_t>::iterator it_GetTime;
            map<string,int>::iterator it_GetCount;
            time_t curtime=time(NULL);

            //先查m_InstrumentIDsNeedQry里的合约。如果未查到，放到m_InstrumentIDsNeedQry2里。
            for(int step=0;step<2&&!bHavePkg;step++)
            {
                vector<string>& vecInstrumentIDs = step==0 ? m_InstrumentIDsNeedQry : m_InstrumentIDsNeedQry2;
                while(!vecInstrumentIDs.empty())
                {
                    //总是对队列的第一个合约进行操作

                    string strInstrumentID=vecInstrumentIDs[0];

                    //如果没有手续费率，查之
                    map<string,int>::iterator it_GetCommissionRateCount=m_QryCommissionRateCount.find(strInstrumentID);
                    bool bHaveCommissionRate=m_RefPlatFormService.IsCommissionRateExist(strInstrumentID);
                    if(!bHaveCommissionRate&&(it_GetCommissionRateCount==m_QryCommissionRateCount.end()||it_GetCommissionRateCount->second==0))
                    {
                        CThostFtdcQryInstrumentCommissionRateField& ReqData=*((CThostFtdcQryInstrumentCommissionRateField*)PkgDataBuf);
                        memset(&ReqData,0,sizeof(ReqData));
                        strncpy(ReqData.BrokerID,m_RefPlatFormService.GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
                        strncpy(ReqData.InvestorID,m_RefPlatFormService.GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;
                        strncpy(ReqData.InstrumentID,strInstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);

                        memset(&PackageHead,0,sizeof(PackageHead));
                        PackageHead.CmdID=QryID_ReqQryInstrumentCommissionRate;
                        bHavePkg=true;
                        break;
                    }

                    //如果没有保证金率，查之
                    map<string,int>::iterator it_GetMarginCount=m_QryMarginRateCount.find(strInstrumentID);
                    bool bHaveMarginRate=m_RefPlatFormService.IsMarginRateExist(strInstrumentID);
                    if(!bHaveMarginRate&&(it_GetMarginCount==m_QryMarginRateCount.end()||it_GetMarginCount->second==0))
                    {
                        CThostFtdcQryInstrumentMarginRateField& ReqData=*((CThostFtdcQryInstrumentMarginRateField*)PkgDataBuf);
                        memset(&ReqData,0,sizeof(ReqData));
                        strncpy(ReqData.BrokerID,m_RefPlatFormService.GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
                        strncpy(ReqData.InvestorID,m_RefPlatFormService.GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
                        strncpy(ReqData.InstrumentID,strInstrumentID.c_str(),sizeof(ReqData.InstrumentID)-1);
                        ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

                        memset(&PackageHead,0,sizeof(PackageHead));
                        PackageHead.CmdID=QryID_ReqQryInstrumentMarginRate;
                        bHavePkg=true;
                        break;
                    }

                    //如果step==0，且查不到手续费率或保证金率，将同品种合约都移到m_InstrumentIDsNeedQry2里
                    if(step==0&&(!bHaveCommissionRate||!bHaveMarginRate))
                    {
                        string strProductID=m_RefPlatFormService.GetProductID(strInstrumentID);
                        for(vector<string>::iterator it=vecInstrumentIDs.begin();it!=vecInstrumentIDs.end();)
                        {
                            if(m_RefPlatFormService.GetProductID(*it)==strProductID)
                            {
                                m_InstrumentIDsNeedQry2.push_back(*it);
                                it=vecInstrumentIDs.erase(it);
                            }
                            else it++;
                        }
                        if(!bHaveCommissionRate) m_QryCommissionRateCount[strInstrumentID]=0;
                        if(!bHaveMarginRate) m_QryMarginRateCount[strInstrumentID]=0;
                        continue;
                    }

                    //剩下的情况就是这个合约查完了，或者是m_InstrumentIDsNeedQry2里查不到的，从队列里删除
                    vecInstrumentIDs.erase(vecInstrumentIDs.begin());
                }
            }
        }

        if(bHavePkg) m_QryRateCount++;
        if(!bHavePkg)
        {
                                                        //所有合约的费率都已经有了，清除检查的标志
            m_RefPlatFormService.SetNeedCheckCommissionRateAndMarginRate(false);
        }
    }

    if(bHavePkg)
    {
        //提前将m_RefPlatFormService.m_bInQry置为true，避免服务端处理太快，已经得到结果后，再执行m_RefPlatFormService.m_bInQry=true，从而导致超时的情况
        m_RefPlatFormService.m_bInQry=true;

        switch(PackageHead.CmdID)
        {
            case QryID_ReqBrokerTradingParam:               //查询经纪公司交易参数
            {
                CThostFtdcQryBrokerTradingParamsField QryInfo={0};
                const PlatformStru_LoginInfo& LoginInfo=m_RefPlatFormService.GetTradingLoginInfo();
                strncpy(QryInfo.BrokerID,LoginInfo.BrokerID,sizeof(QryInfo.BrokerID)-1);
                strncpy(QryInfo.InvestorID,LoginInfo.UserID,sizeof(QryInfo.InvestorID)-1);

                ReqRlt=m_RefPlatFormService.ReqQryBrokerTradingParams(&QryInfo,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询经纪公司交易参数)ReqQryBrokerTradingParams : BrokerID=%s InvestorID=%s Rlt=%d",QryInfo.BrokerID,QryInfo.InvestorID,ReqRlt);
                break;
            }

            case QryID_ReqQryProduct:                       //查询品种信息
                ReqRlt=m_RefPlatFormService.ReqQryProduct((PlatformStru_QryProduct*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询品种信息)ReqQryProduct : ProductID=%s Rlt=%d",((PlatformStru_QryProduct*)PkgDataBuf)->Thost.ProductID,ReqRlt);
                break;

            case QryID_ReqQryInstrument:                    //查询合约信息
                ReqRlt=m_RefPlatFormService.ReqQryInstrument((PlatformStru_QryInstrument*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询合约信息)ReqQryInstrument : InstrumentID=%s Rlt=%d",((PlatformStru_QryInstrument*)PkgDataBuf)->Thost.InstrumentID,ReqRlt);
                break;

            case QryID_ReqQryTradingAccount:                //查询资金
                ReqRlt=m_RefPlatFormService.ReqQryTradingAccount((PlatformStru_QryTradingAccount*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询资金信息)ReqQryTradingAccount Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTrade:                         //查询成交
                ReqRlt=m_RefPlatFormService.ReqQryTrade((PlatformStru_QryTrade*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询成交信息)ReqQryTrade Rlt=%d ",ReqRlt);
                break;

            case QryID_ReqQryOrder:                         //查询报单
                ReqRlt=m_RefPlatFormService.ReqQryOrder((PlatformStru_QryOrder*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询报单信息)QryID_ReqQryOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPositionDetail:        //查询持仓明细
                ReqRlt=m_RefPlatFormService.ReqQryInvestorPositionDetail((PlatformStru_QryInvestorPositionDetail*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询持仓明细)ReqQryInvestorPositionDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPosition:              //查询持仓
                ReqRlt=m_RefPlatFormService.ReqQryInvestorPosition((PlatformStru_QryInvestorPosition*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询持仓汇总)ReqQryInvestorPosition Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfo:                //查询投资者结算结果
                ReqRlt=m_RefPlatFormService.ReqQrySettlementInfo((PlatformStru_QrySettlementInfo*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询投资者结算结果)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryCFMMCTradingAccountKey:        //请求查询保证金监管系统经纪公司资金账户密钥
                ReqRlt=m_RefPlatFormService.ReqQryCFMMCTradingAccountKey((PlatformStru_QryCFMMCTradingAccountKey*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询保证金监管系统经纪公司资金账户密钥)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryDepthMarketData:               //查询行情结果
                ReqRlt=m_RefPlatFormService.ReqQryDepthMarketData((PlatformStru_QryDepthMarketData*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询行情)ReqQryDepthMarketData Rlt=%d",ReqRlt);
                break;




            case QryID_ReqQryInvestorPositionCombineDetail: //请求查询投资者持仓明细(组合)
                ReqRlt=m_RefPlatFormService.ReqQryInvestorPositionCombineDetail((PlatformStru_QryInvestorPositionCombineDetail*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询投资者持仓明细(组合))ReqQryInvestorPositionCombineDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryNotice:                        //请求查询客户通知
                ReqRlt=m_RefPlatFormService.ReqQryNotice((PlatformStru_QryNotice*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询客户通知)ReqQryNotice Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfoConfirm:         //请求查询结算信息确认
                ReqRlt=m_RefPlatFormService.ReqQrySettlementInfoConfirm((PlatformStru_QrySettlementInfoConfirm*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询结算信息确认)ReqQrySettlementInfoConfirm Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferBank:                  //请求查询转帐银行
                ReqRlt=m_RefPlatFormService.ReqQryTransferBank((PlatformStru_QryTransferBank*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询转帐银行)ReqQryTransferBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferSerial:                //请求查询转帐流水
                ReqRlt=m_RefPlatFormService.ReqQryTransferSerial((PlatformStru_QryTransferSerial*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询转帐流水)ReqQryTransferSerial Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryContractBank:                  //请求查询签约银行
                ReqRlt=m_RefPlatFormService.ReqQryContractBank((PlatformStru_QryContractBank*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询签约银行)ReqQryContractBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryAccountregister:               //请求查询银期签约关系
                ReqRlt=m_RefPlatFormService.ReqQryAccountregister((PlatformStru_QryAccountRegister*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询银期签约关系)ReqQryAccountregister Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryExchangeRate:                  //请求查询汇率
                ReqRlt=m_RefPlatFormService.ReqQryExchangeRate((PlatformStru_QryExchangeRate*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询汇率)ReqQryExchangeRate Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrder:                   //请求查询预埋单
                ReqRlt=m_RefPlatFormService.ReqQryParkedOrder((PlatformStru_QryParkedOrder*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询预埋单)ReqQryParkedOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrderAction:             //请求查询预埋撤单
                ReqRlt=m_RefPlatFormService.ReqQryParkedOrderAction((PlatformStru_QryParkedOrderAction*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询预埋撤单)ReqQryParkedOrderAction Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQueryBankAccountMoneyByFuture:    //查询银行余额
                ReqRlt=m_RefPlatFormService.ReqQueryBankAccountMoneyByFuture((PlatformStru_ReqQueryAccount*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询银行余额)ReqQueryBankAccountMoneyByFuture Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryExecOrder:                     //查询执行宣告
                ReqRlt=m_RefPlatFormService.ReqQryExecOrder(*(PlatformStru_QryExecOrderField*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询执行宣告)ReqQryExecOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQueryMaxOrderVolume:              //查询最大报单数量
                ReqRlt=m_RefPlatFormService.ReqQueryMaxOrderVolume((PlatformStru_QueryMaxOrderVolume*)PkgDataBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询最大报单数量)ReqQueryMaxOrderVolume Rlt=%d",ReqRlt);
                break;



            case QryID_ReqQryInstrumentCommissionRate:      //查询合约手续费率. 期货，直接查手续费率；期权，查期权手续费率；InstrumentID为空，m_QryRateCount==1查手续费率; 2查期权手续费率
            {
                CThostFtdcQryInstrumentCommissionRateField& QryCommRate(*((CThostFtdcQryInstrumentCommissionRateField*)PkgDataBuf));
                string strInstrumentID(QryCommRate.InstrumentID);
                m_RefPlatFormService.SetReqCommissionRateInstrumentID(strInstrumentID);
                char ProductClass;
                bool bInstrumentIDEmpty=strInstrumentID.empty();
                if(!bInstrumentIDEmpty) ProductClass=m_RefPlatFormService.GetProductClassType(strInstrumentID);
                if(bInstrumentIDEmpty&&m_QryRateCount==1||!bInstrumentIDEmpty&&ProductClass==THOST_FTDC_PC_Futures)
                {
                    ReqRlt=m_RefPlatFormService.ReqQryInstrumentCommissionRate(&QryCommRate,(int)PackageHead.dwSeq);
                }
#ifdef CTP060300
                else if(bInstrumentIDEmpty&&m_QryRateCount==2||!bInstrumentIDEmpty&&(ProductClass==THOST_FTDC_PC_Options||ProductClass==THOST_FTDC_PC_SpotOption))
                {
                    CThostFtdcQryOptionInstrCommRateField QryOptionCommRate={0};
                    strncpy(QryOptionCommRate.BrokerID,QryCommRate.BrokerID,sizeof(QryOptionCommRate.BrokerID)-1);
                    strncpy(QryOptionCommRate.InvestorID,QryCommRate.InvestorID,sizeof(QryOptionCommRate.InvestorID)-1);;
                    strncpy(QryOptionCommRate.InstrumentID,QryCommRate.InstrumentID,sizeof(QryOptionCommRate.InstrumentID)-1);

                    ReqRlt=m_RefPlatFormService.ReqQryOptionInstrCommRate(&QryOptionCommRate,(int)PackageHead.dwSeq);
                }
#endif

                LOG_INFO("CQryQueue::SendQryCmd : (查询期货/期权合约手续费率)ReqQryInstrumentCommissionRate，InstrumentID=%s ProductClass=%d Rlt=%d",((CThostFtdcQryInstrumentCommissionRateField*)PkgDataBuf)->InstrumentID,ProductClass,ReqRlt);
                if(ReqRlt==0)
                {
                    m_QryCommissionRateTime[strInstrumentID]=time(NULL);
                    map<string,int>::iterator it_GetCount=m_QryCommissionRateCount.find(strInstrumentID);
                    if(it_GetCount==m_QryCommissionRateCount.end())
                        m_QryCommissionRateCount[strInstrumentID]=1;
                    else
                        it_GetCount->second=it_GetCount->second+1;
                }
                break;
            }

            case QryID_ReqQryInstrumentMarginRate:          //请求查询合约保证金率。期货，直接查保证金率；期权，查交易成本；InstrumentID为空，m_QryRateCount==3查保证金率; 4查期权交易成本
            {
                CThostFtdcQryInstrumentMarginRateField& QryMarginRate(*((CThostFtdcQryInstrumentMarginRateField*)PkgDataBuf));
                string strInstrumentID(QryMarginRate.InstrumentID);
                char ProductClass;
                bool bInstrumentIDEmpty=strInstrumentID.empty();
                if(!bInstrumentIDEmpty) ProductClass=m_RefPlatFormService.GetProductClassType(strInstrumentID);
                if(bInstrumentIDEmpty&&m_QryRateCount==3||!bInstrumentIDEmpty&&ProductClass==THOST_FTDC_PC_Futures)
                {
                    ReqRlt=m_RefPlatFormService.ReqQryInstrumentMarginRate(&QryMarginRate,(int)PackageHead.dwSeq);
                }
#ifdef CTP060300
                else if(bInstrumentIDEmpty&&m_QryRateCount==4||!bInstrumentIDEmpty&&(ProductClass==THOST_FTDC_PC_Options||ProductClass==THOST_FTDC_PC_SpotOption))
                {
                    CThostFtdcQryOptionInstrTradeCostField QryOptionTradeCost={0};
                    strncpy(QryOptionTradeCost.BrokerID,QryMarginRate.BrokerID,sizeof(QryOptionTradeCost.BrokerID)-1);
                    strncpy(QryOptionTradeCost.InvestorID,QryMarginRate.InvestorID,sizeof(QryOptionTradeCost.InvestorID)-1);;
                    strncpy(QryOptionTradeCost.InstrumentID,QryMarginRate.InstrumentID,sizeof(QryOptionTradeCost.InstrumentID)-1);
                    QryOptionTradeCost.HedgeFlag=QryMarginRate.HedgeFlag;
                    QryOptionTradeCost.InputPrice=0;
                    QryOptionTradeCost.UnderlyingPrice=0;

                    ReqRlt=m_RefPlatFormService.ReqQryOptionInstrTradeCost(&QryOptionTradeCost,(int)PackageHead.dwSeq);
                }
#endif

                LOG_INFO("CQryQueue::SendQryCmd : (查询期货合约保证金率/期权合约交易成本)ReqQryInstrumentMarginRate，InstrumentID=%s ProductClass=%d Rlt=%d",((CThostFtdcQryInstrumentMarginRateField*)PkgDataBuf)->InstrumentID,ProductClass,ReqRlt);
                if(ReqRlt==0)
                {
                    m_QryMarginRateTime[strInstrumentID]=time(NULL);
                    map<string,int>::iterator it_count=m_QryMarginRateCount.find(strInstrumentID);
                    if(it_count==m_QryMarginRateCount.end())
                        m_QryMarginRateCount[strInstrumentID]=1;
                    else
                        it_count->second=it_count->second+1;
                }
                break;
            }
            default:
                ReqRlt=-1;
                break;
        }

        if(ReqRlt==0) 
        { 
            //成功发出查询请求
            (bCache?m_pReqQueue:m_pReqQueue2)->DeleteCurPackage();
            return true; 
        }                                                               
    }

    return false;
}

//判断Cache中的请求查询的数据包是否正确
//检查数据长度，以及查费率时，相关费率是否已经有了
bool CQryQueue::IsReqQryPackageInCacheValid(const PackageHeadStru& PackageHead,const void* pPackageData)
{
    switch(PackageHead.CmdID)
    {
        case QryID_ReqBrokerTradingParam:               return PackageHead.DataLen==0;
        case QryID_ReqQryProduct:                       return PackageHead.DataLen==(int)sizeof(PlatformStru_QryProduct);
        case QryID_ReqQryInstrument:                    return PackageHead.DataLen==(int)sizeof(PlatformStru_QryInstrument);
        case QryID_ReqQryTradingAccount:                return PackageHead.DataLen==(int)sizeof(PlatformStru_QryTradingAccount);
        case QryID_ReqQryTrade:                         return PackageHead.DataLen==(int)sizeof(PlatformStru_QryTrade);
        case QryID_ReqQryOrder:                         return PackageHead.DataLen==(int)sizeof(PlatformStru_QryOrder);
        case QryID_ReqQryInvestorPositionDetail:        return PackageHead.DataLen==(int)sizeof(PlatformStru_QryInvestorPositionDetail);
        case QryID_ReqQryInvestorPosition:              return PackageHead.DataLen==(int)sizeof(PlatformStru_QryInvestorPosition);
        case QryID_ReqQrySettlementInfo:                return PackageHead.DataLen==(int)sizeof(PlatformStru_QrySettlementInfo);
        case QryID_ReqQryCFMMCTradingAccountKey:        return PackageHead.DataLen==(int)sizeof(PlatformStru_QryCFMMCTradingAccountKey);
        case QryID_ReqQryDepthMarketData:               return PackageHead.DataLen==(int)sizeof(PlatformStru_QryDepthMarketData);
        case QryID_ReqQryInvestorPositionCombineDetail: return PackageHead.DataLen==(int)sizeof(PlatformStru_QryInvestorPositionCombineDetail);
        case QryID_ReqQryNotice:                        return PackageHead.DataLen==(int)sizeof(PlatformStru_QryNotice);
        case QryID_ReqQrySettlementInfoConfirm:         return PackageHead.DataLen==(int)sizeof(PlatformStru_QrySettlementInfoConfirm);
        case QryID_ReqQryTransferBank:                  return PackageHead.DataLen==(int)sizeof(PlatformStru_QryTransferBank);
        case QryID_ReqQryTransferSerial:                return PackageHead.DataLen==(int)sizeof(PlatformStru_QryTransferSerial);
        case QryID_ReqQryContractBank:                  return PackageHead.DataLen==(int)sizeof(PlatformStru_QryContractBank);
        case QryID_ReqQryAccountregister:               return PackageHead.DataLen==(int)sizeof(PlatformStru_QryAccountRegister);
        case QryID_ReqQryExchangeRate:                  return PackageHead.DataLen==(int)sizeof(PlatformStru_QryExchangeRate);
        case QryID_ReqQryParkedOrder:                   return PackageHead.DataLen==(int)sizeof(PlatformStru_QryParkedOrder);
        case QryID_ReqQryParkedOrderAction:             return PackageHead.DataLen==(int)sizeof(PlatformStru_QryParkedOrderAction);
        case QryID_ReqQueryBankAccountMoneyByFuture:    return PackageHead.DataLen==(int)sizeof(PlatformStru_ReqQueryAccount);
        case QryID_ReqQryExecOrder:                     return PackageHead.DataLen==(int)sizeof(PlatformStru_QryExecOrderField);
        case QryID_ReqQueryMaxOrderVolume:              return PackageHead.DataLen==(int)sizeof(PlatformStru_QueryMaxOrderVolume);
        case QryID_ReqQryInstrumentCommissionRate:      //查询合约手续费率
        {
            if(PackageHead.DataLen!=(int)sizeof(CThostFtdcQryInstrumentCommissionRateField))  return false;
            string strInstrumentID(((CThostFtdcQryInstrumentCommissionRateField*)pPackageData)->InstrumentID);
            if(strInstrumentID.empty()) return false;

            ///获取合约手续费率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
            PlatformStru_InstrumentCommissionRate CommissionRate;
		    int ErrNo=m_RefPlatFormService.GetCommissionRate(strInstrumentID, CommissionRate);
            if(ErrNo==0||ErrNo==CustomErrorID) return false;
            //如果查过了，这里就不查了
            if(m_QryCommissionRateCount.find(strInstrumentID)!=m_QryCommissionRateCount.end()) return false;
            //其它情况下，可以查
            return true;
        }
        case QryID_ReqQryInstrumentMarginRate:          //请求查询合约保证金率
        {
            if(PackageHead.DataLen!=(int)sizeof(CThostFtdcQryInstrumentMarginRateField))  return false;
            string strInstrumentID(((CThostFtdcQryInstrumentMarginRateField*)pPackageData)->InstrumentID);
            if(strInstrumentID.empty()) return false;

            ///获取合约保证金率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
            PlatformStru_InstrumentMarginRate MarginRate;
		    int ErrNo=m_RefPlatFormService.GetMarginRate(strInstrumentID, MarginRate);
            if(ErrNo==0||ErrNo==CustomErrorID) return false;
            //如果查过了，这里就不查了
            if(m_QryMarginRateTime.find(strInstrumentID)!=m_QryMarginRateTime.end()) return false;
            //其它情况下，可以查
            return true;
        }
    }
    return false;
}
