#include "StdAfx.h"
#include "TCPDataService.h"
#include "Tools_Ansi.h"

using namespace std;

#define TIMER_SERVER_ADDR_CHECK	1
#define TIMER_HEARTBEAT			2

CTCPDataService* CTCPDataService::m_pInstance = NULL;

//从tcp得到数据后，调用此回调函数进行处理
void CTCPDataService::CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if( m_pInstance == NULL || NULL == m_pInstance->m_pStreamCache || NULL == pData )
		return;

	m_pInstance->m_pStreamCache->SaveDataTo(pData, DataLen);

	int nCacheLen = 0;
	while( (nCacheLen = m_pInstance->m_pStreamCache->GetLen()) >= sizeof(Stru_UniPkgHead) + sizeof(TAIL))
	{
		char szHeadBuf[sizeof(Stru_UniPkgHead)];
		memset(szHeadBuf, 0, sizeof(Stru_UniPkgHead));
		m_pInstance->m_pStreamCache->PeekFrom(szHeadBuf, sizeof(Stru_UniPkgHead));
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

			m_pInstance->m_pStreamCache->DeleteData(i);
			continue;
		}

		Stru_UniPkgHead pkgHead;
		m_pInstance->m_pStreamCache->PeekFrom(&pkgHead, sizeof(Stru_UniPkgHead));
		//等待一个包收齐
		if ( nCacheLen < (int)(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			return;
		}

		// 判断数据是否合法
		if ( VerifyRequest(pkgHead))
		{
			//处理业务返回数据
			RecvData recvData;
			memset(&recvData, 0, sizeof(recvData));
			m_pInstance->m_pStreamCache->GetFrom(&recvData.head, sizeof(Stru_UniPkgHead));
			
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
				//保存数据
				EnterCriticalSection(&m_pInstance->m_csMsgQueue);
				m_pInstance->m_msgQueue.push(recvData);
				LeaveCriticalSection(&m_pInstance->m_csMsgQueue);		
			}

			continue;
		}

		//检查、处理心跳包
		if( pkgHead.cmdid == CMDID_HEARTBEATRSP 
			&& pkgHead.len == 0 )
		{
			Stru_Heartbeat heartBeatPkg;
			memset(&heartBeatPkg, 0, sizeof(heartBeatPkg));
			m_pInstance->m_pStreamCache->GetFrom(&heartBeatPkg, sizeof(Stru_Heartbeat));

			continue;
		}

		//检查并抛弃错误包，如果是一个完整包，但在前面得不到处理，则认为是一个错误包，抛弃
		if( nCacheLen >= (int)(sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL)) )
		{
			m_pInstance->m_pStreamCache->DeleteData( sizeof(Stru_UniPkgHead) + pkgHead.len + sizeof(TAIL) );
			continue;
		}

		break;
	}
}

//开始调用connect()后，调用此函数通知应用层
void CTCPDataService::CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if( m_pInstance != NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetConnectWaiting);
	}
}

//断开tcp连接后，调用此回调函数进行处理
void CTCPDataService::CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if( m_pInstance != NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetDisconnect);
		m_pInstance->SetLogonStatus(false);
	}
}

//连接成功后，调用此回调函数进行处理
void CTCPDataService::CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if( m_pInstance != NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetConnectSuccess);
	}
}

//连接失败后，调用此回调函数进行处理
void CTCPDataService::CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if( m_pInstance != NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetDisconnect);
	}
}

CTCPDataService* CTCPDataService::GetInstance()
{
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CTCPDataService();
	}

	return m_pInstance;
}

void CTCPDataService::DestroyInstance()
{
	if( m_pInstance != NULL) 
	{
		KillTimer(NULL, TIMER_HEARTBEAT);
		Sleep(5);
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CTCPDataService::CTCPDataService(void) 
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
	m_pStreamCache = new CMemoryCache(655360);
	m_hWndSet.clear();

	//创建线程，用于上抛数据
	DWORD dwThreadID;
	m_hWorkerThread = CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)ThreadFunc, this, 0, &dwThreadID);
}

CTCPDataService::~CTCPDataService(void)
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

bool CTCPDataService::Connect(std::string strIP, int nPort)
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
	
	//tServAddr.IP = CTools_Ansi::ConvertStrIP((char*)strIP.c_str());
	//CTools_Ansi::GetIPFromHostName((char*)strIP.c_str(), (DWORD*)&tServAddr.IP, 1);
	CTools_Ansi::PerformWSAStartup();
	CTools_Ansi::GetIPFromHostName((char*)strIP.c_str(), (DWORD*)&tServAddr.IP, 1);
	CTools_Ansi::PerformWSACleanup();
	tServAddr.Port = nPort;
	m_pTcpClient = new CTcpClientWorker(tIPAddr, tServAddr, 
			CallbackReceiveData, CallbackDisconnect, CallbackStartConnect, 
			CallbackConnectSuccess, CallbackConnectFail, NULL);
	SetNetStatus(enmNetConnectWaiting);

	return true;
}

