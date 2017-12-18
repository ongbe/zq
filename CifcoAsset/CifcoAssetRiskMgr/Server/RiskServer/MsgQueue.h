#pragma once

template<class T>
class CMsgQueue
{
public:
	CMsgQueue(void)
	{
		InitializeCriticalSection(&m_CS);
	}

	virtual ~CMsgQueue(void)
	{
		DeleteCriticalSection(&m_CS);
	}
	void AddMsg(const T& pParam)
	{
		EnterCriticalSection(&m_CS);
		m_DataQueue.push(pParam);
		LeaveCriticalSection(&m_CS);
	}
	bool GetMsg(T& pParam)
	{
		bool bRet=false;
		EnterCriticalSection(&m_CS);
		int count=m_DataQueue.size();		
		if(count>0)
		{			
			T& data=m_DataQueue.front();
			memcpy(&pParam,&data,sizeof(T));
			m_DataQueue.pop();			
			bRet=true;
		}
		LeaveCriticalSection(&m_CS);
		return bRet;
	}
protected:
	std::queue<T> m_DataQueue;
	CRITICAL_SECTION m_CS;
private:
	DISALLOW_COPY_AND_ASSIGN(CMsgQueue);
};
