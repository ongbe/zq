// OptionStrategy.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "OptionStrategyDlg.h"
#include "BKColor.h"

#include "ConfigMgr.h"
#include "TCPDataService.h"

#include <gdiplus.h>
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HWND g_hWnd_FT=NULL;

// COptionStrategyApp

BEGIN_MESSAGE_MAP(COptionStrategyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// COptionStrategyApp construction

COptionStrategyApp::COptionStrategyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only COptionStrategyApp object

COptionStrategyApp theApp;


// COptionStrategyApp initialization

BOOL COptionStrategyApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    //_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(428); //{428}代表了第428次内存分配操作发生了泄漏

    if(__argc>1)
    {
        g_hWnd_FT = (HWND)atol(__argv[1]);
        if(!::IsWindow(g_hWnd_FT))
            g_hWnd_FT=NULL;
    }

#ifdef _DEBUG
    {
        if(::MessageBox(NULL, 
            "不需要提示请点No按钮！\n"
            "需要调试请点Yes按钮，这里会为您产生一个ASSERT，然后请点击\"重试\"，\n"
            "然后选中打开\"Module-BookOrder\"工程的VC2008IDE。当IDE捕捉到进城后\n"
            "提示\"Break\"还是\"Continue\"，建议选择\"Continue\"。", 
            "调试 提示", 
            MB_YESNO|MB_ICONWARNING|MB_APPLMODAL)==IDYES) 
        {
            ASSERT(FALSE);
        }
    }
#endif 

    
    // initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    CDataInfo::NewInstance();

    CString csIP = ConfigManager::GetInstance()->GetServerIPAddress();
    int nPort = ConfigManager::GetInstance()->GetServerPort();
    CTCPDataService::GetInstance()->Connect(csIP.GetBuffer(0), nPort);
    csIP.ReleaseBuffer();

    bool bMultiInstance = ConfigManager::GetInstance()->GetMultiInstance();
    if(!bMultiInstance)
    {
        HANDLE   hMutex=::CreateMutex(NULL,TRUE,"OnlyOneSingleApp");
        if (hMutex!=NULL)  
        {  
            if(GetLastError()==ERROR_ALREADY_EXISTS)  
            {  
                AfxMessageBox("该配置只能有一个应用程序运行.");  
                return   FALSE;  
            }  
        } 
    }
    else
    {

    }

    int nWidth = ConfigManager::GetInstance()->GetResolutionWidth();
    int nHight = ConfigManager::GetInstance()->GetResolutionHight();

    float fSplitterH = ConfigManager::GetInstance()->GetSplitterH();
    float fSplitterV = ConfigManager::GetInstance()->GetSplitterV();

    int nColorMode = ConfigManager::GetInstance()->GetColorMode();

	COptionStrategyDlg dlg(nWidth,nHight,fSplitterH,fSplitterV);
    dlg.SetBKColorMode((0==nColorMode)?Color_Light:Color_Thick);

    //_CrtDumpMemoryLeaks();

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int COptionStrategyApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    // release GDI+ resource
    GdiplusShutdown(m_gdiplusToken);

    CDataInfo::DestroyInstance();
    ConfigManager::DestroyConfigerManager();
    CTCPDataService::DestroyInstance();
    PlusinAPI::DestroyInstance();

    return CWinApp::ExitInstance();
}
