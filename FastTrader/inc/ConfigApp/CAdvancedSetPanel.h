#pragma once 

class CAdvancedSetPanel:public wxPanel
{
public:
	enum
	{
		ID_BTN_ADD,
		ID_BTN_DEL,
		ID_CBX_HOTKEY
	};

	CAdvancedSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~ CAdvancedSetPanel();

	void InitControls();
	bool WriteCfg();

	void OnChangeSel(wxCommandEvent& event);
	void OnAdd( wxCommandEvent& event );
	void OnDel( wxCommandEvent& event );

	DECLARE_EVENT_TABLE()

private:
	wxOwnerDrawnComboBox* m_cbxHotKey;
	wxExtListCtrl* m_listCtrl;
};
