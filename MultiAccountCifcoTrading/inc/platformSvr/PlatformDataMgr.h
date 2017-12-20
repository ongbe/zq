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
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/Module-Misc/packagecache.h"
#include "PlatformDataSetList.hpp"
#include "ReqOnRtnTrade.h"


class CPackageCache;

class zqWriteLog;

#if 1
#define LogLock1
#define LogLock2 
#define LogUnlock 
#else
#define LogLock1 \
{ \
	char minfun[1024],buf[1024];  \
	strcpy_s(minfun,__FUNCDNAME__);\
	for (size_t i=0;i<strlen(minfun);++i){	if(minfun[i]=='@') {minfun[i]=0;break;}}\
	sprintf(buf,"lock_Begin:0x%08X,%s(%d)\n",&m_CS,minfun,__LINE__);\
	OutputDebugString(buf); \
}
#define LogLock2 \
{ \
	char minfun[1024],buf[1024];  \
	strcpy_s(minfun,__FUNCDNAME__);\
	for (size_t i=0;i<strlen(minfun);++i){	if(minfun[i]=='@') {minfun[i]=0;break;}}\
	sprintf(buf,"lock_end:0x%08X,%s(%d)\n",&m_CS,minfun,__LINE__);\
	OutputDebugString(buf); \
}
#define LogUnlock \
{ \
	char minfun[1024],buf[1024];  \
	strcpy_s(minfun,__FUNCDNAME__);\
	for (size_t i=0;i<strlen(minfun);++i){	if(minfun[i]=='@') {minfun[i]=0;break;}}\
	sprintf(buf,"LogUnlock:0x%08X,%s(%d)\n",&m_CS,minfun,__LINE__);\
	OutputDebugString(buf); \
}
#endif


#define Lock_CPlatformDataMgr \
                { \
                    LogLock1  \
                    EnterCriticalSection(&m_CS);  \
                }

