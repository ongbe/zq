// FinancialProductDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "FinancialProductDlg.h"
#include "TCPDataService.h"
#include "CGridColumnTraitEdit.h"


// CFinancialProductDlg dialog

IMPLEMENT_DYNAMIC(CFinancialProductDlg, CDialog)

CFinancialProductDlg::CFinancialProductDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFinancialProductDlg::IDD, pParent)
{
	memset(&m_curFinProduct, 0, sizeof(m_curFinProduct));
	m_nCurSelProID = -1;
}

CFinancialProductDlg::~CFinancialProductDlg()
{
}

void CFinancialProductDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_listCtrl);
	DDX_Control(pDX, IDC_TREE_ORG, m_treeCtrl);
	DDX_Control(pDX, IDC_BUTTON_PRO_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_PRO_DEL, m_btnDel);
	DDX_Control(pDX, ID_FIN_PRO_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CFinancialProductDlg, CDialog)
	ON_MESSAGE(RECV_DATA_MSG, &CFinancialProductDlg::OnRecvMessage)
	ON_BN_CLICKED(IDC_BUTTON_PRO_ADD, &CFinancialProductDlg::OnBnClickedButtonProAdd)
	ON_BN_CLICKED(IDC_BUTTON_PRO_DEL, &CFinancialProductDlg::OnBnClickedButtonProDel)
	ON_BN_CLICKED(ID_FIN_PRO_SAVE, &CFinancialProductDlg::OnBnClickedFinProSave)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRODUCT, &CFinancialProductDlg::OnLvnItemchangedListProduct)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_PRODUCT, &CFinancialProductDlg::OnLvnEndlabeleditListProduct)
END_MESSAGE_MAP()


void CFinancialProductDlg::OnBnClickedButtonProAdd()
{
	// TODO: Add your control notification handler code here
	m_listCtrl.SetFocus();
	int nCount = m_listCtrl.GetItemCount();
	if ( nCount > 0 && m_listCtrl.GetItemText(nCount-1, 1).IsEmpty())
	{
		m_listCtrl.SelectRow(nCount-1, true);
		m_listCtrl.EditCell(nCount-1, 1);
		return;
	}
	else
	{
		m_listCtrl.InsertItem(nCount, "");
		m_listCtrl.SetItemText(nCount, 1, "");
		m_listCtrl.SetItemText(nCount, 2, "");
		m_listCtrl.SelectRow(nCount, true);
		m_listCtrl.EditCell(nCount, 1);
	}

	ResetTreeItems();
}

void CFinancialProductDlg::OnBnClickedButtonProDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_listCtrl.GetNextSelectedItem(pos);
	if ( IDYES != MessageBox(lpszDelDataComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	int nFinProductID = m_listCtrl.GetItemData(nSel);
	if ( nFinProductID <= 0)
	{
		m_listCtrl.DeleteItem(nSel);
		return;
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_DelFinancialProduct_Req,
		&nFinProductID, sizeof(nFinProductID)))
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

void CFinancialProductDlg::OnBnClickedFinProSave()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_listCtrl.GetNextSelectedItem(pos);
	m_nCurSelProID = m_listCtrl.GetItemData(nSel);
	if ( m_nCurSelProID <= 0 )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	m_curOrgSet.clear();
	GetTreeItemData(m_curOrgSet);
	if ( m_curOrgSet.empty())
	{
		MessageBox(_T("必须至少选择一个组织架构才能保存！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	std::vector<int> vOrg;
	std::set<int>::iterator it = m_curOrgSet.begin();
	for ( ; it != m_curOrgSet.end(); it++ )
	{
		vOrg.push_back(*it);
	}

	int nDataLen = sizeof(int)+sizeof(int)*vOrg.size();
	char* pBuffer = new char[nDataLen];
	memset(pBuffer, 0, nDataLen);
	memcpy(pBuffer, &m_nCurSelProID, sizeof(int));
	if ( !vOrg.empty() )
	{
		memcpy(pBuffer+sizeof(int), &vOrg[0], sizeof(int)*vOrg.size());
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_SaveAssetOrgProRelation_Req,
		pBuffer, nDataLen))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		return;
	}
	else
	{
		EnableControls(FALSE);
	}

	delete [] pBuffer;
	pBuffer = NULL;
}

BOOL CFinancialProductDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listCtrl.InsertHiddenLabelColumn();
	CGridColumnTraitEdit* pTrait = new CGridColumnTraitEdit;
	m_listCtrl.InsertColumnTrait(1, _T("理财产品"), LVCFMT_LEFT, 100, 0, pTrait);
	pTrait = new CGridColumnTraitEdit;
	m_listCtrl.InsertColumnTrait(2, _T("范围描述"), LVCFMT_LEFT, 170, 1, pTrait);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	//查询所有理财产品信息
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryFinancialProduct_Req, NULL, 0);
	//查询所有资管组织结构
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryAssetMgmtOrg_Req, NULL, 0);
	//查询资管组织与理财产品关系
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryAssetOrgProRelation_Req, NULL, 0);

	return TRUE;
}

void CFinancialProductDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

LRESULT CFinancialProductDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
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

			m_listCtrl.DeleteAllItems();
			std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.begin();
			for ( ; it != m_mapFinancialProduct.end(); ++it )
			{
				int nCount = m_listCtrl.GetItemCount();
				m_listCtrl.InsertItem(nCount, _T(""));
				m_listCtrl.SetItemText(nCount, 1, it->second.szName);
				m_listCtrl.SetItemText(nCount, 2, it->second.szScopeDesc);
				m_listCtrl.SetItemData(nCount, it->second.nFinancialProductID);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_ModifyFinancialProduct_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nFinProductID = *(int*)pRecvData->pData;
			if ( nFinProductID == m_curFinProduct.nFinancialProductID )
			{
				m_mapFinancialProduct[nFinProductID] = m_curFinProduct;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);

			int nCount = m_listCtrl.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( m_listCtrl.GetItemData(i) == m_curFinProduct.nFinancialProductID )
				{
					m_listCtrl.SetFocus();
					break;
				}
			}
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_AddFinancialProduct_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curFinProduct.nFinancialProductID = *(int*)pRecvData->pData;
			m_mapFinancialProduct[m_curFinProduct.nFinancialProductID] = m_curFinProduct;

			int nCount = m_listCtrl.GetItemCount();
			if( m_listCtrl.GetItemData(nCount-1) <= 0)
			{
				m_listCtrl.SetItemData(nCount-1, m_curFinProduct.nFinancialProductID);
				m_listCtrl.SelectRow(nCount-1, true);
				m_nCurSelProID = m_curFinProduct.nFinancialProductID;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);

			int nCount = m_listCtrl.GetItemCount();
			if( m_listCtrl.GetItemData(nCount-1) <= 0)
			{
				m_listCtrl.SetFocus();
			}
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_DelFinancialProduct_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nProductID = *(int*)pRecvData->pData;
			std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.find(nProductID);
			if ( it != m_mapFinancialProduct.end())
			{
				m_mapFinancialProduct.erase(it);
			}

			int nCount = m_listCtrl.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( nProductID == m_listCtrl.GetItemData(i))
				{
					m_listCtrl.DeleteItem(i);
					break;
				}
			}

			std::map<int, std::set<int>>::iterator it_fin = m_mapFinPro2Org.find(nProductID);
			if ( it_fin != m_mapFinPro2Org.end() )
			{
				m_mapFinPro2Org.erase(it_fin);
			}

			if ( nProductID == m_nCurSelProID )
			{
				m_nCurSelProID = -1;
				ResetTreeItems();
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_QryAssetOrgProRelation_Rsp )
	{
		EnableControls(TRUE);
		m_mapFinPro2Org.clear();
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(AssetOrgProRelation);
			for ( int i = 0; i < nCount; i++ )
			{
				AssetOrgProRelation* pRelation = (AssetOrgProRelation*)((char*)pRecvData->pData + i*sizeof(AssetOrgProRelation));

				std::map<int, std::set<int>>::iterator it_fin = m_mapFinPro2Org.find(pRelation->nFinancialProductID);
				if ( it_fin != m_mapFinPro2Org.end())
				{
					it_fin->second.insert(pRelation->nAssetMgmtOrgID);
				}
				else
				{
					std::set<int> setOrg;
					setOrg.insert(pRelation->nAssetMgmtOrgID);
					m_mapFinPro2Org[pRelation->nFinancialProductID] = setOrg;
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_SaveAssetOrgProRelation_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nFinID = *(int*)pRecvData->pData;
			m_mapFinPro2Org[nFinID] = m_curOrgSet;
			MessageBox(_T("保存成功！"), lpszPrompt, MB_ICONINFORMATION);
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

void CFinancialProductDlg::EnableControls( BOOL bFlag )
{
	m_btnAdd.EnableWindow(bFlag);
	m_btnDel.EnableWindow(bFlag);
	m_btnSave.EnableWindow(bFlag);
}

void CFinancialProductDlg::ShowTreeItems()
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

void CFinancialProductDlg::AddOrgItems( int nUpperLevelOrgID, HTREEITEM hParent )
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

void CFinancialProductDlg::OnLvnItemchangedListProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		ResetTreeItems();

		m_nCurSelProID = m_listCtrl.GetItemData(pNMLV->iItem);
		std::map<int, std::set<int>>::iterator it_fin
			= m_mapFinPro2Org.find(m_nCurSelProID);
		if ( it_fin != m_mapFinPro2Org.end())
		{
			std::set<int> setOrg = it_fin->second;
			if ( setOrg.empty())
			{
				return;
			}

			SetTreeItemData(it_fin->second);
		}
	}
}

void CFinancialProductDlg::OnLvnEndlabeleditListProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nRow = pDispInfo->item.iItem;
	int nCol = pDispInfo->item.iSubItem;
	if ( pDispInfo->item.pszText == NULL )
	{
		return;
	}

	CString csNewValue = pDispInfo->item.pszText;
	bool bModify = false;
	int nFinProductID = m_listCtrl.GetItemData(nRow);
	std::map<int, FinancialProduct>::iterator it = m_mapFinancialProduct.find(nFinProductID);
	if ( pDispInfo->item.iSubItem == 1 )
	{
		CString csOldText(_T(""));
		if ( nFinProductID > 0 )
		{
			if ( it != m_mapFinancialProduct.end())
			{
				csOldText = it->second.szName;
			}
		}
		//理财产品名称
		if ( csNewValue.IsEmpty())
		{
			MessageBox(_T("理财产品名称不能为空！"), lpszPrompt, MB_ICONERROR);
			m_listCtrl.SetItemText(nRow, nCol, csOldText);
			return;
		}

		if ( csNewValue.GetLength() > sizeof(OTHERNAMESTRING)-1)
		{
			MessageBox(_T("理财产品名称过长，请重新输入！"), lpszPrompt, MB_ICONERROR);
			m_listCtrl.SetItemText(nRow, nCol, csOldText);
			return;
		}

		if ( nFinProductID > 0 )
		{
			//修改
			if ( it != m_mapFinancialProduct.end())
			{
				if ( csNewValue == it->second.szName )
				{
					//没有改动，直接返回
					return;
				}
				else
				{
					bModify = true;
					m_curFinProduct = it->second;
					strcpy(m_curFinProduct.szName, csNewValue.GetBuffer(0));
					csNewValue.ReleaseBuffer();
				}
			}
		}
		else
		{
			//新增
			memset(&m_curFinProduct, 0, sizeof(m_curFinProduct));
			strcpy(m_curFinProduct.szName, csNewValue.GetBuffer(0));
			csNewValue.ReleaseBuffer();
			if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_AddFinancialProduct_Req,
				&m_curFinProduct, sizeof(m_curFinProduct)))
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
	else if ( pDispInfo->item.iSubItem == 2)
	{
		CString csOldText(_T(""));
		if ( nFinProductID > 0 )
		{
			if ( it != m_mapFinancialProduct.end())
			{
				csOldText = it->second.szScopeDesc;
			}
		}

		//备注
		if ( csNewValue.GetLength() > sizeof(DESCSTRING)-1)
		{
			MessageBox(_T("备注内容过长，请重新输入！"), lpszPrompt, MB_ICONERROR);
			m_listCtrl.SetItemText(nRow, nCol, csOldText);
			return;
		}

		if ( nFinProductID > 0 )
		{
			//修改
			if ( it != m_mapFinancialProduct.end())
			{
				if ( csNewValue == it->second.szScopeDesc )
				{
					//没有改动，直接返回
					return;
				}
				else
				{
					bModify = true;
					m_curFinProduct = it->second;
					strcpy(m_curFinProduct.szScopeDesc, csNewValue.GetBuffer(0));
					csNewValue.ReleaseBuffer();
				}
			}
		}
	}
	else
	{

	}

	if ( bModify )
	{
		//执行修改
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_ModifyFinancialProduct_Req,
			&m_curFinProduct, sizeof(m_curFinProduct)))
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

