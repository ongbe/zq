// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "MAManageClientDoc.h"
#include "MAManageClientView.h"
#include "MainFrm.h"
#include "ModifyPwdDlg.h"
#include "BrokerManageDlg.h"
#include "ServerManageDlg.h"
#include "PermissionSetDlg.h"
#include "TCPDataService.h"
#include "ConfigMgr.h"
#include "AccountOperatorDlg.h"
#include "UserSetDlg.h"
#include "InfoDlg.h"
#include "BatchAddAccountDlg.h"
#include "MessageSendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_MODIFY_PWD, &CMainFrame::OnModifyPwd)
	ON_COMMAND(ID_BROKER_MANAGE, &CMainFrame::OnBrokerManage)
	ON_COMMAND(ID_QUIT, &CMainFrame::OnQuit)
	ON_COMMAND(ID_SERVER_MANAGE, &CMainFrame::OnServerManage)
	ON_COMMAND(ID_PERMISSION_SET, &CMainFrame::OnPermissionSet)
	ON_COMMAND(ID_TOOL_ADD, &CMainFrame::OnAddAccount)
	ON_COMMAND(ID_TOOL_MODIFY, &CMainFrame::OnModifyAccount)
	ON_COMMAND(ID_TOOL_DEL, &CMainFrame::OnDelAccount)
	ON_COMMAND(ID_TOOL_REFRESH, &CMainFrame::OnRefresh)
	ON_COMMAND(ID_POP_MODIFY, &CMainFrame::OnRClickModifyAccount)
	ON_COMMAND(ID_POP_DEL, &CMainFrame::OnRClickDelAccount)
	ON_COMMAND(ID_POP_STOP, &CMainFrame::OnRClickStopAccount)
	ON_UPDATE_COMMAND_UI(ID_MANAGE_LIST, &CMainFrame::OnUpdateManageList)
	ON_COMMAND(ID_MANAGE_LIST, &CMainFrame::OnManageList)
	ON_UPDATE_COMMAND_UI(ID_TOOL_ADD, &CMainFrame::OnUpdateToolAdd)
	ON_MESSAGE(RECV_DATA_MSG, &CMainFrame::OnRecvMessage)
	ON_MESSAGE(MSG_SEARCH, &CMainFrame::OnSearchMessage)
	ON_MESSAGE(MSG_LIST_SEL_CHANGE, &CMainFrame::OnListSelChange)
	ON_MESSAGE(MSG_MAIN_TAB_SEL_CHANGE, &CMainFrame::OnMainTabSelChange)
	ON_MESSAGE(MSG_MODIFY_ACCOUNT, &CMainFrame::OnModifyAccountMessage)
	ON_MESSAGE(MSG_DEL_ACCOUNT, &CMainFrame::OnDelAccountMessage)
	ON_MESSAGE(MSG_ADD_ACCOUNT, &CMainFrame::OnAddAccountMessage)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDD_TREECTRL, &CMainFrame::OnTreeCtrlSelChange)
	ON_NOTIFY(NM_RCLICK, IDD_TREECTRL, &CMainFrame::OnTreeCtrlRClick)
	ON_COMMAND(ID_TOOL_STOP, &CMainFrame::OnToolStop)
	ON_UPDATE_COMMAND_UI(ID_TOOL_MODIFY, &CMainFrame::OnUpdateToolModify)
	ON_UPDATE_COMMAND_UI(ID_TOOL_DEL, &CMainFrame::OnUpdateToolDel)
	ON_UPDATE_COMMAND_UI(ID_TOOL_STOP, &CMainFrame::OnUpdateToolStop)
	ON_COMMAND(ID_USER_SET, &CMainFrame::OnUserSet)
	ON_WM_TIMER()
	ON_COMMAND(ID_TOOL_BATCH_ADD, &CMainFrame::OnToolBatchAdd)
	ON_UPDATE_COMMAND_UI(ID_TOOL_BATCH_ADD, &CMainFrame::OnUpdateToolBatchAdd)
	ON_COMMAND(ID_TOOL_MESSAGE, &CMainFrame::OnToolMessage)
	ON_UPDATE_COMMAND_UI(ID_TOOL_MESSAGE, &CMainFrame::OnUpdateToolMessage)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	m_bAutoMenuEnable = FALSE;
	m_mapAccount.clear();
	m_mapAccountTree.clear();
	m_nSelAccountID = ConfigManager::GetInstance()->GetAccountID();
	memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	m_mapAllPermission.clear();
	m_vPermission.clear();
	m_vIDCardInfo.clear();
	m_bSearchResult = false;
	m_bAccountMenuStatus = true;
	m_bAddPermission = false;
	m_bModifyPermission = false;
	m_bDelPermission = false;
	m_bStatusChangePermission = false;
	m_bShowTreeData = false;

	memset(&m_foreAccountPermission, 0, sizeof(m_foreAccountPermission));
	memset(&m_bkgAccountPermission, 0, sizeof(m_bkgAccountPermission));
	memset(&m_brokerPermission, 0, sizeof(m_brokerPermission));
	memset(&m_serverPermission, 0, sizeof(m_serverPermission));
	memset(&m_permissionSetPermission, 0, sizeof(m_permissionSetPermission));

	m_pInfoDlg = NULL;
}

