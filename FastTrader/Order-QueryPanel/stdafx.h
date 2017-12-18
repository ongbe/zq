// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif
#include "../inc/Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc2/GlobalFunc.h"
#include "../inc/Module-Misc/EventSet.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "../inc/Module-Misc/TradeInfoDlg.h"
#include "../inc/Module-Misc/orderDefines.h"
#include "../inc/Module-Misc/orderCommonFunc.h"
#include "../Module-Misc/LogDefine.h"
#include <vector>
#include <string>
#include <map>
using namespace std;

#define AUTO_SETSIZE()	SetSize(GetSize().x,GetSize().y+wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)-26)




// TODO: reference additional headers your program requires here
