#include "StdAfx.h"
#include "TradeHandler.h"
#include "Tools/WriteLog.h"

CTradeHandler::CTradeHandler(CThostFtdcTraderApi *pUserApi, HANDLE hConnHandle, HANDLE hDoneHandle,
							 TThostFtdcUserIDType UserID, TThostFtdcPasswordType Pwd,
							 TThostFtdcBrokerIDType BrokerID,
							 std::vector<CThostFtdcInputOrderActionField>& vOrderAction,
							 std::vector<CThostFtdcInputOrderField>& vOrderField)
: m_pUserApi(pUserApi)
, m_hConnHandle(hConnHandle)
, m_Donehandle(hDoneHandle)
, m_nRequestID(0)
, m_nOrderRef(0)
, m_pWriteLog(NULL)
{
	memset(m_UserID, 0, sizeof(m_UserID));
	memset(m_Pwd, 0, sizeof(m_Pwd));
	memset(m_BrokerID, 0, sizeof(m_BrokerID));

	strcpy(m_UserID, UserID);
	strcpy(m_Pwd, Pwd);
	strcpy(m_BrokerID, BrokerID);
	
	m_vOrderAction = vOrderAction;
	m_vInputOrder = vOrderField;

	char szBuf[100];
	memset(szBuf, 0, sizeof(szBuf));
	sprintf(szBuf, "ForceClose_%d.log", m_UserID);

#ifdef WRITE_LOG
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE, szBuf);
#endif	
}

CTradeHandler::~CTradeHandler(void)
{
#ifdef WRITE_LOG
	if ( m_pWriteLog != NULL )
	{
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}
#endif
}

void CTradeHandler::OnFrontConnected()
{
	SetEvent(m_hConnHandle);
	CThostFtdcReqUserLoginField reqUserLogin;
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	strcpy(reqUserLogin.BrokerID, m_BrokerID);
	strcpy(reqUserLogin.UserID, m_UserID);
	strcpy(reqUserLogin.Password, m_Pwd);

	m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
}

void CTradeHandler::OnFrontDisconnected( int nReason )
{
	printf("OnFrontDisconnected.\n");
	if ( NULL != m_pWriteLog )
	{
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "OnFrontDisconnected.\n");
	}

	SetEvent(m_Donehandle);
}

void CTradeHandler::OnRspUserLogin( CThostFtdcRspUserLoginField *pRspUserLogin, 
								   CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	printf("OnRspUserLogin.\n");
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	if ( NULL != m_pWriteLog )
	{
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "OnRspUserLogin.\n");
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);
	}

	m_nOrderRef = atoi(pRspUserLogin->MaxOrderRef);

	if ( pRspInfo->ErrorID != 0 )
	{
		//登录失败
		printf("Failed to login, errorcode=%d, errormsg=%s, requestid=%d, chain=%d",
			pRspInfo->ErrorID, pRspInfo->ErrorMsg,nRequestID, bIsLast);
		if ( NULL != m_pWriteLog )
		{
			m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "Failed to login, errorcode=%d, errormsg=%s, requestid=%d, chain=%d",
				pRspInfo->ErrorID, pRspInfo->ErrorMsg,nRequestID, bIsLast);
		}

		SetEvent(m_Donehandle);
	}
	else
	{
		//登录成功，请求结算
		CThostFtdcQrySettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_BrokerID);
		strcpy(req.InvestorID, m_UserID);
		m_pUserApi->ReqQrySettlementInfoConfirm(&req, m_nRequestID++);
	}
}

void CTradeHandler::OnRspOrderInsert( CThostFtdcInputOrderField *pInputOrder, 
									 CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	if ( NULL != m_pWriteLog )
	{
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	}
}

void CTradeHandler::OnRtnOrder( CThostFtdcOrderField *pOrder )
{
	printf("OnRtnOrder.\n");
	printf("OrderSysID=[%s].\n", pOrder->OrderSysID);

	if ( NULL != m_pWriteLog )
	{
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "OnRtnOrder.\n");
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "OrderSysID=[%s].\n", pOrder->OrderSysID);
	}
}

