#pragma once

class FundAccountQryDlg :
    public wxDialog
{
enum
{
    ID_TEXTCTRL=100
};
public:
    FundAccountQryDlg(wxWindow *parent);
    ~FundAccountQryDlg(void);
    void SetContent();

	void WxButton6Click(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);

private:
    wxTextCtrl* m_pTextCtrl;


private:
	DECLARE_EVENT_TABLE();
};
