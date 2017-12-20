#include "StdAfx.h"
#include "COrderInsertEntity.h"
#include "../inc/Module-Misc/tools_util.h"
//#include "PositionDetailInfoList.h"               //去掉对CPositionBaseData的引用
#include "../inc/Order-Common/COrderServiceThread.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

//////////////////////////////////////////////////////////////////////////////////////


COrderInsertEntity::COrderInsertEntity()
{
	
//	m_nReverseTimer = 3;
	m_bIsAuto = FALSE;
	m_bIsParkedOrder = FALSE;
	m_nRspOrderReqCount = 0;
	ZeroMemory(m_bFtdcOrderActionsState, sizeof(BOOL)*conMaxRspOrderReqNum);
	ZeroMemory(m_tRspOrderReqs, sizeof(DataRspOrderInsert)*conMaxRspOrderReqNum);
	m_nFtdcOrderActionCount = 0;
	ZeroMemory(m_tFtdcOrderActions, sizeof(CThostFtdcInputOrderActionField)*conMaxFtdcOrderActionNum);

}

COrderInsertEntity::~COrderInsertEntity()
{
}

COrderInsertEntity* COrderInsertEntity::CreateOrderInsert(ORIORDERREQ& tOriReq)
{
	
	ORDERREQBREAK tOrderReqBreak={0};
	wxString strInstrumentID(tOriReq.strInstrumentID);
	wxString strAccount(tOriReq.strAccount);	
	AnalyseOrderReq(tOriReq, tOrderReqBreak);
	
	COrderInsertEntity* pEntity = new COrderInsertEntity();
	pEntity->m_bIsAuto = tOriReq.bIsAuto;
	pEntity->m_bIsParkedOrder = tOriReq.bIsParketOrder; 
	
	if(tOrderReqBreak.bIsOpen) {
		pEntity->InsertOrder(strInstrumentID, strAccount, tOriReq.bDirection, 0, 
				tOrderReqBreak.fPrice_Open, tOrderReqBreak.dwVolume_Open, 
				tOriReq.bIsHedge, tOriReq.bIsMarketPrice, tOriReq.bSimulate, 
				tOriReq.bIsParketOrder);
	}
	if(tOrderReqBreak.bIsCloseToday) {
		pEntity->InsertOrder(strInstrumentID, strAccount, tOriReq.bDirection, 1, 
				tOrderReqBreak.fPrice_CloseToday, tOrderReqBreak.dwVolume_CloseToday, 
				tOriReq.bIsHedge, tOriReq.bIsMarketPrice, tOriReq.bSimulate, 
				tOriReq.bIsParketOrder);
	}
	if(tOrderReqBreak.bIsClose) {
		pEntity->InsertOrder(strInstrumentID, strAccount, tOriReq.bDirection, 2, 
				tOrderReqBreak.fPrice_Close, tOrderReqBreak.dwVolume_Close, 
				tOriReq.bIsHedge, tOriReq.bIsMarketPrice, tOriReq.bSimulate, 
				tOriReq.bIsParketOrder);
	}
	if(tOrderReqBreak.bIsReverseOpen) {
		pEntity->InsertOrder(strInstrumentID, strAccount, tOriReq.bDirection, 0, 
				tOrderReqBreak.fPrice_ReverseOpen, tOrderReqBreak.dwVolume_ReverseOpen, 
				tOriReq.bIsHedge, tOriReq.bIsMarketPrice, tOriReq.bSimulate, 
				tOriReq.bIsParketOrder);
	}
	
	return pEntity;
	
}

BOOL COrderInsertEntity::GetHoldingVolume(string strCode, BOOL bIsBuy, BOOL bIsHedge, 
										  int& nSumVolume, int& nSumTodayVolume, string strAccount)
{
	char cDirect = bIsBuy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
	char cHedge = bIsHedge ? THOST_FTDC_HF_Hedge : THOST_FTDC_HF_Speculation;
	PlatformStru_Position PositionData;
	nSumVolume=0;
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetPositionData(strAccount,strCode, cDirect, cHedge, PositionData)==0) {
		nSumVolume = util::isInvalidValue(PositionData.Position)?0:PositionData.Position;
		nSumTodayVolume = util::isInvalidValue(PositionData.TodayPosition)?0:PositionData.TodayPosition;
	}
	return nSumVolume>0;
}

