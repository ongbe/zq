// WarningSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "WarningSetDlg.h"
#include "TCPDataService.h"
#include "CGridColumnTraitEdit.h"

// CWarningSetDlg dialog

IMPLEMENT_DYNAMIC(CWarningSetDlg, CDialog)

CWarningSetDlg::CWarningSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningSetDlg::IDD, pParent)
{
	m_vDlgs.clear();
	m_vIndicator.clear();
	memset(&m_curIndModule, 0, sizeof(m_curIndModule));
	m_nCurIndModuleID = -1;
	m_bAbandon = false;

	m_mapRiskWarning.clear();
	m_mapCurRiskWarning.clear();
}

CWarningSetDlg::~CWarningSetDlg()
{
}

void CWarningSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IND_MODULE, m_listModule);
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
	DDX_Control(pDX, IDC_BUTTON_ADD_MODULE, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DEL_MODULE, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_RISK_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CWarningSetDlg, CDialog)
	ON_MESSAGE(RECV_DATA_MSG, &CWarningSetDlg::OnRecvMessage)
	//ON_MESSAGE(MSG_SAVE_WARNING_SET, &CWarningSetDlg::OnSaveWarningSet)
	ON_BN_CLICKED(IDCANCEL, &CWarningSetDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CWarningSetDlg::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CWarningSetDlg::OnTcnSelchangeTab)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_IND_MODULE, &CWarningSetDlg::OnLvnEndlabeleditListIndModule)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IND_MODULE, &CWarningSetDlg::OnLvnItemchangedListIndModule)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MODULE, &CWarningSetDlg::OnBnClickedButtonAddModule)
	ON_BN_CLICKED(IDC_BUTTON_DEL_MODULE, &CWarningSetDlg::OnBnClickedButtonDelModule)
	ON_BN_CLICKED(IDC_BUTTON_RISK_SAVE, &CWarningSetDlg::OnBnClickedButtonRiskSave)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_IND_MODULE, &CWarningSetDlg::OnLvnItemchangingListIndModule)
END_MESSAGE_MAP()


// CWarningSetDlg message handlers

void CWarningSetDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CWarningSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CWarningSetDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nCurSel = m_tabCtrl.GetCurSel();
	for ( UINT i = 0; i < m_vDlgs.size(); i++ )
	{
		CIndicatorTabDlg* pDlg = m_vDlgs[i];
		if ( i == nCurSel )
		{
			pDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg->ShowWindow(SW_HIDE);
		}
	}
}

void CWarningSetDlg::OnDestroy()
{
	for ( UINT i = 0; i < m_vDlgs.size(); i++ )
	{
		if ( m_vDlgs[i] != NULL )
		{
			m_vDlgs[i]->DestroyWindow();
			delete m_vDlgs[i];
			m_vDlgs[i] = NULL;
		}
	}
	m_vDlgs.clear();

	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CWarningSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for ( UINT i = 0; i < m_vIndicator.size(); i++ )
	{
		m_tabCtrl.InsertItem(i, m_vIndicator[i].szName);

		CIndicatorTabDlg* pDlg = new CIndicatorTabDlg;
		pDlg->SetIndicatorID(m_vIndicator[i].nRiskIndicatorID);
		pDlg->Create(IDD_DIALOG_IND_TAB, &m_tabCtrl);
		if ( m_vIndicator[i].nRiskIndicatorID == RI_FundNetValue )
		{
			pDlg->SetSortedOrder(true);
		}

		m_vDlgs.push_back(pDlg);
	}

	CRect rc;
	m_tabCtrl.GetClientRect(rc);
	CRect rcTabItem;
	m_tabCtrl.GetItemRect(0, rcTabItem);
	rc.left += 2;
	rc.right -= 2;
	rc.top += rcTabItem.Height() + 3;
	rc.bottom -= 2;
	for ( UINT i = 0; i < m_vDlgs.size(); i++ )
	{
		CIndicatorTabDlg* pDlg = m_vDlgs[i];
		pDlg->MoveWindow(rc);
		if ( 0 == i )
		{
			pDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			pDlg->ShowWindow(SW_HIDE);
		}
	}

	m_tabCtrl.SetCurSel(0);

	m_listModule.InsertHiddenLabelColumn();
	CGridColumnTraitEdit* pTrait = new CGridColumnTraitEdit;
	m_listModule.InsertColumnTrait(1, _T("模型名称"), LVCFMT_LEFT, 130, 0, pTrait);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	//查询风险预警信息
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryRiskWarning_Req, NULL, 0);
	//查询所有风险模型信息
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryRiskIndModule_Req, NULL, 0);

	return TRUE;
}

