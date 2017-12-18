// AccountManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include <algorithm>
#include "RiskManage.h"
#include "AccountManageDlg.h"

const int DEFAULT_ROW_NUM_PER_PAGE = 100;
static int s_nColumn = 0;
static BOOL s_bAscending = FALSE;
// AccountManageDlg dialog

IMPLEMENT_DYNAMIC(CAccountManageDlg, CDialog)

bool Compare(const CAccountListData& s1,const CAccountListData& s2)
{
	if ( s_bAscending )
	{
		return s1.GetValue(s_nColumn).Compare(s2.GetValue(s_nColumn)) < 0;
	}
	else
	{
		return s1.GetValue(s_nColumn).Compare(s2.GetValue(s_nColumn)) > 0;
	}
}

CAccountManageDlg::CAccountManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountManageDlg::IDD, pParent)
{
	m_bInited = false;
	m_vListData.clear();
	m_nCurShowPage = 1;
	m_nRowNumPerPage = DEFAULT_ROW_NUM_PER_PAGE;
	m_bModifyMenuStatus = false;
	m_bDelMenuStatus = false;
	m_bStopMenuStatus = false;
}

CAccountManageDlg::~CAccountManageDlg()
{
}

void CAccountManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BKG_ACCOUNT, m_userList);
	DDX_Control(pDX, IDC_STATIC_TOTAL, m_staticTotal);
	DDX_Control(pDX, IDC_STATIC_CUR_PAGE, m_staticCurPage);
	DDX_Control(pDX, IDC_STATIC_JUMP_TO, m_staticJumpTo);
	DDX_Control(pDX, IDC_COMBO_PAGE_LIST, m_cbxPageList);
	DDX_Control(pDX, IDC_STATIC_HOME, m_staticHome);
	DDX_Control(pDX, IDC_STATIC_PRE, m_staticPre);
	DDX_Control(pDX, IDC_STATIC_NEXT, m_staticNext);
	DDX_Control(pDX, IDC_STATIC_END, m_staticEnd);
	DDX_Control(pDX, IDC_STATIC_EVERY_PAGE, m_staticEveryPage);
	DDX_Control(pDX, IDC_EDIT_ROW, m_edtRow);
	DDX_Control(pDX, IDC_STATIC_ROW, m_staticRow);
}


BEGIN_MESSAGE_MAP(CAccountManageDlg, CDialog)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE_LIST, &CAccountManageDlg::OnCbnSelchangeComboPageList)
	ON_STN_CLICKED(IDC_STATIC_HOME, &CAccountManageDlg::OnStnClickedStaticHome)
	ON_STN_CLICKED(IDC_STATIC_PRE, &CAccountManageDlg::OnStnClickedStaticPre)
	ON_STN_CLICKED(IDC_STATIC_NEXT, &CAccountManageDlg::OnStnClickedStaticNext)
	ON_STN_CLICKED(IDC_STATIC_END, &CAccountManageDlg::OnStnClickedStaticEnd)
	ON_EN_KILLFOCUS(IDC_EDIT_ROW, &CAccountManageDlg::OnEnKillfocusEditRow)
	ON_BN_CLICKED(IDCANCEL, &CAccountManageDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CAccountManageDlg::OnBnClickedOk)
	ON_CBN_KILLFOCUS(IDC_COMBO_PAGE_LIST, &CAccountManageDlg::OnCbnKillfocusComboPageList)
	ON_CBN_SELENDOK(IDC_COMBO_PAGE_LIST, &CAccountManageDlg::OnCbnSelendokComboPageList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BKG_ACCOUNT, &CAccountManageDlg::OnLvnItemchangedListBkgAccount)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BKG_ACCOUNT, &CAccountManageDlg::OnNMDblclkListBkgAccount)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BKG_ACCOUNT, &CAccountManageDlg::OnNMRClickListBkgAccount)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SPECIAL_IND, &CAccountManageDlg::OnMenuSpecialIndSet)
	ON_COMMAND(ID_FUNDNET_PARAM, &CAccountManageDlg::OnMenuFundNetParam)
	ON_COMMAND(ID_FORCE_CLOSE_PARAM, &CAccountManageDlg::OnMenuForceCloseParam)
	ON_COMMAND(ID_IDR_EXPORT_LIST, &CAccountManageDlg::OnExportList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_BKG_ACCOUNT, &CAccountManageDlg::OnLvnColumnclickListBkgAccount)
END_MESSAGE_MAP()

