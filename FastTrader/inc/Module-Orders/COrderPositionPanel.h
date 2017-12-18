#pragma once
#include "stdafx.h"
#include <wx/spinctrl.h>
#include "../inc/Module-Misc/orderDefines.h"
#include "CBaseInputPanel.h"

// 必要时把类名由COrderPositionPanel改为COrderStopPanel，这才是个合理的止盈止损的概念
class COrderPositionPanel :public CBaseInputPanel
{
public:
	enum {	conROW_HEIGHT=13,
			conROW_WIDTH=12
	};

public:

    COrderPositionPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL,
            const wxString& name = wxPanelNameStr);
   ~COrderPositionPanel();

	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnCheckDoStopClicked(wxCommandEvent& event);
	void OnHookKeyPress(wxKeyEvent& evt);
	DECLARE_EVENT_TABLE()
public:
    static COrderPositionPanel* CreatePane(wxWindow* parent);

	void SetViewMode(int nViewMode);

	void InitPositionPanel();
	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	double GetPriceTick() {
		return m_fPriceTick;
	};
	// 价格偏移对象，成交价/委托价
	int GetOffsetPriceObject();
	void SetOffsetPriceObject(int nPriceObject);
	// 获得止损价格信息
	BOOL GetLoseOffsetPrice(double& fOffsetPrice);
	void SetLoseOffsetPrice(BOOL bIsStopLose, double fOffsetPrice);
	// 获得止盈价格信息
	BOOL GetGainOffsetPrice(double& fOffsetPrice);
	void SetGainOffsetPrice(BOOL bIsStopGain, double fOffsetPrice);
	// 触发价格类型，1 最新价，2 买卖价
	int GetPriceType();
	void SetPriceType(int nPriceType);
	// 平仓方式，1 反向涨跌停价，2 反向加减
	int GetCloseMode();
	void SetCloseMode(int nCloseMode);
	double GetCloseOffsetPrice();
	void SetCloseOffsetPrice(double fOffsetPrice);

	// 设置是在本地执行还是服务器上执行
	void SetIsRunInServer(BOOL bIsRunInServer);
	BOOL GetIsRunInServer();

	void ResetLanguage();
	wxString GetUserInputText();

private:
	int		m_nViewMode;
	double	m_fPriceTick;

};