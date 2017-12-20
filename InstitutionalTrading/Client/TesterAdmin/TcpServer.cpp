#include "stdafx.h"
#include "TcpServer.h"
#include "TestTcpBusiness.h"



//模块初始化，没什么要做的
void InitFunc(void)
{
	return;
}

//模块结束，释放资源
void ReleaseFunc(void)
{
	CTestTcpBusiness::getObj().ClearConnections();
}


//accept后，调用此回调函数进行处理
void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CTestTcpBusiness::getObj().InsertConnection(SvrAddr,RemoteAddr,hSocket);
}


//断开tcp连接后，调用此回调函数进行处理
void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CTestTcpBusiness::getObj().EraseConnection(SvrAddr,RemoteAddr,hSocket);
}


//从tcp得到数据后，调用此回调函数进行处理
void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	CTestTcpBusiness::getObj().RecvData(pData,DataLen,SvrAddr,RemoteAddr,hSocket);
}


//tcp的定时处理
void TcpSvrOntimeFunc(void)
{
	CTestTcpBusiness::getObj().DisconnectTimeoutConnection();
}
