// PlotCtrlServer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#pragma warning(disable : 4996)
#pragma warning(disable : 4786)

#include <set>
#include <map>
#include <string>
using namespace std;

#include "inc\SocketBase.h"
#include "inc\WinService.h"
#include "inc\CommonPkg.h"
#include "PlotCtrlCommon.h"

#define WRITELOGID 0


#define RcvBufSize (10*1024*1024)

typedef struct {
	int		nBuffLen;
	int		nDataLen;
	char*	pBuff;
} BUFFDATA, *LPBUFFDATA;

typedef struct {
	__int64		updateTime;
	char		cDirectMode;
	TPlotIDType	plotID;
} DIRECTMODE, *LPDIRECTMODE;

typedef map<SOCKET, BUFFDATA> MAPSOCKETBUFF;
typedef map<string, set<SOCKET>> MAPINSTSUBS;
// <platID, <InstrumentID, DIRECT>>
typedef map<string, map<string, DIRECTMODE>> MAPPLOTDIRECT;
typedef map<string, DWORD> MAPFILEPOS;

//每一个socket申请一个接收缓冲区
MAPSOCKETBUFF	g_mapRcvBuf;
MAPINSTSUBS		g_mapInstSubs;
MAPPLOTDIRECT	g_mapPlotDirect;
MAPFILEPOS		g_mapFilePos;

BOOL			g_bExit = FALSE;

//处理Pkg
void ProcessPkg(SOCKET hSocket,const char* pPkg);
BOOL MergePlotDirectMap(MAPPLOTDIRECT& mapMain, MAPPLOTDIRECT& mapAppend);
BOOL GetDirectMode(set<string>& setInst, MAPPLOTDIRECT& mapInstDirect);
void PushInstrumentCtrlDirectToClient(MAPPLOTDIRECT& mapPush);
void PushInstrumentCtrlDirect(SOCKET hSocket, MAPPLOTDIRECT& mapPush);
int SendInstrumentCtrlDirect(SOCKET hSocket, INSTRUMENTCTRLDIRECT& direct);
BOOL LoadInstDirect(MAPPLOTDIRECT& mapRetPush);
BOOL ParsePlotData(char* pBuff, MAPPLOTDIRECT& mapRetPush);
char* GetPlotItem(char* csText, MAPPLOTDIRECT& mapRetPush);

int    g_nReadInterval = 30;
string g_strReadPath;
HANDLE g_hMutex = NULL;
HANDLE g_hThreadTimer = NULL;
SYSTEMTIME g_sysTimeBak = {0};

void LockObject() {
	::WaitForSingleObject(g_hMutex, INFINITE);
}
void UnlockObject() {
	::ReleaseMutex(g_hMutex);
}

DWORD __stdcall Proc_Timer(LPVOID lpParameter)
{
	static int nTimer = 32767;
	while(!g_bExit) {
		Sleep(1000);

		nTimer++;
#if _DEBUG
//		printf("Proc_Timer() nTimer==%d in DEBUG.\n", nTimer);
#else
//		printf("Proc_Timer() nTimer==%d in RELEASE.\n", nTimer);
#endif
		if(nTimer<g_nReadInterval)
			continue;
		nTimer=0;
#if _DEBUG
		printf("Proc_Timer() == %ds LoadInstDirect() in DEBUG.\n", g_nReadInterval);
#else
		printf("Proc_Timer() == %ds LoadInstDirect() in RELEASE.\n", g_nReadInterval);
#endif
		MAPPLOTDIRECT mapNewPush;
		LoadInstDirect(mapNewPush);
		if(mapNewPush.size()>0) {
			// 先把mapNewPush与g_mapPlotDirect合并，然手再进行推送
			LockObject();
			MergePlotDirectMap(g_mapPlotDirect, mapNewPush);
			UnlockObject();
			PushInstrumentCtrlDirectToClient(mapNewPush);
		}
	}
	return 0;
}

void LoadConfig()
{
	char csIniFile[1024];
	char csText[1024];
	ZeroMemory(csIniFile, 1024);
	ZeroMemory(csText, 1024);
	DWORD dwLen = ::GetCurrentDirectory(1023, csIniFile);
	if(csIniFile[dwLen-1]!='\\')
		strcat(csIniFile, "\\");
	strcat(csIniFile, "PlotCtrlServer.INI");

	g_nReadInterval = ::GetPrivateProfileInt("Plot", "ReadInterval", 30, csIniFile);
	
	dwLen = ::GetPrivateProfileString("PlotImport", "PlotPath", "PlotData", csText, 1023, csIniFile);
	csText[dwLen]=0;
	g_strReadPath = csText;
}

