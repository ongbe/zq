/***************************************************************************//**
*   @file         FieldValueTool.cpp
*   @brief        将各个业务消息的字段转为用于GUI显示的字符串
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/10/11
********************************************************************************/
#include "stdafx.h"
#include "FieldValueTool.h"
#include "globalDefines.h"
#include "..\inc\cfgmgr\CfgMgr.h"
#include "wx/string.h"


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

#define COMPARELANGUAGE(strsrc, strid, retvalue) for(int i=LANGUAGE_MIN; i<=LANGUAGE_MAX; i++) {\
		wxString strValue=LOADSTRING_LANGUAGE(strid,i);\
		if(strValue==strsrc) {\
			if(language) \
				*language=i;\
			return retvalue;\
		}\
	}


///买卖方向
const wxString CFieldValueTool::Direction2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_D_Buy:
		return LOADSTRING(CFieldValueTool_Buy);//language==LANGUAGE_CHINESE ? Buy_Cn : Buy_En;
		break;
	case THOST_FTDC_D_Sell:
		return LOADSTRING(CFieldValueTool_Sell);//language==LANGUAGE_CHINESE ? Sell_Cn : Sell_En;
		break;
	default: 
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}

const char CFieldValueTool::String2Direction(wxString Dir,int* language)
{
	COMPARELANGUAGE(Dir, CFieldValueTool_Buy, THOST_FTDC_D_Buy)
	COMPARELANGUAGE(Dir, CFieldValueTool_Sell, THOST_FTDC_D_Sell)
	if(language) *language=0;
	return ' ';
}


///持仓多空方向
const wxString CFieldValueTool::PosiDirection2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_PD_Net:
		return LOADSTRING(CFieldValueTool_Net);//language==LANGUAGE_CHINESE ?  Net_Cn : Net_En;
		break;
	case THOST_FTDC_PD_Long:
		return LOADSTRING(CFieldValueTool_Long);//language==LANGUAGE_CHINESE ?  Long_Cn : Long_En;
		break;
	case THOST_FTDC_PD_Short:
		return LOADSTRING(CFieldValueTool_Short);//language==LANGUAGE_CHINESE ?  Short_Cn : Short_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2PosiDirection(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_Net, THOST_FTDC_PD_Net)
	COMPARELANGUAGE(Type, CFieldValueTool_Long, THOST_FTDC_PD_Long)
	COMPARELANGUAGE(Type, CFieldValueTool_Short, THOST_FTDC_PD_Short)
	if(language) *language=0;
	return 0;
}


