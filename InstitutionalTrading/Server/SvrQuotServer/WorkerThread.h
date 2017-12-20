// WorkerThread.h: interface for the CWorkerThread class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

#include "Thread.h"
#include "ThreadPool.h"
//#include "windows.h"

#include "ThreadPoolSelfDefine.h"
#include "ThreadMutex.h"
#define  DELETEPARAMSPACE(ptr)  if(ptr){ delete ptr; ptr = NULL;}
class CThreadPool;
class CWorkerThread  :public CThread
{
public:
	BOOL Terminate();
	CWorkerThread();
	virtual ~CWorkerThread();

	void Run();
	BOOL SetJob(JobInfo tempJobInfo);
	JOBCALLBACKE GetCallBackFuntionlp(void){return m_callBackFunction;}
	void SetThreadPool(CThreadPool * thrpool);
	CThreadPool *GetThreadPool(void){return m_ThreadPool;}
private:
	CThreadPool				*m_ThreadPool;
	JOBCALLBACKE			m_callBackFunction;
	LPVOID 					m_callBackParam;
	unsigned char			*m_callBackParamchar;
	CThreadMutex			m_functionAndParamMutex;
	CThreadMutex			m_threadRunFlagMutex;
	BOOL					m_threadRunFlag;
};

