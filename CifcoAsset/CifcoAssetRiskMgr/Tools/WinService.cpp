// WinService.cpp: implementation of the CWinService class.
//
//////////////////////////////////////////////////////////////////////

#include "WinService.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"



char						CWinService::m_ServiceName[256];					//由应用程序传入
char						CWinService::m_ServiceDisplayName[256];				//由应用程序传入
char						CWinService::m_ServiceDescription[256];				//由应用程序传入
int							CWinService::m_InteractiveID=0;						//服务是否能进行界面交互
WinService_WORKFUNC			CWinService::m_pWorkFunc=NULL;						//工作函数，服务线程循环调用此回调函数，在此回调函数中做真正的工作，参数是循环次数
WinService_DELAYFUNC		CWinService::m_pDelayFunc=NULL;						//延时函数，服务线程循环工作时，每次调用完工作函数后调用此函数进行延时，如Sleep等
WinService_INITFUNC			CWinService::m_pInitFunc=NULL;						//初始化函数，服务启动前调用此函数进行应用程序初始化
WinService_RELEASEFUNC		CWinService::m_pReleaseFunc=NULL;					//释放函数，服务停止前调用此函数，进行应用程序释放

																				//********下面是服务本身使用的参数********
SERVICE_STATUS_HANDLE		CWinService::hWinService_MyServiceStatusHandle=NULL;//服务状态句柄


int							CWinService::m_WinService_RunningID=0;				//服务是否正在运行
int							CWinService::m_WinService_PauseID=0;				//服务是否暂停
int							CWinService::m_WinService_ReleaseOK=0;				//服务是否成功停止

char						CWinService::m_ModuleName[MAX_PATH];				//Current module name with directory

int							CWinService::m_RunInServiceModeID=0;				//是否运行在服务模式下
int							CWinService::m_RunningID_RunMode=0;					//在Run模式下，系统是否正在运行
int							CWinService::m_NeedStopID=0;						//是否需要停止程序

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinService::CWinService()
{

}

CWinService::~CWinService()
{

}


//	名称 ： InServiceInstalled
//	描述 ： 判断一个服务是否已经安装，服务用服务名指定
//	参数 ： ServiceName	:服务名称
//	返回 ： TRUE:存在；FALSE:不存在
BOOL CWinService::IsServiceInstalled(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								READ_CONTROL);

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	return schService?TRUE:FALSE;
}


//	名称 ： GetServiceStatus
//	描述 ： 获取服务的当前状态值
//	参数 ： ServiceName : 服务名称
//	返回 ： 服务的当前状态值，在头文件中有定义,-1为失败
DWORD CWinService::GetServiceStatus(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;
	BOOL			bRlt=FALSE;
	SERVICE_STATUS	iStatus;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								SERVICE_ALL_ACCESS);

	if(schService)
	{
		memset((char*)&iStatus,0,sizeof(iStatus));
		bRlt=QueryServiceStatus(schService,&iStatus);
	}

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	return bRlt?iStatus.dwCurrentState:0xffffffff;
}


//	名称 ： InstallService
//	描述 ： 安装一个服务
//	参数 ： ServiceName	:服务名称
//			DisplayName	:显示名称
//			FileName	:服务所在文件的名称
//	返回 ： TRUE:成功；FALSE:失败
BOOL CWinService::InstallService(char *ServiceName,char *DisplayName,char *FileName,char *Description,bool bInteractive)
{
	SC_HANDLE							schSCManager=NULL;
	SC_HANDLE							schService=NULL;
	SERVICE_DESCRIPTION					sdBuf;


	if(!ServiceName||!DisplayName||!FileName) return FALSE;
	if(IsServiceInstalled(ServiceName)) return TRUE;

	schSCManager=OpenSCManager(		NULL,										// local machine 
									NULL,										// ServicesActive database 
									SC_MANAGER_ALL_ACCESS);						// full access rights 

	if(schSCManager)
	{
		if(bInteractive)
			schService=CreateService(	schSCManager,								// SCManager database 
										ServiceName,								// name of service 
										DisplayName,								// service name to display 
										SERVICE_ALL_ACCESS,							// desired access 
										SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,					// service type 
										SERVICE_AUTO_START,							// start type 
										SERVICE_ERROR_NORMAL,						// error control type 
										FileName,									// service's binary 
										NULL,										// no load ordering group 
										NULL,										// no tag identifier 
										NULL,										// no dependencies 
										NULL,										// LocalSystem account 
										NULL);										// no password 
		else 
			schService=CreateService(	schSCManager,								// SCManager database 
										ServiceName,								// name of service 
										DisplayName,								// service name to display 
										SERVICE_ALL_ACCESS,							// desired access 
										SERVICE_WIN32_OWN_PROCESS,					// service type 
										SERVICE_AUTO_START,							// start type 
										SERVICE_ERROR_NORMAL,						// error control type 
										FileName,									// service's binary 
										NULL,										// no load ordering group 
										NULL,										// no tag identifier 
										NULL,										// no dependencies 
										NULL,										// LocalSystem account 
										NULL);										// no password 
	}
	if(schService&&Description&&Description[0])
	{
		sdBuf.lpDescription = Description;
		ChangeServiceConfig2(		schService,									// handle to service
									SERVICE_CONFIG_DESCRIPTION,					// change: description
									&sdBuf);									// value: new description
	}

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);


	return schService?TRUE:FALSE;
}


