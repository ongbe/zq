// WinService.h: interface for the CWinService class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINSERVICE_H__6F877EE2_D1A7_4DF2_A3EC_DC31134732C8__INCLUDED_)
#define AFX_WINSERVICE_H__6F877EE2_D1A7_4DF2_A3EC_DC31134732C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4996)


#include <windows.h>
#include <winsvc.h>
#include <stdio.h>
#include <conio.h>

#ifdef WIN32
#pragma managed(push,off)
#endif 


typedef void    (*WinService_WORKFUNC)(DWORD);									//工作函数，服务线程循环调用此回调函数，在此回调函数中做真正的工作，参数是循环次数
typedef void	(*WinService_DELAYFUNC)(void);									//延时函数，服务线程循环工作时，每次调用完工作函数后调用此函数进行延时，如Sleep等
typedef void	(*WinService_INITFUNC)(int argc,char *argv[]);					//初始化函数，服务启动前调用此函数进行应用程序初始化
typedef void	(*WinService_RELEASEFUNC)(void);								//释放函数，服务停止前调用此函数，进行应用程序释放



#define			WinService_INSTALLONLYSERVICECMD	"installonly"				//安装服务的命令行参数
#define			WinService_INSTALLSERVICECMD		"install"					//安装服务的命令行参数
#define			WinService_UNINSTALLSERVICECMD		"uninstall"					//卸载服务的命令行参数
#define			WinService_STARTSERVICECMD			"start"						//开启服务的命令行参数
#define			WinService_STOPSERVICECMD			"stop"						//停止服务的命令行参数
#define			WinService_RUNCMD					"run"						//运行，在cmd窗口里
#define         WinService_RUNASYNCMD               "runasyn"                   //运行，异步


class CWinService  
{
public:
	CWinService();
	virtual ~CWinService();


																				//********下面函数是主要函数，由外部(main()中)调用********
	static int Start_WinService(		int argc,
										char *argv[],
										char *pServiceName,
										char *pDispName=NULL,
										char *pDescription=NULL,
										int InteractiveID=0,
										WinService_WORKFUNC pWorkFunc=NULL,
										WinService_INITFUNC pInitFunc=NULL,
										WinService_RELEASEFUNC pReleaseFunc=NULL,
										WinService_DELAYFUNC pDelayFunc=NULL);
	static void Stop_WinService(void);											//停止，不管在服务模式或RUN模式都有效




																				//********下面静态函数可以由外部单独调用********
	static BOOL IsServiceInstalled(char *ServiceName);							//判断一个服务是否已经安装，服务用服务名指定	
	static DWORD GetServiceStatus(char *ServiceName);							//获取服务的当前状态值，返回 ： 服务的当前状态值，在头文件中有定义,-1为失败
	static BOOL InstallService(char *ServiceName,char *DisplayName,char *FileName,char *Description=NULL,bool bInteractive=false);//安装一个服务
	static BOOL UninstallService(char *ServiceName);							//卸载一个服务	
	static BOOL MyStartService(char *ServiceName);								//开启一个服务
	static BOOL MyStopService(char *ServiceName);								//停止一个服务
	static BOOL IsRunInServiceMode(void);										//是否运行在服务模式下





private:

																				
	static BOOL ServiceEntry(void);												//这是服务入口函数，在服务主程序的main()中调用
	static void WINAPI WinService_ServiceMain(DWORD argc,LPTSTR *argv);			//服务主函数，由SCM调用	
	static void WINAPI WinService_MyServiceCtrlHandler(DWORD Opcode);			//服务状态信息分发函数，由SCM调用
	static DWORD mainService_WorkerThread_RunMode(void *arg);

public:
    enum eRunMode
    {
        NotRunning,
        RunInService,
        RunInCmdWindow,
        RunInAsyn
    };

private:

	static char							m_ServiceName[256];						//由应用程序传入
	static char							m_ServiceDisplayName[256];				//由应用程序传入
	static char							m_ServiceDescription[256];				//由应用程序传入
	static int							m_InteractiveID;						//服务是否能进行界面交互
	static WinService_WORKFUNC			m_pWorkFunc;							//工作函数，服务线程循环调用此回调函数，在此回调函数中做真正的工作，参数是循环次数
	static WinService_DELAYFUNC			m_pDelayFunc;							//延时函数，服务线程循环工作时，每次调用完工作函数后调用此函数进行延时，如Sleep等
	static WinService_INITFUNC			m_pInitFunc;							//初始化函数，服务启动前调用此函数进行应用程序初始化
	static WinService_RELEASEFUNC		m_pReleaseFunc;							//释放函数，服务停止前调用此函数，进行应用程序释放

																				//********下面是服务本身使用的参数********
	static SERVICE_STATUS_HANDLE		hWinService_MyServiceStatusHandle;		//服务状态句柄


	static int							m_WinService_RunningID;					//服务是否正在运行
	static int							m_WinService_PauseID;					//服务是否暂停
	static int							m_WinService_ReleaseOK;					//服务是否成功停止

	static char							m_ModuleName[MAX_PATH];					//Current module name with directory

    static eRunMode                     m_RunMode;
	//static int							m_RunInServiceModeID;					//是否运行在服务模式下
	static int							m_RunningID_RunMode;					//在Run模式下，系统是否正在运行
	static int							m_NeedStopID;							//是否需要停止程序

    static HANDLE                       m_hThread_RunAsyn;                      //RunAsyn模式下的运行线程


																				//申明下面两个函数，但不实现它们，目的是禁止拷贝构造和赋值
	CWinService(const CWinService&);											//为了处理拷贝构造
	CWinService& operator= (const CWinService&);								//赋值符重载

};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_WINSERVICE_H__6F877EE2_D1A7_4DF2_A3EC_DC31134732C8__INCLUDED_)
