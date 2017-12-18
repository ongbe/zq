// RestartClientDlg.h : header file
//

#pragma once


// CRestartClientDlg dialog
class CRestartClientDlg : public CDialog
{
// Construction
public:
	CRestartClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RestartClient_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void GetPath(CString& strFilePath);
	void SetAutoStart();
	int KillProcess(LPCSTR	pszWindowTitle);
	int KillProcessWithName(const wchar_t* _lpszFileName);
// Implementation
protected:
	HICON m_hIcon;
	UINT m_nTimer1;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	 void OnTimer(UINT nIDEvent);
};
