// SvrDBFTransfer.h : main header file for the SvrDBFTransfer DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

// CSvrDBFTransferApp
// See SvrDBFTransfer.cpp for the implementation of this class
//

class CSvrDBFTransferApp : public CWinApp
{
public:
	CSvrDBFTransferApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
