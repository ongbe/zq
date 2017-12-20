#pragma once

#ifndef WINVER                          // Specifies that the minimum required platform is Windows XP.
#define WINVER 0x0501           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows XP.
#define _WIN32_WINNT 0x0501     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE                       // Specifies that the minimum required platform is Internet Explorer 6.0.
#define _WIN32_IE 0x0600        // Change this to the appropriate value to target other versions of IE.
#endif
// wxWidgets
#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
#include <wx/wx.h>
#endif
#define IE_MAGIC_NUMBER  0x4945
#define IE_MSG_LOGIN		1
#define IE_MSG_CONTENT		2
#define IE_MSG_EXIT			3
BEGIN_DECLARE_EVENT_TYPES()	
	DECLARE_EVENT_TYPE(wxEVT_GET_NEW_CONTENT, 1000)
END_DECLARE_EVENT_TYPES()