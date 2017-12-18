#pragma once

#include <vector>
#include "../Module-Misc/DataListCtrl.hpp"
using namespace std;

class zqWriteLog;

class TradeRecordPanel :public wxPanel
{
public:
	///标示一个持仓记录，由InstrumentID、Direction、HedgeFlag定义唯一的持仓明细记录


public:
	enum
	{
		ID_ListDetail,
		ID_ListTotal,

		ID_RADIOBTDetail,
		ID_RADIOBTTotal,
        ID_BUTTON_REQRY,
	};

	TradeRecordPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~TradeRecordPanel();
	void OnSize( wxSizeEvent& event );

	CDataListCtrl<TradeKey,PlatformStru_TradeInfo>*  m_pListDetail;
	CDataListCtrl<TradeTotalKey,PlatformStru_TradeTotalInfo>*  m_pListTotal;

    wxButton *     m_pButtonReQry;
	wxRadioButton*  m_pBTDetail;
	wxRadioButton*  m_pBTTotal;

	int m_id;
	DECLARE_EVENT_TABLE()

public:
	static TradeRecordPanel* CreateTradeRecordPanel(wxWindow* parent);
	void OnGetCommissionRate(wxCommandEvent& evt);
    static int RtnTradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
    static int RspQryTradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static int GetCommissionRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
	void OnRcvTrade(wxCommandEvent& evt);
	void OnRspQryTrade(wxCommandEvent& evt);
	void OnRcvTradeRecord(wxCommandEvent& evt);
	void OnRadioButton(wxCommandEvent& event);
	void OnSubscrible(wxCommandEvent& evt);
    void OnReQry(wxCommandEvent& evt);
    void OnReConnectQry(wxCommandEvent& evt);
	void ShowAll();

    void ContextMenuRespond(wxCommandEvent& event);
    void OnConTextMenu(wxContextMenuEvent& event);
	void OnDetailColumnResize(wxExtListEvent& event);
	void OnTotalColumnResize(wxExtListEvent& event);

	void OnCfgChanged(wxCommandEvent& evt);
    void Init();
	void SaveColWidth();
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);

    static bool UpdateListItemCallBackDetail(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);
    static bool UpdateListItemCallBackTotal(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);

#ifdef _USE_MULTI_LANGUAGE
	void OnLanguageChanged(wxCommandEvent& event);
#endif

public:
    zqWriteLog*           m_pWriteLog;


	std::map<int, long> m_FieldID2ColumnIDDetail; //记录字段在哪一列显示 
	std::map<int, long> m_FieldID2ColumnIDTotal; //记录字段在哪一列显示 
    unsigned long m_TextColorDetail;  //字体颜色
    unsigned long m_TextColorTotal;  //字体颜色

};
