#include "StdAfx.h"
#include "COrderAtomEntity.h"
#include "../inc/Module-Misc/tools_util.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

//////////////////////////////////////////////////////////////////////////////////////

COrderAtomEntity* COrderAtomEntity::CreateOrderAtomEntity(
			STDORDERREQ& stdReq, int nMode, LPVOID pOrderInfo, 
			BOOL bNoSendVolZoreOrder)
{
	if(nMode<conModeNewOrder || nMode>conModeCancelThenOrder)
		return NULL;
	if((nMode==conModeCancelOnly || nMode==conModeReplace || 
			nMode==conModeCancelThenOrder) && pOrderInfo==NULL)
		return NULL;

	COrderAtomEntity* pEntity = new COrderAtomEntity(stdReq, nMode, pOrderInfo, 
			bNoSendVolZoreOrder);
	
	return pEntity;
}

COrderAtomEntity::COrderAtomEntity(STDORDERREQ& stdReq, int nMode, 
								   LPVOID pOrderInfo, BOOL bNoSendVolZoreOrder)
{
	m_dwStartTime = 0xFFFFFFFF;
	m_nRunStatus = conNothing;
	ZeroMemory(&m_lastRspInfo, sizeof(m_lastRspInfo));

	m_bNeedNewOrderStatus = FALSE;
	m_bNoSendVolZoreOrder = bNoSendVolZoreOrder;
	m_nLastOrderReqID = 0;
	m_nLastCancelReqID = 0;
	m_nArrayIndex = -1;

	m_nMode = nMode;
	m_stdReq = stdReq;
	if(m_nMode==conModeCancelOnly || m_nMode==conModeReplace || 
			m_nMode==conModeCancelThenOrder) {
		if(IsParketOrder(stdReq))
			m_arrayParkedRsp.push_back(*(CThostFtdcParkedOrderField*)pOrderInfo);
		else {
			m_arrayOrderRsp.push_back(*(PlatformStru_OrderInfo*)pOrderInfo);
			m_strRevokeMsg = MakeRevokeDesciption(*(PlatformStru_OrderInfo*)pOrderInfo);
		}
		m_nArrayIndex++;
	}

	
	m_nNextStep = conStepNothing;
	m_nPriceMode = conPriceNone;
	m_fPrice = 0.0;
	m_nPriceTriggerMode = conPriceNone;
	m_fPriceTrigger = 0.0;
	m_nVolumeMode = conVolumeNone;
	m_nVolume = 0;
}

COrderAtomEntity::~COrderAtomEntity()
{
}

BOOL COrderAtomEntity::ConvertCondToOrderType(int nPriceType, 
											  int nCondType, int& nOrderType)
{
	switch(nPriceType) {
	case 1:	// 1 最新
		switch(nCondType) {
		case 1:		// 1 小于等于指定价格
			nOrderType = OrderType_FT_LastPriceLesserEqualStopPrice;
			break;
		case 2:		// 2 大于等于指定价格
			nOrderType = OrderType_FT_LastPriceGreaterEqualStopPrice;
			break;
		case 3:		// 3 小于
			nOrderType = OrderType_FT_LastPriceLesserThanStopPrice;
			break;
		case 4:		// 4 大于
			nOrderType = OrderType_FT_LastPriceGreaterThanStopPrice;
			break;
		default:
			return FALSE;
			break;
		};
		break;
	case 2:	// 2 买价
		switch(nCondType) {
		case 1:		// 1 小于等于指定价格
			nOrderType = OrderType_FT_BidPriceLesserEqualStopPrice;
			break;
		case 2:		// 2 大于等于指定价格
			nOrderType = OrderType_FT_BidPriceGreaterEqualStopPrice;
			break;
		case 3:		// 3 小于
			nOrderType = OrderType_FT_BidPriceLesserThanStopPrice;
			break;
		case 4:		// 4 大于
			nOrderType = OrderType_FT_BidPriceGreaterThanStopPrice;
			break;
		default:
			return FALSE;
			break;
		};
		break;
	case 3: // 3 卖价
		switch(nCondType) {
		case 1:		// 1 小于等于指定价格
			nOrderType = OrderType_FT_AskPriceLesserEqualStopPrice;
			break;
		case 2:		// 2 大于等于指定价格
			nOrderType = OrderType_FT_AskPriceGreaterEqualStopPrice;
			break;
		case 3:		// 3 小于
			nOrderType = OrderType_FT_AskPriceLesserThanStopPrice;
			break;
		case 4:		// 4 大于
			nOrderType = OrderType_FT_AskPriceGreaterThanStopPrice;
			break;
		default:
			return FALSE;
			break;
		};
		break;
	default:
		return FALSE;
		break;
	};
	return TRUE;
}

