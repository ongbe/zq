// BatchAddAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "MAManageClient.h"
#include "BatchAddAccountDlg.h"
#include "MD5/MD5.h"
#include "ConfigMgr.h"
#include "TCPDataService.h"

// CBatchAddAccountDlg dialog

IMPLEMENT_DYNAMIC(CBatchAddAccountDlg, CDialog)

CBatchAddAccountDlg::CBatchAddAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBatchAddAccountDlg::IDD, pParent)
	, m_csTitle(_T("账户号,密码,用户名,联系方式,证件类型,证件号码"))
{
	m_vIDCardInfo.clear();
	m_vPermission.clear();
	m_mapAccountInfo.clear();
	m_mapErrorInfo.clear();
	m_vSuccessedID.clear();
	m_tempAccountInfoMap.clear();
}

CBatchAddAccountDlg::~CBatchAddAccountDlg()
{
}

void CBatchAddAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_edtPath);
	DDX_Control(pDX, IDC_COMBO_BATCH_PERMISSION, m_cmbPermission);
	DDX_Control(pDX, IDC_COMBO_BATCH_ACCOUNT_STATUS, m_cmbAccountStatus);
	DDX_Control(pDX, IDC_DATE_BATCH, m_dateCtrl);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
}


BEGIN_MESSAGE_MAP(CBatchAddAccountDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CBatchAddAccountDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDOK, &CBatchAddAccountDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(RECV_DATA_MSG, &CBatchAddAccountDlg::OnRecvMessage)
END_MESSAGE_MAP()


// CBatchAddAccountDlg message handlers

void CBatchAddAccountDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, NULL, _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"CSV files (*.csv)|*.csv");
	if( IDOK == dlg.DoModal())
	{
		m_edtPath.SetWindowText(dlg.GetPathName());
	} 
}

void CBatchAddAccountDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString csFilePath;
	m_edtPath.GetWindowText(csFilePath);
	if ( csFilePath.IsEmpty())
	{
		MessageBox(_T("请输入文件路径！"), lpszPrompt, MB_ICONERROR);
		m_edtPath.SetFocus();
		return;
	}

	csFilePath = csFilePath.Trim();
	CString csSuffix = csFilePath.Right(4);
	if ( csSuffix.MakeLower() != _T(".csv"))
	{
		MessageBox(_T("只能导入CSV文件！"), lpszPrompt, MB_ICONERROR);
		m_edtPath.SetFocus();
		return;
	}
	if ( -1 == _access(csFilePath, 0))
	{
		MessageBox(_T("文件路径不存在！"), lpszPrompt, MB_ICONERROR);
		m_edtPath.SetFocus();
		return;
	}

	if ( m_cmbPermission.GetCurSel() == -1 )
	{
		MessageBox(lpszEmptyPermission, lpszPrompt, MB_ICONERROR);
		m_cmbPermission.SetFocus();
		return;
	}

	int nPermissionID = 0;
	int nPermissionSel = m_cmbPermission.GetCurSel();
	if( nPermissionSel >= 0 &&
		nPermissionSel < static_cast<int>(m_vPermission.size()))
	{
		nPermissionID = m_vPermission[nPermissionSel].nPermissionID;
	}

	if ( m_cmbAccountStatus.GetCurSel() == -1 )
	{
		MessageBox(lpszEmptyAccountStatus, lpszPrompt, MB_ICONERROR);
		m_cmbAccountStatus.SetFocus();
		return;
	}

	CTime time;
	m_dateCtrl.GetTime(time);
	char szBuffer[20];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "%4d-%02d-%02d",
		time.GetYear(), time.GetMonth(), time.GetDay());

	int nAccountStatus = m_cmbAccountStatus.GetCurSel();
	int nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();

	if ( !LoadImportFile(csFilePath))
	{
		MessageBox(_T("无法打开文件，或文件内容格式错误！"), lpszPrompt, MB_ICONERROR);
		m_edtPath.SetFocus();
		return;
	}

	m_tempAccountInfoMap = m_mapAccountInfo;
	std::map<int, AccountInfo>::iterator it = m_tempAccountInfoMap.begin();
	for ( it; it != m_tempAccountInfoMap.end(); it++ )
	{
		it->second.nOwnerAccountID = nOwnerAccountID;
		it->second.nAccountType = m_eType;
		it->second.nViewPermission1 = 1;
		it->second.nPermissionID = nPermissionID;
		strcpy(it->second.szDate, szBuffer);

		MD5 md5(it->second.szPassword);
		strcpy(it->second.szPassword, md5.toString().c_str());
	}

	m_vSuccessedID.clear();
	m_mapErrorInfo.clear();
	std::map<int, AccountInfo>::iterator it_temp = m_tempAccountInfoMap.begin();
	if ( it_temp != m_tempAccountInfoMap.end() )
	{
		if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_AddAccount_Req,
			&it_temp->second, sizeof(it_temp->second), it_temp->first))
		{
			std::string strError = CTCPDataService::GetInstance()->GetLastError();
			MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
		}

		m_tempAccountInfoMap.erase(it_temp);

		//设置进度条
		m_progressCtrl.SetRange(0, m_tempAccountInfoMap.size());
		m_progressCtrl.SetStep(1);
		m_progressCtrl.ShowWindow(SW_SHOW);
		m_btnOK.EnableWindow(FALSE);
	}
	else
	{
		m_progressCtrl.ShowWindow(SW_HIDE);
		MessageBox(_T("没有可供导入的数据！"), lpszPrompt, MB_ICONERROR);
	}

	//OnOK();
}

void CBatchAddAccountDlg::SetAccountType( AccountType eType )
{
	m_eType = eType;
}

void CBatchAddAccountDlg::SetPermissions( const std::vector<Permission>& vPermission )
{
	for ( UINT i = 0; i < vPermission.size(); i++ )
	{
		if ( vPermission[i].nPermissionType == m_eType )
		{
			m_vPermission.push_back(vPermission[i]);
		}
	}
}

BOOL CBatchAddAccountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbPermission.ResetContent();
	for ( UINT i = 0; i < m_vPermission.size(); i++ )
	{
		m_cmbPermission.AddString(m_vPermission[i].szName);
	}
	m_cmbPermission.SetCurSel(0);

	m_cmbAccountStatus.AddString(lpszAccountStop);
	m_cmbAccountStatus.AddString(lpszAccoutActive);
	m_cmbAccountStatus.SetCurSel(1);

	m_dateCtrl.SetFormat(_T("yyyy'-'MM'-'dd"));
	CTime time = CTime::GetCurrentTime();
	m_dateCtrl.SetRange(&time, NULL);
	time += CTimeSpan(365, 0, 0, 0);
	m_dateCtrl.SetTime(&time);

	m_progressCtrl.ShowWindow(SW_HIDE);

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	return TRUE;
}

bool CBatchAddAccountDlg::LoadImportFile( const CString& csFilePath )
{
	m_mapAccountInfo.clear();

	CStdioFile file;
	if ( !file.Open(csFilePath, CFile::modeRead))
	{
		return false;
	}

	int i = 0;
	CString csLine;
	while ( file.ReadString(csLine) )
	{
		if ( i == 0 )
		{
			if ( csLine.Trim() != m_csTitle )
			{
				file.Close();
				return false;
			}
		}
		else
		{
			AccountInfo accountInfo;
			memset(&accountInfo, 0, sizeof(accountInfo));
			if ( !ParseLine(csLine, accountInfo))
			{
				file.Close();
				return false;
			}
			else
			{
				m_mapAccountInfo[i] = accountInfo;
			}
		}

		i++;
	}
	
	file.Close();
	return true;
}

bool CBatchAddAccountDlg::ParseLine( const CString& csLine, AccountInfo& accountInfo )
{
	CString csTemp = csLine;
	memset(&accountInfo, 0, sizeof(accountInfo));
	int nPos = csTemp.Find(',');
	//账户号
	if ( nPos == -1 )
	{
		return false;
	}

	CString csAccount = csTemp.Left(nPos);
	if ( csAccount.GetLength() < MIN_ACCOUNT_LENGTH ||
		csAccount.GetLength() > MAX_ACCOUNT_LENGTH ||
		!IsValidString(csAccount))
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//密码
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}

	CString csPwd = csTemp.Left(nPos);
	if ( csPwd.GetLength() < MIN_PWD_LENGTH ||
		csPwd.GetLength() > MAX_PWD_LENGTH ||
		!IsValidString(csPwd))
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//用户名
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}
	CString csUserName = csTemp.Left(nPos);
	if ( csUserName.GetLength() > MAX_ACCOUNT_LENGTH ||
		!IsValidString(csUserName))
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//联系方式
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}

	CString csContactInfo = csTemp.Left(nPos);
	if ( csContactInfo.GetLength() > MAX_CONTACTINFO_LENGTH )
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//证件类型
	nPos = csTemp.Find(',');
	if ( nPos == -1 )
	{
		return false;
	}

	CString csIDCCardType = csTemp.Left(nPos);
	int nIDCardType = atoi(csIDCCardType.GetBuffer(0));
	csIDCCardType.ReleaseBuffer();
	if ( !IsValidIDCardType(nIDCardType))
	{
		return false;
	}
	csTemp = csTemp.Right(csTemp.GetLength()-nPos-1);

	//证件号码
	nPos = csTemp.Find(',');
	if ( nPos != -1 )
	{
		return false;
	}

	CString csIDCardNo = csTemp;
	if ( csIDCardNo.GetLength() > MAX_IDCARD_NO_LENGTH )
	{
		return false;
	}

	strcpy(accountInfo.szAccount, csAccount.GetBuffer(0));
	csAccount.ReleaseBuffer();
	strcpy(accountInfo.szPassword, csPwd.GetBuffer(0));
	csPwd.ReleaseBuffer();
	strcpy(accountInfo.szName, csUserName.GetBuffer(0));
	csUserName.ReleaseBuffer();
	strcpy(accountInfo.szContactInfo, csContactInfo.GetBuffer(0));
	csContactInfo.ReleaseBuffer();
	accountInfo.nIDCardType = nIDCardType;
	strcpy(accountInfo.szIDCardNo, csIDCardNo.GetBuffer(0));
	csIDCardNo.ReleaseBuffer();
	
	return true;
}

