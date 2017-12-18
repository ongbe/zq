#pragma once

#include "ThostFtdcTraderApi.h"
#include <vector>

class CWriteLog;

class CTradeHandler : public CThostFtdcTraderSpi
{
public:
	CTradeHandler(CThostFtdcTraderApi *pUserApi, HANDLE hConnHandle, HANDLE hDoneHandle,
		TThostFtdcUserIDType UserID, TThostFtdcPasswordType Pwd,
		TThostFtdcBrokerIDType BrokerID,
		std::vector<CThostFtdcInputOrderActionField>& vOrderAction,
		std::vector<CThostFtdcInputOrderField>& vOrderField);

	~CTradeHandler(void);

	virtual void OnFrontConnected();

	virtual void OnFrontDisconnected(int nReason);

	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//结算结果确认响应
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, 
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	void InsertOrder();
	void CancelOrder();

private:
	CThostFtdcTraderApi* m_pUserApi;
	HANDLE m_hConnHandle;
	HANDLE m_Donehandle;
	std::vector<CThostFtdcInputOrderField> m_vInputOrder;
	std::vector<CThostFtdcInputOrderActionField> m_vOrderAction;
	TThostFtdcUserIDType m_UserID;
	TThostFtdcPasswordType m_Pwd;
	TThostFtdcBrokerIDType m_BrokerID;
	int m_nRequestID;

	int m_nOrderRef;

	CWriteLog* m_pWriteLog;
};
