// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "RiskManage.h"
#include "RiskManageDoc.h"
#include "RiskManageView.h"
#include "MainFrm.h"
#include "ModifyPwdDlg.h"
#include "PermissionSetDlg.h"
#include "TCPDataService.h"
#include "ConfigMgr.h"
#include "ManageAccountOperatorDlg.h"
#include "TraderOperatorDlg.h"
#include "TrustTradeOperatorDlg.h"
#include "InfoDlg.h"
#include "AssetOrgManageDlg.h"
#include "BatchAddAccountDlg.h"
#include "FinancialProductDlg.h"
#include "WarningSetDlg.h"
#include "RiskMsgTemplateDlg.h"
#include "RiskIndSpecialSet.h"
#include "FundNetParamDlg.h"
#include "AutoForceCloseParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_MODIFY_PWD, &CMainFrame::OnModifyPwd)
	ON_COMMAND(ID_QUIT, &CMainFrame::OnQuit)
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
	ON_WM_TIMER()
	ON_COMMAND(ID_TOOL_BATCH_ADD, &CMainFrame::OnToolBatchAdd)
	ON_UPDATE_COMMAND_UI(ID_TOOL_BATCH_ADD, &CMainFrame::OnUpdateToolBatchAdd)
	ON_COMMAND(ID_ORG_MANAGE, &CMainFrame::OnOrgManage)
	ON_COMMAND(ID_PRODUCT_MANAGE, &CMainFrame::OnProductManage)
	ON_COMMAND(ID_WARNING_SET, &CMainFrame::OnWarningSet)
	ON_COMMAND(ID_MSG_SET, &CMainFrame::OnMsgSet)
	ON_UPDATE_COMMAND_UI(ID_ORG_MANAGE, &CMainFrame::OnUpdateOrgManage)
	ON_UPDATE_COMMAND_UI(ID_PRODUCT_MANAGE, &CMainFrame::OnUpdateProductManage)
	ON_UPDATE_COMMAND_UI(ID_PERMISSION_SET, &CMainFrame::OnUpdatePermissionSet)
	ON_UPDATE_COMMAND_UI(ID_WARNING_SET, &CMainFrame::OnUpdateWarningSet)
	ON_UPDATE_COMMAND_UI(ID_MSG_SET, &CMainFrame::OnUpdateMsgSet)
	ON_COMMAND(ID_SPECIAL_IND, &CMainFrame::OnSpecialInd)
	ON_UPDATE_COMMAND_UI(ID_SPECIAL_IND, &CMainFrame::OnUpdateSpecialInd)
	ON_COMMAND(ID_FUNDNET_PARAM, &CMainFrame::OnFundnetParam)
	ON_UPDATE_COMMAND_UI(ID_FUNDNET_PARAM, &CMainFrame::OnUpdateFundnetParam)
	ON_UPDATE_COMMAND_UI(ID_FORCE_CLOSE_PARAM, &CMainFrame::OnUpdateForceCloseParam)
	ON_COMMAND(ID_FORCE_CLOSE_PARAM, &CMainFrame::OnForceCloseParam)
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
	m_mapUserInfoEx.clear();
	m_tempMapUserInfo.clear();

	m_mapTrustTradeAccountEx.clear();
	m_tempMapTrustTradeAccount.clear();

	m_mapTraderInfoEx.clear();
	m_tempMapTraderInfo.clear();

	m_mapAssetMgmtOrg.clear();
	m_mapAssetMgmtOrgTree.clear();

	m_mapFinancialProduct.clear();
	m_mapFinancialProductTree.clear();

	m_nSelOrgID = ConfigManager::GetInstance()->GetAssetMgmtOrgID();
	memset(&m_curUserInfo, 0, sizeof(m_curUserInfo));
	memset(&m_curTraderInfo, 0, sizeof(m_curTraderInfo));
	memset(&m_curTrustTradeAccount, 0, sizeof(m_curTrustTradeAccount));

	m_mapIndModule.clear();
	m_mapAllPermission.clear();
	m_vIndicator.clear();

	m_bSearchResult = false;
	m_bAddPermission = false;
	m_bModifyPermission = false;
	m_bDelPermission = false;
	m_bStatusChangePermission = false;
	m_bBatchAddAccountPermission = false;
	m_bOrgManagePermission = false;
	m_bFinProductManagePermission = false;
	m_bPermissionSetPermission = false;
	m_bRiskManagePermission = false;
	m_bRiskEvtMsgManagePermission = false;
	m_bShowTreeData = true;

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
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SPECIAL_IND));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_REFRESH));
	//m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_MESSAGE));

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

	m_pInfoDlg = new CInfoDlg();
	m_pInfoDlg->Create( CInfoDlg::IDD, this );
	m_pInfoDlg->CenterWindow();
	m_pInfoDlg->ShowWindow( SW_HIDE );

	m_wndDockPageBar.AddPage(&m_wndTree,"管理列表",4000);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	OnRefresh();
	
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

void CMainFrame::OnQuit()
{
	// TODO: Add your command handler code here
	if( IDYES == MessageBox(lpszQuit, lpszPrompt, MB_ICONQUESTION|MB_YESNO))
	{
		OnClose();
	}
}

void CMainFrame::OnPermissionSet()
{
	// TODO: Add your command handler code here
	CPermissionSetDlg dlg;
	dlg.DoModal();

	//m_mapAllPermission = dlg.GetPermissionNames();
	//UseNewPermissions(newPermission);
	//OnRefresh();
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
	AccountType eType = ((CRiskManageView*)GetActiveView())->GetAccountType();
	if ( eType == ACCOUNT_MANAGE )
	{
		CManageAccountOperatorDlg dlg;
		dlg.SetAssetMgmtOrgTree(m_mapAssetMgmtOrgTree);
		dlg.SetFinancialProduct(m_mapFinancialProductTree);
		dlg.SetOperatorType(OPERATOR_ADD);
		if ( IDOK == dlg.DoModal())
		{
			m_curUserInfo = dlg.GetUserInfo();
			if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_AddAccount_Req,
				&m_curUserInfo, sizeof(m_curUserInfo)))
			{
				std::string strError = CTCPDataService::GetInstance()->GetLastError();
				MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
			}
		}
	}
	else if ( eType == ACCOUNT_TRUST_TRADE)
	{
		CTrustTradeOperatorDlg dlg;
		dlg.SetAssetMgmtOrgTree(m_mapAssetMgmtOrgTree);
		dlg.SetFinancialProduct(m_mapFinancialProductTree);
		dlg.SetTrader(m_mapTraderInfoEx);
		dlg.SetIndicator(m_vIndicator);
		dlg.SetOperatorType(OPERATOR_ADD);
		if ( IDOK == dlg.DoModal())
		{
			m_curTrustTradeAccount = dlg.GetTrustTradeAccount();
			if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_AddTradeAccount_Req,
				&m_curTrustTradeAccount, sizeof(m_curTrustTradeAccount)))
			{
				std::string strError = CTCPDataService::GetInstance()->GetLastError();
				MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
			}
		}

		std::map<int, RiskIndicatorModule> mapIndModule = dlg.GetRiskIndModule();
		//判断风险模型名称是否发生变化
		if( IsIndicatorModuleChanged(mapIndModule))
		{
			m_mapIndModule = mapIndModule;
			RefreshListData( false, false, false, true);
		}
	}
	else
	{
		CTraderOperatorDlg dlg;
		dlg.SetAssetMgmtOrgTree(m_mapAssetMgmtOrgTree);
		dlg.SetFinancialProduct(m_mapFinancialProductTree);
		dlg.SetOperatorType(OPERATOR_ADD);
		if ( IDOK == dlg.DoModal())
		{
			m_curTraderInfo = dlg.GetTraderInfo();
			if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_AddTrader_Req,
				&m_curTraderInfo, sizeof(m_curTraderInfo)))
			{
				std::string strError = CTCPDataService::GetInstance()->GetLastError();
				MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
			}
		}
	}
}

