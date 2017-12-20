

#pragma once


#include "windows.h"

typedef struct {
	CRITICAL_SECTION read_mutex_;
	CRITICAL_SECTION write_mutex_;
	unsigned int num_readers_;
	HANDLE m_hevReadersCleared;

} rwlock_t;



int uv__rwlock_fallback_init(rwlock_t* rwlock);
void uv__rwlock_fallback_destroy(rwlock_t* rwlock);
void uv__rwlock_fallback_rdlock(rwlock_t* rwlock);
void uv__rwlock_fallback_rdunlock(rwlock_t* rwlock);
void uv__rwlock_fallback_wrlock(rwlock_t* rwlock);
void uv__rwlock_fallback_wrunlock(rwlock_t* rwlock);