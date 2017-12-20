//  Name: 持仓模块
//  Desc:
//          1. 处理持仓模块的显示
//          2. 将右键菜单的命令通过顶层窗口传递给下单模块
//  Interface:
//          底层模块platformSvr
//              1. 订阅BID_RspQryInvestorPositionDetail
//              2. 订阅BID_RspQryInstrumentMarginRate
//              3. 订阅BID_RtnTrade
//              4. 订阅BID_RtnOrder
//              5. 订阅BID_RtnDepthMarketData
//              6. DEFAULT_SVR()->GetPositionDetails
//              7. DEFAULT_SVR()->GetCurPrice
//              8. DEFAULT_SVR()->GetProductClassType
//              9. DEFAULT_SVR()->GetTradingDay
//              10.DEFAULT_SVR()->GetInstrumentInfo
//              11.DEFAULT_SVR()->GetQuotInfo
//              12.DEFAULT_SVR()->GetMarginRate
//          配置模块CfgMgr
//              1. CfgMgr::GetInstance()->GetListCfg(GID_POSITIONDETAIL)
//              2. CfgMgr::GetInstance()->GetListColCount(GID_POSITIONDETAIL)
//              3. CfgMgr::GetInstance()->GetListColCfg(GID_POSITIONDETAIL, i);
//          配置程序ConfigApp
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_LOAD_CONFIG_APP) - 表格列设置
//************************************************************************************

#pragma once

#include "cfgMgr/CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"

using namespace std;

class zqWriteLog;
class CMAPositionDetailInfoList : public wxExtListCtrl
{
public:
	CMAPositionDetailInfoList(wxWindow *parent,
           wxWindowID id = wxID_ANY,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_TOGGLE_COLOUR,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxEmptyString);

public:
	virtual ~CMAPositionDetailInfoList(void);

    void ShowAll(IPlatformSingleSvr* pSvr=NULL);

    //配置listctrl的参数
    void Init(void);

    //右键菜单
	void OnContext(wxContextMenuEvent& evt);

	static int TradeCallBackFunc(const AbstractBusinessData&);	  //成交回报
	static int QuotCallBackFunc(const AbstractBusinessData&);	  //行情
	void OnPositionDetailUpdate(wxCommandEvent& evt);                 
	void OnPositionDetailTradeUpdate(wxCommandEvent& evt);
    void OnCfgChanged();
	void OnLanguageChanged(int langid);
	void OnColumnResize(wxExtListEvent& event);

    void OnRspQryPositionDetailLast(wxCommandEvent& evt);
    void OnGetMarginRate(wxCommandEvent& evt);
    void OnRtnTrade(wxCommandEvent& evt);
//    void OnRtnOrder(wxCommandEvent& evt);
    void OnRtnDepthMarketData(wxCommandEvent& evt);
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);
	void SaveColWidth();
	bool ReQry();
	set<IPlatformSingleSvr*>* m_ppSvrSet;

	DECLARE_EVENT_TABLE();
	void ClearAllItemsAndData();
	void UpdateInstrumentOfSvr( IPlatformSingleSvr* pSvr, std::string strInstru, bool bRowsChanged );
	void UpdateCellFromItemData(int row);

private:	
	static CMAPositionDetailInfoList *m_PostionDetailList;
	
	std::map<int, int> m_ColumnID2FieldID; //记录字段在哪一列显示 

	wxColor m_TextColor;

    zqWriteLog*           m_pWriteLog;


};