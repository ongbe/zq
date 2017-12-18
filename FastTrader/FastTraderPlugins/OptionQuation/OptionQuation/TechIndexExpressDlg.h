#pragma once
#include "afxwin.h"


// CTechIndexExpressDlg dialog

class CTechIndexExpressDlg : public CDialog
{
	DECLARE_DYNAMIC(CTechIndexExpressDlg)

public:
	CTechIndexExpressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTechIndexExpressDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TechIndexExpress };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void SetTechIndex(CString strTechIndex);
	CString	m_strTechIndex;
	void Init();
	void OnCloseEvent();
	void OnCopy();

	CString m_strExpress;
	CEdit m_Edit;
};
