// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#include "GlobalConfigMgr.h"
#include "Tools_Ansi.h"
#include "MyXML.h"

#ifdef WIN32
#pragma unmanaged
#endif 


bool GlobalConfigManager::m_bLoad=false;
bool GlobalConfigManager::m_bCalculate_OnRtnTrade=true;
bool GlobalConfigManager::m_bReq_OnRtnTrade=true;
int GlobalConfigManager::m_nDelayBefReq_OnRtnTrade=2;
int GlobalConfigManager::m_nMaxRecordReqPositionDetail_OnRtnTrade=500;
int GlobalConfigManager::m_nMaxItemCountForAutoResort_ListCtrl=200;
bool GlobalConfigManager::m_bCifcoServicesHall_ExtraFunction=false;

bool GlobalConfigManager::m_bUseComb=false;
int GlobalConfigManager::m_DiscountCZCE=3;
int GlobalConfigManager::m_DiscountDCE=3;
int GlobalConfigManager::m_DiscountSHFE=4;
int GlobalConfigManager::m_DiscountCFFEX=0;
bool GlobalConfigManager::m_LockPositionDiscountCZCE=true;
int GlobalConfigManager::m_UnwindPrioritiesCZCE=1;
int GlobalConfigManager::m_UnwindPrioritiesDCE=0;
int GlobalConfigManager::m_UnwindPrioritiesSHFE=0;
int GlobalConfigManager::m_UnwindPrioritiesCFFEX=0;
int GlobalConfigManager::m_PriceForMarginOfTodayPositionCZCE=0;
int GlobalConfigManager::m_PriceForMarginOfTodayPositionDCE=0;
int GlobalConfigManager::m_PriceForMarginOfTodayPositionSHFE=0;
int GlobalConfigManager::m_PriceForMarginOfTodayPositionCFFEX=0;

bool GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LastPrice=true;
bool GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_Volume=false;
bool GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_AskBid=false;
bool GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreSettlementPrice=false;
bool GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_PreClosePrice=false;
bool GlobalConfigManager::m_bCalcuCombQuotOnLegQuot_LimitPrice=false;

bool GlobalConfigManager::m_bFreezeOrderUnknownStatus=false;


bool GlobalConfigManager::m_bOrderType_UIOT_PANE_STANDARD = true;
bool GlobalConfigManager::m_bOrderType_UIOT_PANE_BATCH = true;
bool GlobalConfigManager::m_bOrderType_UIOT_PANE_CONDITION = true;
bool GlobalConfigManager::m_bOrderType_UIOT_PANE_FAKFOK = true;
bool GlobalConfigManager::m_bOrderType_UIOT_PANE_PROFIT = false;
bool GlobalConfigManager::m_bOrderType_UIOT_PANE_STOPLIMIT = true;
bool GlobalConfigManager::m_bOrderType_UIOT_CMD_SIMULATEMARKET = false;
bool GlobalConfigManager::m_bOrderType_UIOT_CMD_PARKED = true;
bool GlobalConfigManager::m_bOrderType_UIOT_CMD_REPLACEORDER = false;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_HEDGE = true;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_OPENCLOSE = true;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_ACCOUNT = false;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_CONDITION_LOCAL = true;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_CONDITION_SERVER = true;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_PARKED_LOCAL = true;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_PARKED_SERVER = true;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_PROFIT_LOCAL = true;
bool GlobalConfigManager::m_bOrderType_UIOT_ITEM_PROFIT_SERVER = true;

////是否支持产品类型
bool GlobalConfigManager::m_bSupport_PC_All=false;
bool GlobalConfigManager::m_bSupport_PC_Futures=true;
bool GlobalConfigManager::m_bSupport_PC_Options=false;
bool GlobalConfigManager::m_bSupport_PC_Combination=true;
bool GlobalConfigManager::m_bSupport_PC_Spot=false;
bool GlobalConfigManager::m_bSupport_PC_EFP=false;


