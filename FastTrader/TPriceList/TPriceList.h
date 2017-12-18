// TPriceList.h : main header file for the TPriceList DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTPriceListApp
// See TPriceList.cpp for the implementation of this class
//

class CTPriceListApp : public CWinApp
{
public:
	CTPriceListApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
