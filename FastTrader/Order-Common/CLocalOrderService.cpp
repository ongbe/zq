#include "StdAfx.h"
#include "CLocalOrderService.h"
#include "wx/file.h"
#include "wx/filename.h"


//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


CLocalOrderService* g_poLocalOrderService = NULL;
CLocalOrderService g_oLocalOrderService;

//合约交易状态通知回调函数
int CLocalOrderService::CallBack_UserLogin(const GUIModuleID,const AbstractBusinessData &data)
{
	if(data.BID!=BID_RspUserLogin || data.Size!=sizeof(DataRspUserLogin))
		return 0;

	wxCommandEvent evt(wxEVT_ORDER_LOGON, wxID_ANY);
	const DataRspUserLogin& login = (const DataRspUserLogin&)data;
	DataRspUserLogin* pTmp=new DataRspUserLogin(login.bIsLast,login.nRequestID,
			login.bIsLast,login.CurTickCount,login.UserLoginField,login.RspInfoField);
	evt.SetClientData(pTmp);
	g_poLocalOrderService->AddPendingEvent(evt);

	return 0;
}

//合约交易状态通知回调函数
int CLocalOrderService::CallBack_InstrumentStatus(const GUIModuleID,const AbstractBusinessData &data)
{
 	if(g_poLocalOrderService == NULL)
		return 0;
	
	if(data.BID!=BID_RtnInstrumentStatus || data.Size!=sizeof(DataInstrumentStatus))
		return 0;	

	wxCommandEvent evt(wxEVT_PLATFORMSVR_STATUSCHANGE, wxID_ANY);
	DataInstrumentStatus* pTmp=new DataInstrumentStatus();
	memcpy(pTmp, &data, sizeof(DataInstrumentStatus));
	evt.SetClientData(pTmp);
	g_poLocalOrderService->AddPendingEvent(evt);


	return 0;
}

int CLocalOrderService::CallBack_DepthMarketCallBack(const GUIModuleID,const AbstractBusinessData& data)
{
//	WXLOG_INFO("COrderServiceThread::funcDepthMarketCallBack");
	if(data.BID!=BID_RtnDepthMarketData || data.Size!=sizeof(DataRtnDepthMarketData))
		return 0;

	if(!g_poLocalOrderService)
		return 0;

	DataRtnDepthMarketData* pTmp=new DataRtnDepthMarketData();
	memcpy(pTmp, &data, sizeof(DataRtnDepthMarketData));

	//post event to CQuotPanel
	wxCommandEvent evt(wxEVT_ORDERSERVICE_THREAD_NEW_QUOT, wxID_ANY);
	evt.SetClientData(pTmp);
	g_poLocalOrderService->AddPendingEvent(evt);

	return 0;
}

int CLocalOrderService::CallBack_RspOrderInsertCallBackFunc(const GUIModuleID,const AbstractBusinessData& data)
{
	if(!g_poLocalOrderService)
		return 0;

	wxCommandEvent evt(wxEVT_ORDERSERVICE_THREAD_NEW_ORDER, wxID_ANY);
	if(data.BID==BID_RspOrderInsert && data.Size==sizeof(DataRspOrderInsert))
	{
		DataRspOrderInsert* pTmp=new DataRspOrderInsert();
		memcpy(pTmp, &data, sizeof(DataRspOrderInsert));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_ErrRtnOrderInsert && data.Size==sizeof(DataErrRtnOrderInsert))
	{
		DataErrRtnOrderInsert* pTmp=new DataErrRtnOrderInsert();
		memcpy(pTmp, &data, sizeof(DataErrRtnOrderInsert));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RtnOrder && data.Size==sizeof(DataRtnOrder))
	{
		DataRtnOrder* pTmp=new DataRtnOrder();
		memcpy(pTmp, &data, sizeof(DataRtnOrder));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RspOrderAction && data.Size==sizeof(DataRspOrderAction))
	{
		DataRspOrderAction* pTmp=new DataRspOrderAction();
		memcpy(pTmp, &data, sizeof(DataRspOrderAction));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_ErrRtnOrderAction && data.Size==sizeof(DataErrRtnOrderAction))
	{
		DataErrRtnOrderAction* pTmp=new DataErrRtnOrderAction();
		memcpy(pTmp, &data, sizeof(DataErrRtnOrderAction));
		evt.SetClientData(pTmp);
	}
	else
	{
		return 0;
	}

	//post event to OrderInsertPanel
	g_poLocalOrderService->AddPendingEvent(evt);

	return 0;
}

int CLocalOrderService::CallBack_TradeCallBackFunc(const GUIModuleID,const AbstractBusinessData &data)	  //成交回报
{
	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
		return 0;

	if(!g_poLocalOrderService)
		return 0;

	DataRtnTrade* pTmp=new DataRtnTrade();
	memcpy(pTmp, &data, sizeof(DataRtnTrade));

	//post event to COrderServiceThread
	wxCommandEvent evt(wxEVT_ORDERSERVICE_THREAD_NEW_TRADE, wxID_ANY);
	evt.SetClientData(pTmp);
	
	g_poLocalOrderService->AddPendingEvent(evt);
	return 0;
}

//int	CLocalOrderService::sm_nReqNum=0;
int	CLocalOrderService::sm_nBatchOrderNum=0;
int	CLocalOrderService::sm_nBatchSubOrderNum=0;
int	CLocalOrderService::sm_nParkedOrderNum=0;
int	CLocalOrderService::sm_nConditionOrderNum=0;
int	CLocalOrderService::sm_nPLOrderNum=0;
int	CLocalOrderService::sm_nPLDetailOrderNum=0;

CLocalOrderService* CLocalOrderService::GetInstance()
{
	if(g_poLocalOrderService == NULL) 
		g_poLocalOrderService = &g_oLocalOrderService;
	return g_poLocalOrderService;
}

BOOL CLocalOrderService::ConvertPLD2Cond(ORDERPARAM& orderPL, BOOL bIsLose, 
										 PLPARAM& plparam, CONDPARAM& cond, 
										 ORDERPARAM& orderNew)
{
	string strName(orderPL.InstrumentID);
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->GetQuotInfo(strName.c_str(), field);

	orderNew = orderPL;

	if(plparam.nPriceType==conPLPTypeLast)
		cond.nPriceType = conCndPTypeLast;
	else 
	{
		if(orderPL.bIsBuy)
			cond.nPriceType = conCndPTypeAsk;
		else
			cond.nPriceType = conCndPTypeBid;
	}
	if(orderPL.bIsBuy)
		cond.nConditionType = (bIsLose?conCndTypeGreaterEqual:conCndTypeLessEqual);
	else
		cond.nConditionType = (bIsLose?conCndTypeLessEqual:conCndTypeGreaterEqual);

	if(orderPL.bIsBuy)
		cond.fConditionPrice = orderPL.fPrice
				+(bIsLose?plparam.fStopLosePriceOffset:-plparam.fStopGainPriceOffset);
	else
		cond.fConditionPrice = orderPL.fPrice
				-(bIsLose?plparam.fStopLosePriceOffset:-plparam.fStopGainPriceOffset);
	if(plparam.nCloseMode==conPLCMBound)
	{
		if(orderPL.bIsBuy)
			orderNew.fPrice = field.UpperLimitPrice;
		else
			orderNew.fPrice = field.LowerLimitPrice;
	}
	else 
	{
		if(orderPL.bIsBuy)
			orderNew.fPrice = cond.fConditionPrice
					+plparam.fClosePriceOffset;
		else
			orderNew.fPrice = cond.fConditionPrice
					-plparam.fClosePriceOffset;
	}
	return TRUE;
}

//BOOL CLocalOrderService::MakeLocalCondOrder_PLDetail(PLORDER& tPLOrder, PLDETAILORDER& tPLDetail, 
//									CONDITIONORDER& tCondGain, CONDITIONORDER& tCondLose)
BOOL CLocalOrderService::MakeLocalCondOrder_PLDetail(PLDETAILORDER& tPLDetail, 
									CONDITIONORDER& tCondGain, CONDITIONORDER& tCondLose)
{

	string strName(tPLDetail.InstrumentID);
	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(PlatformStru_DepthMarketData));

	// 这个地方需要进行查询优化，如果合约为从服务器下载完毕，如何处理，已下载完毕，如何处理
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->GetQuotInfo(strName.c_str(), field);

	// 生成止盈条件单
	if(tPLDetail.plparam.bDoStopGain)
	{
		//tCondGain.bIsRunInServer = tPLDetail.bIsRunInServer;
		tCondGain.nState = conCondActive;
		tCondGain.nPLNum = tPLDetail.nPLNum;
		tCondGain.nPLDetailNum = tPLDetail.FTID;
		tCondGain.nCondPLRelation = conCPRCondFromPL;
		strncpy(tCondGain.InstrumentID, tPLDetail.InstrumentID, 
				min(sizeof(tCondGain.InstrumentID), sizeof(tPLDetail.InstrumentID)));
		tCondGain.orderSrc = tPLDetail.orderSrc;
		//tCondGain.orderSrc.bIsBuy = !tPLDetail.orderSrc.bIsBuy;
		//tCondGain.orderSrc.nOCMode = 1;	//平今
		if(tPLDetail.plparam.nPriceType==conPLPTypeLast)
			tCondGain.cond.nPriceType = conCndPTypeLast;
		else if(tPLDetail.plparam.nPriceType==conPLPTypeBidAsk) {
			if(tCondGain.orderSrc.bIsBuy)
				tCondGain.cond.nPriceType = conCndPTypeAsk;
			else
				tCondGain.cond.nPriceType = conCndPTypeBid;
		}
		else
			tCondGain.cond.nPriceType = tPLDetail.plparam.nPriceType-1;

		if(tPLDetail.orderSrc.bIsBuy)
			tCondGain.cond.nConditionType = conCndTypeLessEqual;
		else
			tCondGain.cond.nConditionType = conCndTypeGreaterEqual;
		if(tPLDetail.plparam.nPriceOffsetType==conPLPRReqPrice)
		{
			if(tPLDetail.orderSrc.bIsBuy)
				tCondGain.cond.fConditionPrice = tPLDetail.orderSrc.fPrice
						-tPLDetail.plparam.fStopGainPriceOffset;
			else
				tCondGain.cond.fConditionPrice = tPLDetail.orderSrc.fPrice
						+tPLDetail.plparam.fStopGainPriceOffset;
		}
		else
		{
			if(tPLDetail.orderSrc.bIsBuy)
				tCondGain.cond.fConditionPrice = tPLDetail.orderSrc.fPrice
						+tPLDetail.fOpenedPriceOffset
						-tPLDetail.plparam.fStopGainPriceOffset;
			else
				tCondGain.cond.fConditionPrice = tPLDetail.orderSrc.fPrice
						+tPLDetail.fOpenedPriceOffset
						+tPLDetail.plparam.fStopGainPriceOffset;
		}
		if(tPLDetail.plparam.nCloseMode==conPLCMBound)
		{
			if(tPLDetail.orderSrc.bIsBuy)
				tCondGain.orderSrc.fPrice = field.UpperLimitPrice;
			else
				tCondGain.orderSrc.fPrice = field.LowerLimitPrice;
		}
		else 
		{
			if(tPLDetail.orderSrc.bIsBuy)
				tCondGain.orderSrc.fPrice = tCondGain.cond.fConditionPrice
						+tPLDetail.plparam.fClosePriceOffset;
			else
				tCondGain.orderSrc.fPrice = tCondGain.cond.fConditionPrice
						-tPLDetail.plparam.fClosePriceOffset;
		}
		tCondGain.nCreateTime = ::wxGetUTCTime();

		tPLDetail.fStopGainForecast = tCondGain.orderSrc.fPrice;
	}
	else
		tPLDetail.fStopGainForecast = util::GetDoubleInvalidValue();

	// 生成止损条件单
	if(tPLDetail.plparam.bDoStopLose)
	{
		//tCondLose.bIsRunInServer = tPLDetail.bIsRunInServer;
		tCondLose.nState = conCondActive;
		tCondLose.nPLNum = tPLDetail.nPLNum;
		tCondLose.nPLDetailNum = tPLDetail.FTID;
		tCondLose.nCondPLRelation = conCPRCondFromPL;
		strncpy(tCondLose.InstrumentID, tPLDetail.InstrumentID, 
				min(sizeof(tCondLose.InstrumentID), sizeof(tPLDetail.InstrumentID)));
		tCondLose.orderSrc = tPLDetail.orderSrc;
		//tCondLose.orderSrc.bIsBuy = !tPLDetail.orderSrc.bIsBuy;
		//tCondLose.orderSrc.nOCMode = 1;	//平今
		if(tPLDetail.plparam.nPriceType==conPLPTypeLast)
			tCondLose.cond.nPriceType = conCndPTypeLast;
		else if(tPLDetail.plparam.nPriceType==conPLPTypeBidAsk) {
			if(tCondLose.orderSrc.bIsBuy)
				tCondLose.cond.nPriceType = conCndPTypeAsk;
			else
				tCondLose.cond.nPriceType = conCndPTypeBid;
		}
		else
			tCondLose.cond.nPriceType = tPLDetail.plparam.nPriceType-1;

		if(tPLDetail.orderSrc.bIsBuy)
			tCondLose.cond.nConditionType = conCndTypeGreaterEqual;
		else
			tCondLose.cond.nConditionType = conCndTypeLessEqual;
		if(tPLDetail.plparam.nPriceOffsetType==conPLPRReqPrice)
		{
			if(tPLDetail.orderSrc.bIsBuy)
				tCondLose.cond.fConditionPrice = tPLDetail.orderSrc.fPrice
						+tPLDetail.plparam.fStopLosePriceOffset;
			else
				tCondLose.cond.fConditionPrice = tPLDetail.orderSrc.fPrice
						-tPLDetail.plparam.fStopLosePriceOffset;
		}
		else
		{
			if(tPLDetail.orderSrc.bIsBuy)
				tCondLose.cond.fConditionPrice = tPLDetail.orderSrc.fPrice
						+tPLDetail.fOpenedPriceOffset
						+tPLDetail.plparam.fStopLosePriceOffset;
			else
				tCondLose.cond.fConditionPrice = tPLDetail.orderSrc.fPrice
						+tPLDetail.fOpenedPriceOffset
						-tPLDetail.plparam.fStopLosePriceOffset;
		}
		if(tPLDetail.plparam.nCloseMode==conPLCMBound)
		{
			if(tPLDetail.orderSrc.bIsBuy)
				tCondLose.orderSrc.fPrice = field.UpperLimitPrice;
			else
				tCondLose.orderSrc.fPrice = field.LowerLimitPrice;
		}
		else 
		{
			if(tPLDetail.orderSrc.bIsBuy)
				tCondLose.orderSrc.fPrice = tCondLose.cond.fConditionPrice
						+tPLDetail.plparam.fClosePriceOffset;
			else
				tCondLose.orderSrc.fPrice = tCondLose.cond.fConditionPrice
						-tPLDetail.plparam.fClosePriceOffset;
		}
		tCondLose.nCreateTime = ::wxGetUTCTime();
		tPLDetail.fStopLoseForecast = tCondLose.orderSrc.fPrice;
	}
	else
		tPLDetail.fStopLoseForecast = util::GetDoubleInvalidValue();

	return TRUE;

	// bIsSuccess==TRUE，意味着条件满足，应该立即将条件单转化为下单
	//wxString strMsg;
	//double fInputPrice=0.0;
	//BOOL bIsSuccess = FALSE;

	//switch(tOrder.nPriceType) {
	//case 1:
	//	fInputPrice = field.LastPrice;
	//	break;
	//case 2:
	//	fInputPrice = field.BidPrice1;
	//	break;
	//case 3:
	//	fInputPrice = field.AskPrice1;
	//	break;
	//};
	//if(!util::isInvalidValue(fInputPrice)) {
	//if(tOrder.nConditionType==1) {
	//	if(fInputPrice <= tOrder.fConditionPrice) {
	//		bIsSuccess = TRUE;
	//	}
	//}
	//else if(tOrder.nConditionType==2) {
	//	if(fInputPrice >= tOrder.fConditionPrice) {
	//		bIsSuccess = TRUE;
	//	}
	//}
	//else if(tOrder.nConditionType==3) {
	//	if(fInputPrice < tOrder.fConditionPrice) {
	//		bIsSuccess = TRUE;
	//	}
	//}
	//else if(tOrder.nConditionType==4) {
	//	if(fInputPrice > tOrder.fConditionPrice) {
	//		bIsSuccess = TRUE;
	//	}
	//}
	//}

}

