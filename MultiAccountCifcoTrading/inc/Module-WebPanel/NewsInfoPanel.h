#ifdef _USE_TOP_SPLITTER
#pragma once
#include <wx/listctrl.h>
class wxAuiNotebook;
class wxIEHtmlWin;
//class SoapThread;
class SoapGuideThread;
class CResearchReportThread;
class CResearchReportInfoPanel;

enum TIMER_MEMBER
{
	ID_TIMER_NEWS_INFO_CHILD_PANEL=11000,
	ID_TIMER_RESEARCH_REPORT_INFO_PANEL,
	ID_TIMER_VIDEO_INFO_PANEL,
	ID_TIMER_GUIDE_INFO_PANEL
};

class CNewsInfoChild: public wxPanel
{
public:
	CNewsInfoChild(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	void AddButton(int id,wxString name);
	void DoQry();
	void OnSize(wxSizeEvent& evt);
	void OnGetList(wxCommandEvent& evt);
	void OnSoapMsg(wxCommandEvent& evt);
	void OnActivated(wxListEvent& event);
	void OnTimer( wxTimerEvent& event);
	void OnClose(wxCloseEvent& evt);

	void StartTimer()
	{
		m_timer.Start(TIMER_INTERVAL);
	}

	void StopTimer()
	{
		m_timer.Stop();
	}

	DECLARE_EVENT_TABLE()
private:
	wxString m_buttonid;
	wxPoint m_buttonpos;
	int     m_buttonindex;
	wxListCtrl* m_ListCtrl;
	CResearchReportThread* m_pThread;
	SoapGuideThread * m_pGuideThread;
	wxTimer m_timer;
	std::vector<int> m_vecLast;

};
class DetailContentDlg :
    public wxDialog
{
public:
    DetailContentDlg(wxWindow *parent);
    ~DetailContentDlg(void);
	wxIEHtmlWin* GetIEHtmlWin(){return m_ie;}
	void OnSize(wxSizeEvent& evt);
	DECLARE_EVENT_TABLE()
private:
	wxIEHtmlWin	*m_ie;
};
class CNewsInfoPanel :
	public wxPanel
{
public:
	CNewsInfoPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	~CNewsInfoPanel(void);
	void InitInfoNotebook();
	void Getlist();

	void StartTimer();

	void StopTimer();

protected:
	void OnSize(wxSizeEvent& evt);

   DECLARE_EVENT_TABLE()
private:
	wxAuiNotebook* m_InfoNotebook;

	std::vector<CNewsInfoChild*> m_vecChildPanels;
	CResearchReportInfoPanel* m_pChild_Direct;

	//此变量用于仅显示按钮而不创建wxAuiNotebook时的OnSize
	CNewsInfoChild* m_pChild_industry;
};

class CResearchReportInfoPanel : public wxPanel
{
public:

	CResearchReportInfoPanel(wxWindow *parent,
		wxString& soapurl,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CResearchReportInfoPanel(void);
	void OnSize(wxSizeEvent& evt);
	void OnSoapMsg(wxCommandEvent& evt);
	void OnActivated(wxListEvent& event);
	void OnTimer( wxTimerEvent& event);
	void OnClose(wxCloseEvent& evt);

	void StartTimer()
	{
		m_timer.Start(TIMER_INTERVAL);
	}

	void StopTimer()
	{
		m_timer.Stop();
	}

protected:
	DECLARE_EVENT_TABLE()
private:
//	wxAuiNotebook* m_InfoNotebook;
	wxListCtrl* m_ListCtrl;
	CResearchReportThread* m_pThread;
	wxString m_soapurl;
	wxTimer m_timer;
	std::vector<int> m_vecLast;
};
#endif

