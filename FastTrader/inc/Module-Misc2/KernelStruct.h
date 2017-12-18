#pragma once

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include "tools_util.h"
#include "Tools_Ansi.h"
#include <sys/timeb.h>

#include <string>
#include <vector>

#ifdef WIN32
#pragma managed(push,off)
#endif 

#include "../ctp/ThostFtdcUserApiStruct.h"

#ifndef CTP060300
#define CTP060300
#endif

///强平(机构版风控手动强平和自动强平)
#define THOST_FTDC_FCC_ForceClose 'Z'

#define DOUBLEINVALIDVALUE	(util::GetDoubleInvalidValue())
#define INTINVALIDVALUE		(util::GetIntInvalidValue())

//当执行一些查询命令时，可能查询结果为空，
//但综合交易平台的API依然认为此查询成功，
//我们将这种情况"纠正"为查询失败，所以各GUI
//子模块需处理这个特殊的ErrorID
#define CustomErrorID 123456789
#define CustomErrorMsg "查询无记录"
#define ORDER_INSERT_FRONTID  -1
#define ORDER_INSERT_SESSIONID -1
#define ORDER_INSERT_ORDERSTATUS 'N'

#define ORDERCLASS_NewOrder 'N'
#define ORDERCLASS_ReplaceOrder 'R'

#define THOST_FTDC_OST_ERROE  'd'

typedef char CommonAccountIDType[13];

typedef char CommonOrderClassType;
typedef char CommonUserIDType[32];                  //用户名ID，对应综合交易平台投资者ID、Pats的UserName
typedef char CommonAccountType[32];                 //账号
typedef char CommonLocalRequestIDType[32];
typedef char CommonLocalRefIDType[32];
typedef char CommonExchangeIDType[32];
typedef char CommonExchangeNameType[32];            //交易所名称
typedef char CommonCurrencyType[16];                //货币名称
typedef char CommonOrderRefType[32];                //报单的OrderRef
typedef char CommonOrderSysIDType[32];              //报单的OrderSysID
typedef char CommonInstrumentIDType[32];            //合约ID
typedef char CommonDateType[16];                    //日期
typedef char CommonTradeIDType[21];                 //成交ID

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
typedef double TThostFtdcUnderlyingMultipleType;


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


///登录信息
struct PlatformStru_LoginInfo
{
	///交易日
	TThostFtdcDateType	TradingDay;
	///登录成功时间
	TThostFtdcTimeType	LoginTime;
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///交易系统名称
	TThostFtdcSystemNameType	SystemName;
	///前置编号
	TThostFtdcFrontIDType	FrontID;
	///会话编号
	TThostFtdcSessionIDType	SessionID;
	///最大报单引用
	TThostFtdcOrderRefType	MaxOrderRef;
	///上期所时间
	TThostFtdcTimeType	SHFETime;
	///大商所时间
	TThostFtdcTimeType	DCETime;
	///郑商所时间
	TThostFtdcTimeType	CZCETime;
	///中金所时间
	TThostFtdcTimeType	FFEXTime;
	///能源中心时间
	TThostFtdcTimeType	INETime;

    //登录成功时的本地时间，也就是设置此结构内容的时间
    struct timeb        LoginTimeLocal;


    PlatformStru_LoginInfo()
    {
        clear();
    }
    PlatformStru_LoginInfo(const CThostFtdcRspUserLoginField& r)
    {
        clear();

        strncpy(TradingDay,r.TradingDay,sizeof(TradingDay));
        strncpy(LoginTime,r.LoginTime,sizeof(LoginTime));
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID));
        strncpy(UserID,r.UserID,sizeof(UserID));
        strncpy(SystemName,r.SystemName,sizeof(SystemName));
        FrontID=r.FrontID;
        SessionID=r.SessionID;
        strncpy(MaxOrderRef,r.MaxOrderRef,sizeof(MaxOrderRef));
        strncpy(SHFETime,r.SHFETime,sizeof(SHFETime));
        strncpy(DCETime,r.DCETime,sizeof(DCETime));
        strncpy(CZCETime,r.CZCETime,sizeof(CZCETime));
        strncpy(FFEXTime,r.FFEXTime,sizeof(FFEXTime));
        strncpy(INETime,r.INETime,sizeof(INETime));

        ftime(&LoginTimeLocal);
    }
    void clear()
    {
        memset(this,0,sizeof(*this));
    }
};

///经纪公司交易参数信息
struct PlatformStru_BrokerTradingParams
{
	PlatformStru_BrokerTradingParams(void)
	{
		clear();
	}
	PlatformStru_BrokerTradingParams(const CThostFtdcBrokerTradingParamsField& r)
	{
		clear();

        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        MarginPriceType=r.MarginPriceType;
        Algorithm=r.Algorithm;
        AvailIncludeCloseProfit=r.AvailIncludeCloseProfit;
        strncpy(CurrencyID,r.CurrencyID,sizeof(CurrencyID)-1);
#ifdef CTP060300
        OptionRoyaltyPriceType=r.OptionRoyaltyPriceType;
#endif
	}
    void clear(void)
    {
        memset(this,0,sizeof(*this));
    };

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///保证金价格类型
	TThostFtdcMarginPriceTypeType	MarginPriceType;
	///盈亏算法
	TThostFtdcAlgorithmType	Algorithm;
	///可用是否包含平仓盈利
	TThostFtdcIncludeCloseProfitType	AvailIncludeCloseProfit;
	///币种代码
	TThostFtdcCurrencyIDType	CurrencyID;
	///期权权利金价格类型
	TThostFtdcOptionRoyaltyPriceTypeType	OptionRoyaltyPriceType;
};


///产品信息
struct PlatformStru_ProductInfo
{
	PlatformStru_ProductInfo(void)
	{
		clear();
	}
	PlatformStru_ProductInfo(const CThostFtdcProductField& r)
	{
		clear();

        strncpy(ProductID,r.ProductID,sizeof(ProductID)-1);
        strncpy(ProductName,r.ProductName,sizeof(ProductName)-1);
        strncpy(ExchangeID,r.ExchangeID,sizeof(ExchangeID)-1);
        ProductClass=r.ProductClass;
        VolumeMultiple=r.VolumeMultiple;
        PriceTick=r.PriceTick;
        MaxMarketOrderVolume=r.MaxMarketOrderVolume;
        MinMarketOrderVolume=r.MinMarketOrderVolume;
        MaxLimitOrderVolume=r.MaxLimitOrderVolume;
        MinLimitOrderVolume=r.MinLimitOrderVolume;
        PositionType=r.PositionType;
        PositionDateType=r.PositionDateType;
        CloseDealType=r.CloseDealType;
        strncpy(TradeCurrencyID,r.TradeCurrencyID,sizeof(TradeCurrencyID)-1);
        MortgageFundUseRange=r.MortgageFundUseRange;
#ifdef CTP060300
        strncpy(ExchangeProductID,r.ExchangeProductID,sizeof(ExchangeProductID)-1);
        UnderlyingMultiple=r.UnderlyingMultiple;
#endif
        strncpy(ProductShortName,r.ProductName,sizeof(ProductShortName)-1);
	}
    void clear(void)
    {
        memset(this,0,sizeof(*this));
    };

    ///产品代码
	TThostFtdcInstrumentIDType	ProductID;
	///产品名称
	TThostFtdcProductNameType	ProductName;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///产品类型
	TThostFtdcProductClassType	ProductClass;
	///合约数量乘数
	TThostFtdcVolumeMultipleType	VolumeMultiple;
	///最小变动价位
	TThostFtdcPriceType	PriceTick;
	///市价单最大下单量
	TThostFtdcVolumeType	MaxMarketOrderVolume;
	///市价单最小下单量
	TThostFtdcVolumeType	MinMarketOrderVolume;
	///限价单最大下单量
	TThostFtdcVolumeType	MaxLimitOrderVolume;
	///限价单最小下单量
	TThostFtdcVolumeType	MinLimitOrderVolume;
	///持仓类型
	TThostFtdcPositionTypeType	PositionType;
	///持仓日期类型
	TThostFtdcPositionDateTypeType	PositionDateType;
	///平仓处理类型
	TThostFtdcCloseDealTypeType	CloseDealType;
	///交易币种类型
	TThostFtdcCurrencyIDType	TradeCurrencyID;
	///质押资金可用范围
	TThostFtdcMortgageFundUseRangeType	MortgageFundUseRange;
	///交易所产品代码
	TThostFtdcInstrumentIDType	ExchangeProductID;
	///合约基础商品乘数
	TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;

    ///产品简称，缺省情况下等于产品名称，产品名称太长时自定义
    TThostFtdcProductNameType   ProductShortName;
};

///合约信息
struct PlatformStru_InstrumentInfo
{
	PlatformStru_InstrumentInfo(void)
	{
		clear();
	}
	PlatformStru_InstrumentInfo(const CThostFtdcInstrumentField& r)
	{
		clear();

		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(ExchangeID, r.ExchangeID,sizeof(ExchangeID)-1);
		strncpy(InstrumentName, r.InstrumentName,sizeof(InstrumentName)-1);
		strncpy(ExchangeInstID, r.ExchangeInstID,sizeof(ExchangeInstID)-1);
		strncpy(ProductID, r.ProductID,sizeof(ProductID)-1);
		ProductClass = r.ProductClass;
		DeliveryYear = r.DeliveryYear;
		DeliveryMonth = r.DeliveryMonth;
		MaxMarketOrderVolume = r.MaxMarketOrderVolume;
		MinMarketOrderVolume = r.MinMarketOrderVolume;
		MaxLimitOrderVolume = r.MaxLimitOrderVolume;
		MinLimitOrderVolume = r.MinLimitOrderVolume;
		VolumeMultiple = r.VolumeMultiple;
		PriceTick = r.PriceTick;
		strncpy(CreateDate,r.CreateDate,sizeof(CreateDate)-1);
		strncpy(OpenDate,r.OpenDate,sizeof(OpenDate)-1);
		strncpy(ExpireDate, r.ExpireDate,sizeof(ExpireDate)-1);
		strncpy(StartDelivDate, r.StartDelivDate,sizeof(StartDelivDate)-1);
		strncpy(EndDelivDate, r.EndDelivDate,sizeof(EndDelivDate)-1);
		InstLifePhase = r.InstLifePhase;
		IsTrading = r.IsTrading;
		PositionType = r.PositionType;
		PositionDateType = r.PositionType;
		LongMarginRatio = r.LongMarginRatio;
		ShortMarginRatio = r.ShortMarginRatio;
        MaxMarginSideAlgorithm=r.MaxMarginSideAlgorithm;

#ifdef CTP060300
		strncpy(UnderlyingInstrID, r.UnderlyingInstrID,sizeof(UnderlyingInstrID)-1);
		StrikePrice = r.StrikePrice;
		OptionsType = r.OptionsType;
		UnderlyingMultiple = r.UnderlyingMultiple;
		CombinationType = r.CombinationType;
#endif

	}
    void clear(void)
    {
        memset(this,0,sizeof(*this));
    };

	///合约代码
	InstrumentIDType	        InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///合约名称
	InstrumentIDType	        InstrumentName;
	///合约在交易所的代码
	TThostFtdcExchangeInstIDType	ExchangeInstID;
	///产品代码
	InstrumentIDType	        ProductID;
	///产品类型
	TThostFtdcProductClassType	ProductClass;
	///交割年份
	TThostFtdcYearType	        DeliveryYear;
	///交割月
	TThostFtdcMonthType	        DeliveryMonth;
	///市价单最大下单量
	TThostFtdcVolumeType	    MaxMarketOrderVolume;
	///市价单最小下单量
	TThostFtdcVolumeType	    MinMarketOrderVolume;
	///限价单最大下单量
	TThostFtdcVolumeType	    MaxLimitOrderVolume;
	///限价单最小下单量
	TThostFtdcVolumeType	    MinLimitOrderVolume;
	///合约数量乘数
	TThostFtdcVolumeMultipleType	VolumeMultiple;
	///最小变动价位
	TThostFtdcPriceType	        PriceTick;
	///创建日
	TThostFtdcDateType	        CreateDate;
	///上市日
	TThostFtdcDateType	        OpenDate;
	///到期日
	TThostFtdcDateType	        ExpireDate;
	///开始交割日
	TThostFtdcDateType	        StartDelivDate;
	///结束交割日
	TThostFtdcDateType	        EndDelivDate;
	///合约生命周期状态
	TThostFtdcInstLifePhaseType	InstLifePhase;
	///当日是否交易
	TThostFtdcBoolType	        IsTrading;
	///持仓类型
	TThostFtdcPositionTypeType	PositionType;
	///持仓日期类型
	TThostFtdcPositionDateTypeType	PositionDateType;
	///多头保证金率
	TThostFtdcRatioType	        LongMarginRatio;
	///空头保证金率
	TThostFtdcRatioType	        ShortMarginRatio;
    ///是否使用大额单边保证金算法(上期所套利规则：同一客户在同一会员处的同品种双向持仓直接按照保证金金额较大的一边收取交易保证金；对交割月份合约在最后5个交易日不参加单向大边折抵)
   	TThostFtdcMaxMarginSideAlgorithmType	MaxMarginSideAlgorithm;

    ///下面是ctp API 6.3.0加入的内容
    ///基础商品代码
	TThostFtdcInstrumentIDType	UnderlyingInstrID;
	///执行价
	TThostFtdcPriceType	StrikePrice;
	///期权类型
	TThostFtdcOptionsTypeType	OptionsType;
	///合约基础商品乘数
	TThostFtdcUnderlyingMultipleType	UnderlyingMultiple;
	///组合类型
	TThostFtdcCombinationTypeType	CombinationType;



    ///下面是自己根据配置生成的项
    ///组合保证金优惠方法：0-不优惠(缺省)，1-第一腿优惠，2-第二腿优惠，3-按套利组合中高腿收取，4-大额单边保证金收取(上期)
    int                         CombMarginDiscountMode;
    ///平仓顺序: 0: 统一先开先平；1: 先单一后组合先开先平
    int                         UnwindPriorities;
    ///计算今仓保证金使用的价格: 0: 按成交价计算(缺省)；1: 按昨结算价计算
    int                         PriceForMarginOfTodayPosition;
    ///对平今指令的支持，0:不支持(但ctp可能会转换成平仓)；1:支持
    int                         CloseTodayInstructionSupport;
    ///对平仓指令的支持，0:不支持；1:支持，可以平昨仓和今仓；2:支持，只能平昨仓
    int                         CloseInstructionSupport;

