
#pragma once

//#include "Module-Misc2\KernelStruct.h"

//仿CTP的Api函数命令字
enum {
	FFT_API_START = 0x18000000, 

	///用户登录请求
	FTTCMD_ReqUserLogin,			//请求命令字
	FTTCMD_ReqUserLogin_Rsp,		//响应命令字

	///登出请求
	FTTCMD_ReqUserLogout, 
	FTTCMD_ReqUserLogout_Rsp, 

	///用户口令更新请求
	FTTCMD_ReqUserPasswordUpdate, 
	FTTCMD_ReqUserPasswordUpdate_Rsp,

	///资金账户口令更新请求
	FTTCMD_ReqTradingAccountPasswordUpdate,
	FTTCMD_ReqTradingAccountPasswordUpdate_Rsp,

	///报单录入请求
	FTTCMD_ReqOrderInsert, 
	FTTCMD_ReqOrderInsert_Rsp, 

	///预埋单录入请求
	FTTCMD_ReqParkedOrderInsert, 
	FTTCMD_ReqParkedOrderInsert_Rsp, 

	///预埋撤单录入请求
	FTTCMD_ReqParkedOrderAction, 
	FTTCMD_ReqParkedOrderAction_Rsp, 

	///报单操作请求
	FTTCMD_ReqOrderAction,
	FTTCMD_ReqOrderAction_Rsp,

	///查询最大报单数量请求
	FTTCMD_ReqQueryMaxOrderVolume,
	FTTCMD_ReqQueryMaxOrderVolume_Rsp,

	///投资者结算结果确认
	FTTCMD_ReqSettlementInfoConfirm,
	FTTCMD_ReqSettlementInfoConfirm_Rsp,

	///请求删除预埋单
	FTTCMD_ReqRemoveParkedOrder,
	FTTCMD_ReqRemoveParkedOrder_Rsp,

	///请求删除预埋撤单
	FTTCMD_ReqRemoveParkedOrderAction,
	FTTCMD_ReqRemoveParkedOrderAction_Rsp,

	///询价录入请求
	FTTCMD_ReqForQuoteInsert,
	FTTCMD_ReqForQuoteInsert_Rsp,

	///执行宣告录入请求
	FTTCMD_ReqExecOrderInsert,
	FTTCMD_ReqExecOrderInsert_Rsp,

	///执行宣告操作请求
	FTTCMD_ReqExecOrderAction,
	FTTCMD_ReqExecOrderAction_Rsp,

	///请求查询报单
	FTTCMD_ReqQryOrder,
	FTTCMD_ReqQryOrder_Rsp,

	///请求查询成交
	FTTCMD_ReqQryTrade,
	FTTCMD_ReqQryTrade_Rsp,

	///请求查询投资者持仓
	FTTCMD_ReqQryInvestorPosition,
	FTTCMD_ReqQryInvestorPosition_Rsp,

	///请求查询资金账户
	FTTCMD_ReqQryTradingAccount,
	FTTCMD_ReqQryTradingAccount_Rsp,

	///请求查询投资者
	FTTCMD_ReqQryInvestor,
	FTTCMD_ReqQryInvestor_Rsp,

	///请求查询合约保证金率
	FTTCMD_ReqQryInstrumentMarginRate,
	FTTCMD_ReqQryInstrumentMarginRate_Rsp,

	///请求查询合约手续费率
	FTTCMD_ReqQryInstrumentCommissionRate,
	FTTCMD_ReqQryInstrumentCommissionRate_Rsp,

	///请求查询交易所
	FTTCMD_ReqQryExchange,
	FTTCMD_ReqQryExchange_Rsp,

	///请求查询产品
	FTTCMD_ReqQryProduct,
	FTTCMD_ReqQryProduct_Rsp,

	///请求查询合约
	FTTCMD_ReqQryInstrument,
	FTTCMD_ReqQryInstrument_Rsp,

	///请求查询投资者结算结果
	FTTCMD_ReqQrySettlementInfo,
	FTTCMD_ReqQrySettlementInfo_Rsp,

	///请求查询投资者持仓明细
	FTTCMD_ReqQryInvestorPositionDetail,
	FTTCMD_ReqQryInvestorPositionDetail_Rsp,

	///请求查询客户通知
	FTTCMD_ReqQryNotice,
	FTTCMD_ReqQryNotice_Rsp,

