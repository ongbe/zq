// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
#include <wx/wx.h>
#endif
#include <map>
#include <vector>
using namespace std;
#include "wx/textfile.h"


#include <time.h>
#include <fstream>

#include <queue>
#include <stack>
#include <set>
#include <map>
#include <string>
#include <winsock2.h> //can also be winsock.h
#include <windows.h> 
#include  <math.h>
#include "../../../inc/Quotation/QuoteCommon.h"
#include "../../../inc/Quotation/QuoteCommon_IO.h"
#include "../../../inc/Quotation/CritSec.h"
#include "../../../inc/Quotation/AutoLock.h"
#include "../../../inc/Quotation/tinyxml.h"
// TODO: reference additional headers your program requires here
extern int ColorMap[];

long ColorStr2Long(const char* pColTextColor);
wxString Double2String(double dbValue, int nNumDigits, bool bUseMax = false);
wxString Double2String_Tip(double dbValue);
#include "../../../inc/Quotation/TCPDataService.h"

using namespace std;

#include "../../../inc/Quotation/commonBNCMFunc.h"
#define BNCMINIFILE "hqServer.ini"

wxString GetBNCMIniFile();
#include "../../../inc\\Module-Misc2\\BusinessData.hpp"

string GetTechNamePhrase(string strTechIndexName, EnumPhrase enumPhrase);
