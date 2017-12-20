#include "stdafx.h"
#include "rwlock.h"



int uv__rwlock_fallback_init(rwlock_t* rwlock) 
{
	InitializeCriticalSection(&rwlock->read_mutex_);
	InitializeCriticalSection(&rwlock->write_mutex_);
	rwlock->num_readers_ = 0;
	rwlock->m_hevReadersCleared = CreateEvent(NULL,TRUE,TRUE,NULL);
	return 0;
}


void uv__rwlock_fallback_destroy(rwlock_t* rwlock) {
	WaitForSingleObject(rwlock->m_hevReadersCleared,INFINITE);
	CloseHandle(rwlock->m_hevReadersCleared);
	DeleteCriticalSection(&rwlock->read_mutex_);
	DeleteCriticalSection(&rwlock->write_mutex_);
}


void uv__rwlock_fallback_rdlock(rwlock_t* rwlock) {
	EnterCriticalSection(&rwlock->write_mutex_);
	EnterCriticalSection(&rwlock->read_mutex_);

	if (++rwlock->num_readers_ == 1)
		ResetEvent(rwlock->m_hevReadersCleared);

	LeaveCriticalSection(&rwlock->read_mutex_);
	LeaveCriticalSection(&rwlock->write_mutex_);
}




void uv__rwlock_fallback_rdunlock(rwlock_t* rwlock) {
	EnterCriticalSection(&rwlock->read_mutex_);

	if (--rwlock->num_readers_ == 0)
		SetEvent(rwlock->m_hevReadersCleared);

	LeaveCriticalSection(&rwlock->read_mutex_);
}


void uv__rwlock_fallback_wrlock(rwlock_t* rwlock) {
	EnterCriticalSection(&rwlock->write_mutex_);
	WaitForSingleObject(rwlock->m_hevReadersCleared,INFINITE);
}



void uv__rwlock_fallback_wrunlock(rwlock_t* rwlock) {
	LeaveCriticalSection(&rwlock->write_mutex_);
}
