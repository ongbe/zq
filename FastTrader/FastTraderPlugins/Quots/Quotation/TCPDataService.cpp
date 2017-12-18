#include "StdAfx.h"
#include "../inc/Quotation/TCPDataService.h"
#include "..\inc\Tools\Tools_Ansi.h"

#define TimerHeartBeat 0x702001

CTCPDataService* g_poCTCPDataService = NULL;

DWORD g_dwPushToFormThreadId = 0; 
HANDLE g_hPushToFormThread = NULL;

// 用于队列处理的关键区域
CRITICAL_SECTION g_CS_QueueDataPackage;
CRITICAL_SECTION g_CS_SetRequestForm;

// 用于缓存数据包的队列
queue<char*> g_queueDataPackage;
set<HWND> g_setRequestForm;

void LockQueueDataPackage() {
	EnterCriticalSection(&g_CS_QueueDataPackage);
}

void UnlockQueueDataPackage() {
	LeaveCriticalSection(&g_CS_QueueDataPackage);
}

void LockSetRequestForm() {
	EnterCriticalSection(&g_CS_SetRequestForm);
}

void UnlockSetRequestForm() {
	LeaveCriticalSection(&g_CS_SetRequestForm);
}

void PushQueueDataPackage(char* pBuff)
{
	g_queueDataPackage.push(pBuff);
}

VOID CALLBACK CTCPDataService::TimerProc_HeartBeat(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if(g_poCTCPDataService==NULL)
		return;
	g_poCTCPDataService->OnTimer_HeartBeat(uMsg, idEvent, dwTime);
}

DWORD WINAPI CTCPDataService::Proc_PushToForm(LPVOID lpvParam)
{
	
	if(g_poCTCPDataService == NULL)
		return 0xffffffff;
	
	char* pBuff = NULL;
	BOOL bIsFound = FALSE;
	
	while(1) { 
		queue<char*> queueTemp;
		LockQueueDataPackage();
		while(!g_queueDataPackage.empty()) {
			queueTemp.push(g_queueDataPackage.front());
			g_queueDataPackage.pop();
		};
		UnlockQueueDataPackage();

		set<HWND>::iterator it;
		queue<HWND> queueInvalidForm;
	
		while(!queueTemp.empty()) {
			pBuff = queueTemp.front();
			queueTemp.pop();
			bIsFound = FALSE;

			if(pBuff != NULL) {

				LockSetRequestForm();
				for(it=g_setRequestForm.begin(); it!=g_setRequestForm.end(); it++) {
					if(IsWindow(*it)) {
						::SendMessage(*it, nDataPkgArriveMSGID, 0, (LPARAM)pBuff);
					}
					else {
						queueInvalidForm.push(*it);
					}
				}
				while(!queueInvalidForm.empty()) {
					g_setRequestForm.erase(queueInvalidForm.front());
					queueInvalidForm.pop();
				}
				UnlockSetRequestForm();
			}
			delete[] pBuff;
		}
		Sleep(5);
	}
	
	return 1;
}

volatile int g_nPkgLen = 0;
volatile int g_nDataDataPos = 0;
char* g_pTmpDataBuff = NULL;
Stru_UniPkgHead g_TmpHead;

