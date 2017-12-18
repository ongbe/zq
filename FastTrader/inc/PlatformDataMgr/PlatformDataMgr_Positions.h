/*
    用于保存和维护CTP的持仓数据
    对于组合合约的持仓，只包含其分腿持仓数据，不包含组合持仓数据
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <set>
#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetails.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_Positions
{
public:
    CPlatformDataMgr_Positions(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Rates& refRates,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders,
        CPlatformDataMgr_PositionDetails& refPositionDetails,
        bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible);
    ~CPlatformDataMgr_Positions(void);

    //查询回报到来
    void QryPositionReach( PlatformStru_Position& PositionInfo,int ErrID,bool bIsLast);

	//报单回报时，更新持仓相关的挂单的冻结手续费和冻结保证金
	void UpdatePositionFreezeFields_RtnOrder(const PlatformStru_OrderInfo& Order,
        const PlatformStru_OrderInfo& OldOrderInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //从成交记录更新持仓。此函数从持仓明细中重新创建并计算持仓。依赖于持仓明细，必须在持仓明细更新完后再更新持仓
    void UpdatePosition_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
        const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails,
        const PlatformStru_InstrumentInfo& InstrumentInfo);

    //动态更新持仓记录的持仓盈亏(盯市)、浮动盈亏(逐笔)、盯市总盈亏、结算价、昨结算价，得到新行情时做一遍
    void UpdatePosition_RtnQuot(const PlatformStru_DepthMarketData& QuotData,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        bool bForceCalculate);
    //结算时更新持仓。根据结算价，更新持仓的盈亏、保证金
    void UpdatePosition_OnSettlement(void);
    //动态更新持仓记录的保证金、保证金率，查询保证金率响应时调用。返回是否进行了更新
    bool UpdatePositionByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate);

    //动态更新持仓记录的手续费，成交修改持仓和查询手续费率响应时调用
    bool UpdatePositionByCommissionRate_RspQryRate(const PlatformStru_InstrumentCommissionRate& CommissionRate);
    //费率查询响应时，更新持仓相关的挂单的冻结手续费和冻结保证金
    void UpdatePositionFreezeFieldsByOrders_RspQryRate(const std::string& strInstrument);

    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
    int GetPositionData(const std::string& strAccount,const std::string& InstrumentID,char PosiDirection,char HedgeFlag,PlatformStru_Position& PositionData);
    //获取所有持仓记录，排除数量为0的记录，返回结果个数
    int GetPositions(std::vector<PlatformStru_Position>& outData);
    //获取所有持仓记录，包含数量为0的记录，返回结果个数
    int GetPositions4(std::vector<PlatformStru_Position>& outData);
    //返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败; 
    //lastseq: 表示数据更新的序列号。如果当前的更新序列号等于lastseq，表示没有更新，返回1(不返回数据)。0表示不判断。函数返回时，lastseq被设置为最新的更新序列号
    int GetPositions2(const std::string& strInstrument,std::set<long>& setFTID,std::vector<PlatformStru_Position>& outData,long& lastseq);
    //返回0：成功，并返回数据，1: 成功，但由于没有更新，不返回数据；-1：失败. lastseq注释见上一条
    int GetPositions3(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData,long& lastseq);

    //清空查询持仓的map
    void ClearMapQryRltPositions(const std::string& strQryInstrument);
    //获取查询的合约ID
    void GetQryPositionInstrument(std::string& outQryInstrument);

    //获取持仓FTID集合，返回结果个数
    int GetPositionFTIDList(std::vector<long>& vec);
    //获取指定FTID的持仓，成功返回0，失败返回-1
    int GetPositionInfo(long lFTID, PlatformStru_Position& outData);
    //获取指定key的持仓信息，成功返回0，失败返回-1
    int GetPositionInfo(PositionKey key, PlatformStru_Position& outData);

    //取全部持仓的占用保证金。考虑上期大额单边保证金优惠
    void GetAllMargin(double& TotalMargin);
    //取平仓盈亏(逐日盯市)
    void GetAllCloseProfitByDate(double&TotalCloseProfitByDate);
    //取全部持仓的持仓盈亏(逐日盯市)
    void GetAllPositionProfitByDate(double&TotalPositionProfitByDate);

    //清除所有数据
    void clear(void);
    //判断是否为空
    bool empty(void);


public:
    //注意：下面定义几个static函数，是为了在组合持仓中也能使用

	//查询持仓时合并同一合约同一方向的历史持仓和今日持仓记录
	static void MergePositions_RspQryPosition( PlatformStru_Position& firstPositionInfo, const PlatformStru_Position& secondPositionInfo );

    //在持仓数据中，计算平仓量和可平仓量, (根据所有挂单)计算全部
    static void CalcuCloseValueInPositionByAllOrders_Internal(PlatformStru_Position& PositionData,
        int CloseVolume,
        int CloseTodayVolume,
        int CloseYesterdayVolume);

    //设置如果有结算价的话，是否使用结算价计算盈亏和保证金
    void SetbUseSettlementPriceToCalculateProfitAndMarginAsPossible(bool bUse)
    {
        m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible=bUse;
    }

private:
    //动态更新持仓记录的持仓盈亏(盯市)、浮动盈亏(逐笔)和盯市总盈亏，结算价、昨结算价和现价
    //计算盈亏优先使用的价格顺序为：结算价 > 最新价 > 昨结算价
    void UpdatePositionByQuot_Internal(PlatformStru_Position& PositionInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo,
        const PlatformStru_DepthMarketData& QuotData,
        bool bForceCalculate);
    //根据一个开仓报单变化数据更新持仓的冻结手续费、冻结保证金和冻结金额
    //持仓是单腿合约，报单是单腿合约或组合合约
    void RefreshPositionFreezeByOneOpenOrder_Internal(const PlatformStru_OrderInfo& Order,
        const PlatformStru_OrderInfo& OldOrderInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo);
    //根据一个平仓报单变化数据更新持仓的挂单量、可平量
    //持仓一定是单腿持仓，报单是非组合报单或组合报单
    void RefreshPositionCloseOrderVolumeByOneOrder_Internal(const PlatformStru_OrderInfo& Order,
        const PlatformStru_OrderInfo& OldOrderInfo,
        const PlatformStru_InstrumentInfo& InstrumentInfo);
    //在持仓数据中，计算平仓量和可平仓量, (根据一个挂单)进行增量计算
    //Deltaxxx是增量，一般来说是负值
    //此函数针对单腿合约持仓
    void CalcuCloseValueInPositionByOneOrder_Internal(PlatformStru_Position& PositionData,
        int DeltaCloseVolume,int DeltaCloseTodayVolume,int DeltaCloseYesterdayVolume);


    //更新持仓相关的挂单的冻结手续费和冻结保证金。成交新建持仓时、得到费率时调用
    void UpdatePositionFreezeFieldsByAllOrders_Internal(PlatformStru_Position& PositionData);

    //动态更新持仓记录的保证金、保证金率，成交修改持仓时调用
    //这里要考虑组合单被打散的情况。另一腿(指被打散的那腿)会变成普通持仓明细，对应的持仓的保证金会增加
    void UpdatePositionByMarginRate_RtnTrade_Internal(PlatformStru_Position& PositionInfo,
        const map<PositionDetailKey,PlatformStru_PositionDetail>& ModifiedPositionDetails);

    //动态更新持仓记录的保证金、保证金率，查询保证金率响应和结算时调用
    void UpdatePositionByMarginRate_RspQryRate_OnSettlement_Internal(const PlatformStru_InstrumentMarginRate& MarginRate,
        PlatformStru_Position& PositionInfo,
        bool bForce);


private:
    Ceasymutex				            m_mutex;

    CPlatformDataMgr_Instruments        &m_RefInstruments;
    CPlatformDataMgr_Rates              &m_RefRates;
    CPlatformDataMgr_Quots              &m_RefQuots;
    CPlatformDataMgr_Trades             &m_RefTrades;
    CPlatformDataMgr_Orders             &m_RefOrders;
    CPlatformDataMgr_PositionDetails    &m_RefPositionDetails;

    //如果有结算价的话，是否使用结算价计算盈亏和保证金
    bool                                    m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible;

    //----------下面是数据集----------
    CPlatformDataSetList<PositionKey,PlatformStru_Position> m_PositionData;
};

#ifdef WIN32
#pragma managed(pop)
#endif 