	char Currency [11];
	int  TicksPerPoint;
	char TickSize[10];
};

///手续费信息
struct PlatformStru_InstrumentCommissionRate
{
    PlatformStru_InstrumentCommissionRate()
    {
        clear();
    };
    PlatformStru_InstrumentCommissionRate(const CThostFtdcInstrumentCommissionRateField& r)
    {
        clear();

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        InvestorRange=r.InvestorRange;
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        OpenRatioByMoney=r.OpenRatioByMoney;
        OpenRatioByVolume=r.OpenRatioByVolume;
        CloseRatioByMoney=r.CloseRatioByMoney;
        CloseRatioByVolume=r.CloseRatioByVolume;
        CloseTodayRatioByMoney=r.CloseTodayRatioByMoney;
        CloseTodayRatioByVolume=r.CloseTodayRatioByVolume;
    };
#ifdef CTP060300
    PlatformStru_InstrumentCommissionRate(const CThostFtdcOptionInstrCommRateField& r)
    {
        clear();

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        InvestorRange=r.InvestorRange;
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        OpenRatioByMoney=r.OpenRatioByMoney;
        OpenRatioByVolume=r.OpenRatioByVolume;
        CloseRatioByMoney=r.CloseRatioByMoney;
        CloseRatioByVolume=r.CloseRatioByVolume;
        CloseTodayRatioByMoney=r.CloseTodayRatioByMoney;
        CloseTodayRatioByVolume=r.CloseTodayRatioByVolume;
        StrikeRatioByMoney=r.StrikeRatioByMoney;
        StrikeRatioByVolume=r.StrikeRatioByVolume;
    };
#endif
    void clear(void)
    {
        memset(this,0,sizeof(*this));
        OpenRatioByMoney=util::GetDoubleInvalidValue();
        OpenRatioByVolume=util::GetDoubleInvalidValue();
        CloseRatioByMoney=util::GetDoubleInvalidValue();
        CloseRatioByVolume=util::GetDoubleInvalidValue();
        CloseTodayRatioByMoney=util::GetDoubleInvalidValue();
        CloseTodayRatioByVolume=util::GetDoubleInvalidValue();
        StrikeRatioByMoney=util::GetDoubleInvalidValue();
        StrikeRatioByVolume=util::GetDoubleInvalidValue();
    };

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
    ///执行手续费率(期权)
	TThostFtdcRatioType	StrikeRatioByMoney;
	///执行手续费(期权)
	TThostFtdcRatioType	StrikeRatioByVolume;
};

///保证金信息
struct PlatformStru_InstrumentMarginRate
{
    PlatformStru_InstrumentMarginRate()
    {
        clear();
    };
    PlatformStru_InstrumentMarginRate(const CThostFtdcInstrumentMarginRateField& r)
    {
        clear();

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        InvestorRange=r.InvestorRange;
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        HedgeFlag=r.HedgeFlag;
        LongMarginRatioByMoney=r.LongMarginRatioByMoney;
        LongMarginRatioByVolume=r.LongMarginRatioByVolume;
        ShortMarginRatioByMoney=r.ShortMarginRatioByMoney;
        ShortMarginRatioByVolume=r.ShortMarginRatioByVolume;
        IsRelative=r.IsRelative;
    };
#ifdef CTP060300
    PlatformStru_InstrumentMarginRate(const CThostFtdcOptionInstrTradeCostField& r)
    {
        clear();

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        HedgeFlag=r.HedgeFlag;
        FixedMargin=r.FixedMargin;
        MiniMargin=r.MiniMargin;
        Royalty=r.Royalty;
        ExchFixedMargin=r.ExchFixedMargin;
        ExchMiniMargin=r.ExchMiniMargin;
    };
#endif
    void clear(void)
    {
        memset(this,0,sizeof(*this));
        LongMarginRatioByMoney=util::GetDoubleInvalidValue();
        LongMarginRatioByVolume=util::GetDoubleInvalidValue();
        ShortMarginRatioByMoney=util::GetDoubleInvalidValue();
        ShortMarginRatioByVolume=util::GetDoubleInvalidValue();
        FixedMargin=util::GetDoubleInvalidValue();
        MiniMargin=util::GetDoubleInvalidValue();
        Royalty=util::GetDoubleInvalidValue();
        ExchFixedMargin=util::GetDoubleInvalidValue();
        ExchMiniMargin=util::GetDoubleInvalidValue();
    };

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

    ///下面用于期权保证金计算
	///期权合约保证金不变部分
	TThostFtdcMoneyType	FixedMargin;
	///期权合约最小保证金
	TThostFtdcMoneyType	MiniMargin;
	///期权合约权利金
	TThostFtdcMoneyType	Royalty;
	///交易所期权合约保证金不变部分
	TThostFtdcMoneyType	ExchFixedMargin;
	///交易所期权合约最小保证金
	TThostFtdcMoneyType	ExchMiniMargin;
};

///期权交易成本
struct PlatformStru_OptionInstrTradeCost
{
    PlatformStru_OptionInstrTradeCost()
    {
        clear();
    };
#ifdef CTP060300
    PlatformStru_OptionInstrTradeCost(const CThostFtdcOptionInstrTradeCostField& r)
    {
        strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
        HedgeFlag=r.HedgeFlag;
        FixedMargin=r.FixedMargin;
        MiniMargin=r.MiniMargin;
        Royalty=r.Royalty;
        ExchFixedMargin=r.ExchFixedMargin;
        ExchMiniMargin=r.ExchMiniMargin;
    };
#endif
    void clear(void)
    {
        memset(this,0,sizeof(*this));
        FixedMargin=util::GetDoubleInvalidValue();
        MiniMargin=util::GetDoubleInvalidValue();
        Royalty=util::GetDoubleInvalidValue();
        ExchFixedMargin=util::GetDoubleInvalidValue();
        ExchMiniMargin=util::GetDoubleInvalidValue();
    };

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///期权合约保证金不变部分
	TThostFtdcMoneyType	FixedMargin;
	///期权合约最小保证金
	TThostFtdcMoneyType	MiniMargin;
	///期权合约权利金
	TThostFtdcMoneyType	Royalty;
	///交易所期权合约保证金不变部分
	TThostFtdcMoneyType	ExchFixedMargin;
	///交易所期权合约最小保证金
	TThostFtdcMoneyType	ExchMiniMargin;
};

///交易账号信息
struct PlatformStru_TradingAccountInfo
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

    PlatformStru_TradingAccountInfo()
    {
        clear();

		const double dInvalidDouble = util::GetDoubleInvalidValue();

	    PreMortgage = dInvalidDouble;
	    PreCredit = dInvalidDouble;
	    PreDeposit = dInvalidDouble;
	    PreBalance = dInvalidDouble;
	    PreMargin = dInvalidDouble;
	    InterestBase = dInvalidDouble;
	    Interest = dInvalidDouble;
	    Deposit = dInvalidDouble;
	    Withdraw = dInvalidDouble;
	    FrozenMargin = dInvalidDouble;
	    FrozenCash = dInvalidDouble;
	    FrozenCommission = dInvalidDouble;
	    CurrMargin = dInvalidDouble;
	    CashIn = dInvalidDouble;
	    Commission = dInvalidDouble;
	    CloseProfit = dInvalidDouble;
	    PositionProfit = dInvalidDouble;
	    Balance = dInvalidDouble;
	    Available = dInvalidDouble;
	    WithdrawQuota = dInvalidDouble;
	    Reserve = dInvalidDouble;

	    Credit = dInvalidDouble;
	    Mortgage = dInvalidDouble;
	    ExchangeMargin = dInvalidDouble;
	    DeliveryMargin = dInvalidDouble;
	    ExchangeDeliveryMargin = dInvalidDouble;
	    StaticProfit = dInvalidDouble;
	    DynamicProfit = dInvalidDouble;
	    RiskDegree = dInvalidDouble;
    };
    void clear(void)
    {
        memset(this,0,sizeof(*this));
    };
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
	CommonAccountType       Account;


    double                  PriceTick;                      //从InstrumentInfo中取出

    //平今量。平仓成交有效。需要根据持仓明细来计算。成交回报、查询成交结束、查询持仓明细结束都会(在持仓明细中)尝试计算平今量，上述情况和查询费率结束会计算手续费，
    int                     CloseTodayVolume;
	//手续费。考虑平今优惠的问题，需要明确平今量后才能计算手续费
	double                  TradeCommission;

    ///逐日盯市平仓盈亏。对于查询得到的成交记录，平仓盈亏为无效值。对于成交回报得到的成交记录，平仓盈亏在更新持仓明细后得到。用于机构版服务器端结算
	double                  CloseProfitByDate;
	///逐笔对冲平仓盈亏。注意事项同上
	double                  CloseProfitByTrade;


	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;


	PlatformStru_TradeInfo()
	{
		Clear();
	}
	PlatformStru_TradeInfo(const CThostFtdcTradeField& r)
	{
		Clear();

		strncpy(BrokerID, r.BrokerID,sizeof(BrokerID)-1);
		strncpy(InvestorID, r.InvestorID,sizeof(InvestorID)-1);
		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(OrderRef, r.OrderRef,sizeof(OrderRef)-1);
		strncpy(UserID, r.UserID,sizeof(UserID)-1);
		strncpy(ExchangeID, r.ExchangeID,sizeof(ExchangeID)-1);

        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),r.TradeID);

        Direction = r.Direction;

        strncpy(OrderSysID,r.OrderSysID,sizeof(OrderSysID)-1);

		strncpy(ParticipantID, r.ParticipantID,sizeof(ParticipantID)-1);
		strncpy(ClientID, r.ClientID,sizeof(ClientID)-1);
		TradingRole = r.TradingRole;
		strncpy(ExchangeInstID, r.ExchangeInstID,sizeof(ExchangeInstID)-1);
		OffsetFlag = r.OffsetFlag;
		HedgeFlag = r.HedgeFlag;
		Price = r.Price;
		Volume = r.Volume;
		strncpy(TradeDate, r.TradeDate,sizeof(TradeDate)-1);
		strncpy(TradeTime, r.TradeTime,sizeof(TradeTime)-1);
		TradeType = r.TradeType;
		PriceSource = r.PriceSource;
		strncpy(TraderID, r.TraderID,sizeof(TraderID)-1);
		strncpy(OrderLocalID, r.OrderLocalID,sizeof(OrderLocalID)-1);
		strncpy(ClearingPartID, r.ClearingPartID,sizeof(ClearingPartID)-1);
		strncpy(BusinessUnit, BusinessUnit,sizeof(BusinessUnit)-1);
		SequenceNo = r.SequenceNo;
		strncpy(TradingDay, r.TradingDay,sizeof(TradingDay)-1);
		SettlementID = r.SettlementID;
		BrokerOrderSeq = r.BrokerOrderSeq;
		TradeSource = r.TradeSource;

		strncpy(Account,r.InvestorID,sizeof(Account)-1);

        if(OffsetFlag==THOST_FTDC_OF_Open) CloseTodayVolume=0;
        else if(OffsetFlag==THOST_FTDC_OF_CloseToday) CloseTodayVolume=Volume;
        else CloseTodayVolume=util::GetIntInvalidValue();
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		TradeCommission=DOUBLEINVALIDVALUE;
        CloseTodayVolume=util::GetIntInvalidValue();
        CloseProfitByDate=DOUBLEINVALIDVALUE;
        CloseProfitByTrade=DOUBLEINVALIDVALUE;
		FTID=0;
		UpdateSeq=0;
	}
    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,%s,Direct%d",
            Account,InstrumentID,TradeDate,TradeID,OrderSysID,Direction);
        prlt[len]=0;
        return prlt;
    }

};

///成交信息
struct PlatformStru_TradeInfoBrief
{
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///成交编号
	TThostFtdcTradeIDType	TradeID;
	///买卖方向
	TThostFtdcDirectionType	Direction;
	///报单编号
	TThostFtdcOrderSysIDType	OrderSysID;
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
	///交易日
	TThostFtdcDateType	TradingDay;
	//账号
	CommonAccountType       Account;

	PlatformStru_TradeInfoBrief()
	{
		Clear();
	}
	PlatformStru_TradeInfoBrief(const PlatformStru_TradeInfo& r)
	{
		Clear();

		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),r.TradeID);
        Direction = r.Direction;
        strncpy(OrderSysID,r.OrderSysID,sizeof(OrderSysID)-1);
		OffsetFlag = r.OffsetFlag;
		HedgeFlag = r.HedgeFlag;
		Price = r.Price;
		Volume = r.Volume;
		strncpy(TradeDate, r.TradeDate,sizeof(TradeDate)-1);
		strncpy(TradeTime, r.TradeTime,sizeof(TradeTime)-1);
		TradeType = r.TradeType;
		strncpy(TradingDay, r.TradingDay,sizeof(TradingDay)-1);
		strncpy(Account,r.InvestorID,sizeof(Account)-1);
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
	}
    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,%s,Direct%d",
            Account,InstrumentID,TradeDate,TradeID,OrderSysID,Direction);
        prlt[len]=0;
        return prlt;
    }
};


