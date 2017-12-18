#include "stdafx.h"

#include <time.h>
#include "CmeApiSvr.h"
#include "DataCenter.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "PlatFormService.hpp"

#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CDataCenter", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
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
class CDataCenterWorkThread : public wxThread
{
public:
    CDataCenterWorkThread(CDataCenter& dc)
        :wxThread(),m_DataCenter(dc)
    {

    };

    // thread execution starts here
    virtual void *Entry()
    {
	    zqWriteLog* m_pWriteLog2;
        m_pWriteLog2 = new zqWriteLog(LOGMODE_LOCALFILE, "DataCenterWorkThread.log");
        m_pWriteLog2->WriteLog_Fmt("DataCenterWorkThread", LOGLEVEL_DEBUGINFO, "DataCenterWorkThread running... ");

        CPackageCache& RefPackageCache=m_DataCenter.m_dataSource;
        char readBuf[4096];
        PackageHeadStru *pPackageHead=(PackageHeadStru *)readBuf;
        char *pData= readBuf+ sizeof(PackageHeadStru);
        while(!TestDestroy())
        {
            if(RefPackageCache.IsEmpty())
            {
                wxThread::Sleep(2);
                continue;
            }
           
            if(!RefPackageCache.PeekFrom(pPackageHead, pData, sizeof(readBuf)-sizeof(PackageHeadStru)))
                continue;

            if(pPackageHead->DataLen<sizeof(AbstractBusinessData))
            {
                RefPackageCache.DeleteCurPackage();
                continue;
            }
            m_DataCenter.doProcessBusinessData(*((AbstractBusinessData*)pData)); 
            RefPackageCache.DeleteCurPackage();
            
        }
        m_pWriteLog2->WriteLog_Fmt("DataCenterWorkThread", LOGLEVEL_DEBUGINFO, "DataCenterWorkThread end ");
        delete m_pWriteLog2;

        return NULL;
    };

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit()
    {
        m_DataCenter.m_pWorkThread = NULL;
    };

private:
    CDataCenter& m_DataCenter;
};



CDataCenter::CDataCenter(CPackageCache& dataSource,CPlatFormService* pHost)
:m_dataSource(dataSource),
m_PlatFormService(pHost)
{
    m_pWriteLog=new zqWriteLog();
    m_pWorkThread = NULL;
	m_pDataMgr=new CPlatformDataMgr(pHost);
}

