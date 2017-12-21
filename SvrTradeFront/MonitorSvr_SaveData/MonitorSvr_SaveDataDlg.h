// MonitorSvr_SaveDataDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMonitorSvr_SaveDataDlg dialog
class CMonitorSvr_SaveDataDlg : public CDialog
{
// Construction
public:
	CMonitorSvr_SaveDataDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MONITORSVR_SAVEDATA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedOk();
	
	CString						m_strFullPath;
	DBAccess					m_dbAccess;


	bool	ReadFile(CString strFullPath, std::vector<ServerOrder>& vecServerOrder);
	bool    SaveData2DB(std::vector<ServerOrder>& vecServerOrder);

	CToolThread*		  m_WriteData2DBThread;
	static DWORD WINAPI ThreadWriteData2DB(PVOID pParam);
	void doProcessBusinessDataMain();
	bool m_bCancel;
	bool CreatTable();
	CProgressCtrl m_Progress;
};
