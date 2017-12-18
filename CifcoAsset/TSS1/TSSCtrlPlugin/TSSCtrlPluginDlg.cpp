// TSSCtrlPluginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TSSCtrlPlugin.h"
#include "TSSCtrlPluginDlg.h"

#include "TcpLayer.h"
#include "inc\CommonPkg.h"
#include "ConfigMgr.h"

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


// CTSSCtrlPluginDlg 对话框

UINT nITFCMDARRIVE = ::RegisterWindowMessage("Modlule-TSSCtrlPlugin ApiMsg Arrive");

CTSSCtrlPluginDlg* g_poMainFrame = NULL;

int CTSSCtrlPluginDlg::CallbackRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId)
{

	if(nCmdId >= CMDID_CONSOLE_MIN && nCmdId <= CMDID_CONSOLE_MAX)
	{
		//if(nCmdId==CMDID_CONSOLE_CLOSE)
		//	AfxMessageBox("nCmdId==CMDID_CONSOLE_CLOSE");
	}
	else 
	{
#ifdef _DEBUG
		//if(nCmdId==CMDID_INSERT_ORDER_VERIFY_ORDER_PUSH)
		//	AfxMessageBox("nCmdId==CMDID_INSERT_ORDER_VERIFY_ORDER_PUSH");
		//if(nCmdId==CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP");
		//else if(nCmdId==CMDID_ACCT_POSITION_INFO_LIST_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ACCT_POSITION_INFO_LIST_RSP");
		//else if(nCmdId==CMDID_ACCT_POSITION_DETAIL_LIST_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ACCT_POSITION_DETAIL_LIST_RSP");
		//if(nCmdId==CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ALL_ORDER_WAIT_ORDER_PRICE_RSP");
		//if(nCmdId==CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ALL_ORDER_WAIT_ORDER_NUM_RSP");
		//else if(nCmdId==CMDID_CONFIG_CHANGE)
		//	AfxMessageBox("nCmdId==CMDID_CONFIG_CHANGE");
		//else if(nCmdId==CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP)
		//	AfxMessageBox("nCmdId==CMDID_CONFIG_ENABLE_MOUSE_INPUT_INST_RSP");
		//else if(nCmdId==CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP)
		//	AfxMessageBox("nCmdId==CMDID_CONFIG_ENABLE_MOUSE_INPUT_VOLUME_RSP");
		//else if(nCmdId==CMDID_CONFIG_INPUT_VOLUMES_RSP)
		//	AfxMessageBox("nCmdId==CMDID_CONFIG_INPUT_VOLUMES_RSP");
		//else if(nCmdId==CMDID_ALL_ORDER_LIST_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ALL_ORDER_LIST_RSP");
		//else if(nCmdId==CMDID_ALL_ORDER_INFO_RSP)
		//	AfxMessageBox("nCmdId==CMDID_ALL_ORDER_INFO_RSP");

		//if(nCmdId==CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP)
		//	g_poMainFrame->ShowMsgLog(CString("CallbackRecvApiMsg() nCmdId == CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP"));

#endif
			//case CMDID_QUOTE_CURRENT_SEL_RSP:
		switch(nCmdId) 
		{
			case CMDID_EXCHANGE_LIST_RSP:
			case CMDID_INSTRUMENT_LIST_RSP:

			case CMDID_INSTRUMENT_INFO_RSP:
			case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
			case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
			case CMDID_ACCT_POSITION_INFO_LIST_RSP:
			case CMDID_ACCT_POSITION_DETAIL_LIST_RSP:
			case CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP:
			case CMDID_ALL_POSITION_DETAIL_OPEN_TIME_LIST_RSP:
			case CMDID_INSERT_ORDER_VERIFY_ORDER_PUSH:
			case CMDID_QUOTE_SNAPSHOT_RSP:
			case CMDID_QUOTE_SEND_QUOTE:
            case CMDID_ALL_ORDER_LIST_RSP:
            case CMDID_ALL_ORDER_INFO_RSP:
			case CMDID_ORDER_ORDER_STATUS_CHANGE:
            case CMDID_ORDER_INSERT_ORDER_RSP:
			case CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY_RSP:
            case CMDID_ORDER_CANCEL_ORDER_RSP:
            case CMDID_ORDER_RSP_ORDER_ACTION1:				
            case CMDID_ORDER_RSP_ORDER_ACTION2:	

			// 增加下单审核的命令字处理
			// 增加执行强制下单审核命令字
			// 增加撤销强制下单审核命令字

				break;
			default:
				return -1;
				break;
		}
	}

	ITFCMDATA* pITFData = new ITFCMDATA;
	ZeroMemory(pITFData, sizeof(ITFCMDATA));
	pITFData->nSeqNum = nRequestId;
	pITFData->nCmd = nCmdId;
	pITFData->nErrorCode = nErrorCode;
	pITFData->nDataLen = nDataLen;
	if(nDataLen>0) {
		BYTE* pNewData = new BYTE[nDataLen];
		memcpy(pNewData, pData, nDataLen);
		pITFData->pData = pNewData;
	}
	if(g_poMainFrame!=NULL && nITFCMDARRIVE!=0) {
		g_poMainFrame->PostMessage(nITFCMDARRIVE, sizeof(ITFCMDATA), (LPARAM)pITFData);
	}
	else {
		delete pITFData;
	}
	return 0;
}


CTSSCtrlPluginDlg::CTSSCtrlPluginDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CTSSCtrlPluginDlg::IDD, pParent)
	: CTSSCtrlPluginOperations(CTSSCtrlPluginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bExitApp = FALSE;
}

void CTSSCtrlPluginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRIDPLOTCTRL, m_gridPlotCtrl);             // associate the grid window with a C++ object
	DDX_Control(pDX, IDC_GRIDPOSITION, m_gridPosition);             // associate the grid window with a C++ object
	DDX_Control(pDX, IDC_GRIDPOSIDETAIL, m_gridPosiDetail);             // associate the grid window with a C++ object
	DDX_Control(pDX, IDC_TABPOSITION, m_ctlTabPosition);
	DDX_Control(pDX, IDC_RCHMSGTEXT, m_ctlMsgText);
}

BEGIN_MESSAGE_MAP(CTSSCtrlPluginDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTSSCtrlPluginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTSSCtrlPluginDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_TABPOSITION, &CTSSCtrlPluginDlg::OnNMClickTabposition)
END_MESSAGE_MAP()


// CTSSCtrlPluginDlg 消息处理程序