PLOTCTRLSERVER_API void InitFunc(void)
{
#if _DEBUG
	getchar();
#endif
	LoadConfig();
	g_hMutex = ::CreateMutex(NULL, FALSE, "");
	DWORD dwThreadID = 0;
	g_hThreadTimer = ::CreateThread(NULL, 0, Proc_Timer, NULL, 0, &dwThreadID);
#if _DEBUG
	printf("Start plotCtrlServer in DEBUG.\n");
#else
	printf("Start plotCtrlServer in RELEASE.\n");
#endif
}

PLOTCTRLSERVER_API void ReleaseFunc(void)
{
	g_bExit = TRUE;
    WaitForSingleObject(g_hThreadTimer, 10000);
	CloseHandle(g_hThreadTimer);

	MAPSOCKETBUFF::iterator it;
	LockObject(); 
	for(it=g_mapRcvBuf.begin();it!=g_mapRcvBuf.end();it++)
    {
		if(it->second.pBuff != NULL) {
			free(it->second.pBuff);
			it->second.pBuff = NULL;
		}
    }
	g_mapRcvBuf.clear();
	UnlockObject();
	::CloseHandle(g_hMutex);
#if _DEBUG
	printf("Close plotCtrlServer in DEBUG.\n");
#else
	printf("Close plotCtrlServer in RELEASE.\n");
#endif
}


//accept后，调用此回调函数进行处理
PLOTCTRLSERVER_API void TcpSvrAcceptFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	BUFFDATA buff;
	ZeroMemory(&buff, sizeof(buff));
	buff.nBuffLen = RcvBufSize;
	buff.pBuff = (char*)malloc(buff.nBuffLen);
    if(buff.pBuff)
        g_mapRcvBuf[hSocket]=buff;

	MAPPLOTDIRECT mapNewPush;
	LockObject();
	mapNewPush = g_mapPlotDirect;
	UnlockObject();
	PushInstrumentCtrlDirect(hSocket, mapNewPush);

}


//断开tcp连接后，调用此回调函数进行处理
PLOTCTRLSERVER_API void TcpSvrDisconnFunc(Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	LockObject();
    MAPSOCKETBUFF::iterator it=g_mapRcvBuf.find(hSocket);
    if(it!=g_mapRcvBuf.end()) 
    {
		free(it->second.pBuff);
        g_mapRcvBuf.erase(it);
    }
	UnlockObject();
#if 0
	MAPINSTSUBS::iterator itSubs;
	LockObject();
	for(itSubs = g_mapInstSubs.begin(); itSubs != g_mapInstSubs.end(); itSubs++) {
		itSubs->second.erase(hSocket);
	}
	UnlockObject();
#endif
}


//从tcp得到数据后，调用此回调函数进行处理
PLOTCTRLSERVER_API void TcpSvrRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
	LockObject();
    MAPSOCKETBUFF::iterator it=g_mapRcvBuf.find(hSocket);

	if(it==g_mapRcvBuf.end()) {
        UnlockObject();
		return;
	}

	if(it->second.nDataLen+DataLen<=RcvBufSize)
	{
		memcpy(((char*)it->second.pBuff)+it->second.nDataLen, pData, DataLen);
		it->second.nDataLen+=DataLen;
	}

	if(it->second.nDataLen>0)
    {
	    //先跳过可能出现的无效数据
	    int off=0;
		while(off<it->second.nDataLen&&Stru_UniPkgHead::IsInvalidPkgHead(it->second.pBuff+off,it->second.nDataLen-off)) off++;

	    //循环解析接收到的数据
	    int LengthUsedByPkg;
		while(Stru_UniPkgHead::IsValidPkg(it->second.pBuff+off,it->second.nDataLen-off,&LengthUsedByPkg))
	    {
			ProcessPkg(hSocket,it->second.pBuff+off);

		    off+=LengthUsedByPkg;
	    }
	    if(off>0)
	    {
			memmove(it->second.pBuff,it->second.pBuff+off,it->second.nDataLen-off);
		    it->second.nDataLen-=off;
	    }
    }
	UnlockObject();

}


//tcp的定时处理
PLOTCTRLSERVER_API void TcpSvrOntimeFunc(void)
{
}

