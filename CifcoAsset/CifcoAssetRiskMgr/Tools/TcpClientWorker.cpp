// TcpClientWorker.cpp: implementation of the CTcpClientWorker class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "TcpClientWorker.h"
#include "tools_ansi.h"

#pragma unmanaged

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTcpClientWorker::CTcpClientWorker(Stru_IPAddress LocalAddr,Stru_IPAddress SvrAddr,
									 TCPCLINTRCVDATAHANDLER pRecvFunc,
									 TCPCLINTDISCONNHANDLER pDisconnFunc,
									 TCPCLINTSTARTCONNECT pStartFunc,
									 TCPCLINTCONNSUCCESSHANDLER pConnSuccFunc,
									 TCPCLINTCONNFAILUREHANDLER pConnFailFunc,
									 TCPCLINTONTIME pOntimeFunc,
									 int SOSNDBUFSize,int SORCVBUFSize,
                                     int ReconnectInterval)
:CSocketBase(SOSNDBUFSize,SORCVBUFSize)
{
	DWORD threadId;

	m_Sem.lock();

	m_hSocket=INVALID_SOCKET;
	m_LocalAddr=LocalAddr;
	m_LocalIP_Real=0;
	m_SvrAddr=SvrAddr;
	m_CreateSocketTime=0;
	m_ShutdownSocketTime=0;
	m_ConnEndTime=0;
	m_ConnStatu=TcpClientConnStatus_Disconnected;

	m_RecvFunc=pRecvFunc;
	m_DisconnFunc=pDisconnFunc;
	m_StartFunc=pStartFunc;
	m_ConnSuccFunc=pConnSuccFunc;
	m_ConnFailFunc=pConnFailFunc;
	m_OntimeFunc=pOntimeFunc;

	m_TmpRecvBufSize=65536;
	m_pTmpRecvBuf=(char*)malloc(m_TmpRecvBufSize);

    m_ReconnectInterval=ReconnectInterval;



	m_Sem.unlock();

	m_bSocketThreadNeedExit=false;
	m_hSocketThread=CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)SocketThreadWorker,this,0,&threadId);

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"CTcpClientWorker(%s/%d,%s/%d,...)",CTools_Ansi::ConvertdwIP(LocalAddr.IP),LocalAddr.Port,CTools_Ansi::ConvertdwIP(SvrAddr.IP),SvrAddr.Port);
}

CTcpClientWorker::~CTcpClientWorker()
{
	m_bSocketThreadNeedExit=true;
	if(m_hSocketThread)
	{
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)m_hSocketThread,8000);
		if(GetExitCodeThread((HANDLE)m_hSocketThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
		{
			TerminateThread((HANDLE)m_hSocketThread,0);
		}
		CloseHandle(m_hSocketThread);
		m_hSocketThread=NULL;
	}

	m_Sem.lock();

	OffLine_Internal2();

	m_Sem.unlock();

	if(m_pTmpRecvBuf)
	{
		free(m_pTmpRecvBuf);
		m_pTmpRecvBuf=NULL;
	}

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"~CTcpClientWorker(), LocalAddr=%s/%d SvrAddr=%s/%d\r\n\r\n\r\n",CTools_Ansi::ConvertdwIP(m_LocalAddr.IP),m_LocalAddr.Port,CTools_Ansi::ConvertdwIP(m_SvrAddr.IP),m_SvrAddr.Port);
}


/**************************************************************************************************//**
*   @brief				GetAddr  获取地址
*   @param	LocalAddr	返回本地地址
*			SvrAddr		返回服务器地址
*	@return				Tcp绑定的地址
*******************************************************************************************************/
void CTcpClientWorker::GetAddr(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr)
{
	LocalAddr=m_LocalAddr;
	SvrAddr=m_SvrAddr;
}


