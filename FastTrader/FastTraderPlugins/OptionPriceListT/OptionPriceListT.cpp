// OptionPriceListT.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "OptionPriceListT.h"
#include "MainFrm.h"

#include "OptionPriceListTDoc.h"
#include "OptionPriceListTView.h"
#include "DlgOptionTTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COptionPriceListTApp

BEGIN_MESSAGE_MAP(COptionPriceListTApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COptionPriceListTApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// COptionPriceListTApp 构造

COptionPriceListTApp::COptionPriceListTApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COptionPriceListTApp 对象

COptionPriceListTApp theApp;


// COptionPriceListTApp 初始化

BOOL COptionPriceListTApp::InitInstance()
{

	if(__argc!=2) {
		//AfxMessageBox("NULL == lpCmdLine");
		//return FALSE;
	}
	HWND hWnd = (HWND)atol(__argv[1]);
	//hWnd = (HWND)0x00091046;
	if(!IsWindow(hWnd)) {
		//AfxMessageBox("!IsWindow(hWnd)");
		//return FALSE;
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

	int nRunMode = ::GetPrivateProfileInt("Startup", "RunMode", conModePopup, GetIniFile());
	_SetRunMode(nRunMode);//conModePopup);//conModeChild);//
	char csTemp[256] = {0};
	int nRet = ::GetPrivateProfileString("Startup", "NewOptionType", "2", csTemp, sizeof(csTemp)-1, GetIniFile());
	csTemp[nRet] = 0;
	_SetNewOptionType(csTemp[0]);

	InitGridMap();
	// 设置确实的显示列
	ResetDefaultShowCol();

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
	//LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
	//// 注册应用程序的文档模板。文档模板
	//// 将用作文档、框架窗口和视图之间的连接
	//CSingleDocTemplate* pDocTemplate;
	//pDocTemplate = new CSingleDocTemplate(
	//	IDR_MAINFRAME,
	//	RUNTIME_CLASS(COptionPriceListTDoc),
	//	RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
	//	RUNTIME_CLASS(COptionPriceListTView));
	//if (!pDocTemplate)
	//	return FALSE;
	//AddDocTemplate(pDocTemplate);


	LoadConfig();

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
	pFrame->Create(NULL, NULL, 0);
	//pFrame->LoadFrame(IDR_MAINFRAME, 0);
	//pFrame->LoadFrame(IDR_MAINFRAME,
	//	WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
	//	NULL);


	//// 分析标准外壳命令、DDE、打开文件操作的命令行
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);


	//// 调度在命令行中指定的命令。如果
	//// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	LONG rtn = GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE);
	//rtn |= WS_EX_TOOLWINDOW;
	rtn &= ~WS_EX_APPWINDOW;
	SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, rtn);

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	//pFrame->SetWindowPos(NULL, 300, 300, 980, 700, SWP_SHOWWINDOW);//|SWP_NOMOVE|SWP_NOSIZE);
	//pFrame->SetWindowPos(NULL, -10000, -10000, 0, 0, SWP_SHOWWINDOW);
	//pFrame->SetTitle("期权T型报价");
	pFrame->SetWindowText("期权T型报价");
	//pFrame->ShowWindow(SW_HIDE);
	pFrame->UpdateWindow();

	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

int COptionPriceListTApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	SetPlusinAPI(NULL);
	PlusinAPI::DestroyInstance();
	SaveConfig();

	return CWinApp::ExitInstance();
}



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
void COptionPriceListTApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// COptionPriceListTApp 消息处理程序


BOOL COptionPriceListTApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(_GetRunMode()==conModePopup) {
		if(pMsg->message==WM_KEYDOWN) {
			if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE) {
				PlusinAPI* poPlusinApi = GetPlusinAPI();
				if(poPlusinApi!=NULL) {
					poPlusinApi->Send(CMDID_CONSOLE_HIDE, NULL, 0, 0);
				}
			}
		}
	}

	return CWinApp::PreTranslateMessage(pMsg);
}
