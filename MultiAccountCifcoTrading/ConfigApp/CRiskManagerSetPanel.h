#pragma once 

class CRiskManagerSetPanel : public wxPanel
{
public:
	CRiskManagerSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CRiskManagerSetPanel();

	void ReadCfg();
	bool WriteCfg();

	DECLARE_EVENT_TABLE()

private:
	wxRadioButton* m_radioNotReset;
	wxRadioButton* m_radioResetAll;
	wxRadioButton* m_radioResetHandAndPrice;
	wxRadioButton* m_radioResetHand;

	wxTextCtrl* m_textHandLimit;
	wxTextCtrl* m_textPriceLimit;
};