// AccountOperatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RiskManage.h"
#include "ManageAccountOperatorDlg.h"
#include "ConfigMgr.h"
#include "TCPDataService.h"
#include "MD5/MD5.h"


// CAccountOperatorDlg dialog

IMPLEMENT_DYNAMIC(CManageAccountOperatorDlg, CDialog)

CManageAccountOperatorDlg::CManageAccountOperatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManageAccountOperatorDlg::IDD, pParent)
{
	m_mapPermissionName.clear();
	m_eType = OPERATOR_ADD;
	memset(&m_curUserInfo, 0, sizeof(m_curUserInfo));
}

CManageAccountOperatorDlg::~CManageAccountOperatorDlg()
{
}

void CManageAccountOperatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_edtAccount);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT_PWD, m_edtPwd);
	DDX_Control(pDX, IDC_EDITACCOUNT_PWD_CONFIRM, m_edtPwdConfirm);
	DDX_Control(pDX, IDC_COMBO_PERMISSION, m_cbxPermission);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT_STATUS, m_cbxAccountStatus);
	DDX_Control(pDX, IDC_EDIT_USER_NAME, m_edtUserName);
	DDX_Control(pDX, IDC_EDIT_CONTACTINFO, m_edtContactInfo);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT_ROLE, m_cbxRole);
	DDX_Control(pDX, IDC_COMBO_ACCOUNT_FIN, m_cbxFinancialProduct);
}


BEGIN_MESSAGE_MAP(CManageAccountOperatorDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CManageAccountOperatorDlg::OnBnClickedOk)
	ON_CBN_SELENDOK(IDC_COMBO_ACCOUNT_ORG, &CManageAccountOperatorDlg::OnCbnSelchangeComboAccountOrg)
	ON_MESSAGE(RECV_DATA_MSG, &CManageAccountOperatorDlg::OnRecvMessage)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCOUNT_ROLE, &CManageAccountOperatorDlg::OnCbnSelchangeComboAccountRole)
END_MESSAGE_MAP()

BOOL CManageAccountOperatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CTCPDataService::GetInstance()->RegisteHWnd(m_hWnd);

	//取消组合框风格
	SetWindowTheme( m_cbxPermission.m_hWnd, L"", L"");
	SetWindowTheme( m_cbxAccountStatus.m_hWnd, L"", L"");
	SetWindowTheme( m_cbxRole.m_hWnd, L"", L"");
	SetWindowTheme( m_cbxFinancialProduct.m_hWnd, L"", L"");

	m_cbxAccountStatus.AddString(lpszAccountStop);
	m_cbxAccountStatus.AddString(lpszAccoutActive);
	m_cbxAccountStatus.SetCurSel(1);

	m_edtAccount.SetLimitText(MAX_ACCOUNT_LENGTH);
	m_edtPwd.SetLimitText(MAX_PWD_LENGTH);
	m_edtPwdConfirm.SetLimitText(MAX_PWD_LENGTH);
	m_edtUserName.SetLimitText(MAX_USER_NAME_LENGTH);
	m_edtContactInfo.SetLimitText(MAX_CONTACTINFO_LENGTH);

	//角色
	m_cbxRole.AddString(_T("管理员"));
	m_cbxRole.AddString(_T("风控员"));
	m_cbxRole.SetCurSel(0);

	//填充所属资管
	int i = 0;
	m_comboTree.SubclassDlgItem (IDC_COMBO_ACCOUNT_ORG, this);
	m_comboTree.SetAssetMgmtOrg(m_mapAssetMgmtOrgTree);

	CString csTitle;
	if ( m_eType == OPERATOR_MODIFY )
	{
		SetModifyData();
		csTitle = _T("修改管理员/风控员账户");
	}
	else
	{
		csTitle = _T("新增管理员/风控员账户");
	}

	SetWindowText(csTitle);

	if ( !CTCPDataService::GetInstance()->SendData(Cmd_RM_QryPermissionNameEx_Req, NULL, 0))
	{
		std::string strError = CTCPDataService::GetInstance()->GetLastError();
		MessageBox(strError.c_str(), lpszPrompt, MB_ICONERROR);
	}

	return TRUE;
}


// CAccountOperatorDlg message handlers

