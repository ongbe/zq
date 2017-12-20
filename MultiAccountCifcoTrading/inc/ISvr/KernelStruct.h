#pragma once

#define DOUBLEINVALIDVALUE	(1.7976931348623158e+308)
#define INTINVALIDVALUE		(0xfeeefeee)

#define ORDERCLASS_NewOrder 'N'
#define ORDERCLASS_ReplaceOrder 'R'

typedef char CommonAccountIDType[13];

typedef char CommonOrderClassType;
typedef char CommonUserIDType[32];                  //用户名ID，对应综合交易平台投资者ID、Pats的UserName
typedef char CommonAccountType[32];                 //账号，对应Pats的Account
typedef char CommonLocalRequestIDType[32];
typedef char CommonLocalRefIDType[32];
typedef char CommonExchangeIDType[32];
typedef char CommonExchangeNameType[32];            //交易所名称
typedef char CommonCurrencyType[16];                //货币名称
typedef char CommonOrderRefType[32];                //报单的OrderRef
typedef char CommonOrderSysIDType[32];              //报单的OrderSysID
typedef char CommonInstrumentIDType[32];            //合约ID
typedef char CommonDateType[16];                    //日期
typedef char CommonTradeIDType[32];                 //成交ID

typedef char ContractShortNamesType[4096];			//合约简称表
typedef char PatsystemAccountType[50];

typedef char InstrumentIDType[64];

typedef char TThostFtdcBrokerIDType[11];
typedef char TThostFtdcAccountIDType[13];
typedef double TThostFtdcMoneyType;
typedef char TThostFtdcDateType[9];
typedef int TThostFtdcSettlementIDType;
typedef char TThostFtdcExchangeIDType[9];
typedef char InstrumentIDType[64];
typedef char TThostFtdcExchangeInstIDType[31];
typedef char TThostFtdcProductClassType;
typedef int TThostFtdcYearType;
typedef int TThostFtdcMonthType;
typedef int TThostFtdcVolumeType;
typedef int TThostFtdcVolumeMultipleType;
typedef double TThostFtdcPriceType;
typedef char TThostFtdcInstLifePhaseType;
typedef int TThostFtdcBoolType;
typedef char TThostFtdcPositionTypeType;
typedef char TThostFtdcPositionDateTypeType;
typedef double TThostFtdcRatioType;
typedef char TThostFtdcInstrumentIDType[31];
typedef char TThostFtdcInvestorRangeType;
typedef char TThostFtdcInvestorIDType[13];
typedef char TThostFtdcHedgeFlagType;
typedef char TThostFtdcOrderRefType[13];
typedef char TThostFtdcUserIDType[16];
typedef char TThostFtdcTradeIDType[21];
typedef char TThostFtdcDirectionType;
typedef char TThostFtdcOrderSysIDType[21];
typedef char TThostFtdcParticipantIDType[11];
typedef char TThostFtdcClientIDType[11];
typedef char TThostFtdcTradingRoleType;
typedef char TThostFtdcOffsetFlagType;
typedef char TThostFtdcTimeType[9];
typedef char TThostFtdcTradeTypeType;
typedef char TThostFtdcPriceSourceType;
typedef char TThostFtdcTraderIDType[21];
typedef char TThostFtdcOrderLocalIDType[13];
typedef char TThostFtdcBusinessUnitType[21];
typedef int TThostFtdcSequenceNoType;
typedef char TThostFtdcTradeSourceType;
typedef char TThostFtdcOrderPriceTypeType;
typedef char TThostFtdcCombOffsetFlagType[5];
typedef char TThostFtdcCombHedgeFlagType[5];
typedef char TThostFtdcTimeConditionType;
typedef char TThostFtdcVolumeConditionType;
typedef char TThostFtdcContingentConditionType;
typedef char TThostFtdcForceCloseReasonType;
typedef int TThostFtdcRequestIDType;
typedef int TThostFtdcInstallIDType;
typedef char TThostFtdcOrderSubmitStatusType;
typedef char TThostFtdcOrderSourceType;
typedef char TThostFtdcOrderStatusType;
typedef char TThostFtdcOrderTypeType;
typedef int TThostFtdcSessionIDType;
typedef int TThostFtdcFrontIDType;
typedef char TThostFtdcProductInfoType[11];
typedef char TThostFtdcErrorMsgType[81];
typedef double TThostFtdcLargeVolumeType;
typedef int TThostFtdcMillisecType;
typedef char TThostFtdcPasswordType[41];
typedef char TThostFtdcParkedOrderIDType[13];
typedef char TThostFtdcUserTypeType;
typedef char TThostFtdcParkedOrderStatusType;
typedef int TThostFtdcErrorIDType;
typedef int TThostFtdcOrderActionRefType;
typedef char TThostFtdcActionFlagType;
typedef char TThostFtdcParkedOrderActionIDType[13];
typedef char TThostFtdcBankIDType[4];
typedef char TThostFtdcBankNameType[101];
typedef char TThostFtdcBankAccountType[41];
typedef char TThostFtdcCurrencyIDType[4];
typedef char TThostFtdcBankBrchIDType[5];
typedef int TThostFtdcLegIDType;
typedef int TThostFtdcLegMultipleType;
typedef char TThostFtdcPosiDirectionType;
typedef char TThostFtdcPositionDateType;
typedef char TThostFtdcTradeCodeType[7];
typedef char TThostFtdcFutureBranchIDType[31];
typedef char TThostFtdcTradeDateType[9];
typedef char TThostFtdcTradeTimeType[9];
typedef char TThostFtdcBankSerialType[13];
typedef int TThostFtdcSerialType;
typedef char TThostFtdcLastFragmentType;
typedef char TThostFtdcIndividualNameType[51];
typedef char TThostFtdcIdCardTypeType;
typedef char TThostFtdcIdentifiedCardNoType[51];
typedef char TThostFtdcGenderType;
typedef char TThostFtdcCountryCodeType[21];
typedef char TThostFtdcCustTypeType;
typedef double TThostFtdcTradeAmountType;
typedef char TThostFtdcFeePayFlagType;
typedef double TThostFtdcCustFeeType;
typedef double TThostFtdcFutureFeeType;
typedef char TThostFtdcAddInfoType[129];
typedef int TThostFtdcFutureSerialType;
typedef char TThostFtdcYesNoIndicatorType;
typedef char TThostFtdcDigestType[36];
typedef char TThostFtdcBankAccTypeType;
typedef char TThostFtdcDeviceIDType[3];
typedef char TThostFtdcBankAccTypeType;
typedef char TThostFtdcBankCodingForFutureType[33];
typedef char TThostFtdcPwdFlagType;
typedef char TThostFtdcOperNoType[17];
typedef int TThostFtdcTIDType;
typedef char TThostFtdcTransferStatusType;
///////////////////////////////////////////////////////////////////////////////////
struct CThostFtdcTradingAccountFieldEx
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///上次质押金额
	TThostFtdcMoneyType	PreMortgage;
	///上次信用额度
	TThostFtdcMoneyType	PreCredit;
	///上次存款额
	TThostFtdcMoneyType	PreDeposit;
	///上次结算准备金
	TThostFtdcMoneyType	PreBalance;
	///上次占用的保证金
	TThostFtdcMoneyType	PreMargin;
	///利息基数
	TThostFtdcMoneyType	InterestBase;
	///利息收入
	TThostFtdcMoneyType	Interest;
	///入金金额
	TThostFtdcMoneyType	Deposit;
	///出金金额
	TThostFtdcMoneyType	Withdraw;
	///冻结的保证金
	TThostFtdcMoneyType	FrozenMargin;
	///冻结的资金
	TThostFtdcMoneyType	FrozenCash;
	///冻结的手续费
	TThostFtdcMoneyType	FrozenCommission;
	///当前保证金总额
	TThostFtdcMoneyType	CurrMargin;
	///资金差额
	TThostFtdcMoneyType	CashIn;
	///手续费
	TThostFtdcMoneyType	Commission;
	///平仓盈亏
	TThostFtdcMoneyType	CloseProfit;
	///持仓盈亏
	TThostFtdcMoneyType	PositionProfit;
	///期货结算准备金
	TThostFtdcMoneyType	Balance;
	///可用资金
	TThostFtdcMoneyType	Available;
	///可取资金
	TThostFtdcMoneyType	WithdrawQuota;
	///基本准备金
	TThostFtdcMoneyType	Reserve;
	///交易日
	TThostFtdcDateType	TradingDay;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;
	///信用额度
	TThostFtdcMoneyType	Credit;
	///质押金额
	TThostFtdcMoneyType	Mortgage;
	///交易所保证金
	TThostFtdcMoneyType	ExchangeMargin;
	///投资者交割保证金
	TThostFtdcMoneyType	DeliveryMargin;
	///交易所交割保证金
	TThostFtdcMoneyType	ExchangeDeliveryMargin;
	///静态权益
	TThostFtdcMoneyType StaticProfit;
	///动态权益
	TThostFtdcMoneyType DynamicProfit;
	///风险度
	TThostFtdcMoneyType RiskDegree;
};

