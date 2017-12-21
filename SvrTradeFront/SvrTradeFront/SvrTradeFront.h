#pragma once
#include <windows.h>
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonMacro.h"
#include "CommonStruct.h"
#include "fttcmddef.h"
#include "fttcommdef.h"
#include "Module-Misc2\\KernelStruct.h"
#include "easymutex.h"
#include "SvrTcp\\Interface_SvrTcp.h"
#include "FTTraderApi.h"
#include "myxml.h"
#include "FileOpr.h"
//#include "tools_util.h"
#include "DataPackageForwarding.h"


#ifndef SVRFTTRADER_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTRADEFRONT_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SVRTRADEFRONT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTRADEFRONT_EXPORTS
#define SVRTRADEFRONT_API __declspec(dllexport)
#else
#define SVRTRADEFRONT_API __declspec(dllimport)
#endif
#endif



#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","SvrTradefrontModule", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

//#define CONFIG_FILE     "quotfunction.ini"
//#define SIMULATE_FILE   "simulate.ini"
#define FORWARINGSETTING_FILE  "forwarding.ini"
#define SSLLINKSETTING_FILE  "SSLSetting.ini"
//#define QUOTTABLENAME   "AQUOTINFO"
//#define DATETIMESTYLE   "YYYYMMDD HH:MM:SS"

//#include "easytime.h"
//#include "easytimespan.hpp"



extern Ceasymutex			g_mutex;//互斥锁
extern map<int,CFTTraderApi*> g_mapSocket2API;//数据管理

inline void _AddAPI(int hSocket)
{
	CeasymutexGuard guard(g_mutex);
	map<int,CFTTraderApi*>::const_iterator it_api = g_mapSocket2API.find(hSocket);
	if(it_api==g_mapSocket2API.end())
	{
		CFTTraderApi* pApi = new CFTTraderApi();
		pApi->Init(hSocket);
		g_mapSocket2API.insert(std::make_pair(hSocket,pApi));
	}
}

inline CFTTraderApi* _DeleteAPI(int hSocket)
{
	CeasymutexGuard guard(g_mutex);
	CFTTraderApi* pApi = NULL;
	map<int,CFTTraderApi*>::const_iterator it_api = g_mapSocket2API.find(hSocket);
	if(it_api!=g_mapSocket2API.end())		
	{
		pApi = it_api->second;
		if(pApi)
		{
			pApi->Release();
			delete pApi;
			g_mapSocket2API.erase(it_api);
		}
	}
	return NULL;
}

inline void _ClearAPI()
{
	CeasymutexGuard guard(g_mutex);
	CFTTraderApi* pApi = NULL;
	map<int,CFTTraderApi*>::const_iterator it_api = g_mapSocket2API.begin();
	while(it_api!=g_mapSocket2API.end())		
	{
		pApi = it_api->second;
		if(pApi)
		{
			delete pApi;
			it_api = g_mapSocket2API.erase(it_api);
		}
		else
			++it_api;
	}
}

inline CFTTraderApi* _GetAPI(int hSocket)
{
	CeasymutexGuard guard(g_mutex);
	map<int,CFTTraderApi*>::const_iterator it_api = g_mapSocket2API.find(hSocket);
	if(it_api!=g_mapSocket2API.end())		
		return it_api->second;

	return NULL;
}

inline bool _LoadFrontAddrs(eFrontAddsType nType, std::set<std::string>& setFrontAdds)
{
	CMyXML myxml("CTPServer.xml");
	setFrontAdds = myxml.GetNodeContent4("Server", (TRADE_TYPE == nType)?"Trading":"MarketData", "item");
	return (setFrontAdds.size()>0) ? true:false;
}

CDataPackageForwarding& _GetForwarding();

int _GetNextSeq();

void _SetThreadServData(CDataPackageForwarding::Stru_ThreadData& tdata);
bool _GetThreadServData(CDataPackageForwarding::Stru_ThreadData& tdata);
