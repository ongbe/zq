
#pragma once

class CTableSetPanel : public wxPanel
{
public:
	enum
	{
		ID_List,
		ID_BtUp,
		ID_BtDown,
		ID_BtSet
	};

	CTableSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		int nModuleType = GID_QUOT_TABLE,
		bool bShowCheckBox = true,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr );

	~CTableSetPanel();

	void OnUp(wxCommandEvent& event);

	void OnDown(wxCommandEvent& event);

	void OnSetFont(wxCommandEvent& event);

	bool WriteCfg();
	void SetListData();

	DECLARE_EVENT_TABLE()

private:
	wxExtListCtrl* m_List;
	wxButton*  m_BtUp;
	wxButton*  m_BtDown;
	wxButton*  m_BtSetTable;

	int m_ModuleType;
};