void CAccountManageDlg::GetSelectAccountID(std::vector<int>& vec)
{
	POSITION pos = m_userList.GetFirstSelectedItemPosition();
	while ( pos != NULL )
	{
		int nIndex = m_userList.GetNextSelectedItem(pos);
		int nAccountID = m_userList.GetItemData(nIndex);
		vec.push_back(nAccountID);
	}
}

BOOL CAccountManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtRow.SetLimitText(2);
	CString csTemp;
	csTemp.Format(_T("%d"), m_nRowNumPerPage);
	m_edtRow.SetWindowText(csTemp);

	DWORD dwStyle = m_userList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |=  LVS_EX_GRIDLINES;
	m_userList.SetExtendedStyle(dwStyle);

	m_imageList.Create(16,16,ILC_COLOR32|ILC_MASK,0,1);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_ARROW_UP));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_ARROW_DOWN));
	m_userList.GetHeaderCtrl()->SetImageList(&m_imageList, LVSIL_SMALL);

	PositionControls();

	m_bInited = true;

	UpdatePageBar();

	return TRUE;
}

void CAccountManageDlg::CreateListColumn( const std::vector<CString>& vCol )
{
	m_vTitle = vCol;
	int nColCount = m_userList.GetHeaderCtrl()->GetItemCount();
	for ( int i = 0; i < nColCount; i++ )
	{
		m_userList.DeleteColumn(0);
	}

	for ( int i = 0; i < static_cast<int>(m_vTitle.size()); i++ )
	{
		m_userList.InsertColumn( i, m_vTitle[i], LVCFMT_LEFT, 90);
	}
}

void CAccountManageDlg::OnSize( UINT nType, int cx, int cy )
{
	CDialog::OnSize(nType, cx, cy);

	if ( m_bInited )
	{
		PositionControls();
	}
}

void CAccountManageDlg::PositionControls()
{
	CRect rc;
	GetClientRect(&rc);

	CRect listRect(rc);
	listRect.bottom = listRect.bottom - 40;
	m_userList.MoveWindow(&listRect);

	int nStartPos = (rc.Width() - 600)/2;
	CRect tempRc(rc);
	tempRc.left = nStartPos;
	tempRc.top = tempRc.bottom - 26;
	tempRc.bottom = tempRc.bottom - 13;
	tempRc.right = tempRc.left + 74;
	m_staticTotal.MoveWindow(&tempRc);

	tempRc.left = nStartPos + 85;
	tempRc.right = tempRc.left + 51;
	m_staticEveryPage.MoveWindow(&tempRc);

	tempRc.left = nStartPos + 140;
	tempRc.right = tempRc.left + 20;
	CRect rcRow(tempRc);
	rcRow.top -= 3;
	rcRow.bottom += 3;
	m_edtRow.MoveWindow(&rcRow);

	tempRc.left = nStartPos + 164;
	tempRc.right = tempRc.left + 15;
	m_staticRow.MoveWindow(&tempRc);

	tempRc.left = nStartPos + 190;
	tempRc.right = tempRc.left + 85;
	m_staticCurPage.MoveWindow(&tempRc);

	tempRc.left = nStartPos + 280;
	tempRc.right = tempRc.left + 38;
	m_staticJumpTo.MoveWindow(&tempRc);

	tempRc.left = nStartPos + 325;
	tempRc.right = tempRc.left + 51;
	CRect rcPageList(tempRc);
	rcPageList.top -= 3;
	rcPageList.bottom += 3;
	m_cbxPageList.MoveWindow(&rcPageList);

	tempRc.left = nStartPos + 393;
	tempRc.right = tempRc.left + 26;
	m_staticHome.MoveWindow(&tempRc);

	tempRc.left = nStartPos + 430;
	tempRc.right = tempRc.left + 38;
	m_staticPre.MoveWindow(&tempRc);

	tempRc.left = nStartPos + 475;
	tempRc.right = tempRc.left + 38;
	m_staticNext.MoveWindow(&tempRc);

	tempRc.left = nStartPos + 525;
	tempRc.right = tempRc.left + 26;
	m_staticEnd.MoveWindow(&tempRc);

}

void CAccountManageDlg::SetData( std::vector<CAccountListData>& vUserInfoEx )
{
	m_vListData = vUserInfoEx;

	m_nCurShowPage = 1;
	ReshowListData();
}

void CAccountManageDlg::AddAccount( const CAccountListData& userInfoEx )
{
	m_vListData.push_back(userInfoEx);

	//int nCount = m_accountList.GetItemCount();
	//if ( nCount < m_nRowNumPerPage )
	//{
	//	m_accountList.InsertItem(nCount, _T(""));
	//	SetRowData(nCount, accountInfoEx);
	//}
	//else
	//{

	//}

	//更新分页工具栏
	//UpdatePageBar();
	ReshowListData();
}