///合约信息
struct PlatformStru_InstrumentInfo
{
	///合约代码
	InstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///合约名称
	InstrumentIDType	InstrumentName;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///产品代码
	InstrumentIDType	ProductID;
	///产品类型
	TThostFtdcProductClassType	ProductClass;
	///交割年份
	TThostFtdcYearType	DeliveryYear;
	///交割月
	TThostFtdcMonthType	DeliveryMonth;
	///市价单最大下单量
	TThostFtdcVolumeType	MaxMarketOrderVolume;
	///市价单最小下单量
	TThostFtdcVolumeType	MinMarketOrderVolume;
	///限价单最大下单量
	TThostFtdcVolumeType	MaxLimitOrderVolume;
	///限价单最小下单量
	TThostFtdcVolumeType	MinLimitOrderVolume;
	///合约数量乘数
	TThostFtdcVolumeMultipleType	VolumeMultiple;
	///最小变动价位
	TThostFtdcPriceType	PriceTick;
	///创建日
	TThostFtdcDateType	CreateDate;
	///上市日
	TThostFtdcDateType	OpenDate;
	///到期日
	TThostFtdcDateType	ExpireDate;
	///开始交割日
	TThostFtdcDateType	StartDelivDate;
	///结束交割日
	TThostFtdcDateType	EndDelivDate;
	///合约生命周期状态
	TThostFtdcInstLifePhaseType	InstLifePhase;
	///当前是否交易
	TThostFtdcBoolType	IsTrading;
	///持仓类型
	TThostFtdcPositionTypeType	PositionType;
	///持仓日期类型
	TThostFtdcPositionDateTypeType	PositionDateType;
	///多头保证金率
	TThostFtdcRatioType	LongMarginRatio;
	///空头保证金率
	TThostFtdcRatioType	ShortMarginRatio;

	char Currency [11];
	int  TicksPerPoint;
	char TickSize[10];
};

///手续费信息
struct PlatformStru_InstrumentCommissionRate
{
	//CCifcoInstrumentCommissionRateField Thost;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///投资者范围
	TThostFtdcInvestorRangeType	InvestorRange;
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///开仓手续费率
	TThostFtdcRatioType	OpenRatioByMoney;
	///开仓手续费
	TThostFtdcRatioType	OpenRatioByVolume;
	///平仓手续费率
	TThostFtdcRatioType	CloseRatioByMoney;
	///平仓手续费
	TThostFtdcRatioType	CloseRatioByVolume;
	///平今手续费率
	TThostFtdcRatioType	CloseTodayRatioByMoney;
	///平今手续费
	TThostFtdcRatioType	CloseTodayRatioByVolume;
};

///保证金信息
struct PlatformStru_InstrumentMarginRate
{
	// CCifcoInstrumentMarginRateField Thost;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///投资者范围
	TThostFtdcInvestorRangeType	InvestorRange;
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///多头保证金率
	TThostFtdcRatioType	LongMarginRatioByMoney;
	///多头保证金费
	TThostFtdcMoneyType	LongMarginRatioByVolume;
	///空头保证金率
	TThostFtdcRatioType	ShortMarginRatioByMoney;
	///空头保证金费
	TThostFtdcMoneyType	ShortMarginRatioByVolume;
	///是否相对交易所收取
	TThostFtdcBoolType	IsRelative;
};

///交易账号信息
struct PlatformStru_TradingAccountInfo
{
	// CCifcoTradingAccountFieldEx Thost;
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///上次质押金额
	TThostFtdcMoneyType	PreMortgage;
	///上次信用额度
	TThostFtdcMoneyType	PreCredit;
	///上次存款额
	TThostFtdcMoneyType	PreDeposit;
	///上次结算准备金
	TThostFtdcMoneyType	PreBalance;
	///上次占用的保证金
	TThostFtdcMoneyType	PreMargin;
	///利息基数
	TThostFtdcMoneyType	InterestBase;
	///利息收入
	TThostFtdcMoneyType	Interest;
	///入金金额
	TThostFtdcMoneyType	Deposit;
	///出金金额
	TThostFtdcMoneyType	Withdraw;
	///冻结的保证金
	TThostFtdcMoneyType	FrozenMargin;
	///冻结的资金
	TThostFtdcMoneyType	FrozenCash;
	///冻结的手续费
	TThostFtdcMoneyType	FrozenCommission;
	///当前保证金总额
	TThostFtdcMoneyType	CurrMargin;
	///资金差额
	TThostFtdcMoneyType	CashIn;
	///手续费
	TThostFtdcMoneyType	Commission;
	///平仓盈亏
	TThostFtdcMoneyType	CloseProfit;
	///持仓盈亏
	TThostFtdcMoneyType	PositionProfit;
	///期货结算准备金
	TThostFtdcMoneyType	Balance;
	///可用资金
	TThostFtdcMoneyType	Available;
	///可取资金
	TThostFtdcMoneyType	WithdrawQuota;
	///基本准备金
	TThostFtdcMoneyType	Reserve;
	///交易日
	TThostFtdcDateType	TradingDay;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;
	///信用额度
	TThostFtdcMoneyType	Credit;
	///质押金额
	TThostFtdcMoneyType	Mortgage;
	///交易所保证金
	TThostFtdcMoneyType	ExchangeMargin;
	///投资者交割保证金
	TThostFtdcMoneyType	DeliveryMargin;
	///交易所交割保证金
	TThostFtdcMoneyType	ExchangeDeliveryMargin;
	///静态权益
	TThostFtdcMoneyType StaticProfit;
	///动态权益
	TThostFtdcMoneyType DynamicProfit;
	///风险度
	TThostFtdcMoneyType RiskDegree;


	/*patsystem*/
	PatsystemAccountType Account;
	//剩余购买力
	PatsystemAccountType BPRemaining;
	//合约占用的资金
	PatsystemAccountType BPUsed;	
	//合约占用资金比率
	PatsystemAccountType Exposure;
	//合约的盈亏比例
	PatsystemAccountType BurnRate;
	PatsystemAccountType MarginPaid;
};


///成交信息
struct PlatformStru_TradeInfo
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///报单引用
	TThostFtdcOrderRefType	OrderRef;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///成交编号
	TThostFtdcTradeIDType	TradeID;
	///买卖方向
	TThostFtdcDirectionType	Direction;
	///报单编号
	TThostFtdcOrderSysIDType	OrderSysID;
	///会员代码
	TThostFtdcParticipantIDType	ParticipantID;
	///客户代码
	TThostFtdcClientIDType	ClientID;
	///交易角色
	TThostFtdcTradingRoleType	TradingRole;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///开平标志
	TThostFtdcOffsetFlagType	OffsetFlag;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///价格
	TThostFtdcPriceType	Price;
	///数量
	TThostFtdcVolumeType	Volume;
	///成交时期
	TThostFtdcDateType	TradeDate;
	///成交时间
	TThostFtdcTimeType	TradeTime;
	///成交类型
	TThostFtdcTradeTypeType	TradeType;
	///成交价来源
	TThostFtdcPriceSourceType	PriceSource;
	///交易所交易员代码
	TThostFtdcTraderIDType	TraderID;
	///本地报单编号
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///结算会员编号
	TThostFtdcParticipantIDType	ClearingPartID;
	///业务单元
	TThostFtdcBusinessUnitType	BusinessUnit;
	///序号
	TThostFtdcSequenceNoType	SequenceNo;
	///交易日
	TThostFtdcDateType	TradingDay;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;
	///经纪公司报单编号
	TThostFtdcSequenceNoType	BrokerOrderSeq;
	///成交来源
	TThostFtdcTradeSourceType	TradeSource;


	//账号
	CommonAccountType szAccount;

	//佣金
	double TradeCommission;

    double                  PriceTick;                      //从InstrumentInfo中取出

	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;


	PlatformStru_TradeInfo()
	{
		Clear();
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		TradeCommission=DOUBLEINVALIDVALUE;
		FTID=0;
		UpdateSeq=0;
	}
	char* tostring(char*prlt,int rltsize) const;

};