//	名称 ： UninstallService
//	描述 ： 卸载一个服务
//	参数 ： ServiceName : 服务名
//	返回 ： TRUE:成功；FALSE:失败
BOOL CWinService::UninstallService(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;
	BOOL			bUninstallRlt=FALSE;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								DELETE);					// only need DELETE access 

	if(schService)
		bUninstallRlt=DeleteService(schService);

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	if(!schSCManager) return FALSE;
	else if(!schSCManager) return TRUE;
	else if(bUninstallRlt) return TRUE;
	else return FALSE;
}


//	名称 ： MyStartService
//	描述 ： 开启一个服务
//	参数 ： ServiceName : 服务名
//	返回 ： TRUE:成功；FALSE:失败
BOOL CWinService::MyStartService(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;
	BOOL			bRlt=FALSE;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								SERVICE_ALL_ACCESS);

	if(schService)
		bRlt=StartService(schService,0,NULL);

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	return bRlt?TRUE:FALSE;
}


//	名称 ： MyStopService
//	描述 ： 停止一个服务
//	参数 ： ServiceName : 服务名
//	返回 ： TRUE:成功；FALSE:失败
BOOL CWinService::MyStopService(char *ServiceName)
{
	SC_HANDLE		schSCManager=NULL;
	SC_HANDLE		schService=NULL;
	BOOL			bRlt=FALSE;
	SERVICE_STATUS	iStatus;

	if(!ServiceName) return FALSE;

	schSCManager=OpenSCManager(	NULL,						//local machine 
								NULL,						//ServicesActive database 
								SC_MANAGER_ALL_ACCESS);		//full access rights 
	if(schSCManager)
		schService=OpenService(	schSCManager,				// SCManager database 
								ServiceName,				// name of service 
								SERVICE_ALL_ACCESS);

	if(schService)
	{
		memset((char*)&iStatus,0,sizeof(iStatus));
		bRlt=ControlService(schService,SERVICE_CONTROL_STOP,&iStatus);
	}

	if(schService) CloseServiceHandle(schService);
	if(schSCManager) CloseServiceHandle(schSCManager);

	return bRlt?TRUE:FALSE;
}

//是否运行在服务模式下
BOOL CWinService::IsRunInServiceMode(void)										
{
	return m_RunInServiceModeID?TRUE:FALSE;
}


//	名称 ： ServiceEntry
//	描述 ： 1. 这是服务入口函数
//			2. 此程序应该在服务程序的main()中调用
//			3. 服务程序的main()中调用此函数后即可退出
//			4. 服务的主要工作在线程里进行，线程函数地址由参数传入，在此函数中创建线程
//	参数 ： pWorkFunc : 工作函数，服务线程循环调用此回调函数，在此回调函数中做真正的工作，参数是循环次数
//			pDelayFunc : 延时函数，服务线程循环工作时，每次调用完工作函数后调用此函数进行延时，如Sleep等
//			pInitFunc :	初始化函数，服务启动前调用此函数进行应用程序初始化
//			pReleaseFunc : 释放函数，服务停止前调用此函数，进行应用程序释放
//	返回 ： TRUE:成功；FALSE:失败
BOOL CWinService::ServiceEntry(void)
{
	BOOL			bRlt;
	SERVICE_TABLE_ENTRY DispatchTable[2]={{m_ServiceName,WinService_ServiceMain},{NULL,NULL}}; 

	bRlt=StartServiceCtrlDispatcher(DispatchTable);

	return bRlt;
}

