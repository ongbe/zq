#pragma once
#include "stdafx.h"
#include "../inc/Module-Misc/orderDefines.h"
#include "COrderBatchPanel.h"
#include "COrderConditionPanel.h"
#include "COrderPositionPanel.h"
#include "COrderStopPanel.h"
#include "COrderRealMsgPanel.h"
#include "COrderInsertCommandPanel.h"
#include "COrderInputPanel.h"
#include "CPatsOrderInputPanel.h"
#include "../inc/Order-Common/CStaticFivePriceList.h"

class COrderInsertView : public wxScrolledWindow
{
public:

	COrderInsertView(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style =wxScrolledWindowStyle | wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~COrderInsertView();

	int GetViewStyle() {
		return m_nViewStyle;
	};
	virtual void SetViewStyle(int nViewStyle, int nViewSubStyle) {
		m_nViewStyle = nViewStyle;
		m_nViewSubStyle = nViewSubStyle;
		m_poOrderInputPanel->SetViewStyle(nViewStyle);
	};
	int GetViewSubStyle() {
		return m_nViewSubStyle;
	};
	virtual void SetViewSubStyle(int nViewSubStyle) {
		m_nViewSubStyle = nViewSubStyle;
	};

	void BringWindow2TopAlways(BOOL b2Top=TRUE);
	void ShowViewStyle(int nViewStyle, int nViewSubStyle=0) {
		m_poOrderInputPanel->SetViewStyle(nViewStyle);
		if((m_nViewStyle != nViewStyle) 
			|| (m_nViewStyle == nViewStyle && m_nViewSubStyle != nViewSubStyle)) {
			LoadStyleUI(nViewStyle, nViewSubStyle);
			SetViewStyle(nViewStyle, nViewSubStyle);
		}
	};
	void ShowViewSubStyle(int nViewSubStyle) {
		if(m_nViewSubStyle != nViewSubStyle) {
			LoadStyleUI(m_nViewStyle, nViewSubStyle);
			SetViewSubStyle(nViewSubStyle);
		}
	};

    void EnablePriceInputCtl(bool bEnabled)
    {
        if(m_poOrderInputPanel!=NULL)
            m_poOrderInputPanel->EnablePriceInputCtl(bEnabled);
    }


//DECLARE_EVENT_TABLE()

protected:
	wxString	GetInputInstrumentID();
	void		SetInputInstrumentID(wxString& strInstrumentID, BOOL bSendChangeEvent=TRUE);
	wxString	GetInputAccount();
	void		SetInputAccount(wxString& strAccount);
	void		SetInputOrderType(int nOrderType);
	int			GetInputOrderType();
	double		GetInputPrice();
	void		SetInputPrice(double fPrice);
	wxString	GetInputPriceText();
	void		SetInputPriceText(wxString& strPrice);
	void		SetInputLimitPriceText(wxString& strPrice);
	wxString	GetInputLimitPriceText();
	void		SetInputLimitPrice(double price);
	double		GetInputLimitPrice();
	int			GetInputQty();
	void		SetInputQty(int nQty);
	BOOL		GetInputDirection();
	void		SetInputDirection(BOOL bDirection);
	int			GetInputOCMode();
	void		SetInputOCMode(int nOCMode);
	BOOL		GetInputHedge();
	void		SetInputHedge(BOOL bHedge);
	BOOL		GetInputAutoOpenClose();
	void		SetInputAutoOpenClose(BOOL bIsAuto);
	void		SetInputAutoTrackPrice(BOOL bIsTrack);	// 设置价格跟盘模式
	BOOL		GetInputAutoTrackPrice();
	void		SetInputAutoTrackLimitPrice(BOOL bIsTrack);// 设置价格跟盘模式
	BOOL		GetInputAutoTrackLimitPrice();

	BOOL		GetConditionSelect();
	BOOL		GetPositionSelect();
	void		SetConditionSelect(BOOL bIsEdit); 
	void		SetStopPositionSelect(BOOL bIsEdit);

    virtual void		LoadStyleUI(int nViewStyle, int nViewSubStyle);

protected:
	int			m_nViewStyle;		//界面风格： 0~2 快期，3 金仕达
	int			m_nViewSubStyle;	//界面风格： 金仕达小窗体风格： 0 标准下单, 1 批量下单, 2 条件单, 3 赢损单，4 停止单

	COrderInputPanel*			m_poOrderInputPanel;
	CPatsOrderInputPanel*		m_poPatsOrderInputPanel;
	COrderBatchPanel*			m_poOrderBatchPanel;
	COrderConditionPanel*		m_poOrderConditionPanel;
	COrderPositionPanel*		m_poOrderPositionPanel;
	COrderStopPanel*			m_poOrderStopPanel;
	COrderRealMsgPanel*			m_poOrderRealMsgPanel;
	COrderInsertCommandPanel*	m_poOrderInsertCommandPanel;
	CStaticFivePriceList*		m_poStaticFivePriceList;

    std::vector<unsigned int>	m_TabOrderVector;

	wxColor		m_colorBak;
	wxPoint		m_posWD;
	wxPoint		m_posOrder;

	static UIENTITY s_tStdView[conUI_MAX];
	static UIENTITY s_tPositionView[conUI_MAX];
	static UIENTITY s_tBatchView[conUI_MAX];
	static UIENTITY s_tConditionView[conUI_MAX];
	static UIENTITY s_tStopView[conUI_MAX];
	static UIENTITY s_tPatsView[conUI_MAX];

};