BOOL CLocalOrderService::ConvertLocal2ReqOrder(CONDITIONORDER& tCondOrder, 
											   PlatformStru_InputOrder& req)
{
	return MakeReq_Order(req, string(tCondOrder.orderSrc.InstrumentID), string(""), 
				OrderType_FT_Immediately, tCondOrder.orderSrc.bIsBuy, tCondOrder.orderSrc.nOCMode, 
				tCondOrder.orderSrc.nVolume, tCondOrder.orderSrc.fPrice, 0.0, 
				tCondOrder.orderSrc.nHedgeStatus, tCondOrder.orderSrc.bIsMarket, FALSE);
}

BOOL CLocalOrderService::ConvertLocal2ServCond(CONDITIONORDER& tOrder, 
											   PlatformStru_InputOrder& req)
{
	ConvertLocal2ReqOrder(tOrder, req);
	req.StopPrice = tOrder.cond.fConditionPrice;
	switch(tOrder.cond.nPriceType) {
	case 1:	// 1 最新
		switch(tOrder.cond.nConditionType) {
		case 1:		// 1 小于等于指定价格
			req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserEqualStopPrice;
			break;
		case 2:		// 2 大于等于指定价格
			req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterEqualStopPrice;
			break;
		case 3:		// 3 小于
			req.ContingentCondition = THOST_FTDC_CC_LastPriceLesserThanStopPrice;
			break;
		case 4:		// 4 大于
			req.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;
			break;
		};
		break;
	case 2:	// 2 买价
		switch(tOrder.cond.nConditionType) {
		case 1:		// 1 小于等于指定价格
			req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserEqualStopPrice;
			break;
		case 2:		// 2 大于等于指定价格
			req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterEqualStopPrice;
			break;
		case 3:		// 3 小于
			req.ContingentCondition = THOST_FTDC_CC_BidPriceLesserThanStopPrice;
			break;
		case 4:		// 4 大于
			req.ContingentCondition = THOST_FTDC_CC_BidPriceGreaterThanStopPrice;
			break;
		};
		break;
	case 3: // 3 卖价
		switch(tOrder.cond.nConditionType) {
		case 1:		// 1 小于等于指定价格
			req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserEqualStopPrice;
			break;
		case 2:		// 2 大于等于指定价格
			req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterEqualStopPrice;
			break;
		case 3:		// 3 小于
			req.ContingentCondition = THOST_FTDC_CC_AskPriceLesserThanStopPrice;
			break;
		case 4:		// 4 大于
			req.ContingentCondition = THOST_FTDC_CC_AskPriceGreaterThanStopPrice;
			break;
		};
		break;
	};
	return TRUE;
}

BOOL CLocalOrderService::ConvertServ2LocalCond(CThostFtdcInputOrderField& inputOrder, 
											   CONDITIONORDER& tCondOrder)
{
	switch(inputOrder.ContingentCondition) {
	case THOST_FTDC_CC_LastPriceLesserEqualStopPrice:
		tCondOrder.cond.nPriceType = 1;
		tCondOrder.cond.nConditionType = 1;
		break;
	case THOST_FTDC_CC_LastPriceGreaterEqualStopPrice:
		tCondOrder.cond.nPriceType = 1;
		tCondOrder.cond.nConditionType = 2;
		break;
	case THOST_FTDC_CC_LastPriceLesserThanStopPrice:
		tCondOrder.cond.nPriceType = 1;
		tCondOrder.cond.nConditionType = 3;
		break;
	case THOST_FTDC_CC_LastPriceGreaterThanStopPrice:
		tCondOrder.cond.nPriceType = 1;
		tCondOrder.cond.nConditionType = 4;
		break;
	case THOST_FTDC_CC_BidPriceLesserEqualStopPrice:
		tCondOrder.cond.nPriceType = 2;
		tCondOrder.cond.nConditionType = 1;
		break;
	case THOST_FTDC_CC_BidPriceGreaterEqualStopPrice:
		tCondOrder.cond.nPriceType = 2;
		tCondOrder.cond.nConditionType = 2;
		break;
	case THOST_FTDC_CC_BidPriceLesserThanStopPrice:
		tCondOrder.cond.nPriceType = 2;
		tCondOrder.cond.nConditionType = 3;
		break;
	case THOST_FTDC_CC_BidPriceGreaterThanStopPrice:
		tCondOrder.cond.nPriceType = 2;
		tCondOrder.cond.nConditionType = 4;
		break;
	case THOST_FTDC_CC_AskPriceLesserEqualStopPrice:
		tCondOrder.cond.nPriceType = 3;
		tCondOrder.cond.nConditionType = 1;
		break;
	case THOST_FTDC_CC_AskPriceGreaterEqualStopPrice:
		tCondOrder.cond.nPriceType = 3;
		tCondOrder.cond.nConditionType = 2;
		break;
	case THOST_FTDC_CC_AskPriceLesserThanStopPrice:
		tCondOrder.cond.nPriceType = 3;
		tCondOrder.cond.nConditionType = 3;
		break;
	case THOST_FTDC_CC_AskPriceGreaterThanStopPrice:
		tCondOrder.cond.nPriceType = 3;
		tCondOrder.cond.nConditionType = 4;
		break;
	default:
		return FALSE;
		break;
	};
	tCondOrder.cond.fConditionPrice = inputOrder.StopPrice;

	tCondOrder.nState = conCondActive;
	tCondOrder.nCreateTime = ::wxGetUTCTime();
	strncpy(tCondOrder.InstrumentID, inputOrder.InstrumentID, 
			sizeof(tCondOrder.InstrumentID)-1);

	strncpy(tCondOrder.orderSrc.InstrumentID, inputOrder.InstrumentID, 
			sizeof(tCondOrder.orderSrc.InstrumentID)-1);
	tCondOrder.orderSrc.nOrderType = OrderType_FT_Immediately;
	tCondOrder.orderSrc.nOCMode = inputOrder.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? 0 : 
			(inputOrder.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? 1 : 
			(inputOrder.CombOffsetFlag[0]==THOST_FTDC_OF_Close ? 2 : -1));
	tCondOrder.orderSrc.bIsBuy = inputOrder.Direction==THOST_FTDC_D_Buy;
	tCondOrder.orderSrc.bIsMarket = inputOrder.OrderPriceType==THOST_FTDC_OPT_AnyPrice;
	tCondOrder.orderSrc.fPrice = inputOrder.LimitPrice;
	tCondOrder.orderSrc.nVolume = inputOrder.VolumeTotalOriginal;
	tCondOrder.orderSrc.nHedgeStatus = inputOrder.CombHedgeFlag[0]-THOST_FTDC_HF_Speculation;
	return TRUE;
}

BOOL CLocalOrderService::CopyOrderRef(ORDERREF& ref, PlatformStru_OrderInfo& order)
{
	// 复制关键引用信息
	ref.FrontID = order.FrontID;
	ref.SessionID = order.SessionID;
	ref.OrderSubmitStatus = order.OrderSubmitStatus;
	ref.OrderStatus = order.OrderStatus;
	strncpy(ref.OrderSysID, order.OrderSysID, 
			sizeof(order.OrderSysID));
	strncpy(ref.OrderRef, order.OrderRef, 
			sizeof(order.OrderRef));
	strncpy(ref.ExchangeID, order.ExchangeID, 
			sizeof(order.ExchangeID));
	strncpy(ref.Account, order.Account, 
			sizeof(order.Account));
	return TRUE;
}


BEGIN_EVENT_TABLE(CLocalOrderService, wxEvtHandler)//wxPanel)
    EVT_COMMAND(wxID_ANY, wxEVT_ORDER_LOGON, OnRcvLogon)
    EVT_COMMAND(wxID_ANY, wxEVT_ORDERSERVICE_THREAD_NEW_QUOT, OnRcvQuot)
    EVT_COMMAND(wxID_ANY, wxEVT_ORDERSERVICE_THREAD_NEW_ORDER, OnRcvOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERSERVICE_THREAD_NEW_TRADE, OnRcvTrade)
	EVT_COMMAND(wxID_ANY, wxEVT_PLATFORMSVR_STATUSCHANGE, OnPlatSvrStatusChange)
	EVT_COMMAND(wxID_ANY, wxEVT_STOP_PLATFORM,OnStopPlatform)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CLocalOrderService::CLocalOrderService():
//wxThread(wxTHREAD_DETACHED),
	wxEvtHandler()
    //, m_timer(this, ID_TIMEOUTTIMER)
{
	// 这个地方使用Start会引起主线程登录查询关键数据错误
//    m_timer.Start(1000);
	m_nTradingDay = 0;
}

CLocalOrderService::~CLocalOrderService() 
{
	//m_timer.Stop();
}

void CLocalOrderService::OnStopPlatform(wxCommandEvent& evt)
{

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspUserLogin, GID_ORDER_SERVICETHREAD);
	
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnInstrumentStatus, GID_ORDER_SERVICETHREAD);

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnDepthMarketData, GID_ORDER_SERVICETHREAD);

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnTrade, GID_ORDER_SERVICETHREAD);	//订阅成交回报

	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderInsert, GID_ORDER_SERVICETHREAD);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_ErrRtnOrderInsert, GID_ORDER_SERVICETHREAD);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RtnOrder, GID_ORDER_SERVICETHREAD);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_RspOrderAction, GID_ORDER_SERVICETHREAD);
	if(DEFAULT_SVR()) DEFAULT_SVR()->UnSubscribeBusinessData(BID_ErrRtnOrderAction, GID_ORDER_SERVICETHREAD);

	m_setMsgWindow.clear();
	SaveDataToFile();

}

void CLocalOrderService::OnInitSubscribe(wxCommandEvent& evt)
{
	InitService();
}

void CLocalOrderService::OnTimer()
{
	LockObject();

	std::set<long> setForDelete;
	std::set<long>::iterator itDel;
	std::map<long, WaitDelOrNew_ServCond>::iterator it;
	DWORD dwCurrTick = ::GetTickCount();

	for(it = m_mapWaitDelServCond.begin(); it != m_mapWaitDelServCond.end(); it++) {
		if(dwCurrTick - it->second.dwStarTime > 5000)
			setForDelete.insert(it->first);
	}
	for(itDel = setForDelete.begin(); itDel != setForDelete.end(); itDel++) {
		m_mapWaitDelServCond.erase(*itDel);
	}
	setForDelete.clear();

	for(it = m_mapWaitNewServCond.begin(); it != m_mapWaitNewServCond.end(); it++) {
		if(dwCurrTick - it->second.dwStarTime > 5000)
			setForDelete.insert(it->first);
	}
	for(itDel = setForDelete.begin(); itDel != setForDelete.end(); itDel++) {
		m_mapWaitNewServCond.erase(*itDel);
	}
	setForDelete.clear();

	UnlockObject();
}

CLocalOrderService* CLocalOrderService::CreateThread()
{
    //wxThread* pThread = new COrderServiceThread();
	return NULL;//pThread;
}

BOOL CLocalOrderService::InitService()
{
	string strBrokerID="";
	m_strInvestorID="";
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->GetBrokerIDUserID(m_strInvestorID,strBrokerID);
	LoadDataFromFile();
	
	if(DEFAULT_SVR()) {
		m_nTradingDay = atoi(DEFAULT_SVR()->GetTradingDay().c_str());
		CheckOverTradingDay();
	}
	

	//s_poOrderServiceThread = this;

	//定制业务信息
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspUserLogin, GID_ORDER_SERVICETHREAD, CallBack_UserLogin);
	
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_RtnInstrumentStatus, GID_ORDER_SERVICETHREAD, CallBack_InstrumentStatus);

	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_RtnDepthMarketData, GID_ORDER_SERVICETHREAD, CallBack_DepthMarketCallBack);

	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_RtnTrade, GID_ORDER_SERVICETHREAD, CallBack_TradeCallBackFunc);	//订阅成交回报

	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderInsert, GID_ORDER_SERVICETHREAD, CallBack_RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_ErrRtnOrderInsert, GID_ORDER_SERVICETHREAD, CallBack_RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_RtnOrder, GID_ORDER_SERVICETHREAD, CallBack_RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction, GID_ORDER_SERVICETHREAD, CallBack_RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) 
		DEFAULT_SVR()->SubscribeBusinessData(BID_ErrRtnOrderAction, GID_ORDER_SERVICETHREAD, CallBack_RspOrderInsertCallBackFunc);

	//m_timer.Start(100);

	return TRUE;

}

long CLocalOrderService::OrderBatchAdd(BATCHORDER& tOrder) 
{
	sm_nBatchOrderNum++;
	tOrder.nNum = sm_nBatchOrderNum;
	tOrder.nState = conBatchActive;
	m_mapBatchOrder[tOrder.nNum] = tOrder;
	return sm_nBatchOrderNum;
}

BOOL CLocalOrderService::OrderBatchDelete(long nNum) 
{
	return m_mapBatchOrder.erase(nNum);
}

void CLocalOrderService::OrderBatchClear() 
{
	m_mapBatchOrder.clear();
}

BOOL CLocalOrderService::OrderBatchActive(long nNum, BOOL bActive) 
{
	BATCHORDER tOrder={0};
	MAPBATCHORDERITEM it = m_mapBatchOrder.find(nNum);
	if(it != m_mapBatchOrder.end()) {
		tOrder = it->second;
		if(tOrder.nState >= conCondDelete) return FALSE;
		tOrder.nState = bActive ? conBatchActive : conBatchPause;
		m_mapBatchOrder[nNum] = tOrder;
		return TRUE;
	}
	else
		return FALSE;
}