//	名称 ： 服务主函数WinService_ServiceMain
//	描述 ： 1. 这是服务主函数
//			2. 服务启动时，SCM会调用此函数，从而启动服务
//			3. ServiceMain is called when the SCM wants to start the service. When it returns, the 
//			   service has stopped. It therefore waits on an event just before the end of the function, 
//			   and that event gets set when it is time to stop.
//			   It also returns on any error because the service cannot start if there is an eror.
//	参数 ： argc、argv均为SCM传进来的参数
void CWinService::WinService_ServiceMain(DWORD argc,LPTSTR *argv)
{
	SERVICE_STATUS      iServiceStatus;
	DWORD				iCount=0;
	HANDLE				hSleepEvent=NULL;


																				//注册服务状态信息更改处理函数
    hWinService_MyServiceStatusHandle=RegisterServiceCtrlHandler(m_ServiceName,WinService_MyServiceCtrlHandler); 
    if(!hWinService_MyServiceStatusHandle) return ;

	m_WinService_RunningID=1;														//服务是否正在运行
	m_WinService_PauseID=1;														//服务处于暂停
	m_WinService_ReleaseOK=0;														//服务没有停止

																				//应用程序需要初始化，将服务状态更改为正在启动状态
	if(m_pInitFunc)
	{
		memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
		iServiceStatus.dwServiceType		= SERVICE_WIN32; 
		iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
		iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
		SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);

		(*m_pInitFunc)(argc,argv);
	}
																				//将服务状态更改为运行状态
	memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
	iServiceStatus.dwServiceType		= SERVICE_WIN32; 
	iServiceStatus.dwCurrentState		= SERVICE_RUNNING; 
	iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
	SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
	m_WinService_PauseID=0;														//服务没有暂停
	m_NeedStopID=0;


	if(!m_pDelayFunc)
		hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);

	while(m_WinService_RunningID)													//执行工作线程，退出即表示服务停止
	{
		if(!m_WinService_PauseID&&m_pWorkFunc)
		{
			(*m_pWorkFunc)(iCount);									//真正工作
			iCount++;
		}

		if(m_NeedStopID)
		{																		//根据外部设置，服务需要停止
			MyStopService(m_ServiceName);
			break;
		}

																				//下面延时
		if(m_pDelayFunc)												//调用应用程序定义的延时函数
			(*m_pDelayFunc)();
		else if(hSleepEvent)
			WaitForSingleObject(hSleepEvent,1000);
		else Sleep(1000);
	}

	if(hSleepEvent)
	{
		CloseHandle(hSleepEvent);
		hSleepEvent=NULL;
	}

																				//需要释放应用程序资源
	if(m_pReleaseFunc)
		(*m_pReleaseFunc)();


	m_WinService_ReleaseOK=1;

}


