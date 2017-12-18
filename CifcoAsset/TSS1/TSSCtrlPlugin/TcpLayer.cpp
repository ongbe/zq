// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TcpLayer.h"
//#include <windows.h>
#include "ConfigMgr.h"
#include "inc\FileOpr.h"
#include "inc\Tools_Ansi.h"
#include "inc\CommonPkg.h"
//#include "RiskManageCmd.h"
#include "inc\TcpClientWorker.h"
#include <windows.h>

#include <string>
#include <queue>
using namespace std;



#ifdef _DEBUG
	#pragma comment(lib, "ToolsD.lib")
#else
	#pragma comment(lib, "Tools.lib")
#endif

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")


#pragma unmanaged


//内部变量
HANDLE CTcpLayer::s_hTimerThread = NULL;
bool CTcpLayer::s_bHasData = false;
bool CTcpLayer::m_bStop=false;
Ceasymutex CTcpLayer::m_mutex;
void* CTcpLayer::m_pTcpClient=NULL;
char* CTcpLayer::m_pRcvBuf=NULL;
int CTcpLayer::m_RcvBufLen=0;
char* CTcpLayer::m_pSndBuf=NULL;
int CTcpLayer::m_SndBufLen=0;
multimap<int,HWND> CTcpLayer::m_Cmd2hWnd;
time_t CTcpLayer::m_HeartbeatTime=0;


DWORD WINAPI CTcpLayer::Proc_Timer(LPVOID pParam)
{
	while(!m_bStop) {
		CTcpLayer::ParseRecvPackageInWindowThread();
		Sleep(20);
	}
	return 0;
}

void CTcpLayer::Init()
{
	m_mutex.lock();

	createtcpclient();

	DWORD dwThreadID = 0;
	s_hTimerThread = CreateThread(NULL, 0, Proc_Timer, NULL, 0, &dwThreadID);

	m_mutex.unlock();
}

void CTcpLayer::Release()
{
	m_mutex.lock();
	CFileOpr::writelocallog("TcpLayer.log","Release，m_pTcpClient=%x ",
							m_pTcpClient);

	if(m_pTcpClient)
	{
		delete m_pTcpClient;
		m_pTcpClient=NULL;
	}
	if(m_pRcvBuf!=NULL) {
		free(m_pRcvBuf);
		m_pRcvBuf = NULL;
	}
	if(m_pSndBuf!=NULL) {
		free(m_pSndBuf);
		m_pSndBuf = NULL;
	}
	m_mutex.unlock();
}

//系统退出前，先Stop，停止分发，再关闭各模块窗口。
void CTcpLayer::Stop()
{
	m_mutex.lock();
	m_bStop=true;
	m_mutex.unlock();
}


//从tcp得到数据后，调用此回调函数进行处理
void CTcpLayer::RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if(m_bStop) return;

	m_mutex.lock();
	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);
	if(!m_pRcvBuf||m_RcvBufLen+DataLen>TcpRecvBufSize)
	{
		CFileOpr::writelocallog("TcpLayer.log","RecvDataCBFunc缓冲区不足，m_pRcvBuf=%x m_RcvBufLen=%d DataLen=%d TcpRecvBufSize=%d localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
								m_pRcvBuf,m_RcvBufLen,DataLen,TcpRecvBufSize,LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	}
	else
	{
		memcpy(m_pRcvBuf+m_RcvBufLen,pData,DataLen);
		m_RcvBufLen+=DataLen;

		s_bHasData = true;
	}
	m_mutex.unlock();

	////通知主窗口有数据到达
	//if(!m_bStop&&m_hMainWnd!=NULL&&IsWindow(m_hMainWnd))
	//{
	//	PostMessage(m_hMainWnd,WM_COMMAND,WndCmd_DataArrival,0L);
	//}

}

//断开tcp连接后，调用此回调函数进行处理
void CTcpLayer::DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","DisconnCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//开始调用connect()后，调用此函数通知应用层
void CTcpLayer::StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","StartConnCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	m_mutex.lock();
	m_mutex.unlock();
}

//连接成功后，调用此回调函数进行处理
void CTcpLayer::ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","StartConnCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);
	m_mutex.lock();
	m_mutex.unlock();
}