#define Unlock_CPlatformDataMgr \
                { \
                    LogUnlock  \
                    LeaveCriticalSection(&m_CS); \
                }

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
	///获取账户资金文本信息
    int GetAccountText(std::string& outData,int language);
    ///动态更新资金账户中的冻结资金，查询报单返回、查询费率返回
    void DynamicUpdateTradingAccount_Freeze();
    ///动态更新资金账户中的冻结资金，报单回报时调用
    void DynamicUpdateTradingAccount_Freeze_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                     const PlatformStru_OrderInfo& OldOrderInfo);
    ///行情更新时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RtnDepthMarketData(const std::string InstrumentID);
    ///报单回报时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                              const PlatformStru_OrderInfo& OldOrderInfo);
    ///成交回报时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RtnTrade(void);
    ///查询持仓响应时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RspQryInvestorPositionLast(void);
    ///查询成交响应时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RspQryTrade(void);
    ///查询报单响应时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RspQryOrder(void);
    ///查询保证金率响应时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RspQryRate(void);
    ///查询手续费率响应时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate(void);

    
    void SetAccountField(DataRspQryTradingAccountEx& TradingAccountField, const CThostFtdcTradingAccountFieldEx& field );

	void SetAccountField(DataRspQryTradingAccount& field );

    //更新资金数据中的动态权益值
    void RefreshDynamicProficOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField);
    //更新资金数据中的风险度值
    void RefreshRiskDegreeOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField);
    //更新资金数据中的可用资金
    void RefreshAvailOfTradingAccount(PlatformStru_TradingAccountInfo& TradingAccountField);

	const string Instrument2ExchangeID(const string& Instrument);
    ///获取指定合约信息
    void SetInstrumentInfo(const std::string& InstrumentID, const PlatformStru_InstrumentInfo& inData);
    ///获取指定合约信息,0成功；-1失败
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
	///添加查询无记录错误次数
	void IncrementReqMarginRateError(const std::string& InstrumentID);
	///添加查询无记录错误次数
	void IncrementReqCommissionRateError(const std::string& InstrumentID);

    ///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
    void SetCurrentPositionContent(int PositionContentMode);




    //查询前清空各种map
    void ClearMapQryRltOrders();
    void ClearMapQryRltTrades();
    void ClearMapQryRltPositionDetails(std::string& strQryInstrument);
    void ClearMapQryRltPositions(std::string& strQryInstrument);
    void ClearMapQryRltPositionCombs(std::string& strQryInstrument);
    


    //----------下面是报单接口----------
    //查询的报单到来
    void QryOrderReach(PlatformStru_OrderInfo& OrderInfo,const int ErrID,const bool bIsLast);
	//新报单到来
    void NewOrderReady(PlatformStru_OrderInfo& OrderInfo, PlatformStru_OrderInfo& OldOrderInfo, const PlatformStru_InstrumentInfo& InstrumentInfo);
    //更新所有报单中的冻结保证金，返回合约编号是否有报单，决定是否需要更新资金面板。查询保证金率响应时调用
    bool UpdateOrderFreezeMargin_RspQryMarginRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //新所有报单中的冻结手续费，返回合约编号是否有报单，决定是否需要更新资金面板。查询手续费率响应时调用
    bool UpdateOrderFreezeCommission_RspQryCommissionRate(std::string&	strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //计算报单信息中的冻结保证金
    void CalcuOrderFreezeMargin_Internal(PlatformStru_OrderInfo& OrderInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //计算报单信息中的冻结手续费
    void CalcuOrderFreezeCommission_Internal(PlatformStru_OrderInfo& OrderInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);

    ///获取指定报单
    bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
    ///获取全部报单
    int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取全部报单
    int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);

    ///获取全部已触发的报单
    int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取合约相关的已触发的报单
    int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已触发的报单
    bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

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

    ///获取未成交报单
    int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取未成交报单
    int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定未成交报单
    bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    //获取指定合约、指定方向、指定开平的挂单数量（手数）
    int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);
    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。内部调用，不加锁。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
	int  GetCloseOrderVolume_Internal(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag);
    //取全部挂单的冻结手续费和冻结保证金。内部调用，不加锁。，如果没有费率，作0处理
    void GetFreezeMargin_Commission_Internal(double& FreezeMargin,double& FreezeCommission);
    


    //----------下面是成交记录接口----------
    //原始的成交回报到来
    void QryTradeReach( PlatformStru_TradeInfo& TradeInfo,const int ErrID,const bool bIsLast);
    //新的成交回报到来
    void NewTradeReady( PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //获取手续费率后，更新所有成交中的手续费，返回合约编号是否有成交
    bool UpdateTradeCommission_RspQryCommissionRate(std::string& strInstrumentID,const PlatformStru_InstrumentInfo& InstrumentInfo);
    //计算成交手续费
    void CalcuTradeCommission_Internal(PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);
    ///获取全部成交单
    int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData);
    ///获取指定成交信息
    bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
    ///获取全部成交单
    int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData);

    //从成交中取手续费。如果没有费率，作0处理
    void GetAllTradeCommission_Internal(double& TotalCommission);
    ///获取成交统计
	double GetShouXuFei(const PlatformStru_TradeInfo& TradeInfo);


    //----------下面是成交统计接口----------
    void QryTradeTotalReach(const bool bIsLast);
	//成交回报时，更新成交统计
    void UpdateTradeTotal_RtnTrade(const PlatformStru_TradeInfo& TradeInfo);
	//获得手续费率响应时，更新相应合约的成交统计
	bool UpdateTradeTotal_RspQryCommissionRate(const std::string& strInstrument);
    void UpdateTradeTotalByTrade_Internal(const PlatformStru_TradeInfo& TradeInfo);
    bool GetTradeTotalDataValue_Internal(const TradeTotalKey& TotalKey,PlatformStru_TradeTotalInfo& TradeTotalInfo);
    ///根据成交更新成交统计
    bool UpdateTradeTotalDataByTrade_Internal(const PlatformStru_TradeInfo& TradeInfo,PlatformStru_TradeTotalInfo& TradeTotalInfo);
    ///获取全部成交统计记录
    int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData);
    ///获取指定合约的成交统计记录成功返回0，失败返回-1
    int GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData);


    //----------下面是持仓明细接口----------
    //查询的持仓明细记录到来
    void QryPositionDetailReach(PlatformStru_PositionDetail& PositionDetailInfo,int ErrID,const bool bIsLast);
    //取出持仓明细中的记录总数
    int GetRecordCountOfPositionDetail(void);
    //从成交记录更新持仓明细，返回平掉的持仓明细的记录集，用于后续计算。ModifiedPositionDetails保存受到影响的持仓明细记录，n*2+0是原来的数据，n*2+1是新数据
    void UpdatePositionDetail_RtnTrade(PlatformStru_TradeInfo& TradeInfo,
                                        std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                        const PlatformStru_InstrumentInfo& InstrumentInfo);
    //从开仓成交记录创建持仓明细
    void CreatePositionDetailFromOpenTrade_Internal(PlatformStru_PositionDetail& PositionDetailInfo,PlatformStru_TradeInfo& TradeInfo);
    //从平仓成交记录更新持仓明细，ModifiedPositionDetails保存受到影响的持仓明细记录，n*2+0是原来的数据，n*2+1是新数据
    void UpdatePositionDetailFromCloseTrade_Internal(PlatformStru_TradeInfo& TradeInfo,
                                                        std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                                        const PlatformStru_InstrumentInfo& InstrumentInfo);
