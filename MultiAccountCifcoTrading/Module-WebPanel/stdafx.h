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
#include <string>
#include <map>
#include <vector>
using namespace std;
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/GlobalFunc.h"
#include "../inc/Module-Misc/tinyxml.h"
#define VIDEOINFO_TYPE_ALL		0
#define VIDEOINFO_TYPE_COPPER	1
#define VIDEOINFO_TYPE_RUBBER	2
#define VIDEOINFO_TYPE_COTTON	3
#define VIDEOINFO_TYPE     VIDEOINFO_TYPE_RUBBER
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define TIMER_INTERVAL   300000
// TODO: reference additional headers your program requires here
