// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DB763F8D_4DC5_4C9D_BA3F_96CB29EC3F57__INCLUDED_)
#define AFX_STDAFX_H__DB763F8D_4DC5_4C9D_BA3F_96CB29EC3F57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "..\inc\Module-Interface\interfaceDefines.h"
#define MASTERNAMEDPIPESTRING "\\\\.\\pipe\\FastTrader\\v0_1\\Master"

#include "..\inc\ctp\ThostFtdcMdApi.h"
#include "..\inc\ctp\ThostFtdcTraderApi.h"
#include "..\inc\ctp\ThostFtdcUserApiDataType.h"
#include "..\inc\ctp\ThostFtdcUserApiStruct.h"


#define ORDERCLASS_NewOrder 'N'
#define ORDERCLASS_ReplaceOrder 'R'

typedef char CommonOrderClassType;
typedef char CommonAccountType[32];
typedef char CommonLocalRequestIDType[32];
typedef char CommonLocalRefIDType[32];
typedef char CommonExchangeIDType[32];
typedef char CommonOrderRefType[32];

typedef char PatsystemAccountType[50];

typedef char InstrumentIDType[64];

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

    //FasttraderID,对于每一条记录的Key，这是一个唯一编号
    long FTID;         
    //每次数据内容发生变化，UpdateSeq加一
    long UpdateSeq;

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
    int         CloseOrderVolume;           //平仓、强平、本地强平挂单量
    int         CloseTodayOrderVolume;      //平今挂单量
    int         CloseYesterdayOrderVolume;  //平昨挂单量
    int         CanCloseVolume;             //可平量Position-TotalCloseOrderVolume
    int         CanCloseTodayVolume;        //可平今量TodayPosition-CloseTodayOrderVolume
    int         CanCloseydVolume;           //可平昨量YdPosition-CloseYesterdayOrderVolume

    double      AveragePositionPrice;       //持仓均价(持仓成本/持仓量)
    double      AverageOpenPrice;           //开仓均价((今日开仓成本+历史开仓成本)/总开仓量)

    int         TodayCloseYdVolume;         //今平历史仓量
	int         YdPositionRemain;	            //上日剩余仓位，从持仓明细计算

    double      CurPrice;                   //现价

    //计算持仓盈亏用的现价
    double      CurPriceUsedByCalcu;                   

    //FasttraderID,对于每一条记录的Key，这是一个唯一编号
    long FTID;                                                
    //每次数据内容发生变化，UpdateSeq加一
    long UpdateSeq;

};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DB763F8D_4DC5_4C9D_BA3F_96CB29EC3F57__INCLUDED_)
