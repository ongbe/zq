// PlusinAPI.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <Windows.h>
#include <winver.h>
#include "PlusinImpl.h"
#include "APIMainDlg.h"
#include "UIThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_ID	1000

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	if ( DEBUG_ON == 0)
	{
		PlusinImpl::GetInstance()->Send(CMDID_CONSOLE_HEARTBEAT, NULL, 0);
	}
}

PlusinImpl* PlusinImpl::m_pInstance = NULL;

PlusinImpl* PlusinImpl::GetInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if ( NULL == m_pInstance )
	{
		m_pInstance = new PlusinImpl();
	}

	return m_pInstance;
}

void PlusinImpl::DestroyInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if ( NULL != m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

PlusinImpl::PlusinImpl()
: m_pUIThread(NULL)
, m_nPos(NEED_NO_POS_NO_BIND)
, m_hAPPHWnd(NULL)
, m_hTargetWnd(NULL)
, m_hNotifyThreadHandle(NULL)
, m_bNotifyThreadRun(true)
, m_bInit(false)
, m_appMsgCallBack( NULL )
, m_strAppID("")
, m_strLicense("")
, m_pAPIMainDlg(NULL)
, m_hInitDoneEvent(NULL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	InitializeCriticalSection(&m_cs);
}

void PlusinImpl::Init(const char* szAppID, 
					  const char* szLicense, 
					  MsgCallBackFunc msgCallBackFn, 
					  HWND hTargetWnd,
					  int nPos, 
					  HWND hAPPHWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( NULL == szAppID || NULL == szLicense )
	{
		return;
	}

	m_strAppID = szAppID;
	m_strLicense = szLicense;
	m_appMsgCallBack = msgCallBackFn;
	m_nPos = nPos;
	m_hAPPHWnd = hAPPHWnd;
	m_hTargetWnd = hTargetWnd;

	m_hInitDoneEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	//HINSTANCE hinstance = GetPlusinAPIDLLHandle();
	//AfxSetResourceHandle(hinstance); 
	m_pAPIMainDlg = new CAPIMainDlg();
	m_pAPIMainDlg->Create(IDD_DIALOG_MAIN, NULL);
	m_pAPIMainDlg->ShowWindow(SW_HIDE);
	m_pAPIMainDlg->SetSaveMsgCallBackFn(SaveMsgCallBackFunc);
	//AfxSetResourceHandle(GetModuleHandle(NULL)); 

	m_pUIThread = new CUIThread(m_pAPIMainDlg, m_hInitDoneEvent);
	m_pUIThread->CreateThread();
	WaitForSingleObject(m_hInitDoneEvent, -1);
	m_bInit = true;
	SetTimer(NULL, TIMER_ID, HEARTBEAT_RATE*1000, TimerProc);
}

PlusinImpl::~PlusinImpl()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if( m_bNotifyThreadRun )
	{
		Logout();
	}
	m_appMsgCallBack = NULL;

	if ( NULL != m_pUIThread )
	{
		m_pUIThread->PostThreadMessage(WM_QUIT, NULL, NULL);
		WaitForSingleObject(m_pUIThread->m_hThread, 2000);
		DWORD dwExitCode = 0;
		if ( GetExitCodeThread(m_pUIThread->m_hThread, &dwExitCode) != 0 &&
			dwExitCode == STILL_ACTIVE )
		{
			TerminateThread(m_pUIThread->m_hThread, 0);
		}
		//Sleep(1000);
		m_pUIThread = NULL;
	}

	if ( NULL != m_pAPIMainDlg )
	{
		m_pAPIMainDlg->DestroyWindow();
		delete m_pAPIMainDlg;
		m_pAPIMainDlg = NULL;
	}

	CloseHandle(m_hInitDoneEvent);

	EnterCriticalSection(&m_cs);
	while( !m_msgQueue.empty() )
	{
		COPYDATASTRUCT apiMsg = m_msgQueue.front();
		m_msgQueue.pop();
		if ( NULL != apiMsg.lpData )
		{
			delete[] apiMsg.lpData;
			apiMsg.lpData = NULL;
		}
	}

	LeaveCriticalSection(&m_cs);

	DeleteCriticalSection(&m_cs);
}

