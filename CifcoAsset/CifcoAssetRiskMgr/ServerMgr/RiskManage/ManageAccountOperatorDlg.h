#pragma once
#include "afxwin.h"
#include <vector>
#include <map>
#include "MyEdit.h"
#include "RiskManageStruct.h"
#include "afxdtctl.h"
#include "AssetOrgComboTree.h"


// CAccountOperatorDlg dialog

class CManageAccountOperatorDlg : public CDialog
{
	DECLARE_DYNAMIC(CManageAccountOperatorDlg)

public:
	CManageAccountOperatorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManageAccountOperatorDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MANAGE_ACCOUNT_OPERATOR };

	virtual BOOL OnInitDialog();

	void SetOperatorType( OperatorType eType);
	void SetUserInfo(const RiskMgmtUserInfo& userInfo );
	const RiskMgmtUserInfo& GetUserInfo();
	void SetAssetMgmtOrgTree(std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetMgmtOrgTree);
	void SetFinancialProduct(std::map<int, std::map<int, FinancialProduct>>& mapFinancialProductTree);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();

private:
	void SetModifyData();

	void FillFinancialProduct( int nAssetMgmtOrgID );

public:
	CMyEdit m_edtAccount;
	CMyEdit m_edtPwd;
	CMyEdit m_edtPwdConfirm;
	CComboBox m_cbxPermission;
	CComboBox m_cbxAccountStatus;
	CEdit m_edtUserName;
	CEdit m_edtContactInfo;
	CComboBox m_cbxRole;
	//CComboBox m_cbxOrg;
	CComboBox m_cbxFinancialProduct;
	CAssetOrgComboTree m_comboTree;

private:
	std::map<int, PermissionNameEx> m_mapPermissionName;
	std::map<int, std::map<int, AssetMgmtOrganization>> m_mapAssetMgmtOrgTree;
	std::map<int, std::map<int, FinancialProduct>> m_mapFinancialProductTree;
	OperatorType m_eType;
	RiskMgmtUserInfo m_curUserInfo;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboAccountOrg();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeComboAccountRole();
};
