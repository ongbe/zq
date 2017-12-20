#include "stdafx.h"

#include <time.h>
#include "ServiceProxy.hpp"
#include "DataCenter.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "PlatFormService.hpp"
#include "../inc/Module-Misc/globalfunc.h"
#include "../inc/ISvr/IPlatformSingleSvr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/GlobalConfigMgr.h"
#include "ReqOnRtnTrade.h"

extern PlatformSvrMgr* g_pPlatformMgr;

#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CDataCenter", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
    }while(0)

#define LOG_INFO2(fmt, ...) \
    do{\
		if(m_DataCenter.m_pWriteLog)\
		{\
		m_DataCenter.m_pWriteLog->WriteLog_Fmt("CDataCenter", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
    }while(0)


#define INIT_OUTDATA(structname)\
    structname field;\
    memset(&field,0,sizeof(structname));\
    outData=field

int g_LockCount=0;
char LastFunction1[256]={0};
char LastFunction2[256]={0};
char LastFunction3[256]={0};
char LastFunction4[256]={0};
#define MUTEX_LOCK()\
	g_LockCount=0;\
	while(m_mutex.TryLock()!=wxMUTEX_NO_ERROR)\
	{\
		g_LockCount++;\
		if(g_LockCount==1000)\
		{\
				LOG_INFO("Lock Failed at (%s)--->(%s)--->(%s)--->(%s)--->(%s)",LastFunction1,LastFunction2,LastFunction3,LastFunction4,__FUNCTION__);\
		}\
		wxThread::Sleep(10);\
	}\
    m_mutex.Unlock();\
	strcpy(LastFunction1,LastFunction2);\
	strcpy(LastFunction2,LastFunction3);\
	strcpy(LastFunction3,LastFunction4);\
    strcpy(LastFunction4,__FUNCTION__);\
    wxMutexLocker lock(m_mutex)
DWORD CDataCenter::DataCenterThreadFunc( void* pThis )
{
	CDataCenter* pWorker = (CDataCenter*)pThis;
	if ( NULL == pWorker )
	{
		return 0;
	}
	char Buf[512];
	sprintf(Buf,"CDataCenter::ThreadId=%u\n", GetCurrentThreadId());
	OutputDebugString(Buf);

	CPackageCache& RefPackageCache=pWorker->m_dataSource;
	char readBuf[4096];
	PackageHeadStru *pPackageHead=(PackageHeadStru *)readBuf;
	char *pData= readBuf+ sizeof(PackageHeadStru);
	while( !pWorker->m_bWorkerThreadExit )
	{
		if(RefPackageCache.IsEmpty())
		{
			wxThread::Sleep(2);
			continue;
		}

		int GetOff=-1;
		if(!RefPackageCache.PeekFrom(pPackageHead, pData, sizeof(readBuf)-sizeof(PackageHeadStru),&GetOff))
			continue;

		//LOG_INFO2("CDataCenterWorkThread PeekFrom:CMDID=%d DataLen=%d GetOff=%d",pPackageHead->CmdID,pPackageHead->DataLen,GetOff);

		if(pPackageHead->DataLen<sizeof(AbstractBusinessData))
		{
			int OriGetOff,NewGetOff;
			RefPackageCache.DeleteCurPackage(&OriGetOff,&NewGetOff);
			//LOG_INFO2("CDataCenterWorkThread DeleteCurPackage(长度不够):OriGetOff=%d NewGetOff=%d",OriGetOff,NewGetOff);
			continue;
		}
		pWorker->doProcessBusinessData(*((AbstractBusinessData*)pData)); 

		int OriGetOff,NewGetOff;
		RefPackageCache.DeleteCurPackage(&OriGetOff,&NewGetOff);
		//LOG_INFO2("CDataCenterWorkThread DeleteCurPackage:OriGetOff=%d NewGetOff=%d",OriGetOff,NewGetOff);

	}
	return 0;
}

CDataCenter::CDataCenter(CPackageCache& dataSource,CPlatFormService* pHost)
:m_dataSource(dataSource)
,m_PlatFormService(pHost)
, m_hWorkerThread(NULL)
, m_bWorkerThreadExit(false)
{
    m_pWriteLog=new zqWriteLog();

	m_pDataMgr=new CPlatformDataMgr(pHost);
}

CDataCenter::~CDataCenter()
{
	SAFE_DELETE(m_pDataMgr);
    LOG_INFO("CDataCenter::~CDataCenter(析构CDataCenter对象)");
	SAFE_DELETE(m_pWriteLog);
}

///定制业务数据
int CDataCenter::SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo)
{
    if(BID>=BID_MAX || GID>=GID_MAX || callback==NULL)
        return -1;


    int ret;
    ret=m_pDataMgr->SubscribeBusinessData(BID,GID,callback);
    if(ret<0) return ret;
    
    if(!sendCurrentInfo)
        return 0;

    switch(BID)
    {
        case BID_RtnDepthMarketData:
        {
            std::set<std::string> InstrumentSet;
            std::set<std::string>::iterator it;
            DataRtnDepthMarketData tmp;

            if(m_pDataMgr->GetInstrumentList(InstrumentSet)>0)
            {
                for(it=InstrumentSet.begin();it!=InstrumentSet.end();it++)
                {
                    if(m_pDataMgr->GetQuotInfo(*it,tmp.DepthMarketDataField)==0)
                    {
                        (callback)(GID, *(AbstractBusinessData*)(&tmp));
                    }
                }
            }
            break;
        }
        default:
            break;
    }




    return 0;
}

///取消业务数据的定制
int CDataCenter::UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)
{
    return m_pDataMgr->UnSubscribeBusinessData(BID,GID);
}