void CAccountManageDlg::ModifyAccount( CAccountListData& listData )
{
	int nIndex = FindAccount(listData.GetKey());
	if ( nIndex == -1 )
	{
		return;
	}
	else
	{
		m_vListData[nIndex] = listData;
		//int nCount = m_accountList.GetItemCount();
		//for ( int i = 0; i < nCount; i++ )
		//{
		//	if ( m_accountList.GetItemData(i) == accountInfoEx.accountInfo.nAccountID )
		//	{
		//		SetRowData(i, accountInfoEx);
		//		break;
		//	}
		//}
		ReshowListData();
	}
}

void CAccountManageDlg::DelAccount( int nAccountID )
{
	int nIndex = FindAccount(nAccountID);
	if ( nIndex == -1 )
	{
		return;
	}
	else
	{
		std::vector<CAccountListData>::iterator it = m_vListData.begin();
		std::advance(it, nIndex);
		m_vListData.erase(it);

		int nCount = m_userList.GetItemCount();
		for ( int i = 0; i < nCount; i++ )
		{
			if ( m_userList.GetItemData(i) == nAccountID )
			{
				m_userList.DeleteItem(i);
				break;
			}
		}

		nCount = m_userList.GetItemCount();
		if ( nCount < m_nRowNumPerPage )
		{
			int nDistance = (m_nCurShowPage-1) * m_nRowNumPerPage + nCount;
			std::vector<CAccountListData>::iterator it_dis = m_vListData.begin();
			if ( nDistance < static_cast<int>(m_vListData.size()))
			{
				std::advance(it_dis, nDistance);
				if ( it_dis != m_vListData.end())
				{
					m_userList.InsertItem(nCount, _T(""));
					SetRowData(nCount, *it_dis);
				}
			}
		}

		UpdatePageBar();
	}
}

void CAccountManageDlg::ClearData()
{
	m_userList.DeleteAllItems();
	m_vListData.clear();
	m_nCurShowPage = 1;
	//m_cbxPageList.ResetContent();

	//UpdatePageBar();
}

void CAccountManageDlg::SetRowData( int nCount, CAccountListData& userInfoEx )
{
	m_userList.SetItemData(nCount, userInfoEx.GetKey());
	for ( int i = 0; i < static_cast<int>(m_vTitle.size()); i++ )
	{
		m_userList.SetItemText(nCount, i, userInfoEx.GetValue(i));
	}
}

int CAccountManageDlg::GetTotalPage()
{
	int nTotalPage = m_vListData.size() / m_nRowNumPerPage;
	if ( m_vListData.size() % m_nRowNumPerPage != 0 )
	{
		nTotalPage += 1;
	}

	return nTotalPage;
}

void CAccountManageDlg::UpdatePageBar()
{
	CString csTotal;
	csTotal.Format(_T("总数：%d"), m_vListData.size());
	m_staticTotal.SetWindowText(csTotal);

	CString csPage;
	int nTotalPage = GetTotalPage();
	csPage.Format(_T("当前第%d/%d页"), m_nCurShowPage, nTotalPage); 
	m_staticCurPage.SetWindowText(csPage);

	int nCount = m_cbxPageList.GetCount();
	if ( nCount > nTotalPage )
	{
		for ( int i = 0; i < nCount - nTotalPage; i++ )
		{
			m_cbxPageList.DeleteString( nCount-1-i );
		}
	}
	else if ( nCount < nTotalPage )
	{
		for ( int i = 0; i < nTotalPage - nCount; i++ )
		{
			CString csTemp;
			csTemp.Format(_T("%d"), nCount+1+i);
			m_cbxPageList.AddString( csTemp );
		}
	}
	else
	{

	}

	m_cbxPageList.SetCurSel(m_nCurShowPage-1);

	if ( nTotalPage == 1 )
	{
		m_staticHome.EnableWindow(FALSE);
		m_staticPre.EnableWindow(FALSE);
		m_staticNext.EnableWindow(FALSE);
		m_staticEnd.EnableWindow(FALSE);
	}
	else
	{
		if ( m_nCurShowPage == 1 )
		{
			m_staticHome.EnableWindow(FALSE);
			m_staticPre.EnableWindow(FALSE);
			m_staticNext.EnableWindow(TRUE);
			m_staticEnd.EnableWindow(TRUE);
		}
		else if ( m_nCurShowPage == nTotalPage )
		{
			m_staticHome.EnableWindow(TRUE);
			m_staticPre.EnableWindow(TRUE);
			m_staticNext.EnableWindow(FALSE);
			m_staticEnd.EnableWindow(FALSE);
		}
		else
		{
			m_staticHome.EnableWindow(TRUE);
			m_staticPre.EnableWindow(TRUE);
			m_staticNext.EnableWindow(TRUE);
			m_staticEnd.EnableWindow(TRUE);
		}
	}
	
}

