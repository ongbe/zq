#pragma once

namespace Client {

	enum {
		conGridNone,					// 0, 被保留
		conGridRealTimeQueryOrder,		// 实时查询报单grid
		conGridRealTimeQueryTrade,		// 实时查询成交grid 
		conGridRealTimeQueryPosition, 	// 实时查询持仓grid
		conGridRealTimeQueryFund		// 实时查询帐号资金grid
	};

	// 实时查询报单grid列定义
	enum {
		// 0, 被保留
		conCol_RTQO_None,
		///投资者代码
		conCol_RTQO_InvestorID,
		// 账号所属资管
		conCol_RTQO_AssertMgmtOrgan,
		// 账号所属理财产品
		conCol_RTQO_FinanProduct,
		/////流中唯一的序列号
		//conCol_RTQO_UniqSequenceNo,
		///经纪公司代码
		conCol_RTQO_BrokerID,
		///合约代码
		conCol_RTQO_InstrumentID,
		///报单引用
		conCol_RTQO_OrderRef,
		///用户代码
		conCol_RTQO_UserID,
		///报单价格条件
		conCol_RTQO_OrderPriceType,
		///买卖方向
		conCol_RTQO_Direction,
		///组合开平标志
		conCol_RTQO_CombOffsetFlag,
		///组合投机套保标志
		conCol_RTQO_CombHedgeFlag,
		///价格
		conCol_RTQO_LimitPrice,
		///数量
		conCol_RTQO_VolumeTotalOriginal,
		///有效期类型
		conCol_RTQO_TimeCondition,
		///GTD日期
		conCol_RTQO_GTDDate,
		///成交量类型
		conCol_RTQO_VolumeCondition,
		///最小成交量
		conCol_RTQO_MinVolume,
		///触发条件
		conCol_RTQO_ContingentCondition,
		///止损价
		conCol_RTQO_StopPrice,
		///强平原因
		conCol_RTQO_ForceCloseReason,
		///自动挂起标志
		conCol_RTQO_IsAutoSuspend,
		///业务单元
		conCol_RTQO_BusinessUnit,
		///请求编号
		conCol_RTQO_RequestID,
		///本地报单编号
		conCol_RTQO_OrderLocalID,
		///交易所代码
		conCol_RTQO_ExchangeID,
		///会员代码
		conCol_RTQO_ParticipantID,
		///客户代码
		conCol_RTQO_ClientID,
		///合约在交易所的代码
		conCol_RTQO_ExchangeInstID,
		///交易所交易员代码
		conCol_RTQO_TraderID,
		///安装编号
		conCol_RTQO_InstallID,
		///报单提交状态
		conCol_RTQO_OrderSubmitStatus,
		///报单提示序号
		conCol_RTQO_NotifySequence,
		///交易日
		conCol_RTQO_TradingDay,
		///结算编号
		conCol_RTQO_SettlementID,
		///报单编号
		conCol_RTQO_OrderSysID,
		///报单来源
		conCol_RTQO_OrderSource,
		///报单状态
		conCol_RTQO_OrderStatus,
		///报单类型
		conCol_RTQO_OrderType,
		///今成交数量
		conCol_RTQO_VolumeTraded,
		///剩余数量
		conCol_RTQO_VolumeTotal,
		///报单日期
		conCol_RTQO_InsertDate,
		///委托时间
		conCol_RTQO_InsertTime,
		///激活时间
		conCol_RTQO_ActiveTime,
		///挂起时间
		conCol_RTQO_SuspendTime,
		///最后修改时间
		conCol_RTQO_UpdateTime,
		///撤销时间
		conCol_RTQO_CancelTime,
		///最后修改交易所交易员代码
		conCol_RTQO_ActiveTraderID,
		///结算会员编号
		conCol_RTQO_ClearingPartID,
		///序号
		conCol_RTQO_SequenceNo,
		///前置编号
		conCol_RTQO_FrontID,
		///会话编号
		conCol_RTQO_SessionID,
		///用户端产品信息
		conCol_RTQO_UserProductInfo,
		///状态信息
		conCol_RTQO_StatusMsg,
		///用户强评标志
		conCol_RTQO_UserForceClose,
		///操作用户代码
		conCol_RTQO_ActiveUserID,
		///经纪公司报单编号
		conCol_RTQO_BrokerOrderSeq,
		///相关报单
		conCol_RTQO_RelativeOrderSysID,
		///郑商所成交数量
		conCol_RTQO_ZCETotalTradedVolume,
	};

