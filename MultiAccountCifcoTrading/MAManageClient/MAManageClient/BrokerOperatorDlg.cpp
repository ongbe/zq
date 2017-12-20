// BrokerOperatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "BrokerOperatorDlg.h"
#include "ConfigMgr.h"


// CBrokerOperatorDlg dialog

IMPLEMENT_DYNAMIC(CBrokerOperatorDlg, CDialog)

CBrokerOperatorDlg::CBrokerOperatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrokerOperatorDlg::IDD, pParent)
	, m_eOperatorType(OPERATOR_ADD)
{
	memset(&m_brokerInfo, 0, sizeof(m_brokerInfo));
}

CBrokerOperatorDlg::~CBrokerOperatorDlg()
{
}

void CBrokerOperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BROKER_CODE, m_edtCode);
	DDX_Control(pDX, IDC_EDIT_COMPANY, m_edtCompany);
	DDX_Control(pDX, IDC_CHECK1, m_cbxShare);
}


BEGIN_MESSAGE_MAP(CBrokerOperatorDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBrokerOperatorDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CBrokerOperatorDlg::SetOperatorType( OperatorType eType )
{
	m_eOperatorType = eType;
}

void CBrokerOperatorDlg::SetBrokerInfo( const BrokerInfo& info )
{
	m_brokerInfo = info;
}

BOOL CBrokerOperatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtCode.SetLimitText(20);
	m_edtCompany.SetLimitText(64);

	if ( m_eOperatorType == OPERATOR_MODIFY )
	{
		m_edtCode.SetWindowText(m_brokerInfo.szCode);
		m_edtCode.EnableWindow(FALSE);
		m_edtCompany.SetWindowText(m_brokerInfo.szCompany);
		m_cbxShare.SetCheck(m_brokerInfo.nShareWithLowerLever);

		SetWindowText(_T("修改经纪公司"));
	}
	else
	{
		SetWindowText(_T("新增经纪公司"));
	}

	return TRUE;
}

const BrokerInfo& CBrokerOperatorDlg::GetBrokerInfo()
{
	return m_brokerInfo;
}


// CBrokerOperatorDlg message handlers

void CBrokerOperatorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csBrokerCode;
	m_edtCode.GetWindowText(csBrokerCode);
	if ( csBrokerCode.IsEmpty())
	{
		MessageBox(lpszEmptyBrokerCode, lpszPrompt, MB_ICONERROR);
		m_edtCode.SetFocus();
		return;
	}

	CString csCompany;
	m_edtCompany.GetWindowText(csCompany);

	strcpy(m_brokerInfo.szCode, csBrokerCode.GetBuffer(0));
	strcpy(m_brokerInfo.szCompany, csCompany.GetBuffer(0));
	csBrokerCode.ReleaseBuffer();
	csCompany.ReleaseBuffer();
	m_brokerInfo.nShareWithLowerLever = m_cbxShare.GetCheck();
	m_brokerInfo.nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();

	OnOK();
}
