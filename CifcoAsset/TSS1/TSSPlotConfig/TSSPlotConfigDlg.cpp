// TSSPlotConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TSSPlotConfig.h"
#include "TSSPlotConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTSSPlotConfigDlg 对话框




CTSSPlotConfigDlg::CTSSPlotConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTSSPlotConfigDlg::IDD, pParent)
	, m_fMaxLose(0)
	, m_nForceCloseTimer(0)
	, m_nCancelForceTimer(0)
	, m_nUpTick(0)
	, m_nLongColor(0)
	, m_nShortColor(0)
	, m_nNoneColor(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTSSPlotConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDPLOT, m_gridPlotCtrl);             // associate the grid window with a C++ object
	DDX_Text(pDX, IDC_EDTMAXLOSE, m_fMaxLose);
	DDV_MinMaxDouble(pDX, m_fMaxLose, 0.0, 999999999999.0);
	DDX_Text(pDX, IDC_EDTFORCECLOSETIMER, m_nForceCloseTimer);
	DDV_MinMaxUInt(pDX, m_nForceCloseTimer, 0, 999999);
	DDX_Text(pDX, IDC_EDTCANCELCLOSETIMER, m_nCancelForceTimer);
	DDV_MinMaxUInt(pDX, m_nCancelForceTimer, 0, 999999);
	DDX_Text(pDX, IDC_EDTUPTICK, m_nUpTick);
	DDV_MinMaxInt(pDX, m_nUpTick, -1000, 1000);
	DDX_Control(pDX, IDC_SCRLUPPOINT, m_ctlMyHScrollBar);
	DDX_Text(pDX, IDC_EDT_LONGCOLOR, m_nLongColor);
	DDX_Text(pDX, IDC_EDT_SHORTCOLOR, m_nShortColor);
	DDX_Text(pDX, IDC_EDT_NONECOLOR, m_nNoneColor);
}

BEGIN_MESSAGE_MAP(CTSSPlotConfigDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CTSSPlotConfigDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CTSSPlotConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_CLEAR, &CTSSPlotConfigDlg::OnBnClickedClear)
	ON_BN_CLICKED(ID_READFILE, &CTSSPlotConfigDlg::OnBnClickedReadfile)
	ON_BN_CLICKED(ID_SAVEFILE, &CTSSPlotConfigDlg::OnBnClickedSavefile)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CTSSPlotConfigDlg 消息处理程序

BOOL CTSSPlotConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_ctlMyHScrollBar.SetScrollRange(0, 100);

	m_gridPlotCtrl.SetDefCellHeight(20);
	m_gridPlotCtrl.SetDefCellMargin(1);
	m_gridPlotCtrl.SetEditable(TRUE);
	m_gridPlotCtrl.EnableSelection(TRUE);
	m_gridPlotCtrl.SetListMode(TRUE);
	m_gridPlotCtrl.SetSingleRowSelection(TRUE);
	m_gridPlotCtrl.SetFixedColumnSelection(FALSE);
	m_gridPlotCtrl.EnableDragAndDrop(FALSE);
	m_gridPlotCtrl.SetColumnResize(TRUE);
	m_gridPlotCtrl.SetHeaderSort(FALSE);
	m_gridPlotCtrl.SetTrackFocusCell(FALSE);
	m_gridPlotCtrl.SetShowFocusRect(TRUE);
	m_gridPlotCtrl.SetBkColor(RGB(255, 255, 255));
	
	m_gridPlotCtrl.SetColumnCount(6);
	m_gridPlotCtrl.SetRowCount(1+MAXROW);
	m_gridPlotCtrl.SetFixedRowCount(1);

	m_gridPlotCtrl.SetColumnWidth(0, 100);
	m_gridPlotCtrl.SetColumnWidth(1, 80);
	m_gridPlotCtrl.SetColumnWidth(2, 80);
	m_gridPlotCtrl.SetColumnWidth(3, 80);
	m_gridPlotCtrl.SetColumnWidth(4, 80);
	m_gridPlotCtrl.SetColumnWidth(5, 100);
	m_gridPlotCtrl.SetItemText(0, 0, "策略名称");
	m_gridPlotCtrl.SetItemText(0, 1, "合约");
	m_gridPlotCtrl.SetItemText(0, 2, "止盈点位");
	m_gridPlotCtrl.SetItemText(0, 3, "止损点位");
	m_gridPlotCtrl.SetItemText(0, 4, "最大手数");
	m_gridPlotCtrl.SetItemText(0, 5, "持仓时间(秒)");
	m_gridPlotCtrl.Invalidate();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTSSPlotConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTSSPlotConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTSSPlotConfigDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTSSPlotConfigDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CTSSPlotConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CTSSPlotConfigDlg::OnBnClickedClear()
{
	// TODO: Add your control notification handler code here

	m_fMaxLose = 0.0;
	m_nForceCloseTimer = 0;
	m_nCancelForceTimer = 0;
	m_nUpTick = 0;
	UpdateData(FALSE);

	for(int i=1; i<=MAXROW; i++) {
		m_gridPlotCtrl.SetItemText(i, 0, "");
		m_gridPlotCtrl.SetItemText(i, 1, "");
		m_gridPlotCtrl.SetItemText(i, 2, "");
		m_gridPlotCtrl.SetItemText(i, 3, "");
		m_gridPlotCtrl.SetItemText(i, 4, "");
		m_gridPlotCtrl.SetItemText(i, 5, "");
	}
	m_gridPlotCtrl.Invalidate();
}

