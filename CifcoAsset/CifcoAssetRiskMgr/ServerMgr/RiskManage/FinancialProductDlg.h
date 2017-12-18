#pragma once
#include "afxcmn.h"
#include "CGridListCtrlEx.h"
#include "RiskManageStruct.h"
#include "afxwin.h"
#include <set>
#include <map>

// CFinancialProductDlg dialog

class CFinancialProductDlg : public CDialog
{
	DECLARE_DYNAMIC(CFinancialProductDlg)

public:
	CFinancialProductDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFinancialProductDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FIN_PRODUCT };

	virtual BOOL OnInitDialog();

	std::map<int, FinancialProduct> GetFinProduct();
	std::map<int, std::map<int, FinancialProduct>> GetOrgFinProductTree();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonProAdd();
	afx_msg void OnBnClickedButtonProDel();
	afx_msg void OnBnClickedFinProSave();
	afx_msg void OnDestroy();

private:
	void EnableControls(BOOL bFlag);
	void ShowTreeItems();
	void AddOrgItems( int nUpperLevelOrgID, HTREEITEM hParent );
	CString ConventErrorMsg( const CString& csMsg );
	void ResetTreeItems();
	void CheckTreeItem(HTREEITEM hItem, BOOL bCheck);
	void CheckTreeItem(HTREEITEM hItem, std::set<int> setOrg);
	void SetTreeItemData(std::set<int> setOrg);
	void GetTreeItemData(std::set<int>& setOrg);
	void GetTreeItemCheckedData(HTREEITEM hItem, std::set<int>& setOrg);

public:
	CGridListCtrlEx m_listCtrl;
	CTreeCtrl m_treeCtrl;

private:
	std::map<int, AssetMgmtOrganization> m_mapAssetMgmtOrg;			//key为资管区域ID
	std::map<int, std::map<int, AssetMgmtOrganization>> m_mapAssetMgmtOrgTree;	//外层key为上一级资管区域ID，里层key为资管区域ID
	std::map<int, FinancialProduct> m_mapFinancialProduct;	//存储理财产品信息，key为理财产品ID
	std::map<int, std::set<int>> m_mapFinPro2Org;	//外层key为理财产品ID，set存储资管组织ID

	FinancialProduct m_curFinProduct;
	int m_nCurSelProID;
	std::set<int> m_curOrgSet;

public:
	CButton m_btnAdd;
	CButton m_btnDel;
	CButton m_btnSave;

	afx_msg void OnLvnItemchangedListProduct(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditListProduct(NMHDR *pNMHDR, LRESULT *pResult);
};
