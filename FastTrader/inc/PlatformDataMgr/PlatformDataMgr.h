/*
    用于保存和维护底层数据
    包括：合约数据、费率数据、持仓明细数据、报单数据、成交单数据、资金数据等
    added by l. 20110216
*/


#pragma once

#include "easymutex.h"

#include "PlatformDataMgr_LoginInfo.h"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Rates.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_TradeTotals.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgr_PositionCombs.h"
#include "PlatformDataMgr_PositionDetails.h"
#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgr_TradingAccount.h"


#ifdef WIN32
#pragma managed(push,off)
#endif 



class CPlatformDataMgr
{
public:
    //bUseComb                              :是否使用组合单功能
    //DiscountDCE_Comb                      :大商所组合单优惠策略，0-不优惠，1-第一腿优惠，2-第二腿优惠，3-按保证金比较高的一边收取(缺省)
    //DiscountCZCE_Comb                     :郑商所组合单优惠策略，
    //UnwindPrioritiesCZCE_Comb             :平仓顺序: 0: 统一先开先平；1: 先单一后组合先开先平
    //UnwindPrioritiesDCE_Comb              :
    //PriceForMarginOfTodayPositionCZCE_Comb:计算今仓保证金使用的价格: 0: 按成交价计算(缺省)；1: 按昨结算价计算
    //PriceForMarginOfTodayPositionDCE_Comb :
    //bFreezeOrderUnknownStatus             :对状态为未知的报单(如手工审核单)，是否执行资金冻结
    //PlatformID                            :平台ID，对多账号和机构版服务器有用，用于将FTID分区
    //MaxPlatformID                         :平台ID的最大值，用于多账号和机构版服务器将FTID分区
    CPlatformDataMgr(
        bool bCalculate_OnRtnTrade=true,
        bool bUseComb=false,
        int DiscountCZCE_Comb=3,
        int DiscountDCE_Comb=3,
        int UnwindPrioritiesCZCE_Comb=1,
        int UnwindPrioritiesDCE_Comb=0,
        int PriceForMarginOfTodayPositionCZCE_Comb=0,
        int PriceForMarginOfTodayPositionDCE_Comb=1,
        bool bFreezeOrderUnknownStatus=false,
        int PlatformID=0,
        int MaxPlatformID=1,
        bool bUseSettlementPriceToCalculateProfitAndMarginAsPossible=false);
    ~CPlatformDataMgr(void);


    //设置投资者信息
    void SetInvestorInfo(const CThostFtdcInvestorField& inData);


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

    //保存汇率信息
    void SaveExchangeRate(const PlatformStru_ExchangeRate& ExchangeRate);
    //获取币种信息
    vector<string> GetCurrencys(const string& BankName,bool bAddUSD=true,bool bAddCNY=true);
    vector<string> GetCurrencys(bool bAddUSD=true,bool bAddCNY=true);

    //设置/获取经纪公司交易参数
    void SetBrokerTradingParam(const PlatformStru_BrokerTradingParams& BrokerTradingParam);
    const PlatformStru_BrokerTradingParams& GetBrokerTradingParam(void);

	SERVER_PARAM_CFG&  GetServerParamCfg();

	//清除所有数据
    void clear(bool bAlwaysClearInstruments_Rates=true);


    //-------------------------------------------------
    //-------------------------------------------------
    //处理深度行情
    void OnRtnDepthMarketData(const PlatformStru_DepthMarketData& MarketData);
    //请求查询行情响应。当组合合约报单需要行情的昨结算价时，会触发查询单腿合约的行情
    void OnRspQryDepthMarketData(const PlatformStru_DepthMarketData& MarketData);
    //报单回报
    void OnRtnOrder(PlatformStru_OrderInfo& OrderInfo);
    //成交回报
    void OnRtnTrade(PlatformStru_TradeInfo& TradeInfo);
    //请求查询产品响应
    void OnRspQryProduct(const PlatformStru_ProductInfo& ProductInfo);
    //请求查询合约响应
    void OnRspQryInstrument(PlatformStru_InstrumentInfo& InstrumentInfo,bool bCTPSimulate,bool bLast);
    //请求查询投资者持仓响应。ErrorID==0表示数据正确；ErrorID==123456789表示查询无记录
    void OnRspQryInvestorPosition(PlatformStru_Position& PositionInfo,int ErrorID,bool bIsLast);
    //请求查询投资者持仓明细响应
    void OnRspQryInvestorPositionDetail(PlatformStru_PositionDetail& PositionDetailInfo,int ErrorID,bool bIsLast);
    //请求查询投资者组合持仓明细响应
    void OnRspQryInvestorPositionCombDetail(PlatformStru_PositionDetailComb& PositionDetailCombInfo,int ErrorID,bool bIsLast);
    //请求查询成交响应
    void OnRspQryTrade(PlatformStru_TradeInfo& TradeInfo,int ErrorID,bool bIsLast);
    //请求查询报单响应
    void OnRspQryOrder(PlatformStru_OrderInfo& OrderInfo,int ErrorID,bool bIsLast);
    //查询交易资金账户信息响应
    void OnRspQryTradingAccount(PlatformStru_TradingAccountInfo& TradingAccount);
    //请求查询合约保证金率响应
    void OnRspQryInstrumentMarginRate(PlatformStru_InstrumentMarginRate& MarginRate,int ErrorID);
    //请求查询合约手续费率响应
    void OnRspQryInstrumentCommissionRate(PlatformStru_InstrumentCommissionRate& CommissionRate,int ErrorID);


