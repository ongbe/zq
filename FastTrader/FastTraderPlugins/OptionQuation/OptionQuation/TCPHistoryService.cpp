#include "StdAfx.h"
#include "TCPHistoryService.h"
#include "Tools_Ansi.h"
#include "FileOpr.h "

using namespace std;
#include "QuoteCommon.h"
#define TIMER_SERVER_ADDR_CHECK	1
#define TIMER_HEARTBEAT			2
//UINT_PTR g_lTimeQuto;
#if 0
#define LOG_INFO(fmt, ...)  ;
#else
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","PlatformCTPQuot", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif

//////////////////////////////////////////////////////////////////////////
HWND CTCPHistoryService::m_HwndParent = NULL;
CTCPHistoryService* CTCPHistoryService::m_pInstance = NULL;
//int g_QuotServerSeqID = 0;
//定时检查licence服务器地址是否就绪，就绪时，发起连接
VOID CALLBACK CTCPHistoryService::OnTimer1(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if( m_pInstance == NULL)
		return;

	if ( m_pInstance->m_step == REQ_STEP_LICENCE_LIST_RECVED)
	{
		KillTimer(NULL, TIMER_SERVER_ADDR_CHECK);
	}
}
void CTCPHistoryService::CallbackOntimeFunc(int UserData)
{
	if( m_pInstance == NULL)
		return;

	if( m_pInstance->GetNetStatus() == enmNetConnectSuccess) 
	{
		m_pInstance->SendData(CMDID_HEARTBEAT, NULL, 0);
	}	
}

//从tcp得到数据后，调用此回调函数进行处理
void CTCPHistoryService::CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
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

		//检查、处理心跳包
		if( pkgHead.cmdid == CMDID_HEARTBEATRSP 
			&& pkgHead.len == 0 )
		{
			Stru_Heartbeat heartBeatPkg;
			memset(&heartBeatPkg, 0, sizeof(heartBeatPkg));
			m_pInstance->m_pStreamCache->GetFrom(&heartBeatPkg, sizeof(Stru_Heartbeat));

			TAIL tail;
			m_pInstance->m_pStreamCache->GetFrom(&tail, sizeof(tail));

	//		OFFER_INFO("CMDID_HEARTBEATRSP \n");
			continue;
		}


		// 将合法请求信息加入消息队列，待Work线程处理
		if ( VerifyRequest(pkgHead))
		{
			Stru_UniPkgHead head;
			m_pInstance->m_pStreamCache->GetFrom(&head, sizeof(Stru_UniPkgHead));
			//处理业务返回数据			
			if(pkgHead.cmdid == Cmd_QueryQuotTimeLineData_Rsp)
			{//处理行情
				std::vector<TimeLineData> vecTimeLineData;
			    if ( pkgHead.userdata1 == CF_ERROR_SUCCESS )
				{
					int nCount = pkgHead.len/sizeof(TimeLineData);
					for(int i =0; i<nCount; i++)
					{
						TimeLineData timeLineData;
						ZeroMemory(&timeLineData, sizeof(TimeLineData));
						m_pInstance->m_pStreamCache->GetFrom(&timeLineData, sizeof(TimeLineData));
						vecTimeLineData.push_back(timeLineData);
					}	
				}
				::SendMessage(m_HwndParent, MESSAGE_QutoTLineHistory, (WPARAM)(&vecTimeLineData), (LPARAM)(head.seq));					
			}
			else if(pkgHead.cmdid == Cmd_QueryQuotKLineData_Rsp)
			{//处理行情
				 std::vector<KLineData> vecKLineData;
				 if ( pkgHead.userdata1 == CF_ERROR_SUCCESS )
				 {
					 int nCount = pkgHead.len/sizeof(KLineData);
					for(int i =0; i<nCount; i++)
					{
						KLineData timeLineData;
						ZeroMemory(&timeLineData, sizeof(KLineData));
						m_pInstance->m_pStreamCache->GetFrom(&timeLineData, sizeof(KLineData));
						vecKLineData.push_back(timeLineData);
					}				
				 }
				::SendMessage(m_HwndParent, MESSAGE_QutoKLineHistory, (WPARAM)(&vecKLineData), (LPARAM)(head.seq));			
			
			}
			TAIL tail;
			m_pInstance->m_pStreamCache->GetFrom(&tail, sizeof(tail));
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
void CTCPHistoryService::CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		m_pInstance->SetNetStatus(enmNetConnectWaiting);
	}
}

//断开tcp连接后，调用此回调函数进行处理
void CTCPHistoryService::CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		std::string strIP = CTools_Ansi::ConvertdwIP(SvrAddr.IP);
	//	OFFER_INFO("断开行情服务器：IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
	//	PUSH_LOG_ERROR(FirstLevelError,true,true,"断开行情服务器：IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
		m_pInstance->SetNetStatus(enmNetDisconnect);
		
		int n = 1;
		::SendMessage(m_HwndParent, MESSAGE_CONNECTFAIL, (WPARAM)&n, NULL);
	}
}

