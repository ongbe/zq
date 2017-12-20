// ServerManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "ServerManageDlg.h"
#include "TCPDataService.h"
#include "ConfigMgr.h"
#include "ServerGroupDlg.h"
#include "ServerAddrDlg.h"

// CServerManageDlg dialog

IMPLEMENT_DYNAMIC(CServerManageDlg, CDialog)

CServerManageDlg::CServerManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerManageDlg::IDD, pParent)
{
	m_vBrokerInfo.clear();
	m_mapServerGroup.clear();
	memset(&m_curServerGroup, 0, sizeof(m_curServerGroup));
	m_nBrokerID = -1;
	m_nCurAddrGroupID = -1;
	m_mapServerAddr.clear();
	memset(&m_curAddr, 0, sizeof(m_curAddr));
	m_bAddModifyPermission = false;
	m_bDelPermission = false;
}

CServerManageDlg::~CServerManageDlg()
{
}

void CServerManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BROKER, m_cmbBroker);
	DDX_Control(pDX, IDC_LIST_GROUP, m_groupList);
	DDX_Control(pDX, IDC_LIST_TRADE_ADDR, m_tradeAddrList);
	DDX_Control(pDX, IDC_LIST_QUOTE_ADDR, m_quoteAddrList);
	DDX_Control(pDX, IDC_BUTTON_GROUP_ADD, m_btnGroupAdd);
	DDX_Control(pDX, IDC_BUTTON_GROUP_DEL, m_btnGroupDel);
	DDX_Control(pDX, IDC_BUTTON_TRADE_ADDR_ADD, m_btnTradeAddrAdd);
	DDX_Control(pDX, IDC_BUTTON_TRADE_ADDR_DEL, m_btnTradeAddrDel);
	DDX_Control(pDX, IDC_BUTTON_TRADE_ADDR_TEST, m_btnTradeAddrModify);
	DDX_Control(pDX, IDC_BUTTON_QUOTE_ADDR_ADD, m_btnQuoteAddrAdd);
	DDX_Control(pDX, IDC_BUTTON_QUOTE_ADDR_DEL, m_btnQuoteAddrDel);
	DDX_Control(pDX, IDC_BUTTON_QUOTE_ADDR_TEST, m_btnQuoteAddrModify);
	DDX_Control(pDX, IDC_BUTTON_GROUP_MODIFY, m_btnGroupMdf);
}


BEGIN_MESSAGE_MAP(CServerManageDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_ADD, &CServerManageDlg::OnBnClickedButtonGroupAdd)
	ON_MESSAGE(RECV_DATA_MSG, &CServerManageDlg::OnRecvMessage)
	ON_CBN_SELCHANGE(IDC_COMBO_BROKER, &CServerManageDlg::OnCbnSelchangeComboBroker)
	ON_BN_CLICKED(IDC_BUTTON_TRADE_ADDR_DEL, &CServerManageDlg::OnBnClickedButtonTradeAddrDel)
	ON_BN_CLICKED(IDC_BUTTON_QUOTE_ADDR_ADD, &CServerManageDlg::OnBnClickedButtonQuoteAddrAdd)
	ON_BN_CLICKED(IDC_BUTTON_QUOTE_ADDR_DEL, &CServerManageDlg::OnBnClickedButtonQuoteAddrDel)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_MODIFY, &CServerManageDlg::OnBnClickedButtonGroupModify)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_DEL, &CServerManageDlg::OnBnClickedButtonGroupDel)
	ON_BN_CLICKED(IDC_BUTTON_TRADE_ADDR_ADD, &CServerManageDlg::OnBnClickedButtonTradeAddrAdd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_GROUP, &CServerManageDlg::OnLvnItemchangedListGroup)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GROUP, &CServerManageDlg::OnNMDblclkListGroup)
	ON_BN_CLICKED(IDC_BUTTON_TRADE_ADDR_MODIFY, &CServerManageDlg::OnBnClickedButtonTradeAddrModify)
	ON_BN_CLICKED(IDC_BUTTON_QUOTE_ADDR_MODIFY, &CServerManageDlg::OnBnClickedButtonQuoteAddrModify)
