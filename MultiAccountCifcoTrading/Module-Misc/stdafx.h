// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
#include "wx/wxprec.h"
#ifndef WX_PRECOMP    
    #include <wx/wx.h>
#endif
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define AUTO_SETSIZE()	SetSize(GetSize().x,GetSize().y+wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)-26)

// TODO: reference additional headers your program requires here