//处理Pkg
void ProcessPkg(SOCKET hSocket,const char* pPkg)
{
    const Stru_UniPkgHead& Pkg=*(const Stru_UniPkgHead*)pPkg;
    const char* pData=pPkg+sizeof(Pkg);

	if(Pkg.cmdid<0x200000) {
		if(Pkg.cmdid==CMDID_HEARTBEAT)
		{
			char buf[sizeof(Stru_UniPkgHead)+4];
			Stru_UniPkgHead& outPkgHead=*(Stru_UniPkgHead*)buf;
			outPkgHead.Set(0,CMDID_HEARTBEATRSP,0,0);
			*(unsigned int*)(buf+sizeof(outPkgHead))=TailID_UniPkg;

			CSocketBase::SendDataTcp(hSocket,buf,sizeof(buf));
			printf("%d: 收到连接[%u]心跳包\n",(int)time(NULL)%3600, hSocket);
		}
	}
	else {
		switch(Pkg.cmdid) {
			case Cmd_OPC_SubscribeCtrlDirect_Req:
			{
#if 0
				int nItemCount = Pkg.len/sizeof(TInstrumentIDType);
				string strInst;
				set<string> setPushInst;

				MAPINSTSUBS::iterator itSubs;
				for(int i=0; i<nItemCount; i++) {
					strInst = pData+i*sizeof(TInstrumentIDType);
					setPushInst.insert(strInst);
					// 调用ProcessPkg()前就已经加锁了，这里不能重复加锁
					//LockObject();
					itSubs = g_mapInstSubs.find(strInst);
					if(itSubs != g_mapInstSubs.end()) {
						itSubs->second.insert(hSocket);
					}
					else {
						set<SOCKET> setSocket;
						setSocket.insert(hSocket);
						g_mapInstSubs.insert(make_pair(strInst, setSocket));
					}
					//UnlockObject();
				}

				char buf[sizeof(Stru_UniPkgHead)+4];
				Stru_UniPkgHead& outPkgHead=*(Stru_UniPkgHead*)buf;
				outPkgHead.Set(0,Cmd_OPC_SubscribeCtrlDirect_Rsp,0,0);
				*(unsigned int*)(buf+sizeof(outPkgHead))=TailID_UniPkg;

				CSocketBase::SendDataTcp(hSocket,buf,sizeof(buf));

				MAPINSTDIRECT mapInstDirect;
				if(GetDirectMode(setPushInst, mapInstDirect))
					PushInstrumentCtrlDirectToClient(mapInstDirect);
#endif
			}
				break;
			case Cmd_OPC_UnSubscribeCtrlDirect_Req:
				break;
		};

	}
}

// 把Append的数据合并到Main中
BOOL MergePlotDirectMap(MAPPLOTDIRECT& mapMain, MAPPLOTDIRECT& mapAppend)
{
	MAPPLOTDIRECT::iterator it;
	MAPPLOTDIRECT::iterator itMain;
	map<string, DIRECTMODE>::iterator itdir;
	
	for(it = mapAppend.begin(); it != mapAppend.end(); it++) {
		itMain = mapMain.find(it->first);
		if(itMain == mapMain.end()) {
			mapMain.insert(make_pair(it->first, it->second));
		}
		else {
			for(itdir = it->second.begin(); itdir != it->second.end(); itdir++) {
				itMain->second[itdir->first] = itdir->second;
			}
		}
	}
	return TRUE;
}

BOOL GetDirectMode(set<string>& setInst, MAPPLOTDIRECT& mapInstDirect)
{
#if 0
	MAPPLOTDIRECT::iterator it;
	set<string>::iterator itInst;
	for(itInst = setInst.begin(); itInst != setInst.end(); itInst++) {
		it = g_mapInstDirect.find(*itInst);
		if(it != g_mapInstDirect.end()) {
			mapInstDirect[it->first] = it->second;
		}
	}
#endif
	return TRUE;
}

