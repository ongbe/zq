/*
    用于保存和维护底层数据
    包括：合约数据、费率数据、持仓明细数据、报单数据、成交单数据、资金数据等
    added by l. 20110216
*/


#pragma once

#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include <hash_map>
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "../inc/platformSvr/PlatformDataSetList.hpp"


class CPackageCache;

class zqWriteLog;


#if 1
#define LogLock1 
#else
#define LogLock1 \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,"..lock1: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif

#if 1
#define LogLock2 
#else
#define LogLock2 \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,"..lock2: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif

#if 1
#define LogUnlock 
#else
#define LogUnlock \
                { \
                    char buf[256];  \
                    const char*pf=__FUNCDNAME__;  \
                    const char*p=strchr(pf,'@');  \
                    int i=p!=NULL?(p-pf):0;\
                    if(i>100) i=100;  \
                    memcpy(buf,".unlock: ",9);  \
                    if(i>0) memcpy(buf+9,pf,i);  \
                    buf[9+i]='\n';  \
                    buf[9+i+1]=0;  \
                    OutputDebugString(buf); \
                }
#endif

#define Lock_CPlatformDataMgr \
                { \
                    LogLock1  \
                    EnterCriticalSection(&m_CS);  \
                    LogLock2 \
                }

#define Unlock_CPlatformDataMgr \
                { \
                    LogUnlock  \
                    LeaveCriticalSection(&m_CS); \
                }

namespace Pats
{
	class CPlatformDataMgr
	{
	public:
		CPlatformDataMgr(CPlatFormService* pHost);
		~CPlatformDataMgr(void);
		//----------下面是资金账户接口----------
		//PATS是实时通过API接口查询获取资金信息
		//不需要管理和缓存资金信息
		//---------------------------------------
		///设置交易资金账户信息
		void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);
		///设置交易资金账户信息
		void SetTradingAccount(const DataRspQryTradingAccount& outData);
		///获取交易资金账户信息
		int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
		int GetTradingAccountAvailable(double& fAvailable);
		int GetTradingAccountID(char* AccountID);


