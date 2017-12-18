/*
 * easymutex.cpp
 *
 *  Created on: Dec 1, 2011
 *      Author: root
 */

#ifdef WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#pragma warning(disable : 4793)
#include <windows.h>
#endif

#include "easymutex.h"

#ifdef WIN32
#pragma unmanaged
#endif 

Ceasymutex::Ceasymutex()
{
#ifndef WIN32
	pthread_mutex_init(&m_mutex,NULL);
#else
	m_pcs=new CRITICAL_SECTION;
	InitializeCriticalSection((CRITICAL_SECTION*)m_pcs);
#endif
}

Ceasymutex::Ceasymutex(const Ceasymutex& r)
{
    *this=r;
}

Ceasymutex& Ceasymutex::operator= (const Ceasymutex&)
{
#ifndef WIN32
	pthread_mutex_init(&m_mutex,NULL);
#else
	m_pcs=new CRITICAL_SECTION;
	InitializeCriticalSection((CRITICAL_SECTION*)m_pcs);
#endif

    return *this;
}

Ceasymutex::~Ceasymutex()
{
#ifndef WIN32
	pthread_mutex_destroy(&m_mutex);
#else
	DeleteCriticalSection((CRITICAL_SECTION*)m_pcs);
	delete (CRITICAL_SECTION*)m_pcs;
    m_pcs=NULL;
#endif
}
void Ceasymutex::lock(void) const
{
#ifndef WIN32
	pthread_mutex_lock(&m_mutex);
#else
	EnterCriticalSection((CRITICAL_SECTION*)m_pcs);
#endif
}

void Ceasymutex::unlock(void) const
{
#ifndef WIN32
	pthread_mutex_unlock(&m_mutex);
#else
	LeaveCriticalSection((CRITICAL_SECTION*)m_pcs);
#endif
}

bool Ceasymutex::trylock(void) const
{
#ifndef WIN32
	return pthread_mutex_trylock(&m_mutex)==0 ? true : false;
#else
	return TryEnterCriticalSection((CRITICAL_SECTION*)m_pcs)==TRUE ? true : false;
#endif
}


CeasymutexGuard::CeasymutexGuard(const Ceasymutex& emutex)
:	m_mutex(emutex)
{
	m_mutex.lock();
}
CeasymutexGuard::~CeasymutexGuard(void)
{
	m_mutex.unlock();
}



