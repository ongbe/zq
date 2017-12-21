#include "stdafx.h"

#include "ClientData.h"
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p){delete(p);  (p)=NULL;} }
#endif
CClientData* CClientData::m_DataManager=NULL;
CClientData::CClientData(void)
{
	m_hWorkerThread =new CToolThread(ThreadFunc, this);
	InitializeCriticalSection(&m_CS);
}

CClientData::~CClientData(void)
{
	if(m_hWorkerThread)
	{
		delete m_hWorkerThread;
		m_hWorkerThread = NULL;
	}	

	RecvData reqData;
	while( m_cmdMsgQueue.GetMsg(reqData) )
	{
		if ( NULL != reqData.pData )
		{
			delete[] reqData.pData;
		}
	}
	DeleteCriticalSection(&m_CS);
}
void CClientData::Init(void)
{
	if(m_DataManager)
	{
		Release();
	}
	m_DataManager=new CClientData();
}
void CClientData::Release(void)
{
	SAFE_DELETE(m_DataManager);
}
CClientData* CClientData::Get()
{
	return m_DataManager;
}
void CClientData::AddMsg( const RecvData& data )
{
	EnterCriticalSection(&m_CS);
	m_cmdMsgQueue.AddMsg(data);
	LeaveCriticalSection(&m_CS);
}

bool CClientData::GetMsg( RecvData& data )
{
	bool bRet = false;
	EnterCriticalSection(&m_CS);
	bRet = m_cmdMsgQueue.GetMsg(data);
	LeaveCriticalSection(&m_CS);
	return bRet;
}

DWORD CClientData::ThreadFunc( void* pData )
{
	CClientData* pWorker = (CClientData*)pData;
	if(pWorker)
		pWorker->Process();
	return 0;
}

void CClientData::Process()
{
	if (NULL==m_hWorkerThread)
	{
		return;
	}
	while(!m_hWorkerThread->IsNeedExit())
	{
		RecvData reqData;
		if (GetMsg(reqData))
		{//从交易端过来数据，转送客户端数据
			//这里还要trace输出.....
			if(reqData.pData)
			{
				delete []reqData.pData;
				reqData.pData = NULL;
			}
		}
		else
		{
			//空闲时自动执行动作
			Sleep(10);
			continue;
		}
	}
}