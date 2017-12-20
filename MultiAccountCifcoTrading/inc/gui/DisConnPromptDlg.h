#pragma once
class zqWriteLog;
class DisConnPromptDlg :
    public wxDialog
{
public:
    DisConnPromptDlg(wxWindow *parent);
    ~DisConnPromptDlg(void);
    void OnTimer(wxTimerEvent& event);
	void OnLeftDown(wxMouseEvent& event);
    DECLARE_EVENT_TABLE()
private:
    wxStaticText* itemStaticText;
    wxTimer m_timer;
    int dotcount;
    ServiceStatus m_laststatus;
	wxWindow *m_parent;
	zqWriteLog* m_pWriteLog;
};
