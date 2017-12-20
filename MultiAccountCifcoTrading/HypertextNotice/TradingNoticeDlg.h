#pragma once
class wxIEHtmlWin;
class CTradingNoticeDlg :
	public wxDialog
{
public:
	CTradingNoticeDlg(wxWindow *parent);
	~CTradingNoticeDlg(void);
public:
	WXLRESULT MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam);
	void OnClose(wxCloseEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnGetNewContent(wxCommandEvent& event);
    DECLARE_EVENT_TABLE()
private:
	wxIEHtmlWin*	m_ie;
};
