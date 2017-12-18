// TestTPriceListDlg.h : 头文件
//

#pragma once


// CTestTPriceListDlg 对话框
class CTestTPriceListDlg : public CDialog
{
// 构造
public:
	CTestTPriceListDlg(CWnd* pParent = NULL);	// 标准构造函数
	BOOL CreateTPriceList(int nMode);

// 对话框数据
	enum { IDD = IDD_TESTTPRICELIST_DIALOG };
	enum {	conTimerCreateTPriceList = 123456 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	HWND	m_pPanel;
public:
};
