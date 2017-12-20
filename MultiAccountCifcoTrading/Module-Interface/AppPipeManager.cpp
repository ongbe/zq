// AppPipeManager.cpp: implementation of the CAppPipeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppPipeManager.h"


#define APPPIPE_TIMERID 753443234
CAppPipeManager* g_poAppPipeManager = NULL;


//定时器回调
void CALLBACK TimerProc_AppPipeManager(HWND hWnd, UINT uMsg, UINT_PTR,DWORD)
{
	if(g_poAppPipeManager!=NULL)// && uMsg == APPPIPE_TIMERID)
		g_poAppPipeManager->OnTimer();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppPipeManager::CAppPipeManager()
{
//    InitializeCriticalSection(&m_CS_EvtCallback);
	m_dwFirstSessionID = GetTickCount();
	
	// 创建目录
	std::string strAppPath;
	GlobalFunc::GetPath(strAppPath);
	strAppPath+="NamedPipe\\";
	if(!wxDirExists(strAppPath.c_str())) 
		wxMkdir(strAppPath);

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	char strFileName[1024];
	ZeroMemory(strFileName, 1024);

	sprintf(strFileName, "NamedPipe\\Service%04d%02d%02d.log", sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	m_pWriteLog = new zqWriteLog(LOGMODE_LOCALFILE, strFileName);
}

CAppPipeManager::~CAppPipeManager()
{
//    DeleteCriticalSection(&m_CS_EvtCallback);
	KillTimer(NULL, APPPIPE_TIMERID);
	g_poAppPipeManager = NULL;
}

BOOL CAppPipeManager::Init()
{
	SetTimer(NULL, APPPIPE_TIMERID, 1000, TimerProc_AppPipeManager); 
	g_poAppPipeManager = this;
	return TRUE;
}

BOOL CAppPipeManager::GetNextSessionID(ISessionIDType& NextSessionID)
{
	++m_dwFirstSessionID;
	NextSessionID = m_dwFirstSessionID;
	return TRUE;
}

int CAppPipeManager::GetAllSessionID(vector<ISessionIDType>& arrOut)
{
	for(MAPSESSIONITEM it = m_mapSession.begin(); 
			it != m_mapSession.end(); it++) {
		arrOut.push_back(it->first);
	}
	return m_mapSession.size();
}

BOOL CAppPipeManager::FindSession(ISessionIDType& SessionID)
{
	return m_mapSession.find(SessionID)!=m_mapSession.end();
}

BOOL CAppPipeManager::GetSessionConnectAppPipe(ISessionIDType& SessionID, string& strConnectAppPipe)
{
	MAPSESSIONITEM it = m_mapSession.find(SessionID);
	if(it != m_mapSession.end()) {
		strConnectAppPipe = it->second.strConnectAppName;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CAppPipeManager::FilterMessageType(ISessionIDType& SessionID, DWORD MsgType)
{
	if(MsgType == IMSGTYPE_A || MsgType == IMSGTYPE_B
			|| MsgType == IMSGTYPE_C || MsgType == IMSGTYPE_D) 
		return FALSE;

	MAPSESSIONITEM it = m_mapSession.find(SessionID);
	if(it != m_mapSession.end()) {
		if(it->second.tLoginData.chUseQuot=='0' && 
				(MsgType == IMSGTYPE_E || MsgType == IMSGTYPE_F)) {
			return TRUE;
		}
		else if(it->second.tLoginData.chUseOrder=='0' && 
				(MsgType == IMSGTYPE_G || MsgType == IMSGTYPE_H)) {
			return TRUE;
		}
		return FALSE;
	}
	else {
		return FALSE;
	}
}

BOOL CAppPipeManager::HeartBeatSession(ISessionIDType& SessionID)
{
	MAPSESSIONITEM it = m_mapSession.find(SessionID);
	if(it != m_mapSession.end()) {
		it->second.dwRemainTime = IMAXREMAINTIME;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CAppPipeManager::RegisterSession(ISessionIDType& SessionID, ISESSION& tSession)
{
	MAPSESSIONITEM it = m_mapSession.find(SessionID);
	if(it == m_mapSession.end()) {
		m_mapSession[SessionID] = tSession;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

BOOL CAppPipeManager::UnregisterSession(ISessionIDType& SessionID)
{
	MAPSESSIONITEM it = m_mapSession.find(SessionID);
	if(it != m_mapSession.end()) {
		m_mapSession.erase(SessionID);
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void CAppPipeManager::OnTimer()
{
	MAPSESSIONITEM it;
	vector<ISessionIDType> arrDel;

	int n=0;
	TRACE("\n"); 
	LockObject();
	for(it = m_mapSession.begin(); it != m_mapSession.end(); it++) {
		n++;
		if(it->second.dwRemainTime==0) {
			arrDel.push_back(it->first);
		}
		else {
			it->second.dwRemainTime--;
			TRACE("%02d, SessionID:=%x, RemainTime:=%u\n", 
					n, it->second.SessionID, it->second.dwRemainTime); 
		}
	}
	for(int i=0; i<(int)arrDel.size(); i++) {
		m_mapSession.erase(arrDel[i]);
		USERLOG_INFO("CAppPipeManager::OnTimer()\r\n客户端SessionId[%x]超时被注销，已注册数[%d]", arrDel[i], n-i-1);
	}
	UnlockObject();
	arrDel.clear();
}
