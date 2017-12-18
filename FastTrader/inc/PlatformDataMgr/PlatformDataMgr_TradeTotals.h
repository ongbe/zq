/*
    用于保存和维护CTP的成交统计数据
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Trades.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_TradeTotals
{
public:
    CPlatformDataMgr_TradeTotals(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Trades& refTrades);
    ~CPlatformDataMgr_TradeTotals(void);


    //查询的成交回报收全，开始计算成交统计
    void QryTradeTotalReach(bool bIsLast);
    //成交回报时，更新相应合约的成交统计
    void UpdateTradeTotal_RtnTrade(const PlatformStru_TradeInfo& TradeInfo);
    //获得手续费率响应时，更新相应合约的成交统计
    bool UpdateTradeTotal_RspQryCommissionRate(const std::string& strInstrument);

    ///获取全部成交统计记录
    int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData);
    ///获取指定合约的成交统计记录，返回数据项的数量
    int GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData);


    //清除所有数据
    void clear(void);
    //判断是否为空
    bool empty(void);

private:
    Ceasymutex				m_mutex;
    CPlatformDataMgr_Trades &m_RefTrades;

    ////----------下面是成交统计数据集----------
    CPlatformDataSetList<TradeTotalKey,PlatformStru_TradeTotalInfo> m_TradeTotalData;
};

#ifdef WIN32
#pragma managed(pop)
#endif 
