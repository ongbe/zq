// UdpSimpleWorker.cpp: implementation of the CUdpSimpleWorker class.
//
//////////////////////////////////////////////////////////////////////

#define FD_SETSIZE 10240		//fd_set的数组大小，系统默认为64，这里改为10240。必须在windows.h前定义

#include <windows.h>
#include "UdpSimpleWorker.h"
#include "tools_ansi.h"
#include "FileOpr.h"


#ifdef WIN32
#pragma unmanaged
#endif 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUdpSimpleWorker::CUdpSimpleWorker(Stru_IPAddress BindAddr,
									 UDPSIMPLERCVDATAHANDLER pRecvFunc,
									 UDPSIMPLEONTIME pOntimeFunc,
									 int SOSNDBUFSize,int SORCVBUFSize)
:CSocketBase(SOSNDBUFSize,SORCVBUFSize)
{
	DWORD threadId;

	m_Sem.lock();

	m_TmpRecvBufSize=65536*MaxUdpRcvPkgCntPerLoop;
	m_pTmpRecvBuf=(char*)malloc(m_TmpRecvBufSize);

	m_hSocket=INVALID_SOCKET;
	m_BindAddr=BindAddr;
	m_BindIP_Real=0;
	m_CreateSocketTime=0;
	m_ShutdownSocketTime=0;

	m_RecvFunc=pRecvFunc;
	m_OntimeFunc=pOntimeFunc;

    m_pRcvDataCBParam=(Stru_SocketRecvDataCallBackParam*)malloc(MaxUdpRcvPkgCntPerLoop*sizeof(Stru_SocketRecvDataCallBackParam));

	m_Sem.unlock();


	m_bSocketThreadNeedExit=false;
	m_hSocketThread=CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)SocketThreadWorker,this,0,&threadId);
    CFileOpr::getObj().writelocallog("threadid","TID:\t% 8u\tCUdpSimpleWorker::SocketThreadWorker",threadId);

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CUdpSimpleWorker",WriteLogLevel_DEBUGINFO,"CUdpSimpleWorker(%s/%d,...)",CTools_Ansi::ConvertdwIP(BindAddr.IP),BindAddr.Port);

}

CUdpSimpleWorker::~CUdpSimpleWorker()
{
	m_bSocketThreadNeedExit=true;
	if(m_hSocketThread)
	{
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)m_hSocketThread,8000);
		if(GetExitCodeThread((HANDLE)m_hSocketThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)m_hSocketThread,0);
		CloseHandle(m_hSocketThread);
		m_hSocketThread=NULL;
	}

	m_Sem.lock();

	OffLine_Internal();

	m_Sem.unlock();

	if(m_pTmpRecvBuf)
	{
		free(m_pTmpRecvBuf);
		m_pTmpRecvBuf=NULL;
	}
	if(m_pRcvDataCBParam)
	{
		free(m_pRcvDataCBParam);
		m_pRcvDataCBParam=NULL;
	}

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CUdpSimpleWorker",WriteLogLevel_DEBUGINFO,"~CUdpSimpleWorker(), BindAddr=%s/%d\r\n\r\n\r\n",CTools_Ansi::ConvertdwIP(m_BindAddr.IP),m_BindAddr.Port);

}


/**************************************************************************************************//**
*   @brief					GetServerAddr  获取服务器地址的列表
*   @param	N/A
*	@return					Tcp绑定的地址
*******************************************************************************************************/
Stru_IPAddress CUdpSimpleWorker::GetServerAddr(void)
{
	return m_BindAddr;
}


/**************************************************************************************************//**
*   @brief					SendData	发送数据
*   @param	[in] pData		要发送的数据
*			[in] DataLen	要发送的数据长度
*			[in] RemoteAddr	发送的目的地址，如IP/Port都为0，则向连接BindAddr的所有Socket发送
*	@return					已发送的长度
*******************************************************************************************************/
int CUdpSimpleWorker::SendData(char* pData,int DataLen,Stru_IPAddress RemoteAddr)
{
	int SendedLen=0;
	SOCKADDR_IN		SendAddr;

	if(!pData) return 0;

	if(DataLen>CSocketBase::m_SOSNDBUFSize)
		DataLen=CSocketBase::m_SOSNDBUFSize;

	m_Sem.lock();

	if(m_hSocket!=INVALID_SOCKET)
	{
		SendAddr.sin_family=AF_INET;
		SendAddr.sin_addr.s_addr=RemoteAddr.IP;
		SendAddr.sin_port=ntohs(RemoteAddr.Port);
		SendedLen=sendto(m_hSocket,pData,DataLen,0,(SOCKADDR*)&SendAddr,sizeof(SOCKADDR_IN));
	}

	m_Sem.unlock();

	return SendedLen;
}