// 成交统计结构
struct PlatformStru_TradeTotalInfo
{
	//账号
	CommonAccountType           Account;
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
	char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,Direct%d,Offset%d",
            Account,ExchangeID,InstrumentID,Direction,OffsetFlag);
        prlt[len]=0;
        return prlt;
    }
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
	CommonLocalRequestIDType	strLocalRequestID;		// 本地的请求标号
	CommonAccountType		    Account;				// 交易账号
	CommonExchangeIDType		strExchange;			// 交易所代码

	double         freezeMargin;           //冻结保证金
	double         freezeExchMargin;       //交易所冻结保证金，用于计算套利保证金优惠
	double         troubleMoney;           //冻结手续费

	double         Leg1FreezeMargin;       //组合合约报单的第一腿冻结保证金
	double         Leg1FreezeExchMargin;   //组合合约报单的第一腿冻结交易所保证金
	double         Leg1TroubleMoney;       //组合合约报单的第一腿冻结手续费
	double         Leg2FreezeMargin;       //组合合约报单的第二腿冻结保证金
	double         Leg2FreezeExchMargin;   //组合合约报单的第二腿冻结交易所保证金
	double         Leg2TroubleMoney;       //组合合约报单的第二腿冻结手续费

    double                      PriceTick;              //从InstrumentInfo中取出

	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;         
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;

	bool bInvalid;      //表示是否无效
	//static PlatformStru_OrderInfo InvalidOrderInfo;


    enum EnumExStatus
    {
        ExSta_none=0,
        ExSta_approving=1       //报单正在手工审核中，此时OrderStatus等于"未知"
    };
    EnumExStatus ExStatus;      //服务端使用的状态


	PlatformStru_OrderInfo()
	{
		Clear();
		bInvalid=true;
	}
	PlatformStru_OrderInfo(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
	PlatformStru_OrderInfo(const CThostFtdcOrderField& r)
	{
		Clear();

		strncpy(BrokerID, r.BrokerID,sizeof(BrokerID)-1);
		strncpy(InvestorID, r.InvestorID,sizeof(InvestorID)-1);
		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(OrderRef, r.OrderRef,sizeof(OrderRef)-1);
		strncpy(UserID, r.UserID,sizeof(UserID)-1);
		OrderPriceType = r.OrderPriceType;
		Direction = r.Direction;
		CombOffsetFlag[0] = r.CombOffsetFlag[0];
		strncpy(CombHedgeFlag, r.CombHedgeFlag, sizeof(CombHedgeFlag)-1);
		LimitPrice = r.LimitPrice;
		VolumeTotalOriginal = r.VolumeTotalOriginal;
		TimeCondition = r.TimeCondition;
		strncpy(GTDDate, r.GTDDate, sizeof(GTDDate)-1);
		VolumeCondition = r.VolumeCondition;
		MinVolume = r.MinVolume;
		ContingentCondition = r.ContingentCondition;
		StopPrice = r.StopPrice;
		ForceCloseReason = r.ForceCloseReason;
		IsAutoSuspend = r.IsAutoSuspend;
		strncpy(BusinessUnit, r.BusinessUnit, sizeof(BusinessUnit)-1);
		RequestID = r.RequestID;
		strncpy(OrderLocalID, r.OrderLocalID, sizeof(OrderLocalID)-1);
		strncpy(ExchangeID, r.ExchangeID, sizeof(ExchangeID)-1);
		strncpy(ParticipantID, r.ParticipantID, sizeof(ParticipantID)-1);
		strncpy(ClientID, r.ClientID, sizeof(ClientID)-1);
		strncpy(ExchangeInstID, r.ExchangeInstID, sizeof(ExchangeInstID)-1);
		strncpy(TraderID, r.TraderID, sizeof(TraderID)-1);
		InstallID = r.InstallID;
		OrderSubmitStatus = r.OrderSubmitStatus;
		NotifySequence = r.NotifySequence;
		strncpy(TradingDay, r.TradingDay, sizeof(TradingDay)-1);
		SettlementID = r.SettlementID;
        strncpy(OrderSysID,r.OrderSysID,sizeof(OrderSysID)-1);
		OrderSource = r.OrderSource;
		OrderStatus = r.OrderStatus;
		ExStatus = PlatformStru_OrderInfo::ExSta_none;
		OrderType = r.OrderType;
		VolumeTraded = r.VolumeTraded;
		VolumeTotal = r.VolumeTotal;
		strncpy(InsertDate, r.InsertDate, sizeof(InsertDate)-1);
		strncpy(InsertTime, r.InsertTime, sizeof(InsertTime)-1);
		strncpy(ActiveTime, r.ActiveTime, sizeof(ActiveTime)-1);
		strncpy(SuspendTime, r.SuspendTime, sizeof(SuspendTime)-1);
		strncpy(UpdateTime, r.UpdateTime, sizeof(UpdateTime)-1);
		strncpy(CancelTime, r.CancelTime, sizeof(CancelTime)-1);
		strncpy(ActiveTraderID, r.ActiveTraderID, sizeof(ActiveTraderID)-1);
		strncpy(ClearingPartID, r.ClearingPartID, sizeof(ClearingPartID)-1);
		SequenceNo = r.SequenceNo;
		FrontID = r.FrontID;
		SessionID = r.SessionID;
		strncpy(UserProductInfo, r.UserProductInfo, sizeof(UserProductInfo)-1);
		strncpy(StatusMsg, r.StatusMsg, sizeof(StatusMsg)-1);
		UserForceClose = r.UserForceClose;
		strncpy(ActiveUserID, r.ActiveUserID, sizeof(ActiveUserID)-1);
		BrokerOrderSeq = r.BrokerOrderSeq;
		strncpy(RelativeOrderSysID, r.RelativeOrderSysID, sizeof(RelativeOrderSysID)-1);

		strncpy(Account,r.InvestorID,sizeof(Account)-1);
        bInvalid=false;
	}
	void Clear(void)
	{
		memset(this,0,sizeof(*this));
		freezeMargin=DOUBLEINVALIDVALUE;
		freezeExchMargin=DOUBLEINVALIDVALUE;
		troubleMoney=DOUBLEINVALIDVALUE;
        Leg1FreezeMargin=DOUBLEINVALIDVALUE;
        Leg1FreezeExchMargin=DOUBLEINVALIDVALUE;
        Leg1TroubleMoney=DOUBLEINVALIDVALUE;
        Leg2FreezeMargin=DOUBLEINVALIDVALUE;
        Leg2FreezeExchMargin=DOUBLEINVALIDVALUE;
        Leg2TroubleMoney=DOUBLEINVALIDVALUE;

		FTID=0;
		UpdateSeq=0;
		bInvalid=true;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}

	char* tostring(char*prlt,int rltsize) const
    {
        if(bInvalid&&prlt&&rltsize>0) 
        {
            strncpy(prlt,"Invalid",rltsize);
            prlt[rltsize-1]=0;
            return prlt;
        }

        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,FrontID%d,SessionID%d,%s",
            Account,InstrumentID,FrontID,SessionID,OrderRef);
        prlt[len]=0;
        return prlt;
    }
};

