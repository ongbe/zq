#pragma once

#include "OptimizeSetupDlg.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "BookOrderOperations.h"
#include "AlgoAutoData.h"
#include "kernelstruct.h"
#include "afxdtctl.h"
#include "WBButton.h"
#include "ColorTextStatic.h" 
#include "NumericEdit.h"

// CGeneralParamDlg dialog

class CGeneralParamDlg : public CDialog //CBookOrderOperations//
{
	DECLARE_DYNAMIC(CGeneralParamDlg)

public:
	CGeneralParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGeneralParamDlg();

// Dialog Data
	enum { IDD = IDD_GENERALOPTIMIZE_DIALOG };

protected:
    virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnBnClickedBtnParamsetup();
    afx_msg void OnDeltaposSpinPricelimit(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnDeltaposSpinMarketdegreed(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnOk();
    afx_msg void OnPaint();
    afx_msg void OnCbnSelchangeComboOrdercount();
    afx_msg void OnEnChangeEditInstrumentid();        
    afx_msg void OnBnClickedBtnCancel();
    afx_msg void OnEnChangeEditMarketdegreed();
    afx_msg void OnBnClickedCheckMultiPrice();
    afx_msg void OnCbnCloseupComboBasicprice();
	DECLARE_MESSAGE_MAP()

private: 
    CWBButton m_btnPlay;
    CWBButton m_btnStop;
    CWBButton m_btnOptSetup;
    CEdit m_editInstrumentID;
    CButton m_checkActiveOptimize;
    CNumericEdit m_editInstrumentCount;
    CNumericEdit m_editPriceLimit;
    CSpinButtonCtrl m_spinPriceLimit;
	CDateTimeCtrl m_editInstrumentStartTime;
	CDateTimeCtrl m_editInstrumentEndTime;
    CSliderCtrl m_slidMarketDegreed;
    CEdit m_editMarketDegreed;
    CSpinButtonCtrl m_spinMarketDegreed;    
    CComboBox m_comboOrderCount;    
//     CComboBox m_comboResetOrderCount;
    CComboBox m_comboBasicPrice;    
//     CNumericEdit m_editSlidPriceSpread1;
    CNumericEdit m_editSlidPriceSpread2;
    CNumericEdit m_editSlidPriceSpread3;
    CNumericEdit m_editSlidPriceSpread4;
    CNumericEdit m_editSlidPriceSpread5;
//     CComboBox m_comboRelateOrderCount;
//     CNumericEdit m_editRelateOrderCount1;
    CNumericEdit m_editRelateOrderCount2;
    CNumericEdit m_editRelateOrderCount3;
    CNumericEdit m_editRelateOrderCount4;
    CNumericEdit m_editRelateOrderCount5;
    CColorTextStatic m_staticMust1;
    CColorTextStatic m_staticMust2;
    CColorTextStatic m_staticMust3;
    CColorTextStatic m_staticMust4;
    CColorTextStatic m_staticTitleSellPrice;
    CColorTextStatic m_staticTitleLatestPrice;
    CColorTextStatic m_staticTitleBuyPrice;
    CColorTextStatic m_staticTitleMiddlePrice;
    CColorTextStatic m_staticTitleYesterdayPrice;
    CColorTextStatic m_staticSellPrice;
    CColorTextStatic m_staticLatestPrice;
    CColorTextStatic m_staticBuyPrice;
    CColorTextStatic m_staticMiddlePrice;
    CColorTextStatic m_staticYesterdayPrice;    
    CComboBox m_comboActiveOptimize;
    CComboBox m_comboIdleTime;
    CComboBox m_comboExecuteTime;
    CComboBox m_comboOverTimeTotal;
    CNumericEdit m_editIdleTime;
    CNumericEdit m_editExecuteTime;
    CButton m_checkOverTimeRetryCount;
    CNumericEdit m_editOverTimeRetryCount;
    CButton m_checkOverTimeTotal;
    CNumericEdit m_editOverTimeTotal;
    CStatic m_staticSplit;
    CToolTipCtrl m_toolTip;

public:
    CNumericEdit m_editOrderCount;
//     CNumericEdit m_editResetOrderCount;
    CEdit m_editBasicPrice;    
    CComboBox m_comboBuysell;
    CComboBox m_comboOpenclose;   
    CButton m_btnCheckMultiPrice;
    CComboBox m_comboMultiPrice;
//     CComboBox m_comboSlidPriceSpread;

    void GetAlgoParamFromWnd(void);
    void SetAlgoParamToWnd(BOOL bEditChanged = FALSE);

    void SetInstrumentID(CString& strInstrumentID);
    CString GetInstrumentID();
    void UpdateDepthMarketContent(PlatformStru_DepthMarketData& depthMarketDate);
    void UpdataOptCombo(int optIndex,BOOL bHaveItem);
    void EnableCtrls(bool bEnable);
    
    CWnd *m_pAlgoAutoOrderDlg;
private:
    BOOL CheckParam(void);
    void AddToolTip();//添加提示功能
    void EnableMultiPriceControls(BOOL bEnable);
    
    int m_nCurVolume;//现交易量 当前的减去上一次的
    PlatformStru_DepthMarketData m_depthMarketDate;
    COptimizeSetupDlg m_OptimizeSetupDlg;
    bool m_bCloseupComboBasicprice;//是否选择完成
public:
    BOOL m_bOkClicked;
    afx_msg void OnCbnDropdownComboBasicprice();
    afx_msg void OnCbnSelchangeComboBasicprice();
};
