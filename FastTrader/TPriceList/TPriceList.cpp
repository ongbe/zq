// TPriceList.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TPriceList.h"
#include "TPriceListDLL.h"
#include "TPriceListPanel.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWnd g_wndParent;

//创建新的T型报价窗口，返回窗口句柄
HWND CreateWnd(HWND hParent)
{
	LockGlobalObject();

	CWnd* pPanel = _GetCurrPanel();
	if(pPanel==NULL) {
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		////AFX_MANAGE_STATE(AfxGetAppModuleState());
		g_wndParent.Attach(hParent);
		CTPriceListPanel* pNew = new CTPriceListPanel();
		if(pNew->Create(CTPriceListPanel::IDD, &g_wndParent)) {
			//::SetParent(pNew->GetSafeHwnd(), hParent);
			_SetCurrPanel(pNew);
			pPanel = pNew;
		}
		else {
			int nError = ::GetLastError();
			TRACE("Error: %d\n", nError);
		}
	}
	_SetMainWnd(hParent);
	HWND hRet = NULL;
	if(pPanel != NULL)
		hRet = pPanel->GetSafeHwnd();

	UnlockGlobalObject();

	return hRet;
}

//设置期权合约
void SetOptionInstruments(const std::vector<PlatformStru_InstrumentInfo>& vecOptions)
{
	LockGlobalObject();
	CTPriceListPanel* pPanel = (CTPriceListPanel*)_GetCurrPanel();
	UnlockGlobalObject();
	if(pPanel != NULL) {
		pPanel->ResetInstrumentList(vecOptions);
	}
}

//新行情到来
void NewMarketData(const PlatformStru_DepthMarketData& NewMD)
{
	LockGlobalObject();
	CTPriceListPanel* pPanel = (CTPriceListPanel*)_GetCurrPanel();
	UnlockGlobalObject();
	if(pPanel != NULL) {
		pPanel->ParseQuot(&NewMD);
	}
}

bool Get_Del_ClickParam(int EventParamID, int& nClickMode, 
						std::string& strInstrumentID, BOOL& bDirection)
{
	bool bRet = false;
	LockGlobalObject();
	CTPriceListPanel* pPanel = (CTPriceListPanel*)_GetCurrPanel();
	if(pPanel != NULL) {
		Stru_PriceListT_ClickParam retParam = {0};
		bRet = pPanel->ReadClickParam(EventParamID, retParam);
		nClickMode = retParam.nClickMode;
		strInstrumentID = retParam.szInstrument;
		bDirection = retParam.bDirection;
	}
	UnlockGlobalObject();
	return bRet;
}

//设置颜色模式，0为浅底色配色；1为深底色配色
void SetColorMode(int colmode)
{
	LockGlobalObject();
	CTPriceListPanel* pPanel = (CTPriceListPanel*)_GetCurrPanel();
	UnlockGlobalObject();
	if(pPanel != NULL) {
		pPanel->ResetColorSuite(colmode);
	}
}


//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CTPriceListApp

BEGIN_MESSAGE_MAP(CTPriceListApp, CWinApp)
END_MESSAGE_MAP()


// CTPriceListApp construction

CTPriceListApp::CTPriceListApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTPriceListApp object

CTPriceListApp theApp;


// CTPriceListApp initialization

BOOL CTPriceListApp::InitInstance()
{
	CWinApp::InitInstance();

	InitGridMap();
	ResetDefaultShowCol();

	return TRUE;
}

int CTPriceListApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd* pPanel = _GetCurrPanel();
	if(pPanel != NULL) {
		delete pPanel;
		_SetCurrPanel(NULL);
	}

	if(g_wndParent.GetSafeHwnd() != NULL) {
		g_wndParent.Detach();
	}

	return CWinApp::ExitInstance();
}
