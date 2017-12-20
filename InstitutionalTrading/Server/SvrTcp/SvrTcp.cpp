// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrTcpBusiness.h"




//模块初始化，没什么要做的
SVRTCP_API void InitFunc(void)
{
}

//模块结束，释放资源
SVRTCP_API void ReleaseFunc(void)
{
	CSvrTcpBusiness::getObj().ClearConnections();
}


//*******************************************************************************************************
//	下面是TcpSvr回调函数
//*******************************************************************************************************

//accept后，调用此回调函数进行处理
SVRTCP_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CSvrTcpBusiness::getObj().InsertConnection(SvrAddr,RemoteAddr,hSocket,true);
}


//断开tcp连接后，调用此回调函数进行处理
SVRTCP_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CSvrTcpBusiness::getObj().EraseConnection(hSocket);
}


//从tcp得到数据后，调用此回调函数进行处理
SVRTCP_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CSvrTcpBusiness::getObj().RecvData(pData,DataLen,SvrAddr,RemoteAddr,hSocket);
}


//tcp的定时处理
SVRTCP_API void TcpSvrOntimeFunc(void)
{
	CSvrTcpBusiness::getObj().DisconnectTimeoutConnection();
}

//*******************************************************************************************************
//	下面是TcpClient回调函数
//*******************************************************************************************************

//从tcp得到数据后，调用此回调函数进行处理
SVRTCP_API void	TcpClientRecvFunc(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	CSvrTcpBusiness::getObj().RecvData(pData,DataLen,LocalAddr,SvrAddr,hSocket);
}
//断开tcp连接后，调用此回调函数进行处理
SVRTCP_API void	TcpClientDisconnFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	CSvrTcpBusiness::getObj().EraseConnection(hSocket);
}
//连接成功后，调用此回调函数进行处理
SVRTCP_API void	TcpClientConnSuccFunc(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData)
{
	CSvrTcpBusiness::getObj().InsertConnection(LocalAddr,SvrAddr,hSocket,false);
}