BOOL COrderAtomEntity::ConvertToAction(PlatformStru_InputOrderAction& actionReq, 
									   PlatformStru_OrderInfo& orderInfo)
{
	actionReq.Thost.FrontID=orderInfo.FrontID;
	actionReq.Thost.SessionID=orderInfo.SessionID;
	memcpy(actionReq.Thost.OrderRef, orderInfo.OrderRef, sizeof(orderInfo.OrderRef));
	memcpy(actionReq.Thost.OrderSysID, orderInfo.OrderSysID, sizeof(orderInfo.OrderSysID));
	memcpy(actionReq.Thost.ExchangeID, orderInfo.ExchangeID, sizeof(orderInfo.ExchangeID));
	memcpy(actionReq.Thost.InstrumentID, orderInfo.InstrumentID, sizeof(orderInfo.InstrumentID));
	return TRUE;
}

BOOL COrderAtomEntity::ConvertToAction(PlatformStru_RemoveParkedOrder& actionReq, 
									   CThostFtdcParkedOrderField& orderInfo)
{
    strncpy(actionReq.Thost.BrokerID, orderInfo.BrokerID, sizeof(actionReq.Thost.BrokerID)-1);
	strncpy(actionReq.Thost.InvestorID, orderInfo.InvestorID, sizeof(actionReq.Thost.InvestorID)-1);
	strncpy(actionReq.Thost.ParkedOrderID, orderInfo.ParkedOrderID, sizeof(actionReq.Thost.ParkedOrderID)-1);
	return TRUE;
}

wxString COrderAtomEntity::MakeRevokeDesciption(PlatformStru_OrderInfo& field)
{
	wxString strValue;
	wxString strOrderSysID = field.OrderSysID;
	strOrderSysID.Trim(false);
	strValue.Printf(LOADFORMATSTRING(CANCELQTYPRC_FORMAT,"%s%s%s%s%s%s%s"), //文本必须包含%s,%s,%s,%s,%s,%s,%s
				strOrderSysID.c_str(), 
				field.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN): 
					(field.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
				field.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL), 
				field.InstrumentID, 
				Number2String(field.VolumeTotalOriginal).c_str(), 
				Double2String(field.LimitPrice).c_str(), 
				field.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ? LOADSTRING(OLS_HEDGE) : 
				(field.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation ? LOADSTRING(OLS_SPECULATE): LOADSTRING(OLS_ARBITRAGE)));
	strValue = REPLACEFMTSTRING(strValue.c_str());
	return strValue;
}

