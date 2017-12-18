/***************************************************************************
*   @file         InstrumentList.h
*   @brief        合约列表板块,类的申明
*
*   @author     ying.qiu
*   @version    0.0.1
*   @date       created on 2010/09/26
********************************************************************************/

#ifndef _INSTRUMENT_LIST_PANEL_H_
#define _INSTRUMENT_LIST_PANEL_H_

#include "Module-Misc/ExtListCtrl.h"
#include "../inc/ISvr/PlatformSvrMgr.h"

struct GROUPINFO
{
	GROUPINFO( wxButton* pButton, wxString strText, bool bcomb)
	{
		pBtn = pButton;
		strGroup = strText;
        bComb=bcomb;
	}

	wxButton* pBtn;
	wxString strGroup;
    bool bComb;                 //是不是组合
};

class InstrumentListPanel:public wxPanel
{
public:
	InstrumentListPanel(wxWindow *parent,
		wxWindowID id = GID_INSTRUMENT,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxEmptyString);

	void InitListCtrl();

	//查询期货保证金率响应的回调函数，全部更新
	static int CallBackFunc_BID_RspQryInstrumentMarginRate(const GUIModuleID GID,const AbstractBusinessData& data);
	//查询期货手续费率响应的回调函数，全部更新
	static int CallBackFunc_BID_RspQryInstrumentCommissionRate(const GUIModuleID GID,const AbstractBusinessData& data);
	//查询期权交易成本响应的回调函数，全部更新
	static int CallBackFunc_BID_RspQryOptionInstrTradeCost(const GUIModuleID GID,const AbstractBusinessData& data);
	//查询期权手续费率响应的回调函数，全部更新
	static int CallBackFunc_BID_RspQryOptionInstrCommRate(const GUIModuleID GID,const AbstractBusinessData& data);

	void OnUpdateForInstrument(wxCommandEvent& evt);
	std::string GetItemText(int row, int col);
	virtual ~InstrumentListPanel(void);
	void OnCfgChanged(wxCommandEvent& evt);

	void ShowInstrument();

	void OnSetFocus(wxFocusEvent& event);
	void OnKillFocus(wxFocusEvent& event);
	void OnActivated(wxExtListEvent& event);
	void OnColumnResize(wxExtListEvent& event);

	void OnContext(wxContextMenuEvent& evt);
	void AddShowRateInstrumentID(const std::string& strID);
	void DoInitQryStep();
	void OnSize(wxSizeEvent& event);
	void OnGroupChanged(wxCommandEvent& event);
	void OnGroupButtonClick(wxCommandEvent& event);
	void OnLanguageChanged(wxCommandEvent& event);
	void OnMAPlatformModify(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()

private:
	int GetColValue(int ColId, PlatformStru_InstrumentInfo* pField,  
		PlatformStru_InstrumentCommissionRate* commField,
		PlatformStru_InstrumentMarginRate* margField,
		std::string &strValue);
	void SaveColWidth();

private:
	wxExtListCtrl* m_pListCtrl;
	wxFont m_BakFont;
	wxColor m_bakHeadBackgroundColor;
	wxColor m_bakHeadColor;
	wxColor m_bakBackgroundColor;
	wxColor m_bakTextColor;
	wxColor m_bakEvenLineBgColor;
	wxColor m_bakOddLineBgColor;
	wxTimer m_timer;
	std::vector<GROUPINFO> m_vGroupButton;
	string m_strExchangeID;
    int m_GroupBtnInd;
	
	bool m_bFocus;
	std::vector<std::string> sm_TmpInstrumentID;
	std::map<int, LIST_COLUMN_CFG> m_mapVisibleField;//记录字段的显示属性
	static wxWindow		*m_InstrumentParent;
	std::map<std::string,  int> m_InstrumentID2RowMap;
	std::map<int, int> m_InstrumentID2ColMap;

//    std::set<wxString> m_EvtInstrument;
//    CRITICAL_SECTION m_CS;


};


#endif