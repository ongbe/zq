#pragma once
#include "afxwin.h"
#include "RiskManageStruct.h"


// CAutoForceCloseParamDlg dialog

class CAutoForceCloseParamDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoForceCloseParamDlg)

public:
	CAutoForceCloseParamDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoForceCloseParamDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FORCE_CLOSE };

	void SetTradeAccountID( int nAccountID );

	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnRecvMessage(WPARAM wParam, LPARAM lParam);

private:
	void SetModifyData();
	void EnableControls( BOOL bFlag );

public:
	CComboBox m_cbxForceCloseInd;
	CString m_csThreshold;
	CComboBox m_cbxCloseStrategy;
	CComboBox m_cbxClosePrice;

private:
	int m_nTradeAccountID;
	ForceCloseParam m_forceCloseParam;

public:
	afx_msg void OnBnClickedOk();
};