void PushInstrumentCtrlDirectToClient(MAPPLOTDIRECT& mapPush)
{
	MAPSOCKETBUFF::iterator it;
	
	for(it = g_mapRcvBuf.begin(); it != g_mapRcvBuf.end(); it++) {
		PushInstrumentCtrlDirect(it->first, mapPush);
	}


	// 这是订阅模式的代码，可以保留

	//MAPINSTSUBS::iterator it;
	//MAPPLOTDIRECT::iterator itPush;
	//INSTRUMENTCTRLDIRECT direct;
	//
	//for(itPush = mapPush.begin(); itPush != mapPush.end(); itPush++) {
	//	it = g_mapInstSubs.find(itPush->first);
	//	if(it != g_mapInstSubs.end()) {
	//		map<int, DIRECTMODE>::iterator itdir;
	//		for(itdir = itPush->second.begin(); itdir != itPush->second.end(); itdir++) {
	//			ZeroMemory(&direct, sizeof(direct));
	//			direct.nPlotID = itdir->second.nPlotID;
	//			direct.cDirectMode = itdir->second.cDirectMode;
	//			strncpy(direct.inst, itPush->first.c_str(), sizeof(direct.inst)-1);

	//			set<SOCKET>::iterator itsck;
	//			for(itsck = it->second.begin(); itsck != it->second.end(); itsck++) {
	//				PushInstrumentCtrlDirect(*itsck, direct);
	//			}
	//		}
	//	}
	//}
}

void PushInstrumentCtrlDirect(SOCKET hSocket, MAPPLOTDIRECT& mapPush)
{
	MAPINSTSUBS::iterator it;
	MAPPLOTDIRECT::iterator itPush;
	INSTRUMENTCTRLDIRECT direct;

	for(itPush = mapPush.begin(); itPush != mapPush.end(); itPush++) {
		map<string, DIRECTMODE>::iterator itdir;
		for(itdir = itPush->second.begin(); itdir != itPush->second.end(); itdir++) {
			ZeroMemory(&direct, sizeof(direct));
			direct.cDirectMode = itdir->second.cDirectMode;
			strncpy(direct.plotID, itdir->second.plotID, sizeof(direct.plotID));
			strncpy(direct.inst, itdir->first.c_str(), sizeof(direct.inst)-1);

			SendInstrumentCtrlDirect(hSocket, direct);
		}
	}
}

int SendInstrumentCtrlDirect(SOCKET hSocket, INSTRUMENTCTRLDIRECT& direct)
{
	char buf[sizeof(Stru_UniPkgHead)+sizeof(direct)+4];
	Stru_UniPkgHead& outPkgHead=*(Stru_UniPkgHead*)buf;
	outPkgHead.Set(sizeof(direct),Cmd_OPC_CtrlDirect_Push,0,0);
	INSTRUMENTCTRLDIRECT& outData=*(INSTRUMENTCTRLDIRECT*)(buf+sizeof(outPkgHead));
	outData = direct;
	*(unsigned int*)(buf+sizeof(outPkgHead)+sizeof(outData))=TailID_UniPkg;

	return CSocketBase::SendDataTcp(hSocket,buf,sizeof(buf));
}

