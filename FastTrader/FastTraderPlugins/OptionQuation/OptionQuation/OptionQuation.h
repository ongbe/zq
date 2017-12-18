// OptionQuation.h : main header file for the OptionQuation DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// COptionQuationApp
// See OptionQuation.cpp for the implementation of this class
//

class COptionQuationApp : public CWinApp
{
public:
	COptionQuationApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
