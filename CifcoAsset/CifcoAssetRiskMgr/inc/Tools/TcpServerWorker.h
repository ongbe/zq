// TcpServerWorker.h: interface for the CTcpServerWorker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSERVERWORKER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_)
#define AFX_TCPSERVERWORKER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <map>
#include <vector>
using namespace std;

#include "socketbase.h"


//从tcp得到数据后，调用此回调函数进行处理
typedef	void	(*TCPSVRRCVDATAHANDLER)(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
//accept后，调用此回调函数进行处理
typedef	void	(*TCPSVRACCEPTHANDLER)(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);      
//断开tcp连接后，调用此回调函数进行处理
typedef	void	(*TCPSVRDISCONNHANDLER)(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);		
//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
typedef void    (*TCPSVRONTIME)(void);


class CTcpServerWorker  : public CSocketBase
{
public:
	CTcpServerWorker(Stru_IPAddress BindAddr,int iMaxListen=16,
					 TCPSVRRCVDATAHANDLER pRecvFunc=NULL,
					 TCPSVRACCEPTHANDLER pAcceptFunc=NULL,
					 TCPSVRDISCONNHANDLER pDisconnFunc=NULL,
					 TCPSVRONTIME pOntimeFunc=NULL,
					 int SOSNDBUFSize=655360,int SORCVBUFSize=655360);
	virtual ~CTcpServerWorker();


	/**************************************************************************************************//**
	*   @brief					GetServerAddr  获取服务器地址
	*   @param	N/A
	*	@return					Tcp绑定的地址
	*******************************************************************************************************/
	Stru_IPAddress GetServerAddr(void);

	/**************************************************************************************************//**
	*   @brief					SendData	发送数据
	*   @param	[in] pData		要发送的数据
	*			[in] DataLen	要发送的数据长度
	*			[in] RemoteAddr	发送的目的地址，如IP/Port都为0，则向连接BindAddr的所有Socket发送
	*	@return					已发送的长度
	*******************************************************************************************************/
	int SendData(char* pData,int DataLen,Stru_IPAddress RemoteAddr=Stru_IPAddress(0,0));

	/**************************************************************************************************//**
	*   @brief					SendData	发送数据
	*   @param	[in] pData		要发送的数据
	*			[in] DataLen	要发送的数据长度
	*			[in] hDataSocket用于发送的数据socket句柄
	*	@return					已发送的长度
	*******************************************************************************************************/
	int SendData(char* pData,int DataLen,SOCKET hDataSocket);

	void OffLineOneDataSocket(SOCKET hDataSocket);
	void OffLineOneDataSocket(Stru_IPAddress RemoteAddr);



private:
	HANDLE	m_hSocketThread;
	volatile bool	m_bSocketThreadNeedExit;

	char	*m_pTmpRecvBuf;									//用于读取tcp数据的缓冲区
	int		m_TmpRecvBufSize;


	SOCKET			m_hListenSocket;
	Stru_IPAddress	m_BindAddr;
	unsigned int	m_BindIP_Real;							//真正绑定的IP，如果BindIP为0，则此值等于本地第一个IP
	int				m_MaxListen;
	time_t			m_CreateListenSocketTime;
	time_t			m_ShutdownListenSocketTime;


	TCPSVRRCVDATAHANDLER	m_RecvFunc;						//数据socket接收到数据后的回调函数
	TCPSVRACCEPTHANDLER		m_AcceptFunc;					//Accept后的回调函数
	TCPSVRDISCONNHANDLER	m_DisconnFunc;					//数据socket断开前的回调函数
	TCPSVRONTIME			m_OntimeFunc;

    Stru_SocketRecvDataCallBackParam* m_pRcvDataCBParam;
    Stru_SocketCallBackParam* m_pAcceptCBParam;
    Stru_SocketCallBackParam* m_pDisconnCBParam;

	std::map<SOCKET,Stru_IPAddress> m_DataSocketMap;


	static DWORD SocketThreadWorker(void *arg);

	void OffLine_Internal(void);
	void OffLineOneDataSocket_Internal(SOCKET hDataSocket);
	void OffLineOneDataSocket_Internal(Stru_IPAddress RemoteAddr);

	/**************************************************************************************************//**
	*   @brief		CreateServerSocket_Internal  检查tcp server 是否要创建socket
	*******************************************************************************************************/
	void CreateServerSocket_Internal(time_t CurTime);

	/**************************************************************************************************//**
	*   @brief		GetSocketSet_Internal  获取要select的socketset
	*******************************************************************************************************/
	void GetSocketSet_Internal(fd_set& ReadSocketSet,fd_set& WriteSocketSet,fd_set& ExceptSocketSet);

	//**************************************************
	//   @brief		处理socket的意外事件
	//**************************************************
	void CheckSocketExcept_Internal(fd_set& ExceptSocketSet);

	//**************************************************
	//   @brief		释放一个数据连接的socket
	//**************************************************
	bool Check_TryReleaseOneDataSocket_Internal(SOCKET hDataSocket);

	//**************************************************
	//   @brief		释放一个监听socket
	//**************************************************
	bool Check_TryReleaseOneListenSocket_Internal(SOCKET hListenSocket);

	//**************************************************
	//   @brief		处理Accept
	//**************************************************
	void TryAccept_Internal(fd_set& ReadSocketSet);

	//**************************************************
	//   @brief		处理Read
	//**************************************************
	void TryRead_Internal(fd_set& ReadSocketSet);

};

#endif // !defined(AFX_TCPSERVERWORKER_H__3C8964B8_6067_4345_9088_DDD8A9AC212E__INCLUDED_)
