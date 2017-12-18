#pragma once
#include "chartctl.h"

#include "TimeLineWin2.h"
#include "common-AlgoTradingNo1Dlg.h"


// CTabpage2_Chart dialog

class CTabpage2_Chart : public CDialog
{
	DECLARE_DYNAMIC(CTabpage2_Chart)

public:
	CTabpage2_Chart(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTabpage2_Chart();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// 放结果的柱状图
	CChartCtl m_ChartCtl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};
