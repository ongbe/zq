// NationalInstPrompt.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ArbitrageDelegation.h"
#include "ArbitrageDelegationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND g_hWnd_FT=NULL;

// CArbitrageDelegationApp

BEGIN_MESSAGE_MAP(CArbitrageDelegationApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CArbitrageDelegationApp construction

CArbitrageDelegationApp::CArbitrageDelegationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CArbitrageDelegationApp object

CArbitrageDelegationApp theApp;


// CArbitrageDelegationApp initialization

BOOL CArbitrageDelegationApp::InitInstance()
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

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

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


    CArbitrageDelegationDlg dlg;
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
