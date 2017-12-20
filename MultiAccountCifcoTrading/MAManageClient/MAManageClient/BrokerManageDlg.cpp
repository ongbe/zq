// BrokerManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "BrokerManageDlg.h"
#include "ConfigMgr.h"
#include "TCPDataService.h"
#include "BrokerOperatorDlg.h"

using namespace std;

// CBrokerManageDlg dialog

IMPLEMENT_DYNAMIC(CBrokerManageDlg, CDialog)

CBrokerManageDlg::CBrokerManageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrokerManageDlg::IDD, pParent)
{
	m_mapBrokerInfo.clear();
	memset(&m_curBrokerInfo, 0, sizeof(m_curBrokerInfo));
	m_bAddPermission = false;
	m_bModifyPermission = false;
	m_bDelPermission = false;
}

CBrokerManageDlg::~CBrokerManageDlg()
{
}

void CBrokerManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BROKER, m_listBroker);
	DDX_Control(pDX, IDC_BUTTON_BROKER_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_BROKER_MDF, m_btnModify);
	DDX_Control(pDX, IDC_BUTTON_BROKER_DEL, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_BROKER_REFRESH, m_BtnRefresh);
}


BEGIN_MESSAGE_MAP(CBrokerManageDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROKER_ADD, &CBrokerManageDlg::OnBnClickedButtonBrokerAdd)
	ON_BN_CLICKED(IDC_BUTTON_BROKER_MDF, &CBrokerManageDlg::OnBnClickedButtonBrokerMdf)
	ON_BN_CLICKED(IDC_BUTTON_BROKER_DEL, &CBrokerManageDlg::OnBnClickedButtonBrokerDel)
	ON_BN_CLICKED(IDC_BUTTON_BROKER_REFRESH, &CBrokerManageDlg::OnBnClickedButtonBrokerRefresh)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//ON_WM_TIMER()
	ON_MESSAGE(RECV_DATA_MSG, &CBrokerManageDlg::OnRecvMessage)
END_MESSAGE_MAP()

BOOL CBrokerManageDlg::PreTranslateMessage( MSG* pMsg )
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_ESCAPE == pMsg->wParam)
			return TRUE;
	}

	return CDialog::PreTranslateMessage( pMsg );
}

BOOL CBrokerManageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle = m_listBroker.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |=  LVS_EX_GRIDLINES;
	m_listBroker.SetExtendedStyle(dwStyle);

	m_listBroker.InsertColumn( 0, _T("经纪公司代码"), LVCFMT_LEFT, 200);
	m_listBroker.InsertColumn( 1, _T("经纪公司名称"), LVCFMT_LEFT, 200);
	m_listBroker.InsertColumn( 2, _T("是否向下跨级共享"), LVCFMT_CENTER, 200);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	//获取经纪公司列表
	EnableControls(TRUE);
	int nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_QryBroker_Req, &nOwnerAccountID, sizeof(int)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableControls(FALSE);
		//SetTimer(TIMER_BROKER_INFO_CHECK, OUT_TIME_SEC*1000, NULL);
	}

	return TRUE;
}

// CBrokerManageDlg message handlers

void CBrokerManageDlg::OnBnClickedButtonBrokerAdd()
{
	// TODO: Add your control notification handler code here
	CBrokerOperatorDlg dlg;
	dlg.SetOperatorType(OPERATOR_ADD);
	if ( IDOK == dlg.DoModal())
	{
		m_curBrokerInfo = dlg.GetBrokerInfo();
		if ( CTCPDataService::GetInstance()->SendData(
			Cmd_MA_AddBroker_Req, &m_curBrokerInfo, sizeof(m_curBrokerInfo)))
		{
			EnableControls(FALSE);
		}
		else
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
	}
}

