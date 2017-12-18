// BookOrderDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TSSCtrlPlugin.h"
#include "TSSCtrlPluginOperations.h"
#include "SequenceManager.h"
#include "Module-Misc/tools_util.h"
#include "ConfigMgr.h"


CTSSCtrlPluginOperations::CTSSCtrlPluginOperations(UINT nIDTemplate, CWnd* pParent/* = NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_nRowID_Plot = 0;
	m_nRowID_Position = 0;
	m_nRowID_PosiDetail = 0;
	m_dwPreCloseTime = 0;
	
	m_bHasLoseOverLimit_AcctMax = false;
	m_dwLoseOLimitStart_AcctMax = 0;
	m_bHasLoseRegress_AcctMax = false;
	m_dwLoseRegressStart_AcctMax = 0;

	//m_nPositionCount = 0;
	//ZeroMemory(m_tPositions, sizeof(m_tPositions));
	m_bExiting = false;
    m_bHasDayLostMax = false;
	m_nLastFTID4Cancel = -1;
	m_nLastSeqNum4OpenVal = -1;
}

CTSSCtrlPluginOperations::~CTSSCtrlPluginOperations()
{
}

BOOL CTSSCtrlPluginOperations::GetInstPlot(std::string& strInstrumentID, PLOTCTRLITEMCFG& item)
{
	MAPINSTCFG::iterator it;
	it = m_mapInstCfg.find(strInstrumentID);
	if(it != m_mapInstCfg.end()) {
		item = it->second;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CTSSCtrlPluginOperations::GetLastPrice(std::string& strInstrumentID, double& fLastPrice)
{
	MAPQUOT::iterator it;
	it = m_mapQuot.find(strInstrumentID);
	if(it != m_mapQuot.end()) {
		fLastPrice = it->second.LastPrice;
		return TRUE;
	}
	else {
		fLastPrice = 0.0;
		return FALSE;
	}
}

BOOL CTSSCtrlPluginOperations::GetInstPriceTick(std::string& strInstrumentID, double& fPriceTick)
{
	MAPINSTINFO::iterator it;
	it = m_mapInstInfo.find(strInstrumentID);
	if(it != m_mapInstInfo.end()) {
		fPriceTick = it->second.PriceTick;
		return TRUE;
	}
	else {
		fPriceTick = 0.0;
		return FALSE;
	}
}

int CTSSCtrlPluginOperations::GetPosiHoldingTime(std::string& strInstrumentID, int nTime)
{
	MAPINSTINFO::iterator it = m_mapInstInfo.find(strInstrumentID);
	if(it != m_mapInstInfo.end()) {
		std::string strTextID(it->second.ExchangeID);
		std::string strExchangeID = _strlwr((char*)(strTextID.c_str()));
		map<string, vector<EXCHANGERESTTIME>> mapRestTime = CConfigManager::GetExchangeRestTime();
		map<string, vector<EXCHANGERESTTIME>>::iterator it = mapRestTime.find(strExchangeID);
		if(it != mapRestTime.end()) {
			int nSubTime = 0;
			for(int i=0; i < it->second.size(); i++) {
				if(nTime < it->second[i].nStart)
					break;
				if(nTime < it->second[i].nEnd) {
					int nSub = nTime - it->second[i].nStart;
					nSubTime += nSub;
					break;
				}
				nSubTime += it->second[i].nSubNum;
			}
			nTime -= nSubTime;
		}
		return nTime;
	}
	else {
		return nTime;
	}
}

BOOL CTSSCtrlPluginOperations::IsInRestTime(std::string& strInstrumentID, int nTime)
{
	MAPINSTINFO::iterator it = m_mapInstInfo.find(strInstrumentID);
	if(it != m_mapInstInfo.end()) {
		std::string strTextID(it->second.ExchangeID);
		std::string strExchangeID = _strlwr((char*)(strTextID.c_str()));
		map<string, vector<EXCHANGERESTTIME>> mapRestTime = CConfigManager::GetExchangeRestTime();
		map<string, vector<EXCHANGERESTTIME>>::iterator it = mapRestTime.find(strExchangeID);
		if(it != mapRestTime.end()) {
			int nSubTime = 0;
			for(int i=0; i < it->second.size(); i++) {
				if(nTime >= it->second[i].nStart && nTime <= it->second[i].nEnd)
					return TRUE;
			}
		}
		return FALSE;
	}
	else 
		return FALSE;
}

int CTSSCtrlPluginOperations::GetRemainTime(std::string& strInstrumentID, std::string& strOpenTime)
{
	int iHour=0;
	int iMin=0;
	int iSec=0;
	sscanf_s(strOpenTime.c_str(), "%02d:%02d:%02d", &iHour, &iMin, &iSec);
	int nOpenTime = iHour*3600+iMin*60+iSec;
	SYSTEMTIME currTime;
	::GetLocalTime(&currTime);
	int nCurrTime = currTime.wHour*3600+currTime.wMinute*60+currTime.wSecond;
	int nCanHTime = GetInstCanHandleTime(strInstrumentID);
	int nCurrHTime = GetPosiHoldingTime(strInstrumentID, nCurrTime);
	int nHoldingTime = GetPosiHoldingTime(strInstrumentID, nOpenTime);
	return nCanHTime - (nCurrHTime - nHoldingTime +1);
	//return (nCurrHTime - nHoldingTime +1);
}

int CTSSCtrlPluginOperations::GetRemainTime(std::string& strOpenTime)
{
	int iHour=0;
	int iMin=0;
	int iSec=0;
	sscanf_s(strOpenTime.c_str(), "%02d:%02d:%02d", &iHour, &iMin, &iSec);
	int nOpenTime = iHour*3600+iMin*60+iSec;
	SYSTEMTIME currTime;
	::GetLocalTime(&currTime);
	int nCurrTime = currTime.wHour*3600+currTime.wMinute*60+currTime.wSecond;
	return nCurrTime-nOpenTime+1;
}

int CTSSCtrlPluginOperations::GetInstCanHandleTime(std::string& strInstrumentID)
{
	MAPINSTCFG::iterator it;
	it = m_mapInstCfg.find(strInstrumentID);
	if(it != m_mapInstCfg.end()) {
		return it->second.singlePosiTime;
	}
	else {
		return 0;
		//return 0x7fffffff;
	}
}

void CTSSCtrlPluginOperations::QueryInstrumentInfo(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)<strInstrumentID.GetLength() 
				? sizeof(szInstrumentID) : strInstrumentID.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_INSTRUMENT_INFO_QUERY, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CTSSCtrlPluginOperations::QueryCurrAccountID()
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

void CTSSCtrlPluginOperations::QueryCurrSubAccountID()
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

void CTSSCtrlPluginOperations::QueryCurrAccountInfo(CString& strCurrAccount)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szSubAccountID[sizeof(TThostFtdcAccountIDType)];
		ZeroMemory(szSubAccountID, sizeof(szSubAccountID));
		strncpy(szSubAccountID, strCurrAccount, sizeof(szSubAccountID)<strCurrAccount.GetLength() 
				? sizeof(szSubAccountID) : strCurrAccount.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ACCOUNT_SUB_ACCOUNT_INFO_QUERY, szSubAccountID, 
				sizeof(szSubAccountID), nSeqNum);
	}
}

void CTSSCtrlPluginOperations::QueryPositionInfo(CString& strCurrAccount)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szSubAccountID[sizeof(CommonAccountType)];
		ZeroMemory(szSubAccountID, sizeof(szSubAccountID));
		strncpy(szSubAccountID, strCurrAccount, sizeof(szSubAccountID)<strCurrAccount.GetLength() 
				? sizeof(szSubAccountID) : strCurrAccount.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ACCT_POSITION_INFO_LIST_QUERY, szSubAccountID, 
				sizeof(szSubAccountID), nSeqNum);
	}
}

void CTSSCtrlPluginOperations::QueryPosiDetailInfo(CString& strCurrAccount)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szSubAccountID[sizeof(CommonAccountType)];
		ZeroMemory(szSubAccountID, sizeof(szSubAccountID));
		strncpy(szSubAccountID, strCurrAccount, sizeof(szSubAccountID)<strCurrAccount.GetLength() 
				? sizeof(szSubAccountID) : strCurrAccount.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ACCT_POSITION_DETAIL_LIST_QUERY, szSubAccountID, 
				sizeof(szSubAccountID), nSeqNum);
	}
}

int CTSSCtrlPluginOperations::QueryOpenOrderVol(CString& strCurrAccount, CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		PlusinAcctInstrumentKey key;
		ZeroMemory(&key, sizeof(key));
		strncpy(key.szAccountID, strCurrAccount, sizeof(key.szAccountID)-1);
		strncpy(key.szInstrumentID, strInstrumentID, sizeof(key.szInstrumentID)-1);
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_QUERY, &key, 
				sizeof(key), nSeqNum);
		return nSeqNum;
	}
	else
		return -1;
}

void CTSSCtrlPluginOperations::QueryOpenOrderVol(CString& strCurrAccount)
{
	MAPINSTCTRL::iterator it;
	set<string> setInst;
	for(it = m_mapInstCtrl.begin(); it != m_mapInstCtrl.end(); it++) {
		setInst.insert(string(it->first));
	}
	set<string>::iterator itInst;
	for(itInst = setInst.begin(); itInst != setInst.end(); itInst++) {
		m_nLastSeqNum4OpenVal = QueryOpenOrderVol(strCurrAccount, CString((*itInst).c_str()));
	}
}

void CTSSCtrlPluginOperations::QueryPosiDetailOpenTime(CString& strCurrAccount)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szSubAccountID[sizeof(CommonAccountType)];
		ZeroMemory(szSubAccountID, sizeof(szSubAccountID));
		strncpy(szSubAccountID, strCurrAccount, sizeof(szSubAccountID)<strCurrAccount.GetLength() 
				? sizeof(szSubAccountID) : strCurrAccount.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_ALL_POSITION_DETAIL_OPEN_TIME_LIST_QUERY, szSubAccountID, 
				sizeof(szSubAccountID), nSeqNum);
		//ShowMsgLog(CString("QueryPosiDetailOpenTime()"));
	}
}

void CTSSCtrlPluginOperations::QueryQuotSnapshot(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)<strInstrumentID.GetLength() 
				? sizeof(szInstrumentID) : strInstrumentID.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_QUOTE_SNAPSHOT_QUERY, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CTSSCtrlPluginOperations::QueryAllOrderFTIDList()
{
    PlusinAPI* poPlusinApi = GetPlusinAPI();
    if(poPlusinApi!=NULL) 
    {
        int nSeqNum = CSequenceManager::GetNextSeqNum();
        LockGlobalObject();
        CSequenceManager& objSeqMng = GetSequenceManager();
        objSeqMng.RegisterSeqNum(nSeqNum, this);
        UnlockGlobalObject();
        poPlusinApi->Send(CMDID_ALL_ORDER_LIST_QUERY, NULL,0, nSeqNum);
    }
}

void CTSSCtrlPluginOperations::QueryOrderInfoByFTID(int& nID)
{
    PlusinAPI* poPlusinApi = GetPlusinAPI();
    if(poPlusinApi!=NULL) 
    {
        int nSeqNum = CSequenceManager::GetNextSeqNum();
        LockGlobalObject();
        CSequenceManager& objSeqMng = GetSequenceManager();
        objSeqMng.RegisterSeqNum(nSeqNum, this);
        UnlockGlobalObject();
        poPlusinApi->Send(CMDID_ALL_ORDER_INFO_QUERY1, &nID,sizeof(int), nSeqNum);
    }
}

void CTSSCtrlPluginOperations::SubscribeQuot(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)<strInstrumentID.GetLength() 
				? sizeof(szInstrumentID) : strInstrumentID.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_QUOTE_SUBSCRIBE, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CTSSCtrlPluginOperations::UnsubscribeQuot(CString& strInstrumentID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		char szInstrumentID[sizeof(InstrumentIDType)];
		ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
		strncpy(szInstrumentID, strInstrumentID, sizeof(szInstrumentID)<strInstrumentID.GetLength() 
				? sizeof(szInstrumentID) : strInstrumentID.GetLength());
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_QUOTE_UNSUBSCRIBE, szInstrumentID, 
			sizeof(szInstrumentID), nSeqNum);
	}
}

void CTSSCtrlPluginOperations::ForceOrderVerify()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		LockGlobalObject();
		CSequenceManager& objSeqMng = GetSequenceManager();
		objSeqMng.RegisterSeqNum(nSeqNum, this);
		UnlockGlobalObject();
		poPlusinApi->Send(CMDID_INSERT_ORDER_VERIFY_OPEN_QUERY, NULL, 0, nSeqNum);
	}
}

void CTSSCtrlPluginOperations::NewOrder(CString& strInstrumentID, BOOL bDirection, int nOCMode, 
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

void CTSSCtrlPluginOperations::NewOrder_NotVerify(CString& strInstrumentID, BOOL bDirection, int nOCMode, 
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
		m_setCloseOrder.insert(nSeqNum);
#if 1
		poPlusinApi->Send(CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY, &tInputOrder, 
				sizeof(tInputOrder), nSeqNum);
#endif
	}
}

BOOL CTSSCtrlPluginOperations::CloseAll()
{
	MAPINSTPOSI::iterator it;
	CString strMsg;
	BOOL bHasDoClose = FALSE;
	double fLastPrice = 0.0, fPriceTick = 0.0;
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nCurrTime = sysTime.wHour*3600 + sysTime.wMinute*60 + sysTime.wSecond;
	for(it = m_mapInstPosi.begin(); it != m_mapInstPosi.end(); it++) {
		if(IsInRestTime(std::string(it->second.InstrumentID), nCurrTime))
			continue;

		if(!GetInstPriceTick(std::string(it->second.InstrumentID), fPriceTick))
			continue;
		if(!GetLastPrice(std::string(it->second.InstrumentID), fLastPrice))
			continue;
		BOOL bDirection = (it->second.PosiDirection==THOST_FTDC_PD_Long?FALSE:TRUE);
		if(it->second.CanCloseTodayVolume>0) {
			double fPrice = fLastPrice+CConfigManager::GetPlotCtrlHead().fcPriceUp*fPriceTick*(bDirection?1.0:-1.0);
			NewOrder_NotVerify(CString(it->second.InstrumentID), 
					bDirection, 
					2, fPrice, 
					it->second.CanCloseTodayVolume);
			bHasDoClose = TRUE;
			strMsg.Format("强行平仓 [%s], [%s], [%d], [%0.2f], 触发最大亏损。", 
					it->second.InstrumentID, (!bDirection?"买":"卖"), it->second.CanCloseTodayVolume, fPrice);
			ShowMsgLog(strMsg);
		}
		if(it->second.CanCloseydVolume>0) {
			double fPrice = fLastPrice+CConfigManager::GetPlotCtrlHead().fcPriceUp*fPriceTick*(bDirection?1.0:-1.0);
			NewOrder_NotVerify(CString(it->second.InstrumentID), 
					(it->second.PosiDirection==THOST_FTDC_PD_Long?FALSE:TRUE), 
					3, fPrice, 
					it->second.CanCloseydVolume);
			bHasDoClose = TRUE;
			strMsg.Format("强行平仓 [%s], [%s], [%d], [%0.2f], 触发最大亏损。", 
					it->second.InstrumentID, (!bDirection?"买":"卖"), it->second.CanCloseydVolume, fPrice);
			ShowMsgLog(strMsg);
		}
	}
	if(bHasDoClose) {
		m_dwPreCloseTime = ::GetTickCount();
	}
	return TRUE;
}

void CTSSCtrlPluginOperations::CancelOrder(CString& strInstrumentID, BOOL bDirection, double fLimitPrice)
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

void CTSSCtrlPluginOperations::CancelOrder(PlatformStru_OrderInfo& tInfo)
{
    PlusinAPI* poPlusinApi = GetPlusinAPI();
    if(poPlusinApi!=NULL) 
    {
        int nSeqNum = CSequenceManager::GetNextSeqNum();
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

BOOL CTSSCtrlPluginOperations::ProcPosiDetailMaxTime()
{
    MAPPOSIDETAIL::iterator it;
    CString strMsg;
    BOOL bHasDoClose = FALSE;
    double fLastPrice = 0.0, fPriceTick = 0.0;

	//strMsg.Format("CTSSCtrlPluginOperations::ProcPosiDetailMaxTime() for start:");
	//ShowMsgLog(strMsg);
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nCurrTime = sysTime.wHour*3600 + sysTime.wMinute*60 + sysTime.wSecond;
	for(it = m_mapPosiDetail.begin(); it != m_mapPosiDetail.end(); it++) {
		if(m_mapInstCfg.find(std::string(it->second.InstrumentID)) == m_mapInstCfg.end())
			continue;

		if(IsInRestTime(std::string(it->second.InstrumentID), nCurrTime))
			continue;

		if(!GetInstPriceTick(std::string(it->second.InstrumentID), fPriceTick))
			continue;
		if(!GetLastPrice(std::string(it->second.InstrumentID), fLastPrice))
			continue;
		//BOOL bIsToday = strcmp(it->second.OpenDate, it->second.TradingDay)==0;
		BOOL bIsToday = strcmp(it->second.OpenDate, "00:00:00")!=0;
		BOOL bDirection = (it->second.Direction==THOST_FTDC_D_Buy?FALSE:TRUE);
		//int nRemainTime = GetRemainTime(std::string(it->second.OpenDate));
		//int nHandleTime = GetInstCanHandleTime(std::string(it->second.InstrumentID));
		//if(nRemainTime > nHandleTime) {
		if(GetRemainTime(std::string(it->second.InstrumentID), std::string(it->second.OpenDate))<=0) {
            //判断其他撤单已撤单完成
            if(m_mapOtherWaitOrder.size() != 0)
                continue;

			{
			PlatformStru_InputOrder tInputOrder;
			ZeroMemory(&tInputOrder, sizeof(tInputOrder));
			MakeReq_Order(tInputOrder, CString(it->second.InstrumentID), 
					m_strCurrSubAccount, OrderType_FT_Immediately, 
					bDirection, 2, 0, 0.0, 0, FALSE, FALSE, FALSE);
			char csKey[256];
			ZeroMemory(csKey, sizeof(csKey));
			sprintf_s(csKey, sizeof(csKey)-1, "%s,%c,%c", 
					tInputOrder.InstrumentID, tInputOrder.Direction, tInputOrder.CombHedgeFlag[0]);
			m_mapCloseOrder.insert(make_pair(string(csKey), tInputOrder));
			}

			// 判读相应的合约、方向的持仓量与可平量是否相当，不等则不能进行平仓下单
			if(!CanClose(CString(it->second.InstrumentID), 
				(it->second.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short), 
				it->second.HedgeFlag))
				continue;
            double fPrice = fLastPrice+CConfigManager::GetPlotCtrlHead().fcPriceUp*fPriceTick*(bDirection?1.0:-1.0);

			NewOrder_NotVerify(CString(it->second.InstrumentID), 
					bDirection, (bIsToday?2:3), fPrice, it->second.Volume);
			bHasDoClose = TRUE;
			strMsg.Format("强行平仓 [%s], [%s], [%d], [%0.2f], 触发持仓时间。", 
					it->second.InstrumentID, (bDirection?"买":"卖"), it->second.Volume, fPrice);
			ShowMsgLog(strMsg);
		}
	}
	if(bHasDoClose)
		m_dwPreCloseTime = ::GetTickCount();
	return TRUE;
}

BOOL CTSSCtrlPluginOperations::ProcPosiMaxLose()
{
	MAPINSTPOSI::iterator it;
	CString strMsg;
	double fLastPrice = 0.0, fPriceTick = 0.0;
	BOOL bHasDoClose = FALSE;
	PLOTCTRLITEMCFG item;
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	int nCurrTime = sysTime.wHour*3600 + sysTime.wMinute*60 + sysTime.wSecond;
	for(it = m_mapInstPosi.begin(); it != m_mapInstPosi.end(); it++) {
		if(m_mapInstCfg.find(std::string(it->second.InstrumentID)) == m_mapInstCfg.end())
			continue;

		if(IsInRestTime(std::string(it->second.InstrumentID), nCurrTime))
			continue;

		if(!GetInstPriceTick(std::string(it->second.InstrumentID), fPriceTick))
			continue;
		if(!GetLastPrice(std::string(it->second.InstrumentID), fLastPrice))
			continue;
		ZeroMemory(&item, sizeof(item));
		if(!GetInstPlot(std::string(it->second.InstrumentID), item))
			continue;
		BOOL bDirection = (it->second.PosiDirection==THOST_FTDC_PD_Long?TRUE:FALSE);
		
		// 计算盈亏点位价差
		double fPriceDiff = (fLastPrice - it->second.AveragePositionPrice)*(bDirection?1.0:-1.0);

        double fPrice = 0.0f;
        int    nVolume = 0;
		// 处理止盈、止损平仓
		if(item.stopGainPoint*fPriceTick<=fPriceDiff || -item.stopLosePoint*fPriceTick>=fPriceDiff) {

            //判断其他撤单已撤单完成
            if(m_mapOtherWaitOrder.size() != 0)
                continue;
			{
			PlatformStru_InputOrder tInputOrder;
			ZeroMemory(&tInputOrder, sizeof(tInputOrder));
			MakeReq_Order(tInputOrder, CString(it->second.InstrumentID), 
					m_strCurrSubAccount, OrderType_FT_Immediately, 
					!bDirection, 2, 0, 0.0, 0, FALSE, FALSE, FALSE);
			char csKey[256];
			ZeroMemory(csKey, sizeof(csKey));
			sprintf_s(csKey, sizeof(csKey)-1, "%s,%c,%c", 
					tInputOrder.InstrumentID, tInputOrder.Direction, tInputOrder.CombHedgeFlag[0]);
			m_mapCloseOrder.insert(make_pair(string(csKey), tInputOrder));
			}

			// 判读相应的合约、方向的持仓量与可平量是否相当，不等则不能进行平仓下单
			if(!CanClose(CString(it->second.InstrumentID), it->second.PosiDirection, it->second.HedgeFlag))
				continue;
			if(it->second.CanCloseTodayVolume>0) {
                nVolume = it->second.CanCloseTodayVolume;
                fPrice = fLastPrice+CConfigManager::GetPlotCtrlHead().fcPriceUp*fPriceTick*(!bDirection?1.0:-1.0);
                NewOrder_NotVerify(CString(it->second.InstrumentID), 
                    !bDirection, 
                    2, fPrice, 
                    it->second.CanCloseTodayVolume);
                bHasDoClose = TRUE;
                strMsg.Format("强行平仓 [%s], [%s], [%d], [%0.2f], 触发止盈或止损。", 
                    it->second.InstrumentID, (!bDirection?"买":"卖"), it->second.CanCloseTodayVolume, fPrice);
                ShowMsgLog(strMsg);
			}
			if(it->second.CanCloseydVolume>0) {
                nVolume = it->second.CanCloseydVolume;
                fPrice = fLastPrice+CConfigManager::GetPlotCtrlHead().fcPriceUp*fPriceTick*(!bDirection?1.0:-1.0);
                NewOrder_NotVerify(CString(it->second.InstrumentID), 
                    !bDirection, 
                    3, fPrice, 
                    it->second.CanCloseydVolume);
                bHasDoClose = TRUE;
                strMsg.Format("强行平仓 [%s], [%s], [%d], [%0.2f], 触发止盈或止损。", 
                    it->second.InstrumentID, (!bDirection?"买":"卖"), it->second.CanCloseydVolume, fPrice);
                ShowMsgLog(strMsg);
			}
		}
	}
	if(bHasDoClose)
		m_dwPreCloseTime = ::GetTickCount();

	return TRUE;
}

void CTSSCtrlPluginOperations::QueryExchangeIDs()
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
		poPlusinApi->Send(CMDID_EXCHANGE_LIST_QUERY, NULL, 0, 
				CSequenceManager::GetNextSeqNum());
}

void CTSSCtrlPluginOperations::QueryInstrumentIDs(CString& strExchangeID)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		char szExchangeID[32];
		ZeroMemory(szExchangeID, sizeof(szExchangeID));
		strncpy(szExchangeID, strExchangeID, strExchangeID.GetLength());
		poPlusinApi->Send(CMDID_INSTRUMENT_LIST_QUERY, szExchangeID, 
				sizeof(szExchangeID), CSequenceManager::GetNextSeqNum());
	}
}

//无审核下单
void CTSSCtrlPluginOperations::SendInputOrder(PlatformStru_InputOrder& inputOrder)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) {
		//无审核下单命令字
		poPlusinApi->Send(CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY, &inputOrder, 
				sizeof(inputOrder), CSequenceManager::GetNextSeqNum());
	}
}

#if 0
void CTSSCtrlPluginOperations::CancelOrder(CString& strInstrumentID, BOOL bDirection, double fLimitPrice)
{
	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi!=NULL) 
	{
		int nSeqNum = CSequenceManager::GetNextSeqNum();
		PlusinOpenOrderKey tKey;
		
		ZeroMemory(&tKey, sizeof(tKey));
		strncpy(tKey.szAccountID, m_strCurrAccount, sizeof(tKey.szAccountID));
		strncpy(tKey.szSubAccountID, m_strCurrSubAccount, sizeof(tKey.szSubAccountID));
		strncpy(tKey.szInstrumentID, strInstrumentID, sizeof(tKey.szInstrumentID));
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
#endif

BOOL CTSSCtrlPluginOperations::MayClose_Time()
{
	if(m_mapCloseWaitOrderRef.size()==0 && m_setCloseOrder.size()==0)
		return TRUE;
	else {
		if((::GetTickCount() - m_dwPreCloseTime)>6500) {
			m_mapCloseWaitOrderRef.clear();
			return TRUE;
		}
		else
			return FALSE;
	}
}

BOOL CTSSCtrlPluginOperations::CheckPosiHasLose(PlatformStru_InputOrder& inputOrder)
{
	char csKey[256];
	ZeroMemory(csKey, sizeof(csKey));
	sprintf(csKey, "%s,%c,%c", inputOrder.InstrumentID, 
			(inputOrder.Direction==THOST_FTDC_D_Buy?THOST_FTDC_PD_Long:THOST_FTDC_PD_Short), 
			inputOrder.CombHedgeFlag[0]);
	
	MAPINSTPOSI::iterator it = m_mapInstPosi.find(std::string(csKey));
	if(it != m_mapInstPosi.end()) {
		return it->second.PositionProfit<0.0;
	}
	else
		return FALSE;
}

BOOL CTSSCtrlPluginOperations::CanCloseAll()
{
	MAPINSTPOSI::iterator it;
	for(it = m_mapInstPosi.begin(); it != m_mapInstPosi.end(); it++) {
		if(it->second.Position != it->second.CanCloseVolume) 
			return FALSE;
	}
	return TRUE;
}

BOOL CTSSCtrlPluginOperations::CanClose(CString& strInstumentID, char cDirect, char cHedge)
{
	MAPINSTPOSI::iterator it;
	char csKey[256];

	ZeroMemory(csKey, sizeof(csKey));
	sprintf(csKey, "%s,%c,%c", strInstumentID, cDirect, cHedge);
	if((it = m_mapInstPosi.find(csKey)) != m_mapInstPosi.end()) {
		return it->second.Position == it->second.CanCloseVolume;
	}
	else
		return TRUE;
}