BOOL COrderInsertEntity::GetAskCloseVolume(string strCode, BOOL bIsBuy, BOOL bIsHedge, 
										   int& nAskVolume, int& nAskTodayVolume, string strAccount)
{
	char cDirection = bIsBuy ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
	char cHedge = bIsHedge ? THOST_FTDC_HF_Hedge : THOST_FTDC_HF_Speculation;
	int nCloseVolume = 0, nCloseYesterdayVolume = 0;

	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetCloseOrderVolume(strAccount,strCode, cDirection, 
		nCloseVolume, nAskTodayVolume, nCloseYesterdayVolume, cHedge) == 0) {
		if(util::isInvalidValue(nAskTodayVolume))
			nAskTodayVolume = 0;
		nAskVolume = nAskTodayVolume+util::isInvalidValue(nCloseVolume)?0:nCloseVolume;
	}
	else {
		nAskVolume = 0;
		nAskTodayVolume = 0;
	}
	return TRUE;
}

BOOL COrderInsertEntity::GetPreVolume(string strCode, BOOL bIsBuy, BOOL bIsHedge, 
									  int& nPreVolume, int& nPreTodayVolume, string strAccount)
{
	int nSumVolume = 0, nSumTodayVolume = 0;
	int nAskVolume = 0, nAskTodayVolume = 0;

	if(!GetHoldingVolume(strCode, bIsBuy, bIsHedge, nSumVolume, nSumTodayVolume, strAccount)) return FALSE;
	if(!GetAskCloseVolume(strCode, !bIsBuy, bIsHedge, nAskVolume, nAskTodayVolume, strAccount)) return FALSE;
	nPreVolume = nSumVolume-nAskVolume;
	nPreTodayVolume = nSumTodayVolume-nAskTodayVolume;
	return TRUE;
}

