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

void SaveConfig()
{
	char strPath[1024];
	ZeroMemory(strPath, sizeof(strPath));
	DWORD dwRet = ::GetCurrentDirectory(1023, strPath);
	strPath[dwRet]='\000';
	if(strPath[dwRet-1]!='\\')
	{
		strPath[dwRet]='\\';
		strPath[dwRet+1]='\000';
	}
	CString strIniFile(strPath);
	strIniFile += ::AfxGetAppName();
	strIniFile += ".INI";

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
	char strPath[1024];
	ZeroMemory(strPath, sizeof(strPath));
	DWORD dwRet = ::GetCurrentDirectory(1023, strPath);
	strPath[dwRet]='\000';
	if(strPath[dwRet-1]!='\\')
	{
		strPath[dwRet]='\\';
		strPath[dwRet+1]='\000';
	}
	CString strIniFile(strPath);
	strIniFile += ::AfxGetAppName();
	strIniFile += ".INI";

	g_nColorStyle = ::GetPrivateProfileInt("BookOrderDlg", "ColorStyle", 1, strIniFile);
	g_dlgSize.cx = ::GetPrivateProfileInt("BookOrderDlg", "DlgWidth", 0, strIniFile);
	g_dlgSize.cy = ::GetPrivateProfileInt("BookOrderDlg", "DlgHeight", 0, strIniFile);
}