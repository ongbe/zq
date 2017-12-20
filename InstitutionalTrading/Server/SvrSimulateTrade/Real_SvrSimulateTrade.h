#pragma once

#include "Interface_SvrSimulateTrade.h"
#include <map>
#include <set>

class CReal_SvrSimulateTrade : public CInterface_SvrSimulateTrade
{
	//---------------------------------------------------------------------------------
	//	数据接口
	//---------------------------------------------------------------------------------

	// 重置模拟成交功能
	bool Reinit(const std::string& strTradingDay);
	
	//重置交易日，并把所有挂单也重置
	bool ResertTradingDay(const std::string& strTradingDay);

	//推送报单
	bool PushOrder(const CThostFtdcOrderField& order);

	//帐号登录
	//返回值：true，登录成功；false，重复登录。
	bool Login(const CThostFtdcRspUserLoginField& rspLogin);

	//帐号登出
	//返回值：true，登出成功；false，登出失败。
	bool Logout(CThostFtdcUserLogoutField& userLogout, int nRequestID);

	//下单接口
	//返回值：0 下单成功；1 用户未登录；2 重复的报单
	int InsertOrder(const CThostFtdcInputOrderField &input, 
				int nRequestID, const std::string& strExchangeID, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrade);
	
	//撤单接口
	//返回值：0 撤单成功；1 撤单失败，无对应的挂单；2 报单状态不适合撤单
	int CancelOrder(const CThostFtdcInputOrderActionField& action, int nRequestID, 
				std::vector<CThostFtdcOrderField>& vecOrder);
	
	//行情接口
	int Quot(const PlatformStru_DepthMarketData& quot, 
				std::vector<CThostFtdcOrderField>& vecOrder, 
				std::vector<CThostFtdcTradeField>& vecTrader);

public:
    CReal_SvrSimulateTrade();
    virtual ~ CReal_SvrSimulateTrade();
};







