
#include "stdafx.h"
#include "CFieldDetail.h"
#include "UserApiDataType.h"


const static char* __UNKOWN__="未知";

///买卖方向
const char* Client::CFieldDetail::Direction2String(char param)
{
	const static char* Buy="买";
	const static char* Sell="卖";
	switch(param)
	{
		case T_D_Buy:return Buy;
		case T_D_Sell:return Sell;
		default: return __UNKOWN__;
	}
}

///开平标志
const char* Client::CFieldDetail::OffsetFlag2String(char param)
{
	const static char* Open="开仓";
	const static char* Close="平仓";
	const static char* ForceClose="强平";
	const static char* CloseToday="平今";
	const static char* CloseYesterday="平昨";
	switch(param)
	{
		case T_OF_Open:		return Open;
		case T_OF_Close:		return Close;
		case T_OF_ForceClose:	return ForceClose;
		case T_OF_CloseToday:	return CloseToday;
		case T_OF_CloseYesterday:return CloseYesterday;
		default: return __UNKOWN__;
	}
}

///报单状态
const char* Client::CFieldDetail::OrderStatus2String(char param)
{
	const static char* AllTraded="全部成交";
	const static char* PartTradedQueueing="部分成交还在队列中";
	const static char* PartTradedNotQueueing="部分成交不在队列中";
	const static char* NoTradeQueueing="未成交还在队列中";
	const static char* NoTradeNotQueueing="未成交不在队列中";
	const static char* Canceled="撤单";
	const static char* Unknown="未知";
	const static char* NotTouched="条件未触发";
	const static char* Touched="条件已触发";
    const static char* Error="错误";
	switch(param)
	{
		case T_OST_AllTraded:				return AllTraded;
		case T_OST_PartTradedQueueing:		return PartTradedQueueing;
		case T_OST_PartTradedNotQueueing:	return PartTradedNotQueueing;
		case T_OST_NoTradeQueueing:		return NoTradeQueueing;
		case T_OST_NoTradeNotQueueing:		return NoTradeNotQueueing;
		case T_OST_Canceled:				return Canceled;
		case T_OST_Unknown:				return Unknown;
		case T_OST_NotTouched:				return NotTouched;
		case T_OST_Touched:				return Touched;
        //case T_OST_ERROE:				    return Error;
		default: return __UNKOWN__;
	}
}

///报单提交状态
const char* Client::CFieldDetail::OrderSubmitStatus2String(char param)
{
	const static char* InsertSubmitted="已经提交";
	const static char* CancelSubmitted="撤单已经提交";
	const static char* ModifySubmitted="修改已经提交";
	const static char* Accepted="已经接受";
	const static char* InsertRejected="报单已经被拒绝";
	const static char* CancelRejected="撤单已经被拒绝";
	const static char* ModifyRejected="改单已经被拒绝";

	switch(param)
	{
		case T_OSS_InsertSubmitted:	return InsertSubmitted;
		case T_OSS_CancelSubmitted:	return CancelSubmitted;
		case T_OSS_ModifySubmitted:	return ModifySubmitted;
		case T_OSS_Accepted:			return Accepted;
		case T_OSS_InsertRejected:		return InsertRejected;
		case T_OSS_CancelRejected:		return CancelRejected;
		case T_OSS_ModifyRejected:		return ModifyRejected;
		default: return __UNKOWN__;
	}
}

