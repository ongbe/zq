#pragma once

#include "Interface_SvrCommon.h"
#include "CommonStruct.h"
#include <map>
#include <set>


using namespace std;

#ifndef SVRTRADEDATA_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTRADEDATA_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTRADEDATA_EXPORTS
#define SVRTRADEDATA_API __declspec(dllexport)
#else
#define SVRTRADEDATA_API __declspec(dllimport)
#endif
#endif

#include "KernelStruct.h"
#include "BusinessData.hpp"
#include "Interface_DBOpr.h"



class SVRTRADEDATA_API CInterface_SvrTradeData : public CInterface_SvrCommon
{
public:
    static CInterface_SvrTradeData& getObj(void);

/***********************************公共合约和行情的操作******************************************************************************/
public: //交易所状态通知	
	virtual void SetIntrumentsStatus(const CThostFtdcInstrumentStatusField & nStatus) = 0;

	//判断是否是交易日
	//字符串为日期格式2013-08-09
	virtual bool IsTradingDay(const std::string & nsDay) = 0;

	//判断交易日是否结算
	//字符串为日期格式2013-08-09
	virtual bool IsSettlemented(const std::string & nsDay) = 0;

	//获取一个日期的上一个交易日
	virtual bool GetLastTrading(const std::string & nsDay, std::string & nsLastDay) = 0;

	//设置当前交易日
	virtual void SetCurrentTradingDay(const std::string & nsCurrentTradingDay,bool nbSetByMySelf=false) = 0;
	
	//获取当前交易日
	virtual void GetCurrentTradingDay(std::string & nsCurrentTradingDay) = 0;

	//交易数据管理模块初始化
	virtual void TradeDataInit(bool nbForce = false) = 0;

	//报盘设置每个账户的链接CTP的状态
	virtual void SetAccountStatus(const std::string & nsAccoutnID,
		const std::string & nsBrokerID,
		const EnumAccountStatus& neUserStatus) = 0;

	//获取每个账户的ctp状态
	virtual void GetAccountStatus(const std::string & nsAccoutnID,
		const std::string & nsBrokerID,
		EnumAccountStatus& neCTPStatus,
		double & nStatusTime) = 0;

    //获取所有委托交易账号的状态
	virtual void GetAllAccountStatus(std::vector<sAccountStatus> & lStatus) = 0;

	//获取所有用户的初始化状态
	virtual void GetAlUserInitStatus(std::vector<sUserInitStatus> & lStatus) = 0;

	//获取用户的ctp链接状态
	virtual EnumAccountStatus GetUserCTPStatus(int nUserID,double & nDifftime) = 0;

	//获取用户的初始化状态
	virtual eUserInitStatus GetUserInitStatus(int nUserID) = 0;

	//获取用户的ctp链接状态
	virtual EnumAccountStatus GetUserCTPStatus(const std::string& nsUser,double & nDifftime) = 0;

	//获取用户的初始化状态
	virtual eUserInitStatus GetUserInitStatus(const std::string& nsUser) = 0;

	//获取交易数据管理模块状态
	virtual eTradeDataStatus GetTraderDataStatus() = 0;

	//获取交易数据管理模块状态
	virtual eExchangeStatus GetExchangeStatus() = 0;

	//登陆和初始化一个委托交易账号
	virtual CF_ERROR LoginAdnInitTraderAccount(const BrokerAccountKey & nKey,int nAccountID) = 0;

public: //合约操作

	///设置指定合约信息
	virtual void SetInstrumentInfo(PlatformStru_InstrumentInfo& outData,bool bLast) = 0;

	///添加主力合约列表
	virtual void AddMainInstrumentList(std::string instrument) = 0;

    //获取交易日
	virtual void GetExchangeData(ExchangeTime & nData) = 0;

	///获取合约列表
	virtual int GetInstrumentList(std::set<std::string> &outData,
		std::string ExchangeID="") = 0;

	///获取主力合约列表
	virtual int GetMainInstrumentList(std::set<std::string> &outData) = 0;

	///设置合约容差列表
	virtual void SetInstrumentVarietyMap(map<string, string>& inData) = 0;

	// 获取合约容差列表
	virtual int GetInstrumentVarietyMap(map<string, string>& outData) = 0;

	///获取指定品种的合约列表,返回合约个数
	virtual int GetInstrumentListByProductID(const std::string& ProductID,
		std::set<std::string> &outData) = 0;