//从tcp得到数据后，调用此回调函数进行处理
void CTCPDataService::CallbackReceiveData(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if(g_poCTCPDataService==NULL)
		return;

	char* pTmpData = pData;
	int nTmpDataLen = DataLen;
	
	LockQueueDataPackage();
	do {
		if(g_nDataDataPos+nTmpDataLen < sizeof(Stru_UniPkgHead)) {
			memcpy(&g_TmpHead+g_nDataDataPos, pData, nTmpDataLen);
			g_nDataDataPos+=nTmpDataLen;
			pTmpData = NULL;
			nTmpDataLen = 0;
		}
		else {
			if(g_nDataDataPos<sizeof(Stru_UniPkgHead)) {
				memcpy(&g_TmpHead+g_nDataDataPos, pData, sizeof(Stru_UniPkgHead)-g_nDataDataPos);
				g_nPkgLen = g_TmpHead.len+sizeof(Stru_UniPkgHead)+sizeof(DWORD);
				g_pTmpDataBuff = new char[g_nPkgLen];
				ZeroMemory(g_pTmpDataBuff, g_nPkgLen);
				memcpy(g_pTmpDataBuff, &g_TmpHead, sizeof(Stru_UniPkgHead));
			}
			if(g_nDataDataPos+DataLen < g_nPkgLen) {
				memcpy(g_pTmpDataBuff+g_nDataDataPos, pData, nTmpDataLen);
				g_nDataDataPos+=nTmpDataLen;
				pTmpData = NULL;
				nTmpDataLen = 0;
			}
			else {
				memcpy(g_pTmpDataBuff+g_nDataDataPos, pData, g_nPkgLen-g_nDataDataPos);
				pTmpData = pTmpData+(g_nPkgLen-g_nDataDataPos);
				nTmpDataLen = nTmpDataLen-(g_nPkgLen-g_nDataDataPos);

				PushQueueDataPackage(g_pTmpDataBuff);

				g_nPkgLen = 0;
				g_nDataDataPos = 0;
				g_pTmpDataBuff = NULL;
			}
		}
	} while(nTmpDataLen>0);
	UnlockQueueDataPackage();
}

//开始调用connect()后，调用此函数通知应用层
void CTCPDataService::CallbackStartConnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if(g_poCTCPDataService!=NULL) {
		g_poCTCPDataService->SetNetStatus(enmNetConnectWaiting);
	}
}

//断开tcp连接后，调用此回调函数进行处理
void CTCPDataService::CallbackDisconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	g_nDataDataPos = 0;
	if(g_pTmpDataBuff != NULL) {
		delete g_pTmpDataBuff;
		g_pTmpDataBuff = NULL;
	}
	if(g_poCTCPDataService!=NULL) {
		g_poCTCPDataService->SetNetStatus(enmNetDisconnect);
	}
}

//连接成功后，调用此回调函数进行处理
void CTCPDataService::CallbackConnectSuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if(g_poCTCPDataService!=NULL) {
		g_poCTCPDataService->SetNetStatus(enmNetConnectSuccess);
	}
}

//连接失败后，调用此回调函数进行处理
void CTCPDataService::CallbackConnectFail(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket)
{
	if(g_poCTCPDataService!=NULL) {
		g_poCTCPDataService->SetNetStatus(enmNetDisconnect);
	}
}

CTCPDataService* CTCPDataService::GetOnlyOneInstance()
{
	bool bIsInitOK = TRUE;
	if(g_poCTCPDataService==NULL) {
		InitializeCriticalSection(&g_CS_QueueDataPackage);
		InitializeCriticalSection(&g_CS_SetRequestForm);
		g_poCTCPDataService = new CTCPDataService();
		bIsInitOK = g_poCTCPDataService->Init();
	}
	if(bIsInitOK)
		return g_poCTCPDataService;
	else
		return NULL;
}

void CTCPDataService::ReleaseOnlyOneInstance()
{
	if(g_poCTCPDataService!=NULL) {
		KillTimer(NULL, TimerHeartBeat);
		::SuspendThread(g_hPushToFormThread);
		Sleep(5);
		DeleteCriticalSection(&g_CS_QueueDataPackage);
		DeleteCriticalSection(&g_CS_SetRequestForm);
		delete g_poCTCPDataService;
		g_poCTCPDataService = NULL;
	}
}

CTCPDataService::CTCPDataService(void) : 
		m_poTcpClient(NULL),
		m_wBNCMPort(0),
		m_nNetStatus(enmNetNothing)
{
}

CTCPDataService::~CTCPDataService(void)
{
	if(m_poTcpClient!=NULL) {
		delete m_poTcpClient;
		m_poTcpClient = NULL;
	}
	m_nNetStatus = enmNetNothing;
}