// 成交统计结构
struct PlatformStru_TradeTotalInfo
{
	//账号
	CommonAccountType szAccount;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///买卖方向
	TThostFtdcDirectionType		Direction;
	///开平标志
	TThostFtdcOffsetFlagType	OffsetFlag;

	double price;
	int volume;
	//佣金
	double TradeCommission;

	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;

	PlatformStru_TradeTotalInfo()
	{
		Clear();
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		FTID=0;
		UpdateSeq=0;
	}
	char* tostring(char*prlt,int rltsize) const;
};


///报单信息
struct PlatformStru_OrderInfo
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///报单引用
	TThostFtdcOrderRefType	OrderRef;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///报单价格条件
	TThostFtdcOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TThostFtdcDirectionType	Direction;
	///组合开平标志
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///价格
	TThostFtdcPriceType	LimitPrice;
	///数量
	TThostFtdcVolumeType	VolumeTotalOriginal;
	///有效期类型
	TThostFtdcTimeConditionType	TimeCondition;
	///GTD日期
	TThostFtdcDateType	GTDDate;
	///成交量类型
	TThostFtdcVolumeConditionType	VolumeCondition;
	///最小成交量
	TThostFtdcVolumeType	MinVolume;
	///触发条件
	TThostFtdcContingentConditionType	ContingentCondition;
	///止损价
	TThostFtdcPriceType	StopPrice;
	///强平原因
	TThostFtdcForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TThostFtdcBoolType	IsAutoSuspend;
	///业务单元
	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///本地报单编号
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///会员代码
	TThostFtdcParticipantIDType	ParticipantID;
	///客户代码
	TThostFtdcClientIDType	ClientID;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///交易所交易员代码
	TThostFtdcTraderIDType	TraderID;
	///安装编号
	TThostFtdcInstallIDType	InstallID;
	///报单提交状态
	TThostFtdcOrderSubmitStatusType	OrderSubmitStatus;
	///报单提示序号
	TThostFtdcSequenceNoType	NotifySequence;
	///交易日
	TThostFtdcDateType	TradingDay;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;
	///报单编号
	TThostFtdcOrderSysIDType	OrderSysID;
	///报单来源
	TThostFtdcOrderSourceType	OrderSource;
	///报单状态
	TThostFtdcOrderStatusType	OrderStatus;
	///报单类型
	TThostFtdcOrderTypeType	OrderType;
	///今成交数量
	TThostFtdcVolumeType	VolumeTraded;
	///剩余数量
	TThostFtdcVolumeType	VolumeTotal;
	///报单日期
	TThostFtdcDateType	InsertDate;
	///委托时间
	TThostFtdcTimeType	InsertTime;
	///激活时间
	TThostFtdcTimeType	ActiveTime;
	///挂起时间
	TThostFtdcTimeType	SuspendTime;
	///最后修改时间
	TThostFtdcTimeType	UpdateTime;
	///撤销时间
	TThostFtdcTimeType	CancelTime;
	///最后修改交易所交易员代码
	TThostFtdcTraderIDType	ActiveTraderID;
	///结算会员编号
	TThostFtdcParticipantIDType	ClearingPartID;
	///序号
	TThostFtdcSequenceNoType	SequenceNo;
	///前置编号
	TThostFtdcFrontIDType	FrontID;
	///会话编号
	TThostFtdcSessionIDType	SessionID;
	///用户端产品信息
	TThostFtdcProductInfoType	UserProductInfo;
	///状态信息
	TThostFtdcErrorMsgType	StatusMsg;
	///用户强评标志
	TThostFtdcBoolType	UserForceClose;
	///操作用户代码
	TThostFtdcUserIDType	ActiveUserID;
	///经纪公司报单编号
	TThostFtdcSequenceNoType	BrokerOrderSeq;
	///相关报单
	TThostFtdcOrderSysIDType	RelativeOrderSysID;
	//成交均价
	TThostFtdcPriceType       AvgPrice;
	//价格2
	PatsystemAccountType       Price2;

	// 通用扩展域
	CommonLocalRequestIDType	strLocalRequestID;			// 本地的请求标号
	CommonAccountType		szAccount;					// 交易账号
	CommonExchangeIDType		strExchange;				// 交易所代码

	TThostFtdcPriceType      freezeMargin;              //冻结保证金
	TThostFtdcPriceType      troubleMoney;              //冻结手续费

    double                  PriceTick;                      //从InstrumentInfo中取出

	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;         
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;

	bool bInvalid;      //表示是否无效
	static PlatformStru_OrderInfo InvalidOrderInfo;

	PlatformStru_OrderInfo()
	{
		Clear();
	}
	PlatformStru_OrderInfo(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		freezeMargin=DOUBLEINVALIDVALUE;
		troubleMoney=DOUBLEINVALIDVALUE;

		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	static PlatformStru_OrderInfo& GetInvalidRecord(void)
	{
		return InvalidOrderInfo;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}

	char* tostring(char*prlt,int rltsize) const;
};

///行情信息
struct PlatformStru_DepthMarketData
{
	///交易日
	TThostFtdcDateType	TradingDay;
	///合约代码
	InstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///最新价
	TThostFtdcPriceType	LastPrice;
	///上次结算价
	TThostFtdcPriceType	PreSettlementPrice;
	///昨收盘
	TThostFtdcPriceType	PreClosePrice;
	///昨持仓量
	TThostFtdcLargeVolumeType	PreOpenInterest;
	///今开盘
	TThostFtdcPriceType	OpenPrice;
	///最高价
	TThostFtdcPriceType	HighestPrice;
	///最低价
	TThostFtdcPriceType	LowestPrice;
	///数量
	TThostFtdcVolumeType	Volume;
	///成交金额
	TThostFtdcMoneyType	Turnover;
	///持仓量
	TThostFtdcLargeVolumeType	OpenInterest;
	///今收盘
	TThostFtdcPriceType	ClosePrice;
	///本次结算价
	TThostFtdcPriceType	SettlementPrice;
	///涨停板价
	TThostFtdcPriceType	UpperLimitPrice;
	///跌停板价
	TThostFtdcPriceType	LowerLimitPrice;
	///昨虚实度
	TThostFtdcRatioType	PreDelta;
	///今虚实度
	TThostFtdcRatioType	CurrDelta;
	///最后修改时间
	TThostFtdcTimeType	UpdateTime;
	///最后修改毫秒
	TThostFtdcMillisecType	UpdateMillisec;
	///申买价一
	TThostFtdcPriceType	BidPrice1;
	///申买量一
	TThostFtdcVolumeType	BidVolume1;
	///申卖价一
	TThostFtdcPriceType	AskPrice1;
	///申卖量一
	TThostFtdcVolumeType	AskVolume1;
	///申买价二
	TThostFtdcPriceType	BidPrice2;
	///申买量二
	TThostFtdcVolumeType	BidVolume2;
	///申卖价二
	TThostFtdcPriceType	AskPrice2;
	///申卖量二
	TThostFtdcVolumeType	AskVolume2;
	///申买价三
	TThostFtdcPriceType	BidPrice3;
	///申买量三
	TThostFtdcVolumeType	BidVolume3;
	///申卖价三
	TThostFtdcPriceType	AskPrice3;
	///申卖量三
	TThostFtdcVolumeType	AskVolume3;
	///申买价四
	TThostFtdcPriceType	BidPrice4;
	///申买量四
	TThostFtdcVolumeType	BidVolume4;
	///申卖价四
	TThostFtdcPriceType	AskPrice4;
	///申卖量四
	TThostFtdcVolumeType	AskVolume4;
	///申买价五
	TThostFtdcPriceType	BidPrice5;
	///申买量五
	TThostFtdcVolumeType	BidVolume5;
	///申卖价五
	TThostFtdcPriceType	AskPrice5;
	///申卖量五
	TThostFtdcVolumeType	AskVolume5;
	///当日均价
	TThostFtdcPriceType	AveragePrice;