BOOL LoadInstDirect(MAPPLOTDIRECT& mapRetPush)
{

	MAPPLOTDIRECT mapPush;
	string strFind(g_strReadPath);
	HANDLE hFile = NULL;
	BY_HANDLE_FILE_INFORMATION fileInfo;

	WIN32_FIND_DATA ffData = {0};
	strFind+="*.*";
	HANDLE hFind = FindFirstFile(strFind.c_str(), &ffData);
	if(hFind == INVALID_HANDLE_VALUE) 
		return FALSE;
	do {
		if(ffData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			continue;
		string strFile = g_strReadPath;
		strFile += ffData.cFileName;

		hFile = ::CreateFile(strFile.c_str(),		// file to open
						   GENERIC_READ,			// open for reading
						   FILE_SHARE_READ|FILE_SHARE_WRITE,	// share for reading
						   NULL,					// default security
						   OPEN_EXISTING,			// existing file only
						   FILE_ATTRIBUTE_NORMAL,	// normal file
						   NULL);					// no attr. template
		if(hFile == INVALID_HANDLE_VALUE) {
			printf("OpenFile [%s] is Fail!\n", strFile.c_str());
			Sleep(50);
			hFile = ::CreateFile(strFile.c_str(),		// file to open
							   GENERIC_READ,			// open for reading
							   FILE_SHARE_READ|FILE_SHARE_WRITE,	// share for reading
							   NULL,					// default security
							   OPEN_EXISTING,			// existing file only
							   FILE_ATTRIBUTE_NORMAL,	// normal file
							   NULL);					// no attr. template
			if(hFile == INVALID_HANDLE_VALUE) {
				printf("After Sleep(50) OpenFile [%s] is Fail!\n", strFile.c_str());
				continue;
			}
		}

		ZeroMemory(&fileInfo, sizeof(fileInfo));
		if(!GetFileInformationByHandle(hFile, &fileInfo))
			continue;

		DWORD dwBytesRead = 0;
		DWORD dwLastPos = 0;
		MAPFILEPOS::iterator it = g_mapFilePos.find(strFile);
		if(it != g_mapFilePos.end())
			dwLastPos = it->second;

		DWORD dwReadLen = fileInfo.nFileSizeLow-dwLastPos;
		if(dwReadLen==0) {
			printf("File [%s] 文件长度[%u], 可读长度为0\n", strFile.c_str(), fileInfo.nFileSizeLow);
			continue;
		}

		char* pBuff = new char[dwReadLen+4];

		ZeroMemory(pBuff, dwReadLen+4);

		printf("file [%s], seek to [%u].\n", strFile.c_str(), dwLastPos);
		::SetFilePointer(hFile, dwLastPos, NULL, FILE_BEGIN);
		if(!::ReadFile(hFile, pBuff, dwReadLen, &dwBytesRead, NULL)) {
			::CloseHandle(hFile);
			continue;
		}
		::CloseHandle(hFile);

		dwLastPos += dwBytesRead;
		g_mapFilePos[strFile] = dwLastPos;
		
		// 把策略文本解析成为map数据格式
		ParsePlotData(pBuff, mapPush);

	} while(FindNextFile(hFind, &ffData));
	FindClose(hFind);

	mapRetPush = mapPush;
	
	return TRUE;
}

BOOL ParsePlotData(char* pBuff, MAPPLOTDIRECT& mapRetPush)
{
	char* pHead = pBuff;
	char* pHeadNext = NULL;
	do {
		pHeadNext = GetPlotItem(pHead, mapRetPush);
		if(pHeadNext!=NULL)
			pHead = pHeadNext;
	} while(pHeadNext!=NULL);
	return TRUE;
}


char* GetPlotItem(char* csText, MAPPLOTDIRECT& mapRetPush)
{
	if(csText==NULL)
		return NULL;

	int nItemLen = 0;
	char* pCurrItem = NULL;
	char* pNextItem = NULL;

	pCurrItem = csText;
	do 
	{
		while(pCurrItem[nItemLen]!='\r' && pCurrItem[nItemLen]!='\000')
		{
			nItemLen++;
		}
		if(pCurrItem[nItemLen]=='\000')
			break;
		if(nItemLen>=20)
			break;
		pCurrItem = pCurrItem+nItemLen+2;
	} while(pCurrItem[nItemLen]!='\000');

	if(pCurrItem[nItemLen]=='\r')
		pNextItem = pCurrItem+nItemLen+2;

	if(nItemLen<20)
		return pNextItem;

	char* csItem = new char[nItemLen+8];
	char csCodes[128];

	ZeroMemory(csItem, nItemLen+8);
	ZeroMemory(csCodes, sizeof(csCodes));
	strncpy(csItem, csText, nItemLen);

	char csDateTime[30] = {0};
	char csPlotID[30] = {0};
	char csInstrumentID[60] = {0};
	int nDirectMode = 0;

	int nRet = sscanf_s(csItem, "%[^,],%[^,],%[^,],%d", 
			csDateTime, sizeof(csDateTime)-1, 
			csPlotID, sizeof(csPlotID)-1, 
			csInstrumentID, sizeof(csInstrumentID)-1,
			&nDirectMode);
	delete[] csItem;
	if(nRet != 4) {
		printf("sscanf_s() return != 4\n");
		return pNextItem;
	}
	printf("sscanf(), %s, %s, %s, %d\n", csDateTime, csPlotID, csInstrumentID, nDirectMode);

	DIRECTMODE directMode = {0};
	switch(nDirectMode) {
	case 1:
		directMode.cDirectMode = OPC_DirectMode_OnlyLong;
		break;
	case -1:
		directMode.cDirectMode = OPC_DirectMode_OnlyShort;
		break;
	case 0:
	default:
		directMode.cDirectMode = OPC_DirectMode_LongShort;
		break;
	};
	strncpy(directMode.plotID, csPlotID, sizeof(directMode.plotID)-1);
	directMode.updateTime = ::GetTickCount();
	
	string strInst(csInstrumentID);
	MAPPLOTDIRECT::iterator it = mapRetPush.find(directMode.plotID);
	if(it == mapRetPush.end()) {
		map<string, DIRECTMODE> mapDirect;
		mapDirect[strInst] = directMode;
		mapRetPush[directMode.plotID] = mapDirect;
	}
	else {
		it->second[strInst] = directMode;
	}

	return pNextItem;
}
