#pragma once
#include "afxwin.h"


// CLogonDlg dialog

class CLogonDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogonDlg)

public:
	CLogonDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogonDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LOGON };

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckRemember();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer( UINT nIDEvent );
	afx_msg void OnClose();

private:
	void LogonSuccess();
	void EnableControls( BOOL bFlag);

private:
	CMyEdit m_edtAccount;
	CMyEdit m_edtPwd;
	CButton m_chkRemember;

	CButton m_btnLogon;
	CButton m_btnCancel;
	bool m_bLogon;
	DWORD m_dwStartLogonTime;
};