void CWarningSetDlg::SetIndicator( const std::vector<RiskIndicator>& vIndicator )
{
	m_vIndicator = vIndicator;
}

void CWarningSetDlg::OnLvnEndlabeleditListIndModule(NMHDR *pNMHDR, LRESULT *pResult)
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

	int nIndModuleID = m_listModule.GetItemData(nRow);
	std::map<int, RiskIndicatorModule>::iterator it = m_mapIndModule.find(nIndModuleID);
	CString csOldText(_T(""));
	if ( nIndModuleID > 0 )
	{
		if ( it != m_mapIndModule.end())
		{
			csOldText = it->second.szName;
		}
	}

	//风险模型名称
	if ( csNewValue.IsEmpty())
	{
		MessageBox(_T("风险模型名称不能为空！"), lpszPrompt, MB_ICONERROR);
		m_listModule.SetItemText(nRow, nCol, csOldText);
		return;
	}

	if ( csNewValue.GetLength() > sizeof(OTHERNAMESTRING)-1)
	{
		MessageBox(_T("风险模型名称过长，请重新输入！"), lpszPrompt, MB_ICONERROR);
		m_listModule.SetItemText(nRow, nCol, csOldText);
		return;
	}

	if ( nIndModuleID > 0 )
	{
		//修改
		if ( it != m_mapIndModule.end())
		{
			if ( csNewValue == it->second.szName )
			{
				//没有改动，直接返回
				return;
			}
			else
			{
				m_curIndModule = it->second;
				strcpy(m_curIndModule.szName, csNewValue.GetBuffer(0));
				csNewValue.ReleaseBuffer();
				//执行修改
				if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_ModifyRiskIndModule_Req,
					&m_curIndModule, sizeof(m_curIndModule)))
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
	}
	else
	{
		//新增
		memset(&m_curIndModule, 0, sizeof(m_curIndModule));
		strcpy(m_curIndModule.szName, csNewValue.GetBuffer(0));
		csNewValue.ReleaseBuffer();
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_AddRiskIndModule_Req,
			&m_curIndModule, sizeof(m_curIndModule)))
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

void CWarningSetDlg::OnLvnItemchangedListIndModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		ResetTabItems();

		m_nCurIndModuleID = m_listModule.GetItemData(pNMLV->iItem);
		for ( UINT i = 0; i < m_vDlgs.size(); i++ )
		{
			m_vDlgs[i]->SetValid(TRUE);
		}

		std::map<int, std::map<int, std::vector<IndLevelWarning>>>::iterator it_module
			= m_mapRiskWarning.find(m_nCurIndModuleID);
		if ( it_module != m_mapRiskWarning.end())
		{
			std::map<int, std::vector<IndLevelWarning>>::iterator it_ind = it_module->second.begin();
			for ( ; it_ind != it_module->second.end(); ++it_ind )
			{
				for ( UINT i = 0; i < m_vDlgs.size(); i++ )
				{
					int nIndicatorID = m_vDlgs[i]->GetIndicatorID();
					if ( nIndicatorID == it_ind->first )
					{
						m_vDlgs[i]->SetWarningData(it_ind->second);
						break;
					}
				}
			}
		}
	}

	*pResult = 0;
}

void CWarningSetDlg::OnBnClickedButtonAddModule()
{
	// TODO: Add your control notification handler code here
	m_listModule.SetFocus();
	int nCount = m_listModule.GetItemCount();
	if ( nCount > 0 && m_listModule.GetItemText(nCount-1, 1).IsEmpty())
	{
		m_listModule.SelectRow(nCount-1, true);
		m_listModule.EditCell(nCount-1, 1);
		return;
	}
	else
	{
		m_listModule.InsertItem(nCount, "");
		m_listModule.SetItemText(nCount, 1, "");
		m_listModule.SelectRow(nCount, true);
		m_listModule.EditCell(nCount, 1);
	}

	ResetTabItems();
}

