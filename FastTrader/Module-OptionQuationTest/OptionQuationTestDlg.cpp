// OptionQuationTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OptionQuationTest.h"
#include "OptionQuationTestDlg.h"

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


// COptionQuationTestDlg dialog




COptionQuationTestDlg::COptionQuationTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionQuationTestDlg::IDD, pParent)
	, m_strInstrument(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
	m_strInstrument ="IF1502";
}

void COptionQuationTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strInstrument);
}

BEGIN_MESSAGE_MAP(COptionQuationTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LAUNCH, &COptionQuationTestDlg::OnBnClickedButtonLaunch)
	
	ON_BN_CLICKED(IDOK, &COptionQuationTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COptionQuationTestDlg message handlers

BOOL COptionQuationTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// TODO: Add extra initialization here
	UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COptionQuationTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COptionQuationTestDlg::OnPaint()
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
HCURSOR COptionQuationTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COptionQuationTestDlg::OnBnClickedButtonLaunch()
{		
	UpdateData(true);
	CString strPath;
	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	std::string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	strPath.Format(_T("%s"), filename.substr(0, splitpos+1).c_str()) ;

	CString strDir;

#ifdef _DEBUG
	strDir.Format(_T("%sOptionQuationD.dll"), strPath);
#else
	strDir.Format(_T("%sOptionQuation.dll"), strPath);	
#endif	

	m_HInstance = LoadLibrary(strDir.GetBuffer(strDir.GetLength()));
	Func_CreateQuationWnd  funcCreate=(Func_CreateQuationWnd)GetProcAddress(m_HInstance,"CreateQuationWnd");	
	funcCreate(m_hWnd);

	Func_SetTradingDay funcSetTrayDay = (Func_SetTradingDay)GetProcAddress(m_HInstance,"SetTradingDay");	

	char strTime[128];
	SYSTEMTIME st;
	GetSystemTime(&st);
	memset(strTime, 0, sizeof(strTime));
	sprintf(strTime,"%.4d%.2d%.2d",st.wYear,st.wMonth,st.wDay);

	std::string strTradingday = strTime;
	funcSetTrayDay(strTradingday.c_str());

	Func_ShowInstrument funcShowInstrument = (Func_ShowInstrument)GetProcAddress(m_HInstance,"ShowInstrument");
	std::string strInstrument = m_strInstrument.GetBuffer(m_strInstrument.GetLength());
	funcShowInstrument(strInstrument);

	//交易时段
	std::string strInstrument2 = m_strInstrument.GetBuffer(m_strInstrument.GetLength());
	std::vector<std::pair<std::string,std::string>> TradingTime;
	TradingTime.push_back(std::make_pair("09:15", "11:30"));//第一交易时段的起止时间
	TradingTime.push_back(std::make_pair("13:00", "15:15"));//第二交易时段的起止时间
	Func_SetTradingScales funcSetTradingScales = (Func_SetTradingScales)GetProcAddress(m_HInstance,"SetTradingScales");
	funcSetTradingScales(strInstrument2, TradingTime);

	Func_InitFinish funcInitFinish = (Func_InitFinish)GetProcAddress(m_HInstance,"InitFinish");
	funcInitFinish(strInstrument);

}

void COptionQuationTestDlg::OnDestroy()
{	
	if(m_HInstance)
		FreeLibrary(m_HInstance);
	CDialog::OnDestroy();
}
void COptionQuationTestDlg::OnBnClickedOk()
{
	OnOK();
}
