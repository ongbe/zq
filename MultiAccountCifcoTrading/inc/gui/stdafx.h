// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include "targetver.h"
// wxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif
//#ifdef _DEBUG
//#define WXLOG_INFO wxLogDebug
//#else
//#define WXLOG_INFO wxLogTrace
//#endif
#include "../inc/Module-Misc/globalDefines.h"

// TODO: reference additional headers your program requires here
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/ISvr/PlatformSvrMgr.h"



#include "wx/odcombo.h"
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include <vector>
#include <string>
#include "wx/statline.h"
#include <map>
#include "cfgMgr/CfgMgr.h"
#include <wx/listctrl.h>
#include "wx/file.h"


using namespace std;
#define AUTO_SETSIZE()	SetSize(GetSize().x,GetSize().y+wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)-26)
#define __CUSTOM_FILE__    (std::string(__FILE__).substr(std::string(__FILE__).rfind('\\')+1).c_str())
#endif
