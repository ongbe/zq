/*
    用于保存和维护CTP的组合持仓数据
    组合持仓使用持仓的结构体
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <string>
using std::vector;
using std::string;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgr_PositionDetails.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CPlatformDataMgr_PositionCombs
{
public:
    CPlatformDataMgr_PositionCombs(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders,
        CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
        CPlatformDataMgr_PositionDetails& refPositionDetails);
    ~CPlatformDataMgr_PositionCombs(void);

    //查询回报到来
    void QryPositionCombReach( PlatformStru_Position& PositionInfo,int ErrID,bool bIsLast);

    //从成交记录更新组合持仓。此函数从组合持仓明细中重新创建并计算组合持仓。依赖于组合持仓明细，必须在组合持仓明细更新完后再更新组合持仓
    void Update_RtnTrade(const PlatformStru_TradeInfo& TradeInfo,
        const map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs);
    //更新组合持仓记录的保证金、保证金率，查询保证金率响应时调用
    bool UpdatePositionByMarginRate_RspQryRate(const PlatformStru_InstrumentMarginRate& MarginRate);

    //根据组合持仓明细和持仓明细，更新组合持仓的持仓成本、开仓成本、持仓均价和开仓均价。在初始化查询完毕后调用
    void UpdateCostByPositionDetialComb_AfterInitQry(void);


    //清空查询组合持仓的map
    void ClearMapQryRltPositionCombs(std::string& strQryInstrument);
    //获取组合持仓的集合
    int GetPositionCombs(std::vector<PlatformStru_Position>& outData);
    //获取指定合约的组合持仓集合
    int GetPositionCombs2(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData);
    //获取查询的合约ID
    void GetQryPositionCombInstrument(std::string& outQryInstrument);

    //获取组合持仓的FTID集合
    int GetPositionCombFTIDList(std::vector<long>& vec);
    //获取指定FTID的组合持仓明细
    int GetPositionCombInfo(long lFTID, PlatformStru_Position& outData);

    //取全部持仓的占用保证金和平仓盈亏(逐日盯市)。
    void GetAllMargin(double& TotalMargin);

    //清除所有数据
    void clear(void);
    //判断是否为空
    bool empty(void);


private:
    //根据组合持仓明细和持仓明细，更新组合持仓的持仓数、持仓成本、开仓成本、持仓均价、开仓均价和保证金。
    void UpdateCostByPositionDetialComb_Internal(const PositionKey& combkey,PlatformStru_Position& comb,bool bUpdateMargin_Position);

private:
    Ceasymutex				                m_mutex;

    CPlatformDataMgr_Instruments            &m_RefInstruments;
    CPlatformDataMgr_Quots                  &m_RefQuots;
    CPlatformDataMgr_Trades                 &m_RefTrades;
    CPlatformDataMgr_Orders                 &m_RefOrders;
    CPlatformDataMgr_PositionDetailCombs    &m_RefPositionDetailCombs;
    CPlatformDataMgr_PositionDetails        &m_RefPositionDetails;


    //----------下面是数据集----------
    CPlatformDataSetList<PositionKey,PlatformStru_Position> m_PositionCombData;


};

#ifdef WIN32
#pragma managed(pop)
#endif 
