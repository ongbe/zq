// OptionQuationTestDlg.h : header file
//

#pragma once


// COptionQuationTestDlg dialog
class COptionQuationTestDlg : public CDialog
{
// Construction
public:
	COptionQuationTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OPTIONQUATIONTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	
	CDlgQuation m_DlgQuation;

	 bool static GetTime(SGetInfo& sGetInfo);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void OnDestroy();
	
public:
	afx_msg void OnBnClickedButtonLaunch();
	CString m_strInstrument;
	afx_msg void OnBnClickedOk();
};
