// MonitorClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonitorClient.h"
#include "MonitorClientDlg.h"

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


// CMonitorClientDlg dialog




CMonitorClientDlg::CMonitorClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorClientDlg::IDD, pParent)
	, m_staticUpRecv(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bFirstAsk = true;
	
}

void CMonitorClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Recv, m_ListInfo);
	DDX_Text(pDX, IDC_STATIC_UPRecv, m_staticUpRecv);
	DDX_Text(pDX, IDC_STATIC_UPSend, m_staticUpSend);

	DDX_Text(pDX, IDC_STATIC_UpDelay, m_staticUpDelay);
	DDX_Text(pDX, IDC_STATIC_DownRecv, m_staticDownRecv);
	DDX_Text(pDX, IDC_STATIC_DownSend, m_staticDownSend);
	DDX_Text(pDX, IDC_STATIC_DownDelay, m_staticDownDelay);
	DDX_Text(pDX, IDC_STATIC_CPU, m_staticCPU);
	DDX_Text(pDX, IDC_STATIC_Memory, m_staticMemory);
	DDX_Text(pDX, IDC_STATIC_HardDisk, m_staticHardDisk);	
}

BEGIN_MESSAGE_MAP(CMonitorClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMonitorClientDlg::OnBnClickedOk)
	ON_MESSAGE(MESSAGE_GETDATA, OnMessageGetData)	
	ON_MESSAGE(MESSAGE_GETSTATISITIC, OnMessageGetStatic)	
	ON_MESSAGE(MESSAGE_GETSTATISITIC_PUSH, OnMessageGetStatic_Push)	
	ON_MESSAGE(MESSAGE_HEARTBEATRSP, OnFirstHeartBeat)	
END_MESSAGE_MAP()


// CMonitorClientDlg message handlers

