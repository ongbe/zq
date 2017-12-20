// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here
#ifndef SVRREDIS_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRDBOPR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRREDIS_EXPORTS
#define SVRREDIS_API __declspec(dllexport)
#else
#define SVRREDIS_API __declspec(dllimport)
#endif
#endif

#include "FileOpr.h "
#if 0
#define LOGIN_LOG(fmt, ...)  ;
#else
#define LOGIN_LOG(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","Login", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif