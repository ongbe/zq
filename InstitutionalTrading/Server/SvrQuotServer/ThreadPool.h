// ThreadPool.h: interface for the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////


#pragma once

//#include "windows.h"
#include "vector"
#include "WorkerThread.h"
#include "iostream"
#include "ThreadPoolSelfDefine.h"
#include "ThreadMutex.h"
#include "MYDEBUG.h"
#include <cassert> 
using namespace std;
class CWorkerThread;
class CThreadPool  
{
public:
	CThreadMutex m_BusyMutex;
	CThreadMutex m_IdleMutex;
	vector <CWorkerThread * > m_BusyList;
	vector <CWorkerThread * > m_IdleList;
	CThreadPool();
	CThreadPool(int initnum);

	void SetMaxNum(int maxnum){m_MaxNum = maxnum;}
	int GetMaxNum(void){return m_MaxNum;}

	void SetAvailLowNum(int minnum){m_AvailLow = minnum;}
	int GetAvailLowNum(void){return m_AvailLow;}

	void SetAvailHighNum(int highnum){m_AvailHigh = highnum;}
	int GetAvailHighNum(void){return m_AvailHigh;}

	int GetActualAvailNum(void){return m_AvailNum;}
	int GetAllNum(void){
		
		int tempavailnum = 0;
		int tempbusysize = 0;
		m_IdleMutex.Lock();
		tempavailnum=m_AvailNum;
		m_IdleMutex.Unlock();

		m_BusyMutex.Lock();
		tempbusysize = m_BusyList.size();
		m_BusyMutex.Unlock();

		return tempavailnum + tempbusysize;
	
	}

	int GetBusyNum(void){

		int tempbusysize = 0;
		m_BusyMutex.Lock();
		tempbusysize = m_BusyList.size();
		m_BusyMutex.Unlock();

		return tempbusysize;
	}
	
	void SetInitNum(int initnum){m_InitNum = initnum;}
	int GetInitNum(void){return m_InitNum;}

	void TerminateAll(void);
	void Run(JobInfo jobInfoparam);

	virtual ~CThreadPool();
//protected:
public:
	CWorkerThread*  GetIdleThread(void);
	void			AppendToIdleList(CWorkerThread *jobthread);
	void			MoveToBusyList(CWorkerThread * jobthread);
	void			MoveToIdleList(CWorkerThread *busythread);
	void			DeleteIdleThread(int num);
	void			CreateIdleThread(int nmu);
private:
	unsigned int m_MaxNum;
	unsigned int m_AvailLow;
	unsigned int m_AvailHigh;
	unsigned int m_AvailNum;
	unsigned int m_InitNum;
};