	///申买价六
	TThostFtdcPriceType	BidPrice6;
	///申买量六
	TThostFtdcVolumeType	BidVolume6;
	///申卖价六
	TThostFtdcPriceType	AskPrice6;
	///申卖量六
	TThostFtdcVolumeType	AskVolume6;
	///申买价七
	TThostFtdcPriceType	BidPrice7;
	///申买量七
	TThostFtdcVolumeType	BidVolume7;
	///申卖价七
	TThostFtdcPriceType	AskPrice7;
	///申卖量七
	TThostFtdcVolumeType	AskVolume7;
	///申买价八
	TThostFtdcPriceType	BidPrice8;
	///申买量八
	TThostFtdcVolumeType	BidVolume8;
	///申卖价八
	TThostFtdcPriceType	AskPrice8;
	///申卖量八
	TThostFtdcVolumeType	AskVolume8;
	///申买价九
	TThostFtdcPriceType	BidPrice9;
	///申买量九
	TThostFtdcVolumeType	BidVolume9;
	///申卖价九
	TThostFtdcPriceType	AskPrice9;
	///申卖量九
	TThostFtdcVolumeType	AskVolume9;
	///申买价十
	TThostFtdcPriceType	BidPrice10;
	///申买量十
	TThostFtdcVolumeType	BidVolume10;
	///申卖价十
	TThostFtdcPriceType	AskPrice10;
	///申卖量十
	TThostFtdcVolumeType	AskVolume10;
	//最新量
	TThostFtdcVolumeType   NewVolume;
};

///更新用户密码信息
struct CCifcoUserPasswordUpdateField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///原来的口令
	TThostFtdcPasswordType	OldPassword;
	///新的口令
	TThostFtdcPasswordType	NewPassword;
};
struct PlatformStru_UserPasswordUpdate
{
	CCifcoUserPasswordUpdateField Thost;
};
///更新用户资金账户密码信息
struct CCifcoTradingAccountPasswordUpdateField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///原来的口令
	TThostFtdcPasswordType	OldPassword;
	///新的口令
	TThostFtdcPasswordType	NewPassword;
};
struct PlatformStru_TradingAccountPasswordUpdate
{
	CCifcoTradingAccountPasswordUpdateField Thost;
};
///录入报单信息
struct PlatformStru_InputOrder
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	InstrumentIDType	InstrumentID;
	///报单引用
	TThostFtdcOrderRefType	OrderRef;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///报单价格条件
	TThostFtdcOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TThostFtdcDirectionType	Direction;
	///组合开平标志
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///价格
	TThostFtdcPriceType	LimitPrice;
	///数量
	TThostFtdcVolumeType	VolumeTotalOriginal;
	///有效期类型
	TThostFtdcTimeConditionType	TimeCondition;
	///GTD日期
	TThostFtdcDateType	GTDDate;
	///成交量类型
	TThostFtdcVolumeConditionType	VolumeCondition;
	///最小成交量
	TThostFtdcVolumeType	MinVolume;
	///触发条件
	TThostFtdcContingentConditionType	ContingentCondition;
	///止损价
	TThostFtdcPriceType	StopPrice;
	///强平原因
	TThostFtdcForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TThostFtdcBoolType	IsAutoSuspend;
	///业务单元
	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///用户强评标志
	TThostFtdcBoolType	UserForceClose;

	// 通用扩展域
	CommonOrderClassType		OrderClass;					// 单据分类：'\000'/'N' New order, 'R' Replace

	CommonAccountType		strAccount;					// 交易账号
	CommonLocalRequestIDType	strLocalRequestID;			// 本地的请求标号
	CommonLocalRefIDType		strLocalRefID;				// 本地的引用号标号
	CommonExchangeIDType		strExchange;				// 交易所代码
};

///录入预埋单信息
struct PlatformStru_ParkedOrder
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///报单引用
	TThostFtdcOrderRefType	OrderRef;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///报单价格条件
	TThostFtdcOrderPriceTypeType	OrderPriceType;
	///买卖方向
	TThostFtdcDirectionType	Direction;
	///组合开平标志
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///组合投机套保标志
	TThostFtdcCombHedgeFlagType	CombHedgeFlag;
	///价格
	TThostFtdcPriceType	LimitPrice;
	///数量
	TThostFtdcVolumeType	VolumeTotalOriginal;
	///有效期类型
	TThostFtdcTimeConditionType	TimeCondition;
	///GTD日期
	TThostFtdcDateType	GTDDate;
	///成交量类型
	TThostFtdcVolumeConditionType	VolumeCondition;
	///最小成交量
	TThostFtdcVolumeType	MinVolume;
	///触发条件
	TThostFtdcContingentConditionType	ContingentCondition;
	///止损价
	TThostFtdcPriceType	StopPrice;
	///强平原因
	TThostFtdcForceCloseReasonType	ForceCloseReason;
	///自动挂起标志
	TThostFtdcBoolType	IsAutoSuspend;
	///业务单元
	TThostFtdcBusinessUnitType	BusinessUnit;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///用户强评标志
	TThostFtdcBoolType	UserForceClose;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///预埋报单编号
	TThostFtdcParkedOrderIDType	ParkedOrderID;
	///用户类型
	TThostFtdcUserTypeType	UserType;
	///预埋单状态
	TThostFtdcParkedOrderStatusType	Status;
	///错误代码
	TThostFtdcErrorIDType	ErrorID;
	///错误信息
	TThostFtdcErrorMsgType	ErrorMsg;

	// 通用扩展域
	CommonOrderClassType		OrderClass;					// 单据分类：'\000'/'N' New order, 'R' Replace

	CommonAccountType		strAccount;					// 交易账号
};

///录入预埋撤单信息
struct CCifcoParkedOrderActionField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///报单操作引用
	TThostFtdcOrderActionRefType	OrderActionRef;
	///报单引用
	TThostFtdcOrderRefType	OrderRef;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///前置编号
	TThostFtdcFrontIDType	FrontID;
	///会话编号
	TThostFtdcSessionIDType	SessionID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
	TThostFtdcOrderSysIDType	OrderSysID;
	///操作标志
	TThostFtdcActionFlagType	ActionFlag;
	///价格
	TThostFtdcPriceType	LimitPrice;
	///数量变化
	TThostFtdcVolumeType	VolumeChange;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///预埋撤单单编号
	TThostFtdcParkedOrderActionIDType	ParkedOrderActionID;
	///用户类型
	TThostFtdcUserTypeType	UserType;
	///预埋撤单状态
	TThostFtdcParkedOrderStatusType	Status;
	///错误代码
	TThostFtdcErrorIDType	ErrorID;
	///错误信息
	TThostFtdcErrorMsgType	ErrorMsg;
};
struct PlatformStru_ParkedOrderAction
{
	CCifcoParkedOrderActionField Thost;
};
///报单操作信息
struct CCifcoInputOrderActionField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///报单操作引用
	TThostFtdcOrderActionRefType	OrderActionRef;
	///报单引用
	TThostFtdcOrderRefType	OrderRef;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///前置编号
	TThostFtdcFrontIDType	FrontID;
	///会话编号
	TThostFtdcSessionIDType	SessionID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
	TThostFtdcOrderSysIDType	OrderSysID;
	///操作标志
	TThostFtdcActionFlagType	ActionFlag;
	///价格
	TThostFtdcPriceType	LimitPrice;
	///数量变化
	TThostFtdcVolumeType	VolumeChange;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
};
struct PlatformStru_InputOrderAction
{
	CCifcoInputOrderActionField Thost;
};
///查询最大报单数量
struct CCifcoQueryMaxOrderVolumeField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///买卖方向
	TThostFtdcDirectionType	Direction;
	///开平标志
	TThostFtdcOffsetFlagType	OffsetFlag;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///最大允许报单数量
	TThostFtdcVolumeType	MaxVolume;
};
struct PlatformStru_QueryMaxOrderVolume
{
	CCifcoQueryMaxOrderVolumeField Thost;
};
///请求删除预埋单
struct CCifcoRemoveParkedOrderField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///预埋报单编号
	TThostFtdcParkedOrderIDType	ParkedOrderID;
};
struct PlatformStru_RemoveParkedOrder
{
	CCifcoRemoveParkedOrderField Thost;
};
///请求删除预埋撤单
struct CCifcoRemoveParkedOrderActionField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///预埋撤单编号
	TThostFtdcParkedOrderActionIDType	ParkedOrderActionID;
};
struct PlatformStru_RemoveParkedOrderAction
{
	CCifcoRemoveParkedOrderActionField Thost;
};
///请求查询报单
struct CCifcoQryOrderField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///报单编号
	TThostFtdcOrderSysIDType	OrderSysID;
	///开始时间
	TThostFtdcTimeType	InsertTimeStart;
	///结束时间
	TThostFtdcTimeType	InsertTimeEnd;
};
struct PlatformStru_QryOrder
{
	CCifcoQryOrderField Thost;
};
///请求查询成交
struct CCifcoQryTradeField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///成交编号
	TThostFtdcTradeIDType	TradeID;
	///开始时间
	TThostFtdcTimeType	TradeTimeStart;
	///结束时间
	TThostFtdcTimeType	TradeTimeEnd;
};
struct PlatformStru_QryTrade
{
	PlatformStru_QryTrade()
	{
		memset(this,0,sizeof(*this));
	};
	CCifcoQryTradeField Thost;
};
///请求查询投资者持仓
struct PlatformStru_QryInvestorPosition
{
	PlatformStru_QryInvestorPosition()
	{
		memset(this,0,sizeof(*this));
	}

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
};


