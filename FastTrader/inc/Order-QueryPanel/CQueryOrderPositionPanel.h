#pragma once

#include "../inc/Module-Misc2/tools_util.h"
#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Order-Common/CLocalOrderService.h"
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "CQueryOrderPLDetailDialog.h"

// 必要时把类名由CQueryOrderPositionPanel改为CQueryOrderStopPanel，这才是个合理的止盈止损的概念
class CQueryOrderPositionPanel: public wxPanel
{
public:
	enum {	//GUI Enum Control ID Start
			ID_QUERYPOSITION_BUTTONQUERY = 1800,
			ID_QUERYPOSITION_BUTTONEDIT,
			ID_QUERYPOSITION_BUTTONDELETE,
			ID_QUERYPOSITION_BUTTONCLEAR,
			ID_QUERYPOSITION_BUTTONPLDETAIL,
			ID_QUERYPOSITION_CHECKSHOWDELETE,
			////GUI Enum Control ID End
			ID_QUERYPOSITION_NUM		//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

public:
    CQueryOrderPositionPanel(wxWindow *parent,
            wxWindowID winid = GID_QUERY_ORDER_POSITION,//
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CQueryOrderPositionPanel(void);

	void SetLocalOrderService(CLocalOrderService* poLocalOrderService) {
		m_poLocalOrderService = poLocalOrderService;
	};

	BOOL NeedOrderFailSound();
	BOOL NeedServCondOverBoundConfirm();
	BOOL NeedServCondOverBoundSound();

	int i_CurOrderCnt;					//当前委托类型的记录数
	int i_CurOrderRows;					//当前委托类型的记录数

	void OnButtonQuery(wxCommandEvent& evt);
	void OnButtonEdit(wxCommandEvent& evt);
	void OnButtonDelete(wxCommandEvent& evt);
	void OnButtonClear(wxCommandEvent& evt);
	void OnLanguageChange(wxCommandEvent& evt);
	void OnActivated(wxExtListEvent& event);
	void OnButtonPLDetail(wxCommandEvent& evt);
	void OnCfgChanged(wxCommandEvent& evt);////配置文件发生变化
	bool Init();

private:

	void ShowOrderPositions(BOOL bIsShowDelete);
	int ShowOneOrder(int iRowNo, 
					 const CLocalOrderService::PLORDER& tPositionOrder, 
					 BOOL bIsNew=TRUE);
	void OnInitSubscribe(wxCommandEvent& evt);
	
	void OnSize(wxSizeEvent& event );
	void OnContext(wxContextMenuEvent& evt);
	void OnRequery(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);
	void OnPLStatusChanged(wxCommandEvent& evt);
	void OnPLDetailStatusChanged(wxCommandEvent& evt);
	void OnServCondOverBoundFail(wxCommandEvent& evt);
	void OnPLDetailDlgPreClose(wxCommandEvent& evt);
	void SaveColWidth();
	wxString GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow);
	wxString GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl);

private:
    static  CRITICAL_SECTION m_CS_EvtCallback;

	CLocalOrderService*		m_poLocalOrderService;
	LPLIST_TABLE_CFG		m_pListCfg;
	int m_iColIndex;
	int m_iRowCnt;

	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID; //记录字段在哪一列显示 
	std::map<int, LIST_COLUMN_CFG> m_FieldAttr;//记录字段的显示属性
	std::map<int, int> m_OrderPositionKey2Row;
	std::map<int, int> m_OrderPositionRow2Key;
	std::map<int, CQueryOrderPLDetailDialog*>	m_mapDetailDlg;
	
	wxButton*		m_pButtonQuery;
	wxButton*		m_pButtonEdit;
	wxButton*		m_pButtonDelete;
	wxButton*		m_pButtonClear;
	wxButton*		m_pButtonPLDetail;
	wxExtListCtrl*	m_pwxExtListCtrl;

	wxFont		m_Font;
	wxColor		m_HeadBackgroundColor;
	wxColor		m_HeadColor;
	wxColor		m_BackgroundColor;
	wxColor		m_TextColor;

DECLARE_EVENT_TABLE()
};