/**************************************************************************************************//**
*   @brief					SendData	发送数据
*   @param	[in] pData		要发送的数据
*			[in] DataLen	要发送的数据长度
*	@return					已发送的长度
*******************************************************************************************************/
int CTcpClientWorker::SendData(char* pData,int DataLen)
{
	int SendedLen=0;

	if(!pData) return 0;

	if(DataLen>CSocketBase::m_SOSNDBUFSize)
		DataLen=CSocketBase::m_SOSNDBUFSize;

	m_Sem.lock();

	if(m_hSocket!=INVALID_SOCKET&&m_ConnStatu==TcpClientConnStatus_Connected)
	{
		SendedLen=send(m_hSocket,pData,DataLen,0);
	}

	m_Sem.unlock();

	return SendedLen;
}


void CTcpClientWorker::OffLine_Internal(Stru_SocketCallBackParam& DisconnCBParam)
{
	if(m_hSocket!=INVALID_SOCKET)
	{
        DisconnCBParam.set(m_LocalAddr,m_SvrAddr,m_hSocket);

        shutdown(m_hSocket,2);
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
		time(&m_ShutdownSocketTime);
	}
	m_ConnStatu=TcpClientConnStatus_Disconnected;

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"OffLine_Internal()");
}

void CTcpClientWorker::OffLine_Internal2(void)
{
	if(m_hSocket!=INVALID_SOCKET)
	{
		shutdown(m_hSocket,2);
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
		time(&m_ShutdownSocketTime);
	}
	m_ConnStatu=TcpClientConnStatus_Disconnected;

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"OffLine_Internal2()");
}



/**************************************************************************************************//**
*   @brief		CreateSocket_Internal  检查tcp server 是否要创建socket
*******************************************************************************************************/
void CTcpClientWorker::CreateSocket_Internal(time_t CurTime,
                                             Stru_SocketCallBackParam& DisconnCBParam,
                                             Stru_SocketCallBackParam& StartCBParam,
                                             Stru_SocketCallBackParam& ConnSuccCBParam,
                                             Stru_SocketCallBackParam& ConnFailCBParam)
{
	SOCKADDR_IN		TmpAddr;
	int iRlt,ErrCode;

	if( m_ReconnectInterval>0&&
        m_hSocket==SOCKET_ERROR&&
		CurTime-m_CreateSocketTime>=4&&
		CurTime-m_ShutdownSocketTime>=4&&
		CurTime-m_ConnEndTime>m_ReconnectInterval)
	{
		if(m_LocalAddr.IP!=0)
			m_LocalIP_Real=m_LocalAddr.IP;
		else if(m_LocalIP[0]!=0)
			m_LocalIP_Real=m_LocalIP[0];
		else m_LocalIP_Real=0;

		m_hSocket=CreateTcpSocket(); 
		if(m_pWriteLog)
			m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"CreateSocket_Internal - CreateTcpSocket()=%x",m_hSocket);

		time(&m_CreateSocketTime);

		if(m_hSocket!=INVALID_SOCKET&&m_LocalIP_Real!=0&&m_LocalAddr.Port!=0)
		{
			TmpAddr.sin_family=AF_INET;
			TmpAddr.sin_addr.s_addr=m_LocalIP_Real;
			TmpAddr.sin_port=htons(m_LocalAddr.Port);

			iRlt=bind(m_hSocket,(SOCKADDR*)&TmpAddr,sizeof(SOCKADDR_IN));
			if(m_pWriteLog)
				m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"CreateSocket_Internal - bind(%s/%d)=%d",CTools_Ansi::ConvertdwIP(m_LocalIP_Real),m_LocalAddr.Port,iRlt);

			if(iRlt==SOCKET_ERROR)
			{
				OffLine_Internal(DisconnCBParam);

				if(m_pWriteLog)
					m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"bind/listen错误，释放hSocket");
			}
		}

		if(m_hSocket!=INVALID_SOCKET)
		{
			TmpAddr.sin_family=AF_INET;
			TmpAddr.sin_addr.s_addr=m_SvrAddr.IP;
			TmpAddr.sin_port=htons(m_SvrAddr.Port);

			iRlt=connect(m_hSocket,(struct sockaddr*)&TmpAddr,sizeof(SOCKADDR));

			if(iRlt==SOCKET_ERROR)
				ErrCode=WSAGetLastError();
			else ErrCode=0;

			if(m_pWriteLog)
				m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"connect(%x,%s/%d)",
											m_hSocket,CTools_Ansi::ConvertdwIP(m_SvrAddr.IP),m_SvrAddr.Port);

            StartCBParam.set(m_LocalAddr,m_SvrAddr,m_hSocket);

			if(iRlt==0)
			{
				m_ConnStatu=TcpClientConnStatus_Connected;
				time(&m_ConnEndTime);

                ConnSuccCBParam.set(m_LocalAddr,m_SvrAddr,m_hSocket);
			}
			else if(iRlt==SOCKET_ERROR)
			{
				if(ErrCode==WSAEWOULDBLOCK)
				{
					m_ConnStatu=TcpClientConnStatus_Connecting;
				}
				else if(ErrCode==WSAEISCONN)
				{
					m_ConnStatu=TcpClientConnStatus_Connected;
					time(&m_ConnEndTime);

                    ConnSuccCBParam.set(m_LocalAddr,m_SvrAddr,m_hSocket);
				}
				else
				{
					m_ConnStatu=TcpClientConnStatus_Disconnected;
					time(&m_ConnEndTime);

                    ConnFailCBParam.set(m_LocalAddr,m_SvrAddr,m_hSocket);

					OffLine_Internal(DisconnCBParam);
					if(m_pWriteLog)
						m_pWriteLog->WriteLog_Fmt("CTcpClientWorker",WriteLogLevel_DEBUGINFO,"connect失败，释放hSocket,iRlt=%d,ErrCode=%d",iRlt,ErrCode);
				}
			}
		}
	}
}


