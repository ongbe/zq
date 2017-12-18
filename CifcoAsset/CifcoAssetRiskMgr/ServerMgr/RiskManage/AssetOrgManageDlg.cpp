// AssetOrgManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "AssetOrgManageDlg.h"
#include "TCPDataService.h"


// CAssetOrgManageDlg dialog

IMPLEMENT_DYNAMIC(CAssetOrgManageDlg, CDialog)

CAssetOrgManageDlg::CAssetOrgManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssetOrgManageDlg::IDD, pParent)
	, m_csOrgName(_T(""))
	, m_csLongitude(_T(""))
	, m_csLatitude(_T(""))
	, m_hSelItem(NULL)
	, m_nSequence(0)
{
	memset(&m_curOrg, 0, sizeof(m_curOrg));
}

CAssetOrgManageDlg::~CAssetOrgManageDlg()
{
}

void CAssetOrgManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ORG_NAME, m_edtOrgName);
	DDX_Control(pDX, IDC_EDIT_LONGITUDE, m_edtLongitude);
	DDX_Control(pDX, IDC_EDIT_LATITUDE, m_edtLatitude);
	DDX_Text(pDX, IDC_EDIT_ORG_NAME, m_csOrgName);
	DDX_Text(pDX, IDC_EDIT_LONGITUDE, m_csLongitude);
	DDX_Text(pDX, IDC_EDIT_LATITUDE, m_csLatitude);
	DDX_Control(pDX, IDC_TREE_ORG, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CAssetOrgManageDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAssetOrgManageDlg::OnBnClickedOk)
	ON_MESSAGE(RECV_DATA_MSG, &CAssetOrgManageDlg::OnRecvMessage)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ORG, &CAssetOrgManageDlg::OnTvnSelchangedTreeOrg)
	ON_BN_CLICKED(ID_BTN_ADD_ORG, &CAssetOrgManageDlg::OnBnClickedBtnAddOrg)
	ON_BN_CLICKED(ID_BTN_SAVE, &CAssetOrgManageDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(ID_BTN_DEL_ORG, &CAssetOrgManageDlg::OnBnClickedBtnDelOrg)
END_MESSAGE_MAP()


// CAssetOrgManageDlg message handlers

void CAssetOrgManageDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

BOOL CAssetOrgManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtOrgName.SetLimitText(MAX_USER_NAME_LENGTH);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	//查询所有资管组织结构
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryAssetMgmtOrg_Req, NULL, 0);

	return TRUE;
}

void CAssetOrgManageDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

