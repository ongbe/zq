// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiDataType.h"
#include "..\..\Miscellaneous\Tools\inc\popmsgwin.h"

// TODO: reference additional headers your program requires here
CPopMsgWin* _GetPopMsg();
void _ReleasePopMsg();