long CLocalOrderService::OrderBatchSubAdd(BATCHSUBORDER& tOrder) 
{
	sm_nBatchSubOrderNum++;
	tOrder.nNum = sm_nBatchSubOrderNum;
	m_mapBatchSubOrder[tOrder.nNum] = tOrder;
	return sm_nBatchSubOrderNum;
}

BOOL CLocalOrderService::OrderBatchSubDelete(long nNum) 
{
	return m_mapBatchSubOrder.erase(nNum);
}

void CLocalOrderService::OrderBatchSubClear() 
{
	m_mapBatchSubOrder.clear();
}

long CLocalOrderService::OrderParkedAdd(PARKEDORDER& tOrder) 
{
	sm_nParkedOrderNum++;
	tOrder.nNum = sm_nParkedOrderNum;
	tOrder.nTradingDay = m_nTradingDay;
	tOrder.nCreateTime = ::wxGetUTCTime();
	m_mapParkedOrder[tOrder.nNum] = tOrder;
	return sm_nParkedOrderNum;
}

BOOL CLocalOrderService::OrderParkedSet(long nNum, PARKEDORDER& tOrder) 
{
	m_mapParkedOrder[nNum] = tOrder;
	return TRUE;
}

BOOL CLocalOrderService::OrderParkedDelete(long nNum) 
{
	MAPPARKEDORDERITEM it = m_mapParkedOrder.find(nNum);
	if(it==m_mapParkedOrder.end()) return FALSE;
	PARKEDORDER tOrder = m_mapParkedOrder[nNum];
	//if(tOrder.nState!=conCondDelete)
	tOrder.nState = conParkedDelete;
	tOrder.nDeleteTime = ::wxGetUTCTime();
	m_mapParkedOrder[nNum] = tOrder;
	return TRUE;
}

void CLocalOrderService::OrderParkedClear() 
{
	m_mapParkedOrder.clear();
}

BOOL CLocalOrderService::OrderParkedActive(long nNum, BOOL bActive) 
{
	PARKEDORDER tOrder={0};
	MAPPARKEDORDERITEM it = m_mapParkedOrder.find(nNum);
	if(it != m_mapParkedOrder.end()) {
		tOrder = it->second;
		if(tOrder.nState >= conParkedDelete) return FALSE;
		tOrder.nState = bActive ? conParkedActive : conParkedPause;
		m_mapParkedOrder[nNum] = tOrder;
		return TRUE;
	}
	else
		return FALSE;
}

long CLocalOrderService::OrderConditionAdd(CONDITIONORDER& tOrder) 
{
	sm_nConditionOrderNum++;
	tOrder.FTID = sm_nConditionOrderNum;
	tOrder.nTradingDay = m_nTradingDay;
	tOrder.nCreateTime = ::wxGetUTCTime();
	tOrder.UpdateSeq++;
	m_mapConditionOrder[tOrder.FTID] = tOrder;
	PostEvent_ConditionStatus(sm_nConditionOrderNum);
	return sm_nConditionOrderNum;
}

BOOL CLocalOrderService::OrderConditionAdd_PLDetail(CONDITIONORDER& tOrder1, CONDITIONORDER& tOrder2) 
{
	sm_nConditionOrderNum++;
	tOrder1.FTID = sm_nConditionOrderNum;
	tOrder1.nState = conCondPause;
	tOrder1.UpdateSeq++;

	sm_nConditionOrderNum++;
	tOrder2.FTID = sm_nConditionOrderNum;
	tOrder2.nState = conCondPause;
	tOrder2.UpdateSeq++;
	
	m_mapConditionOrder[tOrder1.FTID] = tOrder1;
	m_mapConditionOrder[tOrder2.FTID] = tOrder2;
	return TRUE;
}

BOOL CLocalOrderService::OrderConditionSet(long nNum, CONDITIONORDER& tOrder) 
{
	MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
	if(it != m_mapConditionOrder.end()) 
		tOrder.UpdateSeq = it->second.UpdateSeq;
	tOrder.UpdateSeq++;
	m_mapConditionOrder[nNum] = tOrder;
	PostEvent_ConditionStatus(nNum);
	return TRUE;
}

BOOL CLocalOrderService::OrderConditionDelete(long nNum, int nState, BOOL bIsPostEvent) 
{
	MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
	if(it==m_mapConditionOrder.end()) return FALSE;
	CONDITIONORDER& tOrder = it->second;
	if(nState==conCondDelete && 
			!(tOrder.nState==conCondPause || tOrder.nState==conCondActive))
		return FALSE;
	tOrder.nState = nState;
	tOrder.nDeleteTime = ::wxGetUTCTime();
	tOrder.UpdateSeq++;
	if(bIsPostEvent)
		PostEvent_ConditionStatus(nNum);
	return TRUE;
}

void CLocalOrderService::OrderConditionClear() 
{
	m_mapConditionOrder.clear();
}

BOOL CLocalOrderService::OrderConditionActive(long nNum, BOOL bActive) 
{
	MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
	if(it != m_mapConditionOrder.end()) {
		CONDITIONORDER& tOrder = it->second;
		if(tOrder.nState >= conCondDelete) return FALSE;
		tOrder.nState = bActive ? conCondActive : conCondPause;
		tOrder.UpdateSeq++;
		return TRUE;
	}
	else
		return FALSE;
}

long CLocalOrderService::OrderPLAdd(PLORDER& tOrder) 
{
	sm_nPLOrderNum++;
	tOrder.FTID = sm_nPLOrderNum;
	tOrder.UpdateSeq++;
	tOrder.nTradingDay = m_nTradingDay;
	m_mapPLOrder[tOrder.FTID] = tOrder;
	PostEvent_PLStatus(sm_nPLOrderNum);
	return sm_nPLOrderNum;
}

BOOL CLocalOrderService::OrderPLSet(long nNum, PLORDER& tOrder) 
{
	MAPPLORDERITEM it = m_mapPLOrder.find(nNum);
	if(it != m_mapPLOrder.end()) 
		tOrder.UpdateSeq = it->second.UpdateSeq;
	tOrder.UpdateSeq++;
	m_mapPLOrder[nNum] = tOrder;
	PostEvent_PLStatus(nNum);
	return TRUE;
}

BOOL CLocalOrderService::OrderPLDelete(long nNum, int nState, BOOL bIsPostEvent) 
{
	MAPPLORDERITEM it = m_mapPLOrder.find(nNum);
	if(it==m_mapPLOrder.end()) return FALSE;

	PLORDER& tPosOrder = it->second;
	if(nState==conPLDelete && !(tPosOrder.nState==conPLPause || tPosOrder.nState==conPLActive))
		return FALSE;
	tPosOrder.nState = nState;
	tPosOrder.UpdateSeq++;
	if(bIsPostEvent)
		PostEvent_PLStatus(nNum);
	return TRUE;
}

void CLocalOrderService::OrderPLClear() 
{
	m_mapPLOrder.clear();
}

long CLocalOrderService::OrderPLDetailAdd(PLDETAILORDER& tOrder) 
{
	sm_nPLDetailOrderNum++;
	tOrder.FTID = sm_nPLDetailOrderNum;
	tOrder.nTradingDay = m_nTradingDay;
	tOrder.UpdateSeq++;
	m_mapPLDetailOrder[tOrder.FTID] = tOrder;
	PostEvent_PLDetailStatus(sm_nPLDetailOrderNum, tOrder.nPLNum);
	return sm_nPLDetailOrderNum;
}

BOOL CLocalOrderService::OrderPLDetailSet(long nNum, PLDETAILORDER& tOrder) 
{
	MAPPLDETAILORDERITEM it = m_mapPLDetailOrder.find(nNum);
	if(it != m_mapPLDetailOrder.end()) 
		tOrder.UpdateSeq = it->second.UpdateSeq;
	tOrder.UpdateSeq++;
	m_mapPLDetailOrder[nNum] = tOrder;
	PostEvent_PLDetailStatus(nNum, tOrder.nPLNum);
	return TRUE;
}

BOOL CLocalOrderService::OrderPLDetailDelete(long nNum, int nState, BOOL bIsPostEvent) 
{
	MAPPLDETAILORDERITEM it = m_mapPLDetailOrder.find(nNum);
	if(it==m_mapPLDetailOrder.end()) return FALSE;

	PLDETAILORDER& tPLDetailOrder = it->second;
	if(nState==conPosiPLDelete && tPLDetailOrder.nState<conPosiPLDelete ) {
		//if(!tPLDetailOrder.bIsRunInServer)
		if(!tPLDetailOrder.bGainIsServ)
			OrderConditionDelete(tPLDetailOrder.refGain.FTID, conPosiPLDelete, bIsPostEvent);
		if(!tPLDetailOrder.bLoseIsServ)
			OrderConditionDelete(tPLDetailOrder.refLose.FTID, conPosiPLDelete, bIsPostEvent);
		if(tPLDetailOrder.bGainIsServ || tPLDetailOrder.bLoseIsServ)
			// 撤销服务器条件单
			OrderCondServDelete_PLDetail(tPLDetailOrder, bIsPostEvent);
	}
	if(nState==conPosiPLDelete && 
			!(tPLDetailOrder.nState==conPosiPLPause || tPLDetailOrder.nState==conPosiPLActive))
		return FALSE;
	tPLDetailOrder.nState = nState;
	tPLDetailOrder.UpdateSeq++;
	if(bIsPostEvent)
	{
		PostEvent_PLDetailStatus(nNum, tPLDetailOrder.nPLNum);
		// 让条件单查询面板订阅和处理PLDetailChange事件可以解决这个问题
		//// 临时的用法，当界面改为收到事件后只改变相关的记录时，删除这个用法
		//if(!tPLDetailOrder.bIsRunInServer) 
		//{
		//	PostEvent_ConditionStatus(tPLDetailOrder.refLose.FTID);
		//	PostEvent_ConditionStatus(tPLDetailOrder.refGain.FTID);
		//}
	}
	return TRUE;
}

BOOL CLocalOrderService::OrderPLDetailDeleteAll(int nState, BOOL bIsPostEvent) 
{
	MAPPLDETAILORDERITEM it;
	for(it = m_mapPLDetailOrder.begin(); it != m_mapPLDetailOrder.end(); it++) {
		PLDETAILORDER& tPLDetailOrder = it->second;
		if(nState==conPLDetailDelete && tPLDetailOrder.nState!=conPLDetailDelete ) {
			//if(!tPLDetailOrder.bIsRunInServer)
			if(!tPLDetailOrder.bGainIsServ)
				OrderConditionDelete(tPLDetailOrder.refGain.FTID, conPosiPLDelete, FALSE);
			if(!tPLDetailOrder.bLoseIsServ)
				OrderConditionDelete(tPLDetailOrder.refLose.FTID, conPosiPLDelete, FALSE);
			if(tPLDetailOrder.bGainIsServ || tPLDetailOrder.bLoseIsServ)
				// 撤销服务器条件单
				OrderCondServDelete_PLDetail(tPLDetailOrder, bIsPostEvent);
		}
		if(nState==conPLDetailDelete && 
				!(tPLDetailOrder.nState==conPLDetailPause || tPLDetailOrder.nState==conPLDetailActive))
			continue;
		tPLDetailOrder.nState = nState;
		tPLDetailOrder.UpdateSeq++;
	}
	if(bIsPostEvent)
	{
		PostEvent_PLDetailStatus(-1, -1);
		PostEvent_ConditionStatus(-1);
		// 让条件单查询面板订阅和处理PLDetailChange事件可以解决这个问题
		//// 临时的用法，当界面改为收到事件后只改变相关的记录时，删除这个用法
		//if(!tPLDetailOrder.bIsRunInServer) 
		//{
		//	PostEvent_ConditionStatus(tPLDetailOrder.refLose.FTID);
		//	PostEvent_ConditionStatus(tPLDetailOrder.refGain.FTID);
		//}
	}
	return TRUE;
}

int CLocalOrderService::CancelCondServer(ORDERREF& ref, char* InstrumentID)
{
	PlatformStru_InputOrderAction ReqData;
	ZeroMemory(&ReqData, sizeof(ReqData));
	ReqData.Thost.FrontID=ref.FrontID;
	ReqData.Thost.SessionID=ref.SessionID;
	memcpy(ReqData.Thost.OrderRef, ref.OrderRef, sizeof(ref.OrderRef));
	memcpy(ReqData.Thost.OrderSysID, ref.OrderSysID, sizeof(ref.OrderSysID));
	memcpy(ReqData.Thost.ExchangeID, ref.ExchangeID, sizeof(ref.ExchangeID));
	memcpy(ReqData.Thost.InstrumentID, InstrumentID, sizeof(ReqData.Thost.InstrumentID));
	int ret=DEFAULT_SVR()
			? DEFAULT_SVR()->ReqOrderAction(ReqData, 
			DEFAULT_SVR()->GetNextReqID()):-1;
	return ret;
}

BOOL CLocalOrderService::OrderCondServDelete_PLDetail(PLDETAILORDER plDetail, BOOL bIsPostEvent) 
{
	if(!plDetail.bIsRunInServer)
		return FALSE;
	if(plDetail.plparam.bDoStopGain)
	{
		OrderKey orderKey(plDetail.refGain.Account, plDetail.InstrumentID, 
				plDetail.refGain.FrontID, plDetail.refGain.SessionID, plDetail.refGain.OrderRef);
		PlatformStru_OrderInfo outData;

		if(DEFAULT_SVR()
				&& DEFAULT_SVR()->GetOrder(orderKey, outData)) 
		{
			if(outData.OrderStatus == THOST_FTDC_OST_NotTouched) 
				CancelCondServer(plDetail.refGain, plDetail.InstrumentID);
		}
	}
	if(plDetail.plparam.bDoStopLose)
	{
		OrderKey orderKey(plDetail.refLose.Account, plDetail.InstrumentID, 
				plDetail.refLose.FrontID, plDetail.refLose.SessionID, plDetail.refLose.OrderRef);
		PlatformStru_OrderInfo outData;

		if(DEFAULT_SVR()
				&& DEFAULT_SVR()->GetOrder(orderKey, outData)) 
		{
			if(outData.OrderStatus == THOST_FTDC_OST_NotTouched) 
				CancelCondServer(plDetail.refLose, plDetail.InstrumentID);
		}
	}
	return TRUE;
}

void CLocalOrderService::OrderPLDetailClear() 
{
	m_mapPLDetailOrder.clear();
}