void CUdpSimpleWorker::OffLine_Internal(void)
{
	if(m_hSocket!=INVALID_SOCKET)
	{
		shutdown(m_hSocket,2);
		closesocket(m_hSocket);
		m_hSocket=INVALID_SOCKET;
		time(&m_ShutdownSocketTime);
	}

	if(m_pWriteLog)
		m_pWriteLog->WriteLog_Fmt("CUdpSimpleWorker",WriteLogLevel_DEBUGINFO,"OffLine_Internal()");
}


/**************************************************************************************************//**
*   @brief		CreateSocket_Internal  检查tcp server 是否要创建socket
*******************************************************************************************************/
void CUdpSimpleWorker::CreateSocket_Internal(time_t CurTime)
{
	SOCKADDR_IN		TmpAddr;
	int iRlt;

	if( m_hSocket==SOCKET_ERROR&&
		CurTime-m_CreateSocketTime>=4&&
		CurTime-m_ShutdownSocketTime>=4)
	{
		if(m_BindAddr.IP!=0)
			m_BindIP_Real=m_BindAddr.IP;
		else if(m_LocalIP[0]!=0)
			m_BindIP_Real=m_LocalIP[0];
		else m_BindIP_Real=0;

		m_hSocket=CreateUdpSocket(); 
		if(m_pWriteLog)
			m_pWriteLog->WriteLog_Fmt("CUdpSimpleWorker",WriteLogLevel_DEBUGINFO,"CreateSocket_Internal - CreateUdpSocket()=%x",m_hSocket);

		if(m_hSocket!=INVALID_SOCKET&&m_BindIP_Real!=0&&m_BindAddr.Port!=0)
		{
			TmpAddr.sin_family=AF_INET;
			TmpAddr.sin_addr.s_addr=m_BindIP_Real;
			TmpAddr.sin_port=htons(m_BindAddr.Port);

			iRlt=bind(m_hSocket,(SOCKADDR*)&TmpAddr,sizeof(SOCKADDR_IN));
			if(m_pWriteLog)
				m_pWriteLog->WriteLog_Fmt("CUdpSimpleWorker",WriteLogLevel_DEBUGINFO,"CreateSocket_Internal - bind(%s/%d)=%d",CTools_Ansi::ConvertdwIP(m_BindIP_Real),m_BindAddr.Port,iRlt);

			if(iRlt==SOCKET_ERROR)
			{
				shutdown(m_hSocket,2);
				closesocket(m_hSocket);
				m_hSocket=INVALID_SOCKET;
				if(m_pWriteLog)
					m_pWriteLog->WriteLog_Fmt("CUdpSimpleWorker",WriteLogLevel_DEBUGINFO,"bind/listen错误，释放hSocket");
			}
		}
		time(&m_CreateSocketTime);
	}
}


/**************************************************************************************************//**
*   @brief		GetSocketSet_Internal  获取要select的socketset
*******************************************************************************************************/
void CUdpSimpleWorker::GetSocketSet_Internal(void* pReadSocketSet,void* pWriteSocketSet,void* pExceptSocketSet)
{
	fd_set& ReadSocketSet=*((fd_set*)pReadSocketSet);
	fd_set& WriteSocketSet=*((fd_set*)pWriteSocketSet);
	fd_set& ExceptSocketSet=*((fd_set*)pExceptSocketSet);

	memset(&ReadSocketSet,0,sizeof(ReadSocketSet));
	memset(&WriteSocketSet,0,sizeof(WriteSocketSet));
	memset(&ExceptSocketSet,0,sizeof(ExceptSocketSet));

	if(m_hSocket!=INVALID_SOCKET)
	{
		ReadSocketSet.fd_array[ReadSocketSet.fd_count]=m_hSocket;
		ReadSocketSet.fd_count++;
		ExceptSocketSet.fd_array[ExceptSocketSet.fd_count]=m_hSocket;
		ExceptSocketSet.fd_count++;

	}
}


//**************************************************
//   @brief		处理socket的意外事件
//**************************************************
void CUdpSimpleWorker::CheckSocketExcept_Internal(void* pExceptSocketSet)
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
			OffLine_Internal();
		}
	}
}


//**************************************************
//   @brief		处理Read
//**************************************************
void CUdpSimpleWorker::TryRead_Internal(void* pReadSocketSet)
{
	int i,ReadLen,iSize,ReadSize;
	SOCKET hSocket;
	SOCKADDR_IN		TmpAddr;
	Stru_IPAddress	RemoteAddr;

	if(!m_pTmpRecvBuf) return;

    int RecvBufLen=0;

	fd_set& ReadSocketSet=*((fd_set*)pReadSocketSet);

	for(i=0;i<(int)ReadSocketSet.fd_count;i++)
	{
		hSocket=ReadSocketSet.fd_array[i];
		if(hSocket==INVALID_SOCKET)
			continue;
		if(hSocket!=m_hSocket)
			continue;
		
        ReadSize=m_TmpRecvBufSize-RecvBufLen;
        if(ReadSize<=0) continue;
		//如果ReadSize>CSocketBase::m_SORCVBUFSize，在远端断开后，recv会引起WSAENOBUFS错误
		if(ReadSize>CSocketBase::m_SORCVBUFSize)
			ReadSize=CSocketBase::m_SORCVBUFSize;

		//接收数据
		memset(&TmpAddr,0,sizeof(TmpAddr));
		iSize=sizeof(TmpAddr);
		ReadLen=recvfrom(hSocket,m_pTmpRecvBuf+RecvBufLen,ReadSize,0,(SOCKADDR*)&TmpAddr,&iSize);
		if(ReadLen>0)
		{
			RemoteAddr.IP=TmpAddr.sin_addr.S_un.S_addr;
			RemoteAddr.Port=htons(TmpAddr.sin_port);

            for(int j=0;j<MaxUdpRcvPkgCntPerLoop;j++)
            {
                if(!m_pRcvDataCBParam[j].bValid)
                {
                    m_pRcvDataCBParam[j].set(m_pTmpRecvBuf+RecvBufLen,ReadLen,m_BindAddr,RemoteAddr,hSocket);
                    break;
                }
            }
            RecvBufLen+=ReadLen;
		}
	}
}

