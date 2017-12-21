#include "StdAfx.h"
#include "SendData.h"
#include "UDPDataService.h"
#include "MemoryCache.h"
#include "Tools_Ansi.h"

using namespace std;

#define TIMER_SERVER_ADDR_CHECK	1
#define TIMER_HEARTBEAT			2

CUDPDataService* CUDPDataService::m_pInstance = NULL;

//从tcp得到数据后，调用此回调函数进行处理
void CUDPDataService::CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if( m_pInstance == NULL || NULL == m_pInstance->m_pStreamCache || NULL == pData )
		return;

	RecvData recvData;
	memset(&recvData, 0, sizeof(recvData));
	memcpy(&recvData.head, pData, sizeof(Stru_UniPkgUDPHead));
	if( recvData.head.cmdid == CMDID_HEARTBEAT )
		return;

	if ( recvData.head.len != 0 )
	{
		recvData.pData = new char[recvData.head.len];
		memcpy(recvData.pData, pData+sizeof(Stru_UniPkgUDPHead), recvData.head.len);	
	}

	//服务端发过来的数据
	CSendData::Get()->AddMsg(recvData);

	return;
/*

	m_pInstance->m_pStreamCache->SaveDataTo(pData, DataLen);

	int nCacheLen = 0;
	while( (nCacheLen = m_pInstance->m_pStreamCache->GetLen()) >= sizeof(Stru_UniPkgUDPHead) + sizeof(TAIL))
	{
		char szHeadBuf[sizeof(Stru_UniPkgUDPHead)];
		memset(szHeadBuf, 0, sizeof(Stru_UniPkgUDPHead));
		m_pInstance->m_pStreamCache->PeekFrom(szHeadBuf, sizeof(Stru_UniPkgUDPHead));
		int nPackageHead = *(DWORD*)szHeadBuf;

		// 检查并抛弃包头错误的部分
		if( nPackageHead != HeadID_UniPkg)
		{
			int i = 0;
			for( ; i < sizeof(Stru_UniPkgUDPHead) - sizeof(DWORD) + 1; i++)
			{
				if(*(DWORD*)(szHeadBuf + i) == HeadID_UniPkg )
				{
					break;
				}
			}

			m_pInstance->m_pStreamCache->DeleteData(i);
			continue;
		}

		Stru_UniPkgUDPHead pkgHead;
		m_pInstance->m_pStreamCache->PeekFrom(&pkgHead, sizeof(Stru_UniPkgUDPHead));
		//等待一个包收齐
		if ( nCacheLen < (int)(sizeof(Stru_UniPkgUDPHead) + pkgHead.len + sizeof(TAIL)) )
		{
			return;
		}
		
		//检查、处理心跳包
		if( pkgHead.cmdid == CMDID_HEARTBEAT 
			&& pkgHead.len == 0 )
		{
			Stru_Heartbeat heartBeatPkg;
			memset(&heartBeatPkg, 0, sizeof(heartBeatPkg));
			m_pInstance->m_pStreamCache->GetFrom(&heartBeatPkg, sizeof(Stru_Heartbeat));

			continue;
		}
		if(0x18020001 != pkgHead.cmdid)
		{
			int  p=0 ;
		}
		// 判断数据是否合法
		if ( VerifyRequest(pkgHead))
		{
			//处理业务返回数据
			RecvData recvData;
			memset(&recvData, 0, sizeof(recvData));
			m_pInstance->m_pStreamCache->GetFrom(&recvData.head, sizeof(Stru_UniPkgUDPHead));
			
			if ( recvData.head.len != 0 )
			{
				recvData.pData = new char[recvData.head.len];
				m_pInstance->m_pStreamCache->GetFrom(recvData.pData, recvData.head.len);
				
			}

			TAIL tail;
			m_pInstance->m_pStreamCache->GetFrom(&tail, sizeof(tail));

			if ( tail != TailID_UniPkg )
			{
				//包尾不一致
				if ( recvData.pData != NULL )
				{
					delete[] recvData.pData;
					recvData.pData = NULL;
				}
			}
			else
			{
				//服务端发过来的数据
				CSendData::Get()->AddMsg(recvData);
				//保存数据
			}

			continue;
		}

		//检查并抛弃错误包，如果是一个完整包，但在前面得不到处理，则认为是一个错误包，抛弃
		if( nCacheLen >= (int)(sizeof(Stru_UniPkgUDPHead) + pkgHead.len + sizeof(TAIL)) )
		{
			m_pInstance->m_pStreamCache->DeleteData( sizeof(Stru_UniPkgUDPHead) + pkgHead.len + sizeof(TAIL) );
			continue;
		}

		break;
	}*/
}



