#pragma once

#include "../ISvr/BusinessData.hpp"

class CMAPositionDetailInfoList;
class CMAPositionInfoList;
class CComboPositionInfoList;

enum
{
	ID_BUTTON_MOVEPOSITION=1000,
    ID_BUTTON_SWAPPOSITION,
    ID_BUTTON_REQRY,
	ID_WXRADIOBUTTON_Position,
	ID_WXRADIOBUTTON_PositionDetail,
	ID_WXRADIOBUTTON_PositionComb,
};

enum EnumPositionShowType
{
    PositionShowType_Position,
    PositionShowType_PositionDetail,
    PositionShowType_PositionComb
};



class PositionPanel : public wxPanel
{

public:
	PositionPanel(wxWindow *parent,
            wxWindowID winid = GID_POSITIONALL,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
    virtual ~PositionPanel();   

	void OnPanelFocus(wxCommandEvent& evt);
    void OnSize(wxSizeEvent& evt);
    void OnCfgChanged( wxCommandEvent& evt );
    void OnLanguageChanged( wxCommandEvent& event );
    void OnReQry(wxCommandEvent& evt);
    void OnReConnectQry(wxCommandEvent& evt);
	void OnRadioButton(wxCommandEvent& event);	
	void OnMAPlatformAdd(wxCommandEvent& event);
	void OnMAPlatformDelete(wxCommandEvent& evt);
	void OnMovePosition(wxCommandEvent& evt);
	void OnSwapPosition(wxCommandEvent& evt);
	void OnUpdatePosMoveSwapSwap(wxCommandEvent& evt);

	void OnSubscrible(wxCommandEvent& evt);


    //查询持仓响应的回调函数
    static int CallBackFunc_BID_RspQryInvestorPosition(const GUIModuleID GID,const AbstractBusinessData& data);
    //查询持仓明细响应的回调函数
    static int CallBackFunc_BID_RspQryInvestorPositionDetail(const GUIModuleID GID,const AbstractBusinessData& data);
	//查询组合持仓响应的回调函数
	static int CallBackFunc_BID_RspQryInvestorComboPosition(const GUIModuleID GID,const AbstractBusinessData& data);
    //查询保证金率响应的回调函数
    static int CallBackFunc_BID_RspQryInstrumentMarginRate(const GUIModuleID GID,const AbstractBusinessData& data);
    //成交回报的回调函数
    static int CallBackFunc_BID_RtnTrade(const GUIModuleID GID,const AbstractBusinessData& data);
    //报单回报的回调函数
    static int CallBackFunc_BID_RtnOrder(const GUIModuleID GID,const AbstractBusinessData& data);
    //行情回调函数部分更新
    static int CallBackFunc_BID_RtnDepthMarketData(const GUIModuleID GID,const AbstractBusinessData& data);

	DECLARE_EVENT_TABLE();

private:

   	wxRadioButton *m_WxRadioButton_Position;
	wxRadioButton *m_WxRadioButton_PositionDetail;
	wxRadioButton *m_wxRadioButton_PositionCombo;

    wxButton* m_pButtonMovePosition;
    wxButton* m_pButtonSwapPosition;

    wxButton* m_pButtonReQry;


    static CMAPositionDetailInfoList* m_PositionDetailList;
    static CMAPositionInfoList* m_PositionList;
	static CComboPositionInfoList* m_ComPositionList;

    bool m_HaveComb;                    //是否支持组合单
    bool m_HaveMoveSwap;                //是否支持移换仓

    EnumPositionShowType m_ShowType;    //当前显示内容，持仓/持仓明细/组合持仓
public:
	static set<IPlatformSingleSvr*> m_pSvrSet;
};