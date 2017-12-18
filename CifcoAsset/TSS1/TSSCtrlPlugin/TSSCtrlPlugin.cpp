// TSSCtrlPlugin.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "TSSCtrlPlugin.h"
#include "TSSCtrlPluginDlg.h"
#include "ConfigMgr.h"
#include "TcpLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSSCtrlPluginApp

BEGIN_MESSAGE_MAP(CTSSCtrlPluginApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTSSCtrlPluginApp 构造

CTSSCtrlPluginApp::CTSSCtrlPluginApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTSSCtrlPluginApp 对象

CTSSCtrlPluginApp theApp;


// CTSSCtrlPluginApp 初始化

BOOL CTSSCtrlPluginApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
#if 1
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
#endif
	
	AfxInitRichEdit2();
 

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

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//加载配置参数
	CConfigManager::Load();

	LoadConfig();
	BOOKORDERDLGCOLOR colorStyle;
	GetColorStyle(GetColorStyle(), colorStyle);
	SetBookOrderDlgColor(colorStyle, GetColorStyle());

	////初始化通信层
	//CTcpLayer::Init();

	GetChangeOrderManager().Init();

	//LONG rtn = GetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE);
	//rtn |= WS_EX_TOOLWINDOW;
	////rtn &= ~WS_EX_APPWINDOW;
	//SetWindowLong(pFrame->GetSafeHwnd(), GWL_EXSTYLE, rtn);

	//// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	////pFrame->ShowWindow(SW_HIDE);//SW_SHOW);
	//pFrame->SetWindowPos(NULL, -10000, -10000, 0, 0, SWP_SHOWWINDOW);
	//pFrame->SetTitle("一件下单");
	////pFrame->ShowWindow(SW_SHOW);
	//pFrame->UpdateWindow();

	CTSSCtrlPluginDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CTSSCtrlPluginApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	
	//释放通信层
	CTcpLayer::Release();

	SetPlusinAPI(NULL);
	PlusinAPI::DestroyInstance();
	SaveConfig();

	return CWinApp::ExitInstance();
}
