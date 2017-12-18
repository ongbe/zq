/**
* @file   :MutexLock.h
* @brief  :Windows平台临界代码段锁
* @author :王宏飛<hongfei.wang@cithinc.com>
* @version:0.1
* @history:
* 1. created on 2010-01-24
*/

#pragma	once
#include <windows.h>


class MutexLock
{
public:
	MutexLock()
	{
		::InitializeCriticalSection(&m_CS);
	}

	void Lock()
	{
		::EnterCriticalSection(&m_CS);
	}

	void Unlock()
	{
		::LeaveCriticalSection(&m_CS);
	}

	~MutexLock()
	{
		::DeleteCriticalSection(&m_CS);
	}
	
protected:
	CRITICAL_SECTION	m_CS;
};


class MutexLockGurad
{
public:
	MutexLockGurad(MutexLock& lock):m_pLock(&lock)
	{
		m_pLock->Lock();
	}

	~MutexLockGurad()
	{
		m_pLock->Unlock();
	}

protected:
	MutexLock *m_pLock;
};

class MutexLockGuradEx
{
public:
	MutexLockGuradEx(MutexLock* lock):m_pLock(lock)
	{
		if(m_pLock)	m_pLock->Lock();
	}

	~MutexLockGuradEx()
	{
		if(m_pLock)	m_pLock->Unlock();
	}

protected:
	MutexLock *m_pLock;
};


#define	MUTEX_GURAD(MUTEXLOCK)		MutexLockGurad gurad(MUTEXLOCK);		