void CWarningSetDlg::OnBnClickedButtonDelModule()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listModule.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_listModule.GetNextSelectedItem(pos);
	if ( IDYES != MessageBox(lpszDelDataComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	int nIndModuleID = m_listModule.GetItemData(nSel);
	if ( nIndModuleID <= 0)
	{
		m_listModule.DeleteItem(nSel);
		return;
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_DelRiskIndModule_Req,
		&nIndModuleID, sizeof(nIndModuleID)))
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

void CWarningSetDlg::ResetTabItems()
{
	for ( UINT i = 0 ; i < m_vDlgs.size(); i++ )
	{
		m_vDlgs[i]->ResetData();
	}
}

void CWarningSetDlg::EnableControls( BOOL bFlag )
{
	m_listModule.EnableWindow(bFlag);
	m_btnAdd.EnableWindow(bFlag);
	m_btnDel.EnableWindow(bFlag);
	m_btnSave.EnableWindow(bFlag);

	for ( UINT i = 0; i < m_vDlgs.size(); i++ )
	{
		m_vDlgs[i]->EnableControls(bFlag);
	}
}

LRESULT CWarningSetDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_QryRiskIndModule_Rsp )
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

			m_listModule.DeleteAllItems();
			std::map<int, RiskIndicatorModule>::iterator it = m_mapIndModule.begin();
			for ( ; it != m_mapIndModule.end(); ++it )
			{
				int nCount = m_listModule.GetItemCount();
				m_listModule.InsertItem(nCount, _T(""));
				m_listModule.SetItemText(nCount, 1, it->second.szName);
				m_listModule.SetItemData(nCount, it->second.nRiskIndModuleID);
			}

			if ( m_listModule.GetItemCount() > 0 )
			{
				m_listModule.SelectRow(0, true);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_ModifyRiskIndModule_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nIndModuleID = *(int*)pRecvData->pData;
			if ( nIndModuleID == m_curIndModule.nRiskIndModuleID )
			{
				m_mapIndModule[nIndModuleID] = m_curIndModule;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);

			int nCount = m_listModule.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( m_listModule.GetItemData(i) == m_curIndModule.nRiskIndModuleID )
				{
					m_listModule.SetFocus();
					break;
				}
			}
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_AddRiskIndModule_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curIndModule.nRiskIndModuleID = *(int*)pRecvData->pData;
			m_mapIndModule[m_curIndModule.nRiskIndModuleID] = m_curIndModule;

			int nCount = m_listModule.GetItemCount();
			if( m_listModule.GetItemData(nCount-1) <= 0)
			{
				m_listModule.SetItemData(nCount-1, m_curIndModule.nRiskIndModuleID);
				m_listModule.SelectRow(nCount-1, true);
				m_nCurIndModuleID = m_curIndModule.nRiskIndModuleID;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);

			int nCount = m_listModule.GetItemCount();
			if( m_listModule.GetItemData(nCount-1) <= 0)
			{
				m_listModule.SetFocus();
			}
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_DelRiskIndModule_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nModuleID = *(int*)pRecvData->pData;
			std::map<int, RiskIndicatorModule>::iterator it = m_mapIndModule.find(nModuleID);
			if ( it != m_mapIndModule.end())
			{
				m_mapIndModule.erase(it);
			}

			int nCount = m_listModule.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( nModuleID == m_listModule.GetItemData(i))
				{
					m_listModule.DeleteItem(i);
					break;
				}
			}

			std::map<int, std::map<int, std::vector<IndLevelWarning>>>::iterator it_module = m_mapRiskWarning.find(nModuleID);
			if ( it_module != m_mapRiskWarning.end() )
			{
				m_mapRiskWarning.erase(it_module);
			}

			if ( nModuleID == m_nCurIndModuleID )
			{
				m_nCurIndModuleID = -1;
				if ( m_listModule.GetItemCount() > 0 )
				{
					m_listModule.SelectRow(0, true);
				}
				else
				{
					ResetTabItems();
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
	if ( pRecvData->head.cmdid == Cmd_RM_QryRiskWarning_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapRiskWarning.clear();
			int nCount = pRecvData->head.len / sizeof(RiskWarning);
			for ( int i = 0; i < nCount; i++ )
			{
				RiskWarning* pRiskWarning = (RiskWarning*)((char*)pRecvData->pData + i*sizeof(RiskWarning));
				IndLevelWarning levelWarning;
				levelWarning.nRiskIndicatorID = pRiskWarning->nRiskIndicatorID;
				levelWarning.nRiskLevelID = pRiskWarning->nRiskLevelID;
				levelWarning.nResponseType = pRiskWarning->nResponseType;
				levelWarning.fThresholdValue = pRiskWarning->fThresholdValue;
				levelWarning.nColor = pRiskWarning->nColor;

				std::map<int, std::map<int, std::vector<IndLevelWarning>>>::iterator it_module
					= m_mapRiskWarning.find(pRiskWarning->nRiskIndModuleID);
				if ( it_module != m_mapRiskWarning.end())
				{
					std::map<int, std::vector<IndLevelWarning>>::iterator it_ind = 
						it_module->second.find(pRiskWarning->nRiskIndicatorID);
					if ( it_ind != it_module->second.end() )
					{
						it_ind->second.push_back(levelWarning);
					}
					else
					{
						std::vector<IndLevelWarning> vRiskWarning;
						vRiskWarning.push_back(levelWarning);
						(it_module->second)[levelWarning.nRiskIndicatorID] = vRiskWarning;
					}
				}
				else
				{
					std::map<int, std::vector<IndLevelWarning>> mapIndRiskWarning;
					std::vector<IndLevelWarning> vRiskWarning;
					vRiskWarning.push_back(levelWarning);
					mapIndRiskWarning[levelWarning.nRiskIndicatorID] = vRiskWarning;
					m_mapRiskWarning[pRiskWarning->nRiskIndModuleID] = mapIndRiskWarning;
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_SaveRiskWarning_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nRiskModuleID = *(int*)pRecvData->pData;

			m_mapRiskWarning[nRiskModuleID] = m_mapCurRiskWarning;

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

CString CWarningSetDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-02292")))
	{
		csTempMsg = _T("该风险模型正在被使用，无法删除！");
	}
	else if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("风险模型名重复！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}

//LRESULT CWarningSetDlg::OnSaveWarningSet( WPARAM wParam, LPARAM lParam )
//{
//
//}

void CWarningSetDlg::OnBnClickedButtonRiskSave()
{
	// TODO: Add your control notification handler code here
	if ( m_nCurIndModuleID <=0 )
	{
		MessageBox(_T("请先选择一个风险模型！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	//检查各界面数据输入是否正确
	bool bChanged = false;
	for ( UINT i = 0; i < m_vDlgs.size(); i++ )
	{
		if ( !m_vDlgs[i]->RetrieveData())
		{
			m_tabCtrl.SetCurFocus(i);
			return;
		}

		if ( m_vDlgs[i]->IsDataChanged())
		{
			bChanged = true;
		}
	}

	if ( !bChanged )
	{
		MessageBox(_T("无数据改动！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	//获取数据
	std::vector<IndLevelWarning> vAllWarningData;
	for ( UINT i = 0; i < m_vDlgs.size(); i++ )
	{
		std::vector<IndLevelWarning> vIndLevelWarning;
		int nIndicatorID = m_vDlgs[i]->GetIndicatorID();
		vIndLevelWarning = m_vDlgs[i]->GetWarningData();

		vAllWarningData.insert(vAllWarningData.end(), vIndLevelWarning.begin(), vIndLevelWarning.end());
		m_mapCurRiskWarning[nIndicatorID] = vIndLevelWarning;
	}

	if ( vAllWarningData.empty())
	{
		MessageBox(_T("至少需要有一条数据才能保存！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	int nDataLen = sizeof(int) + sizeof(IndLevelWarning)*vAllWarningData.size();
	char* pBuffer = new char[nDataLen];
	memset(pBuffer, 0, nDataLen);
	memcpy(pBuffer, &m_nCurIndModuleID, sizeof(int));
	if ( !vAllWarningData.empty() )
	{
		memcpy(pBuffer+sizeof(int), &vAllWarningData[0], sizeof(IndLevelWarning)*vAllWarningData.size());
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_SaveRiskWarning_Req,
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
	pBuffer = NULL;

	return;
}

std::map<int, RiskIndicatorModule> CWarningSetDlg::GetRiskIndModule()
{
	return m_mapIndModule;
}

void CWarningSetDlg::OnLvnItemchangingListIndModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	// TODO: Add your control notification handler code here
	//检查是否有未保存的数据
	//if ( pNMLV->uNewState == 3 &&
	//	pNMLV->uOldState == 0 )
	//{
	//	m_bAbandon = false;
	//	bool bChanged = false;
	//	for ( UINT i = 0; i < m_vDlgs.size(); i++ )
	//	{
	//		if ( !m_vDlgs[i]->RetrieveData(false))
	//		{
	//			bChanged = true;
	//		}
	//		else
	//		{
	//			if ( m_vDlgs[i]->IsDataChanged())
	//			{
	//				bChanged = true;
	//			}
	//		}
	//	}

	//	if ( bChanged )
	//	{
	//		int nRet = MessageBox(_T("数据尚未保存，切换风险模型将会放弃修改，是否继续？"), lpszPrompt, MB_ICONQUESTION|MB_YESNO);
	//		if ( nRet != IDYES )
	//		{
	//			m_bAbandon = true;
	//			*pResult = TRUE;
	//			return;
	//		}
	//	}
	//}

	//if ( m_bAbandon )
	//{
	//	*pResult = TRUE;
	//}
	//else
	//{
	//	*pResult = FALSE;
	//}
}
