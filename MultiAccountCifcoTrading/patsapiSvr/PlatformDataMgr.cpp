#include "stdafx.h"

#include "wx/file.h"
#include "wx/textfile.h"

#include <vector>
#include <string>
#include <map>

#include "PatsApiSvr.h"
#include "PlatFormService.hpp"
#include "PlatformDataMgr.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/tools_util.h"
#pragma warning(disable:4996)
#if 1
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("CPlatformDataMgr", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif
namespace Pats
{
	CPlatformDataMgr::CPlatformDataMgr(CPlatFormService* pHost):
	m_PlatFormService(pHost)
	{
		InitializeCriticalSection(&m_CS);
		m_pWriteLog=new zqWriteLog(LOGMODE_LOCALFILE,"PlatformDataMgr.log");
	}

	CPlatformDataMgr::~CPlatformDataMgr(void)
	{
		if(m_pWriteLog)
		{
			delete m_pWriteLog;
			m_pWriteLog=NULL;
		}
		DeleteCriticalSection(&m_CS);
	}


	///设置交易资金账户信息
	void CPlatformDataMgr::SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)
	{
		ASSERT("The method is not implemented in pats.");
	}
	///设置交易资金账户信息
	void CPlatformDataMgr::SetTradingAccount(const DataRspQryTradingAccount& outData)
	{
		ASSERT("The method is not implemented in pats.");
	}
	int CPlatformDataMgr::GetTradingAccountAvailable(double& fAvailable)
	{
		ASSERT("The method is not implemented in pats.");
		return 0; 
	}
	int CPlatformDataMgr::GetTradingAccountWithdrawQuota(double& fWithdrawQuota)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	int CPlatformDataMgr::GetTradingAccountID(char* AccountID)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;  
	}
	///设置指定合约信息
	void CPlatformDataMgr::SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData)
	{
		LOG_INFO("CPlatformDataMgr::SetInstrumentInfo");
		EnterCriticalSection(&m_CS);
		m_allInstruments[InstrumentID] = inData;
		m_allInstrumentList.insert(InstrumentID);
		m_allProductID[inData.ProductID].insert(InstrumentID);
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::SetInstrumentInfo-end");
	}
	///获取指定合约信息
	int CPlatformDataMgr::GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentInfo");
		int ret;
		EnterCriticalSection(&m_CS);
		ret=GetInstrumentInfo_Internal(InstrumentID,outData);
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetInstrumentInfo-end");
		return 0;
	}

	///获取指定合约信息，内部调用，不用加锁
	int CPlatformDataMgr::GetInstrumentInfo_Internal(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentInfo_Internal");
		int ret;
		std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
		it =  m_allInstruments.find(InstrumentID);
		memset(&outData,0,sizeof(outData));
		if(it==m_allInstruments.end())
		{
			ret=-1;
		}
		else 
		{
			outData = it->second;
			ret=0;
		}
		LOG_INFO("CPlatformDataMgr::GetInstrumentInfo_Internal-end");
		return ret;
	}

	///获取合约的产品类型，失败返回-1
	char CPlatformDataMgr::GetProductClassType(const std::string& InstrumentID)
	{
		//LOG_INFO("CPlatformDataMgr::GetProductClassType");
		char ProductClassType;
		std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
		EnterCriticalSection(&m_CS);
		it =  m_allInstruments.find(InstrumentID);
		if(it==m_allInstruments.end())
			ProductClassType=-1;
		else 
			ProductClassType = it->second.ProductClass;
		LeaveCriticalSection(&m_CS);
		//LOG_INFO("CPlatformDataMgr::GetProductClassType-end");
		return ProductClassType;
	}
	///获取合约列表
	int CPlatformDataMgr::GetInstrumentList(std::vector<GroupInfo> &outData)
	{
		if(m_allGroupInfo.empty())
		{
			std::map<std::string,int> GroupName2ID;
			std::map<std::string,int>::iterator GroupName2ID_it;
			PlatformStru_InstrumentInfo instInfo;
			std::set<std::string>::iterator it=m_allInstrumentList.begin();
			for(it;it!=m_allInstrumentList.end();++it)
			{
				if(GetInstrumentInfo_Internal(*it,instInfo)==0)
				{
					GroupName2ID_it=GroupName2ID.find(instInfo.ExchangeID);
					if(GroupName2ID_it==GroupName2ID.end())
					{
						GroupInfo ginfo;
						ginfo.GroupName=instInfo.ExchangeID;
						ginfo.InstrVec.push_back(InstrumentInfo(instInfo.InstrumentID,instInfo.InstrumentName));
						m_allGroupInfo.push_back(ginfo);
						GroupName2ID[instInfo.ExchangeID]=m_allGroupInfo.size()-1;
					}
					else
					{
						m_allGroupInfo[GroupName2ID_it->second].InstrVec.push_back(InstrumentInfo(instInfo.InstrumentID,instInfo.InstrumentName));
					}
				}
			}
		}
		outData=m_allGroupInfo;
		return 0;
	}
	///获取合约列表
	int CPlatformDataMgr::GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentList");
		EnterCriticalSection(&m_CS);
		if(ExchangeID.empty())
			outData = m_allInstrumentList;
		else
		{
			outData.clear();
			std::set<std::string>::iterator it=m_allInstrumentList.begin();
			PlatformStru_InstrumentInfo instInfo;
			for(it;it!=m_allInstrumentList.end();++it)
			{
				if(GetInstrumentInfo_Internal(*it,instInfo)==0&&ExchangeID==instInfo.ExchangeID)
				{
					outData.insert(*it);
				}
			}
		}
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetInstrumentList-end");
		return outData.size();
	}
	//添加主力合约列表
	int CPlatformDataMgr::AddMainInstrumentList(std::string instrument)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	///获取合约列表
	int CPlatformDataMgr::GetMainInstrumentList(std::set<std::string> &outData)
	{
		LOG_INFO("CPlatformDataMgr::GetMainInstrumentList");
		EnterCriticalSection(&m_CS);
		outData=m_allInstrumentList;
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetMainInstrumentList-end");
		return outData.size();
	}
	///设置合约容差列表
	void CPlatformDataMgr::SetInstrumentVarietyMap(map<string, string>& inData)
	{
		ASSERT("The method is not implemented in pats.");
	}

	// 获取合约容差列表
	int CPlatformDataMgr::GetInstrumentVarietyMap(map<string, string>& outData)
	{
		outData.clear();
		return 0;
	}

	///获取指定品种的合约列表,返回合约个数
	int CPlatformDataMgr::GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID");
		int ret;
		std::map<std::string, std::set<std::string> >::iterator it;
		EnterCriticalSection(&m_CS);
		it=m_allProductID.find(ProductID);
		if(it==m_allProductID.end()) 
			ret=-1;
		else
		{
			outData = it->second;
			ret=outData.size();
		}
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetInstrumentListByProductID-end");
		return ret;
	}

	///获取合约品种代码
	int CPlatformDataMgr::GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID)
	{
		LOG_INFO("CPlatformDataMgr::GetProductID");
		EnterCriticalSection(&m_CS);
		outData=m_allProductID;
		if(!ExchangeID.empty())
		{
			PlatformStru_InstrumentInfo instInfo;
			std::map<std::string, std::set<std::string> >::iterator it;
			for(it=outData.begin();it!=outData.end();)
			{
				if(it->second.size()==0||
					GetInstrumentInfo_Internal(*(it->second.begin()),instInfo)!=0||
					ExchangeID!=instInfo.ExchangeID)
				{
					outData.erase(it++);
				}
				else
					++it;
			}
		}
		LeaveCriticalSection(&m_CS);
		return outData.size(); 
		LOG_INFO("CPlatformDataMgr::GetProductID-end");
	}


	///获取合约乘数，成功返回合约乘数，失败返回-1
	int CPlatformDataMgr::GetInstrumentMultiple(const std::string& InstrumentID)
	{
		LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple");
		int Multiple;
		std::map<std::string,  PlatformStru_InstrumentInfo>::iterator it;
		EnterCriticalSection(&m_CS);
		it =  m_allInstruments.find(InstrumentID);
		if(it==m_allInstruments.end())
			Multiple=-1;
		else 
			Multiple= it->second.VolumeMultiple;
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetInstrumentMultiple-end");
		return Multiple;
	}

	///获取合约手续费率
	int CPlatformDataMgr::GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetCommissionRate");
		int ret;
		EnterCriticalSection(&m_CS);
		ret=GetCommissionRate_Internal(InstrumentID,outData);
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetCommissionRate-end");
		return ret;
	}

	///获取合约手续费率，内部调用，不用加锁
	int CPlatformDataMgr::GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal");
		int ret=0;
		ZeroMemory(&outData,sizeof(PlatformStru_InstrumentCommissionRate));
		strcpy(outData.InstrumentID,InstrumentID.c_str());
		LOG_INFO("CPlatformDataMgr::GetCommissionRate_Internal-end");
		return ret;
	}

	///设置合约手续费率
	void CPlatformDataMgr::SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)
	{
		ASSERT("The method is not implemented in pats.");
	}

	///设置正在查询手续费率的合约,用于查询返回时对应
	void CPlatformDataMgr::SetReqCommissionRateInstrumentID(const std::string& InstrumentID)
	{
		ASSERT("The method is not implemented in pats.");
	}

	///获取正在查询手续费率的合约
	void CPlatformDataMgr::GetReqCommissionRateInstrumentID(std::string& outData)
	{
		ASSERT("The method is not implemented in pats.");
	}

	///获取合约保证金率
	int CPlatformDataMgr::GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetMarginRate");
		int ret;
		EnterCriticalSection(&m_CS);
		ret=GetMarginRate_Internal(InstrumentID,outData);
		LeaveCriticalSection(&m_CS);
		LOG_INFO("CPlatformDataMgr::GetMarginRate-end");
		return ret; 
	}

	///获取合约保证金率，内部调用，不用加锁
	int CPlatformDataMgr::GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
	{
		LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal");
		int ret=0;
		ZeroMemory(&outData,sizeof(PlatformStru_InstrumentMarginRate));
		strcpy(outData.InstrumentID,InstrumentID.c_str());
		LOG_INFO("CPlatformDataMgr::GetMarginRate_Internal-end");
		return ret; 
	}

	///设置合约保证金率
	void CPlatformDataMgr::SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)
	{
		ASSERT("The method is not implemented in pats.");
	}



	//原始的成交回报到来
	void CPlatformDataMgr::QryTradeReach( const PlatformStru_TradeInfo& TradeInfo,const int ErrID,const bool bIsLast)
	{

		LOG_INFO("CPlatformDataMgr::QryTradeReach");

		EnterCriticalSection(&m_CS);
		m_TradeData.QryRltReach_Internal(TradeInfo,ErrID,bIsLast);
		LeaveCriticalSection(&m_CS);


		LOG_INFO("CPlatformDataMgr::QryTradeReach-end");
	}

	//新的成交回报到来，将成交单放到m_TradesListWaitForAppend中，然后调用ProcessNewTradeList处理
	void CPlatformDataMgr::NewTradeReady( PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		LOG_INFO("CPlatformDataMgr::NewTradeReady");

		Lock_CPlatformDataMgr;
		m_TradeData.NewReach_Internal(TradeInfo);
		Unlock_CPlatformDataMgr;
		LOG_INFO("CPlatformDataMgr::NewTradeReady-end");
	}
	bool CPlatformDataMgr::GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)
	{
		bool brlt;
		EnterCriticalSection(&m_CS);
		PlatformStru_TradeInfo *pTrade;
		if(m_TradeData.GetValue_Internal(pTrade,tradekey))
		{
			outData=*pTrade;
			brlt=true;
		}
		else
		{
			brlt=false;
		}
		LeaveCriticalSection(&m_CS);
		return brlt;
	}
	///获取全部成交单
	int CPlatformDataMgr::GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)
	{
		int irlt;
		Lock_CPlatformDataMgr;

		irlt=m_TradeData.GetAllAboutInstrument2_Internal(strInstrument,outData);

		Unlock_CPlatformDataMgr;

		return irlt;
	}
	int CPlatformDataMgr::GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData)
	{
		int nRet = 0;
		Lock_CPlatformDataMgr;
		std::map<long, PlatformStru_OrderInfo*>::iterator it_FTID = m_OrderData.m_all2.find(lFTID);
		if ( it_FTID != m_OrderData.m_all2.end())
		{
			outData = *(it_FTID->second);
		}
		else
		{
			nRet = -1;
		}
		Unlock_CPlatformDataMgr;

		return nRet;
	}
	int CPlatformDataMgr::GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData)
	{
		int nRet = 0;
		Lock_CPlatformDataMgr;
		std::map<OrderKey, PlatformStru_OrderInfo*>::iterator it_all = m_OrderData.m_all.find(key);
		if ( it_all != m_OrderData.m_all.end())
		{
			outData = *(it_all->second);
		}
		else
		{
			nRet = -1;
		}
		Unlock_CPlatformDataMgr;

		return nRet;
	}
	///获取全部成交单
	int CPlatformDataMgr::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)
	{
		int irlt;
		EnterCriticalSection(&m_CS);
		irlt=m_TradeData.GetAll2_Internal(outData);
		LeaveCriticalSection(&m_CS);
		return irlt;
	}
	//从成交中取手续费。如果没有费率，作0处理
	void CPlatformDataMgr::GetAllCommission_Internal(double& TotalCommission)
	{
		ASSERT("The method is not implemented in pats.");
	}
	//原始报单到来。报单查询响应时调用
	void CPlatformDataMgr::QryOrderReach(const PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast)
	{
		LOG_INFO("CPlatformDataMgr::QryOrderReach");

		std::set<string> setInstruments;

		Lock_CPlatformDataMgr;

		m_OrderData.QryRltReach_Internal(OrderInfo,ErrID,bIsLast);
		Unlock_CPlatformDataMgr;
		LOG_INFO("CPlatformDataMgr::QryOrderReach-end");
	}
	//新报单到来,注意，可能是原有报单的新状态
	void CPlatformDataMgr::NewOrderReady(PlatformStru_OrderInfo& OrderInfo, 
		PlatformStru_OrderInfo& OldOrderInfo,
		const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		LOG_INFO("CPlatformDataMgr::NewOrderReady");

		PlatformStru_OrderInfo& RltOldOrderInfo=PlatformStru_OrderInfo::GetInvalidRecord();

		Lock_CPlatformDataMgr;

		PlatformStru_OrderInfo *pOrder;

		OrderKey OldOrderKey(OrderInfo);
		if(m_OrderData.GetValue_Internal(pOrder,OldOrderKey))
			OldOrderInfo=*pOrder;
		else OldOrderInfo=PlatformStru_OrderInfo::GetInvalidRecord();

		//CalcuOrderFreezeMargin_Internal(OrderInfo,InstrumentInfo);
		//CalcuOrderFreezeCommission_Internal(OrderInfo,InstrumentInfo);

		m_OrderData.NewReach_Internal(OrderInfo);

		Unlock_CPlatformDataMgr;

		LOG_INFO("CPlatformDataMgr::NewOrderReady-end");
	}

	//更新所有报单中的冻结保证金，返回合约编号是否有报单，决定是否需要更新资金面板。查询保证金率响应时调用
	bool CPlatformDataMgr::UpdateOrderFreezeMargin_RspQryMarginRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		ASSERT("The method is not implemented in pats.");
		return false;
	}

	//更新所有报单中的冻结手续费，返回合约编号是否有报单，决定是否需要更新资金面板。查询手续费率响应时调用
	bool CPlatformDataMgr::UpdateOrderFreezeCommission_RspQryCommissionRate(std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		ASSERT("The method is not implemented in pats.");
		return false;
	}

	//计算报单信息中的冻结保证金
	void CPlatformDataMgr::CalcuOrderFreezeMargin_Internal(PlatformStru_OrderInfo& OrderInfo,
		const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		ASSERT("The method is not implemented in pats.");
	}

	//计算报单信息中的冻结手续费
	void CPlatformDataMgr::CalcuOrderFreezeCommission_Internal(PlatformStru_OrderInfo& OrderInfo,
		const PlatformStru_InstrumentInfo& InstrumentInfo)
	{
		ASSERT("The method is not implemented in pats.");
	}
	int CPlatformDataMgr::GetAllOrderFTIDList(std::vector<long>& vec)
	{
		vec.clear();
		Lock_CPlatformDataMgr;
		std::set<long>::iterator it_FTID = m_OrderData.m_allFTID.begin();
		for ( ; it_FTID != m_OrderData.m_allFTID.end(); ++it_FTID )
		{
			vec.push_back(*it_FTID);
		}
		Unlock_CPlatformDataMgr;

		return vec.size();
	}
	///获取全部报单
	int CPlatformDataMgr::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		int irlt;

		Lock_CPlatformDataMgr;

		irlt=m_OrderData.GetAll2_Internal(outData);

		Unlock_CPlatformDataMgr;

		return irlt;
	}
	int CPlatformDataMgr::GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		int irlt;
		EnterCriticalSection(&m_CS);
		irlt=m_OrderData.GetAllAboutInstrument2_Internal(strInstrument,outData);
		LeaveCriticalSection(&m_CS);
		return irlt;
	}
	///获取指定报单
	bool CPlatformDataMgr::GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)
	{
		bool brlt;

		Lock_CPlatformDataMgr;

		PlatformStru_OrderInfo *pOrder;
		if(m_OrderData.GetValue_Internal(pOrder,inOrderKey))
		{
			outData=*pOrder;
			brlt=true;
		}
		else
		{
			outData=PlatformStru_OrderInfo::GetInvalidRecord();
			brlt=false;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}
	///获取全部已触发的报单
	int CPlatformDataMgr::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
		outData.clear();
		for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
		{
			if(it->second==NULL) continue;
			if( it->second->ContingentCondition == THOST_FTDC_CC_Immediately ||
				it->second->ContingentCondition == THOST_FTDC_CC_Touch ||
				it->second->ContingentCondition == THOST_FTDC_CC_TouchProfit)
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}

	///获取合约相关的已触发的报单
	int CPlatformDataMgr::GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		outData.clear();

		std::set<long>* pFTIDSet;
		std::set<long>::iterator it;
		PlatformStru_OrderInfo *pOrderInfo;
		long FTID;

		if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
		{
			for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
			{
				FTID=*it;
				if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
					(pOrderInfo->ContingentCondition == THOST_FTDC_CC_Immediately ||
					pOrderInfo->ContingentCondition == THOST_FTDC_CC_Touch ||
					pOrderInfo->ContingentCondition == THOST_FTDC_CC_TouchProfit))
				{
					outData.push_back(*pOrderInfo);
				}
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}

	///获取指定已触发的报单
	bool CPlatformDataMgr::GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
	{
		bool brlt=false;

		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
		if(it!=m_OrderData.m_all.end()&&
			it->second!=NULL&&
			(   it->second->ContingentCondition == THOST_FTDC_CC_Immediately ||
			it->second->ContingentCondition == THOST_FTDC_CC_Touch ||
			it->second->ContingentCondition == THOST_FTDC_CC_TouchProfit))
		{
			outData=*(it->second);
			brlt=true;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}

	///获取未成交报单
	int CPlatformDataMgr::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
		outData.clear();
		for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
		{
			if(it->second==NULL) continue;
			if( it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
				it->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///获取未成交报单
	int CPlatformDataMgr::GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		outData.clear();

		std::set<long>* pFTIDSet;
		std::set<long>::iterator it;
		PlatformStru_OrderInfo *pOrderInfo;
		long FTID;

		if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
		{
			for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
			{
				FTID=*it;
				if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
					(pOrderInfo->ContingentCondition == THOST_FTDC_OST_PartTradedQueueing ||
					pOrderInfo->ContingentCondition == THOST_FTDC_OST_NoTradeQueueing))
				{
					outData.push_back(*pOrderInfo);
				}
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}

	///获取指定的未成交报单
	bool CPlatformDataMgr::GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
	{
		bool brlt=false;

		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
		if(it!=m_OrderData.m_all.end()&&
			it->second!=NULL&&
			(   it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
			it->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing))
		{
			outData=*(it->second);
			brlt=true;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}
	///获取已成交报单
	int CPlatformDataMgr::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		std::map<OrderKey,  PlatformStru_OrderInfo*>::iterator it;
		outData.clear();
		for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
		{
			if(it->second==NULL) continue;
			if( it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
				it->second->OrderStatus==THOST_FTDC_OST_AllTraded)
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///获取已成交报单
	int CPlatformDataMgr::GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		outData.clear();

		std::set<long>* pFTIDSet;
		std::set<long>::iterator it;
		PlatformStru_OrderInfo *pOrderInfo;
		long FTID;

		if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
		{
			for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
			{
				FTID=*it;
				if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
					(pOrderInfo->ContingentCondition == THOST_FTDC_OST_PartTradedQueueing ||
					pOrderInfo->ContingentCondition == THOST_FTDC_OST_AllTraded))
				{
					outData.push_back(*pOrderInfo);
				}
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///获取指定已成交报单
	bool CPlatformDataMgr::GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
	{
		bool brlt=false;

		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
		if(it!=m_OrderData.m_all.end()&&
			it->second!=NULL&&
			(   it->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
			it->second->OrderStatus==THOST_FTDC_OST_AllTraded))
		{
			outData=*(it->second);
			brlt=true;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}
	///获取已撤单和错误报单
	int CPlatformDataMgr::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it;
		outData.clear();
		for(it=m_OrderData.m_all.begin();it!=m_OrderData.m_all.end();it++)
		{
			if(it->second==NULL) continue;
			if( it->second->OrderStatus==THOST_FTDC_OST_Canceled||
				it->second->OrderStatus==THOST_FTDC_OST_ERROE)
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///获取已撤单和错误报单
	int CPlatformDataMgr::GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)
	{
		Lock_CPlatformDataMgr;

		outData.clear();

		std::set<long>* pFTIDSet;
		std::set<long>::iterator it;
		PlatformStru_OrderInfo *pOrderInfo;
		long FTID;

		if(m_OrderData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
		{
			for(it=pFTIDSet->begin();it!=pFTIDSet->end();it++)
			{
				FTID=*it;
				if(m_OrderData.GetValue2_Internal(pOrderInfo,FTID)&&
					(pOrderInfo->ContingentCondition == THOST_FTDC_OST_Canceled ||
					pOrderInfo->ContingentCondition == THOST_FTDC_OST_ERROE))
				{
					outData.push_back(*pOrderInfo);
				}
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	///获取指定已撤单和错误报单
	bool CPlatformDataMgr::GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)
	{
		bool brlt=false;

		Lock_CPlatformDataMgr;

		std::map<OrderKey,PlatformStru_OrderInfo*>::iterator it=m_OrderData.m_all.find(orderkey);
		if(it!=m_OrderData.m_all.end()&&
			it->second!=NULL&&
			(   it->second->OrderStatus==THOST_FTDC_OST_Canceled||
			it->second->OrderStatus==THOST_FTDC_OST_ERROE))
		{
			outData=*(it->second);
			brlt=true;
		}

		Unlock_CPlatformDataMgr;

		return brlt;
	}

	//获取指定合约、指定方向、指定开平的挂单数量（手数）
	int  CPlatformDataMgr::GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType, const char HedgeFlag)
	{
		int tnVolume = 0;
		std::map<long,  PlatformStru_OrderInfo*>::iterator iter;
		std::set<long>::iterator itFTID;
		std::map<std::string,std::set<long>>::iterator it_Index2;

		if(strInstrumentID.empty())
			return 0;

		Lock_CPlatformDataMgr;

		it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
		if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
		{
			for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
			{
				iter=m_OrderData.m_all2.find(*itFTID);
				if(iter!=m_OrderData.m_all2.end()&&
					iter->second!=NULL&&
					iter->second->Direction == Direction &&
					iter->second->CombOffsetFlag[0] == CloseType&&
					iter->second->CombHedgeFlag[0]==HedgeFlag&&
					(iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
					iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)&&
					strAccount==std::string(iter->second->szAccount))
					tnVolume += iter->second->VolumeTotal;
			}
		}

		Unlock_CPlatformDataMgr;

		return tnVolume;
	}



	//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败
	//成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
	int  CPlatformDataMgr::GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
	{
		LOG_INFO("CPlatformDataMgr::GetCloseOrderVolume");
		int ret;

		Lock_CPlatformDataMgr;

		ret=GetCloseOrderVolume_Internal(strAccount,strInstrumentID,Direction,CloseVolume,CloseTodayVolume,CloseYesterdayVolume,HedgeFlag);

		Unlock_CPlatformDataMgr;

		LOG_INFO("CPlatformDataMgr::GetCloseOrderVolume-end");
		return ret;
	}

	//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。内部调用，不加锁
	//成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
	int  CPlatformDataMgr::GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)
	{
		CloseVolume=CloseTodayVolume=CloseYesterdayVolume=0;

		std::map<std::string,std::set<long>>::iterator it_Index2;
		std::set<long>::iterator itFTID;
		std::map<long,  PlatformStru_OrderInfo*>::iterator iter;

		if(strInstrumentID.empty()) return -1;

		it_Index2=m_OrderData.m_allIndex2.find(strInstrumentID);
		if(it_Index2!=m_OrderData.m_allIndex2.end()&&!it_Index2->second.empty())
		{
			for(itFTID=it_Index2->second.begin();itFTID!=it_Index2->second.end();itFTID++)
			{
				iter=m_OrderData.m_all2.find(*itFTID);
				if(iter!=m_OrderData.m_all2.end()&&
					iter->second!=NULL&&
					iter->second->Direction == Direction &&
					iter->second->CombHedgeFlag[0]==HedgeFlag&&
					(iter->second->OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
					iter->second->OrderStatus==THOST_FTDC_OST_NoTradeQueueing)&&
					strAccount==std::string(iter->second->szAccount))
				{
					if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_Close||
						iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_ForceClose||
						iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_LocalForceClose)
						CloseVolume += iter->second->VolumeTotal;
					if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
						CloseTodayVolume += iter->second->VolumeTotal;
					if(iter->second->CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday)
						CloseYesterdayVolume += iter->second->VolumeTotal;
				}
			}
		}
		return 0;
	}
	int CPlatformDataMgr::GetPositions(std::vector<PlatformStru_Position>& outData)
	{
		outData.clear();

		Lock_CPlatformDataMgr;

		std::map<PositionKey,PlatformStru_Position*>::iterator it = m_PositionData.m_all.begin();
		for ( ; it != m_PositionData.m_all.end(); ++it )
		{
			if ( it->second!=NULL &&
				it->second->Position > 0 )
			{
				outData.push_back(*(it->second));
			}
		}

		Unlock_CPlatformDataMgr;

		return outData.size();
	}
	int CPlatformDataMgr::GetPositions3(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData,long& lastseq)
	{
		int rlt;

		outData.clear();

		Lock_CPlatformDataMgr;

		if(lastseq==m_PositionData.GetUpdateSeq())
		{
			rlt=1;
		}
		else
		{
			std::set<long>* pFTIDSet;
			if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
			{
				PlatformStru_Position *pData;
				std::set<long>::iterator itFTID;
				for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
				{
					if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&
						pData->Position>0)
					{
						outData.push_back(*pData);
					}
				}
			}

			lastseq=m_PositionData.GetUpdateSeq();
			rlt=0;
		}

		Unlock_CPlatformDataMgr;

		return rlt;
	}
	int CPlatformDataMgr::GetPositions2(const std::string& strInstrument,std::set<long>& setFTID,std::vector<PlatformStru_Position>& outData,long& lastseq)
	{
		int rlt;

		outData.clear();

		Lock_CPlatformDataMgr;

		if(lastseq==m_PositionData.GetUpdateSeq())
		{
			rlt=1;
		}
		else
		{
			std::set<long>* pFTIDSet=NULL;
			if(m_PositionData.GetFTIDSetOfInstrument(pFTIDSet,strInstrument))
			{
				setFTID=(*pFTIDSet);

				PlatformStru_Position *pData=NULL;
				std::set<long>::iterator itFTID;
				for(itFTID=pFTIDSet->begin();itFTID!=pFTIDSet->end();itFTID++)
				{
					if(m_PositionData.GetValue2_Internal(pData,(*itFTID))&&
						pData->Position>0)
					{
						outData.push_back(*pData);
					}
					else
					{
						setFTID.erase(*itFTID);
					}
				}
			}
			lastseq=m_PositionData.GetUpdateSeq();
			rlt=0;
		}

		Unlock_CPlatformDataMgr;

		return rlt;
	}
	//查询的持仓记录到来
	void CPlatformDataMgr::QryPositionReach(PlatformStru_Position& PositionInfo,const int ErrID,const bool bIsLast)
	{
		LOG_INFO("CPlatformDataMgr::QryPositionReach:InstrumentID=%s",PositionInfo.InstrumentID)
			Lock_CPlatformDataMgr;
		m_PositionData.QryRltReach_Internal(PositionInfo,ErrID,bIsLast);
		Unlock_CPlatformDataMgr;

		LOG_INFO("CPlatformDataMgr::QryPositionReach-end");
	}
	void CPlatformDataMgr::UpdatePosition_RtnTrade()
	{
		LOG_INFO("CPlatformDataMgr::QryPositionReach:InstrumentID=%s",PositionInfo.InstrumentID)
			Lock_CPlatformDataMgr;
		std::vector<PlatformStru_Position> vecPosition;
		m_PlatFormService->GetAllPosition(vecPosition);
		//
		std::set<PositionKey> newset;
		for (size_t i=0;i<vecPosition.size();++i)
		{
			PositionKey thisKey(vecPosition[i]);
			newset.insert(thisKey);
		}
		std::map<PositionKey,PlatformStru_Position*>::iterator it;
		for(it=m_PositionData.m_all.begin();it!=m_PositionData.m_all.end();++it)
		{
			if(newset.find(it->first)==newset.end())
			{
				//一次最多减少一个持仓记录
				m_PositionData.DeleteRecord2_Internal(it->first);
				break;
			}
		}
		for (size_t i=0;i<vecPosition.size();++i)
		{
			m_PositionData.UpdateRecord_Internal(vecPosition[i]);
		}
		m_PositionData.NewUpdateSeq();
		Unlock_CPlatformDataMgr;

		LOG_INFO("CPlatformDataMgr::QryPositionReach-end");
	}
	bool CPlatformDataMgr::IsTodayPositionDetail(const PlatformStru_PositionDetail& PositionDetail)
	{
		ASSERT("The method is not implemented in pats.");
		return false;
	}
	bool CPlatformDataMgr::IsTodayPosition(const char *OpenDate)
	{
		ASSERT("The method is not implemented in pats.");
		return false;
	}


	//获取持仓记录键值列表,返回持仓记录的数量
	int CPlatformDataMgr::GetPositionKeySet(std::set<PositionKey> &PositionKeySet)
	{
		LOG_INFO("CPlatformDataMgr::GetPositionKeySet");
		PositionKeySet.clear();

		std::map<PositionKey,PlatformStru_Position*>::iterator it;

		EnterCriticalSection(&m_CS);

		it=m_PositionData.m_all.begin();
		for(;it!=m_PositionData.m_all.end();it++)
		{
			if(it->second->Position>0)
			{
				PositionKey tmpKey(it->first);
				PositionKeySet.insert(tmpKey);
			}
		}

		LeaveCriticalSection(&m_CS);

		LOG_INFO("CPlatformDataMgr::GetPositionKeySet-end");
		return PositionKeySet.size();
	}


	//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
	int CPlatformDataMgr::GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)
	{
		LOG_INFO("CPlatformDataMgr::GetPositionData");
		EnterCriticalSection(&m_CS);

		int bRet = 0;
		PositionKey posKey(strAccount.c_str(), InstrumentID.c_str(), Direction, HedgeFlag);
		std::map<PositionKey, PlatformStru_Position*>::iterator it_pos = m_PositionData.m_all.find(posKey);
		if(it_pos != m_PositionData.m_all.end())
		{
			PositionData = *(it_pos->second);
			bRet = 0;
		}
		else 
		{
			bRet = -1;
		}
		LeaveCriticalSection(&m_CS);

		LOG_INFO("CPlatformDataMgr::GetPositionData-end");
		return bRet;
	}

	//获取指定合约行情，成功返回0，失败返回-1
	int CPlatformDataMgr::GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
	{
		int ret;

		Lock_CPlatformDataMgr;

		ret=GetQuotInfo_Internal(InstrumentID,outData);

		Unlock_CPlatformDataMgr;

		return ret;
	}

	//获取指定合约行情，成功返回0，失败返回-1
	int CPlatformDataMgr::GetQuotInfo_Internal(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
	{
		int ret;

		std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
		if(it==m_lastQuot.end())
		{
			memset(&outData,0,sizeof(outData));
			ret=-1;
		}
		else
		{
			outData = it->second;
			ret=0;
		}

		return ret;
	}

	//设置指定合约行情，设置的行情更新，则返回true；否则返回false
	//考虑组合合约的行情更新
	bool CPlatformDataMgr::SetQuotInfo(const PlatformStru_DepthMarketData& inData)
	{
		std::map<std::string,  PlatformStru_DepthMarketData>::iterator it = m_lastQuot.find(std::string(inData.InstrumentID));
		bool toBeContinue=true;

		EnterCriticalSection(&m_CS);

		if(it==m_lastQuot.end())
		{
			m_lastQuot[std::string(inData.InstrumentID)] = inData;
		}
		else
		{
			PlatformStru_DepthMarketData& OldBusiness= it->second;
			//比较行情的时间，丢掉老行情
			if (memcmp(inData.UpdateTime, OldBusiness.UpdateTime, sizeof(OldBusiness.UpdateTime))<0)
				toBeContinue=false;
			if(toBeContinue)
			{
				m_OldQuot[std::string(inData.InstrumentID)]=m_lastQuot[std::string(inData.InstrumentID)];
				m_lastQuot[std::string(inData.InstrumentID)] = inData;
			}
			else
			{
				LOG_INFO("CDataCenter::doProcessBusinessData:Old BID_RtnDepthMarketData InstrumentID=[%s],OldUpdateTime=[%s],NewUpdateTime=[%s]",
					inData.InstrumentID,
					OldBusiness.UpdateTime,
					inData.UpdateTime);
			}
		}
		LeaveCriticalSection(&m_CS);

		return toBeContinue;

	}
	void CPlatformDataMgr::ConvertComboInstrumentID(const std::string strComboID, std::string& strInstrumentID1, std::string& strInstrumentID2)
	{
		char strTemp[128];
		char strCode1[128];
		char strCode2[128];
		ZeroMemory(strCode1, 128);
		ZeroMemory(strCode2, 128);
		sscanf_s(strComboID.c_str(), "%s %[^&]&%s", strTemp, sizeof(strTemp)-1, strCode1, sizeof(strCode1)-1, strCode2, sizeof(strCode2)-1);
		strInstrumentID1 = strCode1;
		strInstrumentID2 = strCode2;
	}
	//获取指定合约的最新价
	double CPlatformDataMgr::GetCurPrice (const std::string& InstrumentID)
	{
		double ret=0.0;
		//组合合约
		char clstype=GetProductClassType(InstrumentID);
		if(clstype==THOST_FTDC_PC_Combination)
		{
			std::string id1,id2;
			ConvertComboInstrumentID(InstrumentID,id1,id2);
			double prc1=GetCurPrice(id1);
			double prc2=GetCurPrice(id2);
			ret= prc1-prc2;
		}
		else if(clstype==THOST_FTDC_PC_Futures)
		{
			EnterCriticalSection(&m_CS);
			std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_lastQuot.find(InstrumentID);
			if(it!=m_lastQuot.end())
			{
				if(it->second.Volume>0) ret= it->second.LastPrice;
				else ret= it->second.PreSettlementPrice;
			}
			LeaveCriticalSection(&m_CS);
		}
		return ret;
	}

	//获取指定合约旧行情，成功返回0，失败返回-1
	int CPlatformDataMgr::GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)
	{
		int ret;

		EnterCriticalSection(&m_CS);

		std::map<std::string,  PlatformStru_DepthMarketData>::iterator it =  m_OldQuot.find(InstrumentID);
		if(it==m_OldQuot.end())
		{
			memset(&outData,0,sizeof(outData));
			ret=-1;
		}
		else
		{
			outData = it->second;
			ret=0;
		}

		LeaveCriticalSection(&m_CS);

		return ret;
	}
	string CPlatformDataMgr::GetTradingDay(const string& ExchangeID)
	{
		ASSERT("The method is not implemented in pats.");
		return string("");
	}

	//获取交易所时间信息
	unsigned long CPlatformDataMgr::GetExchangeTime(const string& ExchangeID,string& time)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	int CPlatformDataMgr::GetExchangeTime(ExchangeTime& outData)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	//设置交易所时间信息
	void CPlatformDataMgr::SetExchangeTime(const ExchangeTime& inData)
	{
		ASSERT("The method is not implemented in pats.");
	}



	//获取结算信息，成功返回0，失败返回-1
	int CPlatformDataMgr::GetLastSettlementInfo(std::string& outData)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}

	//清除结算信息
	void CPlatformDataMgr::ClearLastSettlementInfo(void)
	{
		ASSERT("The method is not implemented in pats.");
	}

	//添加结算信息
	void CPlatformDataMgr::AppendLastSettlementInfo(const std::string& inData,bool bLast)
	{
		ASSERT("The method is not implemented in pats.");
	}


	//业务定制，成功返回0，失败返回-1
	int CPlatformDataMgr::SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback)
	{
		int ret;

		if(BID>=BID_MAX || GID>=GID_MAX || callback==NULL)
			return -1;


		EnterCriticalSection(&m_CS);

		ret=0;
		try
		{
			m_subscribeInfo[BID][GID]=callback;
		}
		catch(...)
		{
			ret=-2;
		}
		LeaveCriticalSection(&m_CS);

		return ret;
	}


	///取消业务数据的定制，成功返回0，失败返回-1
	int CPlatformDataMgr::UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID)
	{
		int ret;

		if(BID>=BID_MAX || GID>=GID_MAX)
			return -1;

		EnterCriticalSection(&m_CS);

		ret=0;

		std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
		if(it1==m_subscribeInfo.end())
			ret=-3;
		else
		{
			std::map<GUIModuleID, GuiCallBackFunc>::iterator it2 = it1->second.find(GID);
			if(it2==it1->second.end())
				ret=-3;
			else
			{
				it1->second.erase(it2);
				if(it1->second.empty())
					m_subscribeInfo.erase(it1);
				ret=0;
			}
		}
		LeaveCriticalSection(&m_CS);

		return ret;
	}


	///获取业务定制信息，成功返回0，失败返回-1
	int CPlatformDataMgr::GetSubscribeBusinessData(const BusinessID BID, std::map<GUIModuleID,GuiCallBackFunc>& outData)
	{
		int ret=0;
		outData.clear();

		EnterCriticalSection(&m_CS);

		std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >::iterator it1=m_subscribeInfo.find(BID);
		if(it1==m_subscribeInfo.end())
			ret=-1;
		else
		{
			outData=it1->second;
			ret=0;
		}
		LeaveCriticalSection(&m_CS);

		return ret;
	}

	SERVER_PARAM_CFG&  CPlatformDataMgr::GetServerParamCfg()
	{
		return m_ServerParamCfg;
	}

	void CPlatformDataMgr::ClearMapQryRltOrders()
	{
		Lock_CPlatformDataMgr;
		m_OrderData.WillQry_Internal(std::string(""));
		Unlock_CPlatformDataMgr;
	}

	//清空查询成交的map
	void CPlatformDataMgr::ClearMapQryRltTrades()
	{
		Lock_CPlatformDataMgr;
		m_TradeData.WillQry_Internal(std::string(""));
		Unlock_CPlatformDataMgr;
	}

	//清空查询持仓明细的map
	void CPlatformDataMgr::ClearMapQryRltPositionDetails(std::string& strQryInstrument)
	{
		ASSERT("The method is not implemented in pats.");
	}

	//清空查询持仓的map
	void CPlatformDataMgr::ClearMapQryRltPositions(std::string& strQryInstrument)
	{
		Lock_CPlatformDataMgr;
		m_PositionData.WillQry_Internal(strQryInstrument);
		Unlock_CPlatformDataMgr;
	}

	//清空查询组合持仓的map
	void CPlatformDataMgr::ClearMapQryRltPositionCombs(std::string& strQryInstrument)
	{
		ASSERT("The method is not implemented in pats.");
	}
	int CPlatformDataMgr::GetPositionFTIDList(std::vector<long>& vec)
	{
		vec.clear();
		Lock_CPlatformDataMgr;
		std::set<long>::iterator it_FTID = m_PositionData.m_allFTID.begin();
		for ( ; it_FTID != m_PositionData.m_allFTID.end(); ++it_FTID )
		{
			vec.push_back(*it_FTID);
		}
		Unlock_CPlatformDataMgr;

		return vec.size();
	}

	int CPlatformDataMgr::GetPositionInfo(long lFTID, PlatformStru_Position& outData)
	{
		int nRet = 0;
		Lock_CPlatformDataMgr;
		std::map<long, PlatformStru_Position*>::iterator it_FTID = m_PositionData.m_all2.find(lFTID);
		if ( it_FTID != m_PositionData.m_all2.end())
		{
			outData = *(it_FTID->second);
		}
		else
		{
			nRet = -1;
		}
		Unlock_CPlatformDataMgr;

		return nRet;
	}

	int CPlatformDataMgr::GetPositionInfo(PositionKey key, PlatformStru_Position& outData)
	{
		int nRet = 0;
		Lock_CPlatformDataMgr;
		std::map<PositionKey, PlatformStru_Position*>::iterator it_all = m_PositionData.m_all.find(key);
		if ( it_all != m_PositionData.m_all.end())
		{
			outData = *(it_all->second);
		}
		else
		{
			nRet = -1;
		}
		Unlock_CPlatformDataMgr;

		return nRet;
	}
	int CPlatformDataMgr::GetPositionCombFTIDList(std::vector<long>& vec)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}

	int CPlatformDataMgr::GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData)
	{
		ASSERT("The method is not implemented in pats.");
		return 0;
	}
	bool CPlatformDataMgr::HavePositionDetail(const std::string& strInstrumentID)
	{
		return true;
	}
}