#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"
#include <wx/spinbutt.h>

enum
{
	ID_EDITORDERCHANGE_OK=1850,
	ID_EDITORDERCHANGE_CANCEL,
	ID_EDITORDERCHANGE_NEWPRICE_TEXT,
	ID_EDITORDERCHANGE_NEWVOLUME_TEXT,
    ID_EDITORDERCHANGE_NEWPRICE_SPINCTRL,
	ID_EDITORDERCHANGE_NEWVOLUME_SPINCTRL,
	ID_EDITORDERCHANGE_STATIC1,
	ID_EDITORDERCHANGE_STATIC2,
	ID_EDITORDERCHANGE_STATIC3,
	ID_EDITORDERCHANGE_STATIC4,
	ID_EDITORDERCHANGE_STATIC5,
	ID_EDITORDERCHANGE_STATIC6,
	ID_EDITORDERCHANGE_STATIC7,
	EDITORDERCHANGESUMNUM_ID	// 标志ID号的数量
};

class CEditOrderChangeDailog : public wxDialog
{
public:
	CEditOrderChangeDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CEditOrderChangeDailog();

    virtual bool Show(bool show = true);

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
	void OnCreateWindow(wxWindowCreateEvent& evt);
	void OnDestroyWindow(wxWindowDestroyEvent& evt);
	void OnDialogCharHook(wxKeyEvent& evt);
	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnVolumeSpinUp(wxSpinEvent& event);
	void OnVolumeSpinDown(wxSpinEvent& event);
	DECLARE_EVENT_TABLE()

public:

	void SetFtdcOrderField(PlatformStru_OrderInfo& ftdcOrderField) {
		m_tftdcOrderField = ftdcOrderField;
	};
	PlatformStru_OrderInfo& GetFtdcOrderField() {
		return m_tftdcOrderField;
	};
	double GetNewPrice() {
		//return m_fNewPrice;
		return GetPrice();
	};
	BOOL GetNewIsMarketPrice() {
		return GetIsPriceMode();
		//return m_bNewIsMarketPrice;
	};
	int GetNewVolume() {
		return GetQty();
		//return m_nNewVolume;
	};

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	
private:
	void ShowOrderReq();

	wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit);
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit);
	
    void SetPrice(double price);
    double GetPrice();
	BOOL GetIsPriceMode();				// 价格是否为市价
    void SetQty(int qty);				// 设置手数
    int GetQty();						// 读取手数

	wxString GetStaticText(UINT nUIID);

public:
	static wxWindow*	sm_poDailog;
	static HHOOK		sm_hDailogKeyBoard;

private:
	double						m_fPriceTick;
	double						m_UpperLimitPrice;
	double						m_LowerLimitPrice;
	int							m_nNewVolume;
	BOOL						m_bNewIsMarketPrice;
	double						m_fNewPrice;
	PlatformStru_OrderInfo		m_tftdcOrderField;
    std::vector<unsigned int>	m_TabOrderVector;

};