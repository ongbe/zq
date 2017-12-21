// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "winbase.h"
#include <queue>
#include "tinyxml.h"
//#pragma pack(push)
//#pragma pack(1)

#include "fttcmddef.h"

#include "CommonPkg.h"
#include "CommonDefine.h"
#include "ReadWriteLock.h"
#include "SocketBase.h"
#include "ToolThread.h"
#include "MsgQueue.h"
#include "CmdMsgQueue.h"
#include "UDPDataService.h"
#include "FileOpr.h"
#include "ThostFtdcUserApiStruct.h"
#include "KernelStruct.h"
#include "..\SvrTradeFront\DataStruct.h"
#include "monitorCommonDefine.h"
#include "textfile.h"

void split(const string& s, char c, vector<string>& v); 
void Load(std::string& UDPSvrIP, unsigned short& UDPSvrPort, std::string& strTime);


// TODO: reference additional headers your program requires here
#define Monitor_info(fmt, ...)   CFileOpr::getObj().writelocallogDaily("log","Monitor", ""fmt,  __VA_ARGS__);
#define MonitorCpu_info(fmt, ...)   CFileOpr::getObj().writelocallogDaily("log","cpu", ""fmt, __VA_ARGS__);

#define LogData(fmt, ...)   CFileOpr::getObj().writelocallogDaily("log","Min1_Line", ""fmt, __VA_ARGS__);