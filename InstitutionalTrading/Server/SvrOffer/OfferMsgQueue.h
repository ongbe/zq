#pragma once
#include "MsgQueue.h"
struct STransferMsg
{
	SOrderTransfer sOrderTransfer;		
	int nProcess;//1:Îªadd£»0Îªupdate
	STransferMsg()
	{
		memset(this, 0 , sizeof(STransferMsg));
	}
	bool operator<(const STransferMsg item) const
	{ 
		if(sOrderTransfer < item.sOrderTransfer)
			return true;
	
		if(nProcess < item.nProcess)
			return true;
		else if(nProcess > item.nProcess)
			return false;
	
		return false;
	}
};
class  CSTransferMsgQueue:
	public CMsgQueue<STransferMsg>
{
public:
	CSTransferMsgQueue(void){}
	virtual ~CSTransferMsgQueue(void){}
	static void Init(void)
	{
		if(m_pTransferMsgQueue)
			Release();
		m_pTransferMsgQueue=new CSTransferMsgQueue();
	}
	static void Release(void)
	{
		SAFE_DELETE(m_pTransferMsgQueue);
	}
	static CSTransferMsgQueue* Get()
	{
		return m_pTransferMsgQueue;
	}

	void AddTransferMsg(SOrderTransfer sOrderTransfer, int nProcess)
	{
	//	m_Mutex_SetTransferMsg.read_lock();
		STransferMsg sTransferMsg;
		sTransferMsg.sOrderTransfer = sOrderTransfer;
		sTransferMsg.nProcess		= nProcess;
	/*	std::set<STransferMsg>::iterator it = m_SetTransferMsg.find(sTransferMsg);
		if(it != m_SetTransferMsg.end())
		{	
			m_Mutex_SetTransferMsg.read_unlock();
			return;	
		}
		m_SetTransferMsg.insert(sTransferMsg);
		m_Mutex_SetTransferMsg.read_unlock();*/
		AddMsg(sTransferMsg);
	}
	bool GetTransferMsg(STransferMsg& sTransferMsg)
	{
		bool bGet = GetMsg(sTransferMsg);
	/*	m_Mutex_SetTransferMsg.read_lock();
		std::set<STransferMsg>::iterator it = m_SetTransferMsg.find(sTransferMsg);
		if(it != m_SetTransferMsg.end())
			m_SetTransferMsg.erase(sTransferMsg);

		m_Mutex_SetTransferMsg.read_unlock();*/
		return bGet;
	}

	
	
private:
	static CSTransferMsgQueue*	m_pTransferMsgQueue;
//	std::set<STransferMsg>		m_SetTransferMsg;
//	CReadWriteLock				m_Mutex_SetTransferMsg;


};
__declspec(selectany) CSTransferMsgQueue* CSTransferMsgQueue::m_pTransferMsgQueue = NULL;