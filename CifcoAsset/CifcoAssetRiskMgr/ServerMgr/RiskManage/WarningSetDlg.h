#pragma once
#include "afxcmn.h"
#include "CGridListCtrlEx.h"
#include "RiskManageStruct.h"
#include "IndicatorTabDlg.h"
#include "afxwin.h"

// CWarningSetDlg dialog

class CWarningSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CWarningSetDlg)

public:
	CWarningSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWarningSetDlg();

	void SetIndicator( const std::vector<RiskIndicator>& vIndicator);
	std::map<int, RiskIndicatorModule> GetRiskIndModule();

// Dialog Data
	enum { IDD = IDD_DIALOG_WARNING_SET };

	virtual BOOL OnInitDialog();

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
	//afx_msg LRESULT OnSaveWarningSet(WPARAM wParam, LPARAM lParam);

private:
	void ResetTabItems();
	void SetWarningData();

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
	std::map<int, RiskIndicatorModule> m_mapIndModule;	//存储理财产品信息，key为理财产品ID
	RiskIndicatorModule m_curIndModule;
	int m_nCurIndModuleID;

	std::map<int, std::map<int, std::vector<IndLevelWarning>>> m_mapRiskWarning; 
	std::map<int, std::vector<IndLevelWarning>> m_mapCurRiskWarning;

	bool m_bAbandon;

public:
	
	afx_msg void OnBnClickedButtonRiskSave();
	afx_msg void OnLvnItemchangingListIndModule(NMHDR *pNMHDR, LRESULT *pResult);
};
