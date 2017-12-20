// ServerGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "ServerGroupDlg.h"


// CServerGroupDlg dialog

IMPLEMENT_DYNAMIC(CServerGroupDlg, CDialog)

CServerGroupDlg::CServerGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerGroupDlg::IDD, pParent)
	, m_eType(OPERATOR_ADD)
{
	memset(&m_serverGroup, 0, sizeof(m_serverGroup));
}

CServerGroupDlg::~CServerGroupDlg()
{
}

void CServerGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GROUP_NAME, m_edtGroup);
	DDX_Control(pDX, IDC_CHECK_GROUP_SHARE, m_chkShare);
}


BEGIN_MESSAGE_MAP(CServerGroupDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CServerGroupDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CServerGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtGroup.SetLimitText(MAX_GROUP_NAME_LENGTH);

	if ( m_eType == OPERATOR_MODIFY )
	{
		m_edtGroup.SetWindowText(m_serverGroup.szServerName);
		m_edtGroup.SetSel(0, strlen(m_serverGroup.szServerName)-1);
		m_chkShare.SetCheck(m_serverGroup.nShare);
	}

	return TRUE;
}

void CServerGroupDlg::SetOperatorType( OperatorType eType )
{
	m_eType = eType;
}

void CServerGroupDlg::SetServerGroup( const ServerGroup& severGroup )
{
	m_serverGroup = severGroup;
}

ServerGroup& CServerGroupDlg::GetServerGroup()
{
	return m_serverGroup;
}


// CServerGroupDlg message handlers

void CServerGroupDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csGroupName;
	m_edtGroup.GetWindowText(csGroupName);
	if ( csGroupName.IsEmpty())
	{
		MessageBox(lpszEmptyServrGroup, lpszPrompt, MB_ICONERROR);
		m_edtGroup.SetFocus();
		return;
	}

	strcpy(m_serverGroup.szServerName, csGroupName.GetBuffer(0));
	csGroupName.ReleaseBuffer();
	m_serverGroup.nShare = m_chkShare.GetCheck();

	OnOK();
}
