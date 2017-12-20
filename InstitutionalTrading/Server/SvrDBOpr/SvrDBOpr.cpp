// testtcpsvr.cpp : Defines the exported functions for the DLL application.
//

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <windows.h>
#include "SvrDBOpr.h"
#include "CommonPkg.h"
#include "CommonDef.h"
#include "EventParam.h"
#include "tmpstackbuf.h"
#include "Interface_SvrDBOpr.h"
#include "ConfigManager.h"


////-----------------------------------------------------------------------------------
////	import Tools4dll库
////-----------------------------------------------------------------------------------
//#ifdef _DEBUG
//#pragma comment(lib, "Tools4dllD.lib")
//#else
//#pragma comment(lib, "Tools4dll.lib")
//#endif
//
//#pragma comment(lib, "SvrNotifyAndAsk.lib")




#define WRITELOGID_SvrDBOpr




//全局互斥锁
Ceasymutex			g_mutex;

//线程参数
HANDLE				g_hThread=NULL;
DWORD				g_idThread=0;
DWORD ThreadWorker(void *arg);





//模块初始化，创建工作线程并向SvrTcp订阅感兴趣的数据包
SVRDBOPR_API void InitFunc(void)
{
	//创建工作线程
	g_hThread=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker,0,0,&g_idThread);

	ConfigManager::Init();
	CInterface_SvrDBOpr::getObj().InitDB(	ConfigManager::Get()->GetDBUser(),
											ConfigManager::Get()->GetDBPwd(),
											ConfigManager::Get()->GetDBSID());

	CInterface_SvrDBOpr::getObj().Conncect();

	//下面订阅本线程感兴趣的数据包
	//CInterface_SvrTcp::getObj().SubscribePkg(CMDID_HEARTBEAT,g_idThread);
}

//模块结束，释放资源，关闭工作线程
SVRDBOPR_API void ReleaseFunc(void)     
{
	if(g_hThread)
	{
		//发送WM_QUIT通知线程结束
		PostThreadMessage(g_idThread,WM_QUIT,0,0);

		//等待线程退出
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)g_hThread,8000);
		if(GetExitCodeThread((HANDLE)g_hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)g_hThread,0);
		CloseHandle(g_hThread);
		g_hThread=NULL;
		g_idThread=0;
	}

	ConfigManager::Release();
}

//工作线程
DWORD ThreadWorker(void *arg)
{
	MSG Msg;
	while(GetMessage(&Msg,NULL,0,0))
	{
	}
	return 0;
}

