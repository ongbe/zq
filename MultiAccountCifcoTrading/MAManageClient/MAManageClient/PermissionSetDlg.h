#pragma once
#include "afxcmn.h"
#include "ParameterTree.h"
#include "afxwin.h"

// CPermissionSetDlg dialog

class CPermissionSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CPermissionSetDlg)

public:
	CPermissionSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPermissionSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PERMISSION_MANAGE };

	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual BOOL OnInitDialog();
	void SetPermissionData( const PermissionData& data);
	std::map<int, Permission>& GetPermissions();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);

private:
	std::map<int, PermissionModule> m_mapModules;
	std::map<int, Permission> m_mapPermission;
	Permission m_curPermission;
	int m_nCurSelPermissionID;
	int m_nCbxBakSel;
	bool m_bAddModifyPermission;
	bool m_bDelPermission;

private:
	void EnableControls(BOOL bFlag);
	void ResetPermissionArea();
	void ShowPermissionData( const char* pData, int nDataLen );
	CString ConventErrorMsg( const CString& csMsg );

public:
	CImageList m_imgState;
	CParameterTree m_treePermission;
	CComboBox m_cbxPermissionType;
	CListCtrl m_listPermissionName;
	CButton m_btnAdd;
	CButton m_btnDel;
	CButton m_btnCommit;

	afx_msg void OnBnClickedButtonAddName();
	afx_msg void OnBnClickedButtonDelName();
	afx_msg void OnLvnEndlabeleditListPermissionName(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnLvnItemchangedListPermissionName(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCommit();
	afx_msg void OnCbnSelchangeComboPermissionType();
};
