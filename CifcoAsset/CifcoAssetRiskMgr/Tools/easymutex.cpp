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
	m_pcs=new CRITICAL_SECTION;
	InitializeCriticalSection((CRITICAL_SECTION*)m_pcs);
}

Ceasymutex::~Ceasymutex()
{
	DeleteCriticalSection((CRITICAL_SECTION*)m_pcs);
	delete (CRITICAL_SECTION*)m_pcs;
}

void Ceasymutex::lock(void)
{
	EnterCriticalSection((CRITICAL_SECTION*)m_pcs);
}

void Ceasymutex::unlock(void)
{
	LeaveCriticalSection((CRITICAL_SECTION*)m_pcs);
}