bool GlobalConfigManager::m_bHasSaveOrderTime = false;

int GlobalConfigManager::MaxSubscribeCount=20;
int GlobalConfigManager::MaxSubscribeCountInPlugins=10;

//各交易所对平今指令的支持，0:不支持；1:支持
int GlobalConfigManager::CloseTodaySupport_SHFE=1;
int GlobalConfigManager::CloseTodaySupport_CZCE=0;
int GlobalConfigManager::CloseTodaySupport_DCE=0;
int GlobalConfigManager::CloseTodaySupport_CFFEX=0;

//各交易所对平仓指令的支持，0:不支持；1:支持，可以平昨仓和今仓；2:支持，只能平昨仓
int GlobalConfigManager::CloseSupport_SHFE=2;
int GlobalConfigManager::CloseSupport_CZCE=1;
int GlobalConfigManager::CloseSupport_DCE=1;
int GlobalConfigManager::CloseSupport_CFFEX=1;

//是否是多账号网络版本，多账号单机版，多账号是否乱序下单
bool GlobalConfigManager::m_bNetVersion_MultiAccount=false;
bool GlobalConfigManager::m_bLocalVersion_MultiAccount=false;
bool GlobalConfigManager::m_bDisorderWhenInsertOrder_MultiAccount=false;
bool GlobalConfigManager::m_bAutoDeriveOrderByFirstAccount_MultiAccount=false;
int  GlobalConfigManager::m_UsePrice_AutoDeriveOrderByFirstAccount_MultiAccount=0;
bool GlobalConfigManager::m_AutoBreak_AutoDeriveOrderByFirstAccount_MultiAccount=false;

//是否记录业务包到businessdata.rec中，用于测试回放
bool GlobalConfigManager::m_Test_bSaveBusinessData=false;
//是否进行测试回放，回放businessdata.rec中的业务包
bool GlobalConfigManager::m_Test_bPlayBusinessData=false;
//是否记录线程ID和线程名称到threadid.log文件中
bool GlobalConfigManager::m_Test_bLogThreadID=false;

//运行模式：1: 仅期货；2: 期货+期权；3: 期权及标的物期货。默认为1
int GlobalConfigManager::m_RunMode=1;

//期权：是否支持接收询价通知的功能，默认不支持
bool GlobalConfigManager::m_CanReceiveForQuote=false;

//如果有结算价的话，是否使用结算价计算盈亏和保证金
bool GlobalConfigManager::m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible=false;


