// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)


#include <afxsock.h>            // MFC socket extensions

using namespace std;
#include <time.h>
#include <sstream>
#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <io.h>
#include<math.h>

#include "ReadWriteLock.h"
#include "textfile.h"
#include "TCPDataService.h"
//#include "CommonDef.h"
//#include "CommonStruct.h"
#include "QuoteCommon.h"
#include "AutoLock.h"
#include "TechDefine.h"
#include "TechIndex.h"
#include "tinyxml.h"
#include "FileOpr.h"
void split(const string& s, char c, vector<string>& v);
bool IsDoubleSame(double db1, double db2);
CString Double2String(double dbValue, int nNumDigits, bool bUseMax = false);
CString Double2String_Tip(double dbValue);

int getExpoBase2(const double& d);
bool	equals(const double& d1, const double& d2);
bool isInvalidValue(const double& d);
void  WriteLog_DepthMarketData(std::string szXns);
void GetFullStr(CString& str, double fValue);
void GetFullStr(CString& str, int nValue, int n);
void AppendLog_DepthMarketData(PlatformStru_DepthMarketData* pData);

//EnumPhrase GetEnumPhraseValue(KLineDataCycle eCycle);
//KLineDataCycle GetKLineDataCycleValue(EnumPhrase enumPhrase);
string GetTechNamePhrase(string strTechIndexName, EnumPhrase enumPhrase);
time_t convert_string_to_time_t(const std::string & time_string);
#define QuationHistory_info(fmt, ...)   CFileOpr::getObj().writelocallogDaily("log","HistoryData", ""fmt,  __VA_ARGS__);
#define QuationInTime_info(fmt, ...)   CFileOpr::getObj().writelocallogDaily("log","InTimeData", ""fmt,__VA_ARGS__);
#define QueryQuation_info(fmt, ...)   CFileOpr::getObj().writelocallogDaily("log","query", ""fmt, __VA_ARGS__);