void CManageAccountOperatorDlg::OnDestroy()
{
	CTCPDataService::GetInstance()->UnRegisteHWnd(m_hWnd);

	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CManageAccountOperatorDlg::SetOperatorType( OperatorType eType )
{
	m_eType = eType;
}

void CManageAccountOperatorDlg::SetUserInfo( const RiskMgmtUserInfo& userInfo )
{
	m_curUserInfo = userInfo;
}

const RiskMgmtUserInfo& CManageAccountOperatorDlg::GetUserInfo()
{
	return m_curUserInfo;
}

void CManageAccountOperatorDlg::SetModifyData()
{
	//带入数据
	m_edtAccount.SetWindowText(m_curUserInfo.szAccount);
	m_edtPwd.SetWindowText(m_curUserInfo.szPassword);
	m_edtPwdConfirm.SetWindowText(m_curUserInfo.szPassword);

	//角色
	if ( m_curUserInfo.nRoleType == ROLE_TYPE_MANAGE )
	{
		m_cbxRole.SetCurSel(0);
	}
	else
	{
		m_cbxRole.SetCurSel(1);
	}

	//所属资管
	m_comboTree.SelectItem(m_curUserInfo.nAssetMgmtOrgID);

	//填充理财产品列表
	FillFinancialProduct(m_curUserInfo.nAssetMgmtOrgID);
	for ( int i = 0; i < m_cbxFinancialProduct.GetCount(); i++ )
	{
		if ( m_curUserInfo.nFinancialProductID == m_cbxFinancialProduct.GetItemData(i))
		{
			m_cbxFinancialProduct.SetCurSel(i);
		}
	}

	//账号状态
	m_cbxAccountStatus.SetCurSel(m_curUserInfo.nStatus);

	//真实姓名
	m_edtUserName.SetWindowText(m_curUserInfo.szName);
	//联系方式
	m_edtContactInfo.SetWindowText(m_curUserInfo.szContactInfo);

	m_edtAccount.EnableWindow(FALSE);
}

void CManageAccountOperatorDlg::OnBnClickedOk()
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

	if ( m_comboTree.GetCurSelItem() == NULL )
	{
		MessageBox(lpszEmptyOrg, lpszPrompt, MB_ICONERROR);
		m_comboTree.SetFocus();
		return;
	}

	if ( m_cbxAccountStatus.GetCurSel() == -1 )
	{
		MessageBox(lpszEmptyAccountStatus, lpszPrompt, MB_ICONERROR);
		m_cbxAccountStatus.SetFocus();
		return;
	}

	//memset(&m_curAccountInfo, 0, sizeof(m_curAccountInfo));
	strcpy(m_curUserInfo.szAccount,csAccount.GetBuffer(0));
	csAccount.ReleaseBuffer();

	if ( csPwd.GetLength() <= MAX_PWD_LENGTH )
	{
		MD5 md5(csPwd.GetBuffer(0));
		csPwd.ReleaseBuffer();
		strcpy(m_curUserInfo.szPassword, md5.toString().c_str());
	}
	else
	{

	}

	m_curUserInfo.nRoleType = static_cast<RoleType>(m_cbxRole.GetCurSel());
	int nPermissionSel = m_cbxPermission.GetCurSel();
	m_curUserInfo.nPermissionID = m_cbxPermission.GetItemData(nPermissionSel);
	m_curUserInfo.nAssetMgmtOrgID = m_comboTree.GetSelectItemData();
	int nFinancialProduct = m_cbxFinancialProduct.GetCurSel();
	if ( nFinancialProduct != -1 )
	{
		m_curUserInfo.nFinancialProductID = m_cbxFinancialProduct.GetItemData(nFinancialProduct);
	}
	else
	{
		m_curUserInfo.nFinancialProductID = 0;
	}
	
	m_curUserInfo.nStatus = m_cbxAccountStatus.GetCurSel();

	//用户资料部分
	CString csUserName;
	m_edtUserName.GetWindowText(csUserName);
	strcpy(m_curUserInfo.szName, csUserName.GetBuffer(0));
	csUserName.ReleaseBuffer();
	CString csContactInfo;
	m_edtContactInfo.GetWindowText(csContactInfo);
	strcpy(m_curUserInfo.szContactInfo, csContactInfo.GetBuffer(0));
	csContactInfo.ReleaseBuffer();

	OnOK();
}

void CManageAccountOperatorDlg::SetAssetMgmtOrgTree( std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetMgmtOrgTree )
{
	m_mapAssetMgmtOrgTree = mapAssetMgmtOrgTree;
}