/**************************************************************************************************//**
*   @brief		GetSocketSet_Internal  获取要select的socketset
*******************************************************************************************************/
void CTcpClientWorker::GetSocketSet_Internal(void* pReadSocketSet,void* pWriteSocketSet,void* pExceptSocketSet)
{
	fd_set& ReadSocketSet=*((fd_set*)pReadSocketSet);
	fd_set& WriteSocketSet=*((fd_set*)pWriteSocketSet);
	fd_set& ExceptSocketSet=*((fd_set*)pExceptSocketSet);

	memset(&ReadSocketSet,0,sizeof(ReadSocketSet));
	memset(&WriteSocketSet,0,sizeof(WriteSocketSet));
	memset(&ExceptSocketSet,0,sizeof(ExceptSocketSet));

	if(m_hSocket!=INVALID_SOCKET&&m_ConnStatu==TcpClientConnStatus_Connected)
	{
		ReadSocketSet.fd_array[ReadSocketSet.fd_count]=m_hSocket;
		ReadSocketSet.fd_count++;
		ExceptSocketSet.fd_array[ExceptSocketSet.fd_count]=m_hSocket;
		ExceptSocketSet.fd_count++;
	}
	else if(m_hSocket!=INVALID_SOCKET&&m_ConnStatu==TcpClientConnStatus_Connecting)
	{
		WriteSocketSet.fd_array[WriteSocketSet.fd_count]=m_hSocket;
		WriteSocketSet.fd_count++;
		ExceptSocketSet.fd_array[ExceptSocketSet.fd_count]=m_hSocket;
		ExceptSocketSet.fd_count++;
	}
}


//**************************************************
//   @brief		处理socket的意外事件
//**************************************************
void CTcpClientWorker::CheckSocketExcept_Internal(void* pExceptSocketSet,Stru_SocketCallBackParam& DisconnCBParam,Stru_SocketCallBackParam& ConnFailCBParam)
{
	int i;
	SOCKET hSocket;

	fd_set& ExceptSocketSet=*((fd_set*)pExceptSocketSet);

	for(i=0;i<(int)ExceptSocketSet.fd_count;i++)
	{
		hSocket=ExceptSocketSet.fd_array[i];
		if(hSocket==INVALID_SOCKET)
			continue;
		
		if(hSocket==m_hSocket)
		{
			if(m_ConnStatu==TcpClientConnStatus_Connecting)
			{
				time(&m_ConnEndTime);

				ConnFailCBParam.set(m_LocalAddr,m_SvrAddr,m_hSocket);
			}

			OffLine_Internal(DisconnCBParam);
		}
	}
}


