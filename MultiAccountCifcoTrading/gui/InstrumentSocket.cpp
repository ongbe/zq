#include "StdAfx.h"
#include "InstrumentSocket.h"
#include "../inc/Module-Misc/packagecache.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

#define PACKAGEHEAD   0x7a8b0a6d
#define PACKAGETAIL   0xa38ca16a
#define CMD_ReqCTPMainContractsInfo 	41	//请求CTP主力合约信息
#define CMD_CTPMainContractsInfo		42	//发送CTP主力合约信息
#pragma pack(push)
#pragma pack(4)
//CTP主力合约信息请求包
typedef struct
{
	DWORD	Head;					//包头
	int		PackageLen;				//包长度
	DWORD	CRCValue;				//CRC校验值
	DWORD	Cmd;					//命令字
	char	UserName[32];			//用户名
	char	UserPW[32];				//密码
	DWORD	Tail;					//包尾
}Stru_ReqCTPMainContractsInfo;
#pragma pack(pop)
CInstrumentSocket::CInstrumentSocket(string strAddr):
m_SORCVBUFSize(655360),
m_SOSNDBUFSize(655360)
{
	m_pTmpRecvBuf=new char[655360];
	SOCKADDR_IN		TmpAddr;
	int iRlt;

	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		return;
	Stru_ReqCTPMainContractsInfo  ReqIBContractsInfo;
	memset(&ReqIBContractsInfo,0,sizeof(Stru_ReqCTPMainContractsInfo));
	m_hSocket=CreateTcpSocket();
	if(m_hSocket!=INVALID_SOCKET)
	{
						std::string apppath;
				GlobalFunc::GetPath(apppath);
				apppath+="ctpmaincontractsinfo.txt";
				wxFile file;
			TmpAddr.sin_family=AF_INET;
			int dotpos=strAddr.find(':');
			string ipstr=strAddr.substr(0,dotpos);
			TmpAddr.sin_addr.s_addr=inet_addr(ipstr.c_str());
			string portstr=strAddr.substr(dotpos+1);
			TmpAddr.sin_port=htons(atoi(portstr.c_str()));
			iRlt=connect(m_hSocket,(struct sockaddr*)&TmpAddr,sizeof(SOCKADDR));
			if(iRlt!=SOCKET_ERROR)
			{
				ReqIBContractsInfo.Head=PACKAGEHEAD;
				ReqIBContractsInfo.PackageLen=sizeof(Stru_ReqCTPMainContractsInfo);
				ReqIBContractsInfo.CRCValue=0;
				ReqIBContractsInfo.Cmd=CMD_ReqCTPMainContractsInfo;
				strcpy(ReqIBContractsInfo.UserName,"FastTraderUser");
				strcpy(ReqIBContractsInfo.UserPW,"FastTraderUserPW");
				ReqIBContractsInfo.Tail=PACKAGETAIL;
                int SendedLen=send(m_hSocket,(const char*)&ReqIBContractsInfo,ReqIBContractsInfo.PackageLen,0);
				
				int ReadLen=recv(m_hSocket,m_pTmpRecvBuf,655360,0);
				DWORD PackageHead=*(DWORD*)m_pTmpRecvBuf;
				int PackageLen=*(int*)(m_pTmpRecvBuf+4);
				DWORD PackageCRC=*(DWORD*)(m_pTmpRecvBuf+8);
				int PackageCmd=*(int*)(m_pTmpRecvBuf+12);
				int DataLen=*(int*)(m_pTmpRecvBuf+16);
				DWORD PackageTail=*(DWORD*)(m_pTmpRecvBuf+DataLen+20);

				if(PackageHead==PACKAGEHEAD&&PackageTail==PACKAGETAIL&&
					PackageCmd==CMD_CTPMainContractsInfo)
				{
					if(file.Open(apppath.c_str(),wxFile::write))
					{
						file.Write(m_pTmpRecvBuf+20,DataLen);
						file.Close();
					}
					GetString(m_pTmpRecvBuf+20,DataLen);
				}

			}
			else
			{
				int ErrCode=WSAGetLastError();
				if(wxFileExists(apppath))
				{
					
					if(file.Open(apppath.c_str()))
					{

						int DataLen=file.Length();
						file.Read(m_pTmpRecvBuf,DataLen);
						GetString(m_pTmpRecvBuf,DataLen);
						file.Close();
					}
				}

			}


	}

}
CInstrumentSocket::~CInstrumentSocket(void)
{
	if(m_pTmpRecvBuf){delete m_pTmpRecvBuf;m_pTmpRecvBuf=NULL;}
	closesocket(m_hSocket);
	WSACleanup();
}
SOCKET CInstrumentSocket::CreateTcpSocket(void)
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
		//iSize=1;											//设置为非阻塞模式
		//ioctlsocket(RltSocket,FIONBIO,(u_long*)&iSize);						
		bRlt1=FALSE;										//带外数据不在数据流中提供
		setsockopt(RltSocket,SOL_SOCKET,SO_OOBINLINE,(char *)&bRlt1,sizeof(BOOL));			
		iLinger.l_onoff=0;
		iLinger.l_linger=0;
		setsockopt(RltSocket,SOL_SOCKET,SO_LINGER,(char*)&iLinger,sizeof(iLinger));
	}
	return RltSocket;
}
bool CInstrumentSocket::string_getline(char* pData,char* out,int len)
{
	static int offset=0;
    if(offset>=(int)strlen(pData))
		return false;
	std::string src(pData+offset);
	int nPos = src.find("\r\n");
	if(nPos>len) nPos=len;
	memcpy(out,pData+offset,nPos);
	offset+=nPos+2;
	return true;
}
void CInstrumentSocket::GetString(char* pData,int datalen)
{
	pData[datalen]=0;
	char szBuf[1024]={0};

	while (string_getline(pData, szBuf, 1024 ) )
	{
		std::string strTemp( szBuf );
		memset( szBuf, 0, 1024 );
		int nPos = strTemp.find( ',');
		if ( -1 == nPos )
		{
			continue;
		}
		strTemp =strTemp.substr(0,nPos);
		if(DEFAULT_SVR()) DEFAULT_SVR()->AddMainInstrumentList( strTemp);
	}
}
