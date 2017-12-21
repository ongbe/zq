// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "UdpDataConnect.h"
#include "FileOpr.h"
#include "Tools_Ansi.h"
#include "CommonPkg.h"
#include "UdpSimpleWorker.h"
#include <windows.h>

#include <string>
using namespace std;


CUdpDataConnect* CUdpDataConnect::sm_pConnect = NULL;


CUdpDataConnect* CUdpDataConnect::CreateConnect(string& SvrIP, WORD SvrPort)
{
	CUdpDataConnect* pConn = new CUdpDataConnect(SvrIP, SvrPort);
	LPVOID hWorker = pConn->Init();
	sm_pConnect = pConn;
	return pConn;
}

CUdpDataConnect::CUdpDataConnect(string& SvrIP, WORD SvrPort)
{
	m_bStop = false;
	m_strSvrIP = SvrIP;
	m_wSvrPort = SvrPort;

	m_pUdpClient = NULL;
	m_hConnect = -1;

	m_pSndBuf = NULL;
	m_SndBufLen = 0;

}

CUdpDataConnect::~CUdpDataConnect()
{
	delete m_pSndBuf;
	m_pSndBuf = NULL;
}

LPVOID CUdpDataConnect::Init()
{
	LPVOID hConn = 0;

	if(!m_bStop) {
		m_pUdpClient=new CUdpSimpleWorker(
				Stru_IPAddress(0, 0), NULL, OnTimeCBFunc, 
				6553600, 6553600);
		hConn = m_pUdpClient;
	}

	return hConn;
}

void CUdpDataConnect::Release()
{
	m_bStop = true;
	CUdpSimpleWorker* pUdpClnt = (CUdpSimpleWorker*)m_pUdpClient;
	sm_pConnect = NULL;
	m_pUdpClient = NULL;

	if(pUdpClnt) {
		delete pUdpClnt;
	}
}

//系统退出前，先Stop，停止分发，再关闭各模块窗口。
void CUdpDataConnect::Stop()
{
	m_bStop=true;
}

//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
void CUdpDataConnect::OnTimeCBFunc()
{
	if(sm_pConnect != NULL)
		sm_pConnect->OnTimer();
}

//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
void CUdpDataConnect::OnTimer(void)
{
	//发送尚未发送的数据
	if(!m_bStop)
		senddata_internal();

	if(m_pUdpClient) {
		char buf[sizeof(Stru_UniPkgHead)+4];
		Stru_UniPkgHead& PkgHead=*(Stru_UniPkgHead*)buf;
		PkgHead.Set(0,CMDID_HEARTBEAT,0,0);
		*(unsigned int*)(buf+sizeof(PkgHead))=TailID_UniPkg;

		Senddata(buf,sizeof(buf));
	}

}

//发送数据
int CUdpDataConnect::Senddata(const char* pData,int DataLen)
{
	if(m_bStop) return -111;

#if 1
	DWORD dwsvrIP=CTools_Ansi::ConvertStrIP(m_strSvrIP.c_str());
	Stru_IPAddress rmtAddr(dwsvrIP,m_wSvrPort);
	return ((CUdpSimpleWorker*)m_pUdpClient)->SendData((char*)pData, DataLen, rmtAddr);
#else

	int brlt=-111;

	if(!m_pSndBuf) m_pSndBuf=(char*)malloc(65536);
	if(!m_pSndBuf||m_SndBufLen+DataLen>65536)
	{
		//CFileOpr::getObj().writelocallog("TcpLayer","senddata缓冲区不足，m_pSndBuf=%x m_SndBufLen=%d DataLen=%d TcpSendBufSize=%d m_pTcpClient=%x hSocket=%x ConnStatus=%d",
		//	m_pSndBuf,m_SndBufLen,DataLen,TcpSendBufSize,m_pTcpClient,
		//	m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getSocket():-1,
		//	m_pTcpClient?((CTcpClientWorker*)m_pTcpClient)->getConnStatus():0);
		brlt=-111;
	}
	else
	{
		memcpy(m_pSndBuf+m_SndBufLen,pData,DataLen);
		m_SndBufLen+=DataLen;

		brlt = senddata_internal();
	}

	return brlt;
#endif
}

//发送数据
int CUdpDataConnect::senddata_internal(void)
{
	int nRet = -111;
	if(m_pSndBuf&&!m_bStop&&m_SndBufLen>0&&m_pUdpClient)
	{
		DWORD dwsvrIP=CTools_Ansi::ConvertStrIP(m_strSvrIP.c_str());
		Stru_IPAddress rmtAddr(dwsvrIP,m_wSvrPort);
		int sentlen = (nRet =((CUdpSimpleWorker*)m_pUdpClient)->SendData(m_pSndBuf,m_SndBufLen,rmtAddr));
		if(sentlen>0&&sentlen<=m_SndBufLen)
		{
			memmove(m_pSndBuf,m_pSndBuf+sentlen,m_SndBufLen-sentlen);
			m_SndBufLen-=sentlen;
		}
	}
	return nRet;
}
