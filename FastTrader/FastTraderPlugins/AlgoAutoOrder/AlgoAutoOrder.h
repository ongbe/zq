// AlgoAutoOrder.h : main header file for the AlgoAutoOrder application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CAlgoAutoOrderApp:
// See AlgoAutoOrder.cpp for the implementation of this class
//

class CAlgoAutoOrderApp : public CWinApp
{
public:
	CAlgoAutoOrderApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAlgoAutoOrderApp theApp;