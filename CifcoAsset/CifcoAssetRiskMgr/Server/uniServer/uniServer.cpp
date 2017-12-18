#pragma warning(disable : 4996)
#pragma warning(disable : 4786)


#include <windows.h>


#include "TcpServerWorker.h"
#include "UdpSimpleWorker.h"
#include "WinService.h"
#include "WriteLog.h"
#include "IniFile.h"

#include <vector>
using namespace std;
#define TIME_VALID   "30140610"
#ifdef _DEBUG
#pragma comment(lib, "Tools4dllD.lib")											
#else 
#pragma comment(lib, "Tools4dll.lib")											
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
        if(TcpSvrAddr.IP!=0||TcpSvrAddr.Port!=0)
		{
			if(TcpSvrMaxListen==0) TcpSvrMaxListen=16;
			pTcpSvr=new CTcpServerWorker(TcpSvrAddr,TcpSvrMaxListen,TcpSvrRecvFunc,TcpSvrAcceptFunc,TcpSvrDisconnFunc,TcpSvrOntimeFunc,IOBufSize,IOBufSize);
		}

	    //创建udp server
        if(UdpSvrAddr.IP!=0||UdpSvrAddr.Port!=0)
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
DWORD GetLocalMatchineIP();


int main(int argc,char* argv[])
{
	char strTime2[64];
	memset(&strTime2, 0, sizeof(strTime2));
	SYSTEMTIME st;
	GetLocalTime(&st);	

	std::string strTime = TIME_VALID;
	int nYear = atoi(strTime.substr(0, 4).c_str());
	int nMonth = atoi(strTime.substr(4,2).c_str());
	int nDay   = atoi(strTime.substr(6,2).c_str());

	int idays	=st.wYear*10000 + st.wMonth*100 + st.wDay;
	int nValid  = nYear*10000 + nMonth*100 + nDay;

	if(idays >nValid)
	{
		::MessageBox(NULL, "试用版本已经过期！","提示", 0);
		return 0;
	}

	CWinService::Start_WinService(		argc,
										argv,
										"uniServer",
										"uniServer",
										"uniServer",
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

	//获取程序路径
	CTools_Win32::GetMainPath(g_MainPath,sizeof(g_MainPath)-1);

	g_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE,"uniServer.log");

	//创建配置文件对象
    CIniFile *pIniFile=NULL;
	sprintf(Buf,"%sfunction.ini",g_MainPath);
	pIniFile=new CIniFile(Buf);
	if(!pIniFile) return;

    //顺序读取配置的服务器功能
	DWORD dwLocalIP = GetLocalMatchineIP();
	char szLocalIP[20];
	memset(szLocalIP, 0, sizeof(szLocalIP));
	sprintf(szLocalIP, "%d.%d.%d.%d", dwLocalIP&0xff, (dwLocalIP>>8)&0xff, 
		(dwLocalIP>>16)&0xff, (dwLocalIP>>24)&0xff);

    int FuncOrd=0;
    while(true)
    {
        char NodeName[256]="Function";
        if(!pIniFile->HasIniRootNode(NodeName,FuncOrd))
            break;

        Stru_Function FuncParam;
        pIniFile->GetIniKeyStr(NodeName,"DllName","",FuncParam.DllName,sizeof(FuncParam.DllName),0,FuncOrd);
        FuncParam.TcpSvrAddr.IP=pIniFile->GetIniKeyIP(NodeName,"ip_TcpSvr",szLocalIP,0,FuncOrd);
        FuncParam.TcpSvrAddr.Port=(unsigned short)pIniFile->GetIniKeyVal(NodeName,"port_TcpSvr",0,0,FuncOrd);
        FuncParam.UdpSvrAddr.IP=pIniFile->GetIniKeyIP(NodeName,"ip_UdpSvr",szLocalIP,0,FuncOrd);
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
    for(int i=0,Cnt=g_vecFunc.size();i<Cnt;i++)
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

DWORD GetLocalMatchineIP()
{
	WSADATA wsaData;
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) != 0 )
	{
		return -1;
	}

	char szLocalHostName[64];
	memset(szLocalHostName, 0, sizeof(szLocalHostName));
	gethostname(szLocalHostName,sizeof(szLocalHostName)-1);
	if(!szLocalHostName[0])
	{
		return -1;
	}

	HOSTENT* pHost = NULL;
	pHost = gethostbyname(szLocalHostName);
	if(!pHost) 
	{
		return -1;
	}

	if( pHost->h_length != 4 || pHost->h_addrtype != 2 ) 
	{
		return -1;
	}

	DWORD** ppAddress = NULL;
	ppAddress = (DWORD**)(pHost->h_addr_list);
	if(!ppAddress)
	{
		return -1;
	}

	DWORD dwLocalIP;
	for(int i = 0; i < 5; i++)
	{
		if( ppAddress[i] == NULL )
			break;

		if( (*(ppAddress[i])) != 0x0100007f)
		{
			dwLocalIP = *(ppAddress[i]);
			break;
		}
	}

	if( ppAddress[0] != NULL && dwLocalIP == 0)
	{
		dwLocalIP = *(ppAddress[0]);
	}

	WSACleanup();
	return dwLocalIP;
}



