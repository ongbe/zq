#pragma once

#include "KernelStruct.h"
#include "PlusinAPI.h"

#include "ParamData.h"
#include <set>
#include "afxwin.h"
#include "afxdtctl.h"
#include "afxcmn.h"

#define WM_USER_SETUPDLG_CLOSE WM_USER+100

enum ARBITRAGE_TYPE{ARBITRAGE_ADD,ARBITRAGE_MODIFY};

// COptSetupDlg dialog

class COptSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(COptSetupDlg)

public:
	COptSetupDlg(CWnd* pParent = NULL);   // standard constructor
    COptSetupDlg(CParamData* pData,set<string>& setInstrumentID,ARBITRAGE_TYPE type,CWnd* pParent = NULL);  
	virtual ~COptSetupDlg();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_OPTSETUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnDeltaposSpinTrigger(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinDeviate(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinDelegate1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDeltaposSpinDelegate2(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnCbnSelchangeComboDirection1();
    afx_msg void OnEnChangeEditCount1();
    afx_msg void OnCbnSelchangeComboOffset();
    afx_msg void OnCbnSelchangeComboArbitragetype();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnCbnSelchangeComboInst1();
    afx_msg void OnCbnEditchangeComboInst1();
    afx_msg void OnCbnSelchangeComboInst2();
    afx_msg void OnCbnEditchangeComboInst2();

    DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
    CComboBox m_comboOffsetIDCtrl;
    CComboBox m_comboArbitrageTypeCtrl;
    CEdit m_editTriggerCtrl;
    CEdit m_editDeviateCtrl;
    CDateTimeCtrl m_datetimeValidateCtrl;
    CComboBox m_combInstrument1Ctrl;
    CComboBox m_combInstrument2Ctrl;
    CComboBox m_comboDirection1Ctrl;
    CComboBox m_comboDirection2Ctrl;
    CEdit m_editCount1Ctrl;
    CEdit m_editCount2Ctrl;
    CEdit m_editDelegate1Ctrl;
    CEdit m_editDelegate2Ctrl;
    CEdit m_editUpperPrice1Ctrl;
    CEdit m_editUpperPrice2Ctrl;
    CEdit m_editLowestPrice1Ctrl;
    CEdit m_editLowestPrice2Ctrl;   

public:

    //从控件中的值构造一个Data
    void CreateSetupParamData();
    CParamData* GetSetupParamData();

    void Update(const PlatformStru_DepthMarketData* pMarketData);

private:

    void EnableCtrls(bool bEnable);
    //检验数据有效性
    bool Verify();
    //将Data中的值回显到控件中去
    void SetSetupParamDataToWnd();

    void SubscribeMarketData(string instrumentID);

private:   
    
    CParamData* m_pParamData;

    //上一次订阅行情的合约
    string m_SubscribedInstrument;

    ARBITRAGE_TYPE m_nArbitrageType;

    set<string> m_setInstrumentID;
    map<string,string> m_mapInputKey2RightInst;//key 全部小写 value 正确的合约

    bool m_bInst1InputError;
    bool m_bInst2InputError;

};

