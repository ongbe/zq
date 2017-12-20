// AccountManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include <algorithm>
#include "MAManageClient.h"
#include "AccountManageDlg.h"

const int DEFAULT_ROW_NUM_PER_PAGE = 100;
static int nColumn = 0;
static BOOL bAscending = FALSE;
// AccountManageDlg dialog

IMPLEMENT_DYNAMIC(CAccountManageDlg, CDialog)

bool Compare(const AccountInfoEx& s1,const AccountInfoEx& s2)
{
	switch( nColumn )
	{
	case 0:	//账户号
		if ( bAscending )
		{
			return strcmp(s1.accountInfo.szAccount, s2.accountInfo.szAccount) < 0;
		}
		else
		{
			return strcmp(s1.accountInfo.szAccount, s2.accountInfo.szAccount) > 0;
		}
	case 1:	//创建者
		if ( bAscending )
		{
			return strcmp(s1.szOwnerAccount, s2.szOwnerAccount) < 0;
		}
		else
		{
			return strcmp(s1.szOwnerAccount, s2.szOwnerAccount) > 0;
		}
	case 2:	//权限
		if ( bAscending )
		{
			return strcmp(s1.szPermissionName, s2.szPermissionName) < 0;
		}
		else
		{
			return strcmp(s1.szPermissionName, s2.szPermissionName) > 0;
		}
	case 3:	//用户名
		if ( bAscending )
		{
			return strcmp(s1.accountInfo.szName, s2.accountInfo.szName) < 0;
		}
		else
		{
			return strcmp(s1.accountInfo.szName, s2.accountInfo.szName) > 0;
		}
	case 4:	//联系方式
		if ( bAscending )
		{
			return strcmp(s1.accountInfo.szContactInfo, s2.accountInfo.szContactInfo) < 0;
		}
		else
		{
			return strcmp(s1.accountInfo.szContactInfo, s2.accountInfo.szContactInfo) > 0;
		}
	case 5:	//证件类型
		return s1.accountInfo.nIDCardType < s2.accountInfo.nIDCardType;
	case 6:	//证件号码
		if ( bAscending )
		{
			return strcmp(s1.accountInfo.szIDCardNo, s2.accountInfo.szIDCardNo) < 0;
		}
		else
		{
			return strcmp(s1.accountInfo.szIDCardNo, s2.accountInfo.szIDCardNo) > 0;
		}
	case 7:	//账号状态
		if ( bAscending )
		{
			return s1.accountInfo.nAccountStatus < s2.accountInfo.nAccountStatus;
		}
		else
		{
			return s1.accountInfo.nAccountStatus > s2.accountInfo.nAccountStatus;
		}
	case 8:	//登录状态
		if ( bAscending )
		{
			return s1.accountInfo.nLogonStatus < s2.accountInfo.nLogonStatus;
		}
		else
		{
			return s1.accountInfo.nLogonStatus > s2.accountInfo.nLogonStatus;
		}
	case 9:	//有效期
		if ( bAscending )
		{
			return strcmp(s1.accountInfo.szDate, s2.accountInfo.szDate) < 0;
		}
		else
		{
			return strcmp(s1.accountInfo.szDate, s2.accountInfo.szDate) > 0;
		}
	default:
		return true;
	}
}

CAccountManageDlg::CAccountManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountManageDlg::IDD, pParent)
{
	m_bInited = false;
	m_vAccountInfoEx.clear();
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
	DDX_Control(pDX, IDC_LIST_BKG_ACCOUNT, m_accountList);
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
	ON_COMMAND(ID_POP_MODIFY, &CAccountManageDlg::OnMenuModify)
	ON_COMMAND(ID_POP_DEL, &CAccountManageDlg::OnMenuDel)
	ON_COMMAND(ID_POP_STOP, &CAccountManageDlg::OnMenuStop)
	ON_COMMAND(ID_IDR_EXPORT_LIST, &CAccountManageDlg::OnExportList)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_BKG_ACCOUNT, &CAccountManageDlg::OnLvnColumnclickListBkgAccount)
END_MESSAGE_MAP()

