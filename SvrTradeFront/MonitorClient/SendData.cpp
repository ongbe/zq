#include "stdafx.h"
#include "SendData.h"


CSendData* CSendData::m_SendData=NULL;
CSendData::CSendData(void)
{
	InitializeCriticalSection(&m_CS);
}

CSendData::~CSendData(void)
{
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
void CSendData::Init(void)
{
	if(m_SendData)
	{
		Release();
	}
	m_SendData=new CSendData();
}
void CSendData::Release(void)
{
	if(m_SendData)
	{
		delete m_SendData;
		m_SendData = NULL;
	}
}
CSendData* CSendData::Get()
{
	return m_SendData;
}

void CSendData::AddMsg( const RecvData& data )
{
	EnterCriticalSection(&m_CS);
	m_cmdMsgQueue.AddMsg(data);
	LeaveCriticalSection(&m_CS);
}

bool CSendData::GetMsg( RecvData& data )
{
	bool bRet = false;
	EnterCriticalSection(&m_CS);
	bRet = m_cmdMsgQueue.GetMsg(data);
	LeaveCriticalSection(&m_CS);
	return bRet;
}