//	//找开仓时间最早和成交编号最小的持仓明细记录
//	bool FindEarlierOpenPosition( std::string strInstrument, 
//		PlatformStru_TradeInfo &TradeInfo, 
//		std::map<PositionDetailKey,PlatformStru_PositionDetail>::iterator& it_prePosition,
//		std::map<PositionDetailKey,PlatformStru_PositionDetail>::iterator& it_minall );
    //判断持仓明细是不是(平仓)成交的对手持仓
    bool IsOpponentPositionDetailOfTrade_IgnoreTradeType(PlatformStru_PositionDetail& PositionDetail,PlatformStru_TradeInfo& TradeInfo);
    //获取某一个(组合)持仓明细的对手持仓明细记录，注意，只找最早的配对今仓或昨仓
    //用于成交更新持仓明细时打散组合持仓的计算。获取时用到组合持仓明细的数据
    PlatformStru_PositionDetail& GetOpponentPositionDetailComb(const PlatformStru_PositionDetail& OriPositionDetail);
    //普通成交(平仓)需要平组合单时，打散对手持仓明细
    bool BreakupPositionDetailComb(PlatformStru_PositionDetail&OppoPositionDetail,int ClosedVolume,
                                    std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails);


	int CloseSinglePosition_Internal( int CloseVolume, PlatformStru_PositionDetail &positionDetail, 
                             int nVolumeMultiple, double dClosePrice);

	//动态更新持仓明细记录的持仓盈亏和浮动盈亏，得到新行情时做一遍。新行情更新bForceCalculate为false，表示仅在价格变动时更新，其它情况为true
    void UpdatePositionDetailByQuot(PlatformStru_DepthMarketData& QuotData,const PlatformStru_InstrumentInfo& InstrumentInfo,bool bForceCalculate);
    //动态更新持仓明细记录的持仓盈亏和浮动盈亏
    void UpdatePositionDetailByQuot_Internal(PlatformStru_DepthMarketData& QuotData,
                                                PlatformStru_PositionDetail& PositionDetail,
                                                const PlatformStru_InstrumentInfo InstrumentInfo,
                                                bool bForceCalculate);
    //动态更新持仓明细记录的保证金、保证金率，查询保证金率响应时调用
    bool UpdatePositionDetailByMarginRate_RspQryRate(PlatformStru_InstrumentMarginRate& MarginRate,
                                            const PlatformStru_InstrumentInfo& InstrumentInfo);
    //动态更新持仓明细记录的保证金、保证金率
    void UpdatePositionDetailByMarginRate_Internal(PlatformStru_InstrumentMarginRate& MarginRate,
                                                    PlatformStru_PositionDetail& PositionDetail,
                                                    const PlatformStru_InstrumentInfo& InstrumentInfo);

    //获取持仓明细记录
    int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq);
    int GetPositionDetails3(const std::string& strInstrument,
                              std::set<long>& setFTID,
                              std::vector<PlatformStru_PositionDetail>& vecValue,
                              long& lastseq);
    bool HavePositionDetail(const std::string& strInstrumentID);
    //获取查询的合约ID
    void GetQryPositionDetailInstrument(std::string& outQryInstrument);



    //----------下面是持仓接口----------
	//查询持仓时合并同一合约同一方向的历史持仓和今日持仓记录
	void MergePositions_RspQryPosition( PlatformStru_Position& firstPositionInfo, PlatformStru_Position secondPositionInfo );
    //查询的持仓记录到来
    void QryPositionReach(PlatformStru_Position& PositionInfo,const int ErrID,const bool bIsLast);
    //从成交记录更新持仓。此函数从持仓明细中重新创建并计算持仓。依赖于持仓明细，必须在持仓明细更新完后再更新持仓
    void UpdatePosition_RtnTrade(PlatformStru_TradeInfo& TradeInfo, 
                                    std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails,
                                    const PlatformStru_InstrumentInfo& InstrumentInfo);
	//更新持仓数据中定制的域，指不是有CTP返回的域
	void UpdatePositionFieldsWithoutCTPSupported_QryPositionReach_Internal(PlatformStru_Position& PositionData,
                                                                            const PlatformStru_InstrumentInfo& InstrumentInfo);
    //动态更新持仓记录的持仓盈亏、价格值，得到新行情时做一遍
    void UpdatePositionByQuot(PlatformStru_DepthMarketData& QuotData,
                                const PlatformStru_InstrumentInfo& InstrumentInfo,
                                bool bForceCalculate);
    //动态更新持仓记录的持仓盈亏(盯市)、浮动盈亏(逐笔)和盯市总盈亏，结算价和昨结算价，根据行情
    void UpdatePositionByQuot_Internal(PlatformStru_DepthMarketData& QuotData,
                                        PlatformStru_Position& PositionInfo,
                                        const PlatformStru_InstrumentInfo& InstrumentInfo,
                                        bool bForceCalculate);
    //动态更新持仓记录的保证金、保证金率，查询保证金率响应时调用
    bool UpdatePositionByMarginRate_RspQryRate(PlatformStru_InstrumentMarginRate& MarginRate);
    //动态更新持仓记录的保证金、保证金率，查询保证金率响应和得到首次行情时调用
    void UpdatePositionByMarginRate_RspQryRate_FirstQuot_Internal(PlatformStru_InstrumentMarginRate& MarginRate,PlatformStru_Position& PositionInfo,bool bForce);
    //动态更新持仓记录的保证金、保证金率，成交修改持仓时调用
    void UpdatePositionByMarginRate_RtnTrade_Internal(PlatformStru_Position& PositionInfo,
                                                        std::vector<PlatformStru_PositionDetail>& ModifiedPositionDetails); 
    //动态更新持仓记录的手续费，查询手续费率响应时调用
    bool UpdatePositionByCommissionRate_RspQryRate(PlatformStru_InstrumentCommissionRate& CommissionRate);
    //动态更新持仓记录的手续费，查询手续费率响应时调用
    void UpdatePositionByCommissionRate_RspQryRate_Internal(PlatformStru_Position& PositionInfo);
	//费率查询响应时，更新持仓相关的挂单的冻结手续费和冻结保证金
	void CPlatformDataMgr::UpdatePositionFreezeFieldsByOrders_RspQryRate(const std::string& strInstrument);
	//报单回报时，更新持仓相关的挂单的冻结手续费和冻结保证金
	void UpdatePositionFreezeFields_RtnOrder(const PlatformStru_OrderInfo& Order, 
                                                const PlatformStru_OrderInfo& OldOrderInfo);
	//取持仓相关的挂单的冻结手续费和冻结保证金。
	void UpdatePositionFreezeFieldsByAllOrders_Internal(PlatformStru_Position& PositionData);
	//取持仓相关的挂单的冻结手续费和冻结保证金，根据一个报单的变化。
	void UpdatePositionFreezeFieldsByOneOrder_Internal(PlatformStru_Position& PositionData, 
                                                        const PlatformStru_OrderInfo& Order, 
                                                        const PlatformStru_OrderInfo& OldOrderInfo);

    //在持仓数据中，计算平仓量和可平仓量, (根据所有挂单)计算全部
    void CalcuCloseValueInPositionByAllOrders_Internal(PlatformStru_Position& PositionData,
                                                        const int CloseVolume,
                                                        const int CloseTodayVolume,
                                                        const int CloseYesterdayVolume);
    //在持仓数据中，计算平仓量和可平仓量, (根据一个挂单)进行增量计算
    //Deltaxxx是增量
    void CalcuCloseValueInPositionByOneOrder_Internal(PlatformStru_Position& PositionData,
                                                        const int DeltaCloseVolume,
                                                        const int DeltaCloseTodayVolume,
                                                        const int DeltaCloseYesterdayVolume);

    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
    int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);
    int GetPositions(std::vector<PlatformStru_Position>& outData);
    int GetPositions2(const std::string& strInstrument,
                        std::set<long>& setFTID,
                        std::vector<PlatformStru_Position>& outData,
                        long& lastseq);
    int GetPositions3(const std::string& strInstrument,
                        std::vector<PlatformStru_Position>& outData,
                        long& lastseq);
    //获取查询的合约ID
    void GetQryPositionInstrument(std::string& outQryInstrument);



    //----------下面是组合持仓明细接口----------
    //查询组合持仓记录的结果到来。注意，可能是查询指定合约的组合持仓
    void QryPositionCombReach(PlatformStru_PositionComb& PositionCombInfo,const int ErrID,const bool bIsLast);
    //获取组合组合持仓记录
    int GetPositionCombs(std::vector< PlatformStru_PositionComb > &PositionCombMap);
	int GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData);
    //获取成交ID对应的组合持仓明细记录
    PlatformStru_PositionComb& GetPositionComb3_Internal(const std::string& strTradeID);
    //获取查询的合约ID
    void GetQryPositionCombInstrument(std::string& outQryInstrument);
	//合并初始化查询得到的单腿持仓为一个组合持仓
	bool MergeOriSingleLegPosition( const PlatformStru_PositionComb &PositionCombo, PlatformStru_PositionComb& newComboPosition );
	//组合单开仓
	void CreatePositionDetailCombFromOpenTrade_Internal( PlatformStru_TradeInfo &TradeInfo );





    //获取指定合约行情，成功返回0，失败返回-1
    int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    //设置指定合约行情，设置的行情更新，则返回true；否则返回false
    bool SetQuotInfo(const PlatformStru_DepthMarketData& inData,bool& bFirstQuot);
    //将两个单腿合约合成组合合约，返回成功与否。合成时会检查合约列表里是否有该组合合约
	bool GetComboInstrumentID_Internal(const std::string strInstrumentID1, 
                                        const std::string strInstrumentID2, 
                                        std::string& strComInstrumentID,
                                        std::string& strLeg1InstrumentID,
                                        std::string& strLeg2InstrumentID);
    //获取指定合约的最新价
    double GetCurPrice (const std::string& InstrumentID);
    //获取指定合约旧行情，成功返回0，失败返回-1
    int GetOldQuotInfo (const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);

    //设置投资者信息
    void SetInvestorInfo(const DataRspQryInvestor& inData);

    string GetTradingDay(const string& ExchangeID);
    //获取交易所时间信息
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    //设置交易所时间信息
    int GetExchangeTime(ExchangeTime& outData);
    //设置交易所时间信息
    void SetExchangeTime(const ExchangeTime& inData);
	//获取全部交易所
	int GetExchangeIDs(vector<string>& outData);

    //获取结算信息，成功返回0，失败返回-1
    int GetLastSettlementInfo(std::string& outData);
    //清除结算信息
    void ClearLastSettlementInfo(void);
    //添加结算信息
    void AppendLastSettlementInfo(const std::string& inData,bool bLast);
    //更新客户签约银行信息
	void UpdateContractBank(CThostFtdcAccountregisterField& inData);

	//获取客户签约银行信息
	int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData);
	//存储签约银行名称
	void SaveBankName(CThostFtdcContractBankField& inData);

		//业务定制，成功返回0，失败返回-1
    int SubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID, const GuiCallBackFunc& callback);
    ///取消业务数据的定制，成功返回0，失败返回-1
    int UnSubscribeBusinessData(const BusinessID& BID, const GUIModuleID& GID);
    ///获取业务定制信息，成功返回0，失败返回-1
    int GetSubscribeBusinessData(const BusinessID BID, std::map<GUIModuleID,GuiCallBackFunc>& outData);

	//
	SERVER_PARAM_CFG&  GetServerParamCfg();
    void SetModuleDataQry(bool bQry);
    bool GetModuleDataQry();

    ///动态更新资金账户中的交易佣金，查询成交返回、成交回报、查询费率返回
    void DynamicUpdateTradingAccount_TradeCommission_Internal();

	//判断一个字符串日期是否是当天
	bool IsTodayPosition(const char *OpenDate);
    bool IsTodayPositionDetail(const PlatformStru_PositionDetail& PositionDetail);

	//为插件添加以下接口
	//获取委托单列表FTID
	int GetAllOrderFTIDList(std::vector<long>& vec);
	//获取委托单详情
	int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData);
	int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData);

	//获取持仓列表FTID
	int GetPositionFTIDList(std::vector<long>& vec);
	//获取持仓详情
	int GetPositionInfo(long lFTID, PlatformStru_Position& outData);
	int GetPositionInfo(PositionKey key, PlatformStru_Position& outData);

	//获取组合持仓列表FTID
	int GetPositionCombFTIDList(std::vector<long>& vec);
	//获取组合持仓详情
	int GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData);

