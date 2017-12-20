#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <map>
#include <vector>
#include "MultiAccountStruct.h"
#include "SortListCtrl.h"


// AccountManageDlg dialog

class CAccountManageDlg : public CDialog
{
	DECLARE_DYNAMIC(CAccountManageDlg)

public:
	CAccountManageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAccountManageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ACCOUNT };

	virtual BOOL OnInitDialog();
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

	void GetSelectAccountID(std::vector<int>& vec);
	void PositionControls();

	void ClearData();
	void SetData( std::map<int, AccountInfoEx>& mapAccountInfoEx );

	void ReshowListData();

	void AddAccount( const AccountInfoEx& accountInfoEx);

	void UpdatePageBar();

	void ModifyAccount( const AccountInfoEx& accountInfoEx);
	void DelAccount( int nAccountID );

	void SetPermissionData(const PermissionData& data);

private:
	void SetRowData( int nCount, const AccountInfoEx& accountInfoEx );
	int GetTotalPage();
	void ShowListData();
	int FindAccount(int nAccountID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CImageList m_imageList;
	CSortListCtrl m_accountList;
	CStatic m_staticTotal;
	CStatic m_staticCurPage;
	CStatic m_staticJumpTo;
	CComboBox m_cbxPageList;
	CStatic m_staticHome;
	CStatic m_staticPre;
	CStatic m_staticNext;
	CStatic m_staticEnd;
	CStatic m_staticEveryPage;
	CEdit m_edtRow;
	CStatic m_staticRow;

private:
	bool m_bInited;
	std::vector<AccountInfoEx> m_vAccountInfoEx;
	int m_nCurShowPage;
	int m_nRowNumPerPage;
	bool m_bModifyMenuStatus;
	bool m_bDelMenuStatus;
	bool m_bStopMenuStatus;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboPageList();
	afx_msg void OnStnClickedStaticHome();
	afx_msg void OnStnClickedStaticPre();
	afx_msg void OnStnClickedStaticNext();
	afx_msg void OnStnClickedStaticEnd();
	afx_msg void OnEnKillfocusEditRow();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnKillfocusComboPageList();
	afx_msg void OnCbnSelendokComboPageList();
	afx_msg void OnLvnItemchangedListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	//ÓÒ¼ü²Ëµ¥ÏìÓ¦
	afx_msg void OnMenuModify();
	afx_msg void OnMenuDel();
	afx_msg void OnMenuStop();
	afx_msg void OnExportList();
	afx_msg void OnLvnColumnclickListBkgAccount(NMHDR *pNMHDR, LRESULT *pResult);
};