///行情信息
struct PlatformStru_DepthMarketData
{
    PlatformStru_DepthMarketData()
    {
        clear();
    }
	PlatformStru_DepthMarketData(const CThostFtdcDepthMarketDataField& r)
	{
		clear();

		strncpy(TradingDay, r.TradingDay,sizeof(TradingDay)-1);
		strncpy(InstrumentID, r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(ExchangeID, r.ExchangeID,sizeof(ExchangeID)-1);
		strncpy(ExchangeInstID, r.ExchangeInstID,sizeof(ExchangeInstID)-1);
		LastPrice = r.LastPrice;
		PreSettlementPrice = r.PreSettlementPrice;
		PreClosePrice = r.PreClosePrice;
		PreOpenInterest = r.PreOpenInterest;
		OpenPrice = r.OpenPrice;
		HighestPrice = r.HighestPrice;
		LowestPrice = r.LowestPrice;
		Volume = r.Volume;
		Turnover = r.Turnover;
		OpenInterest = r.OpenInterest;
		ClosePrice = r.ClosePrice;
		SettlementPrice = r.SettlementPrice;
		UpperLimitPrice = r.UpperLimitPrice;
		LowerLimitPrice = r.LowerLimitPrice;
		PreDelta = r.PreDelta;
		CurrDelta = r.CurrDelta;
		strcpy(UpdateTime, r.UpdateTime);
		UpdateMillisec = r.UpdateMillisec;
		BidPrice1 = r.BidPrice1;
		BidVolume1 = r.BidVolume1;
		AskPrice1 = r.AskPrice1;
		AskVolume1 = r.AskVolume1;
		BidPrice2 = r.BidPrice2;
		BidVolume2 = r.BidVolume2;
		AskPrice2 = r.AskPrice2;
		AskVolume2 = r.AskVolume2;
		BidPrice3 = r.BidPrice3;
		BidVolume3 = r.BidVolume3;
		AskPrice3 = r.AskPrice3;
		AskVolume3 = r.AskVolume3;
		BidPrice4 = r.BidPrice4;
		BidVolume4 = r.BidVolume4;
		AskPrice4 = r.AskPrice4;
		AskVolume4 = r.AskVolume4;
		BidPrice5 = r.BidPrice5;
		BidVolume5 = r.BidVolume5;
		AskPrice5 = r.AskPrice5;
		AskVolume5 = r.AskVolume5;
		AveragePrice = r.AveragePrice;
	}
    void clear(void)
    {
        memset(this,0,sizeof(*this));

        LastPrice=util::GetDoubleInvalidValue();
        PreSettlementPrice=util::GetDoubleInvalidValue();
        PreClosePrice=util::GetDoubleInvalidValue();
        PreOpenInterest=util::GetDoubleInvalidValue();
        OpenPrice=util::GetDoubleInvalidValue();
        HighestPrice=util::GetDoubleInvalidValue();
        LowestPrice=util::GetDoubleInvalidValue();
        Volume=util::GetIntInvalidValue();
        Turnover=util::GetDoubleInvalidValue();
        OpenInterest=util::GetDoubleInvalidValue();
        ClosePrice=util::GetDoubleInvalidValue();
        SettlementPrice=util::GetDoubleInvalidValue();
        UpperLimitPrice=util::GetDoubleInvalidValue();
        LowerLimitPrice=util::GetDoubleInvalidValue();
        PreDelta=util::GetDoubleInvalidValue();
        CurrDelta=util::GetDoubleInvalidValue();
        BidPrice1=util::GetDoubleInvalidValue();
        BidVolume1=util::GetIntInvalidValue();
        AskPrice1=util::GetDoubleInvalidValue();
        AskVolume1=util::GetIntInvalidValue();
        BidPrice2=util::GetDoubleInvalidValue();
        BidVolume2=util::GetIntInvalidValue();
        AskPrice2=util::GetDoubleInvalidValue();
        AskVolume2=util::GetIntInvalidValue();
        BidPrice3=util::GetDoubleInvalidValue();
        BidVolume3=util::GetIntInvalidValue();
        AskPrice3=util::GetDoubleInvalidValue();
        AskVolume3=util::GetIntInvalidValue();
        BidPrice4=util::GetDoubleInvalidValue();
        BidVolume4=util::GetIntInvalidValue();
        AskPrice4=util::GetDoubleInvalidValue();
        AskVolume4=util::GetIntInvalidValue();
        BidPrice5=util::GetDoubleInvalidValue();
        BidVolume5=util::GetIntInvalidValue();
        AskPrice5=util::GetDoubleInvalidValue();
        AskVolume5=util::GetIntInvalidValue();
        AveragePrice=util::GetDoubleInvalidValue();
        BidPrice6=util::GetDoubleInvalidValue();
        BidVolume6=util::GetIntInvalidValue();
        AskPrice6=util::GetDoubleInvalidValue();
        AskVolume6=util::GetIntInvalidValue();
        BidPrice7=util::GetDoubleInvalidValue();
        BidVolume7=util::GetIntInvalidValue();
        AskPrice7=util::GetDoubleInvalidValue();
        AskVolume7=util::GetIntInvalidValue();
        BidPrice8=util::GetDoubleInvalidValue();
        BidVolume8=util::GetIntInvalidValue();
        AskPrice8=util::GetDoubleInvalidValue();
        AskVolume8=util::GetIntInvalidValue();
        BidPrice9=util::GetDoubleInvalidValue();
        BidVolume9=util::GetIntInvalidValue();
        AskPrice9=util::GetDoubleInvalidValue();
        AskVolume9=util::GetIntInvalidValue();
        BidPrice10=util::GetDoubleInvalidValue();
        BidVolume10=util::GetIntInvalidValue();
        AskPrice10=util::GetDoubleInvalidValue();
        AskVolume10=util::GetIntInvalidValue();
        NewVolume=util::GetIntInvalidValue();
    }

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

struct PlatformStru_UserPasswordUpdate
{
    PlatformStru_UserPasswordUpdate()
    {
        memset(this,0,sizeof(*this));
    }; 

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///原来的口令
	TThostFtdcPasswordType	OldPassword;
	///新的口令
	TThostFtdcPasswordType	NewPassword;
};
struct PlatformStru_TradingAccountPasswordUpdate
{
    PlatformStru_TradingAccountPasswordUpdate()
    {
        memset(this,0,sizeof(*this));
    };

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者帐号
	TThostFtdcAccountIDType	AccountID;
	///原来的口令
	TThostFtdcPasswordType	OldPassword;
	///新的口令
	TThostFtdcPasswordType	NewPassword;
	///币种代码
	TThostFtdcCurrencyIDType	CurrencyID;
};
///录入报单信息
struct PlatformStru_InputOrder
{
    //PlatformStru_InputOrder() { memset(this,0,sizeof(*this)); }

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

	CommonAccountType		    strAccount;					// 交易账号
	CommonLocalRequestIDType	strLocalRequestID;			// 本地的请求标号
	CommonLocalRefIDType		strLocalRefID;				// 本地的引用号标号
	CommonExchangeIDType		strExchange;				// 交易所代码
};

///录入预埋单信息
struct PlatformStru_ParkedOrder
{
    //PlatformStru_ParkedOrder() { memset(this,0,sizeof(*this)); }

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

struct PlatformStru_ParkedOrderAction
{
	CThostFtdcParkedOrderActionField Thost;
};
struct PlatformStru_InputOrderAction
{
    PlatformStru_InputOrderAction()
    {
        memset(this,0,sizeof(*this));
    }

	CThostFtdcInputOrderActionField Thost;
};
struct PlatformStru_QueryMaxOrderVolume
{
    PlatformStru_QueryMaxOrderVolume()
    {
        memset(this,0,sizeof(*this));
    };

	CThostFtdcQueryMaxOrderVolumeField Thost;
};
struct PlatformStru_RemoveParkedOrder
{
    PlatformStru_RemoveParkedOrder()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcRemoveParkedOrderField Thost;
};
struct PlatformStru_RemoveParkedOrderAction
{
    PlatformStru_RemoveParkedOrderAction()
    {
        memset(this,0,sizeof(*this));
    };

	CThostFtdcRemoveParkedOrderActionField Thost;
};
struct PlatformStru_QryOrder
{
    PlatformStru_QryOrder()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryOrderField Thost;
};
struct PlatformStru_QryTrade
{
	PlatformStru_QryTrade()
	{
		memset(this,0,sizeof(*this));
	};
	CThostFtdcQryTradeField Thost;
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
	//卖量
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
	CommonAccountType   Account;

	double      CurPrice;                   //现价，用于计算盈亏。优先使用的价格顺序为：结算价 > 最新价 > 昨结算价

    double              PriceTick;          //从InstrumentInfo中取出
    bool                bToday;             //是否今日持仓明细
    TThostFtdcTimeType	OpenTime;           //开仓时间。昨仓为00:00:00，今仓为对应的开仓成交时间
    int                 OpenTimeUTC;        //开仓时间，utc秒数。昨仓为开仓日零时的时间，今仓为正确时间

	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;



	bool bInvalid;      //表示是否无效

	PlatformStru_PositionDetail()
	{
		Clear();
	}
	PlatformStru_PositionDetail(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
	PlatformStru_PositionDetail(const CThostFtdcInvestorPositionDetailField& r)
	{
		Clear();

		strncpy(Account,r.InvestorID,sizeof(Account)-1);
		strncpy(InstrumentID, r.InstrumentID, sizeof(InstrumentID)-1);
		strncpy(BrokerID, r.BrokerID, sizeof(BrokerID)-1);
		strncpy(InvestorID, r.InvestorID, sizeof(InvestorID)-1);

        HedgeFlag = r.HedgeFlag;
		Direction = r.Direction;
        strncpy(OpenDate, r.OpenDate, sizeof(OpenDate)-1);

        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),r.TradeID);

        Volume = r.Volume;
		OpenPrice = r.OpenPrice;
		strncpy(TradingDay, r.TradingDay, sizeof(TradingDay)-1);
		SettlementID = r.SettlementID;
		TradeType = r.TradeType;
		strncpy(CombInstrumentID, r.CombInstrumentID, sizeof(CombInstrumentID)-1);
		strncpy(ExchangeID, r.ExchangeID, sizeof(ExchangeID)-1);

		CloseProfitByDate = r.CloseProfitByDate;
		CloseProfitByTrade = r.CloseProfitByTrade;
		PositionProfitByDate = r.PositionProfitByDate;
		PositionProfitByTrade = r.PositionProfitByTrade;

		Margin = r.Margin;
		ExchMargin = r.ExchMargin;
		MarginRateByMoney = r.MarginRateByMoney;
		MarginRateByVolume = r.MarginRateByVolume;

		LastSettlementPrice = r.LastSettlementPrice;
		SettlementPrice = r.SettlementPrice;
		CloseVolume = r.CloseVolume;
		CloseAmount = r.CloseAmount;

		bInvalid=false;
	}
    //开仓成交创建持仓明细
    PlatformStru_PositionDetail(const PlatformStru_TradeInfo& TradeInfo)
    {
        Clear();

	    strncpy(Account,TradeInfo.Account,sizeof(Account)-1);
        strncpy(InstrumentID,TradeInfo.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(BrokerID,TradeInfo.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,TradeInfo.InvestorID,sizeof(InvestorID)-1);

        HedgeFlag=TradeInfo.HedgeFlag;
        Direction=TradeInfo.Direction;
        strncpy(OpenDate,TradeInfo.TradeDate,sizeof(OpenDate)-1);
        strncpy(TradeID,TradeInfo.TradeID,sizeof(TradeID)-1);

        Volume=TradeInfo.Volume;
        OpenPrice=TradeInfo.Price;
        strncpy(TradingDay,TradeInfo.TradingDay,sizeof(TradingDay)-1);
        SettlementID=TradeInfo.SettlementID;
        TradeType=TradeInfo.TradeType;
        strncpy(ExchangeID,TradeInfo.ExchangeID,sizeof(ExchangeID)-1);

        CloseProfitByDate=0;
        CloseProfitByTrade=0;
        PositionProfitByDate=0;
        PositionProfitByTrade=0;

        Margin=util::GetDoubleInvalidValue();
        MarginRateByMoney=util::GetDoubleInvalidValue();
        MarginRateByVolume=util::GetDoubleInvalidValue();
        ExchMargin=util::GetDoubleInvalidValue();

        LastSettlementPrice=util::GetDoubleInvalidValue();
        SettlementPrice=util::GetDoubleInvalidValue();
        CloseVolume=0;
        CloseAmount=0;

        CurPrice=util::GetDoubleInvalidValue();
        PriceTick=TradeInfo.PriceTick;
        bToday=true;
        strncpy(OpenTime,TradeInfo.TradeTime,sizeof(OpenTime)-1);
        OpenTimeUTC=(int)CTools_Ansi::Get_time_t_utc_from_strTime(OpenDate,OpenTime);

        bInvalid=false;
    }
	void Clear()
	{
        memset(this,0,sizeof(*this));
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
		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const
    {
        if(bInvalid&&prlt&&rltsize>0) 
        {
            strncpy(prlt,"Invalid",rltsize);
            prlt[rltsize-1]=0;
            return prlt;
        }

        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,Direct%d,Hedge%d,TradeType%d,Volume=%d",
            Account,InstrumentID,OpenDate,TradeID,Direction,HedgeFlag,TradeType,Volume);
        prlt[len]=0;
        return prlt;
    }
};

//组合持仓明细
struct PlatformStru_PositionDetailComb
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
	///撮合编号
	TThostFtdcTradeIDType	TradeID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///买卖
	TThostFtdcDirectionType	Direction;
	///持仓量
	TThostFtdcVolumeType	TotalAmt;
	///投资者保证金
	TThostFtdcMoneyType	Margin;
	///交易所保证金
	TThostFtdcMoneyType	ExchMargin;
	///保证金率
	TThostFtdcRatioType	MarginRateByMoney;
	///保证金率(按手数)
	TThostFtdcRatioType	MarginRateByVolume;
    ///单腿编号(整型值，0/1)
	TThostFtdcLegIDType	LegID;
    ///单腿乘数(1)
	TThostFtdcLegMultipleType	LegMultiple;
	///组合持仓合约编码
	TThostFtdcInstrumentIDType	CombInstrumentID;

	CommonAccountType   Account;	//对应账号
    bool                bToday;             //是否今日组合持仓明细

	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;


	bool bInvalid;      //表示是否无效
	//static PlatformStru_PositionDetailComb InvalidPositionDetailComb;

	PlatformStru_PositionDetailComb()
	{
		Clear();
	}
	PlatformStru_PositionDetailComb(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
    //用一条单腿成交，构造一条组合成交明细
    PlatformStru_PositionDetailComb(const PlatformStru_TradeInfo &TradeInfo,bool btoday,const std::string& comTradeID,const std::string& strCombInstrumentID,int legID)
	{
		Clear();

	    strncpy(Account, TradeInfo.Account, sizeof(Account)-1);
	    strncpy(InstrumentID, TradeInfo.InstrumentID, sizeof(InstrumentID)-1);

	    strncpy(TradingDay, TradeInfo.TradingDay, sizeof(TradingDay)-1);
	    strncpy(OpenDate, TradeInfo.TradingDay, sizeof(OpenDate)-1);
	    strncpy(ExchangeID, TradeInfo.ExchangeID, sizeof(ExchangeID)-1);
	    SettlementID = TradeInfo.SettlementID;
	    strncpy(BrokerID, TradeInfo.BrokerID, sizeof(BrokerID)-1);
	    strncpy(InvestorID, TradeInfo.InvestorID, sizeof(InvestorID)-1);

        strncpy(ComTradeID, comTradeID.c_str(), sizeof(ComTradeID)-1);
        strncpy(TradeID, TradeInfo.TradeID, sizeof(TradeID)-1);

        HedgeFlag = TradeInfo.HedgeFlag;
        Direction = TradeInfo.Direction;
        TotalAmt = TradeInfo.Volume;

        Margin = util::GetDoubleInvalidValue();
        ExchMargin = util::GetDoubleInvalidValue();
        MarginRateByMoney = util::GetDoubleInvalidValue();
        MarginRateByVolume = util::GetDoubleInvalidValue();

        LegID = legID;
        LegMultiple = 1;
        strncpy(CombInstrumentID, strCombInstrumentID.c_str(), sizeof(CombInstrumentID)-1);

        bToday=btoday;

        bInvalid=false;
	}
	PlatformStru_PositionDetailComb(const CThostFtdcInvestorPositionCombineDetailField& r)
	{
		Clear();

		strncpy(TradingDay,r.TradingDay,sizeof(TradingDay)-1);
		strncpy(OpenDate,r.OpenDate,sizeof(OpenDate)-1);
		strncpy(ExchangeID,r.ExchangeID,sizeof(ExchangeID)-1);
		SettlementID=r.SettlementID;
		strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
		strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);

        CTools_Ansi::mymemcpy_trim(ComTradeID,sizeof(ComTradeID),r.ComTradeID);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),r.TradeID);

        strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
		HedgeFlag=r.HedgeFlag;
		Direction=r.Direction;
		TotalAmt=r.TotalAmt;
		Margin=r.Margin;
		ExchMargin=r.ExchMargin;
		MarginRateByMoney=r.MarginRateByMoney;
		MarginRateByVolume=r.MarginRateByVolume;
		LegID=r.LegID;
		LegMultiple=r.LegMultiple;
		strncpy(CombInstrumentID,r.CombInstrumentID,sizeof(CombInstrumentID)-1);

		strncpy(Account,r.InvestorID,sizeof(Account)-1);
		bInvalid=false;
	}

	void Clear()
	{
		memset(this, 0,sizeof(*this));
		int nInvalidInt = INTINVALIDVALUE;
		double dInvalidDouble = DOUBLEINVALIDVALUE;
		
        TotalAmt = nInvalidInt;
		Margin = dInvalidDouble;
		ExchMargin = dInvalidDouble;
		MarginRateByMoney = dInvalidDouble;
		MarginRateByVolume = dInvalidDouble;
        LegID = nInvalidInt;
		LegMultiple = nInvalidInt;

        bToday=false;

		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const
    {
        if(bInvalid&&prlt&&rltsize>0) 
        {
            strncpy(prlt,"Invalid",rltsize);
            prlt[rltsize-1]=0;
            return prlt;
        }

        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,Direct%d,Hedge%d,TotalAmt=%d",
            Account,InstrumentID,OpenDate,TradeID,Direction,HedgeFlag,TotalAmt);
        prlt[len]=0;
        return prlt;
    }
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
	TThostFtdcVolumeType	    CombPosition;
	///组合多头冻结
	TThostFtdcVolumeType	    CombLongFrozen;
	///组合空头冻结
	TThostFtdcVolumeType	    CombShortFrozen;

	CommonAccountType           Account;		            //对应账号
	TThostFtdcExchangeIDType    ExchangeID;                 //交易所代码
	double                      PositionProfitByTrade;		//浮动盈亏，所有持仓明细浮盈之和，就是逐笔对冲的持仓盈亏
	double                      TotalPositionProfitByDate;	//盯市总盈亏，盯市平仓盈亏和持仓盈亏之和

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

	double      CurPrice;                   //当前用于计算盈亏的价格 结算价/最新价/昨结算价

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
	//static PlatformStru_Position InvalidPosition;

	PlatformStru_Position()
	{
		Clear();
	}
	PlatformStru_Position(bool binvalid)
	{
		Clear();
		bInvalid=binvalid;
	}
	PlatformStru_Position(const CThostFtdcInvestorPositionField& r)
	{
		Clear();

		strncpy(InstrumentID,r.InstrumentID,sizeof(InstrumentID)-1);
		strncpy(BrokerID,r.BrokerID,sizeof(BrokerID)-1);
		strncpy(InvestorID,r.InvestorID,sizeof(InvestorID)-1);
		PosiDirection=r.PosiDirection;
		HedgeFlag=r.HedgeFlag;
		PositionDate=r.PositionDate;
		YdPosition=r.YdPosition;
		Position=r.Position;
		LongFrozen=r.LongFrozen;
		ShortFrozen=r.ShortFrozen;
		LongFrozenAmount=r.LongFrozenAmount;
		ShortFrozenAmount=r.ShortFrozenAmount;
		OpenVolume=r.OpenVolume;
		CloseVolume=r.CloseVolume;
		OpenAmount=r.OpenAmount;
		CloseAmount=r.CloseAmount;
		PositionCost=r.PositionCost;
		PreMargin=r.PreMargin;
		UseMargin=r.UseMargin;
		FrozenMargin=r.FrozenMargin;
		FrozenCash=r.FrozenCash;
		FrozenCommission=r.FrozenCommission;
		CashIn=r.CashIn;
		Commission=r.Commission;
		CloseProfit=r.CloseProfit;
		PositionProfit=r.PositionProfit;
		PreSettlementPrice=r.PreSettlementPrice;
		SettlementPrice=r.SettlementPrice;
		strncpy(TradingDay,r.TradingDay,sizeof(TradingDay)-1);
		SettlementID=r.SettlementID;
		OpenCost=r.OpenCost;
		ExchangeMargin=r.ExchangeMargin;
		CombPosition=r.CombPosition;
		CombLongFrozen=r.CombLongFrozen;
		CombShortFrozen=r.CombShortFrozen;
		CloseProfitByDate=r.CloseProfitByDate;
		CloseProfitByTrade=r.CloseProfitByTrade;
		TodayPosition=r.TodayPosition;
		MarginRateByMoney=r.MarginRateByMoney;
		MarginRateByVolume=r.MarginRateByVolume;

		strncpy(Account,r.InvestorID,sizeof(Account)-1);
		bInvalid=false;
	}

	void Clear()
	{
		memset(this, 0, sizeof(*this));
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
		FTID=0;
		UpdateSeq=0;
		bInvalid=false;
	}
	bool IsInvalidRecord(void) const
	{
		return bInvalid;
	}
	char* tostring(char*prlt,int rltsize) const
    {
        if(bInvalid&&prlt&&rltsize>0) 
        {
            strncpy(prlt,"Invalid",rltsize);
            prlt[rltsize-1]=0;
            return prlt;
        }

        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,Direct%d,Hedge%d,Position=%d",
            Account,InstrumentID,PosiDirection,HedgeFlag,Position);
        prlt[len]=0;
        return prlt;
    }
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

struct PlatformStru_QryInvestor
{
    PlatformStru_QryInvestor()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryInvestorField Thost;
};
struct PlatformStru_QryProduct
{
    PlatformStru_QryProduct()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryProductField Thost;
};
struct PlatformStru_QryInstrument
{
    PlatformStru_QryInstrument()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryInstrumentField Thost;
};
struct PlatformStru_QryDepthMarketData
{
    PlatformStru_QryDepthMarketData() { memset(this,0,sizeof(*this)); };
	CThostFtdcQryDepthMarketDataField Thost;
};
struct PlatformStru_QrySettlementInfo
{
    PlatformStru_QrySettlementInfo()
    {
        memset(this,0,sizeof(*this));
    }

