/*
    用于在成交回报后查询持仓、持仓明细、资金
*/


#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <string>
#include "Module-Misc2/BusinessData.hpp"
#include "Module-Misc2/packagecache.h"



struct Stru_ReqKeyOnRtnTrade
{
    Stru_ReqKeyOnRtnTrade(const std::string& strbrokerid,const std::string& strinvestorid,const std::string& strinstrumentid)
    {
        strBrokerID=strbrokerid;
        strInvestorID=strinvestorid;
        strInstrumentID=strinstrumentid;
    };
    bool operator< (const Stru_ReqKeyOnRtnTrade& r) const
    {
        if(strBrokerID<r.strBrokerID) return true;
        if(strBrokerID>r.strBrokerID) return false;
        if(strInvestorID<r.strInvestorID) return true;
        if(strInvestorID>r.strInvestorID) return false;
        if(strInstrumentID<r.strInstrumentID) return true;
        if(strInstrumentID>r.strInstrumentID) return false;
        return false;
    };
    bool operator== (const Stru_ReqKeyOnRtnTrade& r) const
    {
        if(strBrokerID==r.strBrokerID||
            strInvestorID==r.strInvestorID||
            strInstrumentID==r.strInstrumentID) 
            return true;
        return false;
    };
    bool operator!= (const Stru_ReqKeyOnRtnTrade& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const Stru_ReqKeyOnRtnTrade& r) const
    {
        return !(*this < r ||*this == r);
    }


	///经纪公司代码
    std::string strBrokerID;
	///投资者代码
    std::string strInvestorID;
	///合约代码
    std::string strInstrumentID;
};

struct Stru_ReqOnRtnTradeTime
{
    Stru_ReqOnRtnTradeTime()
    {
        memset(this,0,sizeof(*this));
    }

    DWORD OnRtnTradeTime;
    DWORD QryPositionDetailTime;
    DWORD QryPositionTime;
    DWORD QryPositionCombTime;
    DWORD QryAccountTime;
    bool bQryPositionComb;             //是否需要查询组合持仓
    int RecordCountOfPositionDetail;    //持仓明细记录集的大小，超过阀值则不自动查询持仓明细
};

class CReqOnRtnTrade
{
public:
    static void Init(void);
    static void Release(void);

    //有成交回报到来
    static void OnRtnTrade(const std::string& strBrokerID,
                            const std::string& strInvestorID,
                            const std::string& strInstrumentID,
                            bool bPositionComb,
                            int RecordCountOfPositionDetail);

    //设置持仓查询优先级，1:持仓；2:持仓明细；3:组合持仓
    static void SetQryPrior(int QryPrior);

    //定时检查是否需要启动查询
    static void CheckOnTime(CServiceProxy& rServiceProxy);

private:
    //查询持仓
    static void ReqPosition_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,Stru_ReqOnRtnTradeTime& ReqTime, CServiceProxy& rServiceProxy);
    //查询持仓明细
    static void ReqPositionDetail_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,Stru_ReqOnRtnTradeTime& ReqTime, CServiceProxy& rServiceProxy);
    //查询组合持仓明细
    static void ReqPositionComb_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,Stru_ReqOnRtnTradeTime& ReqTime, CServiceProxy& rServiceProxy);
    //查询资金
    static void ReqTradingAccount_Internal(const Stru_ReqKeyOnRtnTrade& ReqKey,Stru_ReqOnRtnTradeTime& ReqTime, CServiceProxy& rServiceProxy);


private:

    static bool m_bInit;
    static CRITICAL_SECTION m_CS;

    //成交回报的合约和时间(毫秒)的map
    static std::map<Stru_ReqKeyOnRtnTrade,Stru_ReqOnRtnTradeTime> m_mapReqTime;

    //持仓查询优先级，1:持仓；2:持仓明细；3:组合持仓。查询顺序(如m_QryPrior==1)： 持仓、资金、持仓明细、组合持仓，注意，资金查询一定在持仓之后
    static int m_QryPrior;


};

