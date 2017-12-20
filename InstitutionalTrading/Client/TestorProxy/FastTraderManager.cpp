#include "StdAfx.h"
#include "FastTraderManager.h"

using namespace std;

CFastTraderManager::CFastTraderManager(void)
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	for(int i = 1; i <= conMaxFolderNum; i++)
		m_queueIdleFloder.push(i);
}

CFastTraderManager::~CFastTraderManager(void)
{
	LockObject();
	Clear();
	UnlockObject();
	CloseHandle(m_hMutex);
	m_hMutex = NULL;
}

BOOL CFastTraderManager::LockObject()
{
	::WaitForSingleObject(m_hMutex, INFINITE);
	return TRUE;
}

BOOL CFastTraderManager::UnlockObject()
{
	::ReleaseMutex(m_hMutex);
	return TRUE;
}

BOOL CFastTraderManager::LoadFastTrader(int nFloderNum, HWND hMainWnd, CString& strUserName, 
										CString& strPassword, HANDLE& hProc, DWORD& dwProcID)
{
	CString strExe;
	BOOL bRet = FALSE;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	memset(&pi,0,sizeof(pi));
	memset(&si,0,sizeof(si));

	si.cb=sizeof(si);
	si.wShowWindow=SW_SHOW;
	si.dwFlags=STARTF_USESHOWWINDOW;
	strExe.Format("%d\\FastTrader.exe ConsoleStart %u %s %s", nFloderNum, hMainWnd, strUserName, strPassword);
	bRet = CreateProcess(NULL, strExe.GetBuffer(0), NULL, FALSE, NULL, 0, NULL, NULL, &si, &pi);
	dwProcID = pi.dwProcessId;
	hProc = pi.hProcess;

	return bRet;
}

BOOL CFastTraderManager::UnloadFastTrader(HWND hFTHwnd, HANDLE hProc)
{
	if(hFTHwnd!=NULL) {
		//::PostMessage(hFTHwnd, WM_QUIT, 0, NULL);
		::PostMessage(hFTHwnd, WM_CLOSE, 0, NULL);
	}
	else {
		DWORD nExitCode = 0;
		::TerminateProcess(hProc, nExitCode);
	}
	return TRUE;
}

DWORD CFastTraderManager::GetProcID(int nSeqNum)
{
	DWORD dwRet = -1;

	LockObject();

	map<DWORD, int>::iterator it = m_mapProcID2SeqNum.find(nSeqNum);
	if(it!=m_mapProcID2SeqNum.end())
		dwRet = it->second;
	
	UnlockObject();

	return dwRet;
}

int CFastTraderManager::GetSeqNum(DWORD dwProcID)
{
	int nRet = -1;

	LockObject();

	map<DWORD, int>::iterator it = m_mapProcID2SeqNum.find(dwProcID);
	if(it!=m_mapProcID2SeqNum.end())
		nRet = it->second;
	
	UnlockObject();

	return nRet;
}

BOOL CFastTraderManager::RegisterFastTrader(int nSeqNum, HWND hMainWnd, 
				CString& strUserName, CString& strPassword, DWORD& dwProcID, int& nFolderNum)
{
	
	LockObject();

	map<int, PROCINFO>::iterator it = m_mapProc.find(nSeqNum);
	if(it!=m_mapProc.end()) {
		UnlockObject();
		return FALSE;
	}

	if(m_queueIdleFloder.empty()) {
		UnlockObject();
		return FALSE;
	}

	nFolderNum = m_queueIdleFloder.front();
	m_queueIdleFloder.pop();

	HANDLE hProc = NULL;

	if(!LoadFastTrader(nFolderNum, hMainWnd, strUserName, strPassword, hProc, dwProcID)) {
		m_queueIdleFloder.push(nFolderNum);
		UnlockObject();
		return FALSE;
	}

	PROCINFO info = {0};

	info.nSeqNum = nSeqNum;
	info.nProcID = dwProcID;
	info.hProc = hProc;
	info.nFloderNum = nFolderNum;
	info.hFastTraderHwnd = 0;
	
	m_mapProc.insert(make_pair(nSeqNum, info));
	m_mapProcID2SeqNum.insert(make_pair(dwProcID, nSeqNum));

	UnlockObject();

	return TRUE;
}

BOOL CFastTraderManager::UnregisterFastTrader(int nSeqNum)
{
	
	LockObject();

	map<int, PROCINFO>::iterator it = m_mapProc.find(nSeqNum);
	if(it==m_mapProc.end()) {
		UnlockObject();
		return FALSE;
	}

	UnloadFastTrader(it->second.hFastTraderHwnd, it->second.hProc);
	m_queueIdleFloder.push(it->second.nFloderNum);

	m_mapProcID2SeqNum.erase(it->second.nProcID);
	m_mapProc.erase(nSeqNum);

	UnlockObject();

	return TRUE;
}

void CFastTraderManager::Clear()
{
	map<int, PROCINFO>::iterator it;
	LockObject();
	map<int, PROCINFO> mapProc = m_mapProc;
	UnlockObject();

	for(it = mapProc.begin(); it != mapProc.end(); it++) {
		UnregisterFastTrader(it->first);
	}

	LockObject();
	m_mapProc.clear();
	UnlockObject();
}

BOOL CFastTraderManager::ReseHwnd(DWORD dwProcID, HWND hWnd)
{
	int nSeqNum = GetSeqNum(dwProcID);

	LockObject();

	map<int, PROCINFO>::iterator it = m_mapProc.find(nSeqNum);
	if(it!=m_mapProc.end()) {
		it->second.hFastTraderHwnd = hWnd;
	}

	UnlockObject();

	return TRUE;
}

BOOL CFastTraderManager::DoCommand(int nSeqNum, CMD_INPUTSIMPLE& input)
{
	HWND hWnd = NULL;

	LockObject();

	map<int, PROCINFO>::iterator it = m_mapProc.find(nSeqNum);
	if(it!=m_mapProc.end()) {
		hWnd = it->second.hFastTraderHwnd;
	}

	UnlockObject();

	if(hWnd==NULL)
		return FALSE;

	TRACE("DoCommand %d, %c\n", input.nInputType, input.OpenClose);

	LRESULT lRet = 0;
	COPYDATASTRUCT cpyData = {0};

	ZeroMemory(&cpyData, sizeof(cpyData)); 
	cpyData.dwData = CONSOLE_CMD_INPUTSIMPLE;	//返回交易客户端的主窗口句柄
	cpyData.cbData = sizeof(input);
	cpyData.lpData = (void*)&input;
	lRet = ::SendMessage(hWnd, WM_COPYDATA, CONSOLE_MAGIC_NUMBER, (LPARAM)&cpyData);
	return lRet==0;
}
