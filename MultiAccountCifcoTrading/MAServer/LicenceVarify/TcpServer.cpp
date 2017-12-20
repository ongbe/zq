#include "stdafx.h"
#include "LicenceVarify.h"
#include <map>
#include <vector>
#include <queue>
#include "Tools_Ansi.h"
#include "TcpServerWorker.h"
#include "CommonPkg.h"
#include <string>
using namespace std;

extern CWriteLog* g_pWriteLog;
extern char* g_pSem;
extern int g_SemCount;
extern std::map<SOCKET,Stru_DataSocket> g_TcpUsers;					//已经建立tcp连接的用户
extern std::vector<Stru_Licence> g_vLicence;
extern std::map<string, Stru_ServerAddr> g_serverAddrMap;

void ProcessPackage( Stru_DataSocket& soketData );
bool VerifyRequest( Stru_UniPkgHead& head );
void SaveReqToMsgQueue( Stru_DataSocket& soketData, Stru_UniPkgHead& head );
void ProcessLicenceReq( SOCKET hSocket );
void ProcessAddrReq( SOCKET hSocket, const char* pBuf );

LICENSEVARIFY_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{

}

LICENSEVARIFY_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	if( pData == NULL ) return;

	std::map<SOCKET,Stru_DataSocket>::iterator it = g_TcpUsers.find(hSocket);
	if( it != g_TcpUsers.end() && it->second.pStreamCache != NULL )
	{
		it->second.pStreamCache->SaveDataTo(pData,DataLen);
	}

	ProcessPackage( it->second );
}

LICENSEVARIFY_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	Stru_DataSocket TmpDataSocket;
	TmpDataSocket.hScoket = hSocket;
	TmpDataSocket.pStreamCache = new CMemoryCache(10240);
	TmpDataSocket.RemoteAddr = RemoteAddr;
	time(&TmpDataSocket.HeartbeatTime);

	std::map<SOCKET, Stru_DataSocket>::iterator it = g_TcpUsers.find(hSocket);
	if( it != g_TcpUsers.end())
	{
		if(it->second.pStreamCache)
		{
			delete it->second.pStreamCache;
			it->second.pStreamCache = NULL;
		}

		g_TcpUsers.erase(it);
	}

	g_TcpUsers[hSocket]=TmpDataSocket;

	if(g_pWriteLog)
		g_pWriteLog->WriteLog_Fmt("接受连接",WriteLogLevel_DEBUGINFO,"%s/%d-%d",
		CTools_Ansi::ConvertdwIP(TmpDataSocket.RemoteAddr.IP),
		TmpDataSocket.RemoteAddr.Port, hSocket);
}

LICENSEVARIFY_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	std::map<SOCKET,Stru_DataSocket>::iterator it = g_TcpUsers.find(hSocket);
	if(it != g_TcpUsers.end())
	{
		if(g_pWriteLog)
			g_pWriteLog->WriteLog_Fmt("断开连接",WriteLogLevel_DEBUGINFO,"%s/%d-%d",
			CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
			it->second.RemoteAddr.Port, hSocket);

		if(it->second.pStreamCache)
		{
			delete it->second.pStreamCache;
			it->second.pStreamCache = NULL;
		}

		g_TcpUsers.erase(it);
	}
}

LICENSEVARIFY_API void TcpSvrOntimeFunc(void)
{
	if ( g_TcpUsers.empty() )
	{
		return;
	}

	time_t curTime;
	time(&curTime);

	//检查超时连接
	std::map<SOCKET,Stru_DataSocket>::iterator it = g_TcpUsers.begin();
	std::vector<SOCKET> vTemp;
	vTemp.clear();
	for( ;it != g_TcpUsers.end(); ++it)
	{
		int nDeltaSecs = (int)(curTime-it->second.HeartbeatTime);
		if( nDeltaSecs > 30000 )	//30秒超时
		{
			if(g_pWriteLog)
				g_pWriteLog->WriteLog_Fmt("连接超时，断开连接",WriteLogLevel_DEBUGINFO,"%s/%d 超时时长%ds",
				CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
				it->second.RemoteAddr.Port,
				nDeltaSecs);

			vTemp.push_back( it->first );
		}
	}

	for ( int i = 0; i < static_cast<int>(vTemp.size()); i++ )
	{
		CSocketBase::CloseSocket(vTemp[i]);
	}
}