//持仓明细
struct PlatformStru_PositionDetail
{
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///买卖
	TThostFtdcDirectionType	Direction;
	///开仓日期
	TThostFtdcDateType	OpenDate;
	///成交编号
	TThostFtdcTradeIDType	TradeID;
	///数量 外盘净仓位
	TThostFtdcVolumeType	Volume;
	///开仓价
	TThostFtdcPriceType	OpenPrice;
	///交易日
	TThostFtdcDateType	TradingDay;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;
	///成交类型
	TThostFtdcTradeTypeType	TradeType;
	///组合合约代码
	TThostFtdcInstrumentIDType	CombInstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///逐日盯市平仓盈亏
	TThostFtdcMoneyType	CloseProfitByDate;
	///逐笔对冲平仓盈亏
	TThostFtdcMoneyType	CloseProfitByTrade;
	///逐日盯市持仓盈亏
	TThostFtdcMoneyType	PositionProfitByDate;
	///逐笔对冲持仓盈亏
	TThostFtdcMoneyType	PositionProfitByTrade;
	///投资者保证金
	TThostFtdcMoneyType	Margin;
	///交易所保证金
	TThostFtdcMoneyType	ExchMargin;
	///保证金率
	TThostFtdcRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TThostFtdcRatioType	MarginRateByVolume;
	///昨结算价
	TThostFtdcPriceType	LastSettlementPrice;
	///结算价
	TThostFtdcPriceType	SettlementPrice;
	///平仓量
	TThostFtdcVolumeType	CloseVolume;
	///平仓金额
	TThostFtdcMoneyType	CloseAmount;



	//买量
	int nBuyVolumn;
	//买量
	int nSellVolumn;
	//净持仓均价
	double dOpenAvgPrice;
	//净持仓收益
	double dOpenProfit;
	//总收益
	double dTotalProfit;
	//保证金每手数
	double dMarginPerLot;
	//资金消耗率
	double dPLBurnRate;
	//未平仓合约风险
	double dOpenPositionExposure;
	//剩余购买力
	double dBuyPowerRemain;
	//已用资金
	double dBuyPowerUsed;
	//交付保证金
	double dTotalMarginPaid;
	//账号
	CommonAccountType szAccount;

	double      CurPrice;                   //现价


	//计算持仓盈亏用的现价
	double      CurPriceUsedByCalcu;                   

    double                  PriceTick;                      //从InstrumentInfo中取出
    bool                    bToday;                         //是否今日持仓明细

	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;



	bool bInvalid;      //表示是否无效
	static PlatformStru_PositionDetail InvalidPositionDetail;

	PlatformStru_PositionDetail()
	{
		Clear();
	}
	PlatformStru_PositionDetail(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}

