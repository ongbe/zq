/*
    用于保存和维护CTP的成交数据
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
using std::vector;
using std::map;
using std::multimap;
using std::set;
using std::string;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Rates.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Trades
{
public:
    CPlatformDataMgr_Trades(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Rates& refRates);
    ~CPlatformDataMgr_Trades(void);


    //查询的成交回报到来
    void QryTradeReach( PlatformStru_TradeInfo& TradeInfo,int ErrID,bool bIsLast);

    //新的成交回报到来
    void NewTradeReady( PlatformStru_TradeInfo& TradeInfo,const PlatformStru_InstrumentInfo& InstrumentInfo);

    //更新成交记录的平今量和平仓盈亏，重新计算手续费。
    void UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission( long TradeFTID,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        int CloseTodayVolume,
        double CloseProfitByDate,
        double CloseProfitByTrade,
        double& outCommission);

    //获取手续费率后，更新所有成交中的手续费，返回值表示相关手续费是否有改变，以此决定是否需要更新资金面板。
    bool UpdateTradeCommission_RspQryCommissionRate(const string& strInstrumentID,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        const PlatformStru_InstrumentCommissionRate& CommissionRate);


    //获取全部成交单，返回数据项的数量
    int GetAllTrades(vector<PlatformStru_TradeInfo>& outData);
    //获取指定成交信息
    bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
    bool GetTradeInfo(long FTID, PlatformStru_TradeInfo& outData);
    //获取全部成交的FTID，按合约ID分类。同一个合约的FTID是排序的
    void GetFTIDsByInstrument(map<string,vector<long>>& outFTIDs);
    //获取指定合约的成交单，返回数据项的数量
    int GetTradesOfInstrument(const string& strInstrument,vector<PlatformStru_TradeInfo>& outData);
    int GetTradeBriefsOfInstrument(const string& strInstrument,vector<PlatformStru_TradeInfoBrief>& outData);
    //获取交易手续费之和
    double GetTotalCommission(void);
    //获取持仓交易手续费
    double GetCommission(const string& strInstrument,const string& strAccount,char HedgeFlag,char Direction);

    //准备查询成交前清空要查询的合约名
    void ClearMapQryRltTrades();


    //获取指定TradeID的成交记录Key。
    void GetTradeKeysOfTradeID(const string& strTradeID,vector<TradeKey>& outData);

    //组合成交时，获取OrderSysID一致的成交明细集合。Volume>0
    void GetTradeKeysByTradeComb_OrderSysID(const TradeKey& tradekey,set<TradeKey>& outData);
    void GetTradeKeysByTradeComb_OrderSysID2(const TradeKey& tradekey,map<string,TradeKey>& outData);

    //获取指定成交key对应的Volume
    bool GetTradeVolume(const TradeKey& tradekey,int& Volume);

    //根据成交记录，获取成交统计key的集合
    void GetTradeTotalKeysByTrades(set<TradeTotalKey>& outData);
    //根据成交记录，获取指定key的成交统计数据
    bool GetTradeTotalValue(const TradeTotalKey& TotalKey,PlatformStru_TradeTotalInfo& TradeTotalInfo);

    //清除所有数据
    void clear(void);
    //判断是否为空
    bool empty(void);


    ///获取大于等于MinFTID的全部数据，返回数据项的数量。结果放在数组中。m_bUpdateSeq2FTID为true时有效
    int GetAll_GE_UpdateSeq(long MinUpdateSeq,vector<PlatformStru_TradeInfo>& outDatas);

private:
    //计算成交手续费。注意，对于大商郑商，使用每个合约剩余的平今优惠配额手数
    void CalcuTradeCommission_Internal(PlatformStru_TradeInfo& TradeInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        const PlatformStru_InstrumentCommissionRate& CommissionRate);

private:
    Ceasymutex				        m_mutex;

    CPlatformDataMgr_Instruments    &m_RefInstruments;
    CPlatformDataMgr_Rates          &m_RefRates;

    //----------下面是成交数据集----------
    CPlatformDataSetList<TradeKey,PlatformStru_TradeInfo> m_TradeData;

	//OrderSysID 到 TradeKey 的映射关系。主要用于日内组合成交的配对查询
    multimap<string, TradeKey>      m_mapOrderSysID2TradeKey;
	//TradeID 到 TradeKey 的映射关系。主要用于日内组合成交的配对查询
    multimap<string, TradeKey>      m_mapTradeID2TradeKey;

    //每个合约剩余的平今优惠配额手数。针对大商郑商。这两个交易所的平今优惠，不是按平掉的持仓明细是否今仓来判断，而是按是否有今仓可平来判断。
    //      比如，有昨仓1手今仓1手，平1手，平的是昨仓，但是由于有今仓，因此手续费优惠
    //上期中金按平掉的持仓明细是否今仓来判断
    //分多头和空头
    //map<string,int>                 m_mapInstrument2LongVolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE;
    //map<string,int>                 m_mapInstrument2ShortVolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE;
    map<PositionKey,int>            m_mapPositionKey2VolumeOfCanDiscountCommissionAsCloseToday_DCE_CZCE;

};

#ifdef WIN32
#pragma managed(pop)
#endif 
