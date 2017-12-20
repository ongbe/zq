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
//              6. DEFAULT_SVR()->GetPositionKeySet()-获取持仓记录键值列表,返回持仓记录的数量
//              7. DEFAULT_SVR()->GetPositionData()-获取指定合约、方向、投保的持仓数据
//              8. DEFAULT_SVR()->GetCurPrice()-获取指定合约的现价
//          配置模块CfgMgr
//              1. CfgMgr::GetInstance()->GetListCfg(GID_POSITION)
//              2. CfgMgr::GetInstance()->GetListColCount(GID_POSITION)
//              3. CfgMgr::GetInstance()->GetListColCfg(GID_POSITION, i);
//          配置程序ConfigApp
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_LOAD_CONFIG_APP) - 表格列设置
//          报价表模块
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_GRID_INSTRUMENTID_CHANGED) - 当前合约设置
//          下单板模块
//              1. GETTOPWINDOW()->ProcessEvent(wxEVT_ORDERINSERT_ORDERCHANGE) - 市价反手
//              2. GETTOPWINDOW()->ProcessEvent(wxEVT_ORDERINSERT_MOUSECLOSE) - 鼠标快速平仓
//          顶层窗口模块
//              1. ((MainFrame*)GETTOPWINDOW())->GetStyle() - 取界面类型
//************************************************************************************

#pragma once

#include "..\inc\Module-Misc\ExtListCtrl.h"
#include "..\\cfgMgr\\CfgMgr.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/EventSet.h"

class zqWriteLog;


typedef std::multimap<std::string, PlatformStru_Position > PositionMAP;
class CMovePositionDlg;
class CSwapPositionDlg;

class CMAPositionInfoList : public wxExtListCtrl
{

public:
	CMAPositionInfoList(wxWindow *parent,
					   wxWindowID id = wxID_ANY,
					   const wxPoint& pos = wxDefaultPosition,
					   const wxSize& size = wxDefaultSize,
					   long style = wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_TOGGLE_COLOUR,
					   const wxValidator& validator = wxDefaultValidator,
					   const wxString& name = wxEmptyString);


	virtual ~CMAPositionInfoList(void);

    void ShowAll(IPlatformSingleSvr* pSvr=NULL);

    //配置listctrl的参数
	void Init();

	std::string GetItemText(int row, int col);
	//移仓
    void OnMovePosition();
	//仓位互换
    void OnSwapPosition();
	void OnUpdatePosMoveSwapSwap(wxCommandEvent& evt);
	void OnContext(wxContextMenuEvent& evt);
    void OnCfgChanged();
	void OnLanguageChanged(int langid);
	void OnActivated(wxExtListEvent& event);
	void OnSelected(wxExtListEvent& event);
	void OnColumnResize(wxExtListEvent& event);
	DECLARE_EVENT_TABLE();

	void SaveColWidth();

private:
	CMovePositionDlg*   m_pMovePositionDlg;
	CSwapPositionDlg*   m_pSwapPositionDlg;

public:
    void OnRspQryPositionLast(wxCommandEvent& evt);
    void OnGetMarginRate(wxCommandEvent& evt);
    void OnRtnTrade(wxCommandEvent& evt);
    void OnRtnOrder(wxCommandEvent& evt);
    void OnRtnDepthMarketData(wxCommandEvent& evt);
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);
	void ClearAllItemsAndData();
	void UpdateInstrumentOfSvr( IPlatformSingleSvr* pSvr, std::string strInstru, bool bRowsChanged );
	void UpdateCellFromItemData(int row);
	bool ReQry();
	set<IPlatformSingleSvr*>* m_ppSvrSet;
    zqWriteLog*           m_pWriteLog;

private:
	static CMAPositionInfoList *m_PostionList;
	wxColor m_TextColor;
	std::map<int, int> m_ColumnID2FieldID; //记录字段在哪一列显示 


};