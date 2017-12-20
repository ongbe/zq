
#pragma once

class CAutoOpenCloseSetPanel:public wxPanel
{
public:
	enum
	{
		ID_BTN_SET,
		ID_RADIO1,
		ID_RADIO2,
		ID_RADIO3
	};

	CAutoOpenCloseSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CAutoOpenCloseSetPanel();

	void OnSub(wxCommandEvent& event);
	void OnSet( wxCommandEvent& event );
	void OnRadioButton(wxCommandEvent& event);

	void InitControls();
	bool WriteCfg();

	DECLARE_EVENT_TABLE()

private:
	wxRadioButton* m_radioBtn1;
	wxRadioButton* m_radioBtn2;
	wxRadioButton* m_radioBtn3;
};