CDataCenter::~CDataCenter()
{
	SAFE_DELETE(m_pDataMgr);
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
//            CThostFtdcDepthMarketDataField QuotData;
            DataRtnDepthMarketData tmp;
            PlatformStru_DepthMarketData MarketData;

            if(m_pDataMgr->GetInstrumentList(InstrumentSet)>0)
            {
                for(it=InstrumentSet.begin();it!=InstrumentSet.end();++it)
                {
                    if(m_pDataMgr->GetQuotInfo(*it,MarketData)==0)
                    {
                        tmp.DepthMarketDataField=MarketData;
                        (callback)(*(AbstractBusinessData*)(&tmp));
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
    if(m_pWorkThread)
        return 0;

    m_pWorkThread = new CDataCenterWorkThread(*this);
    if(m_pWorkThread->Create(512*1024) != wxTHREAD_NO_ERROR)
        LOG_INFO("Start:创建DataCenter的内部工作线程失败");
    else
        LOG_INFO("Start:创建DataCenter的内部工作线程成功");

    if(m_pWorkThread->Run() != wxTHREAD_NO_ERROR)
    {
         LOG_INFO("Start:运行DataCenter的内部工作线程失败!");
         return -1;
    }
    else
    {
         LOG_INFO("Start:运行DataCenter的内部工作线程成功!");
    }

    return 0;
}

///停止,定制信息保留，再次启动之后，定制依然有效
int CDataCenter::Stop()
{
    LOG_INFO("Stop");
    if(!m_pWorkThread)
        return 0;

    LOG_INFO("Stop:关闭DataCenter的内部工作线程");
    m_pWorkThread->Delete();
    m_pWorkThread=NULL; //will also be set NULL in calling CDataCenterWorkThread::OnExit()

    return 0;
}

int CDataCenter::doProcessBusinessData(AbstractBusinessData& data)
{
    if(data.BID!=BID_RtnDepthMarketData)
        LOG_INFO("CDataCenter::doProcessBusinessData, BID=%d, Size=%d", data.BID, data.Size);

//    MUTEX_LOCK();

    bool toBeContinue=true;
    switch(data.BID)
    {
        case BID_RtnDepthMarketData://深度行情
        {
            DataRtnDepthMarketData& NewBusiness= (DataRtnDepthMarketData&)data;            
            
            PlatformStru_DepthMarketData MarketData2;
            memset(&MarketData2,0,sizeof(MarketData2));
            MarketData2=NewBusiness.DepthMarketDataField;
            toBeContinue=m_pDataMgr->SetQuotInfo(MarketData2);
            m_pDataMgr->UpdateProfitInDynamicPositionDetail(NewBusiness.DepthMarketDataField);      //更新持仓明细记录中的持仓盈亏
            m_pDataMgr->DynamicUpdateTradingAccount_NewQuot(std::string(NewBusiness.DepthMarketDataField.InstrumentID));


            break;
        }
        case BID_RtnOrder://报单回报,更新报单队列
        {
			DataRtnOrder& NewBusiness= (DataRtnOrder&)data;

            m_pDataMgr->NewOrderReady(NewBusiness.OrderField);
            m_pDataMgr->DynamicUpdateTradingAccount();
			m_pDataMgr->DynamicUpdateAllOrderPanel(NewBusiness.OrderField);
			m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            break;
        }
        case BID_RtnTrade://成交回报,更新报单队列
        {
			//DataRtnTrade& NewBusiness= (DataRtnTrade&)data;

			//PlatformStru_TradeInfo TradeInfo2;
			//memset(&TradeInfo2,0,sizeof(TradeInfo2));
			//TradeInfo2=NewBusiness.TradeField;
			//m_pDataMgr->NewTradeReady(TradeInfo2);

			//m_pDataMgr->DynamicUpdateTradingAccount();
			//m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
			m_pDataMgr->RefreshPositionData();
            break;
        }
        case BID_RspQryInstrument://请求查询合约响应
        {
            DataRspQryInstrument& NewBusiness= (DataRspQryInstrument&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0)
            {
                if(NewBusiness.InstrumentField.InstrumentID[0]!=0)
                {
                    PlatformStru_InstrumentInfo Info2;
                    Info2=NewBusiness.InstrumentField;
                    m_pDataMgr->SetInstrumentInfo(NewBusiness.InstrumentField.InstrumentID,Info2);
                }
                if(NewBusiness.bIsLast)
                {
                    //m_pDataMgr->ProcessNewTradeList();
                    m_pDataMgr->DynamicUpdateTradingAccount();
                }

            }
        }
        break;
        case BID_RspQryInvestorPositionDetail://请求查询投资者持仓明细响应
        {
            DataRspQryInvestorPositionDetail& NewBusiness= (DataRspQryInvestorPositionDetail&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                //添加持仓明细记录
                m_pDataMgr->OriginPositionDetailReady(NewBusiness.InvestorPositionDetailField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
                if(NewBusiness.bIsLast)
                    m_pDataMgr->DynamicUpdateTradingAccount();
				m_PlatFormService->SetSubscribeStatus(NewBusiness.InvestorPositionDetailField.InstrumentID,S_STATUS_POS);
            }
            break;
        }
        case BID_RspQryTrade:
        {
			DataRspQryTrade& NewBusiness= (DataRspQryTrade&)data;
			CThostFtdcRspInfoField& RspInfoField = NewBusiness.RspInfoField;
			if(RspInfoField.ErrorID == 0||RspInfoField.ErrorID==CustomErrorID)
			{
				//更新持仓明细
				m_pDataMgr->OriginTradeReady(NewBusiness.TradeField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
				if(NewBusiness.bIsLast)
					m_pDataMgr->DynamicUpdateTradingAccount();
				m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
			}
        }
        break;
        case BID_RspQryOrder://请求查询报单响应
        {
            DataRspQryOrder& NewBusiness= (DataRspQryOrder&)data;
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->OriginOrderReady(NewBusiness.OrderField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
                if(NewBusiness.bIsLast)
                    m_pDataMgr->DynamicUpdateTradingAccount();
				m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            }
            break;        
        }
        case BID_RspOrderInsert://报单录入回报
        {
            DataRspOrderInsert& NewBusiness= (DataRspOrderInsert&)data;
            if(NewBusiness.RspInfoField.ErrorID!=0)
            {
			   m_pDataMgr->UpdateOrdersforOrderInsert(NewBusiness.InputOrderField,NewBusiness.RspInfoField);
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
                m_pDataMgr->DynamicUpdateTradingAccount();
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
                    PlatformStru_InstrumentMarginRate MarginRate;
                    MarginRate=NewBusiness.InstrumentMarginRateField;
                    m_pDataMgr->SetMarginRate(NewBusiness.InstrumentMarginRateField.InstrumentID,MarginRate);
                    //m_pDataMgr->ProcessNewTradeList();
                    m_pDataMgr->DynamicUpdateTradingAccount();
                }
            }
            break;
        case BID_RspQryInstrumentCommissionRate://请求查询合约手续费率响应
            {
                DataRspQryInstrumentCommissionRate& NewBusiness= (DataRspQryInstrumentCommissionRate&)data;
                if(NewBusiness.RspInfoField.ErrorID==0)
                {
                    //NewBusiness.InstrumentCommissionRateField.InstrumentID,经测试，有时候是合约ID,有时候是品种代码
                    //统一为合约ID

                    //m_allProductID此时已经有数据(查询合约信息必须在 查询手续费率之前)
                    std::set<std::string> InstrumentList;
                    std::string QryInstrumentID;
                    int ret;
                    ret=m_pDataMgr->GetInstrumentListByProductID(NewBusiness.InstrumentCommissionRateField.InstrumentID,InstrumentList);
                    if(ret>0)
                    {//是品种代码
                        m_pDataMgr->GetReqCommissionRateInstrumentID(QryInstrumentID);
                        for(std::set<std::string>::iterator it2=InstrumentList.begin(); it2!=InstrumentList.end(); ++it2)
                        {
                            if(*it2==QryInstrumentID)
                            {
                                PlatformStru_InstrumentCommissionRate Rate2;
                                memset(&Rate2,0,sizeof(Rate2));
                                Rate2=NewBusiness.InstrumentCommissionRateField;
                                m_pDataMgr->SetCommissionRate(*it2,Rate2);
                            }
                        }
                    }
                    else
                    {//是合约ID
                        PlatformStru_InstrumentCommissionRate Rate2;
                        memset(&Rate2,0,sizeof(Rate2));
                        Rate2=NewBusiness.InstrumentCommissionRateField;
                        m_pDataMgr->SetCommissionRate(NewBusiness.InstrumentCommissionRateField.InstrumentID,Rate2);
                    }
                    m_pDataMgr->DynamicUpdateTradingAccount();
                }
                else if(std::string(NewBusiness.RspInfoField.ErrorMsg)==std::string("查询无记录"))
                {
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


    std::list<GuiCallBackFunc> CallbackFuncList;
    std::list<GuiCallBackFunc>::iterator it_callback;
    int ret;
    ret=m_pDataMgr->GetSubscribeBusinessData(data.BID,CallbackFuncList);
    if(ret==0)
    {
        for(it_callback=CallbackFuncList.begin();it_callback!=CallbackFuncList.end();++it_callback)
        {
            (*it_callback)(data);
        }
    }
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
///获取所有委托单信息
 int CDataCenter::GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap)
 {
	 return m_pDataMgr->GetAllOrdersEx(AllOrdersMap);
 }

int CDataCenter::GetPositionDetails(std::multimap<std::string, PositionDetailStru> &PositionDetailMap)
{
    return m_pDataMgr->GetPositionDetails(PositionDetailMap);
}

int CDataCenter::GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru> &PositionDetailMap)
{
    return m_pDataMgr->GetPositionDetails(InstrumentID,PositionDetailMap);
}

//获取所有合约的持仓明细记录
int CDataCenter::GetPositionDetails(std::list<PositionDetailStru> &PositionDetailList)
{
    return m_pDataMgr->GetPositionDetails(PositionDetailList);
}

//获取持仓记录键值列表,返回持仓记录的数量
int CDataCenter::GetPositionKeyList(std::list<PositionKey> &PositionKeyList)
{
    return m_pDataMgr->GetPositionKeyList(PositionKeyList);
}

//获取持仓记录键值列表,返回持仓记录的数量
int CDataCenter::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_pDataMgr->GetPositionKeySet(PositionKeySet);
}

//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
int CDataCenter::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData)
{
    return m_pDataMgr->GetPositionData(strAccount, InstrumentID,Direction,HedgeFlag,PositionData);
}



string CDataCenter::GetTradingDay()
{
    return m_pDataMgr->GetTradingDay();
}
///获取交易所时间
unsigned long CDataCenter::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataMgr->GetExchangeTime(ExchangeID,time);
}
///获取全部报单
int CDataCenter::GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetAllOrders(outData);
}

//获取指定合约、指定方向、指定开平的挂单数量（手数）
int  CDataCenter::GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataMgr->GetWaitOrderVolume(strInstrumentID,Direction,CloseType);
}

//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
int CDataCenter::GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataMgr->GetCloseOrderVolume(strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
}

///获取全部成交单
int CDataCenter::GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->GetAllTrades(outData);
}

///获取成交统计
int CDataCenter::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return m_pDataMgr->GetAllTradeTotalDatas(outData);
}