void CAccountManageDlg::GetSelectAccountID(std::vector<int>& vec)
{
	POSITION pos = m_accountList.GetFirstSelectedItemPosition();
	while ( pos != NULL )
	{
		int nIndex = m_accountList.GetNextSelectedItem(pos);
		int nAccountID = m_accountList.GetItemData(nIndex);
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

	DWORD dwStyle = m_accountList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |=  LVS_EX_GRIDLINES;
	m_accountList.SetExtendedStyle(dwStyle);

	m_imageList.Create(16,16,ILC_COLOR32|ILC_MASK,0,1);
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_ARROW_UP));
	m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_ARROW_DOWN));
	m_accountList.GetHeaderCtrl()->SetImageList(&m_imageList, LVSIL_SMALL);

	m_accountList.InsertColumn( 0, _T("账户号"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 1, _T("创建者"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 2, _T("权限"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 3, _T("用户名"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 4, _T("联系方式"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 5, _T("证件类型"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 6, _T("证件号码"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 7, _T("账号状态"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 8, _T("登录状态"), LVCFMT_LEFT, 90);
	m_accountList.InsertColumn( 9, _T("有效期"), LVCFMT_LEFT, 90);

	PositionControls();

	m_bInited = true;

	UpdatePageBar();

	return TRUE;
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
	m_accountList.MoveWindow(&listRect);

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

void CAccountManageDlg::SetData( std::map<int, AccountInfoEx>& mapAccountInfoEx )
{
	m_vAccountInfoEx.clear();
	std::map<int, AccountInfoEx>::iterator it = mapAccountInfoEx.begin();
	for ( ; it != mapAccountInfoEx.end(); it++ )
	{
		m_vAccountInfoEx.push_back(it->second);
	}

	m_nCurShowPage = 1;
	ReshowListData();
}

void CAccountManageDlg::AddAccount( const AccountInfoEx& accountInfoEx )
{
	m_vAccountInfoEx.push_back(accountInfoEx);

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

void CAccountManageDlg::ModifyAccount( const AccountInfoEx& accountInfoEx )
{
	int nIndex = FindAccount(accountInfoEx.accountInfo.nAccountID);
	if ( nIndex == -1 )
	{
		return;
	}
	else
	{
		m_vAccountInfoEx[nIndex] = accountInfoEx;
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
		std::vector<AccountInfoEx>::iterator it = m_vAccountInfoEx.begin();
		std::advance(it, nIndex);
		m_vAccountInfoEx.erase(it);

		int nCount = m_accountList.GetItemCount();
		for ( int i = 0; i < nCount; i++ )
		{
			if ( m_accountList.GetItemData(i) == nAccountID )
			{
				m_accountList.DeleteItem(i);
				break;
			}
		}

		nCount = m_accountList.GetItemCount();
		if ( nCount < m_nRowNumPerPage )
		{
			int nDistance = (m_nCurShowPage-1) * m_nRowNumPerPage + nCount;
			std::vector<AccountInfoEx>::iterator it_dis = m_vAccountInfoEx.begin();
			if ( nDistance < static_cast<int>(m_vAccountInfoEx.size()))
			{
				std::advance(it_dis, nDistance);
				if ( it_dis != m_vAccountInfoEx.end())
				{
					m_accountList.InsertItem(nCount, _T(""));
					SetRowData(nCount, *it_dis);
				}
			}
		}

		UpdatePageBar();
	}
}

void CAccountManageDlg::ClearData()
{
	m_accountList.DeleteAllItems();
	m_vAccountInfoEx.clear();
	m_nCurShowPage = 1;
	m_cbxPageList.ResetContent();

	UpdatePageBar();
}

void CAccountManageDlg::SetRowData( int nCount, const AccountInfoEx& accountInfoEx )
{
	m_accountList.SetItemData(nCount, accountInfoEx.accountInfo.nAccountID);
	m_accountList.SetItemText(nCount, 0, accountInfoEx.accountInfo.szAccount);
	m_accountList.SetItemText(nCount, 1, accountInfoEx.szOwnerAccount);
	m_accountList.SetItemText(nCount, 2, accountInfoEx.szPermissionName);
	m_accountList.SetItemText(nCount, 3, accountInfoEx.accountInfo.szName);
	m_accountList.SetItemText(nCount, 4, accountInfoEx.accountInfo.szContactInfo);
	m_accountList.SetItemText(nCount, 5, accountInfoEx.szIDCardName);
	m_accountList.SetItemText(nCount, 6, accountInfoEx.accountInfo.szIDCardNo);
	if ( accountInfoEx.accountInfo.nAccountStatus == 0 )
	{
		m_accountList.SetItemText(nCount, 7, lpszAccountStop);
	}
	else
	{
		m_accountList.SetItemText(nCount, 7, lpszAccoutActive);
	}

	if ( accountInfoEx.accountInfo.nLogonStatus == 0 )
	{
		m_accountList.SetItemText(nCount, 8, lpszStatusLogout);
	}
	else
	{
		m_accountList.SetItemText(nCount, 8, lpszStatusLogon);
	}

	if ( strcmp(accountInfoEx.accountInfo.szDate, VALID_FOREVER_DATE) == 0 )
	{
		m_accountList.SetItemText(nCount, 9, _T("永久有效"));
	}
	else
	{
		m_accountList.SetItemText(nCount, 9, accountInfoEx.accountInfo.szDate);
	}
}

int CAccountManageDlg::GetTotalPage()
{
	int nTotalPage = m_vAccountInfoEx.size() / m_nRowNumPerPage;
	if ( m_vAccountInfoEx.size() % m_nRowNumPerPage != 0 )
	{
		nTotalPage += 1;
	}

	return nTotalPage;
}

void CAccountManageDlg::UpdatePageBar()
{
	CString csTotal;
	csTotal.Format(_T("总数：%d"), m_vAccountInfoEx.size());
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
	m_accountList.DeleteAllItems();
	int nDistance = (m_nCurShowPage-1) * m_nRowNumPerPage;
	std::vector<AccountInfoEx>::iterator it_dis = m_vAccountInfoEx.begin();
	if ( nDistance < static_cast<int>(m_vAccountInfoEx.size()))
	{
		std::advance(it_dis, nDistance);
		for ( int i = 0; i < m_nRowNumPerPage && it_dis != m_vAccountInfoEx.end(); i++, it_dis++ )
		{
			m_accountList.InsertItem(i, _T(""));
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
		int nAccountID = m_accountList.GetItemData(pNMLV->iItem);

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

	int nAccountID = m_accountList.GetItemData(pNMItemActivate->iItem);
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

	//弹出右键菜单
	POINT pt;
	GetCursorPos(&pt);
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU_LIST));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	if ( !m_bModifyMenuStatus )
	{
		pPopup->EnableMenuItem(ID_POP_MODIFY, MF_GRAYED);
	}
	else
	{
		pPopup->EnableMenuItem(ID_POP_MODIFY, MF_ENABLED);
	}

	if ( !m_bDelMenuStatus )
	{
		pPopup->EnableMenuItem(ID_POP_DEL, MF_GRAYED);
	}
	else
	{
		pPopup->EnableMenuItem(ID_POP_DEL, MF_ENABLED);
	}

	if ( !m_bStopMenuStatus )
	{
		pPopup->EnableMenuItem(ID_POP_STOP, MF_GRAYED);
	}
	else
	{
		pPopup->EnableMenuItem(ID_POP_STOP, MF_ENABLED);
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}

void CAccountManageDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: Add your message handler code here
}

void CAccountManageDlg::OnMenuModify()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_TOOL_MODIFY, 0);
}

void CAccountManageDlg::OnMenuDel()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_TOOL_DEL, 0);
}

void CAccountManageDlg::OnMenuStop()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_TOOL_STOP, 0);
}

void CAccountManageDlg::SetPermissionData( const PermissionData& data )
{
	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_MODIFY )
	{
		m_bModifyMenuStatus = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_DEL )
	{
		m_bDelMenuStatus = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_CHANGE_STATUS )
	{
		m_bStopMenuStatus = true;
	}
}

void CAccountManageDlg::OnLvnColumnclickListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_accountList.Sort(pNMLV->iSubItem);

	m_nCurShowPage = 1;
	ReshowListData();

	*pResult = 0;
}

int CAccountManageDlg::FindAccount( int nAccountID )
{
	for ( UINT i = 0; i < m_vAccountInfoEx.size(); i++ )
	{
		if ( m_vAccountInfoEx[i].accountInfo.nAccountID == nAccountID )
		{
			return i;
		}
	}

	return -1;
}

void CAccountManageDlg::ReshowListData()
{
	nColumn = 0;
	bAscending = FALSE;
	m_accountList.GetSortParam(nColumn, bAscending);
	if ( nColumn == -1 )
	{
		m_accountList.Sort(0);
		m_accountList.GetSortParam(nColumn, bAscending);
	}

	//排序
	sort(m_vAccountInfoEx.begin(), m_vAccountInfoEx.end(), Compare);

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
		CString csTitle = _T("账户号,创建者,权限,用户名,联系方式,证件类型,证件号码,账号状态,登录状态,有效期");
		file.WriteString(csTitle);
		file.WriteString(_T("\n"));

		for (UINT i = 0; i < m_vAccountInfoEx.size(); i++)
		{
			AccountInfoEx info = m_vAccountInfoEx[i];
			CString csLine;
			CString csAccountStatus = 
				(info.accountInfo.nAccountStatus == 0) ? lpszAccountStop : lpszAccoutActive;
			CString csLogonStatus = 
				(info.accountInfo.nLogonStatus == 0) ? lpszStatusLogout : lpszStatusLogon;
			CString csDate;
			if ( strcmp(info.accountInfo.szDate, VALID_FOREVER_DATE) == 0 )
			{
				csDate = _T("永久有效");
			}
			else
			{
				csDate = info.accountInfo.szDate;
			}

			csLine.Format(_T("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s"),
				info.accountInfo.szAccount, info.szOwnerAccount,
				info.szPermissionName, info.accountInfo.szName,
				info.accountInfo.szContactInfo, info.szIDCardName,
				info.accountInfo.szIDCardNo, csAccountStatus.GetBuffer(0),
				csLogonStatus.GetBuffer(0), csDate.GetBuffer(0));
			csAccountStatus.ReleaseBuffer();
			csLogonStatus.ReleaseBuffer();
			csDate.ReleaseBuffer();

			file.WriteString(csLine);
			file.WriteString(_T("\n"));
		}

		file.Close();
	}
}
