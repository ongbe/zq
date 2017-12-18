// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "wx/wx.h"
#include "wx/odcombo.h"
#include <vector>
#include <string>
using namespace std;

#include "md5sum.h"
#include "AppendXML.h"
#include  <shellapi.h>
#include "TCPDataService.h"
#include "CommonDef.h"
#include "wx/mstream.h"
void GetPath(std::string &filePath);
#define SERVERURL_FILENAME "AutoUpdate.xml"
struct SConfigPara
{
	std::string strVersion; //版本号
	std::string strMainFile;//下载信息文件
	std::string strIP;      //IP地址
	int nPort;				//端口号
};
void str_replace(std::string & str, const std::string & strsrc, const std::string &strdst) ;
#include "ReadWriteLock.h"
#include "FileOpr.h"
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","Update", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);