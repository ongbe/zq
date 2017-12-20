#pragma once
#include "stdafx.h"
#include <wx/spinctrl.h>
#include "CBaseInputPanel.h"

//enum
//{
//	ID_CONDITION_RADIONEWPRICE=1600,
//	ID_CONDITION_RADIOBUYPRICE,
//	ID_CONDITION_RADIOSELLPRICE,
//	ID_CONDITION_COMBOCONDITION,
//	ID_CONDITION_TEXTPRICE,
//	ID_CONDITION_SPINCTRLBUTTON,
//	ID_CONDITION_CHECKTOSERVER,
//	OCONDITIONSUMNUM_ID	// 标志ID号的数量
//};

class COrderConditionPanel :public CBaseInputPanel
{
public:
	enum {	conROW_HEIGHT=13,
			conROW_WIDTH=12
	};

public:

    COrderConditionPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
   ~COrderConditionPanel();

	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnHookKeyPress(wxKeyEvent& evt);
	DECLARE_EVENT_TABLE()
public:
    static COrderConditionPanel* CreatePane(wxWindow* parent);

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	void SetPriceType(int nPriceType);
	int GetPriceType();

	void SetConditionType(int nConditionType);
	int GetConditionType();

	void SetConditionPrice(double fPrice);
	double GetConditionPrice();

	// 设置是在本地执行还是服务器上执行
	void SetIsRunInServer(BOOL bIsRunInServer);
	BOOL GetIsRunInServer();

	void ResetLanguage();
	wxString GetUserInputText();

private:
	double	m_fPriceTick;

};