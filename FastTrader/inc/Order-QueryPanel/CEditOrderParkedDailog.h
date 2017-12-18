#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/CLocalOrderService.h"
#include "../inc/Module-Orders/COrderConditionPanel.h"

#include "../inc/Module-Orders/COrderInputPanel.h"


enum
{
	ID_EDITPARKED_OK=1850,
	ID_EDITPARKED_CANCEL,
	ID_EDITPARKED_RADIO_HANDY,
	ID_EDITPARKED_RADIO_AUTO,
	EDITCONDITIONSUMNUM_ID	// 标志ID号的数量
};

class CEditOrderParkedDailog : public wxDialog
{
public:
	CEditOrderParkedDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CEditOrderParkedDailog();

    virtual bool Show( bool show = true );

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
	void OnCreateWindow(wxWindowCreateEvent& evt);
	void OnDestroyWindow(wxWindowDestroyEvent& evt);
	void OnDialogCharHook(wxKeyEvent& evt);
	DECLARE_EVENT_TABLE()

public:

	void SetParkedOrder(CLocalOrderService::PARKEDORDER& tParkedOrder) {
		m_tParkedOrder = tParkedOrder;
	};
	CLocalOrderService::PARKEDORDER& GetParkedOrder() {
		return m_tParkedOrder;
	};

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	
private:
	void ShowOrderReq();
	void ShowOrderParked();
	void FillParked(CLocalOrderService::PARKEDORDER& tParked);
	wxString GetParkedLog();

public:
	static wxWindow*	sm_poDailog;
	static HHOOK		sm_hDailogKeyBoard;

private:
	//COrderServiceThread*	m_poOrderServiceThread;
	COrderInputPanel*	m_poOrderInputPanel;
    std::vector<unsigned int>	m_TabOrderVector;
	CLocalOrderService::PARKEDORDER		m_tParkedOrder;
	double	m_fPriceTick;

};