	// 实时查询成交grid列定义 
	enum {
		// 0, 被保留
		conCol_RTQT_None,
		///投资者代码
		conCol_RTQT_InvestorID,
		// 账号所属资管
		conCol_RTQT_AssertMgmtOrgan,
		// 账号所属理财产品
		conCol_RTQT_FinanProduct,
		/////流中唯一的序列号
		//conCol_RTQT_UniqSequenceNo,
		///经纪公司代码
		conCol_RTQT_BrokerID,
		///合约代码
		conCol_RTQT_InstrumentID,
		///报单引用
		conCol_RTQT_OrderRef,
		///用户代码
		conCol_RTQT_UserID,
		///交易所代码
		conCol_RTQT_ExchangeID,
		///成交编号
		conCol_RTQT_TradeID,
		///买卖方向
		conCol_RTQT_Direction,
		///报单编号
		conCol_RTQT_OrderSysID,
		///会员代码
		conCol_RTQT_ParticipantID,
		///客户代码
		conCol_RTQT_ClientID,
		///交易角色
		conCol_RTQT_TradingRole,
		///合约在交易所的代码
		conCol_RTQT_ExchangeInstID,
		///开平标志
		conCol_RTQT_OffsetFlag,
		///投机套保标志
		conCol_RTQT_HedgeFlag,
		///价格
		conCol_RTQT_Price,
		///数量
		conCol_RTQT_Volume,
		///成交时期
		conCol_RTQT_TradeDate,
		///成交时间
		conCol_RTQT_TradeTime,
		///成交类型
		conCol_RTQT_TradeType,
		///成交价来源
		conCol_RTQT_PriceSource,
		///交易所交易员代码
		conCol_RTQT_TraderID,
		///本地报单编号
		conCol_RTQT_OrderLocalID,
		///结算会员编号
		conCol_RTQT_ClearingPartID,
		///业务单元
		conCol_RTQT_BusinessUnit,
		///序号
		conCol_RTQT_SequenceNo,
		///交易日
		conCol_RTQT_TradingDay,
		///结算编号
		conCol_RTQT_SettlementID,
		///经纪公司报单编号
		conCol_RTQT_BrokerOrderSeq,
		///成交来源
		conCol_RTQT_TradeSource,
	};

