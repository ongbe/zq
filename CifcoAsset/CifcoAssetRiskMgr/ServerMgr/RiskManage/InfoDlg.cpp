// InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "InfoDlg.h"
#include "TCPDataService.h"
#include "ConfigMgr.h"


// CInfoDlg dialog

IMPLEMENT_DYNAMIC(CInfoDlg, CDialog)

CInfoDlg::CInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoDlg::IDD, pParent)
{

}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInfoDlg::OnBnClickedCancel)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_STATIC_INFO, &CInfoDlg::OnStnClickedStaticInfo)
END_MESSAGE_MAP()


// CInfoDlg message handlers

void CInfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CInfoDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
}

void CInfoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if ( !CTCPDataService::GetInstance()->IsConnected() )
	{
		m_staticInfo.SetWindowText( _T("网络连接中断，正在重连，请稍候...") );

		ShowWindow(SW_SHOW);
	}
	else
	{
		if ( !CTCPDataService::GetInstance()->IsLogon() )
		{
			ReLogon();
			m_staticInfo.SetWindowText( _T("正在登陆服务器，请稍候...") );

			ShowWindow(SW_SHOW);	
		}
		else
		{
			ShowWindow(SW_HIDE);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(TIMER_NET_AND_LOGON_CHECK, 500, NULL);

	ShowWindow(SW_HIDE);
	return TRUE;
}

void CInfoDlg::ReLogon()
{
	LoginReq req;
	memset(&req, 0, sizeof(req));
	CString csAccount = ConfigManager::GetInstance()->GetAccount();
	CString csPwd = ConfigManager::GetInstance()->GetPassword();
	strcpy(req.szAccount, csAccount.GetBuffer(0));
	strcpy(req.szPassword, csPwd.GetBuffer(0));
	csAccount.ReleaseBuffer();
	csPwd.ReleaseBuffer();
	req.nRoleType = ROLE_TYPE_MANAGE;

	CTCPDataService::GetInstance()->SendData(Cmd_RM_Login_Req, &req, sizeof(req), 0, false);
}
void CInfoDlg::OnStnClickedStaticInfo()
{
	// TODO: Add your control notification handler code here
}
