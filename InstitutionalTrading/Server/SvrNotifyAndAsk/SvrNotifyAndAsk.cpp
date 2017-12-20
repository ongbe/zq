// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#include "stdafx.h"
#include <windows.h>
#include "SvrNotifyAndAsk.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "CommonMacro.h"
#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "Interface_SvrNotifyAndAsk.h"
#include "CustomInterfaceSvrNotifyAndAsk.h"
#include "Tools_Win32.h"




#define WRITELOGID_SvrNotifyAndAsk



//全局互斥锁
Ceasymutex			g_mutex;
Ceasymutex			g_mutexEvent;
//线程参数
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;

//订阅数据包的线程map, cmdid -> idThread。
//收到数据包时，用PostThreadMessage()将数据包推送给订阅的线程。
//wParam=WndCmd_YourPkgArrival2
//lParam=EventParamID，EventParam的pEventData是包数据，Stru_UniPkgHead + 数据 + 包尾(4)，userint=hSocket
 multimap<eEventType,unsigned int> g_mapDataType2Thread;

 //问询接口map
multimap<eAskType, CInterface_SvrCommon* const> g_mapDataType2Interface;

DWORD ThreadWorker(void *arg);


//处理一个接收到的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);



//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVRNOTIFYANDASK_API void InitFunc(void)
{
	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
}

//模块结束，释放资源，关闭工作线程
SVRNOTIFYANDASK_API void ReleaseFunc(void)
{
	if(g_hThread)
	{
		//退订数据包
		CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

		//发送WM_QUIT通知线程结束
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		//等待线程退出
		CTools_Win32::StopThread((HANDLE)g_hThread,8);
		g_hThread=NULL;
		g_idThread=0;
	}
}

//因为AllocTmpStackBuf中申请栈空间只有在函数中才能自动释放，所以用DealPackage处理
void DealPackage(unsigned int EventParamID,int& PkgLen,int& hSocket)
{
	AllocTmpStackBuf(TmpPkg,PkgLen);
	if(TmpPkg.m_pbuf&&
		CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
		Stru_UniPkgHead::IsValidPkg(TmpPkg.m_pbuf,PkgLen))
	{
		Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)TmpPkg.m_pbuf);
		void* pPkgData=(char*)TmpPkg.m_pbuf+sizeof(Stru_UniPkgHead);
		
		//调用数据包处理函数处理数据包
		ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);	
	}
}
//工作线程
DWORD ThreadWorker(void *arg)
{
	MSG Msg;

	//订阅本线程感兴趣的数据包
	CInterface_SvrTcp::getObj().SubscribePkg(CMDID_NotifyAndAsk_BroadcastNotify,g_idThread);

	while(GetMessage(&Msg,NULL,0,0))
	{
		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2)
		{
			//数据包的传输ID
			unsigned int EventParamID=(unsigned int)Msg.lParam;
			//数据包长度
			int PkgLen;
			//Socket句柄
			int hSocket;
			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,NULL))
			{
				DealPackage(EventParamID,PkgLen,hSocket);

				//释放EventParam
				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
			}

		}		
		
	}
	return 0;
}

//处理一个SvrTcp推送过来的数据包
void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
{
	//-----------------------------------------------------------------------------------
	//	下面根据数据包的命令字，处理数据包
	//-----------------------------------------------------------------------------------
	switch(PkgHead.cmdid)
	{
		case CMDID_NotifyAndAsk_BroadcastNotify:
		{
			//收到其它服务器广播过来的通知消息
			if(PkgHead.len==sizeof(Stru_NotifyEvent))
			{
				CCustomInterfaceSvrNotifyAndAsk::getObj().NotifyEvent2(*((const Stru_NotifyEvent*)pPkgData),hSocket);
			}
			break;
		}
	}
}