private:

    //----------下面是报单数据集----------
    CPlatformDataSetList<OrderKey,PlatformStru_OrderInfo> m_OrderData;

    //----------下面是成交数据集----------
    CPlatformDataSetList<TradeKey,PlatformStru_TradeInfo> m_TradeData;

    //----------下面是成交统计数据集----------
    CPlatformDataSetList<TradeTotalKey,PlatformStru_TradeTotalInfo> m_TradeTotalData;

    //----------下面是持仓明细数据集----------
    CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail> m_PositionDetailData;

    //----------下面是持仓数据集----------
    CPlatformDataSetList<PositionKey,PlatformStru_Position> m_PositionData;

    //----------下面是组合持仓数据集----------
    CPlatformDataSetList<PositionCombKey,PlatformStru_PositionComb> m_PositionCombData;




    //合约信息：下面三个数据集在查询合约信息结果中得到，或者由gui设置
    std::map<std::string,  PlatformStru_InstrumentInfo> m_allInstruments; //当前所有合约信息，系统初始化时获取
    std::set<std::string>  m_allInstrumentList;                         //当前所有合约列表
    std::set<std::string>  m_allInstrumentListWithoutComb;              //当前所有合约列表（排除组合合约）
    std::map<std::string, std::set<std::string> > m_allProductID;       //所有产品代码

    std::set<std::string>  m_allMainInstrumentList;                         //当前主力合约列表

	std::map<string, string> m_allInstrumentVarietyMap;
    bool m_bNeedCheckCommissionRateAndMarginRate;                       //是否需要检查手续费率和保证金率，当更新合约列表时，此值为true，查询完后，此值为false

	bool m_bHavePostionDetail;
    std::map<std::string, PlatformStru_InstrumentCommissionRate> m_InstrumentCommissionRate;//手续费率
    std::string m_ReqCommissionRateInstrumentID;                 //正在查询手续费率的合约
    std::map<std::string, PlatformStru_InstrumentMarginRate> m_InstrumentMarginRate;//保证金率


    //资金账户信息：资金账户在系统启动前查询一次，然后根据报单回报和成交回报动态更新
    DataRspQryTradingAccount m_TradingAccount;                          //资金账户信息


    //行情数据集
    std::map<std::string,  PlatformStru_DepthMarketData> m_lastQuot;  //最新行情
    std::map<std::string,  PlatformStru_DepthMarketData> m_OldQuot;  //上次老行情


    //杂项数据集
    DataRspQryInvestor m_InvestorInfo;//投资者个人信息
    std::string m_LastSettlementInfo;
    bool m_bSettlementInfoIsReady;
    std::map<BusinessID, std::map<GUIModuleID, GuiCallBackFunc> >  m_subscribeInfo;
	////组合关系<ID1,<ALL,ID2>>,<ID2,<ALL,ID1>>
	//std::multimap<std::string,std::vector<std::string>> m_CombRelation;
