
#pragma once

class CGroupSetPanel:public wxPanel
{
public:
	enum
	{
		ID_GROUPLIST,
		ID_ADD,
		ID_DEL,
		ID_UP,
		ID_DOWN
	};

	CGroupSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CGroupSetPanel();

	void BeginEdit(wxExtListEvent& event);
	void OnLabelEndEdit(wxExtListEvent& event);
	void OnAdd(wxCommandEvent& event);
	void OnDel(wxCommandEvent& event);
	void OnUp(wxCommandEvent& event);
	void OnDown(wxCommandEvent& event);

	void WriteCfg();
	void ReadCfg();

	DECLARE_EVENT_TABLE()

private:
	void InitControls();

private:
	wxExtListCtrl* m_groupListCtrl;

	bool m_bFlag;
};