#pragma once
#include "stdafx.h"
#include <wx/spinctrl.h>
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/CLocalOrderService.h"


enum
{
	ID_ADDPOSIPL_OK=3100,
	ID_ADDPOSIPL_CANCEL,
	ID_ADDPOSIPL_CLOSEMODE_COMBOBOX,
	ID_ADDPOSIPL_PRICE_COMBOBOX,
	ID_ADDPOSIPL_CONDPRICETYPE_COMBOBOX,
	ID_ADDPOSIPL_NEWVOLUME_TEXT,
	ID_ADDPOSIPL_STOPGAINPRICEOFFSET_TEXT,
	ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_TEXT,
	ID_ADDPOSIPL_DOPRICEOFFSET_TEXT,
	ID_ADDPOSIPL_STOPGAIN_CHECK,
	ID_ADDPOSIPL_STOPLOSE_CHECK,
	ID_ADDPOSIPL_TOSERVER_CHECK,
	ID_ADDPOSIPL_DOPRICEOFFSET_RADIO,
	ID_ADDPOSIPL_DOPRICELIMIT_RADIO,
	ID_ADDPOSIPL_NEWVOLUME_SPINCTRL,
	ID_ADDPOSIPL_STOPGAINPRICEOFFSET_SPINCTRL,
	ID_ADDPOSIPL_STOPLOSEPRICEOFFSET_SPINCTRL,
	ID_ADDPOSIPL_DOPRICEOFFSET_SPINCTRL,

	ID_ADDPOSIPL_POSITIONINFO_LABEL,
	ID_ADDPOSIPL_CLOSEMODE_LABEL,
	ID_ADDPOSIPL_QTY_LABEL,
	ID_ADDPOSIPL_PRICE_LABEL,
	ID_ADDPOSIPL_PRICE_LABEL2,
	ID_ADDPOSIPL_STOPGAINPRICE_LABEL,
	ID_ADDPOSIPL_STOPLOSEPRICE_LABEL,
	ID_ADDPOSIPL_CONDPRICE_LABEL,
	ID_ADDPOSIPL_POSIVOLUME_LABEL,		// 平今/平昨可平量，条件单平今/平昨量，可下止盈止损数量
	ID_ADDPOSIPL_TARGETPRICE_LABEL,

	ID_ADDPOSIPL_DETAIL1_LABEL,
	ID_ADDPOSIPL_DETAIL2_LABEL,
	ID_ADDPOSIPL_DETAIL3_LABEL,
	
	ID_ADDPOSIPL_STATICBOX1,
	ID_ADDPOSIPL_STATICBOX2,
	ADDPOSIPLSUMNUM_ID	// 标志ID号的数量
};

class CAddOrderPosiPLDailog : public wxDialog
{
public:
	CAddOrderPosiPLDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CAddOrderPosiPLDailog();

    virtual bool Show( bool show = true );

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
	void OnCreateWindow(wxWindowCreateEvent& evt);
	void OnDestroyWindow(wxWindowDestroyEvent& evt);
    void OnTimer(wxTimerEvent& event);
	void OnDialogCharHook(wxKeyEvent& evt);
	void OnCmbOpenClose(wxCommandEvent& evt);
	void OnVolumeSpinUp(wxSpinEvent& event);
	void OnVolumeSpinDown(wxSpinEvent& event);
	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);

DECLARE_EVENT_TABLE()

public:

	void SetPositionInfo(PlatformStru_Position& positionInfo) {
		m_positionInfo = positionInfo;
	};
	PlatformStru_Position& GetPositionInfo() {
		return m_positionInfo;
	};

	void SetPLDetailOrder(CLocalOrderService::PLDETAILORDER& tPLDetailOrder) {
		m_tPLDetailOrder = tPLDetailOrder;
	};
	CLocalOrderService::PLDETAILORDER& GetPLDetailOrder() {
		return m_tPLDetailOrder;
	};

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};

	BOOL IsAbsCondPrice() {
		return m_bIsAbsCondPrice;
	};

	void SetUpperLimitPrice(double fPrice) {
		m_fUpperLimitPrice = fPrice;
	};
	double GetUpperLimitPrice() {
		return m_fUpperLimitPrice;
	};
	void SetLowerLimitPrice(double fPrice) {
		m_fLowerLimitPrice = fPrice;
	};
	double GetLowerLimitPrice() {
		return m_fLowerLimitPrice;
	};

	void SetOpenClose(int nOCMode);
	int GetOpenClose();
	void SetPriceMode(int nPriceMode);
	int GetPriceMode();
	void SetQty(int qty);
	int GetQty();

	// 获得止盈价格信息
	BOOL GetGainOffsetPrice(double& fOffsetPrice);
	void SetGainOffsetPrice(BOOL bIsStopGain, double fOffsetPrice);
	// 获得止损价格信息
	BOOL GetLoseOffsetPrice(double& fOffsetPrice);
	void SetLoseOffsetPrice(BOOL bIsStopLose, double fOffsetPrice);

	// 触发价格类型，1 最新价，2 买卖价，3 买一价，4 卖一价
	int GetCondPriceType();
	void SetCondPriceType(int nPriceType);

	// 平仓方式，1 反向涨跌停价，2 反向加减
	int GetCloseMode();
	void SetCloseMode(int nCloseMode);
	double GetCloseOffsetPrice();
	void SetCloseOffsetPrice(double fOffsetPrice);

	// 设置是在本地执行还是服务器上执行
	void SetIsRunInServer(BOOL bIsRunInServer);
	BOOL GetIsRunInServer();

	BOOL Init(BOOL bIsAbsCond);
	
private:
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit);
	
	void ShowTradeInfoDlg(const wxString& Title, const wxString& errormessage, BOOL bIsShow);
	void ShowOrderReq(BOOL bIsToday);
	void ShowOrderPLDetail();
	BOOL MakePosiPL(std::string& strAccount, std::string& strInstrumentID,
					BOOL bIsBuy, int nOCMode, double fPrice, int nVolume, 
					CLocalOrderService::PLDETAILORDER& tPLDetail);
	void CalcVolume();
	int GetCondVolume(BOOL bIsServ, CLocalOrderService::ORDERREF& condRef, string& strInstruemtID);
	int CalcPLCondVolume(CLocalOrderService::PLDETAILORDER& plOrder);
	int CalcCanPLVolume(BOOL bIsToday);
	wxString GetCanCloseMsg(BOOL bIsToday);
	wxString GetStaticText(UINT nUIID);
	wxString GetUserInputText();
	wxString GetReqLog();

public:
	static wxWindow*			sm_poDailog;
	static HHOOK				sm_hDailogKeyBoard;

private:
	CLocalOrderService*			m_poLocalOrderService;
    wxTimer						m_timer;//行情更新定时器

	BOOL						m_bIsAbsCondPrice;
	BOOL						m_bInitToday;
	double						m_fPriceTick;
	double						m_fUpperLimitPrice;
	double						m_fLowerLimitPrice;
	int							m_nPosiCanCloseToday;
	int							m_nPosiCanCloseYsday;
	int							m_nPosiPLCloseToday;
	int							m_nPosiPLCloseYsday;
	int							m_nCondCloseToday;
	int							m_nCondCloseYsday;

    std::vector<unsigned int>	m_TabOrderVector;
	PlatformStru_Position		m_positionInfo;
	CLocalOrderService::PLDETAILORDER		m_tPLDetailOrder;

};