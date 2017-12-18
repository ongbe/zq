// Global.cpp : 只包括标准包含文件的源文件
// Module-BookOrder.pch 将作为预编译头
// Global.obj 将包含预编译类型信息

#include "stdafx.h"
#include "Global.h"

HANDLE g_hMutex = CreateMutex(NULL, FALSE, NULL);

BOOL LockGlobalObject()
{
	::WaitForSingleObject(g_hMutex, INFINITE);
	return TRUE;
}

BOOL UnlockGlobalObject()
{
	::ReleaseMutex(g_hMutex);
	return TRUE;
}

BOOL g_bInitStatus = FALSE;

BOOL GetInitStatus()
{
	return g_bInitStatus;
}

void SetInitStatus(BOOL bIsSuccess)
{
	g_bInitStatus = bIsSuccess;
}

HWND g_hTargetWnd = NULL;

void SetTargetHWnd(HWND hTargetWnd)
{
	g_hTargetWnd = hTargetWnd;
}

HWND GetTargetHWnd()
{
	return g_hTargetWnd;
}

PlusinAPI* g_poPlusinApi = NULL;

void SetPlusinAPI(PlusinAPI* poPlusinApi)
{
	g_poPlusinApi = poPlusinApi;
}

PlusinAPI* GetPlusinAPI()
{
	return g_poPlusinApi;
}

UINT g_nBookOrderCloseMSGID = ::RegisterWindowMessage("Modlule-BookOrder BookOrder Dialog Close");

UINT GetBookOrderCloseMSGID()
{
	return g_nBookOrderCloseMSGID;
}

BOOL g_bIsConfirmRaskInfo = FALSE;

void SetIsConfirmRaskInfo(BOOL bConfirm)
{
	g_bIsConfirmRaskInfo = bConfirm;
}

BOOL GetIsConfirmRaskInfo()
{
	return g_bIsConfirmRaskInfo;
}

CString g_strProductShortNames;

void SetProductShortNames(CString& strProductShortNames)
{
	g_strProductShortNames = strProductShortNames;
}

CString& GetProductShortNames()
{
	return g_strProductShortNames;
}

std::set<std::string> g_setExchangeID;

void SetExchangeIDList(std::set<std::string>& setExchangeID)
{
	g_setExchangeID = setExchangeID;
}

std::set<std::string>& GetExchangeIDList()
{
	return g_setExchangeID;
}

std::set<std::string> g_setInstrumentID;

void SetInstrumentIDList(std::set<std::string>& setInstrumentID)
{
	g_setInstrumentID = setInstrumentID;
}

std::set<std::string>& GetInstrumentIDList()
{
	return g_setInstrumentID;
}

std::map<std::string, std::string> g_mapProduct2Exchange;

std::map<std::string, std::string>& GetProduct2ExchangeList()
{
	return g_mapProduct2Exchange;
}

std::set<CWnd*> g_setBookOrderDlg;

std::set<CWnd*>& GetBookOrderDlgSet()
{
	return g_setBookOrderDlg;
}

CSequenceManager g_oSequenceManager;

CSequenceManager& GetSequenceManager()
{
	return g_oSequenceManager;
}

CChangeOrderManager g_oChangeOrderManager;

CChangeOrderManager& GetChangeOrderManager()
{
	return g_oChangeOrderManager;
}

int g_nColorStyle = 0;
BOOKORDERDLGCOLOR g_bookOrderDlgColor = {0};

BOOKORDERDLGCOLOR& GetBookOrderDlgColor()
{
	return g_bookOrderDlgColor;
}

int GetColorStyle()
{
	return g_nColorStyle;
}

void SetBookOrderDlgColor(BOOKORDERDLGCOLOR& tDlgColor, int nColorStyle)
{
	g_nColorStyle = nColorStyle;
	g_bookOrderDlgColor = tDlgColor;
}

SIZE g_dlgSize = {0};

SIZE& GetBookDlgSize()
{
	return g_dlgSize;
}

void SetBookDlgSize(SIZE& dlgSize)
{
	g_dlgSize = dlgSize;
}

// 一定放在最后
void ReleaseGlobalObject()
{
	GetChangeOrderManager().LockObject();
	GetChangeOrderManager().Release();
	GetChangeOrderManager().UnlockObject();
	std::set<CWnd*>& setDlg = GetBookOrderDlgSet();
	std::set<CWnd*>::iterator it = setDlg.begin();
	for(; it!=setDlg.end(); it++)
	{
		if((*it)!=NULL && IsWindow((*it)->GetSafeHwnd()))
			(*it)->ShowWindow(SW_HIDE);
	}
	Sleep(20);
	CWnd* pWnd = NULL;
	it=setDlg.begin();
	while(it!=setDlg.end()) 
	{
		pWnd = *it;
		it=setDlg.erase(it);
		if(pWnd!=NULL)
			delete pWnd;
	}
	CSequenceManager& oSeqMng = GetSequenceManager();
	oSeqMng.LockObject();
	oSeqMng.Clear();
	oSeqMng.UnlockObject();
}

