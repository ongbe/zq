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
#include "aui.h"
#include "../inc/Module-Misc/tinyxml.h"
#include "../inc/Module-Misc2/globalDefines.h"
#include "../inc/Module-Misc/EventSet.h"
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define TAB_STYLE (wxAUI_NB_TOP|wxAUI_NB_TAB_MOVE|wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_TAB_EXTERNAL_MOVE|wxNO_BORDER)


// TODO: reference additional headers your program requires here