//**************************************************
//   @brief		处理Read
//**************************************************
void CTcpClientWorker::TryRead_Internal(void *pReadSocketSet,
                                        Stru_SocketRecvDataCallBackParam& RcvDataCBParam,
                                        Stru_SocketCallBackParam& DisconnCBParam,
                                        Stru_SocketCallBackParam& ConnFailCBParam)
{
	int i,ReadLen,ErrCode;
	Stru_IPAddress	RemoteAddr;

	fd_set& ReadSocketSet=*((fd_set*)pReadSocketSet);

    RcvDataCBParam.DataLen=0;

    if(!m_pTmpRecvBuf) return;
	if(m_hSocket==INVALID_SOCKET)
		return;

	for(i=0;i<(int)ReadSocketSet.fd_count;i++)
	{
		if(ReadSocketSet.fd_array[i]==m_hSocket)
			break;
	}
	if(i>=(int)ReadSocketSet.fd_count) 
		return;

	int ReadSize=m_TmpRecvBufSize;
	
	//如果ReadSize>CSocketBase::m_SORCVBUFSize，在远端断开后，recv会引起WSAENOBUFS错误
	if(ReadSize>CSocketBase::m_SORCVBUFSize)
		ReadSize=CSocketBase::m_SORCVBUFSize;

    ReadLen=recv(m_hSocket,m_pTmpRecvBuf,ReadSize,0);
	if(ReadLen>0)
	{
        RcvDataCBParam.set(m_pTmpRecvBuf,ReadLen,m_LocalAddr,m_SvrAddr,m_hSocket);
	}
	else if(ReadLen==0||
			ReadLen==SOCKET_ERROR&&
			((ErrCode=WSAGetLastError())==WSAECONNRESET||ErrCode==WSAENETDOWN||
			 ErrCode==WSAEFAULT||ErrCode==WSAENOTCONN||ErrCode==WSAENETRESET||
			 ErrCode==WSAENOTSOCK||ErrCode==WSAESHUTDOWN||ErrCode==WSAECONNABORTED||
			 ErrCode==WSAETIMEDOUT))
	{	//远端正常或不正常的关闭了socket

		//记录日志
		if(m_pWriteLog)
			m_pWriteLog->WriteLog_Fmt(	"TryRead_Internal",
										WriteLogLevel_DEBUGINFO,
										"数据连接的远端正常或不正常的关闭。释放该数据连接 (LocalAddr=%s/%d SvrAddr=%s/%d hSocket=%x)",
										CTools_Ansi::ConvertdwIP(m_LocalAddr.IP),m_LocalAddr.Port,
										CTools_Ansi::ConvertdwIP(m_SvrAddr.IP),m_SvrAddr.Port,
										m_hSocket);

		if(m_ConnStatu==TcpClientConnStatus_Connecting)
		{
			time(&m_ConnEndTime);

            ConnFailCBParam.set(m_LocalAddr,m_SvrAddr,m_hSocket);
		}

		//将hDataSocket下线
		OffLine_Internal(DisconnCBParam);
	}
}


//**************************************************
//   @brief		处理Write
//**************************************************
void CTcpClientWorker::TryWrite_Internal(void* pWriteSocketSet,Stru_SocketCallBackParam& ConnSuccCBParam)
{
	int i;
	Stru_IPAddress	RemoteAddr;

	fd_set& WriteSocketSet=*((fd_set*)pWriteSocketSet);

	if(m_hSocket==INVALID_SOCKET)
		return;

	for(i=0;i<(int)WriteSocketSet.fd_count;i++)
	{
		if(WriteSocketSet.fd_array[i]==m_hSocket)
			break;
	}
	if(i>=(int)WriteSocketSet.fd_count) 
		return;

	if(m_ConnStatu==TcpClientConnStatus_Connecting)
	{
		m_ConnStatu=TcpClientConnStatus_Connected;

        ConnSuccCBParam.set(m_LocalAddr,m_SvrAddr,m_hSocket);

        if(m_pWriteLog)
			m_pWriteLog->WriteLog_Fmt(	"TryWrite_Internal",
										WriteLogLevel_DEBUGINFO,
										"连接建立 (LocalAddr=%s/%d SvrAddr=%s/%d hSocket=%x)",
										CTools_Ansi::ConvertdwIP(m_LocalAddr.IP),m_LocalAddr.Port,
										CTools_Ansi::ConvertdwIP(m_SvrAddr.IP),m_SvrAddr.Port,
										m_hSocket);
	}
}

