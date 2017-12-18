#pragma once

#include <vector>

using namespace std;

class TradingNoticeDlg :
    public wxDialog
{
public:
   TradingNoticeDlg(wxWindow *parent);
   ~TradingNoticeDlg();
   void SetContent(wxString content);

protected:
	void OnClose(wxCloseEvent& event);
	void OnBtnNext(wxCommandEvent& event);
	void OnBtnPrev(wxCommandEvent& event);
    void Reload();

private:
    wxTextCtrl* m_itemTextCtrl;
	vector<string>	m_vecContent;
    int m_current;
private:
	DECLARE_EVENT_TABLE();
};
