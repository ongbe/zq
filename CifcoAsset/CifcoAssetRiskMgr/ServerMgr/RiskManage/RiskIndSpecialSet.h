#pragma once
#include "afxcmn.h"
#include "CGridListCtrlEx.h"
#include "RiskManageStruct.h"
#include "IndicatorTabDlg.h"
#include "afxwin.h"

// CRiskIndSpecialSet dialog

class CRiskIndSpecialSet : public CDialog
{
	DECLARE_DYNAMIC(CRiskIndSpecialSet)

public:
	CRiskIndSpecialSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRiskIndSpecialSet();

// Dialog Data
	enum { IDD = IDD_DIALOG_WARNING_SET_PRODUCT };

	virtual BOOL OnInitDialog();

	void SetIndicator( const std::vector<RiskIndicator>& vIndicator);
	void SetTraderID(int nTraderID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLvnEndlabeleditListIndModule(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListIndModule(NMHDR *pNMHDR, LRESULT *pResult);	

	afx_msg void OnBnClickedButtonAddModule();
	afx_msg void OnBnClickedButtonDelModule();
	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);

private:
	void ResetTabItems();
	void SetWarningData();
	void RefreshTabItems();

	void DelProductID(int nSel);
	void EnableControls(BOOL bFlag);
	CString ConventErrorMsg( const CString& csMsg );

private:
	CGridListCtrlEx m_listModule;
	CTabCtrl m_tabCtrl;
	CButton m_btnAdd;
	CButton m_btnDel;
	CButton m_btnSave;
	std::vector<CIndicatorTabDlg*> m_vDlgs;

	std::vector<RiskIndicator> m_vIndicator;
	std::set<CString> m_setProductID;	//存储所有品种信息
	std::vector<CString> m_vUsedProductID;
	CString m_csCurProductID;
	int m_nTraderID;

	std::map<CString, std::map<int, std::vector<IndLevelWarning>>> m_mapRiskWarning; 
	std::map<int, std::vector<IndLevelWarning>> m_mapCurRiskWarning;

public:
	afx_msg void OnBnClickedButtonRiskSave();
};
