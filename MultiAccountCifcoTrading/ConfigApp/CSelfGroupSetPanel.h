#pragma once

#include "stdafx.h"
#include "../inc/Module-Misc/InstrumentGrid.h"
BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(wxEVT_CODE_CLICKED, 1301)
END_DECLARE_EVENT_TYPES()

class MouseHandyInputPaneX;
class SelfGroupFunc;
class CSelfGroupSetPanel:public wxPanel
{
public:
	enum
	{
		ID_EDIT_INPUT_INSTRUMENT_NAME,
		ID_CBX_INSTRUMENT_GROUP,
		ID_BTN_SET,
		ID_CBX_EXCHANGE,
		ID_BTN_UP,
		ID_BTN_DOWN,
		ID_BTN_ADD,
		ID_BTN_DEL,
		ID_BASELIST,
		ID_CUSTOMLIST
	};

public:
	CSelfGroupSetPanel(IPlatformSingleSvr* pSvr,wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);
		
	~CSelfGroupSetPanel();

	void OnButtonSet(wxCommandEvent& event);
	void OnButtonUp(wxCommandEvent& event);
	void OnButtonDown(wxCommandEvent& event);
	void OnButtonAdd(wxCommandEvent& event);

	void OnButtonDel(wxCommandEvent& event);

	void GetCustomSelections( vector<int> &vRows );

	void OnInstrumentGroupSel(wxCommandEvent& event);
	void OnExChangeSel(wxCommandEvent& event); 

	void OnCodeMouseClicked(wxMouseEvent& evt);
	void OnShowCodeClicked(wxCommandEvent& evt); 
	void OnMouseInputCode(wxCommandEvent& evt);
	void OnContractNameChanged(wxCommandEvent& event);
	void OnMousePanelKeyAscii(wxKeyEvent& event);

	void OnCellLeftDoubleClick(wxGridEvent& evt);
	void OnCustomListActivated(wxGridEvent& event);

	void OnTreeItemExpanding(wxTreeEvent& event);
	void OnTreeItemActived(wxTreeEvent& event);

	virtual bool Show(bool bShow = true);
	bool IsChanged();
	bool WriteCfg();
	void ReadCfg();
	//void ReadInstrumentList();
	void InitControls();
	void ResetSelfGroupItems();

	bool IsSystemGroup( const std::string& strGroupName );

	DECLARE_EVENT_TABLE()

private:
	bool ExistInCustomList( const std::string& strInstumentName );
	bool AddInstrumentToGroup( const std::string &strInstrumentName, 
		const std::string& strGroupName );
	void SetCustomListItem( const std::string& strGroupName );
	std::string GetValidInstrument( const wxString& strInstrumentName );

private:
	MouseHandyInputPaneX*	m_MouseHandyPopupPane;
	wxTextCtrl* m_editInputInstrumentName;
	wxOwnerDrawnComboBox*  m_cbxInstrumentGroup;
//////////////////////////////////////////////////////////
	wxOwnerDrawnComboBox*  m_cbxExchange;	
	InstrumentGrid* 	m_listBase;
///////////////////////////////////////////////////////////
	wxTreeCtrl* 		m_treeBase;
//////////////////////////////////////////////////////////////
	SelfGroupFunc*      m_selfgroupfunc;     
///////////////////////////////////////////////////////////
	wxGrid* m_listCustom;

	std::vector<GroupInfo> m_vecGroupInfo;
	BOOL	m_bNoKeyChangeText;
	wxTimer m_timer;
	IPlatformSingleSvr* m_pSvr;
};