CString CFinancialProductDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-02292")))
	{
		csTempMsg = _T("该理财产品正在被使用，无法删除！");
	}
	else if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("理财产品名重复！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}

void CFinancialProductDlg::ResetTreeItems()
{
	m_curOrgSet.clear();
	CheckTreeItem(m_treeCtrl.GetRootItem(), FALSE);
}

void CFinancialProductDlg::CheckTreeItem( HTREEITEM hItem, BOOL bCheck )
{
	if ( hItem == NULL )
	{
		return;
	}

	m_treeCtrl.SetCheck(hItem, bCheck);
	HTREEITEM hChildItem = m_treeCtrl.GetChildItem(hItem);
	while( NULL != hChildItem )
	{
		CheckTreeItem(hChildItem, bCheck);

		hChildItem = m_treeCtrl.GetNextSiblingItem(hChildItem);
	}
}

void CFinancialProductDlg::CheckTreeItem( HTREEITEM hItem, std::set<int> setOrg )
{
	if ( hItem == NULL )
	{
		return;
	}

	int nOrgID = m_treeCtrl.GetItemData(hItem);
	std::set<int>::iterator it_org = setOrg.find(nOrgID);
	if ( it_org != setOrg.end())
	{
		m_treeCtrl.SetCheck(hItem, TRUE);
	}
	else
	{
		m_treeCtrl.SetCheck(hItem, FALSE);
	}
	
	HTREEITEM hChildItem = m_treeCtrl.GetChildItem(hItem);
	while( NULL != hChildItem )
	{
		CheckTreeItem(hChildItem, setOrg);

		hChildItem = m_treeCtrl.GetNextSiblingItem(hChildItem);
	}
}

void CFinancialProductDlg::SetTreeItemData( std::set<int> setOrg )
{
	CheckTreeItem(m_treeCtrl.GetRootItem(), setOrg);
}

void CFinancialProductDlg::GetTreeItemData( std::set<int>& setOrg )
{
	setOrg.clear();
	GetTreeItemCheckedData(m_treeCtrl.GetRootItem(), setOrg);
}

void CFinancialProductDlg::GetTreeItemCheckedData( HTREEITEM hItem, std::set<int>& setOrg )
{
	if ( hItem == NULL )
	{
		return;
	}

	int nOrgID = m_treeCtrl.GetItemData(hItem);
	BOOL bCheck = m_treeCtrl.GetCheck(hItem);
	if ( bCheck && nOrgID > 0 )
	{
		setOrg.insert(nOrgID);
	}

	HTREEITEM hChildItem = m_treeCtrl.GetChildItem(hItem);
	while( NULL != hChildItem )
	{
		GetTreeItemCheckedData(hChildItem, setOrg);

		hChildItem = m_treeCtrl.GetNextSiblingItem(hChildItem);
	}
}

std::map<int, FinancialProduct> CFinancialProductDlg::GetFinProduct()
{
	return m_mapFinancialProduct;
}

std::map<int, std::map<int, FinancialProduct>> CFinancialProductDlg::GetOrgFinProductTree()
{
	std::map<int, std::map<int, FinancialProduct>> retTree;
	std::map<int, std::set<int>>::iterator it_pro = m_mapFinPro2Org.begin();
	for ( ; it_pro != m_mapFinPro2Org.end(); it_pro++ )
	{
		//查找FinancialProduct信息
		std::map<int, FinancialProduct>::iterator it_find 
			= m_mapFinancialProduct.find(it_pro->first);
		if ( it_find == m_mapFinancialProduct.end())
		{
			continue;
		}

		FinancialProduct finPro = it_find->second;
		std::set<int>::iterator it_org = it_pro->second.begin();
		for ( ; it_org != it_pro->second.end(); ++it_org )
		{
			std::map<int, std::map<int, FinancialProduct>>::iterator it 
				= retTree.find(*it_org);
			if ( it != retTree.end())
			{
				(it->second)[finPro.nFinancialProductID] = finPro;
			}
			else
			{
				std::map<int, FinancialProduct> mapTemp;
				mapTemp[finPro.nFinancialProductID] = finPro;
				retTree[*it_org] = mapTemp;
			}
		}
	}

	return retTree;
}
