#pragma once

#include "../inc/Module-Misc2/tools_util.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Order-Common/CLocalOrderService.h"
#include "../inc/Module-Misc/ExtListCtrl.h"

class CQueryOrderPLDetailDialog: public wxDialog//wxPanel
{
public:
	enum {	//GUI Enum Control ID Start
			ID_QUERYPLDETAIL_BUTTONQUERY = 1850,
			ID_QUERYPLDETAIL_BUTTONEDIT,
			ID_QUERYPLDETAIL_BUTTONDELETE,
			ID_QUERYPLDETAIL_BUTTONCLEAR,
			ID_QUERYPLDETAIL_CHECKSHOWDELETE,
			////GUI Enum Control ID End
			ID_QUERYPLDETAIL_NUM		//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

public:
    CQueryOrderPLDetailDialog(wxWindow *parent,
            wxWindowID winid = GID_QUERY_ORDER_PLDETAIL,//
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            //long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX,
            const wxString& name = wxPanelNameStr);
	virtual ~CQueryOrderPLDetailDialog(void);

	void SetLocalOrderService(CLocalOrderService* poLocalOrderService) {
		m_poLocalOrderService = poLocalOrderService;
	};
	void SetOwner(wxPanel* poOwner) {
		m_poOwner = poOwner;
	};

	void OnButtonQuery(wxCommandEvent& evt);
	void OnButtonEdit(wxCommandEvent& evt);
	void OnButtonDelete(wxCommandEvent& evt);
	void OnButtonClear(wxCommandEvent& evt);
	void OnLanguageChange(wxCommandEvent& evt);
	void OnCfgChanged(wxCommandEvent& evt);////配置文件发生变化
	bool Reinit();
	bool Init(long nPLNum);
	void ShowOrderPLDetails(BOOL bIsShowDelete);

private:

	BOOL IsMayEdit(CLocalOrderService::PLDETAILORDER& tOrder, BOOL& bHasServNoTouch);
	BOOL CancelServCond(CLocalOrderService::PLDETAILORDER& plDetailOrder, 
			int nRetGain, int nRetLose);
	int ShowOneOrder(int iRowNo, 
					 const CLocalOrderService::PLDETAILORDER& tPLDetailOrder, 
					 BOOL bIsNew=TRUE);
	void SaveColWidth();
	wxString GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow);
	wxString GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl);

	void OnInitSubscribe(wxCommandEvent& evt);
	
	void OnSize(wxSizeEvent& event );
	void OnClose(wxCloseEvent& evt);
	void OnContext(wxContextMenuEvent& evt);
	void OnRequery(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);
	void OnPLDetailStatusChanged(wxCommandEvent& evt);
	//void OnActivated(wxExtListEvent& event);

private:
    static  CRITICAL_SECTION m_CS_EvtCallback;

	long					m_nPLNum;
	CLocalOrderService*		m_poLocalOrderService;
	wxPanel*				m_poOwner;
	LPLIST_TABLE_CFG		m_pListCfg;

	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID; //记录字段在哪一列显示 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttr;//记录字段的显示属性
	std::map<int, int> m_OrderPLDetailKey2Row;
	std::map<int, int> m_OrderPLDetailRow2Key;
	
	wxButton*		m_pButtonQuery;
	wxButton*		m_pButtonEdit;
	wxButton*		m_pButtonDelete;
	wxButton*		m_pButtonClear;
	wxExtListCtrl*	m_pwxExtListCtrl;

	wxFont		m_Font;
	wxColor		m_HeadBackgroundColor;
	wxColor		m_HeadColor;
	wxColor		m_BackgroundColor;
	wxColor		m_TextColor;

DECLARE_EVENT_TABLE()
};
