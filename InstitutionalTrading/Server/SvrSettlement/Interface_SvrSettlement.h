#pragma once

#include "Interface_SvrCommon.h"
#include "CommonErrorCode.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
//#include "../Module-Misc2/KernelStruct.h"
//#include "../Module-Misc2/BusinessData.hpp"
#include "CommonStruct.h" 
#include <vector>
#include <map>
#include <set>
using namespace std;
#ifndef SVRSETTLEMENT_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRSETTLEMENT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRSETTLEMENT_EXPORTS
#define SVRSETTLEMENT_API __declspec(dllexport)
#else
#define SVRSETTLEMENT_API __declspec(dllimport)
#endif
#endif


class SVRSETTLEMENT_API CInterface_SvrSettlement : public CInterface_SvrCommon
{
public:
    static CInterface_SvrSettlement& getObj(void);
	static void deleteObj(void);


	//---------------------------------------------------------------------------------
	//	数据接口
	//---------------------------------------------------------------------------------
	int getExampleValue(void);

	////---------------------------------------------------------------------------------
	////	开始结算
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         CF_ERROR_NOT_SETTLEMENTTIME 未到结算时间
	////         CF_ERROR_NO_SETTLEMENTPRICE 没有结算价
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR StratSettlement() = 0;

	////---------------------------------------------------------------------------------
	////	完成结算，数据入库
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR FinishSettlement() = 0;

	////---------------------------------------------------------------------------------
	////	查询结算价信息
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         CF_ERROR_NO_SETTLEMENTPRICE 没有结算价   
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR QuerySettlementPrice(std::vector<SettlementPriceField>& outData) = 0;

	////---------------------------------------------------------------------------------
	////	修改结算价信息
	////  返回： CF_ERROR_SUCCESS 代表成功
	////            
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifySettlementPrice(const SettlementPriceField* indata) = 0;

	////---------------------------------------------------------------------------------
	////	查询成交
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         CF_ERROR_NO_TRADEDATA     代表没有成交记录
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR QueryTrade(std::map<TradeKey,PlatformStru_TradeInfo>& outData) = 0;

	////---------------------------------------------------------------------------------
	////	增加一条成交
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         CF_ERROR_REDUPLICATE_TRADE 代表数据库中已有此条成交记录   
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR AddTrade(PlatformStru_TradeInfo* indata) = 0;

	////---------------------------------------------------------------------------------
	////	删除一条成交
	////  返回： CF_ERROR_SUCCESS 代表成功
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR DeleteTrade(PlatformStru_TradeInfo* indata) = 0;

	////---------------------------------------------------------------------------------
	////	修改一条成交
	////  返回： CF_ERROR_SUCCESS 代表成功
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifyTrade(PlatformStru_TradeInfo* indata) = 0;

	////---------------------------------------------------------------------------------
	////	查询持仓明细
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         CF_ERROR_NO_POSITIONDETAIL 代表没有持仓明细
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR QueryPositionDetail(std::map<PositionDetailKey,PlatformStru_PositionDetail>& outData) = 0;

	////---------------------------------------------------------------------------------
	////	增加一条持仓明细
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         CF_ERROR_REDUPLICATE_POSITIONDETAIL 代表数据库中已有此条持仓明细记录   
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR AddPositionDetail(PlatformStru_PositionDetail* indata) = 0;

	////---------------------------------------------------------------------------------
	////	删除一条持仓明细
	////  返回： CF_ERROR_SUCCESS 代表成功
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR DeletePositionDetail(PlatformStru_PositionDetail* indata) = 0;

	////---------------------------------------------------------------------------------
	////	修改一条持仓明细
	////  返回： CF_ERROR_SUCCESS 代表成功
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifyPositionDetail(PlatformStru_PositionDetail* indata) = 0;

	////---------------------------------------------------------------------------------
	////	查询持仓
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         CF_ERROR_NO_POSITION 代表没有持仓
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR QueryPosition(std::map<PositionKey,PlatformStru_Position>& outData) = 0;

	////---------------------------------------------------------------------------------
	////	增加一条持仓
	////  返回： CF_ERROR_SUCCESS 代表成功
	////         CF_ERROR_REDUPLICATE_POSITION 代表数据库中已有此条持仓记录   
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR AddPosition(PlatformStru_Position* indata) = 0;

	////---------------------------------------------------------------------------------
	////	删除一条持仓
	////  返回： CF_ERROR_SUCCESS 代表成功
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR DeletePosition(PlatformStru_Position* indata) = 0;

	////---------------------------------------------------------------------------------
	////	修改一条持仓
	////  返回： CF_ERROR_SUCCESS 代表成功
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifyPosition(PlatformStru_Position* indata) = 0;

	////---------------------------------------------------------------------------------
	////	设置结算时间
	////  返回： CF_ERROR_SUCCESS 代表成功
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR SetSettltmentTime(TThostFtdcTimeType* SettltmentTime) = 0;

	////---------------------------------------------------------------------------------
	////	客户端修改资金信息
	////  返回： CF_ERROR_SUCCESS 代表成功
	////           
	////---------------------------------------------------------------------------------
	//virtual CF_ERROR ModifyFund(PlatformStru_TradingAccountInfo* indata) = 0;

    CInterface_SvrSettlement();
    virtual ~ CInterface_SvrSettlement();
    static CInterface_SvrSettlement* m_pObj;
};







