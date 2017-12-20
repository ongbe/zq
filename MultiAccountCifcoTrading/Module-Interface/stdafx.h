// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif

#include <windows.h>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include "interfaceDefines.h"
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/ISvr/IPlatformSvr.h"
#include "cfgMgr/CfgMgr.h"
#include "..\inc\Module-Misc\GLobalFunc.h"

using namespace std;


#define USERLOG_INFO(fmt, ...) \
	if(m_pWriteLog) {\
		m_pWriteLog->WriteLog_Fmt("NamedPipe", LOGLEVEL_DEBUGINFO, fmt, __VA_ARGS__);\
	}

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

void TRACE(const char* format,...);


// TODO: reference additional headers your program requires here
