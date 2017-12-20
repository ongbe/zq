#pragma once

namespace CltServerMgr {

	enum {
		conGridNone,					// 0, 被保留
		conGridQueryHistoryOrder,		// 实时查询报单grid
		conGridQueryHistoryTrade,		// 实时查询成交grid 
		conGridQueryHistoryPositionDetail, 	// 实时查询持仓明细grid
		conGridQueryHistoryPosition, 	// 实时查询持仓grid
		conGridQueryHistoryFund,		// 实时查询帐号资金grid
		conGridQueryHistoryFundInOut,	// 实时查询帐号资金进出grid
		conGridQueryUserSendMsg,		// 查询用户发送消息grid
		conGridQueryUserRecvMsg,		// 查询用户接收消息grid 
	};

	// 查询历史报单grid列定义
	enum {
		// 0, 被保留
		conCol_HQO_None,
		///交易日
		conCol_HQO_TradingDay,
		///投资者代码
		conCol_HQO_InvestorID,
		///合约代码
		conCol_HQO_InstrumentID,
		///报单引用
		conCol_HQO_OrderRef,
		///报单价格条件
		conCol_HQO_OrderPriceType,
		///买卖方向
		conCol_HQO_Direction,
		///组合开平标志
		conCol_HQO_CombOffsetFlag,
		///组合投机套保标志
		conCol_HQO_CombHedgeFlag,
		///价格
		conCol_HQO_LimitPrice,
		///数量
		conCol_HQO_VolumeTotalOriginal,
		///有效期类型
		conCol_HQO_TimeCondition,
		///GTD日期
		conCol_HQO_GTDDate,
		///成交量类型
		conCol_HQO_VolumeCondition,
		///最小成交量
		conCol_HQO_MinVolume,
		///触发条件
		conCol_HQO_ContingentCondition,
		///止损价
		conCol_HQO_StopPrice,
		///强平原因
		conCol_HQO_ForceCloseReason,
		///自动挂起标志
		conCol_HQO_IsAutoSuspend,
		///业务单元
		conCol_HQO_BusinessUnit,
		///请求编号
		conCol_HQO_RequestID,
		///本地报单编号
		conCol_HQO_OrderLocalID,
		///交易所代码
		conCol_HQO_ExchangeID,
		///会员代码
		conCol_HQO_ParticipantID,
		///客户代码
		conCol_HQO_ClientID,
		///合约在交易所的代码
		conCol_HQO_ExchangeInstID,
		///交易所交易员代码
		conCol_HQO_TraderID,
		///安装编号
		conCol_HQO_InstallID,
		///报单提交状态
		conCol_HQO_OrderSubmitStatus,
		///报单提示序号
		conCol_HQO_NotifySequence,
		///报单编号
		conCol_HQO_OrderSysID,
		///报单来源
		conCol_HQO_OrderSource,
		///报单状态
		conCol_HQO_OrderStatus,
		///报单类型
		conCol_HQO_OrderType,
		///今成交数量
		conCol_HQO_VolumeTraded,
		///剩余数量
		conCol_HQO_VolumeTotal,
		///报单日期
		conCol_HQO_InsertDate,
		///委托时间
		conCol_HQO_InsertTime,
		///激活时间
		conCol_HQO_ActiveTime,
		///挂起时间
		conCol_HQO_SuspendTime,
		///最后修改时间
		conCol_HQO_UpdateTime,
		///撤销时间
		conCol_HQO_CancelTime,
		///最后修改交易所交易员代码
		conCol_HQO_ActiveTraderID,
		///序号
		conCol_HQO_SequenceNo,
		///前置编号
		conCol_HQO_FrontID,
		///会话编号
		conCol_HQO_SessionID,
		///用户端产品信息
		conCol_HQO_UserProductInfo,
		///状态信息
		conCol_HQO_StatusMsg,
		///用户强评标志
		conCol_HQO_UserForceClose,
		///操作用户代码
		conCol_HQO_ActiveUserID,
		///经纪公司报单编号
		conCol_HQO_BrokerOrderSeq,
		///相关报单
		conCol_HQO_RelativeOrderSysID,
	};

	// 查询历史成交grid列定义 
	enum {
		// 0, 被保留
		conCol_HQT_None,
		///交易日
		conCol_HQT_TradingDay,
		///投资者代码
		conCol_HQT_InvestorID,
		///合约代码
		conCol_HQT_InstrumentID,
		///报单引用
		conCol_HQT_OrderRef,
		///交易所代码
		conCol_HQT_ExchangeID,
		///成交编号
		conCol_HQT_TradeID,
		///买卖方向
		conCol_HQT_Direction,
		///报单编号
		conCol_HQT_OrderSysID,
		///开平标志
		conCol_HQT_OffsetFlag,
		///投机套保标志
		conCol_HQT_HedgeFlag,
		///价格
		conCol_HQT_Price,
		///数量
		conCol_HQT_Volume,
		///成交时期
		conCol_HQT_TradeDate,
		///成交时间
		conCol_HQT_TradeTime,
		///成交类型
		conCol_HQT_TradeType,
		///成交价来源
		conCol_HQT_PriceSource,
		///本地报单编号
		conCol_HQT_OrderLocalID,
		///序号
		conCol_HQT_SequenceNo,
		///成交来源
		conCol_HQT_TradeSource,
	};

