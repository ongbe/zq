#pragma once
#include "MsgQueue.h"
class  CRiskMsgQueue:
	public CMsgQueue<SpiData>
{
public:
	CRiskMsgQueue(void){}
	virtual ~CRiskMsgQueue(void){}
	void AddMsg( int CmdID ,int nParam)
	{
		AddMsg(CmdID,sizeof(int),&nParam,NULL,0,true);
	}
	void AddMsg( int CmdID ,int bytelen,void* pParam)
	{
		AddMsg(CmdID,bytelen,pParam,NULL,0,true);
	}
	void AddMsg( int CmdID ,int bytelen,void* pParam,RspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		SpiData data={0};
		data.CmdID=CmdID;
		if(pRspInfo) 
			memcpy(&data.RspInfo,pRspInfo,sizeof(RspInfoField));
		if(pParam)
			memcpy(&data.Main,pParam,bytelen);
		data.nRequestID=nRequestID;
		data.bIsLast=bIsLast;
		EnterCriticalSection(&m_CS);
		m_DataQueue.push(data);
		LeaveCriticalSection(&m_CS);
	}
	static void Init(void)
	{
		if(m_pRiskMsgQueue)
			Release();
		m_pRiskMsgQueue=new CRiskMsgQueue();
	}
	static void Release(void)
	{
		SAFE_DELETE(m_pRiskMsgQueue);
	}
	static CRiskMsgQueue* Get()
	{
		return m_pRiskMsgQueue;
	}
private:
	static CRiskMsgQueue* m_pRiskMsgQueue;
};
__declspec(selectany) CRiskMsgQueue* CRiskMsgQueue::m_pRiskMsgQueue=NULL;
//////////////////////////////////////////////////////////////////////////
class  CRiskMsgQueue_Quot:
	public CMsgQueue<SpiData>
{
public:
	CRiskMsgQueue_Quot(void){}
	virtual ~CRiskMsgQueue_Quot(void){}
	void AddMsg( int CmdID ,int nParam)
	{
		AddMsg(CmdID,sizeof(int),&nParam,NULL,0,true);
	}
	void AddMsg( int CmdID ,int bytelen,void* pParam)
	{
		AddMsg(CmdID,bytelen,pParam,NULL,0,true);
	}
	void AddMsg( int CmdID ,int bytelen,void* pParam,RspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		SpiData data={0};
		data.CmdID=CmdID;
		if(pRspInfo) 
			memcpy(&data.RspInfo,pRspInfo,sizeof(RspInfoField));
		if(pParam)
			memcpy(&data.Main,pParam,bytelen);
		data.nRequestID=nRequestID;
		data.bIsLast=bIsLast;
		EnterCriticalSection(&m_CS);
		m_DataQueue.push(data);
		LeaveCriticalSection(&m_CS);
	}
	static void Init(void)
	{
		if(m_pRiskMsgQueue_Quot)
			Release();
		m_pRiskMsgQueue_Quot=new CRiskMsgQueue_Quot();
	}
	static void Release(void)
	{
		SAFE_DELETE(m_pRiskMsgQueue_Quot);
	}
	static CRiskMsgQueue_Quot* Get()
	{
		return m_pRiskMsgQueue_Quot;
	}
private:
	static CRiskMsgQueue_Quot* m_pRiskMsgQueue_Quot;
};
__declspec(selectany) CRiskMsgQueue_Quot* CRiskMsgQueue_Quot::m_pRiskMsgQueue_Quot=NULL;
//////////////////////////////////////////////////////////////////////////
class  CRiskMsgQueue_Sync:
	public CMsgQueue<SpiData>
{
public:
	CRiskMsgQueue_Sync(void){}
	virtual ~CRiskMsgQueue_Sync(void){}
	void AddMsg( int CmdID ,int nParam)
	{
		AddMsg(CmdID,sizeof(int),&nParam,NULL,0,true);
	}
	void AddMsg( int CmdID ,int bytelen,void* pParam)
	{
		AddMsg(CmdID,bytelen,pParam,NULL,0,true);
	}
	void AddMsg( int CmdID ,int bytelen,void* pParam,RspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		SpiData data={0};
		data.CmdID=CmdID;
		if(pRspInfo) 
			memcpy(&data.RspInfo,pRspInfo,sizeof(RspInfoField));
		if(pParam)
			memcpy(&data.Main,pParam,bytelen);
		data.nRequestID=nRequestID;
		data.bIsLast=bIsLast;
		EnterCriticalSection(&m_CS);
		m_DataQueue.push(data);
		LeaveCriticalSection(&m_CS);
	}
	static void Init(void)
	{
		if(m_pRiskMsgQueue_Sync)
			Release();
		m_pRiskMsgQueue_Sync=new CRiskMsgQueue_Sync();
	}
	static void Release(void)
	{
		SAFE_DELETE(m_pRiskMsgQueue_Sync);
	}
	static CRiskMsgQueue_Sync* Get()
	{
		return m_pRiskMsgQueue_Sync;
	}
private:
	static CRiskMsgQueue_Sync* m_pRiskMsgQueue_Sync;
};
__declspec(selectany) CRiskMsgQueue_Sync* CRiskMsgQueue_Sync::m_pRiskMsgQueue_Sync=NULL;

