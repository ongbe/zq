#include "StdAfx.h"
#include "MouseHandyInputPaneVolume.h"
#include "cfgMgr/CfgMgr.h"



IMPLEMENT_CLASS(MouseHandyInputPaneVolume, wxPopupTransientWindow)

BEGIN_EVENT_TABLE(MouseHandyInputPaneVolume, wxPopupTransientWindow)
    EVT_SIZE( MouseHandyInputPaneVolume::OnSize )
    EVT_SET_FOCUS( MouseHandyInputPaneVolume::OnSetFocus )
    EVT_KILL_FOCUS( MouseHandyInputPaneVolume::OnKillFocus )
	EVT_COMMAND_RANGE(ID_PopupButtonVolume,ID_PopupButtonVolume+8,wxEVT_COMMAND_BUTTON_CLICKED,OnButton)
END_EVENT_TABLE()

MouseHandyInputPaneVolume::MouseHandyInputPaneVolume( wxWindow *parent )
                     :wxPopupTransientWindow( parent )
{
	
	m_panel = new wxScrolledWindow( this, wxID_ANY );
    m_panel->SetBackgroundColour( *wxLIGHT_GREY );
	m_panel->SetAutoLayout(true);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *subSizer = NULL;

	m_nVolumes[0] = 1;
	m_nVolumes[1] = 2;
	m_nVolumes[2] = 3;
	m_nVolumes[3] = 4;
	m_nVolumes[4] = 5;
	m_nVolumes[5] = 10;
	m_nVolumes[6] = 15;
	m_nVolumes[7] = 20;
	m_nVolumes[8] = 50;

	AsyncVolumes();
	
	int i=0;
	wxString strText;

	for(i=0; i<conMaxButtonNum; i++) {
		m_button[i] = NULL;
	}
	for(i=0; i<conMaxButtonNum; i++) {
		if(i%3 == 0) {
			subSizer = new wxBoxSizer( wxHORIZONTAL );
		}
		strText.Printf("%d", m_nVolumes[i]);
		m_button[i] = new wxButton(m_panel, ID_PopupButtonVolume+i, strText, 
							wxPoint(1,1), wxSize(36, 32));
		subSizer->Add( m_button[i], 0, wxALL, 0 );
		if(i%3 == 2 || i+1 == conMaxButtonNum) {
			topSizer->Add(subSizer, 0, wxALL, 0 );
		}
	}

	m_panel->SetAutoLayout( true );
    m_panel->SetSizer( topSizer );
    topSizer->Fit(m_panel);
    topSizer->Fit(this);
}

MouseHandyInputPaneVolume::~MouseHandyInputPaneVolume()
{
}

void MouseHandyInputPaneVolume::AsyncVolumes()
{
	// 这里从xml中读是否要确认的配置
	CfgMgr* pMgr = CFG_MGR_DEFAULT();  
	if(pMgr == NULL) return;
	LPORDER_CFG p = pMgr->GetOrderCfg();
	m_nVolumes[0] = p->Volume[0];
	m_nVolumes[1] = p->Volume[1];
	m_nVolumes[2] = p->Volume[2];
	m_nVolumes[3] = p->Volume[3];
	m_nVolumes[4] = p->Volume[4];
	m_nVolumes[5] = p->Volume[5];
	m_nVolumes[6] = p->Volume[6];
	m_nVolumes[7] = p->Volume[7];
	m_nVolumes[8] = p->Volume[8];
	wxString strText;
	try {
	for(int i=0; i<conMaxButtonNum; i++) {
		strText.Printf("%d", m_nVolumes[i]);
		wxWindow* window=FindWindowById(ID_PopupButtonVolume+i,this);
		if(window)
			window->SetLabel(strText);
		//m_button[i]->SetLabel(strText);
	}
	}
	catch(...) {
	}
}

void MouseHandyInputPaneVolume::Popup(wxWindow *focus)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::Popup"), long(this) );
	AsyncVolumes();
	wxPopupTransientWindow::Popup(focus ? focus : m_panel);
}

void MouseHandyInputPaneVolume::OnDismiss()
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnDismiss"), long(this) );
    wxPopupTransientWindow::OnDismiss();
}

bool MouseHandyInputPaneVolume::ProcessLeftDown(wxMouseEvent& event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::ProcessLeftDown pos(%d, %d)"), long(this), event.GetX(), event.GetY());
    return wxPopupTransientWindow::ProcessLeftDown(event);
}
bool MouseHandyInputPaneVolume::Show( bool show )
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::Show %d"), long(this), int(show));
    return wxPopupTransientWindow::Show(show);
}

void MouseHandyInputPaneVolume::OnSize(wxSizeEvent &event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnSize"), long(this) );
    event.Skip();
}

void MouseHandyInputPaneVolume::OnSetFocus(wxFocusEvent &event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnSetFocus"), long(this) );
    event.Skip();
}

void MouseHandyInputPaneVolume::OnKillFocus(wxFocusEvent &event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnKillFocus"), long(this) );
	//Show(false);
    event.Skip();
}

void MouseHandyInputPaneVolume::OnButton(wxCommandEvent& event)
{
    WXLOG_INFO( wxT("0x%lx MouseHandyInputPaneDigit::OnButton ID %d"), long(this), event.GetId());

	Dismiss();
	int nId = event.GetId()-ID_PopupButtonVolume;
	wxString strText;
	strText.Printf("%d", m_nVolumes[nId]);
	wxCommandEvent evt(wxEVT_MOUSEINPUT_VOLUME, 0);
//	evt.SetString(strText.c_str());
    if(1)
    {
        DWORD EvtParamID;
        std::string EvtParamStr(strText.c_str());
        if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
        {
	        evt.SetInt((int)EvtParamID);
        }
    }
	::wxPostEvent(GetParent(),evt);

    event.Skip();
}
