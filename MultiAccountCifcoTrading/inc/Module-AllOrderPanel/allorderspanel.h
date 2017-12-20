#pragma once

#include "COrderListPanel.h"
#include "../inc/Module-Misc/tools_util.h"
#include "../inc/Module-Misc/TradeInfoDlg.h"
#include "../Module-Misc/DataListCtrl.hpp"


#define  ALLORDER_MIN                      0        //最小
#define  ALLORDER_MAX                      ALLORDER_UserProductInfo        //最大


#define  ALLORDER									'A'        //用于显示所有的委托单
#define  OPENORDER									'O'        //用于显示挂单的委托单
#define  TRADEDORDER								'T'        //用于显示已成交的委托单
#define  CANCELORDER								'C'        //用于显示撒销和错误的委托单
#define  TIMER_CYCLE								1000       //用于取合约保证金率和手续费率时的间隔时间




class CAllOrdersPanel : public COrderListPanel//wxPanel
{
public:
	enum { conAllOrderStyle, conOpenOrderStyle };
	enum { conNoticeNewOrderSuccess, conNoticeNewOrderFail, conNoticeTrader, conNoticeCancelSuccess, conNoticeCancelFail };

public:
    CAllOrdersPanel(int nViewStyle, wxWindow *parent,
            wxWindowID winid,//wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CAllOrdersPanel(void);

    void ShowAll();

private:
	std::map<OrderKey,  CThostFtdcOrderField> outData;
	OrdersPanelMAP m_OrdersPanelMap;

	BOOL NeedOrderSuccessConfirm();
	BOOL NeedOrderSuccessSound();
	BOOL NeedOrderFailConfirm();
	BOOL NeedOrderFailSound();
	BOOL NeedOrderTradeConfirm();
	BOOL NeedOrderTradeSound();
	BOOL NeedCancelSuccessConfirm();
	BOOL NeedCancelSuccessSound();
	BOOL NeedCancelFailConfirm();
	BOOL NeedCancelFailSound();
	bool NeedHideCancelButton();
	void NoticeSound(int nType);
	void OnSubscrible(wxCommandEvent& evt);
	void Init();
	void OnRspOrderAction1(wxCommandEvent& evt);
	void OnRspOrderAction2(wxCommandEvent& evt);
	void OnRcvTrade(wxCommandEvent& evt);
    void OnPanelCharHook(wxCommandEvent& evt);
    //报单查询回报
    static int RspQryOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
    static int RtnOrderCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //报单回报
	static int RspOrderAction1CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //撤单回执1
	static int RspOrderAction2CallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //撤单回执2
	static int TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //成交回报
    static int OrderInsertBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);	  //报单录入回报
    void OnCfgChanged(wxCommandEvent& evt);////配置文件发生变化
	void  PopupTradeDlg(IPlatformSingleSvr* pSvr,const PlatformStru_TradeInfo& rawData);
    void  PopupCancel_InsertDlg(IPlatformSingleSvr* pSvr,const PlatformStru_OrderInfo& rawData);
	int  UpdateActionRet(const PlatformStru_TradeInfo& rawData);
	void ShowInsertDlg(const string& Title, const DataRspOrderInsert& OrderInfo, BOOL bIsShow);

	void OnActivated(wxExtListEvent& event);
	void OnSize( wxSizeEvent& event );
	void OnRadioButton(wxCommandEvent& event);	
	void OnRemove(wxCommandEvent& evt);
    void OnRemoveAll(wxCommandEvent& evt);
    void OnReQry(wxCommandEvent& evt);
    void OnReConnectQry(wxCommandEvent& evt);
    void OnContext(wxContextMenuEvent& evt);
    void OnOrderInsert(wxCommandEvent& evt);
	void OnPanelFocus(wxCommandEvent& evt);
	void OnColumnResize(wxExtListEvent& event);

    //获得某些合约的费率，需要更新相应的报单的冻结数值
    void OnGetCommissionRate(wxCommandEvent& evt);
    void OnGetMarginRate(wxCommandEvent& evt);
    //保证金率回报
    static int GetMarginRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
    //手续费率回报
    static int GetCommissionRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);
    void OnRtnOrder(wxCommandEvent& evt);
    //报单查询响应
    void OnRspQryOrder(wxCommandEvent& evt);
    static bool UpdateListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);
    static bool UpdateOpenListItemCallBack(const void* pNewData,void* pOldData,unsigned long textcol,int FieldID,int ColID,int RowID,bool bnewline,wxExtListItem& outItem);

#ifdef _USE_MULTI_LANGUAGE
	void OnLanguageChanged(wxCommandEvent& evt);
#endif

	void SaveColWidth();
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);
private:
	LPLIST_TABLE_CFG       m_pListCfg;
	WX_DECLARE_STRING_HASH_MAP(unsigned int, ID2RowMAP);
	std::map<int, long> m_FieldID2ColumnID; //记录字段在哪一列显示 


private:

	int		m_nViewStyle;
	GUIModuleID		m_nGID;

	CThostFtdcOrderField  m_OneOrderList;
	PlatformStru_TradeInfo  m_OneTradeList;

	wxButton *     m_pButtonRemove;
	wxButton *     m_pButtonRemoveAll;
    wxButton *     m_pButtonReQry;
	wxRadioButton *m_WxRadioButton1;
	wxRadioButton *m_WxRadioButton2;
	wxRadioButton *m_WxRadioButton3;
	wxRadioButton *m_WxRadioButton4;

	CDataListCtrl<OrderKey,PlatformStru_OrderInfo>*  m_pwxExtListCtrl;

	wxFont m_Font;
	wxColor m_HeadBackgroundColor;
	wxColor m_HeadColor;
	wxColor m_BackgroundColor;
	wxColor m_TextColor;
    wxColor m_EvenLineBgColor;
	wxColor m_OddLineBgColor;
    CTradeInfoDlg* m_pTradeInfoDlg;

    char            m_ShowType;          //当前显示内容，所有/挂单/成交/ 撤单
    unsigned long   m_TextColor2;  //字体颜色

	set<IPlatformSingleSvr*> m_pSvrSet;
public:
	enum
		{	//GUI Enum Control ID Start
			ID_BUTTON_REMOVE = 1011,
			ID_BUTTON_REMOVEALL = 1012,	
			ID_BUTTON_REQRY = 1013,	
			ID_WXRADIOBUTTON1 = 1006,
			ID_WXRADIOBUTTON2 = 1007,
			ID_WXRADIOBUTTON3 = 1008,
			ID_WXRADIOBUTTON4 = 1009,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

DECLARE_EVENT_TABLE()
};


