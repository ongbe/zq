#ifdef _USE_TOP_SPLITTER
#pragma once
#include <wx/listctrl.h>
class wxAuiNotebook;
class CVideoInfoPanel : public wxPanel
{
public:

	CVideoInfoPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CVideoInfoPanel(void);

	void StartTimer()
	{
		m_timer.Start(TIMER_INTERVAL);
	}

	void StopTimer()
	{
		m_timer.Stop();
	}

protected:

	void OnSize(wxSizeEvent& evt);
	void OnActivated(wxListEvent& event);
	void OnTimer( wxTimerEvent& event);
	void OnClose(wxCloseEvent& evt);
	DECLARE_EVENT_TABLE()

private:
	bool parseXML(std::string & xmlfilename);

private:
	wxAuiNotebook* m_InfoNotebook;
	wxListCtrl* m_ListCtrl;
	std::map<long, wxString> m_mapIDTitle;
	wxTimer m_timer;
	std::string m_xmlfilename;
};
#endif