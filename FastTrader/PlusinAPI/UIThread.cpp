#include "StdAfx.h"
#include "APIMainDlg.h"
#include "UIThread.h"

extern HINSTANCE theInstance;
extern volatile long  g_ThreadStatus;
IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
	//{{AFX_MSG_MAP(CScanThread)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CUIThread::CUIThread()
: m_pAPIMainDlg(NULL)
, m_hInitDoneEvent(NULL)
{

}

CUIThread::CUIThread(CAPIMainDlg* pAPIMainDlg, HANDLE hEvent)
: m_pAPIMainDlg(pAPIMainDlg)
, m_hInitDoneEvent(hEvent)
{

}

CUIThread::~CUIThread(void)
{
}

BOOL CUIThread::InitInstance()
{
	m_pMainWnd = m_pAPIMainDlg;
	SetEvent(m_hInitDoneEvent);
	return TRUE;
}

int CUIThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}
