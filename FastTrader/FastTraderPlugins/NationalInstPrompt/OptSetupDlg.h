#pragma once

#include "KernelStruct.h"
#include "PlusinAPI.h"

#include "ParamData.h"
#include <set>

#define WM_USER_CLOSE WM_USER+100

// COptSetupDlg dialog

class COptSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(COptSetupDlg)

public:
	COptSetupDlg(CWnd* pParent = NULL);   // standard constructor
    COptSetupDlg(CParamData* pData,CWnd* pParent = NULL);  
	virtual ~COptSetupDlg();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_OPTSETUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnEnChangeEditBandinstrument();
    afx_msg void OnBnClickedCheckAutoorder();
    DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

    CEdit m_editBandCodeCtrl;
    CEdit m_editIRRCtrl;
    CEdit m_editInterestRatesCtrl;
    CEdit m_editBandPriceCtrl;
    CEdit m_editTransferFactorCtrl;
    CEdit m_editYearInterestCountCtrl;
    CDateTimeCtrl m_datetimePerDelivCtrl;
    CDateTimeCtrl m_datetimeNextDelivCtrl;
    CDateTimeCtrl m_datetimeDelivCtrl;
    CButton m_btnAlarmCtrl;
    CButton m_btnAutoOrderCtrl;
    CEdit m_editInstrumentCtrl;
    CEdit m_editCountCtrl;
    CComboBox m_comboPriceCtrl;
    CComboBox m_comboDirectionCtrl;
    CComboBox m_comboOffsetIDCtrl;
public:    
    CEdit m_editBandInstrumentCtrl;   
    CEdit m_editQuotPriceCtrl;

public:

    //从控件中的值构造一个Data
    void CreateSetupParamData();
    CParamData* GetSetupParamData();

private:

    void EnableCtrls(bool bEnable);
    //检验数据有效性
    bool Verify();
    //将Data中的值回显到控件中去
    void SetSetupParamDataToWnd();

    /*合约输入edit智能匹配的功能 预留（没完成）
    bool MakeConstractIDVarietyMap(string szExchengeID,set<string> setInstrumentIDs, map<string, string>& mapConstractIDVariety);
    */


private:   
    
    CParamData* m_pParamData;

    //上一次订阅行情的合约
    string m_SubscribedInstrument;
};

