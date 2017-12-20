#pragma once

#include <vector>
#include "../Module-Misc/DataListCtrl.hpp"
#include "../MAServer/include/MAAccount/MultiAccountCmd.h"
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"
#include "../inc/MultiAccountCore/CTPLogin.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
using namespace std;

class zqWriteLog;
class MultiPositionPanel :public wxPanel
{
public:
	enum
	{
		ID_BUTTON_REQRY=1000,

	};
	MultiPositionPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);
	DECLARE_EVENT_TABLE()
public:
	void OnSubscrible(wxCommandEvent& evt);	
	void InitListCtrl();
	void OnSize(wxSizeEvent& event);
	void DeleteAllItemData();
	void DeleteAllColumnData();
	void MergePosition(PlatformStru_Position& dest,const PlatformStru_Position& src);
	void ShowAll();
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);
	static int CallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	int AddRow(const PositionKey& key);
	void DeleteRow(int row);
	void SetOneRow(int row,const PlatformStru_Position& pos);
	void UpdateSameInstrumentIDRows(wxString strInstrumentID);
	bool GetPositionFromKey(const PositionKey key,PlatformStru_Position& all);
	void OnRspQryPositionLast(wxCommandEvent& evt);
	void OnGetMarginRate(wxCommandEvent& evt);
	void OnRtnTrade(wxCommandEvent& evt);
	void OnRtnOrder(wxCommandEvent& evt);
	void OnRtnDepthMarketData(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);
	void OnContext(wxContextMenuEvent& evt);
	void SaveColWidth();
	void OnActivated(wxExtListEvent& event);
	void OnSelected(wxExtListEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);
	wxString GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow);
private:
	wxExtListCtrl* m_pList;
	std::map<int, long> m_FieldID2ColumnID;
	std::map<int, long> m_ColumnID2FieldID;
////////////////////////////////////////////////
	std::map<int,PositionKey> m_Row2Key;
	std::map<PositionKey,int> m_Key2Row;
	set<IPlatformSingleSvr*> m_pSvrSet;
};
