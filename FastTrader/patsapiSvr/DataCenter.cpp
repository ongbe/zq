#include "stdafx.h"

#include <time.h>
#include "PatsApiSvr.h"
#include "DataCenter.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "PlatFormService.hpp"
#include "../inc/Module-Misc/GlobalConfigMgr.h"
//#include "ReqOnRtnTrade.h"

#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CDataCenter", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
    }while(0)

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

            toBeContinue=m_pDataMgr->SetQuotInfo(MarketData2);
            break;
        }
        case BID_RtnOrder://报单回报,更新报单队列
        {
			DataRtnOrder& NewBusiness= (DataRtnOrder&)data;

            PlatformStru_InstrumentInfo InstrumentInfo;
			ZeroMemory(&InstrumentInfo, sizeof(InstrumentInfo));
            if(GetInstrumentInfo(std::string(NewBusiness.OrderField.InstrumentID),InstrumentInfo)<0)
		        break;

            PlatformStru_OrderInfo OldOrderInfo;
            m_pDataMgr->NewOrderReady(NewBusiness.OrderField,OldOrderInfo,InstrumentInfo);

			m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            break;
		}
        case BID_RtnTrade://成交回报,更新报单队列
        {
			DataRtnTrade& NewBusiness= (DataRtnTrade&)data;

            PlatformStru_InstrumentInfo InstrumentInfo;
            if(GetInstrumentInfo(std::string(NewBusiness.TradeField.InstrumentID),InstrumentInfo)<0)
		        break;

            m_pDataMgr->NewTradeReady(NewBusiness.TradeField,InstrumentInfo);

            //m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
			m_pDataMgr->UpdatePosition_RtnTrade();

            //if(GlobalConfigManager::IsReq_OnRtnTrade())
            //{
            //    CReqOnRtnTrade::OnRtnTrade(std::string(NewBusiness.TradeField.BrokerID),
            //                                std::string(NewBusiness.TradeField.InvestorID),
            //                                std::string(NewBusiness.TradeField.InstrumentID),
            //                                (NewBusiness.TradeField.TradeType==THOST_FTDC_TRDT_CombinationDerived?true:false),
            //                                m_pDataMgr->GetRecordCountOfPositionDetail());
            //}
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
		case BID_RspQryInvestorPosition:
        {
            DataRspQryInvestorPosition& NewBusiness= (DataRspQryInvestorPosition&)data; 
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->QryPositionReach(NewBusiness.InvestorPositionField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);

            }
            break;
        }
        case BID_RspQryInvestorPositionDetail://请求查询投资者持仓明细响应
            break;
        //case BID_RspQryInvestorPositionComb://请求查询投资者组合持仓明细响应
		//	break;
        case BID_RspQryTrade:
        {
            DataRspQryTrade& NewBusiness= (DataRspQryTrade&)data;
            if(NewBusiness.RspInfoField.ErrorID == 0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->QryTradeReach(NewBusiness.TradeField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);
				//m_PlatFormService->SetSubscribeStatus(NewBusiness.TradeField.InstrumentID,S_STATUS_TRADE);
            }
            break;
        }
        break;
        case BID_RspQryOrder://请求查询报单响应
        {
            DataRspQryOrder& NewBusiness= (DataRspQryOrder&)data;
            if(NewBusiness.RspInfoField.ErrorID==0||NewBusiness.RspInfoField.ErrorID==CustomErrorID)
            {
                m_pDataMgr->QryOrderReach(NewBusiness.OrderField,NewBusiness.RspInfoField.ErrorID,NewBusiness.bIsLast);


                m_PlatFormService->SetSubscribeStatus(NewBusiness.OrderField.InstrumentID,S_STATUS_ORDER);
            }
            break;        
        }
		case BID_RspOrderInsert1://报单录入回报
			{
				DataRspOrderInsert1& NewBusiness= (DataRspOrderInsert1&)data;
				if(NewBusiness.RspInfoField.ErrorID!=0)
				{
					m_PlatFormService->SetSubscribeStatus(NewBusiness.InputOrderField.InstrumentID,S_STATUS_ORDERINSERT);
				}
			}
			break;  
		case BID_RspOrderInsert2://报单录入回报
			{
				DataRspOrderInsert2& NewBusiness= (DataRspOrderInsert2&)data;
				if(NewBusiness.RspInfoField.ErrorID!=0)
				{
					m_PlatFormService->SetSubscribeStatus(NewBusiness.InputOrderField.InstrumentID,S_STATUS_ORDERINSERT);
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
        break;
        case BID_RspQryInstrumentMarginRate://请求查询合约保证金率响应
            {
    //            DataRspQryInstrumentMarginRate& NewBusiness= (DataRspQryInstrumentMarginRate&)data;
    //            if(NewBusiness.RspInfoField.ErrorID==0)
    //            {
    //                std::string strInstrument(NewBusiness.InstrumentMarginRateField.InstrumentID);
    //                PlatformStru_InstrumentInfo InstrumentInfo;
    //                if(GetInstrumentInfo(strInstrument,InstrumentInfo)<0)
		  //              break;

    //                PlatformStru_InstrumentMarginRate MarginRate;
    //                MarginRate=NewBusiness.InstrumentMarginRateField;
    //                m_pDataMgr->SetMarginRate(NewBusiness.InstrumentMarginRateField.InstrumentID,MarginRate);


    //                if(m_pDataMgr->UpdateOrderFreezeMargin_RspQryMarginRate(strInstrument,InstrumentInfo))
    //                    m_pDataMgr->DynamicUpdateTradingAccount_Freeze();

    //                m_pDataMgr->UpdatePositionDetailByMarginRate(MarginRate,InstrumentInfo);

				//	m_pDataMgr->UpdatePositionFreezeFieldsByOrders_RspQryRate_Internal(std::string(MarginRate.InstrumentID));

    //                if(m_pDataMgr->UpdatePositionByMarginRate_RspQryRate(MarginRate))
    //                    m_pDataMgr->DynamicUpdateTradingAccount_RspQryInstrumentMarginRate();
    //            }
				//else if(NewBusiness.RspInfoField.ErrorID==CustomErrorID)
				//{
				//	//查无记录
				//	m_pDataMgr->IncrementReqMarginRateError(NewBusiness.InstrumentMarginRateField.InstrumentID);
				//}
            }
            break;
        case BID_RspQryInstrumentCommissionRate://请求查询合约手续费率响应
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
			//PATS不支持获取交易所时间
            //DataRspUserLogin& NewBusiness= (DataRspUserLogin&)data;
            //if(NewBusiness.RspInfoField.ErrorID==0 && NewBusiness.bTrader)
            //{
            //    ExchangeTime exchangeTime;
            //    m_pDataMgr->GetExchangeTime(exchangeTime);
            //    exchangeTime.CurTickCount = NewBusiness.CurTickCount;
            //    memcpy(&exchangeTime.TradingDay, &NewBusiness.UserLoginField.TradingDay, sizeof(exchangeTime.TradingDay));
            //    memcpy(&exchangeTime.SHFETime, &NewBusiness.UserLoginField.SHFETime, sizeof(exchangeTime.SHFETime));
            //    memcpy(&exchangeTime.DCETime, &NewBusiness.UserLoginField.DCETime, sizeof(exchangeTime.DCETime));
            //    memcpy(&exchangeTime.CZCETime, &NewBusiness.UserLoginField.CZCETime, sizeof(exchangeTime.CZCETime));
            //    memcpy(&exchangeTime.FFEXTime, &NewBusiness.UserLoginField.FFEXTime, sizeof(exchangeTime.FFEXTime));
            //    m_pDataMgr->SetExchangeTime(exchangeTime);
            //}
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
    std::map<GUIModuleID,GuiCallBackFunc> CallbackFuncList;
    std::map<GUIModuleID,GuiCallBackFunc>::iterator it_callback;

    if(m_pDataMgr->GetSubscribeBusinessData(data.BID,CallbackFuncList)==0)
    {
        int i;
        for(i=0,it_callback=CallbackFuncList.begin();it_callback!=CallbackFuncList.end();it_callback++,i++)
        {
            if(it_callback->second)
                (it_callback->second)(it_callback->first,data);
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

//获取持仓记录键值列表,返回持仓记录的数量
int CDataCenter::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
{
    return m_pDataMgr->GetPositionKeySet(PositionKeySet);
}

//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
int CDataCenter::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
{
    return m_pDataMgr->GetPositionData(strAccount, InstrumentID,Direction,HedgeFlag,PositionData);
}

int CDataCenter::GetPositions(std::vector<PlatformStru_Position>& outData)
{
    return m_pDataMgr->GetPositions(outData);
}

int CDataCenter::GetPositions2(const std::string& strInstrument,std::set<long>& setFTID,std::vector<PlatformStru_Position>& outData,long& lastseq)
{
    return m_pDataMgr->GetPositions2(strInstrument,setFTID,outData,lastseq);
}
int CDataCenter::GetPositions3(const std::string& strInstrument3,std::vector<PlatformStru_Position>& outData,long& lastseq)
{
	return m_pDataMgr->GetPositions3(strInstrument3,outData,lastseq);
}
string CDataCenter::GetTradingDay(const string& ExchangeID)
{
    return m_pDataMgr->GetTradingDay(ExchangeID);
}
///获取交易所时间
unsigned long CDataCenter::GetExchangeTime(const string& ExchangeID,string& time)
{
    return m_pDataMgr->GetExchangeTime(ExchangeID,time);
}
///获取全部报单
int CDataCenter::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetAllOrders(outData);
}
int CDataCenter::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetAllOrders2(strInstrument,outData);
}
bool CDataCenter::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetOrder(inOrderKey,outData);
}
///获取全部已触发的报单
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

///获取未成交报单
int CDataCenter::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetWaitOrders(outData);
}
///获取指定的未成交报单
bool CDataCenter::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetWaitOrder(orderkey, outData);
}
int CDataCenter::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetWaitOrders2(strInstrument, outData);
}
///获取已成交报单
int CDataCenter::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTradedOrders(outData);
}
///获取已成交报单
int CDataCenter::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetTradedOrders2(strInstrument, outData);
}
///获取指定已成交报单
bool CDataCenter::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetTradedOrder(orderkey, outData);
}
///获取已撤单和错误报单
int CDataCenter::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetCanceledOrders(outData);
}
///获取已撤单和错误报单
int CDataCenter::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
{
    return m_pDataMgr->GetCanceledOrders2(strInstrument, outData);
}
///获取指定已撤单和错误报单
bool CDataCenter::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
{
    return m_pDataMgr->GetCanceledOrder(orderkey, outData);
}