BOOL COrderAtomEntity::SendSrcOrder(BOOL bIsReplace)
{
	if(m_bNoSendVolZoreOrder && m_stdReq.nVolume<=0)
		return FALSE;

	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return NULL;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(m_stdReq.csUserID));
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;

	int nRet = 0;

	PlatformStru_InputOrder		insert={0};	// 正常单域
	PlatformStru_ParkedOrder	parked={0};	// 预埋单域

	m_nRunStatus = conWaiting;
	if(IsParketOrder(m_stdReq)) {
		MakeReq_Parked(parked, m_stdReq.csInstrumentID, m_stdReq.csUserID, 
				m_stdReq.nOrderType, m_stdReq.bIsBuy, m_stdReq.nOCMode, 
				m_stdReq.nVolume, m_stdReq.fPrice, m_stdReq.fTrigger_Price, 
				m_stdReq.bIsHedge, m_stdReq.bIsMarket, bIsReplace);
		nRet = pSvr->ReqParkedOrderInsert(parked, DEFAULT_SVR()->GetNextReqID());
		if(nRet!=0) {
			m_nRunStatus = conIsFail;
			ZeroMemory(&m_lastRspInfo, sizeof(m_lastRspInfo));
			m_lastRspInfo.ErrorID = nRet;
			sprintf_s(m_lastRspInfo.ErrorMsg, sizeof(m_lastRspInfo.ErrorMsg)-1, 
					LOADFORMATSTRING(ORDER_CE_CHANGEFAILMSG,"%d%s"),//文本必须包含%d  %s
					0, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
		}
		else {
			m_bNeedNewOrderStatus = TRUE;
			m_nLastOrderReqID = parked.RequestID;
		}
		return nRet==0;
	}
	else {
		MakeReq_Order(insert, m_stdReq.csInstrumentID, m_stdReq.csUserID, 
				m_stdReq.nOrderType, m_stdReq.bIsBuy, m_stdReq.nOCMode, 
				m_stdReq.nVolume, m_stdReq.fPrice, m_stdReq.fTrigger_Price, 
				m_stdReq.bIsHedge, m_stdReq.bIsMarket, bIsReplace);
		nRet = pSvr->ReqOrderInsert(insert, DEFAULT_SVR()->GetNextReqID());
		if(nRet!=0) {
			m_nRunStatus = conIsFail;
			ZeroMemory(&m_lastRspInfo, sizeof(m_lastRspInfo));
			m_lastRspInfo.ErrorID = nRet;
			sprintf_s(m_lastRspInfo.ErrorMsg, sizeof(m_lastRspInfo.ErrorMsg)-1, 
					LOADFORMATSTRING(ORDER_CE_CHANGEFAILMSG,"%d%s"),//文本必须包含%d  %s
					0, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
		}
		else {
			m_bNeedNewOrderStatus = TRUE;
			m_nLastOrderReqID = insert.RequestID;
		}
		return nRet==0;
	}
}

// 改单，直接使用改单命令字，当状态为conIsSuccess有效
// conVolumeNone不改数据
BOOL COrderAtomEntity::ChangeOrder(int nPriceMode, double fPrice, 
								   int nPriceTriggerMode, double fPriceTrigger, 
								   int nVolumeMode, int nVolume, BOOL bIsReplaceOrder)
{
	if(nVolume<=0)
		return FALSE;
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(m_stdReq.csUserID));
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;

	int nRet = -1;
	STDORDERREQ stdReq = m_stdReq;

	PlatformStru_InputOrder		insert={0};	// 正常单域
	PlatformStru_InputOrderAction actionInsert={0}; 
	PlatformStru_OrderInfo		orderInfo;

	PlatformStru_ParkedOrder	parked={0};	// 预埋单域
	PlatformStru_RemoveParkedOrder	actionParked={0};
	CThostFtdcParkedOrderField	parkedInfo={0};

	if(IsParketOrder(m_stdReq)) {
		parkedInfo = m_arrayParkedRsp[m_nArrayIndex];
		if(parkedInfo.Status!=THOST_FTDC_PAOS_NotSend) 
			return FALSE;
		if(nPriceMode!=conPriceNone) {
			stdReq.bIsMarket = nPriceMode==conPriceMarket;
			if(nPriceMode==conPriceLimit)
				stdReq.fPrice = fPrice;
		}
		if(nPriceTriggerMode!=conPriceNone) {
			stdReq.bTrigger_IsMarket = nPriceTriggerMode==conPriceMarket;
			if(nPriceTriggerMode==conPriceLimit)
				stdReq.fTrigger_Price = fPrice;
		}
		if(nVolumeMode==conVolumeCustom) {
			stdReq.nVolume = nVolume;
		}
		else if(nVolumeMode==conVolumeRemain) {
			stdReq.nVolume = parkedInfo.VolumeTotalOriginal;
		}
		else if(nVolumeMode==conVolumeRemainIncrement) {
			stdReq.nVolume = parkedInfo.VolumeTotalOriginal+nVolume;
		}
		m_nRunStatus = conWaiting;
		m_nNextStep = conStepNothing;
		MakeReq_Parked(parked, m_stdReq.csInstrumentID, m_stdReq.csUserID, 
				m_stdReq.nOrderType, m_stdReq.bIsBuy, m_stdReq.nOCMode, 
				m_stdReq.nVolume, m_stdReq.fPrice, m_stdReq.fTrigger_Price, 
				m_stdReq.bIsHedge, m_stdReq.bIsMarket, bIsReplaceOrder);
		nRet = pSvr->ReqParkedOrderInsert(parked, DEFAULT_SVR()->GetNextReqID());
		if(nRet!=0) {
			m_nRunStatus = conIsFail;
			ZeroMemory(&m_lastRspInfo, sizeof(m_lastRspInfo));
			m_lastRspInfo.ErrorID = nRet;
			sprintf_s(m_lastRspInfo.ErrorMsg, sizeof(m_lastRspInfo.ErrorMsg), 
					LOADFORMATSTRING(ORDER_CE_CHANGEFAILMSG,"%d%s"),//文本必须包含%d  %s
					0, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
		}
		else {
			m_bNeedNewOrderStatus = TRUE;
			m_nLastOrderReqID = parked.RequestID;
		}
		return nRet==0;
	}
	else {
		orderInfo = m_arrayOrderRsp[m_nArrayIndex];
		if(!(orderInfo.OrderStatus==THOST_FTDC_OST_Unknown ||
				orderInfo.OrderStatus==THOST_FTDC_OST_PartTradedQueueing||
				orderInfo.OrderStatus==THOST_FTDC_OST_NoTradeQueueing||
				orderInfo.OrderStatus==THOST_FTDC_OST_NotTouched)) 
			return FALSE;
		if(nPriceMode!=conPriceNone) {
			stdReq.bIsMarket = nPriceMode==conPriceMarket;
			if(nPriceMode==conPriceLimit)
				stdReq.fPrice = fPrice;
		}
		if(nPriceTriggerMode!=conPriceNone) {
			stdReq.bTrigger_IsMarket = nPriceTriggerMode==conPriceMarket;
			if(nPriceTriggerMode==conPriceLimit)
				stdReq.fTrigger_Price = fPrice;
		}
		if(nVolumeMode==conVolumeCustom) {
			stdReq.nVolume = nVolume;
		}
		else if(nVolumeMode==conVolumeRemain) {
			stdReq.nVolume = orderInfo.VolumeTotal;
		}
		else if(nVolumeMode==conVolumeRemainIncrement) {
			stdReq.nVolume = orderInfo.VolumeTotal+nVolume;
		}
		m_nRunStatus = conCancelling;
		m_nNextStep = conStepNothing;
		MakeReq_Order(insert, m_stdReq.csInstrumentID, m_stdReq.csUserID, 
				m_stdReq.nOrderType, m_stdReq.bIsBuy, m_stdReq.nOCMode, 
				m_stdReq.nVolume, m_stdReq.fPrice, m_stdReq.fTrigger_Price, 
				m_stdReq.bIsHedge, m_stdReq.bIsMarket, bIsReplaceOrder);
		nRet = pSvr->ReqOrderInsert(insert, DEFAULT_SVR()->GetNextReqID());
		if(nRet!=0) {
			m_nRunStatus = conIsFail;
			ZeroMemory(&m_lastRspInfo, sizeof(m_lastRspInfo));
			m_lastRspInfo.ErrorID = nRet;
			sprintf_s(m_lastRspInfo.ErrorMsg, sizeof(m_lastRspInfo.ErrorMsg), 
					LOADFORMATSTRING(ORDER_CE_CHANGEFAILMSG,"%d%s"),//文本必须包含%d  %s
					0, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
		}
		else {
			m_bNeedNewOrderStatus = TRUE;
			m_nLastOrderReqID = insert.RequestID;
		}
		return nRet==0;
	}
}

BOOL COrderAtomEntity::CancelOrder(int nNextStep)
{
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(m_stdReq.csUserID));
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;

	int nRet = -1;

	PlatformStru_InputOrderAction actionInsert={0}; 
	PlatformStru_OrderInfo		orderInfo;

	PlatformStru_RemoveParkedOrder	actionParked={0};
	CThostFtdcParkedOrderField	parkedInfo={0};

	if(IsParketOrder(m_stdReq)) {
		parkedInfo = m_arrayParkedRsp[m_nArrayIndex];
		if(parkedInfo.Status!=THOST_FTDC_PAOS_NotSend) 
			return FALSE;
		m_nRunStatus = conCancelling;
		m_nNextStep = nNextStep;
		ConvertToAction(actionParked, parkedInfo);
		m_nLastCancelReqID = DEFAULT_SVR()->GetNextReqID();
		nRet = pSvr->ReqRemoveParkedOrder(actionParked, m_nLastCancelReqID);
		if(nRet!=0) {
			m_nRunStatus = conIsCancelFail;
			ZeroMemory(&m_lastRspInfo, sizeof(m_lastRspInfo));
			m_lastRspInfo.ErrorID = nRet;
			sprintf_s(m_lastRspInfo.ErrorMsg, sizeof(m_lastRspInfo.ErrorMsg), 
					LOADFORMATSTRING(ORDER_CE_CHANGEFAILMSG,"%d%s"),//文本必须包含%d  %s
					0, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
		}
		else {
			m_bNeedNewOrderStatus = FALSE;
			m_nLastOrderReqID = parkedInfo.RequestID;
		}
		return nRet==0;
	}
	else {
		orderInfo = m_arrayOrderRsp[m_nArrayIndex];
		if(!(orderInfo.OrderStatus==THOST_FTDC_OST_Unknown ||
				orderInfo.OrderStatus==THOST_FTDC_OST_PartTradedQueueing ||
				orderInfo.OrderStatus==THOST_FTDC_OST_NoTradeQueueing ||
				orderInfo.OrderStatus==THOST_FTDC_OST_NotTouched)) 
			return FALSE;
		m_nRunStatus = conCancelling;
		m_nNextStep = nNextStep;
		ConvertToAction(actionInsert, orderInfo);
		m_nLastCancelReqID = DEFAULT_SVR()->GetNextReqID();
		nRet = pSvr->ReqOrderAction(actionInsert, m_nLastCancelReqID);
		if(nRet!=0) {
			m_nRunStatus = conIsCancelFail;
			ZeroMemory(&m_lastRspInfo, sizeof(m_lastRspInfo));
			m_lastRspInfo.ErrorID = nRet;
			sprintf_s(m_lastRspInfo.ErrorMsg, sizeof(m_lastRspInfo.ErrorMsg), 
					LOADFORMATSTRING(ORDER_CE_CHANGEFAILMSG,"%d%s"),//文本必须包含%d  %s
					0, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
		}
		else {
			m_bNeedNewOrderStatus = FALSE;
			m_nLastOrderReqID = orderInfo.RequestID;
		}
		return nRet==0;
	}

	return TRUE;
}

// 执行初始设置的下单，只有状态为conNothing时有效
BOOL COrderAtomEntity::Launch(BOOL bNothingOrder)
{
	if(m_nRunStatus!=conNothing)
		return FALSE;
	
	int nRet = 0;
	BOOL bRet = FALSE;

	switch(m_nMode) {
	case conModeNewOrder:
		m_nNextStep = conStepNothing;
		return SendSrcOrder(FALSE);
		break;
	case conModeCancelOnly:
		//m_nNextStep = conStepNothing;
		return CancelOrder();
		break;
	case conModeReplace:
		m_nNextStep = conStepNothing;
		return SendSrcOrder(TRUE);
		break;
	case conModeCancelThenOrder:
		//m_nNextStep = conStepOrderSource;
		return CancelOrder(conStepOrderSource);
		break;
	default:
		return FALSE;
		break;
	};
	
}

// 撤销当前报单，当状态为conIsSuccess有效
BOOL COrderAtomEntity::Cancel()
{
	if(!(m_nRunStatus==conIsSuccess))
		return FALSE;

	return CancelOrder();
}

// 改单，当状态为conIsSuccess有效
BOOL COrderAtomEntity::Replace(int nPriceMode, double fPrice, 
							   int nPriceTriggerMode, double fPriceTrigger, 
							   int nVolumeMode, int nVolume)
{
	if(nVolume==0)
		return FALSE;
	if(m_nRunStatus!=conIsSuccess)
		return FALSE;

	return ChangeOrder(nPriceMode, fPrice, 
			nPriceTriggerMode, fPriceTrigger, nVolumeMode, nVolume, TRUE);
}

// 撤销并重新下单，当状态为conIsSuccess时有效
// conVolumeNone使用剩余数量下单，同conVolumeRemain
BOOL COrderAtomEntity::CancelThenOrder(int nPriceMode, double fPrice, 
									   int nPriceTriggerMode, double fPriceTrigger, 
									   int nVolumeMode, int nVolume)
{
	if(nVolume==0)
		return FALSE;
	if(m_nRunStatus!=conIsSuccess)
		return FALSE;

	if(CancelOrder()) {
		m_nNextStep = conModeCancelThenOrder;
		m_nPriceMode = nPriceMode;
		m_fPrice = fPrice;
		m_nPriceTriggerMode = nPriceTriggerMode;
		m_fPriceTrigger = fPriceTrigger;
		m_nVolumeMode = nVolumeMode;
		m_nVolume = nVolume;
		return TRUE;
	}
	else
		return FALSE;
}

void COrderAtomEntity::ChangePrice(BOOL bIsMarket, double fPrice)
{
	m_stdReq.bIsMarket = bIsMarket;
	m_stdReq.fPrice = fPrice;
}

void COrderAtomEntity::ChangeTriggerCond(int nPriceType, int nCondType, 
										 BOOL bIsMarket, double fPrice, 
										 BOOL bNeedChangeOrderType)
{
	m_stdReq.nTrigger_PriceType = nPriceType;
	m_stdReq.nTrigger_Condition = nCondType;
	m_stdReq.bTrigger_IsMarket = bIsMarket;
	m_stdReq.fTrigger_Price = fPrice;
	if(bNeedChangeOrderType) {
		int nOrderType = 0;
		if(ConvertCondToOrderType(nPriceType, nCondType, nOrderType))
			m_stdReq.nOrderType = nOrderType;
	}
}

BOOL COrderAtomEntity::ParseParkedStatus(DataRspParkedOrderInsert& field)
{
	if(!IsParketOrder(m_stdReq))
		return FALSE;
	if(field.ParkedOrder.RequestID!=m_nLastOrderReqID)
		return FALSE;

	if(m_bNeedNewOrderStatus) {
		m_nArrayIndex++;
		m_bNeedNewOrderStatus = FALSE;
		m_arrayParkedRsp.push_back(field.ParkedOrder);
	}
	else {
		m_arrayParkedRsp[m_nArrayIndex] = field.ParkedOrder;
	}
	switch(field.ParkedOrder.Status) {
	case THOST_FTDC_PAOS_NotSend:
	case THOST_FTDC_PAOS_Send:
		m_nRunStatus = conIsSuccess;
		break;
	case THOST_FTDC_PAOS_Deleted:
		m_nRunStatus = conIsFail;
		break;
	};
	return TRUE;
}

BOOL COrderAtomEntity::ParseOrderStatus(DataRtnOrder& field)
{
	if(IsParketOrder(m_stdReq))
		return FALSE;
	if(field.OrderField.RequestID!=m_nLastOrderReqID)
		return FALSE;

	if(m_bNeedNewOrderStatus) {
		m_nArrayIndex++;
		m_bNeedNewOrderStatus = FALSE;
		m_arrayOrderRsp.push_back(field.OrderField);
	}
	else {
		m_arrayOrderRsp[m_nArrayIndex] = field.OrderField;
	}
	switch(field.OrderField.OrderStatus) {
	case THOST_FTDC_OST_AllTraded:
	case THOST_FTDC_OST_PartTradedQueueing:
	case THOST_FTDC_OST_PartTradedNotQueueing:
	case THOST_FTDC_OST_NoTradeQueueing:
	case THOST_FTDC_OST_NoTradeNotQueueing:
		if(m_nRunStatus!=conCancelling)
			m_nRunStatus = conIsSuccess;
		break;
	case THOST_FTDC_OST_NotTouched:
	case THOST_FTDC_OST_Touched:
		if(m_nRunStatus!=conCancelling)
			m_nRunStatus = conIsSuccess;
		break;
	case THOST_FTDC_OST_Canceled:
		if(m_nRunStatus==conWaiting)
			m_nRunStatus = conIsFail;
		else if(m_nRunStatus==conCancelling) {
			m_nRunStatus = conIsCancelSuccess;
			if(m_nNextStep==conStepOrderSource) {
				m_nNextStep=conStepNothing;
				return SendSrcOrder(FALSE);
			}
			else if(m_nNextStep==conStepOrderChange) {
				m_nNextStep=conStepNothing;
				return ChangeOrder(m_nPriceMode, m_fPrice, 
					m_nPriceTriggerMode, m_fPriceTrigger, 
					m_nVolumeMode, m_nVolume, FALSE);
			}
		}
		break;
	case THOST_FTDC_OST_Unknown:
		switch(field.OrderField.OrderSubmitStatus) {
		case THOST_FTDC_OSS_InsertSubmitted:
		case THOST_FTDC_OSS_ModifySubmitted:
		case THOST_FTDC_OSS_CancelSubmitted:
			//m_nRunStatus = conHasDone;
			break;
		case THOST_FTDC_OSS_Accepted:
			if(m_nRunStatus==conWaiting)
				m_nRunStatus = conIsSuccess;
			else if(m_nRunStatus==conCancelling) {
				m_nRunStatus = conIsCancelSuccess;
				if(m_nNextStep==conStepOrderSource) {
					m_nNextStep=conStepNothing;
					return SendSrcOrder(FALSE);
				}
				else if(m_nNextStep==conStepOrderChange) {
					m_nNextStep=conStepNothing;
					return ChangeOrder(m_nPriceMode, m_fPrice, 
						m_nPriceTriggerMode, m_fPriceTrigger, 
						m_nVolumeMode, m_nVolume, FALSE);
				}
			}
			break;
		case THOST_FTDC_OSS_InsertRejected:
		case THOST_FTDC_OSS_ModifyRejected:
			m_nRunStatus = conIsFail;
			break;
		case THOST_FTDC_OSS_CancelRejected:
			m_nRunStatus = conIsCancelFail;
			break;
		};
		break;
	};
	return TRUE;
}

BOOL COrderAtomEntity::ParseOrderStatus(DataRspOrderInsert& field)
{
	if(m_nRunStatus!=conWaiting)
		return FALSE;
	if(IsParketOrder(m_stdReq))
		return FALSE;
	if(field.InputOrderField.RequestID!=m_nLastOrderReqID)
		return FALSE;

	m_nRunStatus = conIsFail;
	return TRUE;
}

BOOL COrderAtomEntity::ParseActionStatus(DataRspOrderAction1& field)
{
	if(m_nRunStatus!=conCancelling)
		return FALSE;
	if(IsParketOrder(m_stdReq))
		return FALSE;
	if(field.InputOrderActionField.RequestID!=m_nLastCancelReqID)
		return FALSE;

	m_nRunStatus = conIsCancelFail;
	return TRUE;
}

BOOL COrderAtomEntity::ParseActionStatus(DataRspOrderAction2& field)
{
	if(m_nRunStatus!=conCancelling)
		return FALSE;
	if(IsParketOrder(m_stdReq))
		return FALSE;
	if(field.OrderActionField.RequestID!=m_nLastCancelReqID)
		return FALSE;

	m_nRunStatus = conIsCancelFail;
	return TRUE;
}

wxString COrderAtomEntity::GetOrderDesciption()
{
	wxString strText;
	wxString strItem;

	strText = LOADSTRING(NEW_ORDER_F);	
	strItem.Printf(LOADFORMATSTRING(ORDER_CE_CHANGEMSG,"%s%s%s%s%s%d%s"),//文本必须包含%s %s %s %s %s %d  %s
			Double2String(m_stdReq.fPrice).c_str(),
			wxEmptyString, //m_bIsAuto ? LOADSTRING(PARK_AUTO): wxEmptyString,
			m_stdReq.bIsBuy ? LOADSTRING(OLS_BUY) :LOADSTRING(OLS_SELL),
			m_stdReq.csInstrumentID, 
			m_stdReq.nOCMode==1 ? LOADSTRING(OLS_OPEN): 
			(m_stdReq.nOCMode==2 ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)),
			m_stdReq.nVolume,
			m_stdReq.bIsHedge ? LOADSTRING(OLS_HEDGE):LOADSTRING(OLS_SPECULATE));
	strText += strItem;
	strText += wxT(LOADSTRING(ORDER_CE_CHANGECOMFIRM));//wxT("是否确认?");

	return strText;
}
