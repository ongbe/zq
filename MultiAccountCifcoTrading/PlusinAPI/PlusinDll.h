// PlusinDll.h : main header file for the PlusinDll DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPlusinDllApp
// See PlusinDll.cpp for the implementation of this class
//

class CPlusinDllApp : public CWinApp
{
public:
	CPlusinDllApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