	CThostFtdcQrySettlementInfoField Thost;
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

struct PlatformStru_QryNotice
{
    PlatformStru_QryNotice()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryNoticeField Thost;
};
struct PlatformStru_QrySettlementInfoConfirm
{
    PlatformStru_QrySettlementInfoConfirm()
    {
        memset(this,0,sizeof(*this));
    };

	CThostFtdcQrySettlementInfoConfirmField Thost;
};

///请求查询保证金监管系统经纪公司资金账户密钥
struct PlatformStru_QryCFMMCTradingAccountKey
{
    PlatformStru_QryCFMMCTradingAccountKey()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryCFMMCTradingAccountKeyField Thost;
};
struct PlatformStru_QryTransferBank
{
    PlatformStru_QryTransferBank()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryTransferBankField Thost;
};
struct PlatformStru_QryTransferSerial
{
    PlatformStru_QryTransferSerial()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryTransferSerialField Thost;
};
struct PlatformStru_QryContractBank
{
    PlatformStru_QryContractBank()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryContractBankField Thost;
};
struct PlatformStru_QryAccountRegister
{
    PlatformStru_QryAccountRegister()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcQryAccountregisterField Thost;
};
///请求查询汇率
struct PlatformStru_QryExchangeRate
{
    PlatformStru_QryExchangeRate()
    {
        memset(this,0,sizeof(*this));
    }
    CThostFtdcQryExchangeRateField Thost;
};
///请求查询预埋单
struct PlatformStru_QryParkedOrder
{
    PlatformStru_QryParkedOrder()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryParkedOrderField Thost;
};
struct PlatformStru_QryParkedOrderAction
{
    PlatformStru_QryParkedOrderAction()
    {
        memset(this,0,sizeof(*this));
    }
	CThostFtdcQryParkedOrderActionField Thost;
};

struct PlatformStru_ReqQueryAccount
{
    PlatformStru_ReqQueryAccount()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcReqQueryAccountField Thost;
};
struct PlatformStru_ReqTransfer
{
    PlatformStru_ReqTransfer()
    {
        memset(this,0,sizeof(*this));
    };
	CThostFtdcReqTransferField Thost;
};
//账号信息
struct PlatformStru_Trader
{
	char TraderAccount[21];
	char BackOfficeID[21];
	char Tradable;
	int LossLimit;
};
//交易所信息
struct PlatformStru_Exchange
{
	char Name[11];
	char QueryEnabled;
	char AmendEnabled;
	int Strategy;
	char CustomDecs;
	int Decimals;
	char TicketType;
	char RFQA;
	char RFQT;
	char EnableBlock;
	char EnableBasis;
	char EnableAA;
	char EnableCross;
	int GTStatus;

};
//合约信息
typedef char Array10[11];
typedef Array10 LegStruct[5];
struct PlatformStru_Contract
{
	char  ContractName[11];
	char ContractDate[51];
	char ExchangeName[11];
	char ExpiryDate[9];
	char LastTradeDate[9];
	int NumberOfLegs;
	int TicksPerPoint;
	char TickSize[11];
	char Tradable;
	int GTStatus;
	char Margin[21];
	char ESATemplate;
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
	int TicksPerPoint;
	Array10 TickSize;
	int GTStatus;
};
//报单类型
struct PlatformStru_OrderType
{
	Array10 OrderType;
	Array10 ExchangeName;
	int OrderTypeID;
	unsigned char NumPricesReqd;
	unsigned char NumVolumesReqd;
	unsigned char NumDatesReqd;
	char AutoCreated;
	char TimeTriggered;
	char RealSynthetic;
	char GTCFlag;
	char TicketType[3];
	char PatsOrderType;
	int AmendOTCount;
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
struct PlatformStru_ExchangeRate
{
    PlatformStru_ExchangeRate() { memset(this,0,sizeof(*this)); }

	///经纪公司代码
	TThostFtdcBrokerIDType	    BrokerID;
	///源币种
	TThostFtdcCurrencyIDType	FromCurrencyID;
	///源币种单位数量
	TThostFtdcCurrencyUnitType	FromCurrencyUnit;
	///目标币种
	TThostFtdcCurrencyIDType	ToCurrencyID;
	///汇率
	TThostFtdcExchangeRateType	ExchangeRate;
};

///输入的询价
struct PlatformStru_InputForQuoteField
{
    PlatformStru_InputForQuoteField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_InputForQuoteField(const CThostFtdcInputForQuoteField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ForQuoteRef,thost.ForQuoteRef,sizeof(ForQuoteRef)-1);
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
    };
    void ToThost(CThostFtdcInputForQuoteField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
        strncpy(thost.ForQuoteRef,ForQuoteRef,sizeof(thost.ForQuoteRef)-1);
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,"BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ForQuoteRef=[%s],\t UserID=[%s]",
            BrokerID,InvestorID,InstrumentID,ForQuoteRef,UserID);
        return string(buf);
    };
	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///询价引用
	TThostFtdcOrderRefType	ForQuoteRef;
	///用户代码
	TThostFtdcUserIDType	UserID;
};