BOOL CTSSCtrlPluginDlg::OnInitDialog()
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

	CString strText;
	strText.Format("日内亏损限额: %0.2f, 强平计时: %u秒, 整固计时: %u秒", 
			CConfigManager::GetPlotCtrlHead().maxLoseForDay, 
			CConfigManager::GetPlotCtrlHead().forceCloseTime,
			CConfigManager::GetPlotCtrlHead().cancelForceTime);
	SetDlgItemText(IDC_LBLLIMITLOSE, strText);

	g_poMainFrame = this;

	m_ctlTabPosition.InsertItem(0, "持仓明细");
	m_ctlTabPosition.InsertItem(1, "持仓");
	m_ctlTabPosition.SetCurSel(0);

	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi==NULL) {
		poPlusinApi = PlusinAPI::GetInstance();
		SetPlusinAPI(poPlusinApi);
	}
	poPlusinApi->Init("TSSCtrl", "123", CallbackRecvApiMsg, GetTargetHWnd(), NEED_NO_POS_NO_BIND, GetSafeHwnd());
	poPlusinApi->Logon();

	m_gridPlotCtrl.SetDefCellHeight(20);
	m_gridPlotCtrl.SetDefCellMargin(1);
	m_gridPlotCtrl.SetEditable(FALSE);
	m_gridPlotCtrl.EnableSelection(TRUE);
	m_gridPlotCtrl.SetListMode(TRUE);
	m_gridPlotCtrl.SetSingleRowSelection(TRUE);
	m_gridPlotCtrl.SetFixedColumnSelection(FALSE);
	m_gridPlotCtrl.EnableDragAndDrop(FALSE);
	m_gridPlotCtrl.SetColumnResize(TRUE);
	m_gridPlotCtrl.SetHeaderSort(FALSE);
	m_gridPlotCtrl.SetTrackFocusCell(FALSE);
	m_gridPlotCtrl.SetShowFocusRect(FALSE);
	m_gridPlotCtrl.SetBkColor(RGB(255, 255, 255));
	
	m_gridPlotCtrl.SetColumnCount(7);
	m_gridPlotCtrl.SetRowCount(1);
	m_gridPlotCtrl.SetFixedRowCount(1);

	m_gridPlotCtrl.SetColumnWidth(0, 90);
	m_gridPlotCtrl.SetColumnWidth(1, 60);
	m_gridPlotCtrl.SetColumnWidth(2, 70);
	m_gridPlotCtrl.SetColumnWidth(3, 70);
	m_gridPlotCtrl.SetColumnWidth(4, 70);
	m_gridPlotCtrl.SetColumnWidth(5, 70);
	m_gridPlotCtrl.SetColumnWidth(6, 100);
	m_gridPlotCtrl.SetItemText(0, 0, "策略名称");
	m_gridPlotCtrl.SetItemText(0, 1, "合约");
	m_gridPlotCtrl.SetItemText(0, 2, "策略方向");
	m_gridPlotCtrl.SetItemText(0, 3, "止盈点位");
	m_gridPlotCtrl.SetItemText(0, 4, "止损点位");
	m_gridPlotCtrl.SetItemText(0, 5, "最大手数");
	m_gridPlotCtrl.SetItemText(0, 6, "持仓时间(秒)");
	m_gridPlotCtrl.Invalidate();

	m_gridPosiDetail.SetDefCellHeight(20);
	m_gridPosiDetail.SetDefCellMargin(1);
	m_gridPosiDetail.SetEditable(FALSE);
	m_gridPosiDetail.EnableSelection(TRUE);
	m_gridPosiDetail.SetListMode(TRUE);
	m_gridPosiDetail.SetSingleRowSelection(TRUE);
	m_gridPosiDetail.SetFixedColumnSelection(FALSE);
	m_gridPosiDetail.EnableDragAndDrop(FALSE);
	m_gridPosiDetail.SetColumnResize(TRUE);
	m_gridPosiDetail.SetHeaderSort(FALSE);
	m_gridPosiDetail.SetTrackFocusCell(FALSE);
	m_gridPosiDetail.SetShowFocusRect(FALSE);
	m_gridPosiDetail.SetBkColor(RGB(255, 255, 255));
	
	m_gridPosiDetail.SetColumnCount(7);
	m_gridPosiDetail.SetRowCount(1);
	m_gridPosiDetail.SetFixedRowCount(1);

	m_gridPosiDetail.SetColumnWidth(0, 80);
	m_gridPosiDetail.SetColumnWidth(1, 40);
	m_gridPosiDetail.SetColumnWidth(2, 60);
	m_gridPosiDetail.SetColumnWidth(3, 60);
	m_gridPosiDetail.SetColumnWidth(4, 60);
	m_gridPosiDetail.SetColumnWidth(5, 70);
	m_gridPosiDetail.SetColumnWidth(6, 80);
	m_gridPosiDetail.SetItemText(0, 0, "合约");
	m_gridPosiDetail.SetItemText(0, 1, "买卖");
	m_gridPosiDetail.SetItemText(0, 2, "当前手数");
	m_gridPosiDetail.SetItemText(0, 3, "最新价");
	m_gridPosiDetail.SetItemText(0, 4, "开仓价");
	m_gridPosiDetail.SetItemText(0, 5, "开仓时间");
	m_gridPosiDetail.SetItemText(0, 6, "剩余秒数");
	m_gridPosiDetail.Invalidate();
	m_gridPosiDetail.ShowWindow(SW_SHOW);

	m_gridPosition.SetDefCellHeight(20);
	m_gridPosition.SetDefCellMargin(1);
	m_gridPosition.SetEditable(FALSE);
	m_gridPosition.EnableSelection(TRUE);
	m_gridPosition.SetListMode(TRUE);
	m_gridPosition.SetSingleRowSelection(TRUE);
	m_gridPosition.SetFixedColumnSelection(FALSE);
	m_gridPosition.EnableDragAndDrop(FALSE);
	m_gridPosition.SetColumnResize(TRUE);
	m_gridPosition.SetHeaderSort(FALSE);
	m_gridPosition.SetTrackFocusCell(FALSE);
	m_gridPosition.SetShowFocusRect(FALSE);
	m_gridPosition.SetBkColor(RGB(255, 255, 255));
	
	m_gridPosition.SetColumnCount(9);
	m_gridPosition.SetRowCount(1);
	m_gridPosition.SetFixedRowCount(1);

	m_gridPosition.SetColumnWidth(0, 80);
	m_gridPosition.SetColumnWidth(1, 40);
	m_gridPosition.SetColumnWidth(2, 50);
	m_gridPosition.SetColumnWidth(3, 50);
	m_gridPosition.SetColumnWidth(4, 60);
	m_gridPosition.SetColumnWidth(5, 60);
	m_gridPosition.SetColumnWidth(6, 60);
	m_gridPosition.SetColumnWidth(7, 60);
	m_gridPosition.SetColumnWidth(8, 80);
	m_gridPosition.SetItemText(0, 0, "合约");
	m_gridPosition.SetItemText(0, 1, "买卖");
	m_gridPosition.SetItemText(0, 2, "总持仓");
	m_gridPosition.SetItemText(0, 3, "可平量");
	m_gridPosition.SetItemText(0, 4, "最新价");
	m_gridPosition.SetItemText(0, 5, "持仓均价");
	m_gridPosition.SetItemText(0, 6, "止盈价");
	m_gridPosition.SetItemText(0, 7, "止损价");
	m_gridPosition.SetItemText(0, 8, "持仓盈亏");
	m_gridPosition.Invalidate();
	m_gridPosition.ShowWindow(SW_HIDE);

	//MAPINSTCTRL::iterator it;
	//MAPINSTCFG mapInstCfg;

	//char strKey[128];
	//ZeroMemory(strKey, sizeof(strKey));

	//for(int i=0; i<CConfigManager::GetPlotCtrlItemCount(); i++) {
	//	
	//	PLOTCTRLITEMCFG cfg = CConfigManager::GetPlotCtrlItem(i);
	//	INSTRUMENTCTRLDIRECT ctrl = {0};
	//	strncpy(ctrl.plotID, cfg.plotID, sizeof(ctrl.plotID)-1);
	//	strncpy(ctrl.inst, cfg.inst, sizeof(ctrl.inst)-1);
	//	ctrl.cDirectMode = '3';
	//	
	//	m_mapInstCtrl[std::string(ctrl.inst)] = ctrl;
	//	m_mapPlotInst[std::string(ctrl.plotID)] = std::string(ctrl.inst);
	//	mapInstCfg[std::string(ctrl.inst)] = cfg;
	//}
	//m_mapInstCfg = mapInstCfg;

	//std::queue<PLOTCTRLITEMCFG> queueCfg;

	//std::map<std::string, PLOTCTRLITEMCFG>::iterator itcfg;
	//for(itcfg = mapInstCfg.begin(); itcfg != mapInstCfg.end(); itcfg++) {
	//	queueCfg.push(itcfg->second);
	//}
	//ShowQueue(queueCfg);

	//std::queue<INSTRUMENTCTRLDIRECT> queueCtrl;

	//for(it = m_mapInstCtrl.begin(); it != m_mapInstCtrl.end(); it++) {
	//	queueCtrl.push(it->second);
	//}
	//ShowQueue(queueCtrl);

	////初始化通信层
	//CTcpLayer::Init();
	//CTcpLayer::SubscribePkg(Cmd_OPC_CtrlDirect_Push, GetSafeHwnd());

	CenterWindow();

	SetTimer(conTimerNum, 1000, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTSSCtrlPluginDlg::ResetPlotCtrlData()
{
	MAPINSTCTRL::iterator it;
	MAPINSTCFG mapInstCfg;

	CString strInstRealID;
	char strKey[128];
	ZeroMemory(strKey, sizeof(strKey));

	for(int i=0; i<CConfigManager::GetPlotCtrlItemCount(); i++) {
		
		PLOTCTRLITEMCFG cfg = CConfigManager::GetPlotCtrlItem(i);
		INSTRUMENTCTRLDIRECT ctrl = {0};
		
		strInstRealID = GetInstRealID(CString(cfg.inst));
		strncpy(ctrl.plotID, cfg.plotID, sizeof(ctrl.plotID)-1);
		strncpy(cfg.inst, strInstRealID, sizeof(cfg.inst)-1);
		strncpy(ctrl.inst, strInstRealID, sizeof(ctrl.inst)-1);
		ctrl.cDirectMode = '3';
		
		m_mapInstCtrl[std::string(ctrl.inst)] = ctrl;
		m_mapPlotInst[std::string(ctrl.plotID)] = std::string(ctrl.inst);
		mapInstCfg[std::string(ctrl.inst)] = cfg;
	}
	m_mapInstCfg = mapInstCfg;

	std::queue<PLOTCTRLITEMCFG> queueCfg;

	std::map<std::string, PLOTCTRLITEMCFG>::iterator itcfg;
	for(itcfg = mapInstCfg.begin(); itcfg != mapInstCfg.end(); itcfg++) {
		queueCfg.push(itcfg->second);
	}
	ShowQueue(queueCfg);

	std::queue<INSTRUMENTCTRLDIRECT> queueCtrl;

	for(it = m_mapInstCtrl.begin(); it != m_mapInstCtrl.end(); it++) {
		queueCtrl.push(it->second);
	}
	ShowQueue(queueCtrl);
}

void CTSSCtrlPluginDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTSSCtrlPluginDlg::OnDestroy()
{
	CTcpLayer::UnsubscribeAllPkg(GetSafeHwnd());

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(conTimerNum);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTSSCtrlPluginDlg::OnPaint()
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
HCURSOR CTSSCtrlPluginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTSSCtrlPluginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
#if _DEBUG
	OnOK();
#endif
}

void CTSSCtrlPluginDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
#if _DEBUG
	OnCancel();
#endif
}

