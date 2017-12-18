#pragma once

#include "RiskManageStruct.h"
#include "afxwin.h"
#include "FinancialEdit.h"
// CFundNetParamDlg dialog

class CFundNetParamDlg : public CDialog
{
	DECLARE_DYNAMIC(CFundNetParamDlg)

public:
	CFundNetParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFundNetParamDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FUND_NET_PARAM };

	void SetTradeAccountID( int nAccountID );


	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	void SetModifyData();
	void EnableControls( BOOL bFlag );

private:
	NetFundParam m_netFundParam;
	int m_nTradeAccountID;

public:
	afx_msg void OnBnClickedOk();
	CFinancialEdit m_edtInnerVolumn;

	CComboBox m_cbxInnerFundNet;
	CString m_csPerNet;
	afx_msg void OnDestroy();
	CFinancialEdit m_edtOuterVolumn;
	CFinancialEdit m_edtOuterFundNet;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void CalcPerNet();
};