///输入的执行宣告
struct PlatformStru_InputExecOrderField
{
    PlatformStru_InputExecOrderField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_InputExecOrderField(const CThostFtdcInputExecOrderField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ExecOrderRef,thost.ExecOrderRef,sizeof(ExecOrderRef)-1);
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
        Volume=thost.Volume;
        RequestID=thost.RequestID;
        strncpy(BusinessUnit,thost.BusinessUnit,sizeof(BusinessUnit)-1);
        OffsetFlag=thost.OffsetFlag;
        HedgeFlag=thost.HedgeFlag;
        ActionType=thost.ActionType;
        PosiDirection=thost.PosiDirection;
        ReservePositionFlag=thost.ReservePositionFlag;
        CloseFlag=thost.CloseFlag;
    };
    void ToThost(CThostFtdcInputExecOrderField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
        strncpy(thost.ExecOrderRef,ExecOrderRef,sizeof(thost.ExecOrderRef)-1);
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
        thost.Volume=Volume;
        thost.RequestID=RequestID;
        strncpy(thost.BusinessUnit,BusinessUnit,sizeof(thost.BusinessUnit)-1);
        thost.OffsetFlag=OffsetFlag;
        thost.HedgeFlag=HedgeFlag;
        thost.ActionType=ActionType;
        thost.PosiDirection=PosiDirection;
        thost.ReservePositionFlag=ReservePositionFlag;
        thost.CloseFlag=CloseFlag;
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
            "BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExecOrderRef=[%s],\t UserID=[%s]"
            ",\t Volume=[%d],\t RequestID=[%d],\t BusinessUnit=[%s],\t OffsetFlag=[%d],\t HedgeFlag=[%d]"
            ",\t ActionType=[%d],\t PosiDirection=[%d],\t ReservePositionFlag=[%d],\t CloseFlag=[%d]",
             BrokerID, InvestorID, InstrumentID, ExecOrderRef,UserID,
             Volume, RequestID, BusinessUnit, OffsetFlag,HedgeFlag,
             ActionType, PosiDirection, ReservePositionFlag, CloseFlag);
        return string(buf);
    };

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///执行宣告引用
	TThostFtdcOrderRefType	ExecOrderRef;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///数量
	TThostFtdcVolumeType	Volume;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///业务单元
	TThostFtdcBusinessUnitType	BusinessUnit;
	///开平标志
	TThostFtdcOffsetFlagType	OffsetFlag;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///执行类型
	char	                    ActionType;         //TThostFtdcActionTypeType
	///保留头寸申请的持仓方向
	char	                    PosiDirection;      //TThostFtdcPosiDirectionType
	///期权行权后是否保留期货头寸的标记
	char	                    ReservePositionFlag;//TThostFtdcExecOrderPositionFlagType
	///期权行权后生成的头寸是否自动平仓
	char	                    CloseFlag;          //TThostFtdcExecOrderCloseFlagType
};
///输入执行宣告操作
struct PlatformStru_InputExecOrderActionField
{
    PlatformStru_InputExecOrderActionField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_InputExecOrderActionField(const CThostFtdcInputExecOrderActionField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        ExecOrderActionRef=thost.ExecOrderActionRef;
        strncpy(ExecOrderRef,thost.ExecOrderRef,sizeof(ExecOrderRef)-1);
        RequestID=thost.RequestID;
        FrontID=thost.FrontID;
        SessionID=thost.SessionID;
        strncpy(ExchangeID,thost.ExchangeID,sizeof(ExchangeID)-1);
        strncpy(ExecOrderSysID,thost.ExecOrderSysID,sizeof(ExecOrderSysID)-1);
        ActionFlag=thost.ActionFlag;
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
    };
    void ToThost(CThostFtdcInputExecOrderActionField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        thost.ExecOrderActionRef=ExecOrderActionRef;
        strncpy(thost.ExecOrderRef,ExecOrderRef,sizeof(thost.ExecOrderRef)-1);
        thost.RequestID=RequestID;
        thost.FrontID=FrontID;
        thost.SessionID=SessionID;
        strncpy(thost.ExchangeID,ExchangeID,sizeof(thost.ExchangeID)-1);
        strncpy(thost.ExecOrderSysID,ExecOrderSysID,sizeof(thost.ExecOrderSysID)-1);
        thost.ActionFlag=ActionFlag;
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
             "BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExecOrderActionRef=[%d],\t ExecOrderRef=[%s],\t UserID=[%s]"
             ",\t RequestID=[%d],\t FrontID=[%d],\t SessionID=[%d],\t ExchangeID=[%s],\t ExecOrderSysID=[%s],\t ActionFlag=[%d]",
             BrokerID, InvestorID, InstrumentID, ExecOrderActionRef, ExecOrderRef, UserID,
             RequestID, FrontID, SessionID, ExchangeID, ExecOrderSysID, ActionFlag);
        return string(buf);
    };

    ///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///执行宣告操作引用
	int	                    ExecOrderActionRef;     //TThostFtdcOrderActionRefType
	///执行宣告引用
	TThostFtdcOrderRefType	ExecOrderRef;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///前置编号
	TThostFtdcFrontIDType	FrontID;
	///会话编号
	TThostFtdcSessionIDType	SessionID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///执行宣告操作编号
	char	                ExecOrderSysID[21];     //TThostFtdcExecOrderSysIDType
	///操作标志
	TThostFtdcActionFlagType	ActionFlag;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
};
///执行宣告查询
struct PlatformStru_QryExecOrderField
{
    PlatformStru_QryExecOrderField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_QryExecOrderField(const CThostFtdcQryExecOrderField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ExchangeID,thost.ExchangeID,sizeof(ExchangeID)-1);
        strncpy(ExecOrderSysID,thost.ExecOrderSysID,sizeof(ExecOrderSysID)-1);
        strncpy(InsertTimeStart,thost.InsertTimeStart,sizeof(InsertTimeStart)-1);
        strncpy(InsertTimeEnd,thost.InsertTimeEnd,sizeof(InsertTimeEnd)-1);
    };
    void ToThost(CThostFtdcQryExecOrderField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
        strncpy(thost.ExchangeID,ExchangeID,sizeof(thost.ExchangeID)-1);
        strncpy(thost.ExecOrderSysID,ExecOrderSysID,sizeof(thost.ExecOrderSysID)-1);
        strncpy(thost.InsertTimeStart,InsertTimeStart,sizeof(thost.InsertTimeStart)-1);
        strncpy(thost.InsertTimeEnd,InsertTimeEnd,sizeof(thost.InsertTimeEnd)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
             "BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExecOrderSysID=[%s]"
             ",\t InsertTimeStart=[%s],\t InsertTimeEnd=[%s]",
             BrokerID, InvestorID, InstrumentID, ExchangeID,ExecOrderSysID,
             InsertTimeStart,InsertTimeEnd);
        return string(buf);
    };

    ///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///执行宣告编号
	char	ExecOrderSysID[21];         //TThostFtdcExecOrderSysIDType
	///开始时间
	TThostFtdcTimeType	InsertTimeStart;
	///结束时间
	TThostFtdcTimeType	InsertTimeEnd;
};
///执行宣告操作
struct PlatformStru_ExecOrderActionField
{
    PlatformStru_ExecOrderActionField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_ExecOrderActionField(const CThostFtdcExecOrderActionField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        ExecOrderActionRef=thost.ExecOrderActionRef;
        strncpy(ExecOrderRef,thost.ExecOrderRef,sizeof(ExecOrderRef)-1);
        RequestID=thost.RequestID;
        FrontID=thost.FrontID;
        SessionID=thost.SessionID;
        strncpy(ExchangeID,thost.ExchangeID,sizeof(ExchangeID)-1);
        strncpy(ExecOrderSysID,thost.ExecOrderSysID,sizeof(ExecOrderSysID)-1);
        ActionFlag=thost.ActionFlag;
        strncpy(ActionDate,thost.ActionDate,sizeof(ActionDate)-1);
        strncpy(ActionTime,thost.ActionTime,sizeof(ActionTime)-1);
        strncpy(TraderID,thost.TraderID,sizeof(TraderID)-1);
        InstallID=thost.InstallID;
        strncpy(ExecOrderLocalID,thost.ExecOrderLocalID,sizeof(ExecOrderLocalID)-1);
        strncpy(ActionLocalID,thost.ActionLocalID,sizeof(ActionLocalID)-1);
        strncpy(ParticipantID,thost.ParticipantID,sizeof(ParticipantID)-1);
        strncpy(ClientID,thost.ClientID,sizeof(ClientID)-1);
        strncpy(BusinessUnit,thost.BusinessUnit,sizeof(BusinessUnit)-1);
        OrderActionStatus=thost.OrderActionStatus;
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
        ActionType=thost.ActionType;
        strncpy(StatusMsg,thost.StatusMsg,sizeof(StatusMsg)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
    };
    void ToThost(CThostFtdcExecOrderActionField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        thost.ExecOrderActionRef=ExecOrderActionRef;
        strncpy(thost.ExecOrderRef,ExecOrderRef,sizeof(thost.ExecOrderRef)-1);
        thost.RequestID=RequestID;
        thost.FrontID=FrontID;
        thost.SessionID=SessionID;
        strncpy(thost.ExchangeID,ExchangeID,sizeof(thost.ExchangeID)-1);
        strncpy(thost.ExecOrderSysID,ExecOrderSysID,sizeof(thost.ExecOrderSysID)-1);
        thost.ActionFlag=ActionFlag;
        strncpy(thost.ActionDate,ActionDate,sizeof(thost.ActionDate)-1);
        strncpy(thost.ActionTime,ActionTime,sizeof(thost.ActionTime)-1);
        strncpy(thost.TraderID,TraderID,sizeof(thost.TraderID)-1);
        thost.InstallID=InstallID;
        strncpy(thost.ExecOrderLocalID,ExecOrderLocalID,sizeof(thost.ExecOrderLocalID)-1);
        strncpy(thost.ActionLocalID,ActionLocalID,sizeof(thost.ActionLocalID)-1);
        strncpy(thost.ParticipantID,ParticipantID,sizeof(thost.ParticipantID)-1);
        strncpy(thost.ClientID,ClientID,sizeof(thost.ClientID)-1);
        strncpy(thost.BusinessUnit,BusinessUnit,sizeof(thost.BusinessUnit)-1);
        thost.OrderActionStatus=OrderActionStatus;
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
        thost.ActionType=ActionType;
        strncpy(thost.StatusMsg,StatusMsg,sizeof(thost.StatusMsg)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
            "BrokerID=[%s],\t InvestorID=[%s],\t TraderID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\t UserID=[%s]"
            ",\t InstrumentID=[%s],\t ExchangeID=[%s],\t OrderActionStatus=[%d],\t StatusMsg=[%s]"
            ",\t ActionType=[%d],\t ActionFlag=[%d],\t ActionDate=[%s],\t ActionTime=[%s]"
            ",\t ExecOrderLocalID=[%s],\t ExecOrderSysID=[%s],\t ExecOrderActionRef=[%d],\t ExecOrderRef=[%s],\t ActionLocalID=[%s]"
            ",\t RequestID=[%d],\t FrontID=[%d],\t SessionID=[%d],\t InstallID=[%d],\t BusinessUnit=[%s]",
            BrokerID,InvestorID,TraderID,ParticipantID,ClientID,UserID,
            InstrumentID,ExchangeID,OrderActionStatus,StatusMsg,
            ActionType,ActionFlag,ActionDate,ActionTime,
            ExecOrderLocalID,ExecOrderSysID,ExecOrderActionRef,ExecOrderRef,ActionLocalID,
            RequestID,FrontID,SessionID,InstallID,BusinessUnit);
        return string(buf);
    };

    ///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///执行宣告操作引用
	TThostFtdcOrderActionRefType	ExecOrderActionRef;
	///执行宣告引用
	TThostFtdcOrderRefType	ExecOrderRef;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///前置编号
	TThostFtdcFrontIDType	FrontID;
	///会话编号
	TThostFtdcSessionIDType	SessionID;
	///交易所代码
	TThostFtdcExchangeIDType	ExchangeID;
	///执行宣告操作编号
	TThostFtdcExecOrderSysIDType	ExecOrderSysID;
	///操作标志
	TThostFtdcActionFlagType	ActionFlag;
	///操作日期
	TThostFtdcDateType	ActionDate;
	///操作时间
	TThostFtdcTimeType	ActionTime;
	///交易所交易员代码
	TThostFtdcTraderIDType	TraderID;
	///安装编号
	TThostFtdcInstallIDType	InstallID;
	///本地执行宣告编号
	TThostFtdcOrderLocalIDType	ExecOrderLocalID;
	///操作本地编号
	TThostFtdcOrderLocalIDType	ActionLocalID;
	///会员代码
	TThostFtdcParticipantIDType	ParticipantID;
	///客户代码
	TThostFtdcClientIDType	ClientID;
	///业务单元
	TThostFtdcBusinessUnitType	BusinessUnit;
	///报单操作状态
	TThostFtdcOrderActionStatusType	OrderActionStatus;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///执行类型
	TThostFtdcActionTypeType	ActionType;
	///状态信息
	TThostFtdcErrorMsgType	StatusMsg;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
};

///执行宣告
struct PlatformStru_ExecOrderField
{
    PlatformStru_ExecOrderField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_ExecOrderField(const CThostFtdcExecOrderField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(BrokerID,thost.BrokerID,sizeof(BrokerID)-1);
        strncpy(InvestorID,thost.InvestorID,sizeof(InvestorID)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ExecOrderRef,thost.ExecOrderRef,sizeof(ExecOrderRef)-1);
        strncpy(UserID,thost.UserID,sizeof(UserID)-1);
        Volume=thost.Volume;
        RequestID=thost.RequestID;
        strncpy(BusinessUnit,thost.BusinessUnit,sizeof(BusinessUnit)-1);
        OffsetFlag=thost.OffsetFlag;
        HedgeFlag=thost.HedgeFlag;
        ActionType=thost.ActionType;
        PosiDirection=thost.PosiDirection;
        ReservePositionFlag=thost.ReservePositionFlag;
        CloseFlag=thost.CloseFlag;
        strncpy(ExecOrderLocalID,thost.ExecOrderLocalID,sizeof(ExecOrderLocalID)-1);
        strncpy(ExchangeID,thost.ExchangeID,sizeof(ExchangeID)-1);
        strncpy(ParticipantID,thost.ParticipantID,sizeof(ParticipantID)-1);
        strncpy(ClientID,thost.ClientID,sizeof(ClientID)-1);
        strncpy(ExchangeInstID,thost.ExchangeInstID,sizeof(ExchangeInstID)-1);
        strncpy(TraderID,thost.TraderID,sizeof(TraderID)-1);
        InstallID=thost.InstallID;
        OrderSubmitStatus=thost.OrderSubmitStatus;
        NotifySequence=thost.NotifySequence;
        strncpy(TradingDay,thost.TradingDay,sizeof(TradingDay)-1);
        SettlementID=thost.SettlementID;
        strncpy(ExecOrderSysID,thost.ExecOrderSysID,sizeof(ExecOrderSysID)-1);
        strncpy(InsertDate,thost.InsertDate,sizeof(InsertDate)-1);
        strncpy(InsertTime,thost.InsertTime,sizeof(InsertTime)-1);
        strncpy(CancelTime,thost.CancelTime,sizeof(CancelTime)-1);
        ExecResult=thost.ExecResult;
        strncpy(ClearingPartID,thost.ClearingPartID,sizeof(ClearingPartID)-1);
        SequenceNo=thost.SequenceNo;
        FrontID=thost.FrontID;
        SessionID=thost.SessionID;
        strncpy(UserProductInfo,thost.UserProductInfo,sizeof(UserProductInfo)-1);
        strncpy(StatusMsg,thost.StatusMsg,sizeof(StatusMsg)-1);
        strncpy(ActiveUserID,thost.ActiveUserID,sizeof(ActiveUserID)-1);
        BrokerExecOrderSeq=thost.BrokerExecOrderSeq;

        strncpy(Account,InvestorID,sizeof(Account)-1);
    };
    void ToThost(CThostFtdcExecOrderField& thost)
    {
        memset(&thost,0,sizeof(thost));
        strncpy(thost.BrokerID,BrokerID,sizeof(thost.BrokerID)-1);
        strncpy(thost.InvestorID,InvestorID,sizeof(thost.InvestorID)-1);
        strncpy(thost.InstrumentID,InstrumentID,sizeof(thost.InstrumentID)-1);
        strncpy(thost.ExecOrderRef,ExecOrderRef,sizeof(thost.ExecOrderRef)-1);
        strncpy(thost.UserID,UserID,sizeof(thost.UserID)-1);
        thost.Volume=Volume;
        thost.RequestID=RequestID;
        strncpy(thost.BusinessUnit,BusinessUnit,sizeof(thost.BusinessUnit)-1);
        thost.OffsetFlag=OffsetFlag;
        thost.HedgeFlag=HedgeFlag;
        thost.ActionType=ActionType;
        thost.PosiDirection=PosiDirection;
        thost.ReservePositionFlag=ReservePositionFlag;
        thost.CloseFlag=CloseFlag;
        strncpy(thost.ExecOrderLocalID,ExecOrderLocalID,sizeof(thost.ExecOrderLocalID)-1);
        strncpy(thost.ExchangeID,ExchangeID,sizeof(thost.ExchangeID)-1);
        strncpy(thost.ParticipantID,ParticipantID,sizeof(thost.ParticipantID)-1);
        strncpy(thost.ClientID,ClientID,sizeof(thost.ClientID)-1);
        strncpy(thost.ExchangeInstID,ExchangeInstID,sizeof(thost.ExchangeInstID)-1);
        strncpy(thost.TraderID,TraderID,sizeof(thost.TraderID)-1);
        thost.InstallID=InstallID;
        thost.OrderSubmitStatus=OrderSubmitStatus;
        thost.NotifySequence=NotifySequence;
        strncpy(thost.TradingDay,TradingDay,sizeof(thost.TradingDay)-1);
        thost.SettlementID=SettlementID;
        strncpy(thost.ExecOrderSysID,ExecOrderSysID,sizeof(thost.ExecOrderSysID)-1);
        strncpy(thost.InsertDate,InsertDate,sizeof(thost.InsertDate)-1);
        strncpy(thost.InsertTime,InsertTime,sizeof(thost.InsertTime)-1);
        strncpy(thost.CancelTime,CancelTime,sizeof(thost.CancelTime)-1);
        thost.ExecResult=ExecResult;
        strncpy(thost.ClearingPartID,ClearingPartID,sizeof(thost.ClearingPartID)-1);
        thost.SequenceNo=SequenceNo;
        thost.FrontID=FrontID;
        thost.SessionID=SessionID;
        strncpy(thost.UserProductInfo,UserProductInfo,sizeof(thost.UserProductInfo)-1);
        strncpy(thost.StatusMsg,StatusMsg,sizeof(thost.StatusMsg)-1);
        strncpy(thost.ActiveUserID,ActiveUserID,sizeof(thost.ActiveUserID)-1);
        thost.BrokerExecOrderSeq=BrokerExecOrderSeq;
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
            "BrokerID=[%s],\t InvestorID=[%s],\t TraderID=[%s],\t ParticipantID=[%s],\t ClientID=[%s],\t UserID=[%s],\t ActiveUserID=[%s]"
            ",\t InstrumentID=[%s],\t ExchangeID=[%s],\t ExchangeInstID=[%s],\t Volume=[%d],\t OffsetFlag=[%d],\t HedgeFlag=[%d]"
            ",\t ExecResult=[%d],\t OrderSubmitStatus=[%d],\t NotifySequence=[%d],\t StatusMsg=[%s]"
            ",\t ActionType=[%d],\t InsertDate=[%s],\t InsertTime=[%s],\t CancelTime=[%s],\t TradingDay=[%s]"
            ",\t PosiDirection=[%d],\t ReservePositionFlag=[%d],\t CloseFlag=[%d]"
            ",\t ExecOrderLocalID=[%s],\t ExecOrderSysID=[%s],\t ExecOrderRef=[%s],\t BrokerExecOrderSeq=[%d]"
            ",\t RequestID=[%d],\t FrontID=[%d],\t SessionID=[%d],\t InstallID=[%d],\t BusinessUnit=[%s],\t SettlementID=[%d]"
            ",\t ClearingPartID=[%s],\t SequenceNo=[%d],\t UserProductInfo=[%s]",
            BrokerID,InvestorID,TraderID,ParticipantID,ClientID,UserID,ActiveUserID,
            InstrumentID,ExchangeID,ExchangeInstID,Volume,OffsetFlag,HedgeFlag,
            ExecResult,OrderSubmitStatus,NotifySequence,StatusMsg,
            ActionType,InsertDate,InsertTime,CancelTime,TradingDay,
            PosiDirection,ReservePositionFlag,CloseFlag,
            ExecOrderLocalID,ExecOrderSysID,ExecOrderRef,BrokerExecOrderSeq,
            RequestID,FrontID,SessionID,InstallID,BusinessUnit,SettlementID,
            ClearingPartID,SequenceNo,UserProductInfo);
        return string(buf);
    };

	///经纪公司代码
	TThostFtdcBrokerIDType	BrokerID;
	///投资者代码
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///执行宣告引用
	TThostFtdcOrderRefType	ExecOrderRef;
	///用户代码
	TThostFtdcUserIDType	UserID;
	///数量
	TThostFtdcVolumeType	Volume;
	///请求编号
	TThostFtdcRequestIDType	RequestID;
	///业务单元
	TThostFtdcBusinessUnitType	BusinessUnit;
	///开平标志
	TThostFtdcOffsetFlagType	OffsetFlag;
	///投机套保标志
	TThostFtdcHedgeFlagType	HedgeFlag;
	///执行类型
	char	ActionType;                             //TThostFtdcActionTypeType
	///保留头寸申请的持仓方向
	TThostFtdcPosiDirectionType	PosiDirection;
	///期权行权后是否保留期货头寸的标记
	char	ReservePositionFlag;                    //TThostFtdcExecOrderPositionFlagType
	///期权行权后生成的头寸是否自动平仓
	char	CloseFlag;                              //TThostFtdcExecOrderCloseFlagType
	///本地执行宣告编号
	TThostFtdcOrderLocalIDType	ExecOrderLocalID;
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
	///执行宣告提交状态
	TThostFtdcOrderSubmitStatusType	OrderSubmitStatus;
	///报单提示序号
	TThostFtdcSequenceNoType	NotifySequence;
	///交易日
	TThostFtdcDateType	TradingDay;
	///结算编号
	TThostFtdcSettlementIDType	SettlementID;
	///执行宣告编号
	char	ExecOrderSysID[21];             //TThostFtdcExecOrderSysIDType
	///报单日期
	TThostFtdcDateType	InsertDate;
	///插入时间
	TThostFtdcTimeType	InsertTime;
	///撤销时间
	TThostFtdcTimeType	CancelTime;
	///执行结果
	char	ExecResult;                     //TThostFtdcExecResultType
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
	///操作用户代码
	TThostFtdcUserIDType	ActiveUserID;
	///经纪公司报单编号
	TThostFtdcSequenceNoType	BrokerExecOrderSeq;

