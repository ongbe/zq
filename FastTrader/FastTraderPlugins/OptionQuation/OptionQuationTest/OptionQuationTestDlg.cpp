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
	m_strInstrument ="IF1410";
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
	if(!::IsWindow(m_DlgQuation.m_hWnd))
	{		
		m_DlgQuation.Create(IDD_DIALOG_QUATION, this);			
	}	
	m_DlgQuation.ShowWindow(SW_SHOW);

	UpdateData(true);
	std::string str = m_strInstrument.GetBuffer(m_strInstrument.GetLength());
	//设置交易日
	std::string strTradingday = "20140925";
	m_DlgQuation.SetTradingDay(strTradingday); 
	//设置回调获取当前时间
	m_DlgQuation.SetCallBackFunction(&COptionQuationTestDlg::GetTime);

	//显示合约
	m_DlgQuation.ShowInstrument(str.c_str());
	//该合约详情
	PlatformStru_InstrumentInfo  InstrumentData;
	strcpy(InstrumentData.InstrumentID, str.c_str());
	strcpy(InstrumentData.ExchangeID, "SZS");
	InstrumentData.VolumeMultiple	= 300;	
	InstrumentData.PriceTick		= 0.2;	
	
	//交易刻度	
	std::string strTime = "201409250915201409251130  201409251300201409251515";
	m_DlgQuation.SetTradingScales(str, strTime);
	//设置最新的深度行情

	PlatformStru_DepthMarketData QutoData;
	QutoData.UpperLimitPrice = 2577;
	QutoData.LowerLimitPrice = 2109.2;
	QutoData.PreSettlementPrice = 2334;
	QutoData.HighestPrice = 2366;
	QutoData.LowestPrice  = 2301;
	m_DlgQuation.SetDepthMarketData(str, &QutoData);	
	m_DlgQuation.InitFinish(str);	
}

void COptionQuationTestDlg::OnDestroy()
{	
	CDialog::OnDestroy();
}
bool COptionQuationTestDlg::GetTime(SGetInfo& sGetInfo)
{
	if(sGetInfo.nType == 0)
		sGetInfo.str = "15:15:00";
	return true;
}
void COptionQuationTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