void CLocalOrderService::OrderParked(PARKEDORDER& tOrder)
{
	
	//CThostFtdcInputOrderField req;
	//ZeroMemory(&req, sizeof(CThostFtdcInputOrderField));

	PlatformStru_InputOrder& req=tOrder.req;

	int ret = 0;

 	ret = VerifyOrder(req);

	tOrder.nRegNum=req.RequestID;//sm_nReqNum;
	tOrder.nState = conParkedConfirm;
	tOrder.nRegState=(ret==0 ? conReqSendOk : conReqSendFail);
	if(ret!=0) {
		//sm_nReqNum--;
		//设置错误信息
		strcpy(tOrder.strErrorMsg, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
	}
	else {
		strcpy(tOrder.strErrorMsg, LOADSTRING(SENT_SERVER_SUCCESS));
	}
	PostEvent_ParkedStatus(tOrder.nNum);
}

void CLocalOrderService::OrderCondition(CONDITIONORDER& tOrder, BOOL bIsServCond)
{
	PlatformStru_InputOrder req;
	ZeroMemory(&req, sizeof(PlatformStru_InputOrder));
	
	//if(tOrder.bIsRunInServer)
	if(bIsServCond)
		ConvertLocal2ServCond(tOrder, req);
	else
		ConvertLocal2ReqOrder(tOrder, req);

	int ret = 0;

 	ret = VerifyOrder(req);
	tOrder.ref.RequestID=req.RequestID;
	tOrder.nState = conCondConfirm;
	tOrder.nRegState=(ret==0 ? conReqSendOk : conReqSendFail);
	if(ret!=0) {
		//sm_nReqNum--;
		//设置错误信息
		strcpy(tOrder.strErrorMsg, CFieldValueTool::ReturnCode2String(ret,SVR_LANGUAGE).c_str());
	}
	else {
		strcpy(tOrder.strErrorMsg, LOADSTRING(SENT_SERVER_SUCCESS));
	}

}

BOOL CLocalOrderService::MakePLDetailAndRun(PLORDER& tPLOrder, PlatformStru_TradeInfo& traderInfo)
{
	PLDETAILORDER tPLDetail;
	ZeroMemory(&tPLDetail, sizeof(tPLDetail));

	tPLDetail.bIsRunInServer = tPLOrder.bIsRunInServer;
	tPLDetail.bGainIsServ = tPLOrder.bIsRunInServer;
	tPLDetail.bLoseIsServ = tPLOrder.bIsRunInServer;
	tPLDetail.nPLNum = tPLOrder.FTID;
	tPLDetail.nState = conPLDetailActive;//conPLDetailConfirm;
	strcpy(tPLDetail.InstrumentID, traderInfo.InstrumentID);

	// 复制关键引用信息
	tPLDetail.ref = tPLOrder.ref;
	strncpy(tPLDetail.ref.TradeID, traderInfo.TradeID, 
			sizeof(traderInfo.TradeID));
	tPLDetail.plparam = tPLOrder.plparam;
	tPLDetail.orderSrc = tPLOrder.orderSrc;
	if(tPLDetail.orderSrc.nOCMode==0) {
		tPLDetail.orderSrc.bIsBuy = !tPLOrder.orderSrc.bIsBuy;
		tPLDetail.orderSrc.nOCMode = 1;
	}
	tPLDetail.orderSrc.nVolume = traderInfo.Volume;

	if(tPLDetail.orderSrc.bIsMarket)
		tPLDetail.orderSrc.fPrice = traderInfo.Price;
	tPLDetail.fOpenedPriceOffset = traderInfo.Price - tPLDetail.orderSrc.fPrice;
	if(tPLDetail.plparam.bDoStopGain)
	{
		// 如果比较价格为成交价
		if(tPLDetail.plparam.nPriceOffsetType==1) 
			tPLDetail.fStopGainPrice = 
					traderInfo.Price + 
					(tPLDetail.orderSrc.bIsBuy ? -tPLDetail.plparam.fStopGainPriceOffset
					: tPLDetail.plparam.fStopGainPriceOffset);
		else
			tPLDetail.fStopGainPrice = 
					tPLDetail.orderSrc.fPrice + 
					(tPLDetail.orderSrc.bIsBuy ? -tPLDetail.plparam.fStopGainPriceOffset
					: tPLDetail.plparam.fStopGainPriceOffset);
	}
	if(tPLDetail.plparam.bDoStopLose)
	{
		// 如果比较价格为成交价
		if(tPLDetail.plparam.nPriceOffsetType==1) 
			tPLDetail.fStopLosePrice = 
					traderInfo.Price + 
					(tPLDetail.orderSrc.bIsBuy ? tPLDetail.plparam.fStopLosePriceOffset
					: -tPLDetail.plparam.fStopLosePriceOffset);
		else
			tPLDetail.fStopLosePrice = 
					tPLDetail.orderSrc.fPrice + 
					(tPLDetail.orderSrc.bIsBuy ? tPLDetail.plparam.fStopLosePriceOffset
					: -tPLDetail.plparam.fStopLosePriceOffset);
	}
	OrderPLDetailAdd(tPLDetail);

	CONDITIONORDER tCondGain, tCondLose;
	ZeroMemory(&tCondGain, sizeof(tCondGain));
	ZeroMemory(&tCondLose, sizeof(tCondLose));

	//MakeLocalCondOrder_PLDetail(tPLOrder, tPLDetail, tCondGain, tCondLose);
	MakeLocalCondOrder_PLDetail(tPLDetail, tCondGain, tCondLose);
	if(tPLDetail.plparam.bDoStopGain)
	{
		if(tPLDetail.bGainIsServ) {
		//if(tPLDetail.bIsRunInServer) {
			OrderCondition(tCondGain, TRUE);
			tPLDetail.refGain.RequestID = tCondGain.ref.RequestID;
		}
		else 
		{
			OrderConditionAdd(tCondGain);
			tPLDetail.refGain.FTID = tCondGain.FTID;
		}
	}
	if(tPLDetail.plparam.bDoStopLose)
	{
		if(tPLDetail.bLoseIsServ) {
		//if(tPLDetail.bIsRunInServer) {
			OrderCondition(tCondLose, TRUE);
			tPLDetail.refLose.RequestID = tCondLose.ref.RequestID;
		}
		else
		{
			OrderConditionAdd(tCondLose);
			tPLDetail.refLose.FTID = tCondLose.FTID;
		}
	}
	OrderPLDetailSet(tPLDetail.FTID, tPLDetail);
	return TRUE;
}

void LogTriggerCond(CLocalOrderService::CONDITIONORDER& tOrder, double fInputPrice, 
					PlatformStru_DepthMarketData& rawData)
{
	string strPriceType;
	switch(tOrder.cond.nPriceType) {
	case 1:
		strPriceType = "最新价";
		break;
	case 2:
		strPriceType = "买一价";
		break;
	case 3:
		strPriceType = "卖一价";
		break;
	};
	string strCondType;
	switch(tOrder.cond.nConditionType) {
	case 1:
		strCondType = "<=";
		break;
	case 2:
		strCondType = ">=";
		break;
	case 3:
		strCondType = "<";
		break;
	case 4:
		strCondType = ">";
		break;
	};
	USERLOG_INFO("条件单行情触发，编号[%d], 合约[%s]，方向[%s]，开平[%s], 触发价格[%0.3f]，触发类型[%s]，比较方式[%s]，行情触发价格[%0.3f], 最新价[%0.3f], 买一价[%0.3f], 卖一价[%0.3f]\n", 
			tOrder.FTID, 
			tOrder.orderSrc.InstrumentID,
			(tOrder.orderSrc.bIsBuy?"买入":"卖出"),
			(tOrder.orderSrc.nOCMode==0?"开仓":(tOrder.orderSrc.nOCMode==1?"平今":"平仓")),
			tOrder.cond.fConditionPrice,
			strPriceType.c_str(),
			strCondType.c_str(),
			fInputPrice,
			rawData.LastPrice,
			rawData.BidPrice1,
			rawData.AskPrice1);
}

void CLocalOrderService::DoConditionOrder(PlatformStru_DepthMarketData& rawData)
{

	std::string strInputCode(rawData.InstrumentID);
	double fInputPrice = 0.0;

	std::set<long> setChange;
	std::map<long, long> mapChangePLDetail;

	for(MAPCONDITIONORDERITEM it=m_mapConditionOrder.begin(); it!=m_mapConditionOrder.end(); ++it) {
		CONDITIONORDER& tOrder = it->second;
		//if(tOrder.bIsRunInServer)
		//	continue;
		if(!(tOrder.nState==conCondActive))
			continue;
		if(strInputCode.compare(tOrder.orderSrc.InstrumentID) == 0) {
			switch(tOrder.cond.nPriceType) {
			case 1:
				fInputPrice = rawData.LastPrice;
				break;
			case 2:
				fInputPrice = rawData.BidPrice1;
				break;
			case 3:
				fInputPrice = rawData.AskPrice1;
				break;
			};
			if(util::isInvalidValue(fInputPrice)) 
				continue;
			//if((tOrder.cond.nConditionType==1 && fInputPrice <= tOrder.cond.fConditionPrice)
			//		|| (tOrder.cond.nConditionType==2 && fInputPrice >= tOrder.cond.fConditionPrice)
			//		|| (tOrder.cond.nConditionType==3 && fInputPrice < tOrder.cond.fConditionPrice)
			//		|| (tOrder.cond.nConditionType==4 && fInputPrice > tOrder.cond.fConditionPrice))
			if((tOrder.cond.nConditionType==1 && util::lessOrEqual(fInputPrice, tOrder.cond.fConditionPrice))
					|| (tOrder.cond.nConditionType==2 && util::greaterOrEqual(fInputPrice, tOrder.cond.fConditionPrice))
					|| (tOrder.cond.nConditionType==3 && util::less(fInputPrice, tOrder.cond.fConditionPrice))
					|| (tOrder.cond.nConditionType==4 && util::greater(fInputPrice, tOrder.cond.fConditionPrice)))
			{
				LogTriggerCond(tOrder, fInputPrice, rawData);
				//USERLOG_INFO("条件单行情触发，编号[%d], 合约[%s]，方向[%d]，触发价格[%0.3f]，触发类型[%d]，比较方式[%d]，行情触发价格[%0.3f], 最新价[%0.3f], 买一价[%0.3f], 卖一价[%0.3f]\n", 
				//		tOrder.FTID, 
				//		tOrder.orderSrc.InstrumentID,
				//		tOrder.orderSrc.bIsBuy,
				//		tOrder.cond.fConditionPrice,
				//		tOrder.cond.nPriceType,
				//		tOrder.cond.nConditionType,
				//		fInputPrice,
				//		rawData.LastPrice,
				//		rawData.BidPrice1,
				//		rawData.AskPrice1);
				OrderCondition(tOrder);
				tOrder.UpdateSeq++;
				tOrder.nState = conCondConfirm;
				//m_mapConditionOrder[it->first] = tOrder;
				PostEvent_ConditionStatus(tOrder.FTID);
				if(tOrder.nPLDetailNum>0) {
					PostEvent_PLDetailStatus(tOrder.nPLDetailNum, 0);
				}
				setChange.insert(it->first);
				if(tOrder.nCondPLRelation==conCPRCondFromPL)
					mapChangePLDetail.insert(
							std::pair<long, long>(tOrder.nPLDetailNum, tOrder.FTID));
			}
		}
    }

	long nNum = 0;
	MAPPLDETAILORDERITEM itPos;
	MAPCONDITIONORDERITEM itCond;
	std::map<long, long>::iterator it = mapChangePLDetail.begin();
	
	for(; it!=mapChangePLDetail.end(); it++) {
		itPos = m_mapPLDetailOrder.find(it->first);
		if(itPos!=m_mapPLDetailOrder.end())
		{
			if(itPos->second.refGain.FTID==it->second)
				nNum = itPos->second.refLose.FTID;
			else
				nNum = itPos->second.refGain.FTID;
			itCond = m_mapConditionOrder.find(nNum);
			if(itCond != m_mapConditionOrder.end())
			{
				CONDITIONORDER& tOrder = itCond->second;
				if(tOrder.nCondPLRelation == conCPRCondFromPL) {
					OrderConditionDelete(nNum, conCondDelete, TRUE);
					//PostEvent_ConditionStatus(nNum);
				}
			}
			PLDETAILORDER& tPosOrder = itPos->second;
			//tPosOrder.nState = conPLDetailConfirm;
			//m_mapPLDetailOrder[tPosOrder.FTID]=tPosOrder;
			OrderPLDetailDelete(tPosOrder.FTID, conPLDetailConfirm);
			PostEvent_PLStatus(tPosOrder.FTID);
		}
	}
	mapChangePLDetail.clear();
	setChange.clear();

}

BOOL CLocalOrderService::OrderParkedDoNow(long nNum)
{
	PARKEDORDER tOrder={0};
	MAPPARKEDORDERITEM it = m_mapParkedOrder.find(nNum);
	if(it != m_mapParkedOrder.end()) {
		tOrder = it->second;
		if(tOrder.nState != conCondActive) return FALSE;
		OrderParked(tOrder);
		m_mapParkedOrder[nNum]=tOrder;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CLocalOrderService::OrderConditionDoNow(long nNum)
{
	MAPCONDITIONORDERITEM it = m_mapConditionOrder.find(nNum);
	if(it != m_mapConditionOrder.end()) {
		CONDITIONORDER& tOrder = it->second;
		if(tOrder.nState != conCondActive) return FALSE;
		OrderCondition(tOrder);
		OrderConditionDelete(nNum, conCondConfirm);
		return TRUE;
	}
	else
		return FALSE;
}

//底层平台发过来一条登录应答
void CLocalOrderService::OnRcvLogon(wxCommandEvent& evt)
{
    DataRspUserLogin* pTmp = (DataRspUserLogin*)(evt.GetClientData());
    if(!pTmp) {
        wxASSERT(pTmp);
        return;
    }
	if(!pTmp->bIsLast || pTmp->RspInfoField.ErrorID!=0)
		return;

	m_nTradingDay = atoi(pTmp->UserLoginField.TradingDay);

	LockObject();
	CheckOverTradingDay();
	UnlockObject();

    wxDELETE(pTmp)
    evt.SetClientData(NULL);
}

//底层平台发过来一条新行情
void CLocalOrderService::OnRcvQuot(wxCommandEvent& evt)
{

    DataRtnDepthMarketData* pTmp = (DataRtnDepthMarketData*)(evt.GetClientData());
    if(!pTmp) {
        wxASSERT(pTmp);
        return;
    }

	LockObject();
	DoConditionOrder(pTmp->DepthMarketDataField);
	UnlockObject();

    wxDELETE(pTmp)
    evt.SetClientData(NULL);

}

void CLocalOrderService::OnRcvOrder(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData)
	{
		wxASSERT(pAbstractBusinessData);
		return;
	}
	
	LockObject();

	switch (pAbstractBusinessData->BID) {
		case BID_RtnOrder:
			{
				DataRtnOrder& RawData= *(DataRtnOrder*)(pAbstractBusinessData);
				//if(!IsOwnerOrder(RawData.OrderField))
				//{
				//	UnlockObject();
				//	return;
				//}
				if(!HasOwner(RawData.OrderField.FrontID, RawData.OrderField.SessionID)) {
					UnlockObject();
					return;
				}

				if(RawData.OrderField.OrderStatus==THOST_FTDC_OST_Canceled) {
					std::set<long> setForDelete;
					std::set<long>::iterator itDel;
					std::map<long, WaitDelOrNew_ServCond>::iterator it;

					for(it = m_mapWaitDelServCond.begin(); it != m_mapWaitDelServCond.end(); it++) {
						if(strcmp(it->second.oldCondRef.Account, RawData.OrderField.Account)==0 && 
								strcmp(it->second.oldCondRef.ExchangeID, RawData.OrderField.ExchangeID)==0 &&
								strcmp(it->second.oldCondRef.OrderSysID, RawData.OrderField.OrderSysID)==0) {
							CONDITIONORDER condOrder;
							PlatformStru_InputOrder req;
							ZeroMemory(&condOrder, sizeof(condOrder));
							ZeroMemory(&req, sizeof(req));
							ConvertPLD2Cond(it->second.newOrder, it->second.bIsLose, 
									it->second.plparam, condOrder.cond, condOrder.orderSrc);
							OrderCondition(condOrder, TRUE);
							it->second.newCondRef = condOrder.ref;
							setForDelete.insert(it->first);
							m_mapWaitNewServCond.insert(make_pair(it->first, it->second));
							break;
						}
					}
					for(itDel = setForDelete.begin(); itDel != setForDelete.end(); itDel++) {
						m_mapWaitDelServCond.erase(*itDel);
					}
					if(setForDelete.size())
						PostEvent_PLDetailStatus(-1, -1);
					setForDelete.clear();

				}

				if(RawData.OrderField.OrderStatus==THOST_FTDC_OST_NotTouched) {
					std::set<long> setForDelete;
					std::set<long>::iterator itDel;
					std::map<long, WaitDelOrNew_ServCond>::iterator it;
					for(it = m_mapWaitNewServCond.begin(); it != m_mapWaitNewServCond.end(); it++) {
						if(//strcmp(it->second.newCondRef.Account, RawData.OrderField.Account)==0 && 
								it->second.newCondRef.RequestID == RawData.OrderField.RequestID) {
							MAPPLDETAILORDERITEM itPLD = m_mapPLDetailOrder.find(it->second.nPosiPLNum);
							if(itPLD != m_mapPLDetailOrder.end()) {
								if(it->second.bIsLose)
									CopyOrderRef(itPLD->second.refLose, RawData.OrderField);
								else
									CopyOrderRef(itPLD->second.refGain, RawData.OrderField);
								itPLD->second.UpdateSeq++;
							}
							setForDelete.insert(it->first);
							break;
						}
					}
					for(itDel = setForDelete.begin(); itDel != setForDelete.end(); itDel++) {
						m_mapWaitNewServCond.erase(*itDel);
					}
					if(setForDelete.size())
						PostEvent_PLDetailStatus(-1, -1);
					setForDelete.clear();
				}

				MAPCONDITIONORDERITEM itCond;
				MAPPLDETAILORDERITEM itPLDetail;
				MAPPLORDERITEM itPL;
				std::map<long, long> mapChangePLDetail;

				for(itCond=m_mapConditionOrder.begin(); 
						itCond!=m_mapConditionOrder.end(); itCond++) {
					CONDITIONORDER& tCondOrder = itCond->second;
					if(tCondOrder.nState==conCondPause 
							|| tCondOrder.nState==conCondDelete 
							|| tCondOrder.nState==conCondCompleted 
							|| tCondOrder.nState==conCondCancel) continue;
					if(tCondOrder.ref.RequestID == RawData.OrderField.RequestID) {
						// 复制关键引用信息
						CopyOrderRef(tCondOrder.ref, RawData.OrderField);
						switch(RawData.OrderField.OrderStatus) {
						case THOST_FTDC_OST_Canceled://下单后，CTP自动撤单
							tCondOrder.nRegState = conReqEntrustFail;
							tCondOrder.nState = conCondCancel;
							tCondOrder.UpdateSeq++;
							PostEvent_ConditionStatus(tCondOrder.FTID);
							if(tCondOrder.nPLDetailNum>0) {
								PostEvent_PLDetailStatus(tCondOrder.nPLDetailNum, 0);
							}
							break;
						case THOST_FTDC_OST_NoTradeQueueing:		//下单后，无法立即成交，处于委托状态
						case THOST_FTDC_OST_PartTradedQueueing:		//下单后，部分成交，处于委托状态
							tCondOrder.nRegState = conReqEntrustOk;
							tCondOrder.nState = conCondConfirm;
							tCondOrder.UpdateSeq++;
							PostEvent_ConditionStatus(tCondOrder.FTID);
							if(tCondOrder.nPLDetailNum>0) {
								PostEvent_PLDetailStatus(tCondOrder.nPLDetailNum, 0);
							}
							break;
						case THOST_FTDC_OST_AllTraded:      //下单后，立即成交
							tCondOrder.nRegState = conReqDealClose;
							tCondOrder.nState = conCondCompleted;
							tCondOrder.UpdateSeq++;
							PostEvent_ConditionStatus(tCondOrder.FTID);
							if(tCondOrder.nPLDetailNum>0) {
								PostEvent_PLDetailStatus(tCondOrder.nPLDetailNum, 0);
							}
							if(tCondOrder.nCondPLRelation == conCPRCondFromPL) {
								mapChangePLDetail.insert(
										std::pair<long, long>(tCondOrder.nPLDetailNum, tCondOrder.FTID));
							}
							break;
						default:
							break;
						};
					}
				}

				for(itPLDetail=m_mapPLDetailOrder.begin(); 
						itPLDetail!=m_mapPLDetailOrder.end(); itPLDetail++) {
					PLDETAILORDER& tPLDOrder = itPLDetail->second;
					//if(!(tPLDOrder.nState==conPLDetailActive
					//		|| tPLDOrder.nState==conPLDetailConfirm)) continue;
					if(tPLDOrder.bGainIsServ 
							&& tPLDOrder.refGain.RequestID == RawData.OrderField.RequestID
							&& tPLDOrder.refGain.OrderStatus != THOST_FTDC_OST_Touched) {
						// 复制关键引用信息
						CopyOrderRef(tPLDOrder.refGain, RawData.OrderField);
						switch(RawData.OrderField.OrderStatus) {
						case THOST_FTDC_OST_Touched:
							mapChangePLDetail.insert(
									std::pair<long, long>(tPLDOrder.FTID, tPLDOrder.refGain.FTID));
							break;
						case THOST_FTDC_OST_NotTouched:
							if(RawData.OrderField.OrderSubmitStatus==THOST_FTDC_OSS_InsertSubmitted)
							{
								tPLDOrder.nGainRegState = conReqEntrustOk;
								tPLDOrder.UpdateSeq++;
							}
							break;
						case THOST_FTDC_OST_Canceled:
							tPLDOrder.nGainRegState = conReqDealFail;
							tPLDOrder.UpdateSeq++;
							mapChangePLDetail.insert(
									std::pair<long, long>(tPLDOrder.FTID, tPLDOrder.refGain.FTID));
							break;
						default:
							break;
						};
						PostEvent_PLDetailStatus(tPLDOrder.FTID, -1);
					}
					if(tPLDOrder.bLoseIsServ 
							&& tPLDOrder.refLose.RequestID == RawData.OrderField.RequestID
							&& tPLDOrder.refLose.OrderStatus != THOST_FTDC_OST_Touched) {
						// 复制关键引用信息
						CopyOrderRef(tPLDOrder.refLose, RawData.OrderField);
						switch(RawData.OrderField.OrderStatus) {
						case THOST_FTDC_OST_Touched:
							mapChangePLDetail.insert(
									std::pair<long, long>(tPLDOrder.FTID, tPLDOrder.refLose.FTID));
							break;
						case THOST_FTDC_OST_NotTouched:
							if(RawData.OrderField.OrderSubmitStatus==THOST_FTDC_OSS_InsertSubmitted)
							{
								tPLDOrder.nLoseRegState = conReqEntrustOk;
								tPLDOrder.UpdateSeq++;
							}
							break;
						case THOST_FTDC_OST_Canceled:
							tPLDOrder.nLoseRegState = conReqDealFail;
							tPLDOrder.UpdateSeq++;
							mapChangePLDetail.insert(
									std::pair<long, long>(tPLDOrder.FTID, tPLDOrder.refLose.FTID));
						default:
							break;
						};
						PostEvent_PLDetailStatus(tPLDOrder.FTID, -1);
					}
				}

				// 处理某个止盈止损条件单全部成交的情况
				// 部分成交则不知道如何处理
				std::map<long, long>::iterator it = mapChangePLDetail.begin();
				
				for(; it!=mapChangePLDetail.end(); it++) {
					itPLDetail = m_mapPLDetailOrder.find(it->first);
					if(itPLDetail!=m_mapPLDetailOrder.end())
					{
						PLDETAILORDER& tPLDetailOrder = itPLDetail->second;
						//tPLDetailOrder.nState = conPLDetailConfirm;
						//m_mapPLDetailOrder[tPLDetailOrder.FTID]=tPLDetailOrder;
						OrderPLDetailDelete(tPLDetailOrder.FTID, 
								conPLDetailCompleted, TRUE);
						PostEvent_PLStatus(tPLDetailOrder.FTID);
					}
				}
				mapChangePLDetail.clear();

				for(itPL=m_mapPLOrder.begin(); itPL!=m_mapPLOrder.end(); itPL++) {
					PLORDER& tPLOrder = itPL->second;
					if(!(tPLOrder.nState==conPLActive
							|| tPLOrder.nState==conPLNoTrade
							|| tPLOrder.nState==conPLPartTraded)) continue;
					if(tPLOrder.ref.RequestID == RawData.OrderField.RequestID) {
						// 复制关键引用信息
						CopyOrderRef(tPLOrder.ref, RawData.OrderField);
						switch(RawData.OrderField.OrderStatus) {
						case THOST_FTDC_OST_Canceled://下单后，CTP自动撤单
							if(tPLOrder.nState==conPLNoTrade)
								tPLOrder.nState = conPLCancelNoTrade;
							else if(tPLOrder.nState==conPLPartTraded)
								tPLOrder.nState = conPLCancelPartTraded;
							else
								tPLOrder.nState = conPLFail;
							tPLOrder.nRegState = conReqEntrustFail;
							tPLOrder.UpdateSeq++;
							OrderPLDelete(itPL->first, tPLOrder.nState, TRUE);
							//PostEvent_PLStatus(tPosOrder.FTID);
							break;
						case THOST_FTDC_OST_NoTradeQueueing://下单后，无法立即成交，处于委托状态
							//if(tPLOrder.nState==conPLActive)
								tPLOrder.nState=conPLNoTrade;
							tPLOrder.nRegState = conReqEntrustOk;
							tPLOrder.UpdateSeq++;
							PostEvent_PLStatus(tPLOrder.FTID);
							break;
						case THOST_FTDC_OST_PartTradedQueueing://下单后，部分成交，处于委托状态
							//if(tPLOrder.nState==conPLActive)
								tPLOrder.nState=conPLPartTraded;
							tPLOrder.nRegState = conReqEntrustOk;
							tPLOrder.UpdateSeq++;
							PostEvent_PLStatus(tPLOrder.FTID);
							break;
						case THOST_FTDC_OST_AllTraded:      //下单后，立即成交
							//if(tPLOrder.nState==conPLActive)
								tPLOrder.nState=conPLAllTraded;
							tPLOrder.nRegState = conReqDealClose;
							tPLOrder.UpdateSeq++;
							PostEvent_PLStatus(tPLOrder.FTID);
							break;
						default:
							break;
						};
					}
				}
			}
			break;
		case BID_RspOrderInsert:
		case BID_ErrRtnOrderInsert:
			{
				DataRspOrderInsert& RawData= *(DataRspOrderInsert*)(pAbstractBusinessData);
				MAPCONDITIONORDERITEM itCond;
				MAPPLDETAILORDERITEM itPLDetail;
				MAPPLORDERITEM itPL;
				for(itCond=m_mapConditionOrder.begin(); 
						itCond!=m_mapConditionOrder.end(); itCond++) {
					CONDITIONORDER& tCondOrder = itCond->second;
					if(tCondOrder.nState!=conCondConfirm) continue;
					if(tCondOrder.ref.RequestID == RawData.InputOrderField.RequestID) {
						tCondOrder.nRegState = conReqEntrustFail;
						tCondOrder.nState = conCondCancel;
						tCondOrder.UpdateSeq++;
						//m_mapConditionOrder[itCond->first]=tCondOrder;
						PostEvent_ConditionStatus(tCondOrder.FTID);
					}
				}
				BOOL bHasFound = FALSE;
				for(itPLDetail=m_mapPLDetailOrder.begin(); 
						itPLDetail!=m_mapPLDetailOrder.end(); itPLDetail++) {
					PLDETAILORDER& tPLDetailOrder = itPLDetail->second;
					//if(!tPLDetailOrder.bIsRunInServer) continue;
					//if(tPLDetailOrder.nState!=conPLDetailConfirm) continue;
					if(tPLDetailOrder.nState!=conPLDetailActive) continue;
					if(tPLDetailOrder.refGain.RequestID == RawData.InputOrderField.RequestID) {
						bHasFound = TRUE;
						//tPLDetailOrder.nState = conPLCancel;
						tPLDetailOrder.nGainRegState = conReqEntrustFail;
						tPLDetailOrder.UpdateSeq++;
						if(tPLDetailOrder.bGainIsServ)
						{
							if(pAbstractBusinessData->BID==BID_RspOrderInsert)
							{
							if(RawData.RspInfoField.ErrorID==53 
									&& RawData.InputOrderField.ContingentCondition>=
									THOST_FTDC_CC_LastPriceGreaterThanStopPrice)
							{
								PostEvent_CondServFail(&RawData.InputOrderField, tPLDetailOrder.FTID, 1);
								//ConvertServ2Local_PLDetail(tPLDetailOrder.FTID, FALSE);
							}
							}
						}
						else
							PostEvent_ConditionStatus(tPLDetailOrder.refGain.FTID);
					}
					if(tPLDetailOrder.refLose.RequestID == RawData.InputOrderField.RequestID) {
						bHasFound = TRUE;
						//tPLDetailOrder.nState = conPLCancel;
						tPLDetailOrder.nLoseRegState = conReqEntrustFail;
						tPLDetailOrder.UpdateSeq++;
						if(tPLDetailOrder.bLoseIsServ)
						{
							if(pAbstractBusinessData->BID==BID_RspOrderInsert)
							{
							if(RawData.RspInfoField.ErrorID==53 
									&& RawData.InputOrderField.ContingentCondition>=
									THOST_FTDC_CC_LastPriceGreaterThanStopPrice)
							{
								PostEvent_CondServFail(&RawData.InputOrderField, tPLDetailOrder.FTID, 2);
								//ConvertServ2Local_PLDetail(tPLDetailOrder.FTID, TRUE);
							}
							}
						}
						else
							PostEvent_ConditionStatus(tPLDetailOrder.refLose.FTID);
					}
				}
				if(pAbstractBusinessData->BID==BID_RspOrderInsert)
				{
				if(bHasFound==FALSE 
						&& RawData.RspInfoField.ErrorID==53 
						&& RawData.InputOrderField.ContingentCondition>=
						THOST_FTDC_CC_LastPriceGreaterThanStopPrice)
				{
						PostEvent_CondServFail(&RawData.InputOrderField, 0, 0);
						//ConvertServ2Local_PLDetail(tPLDetailOrder.FTID, FALSE);
				}
				}
				for(itPL=m_mapPLOrder.begin(); itPL!=m_mapPLOrder.end(); itPL++) {
					PLORDER& tPLOrder = itPL->second;
					if(tPLOrder.nState!=conPLActive) continue;
					if(tPLOrder.ref.RequestID == RawData.InputOrderField.RequestID) {
						tPLOrder.nRegState = conReqEntrustFail;
						tPLOrder.nState = conPLFail;
						tPLOrder.UpdateSeq++;
						//m_mapPositionOrder[itPL->first]=tPosOrder;
						//OrderPositionDelete(itPL->first, conPLCancel);
						PostEvent_PLStatus(tPLOrder.FTID);
					}
				}
			}
			break;
		case BID_RspOrderAction:
			break;
		case BID_ErrRtnOrderAction:
			break;
		default:
			break;
	};

	UnlockObject();
	
	wxDELETE(pAbstractBusinessData)
	evt.SetClientData(NULL);
}

BOOL CLocalOrderService::AddPosiPLAndRun(PLDETAILORDER& tPLDetail)
{
	tPLDetail.nState = conPLDetailActive;
	OrderPLDetailAdd(tPLDetail);

	CONDITIONORDER tCondGain, tCondLose;
	ZeroMemory(&tCondGain, sizeof(tCondGain));
	ZeroMemory(&tCondLose, sizeof(tCondLose));

	//MakeLocalCondOrder_PLDetail(tPLOrder, tPLDetail, tCondGain, tCondLose);
	MakeLocalCondOrder_PLDetail(tPLDetail, tCondGain, tCondLose);
	if(tPLDetail.plparam.bDoStopGain)
	{
		if(tPLDetail.bGainIsServ) {
		//if(tPLDetail.bIsRunInServer) {
			OrderCondition(tCondGain, TRUE);
			tPLDetail.refGain.RequestID = tCondGain.ref.RequestID;
		}
		else 
		{
			OrderConditionAdd(tCondGain);
			tPLDetail.refGain.FTID = tCondGain.FTID;
		}
	}
	if(tPLDetail.plparam.bDoStopLose)
	{
		if(tPLDetail.bLoseIsServ) {
		//if(tPLDetail.bIsRunInServer) {
			OrderCondition(tCondLose, TRUE);
			tPLDetail.refLose.RequestID = tCondLose.ref.RequestID;
		}
		else
		{
			OrderConditionAdd(tCondLose);
			tPLDetail.refLose.FTID = tCondLose.FTID;
		}
	}
	OrderPLDetailSet(tPLDetail.FTID, tPLDetail);
	return TRUE;
}

BOOL CLocalOrderService::ConvertServ2Local_PLDetail(long nNum, BOOL bIsLose)
{
	MAPPLDETAILORDERITEM it = m_mapPLDetailOrder.find(nNum);
	if(it==m_mapPLDetailOrder.end())
		return FALSE;
	
	PLDETAILORDER& tPLDetail = it->second;

	if(tPLDetail.nState > conPLDetailActive)
		return FALSE;

	CONDITIONORDER tCondGain, tCondLose;
	ZeroMemory(&tCondGain, sizeof(tCondGain));
	ZeroMemory(&tCondLose, sizeof(tCondLose));

	MakeLocalCondOrder_PLDetail(tPLDetail, tCondGain, tCondLose);
	if(tPLDetail.bGainIsServ && !bIsLose)
	{
		tPLDetail.bGainIsServ = FALSE;
		OrderConditionAdd(tCondGain);
		tPLDetail.refGain.FTID = tCondGain.FTID;
		return TRUE;
	}
	else if(tPLDetail.bLoseIsServ && bIsLose)
	{
		tPLDetail.bLoseIsServ = FALSE;
		OrderConditionAdd(tCondLose);
		tPLDetail.refLose.FTID = tCondLose.FTID;
		return TRUE;
	}
	else
		return FALSE;
}

void CLocalOrderService::OnRcvTrade(wxCommandEvent& evt)
{
//	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
	DataRtnTrade* pRawData= (DataRtnTrade*)(evt.GetClientData());
	if(!pRawData) {
		wxASSERT(pRawData);
		return;
	}


	MAPCONDITIONORDERITEM itCond;
	MAPPLORDERITEM itPL;

	LockObject();

	if(pRawData->TradeField.OffsetFlag!=THOST_FTDC_OF_Open) {
		PositionKey key(pRawData->TradeField.Account, pRawData->TradeField.InstrumentID, 
				(pRawData->TradeField.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long), 
				pRawData->TradeField.HedgeFlag);
		PlatformStru_Position posiInfo;
		if(DEFAULT_SVR()->GetPositionInfo(key, posiInfo)!=0) {
			strcpy(posiInfo.InvestorID, pRawData->TradeField.InvestorID);
			strcpy(posiInfo.Account, pRawData->TradeField.Account);
			strcpy(posiInfo.InstrumentID, pRawData->TradeField.InstrumentID);
			posiInfo.PosiDirection = (pRawData->TradeField.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Short:THOST_FTDC_PD_Long);
			posiInfo.HedgeFlag = pRawData->TradeField.HedgeFlag;
			posiInfo.Position = 0;
			posiInfo.TodayPosition = 0;
			posiInfo.YdPosition = 0;
			posiInfo.CanCloseVolume = 0;
			posiInfo.CanCloseTodayVolume = 0;
			posiInfo.CanCloseydVolume = 0;
		}
		if(AsyncVolumePosiAndPL(posiInfo)) {
			PostEvent_PLDetailStatus(-1, -1);
			PostEvent_ConditionStatus(-1);
		}
	}
	else {
		if(HasOwnerTrade(pRawData->TradeField)) {
			for(itPL=m_mapPLOrder.begin(); itPL!=m_mapPLOrder.end(); itPL++) {
				PLORDER& tPLOrder = itPL->second;
				if(!(tPLOrder.nState==conPLNoTrade 
						|| tPLOrder.nState==conPLPartTraded
						|| tPLOrder.nState==conPLAllTraded)) continue;
				if(strcmp(tPLOrder.ref.OrderSysID, pRawData->TradeField.OrderSysID)==0 && 
						strcmp(tPLOrder.ref.ExchangeID, pRawData->TradeField.ExchangeID)==0) {
					// 这里需要生成盈损单明细
					MakePLDetailAndRun(tPLOrder, pRawData->TradeField);
				}
			}
		}
	}

	UnlockObject();

	wxDELETE(pRawData)
	evt.SetClientData(NULL);
}

void CLocalOrderService::OnPlatSvrStatusChange(wxCommandEvent& evt)
{

	DataInstrumentStatus* pStatus = (DataInstrumentStatus*)evt.GetClientData();
	if(pStatus == NULL)
		return;

	wxString strExchange = pStatus->InstrumentStatusField.ExchangeID;
	wxString strOrderExchange;

	char cOldStatus = 0;
	std::map<string, char>::iterator itStatus;

	LockObject();

	itStatus = m_mapExchangeStatus.find(strExchange.c_str());
	if(itStatus != m_mapExchangeStatus.end()) {
		if(itStatus->second == pStatus->InstrumentStatusField.InstrumentStatus) {
			UnlockObject();
			SAFE_DELETE(pStatus);
			return;
		}
	}
	string strExch = strExchange.c_str();
	m_mapExchangeStatus[strExch] = pStatus->InstrumentStatusField.InstrumentStatus;

	UnlockObject();


	if(!(pStatus->InstrumentStatusField.InstrumentStatus == THOST_FTDC_IS_Continous ||
			pStatus->InstrumentStatusField.InstrumentStatus == THOST_FTDC_IS_AuctionOrdering)) {
		SAFE_DELETE(pStatus);
		return;
	}

	MAPPARKEDORDERITEM it;
	PARKEDORDER tParkedOrder={0};

	LockObject();

	for(it=m_mapParkedOrder.begin(); it!=m_mapParkedOrder.end(); it++) {
		tParkedOrder = it->second;
		if(tParkedOrder.bIsAuto && tParkedOrder.nState == conParkedActive) {
			strOrderExchange = tParkedOrder.ExchangeID;
			if(strOrderExchange.compare(strExchange)==0) {
				OrderParked(tParkedOrder);
				m_mapParkedOrder[it->first] = tParkedOrder;
			}
		}
	}

	UnlockObject();
    SAFE_DELETE(pStatus);
	//SaveDataToFile();

}

BOOL CLocalOrderService::LoadDataFromFile()
{
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";

	FILEHEAD tFileHead={0};

	wxString strFileName;
	
	strFileName = strAppPath;
	strFileName += wxT(m_strInvestorID.c_str());
	strFileName += wxT(".Dat");
	wxFileName filename(strFileName); 
	if(!filename.FileExists())
		return FALSE;

	wxDateTime dtCurr;
	dtCurr = wxDateTime::Today();
	wxDateTime dtFileModify = filename.GetModificationTime();
	
	DWORD dwCurrDate = dtCurr.GetYear()*10000+dtCurr.GetMonth()*100+dtCurr.GetDay();
	DWORD dwFileModifyDate = dtFileModify.GetYear()*10000+dtFileModify.GetMonth()*100+dtFileModify.GetDay();

	wxFile file;
	DWORD dwPos=0;

	// 如果是隔夜的条件单等数据，则清空文件
	if(dwCurrDate-dwFileModifyDate) {
//		file.Create(strFileName, true);
//		file.Close();
		return TRUE;
	}

	file.Open(strFileName);
	if(!file.IsOpened())
		return FALSE;

	file.Read(&tFileHead, sizeof(FILEHEAD));
	dwPos+=sizeof(FILEHEAD);

	tFileHead.dwDate;
	tFileHead.dwTime;
	if(DEFAULT_SVR()) DEFAULT_SVR()->SetReqID((int)tFileHead.dwOrderReqNum);
	sm_nBatchOrderNum = tFileHead.dwBatchGroupNum;
	sm_nBatchSubOrderNum = tFileHead.dwBatchOrderNum;
	sm_nParkedOrderNum = tFileHead.dwParkedOrderNum;
	sm_nConditionOrderNum = tFileHead.dwCondOrderNum;
	sm_nPLOrderNum = tFileHead.dwPLOrderNum;
	sm_nPLDetailOrderNum = tFileHead.dwPLDetailOrderNum;


	PARKEDORDER tParkedOrder={0};

	dwPos = tFileHead.dwParkedOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwParkedCount; i++) {
		file.Read(&tParkedOrder, sizeof(PARKEDORDER));
		m_mapParkedOrder[tParkedOrder.nNum] = tParkedOrder;
	}

	CONDITIONORDER tCondOrder={0};

	dwPos = tFileHead.dwCondActiveOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwCondActiveCount; i++) {
		file.Read(&tCondOrder, sizeof(CONDITIONORDER));
		m_mapConditionOrder[tCondOrder.FTID] = tCondOrder;
	}

	PLORDER tPLOrder={0};

	dwPos = tFileHead.dwPLOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwPLCount; i++) {
		file.Read(&tPLOrder, sizeof(PLORDER));
		m_mapPLOrder[tPLOrder.FTID] = tPLOrder;
	}

	PLDETAILORDER tPLDetailOrder={0};

	dwPos = tFileHead.dwPLDetailOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwPLDetailCount; i++) {
		file.Read(&tPLDetailOrder, sizeof(PLDETAILORDER));
		m_mapPLDetailOrder[tPLDetailOrder.FTID] = tPLDetailOrder;
	}

	BATCHSUBORDER tBatchSub={0};
	
	dwPos = tFileHead.dwBatchSubOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwBatchSubCount; i++) {
		file.Read(&tBatchSub, sizeof(BATCHSUBORDER));
		m_mapBatchSubOrder[tBatchSub.nNum] = tBatchSub;
	}

	file.Close();

	return TRUE;
}

