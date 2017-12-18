// TcpClientWorker.h: interface for the CTcpClientWorker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPCLIENTWORKER_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
#define AFX_TCPCLIENTWORKER_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include "socketbase.h"

#ifdef WIN32
#pragma managed(push,off)
#endif 



//从tcp得到数据后，调用此回调函数进行处理
typedef	void	(*TCPCLINTRCVDATAHANDLER)(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//断开tcp连接后，调用此回调函数进行处理
typedef	void	(*TCPCLINTDISCONNHANDLER)(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//开始调用connect()后，调用此函数通知应用层
typedef void	(*TCPCLINTSTARTCONNECT)(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//连接成功后，调用此回调函数进行处理
typedef	void	(*TCPCLINTCONNSUCCESSHANDLER)(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//连接失败后，调用此回调函数进行处理
typedef	void	(*TCPCLINTCONNFAILUREHANDLER)(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);
//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
typedef void    (*TCPCLINTONTIME)(int UserData);


//Tcp client 连接状态
enum TYPE_TcpClientConnStatus
{
	TcpClientConnStatus_Disconnected         =0,
	TcpClientConnStatus_Connecting           =1,
	TcpClientConnStatus_Connected            =2
};


class CTcpClientWorker  : public CSocketBase
{
public:
    //ReconnectInterval:自动重连的时间间隔，单位为秒。0表示断开后不重连
	CTcpClientWorker(
        Stru_IPAddress LocalAddr,
        Stru_IPAddress SvrAddr,
		TCPCLINTRCVDATAHANDLER pRecvFunc=NULL,
		TCPCLINTDISCONNHANDLER pDisconnFunc=NULL,
		TCPCLINTSTARTCONNECT pStartFunc=NULL,
		TCPCLINTCONNSUCCESSHANDLER pConnSuccFunc=NULL,
		TCPCLINTCONNFAILUREHANDLER pConnFailFunc=NULL,
		TCPCLINTONTIME pOntimeFunc=NULL,
		int SOSNDBUFSize=655360,
        int SORCVBUFSize=655360,
        int ReconnectInterval=60,
        int UserData=0);
	virtual ~CTcpClientWorker();


	/**************************************************************************************************//**
	*   @brief				GetAddr  获取地址
	*   @param	LocalAddr	返回本地地址
	*			SvrAddr		返回服务器地址
	*	@return				Tcp绑定的地址
	*******************************************************************************************************/
	void GetAddr(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr);


	/**************************************************************************************************//**
	*   @brief					SendData	发送数据
	*   @param	[in] pData		要发送的数据
	*			[in] DataLen	要发送的数据长度
	*	@return					已发送的长度
	*	@Note					可能会暂存
	*******************************************************************************************************/
	int SendData(const void* pData,int DataLen);


	//取socket句柄
	int getSocket(void) { return (int)m_hSocket; }
	//取当前连接状态
	TYPE_TcpClientConnStatus getConnStatus(void) { return m_ConnStatu; }
	//取连接结束时间
	time_t getConnEndTime(void) { return m_ConnEndTime; }

    //设置为需要重新连接。可以在执行CSocketBase::CloseSocket(hSocket)后，设置再次连接
    void SetReconnect(void);
public:
	TCPCLINTRCVDATAHANDLER m_RecvFunc;
	TCPCLINTDISCONNHANDLER m_DisconnFunc;
	TCPCLINTSTARTCONNECT m_StartFunc;
	TCPCLINTCONNSUCCESSHANDLER m_ConnSuccFunc;
	TCPCLINTCONNFAILUREHANDLER m_ConnFailFunc;
	TCPCLINTONTIME m_OntimeFunc;


private:
	HANDLE	m_hSocketThread;
	volatile bool	m_bSocketThreadNeedExit;

	char	*m_pTmpRecvBuf;									//用于读取tcp数据的缓冲区
	int		m_TmpRecvBufSize;

	SOCKET			m_hSocket;
	Stru_IPAddress	m_LocalAddr;
	unsigned int	m_LocalIP_Real;							//真正绑定的IP，如果BindIP为0，则此值等于本地第一个IP
	Stru_IPAddress	m_SvrAddr;
	time_t			m_CreateSocketTime;
	time_t			m_ShutdownSocketTime;
	time_t			m_ConnEndTime;
	TYPE_TcpClientConnStatus m_ConnStatu;
    int             m_ReconnectInterval;                    //自动重连的时间间隔，单位为秒
	bool			m_bSuccessBefore;						//曾经连接成功过。如果m_ReconnectInterval为0，则不再重连

	char            *m_pSndBuf;                             //发送数据先放着这个buf中，然后再发送。buf大小为SOSNDBUFSize两倍
    int             m_SndBufSize;
	int             m_SndBufLen;

    int             m_UserData;



	static DWORD SocketThreadWorker(void *arg);

	void OffLine_Internal(Stru_SocketCallBackParam& DisconnCBParam);
	void OffLine_Internal2(void);
	/**************************************************************************************************//**
	*   @brief		CreateSocket_Internal  检查tcp client是否要创建socket
	*******************************************************************************************************/
	void CreateSocket_Internal(
        time_t CurTime,
        Stru_SocketCallBackParam& DisconnCBParam,
        Stru_SocketCallBackParam& StartCBParam,
        Stru_SocketCallBackParam& ConnSuccCBParam,
        Stru_SocketCallBackParam& ConnFailCBParam);

	/**************************************************************************************************//**
	*   @brief		GetSocketSet_Internal  获取要select的socketset
	*******************************************************************************************************/
	void GetSocketSet_Internal(void* pReadSocketSet,void* pWriteSocketSet,void* pExceptSocketSet);

	//**************************************************
	//   @brief		处理socket的意外事件
	//**************************************************
	void CheckSocketExcept_Internal(void* pExceptSocketSet,
		Stru_SocketCallBackParam& DisconnCBParam,
        Stru_SocketCallBackParam& ConnFailCBParam);

	//**************************************************
	//   @brief		处理Read
	//**************************************************
	void TryRead_Internal(void* pReadSocketSet,
                            Stru_SocketRecvDataCallBackParam& RcvDataCBParam,
                            Stru_SocketCallBackParam& DisconnCBParam,
                            Stru_SocketCallBackParam& ConnFailCBParam);

	//**************************************************
	//   @brief		处理Write
	//**************************************************
	void TryWrite_Internal(void* pWriteSocketSet,Stru_SocketCallBackParam& ConnSuccCBParam);


    //实际发送数据，发送m_pSndBuf中的数据
    void senddata_internal(void);

private:
															//申明下面两个函数，但不实现它们，目的是禁止拷贝构造和赋值
	CTcpClientWorker(const CTcpClientWorker&);									
	CTcpClientWorker& operator= (const CTcpClientWorker&);				

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TCPCLIENTWORKER_H__5209856B_BADE_4410_8F99_294036BAF1A3__INCLUDED_)
