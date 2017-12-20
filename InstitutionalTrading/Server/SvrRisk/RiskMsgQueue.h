#pragma once
#include "MsgQueue.h"/*
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
__declspec(selectany) CClient2RiskMsgQueue* CClient2RiskMsgQueue::m_pClient2RiskMsgQueue=NULL;*/

//////////////////////////////////////////////////////////////////////////
//强平所用消息结构
struct SForceClose
{
	int  nRiskEventID;			//风险事件ID
	std::string strInvestID;    //某一个投资者ID
	std::string strInstrument;  //具体某个合约，如果为空，则意味着全平
	int 	nRiskIndicatorID;	//触发的风险指标类型
	int	    nRiskLevelID;		//触发的风险等级

	bool			bIsSigleHoldTime; //是不是单笔持仓的风险
	PositionDetailKey  PostionDetail; //如果改正不为NULL。则代表单笔持仓时间风险
	SForceClose()
	{
		memset(this, 0 , sizeof(SForceClose));
		bIsSigleHoldTime = false;
	}
	bool operator<(const SForceClose item) const
	{ 
		if(nRiskEventID < item.nRiskEventID)
			return true;
		else if(nRiskEventID > item.nRiskEventID)
			return false;

		int i = strcmp(strInvestID.c_str(), item.strInvestID.c_str()) ;			
		if(i>0)
			return true;
		else if(i<0)
			return false;

		i = strcmp(strInstrument.c_str(), item.strInstrument.c_str());			
		if(i>0)
			return true;
		else if(i<0)
			return false;
		
		if(nRiskIndicatorID < item.nRiskIndicatorID)
			return true;
		else if(nRiskIndicatorID > item.nRiskIndicatorID)
			return false;

		if(nRiskLevelID < item.nRiskLevelID)
			return true;
		else if(nRiskLevelID > item.nRiskLevelID)
			return false;

		if(bIsSigleHoldTime < item.bIsSigleHoldTime)
			return true;
		else if(bIsSigleHoldTime > item.bIsSigleHoldTime)
			return false;

		if(PostionDetail < item.PostionDetail)
			return true;		
		
		return false;
	}
};
class  CForceCloseMsgQueue:
	public CMsgQueue<SForceClose>
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

	void AddForceCloseMsg(int nRiskEventID, std::string  strID, std::string strInstrument, PositionDetailKey* PostionDetail, int nRiskIndicatorID, int	  nRiskLevelID)
	{
		m_Mutex_SetID.read_lock();
		SForceClose sForceClose;
		sForceClose.strInvestID			= strID;
		sForceClose.strInstrument		= strInstrument;
		PositionDetailKey Detail;
		if(PostionDetail)
		{
			sForceClose.bIsSigleHoldTime = true;
			Detail = *PostionDetail;
		}
		sForceClose.PostionDetail  = Detail;
		//判断是否重复不需要这几个值
		sForceClose.nRiskEventID		= 0;		
		sForceClose.nRiskIndicatorID	= 0;
		sForceClose.nRiskLevelID		= 0;
		std::set<SForceClose>::iterator it = m_SetID.find(sForceClose);
		if(it != m_SetID.end())
		{	
			m_Mutex_SetID.read_unlock();
			return;	
		}
		sForceClose.nRiskEventID		= nRiskEventID;
		sForceClose.nRiskIndicatorID	= nRiskIndicatorID;
		sForceClose.nRiskLevelID		= nRiskLevelID;
		m_SetID.insert(sForceClose);
		m_Mutex_SetID.read_unlock();
		AddMsg(sForceClose);
	}
	bool GetForceCloseMsg(SForceClose& sForceClose)
	{
		bool bGet = GetMsg(sForceClose);
		m_Mutex_SetID.read_lock();
		std::set<SForceClose>::iterator it = m_SetID.find(sForceClose);
		if(it != m_SetID.end())
			m_SetID.erase(sForceClose);

		m_Mutex_SetID.read_unlock();
		return bGet;
	}

	bool SetForceCloseCount(SForceClose& sForceClose)
	{
		int nCount = 0;
		m_Mutex_SForceClose2Count.write_lock();
		std::map<SForceClose, int>::iterator it = m_SForceClose2Count.find(sForceClose);
		if(it != m_SForceClose2Count.end())
		{
			nCount = it->second;
			m_SForceClose2Count[sForceClose] = ++nCount;
		}
		else
			m_SForceClose2Count[sForceClose] = ++nCount;
		m_Mutex_SForceClose2Count.write_unlock();

		if(nCount >2)
			return false;

		return true;
	}
	void DeleteForceCloseCount(SForceClose& sForceClose)
	{
		m_Mutex_SForceClose2Count.write_lock();
		std::map<SForceClose, int>::iterator it = m_SForceClose2Count.find(sForceClose);
		if(it != m_SForceClose2Count.end())
		{
			m_SForceClose2Count.erase(it);
		}
		m_Mutex_SForceClose2Count.write_unlock();
	}
private:
	static CForceCloseMsgQueue* m_pForceCloseMsgQueue;
	std::set<SForceClose>   m_SetID;
	CReadWriteLock			m_Mutex_SetID;

	std::map<SForceClose, int> m_SForceClose2Count;//强平3次后不再强平操作
	CReadWriteLock			m_Mutex_SForceClose2Count;

};
__declspec(selectany) CForceCloseMsgQueue* CForceCloseMsgQueue::m_pForceCloseMsgQueue=NULL;