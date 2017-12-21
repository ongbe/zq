// MonitorClientDlg.h : header file
//

#pragma once
#include "afxcmn.h"


// CMonitorClientDlg dialog
class CMonitorClientDlg : public CDialog
{
// Construction
public:
	CMonitorClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MONITORCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void Load(std::string& UDPSvrIP, unsigned short& UDPSvrPort, std::string& strTime);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CSortListCtrl m_ListInfo;
	CString m_staticUpRecv;
	CString m_staticUpSend;
	CString m_staticUpDelay;
	CString m_staticDownRecv;
	CString m_staticDownSend;
	CString m_staticDownDelay;
	CString m_staticCPU;
	CString m_staticMemory;
	CString m_staticHardDisk;


	afx_msg void OnStnClickedStaticMemory();
	LRESULT OnMessageGetData(WPARAM wP,LPARAM lP);
	LRESULT OnMessageGetStatic(WPARAM wP,LPARAM lP);
	LRESULT OnMessageGetStatic_Push(WPARAM wP,LPARAM lP);
	LRESULT OnFirstHeartBeat(WPARAM wP,LPARAM lP);

	void OnMessageGetStatic(std::vector<StatisticsData>& vecTimeLineData);
	void OnMessageGetStatic_Push(std::vector<StatisticsData>& vecTimeLineData);
	int OnMessageGetData(RecvData* pData);

	std::map<int, int> m_mapSeq2Index;//seq 到列表的refdata值
	std::map<int, ServerOrder> m_mapSeq2ServerOrder;//seq

	CStatisticsLine  m_UpDownLine;
	CStatisticsLine  m_DelayLine;

	UINT m_uTimer;
	bool m_bFirstAsk;

	std::map<long, StatisticsData> m_mapTime2Statistics;

	void SetLineView(std::map<long, StatisticsData>& mapTime2Statistics);


	CToolThread*	m_hWorkerThread;
	static DWORD WINAPI ThreadFunc(void* pData);
	void Process();
};
