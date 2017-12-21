#include "DataPackageForwarding.h"
#include "SvrTradeFront.h"
#include <Windows.h>
#include <time.h>
#include "SystemParamsWatch.h"
#include "DataStruct.h"


DWORD WINAPI CDataPackageForwarding::TimerProc(LPVOID lpParameter)
{
	if(lpParameter==NULL)
		return -1;

	CDataPackageForwarding* pForwarding = (CDataPackageForwarding*)lpParameter;
	while(!pForwarding->IsExit()) {
		Sleep(1000);
		pForwarding->OnTimer();
	}
	return 0;
}

CDataPackageForwarding::CDataPackageForwarding(void)
{
	char csText[128] = {0};
	DWORD dwRet = ::GetProfileString("Setting", "LastServSeq", "1", csText, 127);

	m_nCurrSeqID = atoi(csText);
	m_nRemotePort = 0;
	m_pUdpConn = NULL;
	m_bIsExit = false;
	m_dwThreadID = 0;
	m_hThread = NULL;
	InitializeCriticalSection(&m_hLock);
}

CDataPackageForwarding::~CDataPackageForwarding(void)
{
	DeleteCriticalSection(&m_hLock);
}

int  CDataPackageForwarding::Init()
{
	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,FORWARINGSETTING_FILE);

	char csAddress[128] = {0};
	short int nPort = {0};

	memset(csAddress,0,sizeof(csAddress));
	GetPrivateProfileString("connect","RemoteAddr","127.0.0.1",csAddress,sizeof(csAddress)-1,lsPath);
	m_strRemoteAddr = csAddress;
	m_nRemotePort = (short int)GetPrivateProfileInt("connect","RemotePort",9009,lsPath);

	memset(csAddress,0,sizeof(csAddress));
	GetPrivateProfileString("connect","LocalAddr","127.0.0.1",csAddress,sizeof(csAddress)-1,lsPath);
	nPort = (short int)GetPrivateProfileInt("connect","LocalPort",32767,lsPath);
	
	m_pUdpConn = new CUdpDataConnect(m_strRemoteAddr, m_nRemotePort);
	m_pUdpConn->Init();

	m_hThread = CreateThread(NULL, 0, TimerProc, this, 0, &m_dwThreadID);

	return 0;
}

void CDataPackageForwarding::Release()
{
	m_bIsExit = true;
	if(WaitForSingleObject(m_hThread, 5000)==WAIT_TIMEOUT) {
		::TerminateThread(m_hThread, -11);
	}
	m_hThread = NULL;

	if(m_pUdpConn != NULL) {
		m_pUdpConn->Release();
		delete m_pUdpConn;
		m_pUdpConn = NULL;
	}
}

int CDataPackageForwarding::GetNextSeqID()
{
	char csText[128] = {0};
	sprintf(csText, "%d", m_nCurrSeqID+1);
	::WriteProfileString("Setting", "LastServSeq", csText);
	return InterlockedExchangeAdd(&m_nCurrSeqID, 1);
}

int CDataPackageForwarding::SendSrcPackage(void* pPackage, int nLen, int nServSeqID, int nSocket)
{
	int nTotalLen = nLen-sizeof(Stru_UniPkgHead)+sizeof(Stru_UniPkgUDPHead);
	if(nTotalLen > 65536)
		return -1;

	unsigned char* pBuff = new unsigned char[nTotalLen];
	std::auto_ptr<unsigned char> ptr(pBuff);
	Stru_UniPkgHead& srcHead = *((Stru_UniPkgHead*)pPackage);
	Stru_UniPkgUDPHead& head = *((Stru_UniPkgUDPHead*)pBuff);
	SYSTEMTIME localtime = {0};

	GetLocalTime(&localtime);
	head.Set(nLen-sizeof(Stru_UniPkgHead)-4, srcHead.cmdid, srcHead.moduleid, 
			srcHead.seq, srcHead.subseq, srcHead.userdata1,	
			1, nServSeqID, nSocket, time(NULL), localtime.wMilliseconds);
	memcpy(pBuff+sizeof(Stru_UniPkgUDPHead), (char*)pPackage+sizeof(Stru_UniPkgHead), nLen-sizeof(Stru_UniPkgHead));
	
	EnterCriticalSection(&m_hLock);
	int nRet = m_pUdpConn->Senddata((const char*)pBuff, nTotalLen);
	LeaveCriticalSection(&m_hLock);
	
	LOG_INFO("SendTo(发送数据) : cmdid=[%d], RequestID=[%d], DirectMode=[%d], ServSeq=[%d], PackageLen=[%d], Ret=[%d]",
			head.cmdid, head.userdata1, head.userdata2, head.userdata3, nTotalLen, nRet);

	return nRet?0:-1;
}