//	std::multimap<std::string,std::string> m_mapCombineRelation;
//	std::map<pair<std::string, std::string>, std::string> m_ComboInstruments;
	//用来匹配组合持仓中的几个单腿的数据集
	std::map<std::string, PlatformStru_PositionComb> m_combinLegs;
	//初始持仓明细，用于查询组合合约成交价
	std::map<std::string, PlatformStru_PositionDetail> m_tempOriPositionDetail;	
	//缓存组合合约中的一条单腿，用于匹配另外一条单腿
	std::map<std::string, PlatformStru_TradeInfo> m_tempTradeCombinLegs;

    ExchangeTime m_exchangeTime; //交易所时间

    zqWriteLog*           m_pWriteLog;

    CRITICAL_SECTION m_CS;

    //专门针对合约信息的锁，管m_allInstruments、m_allInstrumentList、m_allProductID
    CRITICAL_SECTION m_CS_InstrumentInfo;

    //专门针对行情的锁
    CRITICAL_SECTION m_CS_Quot;


	SERVER_PARAM_CFG m_ServerParamCfg;
    //将一条成交明细更新到m_allPositionDetails中
    //返回：成功与否
    //注意：m_allPositionDetails中的同一合约的持仓明细记录按开仓先后顺序排列
    //bool AssignTradeInfoToPositionDetail(PlatformStru_TradeInfo& TradeInfo);



    ///指定合约是否合法(存在)
    bool InstrumentIsValid(const std::string& InstrumentID);
    ///获取合约保证金率，内部调用，不用加锁
    int GetMarginRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///获取合约手续费率，内部调用，不用加锁
    int GetCommissionRate_Internal(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);

    //取全部持仓的占用保证金和持仓盈亏(逐日盯市)。如果没有费率，作0处理
    void GetAllMargin_CloseProfitByDate_Internal(double& TotalMargin,double&TotalCloseProfitByDate);
    //取全部持仓的占用保证金和持仓盈亏(逐日盯市)。如果没有费率，作0处理
    void GetAllPositionProfitByDate_Internal(double&TotalPositionProfitByDate);
    //取全部持仓的平仓盈亏(逐日盯市)
    void GetAllCloseProfitByDate_Internal(double&TotalCloseProfitByDate);

    void TotalCloseProfitByDate_Internal(double& TotalCloseProfitByDate);
	std::map<std::string,  PlatformStru_ContractBank> m_ContractBankInfo;
	std::map<std::string,  CThostFtdcContractBankField> m_BankName;
	std::map<std::string, int> m_reqMarginRateError;//请求保证金查询无记录次数
	std::map<std::string, int> m_reqCommissionRateError;//请求手续费查询无记录次数
    bool m_ModuleDataQry;

protected:
	CPlatFormService* m_PlatFormService;


};

