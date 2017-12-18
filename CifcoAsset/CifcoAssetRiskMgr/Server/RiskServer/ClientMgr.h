#pragma once

#include "SocketCommon.h"

class CWriteLog;
class CThreadSharedData;
class CWorker;

class CClientMgr
{
public:
	static void Init(void);
	static void Release(void);
	static void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	static void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	static void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	static void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	static void TcpSvrOntimeFunc(void);

private:
	CClientMgr(void);
	virtual ~CClientMgr(void);
	DISALLOW_COPY_AND_ASSIGN(CClientMgr);

	void _UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void _TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void _TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void _TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void _TcpSvrOntimeFunc(void);

	void ProcessPackage( Stru_DataSocket& soketData );
	void SaveReqToMsgQueue( Stru_DataSocket& soketData, Stru_UniPkgHead& head );
	bool VerifyRequest( Stru_UniPkgHead& head );

private:
	static CClientMgr* m_pClientMgr;

	CWriteLog* m_pWriteLog;
	std::map<SOCKET,Stru_DataSocket> m_mapTcpUsers;
	CThreadSharedData* m_pThreadShareData;
	CWorker* m_pWorker;
};