	///获取指定合约信息
	virtual int GetInstrumentInfo(const std::string& InstrumentID, 
		PlatformStru_InstrumentInfo& outData) = 0;

	///获取合约的产品类型，失败返回-1
	virtual char GetProductClassType(const std::string& InstrumentID) = 0;

	///获取包含此单腿合约的组合合约列表
	virtual bool GetCombInstrumentIDs_IncludeLeg(const std::string& LegInstrument,
		std::vector<std::string>& vecCombInstruments) = 0;

	///获取合约品种代码
	virtual int GetProductID_InstrumentIDsByExchangeID(std::map<std::string, std::set<std::string> >& outData,
		std::string ExchangeID="") = 0;    

	///获取合约乘数，成功返回合约乘数，失败返回-1
	virtual int GetInstrumentMultiple(const std::string& InstrumentID) = 0;

	//报盘调用，查询合约完毕
	virtual void EndQryInstrument() = 0;

public: //行情操作

	//设置指定合约行情，设置的行情更新，则返回true；否则返回false（比原来的行情更老）
	virtual CF_ERROR SetQuotInfo(PlatformStru_DepthMarketData& inData,bool bSettlementManul = false) = 0;



	///获取指定合约行情
	virtual int GetQuotInfo(const std::string& InstrumentID,
		PlatformStru_DepthMarketData& outData)= 0;

	///获取指定合约旧行情
	virtual int GetOldQuotInfo(const std::string& InstrumentID, 
		PlatformStru_DepthMarketData& outData)= 0;

	///获取指定合约的现价，失败返回无效值
	virtual double GetCurPrice(const std::string& InstrumentID)= 0;

	//获取结算价



/***********************************涉及委托交易账号的操作******************************************************************************/
/*
//费率
//手续费率
//资金
//持仓
//持仓明细
*/
public: //费率操作

	///设置某个账户合约手续费率
	virtual void SetCommissionRate( PlatformStru_InstrumentCommissionRate& outData) = 0;

	///获取某个账户合约手续费率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
	virtual int GetCommissionRate(const string& nsBrokerID,
		const string& nsAccountID,
		const string& InstrumentID,
		PlatformStru_InstrumentCommissionRate& outData) = 0;


	///获取某个用户合约手续费率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
	//因为一个用户绑定一个账号，所以目前可以用这种方式来返回费率
	virtual int GetCommissionRate(const string& nsUserID,	
		const string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) = 0 ;