LRESULT CTSSCtrlPluginDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if ( message == _GetDataPkgArriveMSGID() && wParam==WndCmd_YourPkgArrival ) {
		if(!m_bExitApp) {
#if 1
			char *p = (char*)lParam;
			Stru_UniPkgHead head;					
			memset(&head, 0, sizeof(head));
			memcpy(&head, p, sizeof(head));
			switch(head.cmdid) {
			case Cmd_OPC_CtrlDirect_Push:
			{
				if ( head.userdata1 == OPERATOR_SUCCESS )
				{
					//MAPAssetMgmtOrg mapAssetMgmtOrg;
					int nCount = head.len / sizeof(INSTRUMENTCTRLDIRECT);
					queue<INSTRUMENTCTRLDIRECT> queueData;
					CString strText;
					for ( int i = 0; i < nCount; i++ )
					{
						INSTRUMENTCTRLDIRECT* pCtrlDirect = (INSTRUMENTCTRLDIRECT*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(INSTRUMENTCTRLDIRECT));
						
						TRACE3("Recv Cmd_OPC_CtrlDirect_Push: Data(%s, %s, %c)\n", 
							pCtrlDirect->plotID, pCtrlDirect->inst, pCtrlDirect->cDirectMode);

						strText = pCtrlDirect->plotID;
						strText.MakeLower();
						strncpy(pCtrlDirect->plotID, strText, sizeof(pCtrlDirect->plotID)-1);

						strText = GetInstRealID(CString(pCtrlDirect->inst));
						strncpy(pCtrlDirect->inst, strText, sizeof(pCtrlDirect->plotID)-1);

						MAPPLOTINST::iterator itPlot = 
								m_mapPlotInst.find(std::string(pCtrlDirect->plotID));
						if(itPlot != m_mapPlotInst.end()) {
							m_mapInstCtrl[std::string(pCtrlDirect->inst)] = *pCtrlDirect;
						}
						
						queueData.push(*pCtrlDirect);

					}
					//if(head.userdata2 == head.userdata3 -1)
					//	m_pdataInfo->FinishAeestMgmtOrganization(true);
					ShowQueue(queueData);
				}
				else
				{
					//string str = (char*)(p+sizeof(Stru_UniPkgHead));
					//String^ strtemp = Tools::string2String(str.c_str());
					//MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
				}
			}
				break;
			};
		}
#endif
	}
	else if(message==nITFCMDARRIVE) {
		if(!m_bExitApp) {
			ITFCMDATA* pITFData = (ITFCMDATA*)lParam;
			if(pITFData != NULL) {
				//if(pITFData->nCmd==CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP)
				//	ShowMsgLog(CString("CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP"));
				ParseCmd(pITFData->nCmd, pITFData->nErrorCode, pITFData->pData, pITFData->nDataLen, pITFData->nSeqNum);
				if(pITFData->pData!=NULL) {
					delete[] pITFData->pData;
					pITFData->pData = NULL;
				}
				delete pITFData;
				pITFData = NULL;
			}
		}
	}
	else if(message==GetBookOrderCloseMSGID()) {
		if(!m_bExitApp) {
			CWnd* pWnd = (CWnd*)lParam;
			
			LockGlobalObject();
			std::set<CWnd*>& setBookOrderDlg = GetBookOrderDlgSet();
			std::set<CWnd*>::iterator it = setBookOrderDlg.find(pWnd);
			if(it!=setBookOrderDlg.end()) 
			{
				it=setBookOrderDlg.erase(it);
				SIZE sizeDlg = {0};
				CRect rectDlg;
				pWnd->GetWindowRect(&rectDlg);
				sizeDlg.cx = rectDlg.Width();
				sizeDlg.cy = rectDlg.Height();
				SetBookDlgSize(sizeDlg);
				delete pWnd;
			}
			UnlockGlobalObject();
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CTSSCtrlPluginDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent==conTimerNum) {    
        QueryAllOrderFTIDList();
		QueryPositionInfo(m_strCurrAccount);//查询某个账号的所有持仓详情
		QueryCurrAccountInfo(m_strCurrAccount);//查询某个子账户资金信息
		//QueryPosiDetailInfo(m_strCurrAccount);//查询某个账号的所有持仓明细详情，参数为账户ID
		QueryPosiDetailOpenTime(m_strCurrAccount);//查询所有持仓明细开仓时间
    }

	CTSSCtrlPluginOperations::OnTimer(nIDEvent);
}

void CTSSCtrlPluginDlg::ParseCmd(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	if(nCmdId >= CMDID_CONSOLE_MIN && nCmdId <= CMDID_CONSOLE_MAX)
	{
		ParseCmd_Console(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
	}
	else {
		switch(nCmdId) {
			case CMDID_EXCHANGE_LIST_RSP:
			case CMDID_INSTRUMENT_LIST_RSP:
			case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
            case CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP:
				ParseCmd_BaseBusiness(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
				break;
			case CMDID_INSTRUMENT_INFO_RSP:
			case CMDID_QUOTE_SNAPSHOT_RSP:
			case CMDID_QUOTE_SEND_QUOTE:
            case CMDID_ALL_ORDER_LIST_RSP:
            case CMDID_ALL_ORDER_INFO_RSP:
			case CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP://返回开仓未成交挂单数量
            case CMDID_ORDER_INSERT_ORDER_RSP:
			case CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY_RSP:
            case CMDID_ORDER_ORDER_STATUS_CHANGE:
            case CMDID_ORDER_CANCEL_ORDER_RSP:
            case CMDID_ORDER_RSP_ORDER_ACTION1:				
            case CMDID_ORDER_RSP_ORDER_ACTION2:	
				ParseCmd_Business(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
				break;
			case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
			case CMDID_ACCT_POSITION_INFO_LIST_RSP:
			case CMDID_ACCT_POSITION_DETAIL_LIST_RSP:
			case CMDID_ALL_POSITION_DETAIL_OPEN_TIME_LIST_RSP:
				ParseCmd_Check(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
				break;
			// 下单审核
			case CMDID_INSERT_ORDER_VERIFY_ORDER_PUSH:
				ParseCmd_Verify(nCmdId, nErrorCode, pData, nDataLen, nSeqNum);
				break;
		};
	}
}

BOOL IsTopLevelWin_MayDestroy(HWND hwnd)
{
    DWORD dwProcessID;
	char csClassName[1024]={0};
    GetWindowThreadProcessId(hwnd, &dwProcessID);
	::GetClassName(hwnd, csClassName, 1024);
    if(dwProcessID==GetCurrentProcessId() && stricmp(csClassName, "#32770")==0)
    {
#ifdef _DEBUG
		char csCaption[1024]={0};
		::GetWindowText(hwnd,csCaption,1024);
		CString strText;
		strText.Format("IsTopLevelWin_MayDestroy() Item:%08X %s(%s)\n", hwnd, csCaption, csClassName);
		TRACE(strText);
#endif
		return TRUE;
    }
	return FALSE;
}

BOOL CALLBACK EnumWinFunc(HWND hwnd, LPARAM lParam)
{
    if(IsTopLevelWin_MayDestroy(hwnd))
	{
		CWnd* pWnd = CWnd::FromHandle(hwnd);
		if(pWnd!=NULL && (pWnd->m_nFlags&WF_CONTINUEMODAL) && (pWnd->m_nFlags&WF_MODALLOOP))
		{
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED), NULL);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDNO, BN_CLICKED), NULL);
		}
		else
		{
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDCANCEL, BN_CLICKED), NULL);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDNO, BN_CLICKED), NULL);
			Sleep(10);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDOK, BN_CLICKED), NULL);
			::SendMessage(hwnd, WM_COMMAND, MAKELONG(IDYES, BN_CLICKED), NULL);
		}
	}
	return TRUE;
}

