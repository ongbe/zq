#pragma once

#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/constants.h"
#include "../cfgMgr/CfgMgr.h"
#include "../inc/Module-Misc/DataListCtrl.hpp"

class CComboPositionInfoList : public CDataListCtrl<PositionKey,PlatformStru_Position>
{
public:
	CComboPositionInfoList(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_TOGGLE_COLOUR,
			const wxValidator& validator = wxDefaultValidator,
			const wxString& name = wxPanelNameStr);

	virtual ~CComboPositionInfoList();   

	void InitListCtrl();
	void OnCfgChanged();
	void OnLanguageChanged(wxCommandEvent& event);
	void OnContext(wxContextMenuEvent& evt);

	void OnRtnTrade(wxCommandEvent& evt);
	void OnRspQryPositionDetailComboLast(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);
	void ShowAll();

	static bool UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);
	
	DECLARE_EVENT_TABLE()

	void SaveColWidth();

private:
	std::map<int, long> m_FieldID2ColumnID; //记录字段在哪一列显示
};