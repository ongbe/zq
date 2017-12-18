#pragma once

#include "BKDialog.h"
#include "CustomTabCtrl.h"
#include "TabProbaChartDialog.h"
#include "TabTradeAnalyseDialog.h"
#include "ProfitChartDialog.h"

#define USER_MESEEAG
// CAnalyseModuleDialog dialog

class CAnalyseModuleDialog : public CBKDialog
{
	DECLARE_DYNAMIC(CAnalyseModuleDialog)

public:
	CAnalyseModuleDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnalyseModuleDialog();

// Dialog Data
	enum { IDD = IDD_ANALYSE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CCustomTabContainer     m_TabCtrl;
    CTabProbaChartDialog    *m_TabProbaChartDialog;
    CTabTradeAnalyseDialog  *m_TabTradeAnalyseDialog;
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT QuotUpdateMsgHandler(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnVolatilityUpdate(WPARAM,LPARAM);
    afx_msg LRESULT OnExpireDateUpdate(WPARAM wParam,LPARAM lParam);
private:
    // Ó¯¿÷Í¼´°¿Ú
    CProfitChartDialog m_ProfitChartDialog;
public:
    void UpdateLineChartCtrl();
    void UpdataColorMode(BK_COLOR_MODE eMode);
};
