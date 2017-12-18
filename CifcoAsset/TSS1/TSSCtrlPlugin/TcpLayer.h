#ifndef _TCPLAYER_H_
#define _TCPLAYER_H_

#include "inc\easymutex.h"
#include "inc\SocketBase.h"

#include <map>
#include <string>
using namespace std;

//TcpClient 收发缓冲区的大小
#define SocketBufSize (10*1024*1024)

//Tcp接收后等待处理的数据缓冲区大小
#define TcpRecvBufSize (100*1024*1024)

//Tcp等待发送的数据缓冲区大小
#define TcpSendBufSize (10*1024*1024)


//通知主线程有数据到达的WM_COMMAND消息的wParam命令
#define WndCmd_DataArrival 0x3bbb
//通知模块窗口有消息到达，lParam是消息数据指针，Stru_UniPkgHead + 数据
#define WndCmd_YourPkgArrival 0x3bbc

#define Debug_TcpLayer 0

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CTcpLayer
{
public:
	static DWORD WINAPI Proc_Timer(LPVOID pParam);

public:
	static void Init();
	static void Release();
	static void Stop();

	//发送数据, 对sendData的一个封装
	static bool SendData(UINT nCMDID, void* pData, int nLen, UINT seq);
	//发送数据
	static bool SendData(const char* pData,int DataLen);

	//模块订阅消息，需指定消息ID和模块窗口的句柄
	static void SubscribePkg(int CmdID,HWND hModuleWnd);
	//模块退订消息，需指定消息ID和模块窗口的句柄
	static void UnsubscribePkg(int CmdID,HWND hModuleWnd);
	//模块退订全部消息，需指定消息ID和模块窗口的句柄
	static void UnsubscribeAllPkg(HWND hModuleWnd);


	//在主线程中调用，用于解析并分发接收数据包。
	//接收到数据后，会向主线程发送WM_COMMAND命令，主线程再调用此函数进行解析和分发。
	//这样在窗口线程中处理数据，会比较安全
	static void ParseRecvPackageInWindowThread(void);

private:
	//从tcp得到数据后，调用此回调函数进行处理
	static void	RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//断开tcp连接后，调用此回调函数进行处理
	static void	DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//开始调用connect()后，调用此函数通知应用层
	static void	StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//连接成功后，调用此回调函数进行处理
	static void	ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//连接失败后，调用此回调函数进行处理
	static void	ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);

	//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
	static void OnTimeCBFunc(void);

	//创建tcpclient
	static void createtcpclient(void);

	//发送数据
	static void senddata_internal(void);

private:
	static Ceasymutex m_mutex;
	static void* m_pTcpClient;
	static HANDLE s_hTimerThread;
	static bool s_bHasData;
	static bool m_bStop;

	static char* m_pRcvBuf;
	static int m_RcvBufLen;
	static char* m_pSndBuf;
	static int m_SndBufLen;

	//订阅消息的模块窗口句柄
	static multimap<int,HWND> m_Cmd2hWnd;

	//上次发送心跳时间
	static time_t m_HeartbeatTime;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


