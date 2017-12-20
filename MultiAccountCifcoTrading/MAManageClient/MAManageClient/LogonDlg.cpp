// LogonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "LogonDlg.h"
#include "ConfigMgr.h"
#include "TCPDataService.h"
#include "MD5/MD5.h"

// CLogonDlg dialog

IMPLEMENT_DYNAMIC(CLogonDlg, CDialog)

CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
	, m_bLogon(false)
	, m_dwStartLogonTime(0)
{

}

CLogonDlg::~CLogonDlg()
{
}

void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_edtAccount);
	DDX_Control(pDX, IDC_EDIT_PWD, m_edtPwd);
	DDX_Control(pDX, IDC_CHECK_REMEMBER, m_chkRemember);
	DDX_Control(pDX, IDOK, m_btnLogon);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_REMEMBER, &CLogonDlg::OnBnClickedCheckRemember)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(RECV_DATA_MSG, &CLogonDlg::OnRecvMessage)
	ON_BN_CLICKED(IDOK, &CLogonDlg::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CLogonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtAccount.SetLimitText(MAX_ACCOUNT_LENGTH);
	m_edtPwd.SetLimitText(MAX_PWD_LENGTH);
	if( ConfigManager::GetInstance()->GetRememberAccountFlag())
	{
		m_chkRemember.SetCheck(TRUE);
		m_edtAccount.SetWindowText(ConfigManager::GetInstance()->GetAccount());
	}

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	return TRUE;
}


// CLogonDlg message handlers

void CLogonDlg::OnBnClickedCheckRemember()
{
	// TODO: Add your control notification handler code here
	//bool bFlag = (m_chkRemember.GetCheck() > 0) ? true:false;
	//ConfigManager::GetInstance()->SetRememberAccountFlag(bFlag);
}

void CLogonDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

LRESULT CLogonDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_Login_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			LoginRsp* pLoginRsp = (LoginRsp*)pRecvData->pData;

			bool bFlag = (m_chkRemember.GetCheck() > 0) ? true:false;
			ConfigManager::GetInstance()->SetRememberAccountFlag(bFlag);
			CString csAccount;
			m_edtAccount.GetWindowText(csAccount);
			ConfigManager::GetInstance()->SetAccount(csAccount);
			CString csPwd;
			m_edtPwd.GetWindowText(csPwd);
			MD5 md5(csPwd.GetBuffer(0));
			csPwd.ReleaseBuffer();
			CString csMD5Pwd = md5.toString().c_str();

			ConfigManager::GetInstance()->SetPassword(csMD5Pwd);
			ConfigManager::GetInstance()->SetAccountID(pLoginRsp->nAccountID);
			ConfigManager::GetInstance()->SetPermissonID(pLoginRsp->nPermissionID);

			m_bLogon = true;

			CTCPDataService::GetInstance()->SetLogonStatus(true);
		}
		else
		{
			EnableControls(TRUE);
			KillTimer(TIMER_LOGON_CHECK);
			m_dwStartLogonTime = 0;
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}

	return 0;
}

void CLogonDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csAccount;
	m_edtAccount.GetWindowText(csAccount);
	if ( csAccount.IsEmpty() )
	{
		MessageBox(_T("账号不能为空！"), lpszPrompt, MB_ICONERROR);
		m_edtAccount.SetFocus();
		return;
	}

	if ( csAccount.GetLength() < MIN_ACCOUNT_LENGTH)
	{
		MessageBox(lpszAccountError, lpszPrompt, MB_ICONERROR);
		m_edtAccount.SetFocus();
		return;
	}

	CString csPwd;
	m_edtPwd.GetWindowText(csPwd);
	if ( csPwd.IsEmpty() )
	{
		MessageBox(_T("密码不能为空！"), lpszPrompt, MB_ICONERROR);
		m_edtPwd.SetFocus();
		return;
	}

	if ( csPwd.GetLength() < MIN_PWD_LENGTH )
	{
		MessageBox(lpszPasswordError, lpszPrompt, MB_ICONERROR);
		m_edtPwd.SetFocus();
		return;
	}

	LoginReq req;
	memset(&req, 0, sizeof(req));
	strcpy(req.szAccount, csAccount.GetBuffer(0));
	csAccount.ReleaseBuffer();

	MD5 md5(csPwd.GetBuffer(0));
	csPwd.ReleaseBuffer();
	strcpy(req.szPassword, md5.toString().c_str());

	req.nAccountType = ACCOUNT_TYPE_MANAGE;

	if( !CTCPDataService::GetInstance()->SendData(Cmd_MA_Login_Req, &req, sizeof(req), 0, false))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		return;
	}
	else
	{
		EnableControls(FALSE);
		m_dwStartLogonTime = GetTickCount();
		SetTimer(TIMER_LOGON_CHECK, 100, NULL);
	}

	//OnOK();
}

void CLogonDlg::OnTimer( UINT nIDEvent )
{
	if ( m_bLogon )
	{
		KillTimer(TIMER_LOGON_CHECK);
		OnOK();
	}
	else
	{
		DWORD dwTime = GetTickCount();
		if ( dwTime - m_dwStartLogonTime > OUT_TIME_SEC*1000)
		{
			m_dwStartLogonTime = 0;
			//MessageBox(csTemp);
			KillTimer(TIMER_LOGON_CHECK);
			EnableControls(TRUE);
			MessageBox(lpszLogonOutTime, lpszPrompt, MB_ICONERROR);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CLogonDlg::EnableControls( BOOL bFlag )
{
	m_edtAccount.EnableWindow(bFlag);
	m_edtPwd.EnableWindow(bFlag);
	m_chkRemember.EnableWindow(bFlag);
	m_btnLogon.EnableWindow(bFlag);
	m_btnCancel.EnableWindow(bFlag);
}

void CLogonDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(TIMER_LOGON_CHECK);
	CDialog::OnCancel();
}