DWORD CUdpSimpleWorker::SocketThreadWorker(void *arg)
{
	CUdpSimpleWorker *pUdpSimple;
	fd_set			ReadSocketSet;
	fd_set			WriteSocketSet;
	fd_set			ExceptSocketSet;
    fd_set          BackupExceptSocketSet;
	timeval			WaitTime;
    HANDLE	        hSleepEvent=NULL;


	int				SocketCount;
	time_t			CurSecs;


	pUdpSimple=(CUdpSimpleWorker*)arg;
	if(!pUdpSimple) return -1;

	if(!pUdpSimple->m_pTmpRecvBuf||!pUdpSimple->m_pRcvDataCBParam) return -1;

	hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);


    for(int i=0;i<MaxUdpRcvPkgCntPerLoop;i++)
    {
        pUdpSimple->m_pRcvDataCBParam[i].reset();
    }

	//**************************************************
	//开始网络处理循环
	//**************************************************
	while(!pUdpSimple->m_bSocketThreadNeedExit)
	{
		time(&CurSecs);


		//**************************************************
		//检查本机的网络环境是否ready
		//**************************************************
		if(!pUdpSimple->m_LocalIP[0]&&(pUdpSimple->m_GetLocalIPTime==0||CurSecs-pUdpSimple->m_GetLocalIPTime>=4))
		{
			pUdpSimple->GetLocalMachineIP();
		}
		if(!pUdpSimple->m_LocalIP[0])
		{
    		WaitForSingleObject(hSleepEvent,1000);
			continue;
		}


		pUdpSimple->m_Sem.lock();


		//**************************************************
		//检查、创建、连接路由socket
		//**************************************************
		pUdpSimple->CreateSocket_Internal(CurSecs);

		//**************************************************
		//设置要select的socket
		//**************************************************
		pUdpSimple->GetSocketSet_Internal(&ReadSocketSet,&WriteSocketSet,&ExceptSocketSet);
		if(ReadSocketSet.fd_count==0&&WriteSocketSet.fd_count==0&&ExceptSocketSet.fd_count==0)
		{
			pUdpSimple->m_Sem.unlock();
    		WaitForSingleObject(hSleepEvent,1000);
			continue;
		}

		pUdpSimple->m_Sem.unlock();

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

		pUdpSimple->m_Sem.lock();

		//**************************************************
		//处理数据收发、接收连接等
		//**************************************************
		if(SocketCount<0)
        {
            //关闭ExceptSocketSet中的无效句柄
            pUdpSimple->CheckSocketExcept_Internal(&ExceptSocketSet);
        }

		else if(SocketCount>0)
		{
			//**************************************************
			//处理socket的意外事件
			//**************************************************
			pUdpSimple->CheckSocketExcept_Internal(&ExceptSocketSet);

			//**************************************************
			//检查和处理数据socket
			//**************************************************
			pUdpSimple->TryRead_Internal(&ReadSocketSet);

		}


		pUdpSimple->m_Sem.unlock();

        for(int i=0;i<MaxUdpRcvPkgCntPerLoop;i++)
        {
            if(pUdpSimple->m_pRcvDataCBParam[i].bValid)
            {
                if(pUdpSimple->m_RecvFunc)
                    pUdpSimple->m_RecvFunc(pUdpSimple->m_pRcvDataCBParam[i].pData,
                                            pUdpSimple->m_pRcvDataCBParam[i].DataLen,
                                            pUdpSimple->m_pRcvDataCBParam[i].LocalAddr,
                                            pUdpSimple->m_pRcvDataCBParam[i].RemoteAddr,
                                            pUdpSimple->m_pRcvDataCBParam[i].hSocket);
                pUdpSimple->m_pRcvDataCBParam[i].reset();
            }
        }

		if(pUdpSimple->m_OntimeFunc)
			pUdpSimple->m_OntimeFunc();

	}	
	

    if(hSleepEvent)
    {
	    CloseHandle(hSleepEvent); 
        hSleepEvent=NULL;
    }
	
	return 0;
}
