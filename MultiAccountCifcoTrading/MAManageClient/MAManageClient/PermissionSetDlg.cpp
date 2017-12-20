// PermissionSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "PermissionSetDlg.h"
#include "TCPDataService.h"
#include "ConfigMgr.h"

// CPermissionSetDlg dialog

IMPLEMENT_DYNAMIC(CPermissionSetDlg, CDialog)

CPermissionSetDlg::CPermissionSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPermissionSetDlg::IDD, pParent)
{
	m_mapModules.clear();
	m_mapPermission.clear();
	memset(&m_curPermission, 0, sizeof(m_curPermission));
	m_nCurSelPermissionID = -1;
	m_nCbxBakSel = 0;

	m_bAddModifyPermission = false;
	m_bDelPermission = false;
}

CPermissionSetDlg::~CPermissionSetDlg()
{
}

void CPermissionSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PERMISSION, m_treePermission);
	DDX_Control(pDX, IDC_COMBO_PERMISSION_TYPE, m_cbxPermissionType);
	DDX_Control(pDX, IDC_LIST_PERMISSION_NAME, m_listPermissionName);
	DDX_Control(pDX, IDC_BUTTON_ADD_NAME, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DEL_NAME, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_COMMIT, m_btnCommit);
}


BEGIN_MESSAGE_MAP(CPermissionSetDlg, CDialog)
	ON_MESSAGE(RECV_DATA_MSG, &CPermissionSetDlg::OnRecvMessage)
	ON_BN_CLICKED(IDC_BUTTON_ADD_NAME, &CPermissionSetDlg::OnBnClickedButtonAddName)
	ON_BN_CLICKED(IDC_BUTTON_DEL_NAME, &CPermissionSetDlg::OnBnClickedButtonDelName)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_PERMISSION_NAME, &CPermissionSetDlg::OnLvnEndlabeleditListPermissionName)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PERMISSION_NAME, &CPermissionSetDlg::OnLvnItemchangedListPermissionName)
	ON_BN_CLICKED(IDC_BUTTON_COMMIT, &CPermissionSetDlg::OnBnClickedButtonCommit)
	ON_CBN_SELCHANGE(IDC_COMBO_PERMISSION_TYPE, &CPermissionSetDlg::OnCbnSelchangeComboPermissionType)
END_MESSAGE_MAP()

BOOL CPermissionSetDlg::PreTranslateMessage( MSG* pMsg )
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_ESCAPE == pMsg->wParam)
			return TRUE;
	}

	return CDialog::PreTranslateMessage( pMsg );
}

BOOL CPermissionSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_listPermissionName.GetExtendedStyle();	
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |=  LVS_EX_GRIDLINES;
	m_listPermissionName.SetExtendedStyle(dwStyle);
	m_listPermissionName.InsertColumn(0, "", LVCFMT_LEFT, 180);

	m_cbxPermissionType.AddString("后台管理");
	m_cbxPermissionType.AddString("前台管理");
	m_cbxPermissionType.SetCurSel(0);

	LONG styles = ::GetWindowLong( m_treePermission.m_hWnd, GWL_STYLE );
	styles = styles | TVS_CHECKBOXES | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS  ;
	::SetWindowLong( m_treePermission.m_hWnd, GWL_STYLE, styles );

	m_imgState.Create(IDB_BITMAP_STATUS,13, 1, RGB(255,255,255));
	m_treePermission.SetImageList(&m_imgState,TVSIL_STATE);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	EnableControls(FALSE);

	int nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_QryPermissionModule_Req, NULL, 0)
		|| (!CTCPDataService::GetInstance()->SendData(Cmd_MA_QryPermissionByOwner_Req, 
		&nOwnerAccountID, sizeof(nOwnerAccountID))))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableControls(FALSE);
	}

	return TRUE;
}

