// ThreadPool.cpp: implementation of the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "ThreadPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadPool::CThreadPool() 
{ 
    m_MaxNum = 100; 
    m_AvailLow = 5; 
    m_InitNum = m_AvailNum = 10 ;   
    m_AvailHigh = 20;
    m_BusyList.clear();
    m_IdleList.clear();
	CreateIdleThread(m_InitNum);
} 

CThreadPool::CThreadPool(int initnum) 
{ 
   // assert(initnum>=2 && initnum<=20); 
	initnum = initnum > 30 ? 30 :initnum;
	initnum = initnum < 2 ? 2 :initnum;
    m_MaxNum   = 70; 
    m_AvailLow = initnum-10>0?initnum-10:10;
	m_AvailHigh = m_AvailLow + 20; 
	
    m_InitNum = initnum; 
    m_AvailNum = 0;
    
	
    m_BusyList.clear(); 
    m_IdleList.clear(); 
	CreateIdleThread(m_InitNum);
} 
CThreadPool::~CThreadPool() 
{ 
	TerminateAll(); 
} 

void CThreadPool::TerminateAll() 
{ 
	m_BusyMutex.Lock(); 
    vector<CWorkerThread *>::iterator busypos; 
	for(busypos=m_BusyList.begin();busypos!=m_BusyList.end();++busypos){
		(*busypos)->Terminate();
	} 
    m_BusyMutex.Unlock(); 
	
	m_IdleMutex.Lock(); 
	vector<CWorkerThread *>::iterator idlepos; 
	for(idlepos=m_IdleList.begin();idlepos!=m_IdleList.end();++idlepos){
		(*idlepos)->Terminate();
	} 
    m_IdleMutex.Unlock();
    return; 
} 

CWorkerThread* CThreadPool::GetIdleThread(void) 
{ 
    while(m_IdleList.size() == 0 ) 
        Sleep(10);
    m_IdleMutex.Lock(); 
    if(m_IdleList.size() > 0 ) 
    { 
        CWorkerThread* thr = (CWorkerThread*)m_IdleList.front(); 
		//	m_IdleList.erase(m_IdleList.begin());
        m_IdleMutex.Unlock(); 
        return thr; 
    } 
    m_IdleMutex.Unlock(); 
    return NULL; 
} 

void CThreadPool::AppendToIdleList(CWorkerThread *jobthread) 
{ 
    m_IdleMutex.Lock(); 
    m_IdleList.push_back(jobthread); 
    m_AvailNum++;
    m_IdleMutex.Unlock(); 
} 

void CThreadPool::MoveToBusyList(CWorkerThread *jobthread) 
{ 
	m_BusyMutex.Lock();	
	m_BusyList.push_back(jobthread);
	m_BusyMutex.Unlock();
    m_IdleMutex.Lock();
    vector<CWorkerThread *>::iterator pos; 
	
	for(pos=m_IdleList.begin();pos!=m_IdleList.end();++pos){
		if (*pos==jobthread)
		{ 
			m_IdleList.erase(pos);
			m_AvailNum--;  
			break;
		}
	}
	
    m_IdleMutex.Unlock(); 
} 

void CThreadPool::MoveToIdleList(CWorkerThread* busythread) 
{ 
    if (NULL==busythread)
    {
		return;
    }
		m_BusyMutex.Lock(); 
		vector<CWorkerThread*>::iterator pos; 
		vector<CWorkerThread*>::iterator endpos = m_BusyList.end(); 
		for(pos = m_BusyList.begin();pos != endpos; ++pos){
			if (*pos == busythread)
			{
				m_BusyList.erase(pos); 
				break;
			}
		} 
	m_BusyMutex.Unlock(); 
    m_IdleMutex.Lock(); 
    m_IdleList.push_back(busythread); 
    m_AvailNum++; 
    m_IdleMutex.Unlock(); 
	
	if (GetActualAvailNum() > m_AvailHigh)		//保证最多只有m_AvailLow个空闲线程
	{
		MYDEBUG::OUTPUT("1");
		DeleteIdleThread(m_IdleList.size() - m_AvailHigh);
		MYDEBUG::OUTPUT("2");
	}
	if (GetActualAvailNum() < m_AvailLow && GetAllNum() < GetMaxNum())		//在总线程数目不超过GetMaxNum()的情况下，保证至少有m_AvailLow个空闲线程
	{
		CreateIdleThread(m_AvailLow - m_IdleList.size());
	}
	if(GetAllNum() > GetMaxNum()) {				//保证最多只有GetMaxNum()个线程
		MYDEBUG::OUTPUT("3");
		DeleteIdleThread(GetAllNum() - GetMaxNum());
		MYDEBUG::OUTPUT("4");
	}
	MYDEBUG::OUTPUT("0");
} 

void CThreadPool::CreateIdleThread(int num) 
{ 
	//    printf("CThreadPool::CreateIdleThread|create %d idle threads for need\n",num);
	CWorkerThread * tempwork;
    for(int i=0;i<num;i++)
    { 
        tempwork = new CWorkerThread(); 
        tempwork ->SetThreadPool(this); 
        tempwork->Start(tempwork); 
		AppendToIdleList(tempwork); 
    } 
} 

void CThreadPool::DeleteIdleThread(int num) 
{ 
    for(int i=0;i<num;i++)
    { 
        CWorkerThread* thr = GetIdleThread(); 
		
        vector<CWorkerThread*>::iterator pos;
		m_IdleMutex.Lock(); 
		vector<CWorkerThread*>::iterator endpos=m_IdleList.end();
		m_IdleMutex.Unlock();
		for(pos=m_IdleList.begin();pos!=endpos;++pos){
			if (*pos==thr)
			{
				MYDEBUG::OUTPUT("线程退出ID:%d",(*pos)->GetThreadIdReal());
				m_IdleMutex.Lock(); 
				m_IdleList.erase(pos);
				m_IdleMutex.Unlock();
				thr->Terminate();
				//	m_AvailNum--;
				break;
			}
		}
        
		
    } 
    
} 

void CThreadPool::Run(JobInfo jobInfoparam) 
{  
    
    while(GetBusyNum() == m_MaxNum)  
        Sleep(10);
    if(GetActualAvailNum() < m_AvailLow && GetAllNum() < m_MaxNum)
    { 
        CreateIdleThread(m_AvailLow-GetActualAvailNum());
		MYDEBUG::OUTPUT("%d,%d+++++++++++++++++++++++++++ 创建线程",GetAllNum(),m_MaxNum);
		Sleep(100);
    }
    CWorkerThread *  idlethr = GetIdleThread(); 
    if(idlethr != NULL) 
    { 
        if(!idlethr->SetJob(jobInfoparam))
        {
			return; 
        }
		MoveToBusyList(idlethr);
		int tempi = 0;
		while (-1 == ResumeThread(idlethr->GetThreadHandle()))
		{
			if (3 == tempi)
			{
				MYDEBUG::OUTPUT("线程%d Resume 失败 !",idlethr->GetThreadHandle());
			}
			++tempi;
		}
    } 
} 