bool PlusinImpl::Logon()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if ( !m_bInit || NULL == m_hTargetWnd || !IsWindow(m_hTargetWnd) || NULL == GetAPIHWnd())
	{
		return false;
	}

	//启动回调线程
	DWORD dwThreadID = 0;
	m_hNotifyThreadHandle = CreateThread(NULL, 65535, 
		(LPTHREAD_START_ROUTINE)NotifyThreadFunc, this, 0, &dwThreadID);

	COPYDATASTRUCT data;
	ZeroMemory( &data, sizeof(COPYDATASTRUCT));
	LogonStu* plogonData = new LogonStu;
	ZeroMemory(plogonData, sizeof(LogonStu));
	strcpy_s( plogonData->szAppID, sizeof(plogonData->szAppID)-1, m_strAppID.c_str() );
	strcpy_s( plogonData->szLicense, sizeof(plogonData->szLicense)-1, m_strLicense.c_str());
	GetModuleFileName( NULL, plogonData->szAppPath, 256 );
	plogonData->hPlusinTopWnd = m_hAPPHWnd;
	plogonData->nPos = m_nPos;

	plogonData->dwVersion = GetDllVersion();
	Send(CMDID_CONSOLE_LOGON_REQUEST, plogonData, sizeof(LogonStu));

	delete plogonData;
	plogonData = NULL;

	return true;
}

bool PlusinImpl::Send(int nCmdId, void* pData, int nDataLen, int nRequestId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if ( !m_bInit || NULL == m_hTargetWnd || !IsWindow(m_hTargetWnd) || NULL == GetAPIHWnd())
	{
		return false;
	}

	if ( nDataLen < 0 )
	{
		return false;
	}

	COPYDATASTRUCT copyData;
	ZeroMemory(&copyData, sizeof(copyData));
	char* pBuffer = new char[sizeof(int)+nDataLen];
	memset(pBuffer, 0, sizeof(int)+nDataLen);
	memcpy(pBuffer, &nRequestId, sizeof(int));
	if ( nDataLen > 0 && pData != NULL )
	{
		memcpy(pBuffer+sizeof(int), pData, nDataLen);
	}
	copyData.cbData = sizeof(int) + nDataLen;
	copyData.lpData = pBuffer;
	copyData.dwData = MAKELONG(nCmdId, 0);
	if ( DEBUG_ON == 1)
	{
		::SendMessage(m_hTargetWnd, WM_COPYDATA, (WPARAM)GetAPIHWnd(), (LPARAM)(&copyData));
	}
	else
	{
		//发送超时时间为10秒
		DWORD dwResult = 0;
		LRESULT lResult = ::SendMessageTimeout(m_hTargetWnd, WM_COPYDATA, (WPARAM)GetAPIHWnd(), 
			(LPARAM)(&copyData), SMTO_NORMAL, 10000, &dwResult);
	}

	delete[] pBuffer;
	pBuffer = NULL;
	return true;
}

void PlusinImpl::Logout()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	//关闭回调线程
	m_bNotifyThreadRun = false;
	if ( NULL != m_hNotifyThreadHandle )
	{
		WaitForSingleObject(m_hNotifyThreadHandle, 2000);
		DWORD dwExitCode = 0;
		if ( GetExitCodeThread(m_hNotifyThreadHandle, &dwExitCode) != 0 &&
			dwExitCode == STILL_ACTIVE )
		{
			TerminateThread(m_hNotifyThreadHandle, 0);
		}

		CloseHandle(m_hNotifyThreadHandle);
	}
}

