// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "FileOpr.h "
#if 0
#define NOTIFY_LOG(fmt, ...)  ;
#else
#define NOTIFY_LOG(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","Notify", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

// TODO: reference additional headers your program requires here