void CTSSPlotConfigDlg::OnBnClickedReadfile()
{
	// TODO: Add your control notification handler code here
	CFile file;

	OnBnClickedClear();
	if(file.Open(CONFIGFILENAME, CFile::modeRead|CFile::shareDenyNone)) {
		
		PLOTCTRLHEADCFG head;
		ZeroMemory(&head, sizeof(head));
		file.Read(&head, sizeof(head));
		m_fMaxLose = head.maxLoseForDay;
		m_nForceCloseTimer = head.forceCloseTime;
		m_nCancelForceTimer = head.cancelForceTime;
		m_nUpTick = head.fcPriceUp;
		m_nLongColor = head.dwLongColor;
		m_nShortColor = head.dwShortColor;
		m_nNoneColor = head.dwNoneColor;
		//head.dwLimitAllColor = 0;

		UpdateData(FALSE);

		int nCount = 0;
		CString strText;
		PLOTCTRLITEMCFG item;
		ZeroMemory(&item, sizeof(item));
		while(file.Read(&item, sizeof(item))==sizeof(item)) {
			nCount++;
			m_gridPlotCtrl.SetItemText(nCount, 0, item.plotID);
			m_gridPlotCtrl.SetItemText(nCount, 1, item.inst);
			strText.Format("%u", item.stopGainPoint);
			m_gridPlotCtrl.SetItemText(nCount, 2, strText);
			strText.Format("%u", item.stopLosePoint);
			m_gridPlotCtrl.SetItemText(nCount, 3, strText);
			strText.Format("%u", item.maxPosiVol);
			m_gridPlotCtrl.SetItemText(nCount, 4, strText);
			strText.Format("%u", item.singlePosiTime);
			m_gridPlotCtrl.SetItemText(nCount, 5, strText);
		}
		m_gridPlotCtrl.Invalidate();

		file.Close();
	}
}

