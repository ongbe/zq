#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include <vector>
#include <map>
#include "MultiAccountStruct.h"
#include "afxcmn.h"

// CBatchAddAccountDlg dialog

class CBatchAddAccountDlg : public CDialog
{
	DECLARE_DYNAMIC(CBatchAddAccountDlg)

public:
	CBatchAddAccountDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBatchAddAccountDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BATCH_ADD_ACCOUNT };

	void SetAccountType(AccountType eType);
	void SetPermissions( const std::vector<Permission>& vPermission);
	void SetIDCardInfoData(std::vector<IDCardInfo>& vIDCardInfo );

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedOk();

	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	bool LoadImportFile(const CString& csFilePath);
	bool ParseLine( const CString& csLine, AccountInfo& accountInfo);
	CString CombineLine( const AccountInfo& accountInfo );
	bool IsValidString( const CString& cs);
	bool IsValidIDCardType( int nIDardType );
	bool WriteErrorFile();
	CString ConventErrorMsg( const CString& csMsg );

public:
	CEdit m_edtPath;
	CComboBox m_cmbPermission;
	CComboBox m_cmbAccountStatus;
	CDateTimeCtrl m_dateCtrl;
	CButton m_btnOK;
	CProgressCtrl m_progressCtrl;

private:
	AccountType m_eType;
	std::vector<Permission> m_vPermission;
	std::map<int, AccountInfo> m_mapAccountInfo;
	std::map<int, AccountInfo> m_tempAccountInfoMap;
	std::vector<IDCardInfo> m_vIDCardInfo;
	std::map<int, CString> m_mapErrorInfo;
	std::vector<int> m_vSuccessedID;
	
	CString m_csTitle;
public:
	afx_msg void OnDestroy();
};
