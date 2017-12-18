// Module-BookOrder.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Module-BookOrder.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModuleBookOrderApp

BEGIN_MESSAGE_MAP(CModuleBookOrderApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CModuleBookOrderApp::OnAppAbout)
END_MESSAGE_MAP()


// CModuleBookOrderApp 构造

CModuleBookOrderApp::CModuleBookOrderApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CModuleBookOrderApp 对象

CModuleBookOrderApp theApp;


// CModuleBookOrderApp 初始化

BOOL CModuleBookOrderApp::InitInstance()
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

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象

	LoadConfig();
	BOOKORDERDLGCOLOR colorStyle;
	GetColorStyle(GetColorStyle(), colorStyle);
	SetBookOrderDlgColor(colorStyle, GetColorStyle());

	DWORD dwTimeout = -1;  
	::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)&dwTimeout, 0);  
	if (dwTimeout >= 100) {  
		::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);  
	}

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);


	GetChangeOrderManager().Init();


	LONG rtn = GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE);
	rtn |= WS_EX_TOOLWINDOW;
	//rtn &= ~WS_EX_APPWINDOW;
	SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, rtn);


	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	//pFrame->ShowWindow(SW_HIDE);//SW_SHOW);
	pFrame->SetWindowPos(NULL, -10000, -10000, 0, 0, SWP_SHOWWINDOW);
	pFrame->SetTitle("一件下单");
	//pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}


// CModuleBookOrderApp 消息处理程序




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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

// 用于运行对话框的应用程序命令
void CModuleBookOrderApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CModuleBookOrderApp 消息处理程序


int CModuleBookOrderApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	SetPlusinAPI(NULL);
	PlusinAPI::DestroyInstance();
	SaveConfig();

	return CWinApp::ExitInstance();
}