void CMainFrame::OnModifyAccount()
{
	std::vector<int> vAccount;
	((CRiskManageView*)GetActiveView())->GetSelectAccountID(vAccount);
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

	ModifyAccount(vAccount[0]);
}

void CMainFrame::OnDelAccount()
{
	std::vector<int> vAccount;
	((CRiskManageView*)GetActiveView())->GetSelectAccountID(vAccount);
	if ( vAccount.empty() )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	if ( IDYES != MessageBox(lpszDelDataComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	int nCmdID = 0;
	AccountType eType = ((CRiskManageView*)GetActiveView())->GetAccountType();
	switch(eType)
	{
	case ACCOUNT_TRUST_TRADE:
		nCmdID = Cmd_RM_DelTradeAccount_Req;
		break;
	case ACCOUNT_MANAGE:
		nCmdID = Cmd_RM_DelAccount_Req;
		break;
	case ACCOUNT_TRADER:
		nCmdID = Cmd_RM_DelTrader_Req;
		break;
	default:
		break;
	}
	memset(&m_curUserInfo, 0, sizeof(m_curUserInfo));
	if (!CTCPDataService::GetInstance()->SendData(nCmdID,
		&vAccount[0], vAccount.size()*sizeof(int)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
}

void CMainFrame::OnRefresh()
{
	//请求权限列表及证件类型列表
	int nPermissionID = ConfigManager::GetInstance()->GetPermissonID();
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryPermissionValueByID_Req, 
		&nPermissionID, sizeof(nPermissionID));

	int nAccountID = ConfigManager::GetInstance()->GetAccountID();
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryPermissionName_Req, NULL, 0);

	//查询所有理财产品信息
	int nOrgID = ConfigManager::GetInstance()->GetAssetMgmtOrgID();
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryFinancialProduct_Req, NULL, 0);
	//查询风险模型信息
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryRiskIndModule_Req, NULL, 0);
	//查询风险指标信息
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryRiskIndicator_Req, NULL, 0);
	//查询资管组织与理财产品关系
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryAssetOrgProRelation_Req, NULL, 0);
	//查询所有资管组织结构
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryAssetMgmtOrgByOrgID_Req, &nOrgID, sizeof(nOrgID));
	//查资管ID下属所有账户
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryAccountByOrgID_Req, &nOrgID, sizeof(nOrgID));
	//查询资管ID下属所有交易员账户
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryTraderByOrgID_Req, &nOrgID, sizeof(nOrgID));
	//查询资管ID下属所有委托交易账户
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryTradeAccountByOrgID_Req, &nOrgID, sizeof(nOrgID));
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
	if ( pRecvData->head.cmdid == Cmd_RM_Login_Rsp )
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
	else if ( pRecvData->head.cmdid == Cmd_RM_ForceCloseClient_Push )
	{
		MessageBox(_T("此账户已被停用，您被迫下线！\n如有问题，请联系客服。"), lpszPrompt, MB_ICONINFORMATION);
		PostMessage(WM_CLOSE, 0, 0);
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryPermissionValueByID_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			std::map<int, int> mapActionValue;
			int nCount = pRecvData->head.len / sizeof(PermissionValue);
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionValue* pPermissionValue = (PermissionValue*)((char*)pRecvData->pData + i*sizeof(PermissionValue));
				mapActionValue[pPermissionValue->nActionID] = pPermissionValue->nValue;
			}

			SetModulePermissionData(mapActionValue);
			UpdateClientAreaStatus();
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryPermissionName_Rsp )
	{
		m_mapAllPermission.clear();

		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(PermissionName);
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionName* pPermissionName = (PermissionName*)((char*)pRecvData->pData + i*sizeof(PermissionName));
				m_mapAllPermission[pPermissionName->nPermissionID] = *pPermissionName;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryAccountByOrgID_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(RiskMgmtUserInfo);
			for ( int i = 0; i < nCount; i++ )
			{
				RiskMgmtUserInfo* pUserInfo = (RiskMgmtUserInfo*)((char*)pRecvData->pData + i*sizeof(RiskMgmtUserInfo));
				m_tempMapUserInfo[pUserInfo->nRiskMgmtUserID] = *pUserInfo;
			}

			if ( pRecvData->head.userdata2 == pRecvData->head.userdata3 - 1)
			{
				//分包数据已收齐，开始处理
				m_mapUserInfoEx.clear();
				std::map<int, RiskMgmtUserInfo>::iterator it_temp = m_tempMapUserInfo.begin();
				for ( ; it_temp != m_tempMapUserInfo.end(); ++it_temp )
				{
					RiskMgmtUserInfoEx userInfoEx;
					memset(&userInfoEx, 0, sizeof(userInfoEx));
					GetUserInfoEx(it_temp->second, userInfoEx);
					m_mapUserInfoEx[userInfoEx.userInfo.nRiskMgmtUserID] = userInfoEx;
				}
				m_tempMapUserInfo.clear();

				((CRiskManageView*)GetActiveView())->ClearData(ACCOUNT_MANAGE);

				std::map<int, RiskMgmtUserInfoEx> mapShowData;
				mapShowData.clear();
				CalcShowData(m_nSelOrgID, mapShowData);
				((CRiskManageView*)GetActiveView())->SetData(mapShowData);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryTradeAccountByOrgID_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(TrustTradeAccount);
			for ( int i = 0; i < nCount; i++ )
			{
				TrustTradeAccount* pTrustTradeAccount = (TrustTradeAccount*)((char*)pRecvData->pData + i*sizeof(TrustTradeAccount));
				m_tempMapTrustTradeAccount[pTrustTradeAccount->nTradeAccountID] = *pTrustTradeAccount;
			}

			if ( pRecvData->head.userdata2 == pRecvData->head.userdata3 - 1)
			{
				//分包数据已收齐，开始处理
				m_mapTrustTradeAccountEx.clear();
				std::map<int, TrustTradeAccount>::iterator it_temp = m_tempMapTrustTradeAccount.begin();
				for ( ; it_temp != m_tempMapTrustTradeAccount.end(); ++it_temp )
				{
					TrustTradeAccountEx trustTradeEx;
					memset(&trustTradeEx, 0, sizeof(trustTradeEx));
					GetTrustTradeAccountEx(it_temp->second, trustTradeEx);
					m_mapTrustTradeAccountEx[trustTradeEx.trustAccount.nTradeAccountID] = trustTradeEx;
				}
				m_tempMapTrustTradeAccount.clear();

				((CRiskManageView*)GetActiveView())->ClearData(ACCOUNT_TRUST_TRADE);

				std::map<int, TrustTradeAccountEx> mapShowData;
				mapShowData.clear();
				CalcShowData(m_nSelOrgID, mapShowData);
				((CRiskManageView*)GetActiveView())->SetData(mapShowData);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryTraderByOrgID_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(TraderInfo);
			for ( int i = 0; i < nCount; i++ )
			{
				TraderInfo* pTraderInfo = (TraderInfo*)((char*)pRecvData->pData + i*sizeof(TraderInfo));
				m_tempMapTraderInfo[pTraderInfo->nTraderID] = *pTraderInfo;
			}

			if ( pRecvData->head.userdata2 == pRecvData->head.userdata3 - 1)
			{
				//分包数据已收齐，开始处理
				m_mapTraderInfoEx.clear();
				std::map<int, TraderInfo>::iterator it_temp = m_tempMapTraderInfo.begin();
				for ( ; it_temp != m_tempMapTraderInfo.end(); ++it_temp )
				{
					TraderInfoEx tradeInfoEx;
					memset(&tradeInfoEx, 0, sizeof(tradeInfoEx));
					GetTraderInfoEx(it_temp->second, tradeInfoEx);
					m_mapTraderInfoEx[tradeInfoEx.traderInfo.nTraderID] = tradeInfoEx;
				}
				m_tempMapTraderInfo.clear();

				((CRiskManageView*)GetActiveView())->ClearData(ACCOUNT_TRADER);

				std::map<int, TraderInfoEx> mapShowData;
				mapShowData.clear();
				CalcShowData(m_nSelOrgID, mapShowData);
				((CRiskManageView*)GetActiveView())->SetData(mapShowData);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryAssetMgmtOrgByOrgID_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapAssetMgmtOrg.clear();
			m_mapAssetMgmtOrgTree.clear();

			int nCount = pRecvData->head.len / sizeof(AssetMgmtOrganization);
			for ( int i = 0; i < nCount; i++ )
			{
				AssetMgmtOrganization* pAssetMgmtOrg = (AssetMgmtOrganization*)((char*)pRecvData->pData + i*sizeof(AssetMgmtOrganization));
				m_mapAssetMgmtOrg[pAssetMgmtOrg->nAssetMgmtOrgID] = *pAssetMgmtOrg;
			}

			m_wndTree.DeleteAllItems();
			((CRiskManageView*)GetActiveView())->ClearAllData();
			m_toolBarDlg.ClearItems();

			std::map<int, AssetMgmtOrganization>::iterator it_org = m_mapAssetMgmtOrg.begin();
			for ( ; it_org != m_mapAssetMgmtOrg.end(); it_org++ )
			{
				AssetMgmtOrganization info = it_org->second;
				std::map<int, map<int, AssetMgmtOrganization>>::iterator it = 
					m_mapAssetMgmtOrgTree.find(info.nUpperLevelOrgID);
				if ( it != m_mapAssetMgmtOrgTree.end() )
				{
					it->second.insert(make_pair(info.nAssetMgmtOrgID, info));
				}
				else
				{
					std::map<int, AssetMgmtOrganization> tempOrgMap;
					tempOrgMap.clear();
					tempOrgMap[info.nAssetMgmtOrgID] = info;
					m_mapAssetMgmtOrgTree[info.nUpperLevelOrgID] = tempOrgMap;
				}	
			}

			m_nSelOrgID = ConfigManager::GetInstance()->GetAssetMgmtOrgID();
			ShowTreeItems();
			m_wndTree.SelectItem(m_wndTree.GetRootItem());
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryFinancialProduct_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapFinancialProduct.clear();
			int nCount = pRecvData->head.len / sizeof(FinancialProduct);
			for ( int i = 0; i < nCount; i++ )
			{
				FinancialProduct* pFinancialProduct = (FinancialProduct*)((char*)pRecvData->pData + i*sizeof(FinancialProduct));
				m_mapFinancialProduct[pFinancialProduct->nFinancialProductID] = *pFinancialProduct;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryRiskIndModule_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapIndModule.clear();
			int nCount = pRecvData->head.len / sizeof(RiskIndicatorModule);
			for ( int i = 0; i < nCount; i++ )
			{
				RiskIndicatorModule* pIndModule = (RiskIndicatorModule*)((char*)pRecvData->pData + i*sizeof(RiskIndicatorModule));
				m_mapIndModule[pIndModule->nRiskIndModuleID] = *pIndModule;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryRiskIndicator_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_vIndicator.clear();
			int nCount = pRecvData->head.len / sizeof(RiskIndicator);
			for ( int i = 0; i < nCount; i++ )
			{
				RiskIndicator* pIndModule = (RiskIndicator*)((char*)pRecvData->pData + i*sizeof(RiskIndicator));
				m_vIndicator.push_back(*pIndModule);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryAssetOrgProRelation_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapFinancialProductTree.clear();
			int nCount = pRecvData->head.len / sizeof(AssetOrgProRelation);
			for ( int i = 0; i < nCount; i++ )
			{
				AssetOrgProRelation* pRelation = (AssetOrgProRelation*)((char*)pRecvData->pData + i*sizeof(AssetOrgProRelation));

				std::map<int, FinancialProduct>::iterator it_fin = 
					m_mapFinancialProduct.find(pRelation->nFinancialProductID);
				if ( it_fin == m_mapFinancialProduct.end() )
				{
					continue;
				}

				std::map<int, std::map<int, FinancialProduct>>::iterator it 
					= m_mapFinancialProductTree.find(pRelation->nAssetMgmtOrgID);
				if ( it != m_mapFinancialProductTree.end())
				{
					(it->second)[it_fin->second.nFinancialProductID] = it_fin->second;
				}
				else
				{
					std::map<int, FinancialProduct> mapTemp;
					mapTemp[it_fin->second.nFinancialProductID] = it_fin->second;
					m_mapFinancialProductTree[pRelation->nAssetMgmtOrgID] = mapTemp;
				}
			}

			m_toolBarDlg.ClearItems();
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_AddAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curUserInfo.nRiskMgmtUserID = *(int*)pRecvData->pData;
			RiskMgmtUserInfoEx userInfoEx;
			GetUserInfoEx(m_curUserInfo, userInfoEx);
			m_mapUserInfoEx[userInfoEx.userInfo.nRiskMgmtUserID] = userInfoEx;

			if ( m_bSearchResult )
			{
				MessageBox(lpszAddAccountSuccess, lpszPrompt, MB_ICONINFORMATION);
			}
			else
			{
				((CRiskManageView*)GetActiveView())->AddAccount(userInfoEx);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_AddTrader_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curTraderInfo.nTraderID = *(int*)pRecvData->pData;
			TraderInfoEx traderInfoEx;
			GetTraderInfoEx(m_curTraderInfo, traderInfoEx);
			m_mapTraderInfoEx[traderInfoEx.traderInfo.nTraderID] = traderInfoEx;

			if ( m_bSearchResult )
			{
				MessageBox(lpszAddAccountSuccess, lpszPrompt, MB_ICONINFORMATION);
			}
			else
			{
				((CRiskManageView*)GetActiveView())->AddAccount(traderInfoEx);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_AddTradeAccount_Rsp )
	{
		//批量导入返回不处理
		if( 0 != pRecvData->head.seq)
		{
			return 0;
		}

		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curTrustTradeAccount.nTradeAccountID = *(int*)pRecvData->pData;
			TrustTradeAccountEx trustTradeEx;
			GetTrustTradeAccountEx(m_curTrustTradeAccount, trustTradeEx);
			m_mapTrustTradeAccountEx[trustTradeEx.trustAccount.nTradeAccountID] = trustTradeEx;

			if ( m_bSearchResult )
			{
				MessageBox(lpszAddAccountSuccess, lpszPrompt, MB_ICONINFORMATION);
			}
			else
			{
				((CRiskManageView*)GetActiveView())->AddAccount(trustTradeEx);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_ModifyAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			if ( m_curUserInfo.nRiskMgmtUserID != *(int*)pRecvData->pData )
			{
				return 0;
			}

			RiskMgmtUserInfoEx userInfoEx;
			GetUserInfoEx(m_curUserInfo, userInfoEx);
			m_mapUserInfoEx[userInfoEx.userInfo.nRiskMgmtUserID] = userInfoEx;

			if ( m_bSearchResult )
			{
				MessageBox(lpszModifyAccountSuccess, lpszPrompt, MB_ICONINFORMATION);
			}
			else
			{
				((CRiskManageView*)GetActiveView())->ModifyAccount(userInfoEx);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_ModifyTrader_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			if ( m_curTraderInfo.nTraderID != *(int*)pRecvData->pData )
			{
				return 0;
			}

			TraderInfoEx traderInfoEx;
			GetTraderInfoEx(m_curTraderInfo, traderInfoEx);
			m_mapTraderInfoEx[traderInfoEx.traderInfo.nTraderID] = traderInfoEx;

			if ( m_bSearchResult )
			{
				MessageBox(lpszModifyAccountSuccess, lpszPrompt, MB_ICONINFORMATION);
			}
			else
			{
				((CRiskManageView*)GetActiveView())->ModifyAccount(traderInfoEx);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_ModifyTradeAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			if ( m_curTrustTradeAccount.nTradeAccountID != *(int*)pRecvData->pData )
			{
				return 0;
			}

			TrustTradeAccountEx trustTradeEx;
			GetTrustTradeAccountEx(m_curTrustTradeAccount, trustTradeEx);
			m_mapTrustTradeAccountEx[trustTradeEx.trustAccount.nTradeAccountID] = trustTradeEx;

			if ( m_bSearchResult )
			{
				MessageBox(lpszModifyAccountSuccess, lpszPrompt, MB_ICONINFORMATION);
			}
			else
			{
				((CRiskManageView*)GetActiveView())->ModifyAccount(trustTradeEx);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_DelAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(int);
			for ( int i = 0; i < nCount; i++ )
			{
				int nAccountID = *(int*)((char*)pRecvData->pData+i*sizeof(int));
				std::map<int, RiskMgmtUserInfoEx>::iterator it = 
					m_mapUserInfoEx.find(nAccountID);
				if ( it != m_mapUserInfoEx.end())
				{
					m_mapUserInfoEx.erase(it);
				}

				((CRiskManageView*)GetActiveView())->DelAccount(
					nAccountID, ACCOUNT_MANAGE);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_DelTrader_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(int);
			for ( int i = 0; i < nCount; i++ )
			{
				int nAccountID = *(int*)((char*)pRecvData->pData+i*sizeof(int));
				std::map<int, TraderInfoEx>::iterator it = 
					m_mapTraderInfoEx.find(nAccountID);
				if ( it != m_mapTraderInfoEx.end())
				{
					m_mapTraderInfoEx.erase(it);
				}

				((CRiskManageView*)GetActiveView())->DelAccount(
					nAccountID, ACCOUNT_TRADER);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_DelTradeAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(int);
			for ( int i = 0; i < nCount; i++ )
			{
				int nAccountID = *(int*)((char*)pRecvData->pData+i*sizeof(int));
				std::map<int, TrustTradeAccountEx>::iterator it = 
					m_mapTrustTradeAccountEx.find(nAccountID);
				if ( it != m_mapTrustTradeAccountEx.end())
				{
					m_mapTrustTradeAccountEx.erase(it);
				}

				((CRiskManageView*)GetActiveView())->DelAccount(
					nAccountID, ACCOUNT_TRUST_TRADE);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else
	{

	}

	return 0;
}

void CMainFrame::ShowTreeItems()
{
	if ( !m_bShowTreeData )
	{
		return;
	}

	int nAssetMgmtOrgID = ConfigManager::GetInstance()->GetAssetMgmtOrgID();
	std::map<int, AssetMgmtOrganization>::iterator it_org = m_mapAssetMgmtOrg.find(nAssetMgmtOrgID);
	if ( it_org == m_mapAssetMgmtOrg.end() )
	{
		return;
	}
	else
	{
		HTREEITEM hRoot = NULL;
		hRoot = m_wndTree.InsertItem(it_org->second.szName);

		m_wndTree.SetItemData(hRoot, it_org->second.nAssetMgmtOrgID);
		AddOrgItems(nAssetMgmtOrgID, hRoot);
	}
}

void CMainFrame::AddOrgItems( int nUpperLevelOrgID, HTREEITEM hParent )
{
	if ( !m_bShowTreeData )
	{
		return;
	}

	//显示资管区域
	std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it_owner = 
		m_mapAssetMgmtOrgTree.find(nUpperLevelOrgID);
	if ( it_owner != m_mapAssetMgmtOrgTree.end() )
	{
		std::map<int, AssetMgmtOrganization> tempOrgMap = it_owner->second;
		if ( tempOrgMap.empty())
		{
			return;
		}

		std::map<int, AssetMgmtOrganization>::iterator it_org = tempOrgMap.begin();
		for ( ; it_org != tempOrgMap.end(); it_org++ )
		{
			HTREEITEM hItem = m_wndTree.InsertItem(it_org->second.szName, hParent);
			m_wndTree.SetItemData(hItem, it_org->second.nAssetMgmtOrgID);
			AddOrgItems(it_org->second.nAssetMgmtOrgID, hItem);
		}
	}
}

void CMainFrame::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CFrameWnd::OnDestroy();

	// TODO: Add your message handler code here
}

void CMainFrame::CalcShowData( int nOrgID, std::map<int, RiskMgmtUserInfoEx>& mapShowData )
{
	std::map<int, RiskMgmtUserInfoEx>::iterator it = m_mapUserInfoEx.begin();
	for ( ; it != m_mapUserInfoEx.end(); it++ )
	{
		if ( it->second.userInfo.nAssetMgmtOrgID == nOrgID )
		{
			mapShowData[it->second.userInfo.nRiskMgmtUserID] = it->second;
		}
	}
}

void CMainFrame::CalcShowData( int nOrgID, std::map<int, TrustTradeAccountEx>& mapShowData )
{
	std::map<int, TrustTradeAccountEx>::iterator it = m_mapTrustTradeAccountEx.begin();
	for ( ; it != m_mapTrustTradeAccountEx.end(); it++ )
	{
		if ( it->second.trustAccount.nAssetMgmtOrgID == nOrgID )
		{
			mapShowData[it->second.trustAccount.nTradeAccountID] = it->second;
		}
	}
}

void CMainFrame::CalcShowData( int nOrgID, std::map<int, TraderInfoEx>& mapShowData )
{
	std::map<int, TraderInfoEx>::iterator it = m_mapTraderInfoEx.begin();
	for ( ; it != m_mapTraderInfoEx.end(); it++ )
	{
		if ( it->second.traderInfo.nAssetMgmtOrgID == nOrgID )
		{
			mapShowData[it->second.traderInfo.nTraderID] = it->second;
		}
	}
}

void CMainFrame::OnTreeCtrlSelChange( NMHDR* pNMHDR, LRESULT* pResult )
{
	//   TODO:   Add   your   control   notification   handler   code   here 
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR; 

	if( pNMTreeView->itemNew.state & TVIS_SELECTED )
	{
		m_nSelOrgID = m_wndTree.GetItemData(pNMTreeView->itemNew.hItem);

		((CRiskManageView*)GetActiveView())->ClearAllData();

		//管理/风控账户
		std::map<int, RiskMgmtUserInfoEx> mapRiskMgmtUserShowData;
		mapRiskMgmtUserShowData.clear();
		CalcShowData(m_nSelOrgID, mapRiskMgmtUserShowData);
		((CRiskManageView*)GetActiveView())->SetData(mapRiskMgmtUserShowData);

		//委托交易账户
		std::map<int, TrustTradeAccountEx> mapTrustTradeShowData;
		mapTrustTradeShowData.clear();
		CalcShowData(m_nSelOrgID, mapTrustTradeShowData);
		((CRiskManageView*)GetActiveView())->SetData(mapTrustTradeShowData);

		//委托交易账户
		std::map<int, TraderInfoEx> mapTraderInfoShowData;
		mapTraderInfoShowData.clear();
		CalcShowData(m_nSelOrgID, mapTraderInfoShowData);
		((CRiskManageView*)GetActiveView())->SetData(mapTraderInfoShowData);
	}

	*pResult = 0;
}

void CMainFrame::OnTreeCtrlRClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR; 

	*pResult = 0;
}

void CMainFrame::GetUserInfoEx( const RiskMgmtUserInfo& userInfo, RiskMgmtUserInfoEx& userInfoEx )
{
	memset(&userInfoEx, 0, sizeof(userInfoEx));
	userInfoEx.userInfo = userInfo;

	//填充管理区域
	std::map<int, AssetMgmtOrganization>::iterator it_org = m_mapAssetMgmtOrg.find(userInfo.nAssetMgmtOrgID);
	if ( it_org != m_mapAssetMgmtOrg.end() )
	{
		strcpy(userInfoEx.szAssetMgmtOrgName, it_org->second.szName);
	}

	//填充理财产品名
	std::map<int, FinancialProduct>::iterator it_fin = m_mapFinancialProduct.find(userInfo.nFinancialProductID);
	if ( it_fin != m_mapFinancialProduct.end() )
	{
		strcpy(userInfoEx.szFinancialProductName, it_fin->second.szName);
	}

	//填充权限名
	std::map<int, PermissionName>::iterator it_permission = m_mapAllPermission.find(userInfo.nPermissionID);
	if ( it_permission != m_mapAllPermission.end())
	{
		strcpy(userInfoEx.szPermissionName, it_permission->second.szName);
	}
}

void CMainFrame::GetTrustTradeAccountEx( const TrustTradeAccount& trustTradeAccount, 
										TrustTradeAccountEx& trustTradeAccountEx )
{
	memset(&trustTradeAccountEx, 0, sizeof(trustTradeAccountEx));
	trustTradeAccountEx.trustAccount = trustTradeAccount;

	//填充管理区域
	std::map<int, AssetMgmtOrganization>::iterator it_org = m_mapAssetMgmtOrg.find(trustTradeAccount.nAssetMgmtOrgID);
	if ( it_org != m_mapAssetMgmtOrg.end() )
	{
		strcpy(trustTradeAccountEx.szAssetMgmtOrgName, it_org->second.szName);
	}

	//填充理财产品名
	std::map<int, FinancialProduct>::iterator it_fin = m_mapFinancialProduct.find(trustTradeAccount.nFinancialProductID);
	if ( it_fin != m_mapFinancialProduct.end() )
	{
		strcpy(trustTradeAccountEx.szFinancialProductName, it_fin->second.szName);
	}

	//填充交易员
	std::map<int, TraderInfoEx>::iterator it_trader = m_mapTraderInfoEx.find(trustTradeAccount.nTraderID);
	if ( it_trader != m_mapTraderInfoEx.end())
	{
		strcpy(trustTradeAccountEx.szTraderName, it_trader->second.traderInfo.szName);
	}

	//填充风险模型
	std::map<int, RiskIndicatorModule>::iterator it_module = m_mapIndModule.find(trustTradeAccount.nRiskIndModuleID);
	if ( it_module != m_mapIndModule.end() )
	{
		strcpy(trustTradeAccountEx.szRiskIncModuleName, it_module->second.szName);
	}
}

void CMainFrame::GetTraderInfoEx( const TraderInfo& traderInfo, TraderInfoEx& traderInfoEx )
{
	memset(&traderInfoEx, 0, sizeof(traderInfoEx));
	traderInfoEx.traderInfo = traderInfo;

	//填充管理区域
	std::map<int, AssetMgmtOrganization>::iterator it_org = m_mapAssetMgmtOrg.find(traderInfo.nAssetMgmtOrgID);
	if ( it_org != m_mapAssetMgmtOrg.end() )
	{
		strcpy(traderInfoEx.szAssetMgmtOrgName, it_org->second.szName);
	}

	//填充理财产品名
	std::map<int, FinancialProduct>::iterator it_fin = m_mapFinancialProduct.find(traderInfo.nFinancialProductID);
	if ( it_fin != m_mapFinancialProduct.end() )
	{
		strcpy(traderInfoEx.szFinancialProductName, it_fin->second.szName);
	}
}

void CMainFrame::OnToolStop()
{
	// TODO: Add your command handler code here
	if(((CRiskManageView*)GetActiveView())->GetAccountType() != ACCOUNT_MANAGE)
	{
		MessageBox(_T("[停用/启用]功能只对管理员/风控员账号有效！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	std::vector<int> vAccount;
	((CRiskManageView*)GetActiveView())->GetSelectAccountID(vAccount);
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

	CString csAccount;
	m_toolBarDlg.GetSearchParam(csAccount);
	if ( csAccount.IsEmpty())
	{
		return 0;
	}

	//取消树形控件节点的选中状态
	m_wndTree.SelectItem(NULL);

	m_bSearchResult = true;

	AccountType eCurAccountType = ((CRiskManageView*)GetActiveView())->GetAccountType();
	((CRiskManageView*)GetActiveView())->ClearData( eCurAccountType );

	switch(eCurAccountType)
	{
	case ACCOUNT_TRUST_TRADE:
		{
			std::map<int, TrustTradeAccountEx> resultMap;
			resultMap.clear();
			std::map<int, TrustTradeAccountEx>::iterator it = m_mapTrustTradeAccountEx.begin();
			for ( ; it != m_mapTrustTradeAccountEx.end(); ++it )
			{
				CString csTempAccount = it->second.trustAccount.szAccount;
				if ( -1 != csTempAccount.Find(csAccount) )
				{
					resultMap[it->second.trustAccount.nTradeAccountID] = it->second;
				}
			}

			((CRiskManageView*)GetActiveView())->SetData(resultMap);
		}
		break;
	case ACCOUNT_MANAGE:
		{
			std::map<int, RiskMgmtUserInfoEx> resultMap;
			resultMap.clear();
			std::map<int, RiskMgmtUserInfoEx>::iterator it = m_mapUserInfoEx.begin();
			for ( ; it != m_mapUserInfoEx.end(); ++it )
			{
				CString csTempAccount = it->second.userInfo.szAccount;
				if ( -1 != csTempAccount.Find(csAccount) )
				{
					resultMap[it->second.userInfo.nRiskMgmtUserID] = it->second;
				}
			}

			((CRiskManageView*)GetActiveView())->SetData(resultMap);
		}
		break;
	case ACCOUNT_TRADER:
		{
			std::map<int, TraderInfoEx> resultMap;
			resultMap.clear();
			std::map<int, TraderInfoEx>::iterator it = m_mapTraderInfoEx.begin();
			for ( ; it != m_mapTraderInfoEx.end(); ++it )
			{
				CString csTempAccount = it->second.traderInfo.szName;
				if ( -1 != csTempAccount.Find(csAccount) )
				{
					resultMap[it->second.traderInfo.nTraderID] = it->second;
				}
			}

			((CRiskManageView*)GetActiveView())->SetData(resultMap);
		}
		break;
	default:
		break;
	}

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
	pCmdUI->Enable(m_bModifyPermission);
}

void CMainFrame::OnUpdateToolDel(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bDelPermission);
}

void CMainFrame::OnUpdateToolStop(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStatusChangePermission);
}

void CMainFrame::SetModulePermissionData( std::map<int, int>& mapActionValue )
{
	std::map<int, int>::iterator it = mapActionValue.begin();
	for ( ; it != mapActionValue.end(); ++it )
	{
		int nActionID = it->first;
		bool bValue = (it->second == 1) ? true : false;
		switch(nActionID)
		{
		case ACCOUNT_ADD:
			m_bAddPermission = bValue;
			break;
		case ACCOUNT_MODIFY:
			m_bModifyPermission = bValue;
			break;
		case ACCOUNT_DEL:
			m_bDelPermission = bValue;
			break;
		case ACCOUNT_STOP_ACTIVE:
			m_bStatusChangePermission = bValue;
			break;
		case ACCOUNT_BATCH_IMPORT:
			m_bBatchAddAccountPermission = bValue;
			break;
		case ORGANIZATION_MANAGE:
			m_bOrgManagePermission = bValue;
			break;
		case FIN_PRODUCT_MANAGE:
			m_bFinProductManagePermission = bValue;
			break;
		case PERMISSION_SET:
			m_bPermissionSetPermission = bValue;
			break;
		case RISK_MANAGE:
			m_bRiskManagePermission = bValue;
			break;
		case RISK_EVENT_MSG_MANAGE:
			m_bRiskEvtMsgManagePermission = bValue;
			break;
		default:
			break;
		}
	}
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::UpdateClientAreaStatus()
{
	//m_bShowTreeData = false;
	//if ( m_foreAccountPermission.nCommonOperators != 0 ||
	//	m_bkgAccountPermission.nCommonOperators != 0 )
	//{
	//	m_bShowTreeData = true;
	//}

	//if ( m_foreAccountPermission.nCommonOperators == 0 )
	//{
	//	((CRiskManageView*)GetActiveView())->ShowForeListData(false);
	//}
	//else
	//{
	//	((CRiskManageView*)GetActiveView())->ShowForeListData(true);
	//}

	//if ( m_bkgAccountPermission.nCommonOperators == 0 )
	//{
	//	((CRiskManageView*)GetActiveView())->ShowBkgListData(false);
	//}
	//else
	//{
	//	((CRiskManageView*)GetActiveView())->ShowBkgListData(true);
	//}
}

LRESULT CMainFrame::OnMainTabSelChange( WPARAM wParam, LPARAM lParam )
{

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
	//int nAccount = ConfigManager::GetInstance()->GetAccountID();
	//std::map<int, map<int, AccountInfoEx>>::iterator it_owner = 
	//	m_mapAccountTree.find(nAccount);
	//if ( it_owner == m_mapAccountTree.end() )
	//{
	//	return;
	//}

	//std::map<int, AccountInfoEx>::iterator it_account = it_owner->second.begin();
	//for ( ; it_account != it_owner->second.end(); it_account++ )
	//{
	//	std::map<int, Permission>::iterator it_permission = 
	//		m_mapAllPermission.find(it_account->second.accountInfo.nPermissionID);
	//	//如果权限名改变了，则同时修改m_mapAccount数据，并刷新列表
	//	if ( it_permission != m_mapAllPermission.end() 
	//		&& 0 != strcmp(it_permission->second.szName, it_account->second.szPermissionName))
	//	{
	//		strcpy(it_account->second.szPermissionName, it_permission->second.szName);

	//		std::map<int, AccountInfoEx>::iterator it = m_mapUserInfoEx.find(it_account->first);
	//		if ( it != m_mapUserInfoEx.end() )
	//		{
	//			strcpy(it->second.szPermissionName, it_permission->second.szName);
	//		}

	//		((CRiskManageView*)GetActiveView())->ModifyAccount(it_account->second);
	//	}
	//}
}

void CMainFrame::UseNewPermissions( const std::map<int, PermissionName>& newPermission )
{
	//m_mapAllPermission = newPermission;
	//std::map<int, Permission>::iterator it = m_mapAllPermission.begin();
	//m_vPermission.clear();
	//for ( ; it != m_mapAllPermission.end(); it++ )
	//{
	//	if ( it->second.nPermissionDataSize != 0 )
	//	{
	//		m_vPermission.push_back(it->second);
	//	}
	//}

	//RefreshAccountPermissionName();
}

void CMainFrame::ModifyRiskUser( const RiskMgmtUserInfo& userInfo )
{
	m_curUserInfo = userInfo;

	CManageAccountOperatorDlg dlg;
	dlg.SetAssetMgmtOrgTree(m_mapAssetMgmtOrgTree);
	dlg.SetFinancialProduct(m_mapFinancialProductTree);
	dlg.SetOperatorType(OPERATOR_MODIFY);
	dlg.SetUserInfo(m_curUserInfo);
	if ( IDOK == dlg.DoModal())
	{
		m_curUserInfo = dlg.GetUserInfo();
		if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_ModifyAccount_Req,
			&m_curUserInfo, sizeof(m_curUserInfo)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
	}
}

void CMainFrame::ModifyTrustTradeAccount( const TrustTradeAccount& trustTradeAccount)
{
	m_curTrustTradeAccount = trustTradeAccount;

	CTrustTradeOperatorDlg dlg;
	dlg.SetAssetMgmtOrgTree(m_mapAssetMgmtOrgTree);
	dlg.SetFinancialProduct(m_mapFinancialProductTree);
	dlg.SetTrader(m_mapTraderInfoEx);
	dlg.SetIndicator(m_vIndicator);
	dlg.SetOperatorType(OPERATOR_MODIFY);
	dlg.SetTrustTradeAccount(m_curTrustTradeAccount);
	if ( IDOK == dlg.DoModal())
	{
		m_curTrustTradeAccount = dlg.GetTrustTradeAccount();
		if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_ModifyTradeAccount_Req,
			&m_curTrustTradeAccount, sizeof(m_curTrustTradeAccount)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
	}

	std::map<int, RiskIndicatorModule> mapIndModule = dlg.GetRiskIndModule();
	//判断风险模型名称是否发生变化
	if( IsIndicatorModuleChanged(mapIndModule))
	{
		m_mapIndModule = mapIndModule;
		RefreshListData( false, false, false, true);
	}
}

void CMainFrame::ModifyTraderInfo( const TraderInfo& traderInfo)
{
	m_curTraderInfo = traderInfo;

	CTraderOperatorDlg dlg;
	dlg.SetAssetMgmtOrgTree(m_mapAssetMgmtOrgTree);
	dlg.SetFinancialProduct(m_mapFinancialProductTree);
	dlg.SetOperatorType(OPERATOR_MODIFY);
	dlg.SetTraderInfo(m_curTraderInfo);
	if ( IDOK == dlg.DoModal())
	{
		m_curTraderInfo = dlg.GetTraderInfo();
		if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_ModifyTrader_Req,
			&m_curTraderInfo, sizeof(m_curTraderInfo)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
	}
}

LRESULT CMainFrame::OnModifyAccountMessage( WPARAM wParam, LPARAM lParam )
{
	int nAccountID = lParam;
	ModifyAccount(nAccountID);

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
	//std::map<int, AccountInfoEx>::iterator it = m_mapUserInfoEx.find(m_nSelAccountID);
	//if ( it == m_mapUserInfoEx.end())
	//{
	//	return;
	//}

	//if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
	//{
	//	MessageBox(lpszCanModifySelDataOnly, lpszPrompt, MB_ICONERROR);
	//	return;
	//}

	//ModifyAccount(it->second.accountInfo);
}

void CMainFrame::OnRClickDelAccount()
{
	//std::map<int, AccountInfoEx>::iterator it = m_mapUserInfoEx.find(m_nSelAccountID);
	//if ( it == m_mapUserInfoEx.end())
	//{
	//	return;
	//}

	//if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
	//{
	//	MessageBox(lpszCanDelSelfDataOnly, lpszPrompt, MB_ICONERROR);
	//	return;
	//}

	//if ( IDYES != MessageBox(lpszDelDataCaseCadeComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	//{
	//	return;
	//}

	//memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	//if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_DelAccount_Req,
	//	&m_nSelAccountID, sizeof(int)))
	//{
	//	std::string strError = CTCPDataService::GetInstance()->GetLastError();
	//	MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	//}
}

void CMainFrame::OnRClickStopAccount()
{
	//StopAccount(m_nSelAccountID);
}

//void CMainFrame::StopAccount( int nAccountID )
//{
//	std::map<int, AccountInfoEx>::iterator it = m_mapUserInfoEx.find(nAccountID);
//	if ( it == m_mapUserInfoEx.end())
//	{
//		return;
//	}
//
//	if ( it->second.accountInfo.nOwnerAccountID != ConfigManager::GetInstance()->GetAccountID())
//	{
//		MessageBox(lpszCanModifySelDataOnly, lpszPrompt, MB_ICONERROR);
//		return;
//	}
//
//	memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
//	m_curAccountInfo = it->second.accountInfo;
//	if ( it->second.accountInfo.nAccountStatus == 1)
//	{
//		if( IDYES == MessageBox(lpszModifyAccountStatusWarning, lpszPrompt, MB_YESNO | MB_ICONQUESTION))
//		{
//			m_curAccountInfo.nAccountStatus = 0;
//		}
//		else
//		{
//			memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
//			return;
//		}
//	}
//	else
//	{
//		m_curAccountInfo.nAccountStatus = 1;
//	}
//
//	if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyAccount_Req,
//		&m_curAccountInfo, sizeof(m_curAccountInfo)))
//	{
//		std::string strError = CTCPDataService::GetInstance()->GetLastError();
//		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
//	}
//}


void CMainFrame::OnToolBatchAdd()
{
	// TODO: Add your command handler code here
	CBatchAddAccountDlg dlg;
	dlg.SetAssetMgmtOrgTree(m_mapAssetMgmtOrgTree);
	dlg.SetFinancialProduct(m_mapFinancialProductTree);
	dlg.SetTrader(m_mapTraderInfoEx);
	dlg.DoModal();
	if ( dlg.GetImportedDataCount() > 0 )
	{
		OnRefresh();
	}
}

void CMainFrame::OnUpdateToolBatchAdd(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bBatchAddAccountPermission);
}

void CMainFrame::OnOrgManage()
{
	// TODO: Add your command handler code here
	CAssetOrgManageDlg dlg;
	dlg.DoModal();

	std::map<int, AssetMgmtOrganization> mapAssetOrg = dlg.GetAssetMgmtOrg();
	if( NeedRefreshOrg(mapAssetOrg))
	{
		OnRefresh();
	}
}

void CMainFrame::OnProductManage()
{
	// TODO: Add your command handler code here
	CFinancialProductDlg dlg;
	dlg.DoModal();

	std::map<int, FinancialProduct> mapFinProduct = dlg.GetFinProduct();
	m_mapFinancialProductTree = dlg.GetOrgFinProductTree();

	bool bFinProductChanged = false;
	std::map<int, FinancialProduct>::iterator it_old = m_mapFinancialProduct.begin();
	for ( ; it_old != m_mapFinancialProduct.end(); ++it_old )
	{
		std::map<int, FinancialProduct>::iterator it_new = mapFinProduct.find(it_old->first);
		if ( it_new == mapFinProduct.end() )
		{
			bFinProductChanged = true;
			break;
		}
		else
		{
			if ( 0 != strcmp(it_old->second.szName, it_new->second.szName))
			{
				bFinProductChanged = true;
				break;
			}
		}
	}

	if ( bFinProductChanged )
	{
		m_mapFinancialProduct = mapFinProduct;
		RefreshListData(false, true, false, false);
	}
}

void CMainFrame::OnWarningSet()
{
	// TODO: Add your command handler code here
	CWarningSetDlg dlg;
	dlg.SetIndicator(m_vIndicator);
	dlg.DoModal();

	std::map<int, RiskIndicatorModule> mapIndModule = dlg.GetRiskIndModule();
	//判断风险模型名称是否发生变化
	if( IsIndicatorModuleChanged(mapIndModule))
	{
		m_mapIndModule = mapIndModule;
		RefreshListData( false, false, false, true);
	}
}

void CMainFrame::OnMsgSet()
{
	// TODO: Add your command handler code here
	CRiskMsgTemplateDlg dlg;
	dlg.SetIndicator(m_vIndicator);
	dlg.DoModal();
}

void CMainFrame::OnUpdateOrgManage(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bOrgManagePermission);
}

void CMainFrame::OnUpdateProductManage(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bFinProductManagePermission);
}

void CMainFrame::OnUpdatePermissionSet(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bPermissionSetPermission);
}

void CMainFrame::OnUpdateWarningSet(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bRiskManagePermission);
}

void CMainFrame::OnUpdateMsgSet(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bRiskEvtMsgManagePermission);
}

void CMainFrame::StopAccount( int nAccountID )
{
	std::map<int, RiskMgmtUserInfoEx>::iterator it = m_mapUserInfoEx.find(nAccountID);
	if ( it == m_mapUserInfoEx.end())
	{
		return;
	}

	if ( it->second.userInfo.nStatus == 1)
	{
		int nRet = MessageBox(lpszModifyAccountStatusWarning, lpszPrompt, MB_ICONQUESTION|MB_YESNO);
		if ( nRet != IDYES )
		{
			return;
		}
	}

	m_curUserInfo = it->second.userInfo;
	if( m_curUserInfo.nStatus == 0)
	{
		m_curUserInfo.nStatus = 1;
	}
	else
	{
		m_curUserInfo.nStatus = 0;
	}

	if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_ModifyAccount_Req,
		&m_curUserInfo, sizeof(m_curUserInfo)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
}

void CMainFrame::ModifyAccount( int nAccountID )
{
	AccountType eType = ((CRiskManageView*)GetActiveView())->GetAccountType();
	if ( eType == ACCOUNT_MANAGE )
	{
		std::map<int, RiskMgmtUserInfoEx>::iterator it = m_mapUserInfoEx.find(nAccountID);
		if ( it == m_mapUserInfoEx.end())
		{
			return;
		}

		ModifyRiskUser(it->second.userInfo);
	}
	else if ( eType == ACCOUNT_TRUST_TRADE)
	{
		std::map<int, TrustTradeAccountEx>::iterator it = m_mapTrustTradeAccountEx.find(nAccountID);
		if ( it == m_mapTrustTradeAccountEx.end())
		{
			return;
		}

		ModifyTrustTradeAccount(it->second.trustAccount);
	}
	else
	{
		std::map<int, TraderInfoEx>::iterator it = m_mapTraderInfoEx.find(nAccountID);
		if ( it == m_mapTraderInfoEx.end())
		{
			return;
		}

		ModifyTraderInfo(it->second.traderInfo);
	}
}

void CMainFrame::RefreshListData(bool bOrg, bool bFinProduct, 
								 bool bPermission, bool bIndModule )
{
	if ( bOrg || bFinProduct || bPermission )
	{
		std::map<int, RiskMgmtUserInfoEx>::iterator it_user = m_mapUserInfoEx.begin();
		for ( ; it_user != m_mapUserInfoEx.end(); it_user++ )
		{
			RiskMgmtUserInfoEx userInfoEx;
			GetUserInfoEx(it_user->second.userInfo, userInfoEx);
			it_user->second = userInfoEx;
		}

		((CRiskManageView*)GetActiveView())->ClearAllData();
		std::map<int, RiskMgmtUserInfoEx> mapShowDataUser;
		mapShowDataUser.clear();
		CalcShowData(m_nSelOrgID, mapShowDataUser);
		((CRiskManageView*)GetActiveView())->SetData(mapShowDataUser);
	}

	if ( bOrg || bFinProduct || bIndModule )
	{
		std::map<int, TrustTradeAccountEx>::iterator it_trustTrader = m_mapTrustTradeAccountEx.begin();
		for ( ; it_trustTrader != m_mapTrustTradeAccountEx.end(); it_trustTrader++ )
		{
			TrustTradeAccountEx tradeAccountEx;
			GetTrustTradeAccountEx(it_trustTrader->second.trustAccount, tradeAccountEx);
			it_trustTrader->second = tradeAccountEx;
		}

		std::map<int, TrustTradeAccountEx> mapShowDataTrustTrader;
		mapShowDataTrustTrader.clear();
		CalcShowData(m_nSelOrgID, mapShowDataTrustTrader);
		((CRiskManageView*)GetActiveView())->SetData(mapShowDataTrustTrader);
	}

	if ( bOrg || bFinProduct )
	{
		std::map<int, TraderInfoEx>::iterator it_trader = m_mapTraderInfoEx.begin();
		for ( ; it_trader != m_mapTraderInfoEx.end(); it_trader++ )
		{
			TraderInfoEx traderEx;
			GetTraderInfoEx(it_trader->second.traderInfo, traderEx);
			it_trader->second = traderEx;
		}

		std::map<int, TraderInfoEx> mapShowDataTrader;
		mapShowDataTrader.clear();
		CalcShowData(m_nSelOrgID, mapShowDataTrader);
		((CRiskManageView*)GetActiveView())->SetData(mapShowDataTrader);
	}
}

void CMainFrame::OnSpecialInd()
{
	// TODO: Add your command handler code here
	if(((CRiskManageView*)GetActiveView())->GetAccountType() != ACCOUNT_TRUST_TRADE)
	{
		MessageBox(_T("[特殊指标设置]功能只对委托交易账号有效！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	std::vector<int> vAccount;
	((CRiskManageView*)GetActiveView())->GetSelectAccountID(vAccount);
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

	CRiskIndSpecialSet dlg;
	dlg.SetTraderID(nAccountID);
	dlg.SetIndicator(m_vIndicator);
	dlg.DoModal();
}

void CMainFrame::OnUpdateSpecialInd(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bModifyPermission);
}

bool CMainFrame::IsIndicatorModuleChanged( const std::map<int, RiskIndicatorModule> &mapIndModule )
{
	std::map<int, RiskIndicatorModule>::iterator it_old = m_mapIndModule.begin();
	bool bChanged = false;
	for ( ; it_old != m_mapIndModule.end(); ++it_old )
	{
		std::map<int, RiskIndicatorModule>::const_iterator it_new = mapIndModule.find(it_old->first);
		if ( it_new == mapIndModule.end() )
		{
			bChanged = true;
			break;
		}
		else
		{
			if ( 0 != strcmp(it_old->second.szName, it_new->second.szName))
			{
				bChanged = true;
				break;
			}
		}
	}

	return bChanged;
}

bool CMainFrame::NeedRefreshOrg( std::map<int, AssetMgmtOrganization> &mapAssetOrg )
{
	bool bNeedRefresh = false;
	std::map<int, AssetMgmtOrganization>::iterator it_old = m_mapAssetMgmtOrg.begin();
	for ( ; it_old != m_mapAssetMgmtOrg.end(); it_old++ )
	{
		//检查本节点是否改变
		std::map<int, AssetMgmtOrganization>::iterator it_new = mapAssetOrg.find(it_old->first);
		if ( it_new == mapAssetOrg.end() )
		{
			bNeedRefresh = true;
			break;
		}
		else
		{
			if ( 0 != strcmp(it_old->second.szName, it_new->second.szName))
			{
				bNeedRefresh = true;
				break;
			}
		}
	}

	if ( bNeedRefresh )
	{
		return bNeedRefresh;
	}

	//检查是否在当前组织结构下新增了节点(新增节点ID肯定比当前节点最大ID还大，所以从当前节点最大ID起查找)
	std::map<int, AssetMgmtOrganization>::reverse_iterator rit_old = m_mapAssetMgmtOrg.rbegin();
	std::map<int, AssetMgmtOrganization>::iterator it_add = mapAssetOrg.find(rit_old->first);
	if ( it_add != mapAssetOrg.end())
	{
		it_add++;
		for ( ; it_add != mapAssetOrg.end(); ++it_add)
		{
			std::map<int, AssetMgmtOrganization>::iterator it_find = 
				m_mapAssetMgmtOrg.find(it_add->second.nUpperLevelOrgID);
			if ( it_find != m_mapAssetMgmtOrg.end())
			{
				bNeedRefresh = true;
				break;
			}
		}
	}

	return bNeedRefresh;
}

void CMainFrame::OnFundnetParam()
{
	// TODO: Add your command handler code here
	if(((CRiskManageView*)GetActiveView())->GetAccountType() != ACCOUNT_TRUST_TRADE)
	{
		MessageBox(_T("[基金净值设置]功能只对委托交易账号有效！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	std::vector<int> vAccount;
	((CRiskManageView*)GetActiveView())->GetSelectAccountID(vAccount);
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

	CFundNetParamDlg dlg;
	dlg.SetTradeAccountID(nAccountID);
	dlg.DoModal();
}

void CMainFrame::OnUpdateFundnetParam(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnUpdateForceCloseParam(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnForceCloseParam()
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	if(((CRiskManageView*)GetActiveView())->GetAccountType() != ACCOUNT_TRUST_TRADE)
	{
		MessageBox(_T("[自动强平设置]功能只对委托交易账号有效！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	std::vector<int> vAccount;
	((CRiskManageView*)GetActiveView())->GetSelectAccountID(vAccount);
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

	CAutoForceCloseParamDlg dlg;
	dlg.SetTradeAccountID(nAccountID);
	dlg.DoModal();
}