	// 查询历史持仓明细grid列定义
	enum {
		// 0, 被保留
		conCol_HQPD_None,
		///交易日
		conCol_HQPD_TradingDay,
		///投资者代码
		conCol_HQPD_InvestorID,
		///合约代码
		conCol_HQPD_InstrumentID,
		///投机套保标志
		conCol_HQPD_HedgeFlag,
		///买卖
		conCol_HQPD_Direction,
		///开仓日期
		conCol_HQPD_OpenDate,
		///成交编号
		conCol_HQPD_TradeID,
		///数量 外盘净仓位
		conCol_HQPD_Volume,
		///开仓价
		conCol_HQPD_OpenPrice,
		///成交类型
		conCol_HQPD_TradeType,
		/////组合合约代码
		//conCol_HQPD_CombInstrumentID,
		///交易所代码
		conCol_HQPD_ExchangeID,
		///逐日盯市平仓盈亏
		conCol_HQPD_CloseProfitByDate,
		///逐笔对冲平仓盈亏
		conCol_HQPD_CloseProfitByTrade,
		///逐日盯市持仓盈亏
		conCol_HQPD_PositionProfitByDate,
		///逐笔对冲持仓盈亏
		conCol_HQPD_PositionProfitByTrade,
		///投资者保证金
		conCol_HQPD_Margin,
		///保证金率
		conCol_HQPD_MarginRateByMoney,
		///保证金率(按手数)
		conCol_HQPD_MarginRateByVolume,
		///昨结算价
		conCol_HQPD_LastSettlementPrice,
		///结算价
		conCol_HQPD_SettlementPrice,
		///平仓量
		conCol_HQPD_CloseVolume,
		///平仓金额
		conCol_HQPD_CloseAmount,
	};

	// 查询历史持仓grid列定义
	enum {
		// 0, 被保留
		conCol_HQP_None,
		///交易日
		conCol_HQP_TradingDay,
		///账户号
		conCol_HQP_InvestorID,
		///合约代码
		conCol_HQP_InstrumentID,
		///持仓多空方向
		conCol_HQP_PosiDirection,
		///投机套保标志
		conCol_HQP_HedgeFlag,
		///上日持仓
		conCol_HQP_YdPosition,
		///今日持仓
		conCol_HQP_Position,
		///开仓量
		conCol_HQP_OpenVolume,
		///平仓量
		conCol_HQP_CloseVolume,
		///开仓金额
		conCol_HQP_OpenAmount,
		///平仓金额
		conCol_HQP_CloseAmount,
		///持仓成本
		conCol_HQP_PositionCost,
		///上次占用的保证金
		conCol_HQP_PreMargin,
		///占用的保证金
		conCol_HQP_UseMargin,
		///资金差额
		conCol_HQP_CashIn,
		///手续费
		conCol_HQP_Commission,
		///平仓盈亏
		conCol_HQP_CloseProfit,
		///持仓盈亏
		conCol_HQP_PositionProfit,
		///上次结算价
		conCol_HQP_PreSettlementPrice,
		///本次结算价
		conCol_HQP_SettlementPrice,
		///开仓成本
		conCol_HQP_OpenCost,
		/////组合成交形成的持仓
		//conCol_HQP_CombPosition,
		/////组合多头冻结
		//conCol_HQP_CombLongFrozen,
		/////组合空头冻结
		//conCol_HQP_CombShortFrozen,
		///逐日盯市平仓盈亏
		conCol_HQP_CloseProfitByDate,
		///逐笔对冲平仓盈亏
		conCol_HQP_CloseProfitByTrade,
		///今日持仓
		conCol_HQP_TodayPosition,
		///保证金率
		conCol_HQP_MarginRateByMoney,
		///保证金率(按手数)
		conCol_HQP_MarginRateByVolume,
	};

	// 查询历史帐号资金grid列定义
	enum {
		// 0, 被保留
		conCol_HQA_None,
		///交易日
		conCol_HQA_TradingDay,
		///账户号
		conCol_HQA_InvestorID,
		/////账户名称
		//conCol_HQA_InvestorName,
		///期初结存
		conCol_HQA_StaticProfit,
		///期末结存
		conCol_HQA_DynamicProfit,
		///入金金额
		conCol_HQA_Within,
		///出金金额
		conCol_HQA_Without,
		///占用保证金
		conCol_HQA_CurrMargin,
		///可用资金
		conCol_HQA_Available,
		///平仓盈亏
		conCol_HQA_CloseProfit,
		///持仓盈亏
		conCol_HQA_PositionProfit,
		///手续费
		conCol_HQA_Commission,
		///风险度
		conCol_HQA_RiskDegree,
	};

	// 查询历史帐号资金进出grid列定义
	enum {
		// 0, 被保留
		conCol_HQFIN_None,
		///操作日期
		conCol_HQFIN_OperateDate,
		///账户号
		conCol_HQFIN_InvestorID,
		/////账户名称
		//conCol_HQFIN_InvestorName,
		///出金还是入金
		conCol_HQFIN_InOrOut,
		///发生额
		conCol_HQFIN_Volume,
		///操作员
		conCol_HQFIN_OperateUser,
		///操作时间
		conCol_HQFIN_OperateTime,
		///备注
		conCol_HQFIN_Remark,
	};

	// 查询用户发送消息grid列定义
	enum {
		// 0, 被保留
		conCol_USM_None,
		///消息ID
		conCol_USM_MSGID,
		///标题
		conCol_USM_Title,
		///内容
		conCol_USM_MSG,
		///时间
		conCol_USM_CreateTime,
		///已接收人
		conCol_USM_HasRecvUser,
		///未接收人
		conCol_USM_NotRecvUser,
	};

	// 查询用户接收消息grid列定义
	enum {
		// 0, 被保留
		conCol_URM_None,
		///消息ID
		conCol_URM_TARGETMSGID,
		///标题
		conCol_URM_Title,
		///内容
		conCol_URM_MSG,
		///时间
		conCol_URM_RecvTime,
		///发送人
		conCol_URM_SendUser,
	};
}
