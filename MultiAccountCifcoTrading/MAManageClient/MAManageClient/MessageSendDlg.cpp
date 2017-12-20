// MessageSendDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "MessageSendDlg.h"
#include "TCPDataService.h"
#include "MsgDetailDlg.h"

// CMessageSendDlg dialog
const int nDelta = 200;

IMPLEMENT_DYNAMIC(CMessageSendDlg, CDialog)

CMessageSendDlg::CMessageSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageSendDlg::IDD, pParent)
{
	m_eType = MESSAGE_COMMON;
	m_nRootAccountID = -1;
	m_mapAccount.clear();
	m_mapAccountTree.clear();
	m_targetAccountSet.clear();
	m_setInitSelectAccount.clear();

	memset(&m_msgInfo, 0, sizeof(m_msgInfo));
	m_mapMsgInfo.clear();
}

CMessageSendDlg::~CMessageSendDlg()
{
}

void CMessageSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SHOW_USER_LIST, m_btnShowUserList);
	DDX_Control(pDX, IDC_CHECK1, m_checkBox);
	DDX_Control(pDX, IDC_TREE_USER, m_userTree);
	DDX_Control(pDX, IDC_BTN_SELECT_ALL, m_btnSelectAll);
	DDX_Control(pDX, IDC_BTN_UNSELECT_ALL, m_btnUnSelectAll);
	DDX_Control(pDX, IDC_BTN_ONLINE_USER, m_btnOnlineUser);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_edtTitle);
	DDX_Control(pDX, IDC_STATIC_CONTENT, m_staticContent);
	DDX_Control(pDX, IDC_EDIT_CONTENT, m_edtContent);
	DDX_Control(pDX, IDC_STATIC_EXPIRED, m_staticExpiredDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_MSG, m_dateCtrl);
	DDX_Control(pDX, IDC_BTN_SEND_MSG, m_btnSendMessage);
	DDX_Control(pDX, IDC_LIST_MSG, m_msgList);
	DDX_Control(pDX, IDC_LIST_USER, m_userList);
	DDX_Control(pDX, IDC_STATIC_GROUP, m_groupBox);
	DDX_Control(pDX, IDC_BTN_MSG_SEND_STATUS, m_btnMsgSendStatus);
}


BEGIN_MESSAGE_MAP(CMessageSendDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMessageSendDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMessageSendDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_SHOW_USER_LIST, &CMessageSendDlg::OnBnClickedBtnShowUserList)
	ON_BN_CLICKED(IDC_RADIO_COMMON, &CMessageSendDlg::OnBnClickedRadioCommon)
	ON_BN_CLICKED(IDC_RADIO_PERSONAL, &CMessageSendDlg::OnBnClickedRadioPersonal)
	ON_BN_CLICKED(IDC_CHECK1, &CMessageSendDlg::OnBnClickedCheck1)
	ON_MESSAGE(MSG_TARGETACCOUNT_CHANGE, &CMessageSendDlg::OnAccountListChangeMessage)
	ON_BN_CLICKED(IDC_BTN_SELECT_ALL, &CMessageSendDlg::OnBnClickedBtnSelectAll)
	ON_BN_CLICKED(IDC_BTN_UNSELECT_ALL, &CMessageSendDlg::OnBnClickedBtnUnselectAll)
	ON_BN_CLICKED(IDC_BTN_ONLINE_USER, &CMessageSendDlg::OnBnClickedBtnOnlineUser)
	ON_BN_CLICKED(IDC_BTN_SEND_MSG, &CMessageSendDlg::OnBnClickedBtnSendMsg)
	ON_MESSAGE(RECV_DATA_MSG, &CMessageSendDlg::OnRecvMessage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_MSG_SEND_STATUS, &CMessageSendDlg::OnBnClickedBtnMsgSendStatus)
END_MESSAGE_MAP()


// CMessageSendDlg message handlers

void CMessageSendDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CMessageSendDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CMessageSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edtTitle.SetLimitText(MAX_MSG_TITLE_LENGTH);
	m_edtContent.SetLimitText(MAX_MSG_CONTENT_LENGTH);

	CTime time = CTime::GetCurrentTime();
	m_dateCtrl.SetRange(&time, NULL);

	ShowSmallWindow();
	CenterWindow();

	DWORD dwStyle = m_userList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |=  LVS_EX_GRIDLINES;
	m_userList.SetExtendedStyle(dwStyle);
	m_userList.InsertColumn( 0, _T("账户号"), LVCFMT_LEFT, 90);
	m_userList.InsertColumn( 1, _T("用户名"), LVCFMT_LEFT, 90);

	dwStyle = m_msgList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |=  LVS_EX_GRIDLINES;
	m_msgList.SetExtendedStyle(dwStyle);
	m_msgList.InsertColumn( 0, _T("标题"), LVCFMT_LEFT, 90);
	m_msgList.InsertColumn( 1, _T("正文"), LVCFMT_LEFT, 90);
	m_msgList.InsertColumn( 2, _T("有效日期"), LVCFMT_LEFT, 90);

	if ( m_eType == MESSAGE_COMMON )
	{
		((CButton*)GetDlgItem(IDC_RADIO_COMMON))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_PERSONAL))->SetCheck(FALSE);

		m_btnOnlineUser.EnableWindow(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_COMMON))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_PERSONAL))->SetCheck(TRUE);

		m_btnOnlineUser.EnableWindow(TRUE);
	}

	LONG styles = ::GetWindowLong( m_userTree.m_hWnd, GWL_STYLE );
	styles = styles | TVS_CHECKBOXES | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	::SetWindowLong( m_userTree.m_hWnd, GWL_STYLE, styles );
	m_userTree.ModifyStyle( TVS_CHECKBOXES, 0 );
	m_userTree.ModifyStyle( 0, TVS_CHECKBOXES );

	m_userTree.SetMessageType(m_eType);
	m_userTree.SetAccountData(m_nRootAccountID, m_mapAccount, m_mapAccountTree);
	m_userTree.DrawData();

	m_userTree.SetInitSelectAccount(m_setInitSelectAccount);
	m_userTree.RefreshCheckStatus();

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_QryMessage_Req,
		&m_nRootAccountID, sizeof(m_nRootAccountID)))
	{
		MessageBox(lpszQryError, lpszPrompt, MB_ICONERROR);
	}

	return TRUE;
}

void CMessageSendDlg::OnBnClickedBtnShowUserList()
{
	// TODO: Add your control notification handler code here
	CString csText;
	m_btnShowUserList.GetWindowText(csText);
	if ( _T("<<显示用户列表") == csText )
	{
		ShowLargeWindow();
	}
	else
	{
		ShowSmallWindow();
	}
}

void CMessageSendDlg::ShowSmallWindow()
{
	m_userList.ShowWindow(SW_HIDE);
	m_btnShowUserList.SetWindowText(_T("<<显示用户列表"));

	MoveControls(&m_btnShowUserList);
	MoveControls(&m_groupBox);
	MoveControls(GetDlgItem(IDC_RADIO_COMMON));
	MoveControls(GetDlgItem(IDC_RADIO_PERSONAL));
	MoveControls(&m_checkBox);
	MoveControls(&m_userTree);
	MoveControls(&m_btnSelectAll);
	MoveControls(&m_btnUnSelectAll);
	MoveControls(&m_btnOnlineUser);

	MoveControls(&m_staticTitle);
	MoveControls(&m_edtTitle);
	MoveControls(&m_staticContent);
	MoveControls(&m_edtContent);
	MoveControls(&m_staticExpiredDate);
	MoveControls(&m_dateCtrl);
	MoveControls(&m_btnSendMessage);
	MoveControls(&m_msgList);
	MoveControls(&m_btnMsgSendStatus);

	CRect rc;
	GetWindowRect(&rc);
	//ScreenToClient(rc);
	rc.left += nDelta;
	MoveWindow(rc);

	Invalidate(TRUE);
}

void CMessageSendDlg::ShowLargeWindow()
{
	m_userList.ShowWindow(SW_SHOW);
	m_btnShowUserList.SetWindowText(_T("隐藏用户列表>>"));

	MoveControls(&m_btnShowUserList, FALSE);
	MoveControls(&m_groupBox, FALSE);
	MoveControls(GetDlgItem(IDC_RADIO_COMMON), FALSE);
	MoveControls(GetDlgItem(IDC_RADIO_PERSONAL), FALSE);
	MoveControls(&m_checkBox, FALSE);
	MoveControls(&m_userTree, FALSE);
	MoveControls(&m_btnSelectAll, FALSE);
	MoveControls(&m_btnUnSelectAll, FALSE);
	MoveControls(&m_btnOnlineUser, FALSE);

	MoveControls(&m_staticTitle, FALSE);
	MoveControls(&m_edtTitle, FALSE);
	MoveControls(&m_staticContent, FALSE);
	MoveControls(&m_edtContent, FALSE);
	MoveControls(&m_staticExpiredDate, FALSE);
	MoveControls(&m_dateCtrl, FALSE);
	MoveControls(&m_btnSendMessage, FALSE);
	MoveControls(&m_msgList, FALSE);
	MoveControls(&m_btnMsgSendStatus, FALSE);

	CRect rc;
	GetWindowRect(&rc);
	//ScreenToClient(rc);
	rc.left -= nDelta;
	MoveWindow(rc);

	Invalidate(TRUE);
}

