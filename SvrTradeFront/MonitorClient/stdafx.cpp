// stdafx.cpp : source file that includes just the standard includes
// MonitorClient.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



CString Double2String(double dbValue, int nNumDigits, bool bUseMax /*= false*/)
{
	CString str;

	CString strFormat;
	if(bUseMax )
	{
		if(dbValue>1000)
			str.Format(_T("%.0f"), dbValue);
		else 
		{
			int n = 2;
			if(nNumDigits>2)
				n = nNumDigits;
			strFormat.Format(_T("%%.%df"), n);
			str.Format(strFormat, dbValue);	
		}
	}
	else
	{
		strFormat.Format(_T("%%.%df"), nNumDigits);
		str.Format(strFormat, dbValue);	
	}

	return str;
}
bool IsDoubleSame(double db1, double db2)
{
	if(db1 - db2 > -0.00001 && db1 - db2 < 0.00001)
		return true;
	return false;
}
CString Double2String_Tip(double dbValue)
{
	CString str;
	if(dbValue>=10000)
		str.Format(_T("%.0f"), dbValue);
	else if(dbValue>=1000)
		str.Format(_T("%.2f"), dbValue);
	else
	{
		str.Format(_T("%.3f"), dbValue);
	}
	return str;
}
bool CmdID2String(int nCmdID, std::string& str)
{
	bool bFind = true;
	switch (nCmdID)
	{
	case FFT_API_START:
		str = "";
		break;
		///用户登录请求
	case FTTCMD_ReqUserLogin:
		str = "用户登录请求";
		break;
	case FTTCMD_ReqUserLogin_Rsp:
		str = "用户登录请求";
		break;		//响应命令字

		///登出请求
	case FTTCMD_ReqUserLogout:
		str = "登出请求";
		break; 
	case FTTCMD_ReqUserLogout_Rsp: 
		str = "登出请求";
		break; 

		///用户口令更新请求
	case FTTCMD_ReqUserPasswordUpdate: 
		str = "用户口令更新请求";
		break; 
	case FTTCMD_ReqUserPasswordUpdate_Rsp: 
		str = "用户口令更新请求";
		break;

		///资金账户口令更新请求
	case FTTCMD_ReqTradingAccountPasswordUpdate: 
		str = "资金账户口令更新请求";
		break;
	case FTTCMD_ReqTradingAccountPasswordUpdate_Rsp:
		str = "资金账户口令更新请求";
		break;

		///报单录入请求
	case FTTCMD_ReqOrderInsert: 
		str = "报单录入请求";
		break; 
	case FTTCMD_ReqOrderInsert_Rsp: 
		str = "报单录入请求";
		break; 

		///预埋单录入请求
	case FTTCMD_ReqParkedOrderInsert: 
		str = "预埋单录入请求";
		break; 
	case FTTCMD_ReqParkedOrderInsert_Rsp: 
		str = "预埋单录入请求";
		break; 

		///预埋撤单录入请求
	case FTTCMD_ReqParkedOrderAction: 
		str = "预埋撤单录入请求";
		break; 
	case FTTCMD_ReqParkedOrderAction_Rsp: 
		str = "预埋撤单录入请求";
		break; 

		///报单操作请求
	case FTTCMD_ReqOrderAction: 
		str = "报单操作请求";
		break;
	case FTTCMD_ReqOrderAction_Rsp:
		str = "报单操作请求";
		break;

		///查询最大报单数量请求
	case FTTCMD_ReqQueryMaxOrderVolume: 
		str = "查询最大报单数量请求";
		break;
	case FTTCMD_ReqQueryMaxOrderVolume_Rsp: 
		str = "查询最大报单数量请求";
		break;

		///投资者结算结果确认
	case FTTCMD_ReqSettlementInfoConfirm: 
		str = "投资者结算结果确认";
		break;
	case FTTCMD_ReqSettlementInfoConfirm_Rsp: 
		str = "投资者结算结果确认";
		break;

		///请求删除预埋单
	case FTTCMD_ReqRemoveParkedOrder: 
		str = "请求删除预埋单";
		break;
	case FTTCMD_ReqRemoveParkedOrder_Rsp: 
		str = "请求删除预埋单";
		break;

		///请求删除预埋撤单
	case FTTCMD_ReqRemoveParkedOrderAction: 
		str = "请求删除预埋撤单";
		break;
	case FTTCMD_ReqRemoveParkedOrderAction_Rsp: 
		str = "请求删除预埋撤单";
		break;

		///询价录入请求
	case FTTCMD_ReqForQuoteInsert: 
		str = "询价录入请求";
		break;
	case FTTCMD_ReqForQuoteInsert_Rsp: 
		str = "询价录入请求";
		break;

		///执行宣告录入请求
	case FTTCMD_ReqExecOrderInsert: 
		str = "执行宣告录入请求";
		break;
	case FTTCMD_ReqExecOrderInsert_Rsp: 
		str = "执行宣告录入请求";
		break;

		///执行宣告操作请求
	case FTTCMD_ReqExecOrderAction: 
		str = "执行宣告操作请求";
		break;
	case FTTCMD_ReqExecOrderAction_Rsp: 
		str = "执行宣告操作请求";
		break;

		///请求查询报单
	case FTTCMD_ReqQryOrder: 
		str = "请求查询报单";
		break;
	case FTTCMD_ReqQryOrder_Rsp: 
		str = "请求查询报单";
		break;

		///请求查询成交
	case FTTCMD_ReqQryTrade: 
		str = "请求查询成交";
		break;
	case FTTCMD_ReqQryTrade_Rsp: 
		str = "请求查询成交";
		break;

		///请求查询投资者持仓
	case FTTCMD_ReqQryInvestorPosition: 
		str = "请求查询投资者持仓";
		break;
	case FTTCMD_ReqQryInvestorPosition_Rsp: 
		str = "请求查询投资者持仓";
		break;

		///请求查询资金账户
	case FTTCMD_ReqQryTradingAccount:
		str = "请求查询资金账户";
		break;
	case FTTCMD_ReqQryTradingAccount_Rsp: 
		str = "请求查询资金账户";
		break;

		///请求查询投资者
	case FTTCMD_ReqQryInvestor:
		str = "请求查询投资者";
		break;
	case FTTCMD_ReqQryInvestor_Rsp: 
		str = "请求查询投资者";
		break;

		///请求查询合约保证金率
	case FTTCMD_ReqQryInstrumentMarginRate: 
		str = "请求查询合约保证金率";
		break;
	case FTTCMD_ReqQryInstrumentMarginRate_Rsp: 
		str = "请求查询合约保证金率";
		break;

		///请求查询合约手续费率
	case FTTCMD_ReqQryInstrumentCommissionRate: 
		str = "请求查询合约手续费率";
		break;
	case FTTCMD_ReqQryInstrumentCommissionRate_Rsp: 
		str = "请求查询合约手续费率";
		break;

		///请求查询交易所
	case FTTCMD_ReqQryExchange: 
		str = "请求查询交易所";
		break;
	case FTTCMD_ReqQryExchange_Rsp: 
		str = "请求查询交易所";
		break;

		///请求查询产品
	case FTTCMD_ReqQryProduct: 
		str = "请求查询产品";
		break;
	case FTTCMD_ReqQryProduct_Rsp: 
		str = "请求查询产品";
		break;

		///请求查询合约
	case FTTCMD_ReqQryInstrument: 
		str = "请求查询合约";
		break;
	case FTTCMD_ReqQryInstrument_Rsp: 
		str = "请求查询合约";
		break;

		///请求查询投资者结算结果
	case FTTCMD_ReqQrySettlementInfo: 
		str = "请求查询投资者结算结果";
		break;
	case FTTCMD_ReqQrySettlementInfo_Rsp: 
		str = "请求查询投资者结算结果";
		break;

		///请求查询投资者持仓明细
	case FTTCMD_ReqQryInvestorPositionDetail: 
		str = "请求查询投资者持仓明细";
		break;
	case FTTCMD_ReqQryInvestorPositionDetail_Rsp: 
		str = "请求查询投资者持仓明细";
		break;

		///请求查询客户通知
	case FTTCMD_ReqQryNotice: 
		str = "请求查询客户通知";
		break;
	case FTTCMD_ReqQryNotice_Rsp: 
		str = "请求查询客户通知";
		break;

		///请求查询结算信息确认
	case FTTCMD_ReqQrySettlementInfoConfirm: 
		str = "请求查询结算信息确认";
		break;
	case FTTCMD_ReqQrySettlementInfoConfirm_Rsp: 
		str = "请求查询结算信息确认";
		break;

		///请求查询投资者持仓明细
	case FTTCMD_ReqQryInvestorPositionCombineDetail: 
		str = "请求查询投资者持仓明细";
		break;
	case FTTCMD_ReqQryInvestorPositionCombineDetail_Rsp: 
		str = "请求查询投资者持仓明细";
		break;

		///请求查询保证金监管系统经纪公司资金账户密钥
	case FTTCMD_ReqQryCFMMCTradingAccountKey: 
		str = "请求查询保证金监管系统经纪公司资金账户密钥";
		break;
	case FTTCMD_ReqQryCFMMCTradingAccountKey_Rsp:
		str = "请求查询保证金监管系统经纪公司资金账户密钥";
		break;

		///请求查询期权交易成本
	case FTTCMD_ReqQryOptionInstrTradeCost:
		str = "请求查询期权交易成本";
		break;
	case FTTCMD_ReqQryOptionInstrTradeCost_Rsp:
		str = "请求查询期权交易成本";
		break;

		///请求查询期权合约手续费
	case FTTCMD_ReqQryOptionInstrCommRate:
		str = "请求查询期权合约手续费";
		break;
	case FTTCMD_ReqQryOptionInstrCommRate_Rsp:
		str = "请求查询期权合约手续费";
		break;

		///请求查询执行宣告
	case FTTCMD_ReqQryExecOrder:
		str = "请求查询执行宣告";
		break;
	case FTTCMD_ReqQryExecOrder_Rsp:
		str = "请求查询执行宣告";
		break;

		///请求查询转帐流水
	case FTTCMD_ReqQryTransferSerial:
		str = "请求查询转帐流水";
		break;
	case FTTCMD_ReqQryTransferSerial_Rsp:
		str = "请求查询转帐流水";
		break;

		///请求查询银期签约关系
	case FTTCMD_ReqQryAccountregister:
		str = "请求查询银期签约关系";
		break;
	case FTTCMD_ReqQryAccountregister_Rsp:
		str = "请求查询银期签约关系";
		break;

		///请求查询签约银行
	case FTTCMD_ReqQryContractBank:
		str = "请求查询签约银行";
		break;
	case FTTCMD_ReqQryContractBank_Rsp:
		str = "请求查询签约银行";
		break;

		///请求查询预埋单
	case FTTCMD_ReqQryParkedOrder:
		str = "请求查询预埋单";
		break;
	case FTTCMD_ReqQryParkedOrder_Rsp:
		str = "请求查询预埋单";
		break;

		///请求查询预埋撤单
	case FTTCMD_ReqQryParkedOrderAction:
		str = "请求查询预埋撤单";
		break;
	case FTTCMD_ReqQryParkedOrderAction_Rsp:
		str = "请求查询预埋撤单";
		break;

		///请求查询交易通知
	case FTTCMD_ReqQryTradingNotice:
		str = "请求查询交易通知";
		break;
	case FTTCMD_ReqQryTradingNotice_Rsp:
		str = "请求查询交易通知";
		break;

		///请求查询经纪公司交易参数
	case FTTCMD_ReqQryBrokerTradingParams:
		str = "请求查询经纪公司交易参数";
		break;
	case FTTCMD_ReqQryBrokerTradingParams_Rsp:
		str = "请求查询经纪公司交易参数";
		break;

		///期货发起银行资金转期货请求
	case FTTCMD_ReqFromBankToFutureByFuture:
		str = "期货发起银行资金转期货请求";
		break;
	case FTTCMD_ReqFromBankToFutureByFuture_Rsp:
		str = "期货发起银行资金转期货请求";
		break;

		///期货发起期货资金转银行请求
	case FTTCMD_ReqFromFutureToBankByFuture:
		str = "期货发起期货资金转银行请求";
		break;
	case FTTCMD_ReqFromFutureToBankByFuture_Rsp:
		str = "期货发起期货资金转银行请求";
		break;

		///期货发起查询银行余额请求
	case FTTCMD_ReqQueryBankAccountMoneyByFuture:
		str = "期货发起查询银行余额请求";
		break;
	case FTTCMD_ReqQueryBankAccountMoneyByFuture_Rsp:
		str = "期货发起查询银行余额请求";
		break;


		//仿CTP的Spi函数命令字

	case FFT_SPI_START:
		str = "";break; 

		// 交易服务器连接成功响应
	case FTTCMD_OnFrontConnected_RON:
		str = "交易服务器连接成功响应";
		break;

			// 交易服务器连接断开响应
	case FTTCMD_OnFrontDisconnected_RON:
		str = "交易服务器连接断开响应";
		break;

		///登录请求响应
	case FTTCMD_OnRspUserLogin_RON:
		str = "登录请求响应";
		break;

		///登出请求响应
	case FTTCMD_OnRspUserLogout_RON:
		str = "登出请求响应";
		break;

		///用户口令更新请求响应
	case FTTCMD_OnRspUserPasswordUpdate_RON:		
		str = "用户口令更新请求响应";
		break;

		///资金账户口令更新请求响应
	case FTTCMD_OnRspTradingAccountPasswordUpdate_RON:
		str = "资金账户口令更新请求响应";
		break;

		///报单录入请求响应
	case FTTCMD_OnRspOrderInsert_RON: 
		str = "报单录入请求响应";
		break;

		///预埋单录入请求响应
	case FTTCMD_OnRspParkedOrderInsert_RON: 
		str = "预埋单录入请求响应";break;

		///预埋撤单录入请求响应
	case FTTCMD_OnRspParkedOrderAction_RON: 
		str = "预埋撤单录入请求响应";break;

		///报单操作请求响应
	case FTTCMD_OnRspOrderAction_RON: 
		str = "报单操作请求响应";break;

		///
	case FTTCMD_OnRspQueryMaxOrderVolume_RON: 
		str = "查询最大报单数量响应";break;

		///投资者结算结果确认响应
	case FTTCMD_OnRspSettlementInfoConfirm_RON: 
		str = "投资者结算结果确认响应";break;

		///删除预埋单响应
	case FTTCMD_OnRspRemoveParkedOrder_RON: 
		str = "删除预埋单响应";break;

		///删除预埋撤单响应
	case FTTCMD_OnRspRemoveParkedOrderAction_RON: 
		str = "删除预埋撤单响应";break;

		///执行宣告录入请求响应
	case FTTCMD_OnRspExecOrderInsert_RON: 
		str = "执行宣告录入请求响应";break;

		///执行宣告操作请求响应
	case FTTCMD_OnRspExecOrderAction_RON: 
		str = "执行宣告操作请求响应";break;

		///请求查询报单响应
	case FTTCMD_OnRspQryOrder_RON: 
		str = "请求查询报单响应";break;

		///请求查询成交响应
	case FTTCMD_OnRspQryTrade_RON: 
		str = "请求查询成交响应";break;

		///请求查询投资者持仓响应
	case FTTCMD_OnRspQryInvestorPosition_RON: 
		str = "请求查询投资者持仓响应";break;

		///请求查询资金账户响应
	case FTTCMD_OnRspQryTradingAccount_RON: 
		str = "请求查询资金账户响应";break;

		///请求查询投资者响应
	case FTTCMD_OnRspQryInvestor_RON: 
		str = "请求查询投资者响应";break;

		///请求查询合约保证金率响应
	case FTTCMD_OnRspQryInstrumentMarginRate_RON: 
		str = "请求查询合约保证金率响应";break;

		///请求查询合约手续费率响应
	case FTTCMD_OnRspQryInstrumentCommissionRate_RON: 
		str = "请求查询合约手续费率响应";break;

		///请求查询交易所响应
	case FTTCMD_OnRspQryExchange_RON: 
		str = "请求查询交易所响应";break;

		///请求查询产品响应
	case FTTCMD_OnRspQryProduct_RON:
		str = "请求查询产品响应";break;

		///请求查询合约响应
	case FTTCMD_OnRspQryInstrument_RON: 
		str = "请求查询合约响应";break;

		///请求查询投资者结算结果响应
	case FTTCMD_OnRspQrySettlementInfo_RON:
		str = "请求查询投资者结算结果响应";break;

		///请求查询投资者持仓明细响应
	case FTTCMD_OnRspQryInvestorPositionDetail_RON:
		str = "请求查询投资者持仓明细响应";break;

		///请求查询客户通知响应
	case FTTCMD_OnRspQryNotice_RON: 
		str = "请求查询客户通知响应";break;

		///请求查询结算信息确认响应
	case FTTCMD_OnRspQrySettlementInfoConfirm_RON:
		str = "请求查询结算信息确认响应";break;

		///请求查询投资者持仓明细响应
	case FTTCMD_OnRspQryInvestorPositionCombineDetail_RON:
		str = "请求查询投资者持仓明细响应";break;

		///查询保证金监管系统经纪公司资金账户密钥响应
	case FTTCMD_OnRspQryCFMMCTradingAccountKey_RON:
		str = "查询保证金监管系统经纪公司资金账户密钥响应";break;

		///请求查询期权交易成本响应
	case FTTCMD_OnRspQryOptionInstrTradeCost_RON:
		str = "请求查询期权交易成本响应";break;

		///请求查询期权合约手续费响应
	case FTTCMD_OnRspQryOptionInstrCommRate_RON: 
		str = "请求查询期权合约手续费响应";break;

		///请求查询执行宣告响应
	case FTTCMD_OnRspQryExecOrder_RON: 
		str = "请求查询执行宣告响应";break;

		///请求查询转帐流水响应
	case FTTCMD_OnRspQryTransferSerial_RON: 
		str = "请求查询转帐流水响应";break;

		///请求查询银期签约关系响应
	case FTTCMD_OnRspQryAccountregister_RON: 
		str = "请求查询银期签约关系响应";break;

		///错误应答
	case FTTCMD_OnRspError_RON: 
		str = "错误应答";break;

		///报单通知
	case FTTCMD_OnRtnOrder_RON: 
		str = "报单通知";break;

		///成交通知
	case FTTCMD_OnRtnTrade_RON:
		str = "成交通知";break;

		///报单录入错误回报
	case FTTCMD_OnErrRtnOrderInsert_RON:
		str = "报单录入错误回报";break;

		///报单操作错误回报
	case FTTCMD_OnErrRtnOrderAction_RON: 
		str = "报单操作错误回报";break;

		///合约交易状态通知
	case FTTCMD_OnRtnInstrumentStatus_RON:
		str = "合约交易状态通知";break;

		///交易通知
	case FTTCMD_OnRtnTradingNotice_RON:
		str = "交易通知";break;

		///执行宣告通知
	case FTTCMD_OnRtnExecOrder_RON:
		str = "执行宣告通知";break;

		///执行宣告录入错误回报
	case FTTCMD_OnErrRtnExecOrderInsert_RON:
		str = "执行宣告录入错误回报";break;

		///执行宣告操作错误回报
	case FTTCMD_OnErrRtnExecOrderAction_RON: 
		str = "执行宣告操作错误回报";break;

		///询价录入请求响应
	case FTTCMD_OnRspForQuoteInsert_RON: 
		str = "询价录入请求响应";break;

		///询价录入错误回报
	case FTTCMD_OnErrRtnForQuoteInsert_RON:
		str = "询价录入错误回报";break;

		///请求查询签约银行响应
	case FTTCMD_OnRspQryContractBank_RON:
		str = "请求查询签约银行响应";break;

		///请求查询预埋单响应
	case FTTCMD_OnRspQryParkedOrder_RON: 
		str = "请求查询预埋单响应";break;

		///请求查询预埋撤单响应
	case FTTCMD_OnRspQryParkedOrderAction_RON: 
		str = "请求查询预埋撤单响应";break;

		///请求查询交易通知响应
	case FTTCMD_OnRspQryTradingNotice_RON: 
		str = "请求查询交易通知响应";break;

		///请求查询经纪公司交易参数响应
	case FTTCMD_OnRspQryBrokerTradingParams_RON: 
		str = "请求查询经纪公司交易参数响应";break;

		///期货发起银行资金转期货通知
	case FTTCMD_OnRtnFromBankToFutureByFuture_RON: 
		str = "期货发起银行资金转期货通知";break;

		///期货发起期货资金转银行通知
	case FTTCMD_OnRtnFromFutureToBankByFuture_RON: 
		str = "期货发起期货资金转银行通知";break;

		///期货发起查询银行余额通知
	case FTTCMD_OnRtnQueryBankBalanceByFuture_RON: 
		str = "期货发起查询银行余额通知";break;

		///期货发起银行资金转期货应答
	case FTTCMD_OnRspFromBankToFutureByFuture_RON:
		str = "期货发起银行资金转期货应答";break;

		///期货发起期货资金转银行应答
	case FTTCMD_OnRspFromFutureToBankByFuture_RON:
		str = "期货发起期货资金转银行应答";break;

		///期货发起查询银行余额应答
	case FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON: 
		str = "期货发起查询银行余额应答";break;

	default:
		bFind = false;
		str = "";break;
	}

	return bFind;
}
void split(const string& s, char c, vector<string>& v) 
{
	string::size_type i = 0;
	string::size_type j = s.find(c);
	if(j == -1 && s.length() != 0)
	{
		v.push_back(s);
		return;
	}
	while (j != string::npos) 
	{
		v.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length( )));
	}

}