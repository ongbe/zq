// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// TODO: reference additional headers your program requires here
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiDataType.h"
#include "..\..\Miscellaneous\Tools\inc\popmsgwin.h"

CPopMsgWin* _GetPopMsg();
void _ReleasePopMsg();
int GetNumDigits(double dbTick);
std::string Price2string(double dbData, double dbTick);