int CDataPackageForwarding::SendPackageForApi(void* pData, int nLen, int nRequestID)
{
	int nTotalLen = sizeof(Stru_UniPkgUDPHead)+nLen+4;
	if(nTotalLen > 65536)
		return -1;

	unsigned char* pBuff = new unsigned char[nTotalLen];
	std::auto_ptr<unsigned char> ptr(pBuff);
	Stru_UniPkgUDPHead& head = *((Stru_UniPkgUDPHead*)pBuff);
	SYSTEMTIME localtime = {0};
	Stru_ThreadData tdata;

	memset(&tdata, 0, sizeof(tdata));
	_GetThreadServData(tdata);

	if(pBuff == NULL)
		return -1;

	//memset(pBuff, 0, nTotalLen);
	GetLocalTime(&localtime);
	head.Set(nLen, tdata.base.cmdid, tdata.base.moduleid, 
			tdata.base.seq, tdata.base.subseq, tdata.base.userdata1,	
			2, tdata.nServSeq, tdata.nSocket, (int)time(NULL), localtime.wMilliseconds);
	
	if(pData)
		memcpy(pBuff+sizeof(Stru_UniPkgUDPHead), pData, nLen);
	*(unsigned int*)(pBuff+sizeof(Stru_UniPkgUDPHead) + nLen) = TailID_UniPkg;

	EnterCriticalSection(&m_hLock);
	int nRet = m_pUdpConn->Senddata((const char*)pBuff, nTotalLen);
	LeaveCriticalSection(&m_hLock);
	
	LOG_INFO("SendTo(发送数据) : cmdid=[%d], RequestID=[%d], DirectMode=[%d], ServSeq=[%d], PackageLen=[%d], Ret=[%d]",
			head.cmdid, head.userdata1, head.userdata2, head.userdata3, nTotalLen, nRet);

	return nRet?0:-1;
}

int CDataPackageForwarding::SendPackage(void* pData, int nLen, int nCmd, int nSeq, int nUserData1, int nUserData2, int nUserData3, int nUserData4)
{
	int nTotalLen = sizeof(Stru_UniPkgUDPHead)+nLen+4;
	if(nTotalLen > 65536)
		return -1;

	unsigned char* pBuff = new unsigned char[nTotalLen];
	std::auto_ptr<unsigned char> ptr(pBuff);
	Stru_UniPkgUDPHead& head = *((Stru_UniPkgUDPHead*)pBuff);
	SYSTEMTIME localtime = {0};

	if(pBuff == NULL)
		return -1;

	//memset(pBuff, 0, nTotalLen);
	GetLocalTime(&localtime);
	head.Set(nLen, nCmd, 0, nSeq, 0, nUserData1,	
			nUserData2, nUserData3, nUserData4, 
			(int)time(NULL), localtime.wMilliseconds);
	
	if(pData)
		memcpy(pBuff+sizeof(Stru_UniPkgUDPHead), pData, nLen);
	*(unsigned int*)(pBuff+sizeof(Stru_UniPkgUDPHead) + nLen) = TailID_UniPkg;

	EnterCriticalSection(&m_hLock);
	int nRet = m_pUdpConn->Senddata((const char*)pBuff, nTotalLen);
	LeaveCriticalSection(&m_hLock);

	if(head.cmdid < 0x18020001) {
		LOG_INFO("SendTo(发送数据) : cmdid=[%d], RequestID=[%d], DirectMode=[%d], ServSeq=[%d], PackageLen=[%d], Ret=[%d]",
				head.cmdid, head.userdata1, head.userdata2, head.userdata3, nTotalLen, nRet);
	}
	return nRet?0:-1;
}

void CDataPackageForwarding::OnTimer()
{
	SendServerWatchBaseParams();
}

void CDataPackageForwarding::SendServerWatchBaseParams()
{
	Stru_Watch watch = {0};

	watch.CpuUsage = CSystemParamsWatch::GetCpuUsage();
	watch.Memory = CSystemParamsWatch::GetMemory();
	watch.DiskSpace = CSystemParamsWatch::GetDiskSpace();
	printf("CPU: %02.1f%%, \tMemory: % 5I64dM, \tAvailDiskSpace: % 5I64dG\n", 
			watch.CpuUsage/10.0, watch.Memory/1024/1024, 
			watch.DiskSpace/1024/1024/1024);
	SendPackage(&watch, sizeof(watch), 0x18020001);
}