DWORD PlusinImpl::NotifyThreadFunc( void* pParam )
{
	PlusinImpl* pApi = (PlusinImpl*)pParam;
	if ( NULL == pApi )
	{
		return 0;
	}

	while( pApi->m_bNotifyThreadRun )
	{
		EnterCriticalSection(&pApi->m_cs);
		if ( pApi->m_msgQueue.empty())
		{
			LeaveCriticalSection(&pApi->m_cs);
			Sleep(1);
			continue;
		}
		
		COPYDATASTRUCT apiMsg = pApi->m_msgQueue.front();
		pApi->m_msgQueue.pop();

		LeaveCriticalSection(&pApi->m_cs);

		// 调用回调，上抛数据
		if ( NULL != pApi->m_appMsgCallBack )
		{
			int nCmdId = LOWORD(apiMsg.dwData);
			int nErrorCode = HIWORD(apiMsg.dwData);
			if ( NULL != apiMsg.lpData )
			{
				if ( apiMsg.cbData == sizeof(int))
				{
					pApi->m_appMsgCallBack(nCmdId, nErrorCode, NULL, 0, *(int*)(apiMsg.lpData));
				}
				else if ( apiMsg.cbData > sizeof(int))
				{
					char* pData = (char*)apiMsg.lpData + sizeof(int);
					int nDataLen = apiMsg.cbData - sizeof(int);
					pApi->m_appMsgCallBack(nCmdId, nErrorCode, pData, nDataLen, *(int*)(apiMsg.lpData));
				}
				delete[] apiMsg.lpData;
				apiMsg.lpData = NULL;
			}
		}
	}

	return 0;
}

int PlusinImpl::SaveMsgCallBackFunc( int nCmdId, int nErrorCode, void* pData, int nDataLen )
{
	COPYDATASTRUCT newApiMsg;
	ZeroMemory(&newApiMsg, sizeof(COPYDATASTRUCT));
	newApiMsg.dwData = MAKELONG(nCmdId, nErrorCode);
	newApiMsg.cbData = nDataLen;
	if ( nDataLen == 0 )
	{
		newApiMsg.lpData = NULL;
		newApiMsg.cbData = 0;
	}
	else
	{
		if ( pData == NULL )
		{
			return -1;
		}

		char* pTempData = new char[nDataLen];
		if ( NULL == pTempData )
		{
			return -1;
		}

		memset(pTempData, 0, nDataLen);
		memcpy(pTempData, pData, nDataLen);
		newApiMsg.lpData = pTempData;
	}

	EnterCriticalSection(&m_pInstance->m_cs);
	m_pInstance->m_msgQueue.push(newApiMsg);
	LeaveCriticalSection(&m_pInstance->m_cs);

	return 0;
}

HWND PlusinImpl::GetAPIHWnd()
{
	if ( NULL != m_pAPIMainDlg )
	{
		return m_pAPIMainDlg->m_hWnd;
	}
	else
	{
		return NULL;
	}
}

DWORD PlusinImpl::GetDllVersion()
{
	char szDllPath[MAX_PATH];
	memset(szDllPath, 0, sizeof(szDllPath));
	GetModuleFileName(AfxGetResourceHandle(), szDllPath, sizeof(szDllPath));
	DWORD dwHandle = 0;
	DWORD dwVersionSize = GetFileVersionInfoSize(szDllPath, &dwHandle);
	if ( dwVersionSize <= 0 )
	{
		return 0;
	}

	DWORD dwVersion = 0;
	char* pVersionData = new char[dwVersionSize+1];
	memset(pVersionData, 0, dwVersionSize+1);
	if ( GetFileVersionInfo(szDllPath, dwHandle, dwVersionSize, pVersionData))
	{
		void *pTmp = NULL;
		UINT unLen = 0;
		if ( VerQueryValue(pVersionData, "\\", (LPVOID*)(&pTmp), &unLen)
			&& unLen >= sizeof(DWORD) )
		{
			VS_FIXEDFILEINFO *pInfo = (VS_FIXEDFILEINFO *)pTmp;
			dwVersion = HIWORD(pInfo->dwProductVersionMS)*0x1000000	+LOWORD(pInfo->dwProductVersionMS)*0x10000
				+HIWORD(pInfo->dwProductVersionLS)*0x100		+LOWORD(pInfo->dwProductVersionLS);
		}
	}

	delete[] pVersionData;
	pVersionData = NULL;

	return dwVersion;
}
