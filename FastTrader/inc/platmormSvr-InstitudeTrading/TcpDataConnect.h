#ifndef _TCPDATACONNECT_H_
#define _TCPDATACONNECT_H_

#include "easymutex.h"
#include "SocketBase.h"
#include "InstitudeApi.h"

#include <map>
#include <set>
#include <string>
using namespace std;

//TcpClient 收发缓冲区的大小
#define SocketBufSize (10*1024*1024)

//Tcp接收后等待处理的数据缓冲区大小
#define TcpRecvBufSize (20*1024*1024)

//Tcp等待发送的数据缓冲区大小
#define TcpSendBufSize (10*1024*1024)


#define Debug_TcpLayer 1

class CTcpDataConnect
{
public:
	enum { scktConnect = 1, scktDisConnect, scktDataRecv, scktOnTime };

public:
	// 用来初始收到数据并分发的线程函数
	static DWORD WINAPI Proc_RecvData(LPVOID lpParameter);

	static CTcpDataConnect* CreateConnect(string& SvrIP, WORD SvrPort, CInstitudeApi* pApi);

	static LPVOID FindTcpConnect(SOCKET hConn);

private:
	//从tcp得到数据后，调用此回调函数进行处理
	static void	RecvDataCBFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//断开tcp连接后，调用此回调函数进行处理
	static void	DisconnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//开始调用connect()后，调用此函数通知应用层
	static void	StartConnCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//连接成功后，调用此回调函数进行处理
	static void	ConnSuccCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//连接失败后，调用此回调函数进行处理
	static void	ConnFailCBFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket, int UserData);

	//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
	static void OnTimeCBFunc(int UserData);

public:
	CTcpDataConnect(string& SvrIP, WORD SvrPort, CInstitudeApi* pApi);
	~CTcpDataConnect();

	bool GetSvrAddr(string& SvrIP, WORD& SvrPort) {
		SvrIP = m_strSvrIP;
		SvrPort = m_wSvrPort;
	};
	CInstitudeApi* GetApi() {
		return m_pApi;
	};

	LPVOID Init();
	void Release();
	void Stop();
	void OnTimer();

	//发送数据, 对sendData的一个封装
	bool SendData(UINT nCMDID, void* pData, int nLen, UINT seq, UINT userData1=0, UINT userData2=0, UINT userData3=0, UINT userData4=0);

	//发送数据
	bool Senddata(const char* pData,int DataLen);

	//在主线程中调用，用于解析并分发接收数据包。
	//接收到数据后，会向主线程发送WM_COMMAND命令，主线程再调用此函数进行解析和分发。
	//这样在窗口线程中处理数据，会比较安全
	void ParseRecvPackage(int nMode, SOCKET hConnect);

	//发送数据
	void senddata_internal(void);

private:
	static Ceasymutex	m_mutex;
	static UINT			m_nObjectCount;
	static BOOL			m_bMainThreadActive;
	static HANDLE		m_hMainThread;
	static DWORD		m_dwMainThreadID;
	static UINT			m_nRecvDataMSGID;
	static map<LPVOID, CTcpDataConnect*>	m_mapWorker;
	static map<SOCKET, CTcpDataConnect*>	m_mapConn;
	static map<SOCKET, CInstitudeApi*>		m_mapApi;

private:
	void*				m_pTcpClient;
	SOCKET				m_hConnect;
	bool				m_bStop;

	char*				m_pRcvBuf;
	int					m_RcvBufLen;
	char*				m_pSndBuf;
	int					m_SndBufLen;

	//上次发送心跳时间
	DWORD				m_HeartbeatTime;

	string				m_strSvrIP;
	WORD				m_wSvrPort;
	CInstitudeApi*		m_pApi;
};

#endif
