#include "StdAfx.h"
#include "TCPDataService.h"
#include "Tools_Ansi.h"

using namespace std;

#define TIMER_SERVER_ADDR_CHECK	1
#define TIMER_HEARTBEAT			2

CTCPDataService* CTCPDataService::m_pInstance = NULL;

//定时检查licence服务器地址是否就绪，就绪时，发起连接
VOID CALLBACK CTCPDataService::OnTimer1(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if( m_pInstance == NULL)
		return;

	if ( m_pInstance->m_step == REQ_STEP_INIT && m_pInstance->m_nNetStatus == enmNetConnectSuccess )
	{
		//请求服务地址
		char szBuffer[256];
		strcpy(szBuffer, "LicenceVarify.dll");
		m_pInstance->SendData(CMDID_SERVER_ADDR_REQ, szBuffer, sizeof(szBuffer), 0);
		m_pInstance->m_step = REQ_STEP_REQ_SERVER_ADDR;
		return;
	}

	if ( m_pInstance->m_step == REQ_STEP_SERVER_ADDR_RECVED )
	{
		m_pInstance->Connect( CTools_Ansi::ConvertdwIP(m_pInstance->m_addr.TCPIP), m_pInstance->m_addr.TCPPort);
		m_pInstance->m_step = REQ_STEP_CONNECT_TO_SERVER;
		return;
	}

	if ( m_pInstance->m_step == REQ_STEP_CONNECT_TO_SERVER && m_pInstance->m_nNetStatus == enmNetConnectSuccess)
	{
		m_pInstance->SendData(CMDID_LICENCE_LIST_REQ, NULL, 0, 0);
		m_pInstance->m_step = REQ_STEP_REQ_LICENCE_LIST;
		return;
	}

	if ( m_pInstance->m_step == REQ_STEP_LICENCE_LIST_RECVED)
	{
		KillTimer(NULL, TIMER_SERVER_ADDR_CHECK);
	}
}

VOID CALLBACK CTCPDataService::OnTimer2(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if( m_pInstance == NULL)
		return;

	m_pInstance->OnTimer_HeartBeat(uMsg, idEvent, dwTime);
}

//从tcp得到数据后，调用此回调函数进行处理
void CTCPDataService::CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
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

		// 将合法请求信息加入消息队列，待Work线程处理
		if ( VerifyRequest(pkgHead))
		{
			Stru_UniPkgHead head;
			m_pInstance->m_pStreamCache->GetFrom(&head, sizeof(Stru_UniPkgHead));
			//处理业务返回数据
			if ( head.cmdid == CMDID_SERVER_ADDR_RSP)
			{
				//收到licence服务IP
				m_pInstance->m_step = REQ_STEP_SERVER_ADDR_RECVED;
				memset(&m_pInstance->m_addr, 0, sizeof(m_pInstance->m_addr));
				m_pInstance->m_pStreamCache->GetFrom(&m_pInstance->m_addr, sizeof(Stru_ServerAddr));
			}
			else if ( head.cmdid == CMDID_LICENCE_LIST_RSP )
			{
				m_pInstance->m_licenceMap.clear();
				m_pInstance->m_step = REQ_STEP_LICENCE_LIST_RECVED;
				for ( size_t i = 0; i < head.len / sizeof(Stru_Licence); i++)
				{
					Stru_Licence licence;
					memset(&licence, 0, sizeof(licence));
					m_pInstance->m_pStreamCache->GetFrom(&licence, sizeof(Stru_Licence));
					m_pInstance->m_licenceMap[licence.szAppID] = licence.szLicence;
				}
			}

			TAIL tail;
			m_pInstance->m_pStreamCache->GetFrom(&tail, sizeof(tail));
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
void CTCPDataService::CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetConnectWaiting);
	}
}

//断开tcp连接后，调用此回调函数进行处理
void CTCPDataService::CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetDisconnect);
	}
}

//连接成功后，调用此回调函数进行处理
void CTCPDataService::CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetConnectSuccess);
	}
}

//连接失败后，调用此回调函数进行处理
void CTCPDataService::CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetDisconnect);
	}
}

CTCPDataService* CTCPDataService::GetInstance()
{
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CTCPDataService();
		SetTimer(NULL, TIMER_SERVER_ADDR_CHECK, 100, OnTimer1);
		SetTimer(NULL, TIMER_HEARTBEAT, 10000, OnTimer2);
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
, m_step(REQ_STEP_INIT)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_licenceMap.clear();
	m_pStreamCache = new CMemoryCache(10240);
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

	m_nNetStatus = enmNetNothing;
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
	
	tServAddr.IP = CTools_Ansi::ConvertStrIP((char*)strIP.c_str());
	tServAddr.Port = nPort;
	m_pTcpClient = new CTcpClientWorker(tIPAddr, tServAddr, 
			CallbackReceiveData, CallbackDisconnect, CallbackStartConnect, 
			CallbackConnectSuccess, CallbackConnectFail, NULL);
	SetNetStatus(enmNetConnectWaiting);

	return true;
}

bool CTCPDataService::SendData(UINT nCMDID, void* pData, int nLen, UINT seq)
{
	if( m_pTcpClient != NULL && m_nNetStatus == enmNetConnectSuccess) 
	{
		if ( pData == NULL && nLen != 0)
		{
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
		return false;
	}
}

void CTCPDataService::OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if( m_nNetStatus == enmNetConnectSuccess) 
	{
		SendData(CMDID_HEARTBEAT, NULL, 0);
	}
}

int CTCPDataService::GetPackageHeadTailLen()
{
	return (int)sizeof(Stru_UniPkgHead)+(int)sizeof(DWORD);
}

int CTCPDataService::GetPackageHeadLen()
{
	return (int)sizeof(Stru_UniPkgHead);
}

bool CTCPDataService::VerifyRequest( Stru_UniPkgHead& head )
{
	switch(head.cmdid)
	{
	case CMDID_LICENCE_LIST_RSP:
		return head.len == sizeof(Stru_Licence);
	case CMDID_SERVER_ADDR_RSP:
		return head.len == sizeof(Stru_ServerAddr);
	default:
		return false;
	}
}

bool CTCPDataService::LicenceRecved()
{
	return m_step >= REQ_STEP_LICENCE_LIST_RECVED;
}

bool CTCPDataService::GetLicence( std::string strAppID, std::string& strLicence )
{
	std::map<std::string, std::string>::iterator it = m_licenceMap.find(strAppID);
	if ( it != m_licenceMap.end() )
	{
		strLicence = it->second;
		return true;
	}
	else
	{
		return false;
	}
}


