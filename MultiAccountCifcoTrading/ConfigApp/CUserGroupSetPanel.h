
#pragma once
#include "cfgMgr/AccountGroup.h"

class CUserGroupSetPanel : public wxPanel
{
public:
	enum
	{
		ID_USERGROUPLIST,
		ID_GROUPINFOLIST,
		ID_ACCOUNTLIST,
		ID_ADDGROUP,
		ID_DELGROUP,
		ID_ADDALLACCOUNT,
		ID_ADDACCOUNT,
		ID_DELACCOUNT,
		ID_DELALLACCOUNT,
		ID_ACCOUNTNUMBERIN,
		ID_ACCOUNTNUMBEROUT,
		ID_SHORTKEY,
	};

	CUserGroupSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CUserGroupSetPanel();

	void OnLabelBeginEdit(wxExtListEvent& event);
	void OnLabelEndEdit(wxExtListEvent& event);
	void OnGroupSelected(wxExtListEvent& event);
	void SelectGroup( int nIndex );
	void OnAddGroup(wxCommandEvent& event);
	void OnDelGroup(wxCommandEvent& event);

	void OnAddAllAccount(wxCommandEvent& event);
	void OnAddAccount(wxCommandEvent& event);
	void OnDelAccount(wxCommandEvent& event);
	void OnDelAllAccount(wxCommandEvent& event);
	void OnSelChanged( wxCommandEvent& event );

	bool WriteCfg();
	void ReadCfg();

	DECLARE_EVENT_TABLE()

private:
	void InitControls();
	bool IsExistInCurList(const wxString& name);
	bool IsGroupExist(const wxString& group);
	void RefreshCurGroupData();

private:
	wxExtListCtrl* m_userGroupList;
	wxExtListCtrl* m_groupInfoList;
	wxExtListCtrl* m_accountList;
	wxOwnerDrawnComboBox* m_pCbxShortKey;

	wxStaticText* m_pAccountNumInGroup;
	wxStaticText* m_pAccountNumOutOfGroup;

	std::vector<AccountGroup> m_vAccountGroup;
};