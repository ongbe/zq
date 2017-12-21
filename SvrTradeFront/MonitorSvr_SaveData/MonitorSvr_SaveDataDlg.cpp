// MonitorSvr_SaveDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonitorSvr_SaveData.h"
#include "MonitorSvr_SaveDataDlg.h"

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


// CMonitorSvr_SaveDataDlg dialog




CMonitorSvr_SaveDataDlg::CMonitorSvr_SaveDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorSvr_SaveDataDlg::IDD, pParent)
	, m_strFullPath(_T(""))
{
	m_bCancel = true;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMonitorSvr_SaveDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	DDX_Text(pDX, IDC_EDIT_PATH, m_strFullPath);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}

BEGIN_MESSAGE_MAP(CMonitorSvr_SaveDataDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CMonitorSvr_SaveDataDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDOK, &CMonitorSvr_SaveDataDlg::OnBnClickedOk)
END_MESSAGE_MAP()

#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
// CMonitorSvr_SaveDataDlg message handlers
void CMonitorSvr_SaveDataDlg::OnDestroy()
{
	SAFE_DELETE(m_WriteData2DBThread);
	CDialog::OnDestroy();
}
BOOL CMonitorSvr_SaveDataDlg::OnInitDialog()
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
	
	ConfigManager::Init();

	SYSTEMTIME st;
	GetSystemTime(&st);

	char localPath[256];
	memset(localPath, 0, 256);
	GetModuleFileName( NULL, localPath, 256);
	std::string filename=localPath;
	size_t splitpos=filename.find_last_of('\\');
	char strDir[256];
	memset(strDir, 0, sizeof(256));
	sprintf(strDir,"%slog\\Monitor%.4d%.2d%.2d.log",filename.substr(0, splitpos+1).c_str(), st.wYear,st.wMonth,st.wDay);
	
	m_strFullPath = strDir;
	UpdateData(false);

	m_dbAccess.InitDB(ConfigManager::Get()->GetDBUser(), 
		ConfigManager::Get()->GetDBPwd(), 
		ConfigManager::Get()->GetDBSID());

	m_Progress.SetRange(0,100);
	m_Progress.SetPos(0);
	m_WriteData2DBThread = new CToolThread(ThreadWriteData2DB,this);

	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
bool CMonitorSvr_SaveDataDlg::CreatTable()
{
	bool bSuccss = false;
	SYSTEMTIME st;
	GetSystemTime(&st);
	char strTable[256];
	memset(strTable, 0, sizeof(256));
	sprintf(strTable,"Monitor%.4d%.2d%.2d", st.wYear,st.wMonth,st.wDay);

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "DROP table %s", strTable);
	if ( !m_dbAccess.Excute(szBuffer))
	{
		int i =0;
	}
	else
	{
		
	}

	sprintf(szBuffer, "create table %s\
					  (\
					  cmdid      INTEGER not null,\
					  seq        INTEGER,\
					  len        INTEGER,\
					  userData1        INTEGER,\
					  userData2        INTEGER,\
					  userData3        INTEGER,\
					  userData4        INTEGER,\
					  utc        INTEGER,\
					  milliSecond        INTEGER,\
					  insertdbtime    DATE)", strTable);
	if ( !m_dbAccess.Excute(szBuffer))
	{
		int i =0;
	}
	else
	{
		bSuccss = true;
	}
	return bSuccss;
}
void CMonitorSvr_SaveDataDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMonitorSvr_SaveDataDlg::OnPaint()
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
HCURSOR CMonitorSvr_SaveDataDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMonitorSvr_SaveDataDlg::OnBnClickedButtonBrowse()
{
	CString   FilePathName;//文件名参数定义
	CFileDialog  Dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"Monitor Files(*.log)|*.log|All Files(*.*)|*.*");
	//打开文件
	if(Dlg.DoModal() == IDCANCEL)
		return;
	if(Dlg.DoModal() == IDOK)//是否打开成功
	{
		FilePathName =  Dlg.GetPathName();//取得文件路径及文件名
		m_strFullPath = FilePathName;
		UpdateData(false);
	}
	else//打开失败处理
	{
		//打开失败处理
		MessageBox("打开失败",NULL,MB_OK);
	}
}

