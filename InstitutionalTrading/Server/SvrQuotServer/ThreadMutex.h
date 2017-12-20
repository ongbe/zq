// ThreadMutex.h: interface for the CThreadMutex class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "windows.h"
//#include <afx.h>
class CThreadMutex
{
private:
	HANDLE	this_lock;
public:
	CThreadMutex(){ this_lock=CreateMutex(NULL,FALSE,NULL); }
	~CThreadMutex(){ ReleaseMutex(this_lock);this_lock = NULL;	}// 没有关闭，会泄漏  //pthread_mutex_destroy(&this_lock);
	void Lock(){ 	WaitForSingleObject(this_lock, INFINITE);; }
	void Unlock(){ ReleaseMutex(this_lock); }
    };