///报单价格条件
const char* Client::CFieldDetail::OrderPriceType2String(char param)
{
	const static char* AnyPrice="任意价";
	const static char* LimitPrice="限价";
	const static char* BestPrice="最优价";
	const static char* LastPrice="最新价";
	const static char* LastPricePlusOneTicks="最新价浮动上浮1个ticks";
	const static char* LastPricePlusTwoTicks="最新价浮动上浮2个ticks";
	const static char* LastPricePlusThreeTicks="最新价浮动上浮3个ticks";
	const static char* AskPrice1="卖一价";
	const static char* AskPrice1PlusOneTicks="卖一价浮动上浮1个ticks";
	const static char* AskPrice1PlusTwoTicks="卖一价浮动上浮2个ticks";
	const static char* AskPrice1PlusThreeTicks="卖一价浮动上浮3个ticks";
	const static char* BidPrice1="买一价";
	const static char* BidPrice1PlusOneTicks="买一价浮动上浮1个ticks";
	const static char* BidPrice1PlusTwoTicks="买一价浮动上浮2个ticks";
	const static char* BidPrice1PlusThreeTicks="买一价浮动上浮3个ticks";

	switch(param)
	{
		case T_OPT_AnyPrice:		return AnyPrice;
		case T_OPT_LimitPrice:		return LimitPrice;
		case T_OPT_BestPrice:		return BestPrice;
		case T_OPT_LastPrice:		return LastPrice;
		case T_OPT_LastPricePlusOneTicks:		return LastPricePlusOneTicks;
		case T_OPT_LastPricePlusTwoTicks:		return LastPricePlusTwoTicks;
		case T_OPT_LastPricePlusThreeTicks:	return LastPricePlusThreeTicks;
		case T_OPT_AskPrice1:					return AskPrice1;
		case T_OPT_AskPrice1PlusOneTicks:		return AskPrice1PlusOneTicks;
		case T_OPT_AskPrice1PlusTwoTicks:		return AskPrice1PlusTwoTicks;
		case T_OPT_AskPrice1PlusThreeTicks:	return AskPrice1PlusThreeTicks;
		case T_OPT_BidPrice1:					return BidPrice1;
		case T_OPT_BidPrice1PlusOneTicks:		return BidPrice1PlusOneTicks;
		case T_OPT_BidPrice1PlusTwoTicks:		return BidPrice1PlusTwoTicks;
		case T_OPT_BidPrice1PlusThreeTicks:	return BidPrice1PlusThreeTicks;
		default: return __UNKOWN__;
	}
}

///投机套保标志
const char* Client::CFieldDetail::HedgeFlag2String(char param)
{
	const static char* Speculation="投机";
	const static char* Hedge="套保";
	switch(param)
	{
		case T_HF_Speculation:return Speculation;
		case T_HF_Hedge:return Hedge;
		default: return __UNKOWN__;
	}
}

///持仓多空方向
const char* Client::CFieldDetail::PosiDirection2String(char param)
{
	const static char* Net="净";
	const static char* Long="多头";
	const static char* Short="空头";
	switch(param)
	{
		case T_PD_Net:return Net;
		case T_PD_Long:return Long;
		case T_PD_Short:return Short;
		default: return __UNKOWN__;
	}
}

///强平原因
const char* Client::CFieldDetail::ForceCloseReason2String(char param)
{
	const static char* NotForceClose="非强平";
	const static char* LackDeposit="资金不足";
	const static char* ClientOverPositionLimit="客户超仓";
	const static char* MemberOverPositionLimit="会员超仓";
	const static char* NotMultiple="持仓非整数倍";
	const static char* Violation="违规";
	const static char* Other="其它";

	switch(param)
	{
		case T_FCC_NotForceClose:	return NotForceClose;
		case T_FCC_LackDeposit:	return LackDeposit;
		case T_FCC_ClientOverPositionLimit:	return ClientOverPositionLimit;
		case T_FCC_MemberOverPositionLimit:	return MemberOverPositionLimit;
		case T_FCC_NotMultiple:	return NotMultiple;
		case T_FCC_Violation:		return Violation;
		case T_FCC_Other:			return Other;
		default: return __UNKOWN__;
	}
}

///报单类型
const char* Client::CFieldDetail::OrderType2String(char param)
{
	const static char* Normal="正常";
	const static char* DeriveFromQuote="报价衍生";
	const static char* DeriveFromCombination="组合衍生";
	const static char* Combination="组合报单";
	const static char* ConditionalOrder="条件单";
	const static char* Swap="互换单";

	switch(param)
	{
		case T_ORDT_Normal:			return Normal;
		case T_ORDT_DeriveFromQuote:	return DeriveFromQuote;
		case T_ORDT_DeriveFromCombination:	return DeriveFromCombination;
		case T_ORDT_Combination:		return Combination;
		case T_ORDT_ConditionalOrder:	return ConditionalOrder;
		case T_ORDT_Swap:				return Swap;
		default: return Normal;//return __UNKOWN__;
	}
}

///有效期类型
const char* Client::CFieldDetail::TimeCondition2String(char param)
{
	const static char* IOC="立即完成，否则撤销";
	const static char* GFS="本节有效";
	const static char* GFD="当日有效";
	const static char* GTD="指定日期前有效";
	const static char* GTC="撤销前有效";
	const static char* GFA="集合竞价有效";

	switch(param)
	{
		case T_TC_IOC:		return IOC;
		case T_TC_GFS:		return GFS;
		case T_TC_GFD:		return GFD;
		case T_TC_GTD:		return GTD;
		case T_TC_GTC:		return GTC;
		case T_TC_GFA:		return GFA;
		default: return __UNKOWN__;
	}
}