void CTCPDataService::LoadConfig()
{
	wxString iniFile;
	DWORD dwRetLen = 0;
	char strTemp[1024];

	ZeroMemory(strTemp, sizeof(strTemp));
	
	iniFile = GetBNCMIniFile();
	dwRetLen = GetPrivateProfileString("BNCM", "IPAddr", "", strTemp, sizeof(strTemp)-1, iniFile);
	m_strBNCMIPAddr = strTemp;
	m_wBNCMPort = GetPrivateProfileInt("BNCM", "Port", 0, iniFile);
}

bool CTCPDataService::Init()
{
	LoadConfig();
	g_hPushToFormThread = CreateThread(NULL, 0, Proc_PushToForm, 
			NULL, 0, &g_dwPushToFormThreadId);
	if(g_hPushToFormThread == NULL) {
//		TRACE("CreateThread failed\n"); 
		return false;
	}
	SetTimer(NULL, TimerHeartBeat, 5000, TimerProc_HeartBeat);
	return Connect();
}

bool CTCPDataService::Connect()
{
	if(m_poTcpClient!=NULL) {
		delete m_poTcpClient;
		m_poTcpClient = NULL;
		Sleep(500);
	}

	Stru_IPAddress tIPAddr, tServAddr;

	ZeroMemory(&tIPAddr, sizeof(tIPAddr));
	ZeroMemory(&tServAddr, sizeof(tServAddr));
	
	char ansi_string[30];
	ZeroMemory(&ansi_string, sizeof(ansi_string));
	strcpy(ansi_string,m_strBNCMIPAddr.mb_str());


	tServAddr.IP = CTools_Ansi::ConvertStrIP(ansi_string);
	tServAddr.Port = m_wBNCMPort;
	m_poTcpClient = new CTcpClientWorker(tIPAddr, tServAddr, 
			CallbackReceiveData, CallbackDisconnect, CallbackStartConnect, 
			CallbackConnectSuccess, CallbackConnectFail, NULL);
	SetNetStatus(enmNetConnectWaiting);

	return true;
}

bool CTCPDataService::SendData(char* pBuff, int nLen)
{
	if(m_poTcpClient!=NULL&&m_nNetStatus==enmNetConnectSuccess) {
		m_poTcpClient->SendData(pBuff, nLen);
		return true;
	}
	else {
		return false;
	}
}

bool CTCPDataService::RegisterForm(HWND hWnd)
{
	LockSetRequestForm();
	g_setRequestForm.insert(hWnd);
	UnlockSetRequestForm();
	return true;
}

bool CTCPDataService::UnregisterForm(HWND hWnd)
{
	LockSetRequestForm();
	g_setRequestForm.erase(hWnd);
	UnlockSetRequestForm();
	return true;
}

void CTCPDataService::OnTimer_HeartBeat(UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if(m_nNetStatus==enmNetConnectSuccess) {
		int nPkgLen = sizeof(Stru_UniPkgHead)+sizeof(DWORD);
		char* pBuff = new char[nPkgLen];
		ZeroMemory(pBuff, nPkgLen);
		PackageData(CMDID_Heartbeat, ModuleID_BNCMClient, 0, pBuff, nPkgLen);
		SendData(pBuff, nPkgLen);
		delete[] pBuff;

		//DWORD dwTail = TailID_UniPkg;
		//int nPkgLen = sizeof(Stru_UniPkgHead)+sizeof(DWORD);
		//char* pBuff = new char[nPkgLen];
		//Stru_UniPkgHead* pHead = (Stru_UniPkgHead*)pBuff;
		//pHead->headid = HeadID_UniPkg;
		//pHead->cmdid = CMDID_Heartbeat;
		//pHead->len = 0;//sizeof(Stru_UniPkgHead)+sizeof(DWORD);
		//memcpy(pBuff+(nPkgLen-4), &dwTail, sizeof(DWORD));
		//SendData(pBuff, pHead->len);
		//delete[] pBuff;
	}
}
UINT CTCPDataService::GetDataPkgArriveMSGID()
{
	return nDataPkgArriveMSGID;
}

UINT CTCPDataService::GetDataPkgArriveNextMSGID()
{
	return nDataPkgArriveNextMSGID;
}
