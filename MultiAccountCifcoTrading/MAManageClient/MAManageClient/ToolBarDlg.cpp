// ToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "ToolBarDlg.h"


// CToolBarDlg dialog

IMPLEMENT_DYNAMIC(CToolBarDlg, CDialog)

CToolBarDlg::CToolBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarDlg::IDD, pParent)
{

}

CToolBarDlg::~CToolBarDlg()
{
}

void CToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TOOL_BAR_ACCOUNT, m_edtAccount);
	DDX_Control(pDX, IDC_COMBO_TOOL_BAR_OWNER, m_cbxOwner);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl);
	DDX_Control(pDX, IDC_CHECK_DATE, m_chkDate);
}


BEGIN_MESSAGE_MAP(CToolBarDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CToolBarDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CToolBarDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CToolBarDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_DATE, &CToolBarDlg::OnBnClickedCheckDate)
END_MESSAGE_MAP()


// CToolBarDlg message handlers

void CToolBarDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CToolBarDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
}

void CToolBarDlg::OnBnClickedButtonSearch()
{
	// TODO: Add your control notification handler code here
	GetParent()->GetParent()->SendMessage(MSG_SEARCH, 0, 0);
}

void CToolBarDlg::AddItem( int nAccountID, const CString& csAccount )
{
	int nIndex = m_cbxOwner.AddString(csAccount);
	if ( nIndex != CB_ERR )
	{
		m_cbxOwner.SetItemData(nIndex, nAccountID);
	}
}

void CToolBarDlg::DelItem( int nAccountID )
{
	int nCount = m_cbxOwner.GetCount();
	for ( int i = 0; i < nCount; i++ )
	{
		if ( nAccountID == m_cbxOwner.GetItemData(i))
		{
			m_cbxOwner.DeleteString(i);
			break;
		}
	}
}

void CToolBarDlg::GetSearchParam( int& nOwnerAccountID, CString& csAccount, CString& csDate )
{
	int nSel = m_cbxOwner.GetCurSel();
	if ( nSel != CB_ERR )
	{
		nOwnerAccountID = m_cbxOwner.GetItemData(nSel);
	}

	m_edtAccount.GetWindowText(csAccount);
	if ( m_chkDate.GetCheck() )
	{
		CTime time;
		m_dateCtrl.GetTime(time);
		csDate.Format(_T("%4d-%02d-%02d"),
			time.GetYear(), time.GetMonth(), time.GetDay());
	}
	else
	{
		csDate = _T("");
	}
}

void CToolBarDlg::ClearItems()
{
	m_cbxOwner.ResetContent();
}

BOOL CToolBarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nIndex = m_cbxOwner.AddString(lpszAllAccount);
	if ( nIndex != CB_ERR )
	{
		m_cbxOwner.SetItemData(nIndex, INVALID_ACOUNTID);
	}

	m_cbxOwner.SetCurSel(0);
	m_chkDate.SetCheck(FALSE);
	m_dateCtrl.EnableWindow(FALSE);

	return TRUE;
}

void CToolBarDlg::SetData( std::map<int, AccountInfoEx> accountInfoMap )
{
	m_cbxOwner.ResetContent();
	int nIndex = m_cbxOwner.AddString(lpszAllAccount);
	if ( nIndex != CB_ERR )
	{
		m_cbxOwner.SetItemData(nIndex, INVALID_ACOUNTID);
	}

	std::map<int, AccountInfoEx>::iterator it = accountInfoMap.begin();
	for ( ; it != accountInfoMap.end(); it++ )
	{
		if ( it->second.accountInfo.nAccountType == ACCOUNT_TYPE_MANAGE )
		{
			int nIndex = m_cbxOwner.AddString(it->second.accountInfo.szAccount);
			if ( nIndex != CB_ERR )
			{
				m_cbxOwner.SetItemData(nIndex, it->second.accountInfo.nAccountID);
			}
		}
	}

	m_cbxOwner.SetCurSel(0);
}

void CToolBarDlg::OnBnClickedCheckDate()
{
	// TODO: Add your control notification handler code here
	m_dateCtrl.EnableWindow(m_chkDate.GetCheck());
}
