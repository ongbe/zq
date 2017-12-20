// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MULTIACCOUNTSERVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MULTIACCOUNTSERVER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef MULTIACCOUNTSERVER_EXPORTS
#define MULTIACCOUNTSERVER_API __declspec(dllexport)
#else
#define MULTIACCOUNTSERVER_API __declspec(dllimport)
#endif

#include "SocketBase.h"
#include "CommonPkg.h"
#include "MemoryCache.h"

//每个tcp数据连接的参数
struct Stru_DataSocket
{
	Stru_DataSocket()
	{
		hScoket=INVALID_SOCKET;
		memset(UserName,0,sizeof(UserName));
		bLogin=false;
		ReqType=2;
		time(&HeartbeatTime);
		pStreamCache=NULL;
		RemoteAddr.IP=0;
		RemoteAddr.Port=0;
	}

	SOCKET	hScoket;
	char	UserName[32];			//登录用户名
	bool	bLogin;					//是否已经登录
	int		ReqType;				//请求数据类型，1:I类(所有),2:II类(主力)
	time_t	HeartbeatTime;			//最近一次心跳时间
	CMemoryCache *pStreamCache;		//流Cache
	Stru_IPAddress RemoteAddr;
};

struct RequestData
{
	SOCKET hSocket;			//发送请求的连接句柄
	Stru_UniPkgHead head;	//命令字
	void* pData;			//请求数据

	RequestData()
	{
		hSocket = INVALID_SOCKET;
		ZeroMemory(&head, sizeof(Stru_UniPkgHead));
		pData = NULL;
	}

	RequestData(SOCKET socket, Stru_UniPkgHead pkgHead, BYTE* p)
	{
		hSocket = socket;
		head = pkgHead;
		pData = p;
	}
};

MULTIACCOUNTSERVER_API void InitFunc(void);
MULTIACCOUNTSERVER_API void ReleaseFunc(void);
MULTIACCOUNTSERVER_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
MULTIACCOUNTSERVER_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
MULTIACCOUNTSERVER_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
MULTIACCOUNTSERVER_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
MULTIACCOUNTSERVER_API void TcpSvrOntimeFunc(void);

// This class is exported from the MultiAccountServer.dll
class MULTIACCOUNTSERVER_API CMultiAccountServer {
public:
	CMultiAccountServer(void);
	// TODO: add your methods here.
};