void CTSSPlotConfigDlg::OnBnClickedSavefile()
{
	// TODO: Add your control notification handler code here

	CString strText;
	int nCount = 0, nLen = 0, nIndex = 0;
	BOOL bHasInvalidateData = FALSE;


	CString strCol1, strCol2, strCol3, strCol4, strCol5, strCol6;
	for(int i=1; i<=MAXROW; i++) {

		strCol1 = m_gridPlotCtrl.GetItemText(i, 0);
		strCol1.Trim();

		strCol2 = m_gridPlotCtrl.GetItemText(i, 1);
		strCol2.Trim();
		
		strCol3 = m_gridPlotCtrl.GetItemText(i, 2);
		strCol3.Trim();

		strCol4 = m_gridPlotCtrl.GetItemText(i, 3);
		strCol4.Trim();

		strCol5 = m_gridPlotCtrl.GetItemText(i, 4);
		strCol5.Trim();

		strCol6 = m_gridPlotCtrl.GetItemText(i, 5);
		strCol6.Trim();

		nLen = strCol1.GetLength() + strCol2.GetLength() + 
				strCol3.GetLength() + strCol4.GetLength() + 
				strCol5.GetLength() + strCol6.GetLength();
		if(nLen>0) {
			if(strCol1.GetLength()==0 || strCol2.GetLength()==0 || 
					strCol3.GetLength()==0 || strCol4.GetLength()==0 || 
					strCol5.GetLength()==0 || strCol6.GetLength()==0) {
					bHasInvalidateData = TRUE;
					nIndex = i;
					break;
			}
		}
	}

	if(bHasInvalidateData) {
		CPoint pos;
		m_gridPlotCtrl.GetCellOrigin(nIndex, 0, &pos);
		pos.x+=1;
		pos.y+=1;
		m_gridPlotCtrl.OnLButtonDown(pos);
		AfxMessageBox("当前数据行存在空项，请处理后在保存数据。", MB_OK|MB_ICONERROR);
		return;
	}

	for(int i=1; i<=MAXROW; i++) {

		strCol1 = m_gridPlotCtrl.GetItemText(i, 0);
		strCol1.Trim();
		if(strCol1.GetLength()==0)
			continue;

		strCol2 = m_gridPlotCtrl.GetItemText(i, 1);
		strCol2.Trim();
		if(strCol2.GetLength()==0)
			continue;
		
		strCol3 = m_gridPlotCtrl.GetItemText(i, 2);
		strCol3.Trim();
		if(strCol3.GetLength()==0)
			continue;

		strCol4 = m_gridPlotCtrl.GetItemText(i, 3);
		strCol4.Trim();
		if(strCol4.GetLength()==0)
			continue;

		strCol5 = m_gridPlotCtrl.GetItemText(i, 4);
		strCol5.Trim();
		if(strCol5.GetLength()==0)
			continue;

		strCol6 = m_gridPlotCtrl.GetItemText(i, 5);
		strCol6.Trim();
		if(strCol6.GetLength()==0)
			continue;
		
		nCount++;
	}

	if(nCount == 0) {
		AfxMessageBox("可保存项为0，不能执行保存！", MB_OK|MB_ICONERROR);
		return;
	}

	CFile file;

	if(!UpdateData()) 
		return;

	if(file.Open(CONFIGFILENAME, CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive)) {
		
		PLOTCTRLHEADCFG head;
		ZeroMemory(&head, sizeof(head));
		head.maxLoseForDay = m_fMaxLose;
		head.forceCloseTime = m_nForceCloseTimer;
		head.cancelForceTime = m_nCancelForceTimer;
		head.fcPriceUp = m_nUpTick;
		head.dwLongColor = m_nLongColor;
		head.dwShortColor = m_nShortColor;
		head.dwNoneColor = m_nNoneColor;
		head.dwLimitAllColor = 0;
		
		file.Write(&head, sizeof(head));

		PLOTCTRLITEMCFG item;
		for(int i=1; i<=MAXROW; i++) {
			ZeroMemory(&item, sizeof(item));

			strText = m_gridPlotCtrl.GetItemText(i, 0);
			strText.Trim();
			strText.MakeLower();
			if(strText.GetLength()==0)
				continue;
			strncpy(item.plotID, strText, sizeof(item.plotID)-1);

			strText = m_gridPlotCtrl.GetItemText(i, 1);
			strText.Trim();
			if(strText.GetLength()==0)
				continue;
			strncpy(item.inst, strText, sizeof(item.inst)-1);
			
			strText = m_gridPlotCtrl.GetItemText(i, 2);
			strText.Trim();
			if(strText.GetLength()==0)
				continue;
			item.stopGainPoint = atoi(strText);

			strText = m_gridPlotCtrl.GetItemText(i, 3);
			strText.Trim();
			if(strText.GetLength()==0)
				continue;
			item.stopLosePoint = atoi(strText);

			strText = m_gridPlotCtrl.GetItemText(i, 4);
			strText.Trim();
			if(strText.GetLength()==0)
				continue;
			item.maxPosiVol = atoi(strText);

			strText = m_gridPlotCtrl.GetItemText(i, 5);
			strText.Trim();
			if(strText.GetLength()==0)
				continue;
			item.singlePosiTime = atoi(strText);
			
			file.Write(&item, sizeof(item));

			head.plotItemCount++;
		}
		file.SeekToBegin();
		file.Write(&head, sizeof(head));

		file.Close();
	}
}

void CTSSPlotConfigDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	if(nSBCode == SB_LINEUP && pScrollBar->m_hWnd==m_ctlMyHScrollBar.m_hWnd) {//pScrollBar->GetDlgCtrlID()==IDC_SCRLUPPOINT) {
		ChangeUpPoint(1);
	}
	else if(nSBCode == SB_LINEDOWN && pScrollBar->m_hWnd==m_ctlMyHScrollBar.m_hWnd) {//pScrollBar->GetDlgCtrlID()==IDC_SCRLUPPOINT) {
		ChangeUpPoint(-1);
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CTSSPlotConfigDlg::ChangeUpPoint(int nPos)
{
	CString strVal;
	GetDlgItemText(IDC_EDTUPTICK, strVal);
	int nVal = atoi(strVal);
	nVal+=nPos;
#if 0
	if(nVal<-1000)
		nVal = -1000;
#else
	if(nVal<0)
		nVal = 0;
#endif
	else if(nVal > 1000)
		nVal = 1000;
	strVal.Format("%u", nVal);
	SetDlgItemText(IDC_EDTUPTICK, strVal);
}
