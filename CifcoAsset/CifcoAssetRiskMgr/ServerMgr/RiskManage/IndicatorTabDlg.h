#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "CGridListCtrlEx.h"
#include "RiskManageStruct.h"
#include "IndicatorTabDlg.h"

// CIndicatorTabDlg dialog

class CIndicatorTabDlg : public CDialog
{
	DECLARE_DYNAMIC(CIndicatorTabDlg)

public:
	CIndicatorTabDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIndicatorTabDlg();

	void SetValid(BOOL bValid);
	BOOL IsValid();

	void SetIndicatorID( int nIndID );
	int GetIndicatorID();

	void SetWarningData( const std::vector<IndLevelWarning>& vRiskWarning );
	const std::vector<IndLevelWarning>& GetWarningData();

	void ResetData();

	void EnableControls(BOOL bFlag);

// Dialog Data
	enum { IDD = IDD_DIALOG_IND_TAB };
	
	virtual BOOL OnInitDialog();

	bool IsDataChanged();
	bool RetrieveData( bool bPrompt = true );
	void SetSortedOrder( bool bSortedOrder );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAddLevel();
	afx_msg void OnBnClickedButtonDelLevel();

private:
	void AddEmptyRow();

private:
	BOOL m_bValid;
	int m_nIndicatorID;
	bool m_bInit;
	bool m_bRiskThresholdSortedOrder;	//true, ÉýÐò false, ½µÐò
	
public:
	CButton m_btnAddLevel;
	CButton m_btnDelLevel;
	CGridListCtrlEx m_list;

	std::vector<IndLevelWarning> m_vOriginalData;
	std::vector<IndLevelWarning> m_vCommitData;
};
