
#pragma once
#include "easymutex.h"
class CSafeLock
{
public:

	CSafeLock(Ceasymutex& _lock):m_lock(_lock)
	{
		m_lock.lock();
	}
	~CSafeLock()
	{
		m_lock.unlock();
	}

protected:
	Ceasymutex&    m_lock;

};

