#include "StdAfx.h"
#include "DisConnPromptDlg.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;
#if 0
#define LOG_INFO(fmt, ...) 
#else
#define LOG_INFO(fmt, ...) \
	do{\
	if(m_pWriteLog)\
{\
	m_pWriteLog->WriteLog_Fmt("DisConnPromptDlg", LOGLEVEL_DEBUGINFO, "[%s][%d]:"fmt, __CUSTOM_FILE__, __LINE__, __VA_ARGS__);\
}\
	}while(0)
#endif
DisConnPromptDlg::DisConnPromptDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(DISCONNECTED_PROMPT_TITLE),wxDefaultPosition, wxSize(300,200),wxDOUBLE_BORDER),
m_parent(parent),
m_timer(this),
dotcount(0),
m_pWriteLog(NULL),
m_laststatus(SS_NOLOGIN)
{
	m_pWriteLog=new zqWriteLog();
	DIALOG_CREATE_LOG();
	SetBackgroundColour(DEFAULT_COLOUR);
    itemStaticText = new wxStaticText(this, wxID_STATIC,LOADSTRING(DISCONNECTED_PROMPT_TEXT), wxPoint(20,90), wxSize(260,20), 0 );
    itemStaticText->SetFont(wxFont(12, wxSWISS, wxNORMAL, wxBOLD, false));
    m_timer.Start(500);
} 
BEGIN_EVENT_TABLE(DisConnPromptDlg,wxDialog)
    EVT_TIMER(wxID_ANY, DisConnPromptDlg::OnTimer)
	EVT_LEFT_DOWN(OnLeftDown)
END_EVENT_TABLE()
DisConnPromptDlg::~DisConnPromptDlg(void)
{
	SAFE_DELETE(m_pWriteLog);
}
void DisConnPromptDlg::OnTimer(wxTimerEvent& event)
{
	BOOL bParentShow=::IsWindowVisible((HWND)m_parent->GetHWND());
	BOOL bThisShow=::IsWindowVisible((HWND)GetHWND());
	//if(!m_parent->IsShown()) return;//CITHINC-1122
	if(bParentShow&&bThisShow)
	{
		wxString strText('.',dotcount+1);
		itemStaticText->SetLabel(LOADSTRING(DISCONNECTED_PROMPT_TEXT)+strText);
		ServiceStatus  curstatus;
		memset(&curstatus,0,sizeof(curstatus));
		int quot_status=SS_NOLOGIN,trader_status=SS_NOLOGIN;
		if(DEFAULT_SVR()) 
			curstatus=DEFAULT_SVR()->GetCurrentStatus(&quot_status,&trader_status);
		if((m_laststatus==SS_LOGINED&&curstatus==SS_LOGINED)||
			(m_laststatus==SS_NOLOGIN&&curstatus==SS_NOLOGIN))
		{

			Hide();
		}
		//LOG_INFO("DisConnPromptDlg(Last=%d,Current=%d,Quot=%d,Trade=%d)",
		//	m_laststatus,curstatus,quot_status,trader_status);
		if(dotcount%3==0)
			m_laststatus=curstatus;
		dotcount++;
		if(dotcount>5) dotcount=0;
	}
}
void DisConnPromptDlg::OnLeftDown(wxMouseEvent& event)
{
	wxPoint point=event.GetPosition();
	PostMessage((HWND)m_hWnd,WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y)); 
}