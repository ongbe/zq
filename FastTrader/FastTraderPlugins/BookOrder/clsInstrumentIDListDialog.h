#pragma once
#include "afxwin.h"


// CInstrumentIDListDialog dialog

class CInstrumentIDListDialog : public CDialog
{
public:
	enum { conMaxButtonNum = 1000 };
	enum { conEachRowBtnNum = 4 };

	DECLARE_DYNAMIC(CInstrumentIDListDialog)

public:
	CInstrumentIDListDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstrumentIDListDialog();

	void ShowView(CString& strKeyInputReal, BOOL bShowCmd);
	void SetOutputEditInstrumentID(CEdit* poOutputEdit)
	{
		m_poOutputEditInstrumentID = poOutputEdit;
	}


// Dialog Data
	enum { IDD = IDD_INSTRUMENTIDLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual void OnOK();

	afx_msg void OnBnClickedInstrumentidGroup();
	afx_msg void OnLbnSelchangeInstrumentidlist();
	afx_msg void OnClick_InstrumentIDList();
	afx_msg void OnKeyPress_InstrumentIDList(UINT nKeyNum);
	DECLARE_MESSAGE_MAP()
public:

protected:
	int				m_nCodeHeadsCount;
	CODEHEADDATA*	m_ptCodeHeads;
	BOOL			m_bHasUserChange;
	CButton**		m_pButtonGroup;
	CListBox		m_ctlInstrumentIDList;
	CEdit*			m_poOutputEditInstrumentID;
	std::map<std::string, int>	m_mapCodeHead2NameIndex;
	std::map<std::string, int>	m_mapInstrumentID2NameIndex;

};