ORDERSETTING	g_orderSetting = {conKeyNone, conKeyCtrl, conKeyShift, conOrderByLastPrice, 0};

void SetOrderSetting(const ORDERSETTING& setting)
{
	g_orderSetting = setting;
}

const ORDERSETTING& GetOrderSetting()
{
	return g_orderSetting;
}

std::map<std::string, std::vector<PlatformStru_Position>> g_mapPosition;

BOOL ResetPosition(PlatformStru_Position* pPosi, int nCount)
{
	if(pPosi==NULL || nCount==0)
		return FALSE;

	std::map<std::string, std::vector<PlatformStru_Position>>::iterator it;
	for(int i=0; i<nCount; i++) {
		it = g_mapPosition.find(pPosi[i].InstrumentID);
		if(it != g_mapPosition.end()) {
			it->second.clear();
		}
	}
	for(int i=0; i<nCount; i++) {
		it = g_mapPosition.find(pPosi[i].InstrumentID);
		if(it != g_mapPosition.end()) {
			it->second.push_back(pPosi[i]);
		}
		else {
			std::vector<PlatformStru_Position> vec;
			vec.push_back(pPosi[i]);
			g_mapPosition.insert(std::make_pair(pPosi[i].InstrumentID, vec));
		}
	}
	return TRUE;
}

int g_nLocalCondID = 0;

int GetNextLocalCondID()
{
	return ++g_nLocalCondID;
}

MAPLOCALTRIGGERORDER g_mapLocalTriggerOrder;

const MAPLOCALTRIGGERORDER& GetLocalConds()
{
	return g_mapLocalTriggerOrder;
}

int AppendLocalCond(LOCALTRIGGERORDER& localCond)
{
	MAPLOCALTRIGGERORDER::iterator it;
	MAPPRICETRIGGERITEM::iterator itItem;
	std::string strKey(localCond.InstrumentID);
	localCond.nID = GetNextLocalCondID();

	it = g_mapLocalTriggerOrder.find(strKey);
	if(it != g_mapLocalTriggerOrder.end()) {
		itItem = it->second.find(localCond.nID);
		if(itItem == it->second.end()) {
			it->second.insert(std::make_pair(localCond.nID, localCond));
		}
	}
	else {
		MAPPRICETRIGGERITEM map;
		map.insert(std::make_pair(localCond.nID, localCond));
		g_mapLocalTriggerOrder.insert(std::make_pair(strKey, map));
	}
	return TRUE;
}