void GlobalConfigManager::Load()
{
	CMyXML myxml("platform.xml");
	if(myxml.m_bvalid)
    {
        int iVal;

	    if(myxml.GetNodeContentInt(iVal,"platform","OnRtnTrade","IsCalculate"))
            m_bCalculate_OnRtnTrade = iVal==0 ? false : true;

	    if(myxml.GetNodeContentInt(iVal,"platform","OnRtnTrade","IsReq"))
            m_bReq_OnRtnTrade = iVal==0 ? false : true;

	    if(myxml.GetNodeContentInt(iVal,"platform","OnRtnTrade","DelayBefReq"))
            m_nDelayBefReq_OnRtnTrade = iVal;

	    if(myxml.GetNodeContentInt(iVal,"platform","OnRtnTrade","MaxRecordReqPositionDetail"))
            m_nMaxRecordReqPositionDetail_OnRtnTrade = iVal;

	    if(myxml.GetNodeContentInt(iVal,"platform","ListCtrl","MaxItemCountForAutoResort"))
            m_nMaxItemCountForAutoResort_ListCtrl = iVal;

	    if(myxml.GetNodeContentInt(iVal,"platform","ExtraFunction","CifcoServicesHall"))
            m_bCifcoServicesHall_ExtraFunction = iVal==0 ? false : true;

        m_bUseComb=myxml.GetNodeContentbool2(false,"platform","Combination","UseComb");
        m_DiscountCZCE=myxml.GetNodeContentInt2(3,"platform","Combination","DiscountCZCE");
        m_DiscountDCE=myxml.GetNodeContentInt2(3,"platform","Combination","DiscountDCE");
        m_DiscountSHFE=myxml.GetNodeContentInt2(4,"platform","Combination","DiscountSHFE");
        m_DiscountCFFEX=myxml.GetNodeContentInt2(0,"platform","Combination","DiscountCFFEX");
        m_LockPositionDiscountCZCE=myxml.GetNodeContentbool2(false,"platform","Combination","LockPositionDiscountCZCE");
        m_UnwindPrioritiesCZCE=myxml.GetNodeContentInt2(1,"platform","Combination","UnwindPrioritiesCZCE");
        m_UnwindPrioritiesDCE=myxml.GetNodeContentInt2(0,"platform","Combination","UnwindPrioritiesDCE");
        m_UnwindPrioritiesSHFE=myxml.GetNodeContentInt2(0,"platform","Combination","UnwindPrioritiesSHFE");
        m_UnwindPrioritiesCFFEX=myxml.GetNodeContentInt2(0,"platform","Combination","UnwindPrioritiesCFFEX");
        m_PriceForMarginOfTodayPositionCZCE=myxml.GetNodeContentInt2(0,"platform","Combination","PriceForMarginOfTodayPositionCZCE");
        m_PriceForMarginOfTodayPositionDCE=myxml.GetNodeContentInt2(0,"platform","Combination","PriceForMarginOfTodayPositionDCE");
        m_PriceForMarginOfTodayPositionSHFE=myxml.GetNodeContentInt2(0,"platform","Combination","PriceForMarginOfTodayPositionSHFE");
        m_PriceForMarginOfTodayPositionCFFEX=myxml.GetNodeContentInt2(0,"platform","Combination","PriceForMarginOfTodayPositionCFFEX");
        m_bCalcuCombQuotOnLegQuot_LastPrice=myxml.GetNodeContentbool2(false,"platform","Combination","CalcuCombQuotOnLegQuot_LastPrice");
        m_bCalcuCombQuotOnLegQuot_Volume=myxml.GetNodeContentbool2(false,"platform","Combination","CalcuCombQuotOnLegQuot_Volume");
        m_bCalcuCombQuotOnLegQuot_AskBid=myxml.GetNodeContentbool2(false,"platform","Combination","CalcuCombQuotOnLegQuot_AskBid");
        m_bCalcuCombQuotOnLegQuot_PreSettlementPrice=myxml.GetNodeContentbool2(false,"platform","Combination","CalcuCombQuotOnLegQuot_PreSettlementPrice");
        m_bCalcuCombQuotOnLegQuot_PreClosePrice=myxml.GetNodeContentbool2(false,"platform","Combination","CalcuCombQuotOnLegQuot_PreClosePrice");
        m_bCalcuCombQuotOnLegQuot_LimitPrice=myxml.GetNodeContentbool2(false,"platform","Combination","CalcuCombQuotOnLegQuot_LImitPrice");

	    if(m_bUseComb&&myxml.GetNodeContentInt(iVal,"platform","PlatformDataMgr","FreezeOrderUnknownStatus"))
            m_bFreezeOrderUnknownStatus = iVal==0? false : true;

 	    // 读入系统配置中的下单板支持项
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_PANE_STANDARD"))
            m_bOrderType_UIOT_PANE_STANDARD = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_PANE_BATCH"))
            m_bOrderType_UIOT_PANE_BATCH = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_PANE_CONDITION"))
            m_bOrderType_UIOT_PANE_CONDITION = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_PANE_FAKFOK"))
            m_bOrderType_UIOT_PANE_FAKFOK = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_PANE_PROFIT"))
            m_bOrderType_UIOT_PANE_PROFIT = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_PANE_STOPLIMIT"))
            m_bOrderType_UIOT_PANE_STOPLIMIT = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_CMD_REPLACEORDER"))
            m_bOrderType_UIOT_CMD_REPLACEORDER = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_CMD_SIMULATEMARKET"))
            m_bOrderType_UIOT_CMD_SIMULATEMARKET = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_CMD_PARKED"))
            m_bOrderType_UIOT_CMD_PARKED = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_HEDGE"))
            m_bOrderType_UIOT_ITEM_HEDGE = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_OPENCLOSE"))
            m_bOrderType_UIOT_ITEM_OPENCLOSE = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_ACCOUNT"))
            m_bOrderType_UIOT_ITEM_ACCOUNT = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_CONDITION_LOCAL"))
            m_bOrderType_UIOT_ITEM_CONDITION_LOCAL = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_CONDITION_SERVER"))
            m_bOrderType_UIOT_ITEM_CONDITION_SERVER = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_PARKED_LOCAL"))
            m_bOrderType_UIOT_ITEM_PARKED_LOCAL = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_PARKED_SERVER"))
            m_bOrderType_UIOT_ITEM_PARKED_SERVER = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_PROFIT_LOCAL"))
            m_bOrderType_UIOT_ITEM_PROFIT_LOCAL = iVal!=0;
		if(myxml.GetNodeContentInt(iVal,"platform","OrderType","UIOT_ITEM_PROFIT_SERVER"))
            m_bOrderType_UIOT_ITEM_PROFIT_SERVER = iVal!=0;


		if(myxml.GetNodeContentInt(iVal,"platform","test","HasSaveOrderTime"))
            m_bHasSaveOrderTime = iVal!=0;

	    if(myxml.GetNodeContentInt(iVal,"platform","SupportProductClassType","All"))
            m_bSupport_PC_All = iVal==0? false : true;
	    if(myxml.GetNodeContentInt(iVal,"platform","SupportProductClassType","Futures"))
            m_bSupport_PC_Futures = iVal==0? false : true;
	    if(myxml.GetNodeContentInt(iVal,"platform","SupportProductClassType","Options"))
            m_bSupport_PC_Options = iVal==0? false : true;
	    if(myxml.GetNodeContentInt(iVal,"platform","SupportProductClassType","Combination"))
            m_bSupport_PC_Combination = iVal==0? false : true;
	    if(myxml.GetNodeContentInt(iVal,"platform","SupportProductClassType","Spot"))
            m_bSupport_PC_Spot = iVal==0? false : true;
	    if(myxml.GetNodeContentInt(iVal,"platform","SupportProductClassType","EFP"))
            m_bSupport_PC_EFP = iVal==0? false : true;

	    MaxSubscribeCount=myxml.GetNodeContentInt2(20,"platform","SubscribeQuot","MaxSubscribeCount");
	    MaxSubscribeCountInPlugins=myxml.GetNodeContentInt2(10,"platform","SubscribeQuot","MaxSubscribeCountInPlugins");

        //各交易所对平今指令的支持，0:不支持；1:支持
        CloseTodaySupport_SHFE=myxml.GetNodeContentInt2(1,"platform","TradeParam","CloseTodaySupport_SHFE");
        CloseTodaySupport_CZCE=myxml.GetNodeContentInt2(0,"platform","TradeParam","CloseTodaySupport_CZCE");
        CloseTodaySupport_DCE=myxml.GetNodeContentInt2(0,"platform","TradeParam","CloseTodaySupport_DCE");
        CloseTodaySupport_CFFEX=myxml.GetNodeContentInt2(0,"platform","TradeParam","CloseTodaySupport_CFFEX");

        //各交易所对平仓指令的支持，0:不支持；1:支持，可以平昨仓和今仓；2:支持，只能平昨仓
        CloseSupport_SHFE=myxml.GetNodeContentInt2(2,"platform","TradeParam","CloseSupport_SHFE");
        CloseSupport_CZCE=myxml.GetNodeContentInt2(1,"platform","TradeParam","CloseSupport_CZCE");
        CloseSupport_DCE=myxml.GetNodeContentInt2(1,"platform","TradeParam","CloseSupport_DCE");
        CloseSupport_CFFEX=myxml.GetNodeContentInt2(1,"platform","TradeParam","CloseSupport_CFFEX");

        //是否是多账号网络版本，多账号单机版，多账号是否乱序下单
        m_bNetVersion_MultiAccount=myxml.GetNodeContentbool2(false,"platform","MultiAccount","NetVersion");
        m_bLocalVersion_MultiAccount=myxml.GetNodeContentbool2(false,"platform","MultiAccount","LocalVersion");
        m_bDisorderWhenInsertOrder_MultiAccount=myxml.GetNodeContentbool2(false,"platform","MultiAccount","DisorderWhenInsertOrder");
        m_bAutoDeriveOrderByFirstAccount_MultiAccount=myxml.GetNodeContentbool2(false,"platform","MultiAccount","AutoDeriveOrderByFirstAccount");
        m_UsePrice_AutoDeriveOrderByFirstAccount_MultiAccount=myxml.GetNodeContentInt2(0,"platform","MultiAccount","AutoDeriveOrderByFirstAccount","UsePrice");
        m_AutoBreak_AutoDeriveOrderByFirstAccount_MultiAccount=myxml.GetNodeContentbool2(0,"platform","MultiAccount","AutoDeriveOrderByFirstAccount","AutoBreak");

        m_Test_bSaveBusinessData=myxml.GetNodeContentbool2(false,"platform","Test","SaveBusinessData");
        m_Test_bPlayBusinessData=myxml.GetNodeContentbool2(false,"platform","Test","PlayBusinessData");
        m_Test_bLogThreadID=myxml.GetNodeContentbool2(false,"platform","Test","LogThreadID");

        //运行模式：1: 仅期货；2: 期货+期权；3: 期权及标的物期货。默认为1
        m_RunMode=myxml.GetNodeContentInt2(1,"platform","RunMode");

        //期权：是否支持接收询价通知的功能，默认不支持
        m_CanReceiveForQuote = m_RunMode!=2&&m_RunMode!=3 ? false : myxml.GetNodeContentbool2(false,"platform","Option","ReceiveForQuote");

        //如果有结算价的话，是否使用结算价计算盈亏和保证金
        m_bUseSettlementPriceToCalculateProfitAndMarginAsPossible=myxml.GetNodeContentbool2(false,"platform","PlatformDataMgr","UseSettlementPriceToCalculateProfitAndMarginAsPossible");

		m_bLoad=true;
    }
}

