#include "stdafx.h"
#include "easymutex.h"
#include "Guard.h"

CGuard::CGuard(Ceasymutex* pMutex)
: m_pMutex(pMutex)
{
	if ( NULL != m_pMutex )
	{
		m_pMutex->lock();
	}
}

CGuard::~CGuard(void)
{
	if ( NULL != m_pMutex )
	{
		m_pMutex->unlock();
	}
}