//连接失败后，调用此回调函数进行处理
void CTcpLayer::ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	CFileOpr::writelocallog("TcpLayer.log","ConnFailCBFunc，localaddr=(%u):%u svraddr=(%u):%u, hSocket=%x",
							LocalAddr.IP,LocalAddr.Port,SvrAddr.IP,SvrAddr.Port,hSocket);

	m_mutex.lock();
	m_mutex.unlock();
}

//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
void CTcpLayer::OnTimeCBFunc(void)
{
	m_mutex.lock();

	//发送尚未发送的数据
	if(!m_bStop)
		senddata_internal();

	time_t cursecs=time(NULL);
	if(cursecs-m_HeartbeatTime>=3&&
		m_pTcpClient&&
		((CTcpClientWorker*)m_pTcpClient)->getSocket()>0&&
		((CTcpClientWorker*)m_pTcpClient)->getConnStatus()==TcpClientConnStatus_Connected)
	{
		//每3秒发送心跳
		char buf[sizeof(Stru_UniPkgHead)+4];
		Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
		PkgHead.Set(0,CMDID_HEARTBEAT,0,0);
		*(unsigned int*)(buf+sizeof(PkgHead))=TailID_UniPkg;

		SendData(buf,sizeof(buf));
		m_HeartbeatTime=cursecs;
	}


	m_mutex.unlock();
}

//创建tcpclient
void CTcpLayer::createtcpclient(void)
{
	m_mutex.lock();
	if(!m_bStop&&!m_pTcpClient && CConfigManager::IsLoaded())
	{
		string svrIP,localIP;
		DWORD dwsvrIP,dwlocalIP;
		unsigned short svrPort,localPort;

		if(CConfigManager::GetSvrAddr(svrIP,svrPort))
		{
			dwsvrIP=CTools_Ansi::ConvertStrIP(svrIP.c_str());
			if(dwsvrIP!=0&&svrPort!=0)
			{
				CConfigManager::GetLocalAddr(localIP,localPort);
				dwlocalIP=CTools_Ansi::ConvertStrIP(localIP.c_str());

				m_pTcpClient=new CTcpClientWorker(Stru_IPAddress(dwlocalIP,localPort),Stru_IPAddress(dwsvrIP,svrPort),
					 RecvDataCBFunc,
					 DisconnCBFunc,
					 StartConnCBFunc,
					 ConnSuccCBFunc,
					 ConnFailCBFunc,
					 OnTimeCBFunc,
					 SocketBufSize,SocketBufSize);

				if(Debug_TcpLayer||m_pTcpClient)
					CFileOpr::writelocallog("TcpLayer.log","创建TcpClient，localaddr=%s(%u):%u svraddr=%s(%u):%u, m_pTcpClient=%x",
											localIP,dwlocalIP,localPort,svrIP,dwsvrIP,svrPort,m_pTcpClient);
			}
		}
	}
	m_mutex.unlock();
}

bool CTcpLayer::SendData(UINT nCMDID, void* pData, int nLen, UINT seq)
{
	if(m_bStop) return false;
	Stru_UniPkgHead head;
	head.moduleid = 0;
	head.cmdid = nCMDID;
	head.seq = seq;
	head.len = nLen;

	
	int nTotalLen = sizeof(Stru_UniPkgHead)+head.len+4;
	char* pBuf = new char[nTotalLen];
	auto_ptr<char> ptrBuf( pBuf );
	if ( NULL == pBuf )
	{
		return false;
	}
	Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)pBuf;
	PkgHead.Set(nLen,nCMDID,0,0);

	if(pData)
		memcpy(pBuf+sizeof(Stru_UniPkgHead),pData,nLen);
	*(unsigned int*)(pBuf+sizeof(PkgHead) + nLen)=TailID_UniPkg;

	return CTcpLayer::SendData(pBuf,nTotalLen);
}

//发送数据
bool CTcpLayer::SendData(const char* pData,int DataLen)
{
	if(m_bStop) return false;

	bool brlt=false;
	m_mutex.lock();
	if(!m_pSndBuf) m_pSndBuf=(char*)malloc(TcpSendBufSize);
	if(!m_pSndBuf||m_SndBufLen+DataLen>TcpSendBufSize)
	{
		CFileOpr::writelocallog("TcpLayer.log","senddata缓冲区不足，m_pSndBuf=%x m_SndBufLen=%d DataLen=%d TcpSendBufSize=%d m_pTcpClient=%x hSocket=%x ConnStatus=%d",
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
void CTcpLayer::senddata_internal(void)
{
	if(m_pSndBuf&&
		!m_bStop&&m_SndBufLen>0&&
		m_pTcpClient&&
		((CTcpClientWorker*)m_pTcpClient)->getSocket()>0&&
		((CTcpClientWorker*)m_pTcpClient)->getConnStatus()==TcpClientConnStatus_Connected)
	{
		int sentlen=((CTcpClientWorker*)m_pTcpClient)->SendData(m_pSndBuf,m_SndBufLen);
		if(sentlen>0&&sentlen<=m_SndBufLen)
		{
			memmove(m_pSndBuf,m_pSndBuf+sentlen,m_SndBufLen-sentlen);
			m_SndBufLen-=sentlen;
		}
	}
}

//模块订阅消息，需指定消息ID和模块窗口的句柄
void CTcpLayer::SubscribePkg(int CmdID,HWND hModuleWnd)
{
	m_mutex.lock();
	multimap<int,HWND>::iterator itlower=m_Cmd2hWnd.lower_bound(CmdID);
	multimap<int,HWND>::iterator itupper=m_Cmd2hWnd.upper_bound(CmdID);
	while(itlower!=itupper&&itlower->second!=hModuleWnd) itlower++;
	if(itlower==itupper)
		m_Cmd2hWnd.insert(pair<int,HWND>(CmdID,hModuleWnd));
	m_mutex.unlock();
}

//模块退订消息，需指定消息ID和模块窗口的句柄
void CTcpLayer::UnsubscribePkg(int CmdID,HWND hModuleWnd)
{
	m_mutex.lock();
	multimap<int,HWND>::iterator itlower=m_Cmd2hWnd.lower_bound(CmdID);
	multimap<int,HWND>::iterator itupper=m_Cmd2hWnd.upper_bound(CmdID);
	while(itlower!=itupper&&itlower->second!=hModuleWnd) itlower++;
	if(itlower!=itupper)
		m_Cmd2hWnd.erase(itlower);
	m_mutex.unlock();
}
//模块退订全部消息，需指定消息ID和模块窗口的句柄
void CTcpLayer::UnsubscribeAllPkg(HWND hModuleWnd)
{
	m_mutex.lock();
	multimap<int,HWND>::iterator it=m_Cmd2hWnd.begin();
	while(it!=m_Cmd2hWnd.end())
	{
		if(it->second==hModuleWnd)
			it=m_Cmd2hWnd.erase(it);
		else it++;
	}
	m_mutex.unlock();
}


//在主线程中调用，用于解析并分发接收数据包。
//接收到数据后，会向主线程发送WM_COMMAND命令，主线程再调用此函数进行解析和分发。
//这样在窗口线程中处理数据，会比较安全
void CTcpLayer::ParseRecvPackageInWindowThread(void)
{
	if(m_bStop) return;
	if(!s_bHasData)
		return;

	m_mutex.lock();

	if(!m_pRcvBuf) m_pRcvBuf=(char*)malloc(TcpRecvBufSize);

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

			multimap<int,HWND>::iterator itlower=m_Cmd2hWnd.lower_bound(PkgHead.cmdid);
			multimap<int,HWND>::iterator itupper=m_Cmd2hWnd.upper_bound(PkgHead.cmdid);
			while(itlower!=itupper)
			{
				//分发数据包给各个模块，注意，模块中一定不能进行长时间操作，否则会导致系统卡死
				//lParam为const char*指针，指向Stru_UniPkgHead + PkgData
				if(IsWindow(itlower->second))
					SendMessage(itlower->second,_GetDataPkgArriveMSGID(),WndCmd_YourPkgArrival,(LPARAM)(m_pRcvBuf+off));

				itlower++;
			}

			off+=LengthUsedByPkg;
		}
		if(off>0)
		{
			memmove(m_pRcvBuf,m_pRcvBuf+off,m_RcvBufLen-off);
			m_RcvBufLen-=off;
		}
	}

	s_bHasData = false;

	m_mutex.unlock();
}
