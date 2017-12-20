#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRTCP_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRTCP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTCP_EXPORTS
#define SVRTCP_API __declspec(dllexport)
#else
#define SVRTCP_API __declspec(dllimport)
#endif
#endif

#include "CommonStruct.h"
#include "EventParam.h"
#include <map>
#include <set>
using std::map;
using std::set;
using std::pair;

class SVRTCP_API CInterface_SvrTcp : public CInterface_SvrCommon
{
public:
	//用于获取接口对象，通过该对象使用接口方法
	static CInterface_SvrTcp& getObj(void);


	//---------------------------------------------------------------------------------
	//	数据接口
	//---------------------------------------------------------------------------------

	//业务模块订阅消息，需指定消息ID和线程ID。收到数据包后，Post到对应线程中
	virtual void SubscribePkg(unsigned int CmdID,unsigned int idThread)=0;
	//模块退订消息，需指定消息ID和线程ID
	virtual void UnsubscribePkg(unsigned int CmdID,unsigned int idThread)=0;
	//模块退订全部消息，需指定消息ID和线程ID
	virtual void UnsubscribeAllPkg(unsigned int idThread)=0;

	//判断Socket句柄是否是TcpSvr的句柄。true: 是一个TcpSvr accept得到的数据socket; false: 是一个TcpClient的socket
	virtual bool IsTcpSvr(int socket)=0;
	//获取Socket句柄对应的远端地址
	virtual bool GetRemoteAddr(int socket,unsigned int& outip,unsigned short& outport)=0;
	//获取所有TcpClient连接的socket句柄和远端地址
	virtual void GetSocket_RemoteAddr_of_TcpClient(map<int,pair<unsigned int,unsigned short>>& outData)=0;
	//获取所有TcpSvr连接的socket句柄和远端地址
	virtual void GetSocket_RemoteAddr_of_TcpServer(map<int,pair<unsigned int,unsigned short>>& outData)=0;

	//获取并递增RequestID
	virtual unsigned int get_inc_RequestID(void)=0;

	//发送数据
	virtual bool SendData(int hsocket,const char* pData,int DataLen)=0;
	//发送数据, 对sendData的一个封装
	virtual bool SendPkgData(int hsocket,unsigned int nCMDID, const void* pData, int nLen, unsigned int seq,
		unsigned int Subseq=0,unsigned int Userdata1=0,unsigned int Userdata2=0,unsigned int Userdata3=0,unsigned int Userdata4=0)=0;

	//关闭一个客户端socket
	virtual void Disconnect(int hsocket)=0;

	//获取EventParam的引用对象，用于取事件参数数据
	virtual CEventParam& getEventParamObj(void)=0;


	//*******************************************************************************
	//socket的登录相关信息
	//*******************************************************************************
	struct Stru_LoginInfo
	{
		Stru_LoginInfo()
		{
			memset(this,0,sizeof(*this));
		};
		Stru_LoginInfo(int hsocket)
		{
			memset(this,0,sizeof(*this));
			hSocket=hsocket;
		}
		Stru_LoginInfo(int hsocket,int frontID,int sessionID)
		{
			memset(this,0,sizeof(*this));
			hSocket=hsocket;
			FrontID=frontID;
			sessionID=SessionID;
		};

		int			hSocket;
		int			FrontID;
		int			SessionID;
		UserType	Usertype;
		int			UserID;
		char		UserName[32];
		bool		bLogin;
	};

	//设置/获取Socket的登录信息
	virtual bool SetLoginInfo(int hsocket,const Stru_LoginInfo& logininfo)=0;
	virtual bool GetLoginInfo(int hsocket,Stru_LoginInfo& logininfo)=0;

	//获取所有连接到其它服务器的socket。如果本服务器是从服务，获取主服务的socket；如果本服务器是主服务，获取已经登录成功的所有从服务器socket
	virtual set<int> GetAllServerSockets()=0;

};







