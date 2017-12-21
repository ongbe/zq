#pragma once
#include "stdafx.h"
#include "SocketBase.h"
#include "ReadWriteLock.h"
#include <map>

class CClientData
{
public:
	CClientData(void);
	~CClientData(void);
	static void Init(void);
	static void Release(void);
	static CClientData* Get();
	static CClientData* m_DataManager;

	void AddMsg( const RecvData& data);
	bool GetMsg( RecvData& data);

private:
	CCmdMsgQueue m_cmdMsgQueue;
	CRITICAL_SECTION m_CS;
	
	CToolThread*	m_hWorkerThread;
	static DWORD WINAPI ThreadFunc(void* pData);
	void Process();
public:
	

};
