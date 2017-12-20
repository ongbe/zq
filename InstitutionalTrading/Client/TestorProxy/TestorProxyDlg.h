// TestorProxyDlg.h : 头文件
//

#pragma once
#include "FastTraderManager.h"
#include "GridCtrl\GridCtrl.h"
#include <map>

using namespace std;

// CTestorProxyDlg 对话框
class CTestorProxyDlg : public CDialog
{
public:
	enum {	conColUserSeqNum, conColUserName, conColRecvNum, 
			conColSendNum, conColMsgRecvNum, conColInsertNum, conColRejectNum };

	typedef struct {
		int nFolderNum;
		int nRecvNum;
		int nSendNum;
		int nMsgRecvNum;
		int nInsertNum;
		int nRejectNum;
	} ITEMNUM;

// 构造
public:
	CTestorProxyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTORPROXY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


protected:
	int FindRow(int nSeqNum);
	void UpdateGrid_ID(int nRow, int nSeqNum, CString& strAccount);
	void UpdateGrid_RecvNum(int nSeqNum, int nNum);
	void UpdateGrid_SendNum(int nSeqNum, int nNum);
	void UpdateGrid_MsgRecvNum(int nSeqNum, int nNum);
	void UpdateGrid_InsertNum(int nSeqNum, int nNum);
	void UpdateGrid_RejectNum(int nSeqNum, int nNum);

// 实现
protected:
	HICON m_hIcon;
	CFastTraderManager	m_fastTraderManager;
	CGridCtrl			m_gridStatus;
	map<int, ITEMNUM>	m_mapAccountSum;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};