void CTSSCtrlPluginDlg::ParseCmd_Console(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_CONSOLE_LOGON_RSP:
			if(nErrorCode==SUCCESS)
			{
				ForceOrderVerify();
				QueryExchangeIDs();
				QueryCurrAccountID();
                QueryCurrSubAccountID();
			}
			else
			{
				::MessageBox(NULL, "一键下单功能初始化失败，无法登录到FastTrader", "错误提示", MB_ICONERROR|MB_OK|MB_SYSTEMMODAL);
				::PostMessage(GetSafeHwnd(), WM_CLOSE, 0, NULL);
			}
			break;
		case CMDID_CONSOLE_FASTTRADER_TO_PLUSIN_SHOW:
			break;
		case CMDID_CONSOLE_CLOSE:
			{
				m_bExitApp = TRUE;
				::EnumWindows(EnumWinFunc,(LPARAM)this); 
				LockGlobalObject();
				std::set<CWnd*> setDlg = GetBookOrderDlgSet();
				std::set<CWnd*>::iterator it = setDlg.begin();
				for(; it != setDlg.end(); it++) 
				{
					::SetWindowLong((*it)->GetSafeHwnd(), GWL_HWNDPARENT, (long)this->GetSafeHwnd());
				}
				ReleaseGlobalObject();
				UnlockGlobalObject();
				::EnumWindows(EnumWinFunc,(LPARAM)this); 

				//::PostMessage(GetSafeHwnd(), WM_CLOSE, 0, NULL);
				OnOK();
			}
			break;
		default:
			break;
	};
}

void CTSSCtrlPluginDlg::ParseCmd_BaseBusiness(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_EXCHANGE_LIST_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(CommonExchangeIDType)==0 && pData!=NULL)
				{
					std::set<std::string> setExchange;
					char strExchange[sizeof(CommonExchangeIDType)+4];
					while(!m_queueExchangeForQuery.empty())
						m_queueExchangeForQuery.pop();
					for (int i=0; i<nDataLen/sizeof(CommonExchangeIDType); i++)
					{
						ZeroMemory(strExchange, sizeof(strExchange));
						strncpy(strExchange, (char*)pData+i*sizeof(CommonExchangeIDType), 
								sizeof(CommonExchangeIDType));
						m_queueExchangeForQuery.push(std::string(strExchange));
						setExchange.insert(std::string(strExchange));
					}
					LockGlobalObject();
					SetExchangeIDList(setExchange);
					UnlockGlobalObject();
					CString strExchangeID(m_queueExchangeForQuery.front().c_str());
					m_queueExchangeForQuery.pop();
					QueryInstrumentIDs(strExchangeID);
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得交易所列表", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_INSTRUMENT_LIST_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen%sizeof(InstrumentIDType)==0)
				{
					char szInstrumentID[sizeof(InstrumentIDType)+4];
					for(int i=0; i<nDataLen/sizeof(InstrumentIDType); i++)
					{
						ZeroMemory(szInstrumentID, sizeof(szInstrumentID));
						strncpy(szInstrumentID, (char*)pData+i*sizeof(InstrumentIDType), sizeof(InstrumentIDType));
						LockGlobalObject();
						GetInstrumentIDList().insert(std::string(szInstrumentID));
						UnlockGlobalObject();
					}
				}
				if(nErrorCode==SUCCESS && 
						((pData!=NULL && nDataLen%sizeof(InstrumentIDType)==0) || (pData==NULL && nDataLen==0)))
				{
					if(!m_queueExchangeForQuery.empty()) {
						CString strExchangeID(m_queueExchangeForQuery.front().c_str());
						m_queueExchangeForQuery.pop();
						QueryInstrumentIDs(strExchangeID);
					}
					else 
					{
						SetInitStatus(TRUE);
						MakeInstCompatibleList();
						ResetPlotCtrlData();
						//初始化通信层
						CTcpLayer::Init();
						CTcpLayer::SubscribePkg(Cmd_OPC_CtrlDirect_Push, GetSafeHwnd());
					}
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法合约列表", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_ACCOUNT_CURRENT_ACCOUNT_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen>0 && pData!=NULL) 
				{
					char strUserID[64];
					ZeroMemory(strUserID, sizeof(strUserID));
					strncpy(strUserID, (char*)pData, nDataLen>63 ? 63 : nDataLen);
					m_strCurrAccount = strUserID;
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得当前用户名", MB_ICONERROR|MB_OK);
				}
			}
			break;
        case CMDID_ACCOUNT_CURRENT_SUB_ACCOUNT_RSP:
            {
                if(nErrorCode == SUCCESS && nDataLen>0 && pData!=NULL)
                {
                    char strSubAccount[64];
                    ZeroMemory(strSubAccount, sizeof(strSubAccount));
                    strncpy_s(strSubAccount, (char*)pData, nDataLen>63 ? 63 : nDataLen);
                    m_strCurrSubAccount = strSubAccount;
                    //QueryCurrAccountInfo(CString(strSubAccount));
                }
                else 
                {
                    //AfxMessageBox("一键下单功能初始化失败，无法获得当前子帐号", MB_ICONERROR|MB_OK);
                }
            }
            break;
        default:
            break;
    }
}

void CTSSCtrlPluginDlg::ParseCmd_Verify(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_INSERT_ORDER_VERIFY_ORDER_PUSH:
			TRACE("CMDID_INSERT_ORDER_VERIFY_ORDER_PUSH\n");
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_InputOrder)==0 && pData!=NULL)
				{
					CString strMsg;
					PlatformStru_InputOrder* pInputOrder = NULL;
					int nItemCount = nDataLen/sizeof(PlatformStru_InputOrder);
					for (int i=0; i<nItemCount; i++) {
						pInputOrder = (PlatformStru_InputOrder*)
								((char*)pData+i*sizeof(PlatformStru_InputOrder));

						// 非开仓下单时，不需要审核就执行下单
						if(pInputOrder->CombOffsetFlag[0]!=THOST_FTDC_OF_Open)
							SendInputOrder(*pInputOrder);
						else {
							std::string strInst(pInputOrder->InstrumentID);
							MAPINSTCTRL::iterator itCtrl;

							itCtrl = m_mapInstCtrl.find(strInst);
							if(itCtrl == m_mapInstCtrl.end()) {
								strMsg.Format("[%s]开仓被拒绝，原因: 合约不在配置表中。", pInputOrder->InstrumentID);
								ShowMsgLog(strMsg);
								continue;
							}

							// 方向下单控制代码
							if((itCtrl->second.cDirectMode=='0' || itCtrl->second.cDirectMode=='2') 
									&& pInputOrder->Direction==THOST_FTDC_D_Buy) {
								strMsg.Format("[%s]开仓被拒绝，原因: 合约策略方向为[空]。", pInputOrder->InstrumentID);
								ShowMsgLog(strMsg);
								continue;
							}
							if((itCtrl->second.cDirectMode=='0' || itCtrl->second.cDirectMode=='1') 
									&& pInputOrder->Direction==THOST_FTDC_D_Sell) {
								strMsg.Format("[%s]开仓被拒绝，原因: 合约策略方向为[多]。", pInputOrder->InstrumentID);
								ShowMsgLog(strMsg);
								continue;
							}

							// 最大持仓下单控制代码
							int nVol = 0;
							{
								// 计算当前合约开仓单总数，不分方向
								MAPINSTVOL::iterator it;
								it = m_mapInstOpenWaitVol.find(strInst);
								if(it != m_mapInstOpenWaitVol.end())
									nVol += it->second;
								it = m_mapInstPosiVol.find(strInst);
								if(it != m_mapInstPosiVol.end())
									nVol += it->second;
								nVol += pInputOrder->VolumeTotalOriginal;
							}
							{
								MAPINSTCFG::iterator itcfg;
								itcfg = m_mapInstCfg.find(strInst);
								if(itcfg == m_mapInstCfg.end()) {
									strMsg.Format("[%s]开仓被拒绝，原因: 合约不在配置表中。", pInputOrder->InstrumentID);
									ShowMsgLog(strMsg);
									continue;
								}
								if(nVol > itcfg->second.maxPosiVol) {
									strMsg.Format("[%s]开仓被拒绝，原因: 合约开仓超过最大持仓限制。", pInputOrder->InstrumentID);
									ShowMsgLog(strMsg);
									continue;
								}
							}

							// 需要检查与下单同合约、方向、套保的持仓是否亏损，亏损则禁止开仓
							if(CheckPosiHasLose(*pInputOrder)) {
								strMsg.Format("[%s]开仓被拒绝，原因: 同向合约持仓为亏损。", pInputOrder->InstrumentID);
								ShowMsgLog(strMsg);
								continue;
							}

							// 最大亏损下单控制代码
							if(m_bHasLoseOverLimit_AcctMax) {
								strMsg.Format("[%s]开仓被拒绝，原因: 当前处于资金最大限亏状态。", pInputOrder->InstrumentID);
								ShowMsgLog(strMsg);
								continue;
							}

							// 当处于最大限亏状态时
							{
								PLOTCTRLHEADCFG plotCfg = CConfigManager::GetPlotCtrlHead();
								double fProfit = m_tAccountInfo.CloseProfit+m_tAccountInfo.PositionProfit;
								// 帐号亏损被监视，并且不处于整固阶段
								if(-fProfit > plotCfg.maxLoseForDay) {
									strMsg.Format("[%s]开仓被拒绝，原因: 当前处于资金最大限亏状态。", pInputOrder->InstrumentID);
									ShowMsgLog(strMsg);
									continue;
								}
							}

							// 通过所有进制条件审核后，执行无审核开仓下单
							SendInputOrder(*pInputOrder);

						}
					}
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得交易所列表", MB_ICONERROR|MB_OK);
				}
			}
			break;
		default:
			break;
	}
}

