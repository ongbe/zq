// RestartClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RestartClient.h"
#include "RestartClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CRestartClientDlg dialog

void CRestartClientDlg::GetPath(CString& strFilePath)
{
	TCHAR szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, 256 );
	CString strSystemPath( szLocalPath );
	int nPos = strSystemPath.ReverseFind( '\\' );
	if ( -1 != nPos )
	{
		strSystemPath = strSystemPath.Left( nPos + 1 );
		strFilePath = strSystemPath + strFilePath;
	}	
}
void CRestartClientDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == m_nTimer1)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString strFilePath1 = _T("ResartConfig.txt");
		GetPath(strFilePath1);

		TCHAR buf[100];
		CString str = strFilePath1;
		long lt = st.wYear *10000 + st.wMonth *100 + st.wDay;


		int n = GetPrivateProfileInt(_T("CONFIG"),_T("DAY"),0, strFilePath1); 
		
		TCHAR szHour[256];
		TCHAR szMinute[256];
		GetPrivateProfileString(_T("AutoStart"), _T("hour"), _T(""), szHour, 256, strFilePath1);
		GetPrivateProfileString(_T("AutoStart"), _T("minute"), _T(""), szMinute, 256, strFilePath1);

		int nHour	= _ttoi(szHour);
		int nMinute = _ttoi(szMinute);
		if(st.wHour != nHour || st.wMinute >=nMinute+1 || st.wMinute <nMinute)
			return;

		if(lt == n)//一天只启动一次，其实有上面那个判断就够了
			return;		 			

		CString strTemp;
		strTemp.Format(_T("%d"), lt);
		::WritePrivateProfileString(_T("CONFIG"),_T("DAY"),strTemp, strFilePath1); 	



	//	KillProcess("中期风控终端");
		KillProcessWithName(_T("Client.exe"));

		Sleep(5000);
		strFilePath1 = _T("ResartConfig.txt");
		GetPath(strFilePath1);

		TCHAR szName[256];
		TCHAR szPwd[256];
		GetPrivateProfileString(_T("AutoStart"), _T("username"), _T(""), szName, 256, strFilePath1);
		GetPrivateProfileString(_T("AutoStart"), _T("password"), _T(""), szPwd, 256, strFilePath1);
		
		CString strFilePath = _T("Client.exe");		
		GetPath(strFilePath);
		
		
		strFilePath = _T("Client.exe");
		//strFilePath.Format(_T("Client.exe %s %s"), szName, szPwd);
		GetPath(strFilePath);

		CString str2 = strFilePath;
		CString str3;
		str3.Format(_T("%s %s"),szName,szPwd);
	/*
		int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, str2, wcslen(str2), NULL, 0, NULL, NULL);   
		char *szAnsi = new char[ansiLen + 1];  
		memset(szAnsi, 0, sizeof(szAnsi));	
		WideCharToMultiByte(CP_ACP, NULL, str2, wcslen(str2), szAnsi, ansiLen, NULL, NULL);   
		szAnsi[ansiLen] = '\0'; 
		WinExec(szAnsi,SW_SHOW);*/
		CString strPath;
		GetPath(strPath);
		ShellExecuteW(NULL,_T("open"),strFilePath,str3,strPath, SW_SHOW);
//		delete []szAnsi;

		
	//	KillTimer(109);
		

	}
	CDialog::OnTimer(nIDEvent);
}
//结束进程
int CRestartClientDlg::KillProcess(LPCSTR pszWindowTitle)
{
	HANDLE hProcessHandle;
	ULONG nProcessID;
	HWND TheWindow;
	TheWindow = ::FindWindowA( NULL, pszWindowTitle );

	///方法一：

	::GetWindowThreadProcessId( TheWindow, &nProcessID );
	hProcessHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE,
		nProcessID );
	return ::TerminateProcess( hProcessHandle, 4 );

	///方法二：

//	return ::PostMessage(TheWindow, WM_CLOSE, NULL, NULL);
}
int CRestartClientDlg::KillProcessWithName(const wchar_t* _lpszFileName)
{
	HANDLE hd = NULL;
	HANDLE hpross = NULL;
	int retls = 0;
	PROCESSENTRY32 pinfo = {0};
	wchar_t szFileName[MAX_PATH] = {0};
	int rel = 0;
	hpross = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(INVALID_HANDLE_VALUE == hpross)
	{
		printf("%s\n","CreateToolhelp32Snapshot error.");
		assert(0);
		return 0;
	}
	pinfo.dwSize = sizeof(PROCESSENTRY32);
	retls = Process32First(hpross,&pinfo);
	while(retls)
	{  
		swprintf(szFileName,L"%s",pinfo.szExeFile);
		if (0 == _wcsicmp(szFileName,_lpszFileName))
		{
			hd = OpenProcess(PROCESS_TERMINATE,TRUE,pinfo.th32ProcessID);
			TerminateProcess(hd,13);
			CloseHandle(hd);
			hd = NULL;
			rel = 1;
		}
		if(!Process32Next(hpross, &pinfo))
		{
			break;
		}
	}
	CloseHandle(hpross);
	hpross = NULL;
	return rel;
}

void CRestartClientDlg::SetAutoStart()
{
	//写入注册表,开机自启动 
	HKEY hKey; 
	//找到系统的启动项 
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"); 
	//打开启动项Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey); 
	if(lRet == ERROR_SUCCESS) 
	{ 
		TCHAR pFileName[MAX_PATH] = {0}; 
		//得到程序自身的全路径 
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH); 
		//添加一个子Key,并设置值 // 下面的"getip"是应用程序名字（不加后缀.exe）
		lRet = RegSetValueEx(hKey, _T("AutoStart_uniserver"), 0, REG_SZ, (BYTE *)pFileName, dwRet*2);

		//关闭注册表 
		RegCloseKey(hKey); 
		if(lRet != ERROR_SUCCESS) 
		{ 
			AfxMessageBox(_T("系统参数错误,不能随系统启动")); 
		} 
	} 


	
}

CRestartClientDlg::CRestartClientDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRestartClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRestartClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRestartClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CRestartClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRestartClientDlg message handlers

BOOL CRestartClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	SetAutoStart();
//	m_nTimer1 = SetTimer(109,10,NULL);
	m_nTimer1 = SetTimer(109,1000*60,NULL);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	AfxMessageBox(_T("启动成功"));
	WINDOWPLACEMENT wp;

	wp.length=sizeof(WINDOWPLACEMENT);

	wp.flags=WPF_RESTORETOMAXIMIZED;

	wp.showCmd=SW_HIDE;

	SetWindowPlacement(&wp);





	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRestartClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRestartClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRestartClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRestartClientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