///启动
int CDataCenter::Start()
{
    LOG_INFO("Start");

    if(m_hWorkerThread)
        return 0;
	//创建线程，用于上抛数据
	DWORD dwThreadID;
	m_hWorkerThread = CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)DataCenterThreadFunc, this, 0, &dwThreadID);

    return 0;
}

///停止,定制信息保留，再次启动之后，定制依然有效
int CDataCenter::Stop()
{
    LOG_INFO("Stop");
	if( m_hWorkerThread )
	{
		m_bWorkerThreadExit = true;
		DWORD dwExitCode;
		WaitForSingleObject( m_hWorkerThread, 300 );
		if( GetExitCodeThread(m_hWorkerThread, &dwExitCode) !=0 && dwExitCode == STILL_ACTIVE )
		{
			TerminateThread( m_hWorkerThread, 0 );
		}

		CloseHandle( m_hWorkerThread );
		m_hWorkerThread = NULL;
	}
    return 0;
}

int CDataCenter::doProcessBusinessData(AbstractBusinessData& data)
{
    if(data.BID!=BID_RtnDepthMarketData)
        LOG_INFO("CDataCenter::doProcessBusinessData, BID=%d, Size=%d", data.BID, data.Size);

    DWORD t1,t2,t3;
    t1=GetTickCount();

    bool toBeContinue=true;
    switch(data.BID)
    {
        case BID_RtnDepthMarketData://深度行情
        {
            DataRtnDepthMarketData& NewBusiness= (DataRtnDepthMarketData&)data;   
            std::string strInstrument(NewBusiness.DepthMarketDataField.InstrumentID);
            PlatformStru_InstrumentInfo InstrumentInfo;

            //取合约信息
            if(GetInstrumentInfo(strInstrument,InstrumentInfo)<0)
		        break;

            //在结构中表明合约是否有持仓
            NewBusiness.bHavePosition=m_pDataMgr->HavePositionDetail(strInstrument);

            PlatformStru_DepthMarketData MarketData2;
            memset(&MarketData2,0,sizeof(MarketData2));
            MarketData2=NewBusiness.DepthMarketDataField;

            bool bFirstQuot;
            toBeContinue=m_pDataMgr->SetQuotInfo(MarketData2,bFirstQuot);

            static bool b1=true,b2=true,b3=true;
            if(b1)
                m_pDataMgr->UpdatePositionDetailByQuot(NewBusiness.DepthMarketDataField,InstrumentInfo,false);
            if(b2)
                m_pDataMgr->UpdatePositionByQuot(NewBusiness.DepthMarketDataField,InstrumentInfo,false);
            if(b3)
                m_pDataMgr->DynamicUpdateTradingAccount_RtnDepthMarketData(strInstrument);
            break;
        }
        case BID_RtnOrder://报单回报,更新报单队列
        {
			DataRtnOrder& NewBusiness= (DataRtnOrder&)data;

            PlatformStru_InstrumentInfo InstrumentInfo;
            if(GetInstrumentInfo(std::string(NewBusiness.OrderField.InstrumentID),InstrumentInfo)<0)
		        break;

            PlatformStru_OrderInfo OldOrderInfo;

            m_pDataMgr->NewOrderReady(NewBusiness.OrderField,OldOrderInfo,InstrumentInfo);

			m_pDataMgr->UpdatePositionFreezeFields_RtnOrder(NewBusiness.OrderField, OldOrderInfo);

            m_pDataMgr->DynamicUpdateTradingAccount_RtnOrder(NewBusiness.OrderField, OldOrderInfo);

			m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            break;
        }
        case BID_RtnTrade://成交回报,更新成交记录，同时更新持仓明细和持仓
        {
			DataRtnTrade& NewBusiness= (DataRtnTrade&)data;

            PlatformStru_InstrumentInfo InstrumentInfo;
            if(GetInstrumentInfo(std::string(NewBusiness.TradeField.InstrumentID),InstrumentInfo)<0)
		        break;

            m_pDataMgr->NewTradeReady(NewBusiness.TradeField,InstrumentInfo);

			m_pDataMgr->UpdateTradeTotal_RtnTrade(NewBusiness.TradeField);

			if ( GlobalConfigManager::IsCalculate_OnRtnTrade())
			{
                std::vector<PlatformStru_PositionDetail> ModifiedPositionDetails;
                ModifiedPositionDetails.clear();

				m_pDataMgr->UpdatePositionDetail_RtnTrade(NewBusiness.TradeField,ModifiedPositionDetails,InstrumentInfo);

                m_pDataMgr->UpdatePosition_RtnTrade(NewBusiness.TradeField,ModifiedPositionDetails,InstrumentInfo);

				m_pDataMgr->DynamicUpdateTradingAccount_RtnTrade();
			}

            m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);

            if(GlobalConfigManager::IsReq_OnRtnTrade())
            {
                m_PlatFormService->OnRtnTrade(std::string(NewBusiness.TradeField.BrokerID),
                                            std::string(NewBusiness.TradeField.InvestorID),
                                            std::string(NewBusiness.TradeField.InstrumentID),
                                            (NewBusiness.TradeField.TradeType==THOST_FTDC_TRDT_CombinationDerived?true:false),
                                            m_pDataMgr->GetRecordCountOfPositionDetail());

            }
            break;
        }
        case BID_RspQryInstrument://请求查询合约响应
        {
			DataRspQryInstrument& NewBusiness= (DataRspQryInstrument&)data; 
			if(NewBusiness.RspInfoField.ErrorID==0)
			{
				if(NewBusiness.InstrumentField.InstrumentID[0]!=0)
				{
					PlatformStru_InstrumentInfo& Info2=NewBusiness.InstrumentField;
					m_pDataMgr->SetInstrumentInfo(NewBusiness.InstrumentField.InstrumentID,Info2);
				}
            }
            break;
        }
        case BID_RspQryInvestorPosition://请求查询投资者持仓响应
        {
            DataRspQryInvestorPosition& NewBusiness= (DataRspQryInvestorPosition&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                //将要查询的合约ID拷贝到返回数据中去
                std::string QryInstrument;
                m_pDataMgr->GetQryPositionInstrument(QryInstrument);
                strncpy(NewBusiness.QryInstrumentID,QryInstrument.c_str(),sizeof(NewBusiness.QryInstrumentID)-1);

                m_pDataMgr->QryPositionReach(NewBusiness.InvestorPositionField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);

				if(NewBusiness.bIsLast)
				{
					m_pDataMgr->DynamicUpdateTradingAccount_RspQryInvestorPositionLast();
				}
            }
            break;
        }
        case BID_RspQryInvestorPositionDetail://请求查询投资者持仓明细响应
        {
            DataRspQryInvestorPositionDetail& NewBusiness= (DataRspQryInvestorPositionDetail&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                //将要查询的合约ID拷贝到返回数据中去
                std::string QryInstrument;
                m_pDataMgr->GetQryPositionDetailInstrument(QryInstrument);
                
                strncpy(NewBusiness.QryInstrumentID,QryInstrument.c_str(),sizeof(NewBusiness.QryInstrumentID)-1);

                m_pDataMgr->QryPositionDetailReach(NewBusiness.InvestorPositionDetailField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);

                m_PlatFormService->SetSubscribeStatus(NewBusiness.InvestorPositionDetailField.InstrumentID,S_STATUS_POS);
            }
            break;
        }
        case BID_RspQryInvestorPositionComb://请求查询投资者组合持仓明细响应
        {
            DataRspQryInvestorPositionComb& NewBusiness= (DataRspQryInvestorPositionComb&)data; 
			if (NewBusiness.RspInfoField.ErrorID == 0 || NewBusiness.RspInfoField.ErrorID == CustomErrorID)
			{
                //将要查询的合约ID拷贝到返回数据中去
                std::string QryInstrument;
                m_pDataMgr->GetQryPositionCombInstrument(QryInstrument);
                strncpy(NewBusiness.QryInstrumentID,QryInstrument.c_str(),sizeof(NewBusiness.QryInstrumentID)-1);

				//添加组合持仓明细记录
				m_pDataMgr->QryPositionCombReach(NewBusiness.InvestorPositionCombineDetailField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
			}
			break;
        }
        case BID_RspQryTrade://请求查询成交响应
        {
            DataRspQryTrade& NewBusiness= (DataRspQryTrade&)data;
            if(NewBusiness.RspInfoField.ErrorID == 0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->QryTradeReach(NewBusiness.TradeField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);

                if(NewBusiness.bIsLast)
				{
					//计算成交统计
					m_pDataMgr->QryTradeTotalReach(true);

					//更新资金面板数据(手续费)
                    m_pDataMgr->DynamicUpdateTradingAccount_RspQryTrade();
				}

				m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
            }
            break;
        }
        case BID_RspQryOrder://请求查询报单响应
        {
            DataRspQryOrder& NewBusiness= (DataRspQryOrder&)data;
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->QryOrderReach(NewBusiness.OrderField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);

                if(NewBusiness.bIsLast)
                    m_pDataMgr->DynamicUpdateTradingAccount_RspQryOrder();

                m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            }
            break;        
        }
        case BID_RspOrderInsert://报单录入回报
        {
            DataRspOrderInsert& NewBusiness= (DataRspOrderInsert&)data;
            if(NewBusiness.RspInfoField.ErrorID!=0)
            {
			   m_PlatFormService->SetSubscribeStatus(NewBusiness.InputOrderField.InstrumentID,S_STATUS_ORDER);
            }
        }
        break;  
        case BID_RspQryTradingAccount://交易资金账户信息
        {
            DataRspQryTradingAccount& NewBusiness= (DataRspQryTradingAccount&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->SetTradingAccount(NewBusiness);
            }
        }
        break;
        case BID_RspQryInvestor://请求查询投资者响应
        {
            DataRspQryInvestor& NewBusiness= (DataRspQryInvestor&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->SetInvestorInfo(NewBusiness);
            }
        }
        break;
        case BID_RspQryInstrumentMarginRate://请求查询合约保证金率响应
            {
                DataRspQryInstrumentMarginRate& NewBusiness= (DataRspQryInstrumentMarginRate&)data;
                if(NewBusiness.RspInfoField.ErrorID==0)
                {
                    std::string strInstrument(NewBusiness.InstrumentMarginRateField.InstrumentID);
                    PlatformStru_InstrumentInfo InstrumentInfo;
                    if(GetInstrumentInfo(strInstrument,InstrumentInfo)<0)
		                break;

                    PlatformStru_InstrumentMarginRate MarginRate;
                    MarginRate=NewBusiness.InstrumentMarginRateField;
                    m_pDataMgr->SetMarginRate(NewBusiness.InstrumentMarginRateField.InstrumentID,MarginRate);


                    if(m_pDataMgr->UpdateOrderFreezeMargin_RspQryMarginRate(strInstrument,InstrumentInfo))
                        m_pDataMgr->DynamicUpdateTradingAccount_Freeze();

                    m_pDataMgr->UpdatePositionDetailByMarginRate_RspQryRate(MarginRate,InstrumentInfo);

					m_pDataMgr->UpdatePositionFreezeFieldsByOrders_RspQryRate(std::string(MarginRate.InstrumentID));

                    if(m_pDataMgr->UpdatePositionByMarginRate_RspQryRate(MarginRate))
                        m_pDataMgr->DynamicUpdateTradingAccount_RspQryRate();
                }
				else if(NewBusiness.RspInfoField.ErrorID==CustomErrorID)
				{
					//查无记录
					m_pDataMgr->IncrementReqMarginRateError(NewBusiness.InstrumentMarginRateField.InstrumentID);
				}
            }
            break;
        case BID_RspQryInstrumentCommissionRate://请求查询合约手续费率响应
            {
                DataRspQryInstrumentCommissionRate& NewBusiness= (DataRspQryInstrumentCommissionRate&)data;
                if(NewBusiness.RspInfoField.ErrorID==0)
                {
                    //m_allProductID此时已经有数据(查询合约信息必须在 查询手续费率之前)
                    std::set<std::string> InstrumentList;
                    int ret;
                    bool bHaveOrder=false;
                    bool bHaveTrade=false;
                    bool bHavePosition=false;
                    ret=m_pDataMgr->GetInstrumentListByProductID(NewBusiness.InstrumentCommissionRateField.InstrumentID,InstrumentList);
                    if(ret>0)
                    {//是品种代码
                        for(std::set<std::string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
                        {
                            PlatformStru_InstrumentInfo InstrumentInfo;
                            if(GetInstrumentInfo(*it2,InstrumentInfo)<0)
		                        break;

                            PlatformStru_InstrumentCommissionRate Rate2;
                            memset(&Rate2,0,sizeof(Rate2));
                            Rate2=NewBusiness.InstrumentCommissionRateField;
                            m_pDataMgr->SetCommissionRate(*it2,Rate2);

							//更新报单数据中冻结手续费
                            if(m_pDataMgr->UpdateOrderFreezeCommission_RspQryCommissionRate(*it2,InstrumentInfo)&&!bHaveOrder)
                                bHaveOrder=true;

							//更新成交明细数据中手续费
							if(m_pDataMgr->UpdateTradeCommission_RspQryCommissionRate(*it2,InstrumentInfo)&&!bHaveTrade)
                                bHaveTrade=true;

							//更新成交统计
							m_pDataMgr->UpdateTradeTotal_RspQryCommissionRate(*it2);

							//更新持仓中的冻结域
							m_pDataMgr->UpdatePositionFreezeFieldsByOrders_RspQryRate(*it2);
                            if(m_pDataMgr->UpdatePositionByCommissionRate_RspQryRate(Rate2)&&!bHavePosition)
                                bHavePosition=true;
                        }
                    }
                    else
                    {//是合约ID
                        std::string strInstrument(NewBusiness.InstrumentCommissionRateField.InstrumentID);

                        PlatformStru_InstrumentInfo InstrumentInfo;
                        if(GetInstrumentInfo(strInstrument,InstrumentInfo)<0)
	                        break;

                        PlatformStru_InstrumentCommissionRate Rate2;
                        memset(&Rate2,0,sizeof(Rate2));
                        Rate2=NewBusiness.InstrumentCommissionRateField;
                        m_pDataMgr->SetCommissionRate(NewBusiness.InstrumentCommissionRateField.InstrumentID,Rate2);
    
						//更新报单数据中冻结手续费
                        if(m_pDataMgr->UpdateOrderFreezeCommission_RspQryCommissionRate(strInstrument,InstrumentInfo)&&!bHaveOrder)
                            bHaveOrder=true;

						//更新成交明细数据中手续费
                        if(m_pDataMgr->UpdateTradeCommission_RspQryCommissionRate(strInstrument,InstrumentInfo)&&!bHaveTrade)
                            bHaveTrade=true;

						//更新成交统计
						m_pDataMgr->UpdateTradeTotal_RspQryCommissionRate(strInstrument);

						//更新持仓中的冻结域
						m_pDataMgr->UpdatePositionFreezeFieldsByOrders_RspQryRate(strInstrument);
                        if(m_pDataMgr->UpdatePositionByCommissionRate_RspQryRate(Rate2)&&!bHavePosition)
                            bHavePosition=true;
                    }
                    if(bHaveOrder)
                        m_pDataMgr->DynamicUpdateTradingAccount_Freeze();
                    if(bHaveTrade||bHavePosition)
                        m_pDataMgr->DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate();
                }
                else if(NewBusiness.RspInfoField.ErrorID==CustomErrorID)
                {
					//查无记录
					m_pDataMgr->IncrementReqCommissionRateError(NewBusiness.InstrumentCommissionRateField.InstrumentID);
                }
            }
            break;
        case BID_RspQrySettlementInfo://请求查询投资者结算结果响应
        {
            DataRspQrySettlementInfo& NewBusiness= (DataRspQrySettlementInfo&)data;
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                m_pDataMgr->AppendLastSettlementInfo(std::string(NewBusiness.SettlementInfoField.Content),NewBusiness.bIsLast);
            }
            else
            {
                m_pDataMgr->ClearLastSettlementInfo();
            }
        }
        break;
        case BID_RspUserLogin://登入响应，主要记录交易所的时间
        {
            DataRspUserLogin& NewBusiness= (DataRspUserLogin&)data;
            if(NewBusiness.RspInfoField.ErrorID==0 && NewBusiness.bTrader)
            {
                ExchangeTime exchangeTime;
                m_pDataMgr->GetExchangeTime(exchangeTime);
                exchangeTime.CurTickCount = NewBusiness.CurTickCount;
                memcpy(&exchangeTime.TradingDay, &NewBusiness.UserLoginField.TradingDay, sizeof(exchangeTime.TradingDay));
                memcpy(&exchangeTime.SHFETime, &NewBusiness.UserLoginField.SHFETime, sizeof(exchangeTime.SHFETime));
                memcpy(&exchangeTime.DCETime, &NewBusiness.UserLoginField.DCETime, sizeof(exchangeTime.DCETime));
                memcpy(&exchangeTime.CZCETime, &NewBusiness.UserLoginField.CZCETime, sizeof(exchangeTime.CZCETime));
                memcpy(&exchangeTime.FFEXTime, &NewBusiness.UserLoginField.FFEXTime, sizeof(exchangeTime.FFEXTime));
                m_pDataMgr->SetExchangeTime(exchangeTime);
            }
            break;
        }
		case BID_RspQryContractBank://请求查询签约银行响应
        {
            DataRspQryContractBank& pTmp= (DataRspQryContractBank&)data;
            //CThostFtdcContractBankField* pRawData= &pTmp->ContractBankField;
            if(pTmp.RspInfoField.ErrorID == 0)
            {   

                m_pDataMgr->SaveBankName(pTmp.ContractBankField);
				PlatformStru_QryAccountRegister  QryAccountregister;
				memset(&QryAccountregister,0,sizeof(QryAccountregister));
			   // strcpy(QryAccountregister.Thost.BankID,pTmp.ContractBankField.BankID);
				if(pTmp.bIsLast==1)
				{
					if( m_PlatFormService )
						m_PlatFormService->m_rServiceProxy.ReqQryAccountregister(QryAccountregister);
				}
				//m_pDataMgr->UpdateContractBank(pTmp.ContractBankField);

				/* m_ContractBankInfo[pRawData->BankName]=*pRawData;
				m_Accountregister.push_back(pRawData->BankID);


                m_timer.Start(1000,true);       */       

            }


            break;
        }
		
		case   BID_RspQryAccountregister://请求查询银期签约关系响应
        {
            DataRspQryAccountregister& pTmp= (DataRspQryAccountregister&)data;
            CThostFtdcAccountregisterField* pRawData= &pTmp.Accountregister;
            if(pTmp.RspInfoField.ErrorID == 0)
            {   
               
                m_pDataMgr->UpdateContractBank(pTmp.Accountregister);
            }


            break;
        }

        case BID_RspParkedOrderInsert:       //预埋单录入请求响应
        case BID_RspParkedOrderAction:       //预埋撤单录入请求响应
        case BID_RspRemoveParkedOrder:       //删除预埋单响应    
        case BID_RspRemoveParkedOrderAction: //删除预埋撤单响应
        case BID_RspQryParkedOrder:          //请求查询预埋单响应
        case BID_RspQryParkedOrderAction:    //请求查询预埋撤单响应
        {
            break;
        }

        default:
        break;                  
    }


    if(!toBeContinue)
        return -1;


    t2=GetTickCount();

    std::map<GUIModuleID,GuiCallBackFunc> CallbackFuncList;
    std::map<GUIModuleID,GuiCallBackFunc>::iterator it_callback;

    if(m_pDataMgr->GetSubscribeBusinessData(data.BID,CallbackFuncList)==0)
    {
		data.pPlatformSvr = &(m_PlatFormService->m_rServiceProxy);
        int i;
        for(i=0,it_callback=CallbackFuncList.begin();it_callback!=CallbackFuncList.end();it_callback++,i++)
        {
            if(it_callback->second)
                (it_callback->second)(it_callback->first,data);
        }
    }

    t3=GetTickCount();


    char Buf[512];
    sprintf(Buf,"doProcessBusinessData(%03d): - \t"
                "t1: %03u \t"
                "t2: %03u \t"
                "Total: %03u    ----", 
                data.BID,
                t2-t1,t3-t2,t3-t1);