CMainFrame::~CMainFrame()
{
	if ( NULL != m_pInfoDlg )
	{
		m_pInfoDlg->DestroyWindow();
		delete m_pInfoDlg;
		m_pInfoDlg = NULL;
	}
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_BORDER_3D | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	m_imageList.Create(48,48,ILC_COLOR32|ILC_MASK,0,1);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_ADD));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MODIFY));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DEL));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_STOP));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_REFRESH));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MESSAGE));

	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageList);
	m_wndToolBar.GetToolBarCtrl().EnableToolTips(TRUE);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	m_toolBarDlg.Create( IDD_DIALOG_TOOL_BAR, &m_wndToolBar);
	CRect rc(350, 5, 950, 48);
	m_toolBarDlg.MoveWindow(&rc);
	m_toolBarDlg.ShowWindow(SW_SHOW);

	if (!m_wndDockPageBar.Create(_T("DockPageBar"), this, CSize(230,260),TRUE,IDD_DOCK_BAR))
	{
		TRACE0("Failed to create DockPageBar.\n");
		return -1;
	}

	m_wndDockPageBar.SetBarStyle(m_wndDockPageBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndDockPageBar.EnableDocking (CBRS_ALIGN_LEFT);
	DockControlBar(&m_wndDockPageBar, AFX_IDW_DOCKBAR_LEFT);

	if (!m_wndTree.Create(WS_CHILD|WS_VISIBLE|
		TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS,
		CRect(0, 0, 0, 0), this, IDD_TREECTRL))
	{
		TRACE0("Failed to create instant bar child\n");
		return -1;
	}

	m_imgState.Create(16,16,ILC_COLOR32|ILC_MASK,0,1);
	m_imgState.SetBkColor(RGB(255,255,255));
	m_imgState.Add(AfxGetApp()->LoadIcon(IDI_ICON_NOENTRY));
	m_wndTree.SetImageList(&m_imgState,TVSIL_NORMAL);

	m_pInfoDlg = new CInfoDlg();
	m_pInfoDlg->Create( CInfoDlg::IDD, this );
	m_pInfoDlg->CenterWindow();
	m_pInfoDlg->ShowWindow( SW_HIDE );

	m_wndDockPageBar.AddPage(&m_wndTree,"管理列表",4000);

	UpdateMenuStatus();

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	//请求权限列表及证件类型列表
	int nPermissionID = ConfigManager::GetInstance()->GetPermissonID();
	CTCPDataService::GetInstance()->SendData(Cmd_MA_QryPermissionByID_Req, 
		&nPermissionID, sizeof(nPermissionID));
	CTCPDataService::GetInstance()->SendData(Cmd_MA_QryIDCard_Req, NULL, 0);

	int nAccountID = ConfigManager::GetInstance()->GetAccountID();
	CTCPDataService::GetInstance()->SendData(Cmd_MA_QryPermissionByOwner_Req,
		&nAccountID, sizeof(nAccountID));
	
	CTCPDataService::GetInstance()->SendData(Cmd_MA_QryAccountByOwner_Req, &nAccountID, sizeof(nAccountID));
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.cx = 960;
	cs.cy = 600;

	int nWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int nHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	cs.x = (nWidth - cs.cx) / 2;
	cs.y = (nHeight - cs.cy) / 2;

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




void CMainFrame::OnModifyPwd()
{
	// TODO: Add your command handler code here
	CModifyPwdDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnBrokerManage()
{
	// TODO: Add your command handler code here
	CBrokerManageDlg dlg;
	dlg.SetPermissionData(m_brokerPermission);
	dlg.DoModal();
}

void CMainFrame::OnQuit()
{
	// TODO: Add your command handler code here
	if( IDYES == MessageBox(lpszQuit, lpszPrompt, MB_ICONQUESTION|MB_YESNO))
	{
		OnClose();
	}
}

void CMainFrame::OnServerManage()
{
	// TODO: Add your command handler code here
	CServerManageDlg dlg;
	dlg.SetPermissionData(m_serverPermission);
	dlg.DoModal();
}

void CMainFrame::OnPermissionSet()
{
	// TODO: Add your command handler code here
	CPermissionSetDlg dlg;
	dlg.SetPermissionData(m_permissionSetPermission);
	dlg.DoModal();

	std::map<int, Permission> newPermission = dlg.GetPermissions();
	UseNewPermissions(newPermission);
}

//BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
//{
//	return TRUE;
//}

void CMainFrame::OnUpdateManageList(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_wndDockPageBar.IsVisible());
}

void CMainFrame::OnManageList()
{
	// TODO: Add your command handler code here
	ShowControlBar(&m_wndDockPageBar, !m_wndDockPageBar.IsVisible(), FALSE);
}

void CMainFrame::OnAddAccount()
{
	CAccountOperatorDlg dlg;
	AccountType eType = ((CMAManageClientView*)GetActiveView())->GetAccountType();
	dlg.SetAccountType(eType);
	dlg.SetPermissionData(m_vPermission, m_permissionSetPermission);
	dlg.SetIDCardInfoData(m_vIDCardInfo);
	dlg.SetOperatorType(OPERATOR_ADD);
	if ( IDOK == dlg.DoModal())
	{
		m_curAccountInfo = dlg.GetAccountInfo();
		if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_AddAccount_Req,
			&m_curAccountInfo, sizeof(m_curAccountInfo)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
	}

	if ( dlg.IsPermissionChanged() )
	{
		std::map<int, Permission> newPermission = dlg.GetPermissions();
		UseNewPermissions(newPermission);
	}
}

void CMainFrame::OnModifyAccount()
{
	std::vector<int> vAccount;
	((CMAManageClientView*)GetActiveView())->GetSelectAccountID(vAccount);
	if ( vAccount.empty() )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	if ( vAccount.size() > 1 )
	{
		MessageBox(_T("不允许同时修改多条数据！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	int nAccountID = vAccount.at(0);
	std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(nAccountID);
	if ( it == m_mapAccount.end())
	{
		return;
	}

	if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
	{
		MessageBox(lpszCanModifySelDataOnly, lpszPrompt, MB_ICONERROR);
		return;
	}

	ModifyAccount(it->second.accountInfo);
}

void CMainFrame::OnDelAccount()
{
	std::vector<int> vAccount;
	((CMAManageClientView*)GetActiveView())->GetSelectAccountID(vAccount);
	if ( vAccount.empty() )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	std::vector<int> vDelAccount;
	bool bHasOtherCreatedAccount = false;
	for ( UINT i = 0; i < vAccount.size(); i++ )
	{
		int nAccountID = vAccount.at(i);
		std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(nAccountID);
		if ( it == m_mapAccount.end())
		{
			continue;
		}

		if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
		{
			bHasOtherCreatedAccount = true;
		}

		vDelAccount.push_back(nAccountID);
	}

	if ( vDelAccount.empty() )
	{
		return;
	}

	if ( bHasOtherCreatedAccount )
	{
		MessageBox(lpszCanDelSelfDataOnly, lpszPrompt, MB_ICONERROR);
		return;
	}

	if ( IDYES != MessageBox(lpszDelDataCaseCadeComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_DelAccount_Req,
		&vDelAccount[0], vDelAccount.size()*sizeof(int)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
}

void CMainFrame::OnRefresh()
{
	CTCPDataService::GetInstance()->SendData(Cmd_MA_QryIDCard_Req, NULL, 0);
	int nAccountID = ConfigManager::GetInstance()->GetAccountID();
	CTCPDataService::GetInstance()->SendData(Cmd_MA_QryPermissionByOwner_Req,
		&nAccountID, sizeof(nAccountID));
	CTCPDataService::GetInstance()->SendData(Cmd_MA_QryAccountByOwner_Req, &nAccountID, sizeof(nAccountID));
}

void CMainFrame::OnUpdateToolAdd(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bAddPermission);
}

LRESULT CMainFrame::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_UnLogon_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
		}
		else
		{
			//CString csMsg = (char*)(pRecvData->pData);
			//MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	if ( pRecvData->head.cmdid == Cmd_MA_Login_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			CTCPDataService::GetInstance()->SetLogonStatus(true);
		}
		else
		{
			//CString csMsg = (char*)(pRecvData->pData);
			//MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_ForceCloseClient_Rsp )
	{
		MessageBox(_T("此账户已被停用，您被迫下线！\n如有问题，请联系客服。"), lpszPrompt, MB_ICONINFORMATION);
		PostMessage(WM_CLOSE, 0, 0);
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryPermissionByID_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			Permission permissionInfo = *(Permission*)((char*)pRecvData->pData);

			SetModulePermissionData(permissionInfo);
			UpdateMenuStatus();
			UpdateToolBarStatus();
			UpdateClientAreaStatus();
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryPermissionByOwner_Rsp )
	{
		m_mapAllPermission.clear();
		m_vPermission.clear();

		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(Permission);
			for ( int i = 0; i < nCount; i++ )
			{
				Permission* pPermissionInfo = (Permission*)((char*)pRecvData->pData + i*sizeof(Permission));
				m_mapAllPermission[pPermissionInfo->nPermissionID] = *pPermissionInfo;
				if ( pPermissionInfo->nPermissionDataSize != 0 )
				{
					m_vPermission.push_back(*pPermissionInfo);
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryIDCard_Rsp )
	{
		m_vIDCardInfo.clear();

		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(IDCardInfo);
			for ( int i = 0; i < nCount; i++ )
			{
				IDCardInfo* pIDCardInfo = (IDCardInfo*)((char*)pRecvData->pData + i*sizeof(IDCardInfo));
				m_vIDCardInfo.push_back(*pIDCardInfo);	
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(AccountInfoEx);
			for ( int i = 0; i < nCount; i++ )
			{
				AccountInfoEx* pAccountInfo = (AccountInfoEx*)((char*)pRecvData->pData + i*sizeof(AccountInfoEx));
				m_tempMapAccount[pAccountInfo->accountInfo.nAccountID] = *pAccountInfo;
			}

			if ( pRecvData->head.userdata2 == pRecvData->head.userdata3 - 1)
			{
				//分包数据已收齐，开始处理
				m_mapAccount = m_tempMapAccount;
				m_tempMapAccount.clear();

				m_nSelAccountID = ConfigManager::GetInstance()->GetAccountID();
				m_wndTree.DeleteAllItems();
				m_mapAccountTree.clear();
				((CMAManageClientView*)GetActiveView())->ClearAllData();
				m_toolBarDlg.ClearItems();

				std::map<int, AccountInfoEx>::iterator it_account = m_mapAccount.begin();
				for ( ; it_account != m_mapAccount.end(); it_account++ )
				{
					AccountInfoEx info = it_account->second;
					std::map<int, map<int, AccountInfoEx>>::iterator it = 
						m_mapAccountTree.find(info.accountInfo.nOwnerAccountID);
					if ( it != m_mapAccountTree.end() )
					{
						it->second.insert(make_pair(info.accountInfo.nAccountID, info));
					}
					else
					{
						std::map<int, AccountInfoEx> tempAccountMap;
						tempAccountMap.clear();
						tempAccountMap[info.accountInfo.nAccountID] = info;
						m_mapAccountTree[info.accountInfo.nOwnerAccountID] = tempAccountMap;
					}	
				}

				ShowTreeItems();
				if ( m_bShowTreeData )
				{
					m_toolBarDlg.SetData(m_mapAccount);
				}

				std::map<int, AccountInfoEx> mapShowData;
				mapShowData.clear();
				CalcShowData(m_nSelAccountID, mapShowData);
				((CMAManageClientView*)GetActiveView())->SetData(mapShowData);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_AddAccount_Rsp )
	{
		//批量导入返回不处理
		if( 0 != pRecvData->head.seq)
		{
			return 0;
		}

		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curAccountInfo.nAccountID = *(int*)pRecvData->pData;
			AccountInfoEx accountInfoEx;
			GetAccountInfoEx(m_curAccountInfo, accountInfoEx);
			m_mapAccount[accountInfoEx.accountInfo.nAccountID] = accountInfoEx;
			std::map<int, map<int, AccountInfoEx>>::iterator it = 
				m_mapAccountTree.find(m_curAccountInfo.nOwnerAccountID);
			if ( it != m_mapAccountTree.end() )
			{
				it->second.insert(make_pair(accountInfoEx.accountInfo.nAccountID, accountInfoEx));
			}
			else
			{
				std::map<int, AccountInfoEx> tempAccountMap;
				tempAccountMap.clear();
				tempAccountMap[accountInfoEx.accountInfo.nAccountID] = accountInfoEx;
				m_mapAccountTree[accountInfoEx.accountInfo.nOwnerAccountID] = tempAccountMap;
			}

			if ( accountInfoEx.accountInfo.nAccountType == ACCOUNT_TYPE_MANAGE )
			{
				//刷新树形列表
				AddChildItem(accountInfoEx.accountInfo);
				if ( m_bShowTreeData )
				{
					m_toolBarDlg.AddItem(accountInfoEx.accountInfo.nAccountID, 
						accountInfoEx.accountInfo.szAccount);
				}
			}

			if ( m_bSearchResult )
			{
				MessageBox(lpszAddAccountSuccess, lpszPrompt, MB_ICONINFORMATION);
			}
			else
			{
				if ( m_nSelAccountID == m_curAccountInfo.nOwnerAccountID )
				{
					((CMAManageClientView*)GetActiveView())->AddAccount(accountInfoEx);
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_ModifyAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curAccountInfo.nAccountID = *(int*)pRecvData->pData;

			//判断权限属性2是否改变，若改变，需要重新获取数据
			if ( m_curAccountInfo.nAccountID == ConfigManager::GetInstance()->GetAccountID())
			{
				std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(m_curAccountInfo.nAccountID);
				if ( it != m_mapAccount.end() )
				{
					if ( it->second.accountInfo.nViewPermission2 != m_curAccountInfo.nViewPermission2 )
					{
						OnRefresh();
						return 0;
					}
				}
			}

			AccountInfoEx accountInfoEx;
			GetAccountInfoEx(m_curAccountInfo, accountInfoEx);
			m_mapAccount[accountInfoEx.accountInfo.nAccountID] = accountInfoEx;
			std::map<int, map<int, AccountInfoEx>>::iterator it_owner = 
				m_mapAccountTree.find(accountInfoEx.accountInfo.nOwnerAccountID);
			if ( it_owner != m_mapAccountTree.end() )
			{
				std::map<int, AccountInfoEx>::iterator it_account = 
					it_owner->second.find(accountInfoEx.accountInfo.nAccountID);
				if ( it_account != it_owner->second.end())
				{
					it_account->second = accountInfoEx;
				}
			}

			//if ( m_bSearchResult )
			//{
			//	MessageBox(lpszModifyAccountSuccess, lpszPrompt, MB_ICONINFORMATION);
			//}
			//else
			//{
				if ( m_nSelAccountID == accountInfoEx.accountInfo.nOwnerAccountID )
				{
					((CMAManageClientView*)GetActiveView())->ModifyAccount(accountInfoEx);
				}
			//}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_DelAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(int);
			for ( int i = 0; i < nCount; i++ )
			{
				int nAccountID = *(int*)((char*)pRecvData->pData+i*sizeof(int));
				AccountInfo info;
				memset(&info, 0, sizeof(info));
				std::map<int, AccountInfoEx>::iterator it = 
					m_mapAccount.find(nAccountID);
				if ( it != m_mapAccount.end())
				{
					info = it->second.accountInfo;
					m_mapAccount.erase(it);
				}

				std::map<int, map<int, AccountInfoEx>>::iterator it_owner = 
					m_mapAccountTree.find(info.nOwnerAccountID);
				if ( it_owner != m_mapAccountTree.end() )
				{
					std::map<int, AccountInfoEx>::iterator it_account = 
						it_owner->second.find(nAccountID);
					if ( it_account != it_owner->second.end())
					{
						it_owner->second.erase(it_account);
					}

					if ( it_owner->second.empty())
					{
						m_mapAccountTree.erase(it_owner);
					}
				}

				if ( info.nAccountType == ACCOUNT_TYPE_MANAGE )
				{
					DelChildItem(nAccountID);
					if ( m_bShowTreeData )
					{
						m_toolBarDlg.DelItem(nAccountID);
					}
				}

				if ( m_nSelAccountID == info.nOwnerAccountID )
				{
					((CMAManageClientView*)GetActiveView())->DelAccount(
						nAccountID, 
						static_cast<AccountType>(info.nAccountType));
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_DelAccountCaseCade_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			OnRefresh();
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}

	return 0;
}

void CMainFrame::ShowTreeItems()
{
	if ( !m_bShowTreeData )
	{
		return;
	}

	int nSelAccountID = ConfigManager::GetInstance()->GetAccountID();
	std::map<int, AccountInfoEx>::iterator it_acount = m_mapAccount.find(nSelAccountID);
	if ( it_acount == m_mapAccount.end() )
	{
		return;
	}
	else
	{
		HTREEITEM hRoot = NULL;
		if ( it_acount->second.accountInfo.nViewPermission1 == 0 )
		{
			hRoot = m_wndTree.InsertItem(
				it_acount->second.accountInfo.szAccount, 0, 0 );
		}
		else
		{
			hRoot = m_wndTree.InsertItem(it_acount->second.accountInfo.szAccount, 1, 1);
		}
		m_wndTree.SetItemData(hRoot, it_acount->second.accountInfo.nAccountID);
		AddAccountItems(nSelAccountID, hRoot);
	}
}

void CMainFrame::AddAccountItems( int nOwnerID, HTREEITEM hParent )
{
	if ( !m_bShowTreeData )
	{
		return;
	}

	std::map<int, std::map<int, AccountInfoEx>>::iterator it_owner = 
		m_mapAccountTree.find(nOwnerID);
	if ( it_owner == m_mapAccountTree.end() )
	{
		return;
	}
	else
	{
		std::map<int, AccountInfoEx> tempAccountMap = it_owner->second;
		if ( tempAccountMap.empty())
		{
			return;
		}

		std::map<int, AccountInfoEx>::iterator it_account = tempAccountMap.begin();
		for ( ; it_account != tempAccountMap.end(); it_account++ )
		{
			if ( it_account->second.accountInfo.nAccountType == ACCOUNT_TYPE_MANAGE )
			{
				HTREEITEM hItem = NULL;
				if ( it_account->second.accountInfo.nViewPermission1 == 0 )
				{
					hItem = m_wndTree.InsertItem(
						it_account->second.accountInfo.szAccount, 0, 0, hParent );
				}
				else
				{
					hItem = m_wndTree.InsertItem(it_account->second.accountInfo.szAccount, 1, 1, hParent);
				}
				m_wndTree.SetItemData(hItem, it_account->second.accountInfo.nAccountID);
				AddAccountItems(it_account->second.accountInfo.nAccountID, hItem);
			}
		}
	}

}

void CMainFrame::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CFrameWnd::OnDestroy();

	// TODO: Add your message handler code here
}

void CMainFrame::AddChildItem( const AccountInfo& accountInfo )
{
	if ( !m_bShowTreeData )
	{
		return;
	}

	HTREEITEM hRoot = m_wndTree.GetRootItem();
	HTREEITEM hItem = NULL;
	if ( accountInfo.nViewPermission1 == 0 )
	{
		hRoot = m_wndTree.InsertItem(accountInfo.szAccount, 0, 0, hRoot );
	}
	else
	{
		hRoot = m_wndTree.InsertItem(accountInfo.szAccount, 1, 1, hRoot);
	}
	m_wndTree.SetItemData(hItem, accountInfo.nAccountID);
}

void CMainFrame::DelChildItem( int nAccountID )
{
	if ( !m_bShowTreeData )
	{
		return;
	}

	HTREEITEM hRootItem = m_wndTree.GetRootItem();
	DelChildItem(hRootItem, nAccountID);
}

void CMainFrame::DelChildItem( HTREEITEM hParent, int nAccountID )
{
	if ( hParent == NULL )
	{
		return;
	}

	//查找子节点
	HTREEITEM hChildItem = m_wndTree.GetChildItem(hParent);
	while ( hChildItem != NULL )
	{
		HTREEITEM hBrotherItem = m_wndTree.GetNextSiblingItem(hChildItem);
		if ( m_wndTree.GetItemData( hChildItem ) == nAccountID )
		{
			m_wndTree.DeleteItem( hChildItem );
		}
		else
		{
			DelChildItem(hChildItem, nAccountID);
		}

		//处理子节点的兄弟节点和其子节点
		hChildItem = hBrotherItem;
	}
}

void CMainFrame::CalcShowData( int nOwnerID, std::map<int, AccountInfoEx>& mapShowData )
{
	std::map<int, std::map<int, AccountInfoEx>>::iterator it_owner = 
		m_mapAccountTree.find(nOwnerID);
	if ( it_owner == m_mapAccountTree.end() )
	{
		return;
	}
	else
	{
		std::map<int, AccountInfoEx> tempAccountMap = it_owner->second;
		if ( tempAccountMap.empty())
		{
			return;
		}

		std::map<int, AccountInfoEx>::iterator it_account = tempAccountMap.begin();
		for ( ; it_account != tempAccountMap.end(); it_account++ )
		{
			mapShowData[it_account->second.accountInfo.nAccountID] = it_account->second;

			CalcShowData(it_account->second.accountInfo.nAccountID, mapShowData);
		}
	}
}

void CMainFrame::OnTreeCtrlSelChange( NMHDR* pNMHDR, LRESULT* pResult )
{
	//   TODO:   Add   your   control   notification   handler   code   here 
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR; 

	if( pNMTreeView->itemNew.state & TVIS_SELECTED )
	{
		m_nSelAccountID = m_wndTree.GetItemData(pNMTreeView->itemNew.hItem);

		((CMAManageClientView*)GetActiveView())->ClearAllData();
		std::map<int, AccountInfoEx> mapShowData;
		mapShowData.clear();
		CalcShowData(m_nSelAccountID, mapShowData);
		((CMAManageClientView*)GetActiveView())->SetData(mapShowData);
	}

	*pResult = 0;
}

void CMainFrame::OnTreeCtrlRClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR; 

	CPoint point;
	GetCursorPos(&point);
	CPoint pointInTree = point;
	m_wndTree.ScreenToClient(&pointInTree);

	HTREEITEM item;
	UINT flag = TVHT_ONITEM;
	item = m_wndTree.HitTest(pointInTree, &flag);
	if(item != NULL)
	{
		m_wndTree.SelectItem(item);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_TREE);
		CMenu* pPopup = menu.GetSubMenu(0);

		if ( m_bkgAccountPermission.nCommonOperators & PERMISSION_BKG_COMMON_MODIFY )
		{
			pPopup->EnableMenuItem(ID_POP_MODIFY, MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_POP_MODIFY, MF_GRAYED);
		}

		if ( m_bkgAccountPermission.nCommonOperators & PERMISSION_BKG_COMMON_DEL )
		{
			pPopup->EnableMenuItem(ID_POP_DEL, MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_POP_DEL, MF_GRAYED);
		}

		if ( m_bkgAccountPermission.nCommonOperators & PERMISSION_BKG_COMMON_CHANGE_STATUS )
		{
			pPopup->EnableMenuItem(ID_POP_STOP, MF_ENABLED);
		}
		else
		{
			pPopup->EnableMenuItem(ID_POP_STOP, MF_GRAYED);
		}

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
			point.x, point.y, this);
	}

	*pResult = 0;
}

void CMainFrame::GetAccountInfoEx( const AccountInfo& accountInfo, AccountInfoEx& accountInfoEx )
{
	memset(&accountInfoEx, 0, sizeof(accountInfoEx));
	accountInfoEx.accountInfo = accountInfo;

	//填充所有者账户
	std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(accountInfo.nOwnerAccountID);
	if ( it != m_mapAccount.end() )
	{
		strcpy(accountInfoEx.szOwnerAccount, it->second.accountInfo.szAccount);
	}

	//填充证件类型名称
	for ( UINT i = 0; i < m_vIDCardInfo.size(); i++ )
	{
		if ( accountInfo.nIDCardType == m_vIDCardInfo[i].nIDCardType )
		{
			strcpy(accountInfoEx.szIDCardName, m_vIDCardInfo[i].szIDCardName);
			break;
		}
	}

	//填充权限名
	std::map<int, Permission>::iterator it_permission = m_mapAllPermission.find(accountInfo.nPermissionID);
	if ( it_permission != m_mapAllPermission.end())
	{
		strcpy(accountInfoEx.szPermissionName, it_permission->second.szName);
	}
}

void CMainFrame::OnToolStop()
{
	// TODO: Add your command handler code here
	std::vector<int> vAccount;
	((CMAManageClientView*)GetActiveView())->GetSelectAccountID(vAccount);
	if ( vAccount.empty() )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	if ( vAccount.size() > 1 )
	{
		MessageBox(_T("不允许同时修改多条数据！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	int nAccountID = vAccount.at(0);
	StopAccount(nAccountID);
}

LRESULT CMainFrame::OnSearchMessage( WPARAM wParam, LPARAM lParam )
{
	if ( !m_bShowTreeData )
	{
		return 0;
	}

	//取消树形控件节点的选中状态
	m_wndTree.SelectItem(NULL);

	m_bSearchResult = true;

	AccountType eCurAccountType = ((CMAManageClientView*)GetActiveView())->GetAccountType();
	((CMAManageClientView*)GetActiveView())->ClearData( eCurAccountType );

	int nOwnerAccountID = 0;
	CString csAccount;
	CString csDate;
	m_toolBarDlg.GetSearchParam(nOwnerAccountID, csAccount, csDate);
	std::map<int, AccountInfoEx> mapShowData;
	mapShowData.clear();
	if ( nOwnerAccountID == INVALID_ACOUNTID )
	{
		CalcShowData(ConfigManager::GetInstance()->GetAccountID(), mapShowData);
	}
	else
	{
		std::map<int, map<int, AccountInfoEx>>::iterator it_owner = m_mapAccountTree.find(nOwnerAccountID);
		if ( it_owner != m_mapAccountTree.end() )
		{
			mapShowData = it_owner->second;
		}
	}

	std::map<int, AccountInfoEx> resultMap;
	std::map<int, AccountInfoEx>::iterator it_account = mapShowData.begin();
	for ( ; it_account != mapShowData.end(); it_account++ )
	{
		if ( eCurAccountType == it_account->second.accountInfo.nAccountType)
		{
			if ( csAccount.IsEmpty() && csDate.IsEmpty())
			{
				resultMap[it_account->second.accountInfo.nAccountID] = it_account->second;
			}
			else if ( !csAccount.IsEmpty() && csDate.IsEmpty() )
			{
				CString csTempAccount = it_account->second.accountInfo.szAccount;
				if ( -1 != csTempAccount.Find(csAccount) )
				{
					resultMap[it_account->second.accountInfo.nAccountID] = it_account->second;
				}
			}
			else if ( csAccount.IsEmpty() && !csDate.IsEmpty())
			{
				CString csTempDate = it_account->second.accountInfo.szDate;
				if ( csTempDate <= csDate )
				{
					resultMap[it_account->second.accountInfo.nAccountID] = it_account->second;
				}
			}
			else
			{
				CString csTempAccount = it_account->second.accountInfo.szAccount;
				CString csTempDate = it_account->second.accountInfo.szDate;
				if ( -1 != csTempAccount.Find(csAccount) && csTempDate <= csDate )
				{
					resultMap[it_account->second.accountInfo.nAccountID] = it_account->second;
				}
			}
		}
	}

	((CMAManageClientView*)GetActiveView())->SetData(eCurAccountType, resultMap);

	return 0;
}

LRESULT CMainFrame::OnListSelChange( WPARAM wParam, LPARAM lParam )
{
	//int nAccountID = lParam;
	//std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(nAccountID);
	//if ( it == m_mapAccount.end())
	//{
	//	m_bAccountMenuStatus = false;
	//}
	//else
	//{
	//	if ( it->second.accountInfo.nOwnerAccountID == ConfigManager::GetInstance()->GetAccountID())
	//	{
	//		m_bAccountMenuStatus = true;
	//	}
	//	else
	//	{
	//		m_bAccountMenuStatus = false;
	//	}
	//}

	return 0;
}

void CMainFrame::OnUpdateToolModify(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bAccountMenuStatus&&m_bModifyPermission);
}

void CMainFrame::OnUpdateToolDel(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bAccountMenuStatus&&m_bDelPermission);
}

void CMainFrame::OnUpdateToolStop(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bAccountMenuStatus&&m_bStatusChangePermission);
}

void CMainFrame::OnUserSet()
{
	// TODO: Add your command handler code here
	std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(
		ConfigManager::GetInstance()->GetAccountID());
	if ( it == m_mapAccount.end())
	{
		return;
	}

	m_curAccountInfo = it->second.accountInfo;
	CUserSetDlg dlg;
	dlg.SetIDCardInfoData(m_vIDCardInfo);
	dlg.SetAccountInfo(m_curAccountInfo);
	if ( IDOK == dlg.DoModal())
	{
		m_curAccountInfo = dlg.GetAccountInfo();
		if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyAccount_Req,
			&m_curAccountInfo, sizeof(m_curAccountInfo)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
	}
}

void CMainFrame::UpdateMenuStatus()
{
	if ( m_brokerPermission.nCommonOperators == 0 )
	{
		GetMenu()->GetSubMenu(3)->EnableMenuItem(ID_BROKER_MANAGE, MF_GRAYED);	
	}
	else
	{
		GetMenu()->GetSubMenu(3)->EnableMenuItem(ID_BROKER_MANAGE, MF_ENABLED);
	}

	if ( m_serverPermission.nCommonOperators == 0)
	{
		GetMenu()->GetSubMenu(3)->EnableMenuItem(ID_SERVER_MANAGE, MF_GRAYED);
	}
	else
	{
		GetMenu()->GetSubMenu(3)->EnableMenuItem(ID_SERVER_MANAGE, MF_ENABLED);
	}

	if ( m_permissionSetPermission.nCommonOperators == 0)
	{
		GetMenu()->GetSubMenu(4)->EnableMenuItem(ID_PERMISSION_SET, MF_GRAYED);
	}
	else
	{
		GetMenu()->GetSubMenu(4)->EnableMenuItem(ID_PERMISSION_SET, MF_ENABLED);
	}
}

void CMainFrame::SetModulePermissionData( const Permission& permissionInfo )
{
	memset(&m_foreAccountPermission, 0, sizeof(m_foreAccountPermission));
	memset(&m_bkgAccountPermission, 0, sizeof(m_bkgAccountPermission));
	memset(&m_brokerPermission, 0, sizeof(m_brokerPermission));
	memset(&m_serverPermission, 0, sizeof(m_serverPermission));
	memset(&m_permissionSetPermission, 0, sizeof(m_permissionSetPermission));

	int nCount = permissionInfo.nPermissionDataSize / sizeof(PermissionData);
	for ( int i = 0; i < nCount; i++ )
	{
		PermissionData* pPermissionData = (PermissionData*)(permissionInfo.permissionData + i*sizeof(PermissionData));
		switch ( pPermissionData->nModuleID )
		{
		case MODULE_FORE_ACCOUNT_MANAGE:
			memcpy(&m_foreAccountPermission, pPermissionData, sizeof(PermissionData));
			break;
		case MODULE_BKG_ACCOUNT_MANAGE:
			memcpy(&m_bkgAccountPermission, pPermissionData, sizeof(PermissionData));
			break;
		case MODULE_BROKER_MANAGE:
			memcpy(&m_brokerPermission, pPermissionData, sizeof(PermissionData));
			break;
		case MODULE_SERVER_MANAGE:
			memcpy(&m_serverPermission, pPermissionData, sizeof(PermissionData));
			break;
		case MODULE_PERMISSION_MANAGE:
			memcpy(&m_permissionSetPermission, pPermissionData, sizeof(PermissionData));
			break;
		default:
			break;
		}
	}

	((CMAManageClientView*)GetActiveView())->SetPermissionData(
		m_foreAccountPermission, m_bkgAccountPermission);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::UpdateToolBarStatus()
{
	m_bAddPermission = false;
	m_bModifyPermission = false;
	m_bDelPermission = false;
	m_bStatusChangePermission = false;

	PermissionData data;
	if(((CMAManageClientView*)GetActiveView())->GetAccountType() == ACCOUNT_TYPE_MANAGE )
	{
		data = m_bkgAccountPermission;
	}
	else
	{
		data = m_foreAccountPermission;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_ADD )
	{
		m_bAddPermission = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_MODIFY )
	{
		m_bModifyPermission = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_DEL )
	{
		m_bDelPermission = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_CHANGE_STATUS )
	{
		m_bStatusChangePermission = true;
	}
}

void CMainFrame::UpdateClientAreaStatus()
{
	m_bShowTreeData = false;
	if ( m_foreAccountPermission.nCommonOperators != 0 ||
		m_bkgAccountPermission.nCommonOperators != 0 )
	{
		m_bShowTreeData = true;
	}

	if ( m_foreAccountPermission.nCommonOperators == 0 )
	{
		((CMAManageClientView*)GetActiveView())->ShowForeListData(false);
	}
	else
	{
		((CMAManageClientView*)GetActiveView())->ShowForeListData(true);
	}

	if ( m_bkgAccountPermission.nCommonOperators == 0 )
	{
		((CMAManageClientView*)GetActiveView())->ShowBkgListData(false);
	}
	else
	{
		((CMAManageClientView*)GetActiveView())->ShowBkgListData(true);
	}
}

LRESULT CMainFrame::OnMainTabSelChange( WPARAM wParam, LPARAM lParam )
{
	UpdateToolBarStatus();

	return 0;
}

CString CMainFrame::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("账户号重复！");
	}
	else if ( -1 != csMsg.Find(_T("ORA-00291")) )
	{
		csTempMsg = _T("所选择的权限已被删除！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}

void CMainFrame::RefreshAccountPermissionName()
{
	int nAccount = ConfigManager::GetInstance()->GetAccountID();
	std::map<int, map<int, AccountInfoEx>>::iterator it_owner = 
		m_mapAccountTree.find(nAccount);
	if ( it_owner == m_mapAccountTree.end() )
	{
		return;
	}

	std::map<int, AccountInfoEx>::iterator it_account = it_owner->second.begin();
	for ( ; it_account != it_owner->second.end(); it_account++ )
	{
		std::map<int, Permission>::iterator it_permission = 
			m_mapAllPermission.find(it_account->second.accountInfo.nPermissionID);
		//如果权限名改变了，则同时修改m_mapAccount数据，并刷新列表
		if ( it_permission != m_mapAllPermission.end() 
			&& 0 != strcmp(it_permission->second.szName, it_account->second.szPermissionName))
		{
			strcpy(it_account->second.szPermissionName, it_permission->second.szName);

			std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(it_account->first);
			if ( it != m_mapAccount.end() )
			{
				strcpy(it->second.szPermissionName, it_permission->second.szName);
			}

			((CMAManageClientView*)GetActiveView())->ModifyAccount(it_account->second);
		}
	}
}

void CMainFrame::UseNewPermissions( const std::map<int, Permission>& newPermission )
{
	m_mapAllPermission = newPermission;
	std::map<int, Permission>::iterator it = m_mapAllPermission.begin();
	m_vPermission.clear();
	for ( ; it != m_mapAllPermission.end(); it++ )
	{
		if ( it->second.nPermissionDataSize != 0 )
		{
			m_vPermission.push_back(it->second);
		}
	}

	RefreshAccountPermissionName();
}

void CMainFrame::ModifyAccount( const AccountInfo& accountInfo )
{
	memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	m_curAccountInfo = accountInfo;

	CAccountOperatorDlg dlg;
	AccountType eType = ((CMAManageClientView*)GetActiveView())->GetAccountType();
	dlg.SetAccountType(eType);
	dlg.SetPermissionData(m_vPermission, m_permissionSetPermission);
	dlg.SetIDCardInfoData(m_vIDCardInfo);
	dlg.SetOperatorType(OPERATOR_MODIFY);
	dlg.SetAccountInfo(m_curAccountInfo);
	if ( IDOK == dlg.DoModal())
	{
		m_curAccountInfo = dlg.GetAccountInfo();
		if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyAccount_Req,
			&m_curAccountInfo, sizeof(m_curAccountInfo)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
	}

	if ( dlg.IsPermissionChanged() )
	{
		std::map<int, Permission> newPermission = dlg.GetPermissions();
		UseNewPermissions(newPermission);
	}
}

LRESULT CMainFrame::OnModifyAccountMessage( WPARAM wParam, LPARAM lParam )
{
	int nAccountID = lParam;
	std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(nAccountID);
	if ( it == m_mapAccount.end())
	{
		return 0;
	}

	if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
	{
		//MessageBox(lpszCanModifySelDataOnly, lpszPrompt, MB_ICONERROR);
		return 0;
	}

	ModifyAccount(it->second.accountInfo);
	return 0;
}

LRESULT CMainFrame::OnDelAccountMessage( WPARAM wParam, LPARAM lParam )
{
	return 0;
}

LRESULT CMainFrame::OnAddAccountMessage( WPARAM wParam, LPARAM lParam )
{
	return 0;
}

void CMainFrame::OnRClickModifyAccount()
{
	std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(m_nSelAccountID);
	if ( it == m_mapAccount.end())
	{
		return;
	}

	if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
	{
		MessageBox(lpszCanModifySelDataOnly, lpszPrompt, MB_ICONERROR);
		return;
	}

	ModifyAccount(it->second.accountInfo);
}

void CMainFrame::OnRClickDelAccount()
{
	std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(m_nSelAccountID);
	if ( it == m_mapAccount.end())
	{
		return;
	}

	if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
	{
		MessageBox(lpszCanDelSelfDataOnly, lpszPrompt, MB_ICONERROR);
		return;
	}

	if ( IDYES != MessageBox(lpszDelDataCaseCadeComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_DelAccount_Req,
		&m_nSelAccountID, sizeof(int)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
}

void CMainFrame::OnRClickStopAccount()
{
	StopAccount(m_nSelAccountID);
}

void CMainFrame::StopAccount( int nAccountID )
{
	std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(nAccountID);
	if ( it == m_mapAccount.end())
	{
		return;
	}

	if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
	{
		MessageBox(lpszCanModifySelDataOnly, lpszPrompt, MB_ICONERROR);
		return;
	}

	memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	m_curAccountInfo = it->second.accountInfo;
	if ( it->second.accountInfo.nAccountStatus == 1)
	{
		if( IDYES == MessageBox(lpszModifyAccountStatusWarning, lpszPrompt, MB_YESNO | MB_ICONQUESTION))
		{
			m_curAccountInfo.nAccountStatus = 0;
		}
		else
		{
			memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
			return;
		}
	}
	else
	{
		m_curAccountInfo.nAccountStatus = 1;
	}

	if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyAccount_Req,
		&m_curAccountInfo, sizeof(m_curAccountInfo)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
}


void CMainFrame::OnToolBatchAdd()
{
	// TODO: Add your command handler code here
	CBatchAddAccountDlg dlg;
	AccountType eType = ((CMAManageClientView*)GetActiveView())->GetAccountType();
	dlg.SetAccountType(eType);
	dlg.SetPermissions(m_vPermission);
	dlg.SetIDCardInfoData(m_vIDCardInfo);
	if ( IDOK == dlg.DoModal())
	{
	}
}

void CMainFrame::OnUpdateToolBatchAdd(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bAddPermission);
}

void CMainFrame::OnToolMessage()
{
	// TODO: Add your command handler code here
	CMessageSendDlg dlg;
	dlg.SetAccountData(ConfigManager::GetInstance()->GetAccountID(),
		m_mapAccount, m_mapAccountTree);

	AccountType eAccountType = ((CMAManageClientView*)GetActiveView())->GetAccountType();
	if ( eAccountType == ACCOUNT_TYPE_MANAGE )
	{
		dlg.SetMessageType(MESSAGE_COMMON);
	}
	else
	{
		dlg.SetMessageType(MESSAGE_PERSONAL);
	}

	std::vector<int> vAccount;
	((CMAManageClientView*)GetActiveView())->GetSelectAccountID(vAccount);
	dlg.SetSelectData(vAccount);

	dlg.DoModal();
}

void CMainFrame::OnUpdateToolMessage(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
