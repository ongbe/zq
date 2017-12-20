// ThreadManage.h: interface for the CThreadManage class.
//
//////////////////////////////////////////////////////////////////////


#pragma once


#include "ThreadPool.h"
#pragma warning (disable:4786)
#include "vector"
#include "ThreadPoolSelfDefine.h"
#include "ThreadPoolException.h"
#define  DELETEPARAMSPACE(ptr)  if(ptr){  delete ptr; ptr = NULL;}
#define	 DEBUGCOUT
extern MYDEBUG debug;
class CJobListMutex
{
	   private:
		   HANDLE	joblist_lock;
	   public:
		   CJobListMutex(){ joblist_lock=CreateMutex(NULL,FALSE,NULL); }
		   ~CJobListMutex(){ 
			   ReleaseMutex(joblist_lock);
			   joblist_lock = NULL;
		   }// 没有关闭，会泄漏 
		   void Lock(){ WaitForSingleObject(joblist_lock, INFINITE); }
		   void Unlock(){ ReleaseMutex(joblist_lock); }
};

class CThreadManage  
{
	  
public:
	BOOL GetJob(JobInfo * lpjob);
	BOOL AddJob( JOBCALLBACKE callbackfunction,LPVOID lpParam,int Paramsize ,JobPriority jobPriority) throw (string);
	CThreadManage();
	virtual ~CThreadManage();
	CThreadManage(int num);
	void	TerminateAll(void);
	CThreadPool *GetThreadPool(void){return m_Pool;}
private:
	CThreadPool				*m_Pool;					//线程池指针
	int						m_NumOfThread;				//初始线程池线程数

	CJobListMutex			m_JobMapLock;				//对joblist进行互斥
	vector<JobInfo>			m_JobList;					//任务队列

	BOOL					m_threadRunFlag;			//分发线程运行控制变量
	CJobListMutex			m_threadRunFlagMutex;		//对m_threadRunFlag的访问进行互斥
	
	HANDLE					m_dispenseThreadhandle;		//分发线程句柄
	static DWORD WINAPI _DispenseJobThread(LPVOID lpParam);
};