	// 实时查询持仓grid列定义
	enum {
		// 0, 被保留
		conCol_RTQP_None,
		///投资者代码
		conCol_RTQP_InvestorID,
		// 账号所属资管
		conCol_RTQP_AssertMgmtOrgan,
		// 账号所属理财产品
		conCol_RTQP_FinanProduct,
		/////流中唯一的序列号
		//conCol_RTQP_UniqSequenceNo,
		///经纪公司代码
		conCol_RTQP_BrokerID,
		///合约代码
		conCol_RTQP_InstrumentID,
		///持仓多空方向
		conCol_RTQP_PosiDirection,
		///投机套保标志
		conCol_RTQP_HedgeFlag,
		///持仓日期
		conCol_RTQP_PositionDate,
		///上日持仓
		conCol_RTQP_YdPosition,
		///今日持仓
		conCol_RTQP_Position,
		///多头冻结
		conCol_RTQP_LongFrozen,
		///空头冻结
		conCol_RTQP_ShortFrozen,
		///开仓冻结金额
		conCol_RTQP_LongFrozenAmount,
		///开仓冻结金额
		conCol_RTQP_ShortFrozenAmount,
		///开仓量
		conCol_RTQP_OpenVolume,
		///平仓量
		conCol_RTQP_CloseVolume,
		///开仓金额
		conCol_RTQP_OpenAmount,
		///平仓金额
		conCol_RTQP_CloseAmount,
		///持仓成本
		conCol_RTQP_PositionCost,
		///上次占用的保证金
		conCol_RTQP_PreMargin,
		///占用的保证金
		conCol_RTQP_UseMargin,
		///冻结的保证金
		conCol_RTQP_FrozenMargin,
		///冻结的资金
		conCol_RTQP_FrozenCash,
		///冻结的手续费
		conCol_RTQP_FrozenCommission,
		///资金差额
		conCol_RTQP_CashIn,
		///手续费
		conCol_RTQP_Commission,
		///平仓盈亏
		conCol_RTQP_CloseProfit,
		///持仓盈亏
		conCol_RTQP_PositionProfit,
		///上次结算价
		conCol_RTQP_PreSettlementPrice,
		///本次结算价
		conCol_RTQP_SettlementPrice,
		///交易日
		conCol_RTQP_TradingDay,
		///结算编号
		conCol_RTQP_SettlementID,
		///开仓成本
		conCol_RTQP_OpenCost,
		///交易所保证金
		conCol_RTQP_ExchangeMargin,
		///组合成交形成的持仓
		conCol_RTQP_CombPosition,
		///组合多头冻结
		conCol_RTQP_CombLongFrozen,
		///组合空头冻结
		conCol_RTQP_CombShortFrozen,
		///逐日盯市平仓盈亏
		conCol_RTQP_CloseProfitByDate,
		///逐笔对冲平仓盈亏
		conCol_RTQP_CloseProfitByTrade,
		///今日持仓
		conCol_RTQP_TodayPosition,
		///保证金率
		conCol_RTQP_MarginRateByMoney,
		///保证金率(按手数)
		conCol_RTQP_MarginRateByVolume,
	};

	// 实时查询帐号资金grid列定义
	enum {
		// 0, 被保留
		conCol_RTQA_None,
		///投资者代码
		conCol_RTQA_InvestorID,
		// 账号所属资管
		conCol_RTQA_AssertMgmtOrgan,
		// 账号所属理财产品
		conCol_RTQA_FinanProduct,
		///经纪公司代码
		conCol_RTQA_BrokerID,
		///出入金金额
		conCol_RTQA_Withdraw,
		///当前保证金总额
		conCol_RTQA_CurrMargin,
		///手续费
		conCol_RTQA_Commission,
		///平仓盈亏
		conCol_RTQA_CloseProfit,
		///持仓盈亏
		conCol_RTQA_PositionProfit,
		///总权益
		conCol_RTQA_Balance,
		///可用资金
		conCol_RTQA_Available,
		///质押金额
		conCol_RTQA_Mortgage,
		///交易所保证金
		conCol_RTQA_ExchangeMargin,
		///可取资金
		conCol_RTQA_WithdrawQuota,
		///信用额度
		conCol_RTQA_Credit,
		///保证金冻结
		conCol_RTQA_FrozenMargin,
		///手续费冻结
		conCol_RTQA_FrozenCommission,

		// 以下是昨日资金帐号相关的数据

		/////流中唯一的序列号
		//conCol_RTQA_UniqSequenceNo,
		/////经纪公司代码
		//conCol_RTQA_BrokerID,
		///投资者帐号
		conCol_RTQA_AccountID,
		///上次质押金额
		conCol_RTQA_PreMortgage,
		///上次信用额度
		conCol_RTQA_PreCredit,
		///上次存款额
		conCol_RTQA_PreDeposit,
		///上次总权益
		conCol_RTQA_PreBalance,
		///上次占用的保证金
		conCol_RTQA_PreMargin,
		///基本准备金
		conCol_RTQA_Reserve,
		///上次交易所保证金
		conCol_RTQA_PreExchMargin,
		///历史强平次数
		conCol_RTQA_ForceCloseStat,
		///投资者交割保证金
		conCol_RTQA_DeliveryMargin,
		///交易所交割保证金
		conCol_RTQA_ExchangeDeliveryMargin,
		//基金净值
		conCol_RTQA_NETVALUE,
	};

}
