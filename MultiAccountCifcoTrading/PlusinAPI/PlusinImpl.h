#ifndef PLUSINIMPL_H
#define PLUSINIMPL_H

#include <string>
#include <queue>
#include "../inc/PlusinCommon/PlusinAPI.h"

class CUIThread;
class CAPIMainDlg;

typedef int (*MsgCallBackFuncInner)(int nCmdId, int nErrorCode, void* pData, int nDataLen);

class PlusinImpl : public PlusinAPI
{
public:
	static PlusinImpl* GetInstance();
	static void DestroyInstance();

public:
	void Init(const char* szAppID, 
		const char* szLicense, 
		MsgCallBackFunc msgCallBackFn, 
		HWND hTargetWnd,
		int nPos = NEED_NO_POS_NO_BIND, 
		HWND hAPPHWnd = NULL);
	bool Logon();
	bool Send(int nCmdId, void* pData, int nDataLen, int nRequestId = 0);
	void Logout();

private:
	static int SaveMsgCallBackFunc(int nCmdId, int nErrorCode, void* pData, int nDataLen);
	static DWORD NotifyThreadFunc(void* pParam);
	HWND GetAPIHWnd();
	DWORD GetDllVersion();

private:
	PlusinImpl();
	~PlusinImpl();

	static PlusinImpl* m_pInstance;
	CUIThread* m_pUIThread;
	std::string m_strAppID;
	std::string m_strLicense;
	int m_nPos;
	HWND m_hAPPHWnd;
	HWND m_hTargetWnd;
	bool m_bInit;

	HANDLE m_hNotifyThreadHandle;
	bool m_bNotifyThreadRun;

	std::queue<COPYDATASTRUCT> m_msgQueue;
	CRITICAL_SECTION m_cs;
	MsgCallBackFunc m_appMsgCallBack;
	CAPIMainDlg* m_pAPIMainDlg;
	HANDLE m_hInitDoneEvent;
};


#endif