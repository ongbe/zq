// AccountOperatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MAManageClient.h"
#include "AccountOperatorDlg.h"
#include "ConfigMgr.h"
#include "PermissionSetDlg.h"
#include "MD5/MD5.h"


// CAccountOperatorDlg dialog

IMPLEMENT_DYNAMIC(CAccountOperatorDlg, CDialog)

CAccountOperatorDlg::CAccountOperatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountOperatorDlg::IDD, pParent)
{
	m_vPermission.clear();
	m_vIDCardInfo.clear();
	m_eType = OPERATOR_ADD;
	m_eAccountType = ACCOUNT_TYPE_MANAGE;
	memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	memset(&m_permissionSetData, 0, sizeof(m_permissionSetData));
	m_bPermissionChanged = false;
	m_mapAllPermission.clear();
}

CAccountOperatorDlg::~CAccountOperatorDlg()
{
}

void CAccountOperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_edtAccount);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT_PWD, m_edtPwd);
	DDX_Control(pDX, IDC_EDITACCOUNT_PWD_CONFIRM, m_edtPwdConfirm);
	DDX_Control(pDX, IDC_COMBO_PERMISSION, m_cbxPermission);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT_STATUS, m_cbxAccountStatus);
	DDX_Control(pDX, IDC_EDIT_USER_NAME, m_edtUserName);
	DDX_Control(pDX, IDC_EDIT_CONTACTINFO, m_edtContactInfo);
	DDX_Control(pDX, IDC_COMBO_IDCARD_TYPE, m_cbxIDCardType);
	DDX_Control(pDX, IDC_EDIT_IDCARD_NO, m_edtIDCardNo);
	DDX_Control(pDX, IDC_BUTTON_PERMISSION_SET, m_btnPermissionSet);
	DDX_Control(pDX, IDC_DATE_VALID, m_dateCtrl);
}


BEGIN_MESSAGE_MAP(CAccountOperatorDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CAccountOperatorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_PERMISSION_SET, &CAccountOperatorDlg::OnBnClickedButtonPermissionSet)
END_MESSAGE_MAP()

BOOL CAccountOperatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cbxPermission.ResetContent();
	for ( UINT i = 0; i < m_vPermission.size(); i++ )
	{
		m_cbxPermission.AddString(m_vPermission[i].szName);
	}

	m_cbxIDCardType.ResetContent();
	for ( UINT i = 0; i < m_vIDCardInfo.size(); i++ )
	{
		m_cbxIDCardType.AddString(m_vIDCardInfo[i].szIDCardName);
	}
	m_cbxIDCardType.SetCurSel(0);

	m_cbxAccountStatus.AddString(lpszAccountStop);
	m_cbxAccountStatus.AddString(lpszAccoutActive);
	m_cbxAccountStatus.SetCurSel(1);

	m_edtAccount.SetLimitText(MAX_ACCOUNT_LENGTH);
	m_edtPwd.SetLimitText(MAX_PWD_LENGTH);
	m_edtPwdConfirm.SetLimitText(MAX_PWD_LENGTH);
	m_edtUserName.SetLimitText(MAX_USER_NAME_LENGTH);
	m_edtContactInfo.SetLimitText(MAX_CONTACTINFO_LENGTH);
	m_edtIDCardNo.SetLimitText(MAX_IDCARD_NO_LENGTH);

	((CButton*)GetDlgItem(IDC_RADIO_VALID_FOREVER))->SetCheck(TRUE);
	m_dateCtrl.SetFormat(_T("yyyy'-'MM'-'dd"));

	CTime time = CTime::GetCurrentTime();
	m_dateCtrl.SetRange(&time, NULL);
	time += CTimeSpan(365, 0, 0, 0);
	m_dateCtrl.SetTime(&time);

	CString csOperator;
	if ( m_eType == OPERATOR_MODIFY )
	{
		SetModifyData();
		csOperator = _T("修改");
	}
	else
	{
		csOperator = _T("新增");
		m_curAccountInfo.nViewPermission1 = 1;

	}

	CString csAccountType;
	if ( m_eAccountType == ACCOUNT_TYPE_MANAGE )
	{
		csAccountType = _T("管理");
	}
	else
	{
		csAccountType = _T("前台");
	}

	CString csTitle;
	csTitle.Format(_T("%s%s用户"), csOperator.GetBuffer(0), csAccountType.GetBuffer(0));
	csOperator.ReleaseBuffer();
	csAccountType.ReleaseBuffer();
	SetWindowText(csTitle);

	if ( m_permissionSetData.nCommonOperators == 0 )
	{
		m_btnPermissionSet.EnableWindow(FALSE);
	}
	else
	{
		m_btnPermissionSet.EnableWindow(TRUE);
	}

	return TRUE;
}