void CMessageSendDlg::MoveControls( CWnd* pWnd, BOOL bLeft )
{
	if ( NULL == pWnd )
	{
		return;
	}

	CRect rc;
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);
	if ( bLeft )
	{
		rc.left -= nDelta;
		rc.right -= nDelta;
	}
	else
	{
		rc.left += nDelta;
		rc.right += nDelta;
	}

	pWnd->MoveWindow(rc);
}

void CMessageSendDlg::SetAccountData( int nRootAccountID, 
									 std::map<int, AccountInfoEx> mapAccount, 
									 std::map<int, std::map<int, AccountInfoEx>> mapAccountTree )
{
	m_nRootAccountID = nRootAccountID;
	m_mapAccount = mapAccount;
	m_mapAccountTree = mapAccountTree;
}

void CMessageSendDlg::OnBnClickedRadioCommon()
{
	// TODO: Add your control notification handler code here
	if ( m_eType != MESSAGE_COMMON)
	{
		m_eType = MESSAGE_COMMON;
		m_userList.DeleteAllItems();
		m_targetAccountSet.clear();
		m_userTree.SetMessageType(MESSAGE_COMMON);
		m_userTree.DrawData();
	}

	m_btnOnlineUser.EnableWindow(FALSE);
}

void CMessageSendDlg::OnBnClickedRadioPersonal()
{
	// TODO: Add your control notification handler code here
	if ( m_eType != MESSAGE_PERSONAL)
	{
		m_eType = MESSAGE_PERSONAL;
		m_userList.DeleteAllItems();
		m_targetAccountSet.clear();
		m_userTree.SetMessageType(MESSAGE_PERSONAL);
		m_userTree.DrawData();
	}

	m_btnOnlineUser.EnableWindow(TRUE);
}

void CMessageSendDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	m_checkBox.GetCheck();
	m_userTree.SetSelectChildOnly( m_checkBox.GetCheck()==1 );
}

LRESULT CMessageSendDlg::OnAccountListChangeMessage( WPARAM wParam, LPARAM lParam )
{
	BOOL bChecked = (BOOL)wParam;
	int nAccountID = (int)lParam;

	if ( bChecked )
	{
		//增加账号
		if ( !IsExistInAccountList(nAccountID))
		{
			std::map<int, AccountInfoEx>::iterator it = m_mapAccount.find(nAccountID);
			if ( it != m_mapAccount.end() )
			{
				int nCount = m_userList.GetItemCount();
				m_userList.InsertItem(nCount, _T(""));
				m_userList.SetItemText(nCount, 0, it->second.accountInfo.szAccount);
				m_userList.SetItemText(nCount, 1, it->second.accountInfo.szName);
				m_userList.SetItemData(nCount, nAccountID);

				m_targetAccountSet.insert(nAccountID);
			}
		}
	}
	else
	{
		//删除账号
		std::set<int>::iterator it = m_targetAccountSet.find(nAccountID);
		if ( it != m_targetAccountSet.end() )
		{
			m_targetAccountSet.erase(it);

			int nCount = m_userList.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( m_userList.GetItemData(i) == nAccountID )
				{
					m_userList.DeleteItem(i);
					break;
				}
			}
		}
	}

	return 0;
}

