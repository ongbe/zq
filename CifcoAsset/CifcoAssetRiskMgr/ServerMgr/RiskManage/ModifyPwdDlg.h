#pragma once
#include "afxwin.h"
#include "MyEdit.h"


// CModifyPwdDlg dialog

class CModifyPwdDlg : public CDialog
{
	DECLARE_DYNAMIC(CModifyPwdDlg)

public:
	CModifyPwdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyPwdDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODIFY_PWD };
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void EnableControls(BOOL bFlag);

private:
	CMyEdit m_edtOld;
	CMyEdit m_edtNew;
	CMyEdit m_edtConfirm;
	CButton m_btnOK;
	CButton m_btnCancel;
	
private:
	DWORD m_dwStartTime;
	bool m_bModified;
};