//    OutputDebugString(Buf);

    return 0;  
}
///获取合约列表
int CDataCenter::GetInstrumentList(std::vector<GroupInfo> &outData)
{
    return m_pDataMgr->GetInstrumentList(outData);
}
///获取合约列表
int CDataCenter::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
{
    return m_pDataMgr->GetInstrumentList(outData,ExchangeID);
}
//添加主力合约列表
int CDataCenter::AddMainInstrumentList(std::string instrument)
{
	 return m_pDataMgr->AddMainInstrumentList(instrument);
}
///获取主力合约列表
int CDataCenter::GetMainInstrumentList(std::set<std::string> &outData)
{
    return m_pDataMgr->GetMainInstrumentList(outData);
}
///设置合约容差列表
void CDataCenter::SetInstrumentVarietyMap(map<string, string>& inData)
{
    m_pDataMgr->SetInstrumentVarietyMap(inData);
}

// 获取合约容差列表
int CDataCenter::GetInstrumentVarietyMap(map<string, string>& outData)
{
    return m_pDataMgr->GetInstrumentVarietyMap(outData);
}

///获取指定合约信息
int CDataCenter::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    return m_pDataMgr->GetInstrumentInfo(InstrumentID,outData);
}
///设置指定合约信息
void CDataCenter::SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
{
    m_pDataMgr->SetInstrumentInfo(InstrumentID,outData);
    
}

///获取合约的产品类型，失败返回-1
char CDataCenter::GetProductClassType(const std::string& InstrumentID)
{
    return m_pDataMgr->GetProductClassType(InstrumentID);
}


///获取合约品种代码
int CDataCenter::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
{
    return m_pDataMgr->GetProductID(outData,ExchangeID);
}

///获取合约手续费率
int CDataCenter::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    return m_pDataMgr->GetCommissionRate(InstrumentID,outData);
}
///设置合约手续费率
int CDataCenter::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
{
    m_pDataMgr->SetCommissionRate(InstrumentID,outData);
    return 0;
}
///设置正在查询手续费率的合约
void CDataCenter::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
{
    m_pDataMgr->SetReqCommissionRateInstrumentID(InstrumentID);
}
///获取合约保证金率
int CDataCenter::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    return m_pDataMgr->GetMarginRate(InstrumentID,outData);
}
///设置合约保证金率
int CDataCenter::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
{
    m_pDataMgr->SetMarginRate(InstrumentID,outData);
    return 0;
}

///获取合约乘数，成功返回合约乘数，失败返回-1
int CDataCenter::GetInstrumentMultiple(const std::string& InstrumentID)
{
    return m_pDataMgr->GetInstrumentMultiple(InstrumentID);
}
///获取指定品种的合约列表,返回合约个数
int CDataCenter::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
{
    return m_pDataMgr->GetInstrumentListByProductID(ProductID,outData);
}



