#pragma once
#include "stdafx.h"
#include "CBaseInputPanel.h"
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/CMouseHandyInputPane.h"
#include "../inc/Order-Common/MouseHandyInputPaneVolume.h"
#include "../inc/Order-Common/CMouseHandyInputPaneDigit.h"
#include "../inc/Module-Misc/orderDefines.h"


class CPatsOrderInputPanel : public CBaseInputPanel//wxPanel
{
public:
	CPatsOrderInputPanel(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	~CPatsOrderInputPanel();
    
	static CPatsOrderInputPanel* CreatePane(wxWindow* parent);

//	void OnInstrumentIdListDropdown(wxCommandEvent& evt);

	void OnCmbOrderType(wxCommandEvent& evt);
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
	void OnShow(wxShowEvent& evt);
	void OnCfgChanged(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:
	void SetViewMode(int nViewMode);

	// 合约（交易所、商品、合约日期）
	// 下单类型
	// 方向
	// *开平
	// ***保值
	// 数量
	// 价格
	// 限价
	// 有效期限
	// 帐号
	// 注解
	// 时间
	// 优先权
	
	void SetInstrumentId(wxString& strInstrumentId, BOOL bSendChangeEvent=FALSE);
	wxString GetInstrumentId();
	void SetAccount(wxString& strAccount);
	wxString GetAccount();
	void SetOrderType(int nOrderType);
	int GetOrderType();
	void SetDirection(BOOL bIsBuy);
	BOOL GetDirection();				// 获得买卖方向，TRUE为买入
	void SetOpenClose(int nOCMode);		// 设置开平仓模式，0开仓; 1平今; 2平仓; -1未知
	int GetOpenClose();					// 获得开平仓模式
	void SetPriceText(wxString& strPrice);
	wxString GetPriceText();
	void SetPrice(double price);
	double GetPrice();
	void SetLimitPriceText(wxString& strPrice);
	wxString GetLimitPriceText();
	void SetLimitPrice(double price);
	double GetLimitPrice();
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
	void SetAutoTrackLimitPrice(BOOL bIsTrack);	// 设置价格跟盘模式
	BOOL GetAutoTrackLimitPrice();
	
	void SetViewStyle(int nViewStyle) {
		m_nViewStyle = nViewStyle;
		if(m_MouseHandyPopupPane != NULL)
			m_MouseHandyPopupPane->SetViewStyle(nViewStyle);
	};
	int GetViewStyle() {
		return m_nViewStyle;
	};

	void InitOrderView();					// 设置界面为初始值
	void ClearViewData(int nMode);					// 设置界面为初始值
	void ShowOrder(PlatformStru_InputOrder& req);		// 从数据结构中读数据并显示
	void ResetOrderTypeView(int nSel); 

	void ResetAccountList(vector<string>& accountList);
	void ResetContractList();

	void DynamicShowCtrl();
	void InsertCtrlID2TabCtrl(vector<UINT>& tabCtrlList);
	BOOL CheckCtrlID(int nCtrlID);

	void LockForSetChange() {
		m_bIsLockForSetChange = TRUE;
	};
	void UnlockForSetChange() {
		m_bIsLockForSetChange = FALSE;
	};

	void ResetLanguage();
	wxString GetUserInputText();

private:
	void SetSellBuyColor();
	//BOOL PY2Code(wxString strPY, wxString& strRetCode);

	wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit, wxSize& ctrSize, int nTextID, int nSpinID);
	//wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit, wxSize& ctrSize);
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit, wxSize& ctrSize);

	BOOL NeedMouseHandyPane_Code();		// 判断是否需要显示鼠标快捷面板输入代码
	BOOL NeedMouseHandyPane_Volume();	// 判断是否需要显示鼠标快捷面板输入数量
	BOOL NeedMouseHandyPane_Price();	// 判断是否需要显示鼠标快捷面板输入价格
	BOOL NeedInputAbbreviation();		// 可以输入缩写和拼音
	BOOL NeedAutoOCHideRadio();			// 使用自动平仓时，取消开平仓选择
	int GetContractListStyle();

private:

	int		m_nViewStyle;

	BOOL	m_bIsInitOK;
	double	m_fPriceTick;
	//int		nViewSubMode;				// 面板风格，0 基本，1 小窗委托，2 批量下单，3 条件单，4 盈损单

	CMouseHandyInputPane*		m_MouseHandyPopupPane;
	MouseHandyInputPaneVolume*	m_MouseHandyPopupPaneVolume;
	CMouseHandyInputPaneDigit*	m_MouseHandyPopupPaneDigit;

	BOOL	m_bIsLockForSetChange;
	BOOL	m_bNoKeyChangeText;

	wxColor m_colorBak;

	std::vector<int>			m_arrTabCtrl;

};