#include "StdAfx.h"
#include "COrderChangeEntity.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Order-Common/COrderServiceThread.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

using namespace util;

//////////////////////////////////////////////////////////////////////////////////////


COrderChangeEntity::COrderChangeEntity()
{
	
//	m_nReverseTimer = 3;
	m_bIsAuto = FALSE;
	m_nRspOrderReqCount = 0;
	ZeroMemory(m_bFtdcOrderActionsState, sizeof(BOOL)*conMaxRspOrderReqNum);
	ZeroMemory(m_tRspOrderReqs, sizeof(DataRspOrderInsert)*conMaxRspOrderReqNum);
	m_nFtdcOrderActionCount = 0;
	ZeroMemory(m_tFtdcOrderActions, sizeof(CThostFtdcInputOrderActionField)*conMaxFtdcOrderActionNum);

}

COrderChangeEntity::~COrderChangeEntity()
{
}

COrderChangeEntity* COrderChangeEntity::CreateOrderChange(ORIORDERREQ& tOriReq)
{
	
	wxString strInstrumentID(tOriReq.strInstrumentID);
	
	COrderChangeEntity* pEntity = new COrderChangeEntity();
	//pEntity->m_bIsAuto = tOriReq.bIsAuto;
	
	wxString strAccount(tOriReq.strAccount);
	pEntity->InsertOrder(strInstrumentID, strAccount, tOriReq.bDirection, tOriReq.nOCMode, 
		tOriReq.fPrice, tOriReq.nVolume, tOriReq.bIsHedge, 
		tOriReq.bIsMarketPrice, tOriReq.bSimulate, tOriReq.bIsParketOrder);

	return pEntity;
	
}

BOOL COrderChangeEntity::InsertOrder(wxString& strInstrumentID, wxString& strAccount, 
									 BOOL bDirection, int nOCMode, double fPrice, int nVolume, 
									 BOOL bIsHedge, BOOL bIsMarketPrice, BOOL bSimulate, BOOL bIsParketOrder)
{
	
	int n=m_nRspOrderReqCount;
	string strName = strInstrumentID.c_str();
	string strAcct = strAccount.c_str();

	MakeReq_Order(m_tRspOrderReqs[n].insert, strName, strAcct, OrderType_FT_Immediately, 
			bDirection, nOCMode+1, nVolume, fPrice, 0, bIsHedge, bIsMarketPrice, FALSE);
	m_nRspOrderReqCount++;
	
	//int n=m_nRspOrderReqCount;

	//MakeReq(strInstrumentID, strAccount, bDirection, nOCMode, fPrice, nVolume, 
	//		bIsHedge, bIsMarketPrice, bSimulate, bIsParketOrder, 
	//		m_tRspOrderReqs[n].insert);
	//m_nRspOrderReqCount++;
	return TRUE;

}

BOOL COrderChangeEntity::DoOrder(int& nErrPos, wxString& strRetMsg)
{
	
	int nRet = -1;
	nErrPos = -1;

	strRetMsg="";

	for(int i=0; i<m_nRspOrderReqCount; ++i) {
        nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(m_tRspOrderReqs[i].insert, DEFAULT_SVR()->GetNextReqID()):-1;
		if(nRet!=0) {
			nErrPos = i;
			strRetMsg.Printf(LOADFORMATSTRING(ORDER_CE_CHANGEFAILMSG,"%d%s"),//文本必须包含%d  %s
					i, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
			//strcpy(m_tRspOrderReqs[i].RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet).c_str());
			return FALSE;
		}
	}
	strRetMsg = wxT(LOADSTRING(ORDER_CE_CHANGEOK));//wxT("下单成功！\n");
	return TRUE;

}

BOOL COrderChangeEntity::RevokeOrder(PlatformStru_OrderInfo& ftdcOrderField, wxString& strRetMsg)
{

	// 如果撤单也需要确认后再进行，就把要下的撤单保存到成员变量中，
	// 然后待确认后执行这些保存在成员变量中的撤单，就象自动单一样
	// 判断是否有要撤的单，判读撤单数组里存的撤单数是否大于零即可

	m_nFtdcOrderActionCount = 0;
	ZeroMemory(m_tFtdcOrderActions, sizeof(PlatformStru_InputOrderAction)*conMaxFtdcOrderActionNum);

	PlatformStru_OrderInfo& field = ftdcOrderField;

	PlatformStru_InputOrderAction ReqData={0};
	memset(&ReqData, 0, sizeof(PlatformStru_InputOrderAction));
	
	ReqData.Thost.FrontID=field.FrontID;
	ReqData.Thost.SessionID=field.SessionID;
	memcpy(ReqData.Thost.OrderRef, field.OrderRef, sizeof(field.OrderRef));
	memcpy(ReqData.Thost.OrderSysID, field.OrderSysID, sizeof(field.OrderSysID));
	memcpy(ReqData.Thost.ExchangeID, field.ExchangeID, sizeof(field.ExchangeID));
	memcpy(ReqData.Thost.InstrumentID, field.InstrumentID, sizeof(field.InstrumentID));

	m_tFtdcOrderActions[m_nFtdcOrderActionCount] = ReqData;
	m_nFtdcOrderActionCount++;
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
		strRetMsg += strValue;
	}
	m_strRevokeMsg = strRetMsg;

	return TRUE;
}

