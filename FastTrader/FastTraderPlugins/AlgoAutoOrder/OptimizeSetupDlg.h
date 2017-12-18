#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "AlgoAutoData.h"
#include "WBButton.h"
#include "NumericEdit.h"

#define BkColor_Rule RGB(255,255,255)
#define BkColor_RuleDegreed RGB(0,0,0)
#define BkColor_Select RGB(174,168,217)

// COptimizeSetupDlg dialog

class COptimizeSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(COptimizeSetupDlg)

public:
	COptimizeSetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptimizeSetupDlg();

// Dialog Data
	enum { IDD = IDD_OPTIMIZESETUP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnInitDialog(); 

    afx_msg void OnBnClickedButtonNameadd();
    afx_msg void OnBnClickedButtonNamedelete();
    afx_msg void OnBnClickedButtonDetiladd();
    afx_msg void OnBnClickedButtonDetildelete();
    afx_msg void OnBnClickedButtonSavesetup();
    afx_msg void OnDeltaposSpinMarket(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnChangeEditMarket();
    afx_msg void OnNMCustomdrawSliderMarket(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
    afx_msg void OnPaint();
    afx_msg void OnNMClickListName(NMHDR *pNMHDR, LRESULT *pResult); 
    afx_msg void OnNMClickListDetil(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCbnSelchangeComboPrice();
    afx_msg void OnNMDblclkListName(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnEnKillfocusEditListname();
    afx_msg void OnBnClickedButtonClose();
	DECLARE_MESSAGE_MAP()

private:    
    CListCtrl m_listName;
    CListCtrl m_listDetil;
//     CComboBox m_comboSlidePrice;
    CWBButton m_btnNameAdd;
    CWBButton m_btnNameDel;
    CWBButton m_btnDetilAdd;
    CWBButton m_btnDetilDel;
    CWBButton m_btnSaveSetup;
    CComboBox m_comboTime;
    CComboBox m_comboPrice;    
//     CComboBox m_comboReCount;
    CEdit m_editPriceFrom;
    CEdit m_editPriceTo;
    CNumericEdit m_editMultiple;
    CNumericEdit m_editMarket; 
    CSliderCtrl m_sliderMarket;
    CSpinButtonCtrl m_spinMarket;
    CEdit m_editSlidePrice1;
    CEdit m_editSlidePrice2;
    CEdit m_editSlidePrice3;
    CEdit m_editSlidePrice4;
    CEdit m_editSlidePrice5;
    CEdit m_editReCount1;
    CEdit m_editReCount2;
    CEdit m_editReCount3;
    CEdit m_editReCount4;
    CEdit m_editReCount5;
    CStatic m_staticRule;
    CEdit m_editListName;
    CToolTipCtrl m_toolTip;

public:
    void SetSelectOptIndex(int optIndex); 
    void SetParentWnd(CWnd* pParentWnd);   

private:
    void AddToolTip();
    void InitCtrl(void);
    void EnableWindows(BOOL bEnable);
    void SetOptDataToWnd(BOOL bShow = TRUE);
    void UpdataReluRect(void);
    BOOL CheckParam(void);
    BOOL InterSection(vector<section> &sec,const section &dst);
    BOOL CheckPriceSet(void);
    void UpdateDetilListCtrl(int optNameIndex = -1);
    void InitSingleOPT(SINGLEOPT* pSingle);

    CWnd *m_pGeneralParamDlg;
    int m_comboPrcieChangePreCurSel;
    int m_nOptNameSelect;// 保存选择参数名列表索引
    int m_nOptDetilSelect;
    int m_row;//m_row为被选中行的行序号
    int m_column;//m_column为被选中行的列序号
};
