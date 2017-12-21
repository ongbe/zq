// MonitorSvr_SaveData.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMonitorSvr_SaveDataApp:
// See MonitorSvr_SaveData.cpp for the implementation of this class
//

class CMonitorSvr_SaveDataApp : public CWinApp
{
public:
	CMonitorSvr_SaveDataApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMonitorSvr_SaveDataApp theApp;