BOOL DeleteLocalCondByID(const std::string& strInstrumentID, int nID)
{
	MAPLOCALTRIGGERORDER::iterator it;
	MAPPRICETRIGGERITEM::iterator itItem;
	std::string strKey(strInstrumentID);

	it = g_mapLocalTriggerOrder.find(strKey);
	if(it != g_mapLocalTriggerOrder.end()) {
		itItem = it->second.find(nID);
		if(itItem != it->second.end()) {
			it->second.erase(itItem);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL DeleteLocalCondByPrice(const std::string& strInstrumentID, BOOL bDirection, double fTriggerPrice)
{
	MAPLOCALTRIGGERORDER::iterator it;
	MAPPRICETRIGGERITEM::iterator itItem;
	std::string strKey(strInstrumentID);
	BOOL bRet = FALSE;

	it = g_mapLocalTriggerOrder.find(strKey);
	if(it != g_mapLocalTriggerOrder.end()) {
		itItem = it->second.begin();
		while(itItem != it->second.end()) {
			if(itItem->second.Direction==bDirection && 
					util::equals(fTriggerPrice, itItem->second.TriggerPrice)) {
				itItem = it->second.erase(itItem);
				bRet = TRUE;
			}
			else {
				itItem++;
			}
		}
	}
	return FALSE;
}

void ClearLocalConds()
{
	g_mapLocalTriggerOrder.clear();
}

void OnQuoteForLocalConds(const PlatformStru_DepthMarketData& quote)
{
	MAPLOCALTRIGGERORDER::iterator it;
	MAPPRICETRIGGERITEM::iterator itItem;
	std::string strKey(quote.InstrumentID);
	int nSeqNum = 0;
	double fLimitPrice = 0.0;
	PlatformStru_InputOrder tInputOrder;

	it = g_mapLocalTriggerOrder.find(strKey);
	if(it != g_mapLocalTriggerOrder.end()) {
		itItem = it->second.begin();
		while(itItem != it->second.end()) {
			//double f = ritItem->first;
			if(itItem->second.Direction && util::greaterOrEqual(itItem->second.TriggerPrice, quote.LastPrice)) {
				ZeroMemory(&tInputOrder, sizeof(tInputOrder));
				if(itItem->second.OrderPriceMode==conOrderByMarket) {
					MakeReq_Order(tInputOrder, CString(quote.InstrumentID), 
							CString(itItem->second.AccountID), OrderType_FT_Immediately, 
							TRUE, itItem->second.OpenClose, 
							itItem->second.Qty, 0.0, 0, FALSE, TRUE, FALSE);
				}
				else {
					if(itItem->second.OrderPriceMode==conOrderByAskBidPrice) {
						if(!util::isInvalidValue(quote.AskPrice1))
							fLimitPrice = quote.AskPrice1;
						else
							fLimitPrice = quote.LastPrice;
					}
					else 
						fLimitPrice = itItem->second.TriggerPrice;
					fLimitPrice += itItem->second.OrderOffset * itItem->second.PriceTick;
					if(util::greater(fLimitPrice, quote.UpperLimitPrice))
						fLimitPrice = quote.UpperLimitPrice;
					else if(util::less(fLimitPrice, quote.LowerLimitPrice))
						fLimitPrice = quote.LowerLimitPrice;
					MakeReq_Order(tInputOrder, CString(quote.InstrumentID), 
							CString(itItem->second.AccountID), OrderType_FT_Immediately, 
							TRUE, itItem->second.OpenClose, 
							itItem->second.Qty, fLimitPrice, 0, FALSE, FALSE, FALSE);
				}
				PlusinAPI* poPlusinApi = GetPlusinAPI();
				if(poPlusinApi!=NULL) {
					nSeqNum = CSequenceManager::GetNextSeqNum();
					poPlusinApi->Send(CMDID_ORDER_INSERT_ORDER, &tInputOrder, 
						sizeof(tInputOrder), nSeqNum);
				}
				itItem = it->second.erase(itItem);
			}
			else if(!itItem->second.Direction && util::lessOrEqual(itItem->second.TriggerPrice, quote.LastPrice)) {
				ZeroMemory(&tInputOrder, sizeof(tInputOrder));
				if(itItem->second.OrderPriceMode==conOrderByMarket) {
					MakeReq_Order(tInputOrder, CString(quote.InstrumentID), 
							CString(itItem->second.AccountID), OrderType_FT_Immediately, 
							FALSE, itItem->second.OpenClose, 
							itItem->second.Qty, 0.0, 0, FALSE, TRUE, FALSE);
				}
				else {
					if(itItem->second.OrderPriceMode==conOrderByAskBidPrice) {
						if(!util::isInvalidValue(quote.BidPrice1))
							fLimitPrice = quote.BidPrice1;
						else
							fLimitPrice = quote.LastPrice;
					}
					else 
						fLimitPrice = itItem->second.TriggerPrice;
					fLimitPrice -= itItem->second.OrderOffset * itItem->second.PriceTick;
					if(util::greater(fLimitPrice, quote.UpperLimitPrice))
						fLimitPrice = quote.UpperLimitPrice;
					else if(util::less(fLimitPrice, quote.LowerLimitPrice))
						fLimitPrice = quote.LowerLimitPrice;
					MakeReq_Order(tInputOrder, CString(quote.InstrumentID), 
							CString(itItem->second.AccountID), OrderType_FT_Immediately, 
							FALSE, itItem->second.OpenClose, 
							itItem->second.Qty, fLimitPrice, 0, FALSE, FALSE, FALSE);
				}
				PlusinAPI* poPlusinApi = GetPlusinAPI();
				if(poPlusinApi!=NULL) {
					nSeqNum = CSequenceManager::GetNextSeqNum();
					poPlusinApi->Send(CMDID_ORDER_INSERT_ORDER, &tInputOrder, 
						sizeof(tInputOrder), nSeqNum);
				}
				itItem = it->second.erase(itItem);
			}
			else
				itItem++;
		}
	}
}

CString GetIniFile()
{
	char strPath[1024];
	ZeroMemory(strPath, sizeof(strPath));
	DWORD dwRet = 0;
	dwRet = ::GetModuleFileName(NULL, strPath, 1023);
	if(dwRet<4)
		return CString("");
	strPath[dwRet-4] = '\000';
	CString strIniFile(strPath);
	strIniFile += ".INI";
	return strIniFile;
}

void SaveConfig()
{
	CString strIniFile = GetIniFile();
	CString strText;

	strText.Format("%d", g_nColorStyle);
	::WritePrivateProfileString("BookOrderDlg", "ColorStyle", strText, strIniFile);
	strText.Format("%d", g_dlgSize.cx);
	::WritePrivateProfileString("BookOrderDlg", "DlgWidth", strText, strIniFile);
	strText.Format("%d", g_dlgSize.cy);
	::WritePrivateProfileString("BookOrderDlg", "DlgHeight", strText, strIniFile);
}

void LoadConfig()
{
	CString strIniFile = GetIniFile();

	g_nColorStyle = ::GetPrivateProfileInt("BookOrderDlg", "ColorStyle", 1, strIniFile);
	g_dlgSize.cx = ::GetPrivateProfileInt("BookOrderDlg", "DlgWidth", 0, strIniFile);
	g_dlgSize.cy = ::GetPrivateProfileInt("BookOrderDlg", "DlgHeight", 0, strIniFile);
}