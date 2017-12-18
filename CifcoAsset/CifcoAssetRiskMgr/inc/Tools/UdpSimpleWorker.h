// UdpSimpleWorker.h: interface for the CUdpSimpleWorker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UDPSIMPLEWORKER_H__026DD890_5F2D_4183_B856_C2EFB7217378__INCLUDED_)
#define AFX_UDPSIMPLEWORKER_H__026DD890_5F2D_4183_B856_C2EFB7217378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <map>
#include <vector>
using namespace std;
#include "socketbase.h"

//从udp simple得到数据后，调用此回调函数进行处理
typedef	void	(*UDPSIMPLERCVDATAHANDLER)(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
//定期调用的回调函数，一般每秒会调用一次，可以用于检查心跳超时等
typedef void    (*UDPSIMPLEONTIME)(void);

#define MaxUdpRcvPkgCntPerLoop 128

class CUdpSimpleWorker  : public CSocketBase
{
public:
	CUdpSimpleWorker(Stru_IPAddress BindAddr,
					 UDPSIMPLERCVDATAHANDLER pRecvFunc=NULL,
					 UDPSIMPLEONTIME pOntimeFunc=NULL,
					 int SOSNDBUFSize=655360,int SORCVBUFSize=655360);
	virtual ~CUdpSimpleWorker();


	/**************************************************************************************************//**
	*   @brief					GetServerAddr  获取服务器地址的列表
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
	int SendData(char* pData,int DataLen,Stru_IPAddress RemoteAddr);


private:
	HANDLE	m_hSocketThread;
	bool	m_bSocketThreadNeedExit;
	char	*m_pTmpRecvBuf;									//用于读取tcp数据的缓冲区
	int		m_TmpRecvBufSize;


	SOCKET			m_hSocket;
	Stru_IPAddress	m_BindAddr;
	unsigned int	m_BindIP_Real;							//真正绑定的IP，如果BindIP为0，则此值等于本地第一个IP
	time_t			m_CreateSocketTime;
	time_t			m_ShutdownSocketTime;

	UDPSIMPLERCVDATAHANDLER m_RecvFunc;
	UDPSIMPLEONTIME m_OntimeFunc;

    Stru_SocketRecvDataCallBackParam* m_pRcvDataCBParam;

	static DWORD SocketThreadWorker(void *arg);

	void OffLine_Internal(void);

	/**************************************************************************************************//**
	*   @brief		CreateSocket_Internal  检查udp simple是否要创建socket
	*******************************************************************************************************/
	void CreateSocket_Internal(time_t CurTime);

	/**************************************************************************************************//**
	*   @brief		GetSocketSet_Internal  获取要select的socketset
	*******************************************************************************************************/
	void GetSocketSet_Internal(fd_set& ReadSocketSet,fd_set& WriteSocketSet,fd_set& ExceptSocketSet);

	//**************************************************
	//   @brief		处理socket的意外事件
	//**************************************************
	void CheckSocketExcept_Internal(fd_set& ExceptSocketSet);

	//**************************************************
	//   @brief		处理Read
	//**************************************************
	void TryRead_Internal(fd_set& ReadSocketSet);

};

#endif // !defined(AFX_UDPSIMPLEWORKER_H__026DD890_5F2D_4183_B856_C2EFB7217378__INCLUDED_)
