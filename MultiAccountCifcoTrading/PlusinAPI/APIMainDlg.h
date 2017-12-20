#if !defined(AFX_APIMAINDLG_H__4B23C00B_2A6E_45C0_AD20_46950D709FB1__INCLUDED_)
#define AFX_APIMAINDLG_H__4B23C00B_2A6E_45C0_AD20_46950D709FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// APIMainDlg.h : header file
//
#include "Resource.h"
#include <queue>
#include "PlusinImpl.h"

/////////////////////////////////////////////////////////////////////////////
// CAPIMainDlg dialog

class CAPIMainDlg : public CDialog
{
// Construction
public:
	CAPIMainDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAPIMainDlg)
	enum { IDD = IDD_DIALOG_MAIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAPIMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAPIMainDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg LRESULT OnRecvCopyData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	void SetSaveMsgCallBackFn( MsgCallBackFuncInner msgCallBackFn );

private:
	MsgCallBackFuncInner m_pSaveMsgCallBack;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APIMAINDLG_H__4B23C00B_2A6E_45C0_AD20_46950D709FB1__INCLUDED_)