	void Clear()
	{
		ZeroMemory(this, sizeof(PlatformStru_PositionDetail));
		int nInvalidInt = INTINVALIDVALUE;
		double dInvalidDouble = DOUBLEINVALIDVALUE;
		Volume = nInvalidInt;
		OpenPrice = dInvalidDouble;
		SettlementID = nInvalidInt;
		CloseProfitByDate = dInvalidDouble;
		CloseProfitByTrade = dInvalidDouble;
		PositionProfitByDate = dInvalidDouble;
		PositionProfitByTrade = dInvalidDouble;
		Margin = dInvalidDouble;
		ExchMargin = dInvalidDouble;
		MarginRateByMoney = dInvalidDouble;
		MarginRateByVolume = dInvalidDouble;
		LastSettlementPrice = dInvalidDouble;
		SettlementPrice = dInvalidDouble;
		CloseVolume = nInvalidInt;
		CloseAmount = nInvalidInt;

		nBuyVolumn = nInvalidInt;
		nSellVolumn = nInvalidInt;
		dOpenAvgPrice = dInvalidDouble;
		dOpenProfit = dInvalidDouble;
		dTotalProfit = dInvalidDouble;
		dMarginPerLot = dInvalidDouble;
		dPLBurnRate = dInvalidDouble;
		dOpenPositionExposure = dInvalidDouble;
		dBuyPowerRemain = dInvalidDouble;
		dBuyPowerUsed = dInvalidDouble;
		dTotalMarginPaid = dInvalidDouble;
		CurPrice = dInvalidDouble;
		CurPriceUsedByCalcu = dInvalidDouble;
		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	static PlatformStru_PositionDetail& GetInvalidRecord(void)
	{
		return InvalidPositionDetail;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const;
};


//持仓
struct PlatformStru_Position
{
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///持仓多空方向
	TThostFtdcPosiDirectionType	PosiDirection;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///持仓日期类型
	TThostFtdcPositionDateType	PositionDate;
	///交易日
	TThostFtdcDateType	TradingDay;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;

	///当前净持仓
	TThostFtdcVolumeType	Position;
	///今日持仓(指今日净持仓)
	TThostFtdcVolumeType	TodayPosition;
	///上日持仓(指历史持仓总和，包括今日平掉的部分)
	TThostFtdcVolumeType	YdPosition;

	///开仓量(指今日开仓量)
	TThostFtdcVolumeType	OpenVolume;
	///平仓量(指今日平仓量)
	TThostFtdcVolumeType	CloseVolume;
	///开仓金额(指今开仓金额)
	TThostFtdcMoneyType	OpenAmount;
	///平仓金额(指今平仓金额)
	TThostFtdcMoneyType	CloseAmount;
	///持仓成本(当前净持仓的持仓成本)
	TThostFtdcMoneyType	PositionCost;
	///开仓成本(未平持仓明细的开仓金额的总和(只计算未平部分))
	TThostFtdcMoneyType	OpenCost;

	///多头冻结(多头持仓: 开买冻结；空头持仓：平买冻结)
	TThostFtdcVolumeType	LongFrozen;
	///空头冻结(多头持仓: 平卖冻结；空头持仓：开卖冻结)
	TThostFtdcVolumeType	ShortFrozen;
	///多头冻结金额
	TThostFtdcMoneyType	LongFrozenAmount;
	///空头冻结金额
	TThostFtdcMoneyType	ShortFrozenAmount;
	///冻结的保证金
	TThostFtdcMoneyType	FrozenMargin;
	///冻结的手续费
	TThostFtdcMoneyType	FrozenCommission;
	///冻结的资金
	TThostFtdcMoneyType	FrozenCash;

	///手续费
	TThostFtdcMoneyType	Commission;
	///上次占用的保证金
	TThostFtdcMoneyType	PreMargin;
	///占用的保证金
	TThostFtdcMoneyType	UseMargin;
	///交易所保证金
	TThostFtdcMoneyType	ExchangeMargin;
	///保证金率
	TThostFtdcRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TThostFtdcRatioType	MarginRateByVolume;

	///资金差额
	TThostFtdcMoneyType	CashIn;

	///持仓盈亏(盯市)
	TThostFtdcMoneyType	PositionProfit;
	///平仓盈亏(盯市)
	TThostFtdcMoneyType	CloseProfit;
	///逐日盯市平仓盈亏
	TThostFtdcMoneyType	CloseProfitByDate;
	///逐笔对冲平仓盈亏
	TThostFtdcMoneyType	CloseProfitByTrade;

	///上次结算价
	TThostFtdcPriceType	PreSettlementPrice;
	///本次结算价
	TThostFtdcPriceType	SettlementPrice;

	///组合成交形成的持仓
	TThostFtdcVolumeType	CombPosition;
	///组合多头冻结
	TThostFtdcVolumeType	CombLongFrozen;
	///组合空头冻结
	TThostFtdcVolumeType	CombShortFrozen;

	CommonAccountType szAccount;		//对应账号
	TThostFtdcExchangeIDType   ExchangeID;  //交易所代码
	double PositionProfitByTrade;			//浮动盈亏，所有持仓明细浮盈之和，就是逐笔对冲的持仓盈亏
	double TotalPositionProfitByDate;		//盯市总盈亏，盯市平仓盈亏和持仓盈亏之和

	int         TotalCloseOrderVolume;      //总平仓挂单量
	int         CloseOrderVolume;           //平仓挂单量，指平仓单，区别于平今和平昨
    int         CloseOrderVolumeTdPart;     //平仓挂单量的今仓部分
    int         CloseOrderVolumeYdPart;     //平仓挂单量的昨仓部分
	int         CloseTodayOrderVolume;      //平今挂单量
	int         CloseYesterdayOrderVolume;  //平昨挂单量
	int         CanCloseVolume;             //可平量Position-TotalCloseOrderVolume
	int         CanCloseTodayVolume;        //可平今量TodayPosition-CloseTodayOrderVolume
	int         CanCloseydVolume;           //可平昨量YdPosition-CloseYesterdayOrderVolume

	double      AveragePositionPrice;       //持仓均价(持仓成本/持仓量)
	double      AverageOpenPrice;           //开仓均价((今日开仓成本+历史开仓成本)/总开仓量)

	int         TodayCloseYdVolume;         //今平历史仓量
	int         YdPositionRemain;	        //上日剩余仓位，从持仓明细计算

	double      CurPrice;                   //现价

	//计算持仓盈亏用的现价
	double      CurPriceUsedByCalcu;  

    double                  PriceTick;                      //从InstrumentInfo中取出
    CommonCurrencyType      Currency;                       //货币名称


	// 以下是Pats特有的项
	//买量
	int pats_BuyVolumn;
	//买量
	int pats_SellVolumn;
	//净持仓均价
	double pats_OpenAvgPrice;
	//净持仓收益
	double pats_OpenProfit;
	//总收益
	double pats_TotalProfit;
	//保证金每手数
	double pats_MarginPerLot;
	//资金消耗率
	double pats_PLBurnRate;
	//未平仓合约风险
	double pats_OpenPositionExposure;
	//剩余购买力
	double pats_BuyPowerRemain;
	//已用资金
	double pats_BuyPowerUsed;
	//交付保证金
	double pats_TotalMarginPaid;
	// Pats项到此为止


	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;


	bool bInvalid;      //表示是否无效
	static PlatformStru_Position InvalidPosition;

	PlatformStru_Position()
	{
		Clear();
	}
	PlatformStru_Position(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}

	void Clear()
	{
		ZeroMemory(this, sizeof(PlatformStru_Position));
		int nInvalidInt = INTINVALIDVALUE;
		double dInvalidDouble = DOUBLEINVALIDVALUE;

		YdPosition=nInvalidInt;
		Position=nInvalidInt;
		LongFrozen=nInvalidInt;
		ShortFrozen=nInvalidInt;
		LongFrozenAmount=dInvalidDouble;
		ShortFrozenAmount=dInvalidDouble;
		OpenVolume=nInvalidInt;
		CloseVolume=nInvalidInt;
		OpenAmount=dInvalidDouble;
		CloseAmount=dInvalidDouble;
		PositionCost=dInvalidDouble;
		PreMargin=dInvalidDouble;
		UseMargin=dInvalidDouble;
		FrozenMargin=dInvalidDouble;
		FrozenCash=dInvalidDouble;
		FrozenCommission=dInvalidDouble;
		CashIn=dInvalidDouble;
		Commission=dInvalidDouble;
		CloseProfit=dInvalidDouble;
		PositionProfit=dInvalidDouble;
		PreSettlementPrice=dInvalidDouble;
		SettlementPrice=dInvalidDouble;
		SettlementID=nInvalidInt;
		OpenCost=dInvalidDouble;
		ExchangeMargin=dInvalidDouble;
		CombPosition=nInvalidInt;
		CombLongFrozen=nInvalidInt;
		CombShortFrozen=nInvalidInt;
		CloseProfitByDate=dInvalidDouble;
		CloseProfitByTrade=dInvalidDouble;
		TodayPosition=nInvalidInt;
		MarginRateByMoney=dInvalidDouble;
		MarginRateByVolume=dInvalidDouble;

		PositionProfitByTrade=dInvalidDouble;
		TotalPositionProfitByDate=dInvalidDouble;
		TotalCloseOrderVolume=nInvalidInt;
		CloseOrderVolume=nInvalidInt;
		CloseOrderVolumeTdPart=nInvalidInt;
		CloseOrderVolumeYdPart=nInvalidInt;
		CloseTodayOrderVolume=nInvalidInt;
		CloseYesterdayOrderVolume=nInvalidInt;
		CanCloseVolume=nInvalidInt;
		CanCloseTodayVolume=nInvalidInt;
		CanCloseydVolume=nInvalidInt;
		AveragePositionPrice=dInvalidDouble;
		AverageOpenPrice=dInvalidDouble;
		TodayCloseYdVolume=nInvalidInt;
		YdPositionRemain = nInvalidInt;
		CurPrice=dInvalidDouble;
		CurPriceUsedByCalcu=dInvalidDouble;
		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	static PlatformStru_Position& GetInvalidRecord(void)
	{
		return InvalidPosition;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const;
};



//组合持仓明细
struct PlatformStru_PositionComb
{
	///交易日
	TThostFtdcDateType	TradingDay;
	///开仓日期
	TThostFtdcDateType	OpenDate;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///组合编号
	TThostFtdcTradeIDType	ComTradeID;

	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///买卖
	TThostFtdcDirectionType	Direction;
	//开仓价
	TThostFtdcPriceType	OpenPrice;
	///持仓量
	TThostFtdcVolumeType	TotalAmt;

	//单腿1的信息
	///撮合编号
	TThostFtdcTradeIDType	Leg1TradeID;
	///合约代码
	TThostFtdcInstrumentIDType	Leg1InstrumentID;
	///投资者保证金
	TThostFtdcMoneyType	Leg1Margin;
	///交易所保证金
	TThostFtdcMoneyType	Leg1ExchMargin;
	///保证金率
	TThostFtdcRatioType	Leg1MarginRateByMoney;
	///保证金率(按手数)
	TThostFtdcRatioType	Leg1MarginRateByVolume;
	///单腿编号
	TThostFtdcLegIDType	Leg1ID;
	///单腿乘数
	TThostFtdcLegMultipleType	Leg1Multiple;

	//单腿2的信息
	///撮合编号
	TThostFtdcTradeIDType	Leg2TradeID;
	///合约代码
	TThostFtdcInstrumentIDType	Leg2InstrumentID;
	///投资者保证金
	TThostFtdcMoneyType	Leg2Margin;
	///交易所保证金
	TThostFtdcMoneyType	Leg2ExchMargin;
	///保证金率
	TThostFtdcRatioType	Leg2MarginRateByMoney;
	///保证金率(按手数)
	TThostFtdcRatioType	Leg2MarginRateByVolume;
	///单腿编号
	TThostFtdcLegIDType	Leg2ID;
	///单腿乘数
	TThostFtdcLegMultipleType	Leg2Multiple;

	///组合持仓合约编码
	TThostFtdcInstrumentIDType	InstrumentID;

	CommonAccountType szAccount;	//对应账号

	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;


	bool bInvalid;      //表示是否无效
	static PlatformStru_PositionComb InvalidPositionDetailComb;

	PlatformStru_PositionComb()
	{
		Clear();
	}
	PlatformStru_PositionComb(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}

	void Clear()
	{
		ZeroMemory(this, sizeof(PlatformStru_PositionComb));
		int nInvalidInt = INTINVALIDVALUE;
		double dInvalidDouble = DOUBLEINVALIDVALUE;
		TotalAmt = nInvalidInt;

		Leg1Margin = dInvalidDouble;
		Leg1ExchMargin = dInvalidDouble;
		Leg1MarginRateByMoney = dInvalidDouble;
		Leg1MarginRateByVolume = dInvalidDouble;
		Leg1Multiple = nInvalidInt;

		Leg2Margin = dInvalidDouble;
		Leg2ExchMargin = dInvalidDouble;
		Leg2MarginRateByMoney = dInvalidDouble;
		Leg2MarginRateByVolume = dInvalidDouble;
		Leg2Multiple = nInvalidInt;

		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	static PlatformStru_PositionComb& GetInvalidRecord(void)
	{
		return InvalidPositionDetailComb;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const;
};


///请求查询资金账户信息
struct PlatformStru_QryTradingAccount
{
	PlatformStru_QryTradingAccount()
	{
		memset(this,0,sizeof(*this));
	};

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
};

///请求查询投资者
struct CCifcoQryInvestorField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
};
struct PlatformStru_QryInvestor
{
	CCifcoQryInvestorField Thost;
};
///请求查询合约
struct CCifcoQryInstrumentField
{
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///产品代码
	TThostFtdcInstrumentIDType	ProductID;
};
struct PlatformStru_QryInstrument
{
	CCifcoQryInstrumentField Thost;
};
///请求查询行情
struct CCifcoQryDepthMarketDataField
{
	TThostFtdcInstrumentIDType	InstrumentID;
};
struct PlatformStru_QryDepthMarketData
{
	CCifcoQryDepthMarketDataField Thost;
};
///请求查询投资者结算结果
struct CCifcoQrySettlementInfoField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///交易日
	TThostFtdcDateType	TradingDay;
};
struct PlatformStru_QrySettlementInfo
{
	CCifcoQrySettlementInfoField Thost;
};
///请求查询投资者持仓明细
struct PlatformStru_QryInvestorPositionDetail
{
	PlatformStru_QryInvestorPositionDetail()
	{
		memset(this,0,sizeof(*this));
	}

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
};

///请求查询投资者持仓明细(组合)
struct PlatformStru_QryInvestorPositionCombineDetail
{
	PlatformStru_QryInvestorPositionCombineDetail()
	{
		memset(this,0,sizeof(*this));
	}

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///组合持仓合约编码
	TThostFtdcInstrumentIDType	CombInstrumentID;
};

///请求查询客户通知
struct CCifcoQryNoticeField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
};
struct PlatformStru_QryNotice
{
	CCifcoQryNoticeField Thost;
};
///请求查询结算信息确认
struct CCifcoQrySettlementInfoConfirmField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
};
struct PlatformStru_QrySettlementInfoConfirm
{
	CCifcoQrySettlementInfoConfirmField Thost;
};

///请求查询保证金监管系统经纪公司资金账户密钥
struct CCifcoQryCFMMCTradingAccountKeyField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
};
///请求查询保证金监管系统经纪公司资金账户密钥
struct PlatformStru_QryCFMMCTradingAccountKey
{
	CCifcoQryCFMMCTradingAccountKeyField Thost;
};
///请求查询转账银行
struct CCifcoQryTransferBankField
{
	///银行代码
	TThostFtdcBankIDType	BankID;
	///银行分中心代码
	TThostFtdcBankBrchIDType	BankBrchID;
};
struct PlatformStru_QryTransferBank
{
	CCifcoQryTransferBankField Thost;
};
///请求查询转账流水
struct CCifcoQryTransferSerialField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///银行编码
	TThostFtdcBankIDType	BankID;
};
struct PlatformStru_QryTransferSerial
{
	CCifcoQryTransferSerialField Thost;
};
///请求查询签约银行
struct CCifcoQryContractBankField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///银行代码
	TThostFtdcBankIDType	BankID;
	///银行分中心代码
	TThostFtdcBankBrchIDType	BankBrchID;
};
struct PlatformStru_QryContractBank
{
	CCifcoQryContractBankField Thost;
};
///请求查询银期签约关系
struct CCifcoQryAccountregisterField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///银行编码
	TThostFtdcBankIDType	BankID;
};
struct PlatformStru_QryAccountRegister
{
	CCifcoQryAccountregisterField Thost;
};
///请求查询预埋单
struct CCifcoQryParkedOrderField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
};
///请求查询预埋单
struct PlatformStru_QryParkedOrder
{
	CCifcoQryParkedOrderField Thost;
};
///请求查询预埋撤单
struct CCifcoQryParkedOrderActionField
{
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
};
struct PlatformStru_QryParkedOrderAction
{
	CCifcoQryParkedOrderActionField Thost;
};
///期货发起查询银行余额请求
struct CCifcoReqQueryAccountField
{
	///业务功能码
	TThostFtdcTradeCodeType	TradeCode;
	///银行代码
	TThostFtdcBankIDType	BankID;
	///银行分支机构代码
	TThostFtdcBankBrchIDType	BankBranchID;
	///期商代码
	TThostFtdcBrokerIDType	BrokerID;
	///期商分支机构代码
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///交易日期
	TThostFtdcTradeDateType	TradeDate;
	///交易时间
	TThostFtdcTradeTimeType	TradeTime;
	///银行流水号
	TThostFtdcBankSerialType	BankSerial;
	///交易系统日期 
	TThostFtdcTradeDateType	TradingDay;
	///银期平台消息流水号
	TThostFtdcSerialType	PlateSerial;
	///最后分片标志
	TThostFtdcLastFragmentType	LastFragment;
	///会话号
	TThostFtdcSessionIDType	SessionID;
	///客户姓名
	TThostFtdcIndividualNameType	CustomerName;
	///证件类型
	TThostFtdcIdCardTypeType	IdCardType;
	///证件号码
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///客户类型
	TThostFtdcCustTypeType	CustType;
	///银行帐号
	TThostFtdcBankAccountType	BankAccount;
	///银行密码
	TThostFtdcPasswordType	BankPassWord;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///期货密码
	TThostFtdcPasswordType	Password;
	///期货公司流水号
	TThostFtdcFutureSerialType	FutureSerial;
	///安装编号
	TThostFtdcInstallIDType	InstallID;
	///用户标识
	TThostFtdcUserIDType	UserID;
	///验证客户证件号码标志
	TThostFtdcYesNoIndicatorType	VerifyCertNoFlag;
	///币种代码
	TThostFtdcCurrencyIDType	CurrencyID;
	///摘要
	TThostFtdcDigestType	Digest;
	///银行帐号类型
	TThostFtdcBankAccTypeType	BankAccType;
	///渠道标志
	TThostFtdcDeviceIDType	DeviceID;
	///期货单位帐号类型
	TThostFtdcBankAccTypeType	BankSecuAccType;
	///期货公司银行编码
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///期货单位帐号
	TThostFtdcBankAccountType	BankSecuAcc;
	///银行密码标志
	TThostFtdcPwdFlagType	BankPwdFlag;
	///期货资金密码核对标志
	TThostFtdcPwdFlagType	SecuPwdFlag;
	///交易柜员
	TThostFtdcOperNoType	OperNo;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///交易ID
	TThostFtdcTIDType	TID;
};

