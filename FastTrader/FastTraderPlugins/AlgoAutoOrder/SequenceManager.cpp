#include "StdAfx.h"
#include "SequenceManager.h"


int CSequenceManager::sm_nLastSeqNum = 0;

int CSequenceManager::GetNextSeqNum()
{
	sm_nLastSeqNum++;
	return sm_nLastSeqNum;
}

CSequenceManager::CSequenceManager(void)
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
}

CSequenceManager::~CSequenceManager(void)
{
	LockObject();
	Clear();
	UnlockObject();
	CloseHandle(m_hMutex);
	m_hMutex = NULL;
}


BOOL CSequenceManager::LockObject()
{
	::WaitForSingleObject(m_hMutex, INFINITE);
	return TRUE;
}

BOOL CSequenceManager::UnlockObject()
{
	::ReleaseMutex(m_hMutex);
	return TRUE;
}

CWnd* CSequenceManager::GetSeqNumObject(int nSeqNum)
{
	std::map<int, CWnd*>::iterator it = 
			m_mapSeqNum2Dlg.find(nSeqNum);
	if(it!=m_mapSeqNum2Dlg.end())
		return it->second;
	else
		return NULL;
}

BOOL CSequenceManager::RegisterSeqNum(int nSeqNum, CWnd* pdlg)
{
	std::map<int, CWnd*>::iterator it = 
			m_mapSeqNum2Dlg.find(nSeqNum);
	if(it!=m_mapSeqNum2Dlg.end())
		return FALSE;

	m_mapSeqNum2Dlg.insert(std::pair<int, CWnd*>(nSeqNum, pdlg));
	return TRUE;
}

BOOL CSequenceManager::UnregisterSeqNum(int nSeqNum)
{
	std::map<int, CWnd*>::iterator it = 
			m_mapSeqNum2Dlg.find(nSeqNum);
	if(it==m_mapSeqNum2Dlg.end())
		return FALSE;

	m_mapSeqNum2Dlg.erase(it);
	return TRUE;
}

void CSequenceManager::Clear()
{
	m_mapSeqNum2Dlg.clear();
}