/////////////////////////////////////////////////////////////////////
class  CRisk2ClientMsgQueue:
	public CMsgQueue<Risk2ClientData>
{
public:
	CRisk2ClientMsgQueue(void){}
	virtual ~CRisk2ClientMsgQueue(void){}
	static void Init(void)
	{
		if(m_pRisk2ClientMsgQueue)
			Release();
		m_pRisk2ClientMsgQueue=new CRisk2ClientMsgQueue();
	}
	static void Release(void)
	{
		SAFE_DELETE(m_pRisk2ClientMsgQueue);
	}
	static CRisk2ClientMsgQueue* Get()
	{
		return m_pRisk2ClientMsgQueue;
	}
private:
	static CRisk2ClientMsgQueue* m_pRisk2ClientMsgQueue;
};
__declspec(selectany) CRisk2ClientMsgQueue* CRisk2ClientMsgQueue::m_pRisk2ClientMsgQueue=NULL;
/////////////////////////////////////////////////////////////////////
class  CClient2RiskMsgQueue:
	public CMsgQueue<Client2RiskData>
{
public:
	CClient2RiskMsgQueue(void){}
	virtual ~CClient2RiskMsgQueue(void){}
	static void Init(void)
	{
		if(m_pClient2RiskMsgQueue)
			Release();
		m_pClient2RiskMsgQueue=new CClient2RiskMsgQueue();
	}
	static void Release(void)
	{
		SAFE_DELETE(m_pClient2RiskMsgQueue);
	}
	static CClient2RiskMsgQueue* Get()
	{
		return m_pClient2RiskMsgQueue;
	}
private:
	static CClient2RiskMsgQueue* m_pClient2RiskMsgQueue;
};
__declspec(selectany) CClient2RiskMsgQueue* CClient2RiskMsgQueue::m_pClient2RiskMsgQueue=NULL;

//////////////////////////////////////////////////////////////////////////
//强平所用消息结构
class  CForceCloseMsgQueue:
	public CMsgQueue<std::string>
{
public:
	CForceCloseMsgQueue(void){}
	virtual ~CForceCloseMsgQueue(void){}
	static void Init(void)
	{
		if(m_pForceCloseMsgQueue)
			Release();
		m_pForceCloseMsgQueue=new CForceCloseMsgQueue();
	}
	static void Release(void)
	{
		SAFE_DELETE(m_pForceCloseMsgQueue);
	}
	static CForceCloseMsgQueue* Get()
	{
		return m_pForceCloseMsgQueue;
	}

	void AddForceCloseMsg(std::string  strID)
	{
		m_Mutex_SetID.read_lock();
		std::set<std::string>::iterator it = m_SetID.find(strID);
		if(it != m_SetID.end())
		{	
			m_Mutex_SetID.read_unlock();
			return;	
		}
		m_SetID.insert(strID);
		m_Mutex_SetID.read_unlock();
		AddMsg(strID);
	}
	bool GetForceCloseMsg(std::string& strID)
	{
		bool bGet = GetMsg(strID);
		m_Mutex_SetID.read_lock();
		std::set<std::string>::iterator it = m_SetID.find(strID);
		if(it != m_SetID.end())
			m_SetID.erase(strID);

		m_Mutex_SetID.read_unlock();
		return bGet;
	}
private:
	static CForceCloseMsgQueue* m_pForceCloseMsgQueue;
	std::set<std::string>   m_SetID;
	CReadWriteLock			m_Mutex_SetID;

};
__declspec(selectany) CForceCloseMsgQueue* CForceCloseMsgQueue::m_pForceCloseMsgQueue=NULL;