void CTSSCtrlPluginDlg::ParseCmd_Business(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_INSTRUMENT_INFO_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(PlatformStru_InstrumentInfo))
				{
					PlatformStru_InstrumentInfo* pInfo = (PlatformStru_InstrumentInfo*)pData;
					m_mapInstInfo[std::string(pInfo->InstrumentID)] = *pInfo;
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法指定合约信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_QUOTE_SEND_QUOTE:
		case CMDID_QUOTE_SNAPSHOT_RSP:
			{
				if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_DepthMarketData)==0 && nDataLen>0)
				{
					PlatformStru_DepthMarketData *pQuot = (PlatformStru_DepthMarketData*)pData;
					m_mapQuot[std::string(pQuot->InstrumentID)] = *pQuot;
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得当前帐号信息", MB_ICONERROR|MB_OK);
				}
			}
			break;            
        case CMDID_ALL_ORDER_LIST_RSP:
            {
                if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(int)==0 && nDataLen>0)
                {
                    int *pFTID = NULL;
                    int nCount = nDataLen/sizeof(int);
                    for(int i=0; i<nCount; i++) {
                        pFTID = (int*)pData+i;
                        QueryOrderInfoByFTID(*pFTID);
						if(i==nCount-1)
							m_nLastFTID4Cancel = *pFTID;
                    }
                }
                else 
                {
                    //AfxMessageBox("一键下单功能初始化失败，无法获得当前帐号信息", MB_ICONERROR|MB_OK);
                }
            }
            break;
                
        case CMDID_ALL_ORDER_INFO_RSP://合约是否有挂单 
		    {
                if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_OrderInfo)==0 && nDataLen>0)
                {  
                    PlatformStru_OrderInfo *pFTInfo = (PlatformStru_OrderInfo*)pData;

                    //撤掉策略中所有的挂单
                    if ((pFTInfo->OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
                        pFTInfo->OrderStatus == THOST_FTDC_OST_NoTradeQueueing))
                    {
                        if(m_bHasDayLostMax)//日亏条件
                            CancleOrderLast(*pFTInfo);
                        else
                        {          
                            if(pFTInfo->CombOffsetFlag[0] == THOST_FTDC_OF_Open)
                                break;

                            char csKey[256];
                            ZeroMemory(csKey, sizeof(csKey));
                            sprintf_s(csKey, sizeof(csKey)-1, "%s,%c,%c", 
                                pFTInfo->InstrumentID, pFTInfo->Direction, pFTInfo->CombHedgeFlag[0]);

                            map<string, PlatformStru_InputOrder>::iterator it = m_mapCloseOrder.find(csKey);
                            if (it!=m_mapCloseOrder.end())
                            { 
                                //判断是否已撤单过                                
                                if (m_mapOrderCloseInfo.size() == 0)
                                {
                                    CancleOrderLast(*pFTInfo);
                                }else
                                {
                                    map<string,ORDERCLOSEINFO>::iterator it1 = m_mapOrderCloseInfo.find(pFTInfo->OrderRef);
                                    if (it1 != m_mapOrderCloseInfo.end())
                                    {
                                        if (::GetTickCount() - it1->second.dTime > 6500) 
                                        {
                                            CancleOrderLast(*pFTInfo);                                    
                                        }
                                    }else
                                    {
                                        CancleOrderLast(*pFTInfo);
                                    }
                                }                                 
                            }
							if(m_nLastFTID4Cancel == pFTInfo->FTID) {
								m_mapCloseOrder.clear();
								m_nLastFTID4Cancel = -1;
							}
                        }
                    }
                }
                else 
                {
                    //AfxMessageBox("一键下单功能初始化失败，无法获得当前帐号信息", MB_ICONERROR|MB_OK);
                }
            }
            break;
        case CMDID_ORDER_CANCEL_ORDER_RSP://撤单请求响应
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_InputOrderAction)==0 && nDataLen>0)
            {  
                PlatformStru_InputOrderAction* pInputOrderAction = (PlatformStru_InputOrderAction*)pData;                 

                SYSTEMTIME sys;
                GetLocalTime(&sys);
                TRACE("撤单成功接受回应 %d:%d:%d.%d  OrderRef=%d\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,atoi(pInputOrderAction->Thost.OrderRef));
            }
            break;           
        case CMDID_ORDER_RSP_ORDER_ACTION1:		//CTP撤单失败返回
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_RspOrderAction1)==0 && nDataLen>0)
            {               
                PlatformStru_InputOrderAction Action;
                PlatformStru_RspOrderAction1* pOrderAction1 = (PlatformStru_RspOrderAction1*)pData;
                TRACE("CTP撤单失败返回！\n");
            }
            break;
        case CMDID_ORDER_RSP_ORDER_ACTION2:		//交易所撤单失败返回
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_RspOrderAction2)==0 && nDataLen>0)
            {  
                PlatformStru_InputOrderAction Action;
                PlatformStru_RspOrderAction2* pOrderAction2 = (PlatformStru_RspOrderAction2*)pData;                
                TRACE("交易所撤单失败返回！\n");
            }
            break;             
        case CMDID_ORDER_INSERT_ORDER_WITHOUT_VERIFY_RSP://下单请求响应
            if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_InputOrder)==0 && nDataLen>0)
            {                
                PlatformStru_InputOrder *pInputOrder = (PlatformStru_InputOrder*)pData;

				if(m_setCloseOrder.find(nSeqNum) != m_setCloseOrder.end()) {
					// 记录又本插件发起的所有下单
					//m_mapCloseOrder.insert(make_pair(pInputOrder->OrderRef, *pInputOrder));
					m_mapCloseWaitOrderRef.insert(pInputOrder->OrderRef);
					m_setCloseOrder.erase(nSeqNum);
				}
            }
            break;
        case CMDID_ORDER_ORDER_STATUS_CHANGE:
            {
                if(nErrorCode == SUCCESS && pData!=NULL && nDataLen%sizeof(PlatformStru_OrderInfo)==0 && nDataLen>0)
                {                
                    PlatformStru_OrderInfo* pOrderInfo = (PlatformStru_OrderInfo*)pData;
                    
					// 注销下单监视
					if(pOrderInfo->OrderStatus!=THOST_FTDC_OST_Unknown) {
						m_mapCloseWaitOrderRef.erase(pOrderInfo->OrderRef);
						if(m_mapCloseWaitOrderRef.size()==0)
							m_dwPreCloseTime = 0;
					}

                    if (pOrderInfo->OrderStatus==THOST_FTDC_OST_Canceled ||
                        pOrderInfo->OrderStatus==THOST_FTDC_OST_AllTraded) //已撤单
                    { 
                        map<string,ORDERCLOSEINFO>::iterator it = m_mapOrderCloseInfo.find(pOrderInfo->OrderRef);
                        if (it != m_mapOrderCloseInfo.end())
                        {
                            TRACE("报单状态改变--撤单 OrderRef = %s,合约 = %s,方向 = %s,数量 = %d,价格 = %.2f",
                                pOrderInfo->OrderRef,pOrderInfo->InstrumentID,(pOrderInfo->Direction==THOST_FTDC_D_Buy?"买":"卖"), 
                                pOrderInfo->VolumeTotalOriginal, pOrderInfo->LimitPrice);                        

                            m_mapOrderCloseInfo.erase(it);
                        }

                        //其他挂单是否撤单成功
                        map<string,PlatformStru_OrderInfo>::iterator it1 = m_mapOtherWaitOrder.find(pOrderInfo->OrderRef);
                        if (it1 != m_mapOtherWaitOrder.end())
                        {
                             m_mapOtherWaitOrder.erase(it1);
                        }
                    }
                }
            }
            break;
        case CMDID_ALL_ORDER_WAIT_OPEN_ORDER_NUM_RSP:
            {
                if(nErrorCode == SUCCESS && nDataLen%sizeof(PlusinAcctOpenOrderNumRsp)==0) 
                {
                    MAPINSTVOL mapInstOpenWaitVol;

					int nCount = nDataLen/sizeof(PlusinAcctOpenOrderNumRsp);
					PlusinAcctOpenOrderNumRsp* pRsp = NULL;
					char csKey[256];
					ZeroMemory(csKey, sizeof(csKey));
					for(int i=0; i<nCount; i++) {
						pRsp = (PlusinAcctOpenOrderNumRsp*)pData+i;
						sprintf(csKey, "%s", pRsp->queryCondition.szInstrumentID);
						//mapInstOpenWaitVol.insert(make_pair(std::string(csKey), pRsp->nNum));
						m_mapInstOpenWaitVol[std::string(csKey)] = pRsp->nNum;
					}
					//LockObject();
					//m_mapInstOpenWaitVol = mapInstOpenWaitVol;
					//UnlockObject();

					if(m_nLastSeqNum4OpenVal == nSeqNum) {
						// 处理持仓止盈止损点                    
						if(MayClose_Time()) {
							m_bHasDayLostMax = false;
							ProcPosiMaxLose();
						}
						// 处理单笔最大持仓时间
						if(MayClose_Time()) {        
							m_bHasDayLostMax = false;
							ProcPosiDetailMaxTime();
						}
						m_nLastSeqNum4OpenVal = -1;
					}
                }
                else 
                {
                    //AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
                }
            }
            break;
        default:
            break;
};
}

