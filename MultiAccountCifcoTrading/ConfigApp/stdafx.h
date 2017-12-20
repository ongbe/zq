// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <map>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif
#include "wx/odcombo.h"
#include "wx/treectrl.h"
#include "wx/statline.h"
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/FieldValueTool.h"
#include "cfgMgr/CfgMgr.h"
#include "Const.h"

using namespace std;

typedef struct
{
	std::string GroupName;
	bool   bChecked;
	DWORD			GroupId;
	std::vector<std::string>  InstrVec;
}GroupInfo2X;

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here
