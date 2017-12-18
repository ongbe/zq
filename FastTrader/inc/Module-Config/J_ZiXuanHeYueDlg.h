#pragma once

#include "ConfigPanelBase.h"
#include "GroupSetDlg.h"
#include "../inc/Module-Misc/InstrumentGrid.h"
#include <wx/treectrl.h>
class GroupSetDlg;
class SelfGroupFunc;
class J_ZiXuanHeYueDlg : public ConfigPanelBase
{
public:
	enum
	{
		ID_COM_EXCHANGE,
		ID_COM_SELF_GROUP,
		ID_LIST_SYS_GROUP,
		ID_LIST_SELF_GROUP,
		ID_BTN_ADD,
		ID_BTN_DEL,
		ID_BTN_UP,
		ID_BTN_DOWN,
		ID_BTN_GROUP_SET,
		ID_BTCancel,
		ID_BTN_SORT
	};

	J_ZiXuanHeYueDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL ,
		//long style =0,
		const wxString& name = wxPanelNameStr);

	virtual ~J_ZiXuanHeYueDlg();

	//void SortList( int col, bool a  );
	//void OnColClick(wxExtListEvent& event);
	//void OnColClick1(wxExtListEvent& event);
	void OnKey(wxMouseEvent& evt);
	void OnAdd(wxCommandEvent& event);
	void OnDelete(wxCommandEvent& event);
	void OnUp(wxCommandEvent& event);
	void OnDown(wxCommandEvent& event);
	void OnSet(wxCommandEvent& event);
	void OnReturn(wxCommandEvent& event);
	void OnExchangeSel(wxCommandEvent& event);
	void OnSelfGroupSel(wxCommandEvent& event);
	void OnSaveAndReturn(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	void OnBaseListActivated(wxGridEvent& evt);
	void OnCustomListActivated(wxGridEvent& event);

	void OnSize( wxSizeEvent& event );
	void Init();
	//void ReadInstrumentList();
	void ReadCfg();
	void WriteCfg();
	void GetCustomSelections( vector<int> &vRows );

	void OnLanguageChanged(wxCommandEvent& event);
	void SetGroupVec(vector<INSTRUMENT_GROUP_CFG>& vGroup);

	DECLARE_EVENT_TABLE()
public:
	void OnTreeItemExpanding(wxTreeEvent& event);
	void OnTreeItemActived(wxTreeEvent& event);
private:
	bool ExistInCustomList( const std::string& strInstumentName );

private:
	wxStaticBox* m_pBoxSelfGroupSet;

	wxStaticText* m_pStaticExChange;
	//////////////////////////////////////////////////////////
	wxOwnerDrawnComboBox* m_pComSysGroup;
	InstrumentGrid* m_pListSysGroup;
	///////////////////////////////////////////////////////////
	wxTreeCtrl* 		m_treeBase;
	//////////////////////////////////////////////////////////////
	SelfGroupFunc*      m_selfgroupfunc;     
	///////////////////////////////////////////////////////////

	wxStaticText* m_pStaticSelGroup;

	wxOwnerDrawnComboBox* m_pComSelGroup;
	wxGrid* m_pListSelfGroup;


	wxButton* m_pBtnAdd;
	wxButton* m_pBtnDel;
	wxButton* m_pBtnGroupSet;

	vector<GroupInfo2> m_GroupInfo2;
	vector<InstrumentGroupInfo> m_SysGroupInfo;
	vector<INSTRUMENT_GROUP_CFG> m_vGroup;

	bool m_bChanged;
	wxTimer m_timer;
};


