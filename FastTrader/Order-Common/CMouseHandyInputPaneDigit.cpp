#include "StdAfx.h"
#include "CMouseHandyInputPaneDigit.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"

set<CMouseHandyInputPaneDigit*>	s_setMouseHandyInputPaneDigit;
HHOOK s_hMouseHook_Digit = NULL;
int s_nMouseHookCounter_Digit = 0;

LRESULT CALLBACK Digit_InputMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HC_ACTION) {
		MOUSEHOOKSTRUCT* pMouseHook = (MOUSEHOOKSTRUCT*)lParam;
		//wxMouseEvent evt;
		//evt.m_x = pMouseHook->pt.x;
		//evt.m_y = pMouseHook->pt.y;
		//s_pMouseHandyInputPane->OnMouseMove(evt);
		wxPoint pos;
		pos.x = pMouseHook->pt.x;
		pos.y = pMouseHook->pt.y;
		for(set<CMouseHandyInputPaneDigit*>::iterator it=s_setMouseHandyInputPaneDigit.begin();
				it!=s_setMouseHandyInputPaneDigit.end(); it++) {
				if(*it!=NULL && (*it)->IsShown())
					(*it)->TransMouseHook(pos);
			//s_pMouseHandyInputPaneDigit->TransMouseHook(pos);
		}
	}
	return CallNextHookEx(s_hMouseHook_Digit, nCode, wParam, lParam);
}

BOOL RegisterWindowMouseHook_Digit()
{
	if(s_nMouseHookCounter_Digit==0)
		s_hMouseHook_Digit = SetWindowsHookEx(WH_MOUSE, Digit_InputMouseProc, NULL, GetCurrentThreadId());
	s_nMouseHookCounter_Digit++;
	return TRUE;
}

BOOL UnregisterWindowMouseHook_Digit()
{
	if(s_nMouseHookCounter_Digit==0)
		return FALSE;
	s_nMouseHookCounter_Digit--;
	if(s_nMouseHookCounter_Digit==0)
	{
		UnhookWindowsHookEx(s_hMouseHook_Digit);
		s_hMouseHook_Digit=NULL;
	}
	return TRUE;
}




IMPLEMENT_CLASS(CMouseHandyInputPaneDigit, wxDialog)

BEGIN_EVENT_TABLE(CMouseHandyInputPaneDigit, wxDialog)
    EVT_SIZE( OnSize )
    EVT_SET_FOCUS( OnSetFocus )
    EVT_KILL_FOCUS( OnKillFocus )
	EVT_COMMAND_RANGE(ID_PopupButtonDigit,ID_PopupButtonDigit+18,wxEVT_COMMAND_BUTTON_CLICKED,OnButton)
	EVT_TIMER(wxID_ANY,OnTimer)
END_EVENT_TABLE()

CMouseHandyInputPaneDigit::CMouseHandyInputPaneDigit(
			 wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style,
             const wxString& name)
	:wxDialog(parent, id, title, pos, size, style, name), 
	m_timer(this),
	m_nOwnerUIID(-1)
{
	DIALOG_CREATE_LOG();
	m_nInputIndex = 0;
	m_bDotIsPress = FALSE;
	ZeroMemory(m_strInputDigit, 32);
	
//	m_panel = new wxScrolledWindow( this, wxID_ANY );
//	m_panel->SetBackgroundColour( *wxLIGHT_GREY );
//	m_panel->SetAutoLayout(true);
	vector<string> strDigits;
	SplitString(LOADSTRING(MHIP_DIGITS),strDigits,'#');
	strDigits.resize(conMaxButtonNum);
	int i=0;

	for(i=0; i<conMaxButtonNum; i++) {
		m_button[i] = NULL;
	}
	for(i=0; i<conMaxButtonNum; i++) {
		m_button[i] = new wxButton(this, ID_PopupButtonDigit+i, strDigits[i], 
							wxPoint(1+(i%3)*36, 1+(i/3)*24), wxSize(36, 24));
	}
	SetSize(wxSize(1+36*3, 1+24*((conMaxButtonNum+2)/3)));

//	m_panel->SetAutoLayout( true );
//	m_panel->SetSizer( topSizer );

	s_setMouseHandyInputPaneDigit.insert(this);
	m_timer.Start(500);
}

CMouseHandyInputPaneDigit::~CMouseHandyInputPaneDigit()
{
	m_timer.Stop();
	s_setMouseHandyInputPaneDigit.erase(this);
	if(s_setMouseHandyInputPaneDigit.size()==0)
	{
		UnhookWindowsHookEx(s_hMouseHook_Digit);
		s_hMouseHook_Digit=NULL;
	}
}

