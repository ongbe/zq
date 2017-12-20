// Interface_SvrTcp.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)



#include "Real_SvrSimulateTrade.h"
#include "SvrSimulateTrade.h"
#include "COrderCenter.h"


extern COrderCenter g_orderCenter;


CReal_SvrSimulateTrade::CReal_SvrSimulateTrade()
{
}

CReal_SvrSimulateTrade::~CReal_SvrSimulateTrade()
{
}

// 重置模拟成交功能
bool CReal_SvrSimulateTrade::Reinit(const std::string& strTradingDay)
{
	return g_orderCenter.Reinit(strTradingDay);
}

//重置交易日，并把所有挂单也重置
bool CReal_SvrSimulateTrade::ResertTradingDay(const std::string& strTradingDay)
{
	return g_orderCenter.ResertTradingDay(strTradingDay);
}

//推送报单
bool CReal_SvrSimulateTrade::PushOrder(const CThostFtdcOrderField& order)
{
	return g_orderCenter.PushOrder(order);
}

//帐号登录
//返回值：true，登录成功；false，重复登录。
bool CReal_SvrSimulateTrade::Login(const CThostFtdcRspUserLoginField& rspLogin)
{
	return g_orderCenter.Login(rspLogin);
}

//帐号登出
//返回值：true，登出成功；false，登出失败。
bool CReal_SvrSimulateTrade::Logout(CThostFtdcUserLogoutField& userLogout, int nRequestID)
{
	return g_orderCenter.Logout(userLogout, nRequestID);
}

//下单接口
//返回值：0 下单成功；1 用户未登录；2 重复的报单
int CReal_SvrSimulateTrade::InsertOrder(const CThostFtdcInputOrderField &input, 
										int nRequestID, const std::string& strExchangeID, 
										std::vector<CThostFtdcOrderField>& vecOrder, 
										std::vector<CThostFtdcTradeField>& vecTrade)
{
	return g_orderCenter.InsertOrder(input, nRequestID, strExchangeID, 
								   vecOrder, vecTrade);
}

//撤单接口
//返回值：0 撤单成功；1 撤单失败，无对应的挂单；2 报单状态不适合撤单
int CReal_SvrSimulateTrade::CancelOrder(const CThostFtdcInputOrderActionField& action, int nRequestID, 
										std::vector<CThostFtdcOrderField>& vecOrder)
{
	return g_orderCenter.CancelOrder(action, nRequestID, vecOrder);
}

//行情接口
int CReal_SvrSimulateTrade::Quot(const PlatformStru_DepthMarketData& quot, 
								 std::vector<CThostFtdcOrderField>& vecOrder, 
								 std::vector<CThostFtdcTradeField>& vecTrader)
{
	return g_orderCenter.Quot(quot, vecOrder, vecTrader);
}