DWORD CTcpClientWorker::SocketThreadWorker(void *arg)
{
	CTcpClientWorker *pTcpClient;
	fd_set			ReadSocketSet;
	fd_set			WriteSocketSet;
	fd_set			ExceptSocketSet;
    fd_set          BackupExceptSocketSet;
	timeval			WaitTime;
    HANDLE	        hSleepEvent=NULL;

	int				SocketCount;
	time_t			CurSecs;

    Stru_SocketRecvDataCallBackParam RcvDataCBParam;
    Stru_SocketCallBackParam DisconnCBParam;
    Stru_SocketCallBackParam StartCBParam;
    Stru_SocketCallBackParam ConnSuccCBParam;
    Stru_SocketCallBackParam ConnFailCBParam;

	pTcpClient=(CTcpClientWorker*)arg;
	if(!pTcpClient) return 0;


	hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);



	//**************************************************
	//开始网络处理循环
	//**************************************************
	while(!pTcpClient->m_bSocketThreadNeedExit)
	{
		time(&CurSecs);
        RcvDataCBParam.reset();
        DisconnCBParam.reset();
        StartCBParam.reset();
        ConnSuccCBParam.reset();
        ConnFailCBParam.reset();


		//**************************************************
		//检查本机的网络环境是否ready
		//**************************************************
		if(!pTcpClient->m_LocalIP[0]&&(pTcpClient->m_GetLocalIPTime==0||CurSecs-pTcpClient->m_GetLocalIPTime>=4))
		{
			pTcpClient->GetLocalMachineIP();
		}
		if(!pTcpClient->m_LocalIP[0])
		{
    		WaitForSingleObject(hSleepEvent,1000);
			continue;
		}


		pTcpClient->m_Sem.lock();


		//**************************************************
		//检查、创建、连接路由socket
		//**************************************************
		pTcpClient->CreateSocket_Internal(CurSecs,DisconnCBParam,StartCBParam,ConnSuccCBParam,ConnFailCBParam);
        if(StartCBParam.bValid||ConnSuccCBParam.bValid||ConnFailCBParam.bValid)
		{
			pTcpClient->m_Sem.unlock();

			if(pTcpClient->m_StartFunc&&StartCBParam.bValid)
				pTcpClient->m_StartFunc(StartCBParam.LocalAddr,StartCBParam.RemoteAddr,StartCBParam.hSocket);

			if(pTcpClient->m_ConnSuccFunc&&ConnSuccCBParam.bValid)
				pTcpClient->m_ConnSuccFunc(ConnSuccCBParam.LocalAddr,ConnSuccCBParam.RemoteAddr,ConnSuccCBParam.hSocket);

			if(pTcpClient->m_ConnFailFunc&&ConnFailCBParam.bValid)
				pTcpClient->m_ConnFailFunc(ConnFailCBParam.LocalAddr,ConnFailCBParam.RemoteAddr,ConnFailCBParam.hSocket);

			continue;
		}


		//**************************************************
		//设置要select的socket
		//**************************************************
		pTcpClient->GetSocketSet_Internal(&ReadSocketSet,&WriteSocketSet,&ExceptSocketSet);
		if(ReadSocketSet.fd_count==0&&WriteSocketSet.fd_count==0&&ExceptSocketSet.fd_count==0)
		{
			pTcpClient->m_Sem.unlock();
    		WaitForSingleObject(hSleepEvent,1000);
			continue;
		}

		pTcpClient->m_Sem.unlock();

        //备份所有socket句柄
        BackupExceptSocketSet=ExceptSocketSet;

		//**************************************************
		//执行select
		//**************************************************
		WaitTime.tv_sec=1;
		WaitTime.tv_usec=0;
		SocketCount=select(NULL,&ReadSocketSet,&WriteSocketSet,&ExceptSocketSet,&WaitTime);

		time(&CurSecs);

        if(SocketCount<0)
        {
            memset(&ExceptSocketSet,0,sizeof(ExceptSocketSet));

            int ErrNo=WSAGetLastError();
            if(ErrNo==WSAENOTSOCK)
            {
                //有非法套接字，需要一个个检查
	            fd_set			tmpExceptSocketSet;
                timeval			tmpWaitTime;
                tmpWaitTime.tv_sec=0;
                tmpWaitTime.tv_usec=0;
                for(int i=0;i<(int)BackupExceptSocketSet.fd_count;i++)
                {
                    tmpExceptSocketSet.fd_array[0]=BackupExceptSocketSet.fd_array[i];
                    tmpExceptSocketSet.fd_count=1;
                    if(select(NULL,NULL,NULL,&tmpExceptSocketSet,&tmpWaitTime)<0&&
                        WSAGetLastError()==WSAENOTSOCK)
                    {
                        //放到ExceptSocketSet中去
				        ExceptSocketSet.fd_array[ExceptSocketSet.fd_count]=BackupExceptSocketSet.fd_array[i];
				        ExceptSocketSet.fd_count++;
                    }
                }
            }
        }

		pTcpClient->m_Sem.lock();

		//**************************************************
		//处理数据收发、接收连接等
		//**************************************************
		if(SocketCount<0)
        {
            //关闭ExceptSocketSet中的无效句柄
            pTcpClient->CheckSocketExcept_Internal(&ExceptSocketSet,DisconnCBParam,ConnFailCBParam);
        }

		else if(SocketCount>0)
		{
			//**************************************************
			//处理socket的意外事件
			//**************************************************
			pTcpClient->CheckSocketExcept_Internal(&ExceptSocketSet,DisconnCBParam,ConnFailCBParam);

			//**************************************************
			//检查和处理数据socket
			//**************************************************
			pTcpClient->TryRead_Internal(&ReadSocketSet,RcvDataCBParam,DisconnCBParam,ConnFailCBParam);

			//**************************************************
			//检查和处理数据socket
			//**************************************************
			pTcpClient->TryWrite_Internal(&WriteSocketSet,ConnSuccCBParam);

		}


		pTcpClient->m_Sem.unlock();



		if(pTcpClient->m_DisconnFunc&&DisconnCBParam.bValid)
			pTcpClient->m_DisconnFunc(DisconnCBParam.LocalAddr,DisconnCBParam.RemoteAddr,DisconnCBParam.hSocket);

        if(pTcpClient->m_RecvFunc&&RcvDataCBParam.bValid&&RcvDataCBParam.DataLen>0)
            pTcpClient->m_RecvFunc(RcvDataCBParam.pData,RcvDataCBParam.DataLen,RcvDataCBParam.LocalAddr,RcvDataCBParam.RemoteAddr,RcvDataCBParam.hSocket);

        if(pTcpClient->m_ConnFailFunc&&ConnFailCBParam.bValid)
			pTcpClient->m_ConnFailFunc(ConnFailCBParam.LocalAddr,ConnFailCBParam.RemoteAddr,ConnFailCBParam.hSocket);

        if(pTcpClient->m_ConnSuccFunc&&ConnSuccCBParam.bValid)
			pTcpClient->m_ConnSuccFunc(ConnSuccCBParam.LocalAddr,ConnSuccCBParam.RemoteAddr,ConnSuccCBParam.hSocket);

		if(pTcpClient->m_OntimeFunc)
			pTcpClient->m_OntimeFunc();


	}	
	

    if(hSleepEvent)
    {
	    CloseHandle(hSleepEvent); 
        hSleepEvent=NULL;
    }
	
	return 0;
}
