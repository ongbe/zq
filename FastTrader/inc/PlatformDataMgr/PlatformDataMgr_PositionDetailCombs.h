/*
    用于保存和维护CTP的组合持仓明细数据
    注意：两腿不一定是一对一的关系。如两手a&b, 可能是a1b1a1b1的成交顺序，也可能是a1a1b2的成交顺序
    added by l. 20130204
*/


#pragma once

#include <vector>
#include <map>
#include <string>
#include <set>
using std::string;
using std::map;
using std::vector;
using std::set;

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CPlatformDataMgr_PositionDetailCombs
{
public:
    CPlatformDataMgr_PositionDetailCombs(
        volatile long& baseFTID,
        volatile long& baseUpdateSeq,
        CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders);
    ~CPlatformDataMgr_PositionDetailCombs(void);

    //查询回报到来
    void QryPositionDetailCombReach( 
        PlatformStru_PositionDetailComb& PositionDetailCombInfo,
        int ErrID,
        bool bIsLast);

    //从成交记录更新组合持仓明细。依赖于持仓明细的变化，必须在持仓明细更新完后再更新持仓
    //ComTradeID2CloseVolume:平组合衍生仓位时，被平掉的组合成交ID和对应平仓量的关系
    void Update_RtnTrade(
        const PlatformStru_TradeInfo& TradeInfo,
        const map<PositionDetailKey,int>& mapCloseDeltaVolume,
        const map<string,int>& ComTradeID2CloseVolume,
        map<PositionDetailCombKey,PlatformStru_PositionDetailComb>&ModifiedPositionDetailCombs);



    //根据持仓明细key，取对应的组合持仓明细key和对手组合持仓明细key
    //返回值：map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>>, ComTradeID -> pair<leg1keys,leg2keys>
    void GetCombKeysByPositionDetailKey2(const PositionDetailKey& detailkey,map<string,pair<set<PositionDetailCombKey>,set<PositionDetailCombKey>>>& outkeys,bool bFilterZeroVolume);
    //根据持仓明细key，取对应的组合持仓明细key
    void GetCombKeysByPositionDetailKey(const PositionDetailKey& detailkey,set<PositionDetailCombKey>& outkeys,bool bFilterZeroVolume);
    void GetCombKeysByComTradeID(const string& ComTradeID,set<PositionDetailCombKey>& outkeys,bool bFilterZeroVolume);
    void GetCombKeysByComTradeID2(const string& ComTradeID,set<PositionDetailCombKey>& leg1keys,set<PositionDetailCombKey>& leg2keys,bool bFilterZeroVolume);
    void GetComTradeIDsByPositionDetailKeys(const set<PositionDetailKey>& detailkeys,set<string>& outdatas,bool bFilterZeroVolume);

    //取保证金，失败返回无效值
    double GetExchMargin(const PositionDetailCombKey& key);
    bool GetMargin(const PositionDetailCombKey& key,double& Margin,double& ExchMargin);
    //设置保证金
    bool SetMargin(const PositionDetailCombKey& key,double Margin,double ExchMargin);

    //根据持仓明细的保证金数据，设置组合持仓明细的保证金，不考虑优惠。在进行优惠计算前做一下。
    void SetMarginByPositionDetail(const PlatformStru_PositionDetail& detail);

    //清空查询组合持仓明细的map
    void ClearMapQryRltPositionDetailCombs(string& strQryInstrument);

    //获取查询的合约ID
    void GetQryPositionDetailCombInstrument(string& outQryInstrument);

    //清除所有数据
    void clear(void);

    //设置保证金。成交修改组合持仓明细时，可能要优惠掉某一腿的持仓明细的保证金
    bool SetMargin(const PositionDetailCombKey& key,double Margin,double ExchMargin,double MarginRateByMoney,double MarginRateByVolume);

    //取LegID
    bool GetLegID(const PositionDetailCombKey& key,int& outLegID);

    //根据一个分腿合约，获取相关组合持仓对应的保证金。查询保证金率返回时，根据组合持仓明细的保证金设置组合持仓保证金
    void GetPositionMarginByLegInstrument(const string& LegInstrumentID,map<PositionKey,double>& outMargin,map<PositionKey,double>& outExchMargin);

    //获取一个组合持仓对应的所有组合持仓明细keys
    void GetPositionDetailCombKeysByPositionCombKey(const PositionKey& positionkey,vector<PositionDetailCombKey>& outkeys);

    //取组合持仓明细
    bool GetPositionDetailComb(const PositionDetailCombKey& detailcombkey, PlatformStru_PositionDetailComb& outData);
    //取组合合约ID
    bool GetCombInstrumentID(const PositionDetailCombKey& detailcombkey,string& CombInstrumentID);

    //取组合合约对应的所有组合持仓明细key的集合
    void GetKeysByCombInstrumentID(const string& CombInstrumentID,set<PositionDetailCombKey>& keys);

    //根据日初持仓明细，获取日初组合持仓明细。仅对郑商有效
    void GetStaticPositionDetailComb_CZCE(CPlatformDataSetList<PositionDetailKey,PlatformStru_PositionDetail>& StaticPositionDetail,
        vector<PlatformStru_PositionDetailComb>& vecPositionDetailCombs);

    //取所有未平空的key
    void GetAllKeys(vector<PositionDetailCombKey>& outData);


private:
    //由组合成交创建组合持仓明细
    void CreatePositionDetailCombFromOpenTrade_Internal(const PlatformStru_TradeInfo &TradeInfo,map<PositionDetailCombKey,PlatformStru_PositionDetailComb>& ModifiedPositionDetailCombs);


private:
    Ceasymutex				        m_mutex;
    //判断是否为空
    bool empty(void);

    CPlatformDataMgr_Instruments    &m_RefInstruments;
    CPlatformDataMgr_Quots          &m_RefQuots;
    CPlatformDataMgr_Trades         &m_RefTrades;
    CPlatformDataMgr_Orders         &m_RefOrders;


    //----------下面是数据集----------
    CPlatformDataSetList<PositionDetailCombKey,PlatformStru_PositionDetailComb> m_PositionDetailCombData;

    //分腿合约和组合合约之间的关系
    multimap<string,string>                             m_mapLegInstrumentID2CombInstrumentID;

    //组合合约与组合持仓明细Key之间的关系，key是排序的
    map<string,set<PositionDetailCombKey>>              m_mapCombInstrumentID2Key;

    //组合持仓key和组合持仓明细key之间的关系
    multimap<PositionKey,PositionDetailCombKey>         m_mapPositionCombKey2Key;

    //ComTradeID和Key之间的关系
    multimap<string,PositionDetailCombKey>              m_mapComTradeID2Key;

    //持仓明细Key和组合Key之间的关系，今仓是一对一，昨仓可能是一对多
    map<PositionDetailKey,set<PositionDetailCombKey>>   m_mapPositionDetailKey2Key;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

