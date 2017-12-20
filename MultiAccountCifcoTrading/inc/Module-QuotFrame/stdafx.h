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
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/globalFunc.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

//#define _SUPOORT_QUOT_TIME_CHART  //是否使用分时趋势图

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


#define __CUSTOM_FILE__    (std::string(__FILE__).substr(std::string(__FILE__).rfind('\\')+1).c_str())

//#if 0
//#define LOG_INFO(fmt, ...) 
//#else
//#define LOG_INFO(fmt, ...) \
//    do{\
//		if(m_pWriteLog)\
//		{\
//		m_pWriteLog->WriteLog_Fmt("UserOperate", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
//		}\
//	}while(0)
//#endif


// TODO: reference additional headers your program requires here