///获取指定合约行情
int CDataCenter::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataMgr->GetQuotInfo(InstrumentID,outData);
}


///获取指定合约旧行情
int CDataCenter::GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
{
    return m_pDataMgr->GetOldQuotInfo(InstrumentID,outData);
}

///获取指定合约的现价，失败返回0.0
double CDataCenter::GetCurPrice(const std::string& InstrumentID)
{
    return m_pDataMgr->GetCurPrice(InstrumentID);
}

int CDataCenter::GetPositions2(const std::string& strInstrument,
                               std::set<long>& setFTID,
                               std::vector<PlatformStru_Position>& outData,
                               long& lastseq)
{
    return m_pDataMgr->GetPositions2(strInstrument,setFTID,outData,lastseq);
}

int CDataCenter::GetPositions3(const std::string& strInstrument,
                               std::vector<PlatformStru_Position>& outData,
                               long& lastseq)
{
    return m_pDataMgr->GetPositions3(strInstrument,outData,lastseq);
}

int CDataCenter::GetPositionDetails3(const std::string& strInstrument,
                          std::set<long>& setFTID,
                          std::vector<PlatformStru_PositionDetail>& vecValue,
                          long& lastseq)
{
    return m_pDataMgr->GetPositionDetails3(strInstrument,setFTID,vecValue,lastseq);
}

