/************************************************************************************************
这是SvrTcp的事务处理类，也是接口类的子类。
接口类的接口方法在此类中实现。
单实例类
************************************************************************************************/

#pragma once

#include <time.h>
#include <map>
using namespace std;
#include "easymutex.h"
//#include "SvrTcp.h"
#include "SocketBase.h"
#include "Interface_SvrTcp.h"

//每个tcp连接的接收缓冲区大小
#define RcvBufSize (2*1024*1024)


class CSvrTcpBusiness : public CInterface_SvrTcp
{
    //获取/创建Singleton类. 构造函数为private, 析构函数为public(为了让auto_ptr能析构对象)
    public: static CSvrTcpBusiness& getObj(void);
    private:static auto_ptr<CSvrTcpBusiness> m_pObj;
    private: CSvrTcpBusiness();
    public: virtual ~CSvrTcpBusiness();



	//下面是接口方法，可以在业务模块中使用
public:
	//业务模块订阅消息，需指定消息ID和线程ID。网络收到数据包后，Post到对应线程中
	virtual void SubscribePkg(unsigned int CmdID,unsigned int idThread);
	//模块退订消息，需指定消息ID和线程ID
	virtual void UnsubscribePkg(unsigned int CmdID,unsigned int idThread);
	//模块退订全部消息，需指定消息ID和线程ID
	virtual void UnsubscribeAllPkg(unsigned int idThread);

	//判断Socket句柄是否是TcpSvr的句柄。true: 是一个TcpSvr accept得到的数据socket; false: 是一个TcpClient的socket
	virtual bool IsTcpSvr(int socket);
	//获取Socket句柄对应的远端地址
	virtual bool GetRemoteAddr(int socket,unsigned int& outip,unsigned short& outport);
	//获取所有TcpClient连接的socket句柄和远端地址
	virtual void GetSocket_RemoteAddr_of_TcpClient(map<int,pair<unsigned int,unsigned short>>& outData);
	//获取所有TcpSvr连接的socket句柄和远端地址
	virtual void GetSocket_RemoteAddr_of_TcpServer(map<int,pair<unsigned int,unsigned short>>& outData);

	//获取并递增RequestID
	virtual unsigned int get_inc_RequestID(void);

	//发送数据
	virtual bool SendData(int hsocket,const char* pData,int DataLen);
	//发送数据, 对sendData的一个封装
	virtual bool SendPkgData(int hsocket,unsigned int nCMDID, const void* pData, int nLen, unsigned int seq,
		unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0);

	//关闭一个客户端socket
	void Disconnect(int hsocket);

	//获取EventParam的引用对象，用于取事件参数数据
	virtual CEventParam& getEventParamObj(void);

	//设置/获取Socket的登录信息
	virtual bool SetLoginInfo(int hsocket,const Stru_LoginInfo& logininfo);
	virtual bool GetLoginInfo(int hsocket,Stru_LoginInfo& logininfo);

	//获取所有连接到其它服务器的socket。如果本服务器是从服务，获取主服务的socket；如果本服务器是主服务，获取已经登录成功的所有从服务器socket
	virtual set<int> GetAllServerSockets();


	//下面的方法在SvrTcp中使用
public:
	void ClearConnections(void);
	void InsertConnection(Stru_IPAddress& LocalAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket,bool bTcpSvr);
	void EraseConnection(SOCKET hSocket);
	void RecvData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket);
	void DisconnectTimeoutConnection(void);

public:
	//每个连接的相关数据
	struct Stru_TcpConnection
	{
		Stru_TcpConnection()
		:hSocket(INVALID_SOCKET),
		 bTcpSvr(true),
		 pRcvData(NULL),
		 RcvDataLen(0),
		 RxTime(0),
		 LoginInfo()
		{
		}
		Stru_TcpConnection(SOCKET hsocket,Stru_IPAddress localAddr,Stru_IPAddress remoteAddr,bool btcpSvr)
		:hSocket(hsocket),
		 LocalAddr(localAddr),
		 RemoteAddr(remoteAddr),
		 bTcpSvr(btcpSvr),
		 pRcvData(NULL),
		 RcvDataLen(0),
		 RxTime(0),
		 LoginInfo(hsocket)
		{
			pRcvData=(char*)malloc(RcvBufSize);
			RcvDataLen=0;
			RxTime=time(NULL);
		}
		Stru_TcpConnection(const Stru_TcpConnection& r)
		:hSocket(INVALID_SOCKET),
		 bTcpSvr(true),
		 pRcvData(NULL),
		 RcvDataLen(0),
		 RxTime(0)
		{
			(*this)=r;
		}
		Stru_TcpConnection& operator= (const Stru_TcpConnection& r)
		{
			if(pRcvData)
			{
				free(pRcvData);
				pRcvData=NULL;
			}

			hSocket=r.hSocket;
			LocalAddr=r.LocalAddr;
			RemoteAddr=r.RemoteAddr;
			bTcpSvr=r.bTcpSvr;
			pRcvData=(char*)malloc(RcvBufSize);
			RcvDataLen=r.RcvDataLen;
			RxTime=r.RxTime;
			LoginInfo=r.LoginInfo;

			if(pRcvData&&r.pRcvData&&RcvDataLen>0)
				memcpy(pRcvData,r.pRcvData,RcvDataLen);

			//需要把r.hSocket设置为INVALID_SOCKET，避免r析构时关闭该hSocket
			const_cast<Stru_TcpConnection&>(r).hSocket=INVALID_SOCKET;
			return *this;
		}
		~Stru_TcpConnection()
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
		Stru_IPAddress LocalAddr;
		Stru_IPAddress RemoteAddr;
		//true:TcpSvr, false: TcpClient
		bool	bTcpSvr;
		//Socket的接收缓冲区 
		char*	pRcvData;
		//Socket接收缓冲区内有效数据的长度
		int		RcvDataLen;
		//Socket最后接收时间。1分钟内没有接收到数据，则断开连接。客户端需要在20s周期内向服务器端发送心跳
		time_t	RxTime;

		//连接的登录信息
		Stru_LoginInfo LoginInfo;
	};



private:
	//维护唯一的RequestID
	unsigned int		m_baseRequestID;

	//互斥锁
	Ceasymutex			m_mutex;

	//当前所有Socket的集合
	map<SOCKET,Stru_TcpConnection> m_Connections;
	//所有连接到服务器的socket。如果本服务器是从服务，包含主服务socket；如果本服务器是主服务，包含所有已登录的从服务的socket
	set<int>			m_ServerScokets;		

	//订阅数据包的线程map, cmdid -> idThread。
	//收到数据包时，用PostThreadMessage()将数据包推送给订阅的线程。
	//wParam=WndCmd_YourPkgArrival2
	//lParam=EventParamID，EventParam的pEventData是包数据，Stru_UniPkgHead + 数据 + 包尾(4)，userint=hSocket
	multimap<unsigned int,unsigned int> m_cmdid2idthread;
};



