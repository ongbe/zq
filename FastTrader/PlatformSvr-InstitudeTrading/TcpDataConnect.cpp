// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TcpDataConnect.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "CommonPkg.h"
#include "TcpClientWorker.h"
#include <windows.h>

#include <string>
using namespace std;


#if 0
void _Log(LPSTR pStr)
{
	HANDLE hFile = ::CreateFile("C:\\temp\\inst-tcp.log",	// file to open
                       GENERIC_WRITE,			// open for reading
                       FILE_SHARE_READ,			// share for reading
                       NULL,					// default security
                       OPEN_ALWAYS,				// existing file only
                       FILE_ATTRIBUTE_NORMAL,	// normal file
                       NULL);					// no attr. template
	if(hFile == INVALID_HANDLE_VALUE)
		return;

	DWORD dwRealWrite = 0;
	char csDateTime[32] = {0};
	SYSTEMTIME sysTime = {0};

	::GetLocalTime(&sysTime);
	sprintf(csDateTime, "%04d-%02d-%02d %02d:%02d:%02d.%03d\r\n", 
			sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);

	::SetFilePointer(hFile, 0, NULL, FILE_END);
	::WriteFile(hFile, csDateTime, strlen(csDateTime), &dwRealWrite, NULL);
	::WriteFile(hFile, pStr, strlen(pStr), &dwRealWrite, NULL);
	::WriteFile(hFile, "\r\n", strlen("\r\n"), &dwRealWrite, NULL);
	::CloseHandle(hFile);
}
#endif


//内部变量
Ceasymutex CTcpDataConnect::m_mutex;
map<LPVOID, CTcpDataConnect*> CTcpDataConnect::m_mapWorker;
map<SOCKET, CTcpDataConnect*> CTcpDataConnect::m_mapConn;
map<SOCKET, CInstitudeApi*> CTcpDataConnect::m_mapApi;

UINT CTcpDataConnect::m_nObjectCount = 0;
BOOL CTcpDataConnect::m_bMainThreadActive = FALSE;
HANDLE CTcpDataConnect::m_hMainThread = NULL;
DWORD CTcpDataConnect::m_dwMainThreadID = 0;
UINT CTcpDataConnect::m_nRecvDataMSGID = ::RegisterWindowMessage("m_nRecvDataMSGID");

DWORD WINAPI CTcpDataConnect::Proc_RecvData(LPVOID lpParameter)
{
	m_bMainThreadActive = TRUE;

	map<SOCKET, CTcpDataConnect*>::iterator it;
	MSG Msg;

	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message==m_nRecvDataMSGID) {
			m_mutex.lock();
			if(Msg.lParam==scktOnTime) {
				for(it = m_mapConn.begin(); it != m_mapConn.end(); it++) {
					if(it->second != NULL) 
						it->second->ParseRecvPackage(Msg.lParam, (SOCKET)Msg.wParam);
				}
			}
			else {
				it = m_mapConn.find((SOCKET)Msg.wParam);
				if(it != m_mapConn.end()) {
					if(it->second != NULL) 
						it->second->ParseRecvPackage(Msg.lParam, (SOCKET)Msg.wParam);
					if(Msg.lParam==scktDisConnect) {
						m_mapConn.erase((SOCKET)Msg.wParam);
						m_mapApi.erase((SOCKET)Msg.wParam);
					}
				}
				else {
					CFileOpr::getObj().writelocallog("TcpLayer","Proc_RecvData find connect fail，mode==%d, SOCKET==%x",
							Msg.lParam, Msg.wParam);
				}
			}
			m_mutex.unlock();
		}
	}
	return 0;
}

CTcpDataConnect* CTcpDataConnect::CreateConnect(string& SvrIP, WORD SvrPort, CInstitudeApi* pApi)
{
	CTcpDataConnect* pConn = new CTcpDataConnect(SvrIP, SvrPort, pApi);

	m_mutex.lock();
	LPVOID hWorker = pConn->Init();
	m_mapWorker.insert(make_pair(hWorker, pConn));
	m_mutex.unlock();
	return pConn;
}

LPVOID CTcpDataConnect::FindTcpConnect(SOCKET hConn)
{
	map<LPVOID, CTcpDataConnect*>::iterator it;
	
	for(it = m_mapWorker.begin(); it != m_mapWorker.end(); it++) {
		if(it->first != NULL && ((CTcpClientWorker*)it->first)->getSocket()==hConn)
			return it->second;
	}
	return NULL;
}

