// MonitorSvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include "stdafx.h"
#include "DataManager.h"
#include "SendData.h"
#include "MonitorSvr.h"
#include "SocketBase.h"
#include "WinService.h"
#include "CommonPkg.h"

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif

void Load(std::string& UDPSvrIP, unsigned short& UDPSvrPort, std::string& strTime);

#define WRITELOGID 0

#include <map>
#include <string>
using namespace std;

MonitorSvr_API void InitFunc(void)
{
	std::string		UDPSvrIP;
	unsigned short	UDPSvrPort;
	std::string strTime;
	Load(UDPSvrIP, UDPSvrPort, strTime);

	CDataManager::Init();
	CSendData::Init();	
	CUDPDataService::GetInstance()->Connect(UDPSvrIP, UDPSvrPort);		
}

MonitorSvr_API void ReleaseFunc(void)
{
   
	CDataManager::Release();
	CSendData::Release();
}


MonitorSvr_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{//收到客户端心跳
	
	RecvDataMsg data;
	memset(&data, 0, sizeof(data));
	memcpy(&data, pData, sizeof(Stru_UniPkgUDPHead));
	memcpy(data.pData, pData+sizeof(Stru_UniPkgUDPHead), DataLen - sizeof(Stru_UniPkgUDPHead)- sizeof(TAIL)); 
	data.hSocket = hSocket;
	data.RemoteAddr = RemoteAddr;
	CSendData::Get()->AddClientMsg(data);
	
	/*
if(0)
{
#ifdef _DEBUG
	static int test = 1;
	int nRand = rand()%100;
	static DWORD cmdid = FTTCMD_ReqUserLogin;
	{
		Stru_UniPkgUDPHead Pkg;
		Pkg.headid =HeadID_UniPkg;
		Pkg.cmdid = cmdid;
		Pkg.len = sizeof(int);
		Pkg.seq = 88;
		Pkg.subseq = 0;
		Pkg.userdata1 = 89;
		Pkg.userdata2 = 1;
		Pkg.userdata3 = test;
		Pkg.userdata4 = 8569;
		Pkg.nUTCTime  = time(NULL);
		Pkg.nMiliSecond = nRand;

		char pBuf[sizeof(Stru_UniPkgUDPHead)+sizeof(int)+4];
		memcpy(pBuf, &Pkg, sizeof(Stru_UniPkgUDPHead));		
		int nData = 11;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), &nData, sizeof(nData));
		TAIL tail = TailID_UniPkg;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+Pkg.len, &tail, sizeof(TAIL));
		CSocketBase::SendDataUdp(hSocket,RemoteAddr,pBuf,sizeof(Stru_UniPkgUDPHead)+sizeof(int)+4);
	}
	{
		Stru_UniPkgUDPHead Pkg;
		Pkg.headid =HeadID_UniPkg;
		Pkg.cmdid = cmdid;
		Pkg.len = sizeof(int);
		Pkg.seq = 88;
		Pkg.subseq = 0;
		Pkg.userdata1 = 89;
		Pkg.userdata2 = 2;
		Pkg.userdata3 = test;
		Pkg.userdata4 = 8569;
		Pkg.nUTCTime  = time(NULL);		
		Pkg.nMiliSecond = nRand + rand()%900;

		char pBuf[sizeof(Stru_UniPkgUDPHead)+sizeof(int)+4];
		memcpy(pBuf, &Pkg, sizeof(Stru_UniPkgUDPHead));		
		int nData = 22;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), &nData, sizeof(nData));
		TAIL tail = TailID_UniPkg;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+Pkg.len, &tail, sizeof(TAIL));
		CSocketBase::SendDataUdp(hSocket,RemoteAddr,pBuf,sizeof(Stru_UniPkgUDPHead)+sizeof(int)+4);
	}
	test++;	
	nRand = rand()%500;
	{
		Stru_UniPkgUDPHead Pkg;
		Pkg.headid =HeadID_UniPkg;
		Pkg.cmdid = cmdid;
		Pkg.len = sizeof(int);
		Pkg.seq = 88;
		Pkg.subseq = 0;
		Pkg.userdata1 = 89;
		Pkg.userdata2 = 3;
		Pkg.userdata3 = test;
		Pkg.userdata4 = 8569;
		Pkg.nUTCTime  = time(NULL);
		Pkg.nMiliSecond = nRand;

		char pBuf[sizeof(Stru_UniPkgUDPHead)+sizeof(int)+4];
		memcpy(pBuf, &Pkg, sizeof(Stru_UniPkgUDPHead));		
		int nData = 33;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), &nData, sizeof(nData));
		TAIL tail = TailID_UniPkg;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+Pkg.len, &tail, sizeof(TAIL));
		CSocketBase::SendDataUdp(hSocket,RemoteAddr,pBuf,sizeof(Stru_UniPkgUDPHead)+sizeof(int)+4);
	}
	{
		Stru_UniPkgUDPHead Pkg;
		Pkg.headid =HeadID_UniPkg;
		Pkg.cmdid = cmdid;
		Pkg.len = sizeof(int);
		Pkg.seq = 88;
		Pkg.subseq = 0;
		Pkg.userdata1 = 89;
		Pkg.userdata2 = 4;
		Pkg.userdata3 = test;
		Pkg.userdata4 = 8569;
		Pkg.nUTCTime  = time(NULL);
		Pkg.nMiliSecond = nRand + rand()%500;

		char pBuf[sizeof(Stru_UniPkgUDPHead)+sizeof(int)+4];
		memcpy(pBuf, &Pkg, sizeof(Stru_UniPkgUDPHead));		
		int nData = 44;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead), &nData, sizeof(nData));
		TAIL tail = TailID_UniPkg;
		memcpy(pBuf+sizeof(Stru_UniPkgUDPHead)+Pkg.len, &tail, sizeof(TAIL));
		CSocketBase::SendDataUdp(hSocket,RemoteAddr,pBuf,sizeof(Stru_UniPkgUDPHead)+sizeof(int)+4);
	}
	test++;
	if(FTTCMD_ReqQueryBankAccountMoneyByFuture_Rsp == cmdid)
		cmdid = FTTCMD_OnFrontConnected_RON;
	else if(FTTCMD_OnRspQueryBankAccountMoneyByFuture_RON == cmdid)
		cmdid = FTTCMD_ReqUserLogin;
	else
		cmdid++;
#endif
}*/

}
MonitorSvr_API void UdpOntimeFunc(void)
{
	//掉线的udp客户端去掉发送连接
	CDataManager::Get()->ChangeAllUnConnectTime();

}
