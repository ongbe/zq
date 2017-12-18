#pragma once

#include "BKDialog.h"
#include "ListBoxEx.h"

#define StyleTile 0
#define StyleCenter 1
#define StyleStretch 2

// CTabTradeAnalyseDialog dialog

class CTabTradeAnalyseDialog : public CBKDialog
{
	DECLARE_DYNAMIC(CTabTradeAnalyseDialog)

public:
	CTabTradeAnalyseDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabTradeAnalyseDialog();

// Dialog Data
	enum { IDD = IDD_TRADEANALYSE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CListBoxEx m_listBoxEx;
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
    CBitmap m_bitmap;
    int m_style;

public:
    int SetBitmap(UINT nIDResource);
    void SetBitmapStyle(int style);
};