//	名称 ： WinService_MyServiceCtrlHandler
//	描述 ： 1. 这是服务状态信息分发函数
//			2. 当用户通过控制面板改变本服务的状态时，SCM调用此函数处理
//			3. 此函数首先通知工作线程，然后通知SCM更改服务状态显示
//	参数 ： Opcode: 服务状态信息码
void CWinService::WinService_MyServiceCtrlHandler(DWORD Opcode)
{ 
    switch(Opcode) 
    { 
        case SERVICE_CONTROL_PAUSE: 
		{
			m_WinService_PauseID=1;

			if(hWinService_MyServiceStatusHandle)
			{
				SERVICE_STATUS      iServiceStatus;

				memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
				iServiceStatus.dwServiceType		= SERVICE_WIN32; 
				iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
				iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
				iServiceStatus.dwCurrentState		= SERVICE_PAUSED; 
				SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
			}
            break; 
		}
        case SERVICE_CONTROL_CONTINUE: 
		{
			m_WinService_PauseID=0;

			if(hWinService_MyServiceStatusHandle)
			{
				SERVICE_STATUS      iServiceStatus;

				memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
				iServiceStatus.dwServiceType		= SERVICE_WIN32; 
				iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
				iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
				iServiceStatus.dwCurrentState		= SERVICE_RUNNING; 
				SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
			}
            break; 
		}
		case SERVICE_CONTROL_SHUTDOWN:
        case SERVICE_CONTROL_STOP: 
		{
			int		i;
			HANDLE	hSleepEvent=NULL;

			m_WinService_RunningID=0;

			if(hWinService_MyServiceStatusHandle)
			{
				SERVICE_STATUS      iServiceStatus;

				memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
				iServiceStatus.dwServiceType		= SERVICE_WIN32; 
				iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
				iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
				iServiceStatus.dwCurrentState		= SERVICE_STOP_PENDING; 
				SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
			}

			hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
			for(i=0;i<40;i++)
			{
				if(hSleepEvent)
					WaitForSingleObject(hSleepEvent,500);
				else 
					Sleep(500);

				if(m_WinService_ReleaseOK) break;
			}
			if(hSleepEvent)
			{
				CloseHandle(hSleepEvent); 
				hSleepEvent=NULL;
			}
			

			if(hWinService_MyServiceStatusHandle)
			{
				SERVICE_STATUS      iServiceStatus;

				memset((char*)&iServiceStatus,0,sizeof(iServiceStatus));
				iServiceStatus.dwServiceType		= SERVICE_WIN32; 
				iServiceStatus.dwCurrentState		= SERVICE_START_PENDING; 
				iServiceStatus.dwControlsAccepted	= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
				iServiceStatus.dwCurrentState		= SERVICE_STOPPED; 
				SetServiceStatus(hWinService_MyServiceStatusHandle,&iServiceStatus);
			}
            break; 
		}
        case SERVICE_CONTROL_INTERROGATE: 
            break; 
        default: 
			break;
    } 
} 