BOOL CMonitorClientDlg::OnInitDialog()
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
	CSendData::Get()->Init();
	CClientData::Init();
	m_hWorkerThread =new CToolThread(ThreadFunc, this);

	std::string		UDPSvrIP;
	unsigned short	UDPSvrPort;
	std::string strTimeSpan;
	Load(UDPSvrIP, UDPSvrPort, strTimeSpan);


	

	DWORD dwStyle = m_ListInfo.GetExtendedStyle(); //获取当前扩展样式
	dwStyle |= LVS_EX_FULLROWSELECT; //选中某行使整行高亮（report风格时）
	dwStyle |= LVS_EX_GRIDLINES; //网格线（report风格时）
	//dwStyle |= LVS_EX_CHECKBOXES; //item前生成checkbox控件
	m_ListInfo.SetExtendedStyle(dwStyle); //设置扩展风格


	int nCol = 0;
	m_ListInfo.InsertColumn(nCol++, _T("报单类型"),LVCFMT_LEFT,150,0);
	m_ListInfo.InsertColumn(nCol++, _T("报单Key"),LVCFMT_LEFT,60,0);
	m_ListInfo.InsertColumn(nCol++, _T("报单方向"),LVCFMT_LEFT,60,0);
	m_ListInfo.InsertColumn(nCol++, _T("接收时间"),LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(nCol++, _T("转发时间"),LVCFMT_LEFT,80,0);
	m_ListInfo.InsertColumn(nCol++, _T("延迟时间"),LVCFMT_LEFT,60,0);
	m_ListInfo.InsertColumn(nCol++, _T("报单日期"),LVCFMT_LEFT,80,0);

	int nCol2 = 0;
	HDITEM headerItem;
	headerItem.mask = HDI_LPARAM;
	CHeaderCtrl* pHeaderCtrl = m_ListInfo.GetHeaderCtrl( );
	pHeaderCtrl->GetItem(nCol2, &headerItem);
	headerItem.lParam = CSortListCtrl::DATA_TYPE::STRING_TYPE;
	pHeaderCtrl->SetItem(nCol2++, &headerItem);

	pHeaderCtrl->GetItem(nCol2, &headerItem);
	headerItem.lParam = CSortListCtrl::DATA_TYPE::STRING_TYPE;
	pHeaderCtrl->SetItem(nCol2++, &headerItem);

	pHeaderCtrl->GetItem(nCol2, &headerItem);
	headerItem.lParam = CSortListCtrl::DATA_TYPE::STRING_TYPE;
	pHeaderCtrl->SetItem(nCol2++, &headerItem);

	pHeaderCtrl->GetItem(nCol2, &headerItem);
	headerItem.lParam = CSortListCtrl::DATA_TYPE::STRING_TYPE;
	pHeaderCtrl->SetItem(nCol2++, &headerItem);

	pHeaderCtrl->GetItem(nCol2, &headerItem);
	headerItem.lParam = CSortListCtrl::DATA_TYPE::STRING_TYPE;
	pHeaderCtrl->SetItem(nCol2++, &headerItem);

	pHeaderCtrl->GetItem(nCol2, &headerItem);
	headerItem.lParam = CSortListCtrl::DATA_TYPE::INT_TYPE;
	pHeaderCtrl->SetItem(nCol2++, &headerItem);

	pHeaderCtrl->GetItem(nCol2, &headerItem);
	headerItem.lParam = CSortListCtrl::DATA_TYPE::STRING_TYPE;
	pHeaderCtrl->SetItem(nCol2++, &headerItem);


	//UINT nType = (UINT)(headerItem.lParam);


	CRect rtClient;
	GetDlgItem(IDC_STATIC_UpDownNum)->GetWindowRect(&rtClient);
	ScreenToClient(&rtClient);
	m_UpDownLine.Create(rtClient, this, 1111);
	m_UpDownLine.ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_Delay)->GetWindowRect(&rtClient);
	ScreenToClient(&rtClient);
	m_DelayLine.Create(rtClient, this, 1112);
	m_DelayLine.ShowWindow(SW_SHOW);
	char strTime[128];
	SYSTEMTIME st;
	GetSystemTime(&st);
	memset(strTime, 0, sizeof(strTime));
	sprintf(strTime,"%.4d%.2d%.2d",st.wYear,st.wMonth,st.wDay);

	std::string m_strTradingDay = strTime;

	std::vector<std::pair<std::string,std::string>> TradingTime;

	vector<string> vecOne;
	split(strTimeSpan, ' ', vecOne);
	for(int k =0; k< (int)vecOne.size(); k++)
	{
		std::string strText = vecOne[k];
		vector<string> vec;
		split(strText, '-', vec);
		if(vec.size() ==2)
		{
			TradingTime.push_back(std::make_pair(vec[0], vec[1]));
		}
	}

	//TradingTime.push_back(std::make_pair("09:15", "11:30"));//第一交易时段的起止时间
	//TradingTime.push_back(std::make_pair("13:00", "15:15"));//第二交易时段的起止时间

	std::vector<struct tm> vecTimeScales;
	for(std::vector<std::pair<std::string, std::string>>::const_iterator it = TradingTime.begin(); it != TradingTime.end(); it++)
	{
		std::string strBegin = it->first;
		std::string strEnd   = it->second;		

		struct tm tmScale;
		tmScale.tm_year = atoi(m_strTradingDay.substr(0, 4).c_str()) - 1900;
		tmScale.tm_mon  = atoi(m_strTradingDay.substr(4, 2).c_str()) - 1;
		tmScale.tm_mday = atoi(m_strTradingDay.substr(6, 2).c_str());
		tmScale.tm_hour = atoi(strBegin.substr(0, 2).c_str());
		tmScale.tm_min  = atoi(strBegin.substr(3, 2).c_str());
		tmScale.tm_sec  = 0;		
		vecTimeScales.push_back(tmScale);

		tmScale.tm_hour = atoi(strEnd.substr(0, 2).c_str());
		tmScale.tm_min  = atoi(strEnd.substr(3, 2).c_str());
		tmScale.tm_sec  = 0;		
		vecTimeScales.push_back(tmScale);
	}
	m_UpDownLine.SetScales(vecTimeScales);
	m_DelayLine.SetScales(vecTimeScales);

	double  dbMaxValue =10, dbMinValue =  0.0;
	m_UpDownLine.SetMaxMinValue(dbMaxValue, dbMinValue);
	m_UpDownLine.SetDelay(false);
	m_DelayLine.SetMaxMinValue(dbMaxValue, dbMinValue);
	m_DelayLine.SetDelay(true);


	
	CUDPDataService::GetInstance()->SetMessWnd(m_hWnd);
	CUDPDataService::GetInstance()->Connect(UDPSvrIP, UDPSvrPort);	

	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMonitorClientDlg::Load(std::string& UDPSvrIP, unsigned short& UDPSvrPort, std::string& strTime)
{
	char szLocalPath[256];
	memset(szLocalPath, 0, 256);
	GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath)-1 );

	string strPathFileName( szLocalPath );
	int nPos = strPathFileName.rfind( '\\' );
	if ( nPos == -1) return;
	strPathFileName = strPathFileName.substr( 0, nPos + 1 ) + string("MonitorClientConfig.xml");
	string strValue;	

	TiXmlDocument* pDocument=new TiXmlDocument(strPathFileName.c_str());
	TiXmlDocument& myDocument=*pDocument; 
	bool retval=false;
	if(myDocument.LoadFile())
	{
		TiXmlElement* RootElement=myDocument.RootElement();
		if(RootElement)
		{
			const char* pRootElement = RootElement->Value();
			if(strcmp(pRootElement, "root") ==0)
			{
				TiXmlNode* pNode=RootElement->FirstChild("Server");
				if(pNode)
				{
					const char* pElement = pNode->Value();
					if(strcmp(pElement, "Server") ==0)
					{
						TiXmlElement* pChild=pNode->FirstChildElement("IP");
						if(pChild)
						{
							std::string str = pChild->GetText();
							UDPSvrIP = str;							
						}
						TiXmlElement* pChild2=pNode->FirstChildElement("Port");
						if(pChild)
						{
							std::string str = pChild2->GetText();
							UDPSvrPort = atoi(str.c_str());							
						}
					}
				}	

				TiXmlNode* pNode2=RootElement->FirstChild("TimeSpan");
				if(pNode2)
				{
					TiXmlElement* pChild=pNode2->ToElement();
					if(pChild)
					{
						strTime= pChild->GetText();							
					}					
				}			
			}			
		}
	}
}
void CMonitorClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMonitorClientDlg::OnPaint()
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
HCURSOR CMonitorClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMonitorClientDlg::OnBnClickedOk()
{
	Stru_IPAddress RemoteAddr = CUDPDataService::GetInstance()->GetAddress();
	UINT nCMDID = 1123;
	void* pData = NULL;	
	int nLen =0;
	UINT seq =253;
	bool bNeedLogon = false;

	bool bSend = CUDPDataService::GetInstance()->SendData(RemoteAddr, nCMDID, pData, nLen, seq, bNeedLogon);
	if(bSend)
	{
		int i =0;
	}
	else
	{
		int i =0;
	}
}
int CMonitorClientDlg::OnMessageGetData(RecvData* pData)
{
	if(pData == NULL)
		return 0;
	if(pData->head.cmdid == 0x18020001)
	{//cpu 硬盘 内存 相关
		Stru_Watch *pWatch = (Stru_Watch *)pData->pData;
		if(pWatch)
		{
			CString str;
			str.Format(_T("%02.1f%%"), pWatch->CpuUsage/10.0);
			GetDlgItem(IDC_STATIC_CPU)->SetWindowText(str);

			str.Format(_T("%5I64dM"),  pWatch->Memory/1024/1024);
			GetDlgItem(IDC_STATIC_Memory)->SetWindowText(str);

			str.Format(_T("%5I64dG"), pWatch->DiskSpace/1024/1024/1024);
			GetDlgItem(IDC_STATIC_HardDisk)->SetWindowText(str);

			char BufHead[512];
			memset(BufHead, 0 , sizeof(BufHead));
			sprintf(BufHead,"CPU: %02.1f%%, \tMemory: %5I64dM, \tAvailDiskSpace: %5I64dG\n", pWatch->CpuUsage/10.0, pWatch->Memory/1024/1024, 
				pWatch->DiskSpace/1024/1024/1024);
			MonitorCpu_info("%s", BufHead);
		}
		return 1L;
	}
	else if(pData->head.cmdid == MONITOR_SENDCALCDATA_Rsp)
	{		
		StatisticsData data;
		memcpy(&data, pData->pData, sizeof(StatisticsData));	

		CString str;
		str.Format(_T("%d"), data.dwUpRecv);
		GetDlgItem(IDC_STATIC_UPRecv)->SetWindowText(str);
		str.Format(_T("%d"), data.dwUpSend);
		GetDlgItem(IDC_STATIC_UPSend)->SetWindowText(str);

		str = "0";
		int nMin = data.dwUpRecv;
		if(nMin > data.dwUpSend)
			nMin = data.dwUpSend;
		if(nMin > 0)
			str.Format(_T("%.1f"), 1.0*data.dwUpDelay/nMin);
		GetDlgItem(IDC_STATIC_UpDelay)->SetWindowText(str);

		str.Format(_T("%d"), data.dwDownRecv);
		GetDlgItem(IDC_STATIC_DownRecv)->SetWindowText(str);
		str.Format(_T("%d"), data.dwDownSend);
		GetDlgItem(IDC_STATIC_DownSend)->SetWindowText(str);

		str = "0";
		nMin = data.dwDownRecv;
		if(nMin > data.dwDownSend)
			nMin = data.dwDownSend;
		if(nMin > 0)
			str.Format(_T("%.1f"), 1.0*data.dwDownDelay/nMin);
		GetDlgItem(IDC_STATIC_DownDelay)->SetWindowText(str);		
		return 1L;
	}

	char BufHead[512];
	memset(BufHead, 0, sizeof(BufHead));	
	sprintf(BufHead,"CmdID:[%d]; Seq:[%d]; Len:[%d]; userData1:[%d]; userData2:[%d]; userData3:[%d]; userData4:[%d]; utc:[%d]; milliSecond:[%d]", 
		pData->head.cmdid,  pData->head.seq, pData->head.len, pData->head.userdata1,  pData->head.userdata2, 
		pData->head.userdata3, pData->head.userdata4, pData->head.nUTCTime, pData->head.nMiliSecond);
	if(pData->head.cmdid != 0x18020001)
		MonitorClient_info("%s", BufHead);


	bool bHad = false;//列表里是否已经存在
	ServerOrder order;
	int nRefData = m_ListInfo.GetItemCount();
	int nIndex = 0;//m_ListInfo.GetItemCount();
	std::map<int, int>::iterator it = m_mapSeq2Index.find(pData->head.userdata3);
	if(it == m_mapSeq2Index.end())
	{
		if(pData->head.userdata2 == UPRECEV  || pData->head.userdata2 == UPSEND|| pData->head.userdata2 == DOWNRECEV  || pData->head.userdata2 == DOWNSEND)
		{//发送接收		
			m_mapSeq2Index[pData->head.userdata3] = nRefData;

			order.cmdid = pData->head.cmdid;
			order.len	= pData->head.len;
			order.seq   = pData->head.seq;
			order.userdata1 = pData->head.userdata1;
			order.userdata2 = pData->head.userdata2;
			order.userdata3 = pData->head.userdata3;
			order.userdata4 = pData->head.userdata4;
			order.nUTCTime  = pData->head.nUTCTime;
			order.nMiliSecond = pData->head.nMiliSecond;
			m_mapSeq2ServerOrder[pData->head.userdata3] = order;
		}
	}
	else
	{//得到列表中的序号
		nIndex = it->second;//refdata 值	

		for(int K = 0; K < m_ListInfo.GetItemCount(); K++)
		{
			DWORD dw = m_ListInfo.GetItemData(K);
			if(nIndex == dw)
			{
				nIndex = K;//这个才是真正的序号
				break;
			}

		}


		bHad  = true;

		std::map<int, ServerOrder>::iterator itServer = m_mapSeq2ServerOrder.find(pData->head.userdata3);
		if(itServer != m_mapSeq2ServerOrder.end())
		{
			order = itServer->second;
		}
	}

	CString str;
	if(!bHad)
	{
		std::string  strCMD;
		CmdID2String(pData->head.cmdid, strCMD);
		str.Format(_T("%s"), strCMD.c_str());
		//str.Format(_T("%d"), pData->head.cmdid);//报单类型
		m_ListInfo.InsertItem(nIndex, str);			
		m_ListInfo.SetItemData(nIndex, nRefData);//itemdata
	}	

	str.Format(_T("%d"), pData->head.userdata3);
	m_ListInfo.SetItemText(nIndex, 1, str);//报单key

	if(pData->head.userdata2 == UPRECEV  || pData->head.userdata2 == UPSEND)
	{
		str = _T("上行");//上行
		m_ListInfo.SetItemText(nIndex, 2, str);//报单方向
	}
	else if(pData->head.userdata2 == DOWNRECEV  || pData->head.userdata2 == DOWNSEND)
	{
		str = _T("下行");//下行	
		m_ListInfo.SetItemText(nIndex, 2, str);//报单方向
	}



	if(pData->head.userdata2 == UPRECEV|| pData->head.userdata2 == DOWNRECEV)
	{//接收时间
		time_t dw = (time_t)pData->head.nUTCTime;
		if(dw <0)
			return 0L;
		struct tm dwTime = *localtime(&dw);
		str.Format(_T("%02d:%02d:%02d %d"),  dwTime.tm_hour, dwTime.tm_min, dwTime.tm_sec, pData->head.nMiliSecond );
		m_ListInfo.SetItemText(nIndex, 3, str);
	}
	else if(pData->head.userdata2 == UPSEND|| pData->head.userdata2 == DOWNSEND || pData->head.userdata2 == UPSENDFAIL)
	{//转发时间	
		time_t dw = (time_t)pData->head.nUTCTime;
		if(dw <0)
			return 0L;
		struct tm dwTime = *localtime(&dw);
		str.Format(_T("%02d:%02d:%02d %d"), dwTime.tm_hour, dwTime.tm_min, dwTime.tm_sec, pData->head.nMiliSecond );

		if(pData->head.userdata2 == UPSENDFAIL)
		{//上行发送失败
			//CThostFtdcRspInfoField* RspInfo = (CThostFtdcRspInfoField*)pData->pData;
			//if(RspInfo)
			//{
			//	str.Format(_T("%s"), RspInfo->ErrorMsg);
			//}
		}
		else
			m_ListInfo.SetItemText(nIndex, 4, str);

	}
	if(bHad)
	{//转发延迟
		if(pData->head.userdata2 == UPSEND || pData->head.userdata2 == DOWNSEND)
		{//发送
			int nTime = (pData->head.nUTCTime - order.nUTCTime)*1000 + (pData->head.nMiliSecond - order.nMiliSecond);			
			str.Format(_T("%d"), nTime);
		}
		else if(pData->head.userdata2 == UPRECEV || pData->head.userdata2 == DOWNRECEV)
		{
			int nTime = ( order.nUTCTime- pData->head.nUTCTime)*1000 + (order.nMiliSecond - pData->head.nMiliSecond);			
			str.Format(_T("%d"), nTime);
		}
		else if(pData->head.userdata2 == UPSENDFAIL)
		{//上行发送失败
			CThostFtdcRspInfoField* RspInfo = (CThostFtdcRspInfoField*)pData->pData;
			if(RspInfo)
			{
				str.Format(_T("失败[%s]"), RspInfo->ErrorMsg);				
			}
			//m_ListInfo.SetSelectionMark(nIndex);
			m_ListInfo.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		}
		else if(pData->head.userdata2 == DOWNSENDFAILE)
		{//下行发送失败

		}
		m_ListInfo.SetItemText(nIndex, 5, str);
	}
	//报单日期
	time_t dw = (time_t)pData->head.nUTCTime;
	struct tm dwTime = *localtime(&dw);
	str.Format(_T("%04d/%02d/%02d"),  dwTime.tm_year+1900,  dwTime.tm_mon +1, dwTime.tm_mday);

	m_ListInfo.SetItemText(nIndex, 6, str);

	return 1;
}
LRESULT CMonitorClientDlg::OnMessageGetData(WPARAM wP,LPARAM lP)
{
	RecvData* pData = (RecvData*)wP;
	if(pData == NULL)
		return 0;
	if(pData->head.cmdid == 0x18020001)
	{//cpu 硬盘 内存 相关
		Stru_Watch *pWatch = (Stru_Watch *)pData->pData;
		if(pWatch)
		{
			CString str;
			str.Format(_T("%02.1f%%"), pWatch->CpuUsage/10.0);
			GetDlgItem(IDC_STATIC_CPU)->SetWindowText(str);

			str.Format(_T("%5I64dM"),  pWatch->Memory/1024/1024);
			GetDlgItem(IDC_STATIC_Memory)->SetWindowText(str);
			
			str.Format(_T("%5I64dG"), pWatch->DiskSpace/1024/1024/1024);
			GetDlgItem(IDC_STATIC_HardDisk)->SetWindowText(str);

			char BufHead[512];
			memset(BufHead, 0 , sizeof(BufHead));
			sprintf(BufHead,"CPU: %02.1f%%, \tMemory: %5I64dM, \tAvailDiskSpace: %5I64dG\n", pWatch->CpuUsage/10.0, pWatch->Memory/1024/1024, 
				pWatch->DiskSpace/1024/1024/1024);
			MonitorCpu_info("%s", BufHead);
		}
		return 1L;
	}
	else if(pData->head.cmdid == MONITOR_SENDCALCDATA_Rsp)
	 {		
		 StatisticsData data;
		 memcpy(&data, pData->pData, sizeof(StatisticsData));	
		
		 CString str;
		 str.Format(_T("%d"), data.dwUpRecv);
		 GetDlgItem(IDC_STATIC_UPRecv)->SetWindowText(str);
		 str.Format(_T("%d"), data.dwUpSend);
		 GetDlgItem(IDC_STATIC_UPSend)->SetWindowText(str);

		 str = "0";
		 int nMin = data.dwUpRecv;
		 if(nMin > data.dwUpSend)
			 nMin = data.dwUpSend;
		 if(nMin > 0)
			 str.Format(_T("%.1f"), 1.0*data.dwUpDelay/nMin);
		 GetDlgItem(IDC_STATIC_UpDelay)->SetWindowText(str);

		 str.Format(_T("%d"), data.dwDownRecv);
		 GetDlgItem(IDC_STATIC_DownRecv)->SetWindowText(str);
		 str.Format(_T("%d"), data.dwDownSend);
		 GetDlgItem(IDC_STATIC_DownSend)->SetWindowText(str);

		 str = "0";
		 nMin = data.dwDownRecv;
		 if(nMin > data.dwDownSend)
			 nMin = data.dwDownSend;
		 if(nMin > 0)
			str.Format(_T("%.1f"), 1.0*data.dwDownDelay/nMin);
		 GetDlgItem(IDC_STATIC_DownDelay)->SetWindowText(str);		
		 return 1L;
	 }
	
	char BufHead[512];
	memset(BufHead, 0, sizeof(BufHead));	
	sprintf(BufHead,"CmdID:[%d]; Seq:[%d]; Len:[%d]; userData1:[%d]; userData2:[%d]; userData3:[%d]; userData4:[%d]; utc:[%d]; milliSecond:[%d]", 
		pData->head.cmdid,  pData->head.seq, pData->head.len, pData->head.userdata1,  pData->head.userdata2, 
		pData->head.userdata3, pData->head.userdata4, pData->head.nUTCTime, pData->head.nMiliSecond);
	if(pData->head.cmdid != 0x18020001)
		MonitorClient_info("%s", BufHead);
	

	bool bHad = false;//列表里是否已经存在
	ServerOrder order;
	int nRefData = m_ListInfo.GetItemCount();
	int nIndex = 0;//m_ListInfo.GetItemCount();
	std::map<int, int>::iterator it = m_mapSeq2Index.find(pData->head.userdata3);
	if(it == m_mapSeq2Index.end())
	{
		if(pData->head.userdata2 == UPRECEV  || pData->head.userdata2 == UPSEND|| pData->head.userdata2 == DOWNRECEV  || pData->head.userdata2 == DOWNSEND)
		{//发送接收		
			m_mapSeq2Index[pData->head.userdata3] = nRefData;

			order.cmdid = pData->head.cmdid;
			order.len	= pData->head.len;
			order.seq   = pData->head.seq;
			order.userdata1 = pData->head.userdata1;
			order.userdata2 = pData->head.userdata2;
			order.userdata3 = pData->head.userdata3;
			order.userdata4 = pData->head.userdata4;
			order.nUTCTime  = pData->head.nUTCTime;
			order.nMiliSecond = pData->head.nMiliSecond;
			m_mapSeq2ServerOrder[pData->head.userdata3] = order;
		}
	}
	else
	{//得到列表中的序号
		nIndex = it->second;//refdata 值	

		for(int K = 0; K < m_ListInfo.GetItemCount(); K++)
		{
			DWORD dw = m_ListInfo.GetItemData(K);
			if(nIndex == dw)
			{
				nIndex = K;//这个才是真正的序号
				break;
			}

		}


		bHad  = true;

		std::map<int, ServerOrder>::iterator itServer = m_mapSeq2ServerOrder.find(pData->head.userdata3);
		if(itServer != m_mapSeq2ServerOrder.end())
		{
			order = itServer->second;
		}
	}

	CString str;
	if(!bHad)
	{
		std::string  strCMD;
		CmdID2String(pData->head.cmdid, strCMD);
		str.Format(_T("%s"), strCMD.c_str());
		//str.Format(_T("%d"), pData->head.cmdid);//报单类型
		m_ListInfo.InsertItem(nIndex, str);			
		m_ListInfo.SetItemData(nIndex, nRefData);//itemdata
	}	

	str.Format(_T("%d"), pData->head.userdata3);
	m_ListInfo.SetItemText(nIndex, 1, str);//报单key

	if(pData->head.userdata2 == UPRECEV  || pData->head.userdata2 == UPSEND)
	{
		str = _T("上行");//上行
		m_ListInfo.SetItemText(nIndex, 2, str);//报单方向
	}
	else if(pData->head.userdata2 == DOWNRECEV  || pData->head.userdata2 == DOWNSEND)
	{
		str = _T("下行");//下行	
		m_ListInfo.SetItemText(nIndex, 2, str);//报单方向
	}



	if(pData->head.userdata2 == UPRECEV|| pData->head.userdata2 == DOWNRECEV)
	{//接收时间
		time_t dw = (time_t)pData->head.nUTCTime;
		if(dw <0)
			return 0L;
		struct tm dwTime = *localtime(&dw);
		str.Format(_T("%02d:%02d:%02d %d"),  dwTime.tm_hour, dwTime.tm_min, dwTime.tm_sec, pData->head.nMiliSecond );
		m_ListInfo.SetItemText(nIndex, 3, str);
	}
	else if(pData->head.userdata2 == UPSEND|| pData->head.userdata2 == DOWNSEND || pData->head.userdata2 == UPSENDFAIL)
	{//转发时间	
		time_t dw = (time_t)pData->head.nUTCTime;
		if(dw <0)
			return 0L;
		struct tm dwTime = *localtime(&dw);
		str.Format(_T("%02d:%02d:%02d %d"), dwTime.tm_hour, dwTime.tm_min, dwTime.tm_sec, pData->head.nMiliSecond );

		if(pData->head.userdata2 == UPSENDFAIL)
		{//上行发送失败
			//CThostFtdcRspInfoField* RspInfo = (CThostFtdcRspInfoField*)pData->pData;
			//if(RspInfo)
			//{
			//	str.Format(_T("%s"), RspInfo->ErrorMsg);
			//}
		}
		else
			m_ListInfo.SetItemText(nIndex, 4, str);

	}
	if(bHad)
	{//转发延迟
		if(pData->head.userdata2 == UPSEND || pData->head.userdata2 == DOWNSEND)
		{//发送
			int nTime = (pData->head.nUTCTime - order.nUTCTime)*1000 + (pData->head.nMiliSecond - order.nMiliSecond);			
			str.Format(_T("%d"), nTime);
		}
		else if(pData->head.userdata2 == UPRECEV || pData->head.userdata2 == DOWNRECEV)
		{
			int nTime = ( order.nUTCTime- pData->head.nUTCTime)*1000 + (order.nMiliSecond - pData->head.nMiliSecond);			
			str.Format(_T("%d"), nTime);
		}
		else if(pData->head.userdata2 == UPSENDFAIL)
		{//上行发送失败
			CThostFtdcRspInfoField* RspInfo = (CThostFtdcRspInfoField*)pData->pData;
			if(RspInfo)
			{
				str.Format(_T("失败[%s]"), RspInfo->ErrorMsg);				
			}
			//m_ListInfo.SetSelectionMark(nIndex);
			m_ListInfo.SetItemState(nIndex,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
		}
		else if(pData->head.userdata2 == DOWNSENDFAILE)
		{//下行发送失败

		}
		m_ListInfo.SetItemText(nIndex, 5, str);
	}
	//报单日期
	time_t dw = (time_t)pData->head.nUTCTime;
	struct tm dwTime = *localtime(&dw);
	str.Format(_T("%04d/%02d/%02d"),  dwTime.tm_year+1900,  dwTime.tm_mon +1, dwTime.tm_mday);

	m_ListInfo.SetItemText(nIndex, 6, str);
	
	
	
	return 1L;
}
void CMonitorClientDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == m_uTimer)
	{
		if(m_bFirstAsk)
		{
			Stru_IPAddress RemoteAddr = CUDPDataService::GetInstance()->GetAddress();
			bool bSend = CUDPDataService::GetInstance()->SendData(RemoteAddr, MONITOR_GETSTATISITIC, NULL, 0, 0);	
			if(bSend)
			{
				MonitorClient_info("发送统计数据订阅\n");	
			}
			else
			{
				MonitorClient_info("发送统计数据失败\n");	
			}		
			KillTimer(m_uTimer);
			m_uTimer = SetTimer(1,60000,NULL);
		}
		else
			KillTimer(m_uTimer);
		
	}	
	CDialog::OnTimer(nIDEvent);
}
void CMonitorClientDlg::OnMessageGetStatic(std::vector<StatisticsData>& vecTimeLineData)
{
	m_bFirstAsk = false;
	if(vecTimeLineData.size() == 0)
		return;

	for (int i =0; i< (int)vecTimeLineData.size(); i++)
	{
		StatisticsData& data = vecTimeLineData[i];
		m_mapTime2Statistics[data.tmTime] = data;
	}
	SetLineView(m_mapTime2Statistics);
	return;
}
LRESULT CMonitorClientDlg::OnMessageGetStatic(WPARAM wP,LPARAM lP)
{
	m_bFirstAsk = false;
	std::vector<StatisticsData>& vecTimeLineData = *(std::vector<StatisticsData>*)wP;
	if(vecTimeLineData.size() == 0)
		return 0 ;
	
	for (int i =0; i< (int)vecTimeLineData.size(); i++)
	{
		StatisticsData& data = vecTimeLineData[i];
		m_mapTime2Statistics[data.tmTime] = data;
	}
	SetLineView(m_mapTime2Statistics);
	return 1L;
}
void CMonitorClientDlg::SetLineView(std::map<long, StatisticsData>& mapTime2Statistics)
{
	std::vector<StatData> vecData;
	std::map<long, StatisticsData>::iterator it = mapTime2Statistics.begin();
	while(it != m_mapTime2Statistics.end())
	{
		time_t t = it->first;
		StatisticsData& sData = it->second;

		struct tm local = *localtime(&t);
		StatData statData;
		statData.dwTime = local;
		statData.dwUpRecv = sData.dwUpRecv;
		statData.dwUpSend = sData.dwUpSend;
		statData.dwDownRecv = sData.dwDownRecv;
		statData.dwDownSend = sData.dwDownSend;

		statData.dwDownDelay   = sData.dwDownDelay;
		statData.dwUpDelay  = sData.dwUpDelay;

		vecData.push_back(statData);
		it++;
	}
	m_UpDownLine.SetVectorData(vecData);
	m_DelayLine.SetVectorData(vecData);
}
void CMonitorClientDlg::OnMessageGetStatic_Push(std::vector<StatisticsData>& vecTimeLineData)
{
	if(vecTimeLineData.size() == 0)
		return ;
	int nSize = m_mapTime2Statistics.size();
	for (int i =0; i< (int)vecTimeLineData.size(); i++)
	{
		StatisticsData& data = vecTimeLineData[i];
		m_mapTime2Statistics[data.tmTime] = data;
	}
	if(nSize != m_mapTime2Statistics.size())
	{
		//	AfxMessageBox("实时数据有写入");
	}
	SetLineView(m_mapTime2Statistics);
}
LRESULT CMonitorClientDlg::OnMessageGetStatic_Push(WPARAM wP,LPARAM lP)
{//可以优化处理，vecTimeLineData只有一条数据
	std::vector<StatisticsData>& vecTimeLineData = *(std::vector<StatisticsData>*)wP;
	if(vecTimeLineData.size() == 0)
		return 0 ;
	int nSize = m_mapTime2Statistics.size();
	for (int i =0; i< (int)vecTimeLineData.size(); i++)
	{
		StatisticsData& data = vecTimeLineData[i];
		m_mapTime2Statistics[data.tmTime] = data;
	}
	if(nSize != m_mapTime2Statistics.size())
	{
	//	AfxMessageBox("实时数据有写入");
	}
	SetLineView(m_mapTime2Statistics);
/*
	StatisticsData& data = vecTimeLineData[0];	
*/
	return 1L;
}
LRESULT CMonitorClientDlg::OnFirstHeartBeat(WPARAM wP,LPARAM lP)
{
	m_uTimer = SetTimer(1,500,NULL);
	return 1L;
}

