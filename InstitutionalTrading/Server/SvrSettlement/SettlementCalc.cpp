
#include "SettlementCalc.h"
const static char* __UNKOWN__="未知";
CSettlementCalc::CSettlementCalc(void)
{
}

CSettlementCalc::~CSettlementCalc(void)
{
}
//CF_ERROR CSettlementCalc::FundCalc(void)
//{
//return CF_ERROR_SUCCESS;
//}
//CF_ERROR CSettlementCalc::SaveData(void)
//{
//return CF_ERROR_SUCCESS;
//}
const string CSettlementCalc::ExchangeID2String(const string& ExchangeID)
{
	const string SHFE="上期所";
	const string CZCE="郑商所";
	const string DCE="大商所";
	const string CFFEX="中金所";

	if(ExchangeID=="SHFE")
		return SHFE;
	else if(ExchangeID=="CZCE")
		return CZCE;
	else if(ExchangeID=="DCE")
		return DCE;
	else if(ExchangeID=="CFFEX")
		return CFFEX;
	else
		return __UNKOWN__;
}
const char* CSettlementCalc::TradeDirection2String(char param)
{

	const static char* Buy="买";
	const static char* Sell="卖";
	switch(param)
	{
	case THOST_FTDC_D_Buy:return Buy;
	case THOST_FTDC_D_Sell:return Sell;
	default: return __UNKOWN__;
	}
}
///投机套保标志
const char* CSettlementCalc::HedgeFlag2String(char param)
{
	const static char* Speculation="投机";
	const static char* Arbitrage="套利";
	const static char* Hedge="套保";
	switch(param)
	{
	case THOST_FTDC_HF_Speculation:return Speculation;
	case THOST_FTDC_HF_Arbitrage:return Arbitrage;
	case THOST_FTDC_HF_Hedge:return Hedge;
	default: return __UNKOWN__;
	}
}
///开平标志
const char* CSettlementCalc::OffsetFlag2String(char param)
{
	const static char* Open="开仓";
	const static char* Close="平仓";
	const static char* ForceClose="强平";
	const static char* CloseToday="平今";
	const static char* CloseYesterday="平昨";
	switch(param)
	{
	case THOST_FTDC_OF_Open:		return Open;
	case THOST_FTDC_OF_Close:		return Close;
	case THOST_FTDC_OF_ForceClose:	return ForceClose;
	case THOST_FTDC_OF_CloseToday:	return CloseToday;
	case THOST_FTDC_OF_CloseYesterday:return CloseYesterday;
	default: return __UNKOWN__;
	}
}
const char* CSettlementCalc::Direction2String(char param)
{
	const static char* Net="净";
	const static char* Buy="多头";
	const static char* Sell="空头";
	switch(param)
	{
	case THOST_FTDC_PD_Net:return Net;
	case THOST_FTDC_PD_Long:return Buy;
	case THOST_FTDC_PD_Short:return Sell;
	default: return __UNKOWN__;
	}
}