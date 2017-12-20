#pragma once

#ifndef SVRQUOTSERVER_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRQUOTSERVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRQUOTSERVER_EXPORTS
#define SVRQUOTSERVER_API __declspec(dllexport)
#else
#define SVRQUOTSERVER_API __declspec(dllimport)
#endif
#endif


#include "easymutex.h"
#include "CommonDef.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"
#include "..\\..\\..\\FastTrader\\inc\\Module-Misc2\\KernelStruct.h"


//互斥锁
extern Ceasymutex			g_mutex;
extern Ceasymutex			g_mutexImpl;


#include "FileOpr.h"
#include "tools_util.h"

#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

#define CONFIG_FILE     "quotfunction.ini"
#define SIMULATE_FILE   "simulate.ini"
#define QUOTTABLENAME   "AQUOTINFO"
#define DATETIMESTYLE   "YYYYMMDD HH:MM:SS"

#include "easytime.h"
#include "easytimespan.hpp"

/********************************宏说明******************************************
此行情服务为单一服务,是连接用户与交易所的桥梁, 
通过此服务可以完成
1.行情接收（交易所实时行情）
2.行情转发（推送用户订阅的行情）
3.行情处理（计算K线）
4.行情入库（本地数据库）

_SIMULATE_			模拟行情开关
_Stock_				证券业务开关（依赖SvrDBFTransfer服务）
_DeleteQuot_		删除交易行情线程开关（考虑到本地磁盘的存储空间）
_LoadKlineData_		加载历史K线数据线程开关

注：
	虽然bin目录下的function.ini中加载有其他的DLL，但如果不定义以上的宏，该宏对应的功能也不会存在。
*********************************************************************************/