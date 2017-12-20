#pragma once
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/odcombo.h> 
#include <wx/spinbutt.h>
#include "../inc/Module-Misc/ExtListCtrl.h"
#include "CMAOrderEntity.h"

enum
{
	ID_MAORDERCONFIRMDLG_YES=5300,
	ID_MAORDERCONFIRMDLG_NO,
	ID_MAORDERCONFIRMDLG_CANCEL,
    ID_MAORDERCONFIRMDLG_NEWPRICE_SPINCTRL,
	ID_MAORDERCONFIRMDLG_NEWVOLUME_SPINCTRL,
	ID_MAORDERCONFIRMDLG_NEWPRICE_STATIC,
	ID_MAORDERCONFIRMDLG_NEWVOLUME_STATIC,
	ID_MAORDERCONFIRMDLG_STRATEGY_STATIC,
	ID_MAORDERCONFIRMDLG_MESSAGE_TEXT,
	ID_MAORDERCONFIRMDLG_NEWPRICE_TEXT,
	ID_MAORDERCONFIRMDLG_NEWVOLUME_TEXT,
	ID_MAORDERCONFIRMDLG_STRATEGY_COMBOBOX,
	ID_MAORDERCONFIRMDLG_ACCOUNT_LIST,
    ID_MAORDERCONFIRMDLG_TIMER,
	MAORDERCONFIRMDLGSUMNUM_ID	// 标志ID号的数量
};

class CMAOrderConfirmDlg : public wxDialog
{
public:
	static int wxCALLBACK CompareInt(const wxExtListItem& item1, 
							const wxExtListItem& item2,long sortData);
	static int wxCALLBACK CompareDouble(const wxExtListItem& item1, 
							const wxExtListItem& item2,long sortData);
	static int wxCALLBACK CompareCheckBox(const wxExtListItem& item1, 
							const wxExtListItem& item2,long sortData);

public:
	CMAOrderConfirmDlg(wxWindow *parent);
	virtual ~CMAOrderConfirmDlg();

	CMAOrderEntity* GetMAOrderEntity() {
		return m_pMAOrderEntity;
	};
	void SetOwnerPanel(wxPanel* pOwnerPanel) {
		m_pOwnerPanel = pOwnerPanel;
	};
	void SetAccounts(map<string,bool>& mapAccounts) {
		m_mapAccounts = mapAccounts;
	}
	map<string,bool>& GetAccounts() {
		return m_mapAccounts;
	};

	void OnBtnYes(wxCommandEvent& event);
	void OnBtnNo(wxCommandEvent& event);
 	void OnBtnCancel(wxCommandEvent& event);
	void OnPriceClicked(wxMouseEvent& evt);
	void OnVolumeClicked(wxMouseEvent& evt);
	void OnCmbStrategy(wxCommandEvent& evt);
    void OnHookKeyPress(wxKeyEvent& evt);
    void OnTimer(wxTimerEvent& event);
	void OnClose(wxCloseEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);

	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnVolumeSpinUp(wxSpinEvent& event);
	void OnVolumeSpinDown(wxSpinEvent& event);
    void OnPriceTextChanged(wxCommandEvent& event);
    void OnVolumeTextChanged(wxCommandEvent& event);

	void OnItemClicked(wxExtListEvent& evt);
	void OnItemChanged(wxExtListEvent& evt);
	void OnColClick(wxExtListEvent& evt);

	virtual bool Show(bool show = true);

	void ResetID2Account(map<string,bool>& accsLogon);
	void ResetMAOrderEntity(CMAOrderEntity* pMAOrderEntity, 
			BOOL bDeleteAll=FALSE, BOOL bWriteToUI=TRUE);
	BOOL ChangeMaOrderEntity();
	void ShowMAOrderEntity(CMAOrderEntity* pMAOrderEntity, BOOL bHasSent, BOOL bDeleteAll=FALSE);

	void ResetOrderStatus(DataRtnOrder* rtnOrderStatus);
	void ResetOrderStatus(DataRspOrderInsert* rspOrderInsert);
	void ResetOrderStatus(DataRspOrderAction1* rspOrderAction);
	void ResetOrderStatus(DataRspOrderAction2* rspOrderAction);

private:
	static int GetRowFromList(wxExtListCtrl* pwxListCtrl, int nID);
	static int GetIDFromAccount(std::string strAccount);

	void CreateGUIControls();

	wxTextCtrl* CreateFloatSpinCtrl_Price(wxPoint& ctrPosInit);
	wxTextCtrl* CreateFloatSpinCtrl_Volume(wxPoint& ctrPosInit);
	
	bool Init();
	void ShowAllAccount(BOOL bHasSent=FALSE, BOOL bDeleteAll=FALSE);
	int ShowOneAccount(int iRowNo, CSAOrderEntity* pSAEntity, 
			OrderInsertUIItem& tOrderUIItem, BOOL bIsNew=TRUE, BOOL bHasSent=FALSE);
	int ShowZeroOneAccount(int iRowNo);

	void SetStrategyNumber(int straNum);
	int GetStrategyNumber();
	wxString GetStrategyNumberText();
	void SetPriceText(wxString& strPrice);
	wxString GetPriceText();
	void SetPrice(double fPrice);
	double GetPrice();
	void SetQty(int qty);
	int GetQty();
	wxString GetQtyText();

	wxString GetListCtrlRowLog(wxExtListCtrl* pListCtrl, int nRow);
	wxString GetListCtrlAllRowLog(wxExtListCtrl* pListCtrl);
	wxString GetConfirmStatusLog();

private:
    wxTimer		        m_timer;//行情更新定时器

	wxTextCtrl*			m_pMessageText;
	wxOwnerDrawnComboBox*	m_pCmbStrategy;
	wxExtListCtrl*		m_pwxExtListCtrl;

	map<string,bool>	m_mapAccounts;

	double				m_fPriceTick;
	double				m_fUpperLimitPrice;
	double				m_fLowerLimitPrice;

	BOOL				m_bIsLockForSetChange;
	BOOL				m_bHasSent;
	BOOL				m_bUserChgStraNum;
	BOOL				m_bUserChgAccounts;

    std::vector<UINT>	m_TabOrderVector;
	wxString			m_strCancelInfo;
	CMAOrderEntity*		m_pMAOrderEntity;
	wxPanel*			m_pOwnerPanel;

	std::map<std::string, int> m_mapKey2Row;
	std::map<int, std::string> m_mapRow2Key;
	std::map<int, std::string> m_mapID2Account;

private:
	wxFont		m_Font;
	wxColor		m_HeadBackgroundColor;
	wxColor		m_HeadColor;
	wxColor		m_BackgroundColor;
	wxColor		m_TextColor;

	DECLARE_EVENT_TABLE();
};
