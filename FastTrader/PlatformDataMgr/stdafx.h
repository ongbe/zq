// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>


#define __CUSTOM_FILE__    (std::string(__FILE__).substr(std::string(__FILE__).rfind('\\')+1).c_str())