    //是否有经纪公司交易参数信息
    bool IsHaveBrokerTradingParam(void) { return m_bHaveBrokerTradingParam; }
public:
	SERVER_PARAM_CFG m_ServerParamCfg;


private:
    const int                   m_PlatformID;
    const int                   m_MaxPlatformID;

    //为了兼容多账号版本，这里使用static，以便于在多账号中使用统一的FTID和UpdateSeq, 避免由于不同账号的FTID相同导致UI更新异常
    //每个记录数据的FTID的不变的，因此多账号里使用同一个baseFTID不会引起long不够用的问题。
    //UpdateSeq在多账号里可能出现long不够用的问题，如7fffffff递增后0。由于UpdateSeq只进行是否相等的判断，不进行大于小于的判断，因此折返为0的情况不影响系统运行
    static volatile long        m_baseFTID;                              //FTID基准值，每次使用后递增
    static volatile long        m_baseUpdateSeq;                         //UpdateSeq基准值，每次使用后递增

private:
    //杂项数据集
    CThostFtdcInvestorField     m_InvestorInfo;//投资者个人信息
    bool                        m_bInvestorInfoValid;

    std::string m_LastSettlementInfo;
    bool m_bSettlementInfoIsReady;

    Ceasymutex				m_mutex;

	std::map<std::string,  PlatformStru_ContractBank> m_ContractBankInfo;
	std::map<std::string,  CThostFtdcContractBankField> m_BankName;
    //汇率信息
    map<pair<string,string>,PlatformStru_ExchangeRate> m_ExchangeRates;
    //银行支持的币种，包括查询银期签约关系时查到的币种
    map<string,vector<string>> m_BankCurrency;

    //成交时是否触发计算
    bool m_bCalculate_OnRtnTrade;
    //是否使用组合单功能
    bool m_bUseComb;
	//组合单保证金优惠策略, 0-不优惠，1-第一腿优惠，2-第二腿优惠，3-按保证金比较高的一边收取(缺省)
	int m_DiscountCZCE_Comb;
	int m_DiscountDCE_Comb;
    //平仓顺序: 0: 统一先开先平；1: 先单一后组合先开先平
    int m_UnwindPrioritiesCZCE_Comb;
    int m_UnwindPrioritiesDCE_Comb;
    //计算今仓保证金使用的价格: 0: 按成交价计算(缺省)；1: 按昨结算价计算
    int m_PriceForMarginOfTodayPositionCZCE_Comb;
    int m_PriceForMarginOfTodayPositionDCE_Comb;

    //经纪公司交易参数，其中的期权权利金价格类型在计算权利金时用到
    PlatformStru_BrokerTradingParams m_BrokerTradingParam;
    bool                             m_bHaveBrokerTradingParam;

public:
	//下面是各项数据对象。要注意它们的先后顺序，构造时要按这个顺序来构造
    CPlatformDataMgr_LoginInfo              m_LoginInfo;
    CPlatformDataMgr_Instruments            m_Instruments;
    CPlatformDataMgr_Rates                  m_Rates;
    CPlatformDataMgr_Quots                  m_Quots;
    CPlatformDataMgr_Trades                 m_Trades;
    CPlatformDataMgr_TradeTotals            m_TradeTotals;
    CPlatformDataMgr_Orders                 m_Orders;
    CPlatformDataMgr_PositionDetailCombs    m_PositionDetailCombs;
    CPlatformDataMgr_PositionDetails        m_PositionDetails;
    CPlatformDataMgr_PositionCombs          m_PositionCombs;
    CPlatformDataMgr_Positions              m_Positions;
    CPlatformDataMgr_TradingAccount         m_TradingAccount;

private:

};

#ifdef WIN32
#pragma managed(pop)
#endif 
