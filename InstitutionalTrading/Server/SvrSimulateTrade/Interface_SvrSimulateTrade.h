#pragma once

#include "Interface_SvrCommon.h"
#include <string>
#include <vector>
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"


#ifndef SIMULATETRADE_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRSIMULATETRADE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRSIMULATETRADE_EXPORTS
#define SIMULATETRADE_API __declspec(dllexport)
#else
#define SIMULATETRADE_API __declspec(dllimport)
#endif
#endif


class SIMULATETRADE_API CInterface_SvrSimulateTrade : public CInterface_SvrCommon
{
public:
   static CInterface_SvrSimulateTrade& getObj(void);

public:

	// 重置模拟成交功能
	virtual bool Reinit(const std::string& strTradingDay) = 0;
	
	//重置交易日，并把所有挂单也重置
	virtual bool ResertTradingDay(const std::string& strTradingDay) = 0;

	//推送报单
	virtual bool PushOrder(const CThostFtdcOrderField& order) = 0;

	//帐号登录，用于设置帐号的frontID和sessionID
	//返回值：true，登录成功；false，重复登录。
	virtual bool Login(const CThostFtdcRspUserLoginField& rspLogin) = 0;

	//帐号登出
	//返回值：true，登出成功；false，登出失败。
	virtual bool Logout(CThostFtdcUserLogoutField& userLogout, int nRequestID) = 0;

	//下单接口
	//参数: vecOrder，返回报单
	//单数: vecTrade，返回成交
	//返回值：0 下单成功；1 用户未登录；2 重复的报单
	virtual int InsertOrder(const CThostFtdcInputOrderField &input, 
				int nRequestID, const std::string& strExchangeID, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrade) = 0;
	
	//撤单接口
	//参数: vecOrder，返回报单
	//返回值：0 撤单成功；1 撤单失败，无对应的挂单；2 报单状态不适合撤单
	virtual int CancelOrder(const CThostFtdcInputOrderActionField& action, int nRequestID, 
				std::vector<CThostFtdcOrderField>& vecOrder) = 0;
	
	//行情接口
	//参数: vecOrder，返回报单
	//单数: vecTrade，返回成交
	//返回值：0 行情设置成功；2 无效的最新价
	virtual int Quot(const PlatformStru_DepthMarketData& quot, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrader) = 0;

};
