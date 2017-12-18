// PermissionSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "PermissionSetDlg.h"
#include "TCPDataService.h"
#include "ConfigMgr.h"

// CPermissionSetDlg dialog

IMPLEMENT_DYNAMIC(CPermissionSetDlg, CDialog)

CPermissionSetDlg::CPermissionSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPermissionSetDlg::IDD, pParent)
{
	memset(&m_curPermissionName, 0, sizeof(m_curPermissionName));
	m_nCurSelPermissionID = -1;
	m_nCbxBakSel = 0;

	m_bAddModifyPermission = true;
	m_bDelPermission = true;
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

	m_cbxPermissionType.AddString("管理员");
	m_cbxPermissionType.AddString("风控员");
	m_cbxPermissionType.SetCurSel(0);

	LONG styles = ::GetWindowLong( m_treePermission.m_hWnd, GWL_STYLE );
	styles = styles | TVS_CHECKBOXES | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS  ;
	::SetWindowLong( m_treePermission.m_hWnd, GWL_STYLE, styles );

	m_imgState.Create(IDB_BITMAP_STATUS,13, 1, RGB(255,255,255));
	m_treePermission.SetImageList(&m_imgState,TVSIL_STATE);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	EnableControls(FALSE);

	int nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_QryPermissionNode_Req, NULL, 0)
		|| (!CTCPDataService::GetInstance()->SendData(Cmd_RM_QryPermissionAction_Req, NULL, 0))
		|| (!CTCPDataService::GetInstance()->SendData(Cmd_RM_QryPermissionName_Req, NULL, 0))
		|| (!CTCPDataService::GetInstance()->SendData(Cmd_RM_QryPermissionValue_Req, NULL, 0)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
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
	if ( pRecvData->head.cmdid == Cmd_RM_QryPermissionNode_Rsp )
	{
		m_mapNode.clear();
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(PermissionNode);
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionNode* pNode = (PermissionNode*)((char*)pRecvData->pData + i*sizeof(PermissionNode));
				m_mapNode[pNode->nNodeID] = *pNode;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryPermissionAction_Rsp )
	{
		m_mapAction.clear();
		m_treePermission.ClearItems();
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(PermissionAction);
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionAction* pAction = (PermissionAction*)((char*)pRecvData->pData + i*sizeof(PermissionAction));
				m_mapAction[pAction->nActionID] = *pAction;
			}

			PermissionType eType = static_cast<PermissionType>(m_cbxPermissionType.GetCurSel());
			m_treePermission.SetPermissionModule(m_mapNode, m_mapAction);
			m_treePermission.ShowPermissionModule(eType);

			m_treePermission.Expand(m_treePermission.GetRootItem(), TVE_EXPAND);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryPermissionName_Rsp )
	{
		EnableControls(TRUE);
		m_mapPermissionName.clear();
		m_listPermissionName.DeleteAllItems();
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(PermissionName);
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionName* pPermissionName = (PermissionName*)((char*)pRecvData->pData + i*sizeof(PermissionName));
				m_mapPermissionName[pPermissionName->nPermissionID] = *pPermissionName;

				int nTotalCount = m_listPermissionName.GetItemCount();
				m_listPermissionName.InsertItem(nTotalCount, "");
				m_listPermissionName.SetItemText(nTotalCount, 0, pPermissionName->szName);
				m_listPermissionName.SetItemData(nTotalCount, pPermissionName->nPermissionID);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryPermissionValue_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(PermissionValue);
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionValue* pPermissionValue = (PermissionValue*)((char*)pRecvData->pData + i*sizeof(PermissionValue));

				std::map<int, std::map<int, PermissionValue>>::iterator it_permisson
					= m_mapAllPermissionValue.find(pPermissionValue->nPermissionID);
				if ( it_permisson != m_mapAllPermissionValue.end() )
				{
					(it_permisson->second)[pPermissionValue->nActionID] = *pPermissionValue;
				}
				else
				{
					std::map<int, PermissionValue> mapPermissionValue;
					mapPermissionValue[pPermissionValue->nActionID] = *pPermissionValue;
					m_mapAllPermissionValue[pPermissionValue->nPermissionID] = mapPermissionValue;
				}
			}

			//权限列表默认选中第一行
			if ( m_listPermissionName.GetItemCount() > 0)
			{
				m_listPermissionName.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_ModifyPermissionName_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nPermissionID = *(int*)pRecvData->pData;
			if ( nPermissionID == m_curPermissionName.nPermissionID )
			{
				m_mapPermissionName[nPermissionID] = m_curPermissionName;
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
				if ( m_listPermissionName.GetItemData(i) == m_curPermissionName.nPermissionID )
				{
					m_listPermissionName.SetFocus();
					m_listPermissionName.EditLabel(i);
					break;
				}
			}
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_AddPermissionName_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curPermissionName.nPermissionID = *(int*)pRecvData->pData;
			m_mapPermissionName[m_curPermissionName.nPermissionID] = m_curPermissionName;

			int nCount = m_listPermissionName.GetItemCount();
			if( m_listPermissionName.GetItemData(nCount-1) <= 0)
			{
				m_listPermissionName.SetItemData(nCount-1, m_curPermissionName.nPermissionID);
				m_listPermissionName.SetItemState(nCount-1, LVIS_SELECTED, LVIS_SELECTED);
				m_nCurSelPermissionID = m_curPermissionName.nPermissionID;
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
	else if ( pRecvData->head.cmdid == Cmd_RM_DelPermissionName_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nPermissionID = *(int*)pRecvData->pData;
			std::map<int, PermissionName>::iterator it = m_mapPermissionName.find(nPermissionID);
			if ( it != m_mapPermissionName.end())
			{
				m_mapPermissionName.erase(it);
			}

			std::map<int, std::map<int, PermissionValue>>::iterator it_value
				= m_mapAllPermissionValue.find(nPermissionID);
			if ( it_value != m_mapAllPermissionValue.end() )
			{
				m_mapAllPermissionValue.erase(it_value);
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
	else if ( pRecvData->head.cmdid == Cmd_RM_SavePermissionValue_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nPermissionID = *(int*)pRecvData->pData;
			m_mapAllPermissionValue[nPermissionID] = m_curPermissionValue;
			MessageBox(_T("权限保存成功！"), lpszPrompt, MB_ICONINFORMATION);
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

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_DelPermissionName_Req,
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
		std::map<int, PermissionName>::iterator it = m_mapPermissionName.find(nPermissionID);
		if ( it != m_mapPermissionName.end())
		{
			if ( csValue == it->second.szName )
			{
				//没有改动，直接返回
				return;
			}

			memset(&m_curPermissionName, 0, sizeof(m_curPermissionName));
			m_curPermissionName = it->second;
			strcpy(m_curPermissionName.szName, csValue.GetBuffer(0));
			csValue.ReleaseBuffer();
			if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_ModifyPermissionName_Req,
				&m_curPermissionName, sizeof(m_curPermissionName)))
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
		memset(&m_curPermissionName, 0, sizeof(m_curPermissionName));
		strcpy(m_curPermissionName.szName, csValue.GetBuffer(0));
		csValue.ReleaseBuffer();
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_AddPermissionName_Req,
			&m_curPermissionName, sizeof(m_curPermissionName)))
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
		std::map<int, std::map<int, PermissionValue>>::iterator it_permisson
			= m_mapAllPermissionValue.find(m_nCurSelPermissionID);
		if ( it_permisson != m_mapAllPermissionValue.end())
		{
			std::map<int, PermissionValue> mapPermissionValue = it_permisson->second;
			if ( mapPermissionValue.empty())
			{
				return;
			}

			//查找动作对应的节点，确定权限类型
			std::map<int, PermissionValue>::iterator it_action = mapPermissionValue.begin();
			std::map<int, PermissionAction>::iterator it_findAction = m_mapAction.find(it_action->second.nActionID);
			if ( it_findAction == m_mapAction.end() )
			{
				return;
			}

			std::map<int, PermissionNode>::iterator it_node = m_mapNode.find(it_findAction->second.nNodeID);
			if ( it_node == m_mapNode.end() )
			{
				return;
			}

			m_cbxPermissionType.SetCurSel(it_node->second.nNodeType);
			m_nCbxBakSel = it_node->second.nNodeType;
			PermissionType eType = static_cast<PermissionType>(it_node->second.nNodeType);
			m_treePermission.ShowPermissionModule(eType);
			m_treePermission.Expand(m_treePermission.GetRootItem(), TVE_EXPAND);

			m_treePermission.SetPermissionData(mapPermissionValue);
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
	m_treePermission.ResetTreeData();
}

void CPermissionSetDlg::OnBnClickedButtonCommit()
{
	// TODO: Add your control notification handler code here
	if ( m_nCurSelPermissionID <= 0)
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	m_curPermissionValue.clear();

	std::vector<PermissionValue> vData;
	vData.clear();
	m_treePermission.GetPermissionData(vData);
	for ( UINT i = 0; i < vData.size(); i++ )
	{
		PermissionValue perValue = vData[i];
		m_curPermissionValue[perValue.nActionID] = perValue;
	}

	int nDataLen = sizeof(int)+sizeof(PermissionValue)*vData.size();
	char* pBuffer = new char[nDataLen];
	memset(pBuffer, 0, nDataLen);
	memcpy(pBuffer, &m_nCurSelPermissionID, sizeof(int));
	if ( !vData.empty() )
	{
		memcpy(pBuffer+sizeof(int), &vData[0], sizeof(PermissionValue)*vData.size());
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_SavePermissionValue_Req,
		pBuffer, nDataLen))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableControls(FALSE);
	}

	delete [] pBuffer;
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
	std::map<int, std::map<int, PermissionValue>>::iterator it = m_mapAllPermissionValue.find(m_nCurSelPermissionID);
	if ( it != m_mapAllPermissionValue.end())
	{
		int nRet = MessageBox(lpszChangePermissionTypeWarning, lpszPrompt, MB_ICONQUESTION|MB_YESNO);
		if ( nRet == IDYES )
		{
			m_treePermission.ShowPermissionModule(eType);
			m_treePermission.Expand(m_treePermission.GetRootItem(), TVE_EXPAND);
			m_treePermission.SetPermissionData(it->second);
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

std::map<int, PermissionName>& CPermissionSetDlg::GetPermissionNames()
{
	return m_mapPermissionName;
}
