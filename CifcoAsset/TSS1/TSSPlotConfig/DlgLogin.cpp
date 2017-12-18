// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "TSSPlotConfig.h"
#include "DlgLogin.h"


// CDlgLogin dialog

IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPassword(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDTUSERNAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, 100);
	DDX_Text(pDX, IDC_EDTPASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 30);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLogin message handlers

void CDlgLogin::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	if(!UpdateData())
		return;

	if(m_strPassword.Compare("99178603")==0)
		OnOK();
	else
		AfxMessageBox("√‹¬Î¥ÌŒÛ£¨«Î÷ÿ–¬ ‰»Î°£", MB_OK|MB_ICONERROR);
}