///在成交统计查找参数rawData
int CDataCenter::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return m_pDataMgr->FindIndexFromAllTradeTotalDatas(rawData);
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
//发送组合合约深度行情数据
int CDataCenter::MakeComboDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData,int index)
{
	return m_pDataMgr->MakeComboDepthMarketData(pDepthMarketData,index);
}

int CDataCenter::InitCombRelation()
{
	std::vector<DataRtnDepthMarketData> tmp;
	std::vector<DataRtnDepthMarketData>::iterator it ;
	int ret=m_pDataMgr->InitCombRelation(tmp);
	for(it=tmp.begin();it!=tmp.end();++it)
	{
		bool ret1=m_dataSource.SaveDataTo(BID_RtnDepthMarketData, 0, 0, 0, sizeof(*it), &*it); 
		if(!ret1)
		{
			LOG_INFO("CQuotSpi::OnRtnDepthMarketData,Failed to SaveDataTo m_RefPackageCache:"
				"InstrumentID=%s,LastPrice=%g",
				it->DepthMarketDataField.InstrumentID,it->DepthMarketDataField.LastPrice);
		}
	}
	return ret;
}
SERVER_PARAM_CFG&  CDataCenter::GetServerParamCfg()
{
    return m_pDataMgr->GetServerParamCfg();
}
bool CDataCenter::IsNeedCheckCommissionRateAndMarginRate(void) 
{ 
	return m_pDataMgr->IsNeedCheckCommissionRateAndMarginRate();
}
void CDataCenter::SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck) 
{
	m_pDataMgr->SetNeedCheckCommissionRateAndMarginRate(bNeedCheck);
}
