
#pragma once
#include "rwlock.h"
class CSafeRwLockRead
{
public:

	CSafeRwLockRead(rwlock_t& _rwlock):m_lock(_rwlock)
	{
		uv__rwlock_fallback_rdlock(&m_lock);
	}
	~CSafeRwLockRead()
	{
		uv__rwlock_fallback_rdunlock(&m_lock);
	}

protected:
	rwlock_t&    m_lock;
};


class CSafeRwLockWrite
{
public:

	CSafeRwLockWrite(rwlock_t& _rwlock):m_lock(_rwlock)
	{
		uv__rwlock_fallback_wrlock(&m_lock);
	}
	~CSafeRwLockWrite()
	{
		uv__rwlock_fallback_wrunlock(&m_lock);
	}

protected:
	rwlock_t&    m_lock;
};