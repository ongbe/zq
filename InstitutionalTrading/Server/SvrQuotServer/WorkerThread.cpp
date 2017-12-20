// WorkerThread.cpp: implementation of the CWorkerThread class.
//
//////////////////////////////////////////////////////////////////////

#include "WorkerThread.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkerThread::CWorkerThread()
{
//	m_Job = NULL;
   // m_JobData = NULL;
   // m_ThreadPool = NULL;
	m_ThreadPool = NULL;
	m_callBackFunction = NULL;
	m_callBackParam = NULL;
	m_callBackParamchar = NULL;
	m_threadRunFlag = TRUE;
}

CWorkerThread::~CWorkerThread()
{

}
void CWorkerThread::Run()
{
	BOOL tempThreadFlag = TRUE;
    while(tempThreadFlag)
    {
		m_functionAndParamMutex.Lock();
		if ((NULL != m_callBackFunction))
		{
		LPVOID tempParam=(LPVOID)m_callBackParamchar;
		try{
			MYDEBUG::OUTPUT("当前调用线程:%d",m_ThreadIDReal);
		(*m_callBackFunction)(tempParam);		//调用回调函数m_callBackParamchar
		
		}catch(...){
		//	m_functionAndParamMutex.Unlock();
			MYDEBUG::OUTPUT("Process Job error !");
		}
		if (NULL !=tempParam)
		{
			delete []m_callBackParamchar;
		}
		
		m_callBackFunction = NULL;
		m_callBackParamchar = NULL;
        m_ThreadPool->MoveToIdleList(this);
		}
		m_functionAndParamMutex.Unlock();
		
		m_threadRunFlagMutex.Lock();
		tempThreadFlag = m_threadRunFlag;
		m_threadRunFlagMutex.Unlock();
		if (tempThreadFlag)
		{	
			SuspendThread(m_ThreadHandle);
		}
    }
		MYDEBUG::OUTPUT("线程:%d 退出",m_ThreadIDReal);
}

BOOL CWorkerThread::SetJob(JobInfo tempJobInfo)
{
	m_functionAndParamMutex.Lock();
    m_callBackFunction = tempJobInfo.jobcallback;
	if (NULL != tempJobInfo.lpParam)
	{
			try
			{
				m_callBackParamchar = new unsigned char [tempJobInfo.length+1];
			}catch (bad_alloc & e)
			{
				cout<<e.what()<<endl;
				cout<<("CWorkerThread::SetJob 设置任务时分配空间失败 !")<<endl;
				return FALSE;
			}
		memcpy(m_callBackParamchar,tempJobInfo.lpParam,tempJobInfo.length);	
		m_callBackParamchar[tempJobInfo.length]='\0';
	}
	m_functionAndParamMutex.Unlock();
    return TRUE;
}

void CWorkerThread::SetThreadPool(CThreadPool* thrpool)
{
    m_ThreadPool = thrpool;
}

BOOL CWorkerThread::Terminate()
{
	m_threadRunFlagMutex.Lock();
	m_threadRunFlag = FALSE;
	m_threadRunFlagMutex.Unlock();
	ResumeThread(m_ThreadHandle);
	int ret;
	ret = ::WaitForSingleObject(m_ThreadHandle,100);//时间不能太短，否则结束线程的时候会被卡住
	if (WAIT_OBJECT_0 != ret)
	{
		DWORD dwExitCode;
		GetExitCodeThread(m_ThreadHandle,&dwExitCode);
		if (STILL_ACTIVE ==dwExitCode)
		{
			TerminateThread(m_ThreadHandle,dwExitCode);
			::CloseHandle(m_ThreadHandle);
		}
	}
	m_ThreadHandle = NULL;
	return TRUE;
}

//DEL string CWorkerThread::GetFunctionName()
//DEL {
//DEL  string tempstr;
//DEL  unsigned char * ptemp = (unsigned char *)(m_callBackFunction);
//DEL  for (int i =0 ;i < 4;++i)
//DEL  {
//DEL 	 tempstr += (char)(*ptemp);
//DEL 	// ++ptemp;
//DEL  }
//DEL  return tempstr;
//DEL }
