// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include "testtcpsvr.h"
#include "SocketBase.h"
#include "WinService.h"
#include "CommonPkg.h"

#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif



#define WRITELOGID 0

#include <map>
#include <string>
using namespace std;

#define RcvBufSize (10*1024*1024)
//每一个socket申请一个接收缓冲区
map<SOCKET,char*> g_mapRcvBuf;
map<SOCKET,int> g_mapRcvBufLen;



//处理Pkg
void ProcessPkg(SOCKET hSocket,const char* pPkg);
TESTTCPSVR_API void InitFunc(void)
{
}

TESTTCPSVR_API void ReleaseFunc(void)
{
    map<SOCKET,char*>::iterator it;
    for(it=g_mapRcvBuf.begin();it!=g_mapRcvBuf.end();it++)
    {
        free(it->second);
    }
}


//accept后，调用此回调函数进行处理
TESTTCPSVR_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    char* pBuf=(char*)malloc(RcvBufSize);
    if(pBuf)
    {
        g_mapRcvBuf[hSocket]=pBuf;
        g_mapRcvBufLen[hSocket]=0;
    }
}


//断开tcp连接后，调用此回调函数进行处理
TESTTCPSVR_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    map<SOCKET,char*>::iterator it1=g_mapRcvBuf.find(hSocket);
    if(it1!=g_mapRcvBuf.end()) 
    {
        free(it1->second);
        g_mapRcvBuf.erase(it1);
    }
    map<SOCKET,int>::iterator it2=g_mapRcvBufLen.find(hSocket);
    if(it2!=g_mapRcvBufLen.end()) g_mapRcvBufLen.erase(it2);
}


//从tcp得到数据后，调用此回调函数进行处理
TESTTCPSVR_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    map<SOCKET,char*>::iterator itBuf=g_mapRcvBuf.find(hSocket);
    map<SOCKET,int>::iterator itLen=g_mapRcvBufLen.find(hSocket);

    if(itBuf==g_mapRcvBuf.end()||itLen==g_mapRcvBufLen.end())
        return;

    if(itLen->second+DataLen<=RcvBufSize)
	{
        memcpy(((char*)itBuf->second)+itLen->second,pData,DataLen);
		itLen->second+=DataLen;
	}

    if(itLen->second>0)
    {
	    //先跳过可能出现的无效数据
	    int off=0;
	    while(off<itLen->second&&Stru_UniPkgHead::IsInvalidPkgHead(itBuf->second+off,itLen->second-off)) off++;

	    //循环解析接收到的数据
	    int LengthUsedByPkg;
	    while(Stru_UniPkgHead::IsValidPkg(itBuf->second+off,itLen->second-off,&LengthUsedByPkg))
	    {
            ProcessPkg(hSocket,itBuf->second+off);

		    off+=LengthUsedByPkg;
	    }
	    if(off>0)
	    {
		    memmove(itBuf->second,itBuf->second+off,itLen->second-off);
		    itLen->second-=off;
	    }
    }

}


//tcp的定时处理
TESTTCPSVR_API void TcpSvrOntimeFunc(void)
{
}

//服务器端推送风险事件
#define Cmd_RM_RiskEvent_Req					0x190001	//(临时代码，演示客户端向服务器端请求的过程)
#define Cmd_RM_RiskEvent_Push					0x190002	
//风险事件数据结构
#pragma pack(push)
#pragma pack(1)
struct RiskEvent
{
	int nRiskEventID;
	long lEventTime;			//从1970年起的秒数
	int nTradeAccountID;
	int nRiskIndModuleID;
	int nRiskIndicatorID;
	int nRiskLevelID;
	float fThresholdValue;
	int nMsgSendStatus;
	int nIsValid;					//0 风险事件无效 1 风险事件有效
};
#pragma pack(pop)

//处理Pkg
void ProcessPkg(SOCKET hSocket,const char* pPkg)
{
    const Stru_UniPkgHead& Pkg=*(const Stru_UniPkgHead*)pPkg;
    const char* pData=pPkg+sizeof(Pkg);

    if(Pkg.cmdid==Cmd_RM_RiskEvent_Req)
    {
	    char buf[sizeof(Stru_UniPkgHead)+sizeof(RiskEvent)+4];
	    Stru_UniPkgHead& outPkgHead=*(Stru_UniPkgHead*)buf;
	    outPkgHead.Set(sizeof(RiskEvent),Cmd_RM_RiskEvent_Push,0,0);
        RiskEvent& outData=*(RiskEvent*)(buf+sizeof(outPkgHead));
        outData.nRiskEventID=(int)GetTickCount();
        outData.lEventTime=(long)time(NULL);
        outData.nTradeAccountID=123;
        outData.nRiskIndModuleID=3;
        outData.nRiskIndicatorID=4;
        outData.fThresholdValue=23.0;
        outData.nMsgSendStatus=0;
        outData.nIsValid=0;
	    *(unsigned int*)(buf+sizeof(outPkgHead)+sizeof(outData))=TailID_UniPkg;

         CSocketBase::SendDataTcp(hSocket,buf,sizeof(buf));
    }
    else if(Pkg.cmdid==CMDID_HEARTBEAT)
    {
	    char buf[sizeof(Stru_UniPkgHead)+4];
	    Stru_UniPkgHead& outPkgHead=*(Stru_UniPkgHead*)buf;
	    outPkgHead.Set(0,CMDID_HEARTBEATRSP,0,0);
	    *(unsigned int*)(buf+sizeof(outPkgHead))=TailID_UniPkg;

        CSocketBase::SendDataTcp(hSocket,buf,sizeof(buf));
        printf("%d: 收到心跳包\n",(int)time(NULL)%3600);

    }
}