void CBrokerManageDlg::OnBnClickedButtonBrokerMdf()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listBroker.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSelIndex = m_listBroker.GetNextSelectedItem(pos);
	memset(&m_curBrokerInfo, 0, sizeof(m_curBrokerInfo));
	m_curBrokerInfo.nBrokerID = m_listBroker.GetItemData(nSelIndex);
	CString csCode = m_listBroker.GetItemText(nSelIndex, 0);
	CString csCompany = m_listBroker.GetItemText(nSelIndex, 1);
	CString csShare = m_listBroker.GetItemText(nSelIndex, 2);
	strcpy(m_curBrokerInfo.szCode, csCode.GetBuffer(0));
	strcpy(m_curBrokerInfo.szCompany, csCompany.GetBuffer(0));
	if ( csShare == _T("是"))
	{
		m_curBrokerInfo.nShareWithLowerLever = 1;
	}
	else
	{
		m_curBrokerInfo.nShareWithLowerLever = 0;
	}

	csCode.ReleaseBuffer();
	csCompany.ReleaseBuffer();

	CBrokerOperatorDlg dlg;
	dlg.SetOperatorType(OPERATOR_MODIFY);
	dlg.SetBrokerInfo(m_curBrokerInfo);
	if ( IDOK == dlg.DoModal())
	{
		m_curBrokerInfo = dlg.GetBrokerInfo();
		if ( CTCPDataService::GetInstance()->SendData(
			Cmd_MA_ModifyBroker_Req, &m_curBrokerInfo, sizeof(m_curBrokerInfo)))
		{
			EnableControls(FALSE);
		}
		else
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}
	}
}

void CBrokerManageDlg::OnBnClickedButtonBrokerDel()
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listBroker.GetFirstSelectedItemPosition();
	if ( pos == NULL )
	{
		MessageBox(lpszSelectOneItem, lpszPrompt, MB_ICONERROR);
		return;
	}

	int nSelIndex = m_listBroker.GetNextSelectedItem(pos);
	if ( IDYES != MessageBox(lpszDelBrokerComfirm, lpszPrompt, MB_ICONQUESTION|MB_YESNO) )
	{
		return;
	}

	memset(&m_curBrokerInfo, 0, sizeof(m_curBrokerInfo));
	m_curBrokerInfo.nBrokerID = m_listBroker.GetItemData(nSelIndex);
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_DelBroker_Req, 
		&m_curBrokerInfo.nBrokerID, sizeof(m_curBrokerInfo.nBrokerID)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableControls(FALSE);
	}
}

void CBrokerManageDlg::OnBnClickedButtonBrokerRefresh()
{
	// TODO: Add your control notification handler code here
	//获取经纪公司列表
	int nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();
	if ( !CTCPDataService::GetInstance()->SendData(Cmd_MA_QryBroker_Req, &nOwnerAccountID, sizeof(int)))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}
	else
	{
		EnableControls(FALSE);
		//SetTimer(TIMER_BROKER_INFO_CHECK, OUT_TIME_SEC*1000, NULL);
	}
}

void CBrokerManageDlg::EnableControls( BOOL bFlag )
{
	m_btnAdd.EnableWindow(bFlag&&m_bAddPermission);
	m_btnModify.EnableWindow(bFlag&&m_bModifyPermission);
	m_btnDel.EnableWindow(bFlag&&m_bDelPermission);
	m_BtnRefresh.EnableWindow(bFlag);
}

