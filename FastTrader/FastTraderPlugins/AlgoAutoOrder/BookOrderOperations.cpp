// BookOrderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "BookOrderOperations.h"
#include "SequenceManager.h"
#include "Module-Misc2/tools_util.h"


CBookOrderOperations::CBookOrderOperations(UINT nIDTemplate, CWnd* pParent/* = NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	ZeroMemory(&m_tPriceGridIndex, sizeof(m_tPriceGridIndex));
	ZeroMemory(&m_tOldPriceGridIndex, sizeof(m_tOldPriceGridIndex));
	ZeroMemory(&m_tAccountInfo, sizeof(m_tAccountInfo));
	ZeroMemory(&m_tInstrumentInfo, sizeof(m_tInstrumentInfo));
	m_nPositionCount = 0;
	//ZeroMemory(m_tPositions, sizeof(m_tPositions));
	ZeroMemory(&m_tDepthMarketData, sizeof(m_tDepthMarketData));
	ZeroMemory(&m_bookOrderDlgColor, sizeof(m_bookOrderDlgColor));
	m_nDlgMaxWidth = 40;
	m_ndlgMaxHeight = 20;
	m_bExiting = false;
	m_bNeedInstrumentIDInputPanel = true;
	m_bNeedQtyInputPanel = true;
	m_bUserChangeInstrumentID = true;
	m_nQueryOrderLastSeqNum = -1;
	m_nColorGradientIndex = -1;
	m_dwColorFirst = 0;
	m_dwColorEnd = 0;
	ZeroMemory(&m_tWaitOrderPrice, sizeof(m_tWaitOrderPrice));
	m_tWaitOrderPrice.dHighPrice = util::GetDoubleInvalidValue();
	m_tWaitOrderPrice.dLowPrice = util::GetDoubleInvalidValue();
	ZeroMemory(&m_tOldWaitOrderPrice, sizeof(m_tOldWaitOrderPrice));
	m_tOldWaitOrderPrice.dHighPrice = util::GetDoubleInvalidValue();
	m_tOldWaitOrderPrice.dLowPrice = util::GetDoubleInvalidValue();
}

CBookOrderOperations::~CBookOrderOperations()
{
}

void CBookOrderOperations::RecomputePositionProfit()
{
	for(int i=0; i<m_nPositionCount; i++)
	{
		double fPriceDiff = (m_tPositions[i].PosiDirection==THOST_FTDC_PD_Long ? 1.0 : -1.0) 
				* (m_tDepthMarketData.LastPrice - m_tPositions[i].AveragePositionPrice);
		m_tPositions[i].PositionProfit = fPriceDiff * m_tPositions[i].Position * m_tInstrumentInfo.VolumeMultiple;
	}
}

void CBookOrderOperations::QueryCurrUserID()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ACCOUNT_CURRENT_ACCOUNT_QUERY, NULL, 0, nSeqNum);
	}
}

void CBookOrderOperations::QueryCurrAccountID()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_QUERY, NULL, 0, nSeqNum);
	}
}

void CBookOrderOperations::QueryCurrAccountInfo(CString& strCurrAccount)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szSubAccountID[sizeof(TThostFtdcAccountIDType)];
		ZeroMemory(szSubAccountID, sizeof(szSubAccountID));
		strncpy_s(szSubAccountID, strCurrAccount, sizeof(szSubAccountID)<strCurrAccount.GetLength() 
				? sizeof(szSubAccountID) : strCurrAccount.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ACCOUNT_SUB_ACCOUNT_INFO_QUERY, szSubAccountID, 
				sizeof(szSubAccountID), nSeqNum);
	}
}

void CBookOrderOperations::QueryInstrumentInfo(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy_s(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)<strInstrumentID.GetLength() 
				? sizeof(szInstrumentID) : strInstrumentID.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_INSTRUMENT_INFO_QUERY, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CBookOrderOperations::QueryPositionInfo(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		PlusinAcctInstrumentKey tKey;
		ZeroMemory(&tKey, sizeof(tKey));
		strncpy_s(tKey.szAccountID, m_strCurrAccount, sizeof(tKey.szAccountID));
		strncpy_s(tKey.szSubAccountID, m_strCurrSubAccount, sizeof(tKey.szSubAccountID));
		strncpy_s(tKey.szInstrumentID, strInstrumentID, sizeof(tKey.szInstrumentID));
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_POSITION_INFO_LIST_QUERY, &tKey, 
				sizeof(tKey), nSeqNum);
	}
}

int CBookOrderOperations::QueryOrderWaitVolume(CString& strInstrumentID, double fLimitPrice, char cDirection)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		//TRACE("CBookOrderDialog::QueryOrderWaitVolume(%s, %f, [%c])\n", 
		//		strInstrumentID, fLimitPrice, cDirection);
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		PlusinOpenOrderKey tKey;
		ZeroMemory(&tKey, sizeof(tKey));
		strncpy_s(tKey.szAccountID, m_strCurrAccount, sizeof(tKey.szAccountID));
		strncpy_s(tKey.szSubAccountID, m_strCurrSubAccount, sizeof(tKey.szSubAccountID));
		strncpy_s(tKey.szInstrumentID, strInstrumentID, sizeof(tKey.szInstrumentID));
		tKey.cDirection = cDirection;
		tKey.dLimitPrice = fLimitPrice;
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ALL_ORDER_WAIT_ORDER_NUM_QUERY, &tKey, 
				sizeof(tKey), nSeqNum);
		return nSeqNum;
	}
	return -1;
}

void CBookOrderOperations::QueryOrderWaitVolumeList(CString& strInstrumentID, char cDirection, BOOL bClearWaitMap)
{
	double fTopPrice = m_tPriceGridIndex.fTopPrice;
	double fCurrPrice = 0.0;
	int nRetSeqNum = 0;
	CString strText;

	if(bClearWaitMap)
		m_mapWaitRspOrderWait.clear();
	for(int i=0; i<m_tPriceGridIndex.nGridRowCount; i++)
	{
		fCurrPrice = fTopPrice - m_tPriceGridIndex.fPriceTick * i;
		strText = Price2String(fCurrPrice, m_tPriceGridIndex.fPriceTick);
		nRetSeqNum = QueryOrderWaitVolume(strInstrumentID, fCurrPrice, cDirection);
		if(nRetSeqNum>0)
		{
			m_mapWaitRspOrderWait.insert(std::pair<int, CString>(nRetSeqNum, strText));
		}
	}
}

void CBookOrderOperations::QueryOrderWaitMaxMinPrice(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		//TRACE("CBookOrderDialog::QueryOrderWaitVolume(%s, %f, [%c])\n", 
		//		strInstrumentID, fLimitPrice, cDirection);
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		PlusinAcctInstrumentKey tKey;
		ZeroMemory(&tKey, sizeof(tKey));
		strncpy_s(tKey.szAccountID, m_strCurrAccount, sizeof(tKey.szAccountID));
		strncpy_s(tKey.szSubAccountID, m_strCurrSubAccount, sizeof(tKey.szSubAccountID));
		strncpy_s(tKey.szInstrumentID, strInstrumentID, sizeof(tKey.szInstrumentID));
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ALL_ORDER_WAIT_ORDER_PRICE_QUERY, &tKey, 
				sizeof(tKey), nSeqNum);
	}
}

int CBookOrderOperations::QueryOrderInfo(int nFTID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ALL_ORDER_INFO_QUERY1, &nFTID, sizeof(int), nSeqNum);
		return nSeqNum;
	}
	return -1;
}

void CBookOrderOperations::QueryOrderWaits()
{
	m_mapOrdersForCancel.clear();
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ALL_ORDER_LIST_QUERY, NULL, 0, nSeqNum);
	}
}

void CBookOrderOperations::QueryQuotSnapshot(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy_s(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)<strInstrumentID.GetLength() 
				? sizeof(szInstrumentID) : strInstrumentID.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_QUOTE_SNAPSHOT_QUERY, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CBookOrderOperations::SubscribeQuot(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy_s(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)<strInstrumentID.GetLength() 
				? sizeof(szInstrumentID) : strInstrumentID.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_QUOTE_SUBSCRIBE, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CBookOrderOperations::UnsubscribeQuot(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy_s(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)<strInstrumentID.GetLength() 
				? sizeof(szInstrumentID) : strInstrumentID.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_QUOTE_UNSUBSCRIBE, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CBookOrderOperations::QueryConfig_InstrumentIDInputPanelShow()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_QUERY, NULL, 
			0, nSeqNum);
	}
}

void CBookOrderOperations::QueryConfig_QtyInputPanelShow()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_QUERY, NULL, 
			0, nSeqNum);
	}
}

void CBookOrderOperations::QueryConfig_QtyInputPanelQtyDatas()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_CONFIG_INPUT_VOLUMES_QUERY, NULL, 
			0, nSeqNum);
	}
}

void CBookOrderOperations::NewOrder(CString& strInstrumentID, BOOL bDirection, int nOCMode, 
									double fLimitPrice, int nVolume)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		PlatformStru_InputOrder tInputOrder;
		ZeroMemory(&tInputOrder, sizeof(tInputOrder));
		MakeReq_Order(tInputOrder, strInstrumentID, m_strCurrSubAccount, OrderType_FT_Immediately, 
				bDirection, nOCMode, nVolume, fLimitPrice, 0, FALSE, FALSE, FALSE);
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ORDER_INSERT_ORDER, &tInputOrder, 
			sizeof(tInputOrder), nSeqNum);
	}
}

void CBookOrderOperations::CancelOrder(CString& strInstrumentID, BOOL bDirection, double fLimitPrice)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		PlusinOpenOrderKey tKey;
		
		ZeroMemory(&tKey, sizeof(tKey));
		strncpy_s(tKey.szAccountID, m_strCurrAccount, sizeof(tKey.szAccountID));
		strncpy_s(tKey.szSubAccountID, m_strCurrSubAccount, sizeof(tKey.szSubAccountID));
		strncpy_s(tKey.szInstrumentID, strInstrumentID, sizeof(tKey.szInstrumentID));
		tKey.cDirection = bDirection ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
		tKey.dLimitPrice = fLimitPrice;
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ORDER_CANCEL_ORDER_BY_INSTLMTDIRPRICE, &tKey, 
			sizeof(tKey), nSeqNum);
	}
}

void CBookOrderOperations::CancelAllOrder(CString& strInstrumentID)
{
    PlusinAPI* poPlusinApi = GetPlusinAPI();
    if(poPlusinApi!=NULL) 
    {
        int nSeqNum = CSequenceManager::GetNextSeqNum();
        PlusinAcctInstrumentKey tKey;
        ZeroMemory(&tKey, sizeof(tKey));
        strncpy_s(tKey.szAccountID, m_strCurrAccount, sizeof(tKey.szAccountID)-1);
        strncpy_s(tKey.szSubAccountID, m_strCurrSubAccount, sizeof(tKey.szSubAccountID)-1);
        strncpy_s(tKey.szInstrumentID, strInstrumentID, sizeof(tKey.szInstrumentID)-1);
        LockGlobalObject();
        CSequenceManager& objSeqMng = GetSequenceManager();
        objSeqMng.RegisterSeqNum(nSeqNum, this);
        UnlockGlobalObject();
        poPlusinApi->Send(CMDID_ORDER_CANCEL_ORDER_BY_INST, &tKey, 
            sizeof(tKey), nSeqNum);
    }
}

void CBookOrderOperations::CancelOrder(PlatformStru_InputOrderAction& tAction)
{
    PlusinAPI* poPlusinApi = GetPlusinAPI();
    if(poPlusinApi!=NULL) 
    {
        int nSeqNum = CSequenceManager::GetNextSeqNum();
        //TRACE("CancelOrder nSqNum = %d\n",nSeqNum);
        CSequenceManager& objSeqMng = GetSequenceManager();
        objSeqMng.RegisterSeqNum(nSeqNum, this);
        poPlusinApi->Send(CMDID_ORDER_CANCEL_ORDER, &tAction, sizeof(tAction), nSeqNum);
    }
}

void CBookOrderOperations::CancelOrder(PlatformStru_OrderInfo& tInfo)
{
    PlusinAPI* poPlusinApi = GetPlusinAPI();
    if(poPlusinApi!=NULL) 
    {
        int nSeqNum = CSequenceManager::GetNextSeqNum();
        //TRACE("CancelOrder nSqNum = %d\n",nSeqNum);
        PlatformStru_InputOrderAction tAction;
        ZeroMemory(&tAction, sizeof(tAction));
        tAction.Thost.FrontID=tInfo.FrontID;
        tAction.Thost.SessionID=tInfo.SessionID;
        memcpy(tAction.Thost.OrderRef, tInfo.OrderRef, sizeof(tInfo.OrderRef));
        memcpy(tAction.Thost.OrderSysID, tInfo.OrderSysID, sizeof(tInfo.OrderSysID));
        memcpy(tAction.Thost.ExchangeID, tInfo.ExchangeID, sizeof(tInfo.ExchangeID));
        memcpy(tAction.Thost.InstrumentID, tInfo.InstrumentID, sizeof(tInfo.InstrumentID));
        //LockGlobalObject();
        CSequenceManager& objSeqMng = GetSequenceManager();
        objSeqMng.RegisterSeqNum(nSeqNum, this);
        //UnlockGlobalObject();
        poPlusinApi->Send(CMDID_ORDER_CANCEL_ORDER, &tAction, sizeof(tAction), nSeqNum);
    }
}

void CBookOrderOperations::ChangeOrder(CString& strInstrumentID, BOOL bDirection, int nOCMode, 
									   double fLimitPrice, int nVolume)
{
	CancelOrder(strInstrumentID, bDirection, util::GetDoubleInvalidValue());

	PlatformStru_InputOrder tInputOrder;
	ZeroMemory(&tInputOrder, sizeof(tInputOrder));
	MakeReq_Order(tInputOrder, strInstrumentID, m_strCurrSubAccount, OrderType_FT_Immediately, 
			bDirection, nOCMode, nVolume, fLimitPrice, 0, FALSE, FALSE, FALSE);

// 	GetChangeOrderManager().ResetChangeOrder(m_strCurrAccount, m_strCurrSubAccount, strInstrumentID, 
// 			(bDirection ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell), tInputOrder, this);
}