CUDPDataService* CUDPDataService::GetInstance()
{
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CUDPDataService();
		//这个定时器作为dll的方式使用的时候，OnTimer_HeartBeat得不到执行，
		//所以专门做了一个线程ThreadTimer来做Timer
	//	SetTimer(NULL, TIMER_HEARTBEAT, 8000, OnTimer_HeartBeat);
	}

	return m_pInstance;
}

void CUDPDataService::DestroyInstance()
{
	if( m_pInstance != NULL) 
	{
		KillTimer(NULL, TIMER_HEARTBEAT);
		Sleep(5);
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CUDPDataService::CUDPDataService(void) 
: m_pTcpClient(NULL)
, m_pStreamCache(NULL)
, m_nNetStatus(enmNetNothing)
, m_hWorkerThread(NULL)
, m_bWorkerThreadExit(false)
, m_bLogon(false)
, m_strLastError("")
{
	InitializeCriticalSection(&m_csHWnd);
	InitializeCriticalSection(&m_csMsgQueue);
	memset(&m_addr, 0, sizeof(m_addr));
	m_pStreamCache = new CMemoryCache(5*1024*1024);
	m_hWndSet.clear();

	//创建线程，用于上抛数据
	DWORD dwThreadID;
	m_hWorkerThread = CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)ThreadFunc, this, 0, &dwThreadID);


}

CUDPDataService::~CUDPDataService(void)
{
	if( m_pTcpClient != NULL ) 
	{
		delete m_pTcpClient;
		m_pTcpClient = NULL;
	}

	if ( NULL != m_pStreamCache )
	{
		delete m_pStreamCache;
		m_pStreamCache = NULL;
	}

	if( m_hWorkerThread )
	{
		m_bWorkerThreadExit = true;
		DWORD dwExitCode;
		WaitForSingleObject( m_hWorkerThread, 8000 );
		if( GetExitCodeThread(m_hWorkerThread, &dwExitCode) !=0 && dwExitCode == STILL_ACTIVE )
		{
			TerminateThread( m_hWorkerThread, 0 );
		}

		CloseHandle( m_hWorkerThread );
		m_hWorkerThread = NULL;
	}

	while( !m_msgQueue.empty() )
	{
		RecvData recvData = m_msgQueue.front();
		m_msgQueue.pop();
		if ( NULL != recvData.pData )
		{
			delete[] recvData.pData;
			recvData.pData = NULL;
		}
	}

	m_nNetStatus = enmNetNothing;
	DeleteCriticalSection(&m_csHWnd);
	DeleteCriticalSection(&m_csMsgQueue);
}

bool CUDPDataService::Connect(std::string strIP, int nPort)
{
	if( m_pTcpClient != NULL ) 
	{
		delete m_pTcpClient;
		m_pTcpClient = NULL;
	}

	if ( NULL != m_pStreamCache )
	{
		m_pStreamCache->Empty();
	}

	Stru_IPAddress tIPAddr, tServAddr;
	ZeroMemory(&tIPAddr, sizeof(tIPAddr));
	ZeroMemory(&tServAddr, sizeof(tServAddr));
	
	tServAddr.IP = CTools_Ansi::ConvertStrIP((char*)strIP.c_str());
	tServAddr.Port = nPort;
	m_pTcpClient = new CUdpSimpleWorker(tServAddr, 
			CallbackReceiveData, NULL, 65536000, 65536000);
	//SetNetStatus(enmNetConnectWaiting);
	SetNetStatus(enmNetConnectSuccess);

	return true;
}

