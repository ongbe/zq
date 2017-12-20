// UserSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "UserSetDlg.h"


// CUserSetDlg dialog

IMPLEMENT_DYNAMIC(CUserSetDlg, CDialog)

CUserSetDlg::CUserSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserSetDlg::IDD, pParent)
{
	m_vIDCardInfo.clear();
	memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
}

CUserSetDlg::~CUserSetDlg()
{
}

void CUserSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USER_NAME, m_edtUserName);
	DDX_Control(pDX, IDC_EDIT_CONTACTINFO, m_edtContactInfo);
	DDX_Control(pDX, IDC_COMBO_IDCARD_TYPE, m_cbxIDCardType);
	DDX_Control(pDX, IDC_EDIT_IDCARD_NO, m_edtIDCardNo);
	DDX_Control(pDX, IDC_CHECK_ATTR1, m_chkAttr1);
}


BEGIN_MESSAGE_MAP(CUserSetDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CUserSetDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CUserSetDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDOK, &CUserSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserSetDlg message handlers

void CUserSetDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	m_curAccountInfo.nViewPermission2 = 0;
}

void CUserSetDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	m_curAccountInfo.nViewPermission2 = 1;
}

void CUserSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csUserName;
	m_edtUserName.GetWindowText(csUserName);
	strcpy(m_curAccountInfo.szName, csUserName.GetBuffer(0));
	csUserName.ReleaseBuffer();
	CString csContactInfo;
	m_edtContactInfo.GetWindowText(csContactInfo);
	strcpy(m_curAccountInfo.szContactInfo, csContactInfo.GetBuffer(0));
	csContactInfo.ReleaseBuffer();
	int nIDCardTypeSel = m_cbxIDCardType.GetCurSel();
	if ( nIDCardTypeSel >= 0 &&
		nIDCardTypeSel < static_cast<int>(m_vIDCardInfo.size()))
	{
		m_curAccountInfo.nIDCardType = m_vIDCardInfo[nIDCardTypeSel].nIDCardType;
	}
	else
	{
		m_curAccountInfo.nIDCardType = 1;//默认使用第一项
	}

	CString csIDCardNo;
	m_edtIDCardNo.GetWindowText(csIDCardNo);
	strcpy(m_curAccountInfo.szIDCardNo, csIDCardNo.GetBuffer(0));
	csIDCardNo.ReleaseBuffer();

	m_curAccountInfo.nViewPermission1 = m_chkAttr1.GetCheck();

	OnOK();
}

BOOL CUserSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtUserName.SetLimitText(MAX_USER_NAME_LENGTH);
	m_edtContactInfo.SetLimitText(MAX_CONTACTINFO_LENGTH);
	m_edtIDCardNo.SetLimitText(MAX_IDCARD_NO_LENGTH);
	m_edtUserName.SetWindowText(m_curAccountInfo.szName);
	m_edtContactInfo.SetWindowText(m_curAccountInfo.szContactInfo);
	m_edtIDCardNo.SetWindowText(m_curAccountInfo.szIDCardNo);
	m_cbxIDCardType.ResetContent();
	for ( UINT i = 0; i < m_vIDCardInfo.size(); i++ )
	{
		m_cbxIDCardType.AddString(m_vIDCardInfo[i].szIDCardName);
		if ( m_curAccountInfo.nIDCardType == m_vIDCardInfo[i].nIDCardType )
		{
			m_cbxIDCardType.SetCurSel(i);
		}
	}

	m_chkAttr1.SetCheck( m_curAccountInfo.nViewPermission1);
	if ( m_curAccountInfo.nViewPermission2 == 0 )
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
	}

	return TRUE;
}

void CUserSetDlg::SetIDCardInfoData( std::vector<IDCardInfo>& vIDCardInfo )
{
	m_vIDCardInfo = vIDCardInfo;
}

void CUserSetDlg::SetAccountInfo( const AccountInfo& accountInfo )
{
	m_curAccountInfo = accountInfo;
}

const AccountInfo& CUserSetDlg::GetAccountInfo()
{
	return m_curAccountInfo;
}
