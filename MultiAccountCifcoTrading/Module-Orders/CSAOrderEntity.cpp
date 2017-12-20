#include "StdAfx.h"
#include "CSAOrderEntity.h"
#include "../inc/Module-Misc/tools_util.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

//////////////////////////////////////////////////////////////////////////////////////


CSAOrderEntity::CSAOrderEntity(COrderAtomEntity::STDORDERREQ& stdReq, 
							   OPERPARAM& operParam, ORDERREQBREAK& orderReqBreak)
{
//	m_nReverseTimer = 3;
	m_nDoCancelStatus = conWaiting;
	m_nDoInsertStatus = conWaiting;
	//m_nOrderReqCount = 0;
	//ZeroMemory(m_tOrderReqs, sizeof(CMBORDER)*conMaxOrderReqNum);
	m_nNeedCancelVolume = 0;
	m_nOrderActionCount = 0;
	ZeroMemory(m_tOrderActions, sizeof(CMBORDERACTION)*conMaxOrderActionNum);
	ZeroMemory(m_bOrderActionsState, sizeof(BOOL)*conMaxOrderActionNum);

	m_stdOrderReq = stdReq;
	m_operParam = operParam;
	m_orderReqBreak = orderReqBreak;
}

CSAOrderEntity::~CSAOrderEntity()
{
	for(UINT i=0; i<m_arrOrderEntity.size(); i++) {
		if(m_arrOrderEntity[i] != NULL) {
			delete m_arrOrderEntity[i];
			m_arrOrderEntity[i] = NULL;
		}
	}
	m_arrOrderEntity.clear();
}