void ProcessPackage( Stru_DataSocket& soketData )
{
	if ( NULL == soketData.pStreamCache )
	{
		return;
	}

	int nCacheLen = 0;
	while( (nCacheLen = soketData.pStreamCache->GetLen()) >= sizeof(Stru_UniPkgHead) + sizeof(TAIL))
	{
		char szHeadBuf[sizeof(Stru_UniPkgHead)];
		memset(szHeadBuf, 0, sizeof(Stru_UniPkgHead));
		soketData.pStreamCache->PeekFrom(szHeadBuf, sizeof(Stru_UniPkgHead));
		int nPackageHead = *(DWORD*)szHeadBuf;

		// 检查并抛弃包头错误的部分
		if( nPackageHead != HeadID_UniPkg)
		{
			int i = 0;
			for( ; i < sizeof(Stru_UniPkgHead) - sizeof(DWORD) + 1; i++)
			{
				if(*(DWORD*)(szHeadBuf + i) == HeadID_UniPkg )
				{
					break;
				}
			}

			soketData.pStreamCache->DeleteData(i);
			continue;
		}

		Stru_UniPkgHead pkgHead;
		soketData.pStreamCache->PeekFrom(&pkgHead, sizeof(Stru_UniPkgHead));
		//等待一个包收齐
		if ( nCacheLen < sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL) )
		{
			return;
		}

		// 将合法请求信息加入消息队列，待Work线程处理
		if ( VerifyRequest(pkgHead))
		{
			time(&soketData.HeartbeatTime);
			Stru_UniPkgHead head;
			soketData.pStreamCache->GetFrom(&head, sizeof(Stru_UniPkgHead));
			SaveReqToMsgQueue(soketData, head);
			if(g_pWriteLog)
				g_pWriteLog->WriteLog_Fmt("接收业务数据",WriteLogLevel_DEBUGINFO,"%s/%d moduleID=%d, CMDID=%d",
				CTools_Ansi::ConvertdwIP(soketData.RemoteAddr.IP),
				soketData.RemoteAddr.Port, head.moduleid, head.cmdid);

			continue;
		}

		//检查、处理心跳包
		if( pkgHead.cmdid == CMDID_HEARTBEAT 
			&& pkgHead.len == 0 )
		{
			Stru_Heartbeat heartBeatPkg;
			memset(&heartBeatPkg, 0, sizeof(heartBeatPkg));
			soketData.pStreamCache->GetFrom(&heartBeatPkg, sizeof(Stru_Heartbeat));
			if( heartBeatPkg.tail == TailID_UniPkg )
			{			
				//处理心跳包
				time(&soketData.HeartbeatTime);
				heartBeatPkg.head.cmdid = CMDID_HEARTBEATRSP;
				//heartBeatPkg.head.moduleid = MODULE_ID_DBCOMPARE_CLIENT;
				CSocketBase::SendDataTcp(soketData.hScoket, (char*)&heartBeatPkg, sizeof(Stru_Heartbeat));
			}
			continue;
		}

		//检查并抛弃错误包，如果是一个完整包，但在前面得不到处理，则认为是一个错误包，抛弃
		if( nCacheLen >= (int)(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			soketData.pStreamCache->DeleteData( sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL) );
			if(g_pWriteLog)
			{
				g_pWriteLog->WriteLog_Fmt("抛弃不合法数据",WriteLogLevel_DEBUGINFO,"");
			}
			continue;
		}

		break;
	}
}

