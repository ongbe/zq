// ServerAddrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "ServerAddrDlg.h"


// CServerAddrDlg dialog

IMPLEMENT_DYNAMIC(CServerAddrDlg, CDialog)

CServerAddrDlg::CServerAddrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerAddrDlg::IDD, pParent)
{
	m_eType = OPERATOR_ADD;
	m_eServerType = SERVER_TYPE_TRADE;
	memset(&m_addr, 0, sizeof(m_addr));
}

CServerAddrDlg::~CServerAddrDlg()
{
}

void CServerAddrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ADDR, m_editIP);
	DDX_Control(pDX, IDC_EDIT_PORT, m_edtPort);
}


BEGIN_MESSAGE_MAP(CServerAddrDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CServerAddrDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CServerAddrDlg message handlers

void CServerAddrDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csIP;
	m_editIP.GetWindowText(csIP);
	if ( csIP.IsEmpty())
	{
		MessageBox(lpszEmptyServrAddr, lpszPrompt, MB_ICONERROR);
		m_editIP.SetFocus();
		return;
	}

	CString csPort;
	m_edtPort.GetWindowText(csPort);
	if ( csPort.IsEmpty())
	{
		MessageBox(lpszEmptyServrPort, lpszPrompt, MB_ICONERROR);
		m_edtPort.SetFocus();
		return;
	}

	strcpy(m_addr.szServerAddress, csIP.GetBuffer(0));
	csIP.ReleaseBuffer();
	m_addr.nPort = atoi(csPort.GetBuffer(0));
	csPort.ReleaseBuffer();
	m_addr.nServerType = m_eServerType;

	OnOK();
}

BOOL CServerAddrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_editIP.SetLimitText(128);
	m_edtPort.SetLimitText(5);

	if ( m_eType == OPERATOR_MODIFY )
	{
		m_editIP.SetWindowText(m_addr.szServerAddress);
		CString csPort;
		csPort.Format("%d", m_addr.nPort);
		m_edtPort.SetWindowText(csPort);
	}

	return TRUE;
}

void CServerAddrDlg::SetOperatorInfo( OperatorType eType, ServerType eServerType )
{
	m_eType = eType;
	m_eServerType = eServerType;
}

void CServerAddrDlg::SetServerAddr( const ServerAddr& severAddr )
{
	m_addr = severAddr;
}

ServerAddr& CServerAddrDlg::GetServerAddr()
{
	return m_addr;
}
