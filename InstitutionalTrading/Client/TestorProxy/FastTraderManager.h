#pragma once

#include "fasttrader\inc\gui\ConsoleDefine.h"
#include <map>
#include <queue>

class CFastTraderManager
{
public:
	enum {	conMaxFolderNum = 20 };

	typedef struct {
		int		nSeqNum;
		int		nFloderNum;
		DWORD	nProcID;
		HANDLE	hProc;
		HWND	hFastTraderHwnd;
	} PROCINFO;

public:
	CFastTraderManager(void);
	~CFastTraderManager(void);

	BOOL LockObject();
	BOOL UnlockObject();

	DWORD GetProcID(int nSeqNum);
	int GetSeqNum(DWORD dwProcID);
	BOOL RegisterFastTrader(int nSeqNum, HWND hMainWnd, CString& strUserName, 
							CString& strPassword, DWORD& dwProcID, int& nFolderNum);
	BOOL UnregisterFastTrader(int nSeqNum);
	void Clear();

	BOOL ReseHwnd(DWORD dwProcID, HWND hWnd);
	BOOL DoCommand(int nSeqNum, CMD_INPUTSIMPLE& input);

protected:
	BOOL LoadFastTrader(int nFloderNum, HWND hMainWnd, CString& strUserName, 
						CString& strPassword, HANDLE& hProc, DWORD& dwProcID);
	BOOL UnloadFastTrader(HWND hFTHwnd, HANDLE hProc);

protected:
	HANDLE m_hMutex;
	std::queue<int>			m_queueIdleFloder;
	std::map<int, PROCINFO>	m_mapProc;
	std::map<DWORD, int>	m_mapProcID2SeqNum;

};
