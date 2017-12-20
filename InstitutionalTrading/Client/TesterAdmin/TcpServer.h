
#pragma once
#include <windows.h>
#include "TcpServerWorker.h"
#include "SocketBase.h"
#include "IniFile.h"


typedef	void 	(*Init_uniServerCBFunc)(void);	
typedef	void 	(*Release_uniServerCBFunc)(void);	



//模块初始化，没什么要做的
void InitFunc(void);

//模块结束，释放资源
void ReleaseFunc(void);


//accept后，调用此回调函数进行处理
void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);


//断开tcp连接后，调用此回调函数进行处理
void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);


//从tcp得到数据后，调用此回调函数进行处理
void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);

//tcp的定时处理
void TcpSvrOntimeFunc(void);


class CTcpServer
{
public:
	CTcpServer()
	{
		pTcpSvr = NULL;
		LoadConfig();
	}
	void LoadConfig()
	{
		//获取程序路径
		char lsMainPath[MAX_PATH] = {0};
		CTools_Win32::GetMainPath(lsMainPath,sizeof(lsMainPath)-1);

		//取本机地址
		CTools_Ansi::PerformWSAStartup();
		DWORD dwLocalIP=0;
		char defLocalIP[256];
		if(!CTools_Ansi::GetLocalHostName_IP(NULL,0,&dwLocalIP,1))
			dwLocalIP=CTools_Ansi::ConvertStrIP("127.0.0.1");
		strncpy(defLocalIP,CTools_Ansi::ConvertdwIP(dwLocalIP),sizeof(defLocalIP)-1);
		defLocalIP[sizeof(defLocalIP)-1]=0;
		CTools_Ansi::PerformWSACleanup();

		CIniFile *pIniFile=NULL;
		char Buf[512];
		sprintf(Buf,"%sTestAdmin.ini",lsMainPath);
		pIniFile=new CIniFile(Buf);
		if(pIniFile)
		{			
			int FuncOrd=0;
		    TcpSvrAddr.IP=pIniFile->GetIniKeyIP("TCP","ip_TcpSvr",defLocalIP,0,FuncOrd);
			TcpSvrAddr.Port=pIniFile->GetIniKeyVal("TCP","port_TcpSvr",16,0,FuncOrd);
			IOBufSize=pIniFile->GetIniKeyVal("TCP","IOBufSize",655360,0,FuncOrd);
			TcpSvrMaxListen=pIniFile->GetIniKeyVal("TCP","TcpSvrMaxListen",16,0,FuncOrd);

			delete pIniFile;
		}

		mInitFunc=(Init_uniServerCBFunc)InitFunc;
		mReleaseFunc=(Release_uniServerCBFunc)ReleaseFunc;

		mTcpSvrRecvFunc=(TCPSVRRCVDATAHANDLER)TcpSvrRecvFunc;
		mTcpSvrAcceptFunc=(TCPSVRACCEPTHANDLER)TcpSvrAcceptFunc;
		mTcpSvrDisconnFunc=(TCPSVRDISCONNHANDLER)TcpSvrDisconnFunc;
		mTcpSvrOntimeFunc=(TCPSVRONTIME)TcpSvrOntimeFunc;

	
	}
	void CreateSvr()
	{
		if(TcpSvrAddr.Port!=0)
		{
			if(TcpSvrMaxListen==0) 
				TcpSvrMaxListen=16;
			pTcpSvr=new CTcpServerWorker(TcpSvrAddr,
				TcpSvrMaxListen,
				mTcpSvrRecvFunc,
				mTcpSvrAcceptFunc,
				mTcpSvrDisconnFunc,
				mTcpSvrOntimeFunc,
				IOBufSize,
				IOBufSize);
		}
	}

	void CloseServer(void)
	{
		if(pTcpSvr)
			delete pTcpSvr;
	
	};
public:

	CTcpServerWorker *pTcpSvr;

	Stru_IPAddress TcpSvrAddr;

	int IOBufSize;
	int TcpSvrMaxListen;
	int HeartbeatTimeout;

	Init_uniServerCBFunc mInitFunc;
	Release_uniServerCBFunc mReleaseFunc;
	TCPSVRRCVDATAHANDLER mTcpSvrRecvFunc;
	TCPSVRACCEPTHANDLER mTcpSvrAcceptFunc;
	TCPSVRDISCONNHANDLER mTcpSvrDisconnFunc;
	TCPSVRONTIME mTcpSvrOntimeFunc;
};