		//----------下面是合约信息接口----------
		///获取指定合约信息
		void SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData);
		///获取指定合约信息
		int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
		///获取合约的产品类型，失败返回-1
		char GetProductClassType(const std::string& InstrumentID);
		///获取合约列表
		int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
		int GetInstrumentList(std::vector<GroupInfo> &outData);
		///添加主力合约列表
		int AddMainInstrumentList(std::string instrument);
		///获取主力合约列表
		int GetMainInstrumentList(std::set<std::string> &outData);
		///设置合约容差列表
		void SetInstrumentVarietyMap(map<string, string>& inData);
		// 获取合约容差列表
		int GetInstrumentVarietyMap(map<string, string>& outData);

		///获取指定品种的合约列表,返回合约个数
		int GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData);
		///获取合约品种代码
		int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");
		///获取合约乘数，成功返回合约乘数，失败返回-1
		int GetInstrumentMultiple(const std::string& InstrumentID);
		///获取合约手续费率
		int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
		///设置合约手续费率
		void SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
		///设置正在查询手续费率的合约
		void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
		///获取正在查询手续费率的合约
		void GetReqCommissionRateInstrumentID(std::string& outData);

		///获取合约保证金率
		int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
		///设置合约保证金率
		void SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
	private:
		//判断一个字符串日期是否是当天
		bool IsTodayPosition(const char *OpenDate);
		bool IsTodayPositionDetail(const PlatformStru_PositionDetail& PositionDetail);
		///获取指定合约信息，内部调用，不用加锁
		int GetInstrumentInfo_Internal(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
		///获取合约保证金率，内部调用，不用加锁
		int GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
		///获取合约手续费率，内部调用，不用加锁
		int GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
		//从成交中取手续费。如果没有费率，作0处理
		void GetAllCommission_Internal(double& TotalCommission);
	public:
		//----------下面是杂项接口----------
		string GetTradingDay(const string& ExchangeID);
		//获取交易所时间信息
		unsigned long GetExchangeTime(const string& ExchangeID,string& time);
		//设置交易所时间信息
		int GetExchangeTime(ExchangeTime& outData);
		//设置交易所时间信息
		void SetExchangeTime(const ExchangeTime& inData);

		//获取结算信息，成功返回0，失败返回-1
		int GetLastSettlementInfo(std::string& outData);
		//清除结算信息
		void ClearLastSettlementInfo(void);
		//添加结算信息
		void AppendLastSettlementInfo(const std::string& inData,bool bLast);
		//----------下面是行情接口----------

		//获取指定合约行情，成功返回0，失败返回-1
		int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
		int GetQuotInfo_Internal(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
		//设置指定合约行情，设置的行情更新，则返回true；否则返回false
		bool SetQuotInfo(const PlatformStru_DepthMarketData& inData);
		void ConvertComboInstrumentID(const std::string strComboID, std::string& strInstrumentID1, std::string& strInstrumentID2);
		//获取指定合约的最新价
		double GetCurPrice (const std::string& InstrumentID);
		//获取指定合约旧行情，成功返回0，失败返回-1
		int GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);


		//业务定制，成功返回0，失败返回-1
		int SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback);
		///取消业务数据的定制，成功返回0，失败返回-1
		int UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID);
		///获取业务定制信息，成功返回0，失败返回-1
		int GetSubscribeBusinessData(const BusinessID BID, std::map<GUIModuleID,GuiCallBackFunc>& outData);



		//查询前清空各种map
		void ClearMapQryRltOrders();
		void ClearMapQryRltTrades();
		void ClearMapQryRltPositionDetails(std::string& strQryInstrument);
		void ClearMapQryRltPositions(std::string& strQryInstrument);
		void ClearMapQryRltPositionCombs(std::string& strQryInstrument);


	public:  
		//----------下面是报单接口----------
		void QryOrderReach(const PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast);
		//新报单到来
		void NewOrderReady(PlatformStru_OrderInfo& OrderInfo, 
			PlatformStru_OrderInfo& OldOrderInfo,
			const PlatformStru_InstrumentInfo& InstrumentInfo);
		//更新所有报单中的冻结保证金，返回合约编号是否有报单，决定是否需要更新资金面板。查询保证金率响应时调用
		bool UpdateOrderFreezeMargin_RspQryMarginRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
		//新所有报单中的冻结手续费，返回合约编号是否有报单，决定是否需要更新资金面板。查询手续费率响应时调用
		bool UpdateOrderFreezeCommission_RspQryCommissionRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
		//计算报单信息中的冻结保证金
		void CalcuOrderFreezeMargin_Internal(PlatformStru_OrderInfo& OrderInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
		//计算报单信息中的冻结手续费
		void CalcuOrderFreezeCommission_Internal(PlatformStru_OrderInfo& OrderInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
		int GetAllOrderFTIDList(std::vector<long>& vec);
		///获取全部报单
		int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
		int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///获取指定报单
		bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
		///获取全部已触发的报单
		int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///获取合约相关的已触发的报单
		int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///获取指定已触发的报单
		bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

		///获取未成交报单
		int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///获取未成交报单
		int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///获取指定的未成交报单
		bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

		///获取已成交报单
		int GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///获取已成交报单
		int GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///获取指定已成交报单
		bool GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

		///获取已撤单和错误报单
		int GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///获取已撤单和错误报单
		int GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///获取指定已撤单和错误报单
		bool GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

		//获取指定合约、指定方向、指定开平的挂单数量（手数）
		int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
		//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
		int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);
		//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。内部调用，不加锁。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
		int  GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag);
	public:  
		//----------下面是成交记录接口----------
		//原始的成交回报到来
		void QryTradeReach( const PlatformStru_TradeInfo& TradeInfo,const int ErrID,const bool bIsLast);
		//新的成交回报到来
		void NewTradeReady( PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
		//计算成交手续费
		int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData);
		int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData);    
		///获取全部成交单
		int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData);
		///获取指定成交信息
		bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
		///获取全部成交单
		int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData);
		//从成交中取手续费。如果没有费率，作0处理]
	public:
		//----------下面是持仓接口----------
		//查询的持仓记录到来
		void QryPositionReach(PlatformStru_Position& PositionInfo,const int ErrID,const bool bIsLast);

		//从成交记录更新持仓。此函数从持仓明细中重新创建并计算持仓。依赖于持仓明细，必须在持仓明细更新完后再更新持仓
		void UpdatePosition_RtnTrade();

		//获取持仓记录键值列表,返回持仓记录的数量
		int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);

		int GetPositions(std::vector<PlatformStru_Position>& outData);
		int GetPositions2(const std::string& strInstrument,std::set<long>& setFTID,std::vector<PlatformStru_Position>& outData,long& lastseq);
		int GetPositions3(const std::string& strInstrument3,std::vector<PlatformStru_Position>& outData,long& lastseq);
		//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
		int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);
		//获取持仓列表FTID
		int GetPositionFTIDList(std::vector<long>& vec);
		//获取持仓详情
		int GetPositionInfo(long lFTID, PlatformStru_Position& outData);
		int GetPositionInfo(PositionKey key, PlatformStru_Position& outData);
		//获取组合持仓列表FTID
		int GetPositionCombFTIDList(std::vector<long>& vec);
		//获取组合持仓详情
		int GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData);
		SERVER_PARAM_CFG&  GetServerParamCfg();
		bool HavePositionDetail(const std::string& strInstrumentID);
	private:

		std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >  m_subscribeInfo;

		//合约信息：下面三个数据集在查询合约信息结果中得到，或者由gui设置
		std::map<std::string,  PlatformStru_InstrumentInfo> m_allInstruments; //当前所有合约信息，系统初始化时获取
		std::set<std::string>  m_allInstrumentList;                         //当前所有合约列表
		std::map<std::string, std::set<std::string> > m_allProductID;       //所有产品代码
		std::vector<GroupInfo> m_allGroupInfo;

		//行情数据集
		std::map<std::string,  PlatformStru_DepthMarketData> m_lastQuot;  //最新行情
		std::map<std::string,  PlatformStru_DepthMarketData> m_OldQuot;  //上次老行情


		//----------下面是报单数据集----------
		CPlatformDataSetList<OrderKey,PlatformStru_OrderInfo> m_OrderData;
		//----------下面是成交数据集----------
		CPlatformDataSetList<TradeKey,PlatformStru_TradeInfo> m_TradeData;
		//----------下面是持仓数据集----------
		CPlatformDataSetList<PositionKey,PlatformStru_Position> m_PositionData;

		zqWriteLog*           m_pWriteLog;
		CRITICAL_SECTION m_CS;
		SERVER_PARAM_CFG m_ServerParamCfg;
	protected:
		CPlatFormService* m_PlatFormService;

	};

}