bool GlobalConfigManager::IsCalculate_OnRtnTrade()
{
	return m_bCalculate_OnRtnTrade;
}


//获取成交回报后是否触发的参数
bool GlobalConfigManager::IsReq_OnRtnTrade(void)
{
    return m_bReq_OnRtnTrade;
}

//获取成交回报后触发查询的延迟秒数
int GlobalConfigManager::GetDelayBefReq_OnRtnTrade(void)
{
    return m_nDelayBefReq_OnRtnTrade;
}

//获取成交触发查询持仓明细的最大记录数量阀值，超过这个值就不自动查询持仓明细
int GlobalConfigManager::GetMaxRecordReqPositionDetail_OnRtnTrade(void)
{
    return m_nMaxRecordReqPositionDetail_OnRtnTrade;
}

//列表框自动排序时，最大行数。超过这个行数，停止自动排序，以免影响速度
int GlobalConfigManager::GetMaxItemCountForAutoResort_ListCtrl(void)
{
    return m_nMaxItemCountForAutoResort_ListCtrl;
}

//是否启动CifcoServicesHall功能
bool GlobalConfigManager::IsCifcoServicesHall_ExtraFunction()
{
    return m_bCifcoServicesHall_ExtraFunction;
}

//对状态为未知的报单(如手工审核单)，是否执行资金冻结
bool GlobalConfigManager::IsFreezeOrderUnknownStatus()
{
    return m_bFreezeOrderUnknownStatus;
}