BOOL CLocalOrderService::SaveDataToFile()
{
    // 创建目录
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";
    if(!wxDirExists(strAppPath.c_str())) 
		wxMkdir(strAppPath);

	FILEHEAD tFileHead={0};

	wxFile file;
	DWORD dwPos=0;

    wxString strFileName;

	
	strFileName = strAppPath;
	strFileName += wxT(m_strInvestorID.c_str());
	strFileName += wxT(".Dat");
	file.Create(strFileName, true);
	if(!file.IsOpened()) {
		wxMessageDialog dialog( NULL,LOADSTRING(SaveFail_DataFile_MESSAGE) ,LOADSTRING(SaveFail_Title),wxOK|wxICON_ERROR);
		dialog.ShowModal();
		return FALSE;
	}

	if(!(m_mapParkedOrder.size()>0 || m_mapConditionOrder.size()>0 
			|| m_mapPLOrder.size()>0 || m_mapPLDetailOrder.size() || m_mapBatchSubOrder.size()>0)) {
		file.Close();		
		return FALSE;
	}

	tFileHead.dwDate;
	tFileHead.dwTime;
    tFileHead.dwOrderReqNum = DEFAULT_SVR()?DEFAULT_SVR()->GetReqID():0;//sm_nReqNum;
	tFileHead.dwBatchGroupNum = sm_nBatchOrderNum;
	tFileHead.dwBatchOrderNum = sm_nBatchSubOrderNum;
	tFileHead.dwParkedOrderNum = sm_nParkedOrderNum;
	tFileHead.dwCondOrderNum = sm_nConditionOrderNum;
	tFileHead.dwPLOrderNum = sm_nPLOrderNum;
	tFileHead.dwPLDetailOrderNum = sm_nPLDetailOrderNum;

	file.Write(&tFileHead, sizeof(FILEHEAD));
	dwPos+=sizeof(FILEHEAD);

	LockObject();

	tFileHead.dwParkedOffset = dwPos;
	for(MAPPARKEDORDERITEM itParked=m_mapParkedOrder.begin(); 
			itParked!=m_mapParkedOrder.end(); ++itParked) {
		file.Write(&(itParked->second), sizeof(PARKEDORDER));
		tFileHead.dwParkedCount++;
		tFileHead.dwParkedSize+=sizeof(PARKEDORDER);
		dwPos+=sizeof(PARKEDORDER);
	}

	tFileHead.dwCondActiveOffset = dwPos;
	for(MAPCONDITIONORDERITEM itCond=m_mapConditionOrder.begin(); 
			itCond!=m_mapConditionOrder.end(); ++itCond) {
		file.Write(&(itCond->second), sizeof(CONDITIONORDER));
		tFileHead.dwCondActiveCount++;
		tFileHead.dwCondActiveSize+=sizeof(CONDITIONORDER);
		dwPos+=sizeof(CONDITIONORDER);
	}

	tFileHead.dwPLOffset = dwPos;
	for(MAPPLORDERITEM itPos=m_mapPLOrder.begin(); 
			itPos!=m_mapPLOrder.end(); ++itPos) {
		file.Write(&(itPos->second), sizeof(PLORDER));
		tFileHead.dwPLCount++;
		tFileHead.dwPLSize+=sizeof(PLORDER);
		dwPos+=sizeof(PLORDER);
	}

	tFileHead.dwPLDetailOffset = dwPos;
	for(MAPPLDETAILORDERITEM itPos=m_mapPLDetailOrder.begin(); 
			itPos!=m_mapPLDetailOrder.end(); ++itPos) {
		file.Write(&(itPos->second), sizeof(PLDETAILORDER));
		tFileHead.dwPLDetailCount++;
		tFileHead.dwPLDetailSize+=sizeof(PLDETAILORDER);
		dwPos+=sizeof(PLDETAILORDER);
	}
	
	tFileHead.dwBatchSubOffset = dwPos;
	for(MAPBATCHSUBORDERITEM itSub=m_mapBatchSubOrder.begin(); 
			itSub!=m_mapBatchSubOrder.end(); ++itSub) {
		file.Write(&(itSub->second), sizeof(BATCHSUBORDER));
		tFileHead.dwBatchSubCount++;
		tFileHead.dwBatchSubSize+=sizeof(BATCHSUBORDER);
		dwPos+=sizeof(BATCHSUBORDER);
	}

	UnlockObject();

	file.Seek(0);
	file.Write(&tFileHead, sizeof(FILEHEAD));

	file.Close();

	return TRUE;
}

