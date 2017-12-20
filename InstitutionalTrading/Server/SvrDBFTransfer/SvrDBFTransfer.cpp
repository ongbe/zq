// SvrDBFTransfer.cpp : Defines the initialization routines for the DLL.
//
#pragma warning(disable : C4819)

#include "stdafx.h"
#include "SvrDBFTransfer.h"
#include "CommonPublicDef.h"
#include "FileMonitor.h"
#include "..\\SvrDBOpr\\Interface_SvrDBOpr.h"
#include "ConfigManager.h"
#include "Interface_SvrDBFTransfer.h"
#include <io.h>

#include <map>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma comment(lib, "SvrDBOpr.lib")
#ifdef _DEBUG
#pragma comment(lib, "ToolsD.lib")											
#else 
#pragma comment(lib, "Tools.lib")											
#endif

CFileMonitor        g_FileMonitor;
//线程参数
//上交所
BOOL                g_bContinue1 = TRUE;
HANDLE				g_hThread1=NULL;
DWORD				g_idThread1=0;
//深交所
BOOL                g_bContinue2 = TRUE;
HANDLE				g_hThread2=NULL;
DWORD				g_idThread2=0;

//主线程函数
DWORD ThreadWorker1(void *arg);
DWORD ThreadWorker2(void *arg);

 
//////////////////////////////////////////////////////////////////////////
//模块初始化，创建工作线程
void SVRDBFTRANSFER_API InitFunc(void)
{
    printf("Enter InitFunc\n");
	ConfigManager::Init();	
	g_FileMonitor.Init();
	
    //创建工作线程
    g_hThread1=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker1,0,0,&g_idThread1);
    g_hThread2=CreateThread(NULL,10*1024*1024,(LPTHREAD_START_ROUTINE)ThreadWorker2,0,0,&g_idThread2);
}

//模块结束，释放资源，关闭工作线程
void SVRDBFTRANSFER_API ReleaseFunc(void)
{    
	ConfigManager::Release();
	g_FileMonitor.Release();

    g_bContinue1 = FALSE;
    g_bContinue2 = FALSE;

    if(g_hThread1)
    {
        //等待线程退出
        DWORD ExitCode;
        WaitForSingleObject((HANDLE)g_hThread1,8000);
        if(GetExitCodeThread((HANDLE)g_hThread1,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
            TerminateThread((HANDLE)g_hThread1,0);
        CloseHandle(g_hThread1);
        g_hThread1=NULL;
        g_idThread1=0;
    } 

    if(g_hThread2)
    {
        //等待线程退出
        DWORD ExitCode;
        WaitForSingleObject((HANDLE)g_hThread2,8000);
        if(GetExitCodeThread((HANDLE)g_hThread2,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
            TerminateThread((HANDLE)g_hThread2,0);
        CloseHandle(g_hThread2);
        g_hThread2=NULL;
        g_idThread2=0;
    } 
}

//工作线程
DWORD ThreadWorker1(void *arg)
{
    vector<PlatformStru_DepthMarketData> vItems;
    string strPath = "D:\\DBF\\SHOW2003.DBF";//ConfigManager::Get()->GeDBFShow2003Path();
	LPCTSTR lpszDir = strPath.c_str();

	if(_access(lpszDir,0)==-1)
	{
		printf("File %s is not exit,please check!\n",lpszDir);
		return -1;
	}

	printf("Start Monitor %s \n",lpszDir);
    do 
    {
        string strTime,strTableName = "MY_CRECORD2";
        vItems.clear();
        if(g_FileMonitor.Analyse1(lpszDir, vItems, strTime))
        {
            //入库
/*            printf("SHOW2003 %s update to MY_CRECORD2 DB\n",strTime.c_str());
            int nErrorCode = 0;            
            try
            {    
                CInterface_SvrDBOpr::getObj().SaveStockQuot(eExchange_SHOW2003,strTableName,strTime,vItems,nErrorCode);
				printf("update to MY_CRECORD2 DB succussfully\n");
            }
            catch (CMemoryException* e)
            {
				printf("CMemoryException nErrorCode = %d\n",nErrorCode);
            }
            catch (CFileException* e)
            {
				printf("CFileException nErrorCode = %d\n",nErrorCode);
            }
            catch (CException* e)
            {
				printf("CException nErrorCode = %d\n",nErrorCode);
            }   */         
        }

        Sleep(1000);

    } while (1);

    return 0;
}
DWORD ThreadWorker2(void *arg)
{
    vector<PlatformStru_DepthMarketData> vItems;
	string strPath = "D:\\DBF\\SJSHQ.DBF";//ConfigManager::Get()->GetDBFSJSHQPath();
	LPCTSTR lpszDir = strPath.c_str();

	if(_access(lpszDir,0)==-1)
	{
		printf("File %s is not exit,please check!\n",lpszDir);
		return -1;
	}

	printf("Start Monitor %s \n",lpszDir);
    do 
    {
        string strTime,strTableName = "MY_CRECORD2";
        vItems.clear();
        if(g_FileMonitor.Analyse2(lpszDir, vItems, strTime))
        {
            //入库
   //         printf("SJSHQ %s update to MY_CRECORD2 DB\n",strTime.c_str());
   //         int nErrorCode = 0;
   //         CInterface_SvrDBOpr::getObj().SaveStockQuot(eExchange_SJSHQ,strTableName,strTime,vItems,nErrorCode);
			//printf("update to MY_CRECORD2 DB succussfully\n");
        }

        Sleep(1000);

    } while (1);

    return 0;
}
//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CSvrDBFTransferApp

BEGIN_MESSAGE_MAP(CSvrDBFTransferApp, CWinApp)
END_MESSAGE_MAP()


// CSvrDBFTransferApp construction

CSvrDBFTransferApp::CSvrDBFTransferApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSvrDBFTransferApp object

CSvrDBFTransferApp theApp;


// CSvrDBFTransferApp initialization

BOOL CSvrDBFTransferApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
