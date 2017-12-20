#pragma once

#include "FundAccountQryDlg.h" //资金查询结果 显示对话框
#include "cfgMgr/CfgMgr.h"
class FundAccountQryDlg;
class FundAccountPats :
    public wxPanel
{
public:
	enum
	{
		ID_CBX_CURRENCY=200,
		ID_BUTTON_REQRY,
		ID_STATIC
	};
	static FundAccountPats * CreatePane(wxWindow* parent);
    static int AllCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static int ExchangeRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
public:
    FundAccountPats(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	void OnSize( wxSizeEvent& event );
	void OnExchangeRate(wxCommandEvent& event);
	void OnConfig(wxCommandEvent& event);
    void OnMenuExport(wxCommandEvent& event);
	void OnAutoAdjustColWidth(wxCommandEvent& event);
	void OnCurrencyChangeSel( wxCommandEvent& event );
	void OnContext(wxContextMenuEvent& evt);
	void OnSubscrible(wxCommandEvent& evt);
	void OnAllCallBack(wxCommandEvent& event);
    void QryTradingAccount(wxCommandEvent& event);
	void OnReQry(wxCommandEvent& event);
	void UpdateUIData(string strAccount,PlatformStru_TradingAccountInfo& AccountInfo);
	void SetColumnsText(int row);
	wxString GetColValue(int id,PlatformStru_TradingAccountInfo* info);
	void InsertString(wxArrayString& strArr,wxString str);
	void InitComboBox();
	void InitListCtrl();
	void SaveColWidth();
	void OnCfgChanged(wxCommandEvent& evt);
#ifdef _USE_MULTI_LANGUAGE
	void OnLanguageChange(wxCommandEvent& event);
#endif
	DECLARE_EVENT_TABLE()
private:
    FundAccountQryDlg* m_pFundAccountQryDlg;
	double		m_curExchangeRate;
	wxExtListCtrl* m_list;
};