//连接成功后，调用此回调函数进行处理
void CTCPHistoryService::CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		std::string strIP = CTools_Ansi::ConvertdwIP(SvrAddr.IP);
	//	OFFER_INFO("连接行情服务器成功：IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
	//	PUSH_LOG_ERROR(Logger,true,true,"连接行情服务器成功：IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
		m_pInstance->SetNetStatus(enmNetConnectSuccess);
		m_pInstance->m_pStreamCache->Empty();//清空缓存区，里面有脏数据
		
		int n = 1;
		::SendMessage(m_HwndParent, MESSAGE_CONNECTSUCCESS, (WPARAM)&n, NULL);
	
	}
}

//连接失败后，调用此回调函数进行处理
void CTCPHistoryService::CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	if( m_pInstance !=NULL ) 
	{
		std::string strIP = CTools_Ansi::ConvertdwIP(SvrAddr.IP);
//		OFFER_INFO("连接行情服务器失败：IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
	//	PUSH_LOG_ERROR(FirstLevelError,true,true,"连接行情服务器失败：IP[%s], Port[%d]\n", strIP.c_str(),  SvrAddr.Port);
		m_pInstance->SetNetStatus(enmNetDisconnect);
		int n = 1;
		::SendMessage(m_HwndParent, MESSAGE_CONNECTFAIL, (WPARAM)&n, NULL);
	}
}

CTCPHistoryService* CTCPHistoryService::GetInstance()
{
	if( m_pInstance == NULL ) 
	{
	//	g_QuotServerSeqID = 0;
		m_pInstance = new CTCPHistoryService();
	//	g_lTimeQuto = SetTimer(NULL,NULL,300000,(TIMERPROC)(NULL)); //每5分钟订阅一次行情	
		
	}

	return m_pInstance;
}

void CTCPHistoryService::DestroyInstance()
{
	if( m_pInstance != NULL) 
	{
		KillTimer(NULL, TIMER_HEARTBEAT);
		Sleep(5);
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

CTCPHistoryService::CTCPHistoryService(void) 
: m_pTcpClient(NULL)
, m_pStreamCache(NULL)
, m_nNetStatus(enmNetNothing)
, m_step(REQ_STEP_INIT)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_licenceMap.clear();
	m_pStreamCache = new CMemoryCache(5*1024*1024);
}

CTCPHistoryService::~CTCPHistoryService(void)
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

bool CTCPHistoryService::Connect(std::string strIP, int nPort)
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
		CallbackConnectSuccess, CallbackConnectFail, CallbackOntimeFunc);
	SetNetStatus(enmNetConnectWaiting);

	return true;
}

bool CTCPHistoryService::SendData(UINT nCMDID, void* pData, int nLen, UINT seq)
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

		int nLen = m_pTcpClient->SendData(pBuf, nTotalLen);
		if(nLen >0 && pData!= NULL)
		{
			//	OFFER_INFO("报盘发送订阅行情命令： %s\n", pBuf+sizeof(Stru_UniPkgHead));
			//PUSH_LOG_ERROR(Logger,false,true,"发送订阅行情命令： %s\n", pBuf+sizeof(Stru_UniPkgHead));

		}
		return true;
	}
	else 
	{
		return false;
	}
}
/*
void CTCPHistoryService::OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
if( m_nNetStatus == enmNetConnectSuccess) 
{
SendData(CMDID_HEARTBEAT, NULL, 0);
}
}
*/
int CTCPHistoryService::GetPackageHeadTailLen()
{
	return (int)sizeof(Stru_UniPkgHead)+(int)sizeof(DWORD);
}

int CTCPHistoryService::GetPackageHeadLen()
{
	return (int)sizeof(Stru_UniPkgHead);
}

bool CTCPHistoryService::VerifyRequest( Stru_UniPkgHead& head )
{
	/*	switch(head.cmdid)
	{
	case CMDID_LICENCE_LIST_RSP:
	return head.len == sizeof(Stru_Licence);
	case CMDID_SERVER_ADDR_RSP:
	return head.len == sizeof(Stru_ServerAddr);
	case Cmd_QuotEvent_Push:
	return head.len == sizeof(PlatformStru_DepthMarketData);
	case Cmd_ReInitQuotServerEvent_Rsp:
	return true;
	case Cmd_QuotServerTimeResetEvent_Rsp:
	return true;
	case Cmd_QryQuotServerTimeEvent_Rsp:
	return true;  
	case Cmd_SubscribeQuotEvent_Rsp:
	return true;
	default:
	return false;
	}*/
	return true;
}

bool CTCPHistoryService::LicenceRecved()
{
	return m_step >= REQ_STEP_LICENCE_LIST_RECVED;
}

bool CTCPHistoryService::GetLicence( std::string strAppID, std::string& strLicence )
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

void CTCPHistoryService::SetMessWnd(HWND  HwndParent)
{
	m_HwndParent = HwndParent;
}