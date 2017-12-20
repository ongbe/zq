// ModifyPwdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "ModifyPwdDlg.h"
#include "TCPDataService.h"
#include "ConfigMgr.h"
#include "MD5/MD5.h"


// CModifyPwdDlg dialog

IMPLEMENT_DYNAMIC(CModifyPwdDlg, CDialog)

CModifyPwdDlg::CModifyPwdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyPwdDlg::IDD, pParent)
{
	m_bModified = false;
	m_dwStartTime = 0;
}

CModifyPwdDlg::~CModifyPwdDlg()
{
}

void CModifyPwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OLD_PWD, m_edtOld);
	DDX_Control(pDX, IDC_EDIT_NEW_PWD, m_edtNew);
	DDX_Control(pDX, IDC_EDIT_PWD_CONFIRM, m_edtConfirm);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CModifyPwdDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CModifyPwdDlg::OnBnClickedOk)
	ON_MESSAGE(RECV_DATA_MSG, &CModifyPwdDlg::OnRecvMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CModifyPwdDlg message handlers

void CModifyPwdDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(TIMER_MODIFY_PWD_CHECK);
	CDialog::OnCancel();
}

void CModifyPwdDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CModifyPwdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtOld.SetLimitText(MAX_PWD_LENGTH);
	m_edtNew.SetLimitText(MAX_PWD_LENGTH);
	m_edtConfirm.SetLimitText(MAX_PWD_LENGTH);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);
	return TRUE;
}

void CModifyPwdDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csOldPwd;
	m_edtOld.GetWindowText(csOldPwd);
	if ( csOldPwd.IsEmpty() )
	{
		MessageBox(_T("原密码不能为空！"), lpszPrompt, MB_ICONERROR);
		m_edtOld.SetFocus();
		return;
	}

	//if ( csOldPwd.GetLength() < MIN_PWD_LENGTH )
	//{
	//	MessageBox(lpszPasswordError, lpszPrompt, MB_ICONERROR);
	//	m_edtOld.SetWindowText("");
	//	m_edtOld.SetFocus();
	//	return;
	//}
	MD5 md5(csOldPwd.GetBuffer(0));
	csOldPwd.ReleaseBuffer();
	CString csMD5OldPwd = md5.toString().c_str();
	if ( csMD5OldPwd != ConfigManager::GetInstance()->GetPassword())
	{
		MessageBox(lpszOriPasswordError, lpszPrompt, MB_ICONERROR);
		m_edtOld.SetWindowText("");
		m_edtOld.SetFocus();
		return;
	}

	CString csNewPwd;
	m_edtNew.GetWindowText(csNewPwd);
	if ( csNewPwd.IsEmpty() )
	{
		MessageBox(_T("新密码不能为空！"), lpszPrompt, MB_ICONERROR);
		m_edtNew.SetFocus();
		return;
	}

	if ( csNewPwd.GetLength() < MIN_PWD_LENGTH )
	{
		MessageBox(lpszPasswordError, lpszPrompt, MB_ICONERROR);
		m_edtNew.SetWindowText("");
		m_edtNew.SetFocus();
		return;
	}

	CString csConfirmPwd;
	m_edtConfirm.GetWindowText(csConfirmPwd);
	if ( csConfirmPwd.IsEmpty() )
	{
		MessageBox(_T("请确认新密码！"), lpszPrompt, MB_ICONERROR);
		m_edtConfirm.SetFocus();
		return;
	}
	//if ( csConfirmPwd.GetLength() < MIN_PWD_LENGTH )
	//{
	//	MessageBox(lpszPasswordError, lpszPrompt, MB_ICONERROR);
	//	m_edtConfirm.SetWindowText("");
	//	m_edtConfirm.SetFocus();
	//	return;
	//}

	if ( csNewPwd != csConfirmPwd )
	{
		MessageBox(lpszPasswordNotSame, lpszPrompt, MB_ICONERROR);
		m_edtNew.SetWindowText("");
		m_edtConfirm.SetWindowText("");
		m_edtNew.SetFocus();
		return;
	}

	if ( csOldPwd == csNewPwd )
	{
		MessageBox(lpszSamePassword, lpszPrompt, MB_ICONERROR);
		m_edtNew.SetWindowText("");
		m_edtConfirm.SetWindowText("");
		m_edtNew.SetFocus();
		return;
	}

	ModifyPwd modifyPwd;
	memset(&modifyPwd, 0, sizeof(modifyPwd));
	modifyPwd.nAccountID = ConfigManager::GetInstance()->GetAccountID();

	md5.reset();
	md5.update(csNewPwd.GetBuffer(0));
	csNewPwd.ReleaseBuffer();
	strcpy(modifyPwd.szPassword, md5.toString().c_str());

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyPwd_Req, &modifyPwd, sizeof(modifyPwd)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		return;
	}
	else
	{
		EnableControls(FALSE);
		m_dwStartTime = GetTickCount();
		SetTimer(TIMER_MODIFY_PWD_CHECK, 100, NULL);
	}
	
	//OnOK();
}

void CModifyPwdDlg::EnableControls( BOOL bFlag )
{
	m_edtOld.EnableWindow(bFlag);
	m_edtNew.EnableWindow(bFlag);
	m_edtConfirm.EnableWindow(bFlag);
	m_btnOK.EnableWindow(bFlag);
	m_btnCancel.EnableWindow(bFlag);
}

LRESULT CModifyPwdDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_UnLogon_Rsp )
	{
		KillTimer(TIMER_MODIFY_PWD_CHECK);
		m_dwStartTime = 0;
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
		}
		else
		{
			//CString csMsg = (char*)(pRecvData->pData);
			//MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_ModifyPwd_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			CString csNewPwd;
			m_edtNew.GetWindowText(csNewPwd);
			MD5 md5(csNewPwd.GetBuffer(0));
			csNewPwd.ReleaseBuffer();
			CString csMD5NewPwd = md5.toString().c_str();
			ConfigManager::GetInstance()->SetPassword(csMD5NewPwd);
			MessageBox(lpszPasswordModified, lpszPrompt, MB_ICONINFORMATION);

			m_bModified = true;
		}
		else
		{
			KillTimer(TIMER_MODIFY_PWD_CHECK);
			EnableControls(TRUE);
			m_dwStartTime = 0;
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}

	return 0;
}

void CModifyPwdDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bModified )
	{
		KillTimer(TIMER_MODIFY_PWD_CHECK);
		OnOK();
	}
	else
	{
		DWORD dwTime = GetTickCount();
		if ( dwTime - m_dwStartTime > OUT_TIME_SEC*1000)
		{
			m_dwStartTime = 0;
			//MessageBox(csTemp);
			KillTimer(TIMER_MODIFY_PWD_CHECK);
			EnableControls(TRUE);
			MessageBox(lpszOperatorOutTime, lpszPrompt, MB_ICONERROR);
		}
	}

	CDialog::OnTimer(nIDEvent);
}