	//报盘调用，当某一个用户合约的保证金率查询完成后
	virtual void EndUserQryCommission(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;

public: //保证金率

	///设置合约保证金率
	virtual void SetMarginRate( PlatformStru_InstrumentMarginRate& outData) = 0;

	///获取合约保证金率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
	virtual int GetMarginRate(const string& nsBrokerID,
		const string& nsAccountID,
		const string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) = 0;


	///获取合约保证金率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
	virtual int GetMarginRate(const string& nsUserID,	
		const string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) = 0;

    //报盘调用，当某一个用户合约的保证金率查询完成后
	virtual void EndUserQryMargin(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;


public: //资金
	
	//报盘调用，当某一个主帐号资金查询完成
	virtual void QryFundReach(const PlatformStru_TradingAccountInfo& inData) = 0;


public: //持仓

	//查询到某一个主帐号的持仓
	virtual void QryPositionReach( PlatformStru_Position& PositionInfo, int ErrID, bool bIsLast) = 0;


	//报盘调用，当某一个持仓查询完成后
	virtual void EndUserQryPosition(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;

public: //持仓

	//查询到某一个主帐号的持仓
	virtual void QryPositionDetailReach( PlatformStru_PositionDetail& PositionInfo, int ErrID, bool bIsLast) = 0;


	//报盘调用，当某一个持仓明细查询完成后
	virtual void EndUserQryPositionDetail(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;


/***********************************涉及子账号的操作******************************************************************************/
/*
//持仓明细
//持仓
//组合持仓明细
//组合持仓
//成交
//成交汇总
//报单
//预埋单操作
//预埋撤单操作
*/

public://报单操作

	//查询报单到来
	virtual CF_ERROR QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
		const int ErrID,const bool bIsLast) = 0;

	//报盘调用，当某一个账户的报单后
	virtual void EndUserQryOrder(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;

	////添加一个报单
	virtual CF_ERROR PushOrder(PlatformStru_OrderInfo & nOrderInfo,
		bool nbNeedRiseEvent = true) = 0;

	// 报单录入应答
	virtual CF_ERROR PushRspOrderInsert(int nFrontID,
		int nSessionID,
		CThostFtdcInputOrderField *pInputOrder, 
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast)= 0;
	
	///报单录入错误回报
	virtual CF_ERROR PushErrRtnOrderInsert(int nFrontID,
		int nSessionID,
		CThostFtdcInputOrderField *pInputOrder,
		CThostFtdcRspInfoField *pRspInfo)= 0;

	///报单操作请求响应
	virtual CF_ERROR PushRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, 
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast)= 0;
	
	///报单操作错误回报
	virtual CF_ERROR PushErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction,
		CThostFtdcRspInfoField *pRspInfo)= 0;
	


	//
	///获取指定报单
	virtual CF_ERROR GetUserOrderByKey(const std::string & nUserID,
		const OrderKey& nOrderKey,
		PlatformStru_OrderInfo& outData) = 0;

	
	///获取全部报单
	virtual CF_ERROR GetUserOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////获取全部已触发的报单
	virtual CF_ERROR GetUserTriggerOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////获取已成交报单
	virtual CF_ERROR GetUserTradedOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////获取已撤单和错误报单
	virtual CF_ERROR GetUserCanceledOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////获取未成交报单
	virtual CF_ERROR GetUserWaitOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	///获取未知状态的报单
	virtual CF_ERROR GetUserUnkownOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;


	//
	///获取指定报单
	virtual CF_ERROR GetAccountOrderByKey(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const OrderKey& nOrderKey,
		PlatformStru_OrderInfo& outData) = 0;

	
	///获取全部报单
	virtual CF_ERROR GetAccountOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////获取全部已触发的报单
	virtual CF_ERROR GetAccountTriggerOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////获取已成交报单
	virtual CF_ERROR GetAccountTradedOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////获取已撤单和错误报单
	virtual CF_ERROR GetAccountCanceledOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	/////获取未成交报单
	virtual CF_ERROR GetAccountWaitOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;

	///获取未知状态的报单
	virtual CF_ERROR GetAccountUnkownOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) = 0;


public: //成交操作

	//查询的成交回报到来
	virtual CF_ERROR QryTradeReach( PlatformStru_TradeInfo& TradeInfo,
		int ErrID,
		bool bIsLast) = 0;


	//报盘调用，当某一个账户的成交后
	virtual void EndUserQryTrade(const std::string& nsBrokerID,
		const std::string &nAccountID) = 0;

	//新的成交回报到来
	virtual CF_ERROR PushTrade( PlatformStru_TradeInfo& TradeInfo) = 0;

	//获取全部成交单，返回数据项的数量
	virtual CF_ERROR  GetUserTradeInfo(const std::string & nUserID,
		std::vector<PlatformStru_TradeInfo>& outData) = 0;

	//检查CTP成交里面的报单编号是不是我们的报单编号
	virtual bool CheckTradeOrderSysIDIsOwn(const std::string& nsUserID,
		const std::string& nsOrderSysID) = 0;

	//获取全部成交单，返回数据项的数量
	virtual CF_ERROR  GetAccountTradeInfo(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_TradeInfo>& outData) = 0;


	//获取主账户报单成交大于Seq
	virtual CF_ERROR  GetAccountOrderAndTraderGESeq(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo>& nvecTrader) = 0;


	//获取主账户报单成交大于Seq
	virtual CF_ERROR  GetUserOrderAndTraderGESeq(const std::string & nInvestorID,
		int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo>& nvecTrader) = 0;


public://持仓汇总操作

	//获取持仓汇总，
	virtual CF_ERROR  GetUserPositions(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) = 0;


	//获取总持仓量
	virtual CF_ERROR GetUserPositionVolume(const std::string & nUserID,
		const std::string & nInstrumentID,
		int & nVolume) = 0;

	//获取持仓汇总，
	virtual CF_ERROR  GetAccountPositions(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) = 0;


public: //持仓明细汇总

	//获取持仓明细
	//成功返回CF_ERROR_SUCCESS
	virtual CF_ERROR  GetUserPositionDetail(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) = 0;

	//获取持仓明细，返回对应的key的持仓明细
	//成功返回CF_ERROR_SUCCESS
	virtual CF_ERROR  GetUserPositionDetail(const std::string & nUserID,
		const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) = 0;


	//获取持仓明细
	//成功返回CF_ERROR_SUCCESS
	virtual CF_ERROR  GetAccountPositionDetail(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) = 0;

	//获取持仓明细，返回对应的key的持仓明细
	//成功返回CF_ERROR_SUCCESS
	virtual CF_ERROR  GetAccountPositionDetail(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) = 0;


public: //组合持仓汇总操作
	//获取组合持仓，返回数据项的数量
	virtual CF_ERROR  GetUserPositionComb(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData) = 0;

public: //组合持仓明细操作

	//获取组合持仓明细，返回数据项的数量
	virtual CF_ERROR  GetUserPositionCombDetail(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetailComb>& outData) = 0;

public: //资金操作
	//获取交易员资金信息
	virtual CF_ERROR  GetUserFundInfo(const std::string & nUserID,
		PlatformStru_TradingAccountInfo& outData) = 0;

	//获取交易员初始资金
	virtual CF_ERROR  GetUserInitFund(const std::string & nUserID,
		double& outData) = 0;

	//获取委托交易帐号资金信息
	virtual CF_ERROR  GetAccountFundInfo(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		PlatformStru_TradingAccountInfo& outData) = 0;

public: //提供给结算的接口或者获取汇总操作

	//获取所有委托交易账号的手续费率
	virtual CF_ERROR GetAllCommissionRate(
		std::vector<PlatformStru_InstrumentCommissionRate> & outData) = 0;

	//获取所有委托交易账号的保证金率
	virtual CF_ERROR GetAllMarginRate(
		std::vector<PlatformStru_InstrumentMarginRate> & outData) = 0;


	//获取当前交易日的合约
	virtual CF_ERROR  GetCurrentDayAlInstrumentInfo(
		std::vector<PlatformStru_InstrumentInfo> & outData) = 0;


	//获取所有的结算价，只要有一个合约有结算价，就返回正确
	virtual CF_ERROR  GetAllSettlementPrice(
		std::map<std::string,PlatformStru_DepthMarketData> & outData) = 0;



	//获取当前交易日的结算价
	virtual CF_ERROR  GetCurrentDayAllSettlementPrice(
		std::map<std::string,PlatformStru_DepthMarketData> & outData) = 0;

	//获取所有的成交
	virtual CF_ERROR  GetAllTradeInfo(
		std::map<std::string,std::vector<PlatformStru_TradeInfo>> & outData) = 0;

	//获取所有的持仓明细
	virtual CF_ERROR  GetAllPositionDetail(
		std::map<std::string,std::vector<PlatformStru_PositionDetail>> & outData) = 0;

	//获取所有的持仓
	virtual CF_ERROR  GetAllPosition(
		std::map<std::string,std::vector<PlatformStru_Position>> & outData) = 0;

	//获取所有的资金
	virtual CF_ERROR  GetAllFundInfo(
		std::map<std::string,PlatformStru_TradingAccountInfo> & outData) = 0;

	//
	//开始结算，如果是历史结算，需传入日期
	//成功返回：CF_ERROR_SUCCESS
	//失败返回：
	//         
	//
	virtual CF_ERROR  StartSettlement(eSettlementLoadOption nOption,
		const std::string & nsDay ,
		bool nbHistorySettlement) = 0;


	//结束结算
	virtual CF_ERROR  EndSettlement(const std::string & nsDay) = 0;

	//设置修改后的手续费率和保证金率，并触发成交和持仓和资金的计算
	virtual CF_ERROR  SetModifiedCommissionAndMargin( std::vector<PlatformStru_InstrumentCommissionRate> & inCommssion,
		 std::vector<PlatformStru_InstrumentMarginRate> & inMargin) = 0;

	//设置修改后的成交，并触发成交和持仓和资金的计算
	virtual CF_ERROR  SeUserNewTrades(const std::string & nUserName,
		const std::vector<PlatformStru_TradeInfo> & inTradeInfo) = 0;
	

public:
	//下单判断该用户该合约的手续费率和保证金率是否准备好了
	virtual CF_ERROR CheckUserInstrumentCommissionAndMaginExist(const std::string & nUserName,
		const std::string & nIstrumentID) = 0;
	
	//判断是否可开仓
	virtual CF_ERROR  CanOrderOpen(const PlatformStru_OrderInfo & nOrder) = 0;

	//判断是否可平今
	virtual CF_ERROR  CanOrderColseToday(const PlatformStru_OrderInfo & nOrder) = 0;

	//判断是否可平仓
	virtual CF_ERROR  CanOrderColse(const PlatformStru_OrderInfo & nOrder) = 0;

public:
	//出入金
	virtual CF_ERROR FundInOut(const sFundInOut& nFundInOut) = 0;

	//查询历史出入金
	virtual CF_ERROR QryHistroyFundInOut(const sQryFundInOut & nQryFund,
		std::vector<sFundInOut> & nvecFund) = 0;


public:
	////添加一个CTP报单
	virtual CF_ERROR QryCTPOrder(PlatformStru_OrderInfo & nOrderInfo,const int ErrID,const bool bIsLast) = 0 ;

	//添加一个CTP成交
	virtual CF_ERROR QryCTPTrader(PlatformStru_TradeInfo & nTraderInfo,const int ErrID,const bool bIsLast) = 0;

	////添加一个CTP报单
	virtual CF_ERROR PushCTPOrder(PlatformStru_OrderInfo & nOrderInfo) = 0;

	//添加一个CTP成交
	virtual CF_ERROR PushCTPTrader(PlatformStru_TradeInfo & nTraderInfo) = 0;



	////添加客户端下单
	virtual CF_ERROR PushExecuteInOrder(PlatformStru_OrderInfo & nOrderInfo) = 0;

	//添加一个客户端下单的返回结果
	virtual CF_ERROR PushExecuteOutOrder(PlatformStru_OrderInfo & nOraderInfo) = 0;

public:
	//获取操作数据库的接口
	virtual IInterfaceDBOpr * GetInterfaceDBOpr() = 0;

	//查看用户是否确认了结算单
	virtual bool IsUserSettlementConfirmed(const std::string & nUserName,
		std::string & nsConfirmedDate,
		std::string & nsConfirmedTime) = 0;

	//用户确认了结算单
	virtual void SetUserSettlementConfirmed(const std::string & nsUserName,
		const std::string & nsConfirmedDate,
		const std::string & nsConfirmedTime) = 0 ;

public:
	//获取初始化时间
	virtual CF_ERROR GetReinitTime(sReinitTime & nsReinitTime) = 0;

	//获取数据保存时间
	virtual CF_ERROR GetSaveTime(sSaveTime & nsReinitTime) = 0 ;

	//设置初始化时间
	virtual CF_ERROR SetReinitTime(const sReinitTime & nsReinitTime) = 0;

	//获取交易时间
	virtual CF_ERROR GetTradingTime(sTradingTime & nsTradingTime) = 0 ;


	//设置交易时间
	virtual CF_ERROR SetTradingTime(const sTradingTime & nsTradingTime) = 0;

public: //辅助功能
	virtual void GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,
		PlatformStru_OrderInfo & orderInfo) = 0;

	virtual void GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,
		PlatformStru_InputOrder & inputOrder) = 0;

public:
	//报盘查询不成功调用
	virtual void QryFailed(const BrokerAccountKey & nBrokerAccount,eEventType nEvent) = 0;

	//获取对应用户对应时间段的持仓明细
	virtual CF_ERROR GetPositionDetail_MaxTime(const  std::vector<std::string> nvecAccount, 
		int LowerOpenTime,int UpperOpenTime,
		std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail) = 0;



public:
	virtual bool PushStrategyStopOrRun(const std::string & nsName,bool nsStop) = 0;

	//virtual	bool PushTraderUseroffLine(const std::string & nUserName,bool bOffline) = 0;

	virtual bool PushStrategyInstanceStopOrRun(const std::string & nsUserName,
		const std::string & nsStategyName,bool nsStop) = 0;

// 	virtual bool GetStategyUserInstanceUpdateTime(const std::string & nsUserName,
// 		const std::string & nsStategyName,
// 		time_t & nsUpdateTime) = 0;

    virtual bool PushStrategyTimer(const std::string & nsName,bool nsStop) = 0 ;

    virtual bool ConnectRedisServer() = 0;
    virtual bool DisConnectRedisServer() = 0;
};







