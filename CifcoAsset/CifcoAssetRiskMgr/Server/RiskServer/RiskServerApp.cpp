// RiskServer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Tools/SocketBase.h"
#include "RiskMgr.h"
#include "ClientMgr.h"
#include "DBAccess.h"
#include "DataCenter.h"
#include "RiskMsgQueue.h"
#include "ConfigManager.h"
#include "TopException.h"
#ifdef __cplusplus
extern "C"{
#endif
LPTOP_LEVEL_EXCEPTION_FILTER g_pFilter=NULL;
void InitFunc(void)
{
	g_pFilter=::SetUnhandledExceptionFilter(TopExceptionCallback);
	ConfigManager::Init();
	CDataCenter::Init();
	ALL_RISKMSGQUEUE_INIT();
	CClientMgr::Init();
	CRiskMgr::Init();
}
void ReleaseFunc(void)
{
	CRiskMgr::Release();
	CClientMgr::Release();
	ALL_RISKMSGQUEUE_RELEASE();
	CDataCenter::Release();
	ConfigManager::Release();
	::SetUnhandledExceptionFilter(g_pFilter);
}
void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CClientMgr::UdpRecvFunc(pData,DataLen,SvrAddr,RemoteAddr,hSocket);
}
void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CClientMgr::TcpSvrAcceptFunc(SvrAddr,RemoteAddr,hSocket);
}
void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CClientMgr::TcpSvrDisconnFunc(SvrAddr,RemoteAddr,hSocket);
}
void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CClientMgr::TcpSvrRecvFunc(pData,DataLen,SvrAddr,RemoteAddr,hSocket);
}
void TcpSvrOntimeFunc(void)
{
	CClientMgr::TcpSvrOntimeFunc();
}

#ifdef __cplusplus
}
#endif