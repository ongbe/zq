// restartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "restart.h"
#include "restartDlg.h"

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


// CrestartDlg dialog

void CrestartDlg::GetPath(CString& strFilePath)
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
void CrestartDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == m_nTimer1)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString strFilePath1 = _T("ReStart.txt");
		GetPath(strFilePath1);

		TCHAR buf[100];

		CString str = strFilePath1;
		long lt = st.wYear *10000 + st.wMonth *100 + st.wDay;


		int n = GetPrivateProfileInt(_T("CONFIG"),_T("DAY"),0, strFilePath1); 
		
		if(st.wHour != 8)
			return;

		if(lt == n)
			return;		 			

		CString strTemp;
		strTemp.Format(_T("%d"), lt);
		::WritePrivateProfileString(_T("CONFIG"),_T("DAY"),strTemp, strFilePath1); 	

		CString strFilePath = _T("uniServer.exe");
		GetPath(strFilePath);
		CString str2 = strFilePath;
		str2.Append(_T("  stop")); 

		int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, str2, wcslen(str2), NULL, 0, NULL, NULL);   
		char* szAnsi = new char[ansiLen + 1];  
		memset(szAnsi, 0, sizeof(szAnsi));	
		WideCharToMultiByte(CP_ACP, NULL, str2, wcslen(str2), szAnsi, ansiLen, NULL, NULL);   
		szAnsi[ansiLen] = '\0'; 
		WinExec(szAnsi,SW_SHOW);
		delete []szAnsi;

		Sleep(5000);

		str2 = strFilePath;
		str2.Append(_T("  start"));
		ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, str2, wcslen(str2), NULL, 0, NULL, NULL);   
		szAnsi = new char[ansiLen + 1];   
		memset(szAnsi, 0, sizeof(szAnsi));
		WideCharToMultiByte(CP_ACP, NULL, str2, wcslen(str2), szAnsi, ansiLen, NULL, NULL);   
		szAnsi[ansiLen] = '\0';  
		WinExec(szAnsi,SW_SHOW);

		delete []szAnsi;

	}
	CDialog::OnTimer(nIDEvent);
}
void CrestartDlg::SetAutoStart()
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

CrestartDlg::CrestartDlg(CWnd* pParent /*=NULL*/)
: CDialog(CrestartDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrestartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CrestartDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CrestartDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CrestartDlg message handlers

BOOL CrestartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	SetAutoStart();
	//m_nTimer1 = SetTimer(109,10,NULL);
	m_nTimer1 = SetTimer(109,1000*60*20,NULL);
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

void CrestartDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CrestartDlg::OnPaint()
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
HCURSOR CrestartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CrestartDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