// 标准下单面板
bool GlobalConfigManager::IsOrderType_UIOT_PANE_STANDARD()
{
	return m_bOrderType_UIOT_PANE_STANDARD;
}

// 批量下单面板
bool GlobalConfigManager::IsOrderType_UIOT_PANE_BATCH()
{
    return m_bOrderType_UIOT_PANE_BATCH;
}

// 条件单下单面板
bool GlobalConfigManager::IsOrderType_UIOT_PANE_CONDITION()
{
    return m_bOrderType_UIOT_PANE_CONDITION;
}

// Fak/fok下单面板
bool GlobalConfigManager::IsOrderType_UIOT_PANE_FAKFOK()
{
    return m_bOrderType_UIOT_PANE_FAKFOK;
}

// 盈损单下单面板
bool GlobalConfigManager::IsOrderType_UIOT_PANE_PROFIT()
{
    return m_bOrderType_UIOT_PANE_PROFIT;
}

// 停止单下单面板
bool GlobalConfigManager::IsOrderType_UIOT_PANE_STOPLIMIT()
{
    return m_bOrderType_UIOT_PANE_STOPLIMIT;
}

// 模拟市价功能
bool GlobalConfigManager::IsOrderType_UIOT_CMD_SIMULATEMARKET()
{
    return m_bOrderType_UIOT_CMD_SIMULATEMARKET;
}

