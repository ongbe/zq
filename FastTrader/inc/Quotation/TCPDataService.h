#pragma once

#include "TcpClientWorker.h"
#define  nDataPkgArriveMSGID  (WM_USER +9991)
#define  nDataPkgArriveNextMSGID (WM_USER +9992)
class CTCPDataService
{
public:
	enum {	enmNetNothing, 
			enmNetDisconnect, 
			enmNetConnectWaiting, 
			enmNetConnectSuccess, 
			enmNetConnectFail, 
			enmNetDisconnectWaiting 
	};

public:
	static CTCPDataService* GetOnlyOneInstance();
	static void ReleaseOnlyOneInstance();

	static void	CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//从tcp得到数据后，调用此回调函数进行处理
	static void CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//开始调用connect()后，调用此函数通知应用层
	static void	CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//断开tcp连接后，调用此回调函数进行处理
	static void	CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);						//连接成功后，调用此回调函数进行处理
	static void	CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);							//连接失败后，调用此回调函数进行处理
	
	static DWORD WINAPI Proc_PushToForm(LPVOID lpvParam); 
	static VOID CALLBACK TimerProc_HeartBeat(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

public:

	int GetNetStatus() {
		return m_nNetStatus;
	};
	void SetNetStatus(int nNetStatus) {
		m_nNetStatus = nNetStatus;
	};

	bool RegisterForm(HWND hWnd);
	bool UnregisterForm(HWND hWnd);

	bool Connect();
	bool SendData(char* pBuff, int nLen);
	void OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

protected:
	CTCPDataService(void);
	virtual ~CTCPDataService(void);

	void LoadConfig();
	bool Init();

protected:
	CTcpClientWorker*	m_poTcpClient;
	wxString			m_strBNCMIPAddr;
	WORD				m_wBNCMPort;

	int					m_nNetStatus;
	UINT GetDataPkgArriveMSGID();
	UINT GetDataPkgArriveNextMSGID();
};
