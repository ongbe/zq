#pragma once
#include "stdafx.h"
#include "CBaseInputPanel.h"
#include "CfgMgr/CfgGlobal.h"

//#include "MouseHandyInputPane.h"
#include "CMouseHandyInputPane.h"
#include "MouseHandyInputPaneVolume.h"
//#include "MouseHandyInputPaneDigit.h"
#include "CMouseHandyInputPaneDigit.h"

enum
{
    ID_OIKQ_FIRST=2000,
	ID_OIKQ_INSTRUMENTID_TEXT,
	ID_OIKQ_INSTRUMENTID_COMBOCTRL,
	ID_OIKQ_ACCOUNT_TEXT,
    ID_OIKQ_PRICE_TEXT,
    ID_OIKQ_VOLUME_TEXT,
	ID_OIKQ_BUYSELL_COMBOBOX,
	ID_OIKQ_OPENCLOSE_COMBOBOX,
    ID_OIKQ_PRICE_SPINCTRL,
	ID_OIKQ_VOLUME_SPINCTRL,
	ID_OIKQ_STATIC1,
	ID_OIKQ_STATIC2,
	ID_OIKQ_STATIC3,
	ID_OIKQ_STATIC4,
    ID_OIKQ_AUTOOPENCLOSE_STATIC,
    ID_OIKQ_AUTOTRACKPRICE_STATIC,
    ID_OIKQ_HEDGE_CHECKBOX,
	SUMNUM_OIKQ_ID	// 标志ID号的数量
};

class COrderInputPanel_kq : public CBaseInputPanel//wxPanel
{
public:
	COrderInputPanel_kq(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~COrderInputPanel_kq();
    
	static COrderInputPanel_kq* CreatePane(wxWindow* parent);

	void OnCmbBuySell(wxCommandEvent& evt);
	void OnCmbOpenClose(wxCommandEvent& evt);
	void OnInstrumentIdTextChanged(wxCommandEvent& event);
	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnVolumeSpinUp(wxSpinEvent& event);
	void OnVolumeSpinDown(wxSpinEvent& event);

	void OnTextMaxLen(wxCommandEvent& event);

    void OnStaticLeftUp(wxCommandEvent& event);
    void OnHookKeyPress(wxKeyEvent& evt);

	void OnCodeTextKeyAscii(wxKeyEvent& evt);

	void OnCodePanelKeyAscii(wxKeyEvent& evt);
	void OnVolumePanelKeyAscii(wxKeyEvent& evt);
	void OnPricePanelKeyAscii(wxKeyEvent& evt);

	void OnCodeClicked(wxMouseEvent& evt);
	void OnPriceClicked(wxMouseEvent& evt);
	void OnVolumeClicked(wxMouseEvent& evt);

	void OnNextCodeClicked(wxCommandEvent& evt);
	void OnNextPriceClicked(wxCommandEvent& evt);
	void OnNextVolumeClicked(wxCommandEvent& evt);
	
	void OnMouseInputCode(wxCommandEvent& evt);
	void OnMouseInputDigit(wxCommandEvent& evt);
	void OnMouseInputVolume(wxCommandEvent& evt);
	
	void OnPaint(wxPaintEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:
	void SetViewMode(int nViewMode);

    void SetInstrumentId(wxString& strInstrumentId, BOOL bSendChangeEvent=FALSE);
    wxString GetInstrumentId();
    void SetAccount(wxString& strAccount);
    wxString GetAccount();
    void SetDirection(BOOL bIsBuy);
	BOOL GetDirection();				// 获得买卖方向，TRUE为买入
	void SetOpenClose(int nOCMode);		// 设置开平仓模式，0 开仓，1 平今，2 平仓
	int GetOpenClose();					// 获得开平仓模式，0 开仓，1 平今，2 平仓
    void SetPriceText(wxString& strPrice);
    wxString GetPriceText();
    void SetPrice(double price);
    double GetPrice();
	BOOL GetIsPriceMode();				// 价格是否为市价
    void SetQty(int qty);				// 设置手数
    int GetQty();						// 读取手数
	void SetHedge(BOOL bIsHedge);		// 设置是否保值
	BOOL GetHedge();

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	double GetPriceTick() {
		return m_fPriceTick;
	};

    void SetAutoOpenClose(BOOL bIsAuto);	// 设置自动开平模式
	BOOL GetAutoOpenClose();
    void SetAutoTrackPrice(BOOL bIsTrack);	// 设置价格跟盘模式
	BOOL GetAutoTrackPrice();

    void InitOrderView();					// 设置界面为初始值
    void ClearViewData(int nMode);					// 设置界面为初始值
    void FillOrder(PlatformStru_InputOrder& req);		// 把界面的数据填充到数据结构中
    void ShowOrder(PlatformStru_InputOrder& req);		// 从数据结构中读数据并显示
	void ReloadInstrumentIDList();

	void ResetAccountList(vector<string>& accountList);

	void LockForSetChange() {
		m_bIsLockForSetChange = TRUE;
	};
	void UnlockForSetChange() {
		m_bIsLockForSetChange = FALSE;
	};

private:
    void SetSellBuyColor();
	//BOOL PY2Code(wxString strPY, wxString& strRetCode);

	wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit);
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit);
    wxString Number2String(int num);
    wxString Double2String(double dbl);

	BOOL NeedMouseHandyPane_Code();		// 判断是否需要显示鼠标快捷面板输入代码
	BOOL NeedMouseHandyPane_Volume();	// 判断是否需要显示鼠标快捷面板输入数量
	BOOL NeedMouseHandyPane_Price();	// 判断是否需要显示鼠标快捷面板输入价格
	BOOL NeedInputAbbreviation();		// 可以输入缩写和拼音
	BOOL NeedAutoOCHideRadio();			// 使用自动平仓时，取消开平仓选择

private:

	BOOL	m_bIsInitOK;
	double	m_fPriceTick;
	//int		nViewSubMode;				// 面板风格，0 基本，1 小窗委托，2 批量下单，3 条件单，4 盈损单

    //MouseHandyInputPane*		m_MouseHandyPopupPane;
    CMouseHandyInputPane*		m_MouseHandyPopupPane;
	MouseHandyInputPaneVolume*	m_MouseHandyPopupPaneVolume;
	CMouseHandyInputPaneDigit*	m_MouseHandyPopupPaneDigit;

	BOOL	m_bIsLockForSetChange;
	BOOL	m_bNoKeyChangeText;

	wxColor m_colorBak;

};