END_MESSAGE_MAP()

BOOL CServerManageDlg::PreTranslateMessage( MSG* pMsg )
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_ESCAPE == pMsg->wParam)
			return TRUE;
	}

	return CDialog::PreTranslateMessage( pMsg );
}


// CServerManageDlg message handlers

void CServerManageDlg::OnBnClickedButtonGroupAdd()
{
	// TODO: Add your control notification handler code here
	if ( m_nBrokerID == -1)
	{
		MessageBox(lpszSelectBrokerFirst, lpszPrompt, MB_ICONERROR);
		return;
	}

	CServerGroupDlg dlg;
	dlg.SetOperatorType(OPERATOR_ADD);
	if ( IDOK == dlg.DoModal())
	{
		m_curServerGroup = dlg.GetServerGroup();
		m_curServerGroup.nBrokerID = m_nBrokerID;
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_AddServerGroup_Req,
			&m_curServerGroup, sizeof(m_curServerGroup)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
		else
		{
			EnableAllControls(FALSE);
		}
	}
}

BOOL CServerManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControls();

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);
	//获取经纪公司列表
	int nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_QryBroker_Req, &nOwnerAccountID, sizeof(int)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}

	EnableAllControls(TRUE);

	return TRUE;
}

void CServerManageDlg::InitControls()
{
	DWORD dwStyle = m_groupList.GetExtendedStyle();	
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |=  LVS_EX_GRIDLINES;
	m_groupList.SetExtendedStyle(dwStyle);
	m_tradeAddrList.SetExtendedStyle(dwStyle);
	m_quoteAddrList.SetExtendedStyle(dwStyle);

	m_groupList.InsertColumn( 0, "服务器组", LVCFMT_LEFT, 135);

	m_tradeAddrList.InsertColumn( 0, "交易服务器地址", LVCFMT_LEFT, 230);
	m_tradeAddrList.InsertColumn( 1, "端口", LVCFMT_LEFT, 150);

	m_quoteAddrList.InsertColumn( 0, "行情服务器地址", LVCFMT_LEFT, 230);
	m_quoteAddrList.InsertColumn( 1, "端口", LVCFMT_LEFT, 150);
}

