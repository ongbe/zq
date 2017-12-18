#pragma once
#include "BKDialog.h"
#include "LineChartCtrl.h"


// CTabProbaChartDialog dialog
class CProfitChartDialog;
class CTabProbaChartDialog : public CBKDialog
{
	DECLARE_DYNAMIC(CTabProbaChartDialog)

public:
	CTabProbaChartDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabProbaChartDialog();

// Dialog Data
	enum { IDD = IDD_PROBACHART_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    afx_msg LRESULT UpdateMsgHandler(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
    static CLineChartCtrl m_ProbaChartPlot;
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);

public:
    void Update();

    static void UpdateCross(bool bInRect, CPoint pt);

    CProfitChartDialog* m_pProfitChartDialog;
    void UpdataColorMode(BK_COLOR_MODE eMode);
};
