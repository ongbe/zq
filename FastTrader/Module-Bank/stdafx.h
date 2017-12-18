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
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "..\\ctp\\ThostFtdcUserApiStruct.h" 
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc2/GlobalFunc.h"
#include <map>
#include <vector>
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define AUTO_SETSIZE()	SetSize(GetSize().x,GetSize().y+wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)-26)

#define EVENT_SETCLIENTDATA(datatype)\
	do{\
	if(data.Size!=sizeof(datatype))return 0;\
	datatype* pTmp=new datatype();\
	memcpy(pTmp, &data, sizeof(datatype));\
	evt.SetClientData(pTmp);\
	}while(0)
// TODO: reference additional headers your program requires here