LRESULT CAssetOrgManageDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_QryAssetMgmtOrg_Rsp )
	{
		EnableControls(TRUE);
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

			m_treeCtrl.DeleteAllItems();
			ShowTreeItems();
			m_treeCtrl.Expand(m_treeCtrl.GetRootItem(), TVE_EXPAND);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_AddAssetMgmtOrg_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curOrg.nAssetMgmtOrgID = *(int*)pRecvData->pData;
			//修改内存中的数据
			m_mapAssetMgmtOrg[m_curOrg.nAssetMgmtOrgID] = m_curOrg;

			std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it_tree
				= m_mapAssetMgmtOrgTree.find(m_curOrg.nUpperLevelOrgID);
			if ( it_tree != m_mapAssetMgmtOrgTree.end())
			{
				(it_tree->second)[m_curOrg.nAssetMgmtOrgID] = m_curOrg;
			}
			else
			{
				std::map<int, AssetMgmtOrganization> mapTemp;
				mapTemp[m_curOrg.nAssetMgmtOrgID] = m_curOrg;
				m_mapAssetMgmtOrgTree[m_curOrg.nUpperLevelOrgID] = mapTemp;
			}

			if ( NULL != m_hSelItem )
			{
				m_treeCtrl.SetItemText(m_hSelItem, m_curOrg.szName);
				m_treeCtrl.SetItemData(m_hSelItem, m_curOrg.nAssetMgmtOrgID);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_ModifyAssetMgmtOrg_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nAssetOrgID = *(int*)pRecvData->pData;
			//修改内存中的数据
			std::map<int, AssetMgmtOrganization>::iterator it = m_mapAssetMgmtOrg.find(nAssetOrgID);
			if ( it != m_mapAssetMgmtOrg.end())
			{
				int nUpperLeverOrgID = it->second.nUpperLevelOrgID;
				it->second.dLatitude = m_curOrg.dLatitude;
				it->second.dLongitude = m_curOrg.dLongitude;
				strcpy(it->second.szName, m_curOrg.szName);

				std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it_tree
					= m_mapAssetMgmtOrgTree.find(nUpperLeverOrgID);
				if ( it_tree != m_mapAssetMgmtOrgTree.end() )
				{
					std::map<int, AssetMgmtOrganization>::iterator it_org
						= it_tree->second.find(nAssetOrgID);
					if ( it_org != it_tree->second.end() )
					{
						it_org->second.dLatitude = m_curOrg.dLatitude;
						it_org->second.dLongitude = m_curOrg.dLongitude;
						strcpy(it_org->second.szName, m_curOrg.szName);
					}
				}
			}

			if ( NULL != m_hSelItem )
			{
				m_treeCtrl.SetItemText(m_hSelItem, m_curOrg.szName);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_DelAssetMgmtOrg_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nAssetOrgID = *(int*)pRecvData->pData;
			//删除内存中的数据
			std::map<int, AssetMgmtOrganization>::iterator it = m_mapAssetMgmtOrg.find(nAssetOrgID);
			if ( it != m_mapAssetMgmtOrg.end())
			{
				int nUpperLeverOrgID = it->second.nUpperLevelOrgID;
				m_mapAssetMgmtOrg.erase(it);

				std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it_tree
					= m_mapAssetMgmtOrgTree.find(nUpperLeverOrgID);
				if ( it_tree != m_mapAssetMgmtOrgTree.end() )
				{
					std::map<int, AssetMgmtOrganization>::iterator it_org
						= it_tree->second.find(nAssetOrgID);
					if ( it_org != it_tree->second.end() )
					{
						it_tree->second.erase(it_org);
					}

					if ( it_tree->second.empty())
					{
						m_mapAssetMgmtOrgTree.erase(it_tree);
					}
				}
			}

			if ( NULL != m_hSelItem )
			{
				m_treeCtrl.DeleteItem(m_hSelItem);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else
	{

	}

	return 0;
}

void CAssetOrgManageDlg::ShowTreeItems()
{
	std::map<int, AssetMgmtOrganization>::iterator it_org = m_mapAssetMgmtOrg.begin();
	if ( it_org == m_mapAssetMgmtOrg.end() )
	{
		return;
	}
	else
	{
		HTREEITEM hRoot = NULL;
		hRoot = m_treeCtrl.InsertItem(it_org->second.szName);

		m_treeCtrl.SetItemData(hRoot, it_org->second.nAssetMgmtOrgID);
		AddOrgItems(it_org->second.nAssetMgmtOrgID, hRoot);
	}
}

void CAssetOrgManageDlg::AddOrgItems( int nUpperLevelOrgID, HTREEITEM hParent )
{
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
			HTREEITEM hItem = m_treeCtrl.InsertItem(it_org->second.szName, hParent);
			m_treeCtrl.SetItemData(hItem, it_org->second.nAssetMgmtOrgID);
			AddOrgItems(it_org->second.nAssetMgmtOrgID, hItem);
		}
	}
}

void CAssetOrgManageDlg::OnTvnSelchangedTreeOrg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	if( pNMTreeView->itemNew.state & TVIS_SELECTED )
	{
		m_hSelItem = pNMTreeView->itemNew.hItem;
		int nSelOrgID = m_treeCtrl.GetItemData(pNMTreeView->itemNew.hItem);
		std::map<int, AssetMgmtOrganization>::iterator it = m_mapAssetMgmtOrg.find(nSelOrgID);
		if ( it != m_mapAssetMgmtOrg.end())
		{
			m_edtOrgName.SetWindowText(it->second.szName);
			CString csTemp;
			csTemp.Format(_T("%.6f"), it->second.dLongitude);
			m_edtLongitude.SetWindowText(csTemp);
			csTemp.Format(_T("%.6f"), it->second.dLatitude);
			m_edtLatitude.SetWindowText(csTemp);
		}
		else
		{
			m_edtOrgName.SetWindowText(_T(""));
			m_edtLongitude.SetWindowText(_T(""));
			m_edtLatitude.SetWindowText(_T(""));
		}
	}

	*pResult = 0;
}

