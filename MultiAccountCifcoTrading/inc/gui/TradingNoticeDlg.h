#pragma once
class TradingNoticeDlg :
    public wxDialog
{
public:
   TradingNoticeDlg(wxWindow *parent);
   ~TradingNoticeDlg();
   void SetContent(wxString content);
private:
    wxTextCtrl* m_itemTextCtrl;
};
