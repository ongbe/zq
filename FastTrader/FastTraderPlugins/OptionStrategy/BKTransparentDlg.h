#ifndef _BKTRANSPARENTDLG_H_
#define _BKTRANSPARENTDLG_H_

#include "BKDialog.h"
#include "BKButton.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBKTransparentDlg : public CBKDialog
{
public:
	CBKTransparentDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(CBKTransparentDlg)
	enum { IDD = IDD_TRANSPARENT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBKTransparentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CBKTransparentDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

private:
	void OnPostEraseBkgnd(CDC* pDC);
	void OnCancel();
	void OnOK();

	CButtonST	m_btnLogOff;
	CButtonST	m_btnWorkgroup;
	CButtonST	m_btnOpen;
	CButtonST	m_btnSearch;
	CButtonST	m_btnExplorer;
	CButtonST	m_btnHelp;
	CButtonST	m_btnAbout;
	CButtonST	m_btnCannibal;
	CButtonST	m_chkCheckbox;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
