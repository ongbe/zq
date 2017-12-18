#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/CLocalOrderService.h"
#include "../inc/Module-Orders/COrderPositionPanel.h"

#include "../inc/Module-Orders/COrderInputPanel.h"


enum
{
	ID_EDITPLDETAIL_OK=1900,
	ID_EDITPLDETAIL_CANCEL,
	EDITPLDETAILSUMNUM_ID	// 标志ID号的数量
};

class CEditOrderPLDetailDailog :public wxDialog
{
public:
	CEditOrderPLDetailDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CEditOrderPLDetailDailog();

    virtual bool Show( bool show = true );

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
	void OnCreateWindow(wxWindowCreateEvent& evt);
	void OnDestroyWindow(wxWindowDestroyEvent& evt);
	void OnDialogCharHook(wxKeyEvent& evt);
	DECLARE_EVENT_TABLE()

public:

	void SetPLDetailOrder(CLocalOrderService::PLDETAILORDER& tPLDetailOrder) {
		m_tPLDetailOrder = tPLDetailOrder;
	};
	CLocalOrderService::PLDETAILORDER& GetPLDetailOrder() {
		return m_tPLDetailOrder;
	};

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	
private:
	void ShowTradeInfoDlg(const wxString& Title, const wxString& errormessage, BOOL bIsShow);
	void ShowOrderReq();
	void ShowOrderPLDetail();

public:
	static wxWindow*	sm_poDailog;
	static HHOOK		sm_hDailogKeyBoard;

private:
	COrderInputPanel*		m_poOrderInputPanel;
	COrderPositionPanel*	m_poOrderPositionPanel;
    std::vector<unsigned int>	m_TabOrderVector;
	CLocalOrderService::PLDETAILORDER		m_tPLDetailOrder;
	double	m_fPriceTick;

};