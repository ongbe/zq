// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif
#define WXLOG_INFO wxLogDebug
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/EventSet.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/TradeInfoDlg.h" //交易信息提示对话框
#include <vector>
#include <string>
#include <map>
using namespace std;

#define AUTO_SETSIZE()	SetSize(GetSize().x,GetSize().y+wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)-26)

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here
