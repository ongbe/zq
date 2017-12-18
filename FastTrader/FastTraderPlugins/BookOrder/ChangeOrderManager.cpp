#include "StdAfx.h"
#include "ChangeOrderManager.h"


DWORD WINAPI CChangeOrderManager::ThreadProc(LPVOID lpParameter)
{
	CChangeOrderManager* poChgOrderMng = (CChangeOrderManager*)lpParameter;

	if(poChgOrderMng!=NULL)
		return poChgOrderMng->Run();
	else
		return -1;
}

CChangeOrderManager::CChangeOrderManager(void)
{
	m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
	m_hThread = NULL;
}

CChangeOrderManager::~CChangeOrderManager(void)
{
	::CloseHandle(m_hMutex);
}

BOOL CChangeOrderManager::Init()
{
	m_bExit = FALSE;
	DWORD dwThreadID = 0;
	m_hThread = ::CreateThread(NULL, 0, ThreadProc, this, 0, &dwThreadID);
	return m_hThread!=NULL;
}

int CChangeOrderManager::Run()
{
	while(!m_bExit)
	{
		LockObject();

		std::queue<CChangeOrderKey> queueDel;
		DWORD dwCurrTickCount = ::GetTickCount();
		std::map<CChangeOrderKey, CHANGEORDERDATA>::iterator it = m_mapWaitOrder.begin();
		for(; it != m_mapWaitOrder.end(); it++)
		{
			if(dwCurrTickCount - it->second.dwRemainTime > conMaxRemainTime*1000)
			{
				queueDel.push(it->first);
			}
		}
		while(!queueDel.empty())
		{
			m_mapWaitOrder.erase(queueDel.front());
			queueDel.pop();
		}

		UnlockObject();

		Sleep(50);
	}
	return 0;
}

void CChangeOrderManager::Release()
{
	m_bExit = TRUE;
	Sleep(50);
	::CloseHandle(m_hThread);
}

BOOL CChangeOrderManager::ResetChangeOrder(CString& strAccount, CString& strSubAccount, 
										   CString& strInstrumentID, char cDirection, 
										   PlatformStru_InputOrder& inputOrder, CWnd* pWnd)
{
	CChangeOrderKey oKey(strAccount, strSubAccount, strInstrumentID, cDirection);

	LockObject();

	std::map<CChangeOrderKey, CHANGEORDERDATA>::iterator it = m_mapWaitOrder.find(oKey);
	if(it != m_mapWaitOrder.end())
	{
		it->second.dwRemainTime = ::GetTickCount();
		it->second.inputOrder = inputOrder;
		it->second.pWnd = pWnd;
	}
	else
	{
		CHANGEORDERDATA tData;
		ZeroMemory(&tData, sizeof(tData));
		tData.dwRemainTime = ::GetTickCount();
		tData.inputOrder = inputOrder;
		m_mapWaitOrder.insert(std::pair<CChangeOrderKey, CHANGEORDERDATA>(oKey, tData));
	}

	UnlockObject();

	return TRUE;
}

BOOL CChangeOrderManager::ResetChangeOrderSeqNum(CString& strAccount, CString& strSubAccount, 
												 CString& strInstrumentID, char cDirection, int nSeqNum)
{
	CChangeOrderKey oKey(strAccount, strSubAccount, strInstrumentID, cDirection);
	
	LockObject();
	
	std::map<CChangeOrderKey, CHANGEORDERDATA>::iterator it = m_mapWaitOrder.find(oKey);
	if(it != m_mapWaitOrder.end())
	{
		it->second.nSeqNum = nSeqNum;
	}

	UnlockObject();

	return TRUE;
}

BOOL CChangeOrderManager::ParseRsp(int nSeqNum, int nVolume)
{
	if(nVolume!=0)
		return FALSE;

	PlusinAPI* poPlusinApi = GetPlusinAPI();
	if(poPlusinApi==NULL) 
		return FALSE;

	LockObject();

	std::queue<CChangeOrderKey> queueDel;
	std::map<CChangeOrderKey, CHANGEORDERDATA>::iterator it = m_mapWaitOrder.begin();
	for(; it != m_mapWaitOrder.end(); it++)
	{
		if(it->second.nSeqNum == nSeqNum)
		{
			queueDel.push(it->first);
			int nSeqNum = CSequenceManager::GetNextSeqNum();
			LockGlobalObject();
			CSequenceManager& objSeqMng = GetSequenceManager();
			objSeqMng.RegisterSeqNum(nSeqNum, it->second.pWnd);
			UnlockGlobalObject();
			poPlusinApi->Send(CMDID_ORDER_INSERT_ORDER, &it->second.inputOrder, 
				sizeof(it->second.inputOrder), nSeqNum);
			break;
		}
	}
	while(!queueDel.empty())
	{
		m_mapWaitOrder.erase(queueDel.front());
		queueDel.pop();
	}
	UnlockObject();
	
	return FALSE;
}
