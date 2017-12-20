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

#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/GlobalFunc.h"

#include "../inc/Module-Orders/COrderMessageDlg.h"


#include "wx/odcombo.h"
#include "../inc/Module-Misc/ExtListCtrl.h"
#include <vector>
#include <string>
#include "wx/statline.h"
#include <map>
#include "cfgMgr/CfgMgr.h"
#include <wx/listctrl.h>
#include "wx/file.h"
using namespace std;
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here