    //对应账号
	CommonAccountType           Account;		            
	//FasttraderID,对于每一条记录的Key，这是一个唯一编号
	long FTID;                                                
	//每次数据内容发生变化，UpdateSeq加一
	long UpdateSeq;

};
///询价通知
struct PlatformStru_ForQuoteRspField
{
    PlatformStru_ForQuoteRspField()
    {
        memset(this,0,sizeof(*this));
    };
#ifdef CTP060300
    PlatformStru_ForQuoteRspField(const CThostFtdcForQuoteRspField& thost)
    {
        memset(this,0,sizeof(*this));
        strncpy(TradingDay,thost.TradingDay,sizeof(TradingDay)-1);
        strncpy(InstrumentID,thost.InstrumentID,sizeof(InstrumentID)-1);
        strncpy(ForQuoteSysID,thost.ForQuoteSysID,sizeof(ForQuoteSysID)-1);
        strncpy(ForQuoteTime,thost.ForQuoteTime,sizeof(ForQuoteTime)-1);
        strncpy(ActionDay,thost.ActionDay,sizeof(ActionDay)-1);
    };
#endif
    string tostring(void)
    {
        char buf[1024]={0};
        sprintf_s(buf,sizeof(buf)-1,
            "TradingDay=[%s],\t InstrumentID=[%s],\t ForQuoteSysID=[%s],\t ForQuoteTime=[%s],\t ActionDay=[%s]",
            TradingDay,InstrumentID,ForQuoteSysID,ForQuoteTime,ActionDay);
        return string(buf);
    };

	///交易日
	TThostFtdcDateType	TradingDay;
	///合约代码
	TThostFtdcInstrumentIDType	InstrumentID;
	///询价编号
	TThostFtdcOrderSysIDType	ForQuoteSysID;
	///询价时间
	TThostFtdcTimeType	ForQuoteTime;
	///业务日期
	TThostFtdcDateType	ActionDay;
};


#pragma pack(push)
#pragma pack(1)
struct PlatformStru_AccountPosition
{
	char Profit[21];
	int Buys;
	int Sells;
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

struct PlusinAcctOpenOrderNumRsp
{
	PlusinAcctInstrumentKey queryCondition;
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


struct PlatformStru_RspOrderAction1
{
	CThostFtdcInputOrderActionField InputOrderActionField;
	CThostFtdcRspInfoField     RspInfoField; 
};

struct PlatformStru_RspOrderAction2
{
	CThostFtdcInputOrderActionField OrderActionField;
	CThostFtdcRspInfoField     RspInfoField;  
};

struct PlatformStru_RspOrderInsert1
{
	CThostFtdcInputOrderActionField InputOrderInsertField;
	CThostFtdcRspInfoField     RspInfoField; 
};

struct PlatformStru_RspOrderInsert2
{
	CThostFtdcInputOrderActionField OrderInsertField;
	CThostFtdcRspInfoField     RspInfoField;  
};

struct ExchangeTime
{
    ExchangeTime()
    {
        CurTickCount=0;
        memset(&TradingDay, 0, sizeof(TradingDay));
        memset(&SHFETime, 0, sizeof(SHFETime));
        memset(&DCETime, 0, sizeof(DCETime));
        memset(&CZCETime, 0, sizeof(CZCETime));
        memset(&FFEXTime, 0, sizeof(FFEXTime));
    };
    
    unsigned long             CurTickCount;//本地电脑开机后的毫秒数据
    TThostFtdcDateType	TradingDay;
    ///上期所时间
	TThostFtdcTimeType	SHFETime;
	///大商所时间
	TThostFtdcTimeType	DCETime;
	///郑商所时间
	TThostFtdcTimeType	CZCETime;
	///中金所时间
	TThostFtdcTimeType	FFEXTime;
};

//简单的合约信息
struct SimpleInstrumentInfo
{
	SimpleInstrumentInfo(std::string sid="",std::string sname=""):id(sid),name(sname){}
    SimpleInstrumentInfo(const SimpleInstrumentInfo& r) { *this=r; }
    SimpleInstrumentInfo& operator= (const SimpleInstrumentInfo& r) { id=r.id;  name=r.name;  return *this; }

	std::string id;
	std::string name;
} ;

struct InstrumentGroupInfo
{
    InstrumentGroupInfo() {};
    InstrumentGroupInfo(const InstrumentGroupInfo& r) { *this=r; }
    InstrumentGroupInfo& operator= (const InstrumentGroupInfo& r) { GroupName=r.GroupName;  InstrVec=r.InstrVec;  return *this; }

	std::string GroupName;
	std::vector<SimpleInstrumentInfo> InstrVec;
};

struct CTPReport
{
	std::string strReportType;
	int nSize;
	void* pReport;

	CTPReport()
	{
		strReportType = "";
		nSize = 0;
		pReport = NULL;
	}

	DWORD GetDate()
	{
		int nPos = strReportType.find( ' ');
		if ( nPos == -1 )
		{
			return 0;
		}

		std::string strTemp = strReportType.substr( nPos+1, strReportType.length());
		nPos = strTemp.find( '-' );
		if ( nPos == -1 )
		{
			return 0;
		}

		std::string strDay = strTemp.substr( 0, nPos );
		strTemp = strTemp.substr( nPos+1, strTemp.length());
		nPos = strTemp.find( '-' );
		if ( nPos == -1 )
		{
			return 0;
		}

		std::string strMonth = strTemp.substr( 0, nPos );
		std::string strYear = strTemp.substr( nPos+1, strTemp.length());
		
		DWORD dwDate = atoi(strYear.c_str())*10000 + GetMonth(strMonth)*100 + atoi(strDay.c_str());
		return dwDate;
	}

	DWORD GetMonth( const std::string& month )
	{
		std::string strMonth = month;
		transform( strMonth.begin(), strMonth.end(), strMonth.begin(), toupper);
		if ( strMonth == "JAN" )
		{
			return 1;
		}
		else if ( strMonth == "FEB")
		{
			return 2;
		}
		else if ( strMonth == "MAR")
		{
			return 3;
		}
		else if ( strMonth == "APR")
		{
			return 4;
		}
		else if ( strMonth == "MAY")
		{
			return 5;
		}
		else if ( strMonth == "JUN")
		{
			return 6;
		}
		else if ( strMonth == "JUL")
		{
			return 7;
		}
		else if ( strMonth == "AUG")
		{
			return 8;
		}
		else if ( strMonth == "SEPT")
		{
			return 9;
		}
		else if ( strMonth == "OCT")
		{
			return 10;
		}
		else if ( strMonth == "NOV")
		{
			return 11;
		}
		else if ( strMonth == "DEC")
		{
			return 12;
		}
		else
		{
			return 0;
		}
	}

    //禁止使用拷贝构造
    CTPReport(const CTPReport& r);
    CTPReport& operator= (const CTPReport& r);
};


///标示一个报单记录
struct OrderKey
{
    OrderKey()
    {
        memset(this,0,sizeof(*this));
        FrontID=-1;
        SessionID=-1;
    };

    OrderKey(const char* account,const char* instrumentID,int fid, int sid, const char* orderRef)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        FrontID=fid;
        SessionID=sid;
        CTools_Ansi::mymemcpy_trim(OrderRef,sizeof(OrderRef),orderRef);
    };

    OrderKey(const PlatformStru_OrderInfo& Order)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Order.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
        FrontID=Order.FrontID;
        SessionID=Order.SessionID;
        CTools_Ansi::mymemcpy_trim(OrderRef,sizeof(OrderRef),Order.OrderRef);
    };

    OrderKey& operator= (const OrderKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));
        return *this;
    };

    bool operator< (const OrderKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(FrontID<r.FrontID)
            return true;
        if(FrontID>r.FrontID)
            return false;

        if(SessionID<r.SessionID)
            return true;
        if(SessionID>r.SessionID)
            return false;

        irlt=strncmp(OrderRef,r.OrderRef,sizeof(OrderRef));
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const OrderKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(FrontID!=r.FrontID) return false;

        if(SessionID!=r.SessionID) return false;

        irlt=strncmp(OrderRef,r.OrderRef,sizeof(OrderRef));
        if(irlt!=0) return false;

        return true;
    }

    bool operator> (const OrderKey& r) const
    {
        return !((*this) < r || (*this) == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,FrontID%d,SessionID%d,%s",
            Account,InstrumentID,FrontID,SessionID,OrderRef);
        prlt[len]=0;
        return prlt;
    }
    
    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;       //合约ID
    int                     FrontID;            //前置机有许多，通过此ID标记不同的前置机
    int                     SessionID;          //只要重新登入，就会变化，
    CommonOrderRefType      OrderRef;           //(长度为12个字符，右对齐，左边补空格)报单在GUI中的ID,
                                                //只要FrontID与SessionID的组合唯一，
                                                //OrderRef就能重复使用    
};

///标示一个成交记录
struct TradeKey
{
    TradeKey()
    {
        memset(this,0,sizeof(*this));
    };

    TradeKey(const char* account, 
                const char* instrumentID, 
                const char* tradeID, 
                const char* tradeDate, 
                const char direction,
                const char hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools_Ansi::mymemcpy_trim(TradeDate,sizeof(TradeDate),tradeDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);
        Direction=direction;
        HedgeFlag=hedgeFlag;
    };

    TradeKey(const PlatformStru_TradeInfo& Trade)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Trade.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Trade.InstrumentID);
        CTools_Ansi::mymemcpy_trim(TradeDate,sizeof(TradeDate),Trade.TradeDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),Trade.TradeID);
        Direction=Trade.Direction;
        HedgeFlag=Trade.HedgeFlag;
    };

    TradeKey& operator= (const TradeKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };

    //注意，通过TradeDate和TradeID的比较顺序，保证set<TradeKey>对相同条件的记录，按时间排序
    bool operator< (const TradeKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(TradeDate,r.TradeDate,sizeof(TradeDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(HedgeFlag<r.HedgeFlag) return true;
        if(HedgeFlag>r.HedgeFlag) return false;
        
        if(Direction<r.Direction) return true;
        if(Direction>r.Direction) return false;
        
        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const TradeKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(TradeDate,r.TradeDate,sizeof(TradeDate));
        if(irlt!=0) return false;

        if(HedgeFlag!=r.HedgeFlag)
            return false;
        if(Direction!=r.Direction)
            return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt!=0) return false;

        return true;
    }
    bool operator!= (const TradeKey& r) const
    {
        return !(*this==r);
    }
    
    bool operator> (const TradeKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;

        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(TradeDate);
        if(len+ilen+1<rltsize)   { strcat(prlt,TradeDate);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,TradeID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d HedgeFlag%d",Direction,HedgeFlag);
        ilen=strlen(buf);
        if(len+ilen<rltsize)      { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }

    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;      //合约ID
    CommonDateType          TradeDate;         //成交日期(长度为8个字符，如:20101023)
    CommonTradeIDType       TradeID;           //成交ID(长度为20个字符，右对齐，左边补空格)
	char		            Direction;         //考虑模拟盘自成交情况，加上方向
    char                    HedgeFlag;
};


///标示一个期权行权报单记录
struct ExecOrderKey
{
    ExecOrderKey()
    {
        memset(this,0,sizeof(*this));
        FrontID=-1;
        SessionID=-1;
    };

    ExecOrderKey(const char* account,const char* instrumentID,int fid, int sid, const char* orderRef)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        FrontID=fid;
        SessionID=sid;
        CTools_Ansi::mymemcpy_trim(OrderRef,sizeof(OrderRef),orderRef);
    };

    ExecOrderKey(const PlatformStru_ExecOrderField& Order)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Order.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
        FrontID=Order.FrontID;
        SessionID=Order.SessionID;
        CTools_Ansi::mymemcpy_trim(OrderRef,sizeof(OrderRef),Order.ExecOrderRef);
    };

    ExecOrderKey& operator= (const ExecOrderKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));
        return *this;
    };

    bool operator< (const ExecOrderKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(FrontID<r.FrontID)
            return true;
        if(FrontID>r.FrontID)
            return false;

        if(SessionID<r.SessionID)
            return true;
        if(SessionID>r.SessionID)
            return false;

        irlt=strncmp(OrderRef,r.OrderRef,sizeof(OrderRef));
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const ExecOrderKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(FrontID!=r.FrontID) return false;

        if(SessionID!=r.SessionID) return false;

        irlt=strncmp(OrderRef,r.OrderRef,sizeof(OrderRef));
        if(irlt!=0) return false;

        return true;
    }

    bool operator> (const ExecOrderKey& r) const
    {
        return !((*this) < r || (*this) == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,FrontID%d,SessionID%d,%s",
            Account,InstrumentID,FrontID,SessionID,OrderRef);
        prlt[len]=0;
        return prlt;
    }
    
    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;       //合约ID
    int                     FrontID;            //前置机有许多，通过此ID标记不同的前置机
    int                     SessionID;          //只要重新登入，就会变化，
    CommonOrderRefType      OrderRef;           //(长度为12个字符，右对齐，左边补空格)报单在GUI中的ID,
                                                //只要FrontID与SessionID的组合唯一，
                                                //OrderRef就能重复使用    
};


///标示一个成交统计记录
struct TradeTotalKey
{
    TradeTotalKey(const char* account,
                    const char* exchangeID, 
                    const char* instrumentID, 
                    const char direction, 
                    const char offsetFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),exchangeID);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        Direction=direction;
        OffsetFlag=offsetFlag;
    };

    TradeTotalKey(const PlatformStru_TradeTotalInfo& TradeTotal)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),TradeTotal.Account);
        CTools_Ansi::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),TradeTotal.ExchangeID);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),TradeTotal.InstrumentID);
        Direction=TradeTotal.Direction;
        OffsetFlag=TradeTotal.OffsetFlag;
    };

	TradeTotalKey(const PlatformStru_TradeInfo& TradeInfo)
	{
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),TradeInfo.Account);
        CTools_Ansi::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),TradeInfo.ExchangeID);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),TradeInfo.InstrumentID);
        Direction=TradeInfo.Direction;
        OffsetFlag=TradeInfo.OffsetFlag;
    }


    TradeTotalKey& operator= (const TradeTotalKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };

    bool operator< (const TradeTotalKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(ExchangeID,r.ExchangeID,sizeof(ExchangeID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
            return true;
        if(Direction>r.Direction)
            return false;

        if(OffsetFlag<r.OffsetFlag)
            return true;
        if(OffsetFlag>r.OffsetFlag)
            return false;

        return false;
    }

    bool operator== (const TradeTotalKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(ExchangeID,r.ExchangeID,sizeof(ExchangeID));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;

        if(OffsetFlag!=r.OffsetFlag)
            return false;

        return true;
    }
    
    bool operator> (const TradeTotalKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,Direct%d,Offset%d",
            Account,ExchangeID,InstrumentID,Direction,OffsetFlag);
        prlt[len]=0;
        return prlt;
    }

    CommonAccountType           Account;
    CommonExchangeIDType        ExchangeID;
    CommonInstrumentIDType      InstrumentID;      //合约ID
	char			            Direction;
	char			            OffsetFlag;
};


