#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRTRADINGSTRATEGY_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTRADINGSTRATEGY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTRADINGSTRATEGY_EXPORTS
#define SVRTRADINGSTRATEGY_API __declspec(dllexport)
#else
#define SVRTRADINGSTRATEGY_API __declspec(dllimport)
#endif
#endif

#include "CommonStruct.h"
#include <map>
#include <set>
#include <vector>

class SVRTRADINGSTRATEGY_API CInterface_SvrTradingStrategy : public CInterface_SvrCommon
{
public:
    static CInterface_SvrTradingStrategy& getObj(void);

    //查询交易策略
    virtual void GetTradeStrategy(std::vector<TradeStrategy>& vecStrategy )=0;
    //查询交易策略与组织机构关系
    virtual void GetAssetOrgStrategyRelation(std::map<int,std::vector<int>>& mapRelation)=0;
    //通过组织机构ID获得交易策略
    virtual void GetStrategyIDsByOrgID(int nOrgID,std::vector<int>& vecStrategyID )=0;
    //通过交易策略ID获得组织机构 
    virtual void GetOrgIDsByStrategyID (int nStrategyID,std::vector<int>& vecOrgID )=0;
    //通过交易策略ID取策略信息
    virtual void GetInfoByStrategyID (int nStrategyID,TradeStrategy& oStrategy)=0;
};