void CTradeHandler::OnRspError( CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{

}

void CTradeHandler::OnRspQrySettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, 
												  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	printf("OnRspQrySettlementInfoConfirm.\n");
	if ( NULL != m_pWriteLog )
	{
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "OnRspQrySettlementInfoConfirm.\n");
	}

	if ( pSettlementInfoConfirm != NULL )
	{
		//有信息，需要确认
		CThostFtdcSettlementInfoConfirmField req;
		memset(&req, 0, sizeof(req));
		strcpy(req.BrokerID, m_BrokerID);
		strcpy(req.InvestorID, m_UserID);
		m_pUserApi->ReqSettlementInfoConfirm(&req, m_nRequestID++);
	}
	else
	{
		//无需确认
		CancelOrder();
		InsertOrder();
		SetEvent(m_Donehandle);
	}
}

void CTradeHandler::OnRspSettlementInfoConfirm( CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, 
											   CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast )
{
	printf("OnRspSettlementInfoConfirm.\n");
	if ( NULL != m_pWriteLog )
	{
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, "OnRspSettlementInfoConfirm.\n");
	}

	if(pRspInfo->ErrorID==0)
	{
		CancelOrder();
		InsertOrder();
		SetEvent(m_Donehandle);
	}
}

void CTradeHandler::InsertOrder()
{
	for ( UINT i = 0; i < m_vInputOrder.size(); i++ )
	{
		CThostFtdcInputOrderField order = m_vInputOrder[i];
		sprintf(order.OrderRef, "%12d", ++m_nOrderRef);
		int nRet = m_pUserApi->ReqOrderInsert(&order, 0);
		char szBuf[1024];
		memset(szBuf, 0, sizeof(szBuf));
		sprintf(szBuf, "ReqOrderInsert(报单录入请求):ret=[%d],nRequestID=[%d]\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t InstrumentID=[%s],\t OrderRef=[%s],\t UserID=[%s],\n"
			"\t\t\t OrderPriceType=[%d],\t Direction=[%d],\t CombOffsetFlag=[%s],\t CombHedgeFlag=[%s],\t LimitPrice=[%g],\n"
			"\t\t\t VolumeTotalOriginal=[%d],\t TimeCondition=[%d],\t GTDDate=[%s],\t VolumeCondition=[%d],\t MinVolume=[%d],\n"
			"\t\t\t ContingentCondition=[%d],\t StopPrice=[%g],\t ForceCloseReason=[%d],\t IsAutoSuspend=[%d],\t BusinessUnit=[%s],\n"
			"\t\t\t RequestID=[%d],\t UserForceClose=[%d]",
			nRet, i,
			order.BrokerID, order.InvestorID, order.InstrumentID, order.OrderRef,order.UserID,
			order.OrderPriceType, order.Direction, order.CombOffsetFlag, order.CombHedgeFlag,order.LimitPrice,
			order.VolumeTotalOriginal, order.TimeCondition, order.GTDDate, order.VolumeCondition,order.MinVolume,
			order.ContingentCondition, order.StopPrice, order.ForceCloseReason, order.IsAutoSuspend,order.BusinessUnit,
			order.RequestID, order.UserForceClose);
		printf(szBuf);

		if ( NULL != m_pWriteLog )
		{
			m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, szBuf);
		}
	}

}

void CTradeHandler::CancelOrder()
{
	for ( UINT i = 0; i < m_vOrderAction.size(); i++ )
	{
		CThostFtdcInputOrderActionField orderAction = m_vOrderAction[i];
		int nRet = m_pUserApi->ReqOrderAction(&orderAction, 0);
		char szBuf[1024];
		memset(szBuf, 0, sizeof(szBuf));
		sprintf(szBuf, "ReqOrderAction(报单操作请求):ret=[%d],nRequestID=[%d]\n"
			"\t\t\t BrokerID=[%s],\t InvestorID=[%s],\t OrderActionRef=[%d],\t OrderRef=[%s],\t RequestID=[%d],\n"
			"\t\t\t FrontID=[%d],\t SessionID=[%#x],\t ExchangeID=[%s],\t OrderSysID=[%s],\t ActionFlag=[%d],\n"
			"\t\t\t LimitPrice=[%g],\t VolumeChange=[%d],\t UserID=[%s],\t InstrumentID=[%s]",
			nRet, i,
			orderAction.BrokerID, orderAction.InvestorID, orderAction.OrderActionRef, orderAction.OrderRef,orderAction.RequestID,
			orderAction.FrontID, orderAction.SessionID, orderAction.ExchangeID, orderAction.OrderSysID,orderAction.ActionFlag,
			orderAction.LimitPrice, orderAction.VolumeChange, orderAction.UserID, orderAction.InstrumentID);

		printf(szBuf);
		if ( NULL != m_pWriteLog )
		{
			m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, szBuf);
		}
	}
}
