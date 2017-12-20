#pragma once
/*
#include "Ctp/UserApiStruct.h"
#include "RiskManage/RiskManageStruct.h"
#include "Constants.h"

typedef enum _tagSPIMSGID
{
	MSGID_Begin=1000,
	MSGID_FrontConnected,
	MSGID_FrontDisconnected,
	MSGID_HeartBeatWarning,
	MSGID_RspError,
	MSGID_RtnBrokerDeposit,
	MSGID_RtnInvestorSumInfo,
	MSGID_RtnClientSGDataSyncStart,
	MSGID_RtnClientSGDataSyncEnd,
	MSGID_RspRiskUserLogin,
	MSGID_RspQryInvestorMarginRate,
	MSGID_RtnProduct,
	MSGID_RtnInstrument,
	MSGID_RspQryOrderStat,
	MSGID_RtnExchange,
	MSGID_RspInvestorPositionStatic,
	MSGID_RspInvestorTradeStatic,
	MSGID_RtnRiskDepthMarketData,
	MSGID_RtnTimeSync,
	MSGID_RspInstPositionRate,
	MSGID_RspProductPositionRate,
	MSGID_RtnInvestorGroup,
	MSGID_RtnExchangeMarginRate,
	MSGID_RtnExchangeMarginRateAdjust,
	MSGID_RspQryTradingCodeHash,
	MSGID_RspQryTradingCode,
	MSGID_RtnTradingCode,
	MSGID_RtnDelTradingCode,
	MSGID_RtnSequencialBrokerUserEvent,
	MSGID_RtnSequencialTrade,
	MSGID_RtnSequencialOrder,
	MSGID_RspRiskOrderInsert,
	MSGID_RspRiskOrderAction,
	MSGID_RtnSequencialPosition,
	MSGID_RspRiskNotifyCommand,
	MSGID_RspBatchForceCloseCalc,
	MSGID_RspForceCloseCalc,
	MSGID_RspSetIndexNPPPara,
	MSGID_RtnIndexNPP,
	MSGID_RspRiskDelIndexNPPPara,
	MSGID_RspForceCloseAccount,
	MSGID_RspQryLogin,
	MSGID_RspQrySafePriceRange,
	MSGID_RspSafePriceAccount,
	MSGID_RspQryPriceVaryEffect,
	MSGID_RtnDepartment,
	MSGID_RspIndexNPP,
	MSGID_RtnTradeParam,
	MSGID_RspRiskParkedOrderInsert,
	MSGID_RspRemoveRiskParkedOrder,
	MSGID_RtnSeqRiskParkedOrder,
	MSGID_RspRiskUserPasswordUpd,
	MSGID_RtnSeqDeposit,
	MSGID_RspAddRiskUserEvent,
	MSGID_RspPredictRiskAccount,
	MSGID_RspPredictRiskPosition,
	MSGID_RtnRiskInvestor,
	MSGID_RspQryInvestorLinkMan,
	MSGID_RtnDelExchangeMarginRate,
	MSGID_RtnDelExchangeMarginRateAdjust,
	MSGID_RspQryInvestorDepartment,
	MSGID_RtnDelIndexNPP,
	MSGID_RtnRiskUserFunction,
	MSGID_RtnDelRiskUserFunction,
	MSGID_RtnRiskSyncAccount,
	MSGID_RtnSeqPreRiskAccount,
	MSGID_RtnNoticeToken,
	MSGID_RtnNoticePattern,
	MSGID_RspModNoticePattern,
	MSGID_RtnVaryMarketData,
	MSGID_RspAddRiskNotifyA,
	MSGID_RspAddBizNotice,
	MSGID_RtnSeqBizNotice,
	MSGID_RspRiskQryBrokerDeposit,
	MSGID_RtnRiskParamInfo,
	MSGID_RspModRiskInvestorParam,
	MSGID_RspRemRiskInvestorParam,
	MSGID_RtnRiskInvestorParam,
	MSGID_RtnDelRiskInvestorParam,
	MSGID_RspForceRiskUserLogout,
	MSGID_RtnForceRiskUserLogout,
	MSGID_RspAddRiskPattern,
	MSGID_RspModRiskPattern,
	MSGID_RspRemRiskPattern,
	MSGID_RtnRiskPattern,
	MSGID_RtnDelRiskPattern,
	MSGID_RspAddInvestorPattern,
	MSGID_RspModInvestorPattern,
	MSGID_RspRemInvestorPattern,
	MSGID_RtnInvestorPattern,
	MSGID_RtnDelInvestorPattern,
	MSGID_RtnRiskNotifyToken,
	MSGID_RtnSeqRiskNotifyB,
	MSGID_RspQryPositionStat,
	MSGID_RspQryTradeStat,
	MSGID_RspQryInvestorLinkManHash,
	MSGID_RspQryInvestorDepartmentHash,
	MSGID_RspQryStressTest,
	MSGID_RspQryLowMarginInvestorHash,
	MSGID_RspQryLowMarginInvestor,
	MSGID_RtnLowMarginInvestor,
	MSGID_RtnDelLowMarginInvestor,
}SPIMSGID;
struct SpiData 
{
	int CmdID;
	union SpiDataMain
	{
		int	nReason;
		int	nTimeLapse;
		BrokerDepositField	BrokerDeposit;
		InvestorSumInfoField	InvestorSumInfo;
		SettlementSessionField	SettlementSession;
		RspRiskUserLoginField	RspRiskUserLogin;
		InvestorMarginRateField	InvestorMarginRate;
		ProductField	Product;
		InstrumentField	Instrument;
		OrderStatField	OrderStat;
		ExchangeField	Exchange;
		InvestorPositionStaticField	InvestorPositionStatic;
		InvestorTradeStaticField	InvestorTradeStatic;
		DepthMarketDataField	DepthMarketData;
		CurrentTimeField	CurrentTime;
		RspInstPositionRateField	RspInstPositionRate;
		RspProductPositionRateField	RspProductPositionRate;
		InvestorGroupField	InvestorGroup;
		ExchangeMarginRateField	ExchangeMarginRate;
		ExchangeMarginRateAdjustField	ExchangeMarginRateAdjust;
		InvestorHashField	InvestorHash;
		TradingCodeField	TradingCode;
		SequencialBrokerUserEventField	SequencialBrokerUserEvent;
		SequencialTradeField	SequencialTrade;
		SequencialOrderField	SequencialOrder;
		InputOrderField	InputOrder;
		InputOrderActionField	InputOrderAction;
		SequencialPositionField	SequencialPosition;
		RspForceClosePositionField	RspForceClosePosition;
		IndexNPPField	IndexNPP;
		FullIndexNPPField	FullIndexNPP;
		InvestorRiskAccountField	InvestorRiskAccount;
		NormalRiskQueryField	NormalRiskQuery;
		RspSafePriceRangeField	RspSafePriceRange;
		PriceVaryParamField	PriceVaryParam;
		DepartmentField	Department;
		TradeParamField	TradeParam;
		RiskParkedOrderField	RiskParkedOrder;
		RemoveRiskParkedOrderField	RemoveRiskParkedOrder;
		SeqRiskParkedOrderField	SeqRiskParkedOrder;
		UserPasswordUpdateField	UserPasswordUpdate;
		SeqDepositField	SeqDeposit;
		RiskUserEventField	RiskUserEvent;
		PredictRiskAccountField	PredictRiskAccount;
		PredictRiskPositionField	PredictRiskPosition;
		RiskSyncInvestorField	RiskSyncInvestor;
		InvestorLinkManField	InvestorLinkMan;
		InvestorDepartmentFlatField	InvestorDepartmentFlat;
		RiskUserFunctionField	RiskUserFunction;
		RiskSyncAccountField	RiskSyncAccount;
		SeqPreRiskAccountField	SeqPreRiskAccount;
		NoticeTokenField	NoticeToken;
		NoticePatternField	NoticePattern;
		VaryMarketDataField	VaryMarketData;
		SeqBizNoticeField	SeqBizNotice;
		QueryBrokerDepositField	QueryBrokerDeposit;
		RiskParamInfoField	RiskParamInfo;
		RiskInvestorParamField	RiskInvestorParam;
		RiskLoginInfoField	RiskLoginInfo;
		RiskPatternField	RiskPattern;
		InvestorPatternField	InvestorPattern;
		RiskNotifyTokenField	RiskNotifyToken;
		SeqRiskNotifyBField	SeqRiskNotifyB;
		PositionStatField	PositionStat;
		TradeStatField	TradeStat;
		BrokerInvestorField	BrokerInvestor;
	}Main;
	RspInfoField RspInfo;
	int nRequestID;
	bool bIsLast;
};
typedef enum _tagR2CMSGID
{
	R2CMSGID_BEGIN=50,
	R2CMSGID_INPUTORDER,
	R2CMSGID_FUND,
	R2CMSGID_POSITION,
	R2CMSGID_TRADE,
	R2CMSGID_ORDER,
	R2CMSGID_DEPOSIT,
	R2CMSGID_QUOT,
	R2CMSGID_RISKEVENT,
	R2CMSGID_EVENTHANDLING,
	R2CMSGID_STATISTICS,
	R2CMSGID_TARGETACCOUNT,
	R2CMSGID_MESSAGEINFO,
	R2CMSGID_NETVALUE,//»ù½ð¾»Öµ
}R2CMSGID;
struct Risk2ClientData 
{
	int CmdID;
	int AccID;
	union RIsk2ClientMain
	{
		InputOrderField		InputOrder;
		RiskSyncAccountField  RiskSyncAccount;
		SequencialPositionField Position;
		SequencialTradeField Trade;
		SequencialOrderField Order;
		SeqDepositField Deposit;
		DepthMarketDataField MarketData;
		RiskEvent      Event;
		RiskEventHandling Handling;
		StatisticsEntry  Statistics;
		TargetAccount	 Target;
		MessageInfo		MsgInfo;
		NetValueInfo	netValueInfo;
	}Main;
};
typedef enum _tagC2RMSGID
{
	C2RMSGID_BEGIN=50,
	C2RMSGID_ForceCloseOrderInsert,
}C2RMSGID;
struct Client2RiskData 
{
	int CmdID;
	union Client2RiskMain
	{
		RiskForceCloseOrderField  ForceCloseOrder;
	}Main;
};*/
typedef enum _tagSubscribeID
{
	SubscribeID_Quot,
	SubscribeID_Fund,
	SubscribeID_Position,
	SubscribeID_Trade,
	SubscribeID_Order,
	SubscribeID_Deposit,
	SubscribeID_RiskEvent,
	SubscribeID_RiskEvtHandling,
	SubscribeID_Statistics,
	SubscribeID_NetValue,
}SubscribeID;
typedef struct _tagSubscribeType
{
	int  AccID;
	bool bFund;
	bool bPosition;
	bool bTrade;
	bool bOrder;
	bool bDeposit;
	bool bRiskEvent;
	bool bRiskEvtHandling;
	bool bNetValue;
}SubscribeType;