CTcpDataConnect::CTcpDataConnect(string& SvrIP, WORD SvrPort, CInstitudeApi* pApi)
{
	m_bStop = false;
	m_strSvrIP = SvrIP;
	m_wSvrPort = SvrPort;
	m_pApi = pApi;

	m_pTcpClient = NULL;
	m_hConnect = -1;

	m_pRcvBuf = NULL;
	m_RcvBufLen = 0;
	m_pSndBuf = NULL;
	m_SndBufLen = 0;

	m_mutex.lock();
	m_nObjectCount++;
	m_mutex.unlock();

	if(m_hMainThread==NULL) {
		m_hMainThread = ::CreateThread(NULL, 256*65536, Proc_RecvData, NULL, 0, &m_dwMainThreadID);
		if(m_hMainThread != NULL) {
			CFileOpr::getObj().writelocallog("TcpLayer","CreateConnect，CreateThread Success HANDLE==%x, ID==%d", 
					m_hMainThread, m_dwMainThreadID);
		}
		else {
			DWORD dwError = ::GetLastError();
			CFileOpr::getObj().writelocallog("TcpLayer","CreateConnect，CreateThread Fail, ErrorNum==%d", 
					dwError);
		}
	}
}

CTcpDataConnect::~CTcpDataConnect()
{
	m_mutex.lock();
	
	m_nObjectCount--;

	if(m_nObjectCount==0) {
		::CloseHandle(m_hMainThread);
		if(::WaitForSingleObject(m_hMainThread, 3000)==WAIT_TIMEOUT) {
			::TerminateThread(m_hMainThread, 1);
		}
		m_hMainThread = NULL;
		m_dwMainThreadID = 0;
	}
	delete m_pRcvBuf;
	m_pRcvBuf = NULL;
	delete m_pSndBuf;
	m_pSndBuf = NULL;
	m_mutex.unlock();
}

LPVOID CTcpDataConnect::Init()
{
	LPVOID hConn = 0;

	m_mutex.lock();

	if(!m_bStop)
	{
		string svrIP,localIP;
		DWORD dwsvrIP=0,dwlocalIP=0;
		unsigned short svrPort=0,localPort=0;

		svrIP = m_strSvrIP;
		svrPort = m_wSvrPort;
		dwsvrIP=CTools_Ansi::ConvertStrIP(svrIP.c_str());
		if(dwsvrIP!=0&&svrPort!=0)
		{
			dwlocalIP=CTools_Ansi::ConvertStrIP(localIP.c_str());

			m_pTcpClient=new CTcpClientWorker(
					Stru_IPAddress(dwlocalIP,localPort),
					Stru_IPAddress(dwsvrIP,svrPort),
					RecvDataCBFunc,
					DisconnCBFunc,
					StartConnCBFunc,
					ConnSuccCBFunc,
					ConnFailCBFunc,
					OnTimeCBFunc,
					SocketBufSize,
					SocketBufSize);
			hConn = m_pTcpClient;

			if(Debug_TcpLayer||m_pTcpClient)
				CFileOpr::getObj().writelocallog("TcpLayer","创建TcpClient，localaddr=%s(%u):%u svraddr=%s(%u):%u, m_pTcpClient=%x",
						localIP.c_str(),dwlocalIP,localPort,svrIP.c_str(),dwsvrIP,svrPort,m_pTcpClient);
		}
	}
		
	m_mutex.unlock();

	return hConn;
}

void CTcpDataConnect::Release()
{
	m_mutex.lock();

	m_mapWorker.erase(m_pTcpClient);

	CTcpClientWorker* pTcpClnt = (CTcpClientWorker*)m_pTcpClient;
	CFileOpr::getObj().writelocallog("TcpLayer","Release Start，m_pTcpClient=%x ", m_pTcpClient);
	LPVOID pBak = m_pTcpClient;
	m_pTcpClient = NULL;

	m_mutex.unlock();

	if(pTcpClnt) {
		delete pTcpClnt;
	}
	CFileOpr::getObj().writelocallog("TcpLayer","Release End，m_pTcpClient=%x ",pBak);
}

//系统退出前，先Stop，停止分发，再关闭各模块窗口。
void CTcpDataConnect::Stop()
{
	//m_mutex.lock();
	m_bStop=true;
	//m_mutex.unlock();
}

