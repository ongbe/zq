/*
    用于保存和维护CTP的持仓明细数据
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <set>
#include <string>
#include <map>
using std::string;
using std::set;
using std::vector;
using std::map;
using std::multimap;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Rates.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetailCombs.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 



class CPlatformDataMgr_PositionDetails
{
public:
    CPlatformDataMgr_PositionDetails(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Rates& refRates,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders,
        CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
        int DiscountCZCE_Comb=3,
        int DiscountDCE_Comb=0,
        int UnwindPrioritiesCZCE_Comb=1,
        int UnwindPrioritiesDCE_Comb=0,
        int PriceForMarginOfTodayPositionCZCE_Comb=0,
        int PriceForMarginOfTodayPositionDCE_Comb=1,
        bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible=false);

    ~CPlatformDataMgr_PositionDetails(void);

    //查询回报到来
    void QryPositionDetailReach( PlatformStru_PositionDetail& PositionDetailInfo,int ErrID,bool bIsLast);

    //从成交记录更新持仓明细。会修改TradeInfo的平今量、平仓盈亏
    //ModifiedPositionDetails保存受到影响的持仓明细记录，n*2+0是原来的数据，n*2+1是新数据
    //ComTradeID2CloseVolume:平组合衍生仓位时，被平掉的组合成交ID和对应平仓量的关系
    void UpdatePositionDetail_RtnTrade(
        PlatformStru_TradeInfo& TradeInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        map<string,int>& ComTradeID2CloseVolume);

    //更新持仓明细记录的持仓盈亏、浮动盈亏、结算价、昨结算价、现价，根据行情，得到新行情时做一遍
    void UpdatePositionDetail_RtnDepthMarketData(
        const PlatformStru_DepthMarketData& QuotData,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //结算时，用结算价更新持仓明细记录的持仓盈亏(盯市)、浮动盈亏(逐笔)、保证金、结算价、昨结算价、现价
    void UpdatePositionDetail_OnSettlement();

    //根据日初持仓明细、日内成交记录，重新计算成交的平今量、平仓盈亏、手续费
    //初始化查询完成交和持仓明细后调用
    void ReCalcuTradeInfo_OnInitQryCompleted(void);

    //针对有变化的持仓明细，计算组合持仓明细的保证金优惠。需要在更新持仓明细和更新组合持仓明细后进行。根据保证金优惠设置进行计算，更新相应持仓明细和组合持仓明细的保证金字段，(为保证不漏掉，两腿都算，有点重复)
    void CalcuPositionDetailMarginDiscount_RtnTrade_RspQryRate_OnSettlement(
        map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs);

    //针对有变化的持仓明细，将保证金数据设置到组合持仓明细中。此时未计算优惠
    void SetMarginOfPositionDetailComb(map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails);

    //更新持仓明细记录的保证金, 保证金率, 查询保证金率响应时调用, 只对保证金为无效值的项目进行处理. 对于查询回来的持仓明细, 保证金类数据已经有了，不用再算. 
    void UpdatePositionDetailByMarginRate_RspQryRate(
        const PlatformStru_InstrumentMarginRate& MarginRate,
        const PlatformStru_InstrumentInfo& InstrumentInfo);


    //平仓成交时，根据持仓明细的变化，取持仓明细仓位变化值
    void GetCloseDeltaVolume(const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,map<PositionDetailKey,int>& mapCloseDeltaVolume);

    //取出持仓明细中的记录总数，含平掉的记录
    int GetRecordCountOfPositionDetail(void);

    //根据key取value
    bool GetPositionDetail(const PositionDetailKey& key,PlatformStru_PositionDetail& value);

    //取有效持仓明细(Volume>0)。返回记录数量，lastseq返回数据更新的序列号
    int GetPositionDetails(vector<PlatformStru_PositionDetail>& outData,long& lastseq);
    
    //获取指定合约对应的有效持仓明细记录(Volume>0)。返回0：成功，并返回数据(可能是空数据)；1: 成功，但由于没有更新，不返回数据
    //如果lastseq和原始数据中的updateseq一致，表示数据没有变化，返回1，不带具体数据；
    //否则返回0，setFTID是strInstrument对应的全部有效(仓位大于零)记录的FTID，用于判断有没有需要删除的项；vecValue返回对应的有效数据数组
    int GetPositionDetails3(
        const std::string& strInstrument,
        std::set<long>& setFTID,
        std::vector<PlatformStru_PositionDetail>& vecValue,
        long& lastseq);

    //取全部持仓明细，包括数量为零的记录。lastseq返回数据更新的序列号。
    void GetPositionDetails4(vector<PlatformStru_PositionDetail>& outData,long& lastseq);

    //是否有指定合约的有效持仓明细记录
    bool HavePositionDetail(const string& strInstrumentID);

    //清空查询持仓明细的map
    void ClearMapQryRltPositionDetails(const string& strQryInstrument);

    //获取查询的合约ID
    void GetQryPositionDetailInstrument(string& outQryInstrument);

    //计算指定[合约-账号-方向-投保]持仓明细的保证金之和
    void GetMarginByInstrument(
        const string& strInstrumentID,
        const string& strAccount,
        char Direction,
        char HedgeFlag,
        double& outMargin,
        double& outExchMargin);

    //是否有组合持仓明细数据, 判断m_bHaveComb
    bool HaveCombPositionDetail(void);

    //获取平仓时打散衍生的持仓明细合约iD
    void GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs);

    //获取查询结果的合约集合
    void GetInstrumentSetOfQryRlt(set<std::string>& outData);

    //清除所有数据
    void clear(void);

    //判断是否为空
    bool empty(void);

    //从m_mapOpenTime2Key中取出指定时间范围的PositionDetail
    //时间是time_t秒数，LowerOpenTime<=结果OpenTimeUTC<UpperOpenTime。昨仓持仓明细的OpenTimeUTC为开仓日期零时；今仓是准确的
    //持仓明细的OpenTimeUTC=CTools_Ansi::Get_time_t_utc_from_strTime(OpenDate,OpenTime)
    void GetPositionDetailsInOpenTimeScope(int LowerOpenTime,int UpperOpenTime,vector<PlatformStru_PositionDetail>& outPositionDetails);

    //获取所有持仓明细记录的FTID
    void GetAllFTID(set<long>& outFTIDs);

    //将组合合约ID设置到对应的持仓明细中
    void SetCombInstrumentIDs(const vector<PositionDetailCombKey>& combdetailkeys);

    //开仓成交时将组合合约ID设置到对应的持仓明细中。根据组合持仓明细的变化
    void SetCombInstrumentID_OpenTrade(const map<PositionDetailCombKey,PlatformStru_PositionDetailComb> ModifiedPositionDetailCombs);

    static void Convert_PositionDetailCombKey_to_PositionDetailKey(const PositionDetailCombKey& combkey,PositionDetailKey& detailkey);

    //设置期权今仓权利金计算使用的价格类型. 0: 按成交价计算(缺省)；1: 按昨结算价计算
    void SetPriceForRoyalty_Option(int PriceForRoyalty_Option) { m_PriceForRoyalty_Option=PriceForRoyalty_Option; }

    //设置如果有结算价的话，是否使用结算价计算盈亏和保证金
    void SetbUseSettlementPriceToCalculateProfitAndMarginAsPossible(bool bUse)
    {
        m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible=bUse;
    }

private:

    //从成交回报计算相关持仓明细仓位的变化。注意，只更新仓位，不更新其它值
    //PositionDetailData是数据集。可以是m_PositionDetailData,也可以是临时数据集(为了匹配成交和持仓明细)
    //bCareCombOppoDetail:表示持仓明细是组合衍生, 是否对其对手持仓明细进行打散操作。正常情况下需要，在初始化查询完毕重新演算是如果不是郑商所就不需要
    //ComTradeID2CloseVolume:平组合衍生仓位时，被平掉的组合成交ID和对应平仓量的关系
    void UpdatePositionDetailVolumeOnTrade_Internal(
        PlatformStru_TradeInfo& TradeInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        map<string,int>& ComTradeID2CloseVolume,
        CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetailData,
        CPlatformDataMgr_PositionDetailCombs& PositionDetailCombData,
        bool bCareCombOppoDetail);

    //动态更新合约对应的持仓明细记录的现价、昨结算价、结算价、持仓盈亏(盯市)、浮动盈亏(逐笔)，根据行情
    //新行情更新bForceCalculate为false，表示仅在价格变动时更新，其它情况为true
    //计算盈亏优先使用的价格顺序为：结算价 > 最新价 > 昨结算价
    void UpdatePositionDetailByQuot2_Internal(
        const PlatformStru_DepthMarketData& QuotData,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        bool bForceCalculate);

    //动态更新持仓明细记录的现价、昨结算价、结算价、持仓盈亏(盯市)、浮动盈亏(逐笔)，根据行情
    //新行情更新bForceCalculate为false，表示仅在价格变动时更新，其它情况为true
    //计算盈亏优先使用的价格顺序为：结算价 > 最新价 > 昨结算价
    void UpdatePositionDetailByQuot_Internal(
        const PlatformStru_DepthMarketData& QuotData,
        const PlatformStru_InstrumentInfo InstrumentInfo,
        bool bForceCalculate,
        PlatformStru_PositionDetail& detail,
        bool& bUpdate);

    //更新持仓明细记录的保证金、保证金率、交易所保证金，成交修改持仓明细和查询保证金率响应时调用，只对今仓处理
    //如果有结算价，按结算价计算；否则，按开盘价或昨结算价计算(根据组合持仓明细配置以及今仓昨仓属性)
    //对组合持仓明细，这里不考虑保证金优惠
    void CalculateMargin_Internal(
        const PlatformStru_InstrumentMarginRate& MarginRate,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        PlatformStru_PositionDetail& PositionDetail,
        bool& bUpdate);

    //更新期权合约持仓明细记录的保证金, 交易所保证金, 成交修改持仓明细和查询保证金率响应时调用
    //  每手保证金=MAX(权利金 + 投资者期权合约保证金不变部分, 投资者期权合约最小保证金)
    //  昨仓权利金 = if(上期所，[期权合约收盘价与结算价的较大值]，[期权合约结算价]) × 期权合约乘数
    //  新仓权利金 = 价格（昨结算价/成交价，由OptionRoyaltyPriceType定）*期权合约乘数
    void CalculateMargin_Option_Internal(
        const PlatformStru_InstrumentMarginRate& MarginRate,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        PlatformStru_PositionDetail& PositionDetail,
        bool& bUpdate);


    //判断持仓明细是不是(平仓)成交的对手持仓
    bool IsPositionDetailForCloseTrade_IgnoreTradeType(
        const PlatformStru_PositionDetail& PositionDetail,
        const PlatformStru_TradeInfo& TradeInfo);

    ////获取某一个(组合)持仓明细的对手持仓明细记录，注意，只找最早的配对今仓或昨仓
    ////用于成交更新持仓明细时打散组合持仓的计算。获取时用到组合持仓明细的数据
    //bool GetOpponentPositionDetailKey_Comb_Internal(
    //    bool bToday,
    //    const PlatformStru_PositionDetail& OriPositionDetail,
    //    PositionDetailKey& OppoPositionDetailKey,
    //    CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetailData);



    //从m_mapOpenTime2Key中取出指定账号，指定时间范围的key
    void GetKeysFrommapOpenTime2Key_Internal(int LowerOpenTime,int UpperOpenTime,vector<PositionDetailKey>& outkeys);

    //获取日初持仓明细
    void GetStaticPositionDetail_Internal(CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& StaticPositionDetail);

    //从成交记录更新持仓明细
    //返回平掉的持仓明细的记录集，用于后续计算。ModifiedPositionDetails保存受到影响的持仓明细记录，n*2+0是原来的数据，n*2+1是新数据
    //会修改TradeInfo的平今量、平仓盈亏
    //bPartCalcu:只计算平仓量、平仓盈亏，不计算保证金、持仓盈亏等
    //ComTradeID2CloseVolume:平组合衍生仓位时，被平掉的组合成交ID和对应平仓量的关系
    void UpdatePositionDetail_RtnTrade_Internal(
        PlatformStru_TradeInfo& TradeInfo,
        bool bPartCalcu,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        map<string,int>& ComTradeID2CloseVolume,
        CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& PositionDetails);

private:
    Ceasymutex				                m_mutex;

    CPlatformDataMgr_Instruments            &m_RefInstruments;
    CPlatformDataMgr_Rates                  &m_RefRates;
    CPlatformDataMgr_Quots                  &m_RefQuots;
    CPlatformDataMgr_Trades                 &m_RefTrades;
    CPlatformDataMgr_Orders                 &m_RefOrders;
    CPlatformDataMgr_PositionDetailCombs    &m_RefPositionDetailCombs;

    //平仓顺序: 0: 统一先开先平；1: 先单一后组合先开先平
    int                                     m_UnwindPrioritiesCZCE_Comb;
    int                                     m_UnwindPrioritiesDCE_Comb;

    //计算组合单今仓保证金使用的价格: 0: 按成交价计算(缺省)；1: 按昨结算价计算
    int                                     m_PriceForMarginOfTodayPositionCZCE_Comb;
    int                                     m_PriceForMarginOfTodayPositionDCE_Comb;
    int                                     m_PriceForRoyalty_Option;

    //组合单优惠策略，0-不优惠，1-第一腿优惠，2-第二腿优惠，3-按保证金比较高的一边收取(缺省)
    int                                     m_DiscountDCE_Comb;      
    int                                     m_DiscountCZCE_Comb;

    //----------下面是数据集----------
    CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail> m_PositionDetailData;

    //map: OpenTimeUTC -> PositionDetailKey，用于快速查找持仓时间大于多少的明细
    multimap<int,PositionDetailKey>         m_mapOpenTime2Key;  

    //是否有组合衍生的持仓明细记录 
    bool                                    m_bHaveComb;       

    //如果有结算价的话，是否使用结算价计算盈亏和保证金
    bool                                    m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible;

    //上一次平仓成交时，是否有被打散而衍生的普通持仓明细的合约ID。在GUI中更新持仓明细时，需要更新此衍生合约的持仓明细
    set<string>                             m_DerivedInstrumentIDs_OnCloseTrade;


};

#ifdef WIN32
#pragma managed(pop)
#endif 
