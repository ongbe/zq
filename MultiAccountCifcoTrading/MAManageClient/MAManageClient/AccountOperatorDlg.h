#pragma once
#include "afxwin.h"
#include <vector>
#include <map>
#include "MyEdit.h"
#include "MultiAccountStruct.h"
#include "afxdtctl.h"


// CAccountOperatorDlg dialog

class CAccountOperatorDlg : public CDialog
{
	DECLARE_DYNAMIC(CAccountOperatorDlg)

public:
	CAccountOperatorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAccountOperatorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ACCOUNT_OPERATOR };

	virtual BOOL OnInitDialog();

	void SetOperatorType( OperatorType eType);
	void SetAccountType(AccountType eType);
	void SetAccountInfo(const AccountInfo& accountInfo );
	const AccountInfo& GetAccountInfo();
	void SetPermissionData(std::vector<Permission>& vPermission, const PermissionData& permissionSetData );
	void SetIDCardInfoData(std::vector<IDCardInfo>& vIDCardInfo );

	bool IsPermissionChanged();
	std::map<int, Permission>& GetPermissions();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();

private:
	void SetModifyData();
	void GetDate(const CString& csDate, int& nYear, int& nMonth, int& nDay );

public:
	CMyEdit m_edtAccount;
	CMyEdit m_edtPwd;
	CMyEdit m_edtPwdConfirm;
	CComboBox m_cbxPermission;
	CComboBox m_cbxAccountStatus;
	CEdit m_edtUserName;
	CEdit m_edtContactInfo;
	CComboBox m_cbxIDCardType;
	CEdit m_edtIDCardNo;
	CButton m_btnPermissionSet;

private:
	std::vector<Permission> m_vPermission;
	std::vector<IDCardInfo> m_vIDCardInfo;
	OperatorType m_eType;
	AccountType m_eAccountType;
	AccountInfo m_curAccountInfo;
	PermissionData m_permissionSetData;
	bool m_bPermissionChanged;
	std::map<int, Permission> m_mapAllPermission;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonPermissionSet();
	CDateTimeCtrl m_dateCtrl;
};