void CLocalOrderService::PostEvent_ParkedStatus(long nNum)
{
	std::set<void*>::iterator it;
	for(it = m_setMsgWindow.begin(); it != m_setMsgWindow.end(); it++) {
		wxCommandEvent evt(wxEVT_LOCALPARKED_STATUSCHANGED);
		if(*it) {
			evt.SetInt(nNum);
			((wxEvtHandler*)*it)->AddPendingEvent(evt);
		}
	}
}

void CLocalOrderService::PostEvent_ConditionStatus(long nNum)
{
	std::set<void*>::iterator it;
	for(it = m_setMsgWindow.begin(); it != m_setMsgWindow.end(); it++) {
		wxCommandEvent evt(wxEVT_LOCALCONDITION_STATUSCHANGED);
		if(*it) {
			evt.SetInt(nNum);
			((wxEvtHandler*)*it)->AddPendingEvent(evt);
		}
	}
}

// nMode, 0 条件单 pInputOrder有效，1 止盈, 2 止损 nNum有效 是PLDetail的编号
void CLocalOrderService::PostEvent_CondServFail(CThostFtdcInputOrderField* pInputOrder, 
												long nNum, int nMode)
{
	std::set<void*>::iterator it;
	for(it = m_setMsgWindow.begin(); it != m_setMsgWindow.end(); it++) {
		wxCommandEvent evt(wxEVT_SERVCOND_OVERBOUNDFAIL);
		if(*it) {
			if(nMode==0)
			{
				if(((wxWindow*)*it)->GetId()==GID_QUERY_ORDER_CONDITION)
				{
					CThostFtdcInputOrderField* pNewIOrder = new CThostFtdcInputOrderField;
					*pNewIOrder = *pInputOrder;
					evt.SetClientData(pNewIOrder);
					evt.SetInt(0);
					evt.SetExtraLong(0);
					((wxEvtHandler*)*it)->AddPendingEvent(evt);
				}
			}
			else
			{
				evt.SetString(wxString(pInputOrder->InstrumentID));
				evt.SetInt(nNum);
				evt.SetExtraLong(nMode);
				((wxEvtHandler*)*it)->AddPendingEvent(evt);
			}
		}
	}
}