bool CMouseHandyInputPaneDigit::Show( bool show )
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::Show %d"), long(this), int(show));
	if(show) {
		////SetCapture((HWND)m_hWnd);
		////DWORD dwThreadId = GetCurrThreadId();
		//s_hMouseHook_Digit = SetWindowsHookEx(WH_MOUSE, Digit_InputMouseProc, NULL, GetCurrentThreadId());
		RegisterWindowMouseHook_Digit();
		CfgMgr* pMgr = CFG_MGR_DEFAULT();
		if (pMgr)
		{
			LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
			if(pMainWindowCfg->bIsTop)
				SetWindowPos((HWND)GETTOPWINDOW()->GetHWND(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
		}
	}
	else {
		////ReleaseCapture();
		//UnhookWindowsHookEx(s_hMouseHook_Digit);
		//s_hMouseHook_Digit = NULL;
		UnregisterWindowMouseHook_Digit();
		CfgMgr* pMgr = CFG_MGR_DEFAULT();
		if (pMgr)
		{
			LPMAIN_WINDOW_CFG pMainWindowCfg = pMgr->GetMainWindowCfg();
			if(pMainWindowCfg->bIsTop)
				SetWindowPos((HWND)GETTOPWINDOW()->GetHWND(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
		}
	}
    return wxDialog::Show(show);
}

void CMouseHandyInputPaneDigit::TransMouseHook(wxPoint mousePos)
{
	if(!IsShown()) return;

	wxRect oriRect = GetClientRect();
	wxRect extRect = GetClientRect();
	wxPoint pos = ScreenToClient(mousePos);
	extRect.x-=2;
	extRect.y-=22;
	extRect.width+=4;
	extRect.height+=24;
	if(!extRect.Contains(pos.x, pos.y)) {
		Show(false);
	}
//	else {
//		if(oriRect.Contains(pos.x, pos.y)) {
//			if(FindFocus() != this)
//				SetFocus();
//		}
//		else {
//		}
//	}
}
void CMouseHandyInputPaneDigit::OnSize(wxSizeEvent &event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnSize"), long(this) );
    event.Skip();
}

void CMouseHandyInputPaneDigit::OnSetFocus(wxFocusEvent &event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnSetFocus"), long(this) );
    event.Skip();
}

void CMouseHandyInputPaneDigit::OnKillFocus(wxFocusEvent &event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnKillFocus"), long(this) );
    event.Skip();
}

void CMouseHandyInputPaneDigit::OnButton(wxCommandEvent& event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnButton ID %d"), long(this), event.GetId());
	
	wxString strInputDigit;

	int nId = event.GetId()-ID_PopupButtonDigit;
	char cData = 0;
	if(nId<10) {
		cData = nId<9 ? '1'+nId : '0';
		strInputDigit = "0,";
		strInputDigit += cData;
	}
	else if(nId == 10) {
		cData = VK_SUBTRACT;
		strInputDigit = "0,";
		strInputDigit += cData;
	}
	else if(nId == 11) {
		cData = VK_DECIMAL;
		strInputDigit = "0,";
		strInputDigit += cData;
	}
	else if(nId == 12) {
		cData = VK_BACK;
		strInputDigit = "0,";
		strInputDigit += cData;
	}
	else if(nId == 13) {
		strInputDigit = "1,";
		strInputDigit += "0";
	}
	else if(nId == 14) {
		strInputDigit = "2,";
		strInputDigit += "0";
	}
	else if(nId == 15) {
		strInputDigit = "3,";
		strInputDigit += "0";
	}
	else if(nId == 16) {
		strInputDigit = "4,";
		strInputDigit += "0";
	}
	else if(nId == 17) {
		strInputDigit = "5,";
		strInputDigit += "0";
	}

	wxCommandEvent evt(wxEVT_MOUSEINPUT_DIGIT, m_nOwnerUIID);
    if(1)
    {
	    DWORD EvtParamID;
	    std::string EvtParamStr(strInputDigit.c_str());
	    if(CFTEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
	    {
		    evt.SetInt((int)EvtParamID);
	    }
    }
//	evt.SetString(strInputDigit.c_str());
	::wxPostEvent(GetParent(),evt);

    event.Skip();
}
void CMouseHandyInputPaneDigit::OnTimer(wxTimerEvent& event)
{
	HWND hwnd =::GetActiveWindow();
	DWORD PsId=0;
	GetWindowThreadProcessId(hwnd,&PsId);
	if(PsId!=GetCurrentProcessId()&&IsShown())
		Hide();
}