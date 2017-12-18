#pragma once

#include "ChartCtl.h"
#include "afxwin.h"

// CFeedbackDlg dialog

class CFeedbackDlg : public CDialog
{
	DECLARE_DYNAMIC(CFeedbackDlg)

public:
	CFeedbackDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFeedbackDlg();

// Dialog Data
	enum { IDD = IDD_FEEDBACK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
private:
    CStatic m_staticContract;
    CStatic m_staticDirection;
    CStatic m_staticCount;
    CStatic m_staticCountTotal;
    CStatic m_staticPriceLimit;
    CStatic m_staticAveragePirce;
    CStatic m_staticAveragePirce2;
    CStatic m_staticPrice1;
    CStatic m_staticPrice2;
    CStatic m_staticPrice3;
    CStatic m_staticPrice4;
    CStatic m_staticPrice5;
    CStatic m_staticVolume1;
    CStatic m_staticVolume2;
    CStatic m_staticVolume3;
    CStatic m_staticVolume4;
    CStatic m_staticVolume5;

public:
    CChartCtl m_customTime;
    CChartCtl m_customCount;  
    CWnd* m_pAlgoAutoOrderDlg; 

    //成交回报后处理
    void UpdataHistogram();
    void UpdateDepthMarketContent();
    void UpdataFeedbackData(BOOL bOrderRsp);   
    void UpdataWuDangData(BOOL bFlag);//False:优化参数True:正常
    void UpdataTitleData(void);

private:
    int     m_NewVolume;//市场现量累计
    double  m_MarketOrder;//市场成交量累计   
};