// CAccountOperatorDlg message handlers

void CAccountOperatorDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CAccountOperatorDlg::SetOperatorType( OperatorType eType )
{
	m_eType = eType;
}

void CAccountOperatorDlg::SetAccountType( AccountType eType )
{
	m_eAccountType = eType;
}

void CAccountOperatorDlg::SetAccountInfo( const AccountInfo& accountInfo )
{
	m_curAccountInfo = accountInfo;
}

const AccountInfo& CAccountOperatorDlg::GetAccountInfo()
{
	return m_curAccountInfo;
}

void CAccountOperatorDlg::SetModifyData()
{
	//带入数据
	m_edtAccount.SetWindowText(m_curAccountInfo.szAccount);
	m_edtPwd.SetWindowText(m_curAccountInfo.szPassword);
	m_edtPwdConfirm.SetWindowText(m_curAccountInfo.szPassword);
	//填入权限
	for ( UINT i = 0; i < m_vPermission.size(); i++ )
	{
		if ( m_curAccountInfo.nPermissionID == m_vPermission[i].nPermissionID)
		{
			m_cbxPermission.SetCurSel(i);
			break;
		}
	}

	//账号状态
	m_cbxAccountStatus.SetCurSel(m_curAccountInfo.nAccountStatus);

	//真实姓名
	m_edtUserName.SetWindowText(m_curAccountInfo.szName);
	//联系方式
	m_edtContactInfo.SetWindowText(m_curAccountInfo.szContactInfo);
	//填入证件类型
	for ( UINT i = 0; i < m_vIDCardInfo.size(); i++ )
	{
		if ( m_curAccountInfo.nIDCardType == m_vIDCardInfo[i].nIDCardType )
		{
			m_cbxIDCardType.SetCurSel(i);
			break;
		}
	}

	//证件号
	m_edtIDCardNo.SetWindowText(m_curAccountInfo.szIDCardNo);

	//有效期限
	if ( strcmp(m_curAccountInfo.szDate, VALID_FOREVER_DATE) != 0 )
	{
		int nYear, nMonth, nDay;
		nYear = nMonth = nDay = 0;
		GetDate(m_curAccountInfo.szDate, nYear, nMonth, nDay);
		
		CTime time(nYear, nMonth, nDay, 0, 0, 0);
		if ( time < CTime::GetCurrentTime())
		{
			m_dateCtrl.SetRange(&time, NULL);
		}
		
		m_dateCtrl.SetTime(&time);
		((CButton*)GetDlgItem(IDC_RADIO_USER_VALID_DATE))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_VALID_FOREVER))->SetCheck(FALSE);
	}
	m_edtAccount.EnableWindow(FALSE);
}

void CAccountOperatorDlg::OnBnClickedOk()
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

	if ( csAccount.GetLength() < MIN_ACCOUNT_LENGTH )
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

	CString csPwdConfirm;
	m_edtPwdConfirm.GetWindowText(csPwdConfirm);
	if ( csPwdConfirm.IsEmpty() )
	{
		MessageBox(_T("请确认新密码！"), lpszPrompt, MB_ICONERROR);
		m_edtPwdConfirm.SetFocus();
		return;
	}

	if ( csPwd != csPwdConfirm )
	{
		MessageBox(lpszPasswordNotSame, lpszPrompt, MB_ICONERROR);
		m_edtPwd.SetWindowText(_T(""));
		m_edtPwdConfirm.SetWindowText(_T(""));
		m_edtPwd.SetFocus();
		return;
	}

	if ( m_cbxPermission.GetCurSel() == -1 )
	{
		MessageBox(lpszEmptyPermission, lpszPrompt, MB_ICONERROR);
		m_cbxPermission.SetFocus();
		return;
	}

	if ( m_cbxAccountStatus.GetCurSel() == -1 )
	{
		MessageBox(lpszEmptyAccountStatus, lpszPrompt, MB_ICONERROR);
		m_cbxAccountStatus.SetFocus();
		return;
	}

	//memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	strcpy(m_curAccountInfo.szAccount,csAccount.GetBuffer(0));
	csAccount.ReleaseBuffer();

	if ( csPwd.GetLength() <= MAX_PWD_LENGTH )
	{
		MD5 md5(csPwd.GetBuffer(0));
		csPwd.ReleaseBuffer();
		strcpy(m_curAccountInfo.szPassword, md5.toString().c_str());
	}
	else
	{

	}

	int nPermissionSel = m_cbxPermission.GetCurSel();
	if( nPermissionSel >= 0 &&
		nPermissionSel < static_cast<int>(m_vPermission.size()))
	{
		m_curAccountInfo.nPermissionID = m_vPermission[nPermissionSel].nPermissionID;
	}
	
	m_curAccountInfo.nAccountStatus = m_cbxAccountStatus.GetCurSel();

	//用户资料部分
	CString csUserName;
	m_edtUserName.GetWindowText(csUserName);
	strcpy(m_curAccountInfo.szName, csUserName.GetBuffer(0));
	csUserName.ReleaseBuffer();
	CString csContactInfo;
	m_edtContactInfo.GetWindowText(csContactInfo);
	strcpy(m_curAccountInfo.szContactInfo, csContactInfo.GetBuffer(0));
	csContactInfo.ReleaseBuffer();
	int nIDCardTypeSel = m_cbxIDCardType.GetCurSel();
	if ( nIDCardTypeSel >= 0 &&
		nIDCardTypeSel < static_cast<int>(m_vIDCardInfo.size()))
	{
		m_curAccountInfo.nIDCardType = m_vIDCardInfo[nIDCardTypeSel].nIDCardType;
	}
	else
	{
		m_curAccountInfo.nIDCardType = 1;//默认使用第一项
	}

	if ( ((CButton*)GetDlgItem(IDC_RADIO_USER_VALID_DATE))->GetCheck())
	{
		CTime time;
		m_dateCtrl.GetTime(time);
		sprintf(m_curAccountInfo.szDate, "%4d-%02d-%02d",
			time.GetYear(), time.GetMonth(), time.GetDay());
	}
	else
	{
		strcpy(m_curAccountInfo.szDate, VALID_FOREVER_DATE);
	}

	CString csIDCardNo;
	m_edtIDCardNo.GetWindowText(csIDCardNo);
	strcpy(m_curAccountInfo.szIDCardNo, csIDCardNo.GetBuffer(0));
	csIDCardNo.ReleaseBuffer();

	m_curAccountInfo.nAccountType = m_eAccountType;
	m_curAccountInfo.nOwnerAccountID = ConfigManager::GetInstance()->GetAccountID();

	OnOK();
}

