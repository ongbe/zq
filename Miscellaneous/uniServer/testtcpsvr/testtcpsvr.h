#pragma once
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TESTTCPSVR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TESTTCPSVR_EXPORTS
#define TESTTCPSVR_API __declspec(dllexport)
#else
#define TESTTCPSVR_API __declspec(dllimport)
#endif

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include "SocketBase.h"
#include <vector>
using namespace std;

TESTTCPSVR_API void InitFunc(void);
TESTTCPSVR_API void ReleaseFunc(void);
TESTTCPSVR_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
TESTTCPSVR_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
TESTTCPSVR_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
TESTTCPSVR_API void TcpSvrOntimeFunc(void);




#include <set>
#include <map>
#include <string>
#include "Tools_Ansi.h"
#include "WinService.h"
#include "TcpServerWorker.h"
#include "UdpSimpleWorker.h"
#include "IniFile.h"
#include "StreamCache.h"
#include "WriteLog.h"
#include "FixedPackage.h"