void CBrokerManageDlg::OnDestroy()
{
	KillTimer(TIMER_BROKER_INFO_CHECK);
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

LRESULT CBrokerManageDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_UnLogon_Rsp )
	{
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
	else if ( pRecvData->head.cmdid == Cmd_MA_QryBroker_Rsp )
	{
		//KillTimer(TIMER_BROKER_INFO_CHECK);
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_mapBrokerInfo.clear();
			int nCount = pRecvData->head.len / sizeof(BrokerInfo);
			for ( int i = 0; i < nCount; i++ )
			{
				BrokerInfo* pBrokerInfo = (BrokerInfo*)((char*)pRecvData->pData + i*sizeof(BrokerInfo));
				m_mapBrokerInfo[pBrokerInfo->nBrokerID] = *pBrokerInfo;
			}

			//显示经纪公司信息
			ShowBrokerInfo();
		}
		else
		{
			CString csMsg((char*)(pRecvData->pData));
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_AddBroker_Rsp)
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			//显示经纪公司信息
			m_curBrokerInfo.nBrokerID = *(int*)pRecvData->pData;
			m_mapBrokerInfo[m_curBrokerInfo.nBrokerID] = m_curBrokerInfo;
			int nIndex = m_listBroker.GetItemCount();
			m_listBroker.InsertItem(nIndex, "");
			m_listBroker.SetItemText(nIndex, 0, m_curBrokerInfo.szCode);
			m_listBroker.SetItemText(nIndex, 1, m_curBrokerInfo.szCompany);
			if ( m_curBrokerInfo.nShareWithLowerLever == 0)
			{
				m_listBroker.SetItemText(nIndex, 2, "否");
			}
			else
			{
				m_listBroker.SetItemText(nIndex, 2, "是");
			}

			m_listBroker.SetItemData(nIndex, m_curBrokerInfo.nBrokerID);
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			csMsg = ConventErrorMsg(csMsg);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else if ( pRecvData->head.cmdid == Cmd_MA_ModifyBroker_Rsp)
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			//修改经纪公司信息
			std::map<int, BrokerInfo>::iterator it = m_mapBrokerInfo.find(m_curBrokerInfo.nBrokerID);
			if ( it != m_mapBrokerInfo.end())
			{
				it->second = m_curBrokerInfo;
			}

			int nCount = m_listBroker.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				int nBrokerID = m_listBroker.GetItemData(i);
				if ( nBrokerID == m_curBrokerInfo.nBrokerID )
				{
					m_listBroker.SetItemText(i, 1, m_curBrokerInfo.szCompany);
					if ( m_curBrokerInfo.nShareWithLowerLever == 0)
					{
						m_listBroker.SetItemText(i, 2, "否");
					}
					else
					{
						m_listBroker.SetItemText(i, 2, "是");
					}

					break;
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
	else if ( pRecvData->head.cmdid == Cmd_MA_DelBroker_Rsp)
	{
		EnableControls(TRUE);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			//删除经纪公司信息
			std::map<int, BrokerInfo>::iterator it = m_mapBrokerInfo.find(m_curBrokerInfo.nBrokerID);
			if ( it != m_mapBrokerInfo.end())
			{
				m_mapBrokerInfo.erase(it);
			}

			int nCount = m_listBroker.GetItemCount();
			for ( int i = 0; i < nCount; i++ )
			{
				int nBrokerID = m_listBroker.GetItemData(i);
				if ( nBrokerID == m_curBrokerInfo.nBrokerID )
				{
					m_listBroker.DeleteItem(i);
					break;
				}
			}
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			MessageBox(csMsg, lpszPrompt, MB_ICONERROR);
		}
	}
	else
	{

	}

	return 0;
}

void CBrokerManageDlg::ShowBrokerInfo()
{
	m_listBroker.DeleteAllItems();
	
	map<int, BrokerInfo>::iterator it = m_mapBrokerInfo.begin();
	int i = 0;
	for ( ; it != m_mapBrokerInfo.end(); ++it)
	{
		m_listBroker.InsertItem(i, "");
		m_listBroker.SetItemText(i, 0, it->second.szCode);
		m_listBroker.SetItemText(i, 1, it->second.szCompany);
		if ( it->second.nShareWithLowerLever == 0 )
		{
			m_listBroker.SetItemText(i, 2, "否");
		}
		else
		{
			m_listBroker.SetItemText(i, 2, "是");
		}

		m_listBroker.SetItemData(i, it->second.nBrokerID);

		i++;
	}
}

void CBrokerManageDlg::SetPermissionData( const PermissionData& data )
{
	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_ADD )
	{
		m_bAddPermission = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_MODIFY )
	{
		m_bModifyPermission = true;
	}

	if ( data.nCommonOperators & PERMISSION_BKG_COMMON_DEL )
	{
		m_bDelPermission = true;
	}
}

void CBrokerManageDlg::OnClose()
{
	KillTimer(TIMER_BROKER_INFO_CHECK);
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnCancel();
}

CString CBrokerManageDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("经纪公司代码重复！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}