int CWinService::Start_WinService(int argc,
								  char *argv[],
								  char *pServiceName,
								  char *pDispName,
								  char *pDescription,
								  int InteractiveID,
								  WinService_WORKFUNC pWorkFunc,
								  WinService_INITFUNC pInitFunc,
								  WinService_RELEASEFUNC pReleaseFunc,
								  WinService_DELAYFUNC pDelayFunc)
{


	memset(m_ServiceName,0,sizeof(m_ServiceName));
	strncpy(m_ServiceName,pServiceName,sizeof(m_ServiceName)-1);
	memset(m_ServiceDisplayName,0,sizeof(m_ServiceDisplayName));
	strncpy(m_ServiceDisplayName,pDispName,sizeof(m_ServiceDisplayName)-1);
	memset(m_ServiceDescription,0,sizeof(m_ServiceDescription));
	strncpy(m_ServiceDescription,pDescription,sizeof(m_ServiceDescription)-1);
	m_InteractiveID=InteractiveID;

	m_pInitFunc=pInitFunc;									//初始化函数，服务启动前调用此函数进行应用程序初始化
	m_pReleaseFunc=pReleaseFunc;							//释放函数，服务停止前调用此函数，进行应用程序释放
	m_pWorkFunc=pWorkFunc;									//工作函数，服务线程循环调用此回调函数，在此回调函数中做真正的工作，参数是循环次数
	m_pDelayFunc=pDelayFunc;								//延时函数，服务线程循环工作时，每次调用完工作函数后调用此函数进行延时，如Sleep等





															//********下面取执行程序带路径的名称********
	memset(m_ModuleName,0,sizeof(m_ModuleName));
	GetModuleFileName(NULL,m_ModuleName,sizeof(m_ModuleName)-1);




																				//安装服务
	if(argc>=2&&stricmp(argv[1],WinService_INSTALLONLYSERVICECMD)==0)
	{
		if(argc==2)
		{
			if(!IsServiceInstalled(m_ServiceName))
				InstallService(m_ServiceName,m_ServiceDisplayName,m_ModuleName,m_ServiceDescription);
		}
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);
			if(!IsServiceInstalled(iServiceName))
				InstallService(iServiceName,iServiceName,iServiceName,iServiceName);
		}
		return 0;
	}

																				//安装和启动服务
	if(argc>=2&&stricmp(argv[1],WinService_INSTALLSERVICECMD)==0)	
	{
		if(argc==2)
		{
			if(!IsServiceInstalled(m_ServiceName))
				InstallService(m_ServiceName,m_ServiceDisplayName,m_ModuleName,m_ServiceDescription);
			if(IsServiceInstalled(m_ServiceName))
				MyStartService(m_ServiceName);
		}
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);
			if(!IsServiceInstalled(iServiceName))
				InstallService(iServiceName,iServiceName,iServiceName,iServiceName);
			if(IsServiceInstalled(iServiceName))
				MyStartService(iServiceName);
		}
		return 0;
	}
	
																				//卸载服务
	if(argc>=2&&stricmp(argv[1],WinService_UNINSTALLSERVICECMD)==0)
	{
		if(argc==2)
		{
			if(IsServiceInstalled(m_ServiceName))
				MyStopService(m_ServiceName);
			if(IsServiceInstalled(m_ServiceName))
				UninstallService(m_ServiceName);
		}
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);

			if(IsServiceInstalled(iServiceName))
				MyStopService(iServiceName);
			if(IsServiceInstalled(iServiceName))
				UninstallService(iServiceName);
		}
		return 0;
	}

																				//开启服务
	if(argc>=2&&stricmp(argv[1],WinService_STARTSERVICECMD)==0)
	{
		if(argc==2)
			MyStartService(m_ServiceName);
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);

			MyStartService(iServiceName);
		}
		return 0;
	}

																				//停止服务
	if(argc>=2&&stricmp(argv[1],WinService_STOPSERVICECMD)==0)
	{
		if(argc==2)
			MyStopService(m_ServiceName);
		else
		{
			char	iServiceName[256];
			memset(iServiceName,0,sizeof(iServiceName));
			strncpy(iServiceName,argv[2],sizeof(iServiceName)-1);

			MyStopService(iServiceName);
		}
		return 0;
	}

																				//Run模式，直接运行
	if(argc==2&&stricmp(argv[1],WinService_RUNCMD)==0)
	{
		char		*iargv[2]={"",""};
		HANDLE		hThread=NULL;
		DWORD		ThreadID;
		DWORD		ExitCode;
		char		InputBuf[256],ich;
		HANDLE		hSleepEvent;

		
		m_RunInServiceModeID=0;

		if(m_pInitFunc)
		{
			m_pInitFunc(argc,argv);
		}

		m_RunningID_RunMode=1;
		m_NeedStopID=0;
		hThread=CreateThread(NULL,65536,(LPTHREAD_START_ROUTINE)mainService_WorkerThread_RunMode,NULL,0,&ThreadID);

		printf("press 'exit' to exit......\n");

		memset(InputBuf,0,sizeof(InputBuf));

		hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
		while(!m_NeedStopID)
		{
			if(_kbhit())
			{
				ich=(char)_getche();
				if(ich==10||ich==13)
				{
					if(stricmp(InputBuf,"exit")==0)
						break;
					memset(InputBuf,0,sizeof(InputBuf));
					printf("\r\n");
				}
				else if(ich==8)
				{
					int Len=strlen(InputBuf);
					if(Len>0)
					{
						InputBuf[Len-1]=0;
						printf("\r%s",InputBuf);
					}
				}
				else
					InputBuf[strlen(InputBuf)]=ich;
			}
			else
				WaitForSingleObject(hSleepEvent,100);
		}
		CloseHandle(hSleepEvent); 

		m_RunningID_RunMode=0;
		WaitForSingleObject((HANDLE)hThread,8000);
		if(GetExitCodeThread((HANDLE)hThread,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)hThread,0);


		if(m_pReleaseFunc)
			m_pReleaseFunc();


		return 0;
	}






																				//下面执行服务指定的任务
	m_RunInServiceModeID=1;
	ServiceEntry();



	return 0;
}


//停止，不管在服务模式或RUN模式都有效
void CWinService::Stop_WinService(void)
{
	m_NeedStopID=1;
}


//	Run模式下的工作线程
DWORD CWinService::mainService_WorkerThread_RunMode(void *)						
{
	DWORD			TickCount=0;
	HANDLE			hSleepEvent=NULL;

	if(!m_pDelayFunc)
		hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);

	while(m_RunningID_RunMode)
	{
		if(m_pWorkFunc)
			m_pWorkFunc(TickCount);

		if(m_pDelayFunc)
			m_pDelayFunc();
		else if(hSleepEvent)
			WaitForSingleObject(hSleepEvent,1000);
		else 
			Sleep(1000);


		TickCount++;
	}

	if(hSleepEvent)
	{
		CloseHandle(hSleepEvent); 
		hSleepEvent=NULL;
	}

	return 0;
}
