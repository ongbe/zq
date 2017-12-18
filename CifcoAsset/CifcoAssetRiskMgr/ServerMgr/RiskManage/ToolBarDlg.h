#pragma once
#include "afxwin.h"
#include "MyEdit.h"
#include <map>
#include "RiskManageStruct.h"
#include "afxdtctl.h"

// CToolBarDlg dialog

class CToolBarDlg : public CDialog
{
	DECLARE_DYNAMIC(CToolBarDlg)

public:
	CToolBarDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CToolBarDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TOOL_BAR };

	virtual BOOL OnInitDialog();

	void AddItem(int nAccountID, const CString& csAccount );
	void DelItem(int nAccountID);
	void SetData(std::map<int, RiskMgmtUserInfoEx> accountInfoMap );
	void GetSearchParam(CString& csAccount);
	void ClearItems();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMyEdit m_edtAccount;
	CComboBox m_cbxOwner;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSearch();
	CDateTimeCtrl m_dateCtrl;
	afx_msg void OnBnClickedCheckDate();
	CButton m_chkDate;
};
