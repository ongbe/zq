// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <windows.h>
#include "Tools.h"
#include "popmsgwin.h"

using namespace std;


#define WndUserCmd_YourPkgArrival 0x3bcb


UINT _GetDataRecvMsgID();
UINT _GetEventMsgID();

UINT _GetNextRequestID();

void _SetAddinDllFileName(const std::string& strFile);
const std::string& _GetAddinDllFileName();

void _SetWorkPath(const std::string& strPath);
std::string _GetWorkPath();
std::string _GetProjectsPath();
std::string _GetShareDllsPath();
std::string _GetTempPath();
std::string _GetHistoryDataPath();

void _SetSupportFilesPath(const std::string& strPath);
std::string _GetSupportFilesPath();

void _SetCurrUserID(const std::string& strID);
std::string _GetCurrUserID();
void _SetPassword(const std::string& strPasswd);
std::string _GetPassword();

CPopMsgWin* _GetPopMsg();
void _ReleasePopMsg();