LRESULT CPermissionSetDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_UnLogon_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
		}
		else
		{
			//CString csMsg = (char*)(pRecvData->pData);
			//MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryPermissionModule_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapModules.clear();
			m_treePermission.ClearItems();

			int nCount = pRecvData->head.len / sizeof(PermissionModule);
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionModule* pModule = (PermissionModule*)((char*)pRecvData->pData + i*sizeof(PermissionModule));
				m_mapModules[pModule->nModuleID] = *pModule;
			}

			PermissionType eType = static_cast<PermissionType>(m_cbxPermissionType.GetCurSel());
			m_treePermission.SetPermissionModule(m_mapModules);
			m_treePermission.ShowPermissionModule(eType);

			m_treePermission.Expand(m_treePermission.GetRootItem(), TVE_EXPAND);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	if ( pRecvData->head.cmdid == Cmd_MA_QryPermissionByOwner_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			memset(&m_curPermission, 0, sizeof(m_curPermission));
			m_mapPermission.clear();
			m_listPermissionName.DeleteAllItems();
			int nCount = pRecvData->head.len / sizeof(Permission);
			for ( int i = 0; i < nCount; i++ )
			{
				Permission* pPermission = (Permission*)((char*)pRecvData->pData + i*sizeof(Permission));
				m_mapPermission[pPermission->nPermissionID] = *pPermission;

				int nTotalCount = m_listPermissionName.GetItemCount();
				m_listPermissionName.InsertItem(nTotalCount, "");
				m_listPermissionName.SetItemText(nTotalCount, 0, pPermission->szName);
				m_listPermissionName.SetItemData(nTotalCount, pPermission->nPermissionID);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	if ( pRecvData->head.cmdid == Cmd_MA_ModifyPermission_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nPermissionID = *(int*)pRecvData->pData;
			if ( nPermissionID == m_curPermission.nPermissionID )
			{
				m_mapPermission[nPermissionID] = m_curPermission;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);

			int nCount = m_listPermissionName.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( m_listPermissionName.GetItemData(i) == m_curPermission.nPermissionID )
				{
					m_listPermissionName.SetFocus();
					m_listPermissionName.EditLabel(i);
					break;
				}
			}
		}
	}
	if ( pRecvData->head.cmdid == Cmd_MA_AddPermission_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curPermission.nPermissionID = *(int*)pRecvData->pData;
			m_mapPermission[m_curPermission.nPermissionID] = m_curPermission;

			int nCount = m_listPermissionName.GetItemCount();
			if( m_listPermissionName.GetItemData(nCount-1) <= 0)
			{
				m_listPermissionName.SetItemData(nCount-1, m_curPermission.nPermissionID);
				m_listPermissionName.SetSelectionMark(nCount-1);
				m_nCurSelPermissionID = m_curPermission.nPermissionID;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);

			int nCount = m_listPermissionName.GetItemCount();
			if( m_listPermissionName.GetItemData(nCount-1) <= 0)
			{
				m_listPermissionName.SetFocus();
				m_listPermissionName.EditLabel(nCount-1);
			}
		}
	}
	if ( pRecvData->head.cmdid == Cmd_MA_DelPermission_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nPermissionID = *(int*)pRecvData->pData;
			std::map<int, Permission>::iterator it = m_mapPermission.find(nPermissionID);
			if ( it != m_mapPermission.end())
			{
				m_mapPermission.erase(it);
			}

			int nCount = m_listPermissionName.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( nPermissionID == m_listPermissionName.GetItemData(i))
				{
					m_listPermissionName.DeleteItem(i);
					break;
				}
			}

			if ( nPermissionID == m_nCurSelPermissionID )
			{
				ResetPermissionArea();
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}

	return 0;
}


// CPermissionSetDlg message handlers

void CPermissionSetDlg::OnBnClickedButtonAddName()
{
	// TODO: Add your control notification handler code here
	m_listPermissionName.SetFocus();
	int nCount = m_listPermissionName.GetItemCount();
	if ( nCount > 0 && m_listPermissionName.GetItemText(nCount-1, 0).IsEmpty())
	{
		m_listPermissionName.EditLabel(nCount-1);
		return;
	}
	else
	{
		m_listPermissionName.InsertItem(nCount, "");
		m_listPermissionName.EditLabel(nCount);
	}
}

