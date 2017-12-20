
#pragma once
#include "interface_dbOpr.h"

class CRealDBOpr :public IInterfaceDBOpr
{
public:	

	//创建表或者分区
	//如果表不存在则建表和今日的分区
	//如果表存在，并且今日的分区不存在，则追加今日的分区
	virtual CF_ERROR CreateMyTableAndPartitions(const std::string & strDate);

	//装置某个日期的合约
	virtual CF_ERROR LoadTodayInstrument(const std::string & strTime, 
		std::vector<PlatformStru_InstrumentInfo> & nInfo) ;

	//写合约
	virtual CF_ERROR WriteIntrument(const std::string & strTime, 
		const std::vector<PlatformStru_InstrumentInfo> & nInfo) ;

	//装置账户的费率
	virtual CF_ERROR LoadTodayCommission(const std::string & nsBrokerID,
		const std::string & nsAccountID,
		const std::string & strTime, 
		std::vector<PlatformStru_InstrumentCommissionRate> & nInfo) ;

	//写费率
	virtual CF_ERROR WriteCommission(const std::string & nsBrokerID,
		const std::string & nsAccountID,
		const std::string & strTime, 
		const std::vector<PlatformStru_InstrumentCommissionRate> & nInfo) ;


	//转载账户的保证金
	virtual CF_ERROR LoadTodayMarginRate(const std::string & nsBrokerID,
		const std::string & nsAccountID,
		const std::string & strTime,
		std::vector<PlatformStru_InstrumentMarginRate> & nInfo) ;

	//写保证金
	virtual CF_ERROR WriteMarginRate(const std::string & nsBrokerID,
		const std::string & nsAccountID,
		const std::string & strTime, 
		const std::vector<PlatformStru_InstrumentMarginRate> & nInfo) ;

	//写报单
	virtual CF_ERROR WriteOrderInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const std::vector<PlatformStru_OrderInfo> & nInfo,
		bool nbDelete = true) ;

	//写成交
	virtual CF_ERROR WriteTradeInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const std::vector<PlatformStru_TradeInfo> & nInfo,
		bool nbDelete = true) ;

	//写持仓
	virtual CF_ERROR WritePositionInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const std::vector<PlatformStru_Position> & nInfo,
		bool nbDelete = true) ;

	//写持仓明细
	virtual CF_ERROR WritePositionDetailInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const std::vector<PlatformStru_PositionDetail> & nInfo,
		bool nbDelete = true) ;

	//写资金
	virtual CF_ERROR WriteFundInfos(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		const PlatformStru_TradingAccountInfo & nInfo) ;


	//写出入金
	virtual CF_ERROR WriteFundInOut(const sFundInOut & s) ;

	//写初始资金
	virtual CF_ERROR WriteInitFund(const std::string & nsTableName,
		const std::string & nsUserName,
		const double & ndbVal);

	//写结算行情
	virtual CF_ERROR WriteQutoInfo(const std::string & nsTableName,
		const std::string & strTime,
		const std::vector<PlatformStru_DepthMarketData> & nInfo) ;


	//装载结算行情
	virtual CF_ERROR LoadQutoInfo(const std::string & nsTableName,
		const std::string & strTime,
		std::vector<PlatformStru_DepthMarketData> & nInfo) ;

	//追加一条结算行情
	virtual CF_ERROR AddQutoInfo(const std::string & nsTableName,
		const std::string & strTime,
		const PlatformStru_DepthMarketData & nInfo) ;


	//装载用户的报单
	virtual CF_ERROR LoadTodayUserOrders(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		std::vector<PlatformStru_OrderInfo> & nInfo) ;

	//装载用户的成交
	virtual CF_ERROR LoadTodayUserTrades(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		std::vector<PlatformStru_TradeInfo> & nInfo) ;

	//装载用户的持仓
	virtual CF_ERROR LoadTodayUserPositions(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		std::vector<PlatformStru_Position> & nInfo) ;

	//装载用户的持仓明细
	virtual CF_ERROR LoadTodayUserPositionDetails(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		std::vector<PlatformStru_PositionDetail> & nInfo) ;

	//装载用户的资金
	virtual CF_ERROR LoadTodayUserFundInfo(const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime, 
		PlatformStru_TradingAccountInfo & nInfo) ;


	//查询出入金
	virtual CF_ERROR LoadQryFundInOut(const sQryFundInOut & nQryFund,
		std::vector<sFundInOut> & nvecFund);


	//查询初始总权益
	virtual CF_ERROR LoadUserInitFund(const std::string & nsTableName,
		const std::string & nsUserName,
		double  & ndbVal);


	virtual CF_ERROR LoadUserSettlementConfirm(const std::string & strValidateTime,
		const std::string & nsUserName,
		std::string & nsConfirmDate,
		std::string & nsConfirmTime) ;

	virtual CF_ERROR AddUserSettlementConfirm(const std::string & strValidateTime,
		const std::string & nsUserName,
		const std::string & nsConfirmDate,
		const std::string & nsConfirmTime) ;


	//装载用户的报单
	virtual CF_ERROR LoadTodayAccountOrders(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		std::vector<PlatformStru_OrderInfo> & nInfo) ;

	//装载用户的成交
	virtual CF_ERROR LoadTodayAccountTrades(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		std::vector<PlatformStru_TradeInfo> & nInfo) ;

	//装载用户的持仓
	virtual CF_ERROR LoadTodayAccountPositions(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		std::vector<PlatformStru_Position> & nInfo) ;

	//装载用户的持仓明细
	virtual CF_ERROR LoadTodayAccountPositionDetails(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		std::vector<PlatformStru_PositionDetail> & nInfo) ;

	//装载用户的资金
	virtual CF_ERROR LoadTodayAccountFundInfo(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime, 
		PlatformStru_TradingAccountInfo & nInfo) ;



	//写报单
	virtual CF_ERROR WriteAccountOrderInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const std::vector<PlatformStru_OrderInfo> & nInfo,
		bool nbDelete = true) ;

	//写成交
	virtual CF_ERROR WriteAccountTradeInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const std::vector<PlatformStru_TradeInfo> & nInfo,
		bool nbDelete = true) ;

	//写持仓
	virtual CF_ERROR WriteAccountPositionInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const std::vector<PlatformStru_Position> & nInfo,
		bool nbDelete = true) ;

	//写持仓明细
	virtual CF_ERROR WriteAccountPositionDetailInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const std::vector<PlatformStru_PositionDetail> & nInfo,
		bool nbDelete = true) ;

	//写资金
	virtual CF_ERROR WriteAccountFundInfos(const std::string & nsTableName,
		const std::string & nsBrokerID,
		const std::string & nsInvestorID,
		const std::string & strTime,
		const PlatformStru_TradingAccountInfo & nInfo) ;
};
