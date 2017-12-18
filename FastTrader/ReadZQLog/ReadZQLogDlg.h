// ReadZQLogDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include <map>

// CReadZQLogDlg 对话框
class CReadZQLogDlg : public CDialog
{
// 构造
public:
	CReadZQLogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_READZQLOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	BYTE ConvAscII(BYTE cAsc);
	BOOL OpenAndShowFile(LPCSTR pcsFName);


// 实现
protected:
	HICON m_hIcon;
	CRichEditCtrl m_richLog;
	std::map<BYTE, BYTE>	m_mapAscConv;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
public:
};
