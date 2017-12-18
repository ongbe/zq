// RiskIndSpecialSet.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "RiskIndSpecialSet.h"
#include "TCPDataService.h"
#include "CGridColumnTraitCombo.h"

// CRiskIndSpecialSet dialog

IMPLEMENT_DYNAMIC(CRiskIndSpecialSet, CDialog)

CRiskIndSpecialSet::CRiskIndSpecialSet(CWnd* pParent /*=NULL*/)
	: CDialog(CRiskIndSpecialSet::IDD, pParent)
{
	m_vDlgs.clear();
	m_vIndicator.clear();
	m_csCurProductID = _T("");

	m_mapRiskWarning.clear();
	m_mapCurRiskWarning.clear();
}

CRiskIndSpecialSet::~CRiskIndSpecialSet()
{
}

void CRiskIndSpecialSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IND_MODULE, m_listModule);
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
	DDX_Control(pDX, IDC_BUTTON_ADD_MODULE, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DEL_MODULE, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_RISK_SAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CRiskIndSpecialSet, CDialog)
	ON_MESSAGE(RECV_DATA_MSG, &CRiskIndSpecialSet::OnRecvMessage)
	ON_BN_CLICKED(IDCANCEL, &CRiskIndSpecialSet::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CRiskIndSpecialSet::OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CRiskIndSpecialSet::OnTcnSelchangeTab)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_IND_MODULE, &CRiskIndSpecialSet::OnLvnEndlabeleditListIndModule)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IND_MODULE, &CRiskIndSpecialSet::OnLvnItemchangedListIndModule)
	ON_BN_CLICKED(IDC_BUTTON_ADD_MODULE, &CRiskIndSpecialSet::OnBnClickedButtonAddModule)
	ON_BN_CLICKED(IDC_BUTTON_DEL_MODULE, &CRiskIndSpecialSet::OnBnClickedButtonDelModule)
	ON_BN_CLICKED(IDC_BUTTON_RISK_SAVE, &CRiskIndSpecialSet::OnBnClickedButtonRiskSave)
END_MESSAGE_MAP()


// CRiskIndSpecialSet message handlers
void CRiskIndSpecialSet::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CRiskIndSpecialSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CRiskIndSpecialSet::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
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

void CRiskIndSpecialSet::OnDestroy()
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

BOOL CRiskIndSpecialSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	for ( UINT i = 0; i < m_vIndicator.size(); i++ )
	{
		//市场冲击风险 和 品种最大持仓要显示
		if ( i != 0 && i!= 4 )
		{
			continue;
		}

		m_tabCtrl.InsertItem(i, m_vIndicator[i].szName);

		CIndicatorTabDlg* pDlg = new CIndicatorTabDlg;
		pDlg->SetIndicatorID(m_vIndicator[i].nRiskIndicatorID);
		pDlg->Create(IDD_DIALOG_IND_TAB, &m_tabCtrl);
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

	m_listModule.SetCellMargin(1.5);
	m_listModule.InsertHiddenLabelColumn();
	CGridColumnTraitCombo* pTrait = new CGridColumnTraitCombo;
	pTrait->SetStyle(pTrait->GetStyle()|CBS_DROPDOWNLIST);
	m_listModule.InsertColumnTrait(1, _T("品种名称"), LVCFMT_LEFT, 130, 0, pTrait);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	//查询所有风险模型信息
	CTCPDataService::GetInstance()->SendData(Cmd_RM_ProductIDList_Req, NULL, 0);
	//查询品种风险预警信息
	CTCPDataService::GetInstance()->SendData(Cmd_RM_QryProductRiskWarning_Req, &m_nTraderID, sizeof(int));

	return TRUE;
}

void CRiskIndSpecialSet::SetIndicator( const std::vector<RiskIndicator>& vIndicator )
{
	m_vIndicator = vIndicator;
}