//从tcp得到数据后，调用此回调函数进行处理
void CTcpDataConnect::RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData)
{
	CTcpDataConnect* pConn = NULL;
	map<SOCKET, CTcpDataConnect*>::iterator it;

	m_mutex.lock();
	it = m_mapConn.find(hSocket);
	if(it != m_mapConn.end()) {
		pConn = it->second;
	}

	if(pConn!=NULL && !pConn->m_bStop) {
		if(!pConn->m_pRcvBuf) pConn->m_pRcvBuf=(char*)malloc(TcpRecvBufSize);
		if(!pConn->m_pRcvBuf||pConn->m_RcvBufLen+DataLen>TcpRecvBufSize)
		{
			CFileOpr::getObj().writelocallog("TcpLayer","RecvDataCBFunc缓冲区不足，m_pRcvBuf=%x m_RcvBufLen=%d DataLen=%d TcpRecvBufSize=%d localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
									pConn->m_pRcvBuf,pConn->m_RcvBufLen,DataLen,TcpRecvBufSize,LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
		}
		else
		{
			memcpy(pConn->m_pRcvBuf+pConn->m_RcvBufLen,pData,DataLen);
			pConn->m_RcvBufLen+=DataLen;
		}
		//通知转发线程有数据到达
		::PostThreadMessage(m_dwMainThreadID, m_nRecvDataMSGID, (WPARAM)hSocket, (LPARAM)scktDataRecv);
	}
	m_mutex.unlock();
}

//断开tcp连接后，调用此回调函数进行处理
void CTcpDataConnect::DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData)
{
	CFileOpr::getObj().writelocallog("TcpLayer","DisconnCBFunc，localaddr=(%08x):%u svraddr=(%08x):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	//_Log("DisconnCBFunc()");
	m_mutex.lock();
	::PostThreadMessage(m_dwMainThreadID, m_nRecvDataMSGID, (WPARAM)hSocket, (LPARAM)scktDisConnect);
	m_mutex.unlock();
}

//开始调用connect()后，调用此函数通知应用层
void CTcpDataConnect::StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData)
{
	CFileOpr::getObj().writelocallog("TcpLayer","StartConnCBFunc，localaddr=(%08x):%u svraddr=(%08x):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	m_mutex.lock();
	m_mutex.unlock();
}

//连接成功后，调用此回调函数进行处理
void CTcpDataConnect::ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData)
{
	CFileOpr::getObj().writelocallog("TcpLayer","ConnSuccCBFunc，localaddr=(%08x):%u svraddr=(%08x):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	
	CTcpDataConnect* pConn = NULL;

	m_mutex.lock();
	pConn = (CTcpDataConnect*)FindTcpConnect(hSocket);
	if(pConn != NULL && !pConn->m_bStop) {
		m_mapConn.insert(make_pair(hSocket, pConn));
		m_mapApi.insert(make_pair(hSocket, pConn->GetApi()));
		int nCount = 0;
		//while(nCount<10 && ::PostThreadMessage(m_dwMainThreadID, m_nRecvDataMSGID, (WPARAM)hSocket, (LPARAM)scktConnect)==0) {
		while(!m_bMainThreadActive && nCount<10) {
			Sleep(100);
			nCount++;
		}
		
		if(::PostThreadMessage(m_dwMainThreadID, m_nRecvDataMSGID, (WPARAM)hSocket, (LPARAM)scktConnect)==0) {
			DWORD nError = ::GetLastError();
			CFileOpr::getObj().writelocallog("TcpLayer","ConnSuccCBFunc PostThreadMessage(%u, (SOCKET)%x)==FALSE，ErrorNum=%u，localaddr=(%08x):%u svraddr=(%08x):%u, hSocket=%x",
									m_dwMainThreadID, hSocket, nError, LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
		}
		else {
			CFileOpr::getObj().writelocallog("TcpLayer","ConnSuccCBFunc FindTcpConnect(%x)==TRUE，localaddr=(%08x):%u svraddr=(%08x):%u, hSocket=%x",
									hSocket, LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
		}
	}
	else {
		CFileOpr::getObj().writelocallog("TcpLayer","ConnSuccCBFunc FindTcpConnect(%x)==FALSE，localaddr=(%08x):%u svraddr=(%08x):%u, hSocket=%x",
								hSocket, LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	}
	m_mutex.unlock();
}

//连接失败后，调用此回调函数进行处理
void CTcpDataConnect::ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData)
{
	CFileOpr::getObj().writelocallog("TcpLayer","ConnFailCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
void CTcpDataConnect::OnTimeCBFunc(int UserData)
{
	//CFileOpr::writelocallog("TcpLayer","OnTimeCBFunc()");

	::PostThreadMessage(m_dwMainThreadID, m_nRecvDataMSGID, 0, (LPARAM)scktOnTime);

	////_Log("OnTimeCBFunc()");
	//map<SOCKET, CTcpDataConnect*>::iterator it;

	//m_mutex.lock();

	//for(it = m_mapConn.begin(); it != m_mapConn.end(); it++) {
	//	if(it->second != NULL)
	//		it->second->OnTimer();
	//}

	//m_mutex.unlock();
}

//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
void CTcpDataConnect::OnTimer(void)
{
	//CFileOpr::writelocallog("TcpLayer","CTcpDataConnect::OnTimer()");
	////_Log("CTcpDataConnect::OnTimer()");

	//发送尚未发送的数据
	if(!m_bStop)
		senddata_internal();

	DWORD cursecs = ::GetTickCount();
	//if(m_pTcpClient)
	//	CFileOpr::writelocallog("TcpLayer","CTcpDataConnect::OnTimer() (m_hConnect = %u) PreHeartBeat(%u, %u, %x, %u, %u)", 
	//			m_hConnect, cursecs, m_HeartbeatTime, m_pTcpClient, ((CTcpClientWorker*)m_pTcpClient)->getSocket(), 
	//			((CTcpClientWorker*)m_pTcpClient)->getConnStatus());
	//else
	//	CFileOpr::writelocallog("TcpLayer","CTcpDataConnect::OnTimer() (m_hConnect = %u) PreHeartBeat(%u, %u)", 
	//			m_hConnect, cursecs, m_HeartbeatTime);
	//if(cursecs-m_HeartbeatTime>=3&&
	//	m_pTcpClient&&
	//	((CTcpClientWorker*)m_pTcpClient)->getSocket()>0&&
	//	((CTcpClientWorker*)m_pTcpClient)->getConnStatus()==TcpClientConnStatus_Connected)
	if(cursecs-m_HeartbeatTime>=3000 && m_pTcpClient)
	{
		CFileOpr::getObj().writelocallog("TcpLayer","CTcpDataConnect::OnTimer() SendHeartBeat()");
		////_Log("CTcpDataConnect::OnTimer() SendHeartBeat()");
		//每3秒发送心跳
		char buf[sizeof(Stru_UniPkgHead)+4];
		Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
		PkgHead.Set(0,CMDID_HEARTBEAT,0,0);
		*(unsigned int*)(buf+sizeof(PkgHead))=TailID_UniPkg;

		Senddata(buf,sizeof(buf));
		m_HeartbeatTime=cursecs;
	}

}

bool CTcpDataConnect::SendData(UINT nCMDID, void* pData, int nLen, UINT seq, 
						 UINT userData1, UINT userData2, UINT userData3, UINT userData4)
{
	if(m_bStop) return false;
	Stru_UniPkgHead head;
	head.moduleid = 0;
	head.cmdid = nCMDID;
	head.seq = seq;
	head.len = nLen;
	head.userdata1 = userData1;
	head.userdata2 = userData2;
	head.userdata3 = userData3;
	head.userdata4 = userData4;
	
	int nTotalLen = sizeof(Stru_UniPkgHead)+head.len+4;
	char* pBuf = new char[nTotalLen];
	auto_ptr<char> ptrBuf( pBuf );
	if ( NULL == pBuf )
	{
		return false;
	}
	Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)pBuf;
	PkgHead.Set(nLen,nCMDID,0,seq, 0, userData1, userData2, userData3, userData4);

	if(pData)
		memcpy(pBuf+sizeof(Stru_UniPkgHead),pData,nLen);
	*(unsigned int*)(pBuf+sizeof(PkgHead) + nLen)=TailID_UniPkg;

	return Senddata(pBuf,nTotalLen);
}

//发送数据
bool CTcpDataConnect::Senddata(const char* pData,int DataLen)
{
	if(m_bStop) return false;

	bool brlt=false;
	m_mutex.lock();
	if(!m_pSndBuf) m_pSndBuf=(char*)malloc(TcpSendBufSize);
	if(!m_pSndBuf||m_SndBufLen+DataLen>TcpSendBufSize)
	{
		CFileOpr::getObj().writelocallog("TcpLayer","senddata缓冲区不足，m_pSndBuf=%x m_SndBufLen=%d DataLen=%d TcpSendBufSize=%d m_pTcpClient=%x hSocket=%x ConnStatus=%d",
			m_pSndBuf,m_SndBufLen,DataLen,TcpSendBufSize,m_pTcpClient,
			m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getSocket():-1,
			m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getConnStatus():0);
		brlt=false;
	}
	else
	{
		memcpy(m_pSndBuf+m_SndBufLen,pData,DataLen);
		m_SndBufLen+=DataLen;
		brlt=true;

		senddata_internal();
	}
	m_mutex.unlock();
	return brlt;
}

//发送数据
void CTcpDataConnect::senddata_internal(void)
{
	if(m_pSndBuf&&
		!m_bStop&&m_SndBufLen>0&&
		m_pTcpClient&&
		((CTcpClientWorker*)m_pTcpClient)->getSocket()>0&&
		((CTcpClientWorker*)m_pTcpClient)->getConnStatus()==TcpClientConnStatus_Connected)
	//if(m_pSndBuf&&!m_bStop&&m_SndBufLen>0&&m_hConnect>0)
	{
		int sentlen=((CTcpClientWorker*)m_pTcpClient)->SendData(m_pSndBuf,m_SndBufLen);
		if(sentlen>0&&sentlen<=m_SndBufLen)
		{
			memmove(m_pSndBuf,m_pSndBuf+sentlen,m_SndBufLen-sentlen);
			m_SndBufLen-=sentlen;
		}
	}
}

//在主线程中调用，用于解析并分发接收数据包。
//接收到数据后，会向主线程发送WM_COMMAND命令，主线程再调用此函数进行解析和分发。
//这样在窗口线程中处理数据，会比较安全
void CTcpDataConnect::ParseRecvPackage(int nMode, SOCKET hConnect)
{
	if(m_bStop) {
		CFileOpr::getObj().writelocallog("TcpLayer","ParseRecvPackage return，mode==%d, m_bStop==TRUE, SOCKET==%x",
				nMode, hConnect);
		return;
	}

	m_mutex.lock();

	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);

	switch(nMode) {
	case scktConnect:
		m_hConnect = hConnect;
		if(m_pApi != NULL) {
			m_pApi->OnFrontConnected();
			CFileOpr::getObj().writelocallog("TcpLayer","ParseRecvPackage，mode==connect, m_pApi==%x, SOCKET==%x",
				m_pApi, hConnect);
		}
		else {
			CFileOpr::getObj().writelocallog("TcpLayer","ParseRecvPackage，mode==connect, m_pApi==NULL, SOCKET==%x",
				hConnect);
		}
		break;
	case scktDisConnect:
		if(m_pApi != NULL) {
			m_pApi->OnFrontDisconnected();
		}
		m_hConnect = -1;
		break;
	case scktDataRecv:
		if(m_pRcvBuf)
		{
			//先跳过可能出现的无效数据
			int off=0;
			while(off<m_RcvBufLen&&Stru_UniPkgHead::IsInvalidPkgHead(m_pRcvBuf+off,m_RcvBufLen-off)) off++;

			//循环解析接收到的数据
			int LengthUsedByPkg;
			while(Stru_UniPkgHead::IsValidPkg(m_pRcvBuf+off,m_RcvBufLen-off,&LengthUsedByPkg))
			{
				Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)(m_pRcvBuf+off);

				if(m_pApi != NULL) {
					m_pApi->ParseData(m_pRcvBuf+off+sizeof(Stru_UniPkgHead), 
							PkgHead.len, PkgHead.cmdid, PkgHead.moduleid, 
							PkgHead.seq, PkgHead.subseq, PkgHead.userdata1, PkgHead.userdata2, 
							PkgHead.userdata3, PkgHead.userdata4);
				}

				off+=LengthUsedByPkg;
			}
			if(off>0)
			{
				memmove(m_pRcvBuf,m_pRcvBuf+off,m_RcvBufLen-off);
				m_RcvBufLen-=off;
			}
		}
		break;
	case scktOnTime:
		OnTimer();
		break;
	};

	m_mutex.unlock();
}
