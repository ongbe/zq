#pragma once

namespace CltRisk {

	enum {
		conGridNone,					// 0, 被保留
		conGridQueryUserSendMsg,		// 查询用户发送消息grid
		conGridQueryUserRecvMsg,		// 查询用户接收消息grid 
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

    // 深度行情grid列定义
    enum {
        // 0, 被保留
        conCol_DepthMarket_None,
        ///合约ID
        conCol_DepthMarket_InstrumentID,
        ///最新价
        conCol_DepthMarket_LastPrice,
        ///涨跌
        conCol_DepthMarket_Mixed,
        ///涨跌幅度
        conCol_DepthMarket_MixedRange,
        ///买价
        conCol_DepthMarket_BidPrice1,
        ///买量
        conCol_DepthMarket_BidVolume1,
        // 卖价
        conCol_DepthMarket_AskPrice1,
        ///卖量
        conCol_DepthMarket_AskVolume1,
        ///成交量
        conCol_DepthMarket_Volume,
        ///持仓量
        conCol_DepthMarket_OpenInterest,
        ///现量
        conCol_DepthMarket_CurVolume,
        ///今开盘
        conCol_DepthMarket_OpenPrice,
        ///昨结算
        conCol_DepthMarket_SettlementPrice,
        // 最高价
        conCol_DepthMarket_HighestPrice,
        ///最低价
        conCol_DepthMarket_LowestPrice,
        ///涨停价
        conCol_DepthMarket_UpperLimitPrice,
        ///跌停价
        conCol_DepthMarket_LowerLimitPrice,
        ///更新时间
        conCol_DepthMarket_UpdateTime,        
    };

    // 风险事件grid列定义
    enum {
        // 0, 被保留
        conCol_RiskEvent_None,
        ///风险事件ID
        conCol_RiskEvent_RiskEventID,
        ///交易员ID
        conCol_RiskEvent_TradeInvestorID,
        ///交易员名称
        conCol_RiskEvent_TraderName,
        ///风险指标类型
        conCol_RiskEvent_IndicatorName,
        ///风险详情
        conCol_RiskEvent_RiskDesc,
        ///更新时间
        conCol_RiskEvent_EventTime,
        // 风险事件状态
        conCol_RiskEvent_EventStatus,       
    };

    // 账户和资金grid列定义
    enum {
        // 0, 被保留
        conCol_TradingAccount_None,
        ///交易员
        conCol_TradingAccount_Account,
        ///交易限制
        conCol_TradingAccount_LimitTade,
        ///手动审核
        conCol_TradingAccount_ManuVerify,
        ///登录状态
        conCol_TradingAccount_LoginStatus,
        ///风险度(%)
        conCol_TradingAccount_RiskDegree,
        ///动态权益
        conCol_TradingAccount_DynamicProfit,
        // 持仓盈亏
        conCol_TradingAccount_PositionProfit,       
        ///平仓盈亏
        conCol_TradingAccount_CloseProfit,
        ///占用保证金
        conCol_TradingAccount_CurrMargin,
        ///手续费
        conCol_TradingAccount_Commission,
        ///冻结保证金
        conCol_TradingAccount_FrozenMargin,
        // 冻结手续费
        conCol_TradingAccount_FrozenCommission,   
    };

    // 委托单grid列定义
    enum {
        // 0, 被保留
        conCol_AllOrder_None,
        ///交易员
        conCol_AllOrder_InvestorID,
        ///报单编号
        conCol_AllOrder_OrderSysID,
        ///合约
        conCol_AllOrder_InstrumentID,
        ///买卖
        conCol_AllOrder_Direction,
        ///开平
        conCol_AllOrder_CombOffsetFlag,
        ///挂单状态
        conCol_AllOrder_OrderStatus,
        // 报单价格
        conCol_AllOrder_LimitPrice,
        ///报单手数
        conCol_AllOrder_VolumeTotalOriginal,
        ///未成交手数
        conCol_AllOrder_VolumeTotal,
        ///成交手数
        conCol_AllOrder_Volume,
        ///详细状态
        conCol_AllOrder_StatusMsg,
        ///报单时间
        conCol_AllOrder_InsertTime,
        ///冻结保证金
        conCol_AllOrder_freezeMargin,
        // 冻结手续费
        conCol_AllOrder_troubleMoney,
        ///投保
        conCol_AllOrder_HedgeFlag,
        ///交易所
        conCol_AllOrder_ExchangeID,
        ///前置机
        conCol_AllOrder_FrontID,
        ///会话机
        conCol_AllOrder_SessionID,   
        ///报单引用
        conCol_AllOrder_OrderRef,
        ///状态
        conCol_AllOrder_ExStatus,  
    };

    // 成交单grid列定义
    enum {
        // 0, 被保留
        conCol_Trade_None,
        ///交易员
        conCol_Trade_InvestorID,
        ///合约
        conCol_Trade_InstrumentID,
        ///买卖
        conCol_Trade_Direction,
        ///开平
        conCol_Trade_OffsetFlag,
        ///成交价格
        conCol_Trade_Price,
        ///成交手数
        conCol_Trade_Volume,
        // 手续费
        conCol_Trade_TradeCommission,
        ///成交时间
        conCol_Trade_TradeTime,
        ///成交类型
        conCol_Trade_TradeType,
        ///投保
        conCol_Trade_HedgeFlag,
        ///成交编号
        conCol_Trade_TradeID,
        ///报单编号
        conCol_Trade_OrderSysID,
        ///交易所
        conCol_Trade_ExchangeID, 
    };

    // 持仓grid列定义
    enum {
        // 0, 被保留
        conCol_Position_None,
        ///交易员
        conCol_Position_InvestorID,
        ///合约
        conCol_Position_InstrumentID,
        ///买卖
        conCol_Position_Direction,
        ///投保
        conCol_Position_HedgeFlag,
        ///总持仓
        conCol_Position_Position,
        ///可平量
        conCol_Position_CanCloseVolume,
        // 现价
        conCol_Position_LastPrice,
        ///持仓均价
        conCol_Position_AveragePositionPrice,
        ///开仓均价
        conCol_Position_AverageOpenPrice,
        ///持仓盈亏
        conCol_Position_PositionProfit,
        ///浮动盈亏
        conCol_Position_PositionProfitByTrade,
        ///盯市总盈亏
        conCol_Position_TotalPositionProfitByDate,
        ///占用保证金
        conCol_Position_UseMargin,
        // 总多仓
        conCol_Position_MultiBuyPosition,
        ///总空仓
        conCol_Position_MultiSellPosition,
        ///今多仓
        conCol_Position_TodayBuyPosition,
        ///今空仓
        conCol_Position_TodaySellPosition,
        ///昨仓
        conCol_Position_YesterdayPosition,   
        ///今仓
        conCol_Position_TodayPosition,
        ///今开仓量
        conCol_Position_OpenVolume,  
        // 今平仓量
        conCol_Position_CloseVolume,
        ///总可平今量
        conCol_Position_TotalCanCloseTodayVolume,
        ///总可平昨量
        conCol_Position_TotalCanCloseYesterdayVolume,
        ///平今挂单量
        conCol_Position_CloseTodayOrderVolume,
        ///平仓挂单量
        conCol_Position_CloseOrderVolume,
        ///可平今量
        conCol_Position_CanCloseTodayVolume,
        ///可平昨量
        conCol_Position_CanCloseYesterdayVolume,
        // 交易所
        conCol_Position_ExchangeID, 
    };
}
