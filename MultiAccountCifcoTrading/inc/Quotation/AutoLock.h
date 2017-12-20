// AutoLock.h: interface for the CAutoLock class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __AUTOLOCK_H__
#define __AUTOLOCK_H__
#include "CritSec.h"

class CAutoLock  
{
public:
	CAutoLock(CCritSec* pCritSec);
	~CAutoLock();
protected:
    CCritSec * m_pCritSec;
};

#endif 