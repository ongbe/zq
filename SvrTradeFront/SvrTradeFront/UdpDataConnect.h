#ifndef _TCPDATACONNECT_H_
#define _TCPDATACONNECT_H_

#include "SocketBase.h"

class CUdpDataConnect
{
public:
	static CUdpDataConnect* CreateConnect(string& SvrIP, WORD SvrPort);

private:
	//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
	static void OnTimeCBFunc();

	static CUdpDataConnect* sm_pConnect;

public:
	CUdpDataConnect(string& SvrIP, WORD SvrPort);
	virtual ~CUdpDataConnect();

	bool GetSvrAddr(string& SvrIP, WORD& SvrPort) {
		SvrIP = m_strSvrIP;
		SvrPort = m_wSvrPort;
	};

	LPVOID Init();
	void Release();
	void Stop();
	void OnTimer();

	//发送数据
	int Senddata(const char* pData,int DataLen);

	//发送数据
	int senddata_internal(void);

private:
	void*				m_pUdpClient;
	SOCKET				m_hConnect;
	bool				m_bStop;

	char*				m_pSndBuf;
	int					m_SndBufLen;

	string				m_strSvrIP;
	WORD				m_wSvrPort;
};

#endif
