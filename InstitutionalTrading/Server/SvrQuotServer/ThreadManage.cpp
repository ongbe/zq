// ThreadManage.cpp: implementation of the CThreadManage class.
//
//////////////////////////////////////////////////////////////////////
#pragma warning (disable:4786)
#include "ThreadManage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadManage::CThreadManage()
{
	m_JobList.clear();
	m_NumOfThread = 10; 
	m_threadRunFlag = TRUE;
    m_Pool = new CThreadPool(m_NumOfThread); 
}
CThreadManage::CThreadManage(int num)
{ 
	m_JobList.clear();
    m_NumOfThread = num; 
	m_threadRunFlag = TRUE;
    m_Pool = new CThreadPool(m_NumOfThread); 
	
	SetLastError(0);
	m_dispenseThreadhandle = CreateThread(NULL, 0, _DispenseJobThread, (LPVOID)this, 0, NULL);
	while(NULL == m_dispenseThreadhandle)
	{
		DWORD tempDWORD = GetLastError();
		m_dispenseThreadhandle = CreateThread(NULL, 0, _DispenseJobThread, (LPVOID)this, 0, NULL);
	}
} 

CThreadManage::~CThreadManage()
{ 
    if(NULL != m_Pool) 
        delete m_Pool; 
    m_Pool = NULL;
} 

//void CThreadManage::SetParallelNum(int num)
//{ 
//    m_NumOfThread = num; 
//} 

//DEL void CThreadManage::Run(CJob* job,void* jobdata)
//DEL { 
//DEL //    printf("CThreadManage::Run|begin to deal job.\n");
//DEL    // m_Pool->Run(job,jobdata); 
//DEL } 

void CThreadManage::TerminateAll(void)
{ 
	m_threadRunFlagMutex.Lock();
	m_threadRunFlag = FALSE;
	m_threadRunFlagMutex.Unlock();

	::WaitForSingleObject(m_dispenseThreadhandle,2000);
	if (NULL !=m_dispenseThreadhandle)
	{
		DWORD dwExitCode;
		GetExitCodeThread(m_dispenseThreadhandle,&dwExitCode);
		TerminateThread(m_dispenseThreadhandle,dwExitCode);
		::CloseHandle(m_dispenseThreadhandle);
		m_dispenseThreadhandle = NULL;
	}

    m_Pool->TerminateAll(); 
} 

BOOL CThreadManage::AddJob(JOBCALLBACKE callbackfunction,LPVOID lpParam,int Paramsize,JobPriority jobPriority)
{
	unsigned char *tempa = NULL;
	if (NULL !=lpParam)
	{
	try
	{
		tempa = new unsigned char [Paramsize+1];
	}
	catch (...)
	{
		throw CThreadPoolException("Add job error ,please try again ! ");
		return FALSE;
	}
	memcpy(tempa,lpParam,Paramsize);
	tempa[Paramsize]='\0';
	}
	JobInfo tempInfo;
	tempInfo.jobcallback = callbackfunction;
	tempInfo.lpParam = tempa;
	tempInfo.length=Paramsize;
	
	
	if (NORMAL_PRIORITY == jobPriority)
	{	
	m_JobMapLock.Lock();
	m_JobList.push_back(tempInfo);
	m_JobMapLock.Unlock();
	}else if (HIGH_PRIORITY == jobPriority)
	{
		m_JobMapLock.Lock();
		m_JobList.insert(m_JobList.begin(),tempInfo);
		m_JobMapLock.Unlock();
	}
	//cout<<"任务量数目："<<m_JobList.size()<<endl;
	return TRUE;
}

BOOL CThreadManage::GetJob(JobInfo * lpjob)
{
	BOOL tempflag = FALSE;
	JobInfo tempInfo;
	m_JobMapLock.Lock();
	if (!m_JobList.empty())
	{
		tempInfo = m_JobList.front();
		lpjob->jobcallback = tempInfo.jobcallback;
		lpjob->lpParam = tempInfo.lpParam;
		lpjob->length = tempInfo.length;
		m_JobList.erase(m_JobList.begin());
		tempflag = TRUE;
	}
	m_JobMapLock.Unlock();
	return tempflag;
}
DWORD WINAPI CThreadManage::_DispenseJobThread(LPVOID lpParam){
	
	CThreadManage *tempthis = reinterpret_cast<CThreadManage*>(lpParam);
	JobInfo tempInfo;
	CThreadPool *temppool = tempthis->m_Pool;
	BOOL tempflag = TRUE;
	while(tempflag){
		if (tempthis->GetJob(&tempInfo))
		{
			temppool->Run(tempInfo);
			delete [](tempInfo.lpParam);
		}else{
			Sleep(100);
		}
		tempthis->m_threadRunFlagMutex.Lock();
		tempflag = tempthis->m_threadRunFlag ;
		tempthis->m_threadRunFlagMutex.Unlock();
	}
	//	cout<<"_DispenseJobThread 退出 ！"<<endl;
	return 0xCC44;
}