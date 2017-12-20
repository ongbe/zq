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
#include "../Module-Misc/DataListCtrl.hpp"

using namespace std;


class CPositionDetailInfoList : public CDataListCtrl<PositionDetailKey,PlatformStru_PositionDetail>
{
public:
	CPositionDetailInfoList(wxWindow *parent,
           wxWindowID id = wxID_ANY,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = wxLCEXT_REPORT|wxLCEXT_VRULES|wxLCEXT_TOGGLE_COLOUR,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxEmptyString);

public:
	virtual ~CPositionDetailInfoList(void);

    void ShowAll();

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
    static bool UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);
	void SaveColWidth();
	set<IPlatformSingleSvr*>* m_ppSvrSet;

	DECLARE_EVENT_TABLE();


private:	
	static CPositionDetailInfoList *m_PostionDetailList;
	
    unsigned long   m_TextColor;  //字体颜色
	std::map<int, long> m_FieldID2ColumnID; //记录字段在哪一列显示 

    //每个合约对应的上一次持仓明细数据的更新序号
    //每次需要更新数据时，如果当前的UpdateSeq等于这个值，表示不需要更新
    //因为底层发给UI的数据更新指令可能会阻塞和错序，需要用这个机制避免无效更新
    std::map<std::string,long> m_PositionDetailDataLastUpdateSeq;

    zqWriteLog*           m_pWriteLog;


};