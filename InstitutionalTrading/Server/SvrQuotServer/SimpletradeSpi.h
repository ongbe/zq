#pragma once
#include <stdio.h>
#include <windows.h>
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcTraderApi.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiDataType.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"



//// 会员代码
//TThostFtdcBrokerIDType g_chBrokerID;
//// 交易用户代码
//TThostFtdcUserIDType g_chUserID;
class SimpletradeSpi : public CThostFtdcTraderSpi
{
public:
	// 构造函数，需要一个有效的指向CThostFtdcMduserApi实例的指针
	SimpletradeSpi(CThostFtdcTraderApi *pUserApi);
	  ~SimpletradeSpi() {}

	  // 当客户端与交易托管系统建立起通信连接，客户端需要进行登录
	  virtual void OnFrontConnected();
	 

	  // 当客户端与交易托管系统通信连接断开时，该方法被调用
	  virtual void OnFrontDisconnected(int nReason);
	  

	  // 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
	  virtual void OnRspUserLogin(CThostFtdcRspUserLoginField	*pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool	bIsLast);
	 

	  ///请求查询合约响应
	  virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	  void GetInstrument(map<std::string,PlatformStru_InstrumentInfo>& outData);


private:	
	bool SaveInstrument2DB(vector<PlatformStru_InstrumentInfo>& nVecInstruemnts);
	bool SaveTradingDay2DB(std::string strTradingday);
	  

private:
	// 指向CThostFtdcMduserApi实例的指针
	CThostFtdcTraderApi *m_pUserApi;
	vector<PlatformStru_InstrumentInfo>   m_vecInstruemnts;
	map<std::string,PlatformStru_InstrumentInfo>   m_mapInstruemnts;
	std::string m_BrokerID;
	std::string m_UserID;
	std::string m_CTPPassword;
	CThostFtdcReqUserLoginField m_reqUserLogin;
};