void CAccountManageDlg::ShowListData()
{
	m_userList.DeleteAllItems();
	int nDistance = (m_nCurShowPage-1) * m_nRowNumPerPage;
	std::vector<CAccountListData>::iterator it_dis = m_vListData.begin();
	if ( nDistance < static_cast<int>(m_vListData.size()))
	{
		std::advance(it_dis, nDistance);
		for ( int i = 0; i < m_nRowNumPerPage && it_dis != m_vListData.end(); i++, it_dis++ )
		{
			m_userList.InsertItem(i, _T(""));
			SetRowData(i, *it_dis);
		}
	}
}


// AccountManageDlg message handlers

void CAccountManageDlg::OnCbnSelchangeComboPageList()
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_cbxPageList.GetCurSel() + 1;
	if ( m_nCurShowPage != nCurSel )
	{
		m_nCurShowPage = nCurSel;

		ShowListData();
		UpdatePageBar();
	}
}

void CAccountManageDlg::OnStnClickedStaticHome()
{
	// TODO: Add your control notification handler code here
	if ( m_nCurShowPage > 1 )
	{
		m_nCurShowPage = 1;

		ShowListData();
		UpdatePageBar();
	}
}

void CAccountManageDlg::OnStnClickedStaticPre()
{
	// TODO: Add your control notification handler code here
	if ( m_nCurShowPage > 1 )
	{
		m_nCurShowPage -= 1;

		ShowListData();
		UpdatePageBar();
	}
}

void CAccountManageDlg::OnStnClickedStaticNext()
{
	// TODO: Add your control notification handler code here
	if ( m_nCurShowPage < GetTotalPage() )
	{
		m_nCurShowPage += 1;

		ShowListData();
		UpdatePageBar();
	}
}

void CAccountManageDlg::OnStnClickedStaticEnd()
{
	// TODO: Add your control notification handler code here
	if ( m_nCurShowPage < GetTotalPage() )
	{
		m_nCurShowPage = GetTotalPage();

		ShowListData();
		UpdatePageBar();
	}
}

void CAccountManageDlg::OnEnKillfocusEditRow()
{
	// TODO: Add your control notification handler code here
	CString csText;
	m_edtRow.GetWindowText(csText);
	int nNumber = atoi(csText.GetBuffer(0));
	csText.ReleaseBuffer();
	if ( nNumber == 0 )
	{
		csText.Format(_T("%d"), m_nRowNumPerPage);
		m_edtRow.SetWindowText(csText);
		m_edtRow.SetFocus();
		return;
	}
	else
	{
		if ( m_nRowNumPerPage != nNumber )
		{
			m_nRowNumPerPage = nNumber;
			ShowListData();
			UpdatePageBar();
		}
	}
}

void CAccountManageDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
}

void CAccountManageDlg::OnBnClickedOk()
{
	if( m_edtRow.GetSafeHwnd() == ::GetFocus())
	{
		GetNextDlgTabItem(GetFocus())->SetFocus();
		//OnEnKillfocusEditRow();
		return;
	}

	if ( GetDlgItem(IDC_COMBO_PAGE_LIST) == GetFocus()->GetParent())
	{
		GetNextDlgTabItem(GetFocus())->SetFocus();
		//OnCbnKillfocusComboPageList();
		return;
	}
}

//BOOL CAccountManageDlg::PreTranslateMessage( MSG* pMsg )
//{
//	if (WM_KEYDOWN == pMsg->message)
//	{
//		if (VK_RETURN == pMsg->wParam)
//			return TRUE;
//	}
//
//	return CDialog::PreTranslateMessage( pMsg );
//}

void CAccountManageDlg::OnCbnKillfocusComboPageList()
{
	// TODO: Add your control notification handler code here
	CString csText;
	m_cbxPageList.GetWindowText(csText);
	int nPos = m_cbxPageList.FindStringExact(-1, csText);
	if ( CB_ERR != nPos )
	{
		if ( nPos+1 != m_nCurShowPage )
		{
			m_cbxPageList.SetCurSel( nPos );
			m_nCurShowPage = nPos+1;

			ShowListData();
			UpdatePageBar();
		}
	}
	else
	{
		m_cbxPageList.SetCurSel( m_nCurShowPage-1 );
	}
}

