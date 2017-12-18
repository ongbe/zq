 /*
// QuotServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

*/
#include "stdafx.h"

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <windows.h>


#include "TcpServerWorker.h"
#include "UdpSimpleWorker.h"
#include "WinService.h"
#include "WriteLog.h"
#include "IniFile.h"
#include "Tools_Ansi.h"

#include <vector>
using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "ToolsD.lib")											
#else 
#pragma comment(lib, "Tools.lib")											
#endif


typedef	void 	(*Init_uniServerCBFunc)(void);	
typedef	void 	(*Release_uniServerCBFunc)(void);	


char g_MainPath[MAX_PATH];
CWriteLog *g_pWriteLog=NULL;

struct Stru_Function
{
    Stru_Function()
    {
        memset(this,0,sizeof(*this));
    };

    void LoadDLL(char* pMainPath)
    {
        if(!pMainPath) return;

        char filename[512];
        sprintf(filename,"%s%s",pMainPath,DllName);
        hLib=LoadLibrary(filename);
        if(!hLib) return;

        InitFunc=(Init_uniServerCBFunc)GetProcAddress(hLib,"InitFunc");
        ReleaseFunc=(Release_uniServerCBFunc)GetProcAddress(hLib,"ReleaseFunc");

        TcpSvrRecvFunc=(TCPSVRRCVDATAHANDLER)GetProcAddress(hLib,"TcpSvrRecvFunc");
        TcpSvrAcceptFunc=(TCPSVRACCEPTHANDLER)GetProcAddress(hLib,"TcpSvrAcceptFunc");
        TcpSvrDisconnFunc=(TCPSVRDISCONNHANDLER)GetProcAddress(hLib,"TcpSvrDisconnFunc");
        TcpSvrOntimeFunc=(TCPSVRONTIME)GetProcAddress(hLib,"TcpSvrOntimeFunc");

        UdpRecvFunc=(UDPSIMPLERCVDATAHANDLER)GetProcAddress(hLib,"UdpRecvFunc");
        UdpOntimeFunc=(UDPSIMPLEONTIME)GetProcAddress(hLib,"UdpOntimeFunc");
    };

    void ReleaseDLL(void)
    {
        if(hLib)
        {
            FreeLibrary(hLib);
            hLib=NULL;
        }

        InitFunc=NULL;
        ReleaseFunc=NULL;

        TcpSvrRecvFunc=NULL;
        TcpSvrAcceptFunc=NULL;
        TcpSvrDisconnFunc=NULL;
        TcpSvrOntimeFunc=NULL;

        UdpRecvFunc=NULL;
        UdpOntimeFunc=NULL;
    };

    void CreateSvr(void)
    {
        if(!hLib)
            return;

        //创建tcp server
        if(TcpSvrAddr.Port!=0)
        {
            if(TcpSvrMaxListen==0) TcpSvrMaxListen=16;
            pTcpSvr=new CTcpServerWorker(TcpSvrAddr,TcpSvrMaxListen,TcpSvrRecvFunc,TcpSvrAcceptFunc,TcpSvrDisconnFunc,TcpSvrOntimeFunc,IOBufSize,IOBufSize);
        }

        //创建udp server
        if(UdpSvrAddr.Port!=0)
            pUdpSvr=new CUdpSimpleWorker(UdpSvrAddr,UdpRecvFunc,UdpOntimeFunc,IOBufSize,IOBufSize);
    };

    void ReleaseSvr(void)
    {
        if(pTcpSvr)
        {
            delete pTcpSvr;
            pTcpSvr=NULL;
        }
        if(pUdpSvr)
        {
            delete pUdpSvr;
            pUdpSvr=NULL;
        }
    };





    HINSTANCE hLib;
    CTcpServerWorker *pTcpSvr;
    CUdpSimpleWorker *pUdpSvr;

    char DllName[256];

    Stru_IPAddress TcpSvrAddr;
    Stru_IPAddress UdpSvrAddr;
    int IOBufSize;
    int TcpSvrMaxListen;

    int HeartbeatTimeout;

    Init_uniServerCBFunc InitFunc;
    Release_uniServerCBFunc ReleaseFunc;

    TCPSVRRCVDATAHANDLER TcpSvrRecvFunc;
    TCPSVRACCEPTHANDLER TcpSvrAcceptFunc;
    TCPSVRDISCONNHANDLER TcpSvrDisconnFunc;
    TCPSVRONTIME TcpSvrOntimeFunc;

    UDPSIMPLERCVDATAHANDLER UdpRecvFunc;
    UDPSIMPLEONTIME UdpOntimeFunc;
};

vector<Stru_Function> g_vecFunc;

