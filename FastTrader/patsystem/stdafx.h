// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "ctp/ThostFtdcUserApiStruct.h"
#include "ctp/patsapiSpi.h"
#include "patsIntf.h"
#include <wx/wxprec.h>
#include "../inc/gui/VersionDefine.h"
void DLL_TRACE(const char* format,...);
// TODO: reference additional headers your program requires here

#define __CUSTOM_FILE__    (std::string(__FILE__).substr(std::string(__FILE__).rfind('\\')+1).c_str())

#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
    do{\
		if(m_pWriteLog)\
		{\
		m_pWriteLog->WriteLog_Fmt("PATSYSTEM_API", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
		}\
	}while(0)
#endif