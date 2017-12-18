
#pragma once

class J_ZiXuanHeYueDlg;

class GroupSetDlg : public wxDialog
{
public:
	enum
	{
		ID_ADD,
		ID_DEL,
		ID_UP,
		ID_DOWN,
		ID_COM,
		ID_LIST,
		ID_OK
	};

public:

	GroupSetDlg(J_ZiXuanHeYueDlg *parent);
	~GroupSetDlg();
	void OnAdd(wxCommandEvent& event);
	void OnDel(wxCommandEvent& event);
	void OnUp(wxCommandEvent& event);
	void OnDown(wxCommandEvent& event);
	void OnReturn(wxCommandEvent& event);
	void OnSelected(wxExtListEvent& event);
	void InitCtrols(vector<INSTRUMENT_GROUP_CFG>& vGroup);

	DECLARE_EVENT_TABLE();

private:
	wxOwnerDrawnComboBox *m_pComGroup;
	wxExtListCtrl *m_pList;
	vector<INSTRUMENT_GROUP_CFG> m_vGroup;
	J_ZiXuanHeYueDlg* m_pParent;
};