struct PlatformStru_ReqQueryAccount
{
	CCifcoReqQueryAccountField Thost;
};
///期货发起银行资金转期货请求
struct  CCifcoReqTransferField 
{
	///业务功能码
	TThostFtdcTradeCodeType	TradeCode;
	///银行代码
	TThostFtdcBankIDType	BankID;
	///银行分支机构代码
	TThostFtdcBankBrchIDType	BankBranchID;
	///期商代码
	TThostFtdcBrokerIDType	BrokerID;
	///期商分支机构代码
	TThostFtdcFutureBranchIDType	BrokerBranchID;
	///交易日期
	TThostFtdcTradeDateType	TradeDate;
	///交易时间
	TThostFtdcTradeTimeType	TradeTime;
	///银行流水号
	TThostFtdcBankSerialType	BankSerial;
	///交易系统日期 
	TThostFtdcTradeDateType	TradingDay;
	///银期平台消息流水号
	TThostFtdcSerialType	PlateSerial;
	///最后分片标志
	TThostFtdcLastFragmentType	LastFragment;
	///会话号
	TThostFtdcSessionIDType	SessionID;
	///客户姓名
	TThostFtdcIndividualNameType	CustomerName;
	///证件类型
	TThostFtdcIdCardTypeType	IdCardType;
	///证件号码
	TThostFtdcIdentifiedCardNoType	IdentifiedCardNo;
	///客户类型
	TThostFtdcCustTypeType	CustType;
	///银行帐号
	TThostFtdcBankAccountType	BankAccount;
	///银行密码
	TThostFtdcPasswordType	BankPassWord;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///期货密码
	TThostFtdcPasswordType	Password;
	///安装编号
	TThostFtdcInstallIDType	InstallID;
	///期货公司流水号
	TThostFtdcFutureSerialType	FutureSerial;
	///用户标识
	TThostFtdcUserIDType	UserID;
	///验证客户证件号码标志
	TThostFtdcYesNoIndicatorType	VerifyCertNoFlag;
	///币种代码
	TThostFtdcCurrencyIDType	CurrencyID;
	///转帐金额
	TThostFtdcTradeAmountType	TradeAmount;
	///期货可取金额
	TThostFtdcTradeAmountType	FutureFetchAmount;
	///费用支付标志
	TThostFtdcFeePayFlagType	FeePayFlag;
	///应收客户费用
	TThostFtdcCustFeeType	CustFee;
	///应收期货公司费用
	TThostFtdcFutureFeeType	BrokerFee;
	///发送方给接收方的消息
	TThostFtdcAddInfoType	Message;
	///摘要
	TThostFtdcDigestType	Digest;
	///银行帐号类型
	TThostFtdcBankAccTypeType	BankAccType;
	///渠道标志
	TThostFtdcDeviceIDType	DeviceID;
	///期货单位帐号类型
	TThostFtdcBankAccTypeType	BankSecuAccType;
	///期货公司银行编码
	TThostFtdcBankCodingForFutureType	BrokerIDByBank;
	///期货单位帐号
	TThostFtdcBankAccountType	BankSecuAcc;
	///银行密码标志
	TThostFtdcPwdFlagType	BankPwdFlag;
	///期货资金密码核对标志
	TThostFtdcPwdFlagType	SecuPwdFlag;
	///交易柜员
	TThostFtdcOperNoType	OperNo;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///交易ID
	TThostFtdcTIDType	TID;
	///转账交易状态
	TThostFtdcTransferStatusType	TransferStatus;
};
struct PlatformStru_ReqTransfer
{
	CCifcoReqTransferField Thost;
};
//账号信息
struct PlatformStru_Trader
{
	char TraderAccount[21];
	char BackOfficeID[21];
	CHAR Tradable;
	INT LossLimit;
};
//交易所信息
struct PlatformStru_Exchange
{
	char Name[11];
	CHAR QueryEnabled;
	CHAR AmendEnabled;
	INT Strategy;
	CHAR CustomDecs;
	INT Decimals;
	CHAR TicketType;
	CHAR RFQA;
	CHAR RFQT;
	CHAR EnableBlock;
	CHAR EnableBasis;
	CHAR EnableAA;
	CHAR EnableCross;
	INT GTStatus;

};
//合约信息
typedef CHAR Array10[11];
typedef Array10 LegStruct[5];
struct PlatformStru_Contract
{
	char  ContractName[11];
	char ContractDate[51];
	char ExchangeName[11];
	char ExpiryDate[9];
	char LastTradeDate[9];
	INT NumberOfLegs;
	INT TicksPerPoint;
	char TickSize[11];
	CHAR Tradable;
	INT GTStatus;
	char Margin[21];
	CHAR ESATemplate;
	char MarketRef[17];
	char lnExchangeName[11];
	char lnContractName[11];
	char lnContractDate[51];
	LegStruct ExternalID[33];
};
//商品信息
struct PlatformStru_Commodity
{
	Array10 ExchangeName;
	Array10 ContractName;
	Array10 Currency;
	Array10 Group;
	Array10 OnePoint;
	INT TicksPerPoint;
	Array10 TickSize;
	INT GTStatus;
};
//报单类型
struct PlatformStru_OrderType
{
	Array10 OrderType;
	Array10 ExchangeName;
	INT OrderTypeID;
	BYTE NumPricesReqd;
	BYTE NumVolumesReqd;
	BYTE NumDatesReqd;
	CHAR AutoCreated;
	CHAR TimeTriggered;
	CHAR RealSynthetic;
	CHAR GTCFlag;
	char TicketType[3];
	CHAR PatsOrderType;
	INT AmendOTCount;
	char AlgoXML[51];
	char AmendTypesArray[501];
};
//签约银行信息
struct PlatformStru_ContractBank
{
	///银行编码
	TThostFtdcBankIDType	BankID;
	///银行名称
	TThostFtdcBankNameType	BankName;
	///银行帐号
	TThostFtdcBankAccountType	BankAccount;
	///币种代码
	TThostFtdcCurrencyIDType	CurrencyID;
	///银行分支机构编码
	TThostFtdcBankBrchIDType	BankBranchID;
	///期货公司编码
	TThostFtdcBrokerIDType	BrokerID;
};