void CPermissionSetDlg::OnBnClickedButtonDelName()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listPermissionName.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_listPermissionName.GetNextSelectedItem(pos);
	if ( IDYES != MessageBox(lpszDelDataComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	int nPermissionID = m_listPermissionName.GetItemData(nSel);
	if ( nPermissionID <= 0)
	{
		m_listPermissionName.DeleteItem(nSel);
		return;
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_DelPermission_Req,
		&nPermissionID, sizeof(nPermissionID)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		return;
	}
	else
	{
		EnableControls(FALSE);
	}
}

void CPermissionSetDlg::OnLvnEndlabeleditListPermissionName(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CEdit* pEdit = m_listPermissionName.GetEditControl();
	CString csValue;
	pEdit->GetWindowText(csValue);
	if ( csValue.IsEmpty())
	{
		return;
	}

	if ( csValue.GetLength() > MAX_PERMISSION_NAME_LENGTH )
	{
		MessageBox(_T("权限名过长，请重新输入！"), lpszPrompt, MB_ICONERROR);
		pEdit->SetWindowText(_T(""));
		m_listPermissionName.EditLabel(pDispInfo->item.iItem);
		return;
	}

	m_listPermissionName.SetItemText(pDispInfo->item.iItem, pDispInfo->item.iSubItem, csValue);
	m_listPermissionName.SetItemState(pDispInfo->item.iItem, LVIS_SELECTED, LVIS_SELECTED);
	int nPermissionID = m_listPermissionName.GetItemData(pDispInfo->item.iItem);
	if ( nPermissionID > 0 )
	{
		//修改
		std::map<int, Permission>::iterator it = m_mapPermission.find(nPermissionID);
		if ( it != m_mapPermission.end())
		{
			memset(&m_curPermission, 0, sizeof(m_curPermission));
			m_curPermission = it->second;
			strcpy(m_curPermission.szName, csValue.GetBuffer(0));
			csValue.ReleaseBuffer();
			if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyPermission_Req,
				&m_curPermission, sizeof(m_curPermission)))
			{
				std::string strError = CTCPDataService::GetInstance()->GetLastError();
				MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
				return;
			}
			else
			{
				EnableControls(FALSE);
			}
		}
	}
	else
	{
		//新增
		memset(&m_curPermission, 0, sizeof(m_curPermission));
		m_curPermission.nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();
		strcpy(m_curPermission.szName, csValue.GetBuffer(0));
		csValue.ReleaseBuffer();
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_AddPermission_Req,
			&m_curPermission, sizeof(m_curPermission)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
			return;
		}
		else
		{
			EnableControls(FALSE);
		}
	}
}

void CPermissionSetDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CPermissionSetDlg::OnLvnItemchangedListPermissionName(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		ResetPermissionArea();

		m_nCurSelPermissionID = m_listPermissionName.GetItemData(pNMLV->iItem);
		std::map<int, Permission>::iterator it = m_mapPermission.find(m_nCurSelPermissionID);
		if ( it != m_mapPermission.end())
		{
			if ( it->second.nPermissionType != m_treePermission.GetCurShowType())
			{
				m_cbxPermissionType.SetCurSel(it->second.nPermissionType);
				m_nCbxBakSel = it->second.nPermissionType;
				m_treePermission.ShowPermissionModule(static_cast<PermissionType>(it->second.nPermissionType));
				m_treePermission.Expand(m_treePermission.GetRootItem(), TVE_EXPAND);
			}

			ShowPermissionData( it->second.permissionData, it->second.nPermissionDataSize);
		}
	}
}

void CPermissionSetDlg::EnableControls(BOOL bFlag)
{
	m_btnAdd.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_btnDel.EnableWindow(bFlag&&m_bDelPermission);
	m_btnCommit.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_listPermissionName.EnableWindow(bFlag);
}