	///请求查询结算信息确认
	FTTCMD_ReqQrySettlementInfoConfirm,
	FTTCMD_ReqQrySettlementInfoConfirm_Rsp,

	///请求查询投资者持仓明细
	FTTCMD_ReqQryInvestorPositionCombineDetail,
	FTTCMD_ReqQryInvestorPositionCombineDetail_Rsp,

	///请求查询保证金监管系统经纪公司资金账户密钥
	FTTCMD_ReqQryCFMMCTradingAccountKey,
	FTTCMD_ReqQryCFMMCTradingAccountKey_Rsp,

	///请求查询期权交易成本
	FTTCMD_ReqQryOptionInstrTradeCost,
	FTTCMD_ReqQryOptionInstrTradeCost_Rsp,

	///请求查询期权合约手续费
	FTTCMD_ReqQryOptionInstrCommRate,
	FTTCMD_ReqQryOptionInstrCommRate_Rsp,

	///请求查询执行宣告
	FTTCMD_ReqQryExecOrder,
	FTTCMD_ReqQryExecOrder_Rsp,

	///请求查询转帐流水
	FTTCMD_ReqQryTransferSerial,
	FTTCMD_ReqQryTransferSerial_Rsp,

	///请求查询银期签约关系
	FTTCMD_ReqQryAccountregister,
	FTTCMD_ReqQryAccountregister_Rsp,

	///请求查询签约银行
	FTTCMD_ReqQryContractBank,
	FTTCMD_ReqQryContractBank_Rsp,

	///请求查询预埋单
	FTTCMD_ReqQryParkedOrder,
	FTTCMD_ReqQryParkedOrder_Rsp,

	///请求查询预埋撤单
	FTTCMD_ReqQryParkedOrderAction,
	FTTCMD_ReqQryParkedOrderAction_Rsp,

	///请求查询交易通知
	FTTCMD_ReqQryTradingNotice,
	FTTCMD_ReqQryTradingNotice_Rsp,

	///请求查询经纪公司交易参数
	FTTCMD_ReqQryBrokerTradingParams,
	FTTCMD_ReqQryBrokerTradingParams_Rsp,

	///期货发起银行资金转期货请求
	FTTCMD_ReqFromBankToFutureByFuture,
	FTTCMD_ReqFromBankToFutureByFuture_Rsp,

	///期货发起期货资金转银行请求
	FTTCMD_ReqFromFutureToBankByFuture,
	FTTCMD_ReqFromFutureToBankByFuture_Rsp,

	///期货发起查询银行余额请求
	FTTCMD_ReqQueryBankAccountMoneyByFuture,
	FTTCMD_ReqQueryBankAccountMoneyByFuture_Rsp,
};

//仿CTP的Spi函数命令字
enum {
	FFT_SPI_START = 0x18010000, 

	// 交易服务器连接成功响应
	FTTCMD_OnFrontConnected_RON,

	// 交易服务器连接断开响应
	FTTCMD_OnFrontDisconnected_RON,

	///登录请求响应
	FTTCMD_OnRspUserLogin_RON,

	///登出请求响应
	FTTCMD_OnRspUserLogout_RON,

	///用户口令更新请求响应
	FTTCMD_OnRspUserPasswordUpdate_RON,

	///资金账户口令更新请求响应
	FTTCMD_OnRspTradingAccountPasswordUpdate_RON,

	///报单录入请求响应
	FTTCMD_OnRspOrderInsert_RON,

	///预埋单录入请求响应
	FTTCMD_OnRspParkedOrderInsert_RON,

	///预埋撤单录入请求响应
	FTTCMD_OnRspParkedOrderAction_RON,

	///报单操作请求响应
	FTTCMD_OnRspOrderAction_RON,

	///查询最大报单数量响应
	FTTCMD_OnRspQueryMaxOrderVolume_RON,

	///投资者结算结果确认响应
	FTTCMD_OnRspSettlementInfoConfirm_RON,

	///删除预埋单响应
	FTTCMD_OnRspRemoveParkedOrder_RON,

	///删除预埋撤单响应
	FTTCMD_OnRspRemoveParkedOrderAction_RON,

	///执行宣告录入请求响应
	FTTCMD_OnRspExecOrderInsert_RON,

	///执行宣告操作请求响应
	FTTCMD_OnRspExecOrderAction_RON,