void CMonitorSvr_SaveDataDlg::OnBnClickedOk()
{
	m_bCancel = false;
	m_Progress.SetPos(0);
	GetDlgItem(IDOK)->EnableWindow(false);
}
bool CMonitorSvr_SaveDataDlg::ReadFile(CString strFullPath, std::vector<ServerOrder>& vecServerOrder)
{
	CTextFileRead myfile(strFullPath.GetBuffer(strFullPath.GetLength()));
	if(!myfile.IsOpen())
	{		
		return 0;
	}
	std::string strAll;
	myfile.Read(strAll);
	vector<string> vec;
	split(strAll, '\n', vec);
	for(int i =0; i < (int)vec.size(); i++)
	{		
		ServerOrder order;
		std::string strOne = vec[i];
		vector<string> vecOne;
		split(strOne, ';', vecOne);
		if(vecOne.size() != 9)
			continue;

		std::string strData ="";
		std::string str = vecOne[0];//CmdID:[%d]		
		int nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.cmdid = atoi(strData.c_str());
		}

		str = vecOne[1];//Seq:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.seq = atoi(strData.c_str());
		}

		str = vecOne[2];//Len:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.len = atoi(strData.c_str());
		}

		str = vecOne[3];//userData1:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.userdata1 = atoi(strData.c_str());
		}

		str = vecOne[4];//userData2:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.userdata2 = atoi(strData.c_str());
		}

		str = vecOne[5];//userData3:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.userdata3 = atoi(strData.c_str());
		}	

		str = vecOne[6];//userData4:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.userdata4 = atoi(strData.c_str());
		}			

		str = vecOne[7];//utc:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.nUTCTime = atoi(strData.c_str());
		}		

		str = vecOne[8];//milliSecond:[%d]
		nPos = str.find('[');
		if(nPos != -1)
		{
			std::string strData =	str.substr(nPos+1, str.length()-nPos-2).c_str();
			order.nMiliSecond = atoi(strData.c_str());
		}		


		vecServerOrder.push_back(order);
	}

	return true;
}
bool  CMonitorSvr_SaveDataDlg::SaveData2DB(std::vector<ServerOrder>& vecServerOrder)
{
	

	return true;
}
DWORD WINAPI CMonitorSvr_SaveDataDlg::ThreadWriteData2DB (PVOID pParam)
{
	CMonitorSvr_SaveDataDlg* pMgr=(CMonitorSvr_SaveDataDlg*)pParam;
	pMgr->doProcessBusinessDataMain();
	return 0;
}
void CMonitorSvr_SaveDataDlg::doProcessBusinessDataMain()
{
	while (!m_WriteData2DBThread->IsNeedExit())
	{
		if(!m_bCancel)
		{
			bool bCreate = CreatTable();
			if(bCreate)
			{
				std::vector<ServerOrder> vecServerOrder;
				ReadFile(m_strFullPath,  vecServerOrder);

				for(int i =0; i< (int)vecServerOrder.size(); i++)
				{
					ServerOrder& order = vecServerOrder[i];

					char szBuffer[MAX_SQL_LENGTH];
					memset(szBuffer, 0, sizeof(szBuffer));
					sprintf(szBuffer, "insert into MONITOR20150127(Cmdid, Seq, len, Userdata1, userdata2, USERDATA3, USERDATA4, UTC, MILLISECOND,INSERTDBTIME)\
									  values(%d, %d, %d, %d, %d, %d, %d, %d, %d, sysdate)",  
									  order.cmdid, order.len, order.seq, order.userdata1, order.userdata2,order.userdata3,order.userdata4, order.nUTCTime, order.nMiliSecond);
									  
					if(true == m_dbAccess.Excute(szBuffer))
					{
						char BufHead[512];
						memset(BufHead, 0, sizeof(BufHead));	
						sprintf(BufHead,"存储失败的数据：CmdID:[%d]; Seq:[%d]; Len:[%d]; userData1:[%d]; userData2:[%d]; userData3:[%d]; userData4:[%d]; utc:[%d]; milliSecond:[%d]", 
							order.cmdid,  order.seq, order.len, order.userdata1, order.userdata2,
							order.userdata3, order.userdata4, order.nUTCTime, order.nMiliSecond);
						SaveData("%s", BufHead);
   				    }
					int nPos = (i+1)*100/vecServerOrder.size();
					m_Progress.SetPos(nPos);	
				}	
				m_Progress.SetPos(100);	
				MessageBox(_T("写入数据库完成"), _T("提示"));
			}
			m_bCancel = true;
			GetDlgItem(IDOK)->EnableWindow(true);
		}
		Sleep(2000);
	}
}