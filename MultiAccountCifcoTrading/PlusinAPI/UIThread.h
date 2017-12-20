#ifndef UITHREAD_H
#define UITHREAD_H

#include "afxwin.h"
#include <queue>

class CAPIMainDlg;

class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)

public:
	CUIThread(void);
	CUIThread(CAPIMainDlg* pAPIMainDlg, HANDLE hEvent);
	~CUIThread(void);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()

private:
	CAPIMainDlg* m_pAPIMainDlg;
	HANDLE m_hInitDoneEvent;
};

#endif