// 预埋单按钮
bool GlobalConfigManager::IsOrderType_UIOT_CMD_PARKED()
{
    return m_bOrderType_UIOT_CMD_PARKED;
}

// pats改单菜单项
bool GlobalConfigManager::IsOrderType_UIOT_CMD_REPLACEORDER()
{
    return m_bOrderType_UIOT_CMD_REPLACEORDER;
}

// 下单板保值单项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_HEDGE()
{
    return m_bOrderType_UIOT_ITEM_HEDGE;
}

// 下单板开平项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_OPENCLOSE()
{
    return m_bOrderType_UIOT_ITEM_OPENCLOSE;
}

// 下单板帐号项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_ACCOUNT()
{
    return m_bOrderType_UIOT_ITEM_ACCOUNT;
}

// 本地条件单控制项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_CONDITION_LOCAL()
{
    return m_bOrderType_UIOT_ITEM_CONDITION_LOCAL;
}

// 服务器条件单控制项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_CONDITION_SERVER()
{
    return m_bOrderType_UIOT_ITEM_CONDITION_SERVER;
}

// 本地预埋单控制项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_PARKED_LOCAL()
{
    return m_bOrderType_UIOT_ITEM_PARKED_LOCAL;
}

// 服务器预埋单控制项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_PARKED_SERVER()
{
    return m_bOrderType_UIOT_ITEM_PARKED_SERVER;
}

// 本地预埋单控制项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_PROFIT_LOCAL()
{
	return m_bOrderType_UIOT_ITEM_PROFIT_LOCAL;
}

// 服务器预埋单控制项
bool GlobalConfigManager::IsOrderType_UIOT_ITEM_PROFIT_SERVER()
{
	return m_bOrderType_UIOT_ITEM_PROFIT_SERVER;
}

// 是否保存发送、接收报单及回报的本地时间
bool GlobalConfigManager::HasSaveOrderTime()
{
	return m_bHasSaveOrderTime;
}

//是否支持指定产品类型
bool GlobalConfigManager::IsSupportProductClassType(eProductClassType ProductClassType)
{
    switch(ProductClassType)
    {
        case PC_All: return m_bSupport_PC_All;  break;
        case PC_Futures: return m_bSupport_PC_Futures;  break;
        case PC_Options: return m_bSupport_PC_Options;  break;
        case PC_Combination: return m_bSupport_PC_Combination;  break;
        case PC_Spot: return m_bSupport_PC_Spot;  break;
        case PC_EFP: return m_bSupport_PC_EFP;  break;
        default: return false; break;
    }
}

