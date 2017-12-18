#pragma once
///////////////////////////////////////////////////////////////////////////
class ConfirmationDlg :
    public wxDialog
{
    enum
    {
        ID_TEXTCTRL=100
    };

public:
    ConfirmationDlg(wxWindow *parent);
    ~ConfirmationDlg(void);
    void OnPaint(wxPaintEvent& event);
    DECLARE_EVENT_TABLE()
private:
    wxBitmap m_BackBitmap;
};
