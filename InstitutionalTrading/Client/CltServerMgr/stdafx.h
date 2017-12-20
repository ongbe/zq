// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiDataType.h"
#include "..\..\Miscellaneous\Tools\inc\popmsgwin.h"

#include <string>
#include <queue>

// TODO: reference additional headers your program requires here
#define TIMER_LOGON_CHECK 1
#define MAX_PERMISSION_NAME_LENGTH 20
#define PERMISSION_IP_LENGTH       15
#define MIN_PERMISSION_PORT_VALUE 0
#define MAX_PERMISSION_PORT_VALUE 65535

#define SettlementLoad_TradingDataWithHistory  2

CPopMsgWin* _GetPopMsg();
void _ReleasePopMsg();
void _Replace_stdstring(std::string& strBig, const std::string& strsrc, const std::string &strdst);
int _MakeDateSeq(const std::string& strStartDate, const std::string& strEndDate, 
				  std::queue<std::string>& queueDate);
