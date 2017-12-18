#pragma once
//#include "stdafx.h"
#include "COrderInsertOperations.h"
#include "popmsgwin.h"


class OrderInsertPanel : public COrderInsertOperations
{
public:
	static int RspOrderInsertCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);
	static int TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);	  //成交回报
	static int SubscribeForQuoteCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);	  //询价订阅回报
	static int UnSubscribeForQuoteCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);	  //询价退订回报
	static int ForQuoteRspCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data);	  //询价回报

public:

	OrderInsertPanel(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style =wxScrolledWindowStyle | wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~OrderInsertPanel();

	virtual void SetViewStyle(int nViewStyle, int nViewSubStyle);
	virtual void SetViewSubStyle(int nViewSubStyle);
	void NormalOrder(int nOrderType=OrderType_FT_Immediately, int nMinVolume=1);

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
	void OnBtnParkedOrder(wxCommandEvent& event);
	void OnBtnMarketPrice(wxCommandEvent& event);
	void OnBtnBatchOrder(wxCommandEvent& event);
	void OnBtnConditionOrder(wxCommandEvent& event);
	void OnBtnPositionOrder(wxCommandEvent& event);
	void OnBtnStopOrder(wxCommandEvent& event);
	void OnBtnPatsOrder(wxCommandEvent& event);
	void OnBtnFAKFOKOrder(wxCommandEvent& event);
    void OnInstrumentIdTextChanged(wxCommandEvent& event);
    void OnAutoOpenCloseChanged(wxCommandEvent& event);
    void OnAutoTrackPriceChanged(wxCommandEvent& event);
    void OnBuySellSelChanged(wxCommandEvent& event);
	void OnFivePriceSelected(wxCommandEvent& event);
	void OnCheckWDShowClicked(wxCommandEvent& event);
	void OnCheckWinTopAlwaysClicked(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);
	void OnRcvOrder(wxCommandEvent& evt);
	void OnRcvSubsForQuote(wxCommandEvent& evt);
	void OnRcvUnSubsForQuote(wxCommandEvent& evt);
	void OnRcvForQuoteRsp(wxCommandEvent& evt);
    void OnPanelCharHook(wxCommandEvent& evt);
	void OnCheckDoStopClicked(wxCommandEvent& evt);
	void OnInfoListFocus(wxFocusEvent& evt);

	void OnMAPlatformAdd(wxCommandEvent& evt);
	void OnMAPlatformDelete(wxCommandEvent& evt);
	void OnInitSubscribe(wxCommandEvent& evt);
	void OnCfgChanged(wxCommandEvent& evt);
	void OnPanelFocus(wxCommandEvent& evt);
	void OnCMDGridInstrumentIDChanged(wxCommandEvent& evt);
	void OnCMDViewModeChanged(wxCommandEvent& evt);
	void OnCMDKeyOrder(wxCommandEvent& evt);
	void OnCMDMouseOrder(wxCommandEvent& evt);
	void OnCMDMouseClose(wxCommandEvent& evt);
	void OnCMDOrderChange(wxCommandEvent& evt);
	void OnCMDOrderReplace(wxCommandEvent& evt);
	void OnMouseWheel(wxMouseEvent& evt);
	void DoInitQryStep();
	void OnLanguageChanged(wxCommandEvent& event);
DECLARE_EVENT_TABLE()

public:
    static OrderInsertPanel* CreatePane(wxWindow* parent);

protected:
    wxTimer		m_timer;//行情更新定时器
	bool			m_bUpdateRealMsg_Multiply;
	bool			m_bUpdateRealMsg_Available;
	bool			m_bUpdateRealMsg_OpenVolume;
	bool			m_bUpdateRealMsg_PreClose;
	CPopMsgWin*		m_pPopMsgWnd;
};