#pragma pack(push)
#pragma pack(1)
struct PlatformStru_AccountPosition
{
	CHAR Profit[21];
	INT Buys;
	INT Sells;
};
struct PlatformStru_MessageStruct
{    
	char MsgID[11];
	char MsgText[501];
	char IsAlert;
	char Status;
};
#pragma pack(pop)
struct ConnectivityStatus
{
	char DeviceType[3];
	char ExchangeID[20];
	char DeviceName[20];
	char Commentary[255];
	char Status[3];
	char Severity[3];
};

struct PlusinAcctInstrumentKey
{
	CommonAccountType szAccountID;			//账号
	CommonAccountType szSubAccountID;		//子账号
	CommonInstrumentIDType szInstrumentID;	//合约ID
};

struct PlusinAcctInstrumentLmtPriceKey
{
	CommonAccountType szAccountID;			//账号
	CommonAccountType szSubAccountID;		//子账号
	CommonInstrumentIDType szInstrumentID;	//合约ID
	double dLimitPrice;						//限价
};

struct PlusinOpenOrderKey
{
	CommonAccountType szAccountID;			//账号
	CommonAccountType szSubAccountID;		//子账号
	CommonInstrumentIDType szInstrumentID;	//合约ID
	char cDirection;						//方向
	double dLimitPrice;						//限价
};

struct PlusinOpenOrderNumRsp
{
	PlusinOpenOrderKey queryCondition;
	int nNum;
};

struct ExchangeCurTime
{
	DWORD dwTickCount;
	char szExchangeTime[32];
};

struct WaitOrderPrice
{
	double dHighPrice;
	double dLowPrice;

	WaitOrderPrice()
	{
		dHighPrice = dLowPrice = DOUBLEINVALIDVALUE;
	}
};

///响应信息
struct CCifcoRspInfoField
{
    ///错误代码
    TThostFtdcErrorIDType	ErrorID;
    ///错误信息
    TThostFtdcErrorMsgType	ErrorMsg;
};

struct PlatformStru_RspOrderAction1
{
	CCifcoInputOrderActionField InputOrderActionField;
	CCifcoRspInfoField     RspInfoField; 
};

struct PlatformStru_RspOrderAction2
{
	CCifcoInputOrderActionField OrderActionField;
	CCifcoRspInfoField     RspInfoField;  
};

struct PlatformStru_RspOrderInsert
{
	CCifcoInputOrderActionField InputOrderInsertField;
	CCifcoRspInfoField     RspInfoField; 
};