void CLocalOrderService::PostEvent_PLStatus(long nNum)
{
	std::set<void*>::iterator it;
	for(it = m_setMsgWindow.begin(); it != m_setMsgWindow.end(); it++) {
		wxCommandEvent evt(wxEVT_LOCALPOSITION_STATUSCHANGED);
		if(*it) {
			evt.SetInt(nNum);
			((wxEvtHandler*)*it)->AddPendingEvent(evt);
		}
	}
}

void CLocalOrderService::PostEvent_PLDetailStatus(long nNum, long nLPNum)
{
	std::set<void*>::iterator it;
	for(it = m_setMsgWindow.begin(); it != m_setMsgWindow.end(); it++) {
		wxCommandEvent evt(wxEVT_LOCALPLDETAIL_STATUSCHANGED);
		if(*it) {
			evt.SetInt(nNum);
			evt.SetExtraLong(nLPNum);
			((wxEvtHandler*)*it)->AddPendingEvent(evt);
		}
	}
}

BOOL CLocalOrderService::HasNoTouchLocalOrder()
{
	for(MAPPLORDERITEM itPL=m_mapPLOrder.begin(); 
			itPL!=m_mapPLOrder.end(); ++itPL) 
	{
		if(itPL->second.nState==conPLActive || 
				itPL->second.nState==conPLNoTrade || 
				itPL->second.nState==conPLPartTraded)
		{
			return TRUE;
		}
	}

	for(MAPPARKEDORDERITEM itParked=m_mapParkedOrder.begin(); 
			itParked!=m_mapParkedOrder.end(); ++itParked) 
	{
		if(itParked->second.nState<=conParkedActive)
		{
			return TRUE;
		}
	}

	for(MAPCONDITIONORDERITEM itCond=m_mapConditionOrder.begin(); 
			itCond!=m_mapConditionOrder.end(); ++itCond) {
		if(itCond->second.nState<=conCondActive)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CLocalOrderService::CheckLocalOrder()
{

	if(DEFAULT_SVR()==NULL)
		return;

	MAPCONDITIONORDERITEM itCond;
	MAPPLDETAILORDERITEM itPLDetail;
	MAPPLORDERITEM itPL;

	//LockObject();
	std::set<long> setDel;
	std::set<long>::iterator itDel;

	for(itPL=m_mapPLOrder.begin(); itPL!=m_mapPLOrder.end(); itPL++) {
		PLORDER& tPLOrder = itPL->second;
		//if(!(tPLOrder.nState==conPLActive
		//		|| tPLOrder.nState==conPLNoTrade
		//		|| tPLOrder.nState==conPLPartTraded)) continue;
		OrderKey orderKey(tPLOrder.ref.Account, tPLOrder.InstrumentID, 
				tPLOrder.ref.FrontID, tPLOrder.ref.SessionID, tPLOrder.ref.OrderRef);
		PlatformStru_OrderInfo orderData;
		if(!DEFAULT_SVR()->GetOrder(orderKey, orderData)) {
			setDel.insert(itPL->first);
		}
		else {
			if(tPLOrder.ref.RequestID == orderData.RequestID) {
				// 复制关键引用信息
				CopyOrderRef(tPLOrder.ref, orderData);
				switch(orderData.OrderStatus) {
				case THOST_FTDC_OST_Canceled://下单后，CTP自动撤单
					if(tPLOrder.nState==conPLNoTrade)
						tPLOrder.nState = conPLCancelNoTrade;
					else if(tPLOrder.nState==conPLPartTraded)
						tPLOrder.nState = conPLCancelPartTraded;
					else
						tPLOrder.nState = conPLFail;
					tPLOrder.nRegState = conReqEntrustFail;
					tPLOrder.UpdateSeq++;
					OrderPLDelete(itPL->first, tPLOrder.nState, TRUE);
					//PostEvent_PLStatus(tPosOrder.FTID);
					break;
				case THOST_FTDC_OST_NoTradeQueueing://下单后，无法立即成交，处于委托状态
					//if(tPLOrder.nState==conPLActive)
						tPLOrder.nState=conPLNoTrade;
					tPLOrder.nRegState = conReqEntrustOk;
					tPLOrder.UpdateSeq++;
					PostEvent_PLStatus(tPLOrder.FTID);
					break;
				case THOST_FTDC_OST_PartTradedQueueing://下单后，部分成交，处于委托状态
					//if(tPLOrder.nState==conPLActive)
						tPLOrder.nState=conPLPartTraded;
					tPLOrder.nRegState = conReqEntrustOk;
					tPLOrder.UpdateSeq++;
					PostEvent_PLStatus(tPLOrder.FTID);
					break;
				case THOST_FTDC_OST_AllTraded:      //下单后，立即成交
					//if(tPLOrder.nState==conPLActive)
						tPLOrder.nState=conPLAllTraded;
					tPLOrder.nRegState = conReqDealClose;
					tPLOrder.UpdateSeq++;
					PostEvent_PLStatus(tPLOrder.FTID);
					break;
				default:
					break;
				};
			}
		}
	}
	for(itDel = setDel.begin(); itDel != setDel.end(); itDel++) {
		m_mapPLOrder.erase(*itDel);
	}
	setDel.clear();

	std::map<long, long> mapChangePLDetail;

	for(itCond=m_mapConditionOrder.begin(); 
			itCond!=m_mapConditionOrder.end(); itCond++) {
		CONDITIONORDER& tCondOrder = itCond->second;
		//if(tCondOrder.nState==conCondPause 
		//		|| tCondOrder.nState==conCondDelete 
		//		|| tCondOrder.nState==conCondCompleted 
		//		|| tCondOrder.nState==conCondCancel) continue;
		OrderKey orderKey(tCondOrder.ref.Account, tCondOrder.InstrumentID, 
				tCondOrder.ref.FrontID, tCondOrder.ref.SessionID, tCondOrder.ref.OrderRef);
		PlatformStru_OrderInfo orderData;
		if(!DEFAULT_SVR()->GetOrder(orderKey, orderData)) { 
			if(tCondOrder.nState<=conCondDelete)
				continue;
			else
				setDel.insert(tCondOrder.FTID);
		}
		if(tCondOrder.ref.RequestID == orderData.RequestID) {
			// 复制关键引用信息
			CopyOrderRef(tCondOrder.ref, orderData);
			switch(orderData.OrderStatus) {
			case THOST_FTDC_OST_Canceled://下单后，CTP自动撤单
				tCondOrder.nRegState = conReqEntrustFail;
				tCondOrder.nState = conCondCancel;
				tCondOrder.UpdateSeq++;
				PostEvent_ConditionStatus(tCondOrder.FTID);
				break;
			case THOST_FTDC_OST_NoTradeQueueing:		//下单后，无法立即成交，处于委托状态
			case THOST_FTDC_OST_PartTradedQueueing:		//下单后，部分成交，处于委托状态
				tCondOrder.nRegState = conReqEntrustOk;
				tCondOrder.nState = conCondConfirm;
				tCondOrder.UpdateSeq++;
				PostEvent_ConditionStatus(tCondOrder.FTID);
				break;
			case THOST_FTDC_OST_AllTraded:      //下单后，立即成交
				tCondOrder.nRegState = conReqDealClose;
				tCondOrder.nState = conCondCompleted;
				tCondOrder.UpdateSeq++;
				PostEvent_ConditionStatus(tCondOrder.FTID);
				if(tCondOrder.nCondPLRelation == conCPRCondFromPL) {
					mapChangePLDetail.insert(
							std::pair<long, long>(tCondOrder.nPLDetailNum, tCondOrder.FTID));
				}
				break;
			default:
				break;
			};
		}
	}
	for(itDel = setDel.begin(); itDel != setDel.end(); itDel++) {
		m_mapConditionOrder.erase(*itDel);
	}
	setDel.clear();

	for(itPLDetail=m_mapPLDetailOrder.begin(); 
			itPLDetail!=m_mapPLDetailOrder.end(); itPLDetail++) {
		PLDETAILORDER& tPLDOrder = itPLDetail->second;
		//if(!(tPLDOrder.nState==conPLDetailActive
		//		|| tPLDOrder.nState==conPLDetailConfirm)) continue;
		if(tPLDOrder.bGainIsServ) {
			OrderKey orderKey(tPLDOrder.refGain.Account, tPLDOrder.InstrumentID, 
					tPLDOrder.refGain.FrontID, tPLDOrder.refGain.SessionID, tPLDOrder.refGain.OrderRef);
			PlatformStru_OrderInfo orderData;
			if(!DEFAULT_SVR()->GetOrder(orderKey, orderData)) {
				setDel.insert(itPLDetail->first);
			}
			else {
				if(tPLDOrder.refGain.RequestID == orderData.RequestID) {
					// 复制关键引用信息
					CopyOrderRef(tPLDOrder.refGain, orderData);
					switch(orderData.OrderStatus) {
					case THOST_FTDC_OST_Touched:
						mapChangePLDetail.insert(
								std::pair<long, long>(tPLDOrder.FTID, tPLDOrder.refGain.FTID));
						break;
					case THOST_FTDC_OST_NotTouched:
						if(orderData.OrderSubmitStatus==THOST_FTDC_OSS_InsertSubmitted)
						{
							tPLDOrder.nGainRegState = conReqEntrustOk;
							tPLDOrder.UpdateSeq++;
						}
						break;
					case THOST_FTDC_OST_Canceled:
						tPLDOrder.nGainRegState = conReqDealFail;
						tPLDOrder.UpdateSeq++;
						mapChangePLDetail.insert(
								std::pair<long, long>(tPLDOrder.FTID, tPLDOrder.refGain.FTID));
						break;
					default:
						break;
					};
				}
			}
		}
		if(tPLDOrder.bLoseIsServ) {
			OrderKey orderKey(tPLDOrder.refLose.Account, tPLDOrder.InstrumentID, 
					tPLDOrder.refLose.FrontID, tPLDOrder.refLose.SessionID, tPLDOrder.refLose.OrderRef);
			PlatformStru_OrderInfo orderData;
			if(!DEFAULT_SVR()->GetOrder(orderKey, orderData)) {
				setDel.insert(itPLDetail->first);
			}
			else {
				if(tPLDOrder.refLose.RequestID == orderData.RequestID) {
					// 复制关键引用信息
					CopyOrderRef(tPLDOrder.refLose, orderData);
					switch(orderData.OrderStatus) {
					case THOST_FTDC_OST_Touched:
						mapChangePLDetail.insert(
								std::pair<long, long>(tPLDOrder.FTID, tPLDOrder.refLose.FTID));
						break;
					case THOST_FTDC_OST_NotTouched:
						if(orderData.OrderSubmitStatus==THOST_FTDC_OSS_InsertSubmitted)
						{
							tPLDOrder.nLoseRegState = conReqEntrustOk;
							tPLDOrder.UpdateSeq++;
						}
						break;
					case THOST_FTDC_OST_Canceled:
						tPLDOrder.nLoseRegState = conReqDealFail;
						tPLDOrder.UpdateSeq++;
						mapChangePLDetail.insert(
								std::pair<long, long>(tPLDOrder.FTID, tPLDOrder.refLose.FTID));
					default:
						break;
					};
				}
			}
		}
	}
	for(itDel = setDel.begin(); itDel != setDel.end(); itDel++) {
		m_mapPLDetailOrder.erase(*itDel);
	}
	setDel.clear();

	// 处理某个止盈止损条件单全部成交的情况
	// 部分成交则不知道如何处理
	std::map<long, long>::iterator it;
	
	for(it = mapChangePLDetail.begin(); it!=mapChangePLDetail.end(); it++) {
		itPLDetail = m_mapPLDetailOrder.find(it->first);
		if(itPLDetail!=m_mapPLDetailOrder.end())
		{
			PLDETAILORDER& tPLDetailOrder = itPLDetail->second;
			//tPLDetailOrder.nState = conPLDetailConfirm;
			//m_mapPLDetailOrder[tPLDetailOrder.FTID]=tPLDetailOrder;
			OrderPLDetailDelete(tPLDetailOrder.FTID, 
					conPLDetailCompleted, TRUE);
		}
	}
	mapChangePLDetail.clear();

	PostEvent_PLStatus(-1);
	PostEvent_ConditionStatus(-1);
	PostEvent_PLDetailStatus(-1, -1);
	//UnlockObject();

}

int CLocalOrderService::GetCondOrderVolume(BOOL bIsServ, 
						ORDERREF& condRef, string& strInstruemtID)
{
	if(DEFAULT_SVR()==NULL) 
		return 0;
	
	int nCondVol = 0;
	if(bIsServ) {
		PlatformStru_OrderInfo outData;
		OrderKey orderkey(condRef.Account, strInstruemtID.c_str(), 
					condRef.FrontID, condRef.SessionID, condRef.OrderRef);
		if(!DEFAULT_SVR()->GetOrder(orderkey, outData)) 
			return 0;
		if(outData.OrderStatus==THOST_FTDC_OST_NotTouched)
			nCondVol = outData.VolumeTotalOriginal;
		else
			nCondVol = 0;
	}
	else {
		MAPCONDITIONORDERITEM it;
		MAPCONDITIONORDER& mapCond = GetOrderConditionMap();
		it = mapCond.find(condRef.FTID);
		if(it != mapCond.end()) 
			if(it->second.nState<=conCondActive)
				nCondVol = it->second.orderSrc.nVolume;
			else
				nCondVol = 0;
		else
			nCondVol = 0;
	}

	return nCondVol;
}

int CLocalOrderService::GetPosiPLVolume(PLDETAILORDER& posiPLOrder)
{
	int nVol = 0, nGainVol = 0, nLoseVol = 0;
	nGainVol = GetCondOrderVolume(posiPLOrder.bGainIsServ, posiPLOrder.refGain, 
							std::string(posiPLOrder.orderSrc.InstrumentID));
	nLoseVol = GetCondOrderVolume(posiPLOrder.bLoseIsServ, posiPLOrder.refLose, 
							std::string(posiPLOrder.orderSrc.InstrumentID));
	nVol = max(nGainVol, nLoseVol);
	if(nVol<0)
		nVol = 0;
	return nVol;
}

int CLocalOrderService::GetPosiPLVolume(std::string& strAccount, 
										std::string& strInstrumentID, 
										BOOL bIsBuy, int nHedgeStatus, int nMode)
{
	MAPPLDETAILORDERITEM itPLDetail;
	int nPosiPLVolume = 0;

	for(itPLDetail=m_mapPLDetailOrder.begin(); 
			itPLDetail!=m_mapPLDetailOrder.end(); itPLDetail++) {
		switch(nMode) {
		case 0:
		default:
			if(//strAccount.compare(itPLDetail->second.ref.Account)==0 && 
					strInstrumentID.compare(itPLDetail->second.InstrumentID)==0
					&& bIsBuy==itPLDetail->second.orderSrc.bIsBuy
					&& nHedgeStatus==itPLDetail->second.orderSrc.nHedgeStatus)
				nPosiPLVolume += GetPosiPLVolume(itPLDetail->second);
			break;
		case 1:
			if(//strAccount.compare(itPLDetail->second.ref.Account)==0 && 
					strInstrumentID.compare(itPLDetail->second.InstrumentID)==0
					&& bIsBuy==itPLDetail->second.orderSrc.bIsBuy
					&& nHedgeStatus==itPLDetail->second.orderSrc.nHedgeStatus
					&& itPLDetail->second.orderSrc.nOCMode==1)
				nPosiPLVolume += GetPosiPLVolume(itPLDetail->second);
			break;
		case 2:
			if(//strAccount.compare(itPLDetail->second.ref.Account)==0 && 
					strInstrumentID.compare(itPLDetail->second.InstrumentID)==0
					&& bIsBuy==itPLDetail->second.orderSrc.bIsBuy
					&& nHedgeStatus==itPLDetail->second.orderSrc.nHedgeStatus
					&& itPLDetail->second.orderSrc.nOCMode==2)
				nPosiPLVolume += GetPosiPLVolume(itPLDetail->second);
			break;
		};
	}
	TRACE("GetPosiPLVolume(%s, %s, %d, %d)==%d\n", 
			strAccount.c_str(), strInstrumentID.c_str(), bIsBuy, nHedgeStatus, 
			nPosiPLVolume);
	return nPosiPLVolume;
}

BOOL CLocalOrderService::OrderPLDetailChange(int nNum, int nSubVolume)
{
	// 启用一个守护线程来出来超时的问题
	// 过程：等待服务器条件单删除成功，再进行服务器条件单下单
	// 使用一个map保存等待删除确认的服务器条件单的必要数据
	// 包括：正在进行改单的止盈止损编号、是止盈还是止损、被删除的服务器条件单编号、新下的数量
	// 使用一个map保存等待下单成功的服务器条件单的必要数据
	// 包括：正在进行改单的止盈止损编号、是止盈还是止损、新下单的服务器条件单RequestID
	// 收到服务器条件单下单成功后，填到相关止盈或止损的ref上

	MAPCONDITIONORDERITEM itCond;
	MAPPLDETAILORDERITEM it = m_mapPLDetailOrder.find(nNum);
	if(it == m_mapPLDetailOrder.end())
		return FALSE;

	if(it->second.orderSrc.nVolume<=nSubVolume)
		return FALSE;
	it->second.orderSrc.nVolume -= nSubVolume;

	WaitDelOrNew_ServCond waitDel;

	if(it->second.plparam.bDoStopGain) {
		if(!it->second.bGainIsServ) {
			itCond = m_mapConditionOrder.find(it->second.refGain.FTID);
			if(itCond != m_mapConditionOrder.end()) {
				if(itCond->second.orderSrc.nVolume <= nSubVolume)
					OrderConditionDelete(itCond->second.FTID);
				else
					itCond->second.orderSrc.nVolume -= nSubVolume;
			}
		}
		else {
			OrderKey orderKey(it->second.refGain.Account, it->second.InstrumentID, 
					it->second.refGain.FrontID, it->second.refGain.SessionID, 
					it->second.refGain.OrderRef);
			PlatformStru_OrderInfo orderData;
			if(DEFAULT_SVR()->GetOrder(orderKey, orderData) 
					&& orderData.OrderStatus==THOST_FTDC_OST_NotTouched) {
				if(orderData.VolumeTotalOriginal>nSubVolume) {
					ZeroMemory(&waitDel, sizeof(waitDel));
					waitDel.nPosiPLNum = it->second.FTID;
					waitDel.bIsLose = FALSE;
					waitDel.oldCondRef = it->second.refGain;
					waitDel.plparam = it->second.plparam;
					waitDel.newOrder = it->second.orderSrc;
					waitDel.newOrder.nVolume = orderData.VolumeTotalOriginal-nSubVolume;
					waitDel.dwStarTime = ::GetTickCount();
					m_mapWaitDelServCond.insert(
							make_pair(waitDel.nPosiPLNum*2+waitDel.bIsLose, waitDel));
				}
				CancelCondServer(it->second.refGain, it->second.InstrumentID);
			}
		}
	}
	if(it->second.plparam.bDoStopLose) {
		if(!it->second.bLoseIsServ) {
			itCond = m_mapConditionOrder.find(it->second.refLose.FTID);
			if(itCond != m_mapConditionOrder.end()) {
				if(itCond->second.orderSrc.nVolume <= nSubVolume)
					OrderConditionDelete(itCond->second.FTID);
				else
					itCond->second.orderSrc.nVolume -= nSubVolume;
			}
		}
		else {
			OrderKey orderKey(it->second.refLose.Account, it->second.InstrumentID, 
					it->second.refLose.FrontID, it->second.refLose.SessionID, 
					it->second.refLose.OrderRef);
			PlatformStru_OrderInfo orderData;
			if(DEFAULT_SVR()->GetOrder(orderKey, orderData) 
					&& orderData.OrderStatus==THOST_FTDC_OST_NotTouched) {
				if(orderData.VolumeTotalOriginal>nSubVolume) {
					ZeroMemory(&waitDel, sizeof(waitDel));
					waitDel.nPosiPLNum = it->second.FTID;
					waitDel.bIsLose = TRUE;
					waitDel.plparam = it->second.plparam;
					waitDel.oldCondRef = it->second.refLose;
					waitDel.newOrder = it->second.orderSrc;
					waitDel.newOrder.nVolume = orderData.VolumeTotalOriginal-nSubVolume;
					waitDel.dwStarTime = ::GetTickCount();
					m_mapWaitDelServCond.insert(
							make_pair(waitDel.nPosiPLNum*2+waitDel.bIsLose, waitDel));
				}
				CancelCondServer(it->second.refLose, it->second.InstrumentID);
			}
		}
	}
	//PostEvent_PLDetailStatus(nNum, it->second.nPLNum);
	return TRUE;
}

BOOL CLocalOrderService::AsyncVolumePosiAndPL(PlatformStru_Position& posiInfo)
{
	if(util::isInvalidValue(posiInfo.CanCloseVolume))
		return FALSE;
	
	std::string strAccount(posiInfo.InvestorID);
	std::string strInstrumentID(posiInfo.InstrumentID);
	BOOL bIsBuy = !(posiInfo.PosiDirection==THOST_FTDC_PD_Long);
	int nHedgeStatus = posiInfo.HedgeFlag-THOST_FTDC_HF_Speculation;
	int nPosiPLVolume = GetPosiPLVolume(strAccount, 
			strInstrumentID, bIsBuy, nHedgeStatus);
	if(nPosiPLVolume<=posiInfo.CanCloseVolume)
		return FALSE;
	
	int nSubVol = nPosiPLVolume-posiInfo.CanCloseVolume;
	int nRemainVol = nSubVol;
	MAPPLDETAILORDERITEM itPLDetail;
	std::set<int> setDelete;
	std::map<int, int> mapChange;

	for(itPLDetail=m_mapPLDetailOrder.begin(); 
			itPLDetail!=m_mapPLDetailOrder.end() && nRemainVol>0; 
			itPLDetail++) {
		if(//strAccount.compare(itPLDetail->second.ref.Account)==0 && 
				strInstrumentID.compare(itPLDetail->second.InstrumentID)==0 
				&& bIsBuy==itPLDetail->second.orderSrc.bIsBuy
				&& nHedgeStatus==itPLDetail->second.orderSrc.nHedgeStatus) {
			int nVol = GetPosiPLVolume(itPLDetail->second);
			if(nVol<=nRemainVol) {
				setDelete.insert(itPLDetail->second.FTID);
				nRemainVol -= nVol;
			}
			else {
				mapChange.insert(make_pair(itPLDetail->second.FTID, nRemainVol));
				nRemainVol = 0;
			}
		}
	}

	std::set<int>::iterator itDelete;
	for(itDelete = setDelete.begin(); itDelete != setDelete.end(); itDelete++) {
		OrderPLDetailDelete(*itDelete, conPLDetailDelete, TRUE);
#if 0
		MAPPLDETAILORDERITEM itPLDTmp = m_mapPLDetailOrder.find(*itDelete);
		if(itPLDTmp != m_mapPLDetailOrder.end()) {
			if(!itPLDTmp->second.bGainIsServ && itPLDTmp->second.plparam.bDoStopGain)
				m_mapConditionOrder.erase(itPLDTmp->second.refGain.FTID);
			if(!itPLDTmp->second.bLoseIsServ && itPLDTmp->second.plparam.bDoStopLose)
				m_mapConditionOrder.erase(itPLDTmp->second.refLose.FTID);
			m_mapPLDetailOrder.erase(*itDelete);
		}
#endif
	}
	setDelete.clear();
	
	BOOL bRet = mapChange.size()>0;
	std::map<int, int>::iterator itChange;
	for(itChange = mapChange.begin(); itChange != mapChange.end(); itChange++) {
		OrderPLDetailChange(itChange->first, itChange->second);
	}
	mapChange.clear();
	return bRet;
	
}

void CLocalOrderService::AsyncPosiPLOrderVolume()
{
	if(DEFAULT_SVR()==NULL)
		return;

	MAPCONDITIONORDERITEM itCond;
	MAPPLDETAILORDERITEM itPLDetail;
	MAPPLORDERITEM itPL;

	std::vector<long> vecPosiList;
	if(DEFAULT_SVR()->GetPositionFTIDList(vecPosiList)==0)
		return;

	BOOL bHasChanged = FALSE;
	PlatformStru_Position posiInfo;
	for(int i=0; i<(int)vecPosiList.size(); i++) {
		if(DEFAULT_SVR()->GetPositionInfo(vecPosiList[i], posiInfo)==0)
			if(AsyncVolumePosiAndPL(posiInfo))
				bHasChanged = TRUE;
	}
	if(bHasChanged) {
		PostEvent_PLDetailStatus(-1, -1);
		PostEvent_ConditionStatus(-1);
	}
	
}

BOOL CLocalOrderService::CheckOverTradingDay()
{
	{
	// 本地预埋单
	MAPPARKEDORDER::iterator it = m_mapParkedOrder.begin();
	while(it != m_mapParkedOrder.end()) {
		if(it->second.nTradingDay != m_nTradingDay)
			it = m_mapParkedOrder.erase(it);
		else
			it++;
	}
	}

	{
	// 本地条件单
	MAPCONDITIONORDER::iterator it = m_mapConditionOrder.begin();
	while(it != m_mapConditionOrder.end()) {
		if(it->second.nTradingDay != m_nTradingDay)
			it = m_mapConditionOrder.erase(it);
		else
			it++;
	}
	}

	{
	// 本地盈损单
	MAPPLORDER::iterator it = m_mapPLOrder.begin();
	while(it != m_mapPLOrder.end()) {
		if(it->second.nTradingDay != m_nTradingDay)
			it = m_mapPLOrder.erase(it);
		else
			it++;
	}
	}

	{
	// 本地盈损单明细
	MAPPLDETAILORDER::iterator it = m_mapPLDetailOrder.begin();
	while(it != m_mapPLDetailOrder.end()) {
		if(it->second.nTradingDay != m_nTradingDay)
			it = m_mapPLDetailOrder.erase(it);
		else
			it++;
	}
	}
	
	PostEvent_ParkedStatus(-1);
	PostEvent_ConditionStatus(-1);
	PostEvent_PLStatus(-1);
	PostEvent_PLDetailStatus(-1, -1);

	return TRUE;
}
