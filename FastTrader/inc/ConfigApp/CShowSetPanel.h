#pragma once 

class CShowSetPanel:public wxPanel
{
public:
	enum
	{
		ID_ComColor=100,
		ID_ComShortKey,
		ID_BtSet,
		ID_MYLIST,
		ID_FIXPANEL,
		ID_SHOWKEY,
		//ID_TOPMOST,
		ID_MIN2TRAY,
		//ID_AUTOLOCK
	};

	CShowSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CShowSetPanel();

	void OnSet(wxCommandEvent& event);
	void OnSelColor( wxCommandEvent& event );
	void OnSelShortkey( wxCommandEvent& event );
	void OnListItemSel(wxExtListEvent& event);
	bool WriteCfg();

	DECLARE_EVENT_TABLE()

public:
	long m_itemIndex;
	vector<PanelInfo>* m_PanelInfo;
};