int CDataCenter::GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData)
{
    return m_pDataMgr->GetPositionCombs2(strTradeID, strInstrument, outData);
}

int CDataCenter::GetPositions(std::vector<PlatformStru_Position>& outData)
{
    return m_pDataMgr->GetPositions(outData);
}

int CDataCenter::GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)
{
    return m_pDataMgr->GetPositionDetails(outData,lastseq);
}

int CDataCenter::GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData)
{
    return m_pDataMgr->GetPositionCombs(outData);
}



bool CDataCenter::HavePositionDetail(const std::string& strInstrumentID)
{
    return m_pDataMgr->HavePositionDetail(strInstrumentID);
}


//获取持仓记录键值列表,返回持仓记录的数量
int CDataCenter::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_pDataMgr->GetPositionKeySet(PositionKeySet);
}

//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
int CDataCenter::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    return m_pDataMgr->GetPositionData(strAccount,InstrumentID,Direction,HedgeFlag,PositionData);
}



string CDataCenter::GetTradingDay(const string& ExchangeID)
{
    return m_pDataMgr->GetTradingDay(ExchangeID);
}

bool CDataCenter::IsTodayPosition( const char* pOpenData )
{
	return m_pDataMgr->IsTodayPosition( pOpenData );
}

///获取交易所时间
unsigned long CDataCenter::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataMgr->GetExchangeTime(ExchangeID,time);
}
    ///获取全部交易所
