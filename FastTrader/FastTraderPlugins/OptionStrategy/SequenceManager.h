#pragma once
#include <map>

class CSequenceManager
{
protected:
	static int sm_nLastSeqNum;

protected:
	HANDLE m_hMutex;
	std::map<int, CWnd*> m_mapSeqNum2Dlg;

public:
	static int GetNextSeqNum();

public:
	CSequenceManager(void);
	~CSequenceManager(void);

	BOOL LockObject();
	BOOL UnlockObject();

	CWnd* GetSeqNumObject(int nSeqNum);
	BOOL RegisterSeqNum(int nSeqNum, CWnd* pdlg);
	BOOL UnregisterSeqNum(int nSeqNum);
	void Clear();
};