void WorkFunc(DWORD TickCount);
void InitFunc(int argc,char *argv[]);
void ReleaseFunc(void);
void DelayFunc(void);


int main(int argc,char* argv[])
{
    //获取程序路径
    CTools_Win32::GetMainPath(g_MainPath,sizeof(g_MainPath)-1);

    //获取服务名称
    char ServiceName[128];
    strcpy(ServiceName,"QuotServer");
    CIniFile *pIniFile=NULL;
    char Buf[512];
    sprintf(Buf,"%squotfunction.ini",g_MainPath);
    pIniFile=new CIniFile(Buf);
    if(pIniFile)
    {
        pIniFile->GetIniKeyStr("System","ServiceName","QuotServer",ServiceName,sizeof(ServiceName));
        delete pIniFile;
    }


    CWinService::Start_WinService(		argc,
        argv,
        ServiceName,
        ServiceName,
        ServiceName,
        0,
        WorkFunc,
        InitFunc,
        ReleaseFunc,
        DelayFunc);

    return 1;
}


void WorkFunc(DWORD TickCount)
{
}

void InitFunc(int argc,char *argv[])
{
    char Buf[512];

    g_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"QuotServer.log");

    //创建配置文件对象
    CIniFile *pIniFile=NULL;
    sprintf(Buf,"%squotfunction.ini",g_MainPath);
    pIniFile=new CIniFile(Buf);
    if(!pIniFile) return;

    //取本机地址
    CTools_Ansi::PerformWSAStartup();
    DWORD dwLocalIP=0;
    char defLocalIP[256];
    if(!CTools_Ansi::GetLocalHostName_IP(NULL,0,&dwLocalIP,1))
        dwLocalIP=CTools_Ansi::ConvertStrIP("127.0.0.1");
    strncpy(defLocalIP,CTools_Ansi::ConvertdwIP(dwLocalIP),sizeof(defLocalIP)-1);
    defLocalIP[sizeof(defLocalIP)-1]=0;
    CTools_Ansi::PerformWSACleanup();

    //顺序读取配置的服务器功能
    int FuncOrd=0;
    while(true)
    {
        char NodeName[256]="Function";
        if(!pIniFile->HasIniRootNode(NodeName,FuncOrd))
            break;

        Stru_Function FuncParam;
        pIniFile->GetIniKeyStr(NodeName,"DllName","",FuncParam.DllName,sizeof(FuncParam.DllName),0,FuncOrd);
        FuncParam.TcpSvrAddr.IP=pIniFile->GetIniKeyIP(NodeName,"ip_TcpSvr",defLocalIP,0,FuncOrd);
        FuncParam.TcpSvrAddr.Port=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"port_TcpSvr",0,0,FuncOrd);
        FuncParam.UdpSvrAddr.IP=pIniFile->GetIniKeyIP(NodeName,"ip_UdpSvr",defLocalIP,0,FuncOrd);
        FuncParam.UdpSvrAddr.Port=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"port_UdpSvr",0,0,FuncOrd);

        FuncParam.HeartbeatTimeout=pIniFile->GetIniKeyVal(NodeName,"heartbeattimeout",16,0,FuncOrd);
        FuncParam.IOBufSize=pIniFile->GetIniKeyVal(NodeName,"IOBufSize",655360,0,FuncOrd);
        FuncParam.TcpSvrMaxListen=pIniFile->GetIniKeyVal(NodeName,"TcpSvrMaxListen",16,0,FuncOrd);

        g_vecFunc.push_back(FuncParam);

        FuncOrd++;
    };

    //释放pIniFile
    if(pIniFile)
    {
        delete pIniFile;
        pIniFile=NULL;
    }


    //顺序加载Func
    for(int i=0,Cnt=g_vecFunc.size();i<Cnt;i++)
    {
        g_vecFunc[i].LoadDLL(g_MainPath);

        if(g_vecFunc[i].hLib&&g_vecFunc[i].InitFunc!=NULL)
            g_vecFunc[i].InitFunc();

        g_vecFunc[i].CreateSvr();
    }
}

void ReleaseFunc(void)
{
    int Cnt=(int)g_vecFunc.size();
    for(int i=Cnt-1;i>=0;i--)
    {
        g_vecFunc[i].ReleaseSvr();

        if(g_vecFunc[i].hLib&&g_vecFunc[i].ReleaseFunc!=NULL)
            g_vecFunc[i].ReleaseFunc();

        g_vecFunc[i].ReleaseDLL();
    }
    if(g_pWriteLog)
    {
        delete g_pWriteLog;
        g_pWriteLog=NULL;
    }
}

void DelayFunc(void)
{
    Sleep(1000);
}



