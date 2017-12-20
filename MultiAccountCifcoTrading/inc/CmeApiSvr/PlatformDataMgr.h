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


class CPackageCache;

class zqWriteLog;



class CPlatformDataMgr
{
public:
    CPlatformDataMgr(CPlatFormService* pHost);
    ~CPlatformDataMgr(void);

    ///设置交易资金账户信息
    void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);
    ///设置交易资金账户信息
    void SetTradingAccount(const DataRspQryTradingAccount& outData);
    ///获取交易资金账户信息
	int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
	int GetTradingAccountAvailable(double& fAvailable);
	int GetTradingAccountID(char* AccountID);
    int GetTradingAccount(PlatformStru_TradingAccountInfo& outData);
    ///动态更新资金账户信息，根据成交回报、行情、报单回报、费率查询回报、合约查询回报
    void DynamicUpdateTradingAccount(void);
    ///动态更新资金账户信息，根据行情
    void DynamicUpdateTradingAccount_NewQuot(const std::string InstrumentID);
	void SetAccountField(DataRspQryTradingAccountEx& TradingAccountField, const CThostFtdcTradingAccountFieldEx& field );

	void SetAccountField(DataRspQryTradingAccountEx& TradingAccountField, const CThostFtdcTradingAccountField& field );

	void Assign(DataRspQryTradingAccountEx& TradingAccountField,const DataRspQryTradingAccount& data );


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



    bool IsNeedCheckCommissionRateAndMarginRate(void) { return m_bNeedCheckCommissionRateAndMarginRate;}
    void SetNeedCheckCommissionRateAndMarginRate(bool bNeedCheck) { m_bNeedCheckCommissionRateAndMarginRate=bNeedCheck;}



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



    //原始的成交回报到来
    void OriginTradeReady( const PlatformStru_TradeInfo TradeInfo,const int ErrID,const bool bIsLast);
    //新的成交回报到来
    void NewTradeReady( const PlatformStru_TradeInfo TradeInfo);
    ///获取全部成交单
    int GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData);
    ///获取成交统计
    int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
	///在成交统计查找参数rawData
	int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );


    //原始报单到来
    void OriginOrderReady(const PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast);
    //新报单到来
    void NewOrderReady(const PlatformStru_OrderInfo& OrderInfo);
    ///获取全部报单
    int GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData);
    //获取指定合约、指定方向、指定开平的挂单数量（手数）
    int GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    int GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    
	//新报单到来,更新所有委托单
	void DynamicUpdateAllOrderPanel(const PlatformStru_OrderInfo& OrderInfo);
	///获取所有委托单信息
    int GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap);
	//报单录入回报，更新所有委托单
	void UpdateOrdersforOrderInsert(CThostFtdcInputOrderField &InputOrderField,CThostFtdcRspInfoField &RspInfoField);


    //原始持仓明细记录到来
    void OriginPositionDetailReady(PlatformStru_Position PositionDetailInfo,const int ErrID,const bool bIsLast);
    //更新动态持仓明细记录中的盈亏数值，得到新行情时做一遍
    void UpdateProfitInDynamicPositionDetail(PlatformStru_DepthMarketData& QuotData);

    //动态更新持仓明细记录，每次成交回报和得到保证金率时做一遍
    //根据m_TradesListForUpdatePositionDetail来更新m_allPositionDetail
    void ProcessNewTradeList(void);
    //获取持仓明细记录
    int GetPositionDetails(std::multimap<std::string, PositionDetailStru > &PositionDetailMap);
    int GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru > &PositionDetailMap);
    int GetPositionDetails(std::list<PositionDetailStru> &PositionDetailList);

    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeyList(std::list<PositionKey> &PositionKeyList);
    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
    int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData);





    //获取指定合约行情，成功返回0，失败返回-1
    int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    //设置指定合约行情，设置的行情更新，则返回true；否则返回false
    bool SetQuotInfo(const PlatformStru_DepthMarketData& inData);
	void ConvertComboInstrumentID(const std::string strComboID, std::string& strInstrumentID1, std::string& strInstrumentID2);
    //获取指定合约的最新价
    double GetCurPrice (const std::string& InstrumentID);
    //获取指定合约旧行情，成功返回0，失败返回-1
    int GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);

    //设置投资者信息
    void SetInvestorInfo(const DataRspQryInvestor& inData);

    string GetTradingDay();
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


    //业务定制，成功返回0，失败返回-1
    int SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback);
    ///取消业务数据的定制，成功返回0，失败返回-1
    int UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID);
    ///获取业务定制信息，成功返回0，失败返回-1
    int GetSubscribeBusinessData(const BusinessID BID, std::list<GuiCallBackFunc>& outData);
    //获取合约列表后，初始化组合关系map
	int InitCombRelation(std::vector<DataRtnDepthMarketData>& vec);
	//发送组合合约深度行情数据
	int MakeComboDepthMarketData(PlatformStru_DepthMarketData *pDepthMarketData,int index=0);
	// 刷新持仓信息
	void RefreshPositionData();
	//
	SERVER_PARAM_CFG&  GetServerParamCfg();
private:


    //合约信息：下面三个数据集在查询合约信息结果中得到，或者由gui设置
    stdext::hash_map<std::string,  PlatformStru_InstrumentInfo> m_allInstruments; //当前所有合约信息，系统初始化时获取
    std::set<std::string>  m_allInstrumentList;                         //当前所有合约列表
    std::set<std::string>  m_allMainInstrumentList;                         //当前主力合约列表
    std::map<std::string, std::set<std::string> > m_allProductID;       //所有产品代码
	std::vector<GroupInfo> m_allGroupInfo;
	std::map<string, string> m_allInstrumentVarietyMap;
    bool m_bNeedCheckCommissionRateAndMarginRate;                       //是否需要检查手续费率和保证金率，当更新合约列表时，此值为true，查询完后，此值为false
    std::map<std::string, PlatformStru_InstrumentCommissionRate> m_InstrumentCommissionRate;//手续费率
    std::string m_ReqCommissionRateInstrumentID;                 //正在查询手续费率的合约
    std::map<std::string, PlatformStru_InstrumentMarginRate> m_InstrumentMarginRate;//保证金率


    //资金账户信息：资金账户在系统启动前查询一次，然后根据报单回报和成交回报动态更新
    DataRspQryTradingAccount m_TradingAccount;                          //资金账户信息


    //成交记录信息：成交记录数据集，系统启动前查询一次，然后根据成交回报动态更新
    std::map<TradeKey,  PlatformStru_TradeInfo> m_originTrades;           //初始成交单，系统启动时查询的成交单，然后不再更新。
    std::map<TradeKey,  PlatformStru_TradeInfo> m_allTrades;              //所有成交单，系统启动时查询，并根据成交回报动态更新
    std::multimap<std::string,TradeKey> m_allTradeIndex;              //全部成交单的索引
    std::list<PlatformStru_TradeInfo> m_TradesListWaitForAppend;          //等待添加的成交单列表。通过OnRtnTrade过来的成交单，必须等待原始成交单和原始持仓明细得到后才能添加
    bool m_bGetOriginTradeData;                                       //是否已经完全获取到初始的成交信息

	vector<TotalInfo>	m_TradeTotalData;	                          // 成交统计数据


    //报单记录数据集
    std::map<OrderKey,  PlatformStru_OrderInfo> m_originOrders;           //初始委托单，系统启动时查询的报单，然后不再更新。
    std::map<OrderKey,  PlatformStru_OrderInfo> m_allOrders;              //所有委托单，系统启动时查询，然后随报单回报更新。包括挂单中的，已成交的，已撤单的
    std::multimap<std::string,OrderKey> m_allOrderIndex;              //全部委托单的索引
    std::list<PlatformStru_OrderInfo> m_OrdersListWaitForAppend;          //等待添加的报单列表。通过OnRtnOrder过来的报单，必须等待原始报单得到后才能添加
    bool m_bGetOriginOrderData;                                       //是否已经完全获取到初始的报单信息


    //持仓数据集
    std::map<PositionDetailKey,PositionDetailStru> m_allPosition;          //全部持仓明细记录，每次得到成交回报时会根据原始持仓明细和成交回报明细动态更新此记录，每次得到行情和费率时更新相应的盈亏参数值
    std::multimap<std::string,PositionDetailKey> m_allPositionIndex;       //全部持仓明细的索引
    bool m_bGetOriginPositionDetailData;                                        //是否已经完全获取到初始的持仓明细信息
    long m_MaxPositionDetailID;
    
	//所有委托单数据集
	std::map <AllOrderKey,  PlatformStru_OrderInfo> m_OrdersPanel;  

    //行情数据集
    std::map<std::string,  PlatformStru_DepthMarketData> m_lastQuot;  //最新行情
    std::map<std::string,  PlatformStru_DepthMarketData> m_OldQuot;  //上次老行情


    //杂项数据集
    DataRspQryInvestor m_InvestorInfo;//投资者个人信息
    std::string m_LastSettlementInfo;
    bool m_bSettlementInfoIsReady;
    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >  m_subscribeInfo;
	//组合关系<ID1,<ALL,ID2>>,<ID2,<ALL,ID1>>
	std::multimap<std::string,std::vector<std::string>> m_CombRelation;

    ExchangeTime m_exchangeTime; //交易所时间

    zqWriteLog*           m_pWriteLog;

    CRITICAL_SECTION m_CS;

	SERVER_PARAM_CFG m_ServerParamCfg;
    //判断一个字符串日期是否是当天
    bool IsTodayPosition(const char *OpenDate);

    ///获取指定合约信息，内部调用，不用加锁
    int GetInstrumentInfo_Internal(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///获取合约保证金率，内部调用，不用加锁
    int GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///获取合约手续费率，内部调用，不用加锁
    int GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);

    //取全部挂单的冻结手续费和冻结保证金。内部调用，不加锁。，如果没有费率，作0处理
    void GetFreezeMargin_Commission_Internal(double& FreezeMargin,double& FreezeCommission);
    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。内部调用，不加锁
    //成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    int  GetCloseOrderVolume_Internal(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag);
    //获取指定合约不同方向的挂单量。内部调用，不加锁。不区分投保
    void GetWaitOrderVolume_Internal(std::map<std::string,WaitOrderVolumeStru>& WaitOrderVolume);
    //从成交中取手续费。如果没有费率，作0处理
    void GetAllCommission_Internal(double& TotalCommission);
    //取全部持仓的占用保证金和持仓盈亏(逐日盯市)。如果没有费率，作0处理
    void GetAllMargin_PositionProfitByDate_Internal(double& TotalMargin,double&TotalPositionProfitByDate,double&TotalCloseProfitByDate);
	
	void SumTradeData(const PlatformStru_TradeInfo& rawData);
	double GetShouXuFei(const PlatformStru_TradeInfo& raw);
protected:
	CPlatFormService* m_PlatFormService;
};