BOOL COrderChangeEntity::DoRevokeOrder(int& nErrPos, wxString& strRetMsg)
{
	
	int nRet = -1;
	nErrPos = -1;

	strRetMsg="";

	for(int i=0; i<m_nFtdcOrderActionCount; ++i) {
        nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderAction(m_tFtdcOrderActions[i], DEFAULT_SVR()->GetNextReqID()):-1;
		if(nRet!=0) {
			//COrderServiceThread::sm_nReqNum--;
			nErrPos = i;
			strRetMsg.Printf(LOADFORMATSTRING(ORDER_CE_REVOKEMSG,"%d%s"),//文本必须包含%d  %s
					i, CFieldValueTool::ReturnCode2String(nRet,SVR_LANGUAGE).c_str());
			//strcpy(m_tRspOrderReqs[i].RspInfoField.ErrorMsg, CFieldValueTool::ReturnCode2String(nRet).c_str());
			return FALSE;
		}
	}
	strRetMsg = wxT(LOADSTRING(ORDER_CE_REVOKEOK));//wxT("撤单成功！\n");
	return TRUE;

}

wxString COrderChangeEntity::GetOrderDesciption(int nViewStyle)
{
	wxString strText;
	wxString strItem;

	strText = LOADSTRING(NEW_ORDER_F);	
	for(int i=0; i<m_nRspOrderReqCount; ++i) {
		PlatformStru_InputOrder& req = m_tRspOrderReqs[i].insert;
		strItem.Printf(LOADFORMATSTRING(ORDER_CE_CHANGEMSG,"%s%s%s%s%s%d%s"),//文本必须包含%s %s %s %s %s %d  %s
			Double2String(req.LimitPrice).c_str(),
				m_bIsAuto ? LOADSTRING(PARK_AUTO): wxEmptyString,
				req.Direction==THOST_FTDC_D_Buy ? LOADSTRING(OLS_BUY) :LOADSTRING(OLS_SELL),
				req.InstrumentID, 
				req.CombOffsetFlag[0]==THOST_FTDC_OF_Open ? LOADSTRING(OLS_OPEN): 
				(req.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday ? LOADSTRING(OLS_CLOSE_TODAY) : LOADSTRING(OLS_CLOSE)),
				req.VolumeTotalOriginal,
				req.CombHedgeFlag[0]==THOST_FTDC_HF_Hedge ?LOADSTRING(OLS_HEDGE):LOADSTRING(OLS_SPECULATE)  );
		strText += strItem;
	}
	strText += wxT(LOADSTRING(ORDER_CE_CHANGECOMFIRM));//wxT("是否确认?");

	return strText;
}

BOOL COrderChangeEntity::MarkRevokeItemState(DataRtnOrder& field)
{
	wxString strTmpSysID;
	wxString strInSysID = field.OrderField.OrderSysID;
	
	strInSysID.Trim(false);
	for(int i=0; i<m_nFtdcOrderActionCount; i++) {
		strTmpSysID = m_tFtdcOrderActions[i].Thost.OrderSysID;
		strTmpSysID.Trim(false);
		if(strTmpSysID.compare(strInSysID)==0 && field.OrderField.OrderStatus==THOST_FTDC_OST_Canceled) {
			m_bFtdcOrderActionsState[i]=TRUE;
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL COrderChangeEntity::FindRspOrderAction(DataRspOrderAction2& act)
{
	wxString strTmpSysID;
	wxString strInSysID = act.OrderActionField.OrderSysID;
	
	strInSysID.Trim(false);
	for(int i=0; i<m_nFtdcOrderActionCount; i++) {
		strTmpSysID = m_tFtdcOrderActions[i].Thost.OrderSysID;
		strTmpSysID.Trim(false);
		if(strTmpSysID.compare(strInSysID)==0) {
			m_bFtdcOrderActionsState[i]=FALSE;
			return TRUE;
		}
	}
	
	return FALSE;
}
