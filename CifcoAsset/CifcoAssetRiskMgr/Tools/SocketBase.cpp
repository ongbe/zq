// SocketBase.cpp: implementation of the CSocketBase class.
//
//////////////////////////////////////////////////////////////////////

#include <winsock2.h>       
#include "SocketBase.h"


#pragma comment(lib, "ws2_32.lib")											

#pragma unmanaged

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSocketBase::CSocketBase(int SOSNDBUFSize,int SORCVBUFSize)
:m_SOSNDBUFSize(SOSNDBUFSize),
 m_SORCVBUFSize(SORCVBUFSize)
{
	WSADATA			wsaData;

	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"socketbase.log");

	memset(m_LocalIP,0,sizeof(m_LocalIP));
	m_GetLocalIPTime=0;


	WSAStartup(MAKEWORD(2,2),&wsaData);	
	
	GetLocalMachineIP();

}

CSocketBase::~CSocketBase()
{
	WSACleanup();       

	if(m_pWriteLog)
	{
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}

}

bool CSocketBase::GetLocalMachineIP(void)
{
	int				i,Count;
	HOSTENT			*pHost;
	DWORD			**ppAddress;
	char			LocalHostName[64];


	memset(m_LocalIP,0,sizeof(m_LocalIP));
	time(&m_GetLocalIPTime);

	memset((char*)LocalHostName,0,sizeof(LocalHostName));

 	gethostname(LocalHostName,sizeof(LocalHostName)-1);
	if(!LocalHostName[0]) return false;

	pHost=gethostbyname(LocalHostName);
	if(!pHost) return false;
	if(pHost->h_length!=4||pHost->h_addrtype!=2) return false;

	ppAddress=(DWORD**)(pHost->h_addr_list);
	if(!ppAddress) return false;

	for(i=0,Count=0;i<5&&Count<ArrayElements(m_LocalIP);i++)
	{
		if(ppAddress[i]==NULL)
			break;
		if( (*(ppAddress[i])) != 0x0100007f)
		{
			m_LocalIP[Count]=*(ppAddress[i]);
			Count++;
		}
	}

	if(ppAddress[0]!=NULL&&m_LocalIP[0]==0)
	{
		m_LocalIP[0]=*(ppAddress[0]);
		Count=1;
	}

	
	return m_LocalIP[0]==0?false:true;
}





SOCKET CSocketBase::CreateUdpSocket(void)
{
	SOCKET RltSocket;
	linger iLinger;
	int iSize;

	RltSocket=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); 
	if(RltSocket!=INVALID_SOCKET)
	{
		iSize=m_SORCVBUFSize;
		setsockopt(RltSocket,SOL_SOCKET,SO_RCVBUF,(char *)&iSize,sizeof(int));
		iSize=m_SOSNDBUFSize;
		setsockopt(RltSocket,SOL_SOCKET,SO_SNDBUF,(char *)&iSize,sizeof(int));
		iLinger.l_onoff=0;
		iLinger.l_linger=0;
		setsockopt(RltSocket,SOL_SOCKET,SO_LINGER,(char*)&iLinger,sizeof(iLinger));
	}
	return RltSocket;
}

SOCKET CSocketBase::CreateTcpSocket(void)
{
	SOCKET RltSocket;
	linger iLinger;
	BOOL bRlt1;
	int iSize;

	RltSocket=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP); 
	if(RltSocket!=INVALID_SOCKET)
	{
		iSize=m_SORCVBUFSize;
		setsockopt(RltSocket,SOL_SOCKET,SO_RCVBUF,(char *)&iSize,sizeof(int));
		iSize=m_SOSNDBUFSize;
		setsockopt(RltSocket,SOL_SOCKET,SO_SNDBUF,(char *)&iSize,sizeof(int));
		iSize=1;											//设置为非阻塞模式
		ioctlsocket(RltSocket,FIONBIO,(u_long*)&iSize);						
		bRlt1=FALSE;										//带外数据不在数据流中提供
		setsockopt(RltSocket,SOL_SOCKET,SO_OOBINLINE,(char *)&bRlt1,sizeof(BOOL));			
		iLinger.l_onoff=0;
		iLinger.l_linger=0;
		setsockopt(RltSocket,SOL_SOCKET,SO_LINGER,(char*)&iLinger,sizeof(iLinger));
	}
	return RltSocket;
}


SOCKET CSocketBase::AcceptTcpSocket(SOCKET ListenSocket,unsigned int& RemoteIP,unsigned short& RemotePort)
{
	SOCKET RltSocket;
	int iSize;
	linger iLinger;
	BOOL bRlt1;
	SOCKADDR_IN RemoteAddr;

	iSize=sizeof(SOCKADDR_IN);
	RltSocket=accept(ListenSocket,(SOCKADDR*)&RemoteAddr,&iSize);
	if(RltSocket!=INVALID_SOCKET)
	{
		RemoteIP=RemoteAddr.sin_addr.S_un.S_addr;
		RemotePort=htons(RemoteAddr.sin_port);

		iSize=m_SOSNDBUFSize;
		setsockopt(RltSocket,SOL_SOCKET,SO_SNDBUF,(char*)&iSize,4);
		iSize=m_SORCVBUFSize;
		setsockopt(RltSocket,SOL_SOCKET,SO_RCVBUF,(char*)&iSize,4);
		iSize=1;					//设置为非阻塞模式
		ioctlsocket(RltSocket,FIONBIO,(u_long*)&iSize);							
		bRlt1=FALSE;				//带外数据不在数据流中提供
		setsockopt(RltSocket,SOL_SOCKET,SO_OOBINLINE,(char *)&bRlt1,sizeof(BOOL));
		iLinger.l_onoff=0;
		iLinger.l_linger=0;
		setsockopt(RltSocket,SOL_SOCKET,SO_LINGER,(char*)&iLinger,sizeof(iLinger));
	}
	return RltSocket;
}

int CSocketBase::SendDataTcp(SOCKET hSocket,const void* pData,int DataLen)
{
    return send(hSocket,(const char*)pData,DataLen,0);
}

int CSocketBase::SendDataUdp(SOCKET hSocket,Stru_IPAddress RemoteAddr,const void* pData,int DataLen)
{
	SOCKADDR_IN		SendAddr;
    SendAddr.sin_family=AF_INET;
    SendAddr.sin_addr.s_addr=RemoteAddr.IP;
    SendAddr.sin_port=ntohs(RemoteAddr.Port);
    return sendto(hSocket,(const char*)pData,DataLen,0,(SOCKADDR*)&SendAddr,sizeof(SOCKADDR_IN));
}

void CSocketBase::CloseSocket(SOCKET hSocket)
{
	shutdown(hSocket,2);
	closesocket(hSocket);
}
