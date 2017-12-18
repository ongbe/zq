#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"
#include <wx/spinbutt.h>

enum
{
	ID_EDITORDERREPLACE_OK=1850,
	ID_EDITORDERREPLACE_CANCEL,
	ID_EDITORDERREPLACE_NEWPRICE_TEXT,
	ID_EDITORDERREPLACE_NEWPRICE2_TEXT,
	ID_EDITORDERREPLACE_NEWVOLUME_TEXT,
    ID_EDITORDERREPLACE_NEWPRICE_SPINCTRL,
    ID_EDITORDERREPLACE_NEWPRICE2_SPINCTRL,
	ID_EDITORDERREPLACE_NEWVOLUME_SPINCTRL,
	ID_EDITORDERREPLACE_TRADERACCOUNT_COMBO,
	ID_EDITORDERREPLACE_ORDERTYPE_LABEL,
	ID_EDITORDERREPLACE_STATIC1,
	ID_EDITORDERREPLACE_STATIC2,
	ID_EDITORDERREPLACE_STATIC3,
	ID_EDITORDERREPLACE_STATIC4,
	ID_EDITORDERREPLACE_STATIC5,
	ID_EDITORDERREPLACE_STATIC6,
	ID_EDITORDERREPLACE_STATIC7,
	ID_EDITORDERREPLACE_STATIC8,
	EDITORDERRREPLACESUMNUM_ID	// 标志ID号的数量
};

class CEditOrderReplaceDailog : public wxDialog
{
public:
	CEditOrderReplaceDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CEditOrderReplaceDailog();

    virtual bool Show(bool show = true);

    void OnOk(wxCommandEvent& evt);
    void OnCancel(wxCommandEvent& evt);
	void OnCreateWindow(wxWindowCreateEvent& evt);
	void OnDestroyWindow(wxWindowDestroyEvent& evt);
	void OnDialogCharHook(wxKeyEvent& evt);
	void OnPriceSpinUp(wxSpinEvent& evt);
	void OnPriceSpinDown(wxSpinEvent& evt);
	void OnPrice2SpinUp(wxSpinEvent& evt);
	void OnPrice2SpinDown(wxSpinEvent& evt);
	void OnVolumeSpinUp(wxSpinEvent& evt);
	void OnVolumeSpinDown(wxSpinEvent& evt);
	DECLARE_EVENT_TABLE()

public:

	void SetFtdcOrderField(PlatformStru_OrderInfo& ftdcOrderField) {
		m_tftdcOrderField = ftdcOrderField;
		Convert(m_tftdcOrderField, m_tftdcOrderInputField);
	};
	PlatformStru_OrderInfo& GetFtdcOrderField() {
		return m_tftdcOrderField;
	};

	PlatformStru_InputOrder& GetFtdcOrderInputField() {
		return m_tftdcOrderInputField;
	};

	double GetNewPrice() {
		return GetPrice();
	};
	double GetNewPrice2() {
		return GetPrice2();
	};
	BOOL GetNewIsMarketPrice() {
		return GetIsPriceMode();
	};
	int GetNewVolume() {
		return GetQty();
	};
	wxString GetNewTraderAccount() {
		return GetTraderAccount();
	};

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	
private:
	void ShowOrderReq();
	void Convert(PlatformStru_OrderInfo& tftdcOrderField, 
			PlatformStru_InputOrder& tftdcOrderInputField);
	wxString GetStaticText(UINT nUIID);

	wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit);
	wxTextCtrl* CreateFloatSpinCtrl_Price2(wxPoint& ctrPosInit);
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit);
	
    void SetPrice(double price);
    double GetPrice();
    void SetPrice2(double price);
    double GetPrice2();
	BOOL GetIsPriceMode();				// 价格是否为市价
    void SetQty(int qty);				// 设置手数
    int GetQty();						// 读取手数
	void SetTraderAccount(wxString& strTraderAccount);
	wxString GetTraderAccount();

public:
	static wxWindow*	sm_poDailog;
	static HHOOK		sm_hDailogKeyBoard;

private:
	double						m_fPriceTick;
	PlatformStru_OrderInfo		m_tftdcOrderField;
	PlatformStru_InputOrder		m_tftdcOrderInputField;
	std::vector<unsigned int>	m_TabOrderVector;

};
