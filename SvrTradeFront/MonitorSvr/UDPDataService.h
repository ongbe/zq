#pragma once
#include "stdafx.h"
#include "UdpSimpleWorker.h"
#include "CommonPkg.h"
#include <set>
#include <queue>



#define RECV_DATA_MSG WM_USER+500



class CMemoryCache;

class CUDPDataService
{
public:
	enum 
	{	
		enmNetNothing, 
		enmNetDisconnect, 
		enmNetConnectWaiting, 
		enmNetConnectSuccess, 
		enmNetConnectFail, 
		enmNetDisconnectWaiting 
	};

public:
	static CUDPDataService* GetInstance();
	static void DestroyInstance();

	static void	CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//从tcp得到数据后，调用此回调函数进行处理
	
	static void CALLBACK OnTimer_HeartBeat(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	static DWORD ThreadFunc(void* pData);

public:
	int GetNetStatus() { return m_nNetStatus;};
	void SetNetStatus(int nNetStatus) { m_nNetStatus = nNetStatus;};
	bool IsConnected();

	bool Connect( std::string strIP, int nPort );
	bool SendData(Stru_IPAddress RemoteAddr, UINT nCMDID, void* pBuff, int nLen, UINT seq = 0, bool bNeedLogon = true, int nUserData1 = 0);
	
	void RegisteHWnd( HWND hWnd);
	void UnRegisteHWnd( HWND hWnd);

	void SetLogonStatus(bool bLogon);
	bool IsLogon();
	std::string GetLastError();

private:
	CUDPDataService(void);
	virtual ~CUDPDataService(void);

	static bool VerifyRequest(Stru_UniPkgUDPHead& head);

private:
	static CUDPDataService* m_pInstance;
	CUdpSimpleWorker*	m_pTcpClient;
	CMemoryCache*		m_pStreamCache;	
	int					m_nNetStatus;

	Stru_ServerAddr		m_addr;
	std::set<HWND>		m_hWndSet;
	CRITICAL_SECTION	m_csHWnd;
	CRITICAL_SECTION	m_csMsgQueue;

	std::queue<RecvData> m_msgQueue;
	HANDLE				m_hWorkerThread;
	bool				m_bWorkerThreadExit;

	bool				m_bLogon;
	std::string			m_strLastError;
	//HANDLE				m_hTimerThread;
	//bool				m_bTimerThreadExit;
};
