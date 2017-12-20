#ifdef _USE_TOP_SPLITTER
#pragma once
#include <wx/listctrl.h>
class wxAuiNotebook;
class SoapGuideThread;
class MyTextCtrl : public wxTextCtrl
{
public:
    MyTextCtrl(wxWindow *parent, wxWindowID id,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = 0,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxTextCtrlNameStr);
	void OnContextMenu(wxContextMenuEvent& event);
	DECLARE_EVENT_TABLE()
};
class CGuideInfoPanel : public wxPanel
{
public:

	CGuideInfoPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CGuideInfoPanel(void);
	void OnSize(wxSizeEvent& evt);
	void OnSoapMsg(wxCommandEvent& evt);
	void OnTimer( wxTimerEvent& event);
	void OnClose(wxCloseEvent& evt);

	void StartTimer()
	{
		m_timer.Start(TIMER_INTERVAL/60);
	}

	void StopTimer()
	{
		m_timer.Stop();
	}

protected:
	DECLARE_EVENT_TABLE()
private:
	wxAuiNotebook* m_InfoNotebook;
	MyTextCtrl * m_TextCtrl;
	SoapGuideThread* m_pThread;
	wxTimer m_timer;
	std::vector<string> m_vecLast;
};
#endif
