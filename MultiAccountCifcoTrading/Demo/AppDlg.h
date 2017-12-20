// AppDlg.h : header file
//

#include "afxwin.h"
#if !defined(AFX_APPDLG_H__452BE6C1_144F_40E1_B88A_8D5A632E2188__INCLUDED_)
#define AFX_APPDLG_H__452BE6C1_144F_40E1_B88A_8D5A632E2188__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAppDlg dialog
class PlusinAPI;

class CAppDlg : public CDialog
{
// Construction
public:
	CAppDlg(CWnd* pParent = NULL, HWND hTargetWnd = NULL);	// standard constructor

	~CAppDlg();

// Dialog Data
	//{{AFX_DATA(CAppDlg)
	enum { IDD = IDD_APP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAppDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAppDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	static int OnRecvApiMsg(int nCmdId, int nErrorCode, void* pData, int nDataLen, int nRequestId );

	static void ProcessConsoleMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );
	static void ProcessExchangeMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );
	static void ProcessInstrumentMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );
	static void ProcessQuoteMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );
	static void ProcessAccountMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );
	static void ProcessAllOrderMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );
	static void ProcessPositionMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );
	static void ProcessOrderMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );
	static void ProcessConfigMsg( int nCmdId, int nErrorCode, void* pData, int nDataLen );

	int GetNextRequestId();

	PlusinAPI* m_pApi;
	HWND m_hTargetWnd;
	CListBox m_listBox;
	int m_nRequestId;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton27();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPDLG_H__452BE6C1_144F_40E1_B88A_8D5A632E2188__INCLUDED_)