void CRiskIndSpecialSet::OnLvnEndlabeleditListIndModule(NMHDR *pNMHDR, LRESULT *pResult)
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
	if ( nRow < static_cast<int>(m_vUsedProductID.size()) && csNewValue == m_vUsedProductID[nRow])
	{
		//未发生改变
		return;
	}

	bool bFind = false;
	for ( UINT i = 0; i < m_vUsedProductID.size(); i++ )
	{
		//不用和自身比较
		if ( i == nRow )
		{
			continue;
		}

		if ( csNewValue == m_vUsedProductID[i])
		{
			bFind = true;
			break;
		}
	}

	if ( bFind )
	{
		MessageBox(_T("品种名称重复！"), lpszPrompt, MB_ICONERROR);
		m_listModule.SetItemText(nRow, nCol, m_vUsedProductID[nRow]);
		return;
	}
	else
	{
		m_vUsedProductID[nRow] = csNewValue;
		m_listModule.SelectRow(nRow, true);

		//刷新指标预警设置窗口
		m_csCurProductID = m_listModule.GetItemText(nRow, 1);

		RefreshTabItems();
	}
}

void CRiskIndSpecialSet::OnLvnItemchangedListIndModule(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		m_csCurProductID = m_listModule.GetItemText(pNMLV->iItem, 1);

		RefreshTabItems();

	}
}

void CRiskIndSpecialSet::OnBnClickedButtonAddModule()
{
	// TODO: Add your control notification handler code here
	m_listModule.SetFocus();
	int nCount = m_listModule.GetItemCount();
	if ( nCount > 0 && m_listModule.GetItemText(nCount-1, 1).IsEmpty())
	{
		m_listModule.SelectRow(nCount-1, true);
		return;
	}
	else
	{
		m_listModule.InsertItem(nCount, "");
		m_listModule.SetItemText(nCount, 1, "");
		m_listModule.SelectRow(nCount, true);

		m_vUsedProductID.push_back(_T(""));
	}

	ResetTabItems();
}

void CRiskIndSpecialSet::OnBnClickedButtonDelModule()
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


	CString csProductID = m_listModule.GetItemText(nSel, 1);
	std::map<CString, std::map<int, std::vector<IndLevelWarning>>>::iterator it
		= m_mapRiskWarning.find(csProductID);
	if ( it == m_mapRiskWarning.end())
	{
		m_listModule.DeleteItem(nSel);
		DelProductID(nSel);
		return;
	}

	ProducrRiskWarningKey key;
	memset(&key, 0, sizeof(key));
	strcpy(key.szProductID, csProductID.GetBuffer(0));
	csProductID.ReleaseBuffer();
	key.nTraderID = m_nTraderID;
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_DelProductRiskWarning_Req,
		&key, sizeof(key)))
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

void CRiskIndSpecialSet::ResetTabItems()
{
	for ( UINT i = 0 ; i < m_vDlgs.size(); i++ )
	{
		m_vDlgs[i]->ResetData();
	}
}

void CRiskIndSpecialSet::EnableControls( BOOL bFlag )
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

