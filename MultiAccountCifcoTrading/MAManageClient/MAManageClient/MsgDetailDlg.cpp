// MsgDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "MsgDetailDlg.h"
#include "TCPDataService.h"


// CMsgDetailDlg dialog

IMPLEMENT_DYNAMIC(CMsgDetailDlg, CDialog)

CMsgDetailDlg::CMsgDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDetailDlg::IDD, pParent)
{
	memset(&m_msgInfo, 0, sizeof(m_msgInfo));
}

CMsgDetailDlg::~CMsgDetailDlg()
{
}

void CMsgDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG_DETAIL_TITLE, m_edtTitle);
	DDX_Control(pDX, IDC_EDIT_MSG_DETAIL_CONTENT, m_edtContent);
	DDX_Control(pDX, IDC_EDIT_MSG_SEND_LIST, m_edtSendList);
	DDX_Control(pDX, IDC_EDIT_MSG_WAIT_LIST, m_edtWaitForSendList);
	DDX_Control(pDX, IDC_STATIC_SEND_TOTAL, m_staticSendTotal);
	DDX_Control(pDX, IDC_STATIC_WAIT_TOTAL, m_staticWaitTotal);
}


BEGIN_MESSAGE_MAP(CMsgDetailDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMsgDetailDlg::OnBnClickedOk)
	ON_MESSAGE(RECV_DATA_MSG, &CMsgDetailDlg::OnRecvMessage)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMsgDetailDlg message handlers

void CMsgDetailDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

BOOL CMsgDetailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtTitle.SetLimitText(MAX_MSG_TITLE_LENGTH);
	m_edtContent.SetLimitText(MAX_MSG_CONTENT_LENGTH);

	m_edtTitle.SetWindowText(m_msgInfo.szTitle);
	m_edtContent.SetWindowText(m_msgInfo.szContent);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	if ( (!CTCPDataService::GetInstance()->SendData(Cmd_MA_QryTargetAccount_Req,
				&m_msgInfo.nMessageID, sizeof(m_msgInfo.nMessageID)))
		|| (!CTCPDataService::GetInstance()->SendData(Cmd_MA_QryWaitForSendAccount_Req,
		&m_msgInfo.nMessageID, sizeof(m_msgInfo.nMessageID))))
	{
		MessageBox(lpszQryError, lpszPrompt, MB_ICONERROR);
	}

	return TRUE;
}

void CMsgDetailDlg::SetMessageInfo( const MessageInfo& msgInfo )
{
	m_msgInfo = msgInfo;
}

LRESULT CMsgDetailDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_QryTargetAccount_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(TargetAccount);
			CString csSendList;
			for ( int i = 0; i < nCount; i++ )
			{
				TargetAccount account;
				memset(&account, 0, sizeof(account));
				memcpy(&account, 
					(char*)pRecvData->pData + i*sizeof(TargetAccount), sizeof(TargetAccount));

				if ( i != 0)
				{
					csSendList += _T(",");
				}

				csSendList += account.szAccount;
			}

			CString csTotal;
			if ( m_msgInfo.nType == MESSAGE_COMMON)
			{
				csTotal.Format(_T("管理账号 共%d人"), nCount);
			}
			else
			{
				csTotal.Format(_T("前台账号 共%d人"), nCount);
			}
			m_staticSendTotal.SetWindowText(csTotal);
			m_edtSendList.SetWindowText(csSendList);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryWaitForSendAccount_Rsp)
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(TargetAccount);
			CString csWaitForSendList;
			for ( int i = 0; i < nCount; i++ )
			{
				TargetAccount account;
				memset(&account, 0, sizeof(account));
				memcpy(&account, 
					(char*)pRecvData->pData + i*sizeof(TargetAccount), sizeof(TargetAccount));

				if ( i != 0)
				{
					csWaitForSendList += _T(",");
				}

				csWaitForSendList += account.szAccount;
			}

			CString csTotal;
			csTotal.Format(_T("前台账号 共%d人"), nCount);
			m_staticWaitTotal.SetWindowText(csTotal);
			m_edtWaitForSendList.SetWindowText(csWaitForSendList);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}

	return 0;
}

void CMsgDetailDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}
