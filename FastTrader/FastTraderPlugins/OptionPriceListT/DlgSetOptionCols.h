#pragma once
#include "afxcmn.h"


// CDlgSetOptionCols dialog

class CDlgSetOptionCols : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetOptionCols)

public:
	CDlgSetOptionCols(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetOptionCols();

// Dialog Data
	enum { IDD = IDD_DLG_SETOPTIONCOLS };

protected:

	void ShowCols();
	void SwapTwoRow(int nItem, int nItemTow);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDowm();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ocxListCols;
};