void CAccountOperatorDlg::SetPermissionData( std::vector<Permission>& vPermission,
											const PermissionData& permissionSetData )
{
	for ( UINT i = 0; i < vPermission.size(); i++ )
	{
		if ( vPermission[i].nPermissionType == m_eAccountType )
		{
			m_vPermission.push_back(vPermission[i]);
		}
	}

	m_permissionSetData = permissionSetData;
}

void CAccountOperatorDlg::SetIDCardInfoData( std::vector<IDCardInfo>& vIDCardInfo )
{
	m_vIDCardInfo = vIDCardInfo;
}

void CAccountOperatorDlg::OnBnClickedButtonPermissionSet()
{
	// TODO: Add your control notification handler code here
	CPermissionSetDlg dlg;
	dlg.SetPermissionData(m_permissionSetData);
	dlg.DoModal();

	m_bPermissionChanged = true;
	m_mapAllPermission = dlg.GetPermissions();
	std::map<int, Permission>::iterator it = m_mapAllPermission.begin();
	m_vPermission.clear();
	for ( ; it != m_mapAllPermission.end(); it++ )
	{
		if ( it->second.nPermissionType == m_eAccountType )
		{
			m_vPermission.push_back(it->second);
		}
	}

	m_cbxPermission.ResetContent();
	for ( UINT i = 0; i < m_vPermission.size(); i++ )
	{
		m_cbxPermission.AddString(m_vPermission[i].szName);
		if ( OPERATOR_MODIFY == m_eType 
			&& m_curAccountInfo.nPermissionID == m_vPermission[i].nPermissionID)
		{
			m_cbxPermission.SetCurSel(i);
		}
	}
}

bool CAccountOperatorDlg::IsPermissionChanged()
{
	return m_bPermissionChanged;
}

std::map<int, Permission>& CAccountOperatorDlg::GetPermissions()
{
	return m_mapAllPermission;
}

void CAccountOperatorDlg::GetDate( const CString& csDate, int& nYear, 
								  int& nMonth, int& nDay )
{
	int nPos = csDate.Find(_T("-"));
	if ( nPos == -1 )
	{
		return;
	}

	CString csYear = csDate.Left(nPos);
	nYear = atoi(csYear.GetBuffer(0));
	csYear.ReleaseBuffer();

	CString csTemp = csDate.Right(csDate.GetLength()-nPos-1);
	nPos = csTemp.Find(_T("-"));
	if ( nPos == -1 )
	{
		return;
	}

	CString csMonth = csTemp.Left(nPos);
	nMonth = atoi(csMonth.GetBuffer(0));
	csMonth.ReleaseBuffer();

	CString csDay = csTemp.Right(csTemp.GetLength()-nPos-1);
	nDay = atoi(csDay.GetBuffer(0));
	csDay.ReleaseBuffer();
}
