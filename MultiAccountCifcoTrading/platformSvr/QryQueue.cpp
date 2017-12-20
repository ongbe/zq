#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "ServiceProxy.hpp"
#include "PlatFormService.hpp"
#include "QryQueue.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "ReqOnRtnTrade.h"

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

CQryQueue::CQryQueue()
{
    DWORD threadId;


    m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"QryQueue.log");

    m_pReqQueue= new CPackageCache(20*1024*1024,3);
    m_pReqQueue2= new CPackageCache(20*1024*1024,3);
    m_LastQryTime=0;
    m_bNeedExit=false;
	m_bDoingInitQry=false;                        //正在进行初始化查询，此时不能查费率
	m_bInQry=false;
    m_pPlatFormService=NULL;

    m_hThread=CreateThread(NULL,2*1024*1024,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&threadId);
}
void CQryQueue::StopThread()
{
	DWORD			ExitCode;
    m_bNeedExit=true;
	if(m_hThread)
	{
		WaitForSingleObject((HANDLE)m_hThread,300);
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
    if(!m_bInQry)
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
	if(m_pPlatFormService==NULL) return false;
	return m_pPlatFormService->GetTradeStatus()==CPlatFormParam::CONNSTATUS_Connected;
}

//线程函数
DWORD CQryQueue::ThreadProc(void *arg)
{
    CQryQueue   *pQryQueue=(CQryQueue*)arg;
    int ReqRlt;

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
            if(pQryQueue->SendQryCmd(ReqRlt))
            {
                pQryQueue->UpdateQryTime();
                Sleep(1000);
            }
            else
            {
                if(pQryQueue->IsCacheEmpty()&&pQryQueue->IsCache2Empty())
                    Sleep(100);
                else
                {
                    if(ReqRlt<0)
                        Sleep(200);
                }
            }
        }
        if(pQryQueue->IsCacheEmpty())
        {
			if ( pQryQueue->m_pPlatFormService )
			{
				pQryQueue->m_pPlatFormService->CheckOnTime();
			}
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
//ReqRlt表示如果进行了查询，查询返回值，<0表示查询失败
bool CQryQueue::SendQryCmd(int& ReqRlt)
{
    PackageHeadStru PackageHead;
    bool bDataErr;
    bool bDataIngore=false;

     
    memset(&PackageHead,0,sizeof(PackageHead));
    ReqRlt=0;
    bool bCache=!IsCacheEmpty();
    bool bCache2=!IsCache2Empty();

    if(m_bDoingInitQry) 
        bCache2=false;              //如果正在初始化查询，就不要查费率了。等初始化查询结束后再查。

    if(bCache||bCache2)
    {
        if(bCache)
        {
            if(m_pReqQueue->GetCurPackageDataLen()>sizeof(m_SendQryCmdBuf))
            {
                m_pReqQueue->DeleteCurPackage();
                return false;
            }
            m_pReqQueue->PeekFrom(&PackageHead,m_SendQryCmdBuf,sizeof(m_SendQryCmdBuf));
        }
        else if(bCache2)
        {
            if(m_pReqQueue2->GetCurPackageDataLen()>sizeof(m_SendQryCmdBuf))
            {
                m_pReqQueue2->DeleteCurPackage();
                return false;
            }
            m_pReqQueue2->PeekFrom(&PackageHead,m_SendQryCmdBuf,sizeof(m_SendQryCmdBuf));
        }


//        ReqRlt=-999;
        bDataErr=false;
        bDataIngore=false;
        switch(PackageHead.CmdID)
        {
            case QryID_ReqQryInstrument:                    //查询合约信息
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInstrument))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryInstrument((PlatformStru_QryInstrument*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询合约信息)ReqQryInstrument : InstrumentID=%s Rlt=%d",((PlatformStru_QryInstrument*)m_SendQryCmdBuf)->Thost.InstrumentID,ReqRlt);
                break;

            case QryID_ReqQryTradingAccount:                //查询资金
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTradingAccount))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryTradingAccount((PlatformStru_QryTradingAccount*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询资金信息)ReqQryTradingAccount Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTrade:                         //查询成交
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTrade))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryTrade((PlatformStru_QryTrade*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询成交信息)ReqQryTrade Rlt=%d ",ReqRlt);
                break;

            case QryID_ReqQryOrder:                         //查询报单
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryOrder))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryOrder((PlatformStru_QryOrder*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询报单信息)QryID_ReqQryOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPositionDetail:        //查询持仓明细
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPositionDetail))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryInvestorPositionDetail((PlatformStru_QryInvestorPositionDetail*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询持仓明细)ReqQryInvestorPositionDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryInvestorPosition:              //查询持仓汇总
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPosition))  bDataErr=true;
                else 
                    ReqRlt=m_pPlatFormService->ReqQryInvestorPosition((PlatformStru_QryInvestorPosition*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询持仓汇总)ReqQryInvestorPosition Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfo:              //查询投资者结算结果
                if(PackageHead.DataLen!=sizeof(PlatformStru_QrySettlementInfo))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQrySettlementInfo((PlatformStru_QrySettlementInfo*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询投资者结算结果)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryCFMMCTradingAccountKey:        //请求查询保证金监管系统经纪公司资金账户密钥
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryCFMMCTradingAccountKey))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryCFMMCTradingAccountKey((PlatformStru_QryCFMMCTradingAccountKey*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询保证金监管系统经纪公司资金账户密钥)ReqQrySettlementInfo Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryDepthMarketData:              //查询行情结果
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryDepthMarketData))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryDepthMarketData((PlatformStru_QryDepthMarketData*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询行情)ReqQryDepthMarketData Rlt=%d",ReqRlt);
                break;




            case QryID_ReqQryInvestorPositionCombineDetail:              //请求查询投资者持仓明细(组合)
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryInvestorPositionCombineDetail))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryInvestorPositionCombineDetail((PlatformStru_QryInvestorPositionCombineDetail*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询投资者持仓明细(组合))ReqQryInvestorPositionCombineDetail Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryNotice:              //请求查询客户通知
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryNotice))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryNotice((PlatformStru_QryNotice*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询客户通知)ReqQryNotice Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQrySettlementInfoConfirm:              //请求查询结算信息确认
                if(PackageHead.DataLen!=sizeof(PlatformStru_QrySettlementInfoConfirm))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQrySettlementInfoConfirm((PlatformStru_QrySettlementInfoConfirm*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询结算信息确认)ReqQrySettlementInfoConfirm Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferBank:              //请求查询转帐银行
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTransferBank))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryTransferBank((PlatformStru_QryTransferBank*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询转帐银行)ReqQryTransferBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryTransferSerial:              //请求查询转帐流水
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryTransferSerial))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryTransferSerial((PlatformStru_QryTransferSerial*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询转帐流水)ReqQryTransferSerial Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryContractBank:              //请求查询签约银行
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryContractBank))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryContractBank((PlatformStru_QryContractBank*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询签约银行)ReqQryContractBank Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryAccountregister:              //请求查询银期签约关系
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryAccountRegister))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryAccountregister((PlatformStru_QryAccountRegister*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询银期签约关系)ReqQryAccountregister Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrder:              //请求查询预埋单
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryParkedOrder))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryParkedOrder((PlatformStru_QryParkedOrder*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询预埋单)ReqQryParkedOrder Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQryParkedOrderAction:              //请求查询预埋撤单
                if(PackageHead.DataLen!=sizeof(PlatformStru_QryParkedOrderAction))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQryParkedOrderAction((PlatformStru_QryParkedOrderAction*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询预埋撤单)ReqQryParkedOrderAction Rlt=%d",ReqRlt);
                break;

            case QryID_ReqQueryBankAccountMoneyByFuture:              //查询银行余额
                if(PackageHead.DataLen!=sizeof(PlatformStru_ReqQueryAccount))  bDataErr=true;
                else ReqRlt=m_pPlatFormService->ReqQueryBankAccountMoneyByFuture((PlatformStru_ReqQueryAccount*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                LOG_INFO("CQryQueue::SendQryCmd : (查询银行余额)ReqQueryBankAccountMoneyByFuture Rlt=%d",ReqRlt);
                break;



            case QryID_ReqQryInstrumentCommissionRate:      //查询合约手续费率

                if(PackageHead.DataLen!=sizeof(CThostFtdcQryInstrumentCommissionRateField))  
                {
                    bDataErr=true;
                }
                else
                {
                    PlatformStru_InstrumentCommissionRate CommissionRate;
					int ErrNo=m_pPlatFormService->GetCommissionRate(((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID, CommissionRate);
                    if(ErrNo==0||ErrNo==CustomErrorID)
                    {
//                        ReqRlt=-1;                          //已经有了，不用查了
                        bDataIngore=true;
                    }
                    else if(((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID[0]==0)
                    {
                        bDataIngore=true;
                    }
                    else 
                    {
                        DWORD CurTickCount=GetTickCount();
                        std::map<std::string,unsigned int>::iterator it;
                        std::map<std::string,int>::iterator it_count;
                        std::string strInstrumentID=std::string(((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID);
                        it=m_QryCommissionRateTime.find(strInstrumentID);
                        it_count=m_QryCommissionRateCount.find(strInstrumentID);
                        if(it!=m_QryCommissionRateTime.end()&&
                            (CurTickCount>=it->second&&CurTickCount-it->second<60000||
                             CurTickCount<it->second&&0xffffffff-it->second+CurTickCount<60000))
                        {                                   //同一个合约，查询一次后，必须过60s才再次查询
                            REQQUEUE_DELETECURPACKAGE();
                        }
                        else
                        {
                            m_pPlatFormService->SetReqCommissionRateInstrumentID(std::string(((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID));
                            ReqRlt=m_pPlatFormService->ReqQryInstrumentCommissionRate((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                            LOG_INFO("CQryQueue::SendQryCmd : (查询合约手续费率)ReqQryInstrumentCommissionRate，InstrumentID=%s Rlt=%d",((CThostFtdcQryInstrumentCommissionRateField*)m_SendQryCmdBuf)->InstrumentID,ReqRlt);
                            if(ReqRlt==0)
                            {
                                m_QryCommissionRateTime[strInstrumentID]=GetTickCount();
                                if(it_count==m_QryCommissionRateCount.end())
                                    m_QryCommissionRateCount[strInstrumentID]=1;
                                else
                                    it_count->second=it_count->second+1;

                            }
                        }
                    }
                }
                break;

            case QryID_ReqQryInstrumentMarginRate:          //请求查询合约保证金率
                if(PackageHead.DataLen!=sizeof(CThostFtdcQryInstrumentMarginRateField))  
                {
                    bDataErr=true;
                }
                else
                {
                    PlatformStru_InstrumentMarginRate MarginRate;
					int ErrNo=m_pPlatFormService->GetMarginRate(((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf)->InstrumentID, MarginRate);
                    if(ErrNo==0||ErrNo==CustomErrorID)
                    {
//                        ReqRlt=-1;                          //已经有了，不用查了
                        bDataIngore=true;
                    }
                    else if(((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf)->InstrumentID[0]==0)
                    {
                        bDataIngore=true;
                    }
                    else 
                    {
                        DWORD CurTickCount=GetTickCount();
                        std::map<std::string,unsigned int>::iterator it;
                        std::map<std::string,int>::iterator it_count;
                        std::string strInstrumentID;
                        strInstrumentID=std::string(((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf)->InstrumentID);
                        it=m_QryMarginRateTime.find(strInstrumentID);
                        it_count=m_QryMarginRateCount.find(strInstrumentID);
                        if(it!=m_QryMarginRateTime.end()&&
                            (CurTickCount>=it->second&&CurTickCount-it->second<60000||
                             CurTickCount<it->second&&0xffffffff-it->second+CurTickCount<60000))
                        {                                   //同一个合约，查询一次后，必须过60s才再次查询
                            REQQUEUE_DELETECURPACKAGE();
                        }
                        else
                        {
                            ReqRlt=m_pPlatFormService->ReqQryInstrumentMarginRate((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf,(int)PackageHead.dwSeq);
                            LOG_INFO("CQryQueue::SendQryCmd : (查询合约保证金率)ReqQryInstrumentMarginRate，InstrumentID=%s Rlt=%d",((CThostFtdcQryInstrumentMarginRateField*)m_SendQryCmdBuf)->InstrumentID,ReqRlt);
                            if(ReqRlt==0)
                            {
                                m_QryMarginRateTime[strInstrumentID]=GetTickCount();
                                if(it_count==m_QryMarginRateCount.end())
                                    m_QryMarginRateCount[strInstrumentID]=1;
                                else
                                    it_count->second=it_count->second+1;
                            }
                        }
                    }
                }
                break;

        }
        if(bDataIngore)
        {
            REQQUEUE_DELETECURPACKAGE();
            return false;
        }
        else if(bDataErr)
        {
            REQQUEUE_DELETECURPACKAGE();
            return false;
        }
        else if(ReqRlt==0) 
        { 
            m_bInQry=true;
            REQQUEUE_DELETECURPACKAGE();
            return true; 
        }                                                               
        else 
        {
            return false;         
        }
    }
    else
    {                                                       //发送队列里没有待发送查询请求，则执行手续费率和保证金率的查询
       if(m_pPlatFormService->IsNeedCheckCommissionRateAndMarginRate())
       {
            std::set<std::string>::iterator it_InstrumentID;
            std::set<std::string> AllInstruments;
            PlatformStru_InstrumentInfo InstrumentInfo;
            PlatformStru_InstrumentCommissionRate CommissionRate;
            PlatformStru_InstrumentMarginRate MarginRate;
            DWORD CurTickCount=GetTickCount();
            bool bGetAllRate=true;

            m_pPlatFormService->GetInstrumentList(AllInstruments);
            it_InstrumentID=AllInstruments.begin();

            for(;it_InstrumentID!=AllInstruments.end();++it_InstrumentID)
            {
                if(it_InstrumentID->empty()) continue;
                if(m_pPlatFormService->GetInstrumentInfo(*it_InstrumentID,InstrumentInfo)<0) continue;
                if(InstrumentInfo.ProductClass!=THOST_FTDC_PC_Futures) continue;    //只查询期货的费率
                if(!m_pPlatFormService) return false;

                                                            //下面检查每个合约是否都已经有费率，如果没有，检查时会触发一次费率的查询
                if( m_pPlatFormService->GetCommissionRate(*it_InstrumentID,CommissionRate)<0)
                {
                    std::map<std::string,unsigned int>::iterator it;
                    std::map<std::string,int>::iterator it_count;
                    it=m_QryCommissionRateTime.find(*it_InstrumentID);
                    it_count=m_QryCommissionRateCount.find(*it_InstrumentID);
                    if((it==m_QryCommissionRateTime.end()||
                        CurTickCount>=it->second&&CurTickCount-it->second>60000||
                        CurTickCount<it->second&&0xffffffff-it->second+CurTickCount>60000)&&
                        (it_count==m_QryCommissionRateCount.end()||
                         it_count->second<=3))
					{
						//如果当前没有该费率，则放一条到查询队列中
						CThostFtdcQryInstrumentCommissionRateField ReqData;
						memset(&ReqData,0,sizeof(ReqData));
						strncpy(ReqData.BrokerID,m_pPlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
						strncpy(ReqData.InvestorID,m_pPlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);;
						strncpy(ReqData.InstrumentID,it_InstrumentID->c_str(),sizeof(ReqData.InstrumentID)-1);;

						ReqQryInstrumentCommissionRate(ReqData,0);
						return false;
					}
                    if(bGetAllRate) bGetAllRate=false;
                }

                if( m_pPlatFormService->GetMarginRate(*it_InstrumentID,MarginRate)<0)
                {
                    std::map<std::string,unsigned int>::iterator it;
                    std::map<std::string,int>::iterator it_count;
                    it=m_QryMarginRateTime.find(*it_InstrumentID);
                    it_count=m_QryMarginRateCount.find(*it_InstrumentID);
                    if((it==m_QryMarginRateTime.end()||
                        CurTickCount>=it->second&&CurTickCount-it->second>60000||
                        CurTickCount<it->second&&0xffffffff-it->second+CurTickCount>60000)&&
                        (it_count==m_QryMarginRateCount.end()||
                         it_count->second<=3))
					{                                                  //如果当前没有该费率，则放一条到查询队列中
						CThostFtdcQryInstrumentMarginRateField ReqData;
						memset(&ReqData,0,sizeof(ReqData));
						strncpy(ReqData.BrokerID,m_pPlatFormService->GetTradeBrokerID().c_str(),sizeof(ReqData.BrokerID)-1);
						strncpy(ReqData.InvestorID,m_pPlatFormService->GetTradeInvestorID().c_str(),sizeof(ReqData.InvestorID)-1);
						strncpy(ReqData.InstrumentID,it_InstrumentID->c_str(),sizeof(ReqData.InstrumentID)-1);
						ReqData.HedgeFlag=THOST_FTDC_HF_Speculation;

						ReqQryInstrumentMarginRate(ReqData,0);

						return false;
					}
                    if(bGetAllRate) bGetAllRate=false;
                }

            }
            if(it_InstrumentID==AllInstruments.end()&&bGetAllRate)
            {
                                                            //因为所有合约的费率都已经有了，清除检查的标志
                m_pPlatFormService->SetNeedCheckCommissionRateAndMarginRate(false);
                return false;
            }
        }
    }
    return false;
}