BOOL CSAOrderEntity::HasTrade(const string& strUserID)
{
	if(g_pPlatformMgr==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(strUserID);
	else
		pSvr = DEFAULT_SVR();
	if(pSvr!=NULL)
		return pSvr->GetQryFinished();
	else
		return FALSE;
}

CSAOrderEntity* CSAOrderEntity::CreateSAOrderEntity(COrderAtomEntity::STDORDERREQ& stdReq, 
													OPERPARAM& operParam)
{
	
	std::string strUserID(stdReq.csUserID);
	std::string strAccount(stdReq.csAccount);

	std::string strInstrumentID(stdReq.csInstrumentID);
	ORDERREQBREAK tOrderReqBreak={0};
	AnalyseOrderReq(stdReq, operParam, tOrderReqBreak);
	
	if(operParam.bSimulate) {
		string strInstrumentID(stdReq.csInstrumentID);
		BOOL bIsBuy = stdReq.bIsBuy;
		double fPrice = 0.0;
		BOOL bIsMarketPrice = stdReq.bIsMarket;
		//Macro
		AUTOMARKETPRICE2(strInstrumentID, bIsBuy, bIsMarketPrice, fPrice)
	}

	CSAOrderEntity* pEntity = new CSAOrderEntity(stdReq, operParam, tOrderReqBreak);
	
	if(tOrderReqBreak.bIsOpen) {
		pEntity->InsertOrder(stdReq, stdReq.bIsBuy, 
				conOCOpen, tOrderReqBreak.dwVolume_Open);
	}
	if(tOrderReqBreak.bIsCloseToday) {
		pEntity->InsertOrder(stdReq, stdReq.bIsBuy, 
				conOCClodeToday, tOrderReqBreak.dwVolume_CloseToday);
	}
	if(tOrderReqBreak.bIsClose) {
		pEntity->InsertOrder(stdReq, stdReq.bIsBuy, 
				conOCClose, tOrderReqBreak.dwVolume_Close);
	}
	if(tOrderReqBreak.bIsReverseOpen) {
		pEntity->InsertOrder(stdReq, stdReq.bIsBuy, 
				conOCOpen, tOrderReqBreak.dwVolume_ReverseOpen);
	}

	if(operParam.bNeedCancel)
		pEntity->MakeRevokeActions(strUserID, strAccount, 
				strInstrumentID, stdReq.bIsBuy, stdReq.nOCMode);

	return pEntity;
}

BOOL CSAOrderEntity::GetHoldingVolume(string& strUserID, string& strAccount, string& strCode, 
									  BOOL bIsBuy, BOOL bIsHedge, 
									  int& nSumVolume, int& nSumTodayVolume)
{
	char cDirect = bIsBuy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
	char cHedge = bIsHedge ? THOST_FTDC_HF_Hedge : THOST_FTDC_HF_Speculation;
	PlatformStru_Position PositionData;
	nSumVolume=0;
	nSumTodayVolume=0;
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(strUserID);
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;
	if(pSvr->GetPositionData(strUserID, strCode, cDirect, cHedge, PositionData)==0) {
		nSumVolume = util::isInvalidValue(PositionData.Position)?
				0:PositionData.Position;
		nSumTodayVolume = util::isInvalidValue(PositionData.TodayPosition)?
				0:PositionData.TodayPosition;
	}
	return nSumVolume>0;
}

BOOL CSAOrderEntity::GetCanCloseVolume(string& strUserID, string& strAccount, string& strCode, 
									   BOOL bIsBuy, BOOL bIsHedge, 
									   int& nCanCloseVolume, int& nCanCloseTodayVolume)
{
	char cDirect = bIsBuy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
	char cHedge = bIsHedge ? THOST_FTDC_HF_Hedge : THOST_FTDC_HF_Speculation;
	PlatformStru_Position PositionData;
	nCanCloseVolume=0;
	nCanCloseTodayVolume=0;
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(strUserID);
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;
	if(pSvr->GetPositionData(strUserID, strCode, cDirect, cHedge, PositionData)==0) {
		nCanCloseVolume = util::isInvalidValue(PositionData.CanCloseVolume)
				?0:PositionData.CanCloseVolume;
		nCanCloseTodayVolume = util::isInvalidValue(PositionData.CanCloseTodayVolume)
				?0:PositionData.CanCloseTodayVolume;
	}
	return nCanCloseVolume>0;
}

BOOL CSAOrderEntity::GetPreCloseVolume(string& strUserID, string& strAccount, string& strCode, 
									   BOOL bIsBuy, BOOL bIsHedge, 
									   int& nPreCloseVolume, int& nPreCloseTodayVolume)
{
	char cDirect = bIsBuy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
	char cHedge = bIsHedge ? THOST_FTDC_HF_Hedge : THOST_FTDC_HF_Speculation;
	PlatformStru_Position PositionData;
	nPreCloseVolume=0;
	nPreCloseTodayVolume=0;
	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(strUserID);
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;
	if(pSvr->GetPositionData(strUserID, strCode, cDirect, cHedge, PositionData)==0) {
		nPreCloseVolume = util::isInvalidValue(PositionData.CloseOrderVolume)
				?0:PositionData.CloseOrderVolume;
		nPreCloseTodayVolume = util::isInvalidValue(PositionData.CloseOrderVolumeTdPart)
				?0:PositionData.CloseOrderVolumeTdPart;
	}
	return nPreCloseVolume>0;
}

BOOL CSAOrderEntity::AnalyseOrderReq(COrderAtomEntity::STDORDERREQ& stdReq, OPERPARAM& operParam, 
									 ORDERREQBREAK& tOrderReqBreak)
{
		
	int nSumVolume = 0, nSumTodayVolume = 0, nSumYesterdayVolume = 0;
	//int nAskVolume = 0, nAskTodayVolume = 0, nAskYesterdayVolume = 0;
	//int nPreVolume = 0, nPreTodayVolume = 0, nPreYesterdayVolume = 0;	// 可平仓数

	std::string strInstrumentID(stdReq.csInstrumentID);

	switch(operParam.nOperType) {
	case conOperNoneAuto:
		// 非自动模式
		// 双击标价表总是开仓，应设置为非自动模式

		switch(stdReq.nOCMode) {
		case 0:
		case 1:
			tOrderReqBreak.bIsOpen = TRUE;
			tOrderReqBreak.dwVolume_Open = stdReq.nVolume;
			tOrderReqBreak.fPrice_Open = stdReq.fPrice;
			break;
		case 2:
			tOrderReqBreak.bIsCloseToday = TRUE;
			tOrderReqBreak.dwVolume_CloseToday = stdReq.nVolume;
			tOrderReqBreak.fPrice_CloseToday = stdReq.fPrice;
			break;
		case 3:
			tOrderReqBreak.bIsClose = TRUE;
			tOrderReqBreak.dwVolume_Close = stdReq.nVolume;
			tOrderReqBreak.fPrice_Close = stdReq.fPrice;
			break;
		};
		break;

	case conOperAuto:
		// 自动模式
		
		//GetHoldingVolume(string(tOriReq.csUserID), string(tOriReq.csAccount), strInstrumentID, 
		//		!tOriReq.bIsBuy, tOriReq.bIsHedge, nSumVolume, nSumTodayVolume);
		////GetAskVolume(strInstrumentID, bDirection, nAskVolume, nAskTodayVolume);
		if(operParam.bIsCanClose) 
			CSAOrderEntity::GetCanCloseVolume(string(stdReq.csUserID), string(stdReq.csAccount), 
					strInstrumentID, !stdReq.bIsBuy, stdReq.bIsHedge, nSumVolume, nSumTodayVolume);
		else
			CSAOrderEntity::GetHoldingVolume(string(stdReq.csUserID), string(stdReq.csAccount), 
					strInstrumentID, !stdReq.bIsBuy, stdReq.bIsHedge, nSumVolume, nSumTodayVolume);

		nSumYesterdayVolume = nSumVolume - nSumTodayVolume;
		//nAskYesterdayVolume = nAskVolume - nAskTodayVolume;
		
		if(nSumVolume==0 && operParam.nAutoMode==conAutoReverseOpen) {
			tOrderReqBreak.bIsOpen = TRUE;
			tOrderReqBreak.dwVolume_Open = operParam.nDefaultVolume;
			tOrderReqBreak.fPrice_Open = stdReq.fPrice;
		}
		else {

			switch(operParam.nAutoMode) {
			case conAutoCloseAll:	// 全平
				tOrderReqBreak.bIsClose = nSumYesterdayVolume!=0;
				tOrderReqBreak.dwVolume_Close = nSumYesterdayVolume;
				tOrderReqBreak.fPrice_Close = stdReq.fPrice;
				tOrderReqBreak.bIsCloseToday = nSumTodayVolume!=0;
				tOrderReqBreak.dwVolume_CloseToday = nSumTodayVolume;
				tOrderReqBreak.fPrice_CloseToday = stdReq.fPrice;
				break;
			case conAutoCloseDefault:	// 按默认手数平仓
			{
				int nTradeVolume = (operParam.nDefaultVolume>nSumVolume ? 
						nSumVolume : operParam.nDefaultVolume);
				int nTradeTodayVolume = (nTradeVolume>nSumTodayVolume ? 
						nSumTodayVolume : nTradeVolume);
				int nTradeYesterdayVolume = (nTradeVolume>nTradeTodayVolume ? 
						nTradeVolume-nTradeTodayVolume : 0);

				tOrderReqBreak.bIsClose = nTradeYesterdayVolume!=0;
				tOrderReqBreak.dwVolume_Close = nTradeYesterdayVolume;
				tOrderReqBreak.fPrice_Close = stdReq.fPrice;
				tOrderReqBreak.bIsCloseToday = nTradeTodayVolume!=0;
				tOrderReqBreak.dwVolume_CloseToday = nTradeTodayVolume;
				tOrderReqBreak.fPrice_CloseToday = stdReq.fPrice;
			}
				break;
			case conAutoReverseOpen:	// 按默认手数平仓，多出部分反向开仓
			{
				int nTradeVolume = (operParam.nDefaultVolume>nSumVolume ? 
						nSumVolume : operParam.nDefaultVolume);
				int nTradeTodayVolume = (nTradeVolume>nSumTodayVolume ? 
						nSumTodayVolume : nTradeVolume);
				int nTradeYesterdayVolume = (nTradeVolume>nTradeTodayVolume ? 
						nTradeVolume-nTradeTodayVolume : 0);
				int nReverseVolume = operParam.nDefaultVolume > nSumVolume ? 
						operParam.nDefaultVolume - nSumVolume : 0;

				tOrderReqBreak.bIsReverseOpen = nReverseVolume>0;
				tOrderReqBreak.dwVolume_ReverseOpen = nReverseVolume;
				tOrderReqBreak.fPrice_ReverseOpen = stdReq.fPrice;
				tOrderReqBreak.bIsClose = nTradeYesterdayVolume!=0;
				tOrderReqBreak.dwVolume_Close = nTradeYesterdayVolume;
				tOrderReqBreak.fPrice_Close = stdReq.fPrice;
				tOrderReqBreak.bIsCloseToday = nTradeTodayVolume!=0;
				tOrderReqBreak.dwVolume_CloseToday = nTradeTodayVolume;
				tOrderReqBreak.fPrice_CloseToday = stdReq.fPrice;
			}
				break;
			};
		}
		break;
	};
	return TRUE;
}

BOOL CSAOrderEntity::MakeRevokeActions(string& strUserID, string& strAccount, 
									   string& strInstrumentID, BOOL bDirection, int nOCMode)
{

	// 如果撤单也需要确认后再进行，就把要下的撤单保存到成员变量中，
	// 然后待确认后执行这些保存在成员变量中的撤单，就象自动单一样
	// 判断是否有要撤的单，判读撤单数组里存的撤单数是否大于零即可
	std::string strRetMsg;
	std::vector<PlatformStru_OrderInfo> allOrders;
    int i,count;

	m_nNeedCancelVolume = 0;
	m_nOrderActionCount = 0;
	ZeroMemory(m_tOrderActions, sizeof(CMBORDERACTION)*conMaxOrderActionNum);

	allOrders.clear();

	if(g_pPlatformMgr==NULL || DEFAULT_SVR()==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(m_stdOrderReq.csUserID));
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;
	if(pSvr->GetAllOrders(allOrders)>0) 
    {
        count=allOrders.size();

		for(i=0;i<count;i++) 
        {
            PlatformStru_OrderInfo& field = allOrders[i];
			if(strUserID.compare(field.InvestorID)==0 &&
					strInstrumentID.compare(field.InstrumentID)==0 && 
					(field.Direction==THOST_FTDC_D_Buy) == bDirection && 
					//(field.CombOffsetFlag[0]!=THOST_FTDC_OF_Close || 
					// field.CombOffsetFlag[0]!=THOST_FTDC_OF_CloseToday || 
					// field.CombOffsetFlag[0]!=THOST_FTDC_OF_CloseYesterday) && 
					field.ContingentCondition==THOST_FTDC_CC_Immediately &&
					(field.OrderStatus==THOST_FTDC_OST_PartTradedQueueing || 
					 field.OrderStatus==THOST_FTDC_OST_NoTradeQueueing)) {

				PlatformStru_InputOrderAction action={0};
				CCifcoInputOrderActionField& ReqData = action.Thost;
				ZeroMemory(&ReqData, sizeof(PlatformStru_InputOrderAction));
				
				ReqData.FrontID=field.FrontID;
				ReqData.SessionID=field.SessionID;
				memcpy(ReqData.OrderRef, field.OrderRef, sizeof(field.OrderRef));
				memcpy(ReqData.OrderSysID, field.OrderSysID, sizeof(field.OrderSysID));
				memcpy(ReqData.ExchangeID, field.ExchangeID, sizeof(field.ExchangeID));
				memcpy(ReqData.InstrumentID, field.InstrumentID, sizeof(field.InstrumentID));

				m_tOrderActions[m_nOrderActionCount].action = action;
				m_nOrderActionCount++;

				m_nNeedCancelVolume += field.VolumeTotal;
				{
					wxString strValue;
					wxString strOrderSysID = field.OrderSysID;
					strOrderSysID.Trim(false);
					strValue.Printf(LOADFORMATSTRING(CANCELQTYPRC_FORMAT,"%s%s%s%s%s%s%s"), //文本必须包含%s,%s,%s,%s,%s,%s,%s
								strOrderSysID.c_str(), 
								field.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN): 
								(field.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
								field.Direction==THOST_FTDC_D_Buy ?LOADSTRING(OLS_BUY) :LOADSTRING(OLS_SELL), 
								field.InstrumentID, Number2String(field.VolumeTotalOriginal).c_str(), 
								field.OrderPriceType==THOST_FTDC_OPT_AnyPrice ? LOADSTRING(OLS_MARKET) : 
									Double2String(field.LimitPrice).c_str(),
								field.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ? LOADSTRING(OLS_HEDGE) : 
								(field.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation ? LOADSTRING(OLS_SPECULATE):LOADSTRING(OLS_ARBITRAGE)));
					strValue = REPLACEFMTSTRING(strValue.c_str());
					strRetMsg += strValue;
				}
		
			}
		}
	}
	m_strRevokeMsg = strRetMsg;

	return TRUE;
}

//BOOL CSAOrderEntity::MakeRevokeAction_Replace(OrderKey& orderKey)
//{
//	// 改单功能一般是先撤原报单，后下新报单
//
//	std::string strRetMsg;
//
//	//m_nOrderActionCount = 0;
//	//ZeroMemory(m_tOrderActions, sizeof(PlatformStru_InputOrderAction)*conMaxOrderActionNum);
//
//	PlatformStru_OrderInfo ftdcOrderField;
//
//	if(g_pPlatformMgr==NULL)
//		return NULL;
//	IPlatformSingleSvr* pSvr = NULL;
//	if(DEFAULT_SVR()->IsMultipleAccounts())
//		pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(m_oriOrderReq.csUserID));
//	else
//		pSvr = DEFAULT_SVR();
//	if(pSvr==NULL) 
//		return FALSE;
//	if(!pSvr->GetOrder(orderKey, ftdcOrderField)) 
//		return FALSE;
//
//	PlatformStru_OrderInfo& field = ftdcOrderField;
//
//	PlatformStru_InputOrderAction ReqData={0};
//	ZeroMemory(&ReqData, sizeof(PlatformStru_InputOrderAction));
//	
//	ReqData.Thost.FrontID=field.FrontID;
//	ReqData.Thost.SessionID=field.SessionID;
//	memcpy(ReqData.Thost.OrderRef, field.OrderRef, sizeof(field.OrderRef));
//	memcpy(ReqData.Thost.OrderSysID, field.OrderSysID, sizeof(field.OrderSysID));
//	memcpy(ReqData.Thost.ExchangeID, field.ExchangeID, sizeof(field.ExchangeID));
//	memcpy(ReqData.Thost.InstrumentID, field.InstrumentID, sizeof(field.InstrumentID));
//
//	m_tOrderActions[m_nOrderActionCount].action = ReqData;
//	m_nOrderActionCount++;
//	{
//		wxString strValue;
//		wxString strOrderSysID = field.OrderSysID;
//		strOrderSysID.Trim(false);
//		strValue.Printf(LOADFORMATSTRING(CANCELQTYPRC_FORMAT,"%s%s%s%s%s%s%s"), //文本必须包含%s,%s,%s,%s,%s,%s,%s
//					strOrderSysID.c_str(), 
//					field.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN): 
//						(field.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)), 
//					field.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY): LOADSTRING(OLS_SELL), 
//					field.InstrumentID, 
//					Number2String(field.VolumeTotalOriginal).c_str(), 
//					Double2String(field.LimitPrice).c_str(), 
//					field.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ? LOADSTRING(OLS_HEDGE) : 
//					(field.CombHedgeFlag[0]==THOST_FTDC_HF_Speculation ? LOADSTRING(OLS_SPECULATE): LOADSTRING(OLS_ARBITRAGE)));
//		strValue = REPLACEFMTSTRING(strValue.c_str());
//		strRetMsg += strValue;
//	}
//	m_strRevokeMsg = strRetMsg;
//
//	return TRUE;
//}

//BOOL CSAOrderEntity::InsertOrder(string& strUserID, string& strAccount, string& strInstrumentID, 
//								 BOOL bIsBuy, int nOCMode, double fPrice, int nVolume, 
//								 BOOL bIsHedge, BOOL bIsMarketPrice, BOOL bSimulate, 
//								 BOOL bIsParketOrder)
//{
//	
//	int n=m_nOrderReqCount;
//	
//	if(bSimulate) {
//		//Macro
//		AUTOMARKETPRICE2(strInstrumentID, bIsBuy, bIsMarketPrice, fPrice)
//	}
//	
//	m_tOrderReqs[n].bIsParked = bIsParketOrder;
//	if(bIsParketOrder)
//		MakeReq_Parked(m_tOrderReqs[n].order.parked, strInstrumentID, strUserID, OrderType_FT_Immediately, 
//				bIsBuy, nOCMode+1, nVolume, fPrice, 0, bIsHedge, bIsMarketPrice, FALSE);
//	else
//		MakeReq_Order(m_tOrderReqs[n].order.insert, strInstrumentID, strUserID, OrderType_FT_Immediately, 
//				bIsBuy, nOCMode+1, nVolume, fPrice, 0, bIsHedge, bIsMarketPrice, FALSE);
//	m_nOrderReqCount++;
//		
//	return TRUE;
//
//}

BOOL CSAOrderEntity::InsertOrder(COrderAtomEntity::STDORDERREQ stdReq, 
								 BOOL bIsBuy, int nOCMode, int nVolume)
{
	
	COrderAtomEntity::STDORDERREQ stdNewReq = stdReq;

	stdNewReq.bIsBuy = bIsBuy;
	stdNewReq.nOCMode = nOCMode;
	stdNewReq.nVolume = nVolume;

	COrderAtomEntity* pNewEntity = COrderAtomEntity::CreateOrderAtomEntity(
			stdNewReq, COrderAtomEntity::conModeNewOrder, NULL, 
			m_operParam.bNoSendVolZoreOrder);
	m_arrOrderEntity.push_back(pNewEntity);
		
	return TRUE;

}

int CSAOrderEntity::GetRunStatus(int nOCMode)
{
	for(int i=0; i<m_arrOrderEntity.size(); i++) {
		if(m_arrOrderEntity[i]!=NULL)
			if(m_arrOrderEntity[i]->GetStdOrderReq().nOCMode==nOCMode)
				return m_arrOrderEntity[i]->GetRunStatus();
	}
	return COrderAtomEntity::conNothing;
}

BOOL CSAOrderEntity::GetVolumeSum_Trade(int& nOpenVolume, int& nTodayVolume, int& nYdVolume)
{
	if(m_nDoInsertStatus==conWaiting)
		return FALSE;
	
	 UINT i=0;

	for(; i<m_arrOrderEntity.size(); i++) {
		if(!COrderAtomEntity::IsParketOrder(m_arrOrderEntity[i]->GetStdOrderReq())) {
			if(m_arrOrderEntity[i]->GetStdOrderReq().nOCMode == conOCOpen) {
				nOpenVolume += m_arrOrderEntity[i]->GetTradeVolume();
			}
			if(m_arrOrderEntity[i]->GetStdOrderReq().nOCMode == conOCClodeToday) {
				nTodayVolume += m_arrOrderEntity[i]->GetTradeVolume();
			}
			if(m_arrOrderEntity[i]->GetStdOrderReq().nOCMode == conOCClose) {
				nYdVolume += m_arrOrderEntity[i]->GetTradeVolume();
			}
		}
	}
	return TRUE;
}

BOOL CSAOrderEntity::GetCancelVolumeSum(int& nWaitVolume, int& nRealVolume)
{
	nWaitVolume = m_nNeedCancelVolume;

	if(m_nDoCancelStatus==conWaiting)
		return TRUE;
	
	int i=0;

	for(; i<m_nOrderActionCount; i++) {
		if(m_tOrderActions[i].orderRsp.OrderStatus == THOST_FTDC_OST_Canceled) 
			nRealVolume += m_tOrderActions[i].orderRsp.VolumeTotal;
	}
	return TRUE;
}

int CSAOrderEntity::SumCanCancel()
{
	int nSum = 0;
	for(int i=0; i<m_arrOrderEntity.size(); i++)
		if(m_arrOrderEntity[i]->IsCanCancel())
			nSum++;
	return nSum;
}

BOOL CSAOrderEntity::DoOrderReq(int& nErrPos, std::string& strRetMsg, BOOL bNoSendZero)
{
	if(m_nDoInsertStatus!=conWaiting)
		return FALSE;
	
	m_nDoInsertStatus = conHasDone;
	
	int nRet = -1;
	nErrPos = -1;

	strRetMsg="";

	for(UINT i=0; i<m_arrOrderEntity.size(); ++i) {
		if(m_arrOrderEntity[i] != NULL) {
			if(!m_arrOrderEntity[i]->Launch()) {
				m_nDoInsertStatus = conIsFail;
				nErrPos = i;
				char strText[1024];
				ZeroMemory(strText, sizeof(strText));
				sprintf_s(strText, sizeof(strText)-1, "%d, %s", 
						m_arrOrderEntity[i]->GetLastRspInfo().ErrorID, 
						m_arrOrderEntity[i]->GetLastRspInfo().ErrorMsg);
				strRetMsg = strText;
				return FALSE;
			}
		}
	}
	strRetMsg = LOADSTRING(ORDER_IE_CHANGEOK);//wxT("下单成功！\n");
	return TRUE;

}

BOOL CSAOrderEntity::DoRevokeAction(int& nErrPos, string& strRetMsg)
{
	if(m_nDoCancelStatus!=conWaiting)
		return FALSE;
	
	m_nDoCancelStatus = conHasDone;
	
	if(g_pPlatformMgr==NULL)
		return FALSE;
	IPlatformSingleSvr* pSvr = NULL;
	if(DEFAULT_SVR()->IsMultipleAccounts())
		pSvr = g_pPlatformMgr->GetPlatformSvr(std::string(m_stdOrderReq.csUserID));
	else
		pSvr = DEFAULT_SVR();
	if(pSvr==NULL) 
		return FALSE;

	int nRet = -1;
	nErrPos = -1;

	strRetMsg="";

	for(int i=0; i<m_nOrderActionCount; ++i) {
		nRet = pSvr->ReqOrderAction(m_tOrderActions[i].action, DEFAULT_SVR()->GetNextReqID());
		if(nRet!=0) {
			m_nDoCancelStatus = conIsFail;
			char strText[1024];
			ZeroMemory(strText, sizeof(strText));
			nErrPos = i;
			sprintf_s(strText, LOADFORMATSTRING(ORDER_IE_REVOKEMSG,"%d%s"),//文本必须包含%d %s
					i, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
			strRetMsg = strText;
			strcpy(m_tOrderActions[i].rspInfo.ErrorMsg, 
					CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
			return FALSE;
		}
	}
	strRetMsg = wxT(LOADSTRING(ORDER_IE_REVOKEOK));//wxT("撤单成功！\n");
	return TRUE;

}

BOOL CSAOrderEntity::Cancel(int& nCancelSum, string& strRetMsg)
{
	if(m_nDoInsertStatus!=conHasDone)
		return FALSE;
	
	m_nDoInsertStatus=conIsCancel;

	UINT i=0;

	for(; i<m_arrOrderEntity.size(); i++) {
		if(m_arrOrderEntity[i]!=NULL) 
			if(m_arrOrderEntity[i]->Cancel())
				nCancelSum++;
	}
	return TRUE;
}

void CSAOrderEntity::ChangePrice(BOOL bIsMarket, double fPrice)
{
	if(m_nDoCancelStatus!=conWaiting)
		return;

	m_stdOrderReq.bIsMarket = bIsMarket;
	m_stdOrderReq.fPrice = fPrice;

	for(UINT i=0; i<m_arrOrderEntity.size(); ++i) {
		m_arrOrderEntity[i]->ChangePrice(bIsMarket, fPrice);
	}	
}

void CSAOrderEntity::ChangeTriggerCond(int nPriceType, int nCondType, 
									   BOOL bIsMarket, double fPrice, 
									   BOOL bNeedChangeOrderType)
{
	if(m_nDoCancelStatus!=conWaiting)
		return;

	m_stdOrderReq.bIsMarket = bIsMarket;
	m_stdOrderReq.fPrice = fPrice;

	for(UINT i=0; i<m_arrOrderEntity.size(); ++i) {
		m_arrOrderEntity[i]->ChangeTriggerCond(nPriceType, nCondType, 
				bIsMarket, fPrice, bNeedChangeOrderType);
	}	
}

BOOL CSAOrderEntity::ResetParkedStatus(DataRspParkedOrderInsert& field)
{
	BOOL bRet = FALSE;
	UINT i=0;
	for(; i<m_arrOrderEntity.size(); i++) {
		if(m_arrOrderEntity[i] != NULL) 
			if(m_arrOrderEntity[i]->ParseParkedStatus(field))
				bRet = TRUE;
	}
	return bRet;
}

BOOL CSAOrderEntity::ResetOrderStatus(DataRtnOrder& field)
{
	BOOL bRet = FALSE;
	UINT i=0;
	for(; i<m_arrOrderEntity.size(); i++) {
		if(m_arrOrderEntity[i] != NULL) {
			if(m_arrOrderEntity[i]->ParseOrderStatus(field)) {
				bRet = TRUE;
				if(m_arrOrderEntity[i]->GetRunStatus()==COrderAtomEntity::conIsFail)
					m_nDoInsertStatus = conIsFail;
			}
		}
	}
	return bRet;
}

BOOL CSAOrderEntity::ResetOrderStatus(DataRspOrderInsert& field)
{
	BOOL bRet = FALSE;
	UINT i=0;
	for(; i<m_arrOrderEntity.size(); i++) {
		if(m_arrOrderEntity[i] != NULL) 
			if(m_arrOrderEntity[i]->ParseOrderStatus(field)) {
				bRet = TRUE; 
				if(m_arrOrderEntity[i]->GetRunStatus()==COrderAtomEntity::conIsFail)
					m_nDoInsertStatus = conIsFail;
			}
	}
	return bRet;
}

BOOL CSAOrderEntity::ResetActionStatus(DataRspOrderAction1& field)
{
	BOOL bRet = FALSE;
	UINT i=0;
	for(; i<m_arrOrderEntity.size(); i++) {
		if(m_arrOrderEntity[i] != NULL) 
			if(m_arrOrderEntity[i]->ParseActionStatus(field))
				bRet = TRUE;
	}
	if(bRet)
		m_nDoCancelStatus = conIsFail;
	return bRet;
}

BOOL CSAOrderEntity::ResetActionStatus(DataRspOrderAction2& field)
{
	BOOL bRet = FALSE;
	UINT i=0;
	for(; i<m_arrOrderEntity.size(); i++) {
		if(m_arrOrderEntity[i] != NULL) 
			if(m_arrOrderEntity[i]->ParseActionStatus(field))
				bRet = TRUE;
	}
	if(bRet)
		m_nDoCancelStatus = conIsFail;
	return bRet;
}

BOOL CSAOrderEntity::MarkOrderActionStatus(DataRtnOrder& field)
{
	string strTmpSysID;
	string strTmpExchangeID;
	string strInSysID = field.OrderField.OrderSysID;
	string strInExchangeID = field.OrderField.ExchangeID;
	
	for(int i=0; i<m_nOrderActionCount; i++) {
		strTmpSysID = m_tOrderActions[i].action.Thost.OrderSysID;
		strTmpExchangeID = m_tOrderActions[i].action.Thost.ExchangeID;
		if(strTmpSysID.compare(strInSysID)==0 
				&& strTmpExchangeID.compare(strInExchangeID)==0
				&& field.OrderField.OrderStatus==THOST_FTDC_OST_Canceled) {
			m_tOrderActions[i].orderRsp = field.OrderField;
			m_bOrderActionsState[i]=TRUE;
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CSAOrderEntity::FindOrderAction(DataRspOrderAction2& act)
{
	std::string strTmpSysID;
	string strTmpExchangeID;
	std::string strInSysID = act.OrderActionField.OrderSysID;
	string strInExchangeID = act.OrderActionField.ExchangeID;
	
	for(int i=0; i<m_nOrderActionCount; i++) {
		strTmpSysID = m_tOrderActions[i].action.Thost.OrderSysID;
		strTmpExchangeID = m_tOrderActions[i].action.Thost.ExchangeID;
		if(strTmpSysID.compare(strInSysID)==0
				&& strTmpExchangeID.compare(strInExchangeID)==0) {
			m_bOrderActionsState[i]=FALSE;
			return TRUE;
		}
	}
	
	return FALSE;
}

string CSAOrderEntity::GetOrderDesciption()
{
	std::string strText;

	strText = wxT(LOADSTRING(ORDER_IE_TITLE));//wxT("下单: \n");	
	for(UINT i=0; i<m_arrOrderEntity.size(); ++i) {
		if(m_arrOrderEntity[i]!=NULL) {
			strText += m_arrOrderEntity[i]->GetOrderDesciption().c_str();
		}
	}
	strText += wxT(LOADSTRING(ORDER_IE_CHANGECOMFIRM));//wxT("是否确认?");

	return strText;
}