void CPermissionSetDlg::ResetPermissionArea()
{
	int nState = 1;
	m_treePermission.SetItemState(m_treePermission.GetRootItem(), 
		INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
}

void CPermissionSetDlg::OnBnClickedButtonCommit()
{
	// TODO: Add your control notification handler code here
	if ( m_nCurSelPermissionID <= 0)
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	memset(&m_curPermission, 0, sizeof(m_curPermission));
	std::map<int, Permission>::iterator it = m_mapPermission.find(m_nCurSelPermissionID);
	if ( it == m_mapPermission.end())
	{
		return;
	}

	m_curPermission = it->second;
	m_curPermission.nPermissionType = m_cbxPermissionType.GetCurSel();
	std::vector<PermissionData> vData;
	vData.clear();
	m_treePermission.GetPermissionData(vData);
	if ( !vData.empty())
	{
		m_curPermission.nPermissionDataSize = vData.size()*sizeof(PermissionData);
		memcpy(m_curPermission.permissionData, &vData[0], m_curPermission.nPermissionDataSize);
	}
	else
	{
		m_curPermission.nPermissionDataSize = 0;
		memset(m_curPermission.permissionData, 0, sizeof(m_curPermission.permissionData));
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyPermission_Req,
		&m_curPermission, sizeof(m_curPermission)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		return;
	}
	else
	{
		EnableControls(FALSE);
	}
}

void CPermissionSetDlg::OnCbnSelchangeComboPermissionType()
{
	// TODO: Add your control notification handler code here
	if ( m_nCbxBakSel == m_cbxPermissionType.GetCurSel())
	{
		return;
	}

	//判断是否已经存在权限数据
	PermissionType eType = static_cast<PermissionType>(m_cbxPermissionType.GetCurSel());
	std::map<int, Permission>::iterator it = m_mapPermission.find(m_nCurSelPermissionID);
	if ( it != m_mapPermission.end())
	{
		int nRet = MessageBox(lpszChangePermissionTypeWarning, lpszPrompt, MB_ICONQUESTION|MB_YESNO);
		if ( nRet == IDYES )
		{
			m_treePermission.ShowPermissionModule(eType);
			m_treePermission.Expand(m_treePermission.GetRootItem(), TVE_EXPAND);
			if ( eType == it->second.nPermissionType )
			{
				ShowPermissionData( it->second.permissionData, it->second.nPermissionDataSize);
			}
		}
		else
		{
			m_cbxPermissionType.SetCurSel(m_nCbxBakSel);
		}
	}
	else
	{
		m_treePermission.ShowPermissionModule(eType);
		m_treePermission.Expand(m_treePermission.GetRootItem(), TVE_EXPAND);
	}

	m_nCbxBakSel = m_cbxPermissionType.GetCurSel();
}

void CPermissionSetDlg::ShowPermissionData( const char* pData, int nDataLen )
{
	if ( pData == NULL || nDataLen == 0 )
	{
		return;
	}

	std::map<int, PermissionData> mapData;
	mapData.clear();

	int nCount = nDataLen / sizeof(PermissionData);
	if ( nCount == 0 )
	{
		return;
	}

	for ( int i = 0; i < nCount; i++ )
	{
		PermissionData* pPermissionData = (PermissionData*)(pData + i*sizeof(PermissionData));
		mapData[pPermissionData->nModuleID] = *pPermissionData;
	}

	if ( !mapData.empty())
	{
		m_treePermission.SetPermissionData(mapData);
	}
}

void CPermissionSetDlg::SetPermissionData( const PermissionData& data )
{
	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_ADD_MODIFY )
	{
		m_bAddModifyPermission = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_DEL )
	{
		m_bDelPermission = true;
	}

	//m_bAddModifyPermission = true;
	//m_bDelPermission = true;
}

CString CPermissionSetDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-02292")))
	{
		csTempMsg = _T("该权限正在被使用，无法删除！请先删除对应帐号信息。");
	}
	else if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("权限名重复！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}

void CPermissionSetDlg::OnClose()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnCancel();
}

std::map<int, Permission>& CPermissionSetDlg::GetPermissions()
{
	return m_mapPermission;
}