	///请求查询报单响应
	FTTCMD_OnRspQryOrder_RON,

	///请求查询成交响应
	FTTCMD_OnRspQryTrade_RON,

	///请求查询投资者持仓响应
	FTTCMD_OnRspQryInvestorPosition_RON,

	///请求查询资金账户响应
	FTTCMD_OnRspQryTradingAccount_RON,

	///请求查询投资者响应
	FTTCMD_OnRspQryInvestor_RON,

	///请求查询合约保证金率响应
	FTTCMD_OnRspQryInstrumentMarginRate_RON,

	///请求查询合约手续费率响应
	FTTCMD_OnRspQryInstrumentCommissionRate_RON,

	///请求查询交易所响应
	FTTCMD_OnRspQryExchange_RON,

	///请求查询产品响应
	FTTCMD_OnRspQryProduct_RON,

	///请求查询合约响应
	FTTCMD_OnRspQryInstrument_RON,

	///请求查询投资者结算结果响应
	FTTCMD_OnRspQrySettlementInfo_RON,

	///请求查询投资者持仓明细响应
	FTTCMD_OnRspQryInvestorPositionDetail_RON,

	///请求查询客户通知响应
	FTTCMD_OnRspQryNotice_RON,

	///请求查询结算信息确认响应
	FTTCMD_OnRspQrySettlementInfoConfirm_RON,

	///请求查询投资者持仓明细响应
	FTTCMD_OnRspQryInvestorPositionCombineDetail_RON,

	///查询保证金监管系统经纪公司资金账户密钥响应
	FTTCMD_OnRspQryCFMMCTradingAccountKey_RON,

	///请求查询期权交易成本响应
	FTTCMD_OnRspQryOptionInstrTradeCost_RON,

	///请求查询期权合约手续费响应
	FTTCMD_OnRspQryOptionInstrCommRate_RON,

	///请求查询执行宣告响应
	FTTCMD_OnRspQryExecOrder_RON,

	///请求查询转帐流水响应
	FTTCMD_OnRspQryTransferSerial_RON,

	///请求查询银期签约关系响应
	FTTCMD_OnRspQryAccountregister_RON,

	///错误应答
	FTTCMD_OnRspError_RON,

	///报单通知
	FTTCMD_OnRtnOrder_RON,

	///成交通知
	FTTCMD_OnRtnTrade_RON,

	///报单录入错误回报
	FTTCMD_OnErrRtnOrderInsert_RON,

	///报单操作错误回报
	FTTCMD_OnErrRtnOrderAction_RON,

	///合约交易状态通知
	FTTCMD_OnRtnInstrumentStatus_RON,

	///交易通知
	FTTCMD_OnRtnTradingNotice_RON,

	///执行宣告通知
	FTTCMD_OnRtnExecOrder_RON,

	///执行宣告录入错误回报
	FTTCMD_OnErrRtnExecOrderInsert_RON,

	///执行宣告操作错误回报
	FTTCMD_OnErrRtnExecOrderAction_RON,

	///询价录入请求响应
	FTTCMD_OnRspForQuoteInsert_RON,

	///询价录入错误回报
	FTTCMD_OnErrRtnForQuoteInsert_RON,

	///请求查询签约银行响应
	FTTCMD_OnRspQryContractBank_RON,

	///请求查询预埋单响应
	FTTCMD_OnRspQryParkedOrder_RON,

	///请求查询预埋撤单响应
	FTTCMD_OnRspQryParkedOrderAction_RON,

	///请求查询交易通知响应
	FTTCMD_OnRspQryTradingNotice_RON,

	///请求查询经纪公司交易参数响应
	FTTCMD_OnRspQryBrokerTradingParams_RON,

	///期货发起银行资金转期货通知
	FTTCMD_OnRtnFromBankToFutureByFuture_RON,

	///期货发起期货资金转银行通知
	FTTCMD_OnRtnFromFutureToBankByFuture_RON,

	///期货发起查询银行余额通知
	FTTCMD_OnRtnQueryBankBalanceByFuture_RON,

	///期货发起银行资金转期货应答
	FTTCMD_OnRspFromBankToFutureByFuture_RON,

	///期货发起期货资金转银行应答
	FTTCMD_OnRspFromFutureToBankByFuture_RON,

	///期货发起查询银行余额应答
	FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON,

};