void SaveReqToMsgQueue( Stru_DataSocket& soketData, Stru_UniPkgHead& head )
{
	char* pBuf = NULL;
	if ( head.len != 0 )
	{
		pBuf = new char[head.len];
		if ( NULL == pBuf )
		{
			soketData.pStreamCache->DeleteData(head.len+sizeof(TAIL));
			if(g_pWriteLog)
				g_pWriteLog->WriteLog_Fmt("内存不够",WriteLogLevel_DEBUGINFO, "%d", head.cmdid);
			return;
		}

		memset( pBuf, 0, head.len );
		soketData.pStreamCache->GetFrom(pBuf, head.len);
	}

	TAIL tail;
	soketData.pStreamCache->GetFrom(&tail, sizeof(TAIL));
	if ( tail == TailID_UniPkg )
	{
		if ( head.cmdid == CMDID_LICENCE_LIST_REQ)
		{
			ProcessLicenceReq(soketData.hScoket);
		}
		else if ( head.cmdid == CMDID_SERVER_ADDR_REQ)
		{
			ProcessAddrReq(soketData.hScoket, pBuf);
		}
		else
		{

		}
	}
	else
	{
		if(g_pWriteLog)
			g_pWriteLog->WriteLog_Fmt("包尾不一致",WriteLogLevel_DEBUGINFO, "%d", tail);
	}

	if ( NULL != pBuf )
	{
		delete[] pBuf;
		pBuf = NULL;
	}

	return;
}

bool VerifyRequest( Stru_UniPkgHead& head )
{
	switch(head.cmdid)
	{
	case CMDID_LICENCE_LIST_REQ:
		return head.len == 0;
	case CMDID_SERVER_ADDR_REQ:
		return true;
	default:
		return false;
	}
}

void ProcessLicenceReq( SOCKET hSocket )
{
	int nTotalLen = sizeof(Stru_UniPkgHead) + sizeof(Stru_Licence)*g_vLicence.size() + sizeof(TAIL);
	char* pSendBuf = new char[nTotalLen];

	Stru_UniPkgHead head;
	head.cmdid = CMDID_LICENCE_LIST_RSP;
	head.len = sizeof(Stru_Licence)*g_vLicence.size();
	memcpy(pSendBuf, &head, sizeof(Stru_UniPkgHead));

	if ( !g_vLicence.empty())
	{
		memcpy(pSendBuf + sizeof(Stru_UniPkgHead), &g_vLicence[0], sizeof(Stru_Licence)*g_vLicence.size());
	}

	TAIL tail = TailID_UniPkg;
	memcpy(pSendBuf + sizeof(Stru_UniPkgHead) + sizeof(Stru_Licence)*g_vLicence.size(), &tail, sizeof(tail));

	CSocketBase::SendDataTcp(hSocket, pSendBuf, nTotalLen);

	delete [] pSendBuf;
	pSendBuf = NULL;
}

void ProcessAddrReq( SOCKET hSocket, const char* pBuf )
{
	Stru_ServerAddr addr;
	memset(&addr, 0, sizeof(addr));

	std::map<std::string, Stru_ServerAddr>::iterator it = g_serverAddrMap.find(pBuf);
	if ( it == g_serverAddrMap.end())
	{
		return;
	}
	else
	{
		addr = it->second;
	}

	int nTotalLen = sizeof(Stru_UniPkgHead) + sizeof(Stru_ServerAddr) + sizeof(TAIL);
	char* pSendBuf = new char[nTotalLen];

	Stru_UniPkgHead head;
	head.cmdid = CMDID_SERVER_ADDR_RSP;
	head.len = sizeof(Stru_ServerAddr);
	memcpy(pSendBuf, &head, sizeof(Stru_UniPkgHead));
	memcpy(pSendBuf + sizeof(Stru_UniPkgHead), &addr, sizeof(Stru_ServerAddr));

	TAIL tail = TailID_UniPkg;
	memcpy(pSendBuf + sizeof(Stru_UniPkgHead) + sizeof(Stru_ServerAddr), &tail, sizeof(tail));

	CSocketBase::SendDataTcp(hSocket, pSendBuf, nTotalLen);

	delete [] pSendBuf;
	pSendBuf = NULL;
}
