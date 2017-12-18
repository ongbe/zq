#pragma once
#include "TcpClientWorker.h"
#include "CommonPkg.h"
#include "MemoryCache.h"
#include "KernelStruct.h"

//////////////////////////////////////////////////////////////////////////
//行情接收ID，以后去掉
#define  MAKE_CMDID(_base,_offset) \
	((int)( (int)(_base) << 16 ) + (int)(_offset))
#define  CMDID_BASE_HIWORD_QUOT						0x000F //行情高位CMDID
#define Cmd_SubscribeQuotEvent_Req						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0001)	//订阅行情事件请求
#define Cmd_SubscribeQuotEvent_Rsp						    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0002)	//订阅行情事件返回
#define Cmd_UnSubscribeQuotEvent_Req						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0003)	//退订行情事件请求
#define Cmd_UnSubscribeQuotEvent_Rsp						MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0004)	//退订行情事件返回
#define Cmd_QuotEvent_Push								    MAKE_CMDID(CMDID_BASE_HIWORD_QUOT,0x0005)	//行情事件推送

/*
enum 
{	
	enmNetNothing, 
	enmNetDisconnect, 
	enmNetConnectWaiting, 
	enmNetConnectSuccess, 
	enmNetConnectFail, 
	enmNetDisconnectWaiting 
};*/
class CTCPHistoryService
{
public:


	enum REQ_STEP
	{
		REQ_STEP_INIT,
		REQ_STEP_REQ_SERVER_ADDR,
		REQ_STEP_SERVER_ADDR_RECVED,
		REQ_STEP_CONNECT_TO_SERVER,
		REQ_STEP_REQ_LICENCE_LIST,
		REQ_STEP_LICENCE_LIST_RECVED
	};

public:
	static CTCPHistoryService* GetInstance();
	static void DestroyInstance();

	static void	CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);	//从tcp得到数据后，调用此回调函数进行处理
	static void CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);							//开始调用connect()后，调用此函数通知应用层
	static void	CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);							//断开tcp连接后，调用此回调函数进行处理
	static void	CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);						//连接成功后，调用此回调函数进行处理
	static void	CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket,int UserData);							//连接失败后，调用此回调函数进行处理
	static void	CallbackOntimeFunc(int UserData);				

	static VOID CALLBACK OnTimer1(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	static VOID CALLBACK OnTimer2(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

public:
	int GetNetStatus() { return m_nNetStatus;};
	void SetNetStatus(int nNetStatus) { m_nNetStatus = nNetStatus;};

	bool Connect( std::string strIP, int nPort );
	bool SendData(UINT nCMDID, void* pBuff, int nLen, UINT seq = 0);
	//	void OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	bool LicenceRecved();
	bool GetLicence(std::string strAppID, std::string& strLicence);

private:
	CTCPHistoryService(void);
	virtual ~CTCPHistoryService(void);

	int GetPackageHeadTailLen();
	int GetPackageHeadLen();
	static bool VerifyRequest(Stru_UniPkgHead& head);

private:
	static CTCPHistoryService* m_pInstance;
	CTcpClientWorker*	m_pTcpClient;
	CMemoryCache*		m_pStreamCache;	
	int					m_nNetStatus;

	Stru_ServerAddr		m_addr;
	REQ_STEP			m_step;
	std::map<std::string, std::string> m_licenceMap;

public:	
	static HWND               m_HwndParent;
	void			   SetMessWnd(HWND  HwndParent);
};
