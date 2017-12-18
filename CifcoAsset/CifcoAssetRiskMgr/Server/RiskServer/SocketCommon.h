#pragma once

#include "Tools/CommonPkg.h"
#include "Tools/MemoryCache.h"
#include "Tools/SocketBase.h"

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