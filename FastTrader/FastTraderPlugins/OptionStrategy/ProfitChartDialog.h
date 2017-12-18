#pragma once
#include "BKDialog.h"
#include "LineChartCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


#define     TOP_HIGHT           25  //text on LineChartCtrl hight
#define     BOTTOM_HIGHT        55  //text under LineChartCtrl hight


// CProfitChartDialog dialog
class CTabProbaChartDialog;
class CProfitChartDialog : public CBKDialog
{
	DECLARE_DYNAMIC(CProfitChartDialog)

public:
	CProfitChartDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProfitChartDialog();

// Dialog Data
	enum { IDD = IDD_PROFIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg LRESULT UpdateMsgHandler(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
    // Ó¯¿÷Í¼
    static CLineChartCtrl m_ProfitChartPlot;
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
private:
    BOOL m_bInitDialog;
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //afx_msg void OnProcUpdateFromProbaDialog(WPARAM wParam,LPARAM lParam);
private:
    CComboBox m_comboStyle1Ctrl;
    CComboBox m_comboStyle2Ctrl;
    CEdit m_editPercentCtrl;
    CSpinButtonCtrl m_spinPercentCtrl;
    CDateTimeCtrl m_dataTimeCtrl;
    CEdit m_editRestCtrl;
    CSpinButtonCtrl m_spinRestCtrl;
public:
    void Update();   
    
    static void UpdateCross(bool bInRect, CPoint pt);

    CTabProbaChartDialog* m_pTabProbaChartDialog;
private:
    CString m_strVolatilityValue;//²¨¶¯ÂÊ
    CButton m_checkUpdateTime;
    CComboBox m_comboUpdateTime;
public:
    afx_msg void OnBnClickedCheckUpdatetime();
    void UpdateVolatility(CString sVolatility);
    void UpdateExpireDate(CTime tExpireDate,CString strRestDays);
    afx_msg void OnCbnSelchangeComboStyle2();
    afx_msg void OnDeltaposSpinPercent(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void UpdataColorMode(BK_COLOR_MODE eMode);
};
