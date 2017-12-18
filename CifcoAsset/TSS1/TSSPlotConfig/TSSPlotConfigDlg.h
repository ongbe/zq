// TSSPlotConfigDlg.h : 头文件
//

#pragma once

#include "GridCtrl.h"
#include "afxwin.h"

// CTSSPlotConfigDlg 对话框
class CTSSPlotConfigDlg : public CDialog
{
// 构造
public:
	CTSSPlotConfigDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TSSPLOTCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	void ChangeUpPoint(int nPos);

// 实现
protected:
	HICON m_hIcon;
	CGridCtrl	m_gridPlotCtrl;
	CScrollBar m_ctlMyHScrollBar;
	double m_fMaxLose;
	UINT m_nForceCloseTimer;
	UINT m_nCancelForceTimer;
	int m_nUpTick;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedReadfile();
	afx_msg void OnBnClickedSavefile();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	unsigned int m_nLongColor;
	unsigned int m_nShortColor;
	unsigned int m_nNoneColor;
};
