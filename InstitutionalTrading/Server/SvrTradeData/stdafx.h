// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma warning(disable : 4996)

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "FileOpr.h "

// TODO: reference additional headers your program requires here
#if 0
#define TRADE_LOG(fmt, ...)  ;
#else
#define TRADE_LOG(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","TradeData", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif


