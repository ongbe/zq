// restartDlg.h : header file
//

#pragma once


// CrestartDlg dialog
class CrestartDlg : public CDialog
{
// Construction
public:
	CrestartDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RESTART_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void GetPath(CString& strFilePath);
	void SetAutoStart();
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
