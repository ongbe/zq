#include "StdAfx.h"
#include "Tools/SocketBase.h"
#include "Tools/WriteLog.h"
#include "Tools/Tools_Ansi.h"
#include "ThreadSharedData.h"

#include "Worker.h"
#include "ClientMgr.h"
#include "RiskManage/RiskManageCmd.h"

CClientMgr* CClientMgr::m_pClientMgr=NULL;

CClientMgr::CClientMgr(void)
: m_pWriteLog(NULL)
, m_pThreadShareData(NULL)
{
	m_pThreadShareData = new CThreadSharedData();

	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"ClientMgr.log");

	//创建Worker线程
	m_pWorker = new CWorker(m_pThreadShareData);
}

CClientMgr::~CClientMgr(void)
{
	if ( m_pWorker != NULL )
	{
		delete m_pWorker;
		m_pWorker = NULL;
	}

	if(m_pWriteLog != NULL)
	{
		m_pWriteLog->WriteLog_Fmt("关闭服务", WriteLogLevel_DEBUGINFO, "Line=%d", __LINE__ );
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}

	if ( NULL != m_pThreadShareData )
	{
		delete m_pThreadShareData;
		m_pThreadShareData = NULL;
	}
}

void CClientMgr::Init(void)
{
	if(m_pClientMgr)
	{
		Release();
	}
	m_pClientMgr=new CClientMgr();
}

void CClientMgr::Release(void)
{
	SAFE_DELETE(m_pClientMgr);
}

void CClientMgr::UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	if ( NULL == m_pClientMgr )
	{
		return;
	}

	m_pClientMgr->_UdpRecvFunc(pData, DataLen, SvrAddr, RemoteAddr, hSocket);
}

void CClientMgr::TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	if ( NULL == m_pClientMgr )
	{
		return;
	}

	m_pClientMgr->_TcpSvrAcceptFunc(SvrAddr, RemoteAddr, hSocket);
}

void CClientMgr::TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	if ( NULL == m_pClientMgr )
	{
		return;
	}

	m_pClientMgr->_TcpSvrDisconnFunc(SvrAddr, RemoteAddr, hSocket);
}

void CClientMgr::TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	if ( NULL == m_pClientMgr )
	{
		return;
	}

	m_pClientMgr->_TcpSvrRecvFunc(pData, DataLen, SvrAddr, RemoteAddr, hSocket);
}

void CClientMgr::TcpSvrOntimeFunc(void)
{
	if ( NULL == m_pClientMgr )
	{
		return;
	}

	m_pClientMgr->_TcpSvrOntimeFunc();
}

void CClientMgr::_UdpRecvFunc( char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket )
{

}

void CClientMgr::_TcpSvrAcceptFunc( Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket )
{
	Stru_DataSocket TmpDataSocket;
	TmpDataSocket.hScoket = hSocket;
	TmpDataSocket.pStreamCache = new CMemoryCache(655360);
	TmpDataSocket.RemoteAddr = RemoteAddr;
	time(&TmpDataSocket.HeartbeatTime);

	std::map<SOCKET, Stru_DataSocket>::iterator it = m_mapTcpUsers.find(hSocket);
	if( it != m_mapTcpUsers.end())
	{
		if(it->second.pStreamCache)
		{
			delete it->second.pStreamCache;
			it->second.pStreamCache = NULL;
		}

		m_mapTcpUsers.erase(it);
	}

	m_mapTcpUsers[hSocket]=TmpDataSocket;

	if(m_pWriteLog)
	{
		m_pWriteLog->WriteLog_Fmt("接受连接",WriteLogLevel_DEBUGINFO,"%s/%d-%d",
			CTools_Ansi::ConvertdwIP(TmpDataSocket.RemoteAddr.IP),
			TmpDataSocket.RemoteAddr.Port, hSocket);
	}
}

void CClientMgr::_TcpSvrDisconnFunc( Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket )
{
	std::map<SOCKET,Stru_DataSocket>::iterator it = m_mapTcpUsers.find(hSocket);
	if(it != m_mapTcpUsers.end())
	{
		if(m_pWriteLog)
		{
			m_pWriteLog->WriteLog_Fmt("断开连接",WriteLogLevel_DEBUGINFO,"%s/%d-%d",
				CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
				it->second.RemoteAddr.Port, hSocket);
		}

		if(it->second.pStreamCache)
		{
			delete it->second.pStreamCache;
			it->second.pStreamCache = NULL;
		}

		SOCKET hSocket = it->first;
		m_mapTcpUsers.erase(it);

		//更新在线用户
		if ( NULL != m_pThreadShareData )
		{
			m_pThreadShareData->OffLineUser(hSocket);
		}
	}
}