DWORD CMonitorClientDlg::ThreadFunc( void* pData )
{
	CMonitorClientDlg* pWorker = (CMonitorClientDlg*)pData;
	if(pWorker)
		pWorker->Process();
	return 0;
}

void CMonitorClientDlg::Process()
{
	if (NULL==m_hWorkerThread)
	{
		return;
	}
	while(!m_hWorkerThread->IsNeedExit())
	{
		RecvData reqData;
		if (CSendData::Get()->GetMsg(reqData))
		{//从交易端过来数据，转送客户端数据
			if(reqData.head.cmdid == MONITOR_GETSTATISITIC_Rsp)
			{
				std::vector<StatisticsData> vecData;
				int nCount = reqData.head.len/sizeof(StatisticsData);
				for(int i =0; i<nCount; i++)
				{
					StatisticsData timeLineData;
					ZeroMemory(&timeLineData, sizeof(StatisticsData));
					memcpy(&timeLineData, (char*)reqData.pData + i*sizeof(StatisticsData), sizeof(StatisticsData));			
					vecData.push_back(timeLineData);					
				}	
				OnMessageGetStatic(vecData);			
			}
			else if(reqData.head.cmdid == MONITOR_GETSTATISITIC_Push)
			{
				std::vector<StatisticsData> vecData;
				int nCount = reqData.head.len/sizeof(StatisticsData);
				for(int i =0; i<nCount; i++)
				{
					StatisticsData timeLineData;
					ZeroMemory(&timeLineData, sizeof(StatisticsData));
					memcpy(&timeLineData, (char*)reqData.pData + i*sizeof(StatisticsData), sizeof(StatisticsData));
					vecData.push_back(timeLineData);					
				}	
				OnMessageGetStatic_Push(vecData);			
			}			
			else
			{
				if ( reqData.head.len != 0 )
				{
					OnMessageGetData(&reqData);
					//reqData.pData = new char[reqData.head.len];
				//	m_pInstance->m_pStreamCache->GetFrom(reqData.pData, reqData.head.len);					
					//::SendMessage(m_HwndParent, MESSAGE_GETDATA, (WPARAM)(&recvData), recvData.head.len+ sizeof(Stru_UniPkgUDPHead));
				}
			}

		}
		else
		{
			//空闲时自动执行动作
			Sleep(10);
			continue;
		}
	}
}