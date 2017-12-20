/************************************************************************************************
这是SvrTcp的事务处理类，也是接口类的子类。
接口类的接口方法在此类中实现。
************************************************************************************************/

#pragma once

#include <time.h>
#include <map>
using namespace std;
#include "easymutex.h"


#include <time.h>
#include <map>
using namespace std;
#include "SocketBase.h"
#define  WM_TCP_MESSAGE WM_USER+9

//每个tcp连接的接收缓冲区大小
#define RcvBufSize (10*1024*1024)


//每个连接的相关数据
struct Stru_SvrTcpConnection
{
	Stru_SvrTcpConnection()
		:hSocket(INVALID_SOCKET),
		pRcvData(NULL),
		RcvDataLen(0),
		RxTime(0)
	{
	}
	Stru_SvrTcpConnection(SOCKET hsocket,Stru_IPAddress & nAdd)
		:hSocket(INVALID_SOCKET),
		pRcvData(NULL),
		RcvDataLen(0),
		RxTime(0),
		IPAddress(nAdd)
	{
		hSocket=hsocket;
		pRcvData=(char*)malloc(RcvBufSize);
		RcvDataLen=0;
		RxTime=time(NULL);
	}
	Stru_SvrTcpConnection(const Stru_SvrTcpConnection& r)
		:hSocket(INVALID_SOCKET),
		pRcvData(NULL),
		RcvDataLen(0),
		RxTime(0)
	{
		(*this)=r;
	}
	Stru_SvrTcpConnection& operator= (const Stru_SvrTcpConnection& r)
	{
		if(pRcvData)
		{
			free(pRcvData);
			pRcvData=NULL;
		}

		hSocket=r.hSocket;
		pRcvData=(char*)malloc(RcvBufSize);
		RcvDataLen=r.RcvDataLen;
		RxTime=r.RxTime;
		IPAddress = r.IPAddress;

		if(pRcvData&&r.pRcvData&&RcvDataLen>0)
			memcpy(pRcvData,r.pRcvData,RcvDataLen);

		//需要把r.hSocket设置为INVALID_SOCKET，避免r析构时关闭该hSocket
		const_cast<Stru_SvrTcpConnection&>(r).hSocket=INVALID_SOCKET;
		return *this;
	}
	~Stru_SvrTcpConnection()
	{
		if(pRcvData)
		{
			free(pRcvData);
			pRcvData=NULL;
		}
		if(hSocket!=INVALID_SOCKET)
		{
			CSocketBase::CloseSocket(hSocket);
			hSocket=INVALID_SOCKET;
		}
	}

	SOCKET	hSocket;
	//Socket的接收缓冲区 
	char*	pRcvData;
	//Socket接收缓冲区内有效数据的长度
	int		RcvDataLen;
	//Socket最后接收时间。1分钟内没有接收到数据，则断开连接。客户端需要在20s周期内向服务器端发送心跳
	time_t	RxTime;
	//
	Stru_IPAddress IPAddress;
};


class CTestTcpBusiness 
{
	//下面getObj()、构造/析构方法和m_pObj用于定义单实例类
public:
	//用于获取接口对象，通过该对象使用接口方法
	static CTestTcpBusiness& getObj(void);
private:
    CTestTcpBusiness();
    virtual ~ CTestTcpBusiness();
    static CTestTcpBusiness* m_pObj;


	//下面的方法在SvrTcp中使用
public:
	void ClearConnections(void);
	void InsertConnection(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void EraseConnection(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void RecvData(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void DisconnectTimeoutConnection(void);
	//关闭一个客户端socket
	void Disconnect(int hsocket);

	//下面是接口方法

	//业务模块订阅消息，需指定消息ID和线程ID。网络收到数据包后，Post到对应线程中
	virtual void SubscribePkg(unsigned int CmdID,unsigned int idThread);
	//模块退订消息，需指定消息ID和线程ID
	virtual void UnsubscribePkg(unsigned int CmdID,unsigned int idThread);
	//模块退订全部消息，需指定消息ID和线程ID
	virtual void UnsubscribeAllPkg(unsigned int idThread);

	//获取并递增RequestID
	virtual unsigned int get_inc_RequestID(void);

	//发送数据
	virtual bool SendData(int hsocket,const char* pData,int DataLen);
	//发送数据, 对sendData的一个封装
	virtual bool SendPkgData(int hsocket,unsigned int nCMDID, void* pData, int nLen, unsigned int seq,
		unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0);


public:
	void AllocateUser(int hSocket,string & nUserName);
	bool GetIPAddress(int hSocket,string & nAdd);

	bool BroadcastPkgData(unsigned int nCMDID, void* pData, int nLen, unsigned int seq,
		unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0);
private:
	//维护唯一的RequestID
	unsigned int		m_baseRequestID;

	//互斥锁
	static Ceasymutex			m_mutex;


	//当前所有Socket的集合
	map<SOCKET,Stru_SvrTcpConnection> m_Connections;
	multimap<string,SOCKET> m_UserSocket; //找到对应用户的socket

	//订阅数据包的线程map, cmdid -> hwnd。
	//收到数据包时，用PostThreadMessage()将数据包推送给订阅的线程。
	//wParam=WndCmd_YourPkgArrival2
	//lParam=EventParamID，EventParam的pEventData是包数据，Stru_UniPkgHead + 数据 + 包尾(4)，userint=hSocket
	multimap<unsigned int,unsigned int> m_cmdid2idthread;
};