void CTSSCtrlPluginDlg::ParseCmd_Check(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nSeqNum)
{
	switch(nCmdId) {
		case CMDID_ACCOUNT_SUB_ACCOUNT_INFO_RSP:
			{
				if(nErrorCode==SUCCESS && pData!=NULL && nDataLen==sizeof(PlatformStru_TradingAccountInfo))
				{
					PLOTCTRLHEADCFG plotCfg = CConfigManager::GetPlotCtrlHead();
					memcpy(&m_tAccountInfo, pData, sizeof(PlatformStru_TradingAccountInfo));
					double fProfit = m_tAccountInfo.CloseProfit+m_tAccountInfo.PositionProfit-m_tAccountInfo.Commission;
					CString strText;
					strText.Format("日内总盈亏: %0.2f", fProfit);
					SetDlgItemText(IDC_LBLFUNDPROFIT, strText);
					DWORD dwCurrTick = ::GetTickCount();
					{
						CString strItem;
						if(!m_bHasLoseOverLimit_AcctMax)
							strItem.Format("限亏强平计时: 未开始, ");
						else
							strItem.Format("限亏强平计时: %u, ", 
									(dwCurrTick-m_dwLoseOLimitStart_AcctMax-
									(m_bHasLoseRegress_AcctMax?dwCurrTick-m_dwLoseRegressStart_AcctMax:0))/1000);
						strText = strItem;
						if(!m_bHasLoseRegress_AcctMax)
							strItem.Format("整固计时: 未开始");
						else
							strItem.Format("整固计时: %u", 
									(dwCurrTick-m_dwLoseRegressStart_AcctMax)/1000);
						strText += strItem;
					}
					SetDlgItemText(IDC_LBLLOSTSTATUS, strText);
					// 处理帐号总亏损逻辑
					if(m_bHasLoseOverLimit_AcctMax) {
						// 帐号总亏损已监视的处理
						if(m_bHasLoseRegress_AcctMax) {
							// 帐号亏损处于整固阶段
							if(-fProfit > plotCfg.maxLoseForDay) {
								// 帐号亏损重新跌入设置值下方，重新开始新一轮亏损监视
								m_bHasLoseOverLimit_AcctMax = true;
								m_dwLoseOLimitStart_AcctMax = ::GetTickCount();
								m_bHasLoseRegress_AcctMax = false;
								m_dwLoseRegressStart_AcctMax = ::GetTickCount();
							}
							else if((dwCurrTick - m_dwLoseRegressStart_AcctMax)/1000 >= plotCfg.cancelForceTime) {
								// 亏损整固完成，撤销亏损监视状态
								m_bHasLoseOverLimit_AcctMax = false;
								m_dwLoseOLimitStart_AcctMax = ::GetTickCount();
								m_bHasLoseRegress_AcctMax = false;
								m_dwLoseRegressStart_AcctMax = ::GetTickCount();
							}
						}
						else {
							// 帐号亏损被监视，并且不处于整固阶段
							if(-fProfit <= plotCfg.maxLoseForDay) {
								// 亏损回到设置值上面，需要进行整固监视
								m_bHasLoseRegress_AcctMax = true;
								m_dwLoseRegressStart_AcctMax = ::GetTickCount();
							}
							else {
								// 账户一直处于亏损监视状态
								if((dwCurrTick - m_dwLoseOLimitStart_AcctMax)/1000 > plotCfg.forceCloseTime) {
									// 帐号亏损达到指定全平的时间条件，执行平所有持仓的操作
									// 处理限亏全平
									if(MayClose_Time() && CanCloseAll()) {
                                        m_bHasDayLostMax = true;
										CloseAll();
										m_bHasLoseOverLimit_AcctMax = false;
										m_dwLoseOLimitStart_AcctMax = ::GetTickCount();
										m_bHasLoseRegress_AcctMax = false;
										m_dwLoseRegressStart_AcctMax = ::GetTickCount();
									}
								}
								//else {
								//}
							}
						}
					}
					else if(-fProfit > plotCfg.maxLoseForDay) {// && m_mapInstPosi.size()>0) {
						// 当帐号亏损大于设置的亏损额时
						m_bHasLoseOverLimit_AcctMax = true;
						m_dwLoseOLimitStart_AcctMax = ::GetTickCount();
						m_bHasLoseRegress_AcctMax = false;
						m_dwLoseRegressStart_AcctMax = ::GetTickCount();
					}
				}
				else 
				{
					//AfxMessageBox("一键下单功能初始化失败，无法获得当前帐号信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_ACCT_POSITION_INFO_LIST_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_Position)==0) 
				{
					MAPINSTPOSI mapInstPosi;
					MAPINSTVOL mapInstPosiVol;//m_mapInstPosiVol;
					std::queue<PlatformStru_Position> queueData;
					MAPINSTVOL::iterator it;

					int nCount = nDataLen/sizeof(PlatformStru_Position);
					PlatformStru_Position* pPosi = NULL;
					char csKey[256];
					ZeroMemory(csKey, sizeof(csKey));
					for(int i=0; i<nCount; i++) {
						pPosi = (PlatformStru_Position*)pData+i;
						strncpy(csKey, pPosi->InstrumentID, sizeof(pPosi->InstrumentID)-1);
						it = mapInstPosiVol.find(std::string(csKey));
						if(it != mapInstPosiVol.end())
							it->second += pPosi->Position;
						else
							mapInstPosiVol.insert(make_pair(std::string(csKey), pPosi->Position));
						
						sprintf(csKey, "%s,%c,%c", pPosi->InstrumentID, pPosi->PosiDirection, pPosi->HedgeFlag);
						mapInstPosi[std::string(csKey)] = *pPosi;

						MAPQUOT::iterator it = m_mapQuot.find(std::string(pPosi->InstrumentID));
						if(it == m_mapQuot.end()) {
							// 订阅相关合约的行情
							QueryQuotSnapshot(CString(pPosi->InstrumentID));
							SubscribeQuot(CString(pPosi->InstrumentID));
						}
						QueryInstrumentInfo(CString(pPosi->InstrumentID));

						queueData.push(*pPosi);
					}
					//LockObject();
					m_mapInstPosiVol = mapInstPosiVol;
					m_mapInstPosi = mapInstPosi;
					//UnlockObject();
					ShowQueue(queueData, nCount);

					// 然后进行未成交数量查询
					QueryOpenOrderVol(m_strCurrAccount);

					// 把这些挪到未成交数量rsp处理代码中
					//// 处理持仓止盈止损点
					//if(MayClosePosition_Time()) 
					//	ProcPosiMaxLose();
				}
				else 
				{
					//AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		case CMDID_ALL_POSITION_DETAIL_OPEN_TIME_LIST_RSP:
		//case CMDID_ACCT_POSITION_DETAIL_LIST_RSP:
			{
				if(nErrorCode == SUCCESS && nDataLen%sizeof(PlatformStru_PositionDetail)==0) 
				{
					MAPPOSIDETAIL mapPosiDetail;
					std::queue<PlatformStru_PositionDetail> queueData;

					int nCount = nDataLen/sizeof(PlatformStru_PositionDetail);
					PlatformStru_PositionDetail* pPosi = NULL;
					char csKey[256];
					ZeroMemory(csKey, sizeof(csKey));
					for(int i=0; i<nCount; i++) {
						pPosi = (PlatformStru_PositionDetail*)pData+i;
						sprintf(csKey, "%s,%s,%c", pPosi->ExchangeID, 
								pPosi->TradeID, pPosi->Direction);
						mapPosiDetail.insert(make_pair(std::string(csKey), *pPosi));
						queueData.push(*pPosi);
					}
					//LockObject();
					m_mapPosiDetail = mapPosiDetail;
					//UnlockObject();
					ShowQueue(queueData, nCount);
					// 把这些挪到未成交数量rsp处理代码中
					//// 处理单笔最大持仓时间
					//if(MayClosePosition_Time()) 
					//	ProcPosiDetailMaxTime();
				}
				else 
				{
					//AfxMessageBox("无法获得持仓信息", MB_ICONERROR|MB_OK);
				}
			}
			break;
		default:
			break;
	};
}

int CTSSCtrlPluginDlg::GetRowIndexByRowID_Plot(int nRowID) 
{
	for(int i=0; i<m_gridPlotCtrl.GetRowCount(); i++) {
		if(m_gridPlotCtrl.GetItemData(i, 0) == nRowID)
			return i;
	}
	return -1;
}

int CTSSCtrlPluginDlg::GetColIndexByColID_Plot(int nColID) 
{
#if 0
	for(int i=0; i<m_lstPlotStatus.GetItemCount(); i++) {
		if(m_lstPlotStatus.GetColumn() == nColID)
			return dataGridPosition->Columns[i]->Index;
	}
#endif
	return -1;
}

int CTSSCtrlPluginDlg::GetRowIndexByRowID_Position(int nRowID) 
{
	for(int i=0; i<m_gridPosition.GetRowCount(); i++) {
		if(m_gridPosition.GetItemData(i, 0) == nRowID)
			return i;
	}
	return -1;
}

int CTSSCtrlPluginDlg::GetColIndexByColID_Position(int nColID) 
{
#if 0
	for(int i=0; i<m_gridPosition.GetItemCount(); i++) {
		if(m_gridPosition.GetColumn() == nColID)
			return dataGridPosition->Columns[i]->Index;
	}
#endif
	return -1;
}

int CTSSCtrlPluginDlg::GetRowIndexByRowID_PosiDetail(int nRowID) 
{
	for(int i=0; i<m_gridPosiDetail.GetRowCount(); i++) {
		if(m_gridPosiDetail.GetItemData(i, 0) == nRowID)
			return i;
	}
	return -1;
}

int CTSSCtrlPluginDlg::GetColIndexByColID_PosiDetail(int nColID) 
{
#if 0
	for(int i=0; i<m_gridPosiDetail.GetItemCount(); i++) {
		if(m_gridPosiDetail.GetColumn() == nColID)
			return dataGridPosition->Columns[i]->Index;
	}
#endif
	return -1;
}

bool CTSSCtrlPluginDlg::DeleteRow_Plot(std::string& strKey) 
{
	std::map<string, int>::iterator it;
	int nRowID = -1;
	int nIndex = -1;

	//LockObject();
	it = mapKey2Index_Plot.find(strKey);
	if(it == mapKey2Index_Plot.end()) {
		//UnlockObject();
		return false;
	}
	
	nRowID = it->second;
	//UnlockObject();
	nIndex = GetRowIndexByRowID_Plot(nRowID);
	if(nIndex<0)
		return false;

	m_gridPlotCtrl.DeleteRow(nIndex);
	//LockObject();
	mapKey2Index_Plot.erase(it);
	mapIndex2Key_Plot.erase(nRowID);
	//UnlockObject();
	return true;
}

void CTSSCtrlPluginDlg::UpdateRow(int nIndex, INSTRUMENTCTRLDIRECT& plotCtrl) 
{
	if(nIndex<0)
		return;

	m_gridPlotCtrl.SetItemText(nIndex, 0, plotCtrl.plotID);
	m_gridPlotCtrl.SetItemText(nIndex, 1, plotCtrl.inst);
	CString strText;
	if(plotCtrl.cDirectMode=='0')
		strText="禁止开仓";
	else if(plotCtrl.cDirectMode=='1')
		strText="多";
	else if(plotCtrl.cDirectMode=='2')
		strText="空";
	else if(plotCtrl.cDirectMode=='3')
		strText="无";
	m_gridPlotCtrl.SetItemText(nIndex, 2, strText);
	DWORD dwTextColor = 0;
	if(plotCtrl.cDirectMode=='0')
		dwTextColor = CConfigManager::GetPlotCtrlHead().dwLimitAllColor;
	else if(plotCtrl.cDirectMode=='1')
		dwTextColor = CConfigManager::GetPlotCtrlHead().dwLongColor;
	else if(plotCtrl.cDirectMode=='2')
		dwTextColor = CConfigManager::GetPlotCtrlHead().dwShortColor;
	else if(plotCtrl.cDirectMode=='3')
		dwTextColor = CConfigManager::GetPlotCtrlHead().dwNoneColor;
	//m_gridPlotCtrl.SetItemBkColour(nIndex, 2, RGB(0, 0, 0));
	//dwTextColor = RGB(255, 0, 0);
	m_gridPlotCtrl.SetItemFgColour(nIndex, 2, dwTextColor);
}

void CTSSCtrlPluginDlg::UpdateRow(int nIndex, PLOTCTRLITEMCFG& plotCtrl) 
{
	if(nIndex<0)
		return;

	CString strText;
	m_gridPlotCtrl.SetItemText(nIndex, 0, plotCtrl.plotID);
	m_gridPlotCtrl.SetItemText(nIndex, 1, plotCtrl.inst);
	//m_gridPlotCtrl.SetItemText(nIndex, 2, CString(plotCtrl.cDirectMode));
	strText.Format("%d", plotCtrl.stopGainPoint);
	m_gridPlotCtrl.SetItemText(nIndex, 3, strText);
	strText.Format("%d", plotCtrl.stopLosePoint);
	m_gridPlotCtrl.SetItemText(nIndex, 4, strText);
	strText.Format("%d", plotCtrl.maxPosiVol);
	m_gridPlotCtrl.SetItemText(nIndex, 5, strText);
	strText.Format("%d", plotCtrl.singlePosiTime);
	m_gridPlotCtrl.SetItemText(nIndex, 6, strText);
}
 
void CTSSCtrlPluginDlg::UpdateRow(int nIndex, PlatformStru_Position& position) 
{
	if(nIndex<0)
		return;

	CString strText;
	m_gridPosition.SetItemText(nIndex, 0, position.InstrumentID);
	m_gridPosition.SetItemText(nIndex, 1, (position.PosiDirection==THOST_FTDC_PD_Long?"买":"卖"));
	strText.Format("%d", position.Position);
	m_gridPosition.SetItemText(nIndex, 2, strText);
	strText.Format("%d", position.CanCloseVolume);
	m_gridPosition.SetItemText(nIndex, 3, strText);
	double fLastPrice = 0.0;
	if(GetLastPrice(std::string(position.InstrumentID), fLastPrice))
		strText.Format("%0.2f", fLastPrice);
	else
		strText.Format("-");
	m_gridPosition.SetItemText(nIndex, 4, strText);
	strText.Format("%0.2f", position.AveragePositionPrice);
	m_gridPosition.SetItemText(nIndex, 5, strText);
	PLOTCTRLITEMCFG item;
	ZeroMemory(&item, sizeof(item));
	if(GetInstPlot(std::string(position.InstrumentID), item)) {
		BOOL bDirect = position.PosiDirection==THOST_FTDC_PD_Long;
		double fPriceTick = 0.0;
		BOOL bHasPriceTick = GetInstPriceTick(std::string(position.InstrumentID), fPriceTick);
		if(bHasPriceTick)
			strText.Format("%0.2f", position.AveragePositionPrice+item.stopGainPoint*(bDirect?1.0:-1.0)*fPriceTick);
		else
			strText.Format("-");
		m_gridPosition.SetItemText(nIndex, 6, strText);
		if(bHasPriceTick)
			strText.Format("%0.2f", position.AveragePositionPrice-item.stopLosePoint*(bDirect?1.0:-1.0)*fPriceTick);
		else
			strText.Format("-");
		m_gridPosition.SetItemText(nIndex, 7, strText);
	}
	strText.Format("%0.2f", position.PositionProfit);
	m_gridPosition.SetItemText(nIndex, 8, strText);
}
 
void CTSSCtrlPluginDlg::UpdateRow(int nIndex, PlatformStru_PositionDetail& posiDetail) 
{
	if(nIndex<0)
		return;

	CString strText;
	m_gridPosiDetail.SetItemText(nIndex, 0, posiDetail.InstrumentID);
	m_gridPosiDetail.SetItemText(nIndex, 1, posiDetail.Direction==THOST_FTDC_D_Buy?"买":"卖");
	strText.Format("%d", posiDetail.Volume);
	m_gridPosiDetail.SetItemText(nIndex, 2, strText);
	double fLastPrice = 0.0;
	if(GetLastPrice(std::string(posiDetail.InstrumentID), fLastPrice))
		strText.Format("%0.2f", fLastPrice);
	else
		strText.Format("-");
	m_gridPosiDetail.SetItemText(nIndex, 3, strText);
	strText.Format("%0.2f", posiDetail.OpenPrice);
	m_gridPosiDetail.SetItemText(nIndex, 4, strText);
	m_gridPosiDetail.SetItemText(nIndex, 5, posiDetail.OpenDate);
	//int nRemainTime = GetRemainTime(std::string(posiDetail.OpenDate));
	//int nHandleTime = GetInstCanHandleTime(std::string(posiDetail.InstrumentID));
	//strText.Format("%d", nHandleTime-nRemainTime);
	strText.Format("%d", GetRemainTime(std::string(posiDetail.InstrumentID), std::string(posiDetail.OpenDate)));
	m_gridPosiDetail.SetItemText(nIndex, 6, strText);
}

void CTSSCtrlPluginDlg::ShowQueue(std::queue<INSTRUMENTCTRLDIRECT>& queueData) 
{
	std::map<string, int>::iterator it;
	int nIndex = -1;
	char strTemp[1024];
	std::string strKey;
	strKey.reserve(1024);
	while(!queueData.empty()) {
		INSTRUMENTCTRLDIRECT field = queueData.front();
		queueData.pop();

		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%s, %s", field.plotID, field.inst);
		strKey = strTemp;
		it = mapKey2Index_Plot.find(strKey);
		if(it != mapKey2Index_Plot.end()) {
			nIndex = GetRowIndexByRowID_Plot(it->second);
			if(nIndex>=0) {
				UpdateRow(nIndex, field);
			}
		}
	}
	m_gridPlotCtrl.Invalidate();
}

void CTSSCtrlPluginDlg::ShowQueue(std::queue<PLOTCTRLITEMCFG>& queueData) 
{
	std::map<string, int>::iterator it;
	int nIndex = -1;
	char strTemp[1024];
	std::string strKey;
	strKey.reserve(1024);
	while(!queueData.empty()) {
		PLOTCTRLITEMCFG field = queueData.front();
		queueData.pop();

		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%s, %s", field.plotID, field.inst);
		strKey = strTemp;
		it = mapKey2Index_Plot.find(strKey);
		if(it != mapKey2Index_Plot.end()) {
			nIndex = GetRowIndexByRowID_Plot(it->second);
		}
		else {
			int nRowID = GetNextRowID_Plot();
			nIndex = m_gridPlotCtrl.GetRowCount();
			m_gridPlotCtrl.SetRowCount(m_gridPlotCtrl.GetRowCount()+1);
			//nIndex = m_gridPlotCtrl.InsertRow("", m_gridPlotCtrl.GetRowCount()-1);
			if(nIndex>=0) {
				m_gridPlotCtrl.SetItemData(nIndex, 0, (LPARAM)nRowID);
				mapKey2Index_Plot.insert(std::make_pair(strKey, nRowID));
				mapIndex2Key_Plot.insert(std::make_pair(nRowID, strKey));
			}
		}
		if(nIndex>=0) {
			UpdateRow(nIndex, field);
		}
	}
	m_gridPlotCtrl.Invalidate();
}

void CTSSCtrlPluginDlg::ShowQueue(std::queue<PlatformStru_Position>& queueData, int nItemCount)
{
	std::map<string, int>::iterator it;
	int nIndex = 0;
	char strTemp[1024];
	std::string strKey;
	strKey.reserve(1024);

	m_gridPosition.SetRowCount(nItemCount+1);
	mapKey2Index_Position.clear();
	mapIndex2Key_Position.clear();

	while(!queueData.empty()) {
		PlatformStru_Position field = queueData.front();
		queueData.pop();

		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%s,%s,%c,%c", field.ExchangeID, field.InstrumentID, field.PosiDirection, field.HedgeFlag);
		strKey = strTemp;
		it = mapKey2Index_Position.find(strKey);
		if(it != mapKey2Index_Position.end()) {
			nIndex = GetRowIndexByRowID_Position(it->second);
		}
		else {
			int nRowID = GetNextRowID_Position();
			nIndex++;
			if(nIndex>=0) {
				m_gridPosition.SetItemData(nIndex, 0, (LPARAM)nRowID);
				mapKey2Index_Position.insert(std::make_pair(strKey, nRowID));
				mapIndex2Key_Position.insert(std::make_pair(nRowID, strKey));
			}
		}
		if(nIndex>=0) {
			UpdateRow(nIndex, field);
		}
	}
	m_gridPosition.Invalidate();
}

void CTSSCtrlPluginDlg::ShowQueue(std::queue<PlatformStru_PositionDetail>& queueData, int nItemCount)
{
	std::map<string, int>::iterator it;
	int nIndex = 0;
	char strTemp[1024];
	std::string strKey;
	strKey.reserve(1024);

	m_gridPosiDetail.SetRowCount(nItemCount+1);
	mapKey2Index_PosiDetail.clear();
	mapIndex2Key_PosiDetail.clear();

	while(!queueData.empty()) {
		PlatformStru_PositionDetail field = queueData.front();
		queueData.pop();

		memset(strTemp, 0, sizeof(strTemp));
		sprintf(strTemp, "%s,%s,%c", field.ExchangeID, field.TradeID, field.Direction);
		strKey = strTemp;
		it = mapKey2Index_PosiDetail.find(strKey);
		if(it != mapKey2Index_PosiDetail.end()) {
			nIndex = GetRowIndexByRowID_PosiDetail(it->second);
		}
		else {
			int nRowID = GetNextRowID_PosiDetail();
			nIndex++;
			//nIndex = m_gridPosiDetail.GetRowCount();
			//m_gridPosiDetail.SetRowCount(m_gridPosiDetail.GetRowCount()+1);
			////nIndex = m_gridPosiDetail.InsertRow("", m_gridPosiDetail.GetRowCount()-1);
			if(nIndex>=0) {
				m_gridPosiDetail.SetItemData(nIndex, 0, (LPARAM)nRowID);
				mapKey2Index_PosiDetail.insert(std::make_pair(strKey, nRowID));
				mapIndex2Key_PosiDetail.insert(std::make_pair(nRowID, strKey));
			}
		}
		if(nIndex>=0) {
			UpdateRow(nIndex, field);
		}
	}
	m_gridPosiDetail.Invalidate();
}

void CTSSCtrlPluginDlg::OnNMClickTabposition(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	switch(m_ctlTabPosition.GetCurSel()) {
	case 0:
		m_gridPosition.ShowWindow(SW_HIDE);
		m_gridPosiDetail.ShowWindow(SW_SHOW);
		break;
	case 1:
		m_gridPosition.ShowWindow(SW_SHOW);
		m_gridPosiDetail.ShowWindow(SW_HIDE);
		break;
	};
	*pResult = 0;
}

void CTSSCtrlPluginDlg::ShowMsgLog(CString& strMsg)
{
	CString strLog;
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);
	strLog.Format("%02d:%02d:%02d.%03d:\r\n    %s\r\n", 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, strMsg);
	m_ctlMsgText.SetSel(-1, -1);
	m_ctlMsgText.ReplaceSel(strLog);
}

void CTSSCtrlPluginDlg::CancleOrderLast(PlatformStru_OrderInfo& tInfo)
{
    CancelOrder(tInfo);

    SYSTEMTIME sys;
    GetLocalTime(&sys);
    TRACE("主动撤单 %d:%d:%d.%d  OrderRef=%d\n",sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,atoi(tInfo.OrderRef));

    ORDERCLOSEINFO oCloseInfo;
    oCloseInfo.szOrderRef = tInfo.OrderRef;
    strcpy(oCloseInfo.szInstrumentID , tInfo.InstrumentID);
    oCloseInfo.szDirection = tInfo.Direction;
    oCloseInfo.dTime = ::GetTickCount();
    oCloseInfo.bFlag = true;

    m_mapOrderCloseInfo.insert(make_pair(tInfo.OrderRef,oCloseInfo));
}