void CAssetOrgManageDlg::OnBnClickedBtnAddOrg()
{
	// TODO: Add your control notification handler code here
	if ( NULL == m_hSelItem )
	{
		MessageBox(_T("请选择一个资管组织区域！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSelOrgID = m_treeCtrl.GetItemData(m_hSelItem);
	std::map<int, AssetMgmtOrganization>::iterator it = m_mapAssetMgmtOrg.find(nSelOrgID);
	if ( it == m_mapAssetMgmtOrg.end())
	{
		MessageBox(_T("该资管组织区域尚未保存，不能在该节点上创建子机构！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	m_nSequence++;
	CString csTemp;
	csTemp.Format(_T("新建机构%d"), m_nSequence);
	m_hSelItem = m_treeCtrl.InsertItem(csTemp, m_hSelItem);
	m_treeCtrl.SelectItem(m_hSelItem);

	m_edtOrgName.SetWindowText(csTemp);
	m_edtLongitude.SetWindowText(_T(""));
	m_edtLatitude.SetWindowText(_T(""));
}

void CAssetOrgManageDlg::OnBnClickedBtnSave()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if ( m_csOrgName.IsEmpty())
	{
		MessageBox(_T("请资管组织区域名称！"), lpszPrompt, MB_ICONERROR);
		m_edtOrgName.SetFocus();
		return;
	}

	if ( m_csLongitude.IsEmpty())
	{
		MessageBox(_T("请输入经度！"), lpszPrompt, MB_ICONERROR);
		m_edtLongitude.SetFocus();
		return;
	}

	if ( m_csLongitude.IsEmpty())
	{
		MessageBox(_T("请输入纬度！"), lpszPrompt, MB_ICONERROR);
		m_edtLatitude.SetFocus();
		return;
	}

	if ( m_hSelItem == NULL )
	{
		MessageBox(_T("请选择一个资管组织区域！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSelOrgID = m_treeCtrl.GetItemData(m_hSelItem);
	std::map<int, AssetMgmtOrganization>::iterator it = m_mapAssetMgmtOrg.find(nSelOrgID);
	if ( it != m_mapAssetMgmtOrg.end())
	{
		//修改
		m_curOrg = it->second;
		strcpy(m_curOrg.szName, m_csOrgName.GetBuffer(0));
		m_csOrgName.ReleaseBuffer();
		m_curOrg.dLatitude = atof(m_csLatitude.GetBuffer(0));
		m_csLatitude.ReleaseBuffer();
		m_curOrg.dLongitude = atof(m_csLongitude.GetBuffer(0));
		m_csLongitude.ReleaseBuffer();

		if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_ModifyAssetMgmtOrg_Req,
			&m_curOrg, sizeof(m_curOrg)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
		else
		{
			EnableControls(FALSE);
		}
	}
	else
	{
		//新增
		memset(&m_curOrg, 0, sizeof(m_curOrg));
		HTREEITEM hParent = m_treeCtrl.GetParentItem(m_hSelItem);
		if ( hParent == NULL )
		{
			return;
		}

		int nUpperLeverOrgID = m_treeCtrl.GetItemData(hParent);
		std::map<int, AssetMgmtOrganization>::iterator it_upper = m_mapAssetMgmtOrg.find(nUpperLeverOrgID);
		if ( it_upper == m_mapAssetMgmtOrg.end() )
		{
			return;
		}

		m_curOrg.nUpperLevelOrgID = it_upper->second.nAssetMgmtOrgID;
		m_curOrg.nScaleLevel = it_upper->second.nScaleLevel++;
		strcpy(m_curOrg.szName, m_csOrgName.GetBuffer(0));
		m_csOrgName.ReleaseBuffer();
		m_curOrg.dLatitude = atof(m_csLatitude.GetBuffer(0));
		m_csLatitude.ReleaseBuffer();
		m_curOrg.dLongitude = atof(m_csLongitude.GetBuffer(0));
		m_csLongitude.ReleaseBuffer();

		if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_AddAssetMgmtOrg_Req,
			&m_curOrg, sizeof(m_curOrg)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
		else
		{
			EnableControls(FALSE);
		}
	}
}

void CAssetOrgManageDlg::OnBnClickedBtnDelOrg()
{
	// TODO: Add your control notification handler code here
	if ( m_hSelItem == NULL )
	{
		MessageBox(_T("请选择一个资管组织区域！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSelOrgID = m_treeCtrl.GetItemData(m_hSelItem);
	std::map<int, AssetMgmtOrganization>::iterator it = m_mapAssetMgmtOrg.find(nSelOrgID);
	if ( it == m_mapAssetMgmtOrg.end())
	{
		//无效节点，直接删除
		m_treeCtrl.DeleteItem(m_hSelItem);
		return;
	}

	//有子机构的节点不允许直接删除
	HTREEITEM hItem = m_treeCtrl.GetChildItem(m_hSelItem);
	if ( hItem != NULL )
	{
		MessageBox(_T("请先删除下属资管组织区域！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	if (!CTCPDataService::GetInstance()->SendData(Cmd_RM_DelAssetMgmtOrg_Req,
		&nSelOrgID, sizeof(nSelOrgID)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableControls(FALSE);
	}
}

void CAssetOrgManageDlg::EnableControls( BOOL bFlag )
{
	GetDlgItem(ID_BTN_ADD_ORG)->EnableWindow(bFlag);
	GetDlgItem(ID_BTN_DEL_ORG)->EnableWindow(bFlag);
	GetDlgItem(ID_BTN_SAVE_ORG)->EnableWindow(bFlag);
}

std::map<int, AssetMgmtOrganization> CAssetOrgManageDlg::GetAssetMgmtOrg()
{
	return m_mapAssetMgmtOrg;
}

CString CAssetOrgManageDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-02292")))
	{
		csTempMsg = _T("该组织架构正在被使用，无法删除！");
	}
	else if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("组织架构名重复！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}