bool CTCPDataService::SendData(UINT nCMDID, void* pData, int nLen, UINT seq, bool bNeedLogon)
{
	if ( bNeedLogon && !m_bLogon )
	{
		m_strLastError = "操作错误，账号未登录！";
		return false;
	}

	if( m_pTcpClient != NULL && m_nNetStatus == enmNetConnectSuccess) 
	{
		if ( pData == NULL && nLen != 0)
		{
			m_strLastError = "请求数据异常！";
			return false;
		}

		Stru_UniPkgHead head;
		head.moduleid = 0;
		head.cmdid = nCMDID;
		head.seq = seq;
		head.len = nLen;

		int nTotalLen = sizeof(Stru_UniPkgHead)+head.len+sizeof(TAIL);
		char* pBuf = new char[nTotalLen];
		memset(pBuf, 0, sizeof(nTotalLen));
		auto_ptr<char> ptrBuf( pBuf );
		if ( NULL == pBuf )
		{
			m_strLastError = "系统内存不够！";
			return false;
		}

		memcpy(pBuf, &head, sizeof(Stru_UniPkgHead));
		memcpy(pBuf+sizeof(Stru_UniPkgHead), pData, nLen);
		TAIL tail = TailID_UniPkg;
		memcpy(pBuf+sizeof(Stru_UniPkgHead)+head.len, &tail, sizeof(TAIL));

		m_pTcpClient->SendData(pBuf, nTotalLen);
		return true;
	}
	else 
	{
		m_strLastError = "网络连接异常，发送请求失败！";
		return false;
	}
}

void CTCPDataService::OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if( m_nNetStatus == enmNetConnectSuccess) 
	{
		SendData(CMDID_HEARTBEAT, NULL, 0, 0, false);
	}
}

void CTCPDataService::RegisteHWnd( HWND hWnd )
{
	EnterCriticalSection(&m_csHWnd);
	m_hWndSet.insert(hWnd);
	LeaveCriticalSection(&m_csHWnd);
}

void CTCPDataService::UnRegisteHWnd( HWND hWnd )
{
	EnterCriticalSection(&m_csHWnd);
	m_hWndSet.erase(hWnd);
	LeaveCriticalSection(&m_csHWnd);
}

bool CTCPDataService::IsConnected()
{
	return m_nNetStatus == enmNetConnectSuccess;
}

bool CTCPDataService::VerifyRequest( Stru_UniPkgHead& head )
{
	switch(head.cmdid)
	{
	case Cmd_MA_UnLogon_Rsp:
	case Cmd_MA_Login_Rsp:
	case Cmd_MA_ModifyPwd_Rsp:
	case Cmd_MA_QryBroker_Rsp:
	case Cmd_MA_AddBroker_Rsp:
	case Cmd_MA_DelBroker_Rsp:
	case Cmd_MA_ModifyBroker_Rsp:
	case Cmd_MA_ForceCloseClient_Rsp:

	case Cmd_MA_AddServerGroup_Rsp:
	case Cmd_MA_DelServerGroup_Rsp:
	case Cmd_MA_ModifyServerGroup_Rsp:
	case Cmd_MA_QryServerGroup_Rsp:
	case Cmd_MA_AddServerAddr_Rsp:
	case Cmd_MA_DelServerAddr_Rsp:
	case Cmd_MA_ModifyServerAddr_Rsp:
	case Cmd_MA_QryServerAddr_Rsp:

	case Cmd_MA_AddPermissionModule_Rsp:
	case Cmd_MA_DelPermissionModule_Rsp:
	case Cmd_MA_ModifyPermissionModule_Rsp:
	case Cmd_MA_QryPermissionModule_Rsp:

	case Cmd_MA_AddPermission_Rsp:
	case Cmd_MA_DelPermission_Rsp:
	case Cmd_MA_ModifyPermission_Rsp:
	case Cmd_MA_QryPermissionByOwner_Rsp:
	case Cmd_MA_QryPermissionByID_Rsp:
		return true;

	case Cmd_MA_QryAccount_Rsp:
	case Cmd_MA_AddAccount_Rsp:
	case Cmd_MA_DelAccount_Rsp:
	case Cmd_MA_ModifyAccount_Rsp:
	case Cmd_MA_DelAccountCaseCade_Rsp:
		return true;

	case Cmd_MA_AddIDCard_Rsp:
	case Cmd_MA_DelIDCard_Rsp:
	case Cmd_MA_ModifyIDCard_Rsp:
	case Cmd_MA_QryIDCard_Rsp:
		return true;

	case Cmd_MA_AddMessage_Rsp:
	case Cmd_MA_QryMessage_Rsp:
	case Cmd_MA_QryTargetAccount_Rsp:
	case Cmd_MA_QryWaitForSendAccount_Rsp:
		return true;

	default:
		return false;
	}
}

DWORD CTCPDataService::ThreadFunc( void* pData )
{
	CTCPDataService* pWorker = (CTCPDataService*)pData;
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

std::string CTCPDataService::GetLastError()
{
	return m_strLastError;
}

void CTCPDataService::SetLogonStatus( bool bLogon )
{
	EnterCriticalSection(&m_csHWnd);
	m_bLogon = bLogon;
	LeaveCriticalSection(&m_csHWnd);
}

bool CTCPDataService::IsLogon()
{
	return m_bLogon;
}
