// AnalyseOrderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AnalyseOrder.h"
#include "AnalyseOrderDlg.h"
#include <map>
#include <string>

using namespace std;

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


// CAnalyseOrderDlg 对话框




CAnalyseOrderDlg::CAnalyseOrderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalyseOrderDlg::IDD, pParent)
	, m_strTradingDay(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAnalyseOrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_TRADINGDAY, m_strTradingDay);
	DDV_MaxChars(pDX, m_strTradingDay, 8);
}

BEGIN_MESSAGE_MAP(CAnalyseOrderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CAnalyseOrderDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAnalyseOrderDlg 消息处理程序

BOOL CAnalyseOrderDlg::OnInitDialog()
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

	CString strDate;
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);
	strDate.Format("%04d%02d%02d", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	m_strTradingDay = strDate;
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAnalyseOrderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAnalyseOrderDlg::OnPaint()
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
HCURSOR CAnalyseOrderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAnalyseOrderDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(!UpdateData())
		return;

	map<string, ORDERANALYSE> mapData;
	map<string, ORDERANALYSE>::iterator it;
	CString strInputText("\r\n"), strFirstText("\r\n");
	CString strFileInput("userdata\\"), strFileFirst("userdata\\"), strFileCtp("userdata\\"), strFileResult("userdata\\");

	strFileInput += m_strTradingDay;
	strFileFirst += m_strTradingDay;
	strFileCtp += m_strTradingDay;
	strFileResult += m_strTradingDay;

	strFileInput += "-input.dat";
	strFileFirst += "-first.dat";
	strFileCtp += "-ctp.dat";
	strFileResult += "-result.txt";

	CFile file;
	int nInputCount = 0, nFirstCount = 0, nCtpCount = 0;
	CString strText;
	ORDERTIME item = {0};
	ORDERANALYSE oana = {0};

	if(file.Open(strFileInput, CFile::modeRead | CFile::shareDenyNone)) {
		nInputCount = file.GetLength()/sizeof(ORDERTIME);
		while(file.Read(&item, sizeof(item))) {
			ZeroMemory(&oana, sizeof(oana));
			oana.FrontID = item.FrontID;
			oana.SessionID = item.SessionID;
			strncpy(oana.OrderRef, item.OrderRef, sizeof(oana.OrderRef));
			strncpy(oana.BrokerID, item.BrokerID, sizeof(oana.BrokerID));
			strncpy(oana.InvestorID, item.InvestorID, sizeof(oana.InvestorID));
			oana.timeinput = item.DateTime;
			strText.Format("%d,%d,%s", item.FrontID, item.SessionID, item.OrderRef);
			mapData.insert(make_pair(string(strText), oana));
			strText.Format("Input报单:  %s,%d,%d  \t%02d:%02d:%02d.%03d\r\n", 
					item.OrderRef, item.FrontID, item.SessionID, 
					item.DateTime.wHour, item.DateTime.wMinute, item.DateTime.wSecond, 
					item.DateTime.wMilliseconds);
			strInputText += strText;
		}
		file.Close();
	}
	else {
		AfxMessageBox("无法打开交易日["+m_strTradingDay+"]的输入文件");
		return;
	}

	if(file.Open(strFileFirst, CFile::modeRead | CFile::shareDenyNone)) {
		nFirstCount = file.GetLength()/sizeof(ORDERTIME);
		while(file.Read(&item, sizeof(item))) {
			strText.Format("%d,%d,%s", item.FrontID, item.SessionID, item.OrderRef);
			it = mapData.find(string(strText));
			if(it != mapData.end()) {
				it->second.timefirst = item.DateTime;
			}
			strText.Format("Server报单:  %s,%d,%d  \t%02d:%02d:%02d.%03d\r\n", 
					item.OrderRef, item.FrontID, item.SessionID, 
					item.DateTime.wHour, item.DateTime.wMinute, item.DateTime.wSecond, 
					item.DateTime.wMilliseconds);
			strFirstText += strText;
		}
		file.Close();
	}

	if(file.Open(strFileCtp, CFile::modeRead | CFile::shareDenyNone)) {
		nCtpCount = file.GetLength()/sizeof(ORDERTIME);
		while(file.Read(&item, sizeof(item))) {
			strText.Format("%d,%d,%s", item.FrontID, item.SessionID, item.OrderRef);
			it = mapData.find(string(strText));
			if(it != mapData.end()) {
				it->second.timectp = item.DateTime;
			}
		}
		file.Close();
	}

	if(file.Open(strFileResult, CFile::modeCreate | CFile::modeWrite)) {
		
		int nSumFirst = 0, nSumCtp = 0;
		int nSumFirstError = 0, nSumCtpError = 0;
		int nMaxFirst = 0, nMaxCtp = 0;
		int nSumFirst_Over1S = 0, nSumCtp_Over1S = 0;
		int nFirstCount_Over1S = 0, nCtpCount_Over1S = 0;

		strText.Format("报单返回时间分析报告\r\n报单Key(FrontID,SessionID,OrderRef), 时间单位：毫秒\r\n\r\n");
		file.Write(strText, strText.GetLength());
		for(it = mapData.begin(); it != mapData.end(); it++) {
			int nTime1 = -1, nTime2 = -1;
			int nTimeFist = -1, nTimeCtp = -1;

			oana = it->second;
			TRACE(	"FrontID = %d, SessionID = %d, OrderRef = %s\n"
					"Input  Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n"
					"Server Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n"
					"Ctp    Time: %04d-%02d-%02d %02d:%02d:%02d.%03d\n\n", 
					oana.FrontID, oana.SessionID, oana.OrderRef, 
					oana.timeinput.wYear, oana.timeinput.wMonth, oana.timeinput.wDay, 
					oana.timeinput.wHour, oana.timeinput.wMinute, oana.timeinput.wSecond, 
					oana.timeinput.wMilliseconds, 
					oana.timefirst.wYear, oana.timefirst.wMonth, oana.timefirst.wDay, 
					oana.timefirst.wHour, oana.timefirst.wMinute, oana.timefirst.wSecond, 
					oana.timefirst.wMilliseconds, 
					oana.timectp.wYear, oana.timectp.wMonth, oana.timectp.wDay, 
					oana.timectp.wHour, oana.timectp.wMinute, oana.timectp.wSecond, 
					oana.timectp.wMilliseconds);

			if(oana.timefirst.wYear>0) {
				nTime2 = (oana.timefirst.wHour*3600 + oana.timefirst.wMinute*60
							+oana.timefirst.wSecond)*1000 + oana.timefirst.wMilliseconds;
				nTime1 = (oana.timeinput.wHour*3600 + oana.timeinput.wMinute*60
							+oana.timeinput.wSecond)*1000 + oana.timeinput.wMilliseconds;
				nSumFirst += nTime2-nTime1;
				nTimeFist = nTime2-nTime1;
				if(nMaxFirst < nTimeFist)
					nMaxFirst = nTimeFist;
				if(nTimeFist > 1000) {
					nSumFirst_Over1S += nTimeFist;
					nFirstCount_Over1S++;
				}
			}
			else
				nSumFirstError++;
			if(oana.timectp.wYear>0) {
				nTime2 = (oana.timectp.wHour*3600 + oana.timectp.wMinute*60
							+oana.timectp.wSecond)*1000 + oana.timectp.wMilliseconds;
				nTime1 = (oana.timeinput.wHour*3600 + oana.timeinput.wMinute*60
							+oana.timeinput.wSecond)*1000 + oana.timeinput.wMilliseconds;
				nSumCtp += nTime2-nTime1;
				nTimeCtp = nTime2-nTime1;
				if(nMaxCtp < nTimeCtp)
					nMaxCtp = nTimeCtp;
				if(nTimeCtp > 1000) {
					nSumCtp_Over1S += nTimeCtp;
					nCtpCount_Over1S++;
				}
			}
			else
				nSumCtpError++;
			strText.Format("报单(%d,%d,%s) 服务器首次返回时间: %d, CTP首次返回时间: %d\r\n", 
					oana.FrontID, oana.SessionID, oana.OrderRef, 
					nTimeFist, nTimeCtp);
			file.Write(strText, strText.GetLength());
		}
		strText.Format("\r\n下单数量: %d, 服务器回报数量: %d, CTP回报数量: %d\r\n", 
				nInputCount, nFirstCount, nCtpCount);
		file.Write(strText, strText.GetLength());
		strText.Format("服务器无返回数: %d, CTP无返回数: %d\r\n", 
				nInputCount-nFirstCount, nInputCount-nCtpCount);
		file.Write(strText, strText.GetLength());
		strText.Format("服务器平均返回时间: %d, 最大返回时间: %d; CTP平均返回时间: %d, CTP最大返回时间: %d\r\n", 
				nFirstCount>0?nSumFirst/nFirstCount:0, nMaxFirst, nCtpCount>0?nSumCtp/nCtpCount:0, nMaxCtp);
		file.Write(strText, strText.GetLength());
		strText.Format("大于1秒的服务器回报数量: %d, 累计时间: %d, 平均返回时间: %d\r\n", 
				nFirstCount_Over1S, nSumFirst_Over1S, nFirstCount_Over1S>0?nSumFirst_Over1S/nFirstCount_Over1S:0);
		file.Write(strText, strText.GetLength());
		strText.Format("大于1秒的CTP回报数量: %d, 累计时间: %d, 平均返回时间: %d\r\n", 
				nCtpCount_Over1S, nSumCtp_Over1S, nCtpCount_Over1S>0?nSumCtp_Over1S/nCtpCount_Over1S:0);
		file.Write(strText, strText.GetLength());

		strInputText += "\r\n";
		strFirstText += "\r\n";
		file.Write(strInputText, strInputText.GetLength());
		file.Write(strFirstText, strFirstText.GetLength());

		file.Close();
	}
	

	//OnOK();
}
