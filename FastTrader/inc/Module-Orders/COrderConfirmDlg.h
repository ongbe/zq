#pragma once
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include "COrderConfirmPanel.h"
#include "COrderInsertEntity.h"
#include "CConfirmItemChange.h"

enum
{
	ID_ORDERCONFIRMDLG_YES=5100,
	ID_ORDERCONFIRMDLG_NO,
	ID_ORDERCONFIRMDLG_CONFIRMMSG_STATIC=1852,
	ID_ORDERCONFIRMDLG_CANCELINFO_STATIC,
	ID_ORDERCONFIRMDLG_TABPAGE,
	ID_ORDERCONFIRMDLG_CHECK_CONFIG,
    ID_ORDERCONFIRMDLG_TIMER,
	ORDERCONFIRMDLGSUMNUM_ID	// 标志ID号的数量
};

class COrderConfirmDlg : public wxDialog
{
public:
	enum {	conMaxCmbOrderNum = 10 };

public:
	COrderConfirmDlg(wxWindow *parent);
	virtual ~COrderConfirmDlg();

	void OnBtnYes(wxCommandEvent& event);
	void OnBtnNo(wxCommandEvent& event);
    void OnHookKeyPress(wxKeyEvent& evt);
    void OnTimer(wxTimerEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);

	virtual bool Show(bool show = true);

	void SetOrderInfo(const wxString& info) {
		m_strCancelInfo = info;
	};

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	double GetPriceTick() {
		return m_fPriceTick;
	};
	void SetIsAuto(BOOL bIsAuto) {
		m_bIsAuto = bIsAuto;
	};
	BOOL GetIsAuto() {
		return m_bIsAuto;
	};
	void SetIsMarketPrice(BOOL bIsMarketPrice) {
		m_bIsMarketPrice = bIsMarketPrice;
	};
	BOOL GetIsMarketPrice() {
		return m_bIsMarketPrice;
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
	
	void SetConfirmItemChange(CConfirmItemChange& confirmChg) {
		m_confirmChg = confirmChg;
	};
	CConfirmItemChange& GetConfirmItemChange() {
		return m_confirmChg;
	};

	int GetCmbOrderCount() {
		return m_nCmbOrderCount;
	};

	void AddCmbOrder(COrderInsertEntity::CMBORDER* pCmbOrder) {
		if(m_nCmbOrderCount < conMaxCmbOrderNum) {
			m_pCmbOrders[m_nCmbOrderCount] = pCmbOrder;
			m_nCmbOrderCount++;
		}
	};
	COrderInsertEntity::CMBORDER* GetCmbOrder(int nIndex) {
		if(nIndex >= 0 && nIndex < m_nCmbOrderCount)
			return m_pCmbOrders[nIndex];
		else 
			return NULL;
	};

	void ClearFtdcOrderFields() {
		m_nCmbOrderCount = 0;
		ZeroMemory(m_pCmbOrders, sizeof(COrderInsertEntity::CMBORDER*)*conMaxCmbOrderNum);
		ZeroMemory(m_poOrderConfirmPanels, sizeof(COrderConfirmPanel*)*conMaxCmbOrderNum);
	};

	wxString GetUserInputText();

private:
	void OnClose(wxCloseEvent& event);
	void CreateGUIControls();
	int ComputeLineNum(char* pStr);

private:
    wxTimer		        m_timer;//行情更新定时器
	wxStaticText*		m_pStaticConfirmMsg;
	wxStaticText*		m_pStaticCancelInfo;
	wxBookCtrl*			m_pBookCtrl;

	double				m_fPriceTick;
	double				m_fUpperLimitPrice;
	double				m_fLowerLimitPrice;
	BOOL				m_bIsAuto;
	BOOL				m_bIsMarketPrice;

    std::vector<UINT>	m_TabOrderVector;
	wxString			m_strCancelInfo;
	int					m_nCmbOrderCount;
	CConfirmItemChange	m_confirmChg;
	COrderConfirmPanel*	m_poOrderConfirmPanels[conMaxCmbOrderNum];
	COrderInsertEntity::CMBORDER*	m_pCmbOrders[conMaxCmbOrderNum];

private:
	DECLARE_EVENT_TABLE();
};
