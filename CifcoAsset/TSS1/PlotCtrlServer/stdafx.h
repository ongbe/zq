// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "inc\SocketBase.h"


// TODO: reference additional headers your program requires here

#ifdef PLOTCTRLSERVER_EXPORTS
#define PLOTCTRLSERVER_API __declspec(dllexport)
#else
#define PLOTCTRLSERVER_API __declspec(dllimport)
#endif



PLOTCTRLSERVER_API void InitFunc(void);
PLOTCTRLSERVER_API void ReleaseFunc(void);
PLOTCTRLSERVER_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
PLOTCTRLSERVER_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
PLOTCTRLSERVER_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
PLOTCTRLSERVER_API void TcpSvrOntimeFunc(void);
