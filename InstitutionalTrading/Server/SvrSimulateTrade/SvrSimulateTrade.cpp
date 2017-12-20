// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrSimulateTrade.h"
//#include "CommonPkg.h"
//#include "CommonDef.h"
//#include "EventParam.h"
//#include "tmpstackbuf.h"
//#include "..\\SvrTcp\\Interface_SvrTcp.h"
#include "COrderCenter.h"

//-----------------------------------------------------------------------------------
//	import Tools4dll库
//-----------------------------------------------------------------------------------
#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
#endif

////-----------------------------------------------------------------------------------
////	下面import本模块需要引用的其它模块
////-----------------------------------------------------------------------------------
//#pragma comment(lib, "SvrTcp.lib")


#define WRITELOGID_SvrSimulateTrade


//全局互斥锁
Ceasymutex			g_mutex;
COrderCenter		g_orderCenter;

//线程参数
//HANDLE				g_hThread=NULL;
//DWORD				g_idThread=0;
//DWORD ThreadWorker(void *arg);

//
////处理一个接收到的数据包
//void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData);


//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
void InitFunc(void)
{
	////创建工作线程
	//g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);
	//
	////下面订阅本线程感兴趣的数据包
	//CInterface_SvrTcp::getObj().SubscribePkg(CMDID_Heartbeat,g_idThread);
}

//模块结束，释放资源，关闭工作线程
void ReleaseFunc(void)
{
	//if(g_hThread)
	//{
	//	//退订数据包
	//	CInterface_SvrTcp::getObj().UnsubscribeAllPkg(g_idThread);

	//	//发送WM_QUIT通知线程结束
	//	PostThreadMessage(g_idThread,WM_QUIT,0,0);

	//	//等待线程退出
	//	DWORD ExitCode;
	//	WaitForSingleObject((HANDLE)g_hThread,8000);
	//	if(GetExitCodeThread((HANDLE)g_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
	//		TerminateThread((HANDLE)g_hThread,0);
	//	CloseHandle(g_hThread);
	//	g_hThread=NULL;
	//	g_idThread=0;
	//}
}

////工作线程
//DWORD ThreadWorker(void *arg)
//{
//	MSG Msg;
//	while(GetMessage(&Msg,NULL,0,0))
//	{
//		if(Msg.message==WM_COMMAND&&Msg.wParam==WndCmd_YourPkgArrival2)
//		{
//			//数据包的传输ID
//			unsigned int EventParamID=(unsigned int)Msg.lParam;
//			//数据包长度
//			int PkgLen;
//			//Socket句柄
//			int hSocket;
//			if(CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,NULL,&PkgLen,0,NULL))
//			{
//				//申请临时空间并获取数据包
//				AllocTmpStackBuf(TmpPkg,PkgLen,10*1024);
//				if(TmpPkg.m_pbuf&&
//					CInterface_SvrTcp::getObj().getEventParamObj().GetEventParam(EventParamID,NULL,NULL,TmpPkg.m_pbuf,&PkgLen,PkgLen,&hSocket)&&
//					Stru_UniPkgHead::IsValidPkg(TmpPkg.m_pbuf,PkgLen))
//				{
//					Stru_UniPkgHead& PkgHead=*((Stru_UniPkgHead*)TmpPkg.m_pbuf);
//					void* pPkgData=(char*)TmpPkg.m_pbuf+sizeof(Stru_UniPkgHead);
//
//					//调用数据包处理函数处理数据包
//					ProcessOneUniPkg_InThread(hSocket,PkgHead,pPkgData);
//				}
//
//				//释放EventParam
//				CInterface_SvrTcp::getObj().getEventParamObj().DeleteEventParam(EventParamID);
//			}
//
//		}
//	}
//	return 0;
//}
//
////处理一个SvrTcp推送过来的数据包
//void ProcessOneUniPkg_InThread(int hSocket,const Stru_UniPkgHead& PkgHead,const void*pPkgData)
//{
//	//-----------------------------------------------------------------------------------
//	//	下面根据数据包的命令字，处理数据包
//	//-----------------------------------------------------------------------------------
//	switch(PkgHead.cmdid)
//	{
//		case CMDID_Heartbeat:
//		{
//			break;
//		}
//	}
//}