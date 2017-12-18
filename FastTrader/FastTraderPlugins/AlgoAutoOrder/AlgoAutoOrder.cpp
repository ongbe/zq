// AlgoAutoOrder.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAlgoAutoOrderApp

BEGIN_MESSAGE_MAP(CAlgoAutoOrderApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CAlgoAutoOrderApp::OnAppAbout)
END_MESSAGE_MAP()


// CAlgoAutoOrderApp construction

CAlgoAutoOrderApp::CAlgoAutoOrderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CAlgoAutoOrderApp object

CAlgoAutoOrderApp theApp;


// CAlgoAutoOrderApp initialization

BOOL CAlgoAutoOrderApp::InitInstance()
{
    if(__argc!=2)
    {
        //AfxMessageBox("NULL == lpCmdLine");
        return FALSE;
    }
    HWND hWnd = (HWND)atol(__argv[1]);
    //hWnd = (HWND)0x00091046;
    if(!IsWindow(hWnd))
    {
        //AfxMessageBox("!IsWindow(hWnd)");
        return FALSE;
    }

   // _asm int 3
    SetTargetHWnd(hWnd);

#ifdef _DEBUG
    {
        CString strText;
        strText.Format("%x", hWnd);
        if(::MessageBox(NULL, 
            "不需要提示请点No按钮！\n"
            "需要调试请点Yes按钮，这里会为您产生一个ASSERT，然后请点击\"重试\"，\n"
            "然后选中打开\"Module-BookOrder\"工程的VC2008IDE。当IDE捕捉到进城后\n"
            "提示\"Break\"还是\"Continue\"，建议选择\"Continue\"。", 
            "调试 提示", 
            MB_YESNO|MB_ICONWARNING|MB_APPLMODAL)==IDYES) 
        {
            //AfxMessageBox(strText);
            ASSERT(FALSE);
        }
    }
#endif

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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);


    LoadConfig();
    BOOKORDERDLGCOLOR colorStyle;
    GetColorStyle(GetColorStyle(), colorStyle);
    SetBookOrderDlgColor(colorStyle, GetColorStyle());

    DWORD dwTimeout = -1;  
    ::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)&dwTimeout, 0);  
    if (dwTimeout >= 100) {  
        ::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);  
    }

    //GetChangeOrderManager().Init();

    LONG rtn = GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE);
    rtn |= WS_EX_TOOLWINDOW;
    SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, rtn);

    pFrame->SetWindowPos(NULL, -10000, -10000, 0, 0, SWP_SHOWWINDOW);
    pFrame->SetTitle("盯盘建仓平仓");
   
	// The one and only window has been initialized, so show and update it
	//pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


// CAlgoAutoOrderApp message handlers




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CAlgoAutoOrderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CAlgoAutoOrderApp message handlers