///开平标志
const wxString CFieldValueTool::OffsetFlag2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_OF_Open:
		return LOADSTRING(CFieldValueTool_Open);//language==LANGUAGE_CHINESE ?  Open_Cn : Open_En;
		break;
	case THOST_FTDC_OF_Close:
		return LOADSTRING(CFieldValueTool_Close);//language==LANGUAGE_CHINESE ?  Close_Cn : Close_En;
		break;
	case THOST_FTDC_OF_ForceClose:
		return LOADSTRING(CFieldValueTool_ForceClose);//language==LANGUAGE_CHINESE ?  ForceClose_Cn : ForceClose_En;
		break;
	case THOST_FTDC_OF_CloseToday:
		return LOADSTRING(CFieldValueTool_CloseToday);//language==LANGUAGE_CHINESE ?  CloseToday_Cn : CloseToday_En;
		break;
	case THOST_FTDC_OF_CloseYesterday:
		return LOADSTRING(CFieldValueTool_CloseYesterday);//language==LANGUAGE_CHINESE ?  CloseYesterday_Cn : CloseYesterday_En;;
		break;
	default: 
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2OffsetFlag(wxString flag,int* language)
{
	COMPARELANGUAGE(flag, CFieldValueTool_Open, THOST_FTDC_OF_Open)
	//if(flag==Open_Cn)			{if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_OF_Open;}
	COMPARELANGUAGE(flag, CFieldValueTool_Close, THOST_FTDC_OF_Close)
	//if(flag==Close_Cn)			{if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_OF_Close;}
	COMPARELANGUAGE(flag, CFieldValueTool_ForceClose, THOST_FTDC_OF_ForceClose)
	//if(flag==ForceClose_Cn)		{if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_OF_ForceClose;}
	COMPARELANGUAGE(flag, CFieldValueTool_CloseToday, THOST_FTDC_OF_CloseToday)
	//if(flag==CloseToday_Cn)		{if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_OF_CloseToday;}
	COMPARELANGUAGE(flag, CFieldValueTool_CloseYesterday, THOST_FTDC_OF_CloseYesterday)
	//if(flag==CloseYesterday_Cn)	{if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_OF_CloseYesterday;}
	//if(flag==Open_En)			{if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_OF_Open;}
	//if(flag==Close_En)			{if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_OF_Close;}
	//if(flag==ForceClose_En)		{if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_OF_ForceClose;}
	//if(flag==CloseToday_En)		{if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_OF_CloseToday;}
	//if(flag==CloseYesterday_En)	{if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_OF_CloseYesterday;}
	if(language) *language=0;
	return ' ';
}
///报单状态
const wxString CFieldValueTool::OrderStatus2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_OST_AllTraded:
		return LOADSTRING(CFieldValueTool_AllTraded);//language==LANGUAGE_CHINESE ?  AllTraded_Cn : AllTraded_En;
		break;
	case THOST_FTDC_OST_PartTradedQueueing:
		return LOADSTRING(CFieldValueTool_PartTradedQueueing);//language==LANGUAGE_CHINESE ?  PartTradedQueueing_Cn : PartTradedQueueing_En;
		break;
	case THOST_FTDC_OST_PartTradedNotQueueing:
		return LOADSTRING(CFieldValueTool_PartTradedNotQueueing);//language==LANGUAGE_CHINESE ?  PartTradedNotQueueing_Cn : PartTradedNotQueueing_En;
		break;
	case THOST_FTDC_OST_NoTradeQueueing:
		return LOADSTRING(CFieldValueTool_NoTradeQueueing);//language==LANGUAGE_CHINESE ?  NoTradeQueueing_Cn : NoTradeQueueing_En;
		break;
	case THOST_FTDC_OST_NoTradeNotQueueing:
		return LOADSTRING(CFieldValueTool_NoTradeNotQueueing);//language==LANGUAGE_CHINESE ?  NoTradeNotQueueing_Cn : NoTradeNotQueueing_En;
		break;
	case THOST_FTDC_OST_Canceled:
		return LOADSTRING(CFieldValueTool_Canceled);//language==LANGUAGE_CHINESE ?  Canceled_Cn : Canceled_En;
		break;
	case THOST_FTDC_OST_Unknown:
		return LOADSTRING(CFieldValueTool_Unknown);//language==LANGUAGE_CHINESE ?  Unknown_Cn : Unknown_En;
		break;
	case THOST_FTDC_OST_NotTouched:
		return LOADSTRING(CFieldValueTool_NotTouched);//language==LANGUAGE_CHINESE ?  NotTouched_Cn : NotTouched_En;
		break;
	case THOST_FTDC_OST_Touched:
		return LOADSTRING(CFieldValueTool_Touched);//language==LANGUAGE_CHINESE ?  Touched_Cn : Touched_En;
		break;
	case THOST_FTDC_OST_ERROE:
		return LOADSTRING(CFieldValueTool_Error);//language==LANGUAGE_CHINESE ?  Error_Cn : Error_En;
		break;
	default: 
		return LOADSTRING(CFieldValueTool_Unknown);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2OrderStatus(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_AllTraded, THOST_FTDC_OST_AllTraded)
	//if(Type==AllTraded_Cn)						{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_OST_AllTraded;}
	COMPARELANGUAGE(Type, CFieldValueTool_PartTradedQueueing, THOST_FTDC_OST_PartTradedQueueing)
	//if(Type==PartTradedQueueing_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_PartTradedQueueing;}
	COMPARELANGUAGE(Type, CFieldValueTool_PartTradedNotQueueing, THOST_FTDC_OST_PartTradedNotQueueing)
	//if(Type==PartTradedNotQueueing_Cn)			{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_PartTradedNotQueueing;}
	COMPARELANGUAGE(Type, CFieldValueTool_NoTradeQueueing, THOST_FTDC_OST_NoTradeQueueing)
	//if(Type==NoTradeQueueing_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_NoTradeQueueing;}
	COMPARELANGUAGE(Type, CFieldValueTool_NoTradeNotQueueing, THOST_FTDC_OST_NoTradeNotQueueing)
	//if(Type==NoTradeNotQueueing_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_NoTradeNotQueueing;}
	COMPARELANGUAGE(Type, CFieldValueTool_Canceled, THOST_FTDC_OST_Canceled)
	//if(Type==Canceled_Cn)						{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_Canceled;}
	COMPARELANGUAGE(Type, CFieldValueTool_Unknown, THOST_FTDC_OST_Unknown)
	//if(Type==Unknown_Cn)						{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_Unknown;}
	COMPARELANGUAGE(Type, CFieldValueTool_NotTouched, THOST_FTDC_OST_NotTouched)
	//if(Type==NotTouched_Cn)						{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_NotTouched;}
	COMPARELANGUAGE(Type, CFieldValueTool_Touched, THOST_FTDC_OST_Touched)
	//if(Type==Touched_Cn)						{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_Touched;}
	COMPARELANGUAGE(Type, CFieldValueTool_ERROE, THOST_FTDC_OST_ERROE)
	//if(Type==Error_Cn)							{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OST_ERROE;}
	//if(Type==AllTraded_En)						{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_AllTraded;}
	//if(Type==PartTradedQueueing_En)				{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_PartTradedQueueing;}
	//if(Type==PartTradedNotQueueing_En)			{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_PartTradedNotQueueing;}
	//if(Type==NoTradeQueueing_En)				{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_NoTradeQueueing;}
	//if(Type==NoTradeNotQueueing_En)				{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_NoTradeNotQueueing;}
	//if(Type==Canceled_En)						{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_Canceled;}
	//if(Type==Unknown_En)						{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_Unknown;}
	//if(Type==NotTouched_En)						{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_NotTouched;}
	//if(Type==Touched_En)						{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_Touched;}
	//if(Type==Error_En)							{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OST_ERROE;}
	if(language) *language=0;
	return 0;
}
///报单提交状态
const wxString CFieldValueTool::OrderSubmitStatus2String(char param,int language)
{
	switch(param) {
		case THOST_FTDC_OSS_InsertSubmitted:
			return LOADSTRING(CFieldValueTool_InsertSubmitted);//language==LANGUAGE_CHINESE ?  InsertSubmitted_Cn : InsertSubmitted_En;
			break;
		case THOST_FTDC_OSS_CancelSubmitted:
			return LOADSTRING(CFieldValueTool_CancelSubmitted);//language==LANGUAGE_CHINESE ?  CancelSubmitted_Cn : CancelSubmitted_En;
			break;
		case THOST_FTDC_OSS_ModifySubmitted:
			return LOADSTRING(CFieldValueTool_ModifySubmitted);//language==LANGUAGE_CHINESE ?  ModifySubmitted_Cn : ModifySubmitted_En;
			break;
		case THOST_FTDC_OSS_Accepted:
			return LOADSTRING(CFieldValueTool_Accepted);//language==LANGUAGE_CHINESE ?  Accepted_Cn : Accepted_En;
			break;
		case THOST_FTDC_OSS_InsertRejected:
			return LOADSTRING(CFieldValueTool_InsertRejected);//language==LANGUAGE_CHINESE ?  InsertRejected_Cn : InsertRejected_En;
			break;
		case THOST_FTDC_OSS_CancelRejected:
			return LOADSTRING(CFieldValueTool_CancelRejected);//language==LANGUAGE_CHINESE ?  CancelRejected_Cn : CancelRejected_En;
			break;
		case THOST_FTDC_OSS_ModifyRejected:
			return LOADSTRING(CFieldValueTool_ModifyRejected);//language==LANGUAGE_CHINESE ?  ModifyRejected_Cn : ModifyRejected_En;
			break;
		default:
			return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
			break;
	}
}
const char CFieldValueTool::String2OrderSubmitStatus(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_InsertSubmitted, THOST_FTDC_OSS_InsertSubmitted)
	//if(Type==InsertSubmitted_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OSS_InsertSubmitted;}
	COMPARELANGUAGE(Type, CFieldValueTool_CancelSubmitted, THOST_FTDC_OSS_CancelSubmitted)
	//if(Type==CancelSubmitted_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OSS_CancelSubmitted;}
	COMPARELANGUAGE(Type, CFieldValueTool_ModifySubmitted, THOST_FTDC_OSS_ModifySubmitted)
	//if(Type==ModifySubmitted_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OSS_ModifySubmitted;}
	COMPARELANGUAGE(Type, CFieldValueTool_Accepted, THOST_FTDC_OSS_Accepted)
	//if(Type==Accepted_Cn)						{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OSS_Accepted;}
	COMPARELANGUAGE(Type, CFieldValueTool_InsertRejected, THOST_FTDC_OSS_InsertRejected)
	//if(Type==InsertRejected_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OSS_InsertRejected;}
	COMPARELANGUAGE(Type, CFieldValueTool_CancelRejected, THOST_FTDC_OSS_CancelRejected)
	//if(Type==CancelRejected_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OSS_CancelRejected;}
	COMPARELANGUAGE(Type, CFieldValueTool_ModifyRejected, THOST_FTDC_OSS_ModifyRejected)
	//if(Type==ModifyRejected_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return 	THOST_FTDC_OSS_ModifyRejected;}

	//if(Type==InsertSubmitted_En)				{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OSS_InsertSubmitted;}
	//if(Type==CancelSubmitted_En)				{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OSS_CancelSubmitted;}
	//if(Type==ModifySubmitted_En)				{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OSS_ModifySubmitted;}
	//if(Type==Accepted_En)						{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OSS_Accepted;}
	//if(Type==InsertRejected_En)					{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OSS_InsertRejected;}
	//if(Type==CancelRejected_En)					{ if(language) *language=LANGUAGE_ENGLISH;return 	THOST_FTDC_OSS_CancelRejected;}
	//if(Type==ModifyRejected_En)					{ if(language) *language=LANGUAGE_ENGLISH;return	THOST_FTDC_OSS_ModifyRejected;}
	if(language) *language=0;
	return 0;

}
///报单价格条件
const wxString CFieldValueTool::OrderPriceType2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_OPT_AnyPrice:
		return LOADSTRING(CFieldValueTool_AnyPrice);//language==LANGUAGE_CHINESE ?  AnyPrice_Cn : AnyPrice_En;
		break;
	case THOST_FTDC_OPT_LimitPrice:
		return LOADSTRING(CFieldValueTool_LimitPrice);//language==LANGUAGE_CHINESE ?  LimitPrice_Cn : LimitPrice_En;
		break;
	case THOST_FTDC_OPT_BestPrice:
		return LOADSTRING(CFieldValueTool_BestPrice);//BestPrice;
		break;
	case THOST_FTDC_OPT_LastPrice:
		return LOADSTRING(CFieldValueTool_LastPrice);//LastPrice;
		break;
	case THOST_FTDC_OPT_LastPricePlusOneTicks:
		return LOADSTRING(CFieldValueTool_LastPricePlusOneTicks);//LastPricePlusOneTicks;
		break;
	case THOST_FTDC_OPT_LastPricePlusTwoTicks:
		return LOADSTRING(CFieldValueTool_LastPricePlusTwoTicks);//LastPricePlusTwoTicks;
		break;
	case THOST_FTDC_OPT_LastPricePlusThreeTicks:
		return LOADSTRING(CFieldValueTool_LastPricePlusThreeTicks);//LastPricePlusThreeTicks;
		break;
	case THOST_FTDC_OPT_AskPrice1:
		return LOADSTRING(CFieldValueTool_AskPrice1);//AskPrice1;
		break;
	case THOST_FTDC_OPT_AskPrice1PlusOneTicks:
		return LOADSTRING(CFieldValueTool_AskPrice1PlusOneTicks);//AskPrice1PlusOneTicks;
		break;
	case THOST_FTDC_OPT_AskPrice1PlusTwoTicks:
		return LOADSTRING(CFieldValueTool_AskPrice1PlusTwoTicks);//AskPrice1PlusTwoTicks;
		break;
	case THOST_FTDC_OPT_AskPrice1PlusThreeTicks:
		return LOADSTRING(CFieldValueTool_AskPrice1PlusThreeTicks);//AskPrice1PlusThreeTicks;
		break;
	case THOST_FTDC_OPT_BidPrice1:
		return LOADSTRING(CFieldValueTool_BidPrice1);//BidPrice1;
		break;
	case THOST_FTDC_OPT_BidPrice1PlusOneTicks:
		return LOADSTRING(CFieldValueTool_BidPrice1PlusOneTicks);//BidPrice1PlusOneTicks;
		break;
	case THOST_FTDC_OPT_BidPrice1PlusTwoTicks:
		return LOADSTRING(CFieldValueTool_BidPrice1PlusTwoTicks);//BidPrice1PlusTwoTicks;
		break;
	case THOST_FTDC_OPT_BidPrice1PlusThreeTicks:
		return LOADSTRING(CFieldValueTool_BidPrice1PlusThreeTicks);//BidPrice1PlusThreeTicks;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2OrderPriceType(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_AnyPrice, THOST_FTDC_OPT_AnyPrice)
	//if(Type==AnyPrice_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_OPT_AnyPrice;}                        	
	COMPARELANGUAGE(Type, CFieldValueTool_LimitPrice, THOST_FTDC_OPT_LimitPrice)
	//if(Type==LimitPrice_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_OPT_LimitPrice;}                      	
	COMPARELANGUAGE(Type, CFieldValueTool_BestPrice, THOST_FTDC_OPT_BestPrice)
	//if(Type==BestPrice)					{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_OPT_BestPrice;}
	COMPARELANGUAGE(Type, CFieldValueTool_LastPrice, THOST_FTDC_OPT_LastPrice)
	//if(Type==LastPrice)					{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_OPT_LastPrice;}
	COMPARELANGUAGE(Type, CFieldValueTool_LastPricePlusOneTicks, THOST_FTDC_OPT_LastPricePlusOneTicks)
	//if(Type==LastPricePlusOneTicks)		{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_OPT_LastPricePlusOneTicks;}
	COMPARELANGUAGE(Type, CFieldValueTool_LastPricePlusTwoTicks, THOST_FTDC_OPT_LastPricePlusTwoTicks)
	//if(Type==LastPricePlusTwoTicks )	{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_OPT_LastPricePlusTwoTicks;}
	COMPARELANGUAGE(Type, CFieldValueTool_LastPricePlusThreeTicks, THOST_FTDC_OPT_LastPricePlusThreeTicks)
	//if(Type==LastPricePlusThreeTicks )	{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_OPT_LastPricePlusThreeTicks;}
	COMPARELANGUAGE(Type, CFieldValueTool_AskPrice1, THOST_FTDC_OPT_AskPrice1)
	//if(Type==AskPrice1   )				{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_OPT_AskPrice1;}
	COMPARELANGUAGE(Type, CFieldValueTool_AskPrice1PlusOneTicks, THOST_FTDC_OPT_AskPrice1PlusOneTicks)
	//if(Type==AskPrice1PlusOneTicks )	{ if(language) *language=LANGUAGE_CHINESE;return   	THOST_FTDC_OPT_AskPrice1PlusOneTicks;}
	COMPARELANGUAGE(Type, CFieldValueTool_AskPrice1PlusTwoTicks, THOST_FTDC_OPT_AskPrice1PlusTwoTicks)
	//if(Type==AskPrice1PlusTwoTicks )	{ if(language) *language=LANGUAGE_CHINESE;return   	THOST_FTDC_OPT_AskPrice1PlusTwoTicks;}
	COMPARELANGUAGE(Type, CFieldValueTool_AskPrice1PlusThreeTicks, THOST_FTDC_OPT_AskPrice1PlusThreeTicks)
	//if(Type==AskPrice1PlusThreeTicks )	{ if(language) *language=LANGUAGE_CHINESE;return  THOST_FTDC_OPT_AskPrice1PlusThreeTicks;}
	COMPARELANGUAGE(Type, CFieldValueTool_BidPrice1, THOST_FTDC_OPT_BidPrice1)
	//if(Type==BidPrice1      )			{ if(language) *language=LANGUAGE_CHINESE;return  THOST_FTDC_OPT_BidPrice1;}
	COMPARELANGUAGE(Type, CFieldValueTool_BidPrice1PlusOneTicks, THOST_FTDC_OPT_BidPrice1PlusOneTicks)
	//if(Type==BidPrice1PlusOneTicks  )	{ if(language) *language=LANGUAGE_CHINESE;return  THOST_FTDC_OPT_BidPrice1PlusOneTicks;}
	COMPARELANGUAGE(Type, CFieldValueTool_BidPrice1PlusTwoTicks, THOST_FTDC_OPT_BidPrice1PlusTwoTicks)
	//if(Type==BidPrice1PlusTwoTicks )	{ if(language) *language=LANGUAGE_CHINESE;return  THOST_FTDC_OPT_BidPrice1PlusTwoTicks;}
	COMPARELANGUAGE(Type, CFieldValueTool_BidPrice1PlusThreeTicks, THOST_FTDC_OPT_BidPrice1PlusThreeTicks)
	//if(Type==BidPrice1PlusThreeTicks )	{ if(language) *language=LANGUAGE_CHINESE;return  THOST_FTDC_OPT_BidPrice1PlusThreeTicks;}
	//if(Type==AnyPrice_En)				{ if(language) *language=LANGUAGE_ENGLISH;return  THOST_FTDC_OPT_AnyPrice;}                         	
	//if(Type==LimitPrice_En)				{ if(language) *language=LANGUAGE_ENGLISH;return  THOST_FTDC_OPT_LimitPrice;}  
	if(language) *language=0;
	return 0;
}
///投机套保标志
const wxString CFieldValueTool::HedgeFlag2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_HF_Speculation:
		return LOADSTRING(CFieldValueTool_Speculation);//language==LANGUAGE_CHINESE ?  Speculation_Cn : Speculation_En;
		break;
	case THOST_FTDC_HF_Hedge:
		return LOADSTRING(CFieldValueTool_Hedge);//language==LANGUAGE_CHINESE ?  Hedge_Cn : Hedge_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2HedgeFlag(wxString Dir,int* language)
{
	COMPARELANGUAGE(Dir, CFieldValueTool_Speculation, THOST_FTDC_HF_Speculation)
	//if(Dir==Speculation_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_HF_Speculation;}
	COMPARELANGUAGE(Dir, CFieldValueTool_Hedge, THOST_FTDC_HF_Hedge)
	//if(Dir==Hedge_Cn)			{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_HF_Hedge;}
	//if(Dir==Speculation_En)		{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_HF_Speculation;}
	//if(Dir==Hedge_En)			{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_HF_Hedge;}
	if(language) *language=0;
	return 0;
}
///用户强平标志
const wxString CFieldValueTool::UserForceClose2String(int UserForceClose,int language)
{
	switch( UserForceClose) {
	case 0:
		return LOADSTRING(CFieldValueTool_NO);//language==LANGUAGE_CHINESE ?  NO_Cn : NO_En;
	case 1:
		return LOADSTRING(CFieldValueTool_YES);//language==LANGUAGE_CHINESE ?  YES_Cn : YES_En;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
	}
}
const int CFieldValueTool::String2UserForceClose(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_YES, 1)
	//if(Type==YES_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return 1;}
	COMPARELANGUAGE(Type, CFieldValueTool_NO, 0)
	//if(Type==NO_Cn)						{ if(language) *language=LANGUAGE_CHINESE;return 0;}
	//if(Type==YES_En)					{ if(language) *language=LANGUAGE_ENGLISH;return 1;}
	//if(Type==NO_En)						{ if(language) *language=LANGUAGE_ENGLISH;return 0;}
	if(language) *language=0;
	return 0;
}
///成交类型
const wxString CFieldValueTool::TradeType2String(char TradeType,int language)
{
	switch(TradeType)
	{
	case 0x00:
	case THOST_FTDC_TRDT_Common:			
		return LOADSTRING(CFieldValueTool_Common);//language==LANGUAGE_CHINESE ?  Common_Cn : Common_En;
		break;
	case THOST_FTDC_TRDT_OptionsExecution:
		return LOADSTRING(CFieldValueTool_OptionsExecution);//language==LANGUAGE_CHINESE ?  OptionsExecution_Cn : OptionsExecution_En;
		break;
	case THOST_FTDC_TRDT_OTC:
		return LOADSTRING(CFieldValueTool_OTC);//language==LANGUAGE_CHINESE ?  OTC_Cn : OTC_En;
		break;
	case THOST_FTDC_TRDT_EFPDerived:
		return LOADSTRING(CFieldValueTool_EFPDerived);//language==LANGUAGE_CHINESE ?  EFPDerived_Cn : EFPDerived_En;
		break;
	case THOST_FTDC_TRDT_CombinationDerived:
		return LOADSTRING(CFieldValueTool_CombinationDerived);//language==LANGUAGE_CHINESE ?  CombinationDerived_Cn : CombinationDerived_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}

const wxString CFieldValueTool::ExchangeID2Name(char* ExchangeID,int language)
{
    char buf[256];
    if(strlen(ExchangeID)>sizeof(buf)/2) return wxString(ExchangeID);
    sprintf(buf,"CFieldValueTool_ExchangeName_%s",ExchangeID);
    std::string tmpstr(buf);
    std::string rltstr=LOADSTRING_TEXT(tmpstr);
    if(rltstr!=tmpstr) return wxString(rltstr);
    else return wxString(ExchangeID);
}

const char CFieldValueTool::String2TradeType(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_Common, THOST_FTDC_TRDT_Common)
	//if(Type==Common_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_TRDT_Common;}
	COMPARELANGUAGE(Type, CFieldValueTool_OptionsExecution, THOST_FTDC_TRDT_OptionsExecution)
	//if(Type==OptionsExecution_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_TRDT_OptionsExecution;}
	COMPARELANGUAGE(Type, CFieldValueTool_OTC, THOST_FTDC_TRDT_OTC)
	//if(Type==OTC_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_TRDT_OTC;}
	COMPARELANGUAGE(Type, CFieldValueTool_EFPDerived, THOST_FTDC_TRDT_EFPDerived)
	//if(Type==EFPDerived_Cn)				{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_TRDT_EFPDerived;}
	COMPARELANGUAGE(Type, CFieldValueTool_CombinationDerived, THOST_FTDC_TRDT_CombinationDerived)
	//if(Type==CombinationDerived_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_TRDT_CombinationDerived;}
	//if(Type==Common_En)					{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_TRDT_Common;}
	//if(Type==OptionsExecution_En)		{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_TRDT_OptionsExecution;}
	//if(Type==OTC_En)					{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_TRDT_OTC;}
	//if(Type==EFPDerived_En)				{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_TRDT_EFPDerived;}
	//if(Type==CombinationDerived_En)		{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_TRDT_CombinationDerived;}
	if(language) *language=0;
	return 0;
}

///强平原因
const wxString CFieldValueTool::ForceCloseReason2String(char param,int language)
{
	switch(param)
	{
	case THOST_FTDC_FCC_NotForceClose:
		return LOADSTRING(CFieldValueTool_NotForceClose);//language==LANGUAGE_CHINESE ?  NotForceClose_Cn : NotForceClose_En;
		break;
	case THOST_FTDC_FCC_LackDeposit:
		return LOADSTRING(CFieldValueTool_LackDeposit);//language==LANGUAGE_CHINESE ?  LackDeposit_Cn : LackDeposit_En;
		break;
	case THOST_FTDC_FCC_ClientOverPositionLimit:
		return LOADSTRING(CFieldValueTool_ClientOverPositionLimit);//language==LANGUAGE_CHINESE ?  ClientOverPositionLimit_Cn : ClientOverPositionLimit_En;
		break;
	case THOST_FTDC_FCC_MemberOverPositionLimit:
		return LOADSTRING(CFieldValueTool_MemberOverPositionLimit);//language==LANGUAGE_CHINESE ?  MemberOverPositionLimit_Cn : MemberOverPositionLimit_En;
		break;
	case THOST_FTDC_FCC_NotMultiple:
		return LOADSTRING(CFieldValueTool_NotMultiple);//language==LANGUAGE_CHINESE ?  NotMultiple_Cn : NotMultiple_En;
		break;
	case THOST_FTDC_FCC_Violation:
		return LOADSTRING(CFieldValueTool_Violation);//language==LANGUAGE_CHINESE ?  Violation_Cn : Violation_En;
		break;
	case THOST_FTDC_FCC_Other:
		return LOADSTRING(CFieldValueTool_Other);//language==LANGUAGE_CHINESE ?  Other_Cn : Other_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2ForceCloseReason(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_NotForceClose, THOST_FTDC_FCC_NotForceClose)
	//if(Type==NotForceClose_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_FCC_NotForceClose;}
	COMPARELANGUAGE(Type, CFieldValueTool_LackDeposit, THOST_FTDC_FCC_LackDeposit)
	//if(Type==LackDeposit_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_FCC_LackDeposit;}
	COMPARELANGUAGE(Type, CFieldValueTool_ClientOverPositionLimit, THOST_FTDC_FCC_ClientOverPositionLimit)
	//if(Type==ClientOverPositionLimit_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_FCC_ClientOverPositionLimit;}
	COMPARELANGUAGE(Type, CFieldValueTool_MemberOverPositionLimit, THOST_FTDC_FCC_MemberOverPositionLimit)
	//if(Type==MemberOverPositionLimit_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_FCC_MemberOverPositionLimit;}
	COMPARELANGUAGE(Type, CFieldValueTool_NotMultiple, THOST_FTDC_FCC_NotMultiple)
	//if(Type==NotMultiple_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_FCC_NotMultiple;}
	COMPARELANGUAGE(Type, CFieldValueTool_Violation, THOST_FTDC_FCC_Violation)
	//if(Type==Violation_Cn)						{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_FCC_Violation;}
	COMPARELANGUAGE(Type, CFieldValueTool_Other, THOST_FTDC_FCC_Other)
	//if(Type==Other_Cn)							{ if(language) *language=LANGUAGE_CHINESE;return THOST_FTDC_FCC_Other;}

	//if(Type==NotForceClose_En)					{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_FCC_NotForceClose;}
	//if(Type==LackDeposit_En)					{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_FCC_LackDeposit;}
	//if(Type==ClientOverPositionLimit_En)		{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_FCC_ClientOverPositionLimit;}
	//if(Type==MemberOverPositionLimit_En)		{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_FCC_MemberOverPositionLimit;}
	//if(Type==NotMultiple_En)					{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_FCC_NotMultiple;}
	//if(Type==Violation_En)						{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_FCC_Violation;}
	//if(Type==Other_En)							{ if(language) *language=LANGUAGE_ENGLISH;return THOST_FTDC_FCC_Other;}
	if(language) *language=0;
	return 0;
}
///报单类型
const wxString CFieldValueTool::OrderType2String(char param,int language)
{

	switch(param) {
	case 0:
	case THOST_FTDC_ORDT_Normal:
		return LOADSTRING(CFieldValueTool_Normal);//language==LANGUAGE_CHINESE ?  Normal_Cn : Normal_En;
		break;
		break;
	case THOST_FTDC_ORDT_DeriveFromQuote:
		return LOADSTRING(CFieldValueTool_DeriveFromQuote);//language==LANGUAGE_CHINESE ?  DeriveFromQuote_Cn : DeriveFromQuote_En;
		break;
	case THOST_FTDC_ORDT_DeriveFromCombination:
		return LOADSTRING(CFieldValueTool_DeriveFromCombination);//language==LANGUAGE_CHINESE ?  DeriveFromCombination_Cn : DeriveFromCombination_En;
		break;
	case THOST_FTDC_ORDT_Combination:
		return LOADSTRING(CFieldValueTool_Combination);//language==LANGUAGE_CHINESE ?  Combination_Cn : Combination_En;
		break;
	case THOST_FTDC_ORDT_ConditionalOrder:
		return LOADSTRING(CFieldValueTool_ConditionalOrder);//language==LANGUAGE_CHINESE ?  ConditionalOrder_Cn : ConditionalOrder_En;
		break;
	case THOST_FTDC_ORDT_Swap:
		return LOADSTRING(CFieldValueTool_Swap);//language==LANGUAGE_CHINESE ?  Swap_Cn : Swap_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//language==LANGUAGE_CHINESE ?  Normal_Cn : Normal_En;//return __UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2OrderType(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_Normal, THOST_FTDC_ORDT_Normal)
	//if(Type==Normal_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_Normal;}
	COMPARELANGUAGE(Type, CFieldValueTool_DeriveFromQuote, THOST_FTDC_ORDT_DeriveFromQuote)
	//if(Type==DeriveFromQuote_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_DeriveFromQuote;}
	COMPARELANGUAGE(Type, CFieldValueTool_DeriveFromCombination, THOST_FTDC_ORDT_DeriveFromCombination)
	//if(Type==DeriveFromCombination_Cn)	{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_DeriveFromCombination;}
	COMPARELANGUAGE(Type, CFieldValueTool_Combination, THOST_FTDC_ORDT_Combination)
	//if(Type==Combination_Cn)			{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_Combination;}
	COMPARELANGUAGE(Type, CFieldValueTool_ConditionalOrder, THOST_FTDC_ORDT_ConditionalOrder)
	//if(Type==ConditionalOrder_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_ConditionalOrder;}
	COMPARELANGUAGE(Type, CFieldValueTool_Swap, THOST_FTDC_ORDT_Swap)
	//if(Type==Swap_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_Swap;}

	//if(Type==Normal_En)					{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_Normal;}
	//if(Type==DeriveFromQuote_En)		{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_DeriveFromQuote;}
	//if(Type==DeriveFromCombination_En)	{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_DeriveFromCombination;}
	//if(Type==Combination_En)			{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_Combination;}
	//if(Type==ConditionalOrder_En)		{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_ConditionalOrder;}
	//if(Type==Swap_En)					{ if(language) *language=LANGUAGE_CHINESE;return   THOST_FTDC_ORDT_Swap;}
	if(language) *language=0;
	return 0;
}
///有效期类型
const wxString CFieldValueTool::TimeCondition2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_TC_IOC:
		return LOADSTRING(CFieldValueTool_IOC);// language==LANGUAGE_CHINESE ?  IOC_Cn : IOC_En;
		break;
	case THOST_FTDC_TC_GFS:
		return LOADSTRING(CFieldValueTool_GFS);// language==LANGUAGE_CHINESE ?  GFS_Cn : GFS_En;
		break;
	case THOST_FTDC_TC_GFD:
		return LOADSTRING(CFieldValueTool_GFD);// language==LANGUAGE_CHINESE ?  GFD_Cn : GFD_En;
		break;
	case THOST_FTDC_TC_GTD:
		return LOADSTRING(CFieldValueTool_GTD);// language==LANGUAGE_CHINESE ?  GTD_Cn : GTD_En;
		break;
	case THOST_FTDC_TC_GTC:
		return LOADSTRING(CFieldValueTool_GTC);// language==LANGUAGE_CHINESE ?  GTC_Cn : GTC_En;
		break;
	case THOST_FTDC_TC_GFA:
		return LOADSTRING(CFieldValueTool_GFA);// language==LANGUAGE_CHINESE ?  GFA_Cn : GFA_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2TimeCondition(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_IOC, THOST_FTDC_TC_IOC)
	//if(Type==IOC_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_IOC;}
	COMPARELANGUAGE(Type, CFieldValueTool_GFS, THOST_FTDC_TC_GFS)
	//if(Type==GFS_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GFS;}
	COMPARELANGUAGE(Type, CFieldValueTool_GFD, THOST_FTDC_TC_GFD)
	//if(Type==GFD_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GFD;}
	COMPARELANGUAGE(Type, CFieldValueTool_GTD, THOST_FTDC_TC_GTD)
	//if(Type==GTD_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GTD;}
	COMPARELANGUAGE(Type, CFieldValueTool_GTC, THOST_FTDC_TC_GTC)
	//if(Type==GTC_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GTC;}
	COMPARELANGUAGE(Type, CFieldValueTool_GFA, THOST_FTDC_TC_GFA)
	//if(Type==GFA_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GFA;}

	//if(Type==IOC_En)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_IOC;}
	//if(Type==GFS_En)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GFS;}
	//if(Type==GFD_En)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GFD;}
	//if(Type==GTD_En)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GTD;}
	//if(Type==GTC_En)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GTC;}
	//if(Type==GFA_En)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_TC_GFA;}
	if(language) *language=0;
	return 0;
}
///成交量类型
const wxString CFieldValueTool::VolumeCondition2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_VC_AV:
		return LOADSTRING(CFieldValueTool_AV);// language==LANGUAGE_CHINESE ?AV_Cn:AV_En;
		break;
	case THOST_FTDC_VC_MV:
		return LOADSTRING(CFieldValueTool_MV);// language==LANGUAGE_CHINESE ?MV_Cn:MV_En;
		break;
	case THOST_FTDC_VC_CV:
		return LOADSTRING(CFieldValueTool_CV);// language==LANGUAGE_CHINESE ?CV_Cn:CV_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2VolumeCondition(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_AV, THOST_FTDC_VC_AV)
	//if(Type==AV_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_VC_AV;}
	COMPARELANGUAGE(Type, CFieldValueTool_MV, THOST_FTDC_VC_MV)
	//if(Type==MV_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_VC_MV;}
	COMPARELANGUAGE(Type, CFieldValueTool_CV, THOST_FTDC_VC_CV)
	//if(Type==CV_Cn)					{ if(language) *language=LANGUAGE_CHINESE;return	THOST_FTDC_VC_CV;}
	//if(Type==AV_En)					{ if(language) *language=LANGUAGE_ENGLISH;return	THOST_FTDC_VC_AV;}
	//if(Type==MV_En)					{ if(language) *language=LANGUAGE_ENGLISH;return	THOST_FTDC_VC_MV;}
	//if(Type==CV_En)					{ if(language) *language=LANGUAGE_ENGLISH;return	THOST_FTDC_VC_CV;}
	if(language) *language=0;
	return 0;
}
///业务请求函数返回结果
const wxString CFieldValueTool::ReturnCode2String(int param,int language)
{
	switch(param)
	{
	case 0:
		return LOADSTRING(CFieldValueTool_code0);//language==LANGUAGE_CHINESE ?  code0_Cn : code0_En;
		break;
	case -1:
		return LOADSTRING(CFieldValueTool_code1);//language==LANGUAGE_CHINESE ?  code1_Cn : code1_En;
		break;
	case -2:
		return LOADSTRING(CFieldValueTool_code2);//language==LANGUAGE_CHINESE ?  code2_Cn : code2_En;
		break;
	case -3:
		return LOADSTRING(CFieldValueTool_code3);//language==LANGUAGE_CHINESE ?  code3_Cn : code3_En;
		break;
	case -98:
		return LOADSTRING(CFieldValueTool_code98);//language==LANGUAGE_CHINESE ?  code98_Cn : code98_En;
		break;
	case -99:
		return LOADSTRING(CFieldValueTool_code99);//language==LANGUAGE_CHINESE ?  code99_Cn : code99_En;
		break;
	case -999:
		return LOADSTRING(CFieldValueTool_code999);//language==LANGUAGE_CHINESE ?  code999_Cn : code999_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const int CFieldValueTool::String2ReturnCode(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_code0, 0)
	//if(Type==code0_Cn){ if(language) *language=LANGUAGE_CHINESE;return      0;}
	COMPARELANGUAGE(Type, CFieldValueTool_code1, 1)
	//if(Type==code1_Cn){ if(language) *language=LANGUAGE_CHINESE;return      -1;}
	COMPARELANGUAGE(Type, CFieldValueTool_code2, 2)
	//if(Type==code2_Cn){ if(language) *language=LANGUAGE_CHINESE;return      -2;}
	COMPARELANGUAGE(Type, CFieldValueTool_code3, 3)
	//if(Type==code3_Cn){ if(language) *language=LANGUAGE_CHINESE;return      -3;}
	COMPARELANGUAGE(Type, CFieldValueTool_code98, 98)
	//if(Type==code98_Cn){ if(language) *language=LANGUAGE_CHINESE;return      -98;}
	COMPARELANGUAGE(Type, CFieldValueTool_code99, 99)
	//if(Type==code99_Cn){ if(language) *language=LANGUAGE_CHINESE;return      -99;}
	COMPARELANGUAGE(Type, CFieldValueTool_code999, 999)
	//if(Type==code999_Cn){ if(language) *language=LANGUAGE_CHINESE;return  -999;}

	//if(Type==code0_En){ if(language) *language=LANGUAGE_CHINESE;return      0;}
	//if(Type==code1_En){ if(language) *language=LANGUAGE_CHINESE;return      -1;}
	//if(Type==code2_En){ if(language) *language=LANGUAGE_CHINESE;return      -2;}
	//if(Type==code3_En){ if(language) *language=LANGUAGE_CHINESE;return      -3;}
	//if(Type==code98_En){ if(language) *language=LANGUAGE_CHINESE;return      -98;}
	//if(Type==code99_En){ if(language) *language=LANGUAGE_CHINESE;return      -99;}
	//if(Type==code999_En){ if(language) *language=LANGUAGE_CHINESE;return -999;}
	if(language) *language=0;
	return 0;
}
///品种类型
const wxString CFieldValueTool::ProductType2String(char param,int language)
{
	switch(param) {
	case THOST_FTDC_PC_Futures:
		return LOADSTRING(CFieldValueTool_Futures);//language==LANGUAGE_CHINESE ?  Futures_Cn : Futures_En;
		break;
	case THOST_FTDC_PC_Options:
		return LOADSTRING(CFieldValueTool_Options);//language==LANGUAGE_CHINESE ?  Options_Cn : Options_En;
		break;
	case THOST_FTDC_PC_Combination:
		return LOADSTRING(CFieldValueTool_Combination);//language==LANGUAGE_CHINESE ?  Combination_Cn : Combination_En;
		break;
	case THOST_FTDC_PC_Spot:
		return LOADSTRING(CFieldValueTool_Spot);//language==LANGUAGE_CHINESE ?  Spot_Cn : Spot_En;
		break;
	case THOST_FTDC_PC_EFP:
		return LOADSTRING(CFieldValueTool_EFP);//language==LANGUAGE_CHINESE ?  EFP_Cn : EFP_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const char CFieldValueTool::String2ProductType(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_Futures, THOST_FTDC_PC_Futures)
	//if(Type==Futures_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return            		THOST_FTDC_PC_Futures;}
	COMPARELANGUAGE(Type, CFieldValueTool_Options, THOST_FTDC_PC_Options)
	//if(Type==Options_Cn)		{ if(language) *language=LANGUAGE_CHINESE;return            		THOST_FTDC_PC_Options;}
	COMPARELANGUAGE(Type, CFieldValueTool_Combination, THOST_FTDC_PC_Combination)
	//if(Type==Combination_Cn)	{ if(language) *language=LANGUAGE_CHINESE;return        		THOST_FTDC_PC_Combination;} 
	COMPARELANGUAGE(Type, CFieldValueTool_Spot, THOST_FTDC_PC_Spot)
	//if(Type==Spot_Cn)			{ if(language) *language=LANGUAGE_CHINESE;return                		THOST_FTDC_PC_Spot;}
	COMPARELANGUAGE(Type, CFieldValueTool_EFP, THOST_FTDC_PC_EFP)
	//if(Type==EFP_Cn)			{ if(language) *language=LANGUAGE_CHINESE;return                		THOST_FTDC_PC_EFP;}

	//if(Type==Futures_En)		{ if(language) *language=LANGUAGE_ENGLISH;return			THOST_FTDC_PC_Futures;}
	//if(Type==Options_En)		{ if(language) *language=LANGUAGE_ENGLISH;return            		THOST_FTDC_PC_Options;}
	//if(Type==Combination_En)	{ if(language) *language=LANGUAGE_ENGLISH;return        		THOST_FTDC_PC_Combination;} 
	//if(Type==Spot_En)			{ if(language) *language=LANGUAGE_ENGLISH;return                		THOST_FTDC_PC_Spot;}
	//if(Type==EFP_En)			{ if(language) *language=LANGUAGE_ENGLISH;return		THOST_FTDC_PC_EFP;}
	if(language) *language=0;
	return 0;
}
const wxString CFieldValueTool::TodayPosition2String(int param,int language)
{
	switch(param) {
	case 1:
		return LOADSTRING(CFieldValueTool_PrePositon);//language==LANGUAGE_CHINESE ?  PrePositon_Cn : PrePositon_En;
		break;
	case 2:
		return LOADSTRING(CFieldValueTool_TodayPositon);//language==LANGUAGE_CHINESE ?  TodayPositon_Cn : TodayPositon_En;
		break;
	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
		break;
	}
}
const int CFieldValueTool::String2TodayPosition(wxString Type,int* language)
{
	COMPARELANGUAGE(Type, CFieldValueTool_PrePositon, 1)
	//if(Type==PrePositon_Cn){ if(language) *language=LANGUAGE_CHINESE;return      1;}
	COMPARELANGUAGE(Type, CFieldValueTool_TodayPositon, 2)
	//if(Type==TodayPositon_Cn){ if(language) *language=LANGUAGE_CHINESE;return    2;}
	//if(Type==PrePositon_En){ if(language) *language=LANGUAGE_ENGLISH;return      1;}
	//if(Type==TodayPositon_En){ if(language) *language=LANGUAGE_ENGLISH;return    2;}
	if(language) *language=0;
	return 0;
}
const wxString CFieldValueTool::OrderStatusMsg2String(int param,int language)
{
	switch(param)
	{
	case 100:
		return LOADSTRING(CFieldValueTool_Queued);//language==LANGUAGE_CHINESE?Queued_Cn:Queued_En;
		break;
	case 101:
		return LOADSTRING(CFieldValueTool_Sent);//language==LANGUAGE_CHINESE?Sent_Cn:Sent_En;
		break;
	case 102:
		return LOADSTRING(CFieldValueTool_Working);//language==LANGUAGE_CHINESE?Working_Cn:Working_En;
		break;
	case 103:
		return LOADSTRING(CFieldValueTool_Rejected);//language==LANGUAGE_CHINESE?Rejected_Cn:Rejected_En;
		break;
	case 104:
		return LOADSTRING(CFieldValueTool_Cancelled);//language==LANGUAGE_CHINESE?Cancelled_Cn:Cancelled_En;
		break;
	case 105:
		return LOADSTRING(CFieldValueTool_BalCancelled);//language==LANGUAGE_CHINESE?BalCancelled_Cn:BalCancelled_En;
		break;
	case 106:
		return LOADSTRING(CFieldValueTool_PartFilled);//language==LANGUAGE_CHINESE?PartFilled_Cn:PartFilled_En;
		break;
	case 107:
		return LOADSTRING(CFieldValueTool_Filled);//language==LANGUAGE_CHINESE?Filled_Cn:Filled_En;
		break;
	case 108:
		return LOADSTRING(CFieldValueTool_CancelPending);//language==LANGUAGE_CHINESE?CancelPending_Cn:CancelPending_En;
		break;
	case 109:
		return LOADSTRING(CFieldValueTool_AmendPending);//language==LANGUAGE_CHINESE?AmendPending_Cn:AmendPending_En;
		break;
	case 110:
		return LOADSTRING(CFieldValueTool_UnconfirmedFilled);//language==LANGUAGE_CHINESE?UnconfirmedFilled_Cn:UnconfirmedFilled_En;
		break;
	case 111:
		return LOADSTRING(CFieldValueTool_UnconfirmedPartFilled);//language==LANGUAGE_CHINESE?UnconfirmedPartFilled_Cn:UnconfirmedPartFilled_En;
		break;
	case 112:
		return LOADSTRING(CFieldValueTool_HeldOrder);//language==LANGUAGE_CHINESE?HeldOrder_Cn:HeldOrder_En;
		break;
	case 113:
		return LOADSTRING(CFieldValueTool_CancelHeldOrder);//language==LANGUAGE_CHINESE?CancelHeldOrder_Cn:CancelHeldOrder_En;
		break;
	case 114:
		return LOADSTRING(CFieldValueTool_Transferred);//language==LANGUAGE_CHINESE?Transferred_Cn:Transferred_En;
		break;
	case 115:
		return LOADSTRING(CFieldValueTool_ExternalCancelled);//language==LANGUAGE_CHINESE?ExternalCancelled_Cn:ExternalCancelled_En;
		break;



	default:
		return LOADSTRING(CFieldValueTool_UNKOWN);//__UNKOWN__;
	}
}
const int CFieldValueTool::String2OrderStatusMsg(wxString Type,int* language)
{
	
	COMPARELANGUAGE(Type, CFieldValueTool_Queued, 100)
	//if(Type==Queued_Cn){if(language) *language=LANGUAGE_CHINESE;return 100;}
	COMPARELANGUAGE(Type, CFieldValueTool_Sent, 101)
	//if(Type==Sent_Cn){if(language) *language=LANGUAGE_CHINESE;return 101;}
	COMPARELANGUAGE(Type, CFieldValueTool_Working, 102)
	//if(Type==Working_Cn){if(language) *language=LANGUAGE_CHINESE;return 102;}
	COMPARELANGUAGE(Type, CFieldValueTool_Rejected, 103)
	//if(Type==Rejected_Cn){if(language) *language=LANGUAGE_CHINESE;return 103;}
	COMPARELANGUAGE(Type, CFieldValueTool_Cancelled, 104)
	//if(Type==Cancelled_Cn){if(language) *language=LANGUAGE_CHINESE;return 104;}
	COMPARELANGUAGE(Type, CFieldValueTool_BalCancelled, 105)
	//if(Type==BalCancelled_Cn){if(language) *language=LANGUAGE_CHINESE;return 105;}
	COMPARELANGUAGE(Type, CFieldValueTool_PartFilled, 106)
	//if(Type==PartFilled_Cn){if(language) *language=LANGUAGE_CHINESE;return 106;}
	COMPARELANGUAGE(Type, CFieldValueTool_Filled, 107)
	//if(Type==Filled_Cn){if(language) *language=LANGUAGE_CHINESE;return 107;}
	COMPARELANGUAGE(Type, CFieldValueTool_CancelPending, 108)
	//if(Type==CancelPending_Cn){if(language) *language=LANGUAGE_CHINESE;return 108;}
	COMPARELANGUAGE(Type, CFieldValueTool_AmendPending, 109)
	//if(Type==AmendPending_Cn){if(language) *language=LANGUAGE_CHINESE;return 109;}
	COMPARELANGUAGE(Type, CFieldValueTool_UnconfirmedFilled, 110)
	//if(Type==UnconfirmedFilled_Cn){if(language) *language=LANGUAGE_CHINESE;return 110;}
	COMPARELANGUAGE(Type, CFieldValueTool_UnconfirmedPartFilled, 111)
	//if(Type==UnconfirmedPartFilled_Cn){if(language) *language=LANGUAGE_CHINESE;return 111;}
	COMPARELANGUAGE(Type, CFieldValueTool_HeldOrder, 112)
	//if(Type==HeldOrder_Cn){if(language) *language=LANGUAGE_CHINESE;return 112;}
	COMPARELANGUAGE(Type, CFieldValueTool_CancelHeldOrder, 113)
	//if(Type==CancelHeldOrder_Cn){if(language) *language=LANGUAGE_CHINESE;return 113;}
	COMPARELANGUAGE(Type, CFieldValueTool_Transferred, 114)
	//if(Type==Transferred_Cn){if(language) *language=LANGUAGE_CHINESE;return 114;}
	COMPARELANGUAGE(Type, CFieldValueTool_ExternalCancelled, 115)
	//if(Type==ExternalCancelled_Cn){if(language) *language=LANGUAGE_CHINESE;return 115;}
	//if(Type==Queued_En){if(language) *language=LANGUAGE_ENGLISH;return 100;}
	//if(Type==Sent_En){if(language) *language=LANGUAGE_ENGLISH;return 101;}
	//if(Type==Working_En){if(language) *language=LANGUAGE_ENGLISH;return 102;}
	//if(Type==Rejected_En){if(language) *language=LANGUAGE_ENGLISH;return 103;}
	//if(Type==Cancelled_En){if(language) *language=LANGUAGE_ENGLISH;return 104;}
	//if(Type==BalCancelled_En){if(language) *language=LANGUAGE_ENGLISH;return 105;}
	//if(Type==PartFilled_En){if(language) *language=LANGUAGE_ENGLISH;return 106;}
	//if(Type==Filled_En){if(language) *language=LANGUAGE_ENGLISH;return 107;}
	//if(Type==CancelPending_En){if(language) *language=LANGUAGE_ENGLISH;return 108;}
	//if(Type==AmendPending_En){if(language) *language=LANGUAGE_ENGLISH;return 109;}
	//if(Type==UnconfirmedFilled_En){if(language) *language=LANGUAGE_ENGLISH;return 110;}
	//if(Type==UnconfirmedPartFilled_En){if(language) *language=LANGUAGE_ENGLISH;return 111;}
	//if(Type==HeldOrder_En){if(language) *language=LANGUAGE_ENGLISH;return 112;}
	//if(Type==CancelHeldOrder_En){if(language) *language=LANGUAGE_ENGLISH;return 113;}
	//if(Type==Transferred_En){if(language) *language=LANGUAGE_ENGLISH;return 114;}
	//if(Type==ExternalCancelled_En){if(language) *language=LANGUAGE_ENGLISH;return 115;}
	if(language) *language=0;
	return 0;
}

// 功能：对一个状态信息字符串进行模糊转换为当前语言
// 返回值：新的语言值
// 使用须知
// 正常情况下用于处理Pats的StatusMsg，Pats的StatusMsg底层做个转译的，可以处理
// 内盘的StatusMsg是有交易所返回的，应该不进行语言转换
const wxString CFieldValueTool::ConvertStatusMsg(wxString strStatusMsg)
{
	int valStatusMsg=CFieldValueTool::String2OrderStatusMsg(strStatusMsg);
	if(valStatusMsg>0) {
		strStatusMsg=CFieldValueTool::OrderStatusMsg2String(valStatusMsg,SVR_LANGUAGE);
		return strStatusMsg;
	}
	valStatusMsg=CFieldValueTool::String2ReturnCode(strStatusMsg);
	if(valStatusMsg>0) {
		strStatusMsg=CFieldValueTool::ReturnCode2String(valStatusMsg,SVR_LANGUAGE);
		return strStatusMsg;
	}
	valStatusMsg=CFieldValueTool::String2OrderSubmitStatus(strStatusMsg);
	if(valStatusMsg>0) {
		strStatusMsg=CFieldValueTool::OrderSubmitStatus2String(valStatusMsg,SVR_LANGUAGE);
		return strStatusMsg;
	}
	valStatusMsg=CFieldValueTool::String2OrderStatus(strStatusMsg);
	if(valStatusMsg>0) {
		strStatusMsg=CFieldValueTool::OrderStatus2String(valStatusMsg,SVR_LANGUAGE);
		return strStatusMsg;
	}
	return strStatusMsg;
}
