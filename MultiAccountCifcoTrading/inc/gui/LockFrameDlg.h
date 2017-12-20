#pragma once

class LockFrameDlg :
    public wxDialog
{
public:
    enum
    {
        wxID_TEXTCTRL=150
    };
    LockFrameDlg(wxWindow *parent);
    ~LockFrameDlg(void);
    DECLARE_EVENT_TABLE()
public:
    void OnOK(wxCommandEvent& evt);
    void OnClose(wxCloseEvent& evt);
};