void CClientMgr::_TcpSvrRecvFunc( char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket )
{
	if( pData == NULL ) return;

	std::map<SOCKET,Stru_DataSocket>::iterator it = m_mapTcpUsers.find(hSocket);
	if( it != m_mapTcpUsers.end() && it->second.pStreamCache != NULL )
	{
		it->second.pStreamCache->SaveDataTo(pData,DataLen);
		ProcessPackage( it->second );
	}
}

void CClientMgr::_TcpSvrOntimeFunc( void )
{
	if ( m_mapTcpUsers.empty() )
	{
		return;
	}

	time_t curTime;
	time(&curTime);

	//检查超时连接
	std::map<SOCKET,Stru_DataSocket>::iterator it = m_mapTcpUsers.begin();
	std::vector<SOCKET> vTemp;
	vTemp.clear();
	for( ;it != m_mapTcpUsers.end(); ++it)
	{
		int nDeltaSecs = (int)(curTime-it->second.HeartbeatTime);
		if( nDeltaSecs > 30000 )	//30秒超时
		{
			if(m_pWriteLog)
			{
				m_pWriteLog->WriteLog_Fmt("连接超时，断开连接",WriteLogLevel_DEBUGINFO,"%s/%d 超时时长%ds",
					CTools_Ansi::ConvertdwIP(it->second.RemoteAddr.IP),
					it->second.RemoteAddr.Port, nDeltaSecs);
			}

			vTemp.push_back( it->first );
		}
	}

	for ( int i = 0; i < static_cast<int>(vTemp.size()); i++ )
	{
		CSocketBase::CloseSocket(vTemp[i]);
	}
}

void CClientMgr::ProcessPackage( Stru_DataSocket& soketData )
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
		if ( nCacheLen < static_cast<int>(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			return;
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

		// 将合法请求信息加入消息队列，待Work线程处理
		if ( VerifyRequest(pkgHead))
		{
			time(&soketData.HeartbeatTime);
			Stru_UniPkgHead head;
			soketData.pStreamCache->GetFrom(&head, sizeof(Stru_UniPkgHead));
			SaveReqToMsgQueue(soketData, head);
			if(m_pWriteLog)
			{
				m_pWriteLog->WriteLog_Fmt("接收业务数据",WriteLogLevel_DEBUGINFO,"%s/%d moduleID=%d, CMDID=%d",
					CTools_Ansi::ConvertdwIP(soketData.RemoteAddr.IP),
					soketData.RemoteAddr.Port, head.moduleid, head.cmdid);
			}

			continue;
		}

		//检查并抛弃错误包，如果是一个完整包，但在前面得不到处理，则认为是一个错误包，抛弃
		if( nCacheLen >= (int)(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			soketData.pStreamCache->DeleteData( sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL) );
			if(m_pWriteLog)
			{
				m_pWriteLog->WriteLog_Fmt("抛弃不合法数据",WriteLogLevel_DEBUGINFO,"");
			}
			continue;
		}

		break;
	}
}

void CClientMgr::SaveReqToMsgQueue( Stru_DataSocket& soketData, Stru_UniPkgHead& head )
{
	BYTE* pBuf = NULL;
	if ( head.len != 0 )
	{
		pBuf = new BYTE[head.len];
		if ( NULL == pBuf )
		{
			soketData.pStreamCache->DeleteData(head.len+sizeof(TAIL));
			if(m_pWriteLog)
				m_pWriteLog->WriteLog_Fmt("内存不够",WriteLogLevel_DEBUGINFO, "%d", head.cmdid);
			return;
		}

		memset( pBuf, 0, head.len );
		soketData.pStreamCache->GetFrom(pBuf, head.len);
	}

	TAIL tail;
	soketData.pStreamCache->GetFrom(&tail, sizeof(TAIL));
	if ( tail == TailID_UniPkg )
	{
		if ( NULL != m_pThreadShareData )
		{
			m_pThreadShareData->AddMsg(RequestData(soketData.hScoket, head, pBuf));
		}
	}
	else
	{
		if(m_pWriteLog)
			m_pWriteLog->WriteLog_Fmt("包尾不一致",WriteLogLevel_DEBUGINFO, "%d", tail);

		if ( NULL != pBuf )
		{
			delete[] pBuf;
			pBuf = NULL;
		}
	}

	return;
}

bool CClientMgr::VerifyRequest( Stru_UniPkgHead& head )
{
	return true;
}