LRESULT CServerManageDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_UnLogon_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
		}
		else
		{
			//CString csMsg = (char*)(pRecvData->pData);
			//MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryBroker_Rsp )
	{
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_vBrokerInfo.clear();
			m_cmbBroker.ResetContent();

			m_mapServerGroup.clear();
			m_groupList.DeleteAllItems();
			m_nCurAddrGroupID = -1;

			m_tradeAddrList.DeleteAllItems();
			m_quoteAddrList.DeleteAllItems();
			m_mapServerAddr.clear();

			int nCount = pRecvData->head.len / sizeof(BrokerInfo);
			for ( int i = 0; i < nCount; i++ )
			{
				BrokerInfo* pBrokerInfo = (BrokerInfo*)((char*)pRecvData->pData + i*sizeof(BrokerInfo));
				m_vBrokerInfo.push_back(*pBrokerInfo);

				CString csTemp;
				csTemp.Format("%s(%s)", pBrokerInfo->szCompany, pBrokerInfo->szCode);
				m_cmbBroker.AddString(csTemp);
			}

			m_cmbBroker.SetCurSel( -1 );
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryServerGroup_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapServerGroup.clear();
			m_groupList.DeleteAllItems();
			m_nCurAddrGroupID = -1;

			m_tradeAddrList.DeleteAllItems();
			m_quoteAddrList.DeleteAllItems();
			m_mapServerAddr.clear();

			int nCount = pRecvData->head.len / sizeof(ServerGroup);
			for ( int i = 0; i < nCount; i++ )
			{
				ServerGroup* pServerGroup = (ServerGroup*)((char*)pRecvData->pData + i*sizeof(ServerGroup));
				m_mapServerGroup[pServerGroup->nServerNo] = *pServerGroup;

				m_groupList.InsertItem(i, "");
				m_groupList.SetItemText(i, 0, pServerGroup->szServerName);
				m_groupList.SetItemData(i, pServerGroup->nServerNo);
			}		
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_AddServerGroup_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curServerGroup.nServerNo = *(int*)pRecvData->pData;
			m_mapServerGroup[m_curServerGroup.nServerNo] = m_curServerGroup;
			int nCount = m_groupList.GetItemCount();
			m_groupList.InsertItem(nCount, "");
			m_groupList.SetItemText(nCount, 0, m_curServerGroup.szServerName);
			m_groupList.SetItemData(nCount, m_curServerGroup.nServerNo);		
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_ModifyServerGroup_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			//m_curServerGroup.nServerNo = *(int*)pRecvData->pData;
			m_mapServerGroup[m_curServerGroup.nServerNo] = m_curServerGroup;
			int nCount = m_groupList.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( m_curServerGroup.nServerNo == m_groupList.GetItemData(i))
				{
					m_groupList.SetItemText(i, 0, m_curServerGroup.szServerName);
				}
			}		
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_DelServerGroup_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nServerNo = *(int*)pRecvData->pData;
			std::map<int, ServerGroup>::iterator it = m_mapServerGroup.find(nServerNo);
			if ( it != m_mapServerGroup.end())
			{
				m_mapServerGroup.erase(it);
			}

			int nCount = m_groupList.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				if ( nServerNo == m_groupList.GetItemData(i))
				{
					m_groupList.DeleteItem(i);
					break;
				}
			}

			if ( nServerNo == m_nCurAddrGroupID )
			{
				m_mapServerAddr.clear();
				m_tradeAddrList.DeleteAllItems();
				m_quoteAddrList.DeleteAllItems();
				m_nCurAddrGroupID = -1;
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_QryServerAddr_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapServerAddr.clear();
			m_tradeAddrList.DeleteAllItems();
			m_quoteAddrList.DeleteAllItems();

			int nCount = pRecvData->head.len / sizeof(ServerAddr);
			for ( int i = 0; i < nCount; i++ )
			{
				ServerAddr* pServerAddr = (ServerAddr*)((char*)pRecvData->pData + i*sizeof(ServerAddr));
				m_mapServerAddr[pServerAddr->nAddrID] = *pServerAddr;

				//nServerType：0 交易 1 行情
				if ( pServerAddr->nServerType == SERVER_TYPE_TRADE )
				{
					int nCount = m_tradeAddrList.GetItemCount();
					m_tradeAddrList.InsertItem(nCount, "");
					m_tradeAddrList.SetItemText(nCount, 0, pServerAddr->szServerAddress);
					CString csPort;
					csPort.Format("%d", pServerAddr->nPort);
					m_tradeAddrList.SetItemText(nCount, 1, csPort);
					m_tradeAddrList.SetItemData(nCount, pServerAddr->nAddrID);
				}
				else
				{
					int nCount = m_quoteAddrList.GetItemCount();
					m_quoteAddrList.InsertItem(nCount, "");
					m_quoteAddrList.SetItemText(nCount, 0, pServerAddr->szServerAddress);
					CString csPort;
					csPort.Format("%d", pServerAddr->nPort);
					m_quoteAddrList.SetItemText(nCount, 1, csPort);
					m_quoteAddrList.SetItemData(nCount, pServerAddr->nAddrID);
				}
			}		
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_AddServerAddr_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_curAddr.nAddrID = *(int*)pRecvData->pData;
			m_mapServerAddr[m_curAddr.nAddrID] = m_curAddr;
			if ( m_curAddr.nServerType == SERVER_TYPE_TRADE )
			{
				int nCount = m_tradeAddrList.GetItemCount();
				m_tradeAddrList.InsertItem(nCount, "");
				m_tradeAddrList.SetItemText(nCount, 0, m_curAddr.szServerAddress);
				CString csPort;
				csPort.Format("%d", m_curAddr.nPort);
				m_tradeAddrList.SetItemText(nCount, 1, csPort);
				m_tradeAddrList.SetItemData(nCount, m_curAddr.nAddrID);	
			}
			else
			{
				int nCount = m_quoteAddrList.GetItemCount();
				m_quoteAddrList.InsertItem(nCount, "");
				m_quoteAddrList.SetItemText(nCount, 0, m_curAddr.szServerAddress);
				CString csPort;
				csPort.Format("%d", m_curAddr.nPort);
				m_quoteAddrList.SetItemText(nCount, 1, csPort);
				m_quoteAddrList.SetItemData(nCount, m_curAddr.nAddrID);	
			}	
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_ModifyServerAddr_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapServerAddr[m_curAddr.nAddrID] = m_curAddr;
			if ( m_curAddr.nServerType == SERVER_TYPE_TRADE)
			{
				int nCount = m_tradeAddrList.GetItemCount();
				for ( int i = 0; i < nCount; i++ )
				{
					if ( m_curAddr.nAddrID == m_tradeAddrList.GetItemData(i))
					{
						m_tradeAddrList.SetItemText(i, 0, m_curAddr.szServerAddress);
						CString csPort;
						csPort.Format("%d", m_curAddr.nPort);
						m_tradeAddrList.SetItemText(i, 1, csPort);
					}
				}	
			}
			else
			{
				int nCount = m_quoteAddrList.GetItemCount();
				for ( int i = 0; i < nCount; i++ )
				{
					if ( m_curAddr.nAddrID == m_quoteAddrList.GetItemData(i))
					{
						m_quoteAddrList.SetItemText(i, 0, m_curAddr.szServerAddress);
						CString csPort;
						csPort.Format("%d", m_curAddr.nPort);
						m_quoteAddrList.SetItemText(i, 1, csPort);
					}
				}
			}	
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_DelServerAddr_Rsp )
	{
		EnableAllControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nAddrID = *(int*)pRecvData->pData;
			int nServerType = SERVER_TYPE_TRADE;
			std::map<int, ServerAddr>::iterator it = m_mapServerAddr.find(nAddrID);
			if ( it != m_mapServerAddr.end())
			{
				nServerType = it->second.nServerType;
				m_mapServerAddr.erase(it);
			}

			if ( nServerType == SERVER_TYPE_TRADE)
			{
				int nCount = m_tradeAddrList.GetItemCount();
				for ( int i = 0; i < nCount; i++ )
				{
					if ( nAddrID == m_tradeAddrList.GetItemData(i))
					{
						m_tradeAddrList.DeleteItem(i);
						break;
					}
				}
			}
			else
			{
				int nCount = m_quoteAddrList.GetItemCount();
				for ( int i = 0; i < nCount; i++ )
				{
					if ( nAddrID == m_quoteAddrList.GetItemData(i))
					{
						m_quoteAddrList.DeleteItem(i);
						break;
					}
				}
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

void CServerManageDlg::OnCbnSelchangeComboBroker()
{
	// TODO: Add your control notification handler code here
	int nSel = m_cmbBroker.GetCurSel();
	if ( nSel >= 0 && nSel < static_cast<int>(m_vBrokerInfo.size()))
	{
		m_nBrokerID = m_vBrokerInfo[nSel].nBrokerID;
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_QryServerGroup_Req,
			&m_nBrokerID, sizeof(int)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
		else
		{
			EnableAllControls(FALSE);
		}
	}
}

void CServerManageDlg::EnableAllControls( BOOL bFlag )
{
	m_btnGroupAdd.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_btnGroupMdf.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_btnGroupDel.EnableWindow(bFlag&&m_bDelPermission);

	m_btnTradeAddrAdd.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_btnTradeAddrModify.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_btnTradeAddrDel.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_btnQuoteAddrAdd.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_btnQuoteAddrDel.EnableWindow(bFlag&&m_bAddModifyPermission);
	m_btnQuoteAddrModify.EnableWindow(bFlag&&m_bAddModifyPermission);
}

void CServerManageDlg::OnBnClickedButtonTradeAddrDel()
{
	// TODO: Add your control notification handler code here
	memset(&m_curAddr, 0, sizeof(m_curAddr));
	POSITION pos = m_tradeAddrList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_tradeAddrList.GetNextSelectedItem(pos);

	if ( IDYES != MessageBox(lpszDelDataComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	m_curAddr.nAddrID = m_tradeAddrList.GetItemData(nSel);
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_DelServerAddr_Req,
		&m_curAddr.nAddrID, sizeof(m_curAddr.nAddrID)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableAllControls(FALSE);
	}
}

void CServerManageDlg::OnBnClickedButtonQuoteAddrAdd()
{
	// TODO: Add your control notification handler code here
	AddServerAddress( SERVER_TYPE_QUOTE );
}

void CServerManageDlg::OnBnClickedButtonQuoteAddrDel()
{
	// TODO: Add your control notification handler code here
	memset(&m_curAddr, 0, sizeof(m_curAddr));
	POSITION pos = m_quoteAddrList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_quoteAddrList.GetNextSelectedItem(pos);

	if ( IDYES != MessageBox(lpszDelDataComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	m_curAddr.nAddrID = m_quoteAddrList.GetItemData(nSel);
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_DelServerAddr_Req,
		&m_curAddr.nAddrID, sizeof(m_curAddr.nAddrID)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableAllControls(FALSE);
	}
}

void CServerManageDlg::OnBnClickedButtonGroupModify()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_groupList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectServerGroupFirst, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_groupList.GetNextSelectedItem(pos);

	int nGroupNo = m_groupList.GetItemData(nSel);
	std::map<int, ServerGroup>::iterator it = m_mapServerGroup.find(nGroupNo);
	if( it == m_mapServerGroup.end())
	{
		return;
	}

	CServerGroupDlg dlg;
	dlg.SetOperatorType(OPERATOR_MODIFY);
	dlg.SetServerGroup(it->second);
	if ( IDOK == dlg.DoModal())
	{
		m_curServerGroup = dlg.GetServerGroup();
		m_curServerGroup.nBrokerID = m_nBrokerID;
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyServerGroup_Req,
			&m_curServerGroup, sizeof(m_curServerGroup)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
		else
		{
			EnableAllControls(FALSE);
		}
	}
}

void CServerManageDlg::OnBnClickedButtonGroupDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_groupList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectServerGroupFirst, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_groupList.GetNextSelectedItem(pos);

	if ( IDYES != MessageBox(lpszDelDataComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	int nGroupNo = m_groupList.GetItemData(nSel);
	std::map<int, ServerGroup>::iterator it = m_mapServerGroup.find(nGroupNo);
	if( it == m_mapServerGroup.end())
	{
		m_groupList.DeleteItem(nSel);
		return;
	}

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_DelServerGroup_Req,
		&nGroupNo, sizeof(nGroupNo)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableAllControls(FALSE);
	}
}

void CServerManageDlg::EnableAddrControls( BOOL bFlag )
{
	m_btnTradeAddrAdd.EnableWindow(bFlag);
	m_btnTradeAddrDel.EnableWindow(bFlag);
	m_btnQuoteAddrAdd.EnableWindow(bFlag);
	m_btnQuoteAddrDel.EnableWindow(bFlag);
}

void CServerManageDlg::OnBnClickedButtonTradeAddrAdd()
{
	// TODO: Add your control notification handler code here
	AddServerAddress( SERVER_TYPE_TRADE );
}

void CServerManageDlg::AddServerAddress(ServerType eType)
{
	POSITION pos = m_groupList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectServerGroupFirst, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_groupList.GetNextSelectedItem(pos);

	CServerAddrDlg dlg;
	dlg.SetOperatorInfo(OPERATOR_ADD, eType);
	if ( IDOK == dlg.DoModal())
	{
		m_curAddr = dlg.GetServerAddr();
		m_curAddr.nServerNo = m_nCurAddrGroupID;
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_AddServerAddr_Req,
			&m_curAddr, sizeof(m_curAddr)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
		else
		{
			EnableAllControls(FALSE);
		}
	}
}

void CServerManageDlg::OnLvnItemchangedListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if ( pNMLV->uNewState & LVIS_SELECTED )
	{
		m_tradeAddrList.DeleteAllItems();
		m_quoteAddrList.DeleteAllItems();
		m_nCurAddrGroupID = m_groupList.GetItemData(pNMLV->iItem);
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_QryServerAddr_Req, 
			&m_nCurAddrGroupID, sizeof(m_nCurAddrGroupID)))
		{
		}
		else
		{
			EnableAllControls(FALSE);
		}
	}
}

