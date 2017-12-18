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

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#include "RiskServerStruct.h"
#include "RiskManage\RiskManageCmd.h"
#include "RiskManage\RiskManageStruct.h"
#include "ReadWriteLock.h"
#include "tinyxml.h"
#include <time.h>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <io.h>
// TODO: reference additional headers your program requires here
void TRACE(const char* format,...);
void GetFolderFileName(std::string& strFileName,const std::string& folder="");
int GetTodayString(char* datestring=NULL);
int GetIDPre();