///成交量类型
const char* Client::CFieldDetail::VolumeCondition2String(char param)
{
	const static char* AV="任何数量";
	const static char* MV="最小数量";
	const static char* CV="全部数量";
	switch(param)
	{
		case T_VC_AV:return AV;
		case T_VC_MV:return MV;
		case T_VC_CV:return CV;
		default: return __UNKOWN__;
	}
}

///业务请求函数返回结果
const char* Client::CFieldDetail::ReturnCode2String(int param)
{
	const static char* code0="成功";
	const static char* code1="网络连接失败";
	const static char* code2="未处理请求超过许可数";
	const static char* code3="每秒发送请求数超过许可数";
	const static char* code98="帐号无交易权限";
	const static char* code99="无效的交易帐号";
	const static char* code999="其它原因失败";
	switch(param)
	{
		case 0:		return code0;
		case -1:	return code1;
		case -2:	return code2;
		case -3:	return code3;
		case -98:	return code98;
		case -99:	return code99;
		case -999:	return code999;
		default:	return __UNKOWN__;
	}
}

///品种类型
const char* Client::CFieldDetail::ProductType2String(char param)
{
	const static char* Futures="期货";
	const static char* Options="期权";
	const static char* Combination="组合";
	const static char* Spot="即期";
	const static char* EFP="期转现";


	switch(param)
	{
		case T_PC_Futures:			return Futures;
		case T_PC_Options:	        return Options;
		case T_PC_Combination:  	return Combination;
		case T_PC_Spot:		    return Spot;
		case T_PC_EFP:	            return EFP;
		default: return __UNKOWN__;
	}
}



const char* Client::CFieldDetail::UserForceClose2String(int UserForceClose)
{
	const static char* YES="是";
	const static char* NO="否";
	switch( UserForceClose)
	{
		case 0:return NO;
		case 1:return YES;
		default:return __UNKOWN__;
	}
}

///成交类型
const char* Client::CFieldDetail::TradeType2String(char TradeType)
{
	const static char* Common="普通成交";
	const static char* OptionsExecution="期权执行";
	const static char* OTC="OTC成交";
	const static char* EFPDerived="期转现衍生成交";
	const static char* CombinationDerived="组合衍生成交";

	switch(TradeType)
	{
		case T_TRDT_Common:			return Common;
		case T_TRDT_OptionsExecution:	return OptionsExecution;
		case T_TRDT_OTC:  				return OTC;
		case T_TRDT_EFPDerived:		return EFPDerived;
		case T_TRDT_CombinationDerived:return CombinationDerived;
		case 0x00:								return Common;
		default: return __UNKOWN__;
	}
}

//交易角色
const char* Client::CFieldDetail::TraderRole2String(int param)
{
	const static char* TraderRoleBroker="代理";
	const static char* TraderRoleHost="自营";
	const static char* TraderRoleMaker="做市商";
	switch(param)
	{
	case T_ER_Broker:			return TraderRoleBroker;
	case T_ER_Host:	        return TraderRoleHost;
	case T_ER_Maker:  		    return TraderRoleMaker;
	default: return __UNKOWN__;
	}
}


//成交来源
const char* Client::CFieldDetail::TradeSourceString(int param)
{
	const static char* TradeSourceNormal="交易所普通回报";
	const static char* TradeSourceQuery="查询";	
	switch(param)
	{
	case T_TSRC_NORMAL :			return TradeSourceNormal;
	case T_TSRC_QUERY:	        return TradeSourceQuery;
	default: return __UNKOWN__;
	}
}


//成交价来源
const char* Client::CFieldDetail::PriceType2String(int param)
{
	const static char* PriceTypeLast="前成交价";
	const static char* PriceTypeBuy="买委托价";
	const static char* PriceTypeSell="卖委托价";
	switch(param)
	{
	case T_PSRC_LastPrice:			return PriceTypeLast;
	case T_PSRC_Buy:	        return PriceTypeBuy;
	case T_PSRC_Sell:  		    return PriceTypeSell;
	default: return __UNKOWN__;
	}
}

