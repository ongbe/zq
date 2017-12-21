#pragma once

#include <string>
#include "UdpDataConnect.h"
#include "CommonPkg.h"
#include <windows.h>
#include <queue>

class CDataPackageForwarding
{
public:

#pragma pack(push)
#pragma pack(1)

	//typedef struct {
	//	Stru_UniPkgHead base;
	//	int nUtcTime;
	//	int nMS;
	//} Stru_UDPPkgHead;

	typedef struct {
		Stru_UniPkgHead base;
		int nServSeq;
		int nSocket;
	} Stru_ThreadData;

	typedef struct {
		int CpuUsage;		// 当前cpu占用率，千分率
		INT64 Memory;		// 当前内存使用量
		INT64 DiskSpace;	// 服务程序所在硬盘当前可用空间
	} Stru_Watch;

#pragma pack(pop)

public:
	static DWORD WINAPI TimerProc(LPVOID lpParameter);

public:
	CDataPackageForwarding(void);
	virtual ~CDataPackageForwarding(void);

	bool IsExit() { return m_bIsExit;};

	int  Init();
	void Release();

	int GetNextSeqID();
	int SendSrcPackage(void* pPackage, int nLen, int nServSeqID, int nSocket);
	int SendPackageForApi(void* pData, int nLen, int nRequestID);
	int SendPackage(void* pData, int nLen, int nCmd, int nSeq=0, 
			int nUserData1=0, int nUserData2=0, int nUserData3=0, int nUserData4=0);
	
	void OnTimer();

protected:
	void SendServerWatchBaseParams();

protected:
	volatile long	m_nCurrSeqID;
	std::string		m_strRemoteAddr;
	short int		m_nRemotePort;
	CUdpDataConnect*	m_pUdpConn;
	bool			m_bIsExit;
	DWORD			m_dwThreadID;
	HANDLE			m_hThread;
	CRITICAL_SECTION	m_hLock;
};
