#ifndef _TCPLAYER_H_
#define _TCPLAYER_H_

#include "easymutex.h"
#include "SocketBase.h"
#include "InstitudeApi.h"

#include <map>
#include <string>
using namespace std;

//TcpClient 收发缓冲区的大小
#define SocketBufSize (10*1024*1024)

//Tcp接收后等待处理的数据缓冲区大小
#define TcpRecvBufSize (100*1024*1024)

//Tcp等待发送的数据缓冲区大小
#define TcpSendBufSize (10*1024*1024)


#define Debug_TcpLayer 1

class CTcpLayer
{
public:
	enum { scktConnect = 1, scktDisConnect, scktDataRecv };

public:
	// 用来初始收到数据并分发的线程函数
	static DWORD WINAPI Proc_RecvData(LPVOID lpParameter);

	static void SetSvrAddr(string& SvrIP, WORD SvrPort) {
		m_strSvrIP = SvrIP;
		m_wSvrPort = SvrPort;
	};
	static bool GetSvrAddr(string& SvrIP, WORD& SvrPort) {
		SvrIP = m_strSvrIP;
		SvrPort = m_wSvrPort;
	};

	static void Init();
	static void Release();
	static void Stop();

	//发送数据, 对sendData的一个封装
	static bool SendData(UINT nCMDID, void* pData, int nLen, UINT seq, UINT userData1=0, UINT userData2=0, UINT userData3=0, UINT userData4=0);

	//发送数据
	static bool Senddata(const char* pData,int DataLen);

	//注册Api
	static void RegisterApi(CInstitudeApi* pApi);
	//注销Api
	static void UnRegisterApi(CInstitudeApi* pApi);
	//注销所有Api
	static void UnRegisterApiAll();


	//在主线程中调用，用于解析并分发接收数据包。
	//接收到数据后，会向主线程发送WM_COMMAND命令，主线程再调用此函数进行解析和分发。
	//这样在窗口线程中处理数据，会比较安全
	static void ParseRecvPackage(int nMode);

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
	static bool m_bStop;

	static char* m_pRcvBuf;
	static int m_RcvBufLen;
	static char* m_pSndBuf;
	static int m_SndBufLen;

	//上次发送心跳时间
	static time_t m_HeartbeatTime;



	static set<CInstitudeApi*> m_setApi;

	static string		m_strSvrIP;
	static WORD			m_wSvrPort;

	static HANDLE		m_hMainThread;
	static DWORD		m_dwMainThreadID;
	static UINT			m_nRecvDataMSGID;
};

#endif