int CDataCenter::GetExchangeIDs(vector<string>& outData)
{
	return m_pDataMgr->GetExchangeIDs(outData);
}

///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
void CDataCenter::SetCurrentPositionContent(int PositionContentMode)
{
    return m_pDataMgr->SetCurrentPositionContent(PositionContentMode);
}



///获取全部报单
int CDataCenter::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetAllOrders(outData);
}
///获取全部报单
int CDataCenter::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetAllOrders2(strInstrument,outData);
}
///获取指定报单
bool CDataCenter::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetOrder(inOrderKey,outData);
}
int CDataCenter::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTriggerOrders(outData);
}

///获取合约相关的已触发的报单
int CDataCenter::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTriggerOrders2(strInstrument,outData);
}

///获取指定已触发的报单
bool CDataCenter::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetTriggerOrder(orderkey,outData);
}

///获取已成交报单
int CDataCenter::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTradedOrders(outData);
}

///获取已成交报单
int CDataCenter::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTradedOrders2(strInstrument,outData);
}

///获取指定已成交报单
bool CDataCenter::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetTradedOrder(orderkey,outData);
}

///获取已撤单和错误报单
int CDataCenter::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetCanceledOrders(outData);
}

///获取已撤单和错误报单
int CDataCenter::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetCanceledOrders2(strInstrument,outData);
}

///获取指定已撤单和错误报单
bool CDataCenter::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetCanceledOrder(orderkey,outData);
}

///获取未成交报单
int CDataCenter::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetWaitOrders(outData);
}

///获取未成交报单
int CDataCenter::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetWaitOrders2(strInstrument,outData);
}

///获取指定未成交报单
bool CDataCenter::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetWaitOrder(orderkey,outData);
}



//获取指定合约、指定方向、指定开平的挂单数量（手数）
int  CDataCenter::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataMgr->GetWaitOrderVolume(strAccount,strInstrumentID,Direction,CloseType);
}

//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
int CDataCenter::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataMgr->GetCloseOrderVolume(strAccount,strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
}

///获取全部成交单
int CDataCenter::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->GetAllTrades(outData);
}
///获取指定成交信息
bool CDataCenter::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    return m_pDataMgr->GetTradeInfo(tradekey,outData);
}
///获取指定成交信息
int CDataCenter::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->GetTradesOfInstrument(strInstrument,outData);
}

///获取全部成交统计记录
int CDataCenter::GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_pDataMgr->GetAllTradeTotals(outData);
}

///获取指定合约的成交统计记录成功返回0，失败返回-1
int CDataCenter::GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)
{
    return m_pDataMgr->GetTradeTotalOfInstrument(strInstrument,outData);
}

///获取成交统计
int CDataCenter::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return 0;
//    return m_pDataMgr->GetAllTradeTotalDatas(outData);
}

///在成交统计查找参数rawData
int CDataCenter::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return 0;
//    return m_pDataMgr->FindIndexFromAllTradeTotalDatas(rawData);
}
int CDataCenter::GetTradingAccountAvailable(double& fAvailable)
{
    return m_pDataMgr->GetTradingAccountAvailable(fAvailable);
}
int CDataCenter::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
{
    return m_pDataMgr->GetTradingAccountWithdrawQuota(fWithdrawQuota);
}
int CDataCenter::GetTradingAccountID(char* AccountID)
{
    return m_pDataMgr->GetTradingAccountID(AccountID);
}
///获取交易资金账户信息
int CDataCenter::GetTradingAccount(PlatformStru_TradingAccountInfo& outData)
{
    return m_pDataMgr->GetTradingAccount(outData);
}
///获取账户资金文本信息
int CDataCenter::GetAccountText(std::string& outData,int language)
{
    return m_pDataMgr->GetAccountText(outData,language);
}
///设置交易资金账户信息
void CDataCenter::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
{
    m_pDataMgr->SetTradingAccount(inData);
}
    
///请求查询投资者结算结果响应
int CDataCenter::GetLastSettlementInfo(std::string& outData)
{
    return m_pDataMgr->GetLastSettlementInfo(outData);
}
 //获取客户签约银行信息
int CDataCenter::GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)
{
    return m_pDataMgr->GetContractBank(outData);
}

SERVER_PARAM_CFG&  CDataCenter::GetServerParamCfg()
{
    return m_pDataMgr->GetServerParamCfg();
}

void CDataCenter::SetModuleDataQry(bool bQry)
{
    m_pDataMgr->SetModuleDataQry(bQry);
}
bool CDataCenter::GetModuleDataQry()
{
    return m_pDataMgr->GetModuleDataQry();
}

void CDataCenter::ClearMapQryRltOrders()
{
    m_pDataMgr->ClearMapQryRltOrders();
}

void CDataCenter::ClearMapQryRltTrades()
{
    m_pDataMgr->ClearMapQryRltTrades();
}

void CDataCenter::ClearMapQryRltPositionDetails(std::string& strQryInstrument)
{
    m_pDataMgr->ClearMapQryRltPositionDetails(strQryInstrument);
}

void CDataCenter::ClearMapQryRltPositions(std::string& strQryInstrument)
{
    m_pDataMgr->ClearMapQryRltPositions(strQryInstrument);
}

void CDataCenter::ClearMapQryRltPositionCombs(std::string& strQryInstrument)
{
    m_pDataMgr->ClearMapQryRltPositionCombs(strQryInstrument);
}

int CDataCenter::GetAllOrderFTIDList( std::vector<long>& vec )
{
	return m_pDataMgr->GetAllOrderFTIDList(vec);
}

int CDataCenter::GetAllOrderInfo( long lFTID, PlatformStru_OrderInfo& outData )
{
	return m_pDataMgr->GetAllOrderInfo(lFTID, outData);
}

int CDataCenter::GetAllOrderInfo( OrderKey key, PlatformStru_OrderInfo& outData )
{
	return m_pDataMgr->GetAllOrderInfo(key, outData);
}

int CDataCenter::GetPositionFTIDList( std::vector<long>& vec )
{
	return m_pDataMgr->GetPositionFTIDList(vec);
}

int CDataCenter::GetPositionInfo( long lFTID, PlatformStru_Position& outData )
{
	return m_pDataMgr->GetPositionInfo(lFTID, outData);
}

int CDataCenter::GetPositionInfo( PositionKey key, PlatformStru_Position& outData )
{
	return m_pDataMgr->GetPositionInfo(key, outData);
}

int CDataCenter::GetPositionCombFTIDList( std::vector<long>& vec )
{
	return m_pDataMgr->GetPositionCombFTIDList(vec);
}

int CDataCenter::GetPositionCombInfo( long lFTID, PlatformStru_PositionComb& outData )
{
	return m_pDataMgr->GetPositionCombInfo(lFTID, outData);
}

