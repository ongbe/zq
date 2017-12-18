// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN 
#include <wx/wxprec.h>
#ifndef WX_PRECOMP    
#include <wx/wx.h>
#endif
#include <vector>
using namespace std;
#include <winsock2.h> //can also be winsock.h
#include <windows.h> 
#include "wx/textfile.h"

#include "../../../inc/Quotation/tinyxml.h"
#include "../../../inc/Quotation/QuoteCommon.h"
#include "../../../inc/Quotation/Quotation.h"


//#include "..\inc\Module-Interface\interfaceDefines.h"
//#define MASTERNAMEDPIPESTRING "\\\\.\\pipe\\FastTrader\\v0_1\\Master"
#include "../../../inc/PlusinCommon/PlusinAPI.h"
#include "..\\inc\\Module-Misc2\\BusinessData.hpp"
//#include "..\\inc\gui\\TopException.h"
#include "..\\inc\\Module-Misc2\\GLobalFunc.h"
#include "../../../inc/Module-Misc/constants.h"


