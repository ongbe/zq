// NationalInstPrompt.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



// CArbitrageDelegationApp:
// See NationalInstPrompt.cpp for the implementation of this class
//

class CArbitrageDelegationApp : public CWinApp
{
public:
	CArbitrageDelegationApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CArbitrageDelegationApp theApp;
extern HWND g_hWnd_FT;