//获取指定合约、指定方向、指定开平的挂单数量（手数）
int  CDataCenter::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag)
{
    return m_pDataMgr->GetWaitOrderVolume(strAccount, strInstrumentID,Direction,CloseType);
}

//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
int CDataCenter::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
{
    return m_pDataMgr->GetCloseOrderVolume(strAccount,strInstrumentID, Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume);
}

bool CDataCenter::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
{
    return m_pDataMgr->GetTradeInfo(tradekey, outData);
}
///获取全部成交单
int CDataCenter::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->GetAllTrades(outData);
}
///获取成交统计
int CDataCenter::GetAllTradeTotalDatas(vector<TotalInfo>& outData)
{
    return -1;//m_pDataMgr->GetAllTradeTotalDatas(outData);
}
///获取指定成交信息
int CDataCenter::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
{
    return m_pDataMgr->GetTradesOfInstrument(strInstrument,outData);
}
///在成交统计查找参数rawData
int CDataCenter::FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )
{
    return -1;//m_pDataMgr->FindIndexFromAllTradeTotalDatas(rawData);
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

SERVER_PARAM_CFG&  CDataCenter::GetServerParamCfg()
{
    return m_pDataMgr->GetServerParamCfg();
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

int CDataCenter::GetPositionCombInfo( long lFTID, PlatformStru_PositionCombDetail& outData )
{
	return m_pDataMgr->GetPositionCombInfo(lFTID, outData);
}

