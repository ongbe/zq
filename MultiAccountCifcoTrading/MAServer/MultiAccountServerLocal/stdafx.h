// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <WinSock2.h>

#pragma warning(disable : 4996)
// TODO: reference additional headers your program requires here

#define MAX_SQL_LENGTH			5000	//SQL语句最大长度
#define MAX_USABLE_SQL_LENGTH	4999
#define LOG_BUFFER_SIZE			256
#define LOG_BUFFER_USABLE_SIZE	255
