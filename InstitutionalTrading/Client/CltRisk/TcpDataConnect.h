#ifndef _TCPDATACONNECT_H_
#define _TCPDATACONNECT_H_

#include "easymutex.h"
#include "SocketBase.h"

#include <map>
#include <string>
using namespace std;

//TcpClient 收发缓冲区的大小
#define SocketBufSize_Quot (10*1024*1024)

//Tcp接收后等待处理的数据缓冲区大小
#define TcpRecvBufSize_Quot (100*1024*1024)

//Tcp等待发送的数据缓冲区大小
#define TcpSendBufSize_Quot (10*1024*1024)


#define WM_USER_Win_Quot              0x5111		//WM_USER+0x111的值，从WinUser.h中取出的，因为考虑clr中托管与非托管代码混用的方便，没有包含windows.h


//通知主线程窗口有数据到达的WM_USER_Win消息的wParam值
#define WndUserCmd_DataArrival_Quot 0x4bbb
//通知主线程窗口连接状态发生变化的WM_USER_Win消息的wParam值。lParam==0/1/2/3分别表示连接断开/连接开始/连接成功/连接失败
#define WndUserCmd_QuotConnStatus_Quot  0x4bbc

//通知模块窗口有消息到达，lParam是消息数据指针，Stru_UniPkgHead + 数据
#define WndUserCmd_YourPkgArrival_Quot 0x4bcb

#define Debug_TcpDataConnect 1

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CTcpDataConnect
{
public:
    static void Init();
    static void Release();
    static void Stop();

	//判断是否处于连接状态
	static bool IsConnected();

	//发送数据, 对sendData的一个封装
	static bool SendData(UINT nCMDID, void* pData, int nLen, UINT seq, unsigned int userdata1=0, unsigned int userdata2=0, unsigned int userdata3=0, unsigned int userdata4=0);

    //发送数据
    static bool Senddata(const char* pData,int DataLen);


    //设置主窗口句柄
    static void SetMainWndHandle(int hMainWnd);


    //模块订阅消息，需指定消息ID和模块窗口的句柄
    static void SubscribePkg(unsigned int CmdID,int hModuleWnd);
    //模块退订消息，需指定消息ID和模块窗口的句柄
    static void UnsubscribePkg(unsigned int CmdID,int hModuleWnd);
    //模块退订全部消息，需指定消息ID和模块窗口的句柄
    static void UnsubscribeAllPkg(int hModuleWnd);


    //在主线程中调用，用于解析并分发接收数据包。
    //接收到数据后，会向主线程发送WM_USER_Win命令，主线程再调用此函数进行解析和分发。
    //这样在窗口线程中处理数据，会比较安全
	//****注意：WndUserCmd_YourPkgArrival消息的接收处理过程不能被阻塞，如MessageBox()的调用等。如果阻塞，会导致系统卡死或崩溃
	//****(因为主窗口消息处理会重入。在接收窗口的消息处理被阻塞的情况下，如果主窗口的消息队列还有消息，主窗口线程会保存当前被阻塞的上下文数据，然后再次调用消息处理函数，即重入。(似乎是winform的机制)。这样会导致m_pRcvBuf冲突并崩溃
    static void ParseRecvPackageInWindowThread(void);

private:
	//从tcp得到数据后，调用此回调函数进行处理
	static void	RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//断开tcp连接后，调用此回调函数进行处理
	static void	DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//开始调用connect()后，调用此函数通知应用层
	static void	StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//连接成功后，调用此回调函数进行处理
	static void	ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//连接失败后，调用此回调函数进行处理
	static void	ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);

	//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
	static void OnTimeCBFunc(int UserData);

    //创建tcpclient
    static void createtcpclient(void);

    //发送数据
    static void senddata_internal(void);

public:
    CTcpDataConnect(string& SvrIP, WORD SvrPort);
    ~CTcpDataConnect();
private:
    static Ceasymutex m_mutex;
    static void* m_pTcpClient;
    static bool m_bStop;

    static char* m_pRcvBuf;
    static int m_RcvBufLen;
    static char* m_pSndBuf;
    static int m_SndBufLen;

    //主窗口句柄
    static int m_hMainWnd;

    //订阅消息的模块窗口句柄
    static multimap<unsigned int,int> m_Cmd2hWnd;

	//上次发送心跳时间
	static time_t m_HeartbeatTime;

	//是否已经连接
	static bool m_bConnected;
	//是否断开了连接。指从已连接到连接断开的变化，不包含初始化的未连接状态
	static bool m_bDisConnected;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif


