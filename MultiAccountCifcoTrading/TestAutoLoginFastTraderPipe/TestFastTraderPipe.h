// TestFastTraderPipe.h : main header file for the TESTFASTTRADERPIPE application
//

#if !defined(AFX_TESTFASTTRADERPIPE_H__E02F3F35_43DD_4383_A7DD_E5D6AAB129A9__INCLUDED_)
#define AFX_TESTFASTTRADERPIPE_H__E02F3F35_43DD_4383_A7DD_E5D6AAB129A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestFastTraderPipeApp:
// See TestFastTraderPipe.cpp for the implementation of this class
//

class CTestFastTraderPipeApp : public CWinApp
{
public:
	CTestFastTraderPipeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestFastTraderPipeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestFastTraderPipeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTFASTTRADERPIPE_H__E02F3F35_43DD_4383_A7DD_E5D6AAB129A9__INCLUDED_)