LRESULT CRiskIndSpecialSet::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_ProductIDList_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_setProductID.clear();
			CGridColumnTraitCombo* pTrait = (CGridColumnTraitCombo*)m_listModule.GetColumnTrait(1);

			int nCount = pRecvData->head.len / sizeof(InstrumentIDType);
			for ( int i = 0; i < nCount; i++ )
			{
				InstrumentIDType* pProductID = (InstrumentIDType*)((char*)pRecvData->pData + i*sizeof(InstrumentIDType));
				m_setProductID.insert(*pProductID);
				pTrait->AddItem(i, *pProductID);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	if ( pRecvData->head.cmdid == Cmd_RM_QryProductRiskWarning_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapRiskWarning.clear();
			m_vUsedProductID.clear();
			m_listModule.DeleteAllItems();

			int nCount = pRecvData->head.len / sizeof(ProductRiskWarning);
			for ( int i = 0; i < nCount; i++ )
			{
				ProductRiskWarning* pRiskWarning = (ProductRiskWarning*)((char*)pRecvData->pData + i*sizeof(ProductRiskWarning));

				IndLevelWarning levelWarning;
				levelWarning.nRiskIndicatorID = pRiskWarning->nRiskIndicatorID;
				levelWarning.nRiskLevelID = pRiskWarning->nRiskLevelID;
				levelWarning.nResponseType = pRiskWarning->nResponseType;
				levelWarning.fThresholdValue = pRiskWarning->fThresholdValue;
				levelWarning.nColor = pRiskWarning->nColor;

				std::map<CString, std::map<int, std::vector<IndLevelWarning>>>::iterator it_module
					= m_mapRiskWarning.find(pRiskWarning->szProductID);
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
					m_mapRiskWarning[pRiskWarning->szProductID] = mapIndRiskWarning;

					//品种列表中填入数据
					std::set<CString>::iterator it = m_setProductID.find(pRiskWarning->szProductID);
					if ( it != m_setProductID.end())
					{
						int nCount = m_listModule.GetItemCount();
						m_listModule.InsertItem(nCount, _T(""));
						m_listModule.SetItemText(nCount, 1, *it);
						m_vUsedProductID.push_back(*it);
					}
				}
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
	else if ( pRecvData->head.cmdid == Cmd_RM_SaveProductRiskWarning_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			ProducrRiskWarningKey key = *(ProducrRiskWarningKey*)pRecvData->pData;
			m_mapRiskWarning[key.szProductID] = m_mapCurRiskWarning;

			MessageBox(_T("保存成功！"), lpszPrompt, MB_ICONINFORMATION);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_RM_DelProductRiskWarning_Rsp )
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			ProducrRiskWarningKey key = *(ProducrRiskWarningKey*)pRecvData->pData;

			std::map<CString, std::map<int, std::vector<IndLevelWarning>>>::iterator it
				= m_mapRiskWarning.find(key.szProductID);
			if ( it != m_mapRiskWarning.end())
			{
				m_mapRiskWarning.erase(it);
			}

			int nCount = m_listModule.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( key.szProductID == m_listModule.GetItemText(i, 1))
				{
					m_listModule.DeleteItem(i);
					DelProductID(i);
					break;
				}
			}

			if ( key.szProductID == m_csCurProductID )
			{
				m_csCurProductID = _T("");
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
	else
	{

	}

	return 0;
}

CString CRiskIndSpecialSet::ConventErrorMsg( const CString& csMsg )
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

void CRiskIndSpecialSet::DelProductID( int nSel )
{
	std::vector<CString>::iterator it = m_vUsedProductID.begin();
	std::advance(it, nSel);
	if ( it != m_vUsedProductID.end())
	{
		m_vUsedProductID.erase(it);
	}
}

void CRiskIndSpecialSet::OnBnClickedButtonRiskSave()
{
	// TODO: Add your control notification handler code here
	if ( m_csCurProductID.IsEmpty())
	{
		MessageBox(_T("请先选择一个品种！"), lpszPrompt, MB_ICONERROR);
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

	ProducrRiskWarningKey key;
	memset(&key, 0, sizeof(key));
	strcpy(key.szProductID, m_csCurProductID.GetBuffer(0));
	m_csCurProductID.ReleaseBuffer();
	key.nTraderID = m_nTraderID;

	int nDataLen = sizeof(ProducrRiskWarningKey) + sizeof(IndLevelWarning)*vAllWarningData.size();
	char* pBuffer = new char[nDataLen];
	memset(pBuffer, 0, nDataLen);
	memcpy(pBuffer, &key, sizeof(ProducrRiskWarningKey));
	if ( !vAllWarningData.empty() )
	{
		memcpy(pBuffer+sizeof(ProducrRiskWarningKey), &vAllWarningData[0], sizeof(IndLevelWarning)*vAllWarningData.size());
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_SaveProductRiskWarning_Req,
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

void CRiskIndSpecialSet::SetTraderID( int nTraderID )
{
	m_nTraderID = nTraderID;
}

void CRiskIndSpecialSet::RefreshTabItems()
{
	ResetTabItems();

	if ( m_csCurProductID.IsEmpty() )
	{
		return;
	}

	for ( UINT i = 0; i < m_vDlgs.size(); i++ )
	{
		m_vDlgs[i]->SetValid(TRUE);
	}

	std::map<CString, std::map<int, std::vector<IndLevelWarning>>>::iterator it_module
		= m_mapRiskWarning.find(m_csCurProductID);
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
