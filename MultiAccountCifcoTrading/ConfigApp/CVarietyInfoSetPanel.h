#pragma once 

#include <vector>

class CVarietyInfoSetPanel:public wxPanel
{
public:
	enum
	{
		ID_List,
		ID_BtAdd,
		ID_BtSub,
		ID_Com
	};

	CVarietyInfoSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CVarietyInfoSetPanel();

	void OnAdd(wxCommandEvent& event);
	void OnDel(wxCommandEvent& event);
	void OnSelc( wxCommandEvent& event );
	void ClickFun(wxExtListEvent& event);
	void RClickFun(wxExtListEvent& event);
	void OnFocusChange(wxExtListEvent& event);
	void ItemSel(wxExtListEvent& event);
	void OnLabelEndEdit(wxExtListEvent& event);
	void OnKeyFun(wxCommandEvent& event);

	void SetListData();
	void ReadCfg();
	bool IsChanged();
	bool WriteCfg();

	DECLARE_EVENT_TABLE()

private:
	wxExtListCtrl* m_listCtrl;
	wxOwnerDrawnComboBox* m_cbx;
	std::vector<VarietyInfo>* m_varietyInfo;
};