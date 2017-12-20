// AppPipeManager.h: interface for the CAppPipeManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPPIPEMANAGER_H__F4D72367_BFB0_4A38_BE19_AF997CA914BB__INCLUDED_)
#define AFX_APPPIPEMANAGER_H__F4D72367_BFB0_4A38_BE19_AF997CA914BB__INCLUDED_


#include "interfaceDefines.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAppPipeManager  
{
public:
	typedef map<ISessionIDType, ISESSION>	MAPSESSION;
	typedef MAPSESSION::iterator	MAPSESSIONITEM;

protected:
	DWORD		m_dwFirstSessionID;
	MAPSESSION	m_mapSession;
	wxMutex		m_oMutex;
    //CRITICAL_SECTION	m_CS_EvtCallback;
	zqWriteLog*	m_pWriteLog;

public:
	DWORD GetSessionCount() {
		return m_mapSession.size();
	};
	BOOL GetNextSessionID(ISessionIDType& NextSessionID);
	int GetAllSessionID(vector<ISessionIDType>& arrOut);
	BOOL GetSessionConnectAppPipe(ISessionIDType& SessionID, string& strConnectAppPipe);
	BOOL FindSession(ISessionIDType& SessionID);
	BOOL FilterMessageType(ISessionIDType& SessionID, DWORD MsgType);
	BOOL RegisterSession(ISessionIDType& SessionID, ISESSION& tSession);
	BOOL UnregisterSession(ISessionIDType& SessionID);
	BOOL HeartBeatSession(ISessionIDType& SessionID);

	// LockObject()与UnloadObject()这两个函数必须成对调用
	void LockObject() {
		m_oMutex.Lock();
		//EnterCriticalSection(&m_CS_EvtCallback);
	};
	void UnlockObject() {
		m_oMutex.Unlock();
		//LeaveCriticalSection(&m_CS_EvtCallback);
	};

public:
	CAppPipeManager();
	virtual ~CAppPipeManager();

	BOOL Init();
	void OnTimer();

};

#endif // !defined(AFX_APPPIPEMANAGER_H__F4D72367_BFB0_4A38_BE19_AF997CA914BB__INCLUDED_)