bool CMessageSendDlg::IsExistInAccountList( int nAccountID )
{
	std::set<int>::iterator it = m_targetAccountSet.find(nAccountID);
	if ( it != m_targetAccountSet.end() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CMessageSendDlg::OnBnClickedBtnSelectAll()
{
	// TODO: Add your control notification handler code here
	m_userTree.SelectAll();
}

void CMessageSendDlg::OnBnClickedBtnUnselectAll()
{
	// TODO: Add your control notification handler code here
	m_targetAccountSet.clear();
	m_userList.DeleteAllItems();
	m_userTree.UnSelectAll();
}

void CMessageSendDlg::OnBnClickedBtnOnlineUser()
{
	// TODO: Add your control notification handler code here
	m_targetAccountSet.clear();
	m_userList.DeleteAllItems();

	std::set<int> setSelectAccount;
	std::map<int, AccountInfoEx>::iterator it = m_mapAccount.begin();
	for ( ; it != m_mapAccount.end(); it++ )
	{
		if ( it->second.accountInfo.nLogonStatus == 1 )
		{
			setSelectAccount.insert(it->first);
		}
	}

	m_userTree.SetInitSelectAccount(setSelectAccount);
	m_userTree.RefreshCheckStatus();
}

void CMessageSendDlg::SetMessageType( MessageType eType )
{
	m_eType = eType;
}

void CMessageSendDlg::SetSelectData( std::vector<int> vSelectAccount )
{
	for ( UINT i = 0; i < vSelectAccount.size(); i++ )
	{
		m_setInitSelectAccount.insert(vSelectAccount[i]);
	}
}

void CMessageSendDlg::OnBnClickedBtnSendMsg()
{
	// TODO: Add your control notification handler code here
	CString csTitle;
	m_edtTitle.GetWindowText(csTitle);
	if ( csTitle.IsEmpty() )
	{
		MessageBox(_T("标题不能为空！"), lpszPrompt, MB_ICONERROR);
		m_edtTitle.SetFocus();
		return;
	}

	CString csContent;
	m_edtContent.GetWindowText(csContent);
	if ( csContent.IsEmpty() )
	{
		MessageBox(_T("内容不能为空！"), lpszPrompt, MB_ICONERROR);
		m_edtContent.SetFocus();
		return;
	}

	if ( m_targetAccountSet.empty() )
	{
		MessageBox(_T("发送列表不能为空！"), lpszPrompt, MB_ICONERROR);
		return;
	}

	memset(&m_msgInfo, 0, sizeof(m_msgInfo));
	strcpy(m_msgInfo.szTitle, csTitle.GetBuffer(0));
	csTitle.ReleaseBuffer();
	strcpy(m_msgInfo.szContent, csContent.GetBuffer(0));
	csContent.ReleaseBuffer();
	m_msgInfo.nType = m_eType;
	m_msgInfo.nOwner = m_nRootAccountID;
	CTime time;
	m_dateCtrl.GetTime(time);
	sprintf(m_msgInfo.szDate, "%4d-%02d-%02d",
		time.GetYear(), time.GetMonth(), time.GetDay());

	int nBufferSize = sizeof(MessageInfo) + sizeof(TargetAccount)*m_targetAccountSet.size();
	char* pBuffer = new char[nBufferSize];
	if ( NULL == pBuffer )
	{
		return;
	}

	memcpy(pBuffer, &m_msgInfo, sizeof(m_msgInfo));
	std::set<int>::iterator it = m_targetAccountSet.begin();
	for ( ; it != m_targetAccountSet.end(); it++ )
	{
		TargetAccount account;
		memset(&account, 0, sizeof(account));
		account.nAccountID = *it;
		std::map<int, AccountInfoEx>::iterator it_account = m_mapAccount.find(*it);
		if ( it_account != m_mapAccount.end())
		{
			strcpy(account.szAccount, it_account->second.accountInfo.szAccount);
		}

		int nDis = std::distance(m_targetAccountSet.begin(), it);
		memcpy(pBuffer + sizeof(MessageInfo) + nDis*sizeof(TargetAccount),
			&account, sizeof(TargetAccount));
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_AddMessage_Req,
		pBuffer, nBufferSize))
	{
		MessageBox(lpszQryError, lpszPrompt, MB_ICONERROR);
	}
	else
	{
		m_btnSendMessage.EnableWindow(FALSE);
	}

	delete [] pBuffer;
	pBuffer = NULL;
}

LRESULT CMessageSendDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_AddMessage_Rsp )
	{
		m_btnSendMessage.EnableWindow(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_msgInfo.nMessageID = *(int*)pRecvData->pData;
			m_mapMsgInfo[m_msgInfo.nMessageID] = m_msgInfo;

			//在列表中添加一条数据
			m_msgList.InsertItem(0, _T(""));
			m_msgList.SetItemText(0, 0, m_msgInfo.szTitle);
			m_msgList.SetItemText(0, 1, m_msgInfo.szContent);
			m_msgList.SetItemText(0, 2, m_msgInfo.szDate);
			m_msgList.SetItemData(0, m_msgInfo.nMessageID);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryMessage_Rsp)
	{
		m_msgList.DeleteAllItems();
		m_mapMsgInfo.clear();

		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(MessageInfo);
			for ( int i = 0; i < nCount; i++ )
			{
				MessageInfo* pMsgInfo = 
					(MessageInfo*)((char*)pRecvData->pData + i*sizeof(MessageInfo));
				m_mapMsgInfo[pMsgInfo->nMessageID] = *pMsgInfo;
			}

			//在列表中显示数据
			std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.begin();
			for ( ; it != m_mapMsgInfo.end(); it++ )
			{
				m_msgList.InsertItem(0, _T(""));
				m_msgList.SetItemText(0, 0, it->second.szTitle);
				m_msgList.SetItemText(0, 1, it->second.szContent);
				m_msgList.SetItemText(0, 2, it->second.szDate);
				m_msgList.SetItemData(0, it->second.nMessageID);
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}

	return 0;
}

void CMessageSendDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CMessageSendDlg::OnBnClickedBtnMsgSendStatus()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_msgList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_msgList.GetNextSelectedItem(pos);
	int nMessageID = m_msgList.GetItemData(nSel);
	std::map<int, MessageInfo>::iterator it = m_mapMsgInfo.find(nMessageID);
	if ( it == m_mapMsgInfo.end())
	{
		return;
	}

	CMsgDetailDlg dlg;
	dlg.SetMessageInfo(it->second);
	dlg.DoModal();
}