bool CBatchAddAccountDlg::IsValidString( const CString& cs )
{
	if ( cs.IsEmpty() )
	{
		return true;
	}

	for ( int i = 0; i < cs.GetLength(); i++ )
	{
		UINT nChar = cs.GetAt(i);
		if ( !(
			( nChar >= _T( 'a' ) && nChar <= _T( 'z' ) ) ||
			( nChar >= _T( 'A' ) && nChar <= _T( 'Z' ) ) ||
			( nChar >= _T( '0' ) && nChar <= _T( '9' ) ))
			 )
		{
			return false;
		}
	}

	return true;
}

bool CBatchAddAccountDlg::IsValidIDCardType( int nIDardType )
{
	for ( UINT i = 0; i < m_vIDCardInfo.size(); i++ )
	{
		if ( nIDardType == m_vIDCardInfo[i].nIDCardType )
		{
			return true;
		}
	}

	return false;
}

void CBatchAddAccountDlg::SetIDCardInfoData( std::vector<IDCardInfo>& vIDCardInfo )
{
	m_vIDCardInfo = vIDCardInfo;
}

LRESULT CBatchAddAccountDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_MA_AddAccount_Rsp )
	{
		int nID = pRecvData->head.seq;
		m_progressCtrl.SetPos(nID);
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			m_vSuccessedID.push_back(nID);	
		}
		else
		{
			CString csMsg = (char*)(pRecvData->pData);
			m_mapErrorInfo[nID] = ConventErrorMsg(csMsg);
		}

		std::map<int, AccountInfo>::iterator it = m_tempAccountInfoMap.begin();
		if ( it != m_tempAccountInfoMap.end() )
		{
			if (!CTCPDataService::GetInstance()->SendData(Cmd_MA_AddAccount_Req,
				&it->second, sizeof(it->second), it->first))
			{
				std::string strError = CTCPDataService::GetInstance()->GetLastError();
				m_mapErrorInfo[it->first] = strError.c_str();
			}
			
			m_tempAccountInfoMap.erase(it);
		}
		else
		{
			//导入完成
			m_progressCtrl.ShowWindow(SW_HIDE);
			m_btnOK.EnableWindow(TRUE);

			WriteErrorFile();

			CString csMsg;
			csMsg.Format(_T("本次批量导入完成，成功%d条，失败%d条，是否查看失败文件？"),
				static_cast<int>(m_vSuccessedID.size()), 
				static_cast<int>(m_mapErrorInfo.size()));
			if ( IDYES == MessageBox(csMsg, lpszPrompt, MB_ICONQUESTION|MB_YESNO))
			{
				CString csSysPath = ConfigManager::GetInstance()->GetSystemPath();
				CString csErrorFile = csSysPath + _T("\\ImportError.csv");
				ShellExecute(NULL, _T("open"), csErrorFile, NULL,NULL,SW_SHOWNORMAL); 
			}
		}
	}
	else
	{

	}

	return 0;
}

void CBatchAddAccountDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}

bool CBatchAddAccountDlg::WriteErrorFile()
{
	CString csSysPath = ConfigManager::GetInstance()->GetSystemPath();
	CString csErrorFile = csSysPath + _T("\\ImportError.csv");
	CStdioFile file;
	if ( !file.Open(csErrorFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		return false;
	}

	file.WriteString(m_csTitle);
	file.WriteString(_T("\n"));
	std::map<int, CString>::iterator it = m_mapErrorInfo.begin();
	for ( ; it != m_mapErrorInfo.end(); it++ )
	{
		std::map<int, AccountInfo>::iterator it_account = m_mapAccountInfo.find(it->first);
		if ( it_account != m_mapAccountInfo.end() )
		{
			CString csLine = CombineLine(it_account->second);
			csLine += _T(",");
			csLine += it->second;
			file.WriteString(csLine);
			file.WriteString(_T("\n"));
		}
	}

	file.Close();
	return true;
}

CString CBatchAddAccountDlg::CombineLine( const AccountInfo& accountInfo )
{
	//账户号,密码,用户名,联系方式,证件类型,证件号码
	CString csLine;
	csLine.Format(_T("%s,%s,%s,%s,%d,%s"),
		accountInfo.szAccount, accountInfo.szPassword, accountInfo.szName,
		accountInfo.szContactInfo, accountInfo.nIDCardType, accountInfo.szIDCardNo);
	return csLine;
}

void CBatchAddAccountDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

CString CBatchAddAccountDlg::ConventErrorMsg( const CString& csMsg )
{
	CString csTempMsg;
	if ( -1 != csMsg.Find(_T("ORA-00001")))
	{
		csTempMsg = _T("账户号重复！");
	}
	else if ( -1 != csMsg.Find(_T("ORA-00291")) )
	{
		csTempMsg = _T("所选择的权限已被删除！");
	}
	else
	{
		csTempMsg = csMsg;
	}

	return csTempMsg;
}