void CAccountManageDlg::OnCbnSelendokComboPageList()
{
	// TODO: Add your control notification handler code here
	int i = 0;
}

void CAccountManageDlg::OnLvnItemchangedListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		int nAccountID = m_userList.GetItemData(pNMLV->iItem);

		//AfxGetMainWnd()->SendMessage(MSG_LIST_SEL_CHANGE, 0, nAccountID);
	}
}


void CAccountManageDlg::OnNMDblclkListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	if ( pNMItemActivate->iItem == -1 )
	{
		return;
	}

	int nAccountID = m_userList.GetItemData(pNMItemActivate->iItem);
	AfxGetMainWnd()->SendMessage(MSG_MODIFY_ACCOUNT, 0, nAccountID);

	*pResult = 0;
}

void CAccountManageDlg::OnNMRClickListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	if ( pNMItemActivate->iItem == -1 )
	{
		return;
	}

	if( m_eType != ACCOUNT_TRUST_TRADE)
	{
		return;
	}

	//弹出右键菜单
	POINT pt;
	GetCursorPos(&pt);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_LIST));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}

void CAccountManageDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}

void CAccountManageDlg::OnMenuSpecialIndSet()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_SPECIAL_IND, 0);
}

void CAccountManageDlg::OnMenuFundNetParam()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FUNDNET_PARAM, 0);
}

void CAccountManageDlg::OnMenuForceCloseParam()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_FORCE_CLOSE_PARAM, 0);
}

//void CAccountManageDlg::SetPermissionData( const PermissionData& data )
//{
//	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_MODIFY )
//	{
//		m_bModifyMenuStatus = true;
//	}
//
//	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_DEL )
//	{
//		m_bDelMenuStatus = true;
//	}
//
//	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_CHANGE_STATUS )
//	{
//		m_bStopMenuStatus = true;
//	}
//}

void CAccountManageDlg::OnLvnColumnclickListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_userList.Sort(pNMLV->iSubItem);

	m_nCurShowPage = 1;
	ReshowListData();

	*pResult = 0;
}

int CAccountManageDlg::FindAccount( int nAccountID )
{
	for ( UINT i = 0; i < m_vListData.size(); i++ )
	{
		if ( m_vListData[i].GetKey() == nAccountID )
		{
			return i;
		}
	}

	return -1;
}

void CAccountManageDlg::ReshowListData()
{
	s_nColumn = 0;
	s_bAscending = FALSE;
	m_userList.GetSortParam(s_nColumn, s_bAscending);
	if ( s_nColumn == -1 )
	{
		m_userList.Sort(0);
		m_userList.GetSortParam(s_nColumn, s_bAscending);
	}

	//排序
	sort(m_vListData.begin(), m_vListData.end(), Compare);

	ShowListData();

	UpdatePageBar();
}

void CAccountManageDlg::OnExportList()
{
	//CString csFile = m_csDate + ".csv";
	CFileDialog dlg(FALSE, NULL, _T(".csv"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"CSV files (*.csv)");
	if( IDOK == dlg.DoModal())
	{
		CString csFile = dlg.GetPathName();
		CStdioFile file;
		CString strSeparator(_T(","));

		if (!file.Open(csFile, CFile::modeWrite | CFile::modeCreate| CFile::typeText))
		{
			return;
		}

		//标题行
		CString csTitle = _T("账户号,角色,权限,所属资管,理财产品,账号状态,用户名,联系方式");
		for ( int i = 0; i < static_cast<int>(m_vTitle.size()); i++ )
		{
			if ( i != 0 )
			{
				csTitle += _T(",");
			}

			csTitle += m_vTitle[i];
		}
		file.WriteString(csTitle);
		file.WriteString(_T("\n"));

		for (UINT i = 0; i < m_vListData.size(); i++)
		{
			CAccountListData info = m_vListData[i];
			CString csLine;
			for ( int j = 0; j < static_cast<int>(m_vTitle.size()); j++ )
			{
				if ( j != 0 )
				{
					csLine += _T(",");
				}

				csLine += info.GetValue(j);
			}

			file.WriteString(csLine);
			file.WriteString(_T("\n"));
		}

		file.Close();
	}
}

void CAccountManageDlg::SetAccountType( AccountType eType )
{
	m_eType = eType;
}
