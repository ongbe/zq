#pragma once
#include "stdafx.h"
class CReadWriteLock
{
public:
	CReadWriteLock(void)
	{
		InitializeCriticalSection(&m_cs); 
	}
	virtual ~CReadWriteLock(void)
	{
		DeleteCriticalSection(&m_cs);
	}
	inline void write_lock() const
	{
		EnterCriticalSection(&m_cs);
	}
	inline void write_unlock() const
	{
		LeaveCriticalSection(&m_cs);
	}
	inline void read_lock() const
	{
		EnterCriticalSection(&m_cs); 
	}
	inline void read_unlock() const
	{
		LeaveCriticalSection(&m_cs); 
	}
private:
	mutable CRITICAL_SECTION	m_cs;
};
//#define CALLFUNC(funcname,para)\
//do {\
//	static LPFUNC func=(LPFUNC)GetProcAddress(GetModuleHandle("kernel32.dll"),#funcname);\
//	if(func) func(para); \
//} while (0)
//	
//class CReadWriteLock
//{
//public:
//	typedef VOID (WINAPI *LPFUNC)(PSRWLOCK SRWLock);
//	CReadWriteLock(void)
//	{
//		m_bSRWLock = (LOBYTE(LOWORD(GetVersion())))>5;
//		if(!m_bSRWLock)
//			InitializeCriticalSection(&m_cs); 
//		else
//		{
//			CALLFUNC(InitializeSRWLock,&m_rw); 
//		}
//	}
//	virtual ~CReadWriteLock(void)
//	{
//		if(!m_bSRWLock)
//			DeleteCriticalSection(&m_cs);
//	}
//	void write_lock() const
//	{
//		if(!m_bSRWLock)
//			EnterCriticalSection(&m_cs);
//		else 
//			CALLFUNC(AcquireSRWLockExclusive,&m_rw);  
//	}
//	void write_unlock() const
//	{
//		if(!m_bSRWLock)
//			LeaveCriticalSection(&m_cs);
//		else 
//			CALLFUNC(ReleaseSRWLockExclusive,&m_rw);  
//	}
//	void read_lock() const
//	{
//		if(!m_bSRWLock)
//			EnterCriticalSection(&m_cs); 
//		else 
//			CALLFUNC(AcquireSRWLockShared,&m_rw); 
//	}
//	void read_unlock() const
//	{
//		if(!m_bSRWLock)
//			LeaveCriticalSection(&m_cs); 
//		else 
//			CALLFUNC(ReleaseSRWLockShared,&m_rw);
//	}
//private:
//	bool m_bSRWLock;
//	mutable CRITICAL_SECTION	m_cs;
//	mutable SRWLOCK				m_rw;
//};