#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "RiskManageStruct.h"
#include <map>

// CAssetOrgManageDlg dialog

class CAssetOrgManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CAssetOrgManageDlg)

public:
	CAssetOrgManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAssetOrgManageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ORG_MANAGE };

	virtual BOOL OnInitDialog();

	std::map<int, AssetMgmtOrganization> GetAssetMgmtOrg();	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()

private:
	void ShowTreeItems();
	void AddOrgItems( int nUpperLevelOrgID, HTREEITEM hParent );
	void EnableControls( BOOL bFlag );
	CString ConventErrorMsg( const CString& csMsg );

public:
	CEdit m_edtOrgName;
	CEdit m_edtLongitude;
	CEdit m_edtLatitude;
	CString m_csOrgName;
	CString m_csLongitude;
	CString m_csLatitude;
	CTreeCtrl m_treeCtrl;

private:
	std::map<int, AssetMgmtOrganization> m_mapAssetMgmtOrg;			//key为资管区域ID
	std::map<int, std::map<int, AssetMgmtOrganization>> m_mapAssetMgmtOrgTree;	//外层key为上一级资管区域ID，里层key为资管区域ID
	HTREEITEM m_hSelItem;

	AssetMgmtOrganization m_curOrg;
	int m_nSequence;

public:
	afx_msg void OnTvnSelchangedTreeOrg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnAddOrg();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnDelOrg();
};