void CServerManageDlg::ClearAllData()
{
	m_mapServerGroup.clear();
	m_groupList.DeleteAllItems();
	m_tradeAddrList.DeleteAllItems();
	m_quoteAddrList.DeleteAllItems();
	m_mapServerAddr.clear();
	m_nCurAddrGroupID = -1;
}

void CServerManageDlg::OnNMDblclkListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	POSITION pos = m_groupList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		//MessageBox(lpszSelectServerGroupFirst, lpszPrompt, MB_ICONERROR);
		return;
	}

	OnBnClickedButtonGroupModify();
}

void CServerManageDlg::OnBnClickedButtonTradeAddrModify()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_tradeAddrList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectServerAddrFirst, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_tradeAddrList.GetNextSelectedItem(pos);

	int nAddrNo = m_tradeAddrList.GetItemData(nSel);
	std::map<int, ServerAddr>::iterator it = m_mapServerAddr.find(nAddrNo);
	if( it == m_mapServerAddr.end())
	{
		return;
	}

	CServerAddrDlg dlg;
	dlg.SetOperatorInfo(OPERATOR_MODIFY, SERVER_TYPE_TRADE);
	dlg.SetServerAddr(it->second);
	if ( IDOK == dlg.DoModal())
	{
		m_curAddr = dlg.GetServerAddr();
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyServerAddr_Req,
			&m_curAddr, sizeof(m_curAddr)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
		else
		{
			EnableAllControls(FALSE);
		}
	}
}

void CServerManageDlg::OnBnClickedButtonQuoteAddrModify()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_quoteAddrList.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectServerAddrFirst, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSel = m_quoteAddrList.GetNextSelectedItem(pos);

	int nAddrNo = m_quoteAddrList.GetItemData(nSel);
	std::map<int, ServerAddr>::iterator it = m_mapServerAddr.find(nAddrNo);
	if( it == m_mapServerAddr.end())
	{
		return;
	}

	CServerAddrDlg dlg;
	dlg.SetOperatorInfo(OPERATOR_MODIFY, SERVER_TYPE_QUOTE);
	dlg.SetServerAddr(it->second);
	if ( IDOK == dlg.DoModal())
	{
		m_curAddr = dlg.GetServerAddr();
		if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_ModifyServerAddr_Req,
			&m_curAddr, sizeof(m_curAddr)))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
		else
		{
			EnableAllControls(FALSE);
		}
	}
}

void CServerManageDlg::SetPermissionData( const PermissionData& data )
{
	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_ADD_MODIFY )
	{
		m_bAddModifyPermission = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_DEL )
	{
		m_bDelPermission = true;
	}
}

CString CServerManageDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("服务器组重复！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}
