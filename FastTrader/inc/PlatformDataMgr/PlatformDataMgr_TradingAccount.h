/*
    用于保存和维护CTP的账户资金数据
    added by l. 20130204
*/


#pragma once

#include <string>

#include "easymutex.h"

#include "PlatformDataSetList.hpp"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetails.h"
#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgr_PositionCombs.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CPlatformDataMgr_TradingAccount
{
public:
    CPlatformDataMgr_TradingAccount(CPlatformDataMgr_Instruments& refInstruments,
        CPlatformDataMgr_Quots& refQuots,
        CPlatformDataMgr_Trades& refTrades,
        CPlatformDataMgr_Orders& refOrders,
        CPlatformDataMgr_PositionDetailCombs& refPositionDetailCombs,
        CPlatformDataMgr_PositionDetails& refPositionDetails,
        CPlatformDataMgr_PositionCombs& refPositionCombs,
        CPlatformDataMgr_Positions& refPositions);
    ~CPlatformDataMgr_TradingAccount(void);

    ///设置交易资金账户信息
    void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);
    int GetTradingAccountAvailable(double& fAvailable);
	double GetTradingAccountDynamicProfit(void);
    int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
    int GetTradingAccountID(char* AccountID,int rltsize);
    int GetTradingAccount(PlatformStru_TradingAccountInfo& outData);

    ///获取账户资金文本信息
    int GetAccountText(std::string& outData,int language);
    ///查询报单\保证金率响应时，动态更新资金账户信息
    void UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData(void);
    ///行情更新时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RtnDepthMarketData(const std::string InstrumentID);
    ///成交回报时，动态更新资金账户信息
    void UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted(void);
    ///查询保证金率响应时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RspQryRate(void);
    ///查询手续费率响应时，动态更新资金账户信息
    void DynamicUpdateTradingAccount_RspQryInstrumentCommissionRate(void);

    //清除所有数据
    void clear(void);
    bool IsValid(void);

private:
    //计算资金数据中的静态权益值
    void CalcuStaticProfic_Internal();
    //计算资金数据中的动态权益值
    void CalcuDynamicProfic_Internal();
    //计算资金数据中的可用资金
    void CalcuAvailable_Interanl();
    //计算资金数据中的风险度值
    void CalcuRiskDegree_Internal();

private:
    Ceasymutex				            m_mutex;

    CPlatformDataMgr_Instruments        &m_RefInstruments;
    CPlatformDataMgr_Quots              &m_RefQuots;
    CPlatformDataMgr_Trades             &m_RefTrades;
    CPlatformDataMgr_Orders             &m_RefOrders;
    CPlatformDataMgr_PositionDetailCombs&m_RefPositionDetailCombs;
    CPlatformDataMgr_PositionDetails    &m_RefPositionDetails;
    CPlatformDataMgr_PositionCombs      &m_RefPositionCombs;
    CPlatformDataMgr_Positions          &m_RefPositions;


    //资金账户信息：资金账户在系统启动前查询一次，然后根据报单回报和成交回报动态更新
    PlatformStru_TradingAccountInfo     m_TradingAccount;   //资金账户信息
    bool                                m_bValid;           //设置初始资金后才是有效的

};

#ifdef WIN32
#pragma managed(pop)
#endif 