///标示一个持仓明细记录，由InstrumentID、OpenDate、TradeID、Direction、TradeType定义唯一的持仓明细记录
struct PositionDetailKey
{
    PositionDetailKey()
    {
        memset(this,0,sizeof(*this));
    }
	PositionDetailKey(const char* account, 
                        const char* instrumentID, 
                        const char* openDate, 
                        const char* tradeID, 
                        char direction, 
                        char tradeType,
                        char hedgeFlag)
    {
	    set(account,instrumentID,openDate,tradeID,direction,tradeType,hedgeFlag);
    };

    PositionDetailKey(const PlatformStru_PositionDetail& PositionDetail)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),PositionDetail.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),PositionDetail.InstrumentID);
        CTools_Ansi::mymemcpy_trim(OpenDate,sizeof(OpenDate),PositionDetail.OpenDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),PositionDetail.TradeID);

        Direction=PositionDetail.Direction;
        TradeType=PositionDetail.TradeType;
        HedgeFlag=PositionDetail.HedgeFlag;
    };

    PositionDetailKey& operator= (const PositionDetailKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };
	void set(const char* account,
        const char* instrumentID,
        const char* openDate,
        const char* tradeID,
        char direction,
        char tradeType,
        char hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools_Ansi::mymemcpy_trim(OpenDate,sizeof(OpenDate),openDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);

        Direction=direction;
        TradeType=tradeType;
        HedgeFlag=hedgeFlag;
    };


    //注意，通过OpenDate和TradeID的比较顺序，保证set<PositionDetailKey>对相同条件的持仓明细，按时间排序
    bool operator< (const PositionDetailKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools_Ansi::mycompare_trynumeric(TradeID,r.TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
			return true;
		if(Direction>r.Direction)
			return false;

        if(TradeType<r.TradeType)
			return true;
		if(TradeType>r.TradeType) 
			return false;

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag) 
			return false;


        return false;
    }

    bool operator== (const PositionDetailKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt!=0) return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;
        if(TradeType!=r.TradeType)
            return false;
        if(HedgeFlag!=r.HedgeFlag)
            return false;

        return true;
    }
    
    bool operator!= (const PositionDetailKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionDetailKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,Direct%d,TradeType%d,HedgeFlag%d",
            Account,InstrumentID,OpenDate,TradeID,Direction,TradeType,HedgeFlag);
        prlt[len]=0;
        return prlt;
    }


	CommonAccountType       Account;		    //账号
    CommonInstrumentIDType  InstrumentID;       //合约ID
    CommonDateType          OpenDate;           //开仓日期
    CommonTradeIDType       TradeID;            //成交ID(长度为20个字符，右对齐，左边补空格)
    char                    Direction;          //买卖方向
    char                    TradeType;          //成交类型，'0'/0x0: 普通成交； '4': 组合衍生成交
    char                    HedgeFlag;          //投机套保标志
};


///标示一个持仓记录，由InstrumentID、Direction、HedgeFlag定义唯一的持仓明细记录
struct PositionKey
{
    PositionKey()
    {
        memset(this,0,sizeof(*this));
    };
	PositionKey(const char* account, const char* instrumentID, const char& posiDirection, const char& hedgeFlag)
    {
        set(account, instrumentID, posiDirection, hedgeFlag);
    }
	void set(const char* account, const char* instrumentID, const char& posiDirection, const char& hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        PosiDirection=posiDirection;
        HedgeFlag=hedgeFlag;
    };

    PositionKey(const PlatformStru_Position& Position)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Position.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Position.InstrumentID);
        PosiDirection=Position.PosiDirection;
        HedgeFlag=Position.HedgeFlag;
    };

	PositionKey(const PlatformStru_OrderInfo& Order)
	{
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Order.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
		if(Order.CombOffsetFlag[0]==THOST_FTDC_OF_Open)
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		else if(Order.CombOffsetFlag[0]==THOST_FTDC_OF_Close||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_ForceClose||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_CloseYesterday||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_LocalForceClose)
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
		else 
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		HedgeFlag=Order.CombHedgeFlag[0];
	};
	PositionKey(const PlatformStru_TradeInfo& Trade)
	{
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),Trade.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Trade.InstrumentID);
		if(Trade.OffsetFlag==THOST_FTDC_OF_Open)
            PosiDirection = Trade.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		else if(Trade.OffsetFlag==THOST_FTDC_OF_Close||
				Trade.OffsetFlag==THOST_FTDC_OF_ForceClose||
				Trade.OffsetFlag==THOST_FTDC_OF_CloseToday||
				Trade.OffsetFlag==THOST_FTDC_OF_CloseYesterday||
				Trade.OffsetFlag==THOST_FTDC_OF_LocalForceClose)
			PosiDirection = Trade.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
		else 
			PosiDirection = Trade.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		HedgeFlag=Trade.HedgeFlag;
	};
	PositionKey(const PlatformStru_PositionDetail& PositionDetail)
	{
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),PositionDetail.Account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),PositionDetail.InstrumentID);
        PosiDirection = PositionDetail.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		HedgeFlag=PositionDetail.HedgeFlag;
	};

    PositionKey& operator= (const PositionKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));
        return *this;
    };


    bool operator< (const PositionKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(PosiDirection<r.PosiDirection)
			return true;
		if(PosiDirection>r.PosiDirection)
			return false;

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag) 
			return false;

        return false;
    }

    bool operator== (const PositionKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(PosiDirection!=r.PosiDirection)
            return false;

        if(HedgeFlag!=r.HedgeFlag)
            return false;

        return true;
    }
    
    bool operator!= (const PositionKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionKey& r) const
    {
        return !(*this < r ||*this == r);
    }
    bool IsSamePositionKey(const PlatformStru_PositionDetail& PositionDetail) const
    {
        int off,len;

        CTools_Ansi::mygettrimpos(PositionDetail.Account,off,len);
        if(strlen(Account)!=len||memcmp(Account,PositionDetail.Account,len)!=0)
            return false;

        CTools_Ansi::mygettrimpos(PositionDetail.InstrumentID,off,len);
        if(strlen(InstrumentID)!=len||memcmp(InstrumentID,PositionDetail.InstrumentID,len)!=0)
            return false;

        if( HedgeFlag != PositionDetail.HedgeFlag)
            return false;

        if( PosiDirection==THOST_FTDC_PD_Long && PositionDetail.Direction!=THOST_FTDC_D_Buy ||
            PosiDirection==THOST_FTDC_PD_Short && PositionDetail.Direction!=THOST_FTDC_D_Sell )
            return false;

        return true;
    }
    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,Direct%d,Hedge%d",
            Account,InstrumentID,PosiDirection,HedgeFlag);
        prlt[len]=0;
        return prlt;
    }

	CommonAccountType       Account;			//账号
    CommonInstrumentIDType  InstrumentID;      //合约ID
    char                    PosiDirection;     //买卖方向
    char                    HedgeFlag;         //投机套保标志
};

///标示一个组合持仓明细记录，由Account、InstrumentID、OpenDate、TradeID、ComTradeID, Direction、hedgeFlag定义唯一的组合持仓明细记录
struct PositionDetailCombKey
{
    PositionDetailCombKey()
    {
        memset(this,0,sizeof(*this));
    }
	PositionDetailCombKey(const char* account,
        const char* instrumentID,
        const char* openDate,
        const char* tradeID,
        const char* comTradeID,
        char direction,
        char hedgeFlag)
    {
        set(account,instrumentID,openDate,tradeID,comTradeID,direction,hedgeFlag);
    };

    PositionDetailCombKey(const PlatformStru_PositionDetailComb& r)
    {
        new(this) PositionDetailCombKey(r.Account,r.InstrumentID,r.OpenDate,r.TradeID,r.ComTradeID,r.Direction,r.HedgeFlag);
    };

	void set(const char* account,
        const char* instrumentID,
        const char* openDate,
        const char* tradeID,
        const char* comTradeID,
        char direction,
        char hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools_Ansi::mymemcpy_trim(OpenDate,sizeof(OpenDate),openDate);
        CTools_Ansi::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);
        CTools_Ansi::mymemcpy_trim(ComTradeID,sizeof(ComTradeID),comTradeID);

        Direction=direction;
        HedgeFlag=hedgeFlag;
    };

    PositionDetailCombKey& operator= (const PositionDetailCombKey& r)
    {
        memcpy((char*)this,(char*)&r,sizeof(*this));

        return *this;
    };


    //注意，通过OpenDate和TradeID的比较顺序，保证set<PositionDetailCombKey>对相同条件的持仓明细，按时间排序
    bool operator< (const PositionDetailCombKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag)
			return false;

        if(Direction<r.Direction)
			return true;
		if(Direction>r.Direction)
			return false;

        irlt=CTools_Ansi::mycompare_trynumeric(TradeID,r.TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools_Ansi::mycompare_trynumeric(ComTradeID,r.ComTradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const PositionDetailCombKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt!=0) return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt!=0) return false;

        irlt=strncmp(ComTradeID,r.ComTradeID,sizeof(ComTradeID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;
        if(HedgeFlag!=r.HedgeFlag)
            return false;

        return true;
    }
    
    bool operator!= (const PositionDetailCombKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionDetailCombKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=sprintf_s(prlt,rltsize-1,
            "%s,%s,%s,%s,%s,Direct%d,HedgeFlag%d",
            Account,InstrumentID,OpenDate,TradeID,ComTradeID,Direction,HedgeFlag);
        prlt[len]=0;
        return prlt;
    }


	CommonAccountType       Account;		    //账号
    CommonInstrumentIDType  InstrumentID;       //合约ID
    CommonDateType          OpenDate;           //开仓日期
    CommonTradeIDType       TradeID;            //成交ID(长度为20个字符，右对齐，左边补空格)
	CommonTradeIDType       ComTradeID;         //组合成交ID。对于查询回来的组合持仓明细，由ctp定；对于盘中成交产生的，用OrderSysID_xxxx临时定义，一个OrderSysID可能对应多对组合持仓明细。
    char                    Direction;          //买卖方向
    char                    HedgeFlag;          //投机套保标志
};

//用Account和InstrumentID描述的Key值
struct Account_InstrumentKey
{
    Account_InstrumentKey()
    {
        memset(this,0,sizeof(*this));
    }
    Account_InstrumentKey(const char* account,const char* instrumentID)
    {
        memset(this,0,sizeof(*this));

        if(account) CTools_Ansi::mymemcpy_trim(Account,sizeof(Account),account);
        if(instrumentID) CTools_Ansi::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
    };
    Account_InstrumentKey& operator= (const Account_InstrumentKey& r)
    {
        if (this == &r) return *this;
        memcpy((char*)this,(char*)&r,sizeof(*this));
        return *this;
    };

    bool operator< (const Account_InstrumentKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const Account_InstrumentKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        return true;
    }
    bool operator!= (const Account_InstrumentKey& r) const
    {
        return !(*this==r);
    }
    
    bool operator> (const Account_InstrumentKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;      //合约ID
};


//下面定义一个合约不同挂单量的数据结构
struct WaitOrderVolumeStru
{
    int         OpenVolume_Buy_Speculation;                 //开仓-买-投机
    int         CloseVolume_Buy_Speculation;                //平仓-买-投机
    int         CloseTodayVolume_Buy_Speculation;           //平今-买-投机
    int         CloseYesterdayVolume_Buy_Speculation;       //平昨-买-投机

    int         OpenVolume_Sell_Speculation;                //开仓-卖-投机
    int         CloseVolume_Sell_Speculation;               //平仓-卖-投机
    int         CloseTodayVolume_Sell_Speculation;          //平今-卖-投机
    int         CloseYesterdayVolume_Sell_Speculation;      //平昨-卖-投机

    int         OpenVolume_Buy_Hedge;                       //开仓-买-套保
    int         CloseVolume_Buy_Hedge;                      //平仓-买-套保
    int         CloseTodayVolume_Buy_Hedge;                 //平今-买-套保
    int         CloseYesterdayVolume_Buy_Hedge;             //平昨-买-套保

    int         OpenVolume_Sell_Hedge;                      //开仓-卖-套保
    int         CloseVolume_Sell_Hedge;                     //平仓-卖-套保
    int         CloseTodayVolume_Sell_Hedge;                //平今-卖-套保
    int         CloseYesterdayVolume_Sell_Hedge;            //平昨-卖-套保
};

/************服务器参数**begin********************************************************************************************************/
typedef struct tagServerParamCfgItem
{
	std::vector<std::string> TradeAddr;		//获取交易前置机地址
	std::vector<std::string> QuotAddr;		//获取行情前置机地址
	std::string BrokerID;					//获取经纪公司ID
	std::string InvestorID;					//登录的账户 
	std::string ServerName;					//中文
	int CTPSimulate;						//服务器是否CTP模拟服务器, 如是，则需要将合约对应的ExchangeID还原为真实的交易所ID(CTP都设为SHFE)
}SERVER_PARAM_CFG_ITEM, *LPSERVER_PARAM_CFG_ITEM;

typedef struct tagServerParamCfg
{
	std::vector<SERVER_PARAM_CFG_ITEM> vec;
	std::string UniserverAddr;
	std::string VersionStr;
	int LanguageId;
	int CurSvrGrpID;    //当前的服务器组ID
	//下面代理服务器参数设置
	std::string ProxyType;//socks4,socks5,http
	std::string ProxyAddr;
	std::string ProxyPort;
	std::string ProxyUser;
	std::string ProxyPwd;
}SERVER_PARAM_CFG,* LPSERVER_PARAM_CFG;
/************服务器参数**end********************************************************************************************************/


#ifdef WIN32
#pragma managed(pop)
#endif 