BOOL COrderInsertEntity::AnalyseOrderReq(ORIORDERREQ& tOriReq, ORDERREQBREAK& tOrderReqBreak)
{
		
	int nSumVolume = 0, nSumTodayVolume = 0, nSumYesterdayVolume = 0;
	//int nAskVolume = 0, nAskTodayVolume = 0, nAskYesterdayVolume = 0;
	//int nPreVolume = 0, nPreTodayVolume = 0, nPreYesterdayVolume = 0;	// 可平仓数

	wxString strInstrumentID(tOriReq.strInstrumentID);

	if(!tOriReq.bIsAuto) {

		// 非自动模式
		// 双击标价表总是开仓，应设置为非自动模式

		switch(tOriReq.nOCMode) {
		case 0:
			tOrderReqBreak.bIsOpen = TRUE;
			tOrderReqBreak.dwVolume_Open = tOriReq.nVolume;
			tOrderReqBreak.fPrice_Open = tOriReq.fPrice;// + 
					//tOriReq.nPriceOffset*tOriReq.fPriceTick;
			break;
		case 1:
			tOrderReqBreak.bIsCloseToday = TRUE;
			tOrderReqBreak.dwVolume_CloseToday = tOriReq.nVolume;
			tOrderReqBreak.fPrice_CloseToday = tOriReq.fPrice;// + 
					//tOriReq.nPriceOffset*tOriReq.fPriceTick;
			break;
		case 2:
			tOrderReqBreak.bIsClose = TRUE;
			tOrderReqBreak.dwVolume_Close = tOriReq.nVolume;
			tOrderReqBreak.fPrice_Close = tOriReq.fPrice;// + 
					//tOriReq.nPriceOffset*tOriReq.fPriceTick;
			break;
		};

	}
	else {

		// 自动模式
		
		GetHoldingVolume(strInstrumentID.c_str(), !tOriReq.bDirection, tOriReq.bIsHedge, nSumVolume, nSumTodayVolume, string(tOriReq.strAccount));
		//GetAskVolume(strInstrumentID, bDirection, nAskVolume, nAskTodayVolume);

		nSumYesterdayVolume = nSumVolume - nSumTodayVolume;
		//nAskYesterdayVolume = nAskVolume - nAskTodayVolume;
		
		if(nSumVolume==0) {

			tOrderReqBreak.bIsOpen = TRUE;
			tOrderReqBreak.dwVolume_Open = tOriReq.nVolume;
			tOrderReqBreak.fPrice_Open = tOriReq.fPrice;// + 
					//tOriReq.nPriceOffset*tOriReq.fPriceTick;

		}
		else {

			switch(tOriReq.nAutoMode) {
			case 1:	// 全平
				if(nSumYesterdayVolume == 0) {
					tOrderReqBreak.bIsCloseToday = TRUE;
					tOrderReqBreak.dwVolume_CloseToday = nSumTodayVolume;
					tOrderReqBreak.fPrice_CloseToday = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
				else if(nSumTodayVolume == 0) {
					tOrderReqBreak.bIsClose = TRUE;
					tOrderReqBreak.dwVolume_Close = nSumYesterdayVolume;
					tOrderReqBreak.fPrice_Close = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
				else {
					tOrderReqBreak.bIsClose = TRUE;
					tOrderReqBreak.dwVolume_Close = nSumYesterdayVolume;
					tOrderReqBreak.fPrice_Close = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
					tOrderReqBreak.bIsCloseToday = TRUE;
					tOrderReqBreak.dwVolume_CloseToday = nSumTodayVolume;
					tOrderReqBreak.fPrice_CloseToday = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
				break;
			case 2:	// 按默认手数平仓
			{
				int nTradeVolume = (tOriReq.nDefaultVolume>nSumVolume ? 
						nSumVolume : tOriReq.nDefaultVolume);
				int nTradeTodayVolume = (nTradeVolume>nSumTodayVolume ? 
						nSumTodayVolume : nTradeVolume);
				int nTradeYesterdayVolume = (nTradeVolume>nTradeTodayVolume ? 
						nTradeVolume-nTradeTodayVolume : 0);
				//int nTradeVolume = (tOriReq.nDefaultVolume>nSumVolume ? 
				//		nSumVolume : tOriReq.nDefaultVolume);
				//int nTradeYesterdayVolume = (nTradeVolume>nSumYesterdayVolume ? 
				//		nSumYesterdayVolume : nTradeVolume);
				//int nTradeTodayVolume = (nTradeVolume>nTradeYesterdayVolume ? 
				//		nTradeVolume-nTradeYesterdayVolume : 0);

				if(nTradeYesterdayVolume == 0) {
					tOrderReqBreak.bIsCloseToday = TRUE;
					tOrderReqBreak.dwVolume_CloseToday = nTradeTodayVolume;
					tOrderReqBreak.fPrice_CloseToday = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
				else if(nTradeTodayVolume == 0) {
					tOrderReqBreak.bIsClose = TRUE;
					tOrderReqBreak.dwVolume_Close = nTradeYesterdayVolume;
					tOrderReqBreak.fPrice_Close = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
				else {
					tOrderReqBreak.bIsClose = TRUE;
					tOrderReqBreak.dwVolume_Close = nTradeYesterdayVolume;
					tOrderReqBreak.fPrice_Close = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
					tOrderReqBreak.bIsCloseToday = TRUE;
					tOrderReqBreak.dwVolume_CloseToday = nTradeTodayVolume;
					tOrderReqBreak.fPrice_CloseToday = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
			}
				break;
			case 3:	// 按默认手数平仓，多出部分反向开仓
			{
				int nTradeVolume = (tOriReq.nDefaultVolume>nSumVolume ? 
						nSumVolume : tOriReq.nDefaultVolume);
				int nTradeTodayVolume = (nTradeVolume>nSumTodayVolume ? 
						nSumTodayVolume : nTradeVolume);
				int nTradeYesterdayVolume = (nTradeVolume>nTradeTodayVolume ? 
						nTradeVolume-nTradeTodayVolume : 0);
				int nReverseVolume = tOriReq.nDefaultVolume > nSumVolume ? 
						tOriReq.nDefaultVolume - nSumVolume : 0;
				//int nTradeVolume = (tOriReq.nDefaultVolume>nSumVolume ? 
				//		nSumVolume : tOriReq.nDefaultVolume);
				//int nTradeYesterdayVolume = (nTradeVolume>nSumYesterdayVolume ? 
				//		  nSumYesterdayVolume : nTradeVolume);
				//int nTradeTodayVolume = (nTradeVolume>nTradeYesterdayVolume ? 
				//		nTradeVolume-nTradeYesterdayVolume : 0);
				//int nReverseVolume = tOriReq.nDefaultVolume > nSumVolume ? 
				//		tOriReq.nDefaultVolume - nSumVolume : 0;

				if(nReverseVolume > 0) {
					tOrderReqBreak.bIsReverseOpen = TRUE;
					tOrderReqBreak.dwVolume_ReverseOpen = nReverseVolume;
					tOrderReqBreak.fPrice_ReverseOpen = tOriReq.fPrice;
					//if(tOriReq.bDirection)
					//	tOrderReqBreak.fPrice_ReverseOpen = tOriReq.fBidPrice1 - 
					//			tOriReq.nPriceOffset*tOriReq.fPriceTick;
					//else
					//	tOrderReqBreak.fPrice_ReverseOpen = tOriReq.fAskPrice1 + 
					//			tOriReq.nPriceOffset*tOriReq.fPriceTick;

				}
				if(nTradeYesterdayVolume == 0) {
					tOrderReqBreak.bIsCloseToday = TRUE;
					tOrderReqBreak.dwVolume_CloseToday = nTradeTodayVolume;
					tOrderReqBreak.fPrice_CloseToday = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
				else if(nTradeTodayVolume == 0) {
					tOrderReqBreak.bIsClose = TRUE;
					tOrderReqBreak.dwVolume_Close = nTradeYesterdayVolume;
					tOrderReqBreak.fPrice_Close = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
				else {
					tOrderReqBreak.bIsClose = TRUE;
					tOrderReqBreak.dwVolume_Close = nTradeYesterdayVolume;
					tOrderReqBreak.fPrice_Close = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
					tOrderReqBreak.bIsCloseToday = TRUE;
					tOrderReqBreak.dwVolume_CloseToday = nTradeTodayVolume;
					tOrderReqBreak.fPrice_CloseToday = tOriReq.fPrice;// + 
							//tOriReq.nPriceOffset*tOriReq.fPriceTick;
				}
			}
				break;
			};
		}
	}
	return TRUE;

}

BOOL COrderInsertEntity::InsertOrder(wxString& strInstrumentID,  wxString& strAccount, 
									 BOOL bDirection, int nOCMode, double fPrice, int nVolume, 
									 BOOL bIsHedge, BOOL bIsMarketPrice, BOOL bSimulate, 
									 BOOL bIsParketOrder)
{
	
	int n=m_nRspOrderReqCount;
	string strName = strInstrumentID.c_str();
	string strAcct = strAccount.c_str();
	
	if(bSimulate) {
		//Macro
		AUTOMARKETPRICE2(strName, bDirection, bIsMarketPrice, fPrice)
	}
	
	m_tRspOrderReqs[n].bIsParked = bIsParketOrder;
	if(bIsParketOrder)
		MakeReq_Parked(m_tRspOrderReqs[n].order.parked, strName, strAcct, OrderType_FT_Immediately, 
				bDirection, nOCMode+1, nVolume, fPrice, 0, bIsHedge, bIsMarketPrice, FALSE);
	else
		MakeReq_Order(m_tRspOrderReqs[n].order.insert, strName, strAcct, OrderType_FT_Immediately, 
				bDirection, nOCMode+1, nVolume, fPrice, 0, bIsHedge, bIsMarketPrice, FALSE);
	m_nRspOrderReqCount++;
	
//	int n=m_nRspOrderReqCount;
//
//	m_tRspOrderReqs[n].bIsParked = bIsParketOrder;
//	if(m_tRspOrderReqs[n].bIsParked)
//		MakeReq(strInstrumentID, strAccount,bDirection, nOCMode, fPrice, nVolume, 
//				bIsHedge, bIsMarketPrice, bSimulate, bIsParketOrder, 
//				m_tRspOrderReqs[n].order.parked);
//	else
//		MakeReq(strInstrumentID, strAccount,bDirection, nOCMode, fPrice, nVolume, 
//				bIsHedge, bIsMarketPrice, bSimulate, bIsParketOrder, 
//				m_tRspOrderReqs[n].order.insert);
//	m_nRspOrderReqCount++;
	
	return TRUE;

}

BOOL COrderInsertEntity::DoOrder(int& nErrPos, wxString& strRetMsg)
{
	
	int nRet = -1;
	nErrPos = -1;

	strRetMsg="";

	for(int i=0; i<m_nRspOrderReqCount; ++i) {
		if(m_tRspOrderReqs[i].bIsParked)
        {
            nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqParkedOrderInsert(m_tRspOrderReqs[i].order.parked, DEFAULT_SVR()->GetNextReqID()):-1;
        }
		else
        {
            nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(m_tRspOrderReqs[i].order.insert, DEFAULT_SVR()->GetNextReqID()):-1;
        }
		if(nRet!=0) {
			nErrPos = i;
			strRetMsg.Printf(LOADFORMATSTRING(ORDER_IE_CHANGEFAILMSG,"%d%s"),//文本必须包含%d  %s
					i, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
			strcpy(m_tRspOrderReqs[i].rspInfo.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
			return FALSE;
		}
	}
	strRetMsg = LOADSTRING(ORDER_IE_CHANGEOK);//wxT("下单成功！\n");
	return TRUE;

}

BOOL COrderInsertEntity::RevokeOrder(wxString& strInstrumentID, BOOL bDirection, int nOCMode, wxString& strRetMsg)
{

	// 如果撤单也需要确认后再进行，就把要下的撤单保存到成员变量中，
	// 然后待确认后执行这些保存在成员变量中的撤单，就象自动单一样
	// 判断是否有要撤的单，判读撤单数组里存的撤单数是否大于零即可
    std::vector<PlatformStru_OrderInfo> allOrders;
    int i,count;

	m_nFtdcOrderActionCount = 0;
	ZeroMemory(m_tFtdcOrderActions, sizeof(CThostFtdcInputOrderActionField)*conMaxFtdcOrderActionNum);

	allOrders.clear();
	if(DEFAULT_SVR()&&DEFAULT_SVR()->GetAllOrders(allOrders) > 0) 
    {
        count=allOrders.size();

		for(i=0;i<count;i++) 
        {
            PlatformStru_OrderInfo& field = allOrders[i];
			if(strInstrumentID.compare(field.InstrumentID)==0 && 
					(field.Direction==THOST_FTDC_D_Buy) == bDirection && 
					(field.CombOffsetFlag[0]!=THOST_FTDC_OF_Open) && 
					field.ContingentCondition==THOST_FTDC_CC_Immediately &&
					(field.OrderStatus==THOST_FTDC_OST_PartTradedQueueing || 
					 field.OrderStatus==THOST_FTDC_OST_NoTradeQueueing || 
					 field.OrderStatus==THOST_FTDC_OST_PartTradedNotQueueing ||
					 field.OrderStatus==THOST_FTDC_OST_NoTradeNotQueueing)) {

				CCifcoInputOrderActionField ReqData={0};
				memset(&ReqData, 0, sizeof(CThostFtdcInputOrderActionField));
				
				ReqData.FrontID=field.FrontID;
				ReqData.SessionID=field.SessionID;
				memcpy(ReqData.OrderRef, field.OrderRef, sizeof(field.OrderRef));
				memcpy(ReqData.OrderSysID, field.OrderSysID, sizeof(field.OrderSysID));
				memcpy(ReqData.ExchangeID, field.ExchangeID, sizeof(field.ExchangeID));
				memcpy(ReqData.InstrumentID, field.InstrumentID, sizeof(field.InstrumentID));

				m_tFtdcOrderActions[m_nFtdcOrderActionCount].action.Thost = ReqData;
				m_nFtdcOrderActionCount++;
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

BOOL COrderInsertEntity::DoRevokeOrder(int& nErrPos, wxString& strRetMsg)
{
	
	int nRet = -1;
	nErrPos = -1;

	strRetMsg="";

	for(int i=0; i<m_nFtdcOrderActionCount; ++i) {
        nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderAction(m_tFtdcOrderActions[i].action, DEFAULT_SVR()->GetNextReqID()):-1;
		if(nRet!=0) {
			//COrderServiceThread::sm_nReqNum--;
			nErrPos = i;
			strRetMsg.Printf(LOADFORMATSTRING(ORDER_IE_REVOKEMSG,"%d%s"),//文本必须包含%d %s
					i, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
			strcpy(m_tFtdcOrderActions[i].rspInfo.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
			return FALSE;
		}
	}
	strRetMsg = wxT(LOADSTRING(ORDER_IE_REVOKEOK));//wxT("撤单成功！\n");
	return TRUE;

}

wxString COrderInsertEntity::GetOrderDesciption(int nViewStyle)
{
	wxString strText;
	wxString strItem;

	strText = wxT(LOADSTRING(ORDER_IE_TITLE));//wxT("下单: \n");	
	for(int i=0; i<m_nRspOrderReqCount; ++i) {
		PlatformStru_InputOrder& req = m_tRspOrderReqs[i].order.insert;
		strItem.Printf(LOADFORMATSTRING(ORDER_IE_CHANGEMSG,"%s%s%s%s%s%d%s"),//文本必须包含%s %s %s %s %s %d  %s 
				req.OrderPriceType==THOST_FTDC_OPT_AnyPrice ? LOADSTRING(OLS_MARKET) : Double2String(req.LimitPrice).c_str(),
				m_bIsAuto ? LOADSTRING(PARK_AUTO) : wxEmptyString,//_("自动")
				req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY):LOADSTRING(OLS_SELL),
				req.InstrumentID, 
				req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ?LOADSTRING(OLS_OPEN): 
				(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)),
				req.VolumeTotalOriginal,
				req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ?LOADSTRING(OLS_HEDGE) : LOADSTRING(OLS_SPECULATE));
		strText += strItem;
	}
	strText += wxT(LOADSTRING(ORDER_IE_CHANGECOMFIRM));//wxT("是否确认?");

	return strText;
}

BOOL COrderInsertEntity::MarkRevokeItemState(DataRtnOrder& field)
{
	wxString strTmpSysID;
	wxString strInSysID = field.OrderField.OrderSysID;
	
	strInSysID.Trim(false);
	for(int i=0; i<m_nFtdcOrderActionCount; i++) {
		strTmpSysID = m_tFtdcOrderActions[i].action.Thost.OrderSysID;
		strTmpSysID.Trim(false);
		if(strTmpSysID.compare(strInSysID)==0 && field.OrderField.OrderStatus==THOST_FTDC_OST_Canceled) {
			m_bFtdcOrderActionsState[i]=TRUE;
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL COrderInsertEntity::FindRspOrderAction(DataRspOrderAction2& act)
{
	wxString strTmpSysID;
	wxString strInSysID = act.OrderActionField.OrderSysID;
	
	strInSysID.Trim(false);
	for(int i=0; i<m_nFtdcOrderActionCount; i++) {
		strTmpSysID = m_tFtdcOrderActions[i].action.Thost.OrderSysID;
		strTmpSysID.Trim(false);
		if(strTmpSysID.compare(strInSysID)==0) {
			m_bFtdcOrderActionsState[i]=FALSE;
			return TRUE;
		}
	}
	
	return FALSE;
}