bool CUDPDataService::SendData(Stru_IPAddress RemoteAddr, UINT nCMDID, void* pData, int nLen, UINT seq, bool bNeedLogon, int nUserData1)
{
	//if ( bNeedLogon && !m_bLogon )
	//{
	//	m_strLastError = "操作错误，账号未登录！";
	//	return false;
	//}

	if( m_pTcpClient != NULL && m_nNetStatus == enmNetConnectSuccess) 
	{
		if ( pData == NULL && nLen != 0)
		{
			m_strLastError = "请求数据异常！";
			return false;
		}

		Stru_UniPkgUDPHead head;
		head.moduleid = 0;
		head.cmdid = nCMDID;
		head.seq = seq;
		head.len = nLen;
		head.userdata1 = nUserData1;

		int nTotalLen = sizeof(Stru_UniPkgUDPHead)+head.len+sizeof(TAIL);
		char* pBuf = new char[nTotalLen];
		memset(pBuf, 0, sizeof(nTotalLen));
		auto_ptr<char> ptrBuf( pBuf );
		if ( NULL == pBuf )
		{
			m_strLastError = "系统内存不够！";
			return false;
		}

		memcpy(pBuf, &head, sizeof(Stru_UniPkgUDPHead));
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), pData, nLen);
		TAIL tail = TailID_UniPkg;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+head.len, &tail, sizeof(TAIL));

		int nLen = m_pTcpClient->SendData(pBuf, nTotalLen, RemoteAddr);
		if(nLen > 0)
			return true;

		return false;
	}
	else 
	{
		m_strLastError = "网络连接异常，发送请求失败！";
		return false;
	}
}

void CALLBACK CUDPDataService::OnTimer_HeartBeat(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if ( NULL != m_pInstance )
	{
	//	if( m_pInstance->m_nNetStatus == enmNetConnectSuccess) 
		{
		//	Stru_IPAddress address = m_pInstance->GetAddress();
		//	m_pInstance->SendData(address, CMDID_HEARTBEAT, 0, 0, false);
		}
	}
}

void CUDPDataService::RegisteHWnd( HWND hWnd )
{
	EnterCriticalSection(&m_csHWnd);
	m_hWndSet.insert(hWnd);
	LeaveCriticalSection(&m_csHWnd);
}

void CUDPDataService::UnRegisteHWnd( HWND hWnd )
{
	EnterCriticalSection(&m_csHWnd);
	m_hWndSet.erase(hWnd);
	LeaveCriticalSection(&m_csHWnd);
}

bool CUDPDataService::IsConnected()
{
	return m_nNetStatus == enmNetConnectSuccess;
}

bool CUDPDataService::VerifyRequest( Stru_UniPkgUDPHead& head )
{
	return true;
}
DWORD CUDPDataService::ThreadFunc( void* pData )
{
	CUDPDataService* pWorker = (CUDPDataService*)pData;
	if ( NULL == pWorker )
	{
		return 0;
	}

	while( !pWorker->m_bWorkerThreadExit )
	{
		EnterCriticalSection(&pWorker->m_csMsgQueue);
		if ( pWorker->m_msgQueue.empty())
		{
			LeaveCriticalSection(&pWorker->m_csMsgQueue);
			Sleep(1);
			continue;
		}

		RecvData recvData = pWorker->m_msgQueue.front();
		pWorker->m_msgQueue.pop();

		LeaveCriticalSection(&pWorker->m_csMsgQueue);

		//发送数据给UI层处理
		EnterCriticalSection(&pWorker->m_csHWnd);
		std::set<HWND> tempSet = m_pInstance->m_hWndSet;
		LeaveCriticalSection(&pWorker->m_csHWnd);

		std::set<HWND>::iterator it = tempSet.begin();
		for (; it != tempSet.end(); ++it )
		{
			if ( IsWindow(*it))
			{
				SendMessage(*it, RECV_DATA_MSG, 0, (LPARAM)(&recvData) );
			}
		}

		if ( recvData.pData != NULL )
		{
			delete [] recvData.pData;
			recvData.pData = NULL;
		}
	}

	return 0;
}

std::string CUDPDataService::GetLastError()
{
	return m_strLastError;
}

void CUDPDataService::SetLogonStatus( bool bLogon )
{
	EnterCriticalSection(&m_csHWnd);
	m_bLogon = bLogon;
	LeaveCriticalSection(&m_csHWnd);
}

bool CUDPDataService::IsLogon()
{
	return m_bLogon;
}
