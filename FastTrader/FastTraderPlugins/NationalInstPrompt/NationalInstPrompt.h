// NationalInstPrompt.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols



// CNationalInstPromptApp:
// See NationalInstPrompt.cpp for the implementation of this class
//

class CNationalInstPromptApp : public CWinApp
{
public:
	CNationalInstPromptApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CNationalInstPromptApp theApp;
extern HWND g_hWnd_FT;