void CManageAccountOperatorDlg::SetFinancialProduct( std::map<int, std::map<int, FinancialProduct>>& mapFinancialProductTree )
{
	m_mapFinancialProductTree = mapFinancialProductTree;
}

void CManageAccountOperatorDlg::FillFinancialProduct( int nAssetMgmtOrgID )
{
	m_cbxFinancialProduct.ResetContent();
	std::map<int, std::map<int, FinancialProduct>>::iterator it =
		m_mapFinancialProductTree.find(nAssetMgmtOrgID);
	if ( it == m_mapFinancialProductTree.end())
	{
		return;
	}

	std::map<int, FinancialProduct>::iterator it_fin = it->second.begin();
	int i = 0;
	for ( ; it_fin != it->second.end(); ++it_fin )
	{
		m_cbxFinancialProduct.AddString(it_fin->second.szName);
		m_cbxFinancialProduct.SetItemData(i, it_fin->second.nFinancialProductID);
		i++;
	}
}

void CManageAccountOperatorDlg::OnCbnSelchangeComboAccountOrg()
{
	// TODO: Add your control notification handler code here
	FillFinancialProduct(m_comboTree.GetSelectItemData());
}

LRESULT CManageAccountOperatorDlg::OnRecvMessage( WPARAM wParam, LPARAM lParam )
{
	if ( 0 == lParam )
	{
		return -1;
	}

	RecvData* pRecvData = (RecvData*)lParam;
	if ( pRecvData->head.cmdid == Cmd_RM_QryPermissionNameEx_Rsp )
	{
		m_mapPermissionName.clear();
		if ( pRecvData->head.userdata1 == OPERATOR_SUCCESS )
		{
			int nCount = pRecvData->head.len / sizeof(PermissionNameEx);
			for ( int i = 0; i < nCount; i++ )
			{
				PermissionNameEx* pPermissionName = (PermissionNameEx*)((char*)pRecvData->pData + i*sizeof(PermissionNameEx));
				m_mapPermissionName[pPermissionName->nPermissionID] = *pPermissionName;
			}

			RoleType eRoleType = static_cast<RoleType>(m_cbxRole.GetCurSel());
			m_cbxPermission.ResetContent();
			int i = 0;
			std::map<int, PermissionNameEx>::iterator it_permission = m_mapPermissionName.begin();
			for ( ; it_permission != m_mapPermissionName.end(); ++it_permission )
			{
				if ( eRoleType == ROLE_TYPE_MANAGE )
				{
					if ( it_permission->second.nPermissionType == PERMISSION_MANAGE )
					{
						m_cbxPermission.AddString(it_permission->second.szName);
						m_cbxPermission.SetItemData(i, it_permission->first);
						i++;
					}
				}
				else
				{
					if ( it_permission->second.nPermissionType == PERMISSION_RISK_CONTROL )
					{
						m_cbxPermission.AddString(it_permission->second.szName);
						m_cbxPermission.SetItemData(i, it_permission->first);
						i++;
					}
				}
			}

			if ( m_eType == OPERATOR_MODIFY )
			{
				//填入权限
				for ( int i = 0; i < m_cbxPermission.GetCount(); i++ )
				{
					if ( m_curUserInfo.nPermissionID == m_cbxPermission.GetItemData(i))
					{
						m_cbxPermission.SetCurSel(i);
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

void CManageAccountOperatorDlg::OnCbnSelchangeComboAccountRole()
{
	// TODO: Add your control notification handler code here
	RoleType eRoleType = static_cast<RoleType>(m_cbxRole.GetCurSel());
	m_cbxPermission.ResetContent();
	int i = 0;
	std::map<int, PermissionNameEx>::iterator it_permission = m_mapPermissionName.begin();
	for ( ; it_permission != m_mapPermissionName.end(); ++it_permission )
	{
		if ( eRoleType == ROLE_TYPE_MANAGE )
		{
			if ( it_permission->second.nPermissionType == PERMISSION_MANAGE )
			{
				m_cbxPermission.AddString(it_permission->second.szName);
				m_cbxPermission.SetItemData(i, it_permission->first);
				i++;
			}
		}
		else
		{
			if ( it_permission->second.nPermissionType == PERMISSION_RISK_CONTROL )
			{
				m_cbxPermission.AddString(it_permission->second.szName);
				m_cbxPermission.SetItemData(i, it_permission->first);
				i++;
			}
		}
	}
}
