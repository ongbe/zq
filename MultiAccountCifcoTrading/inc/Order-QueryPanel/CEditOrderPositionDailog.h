#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/COrderServiceThread.h"
#include "../inc/Module-Orders/COrderPositionPanel.h"

#include "../inc/Module-Orders/COrderInputPanel.h"


enum
{
	ID_EDITPOSITION_OK=1850,
	ID_EDITPOSITION_CANCEL,
	EDITPOSITIONSUMNUM_ID	// 标志ID号的数量
};

class CEditOrderPositionDailog :public wxDialog
{
public:
	CEditOrderPositionDailog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CEditOrderPositionDailog();

    virtual bool Show( bool show = true );

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
	void OnCreateWindow(wxWindowCreateEvent& evt);
	void OnDestroyWindow(wxWindowDestroyEvent& evt);
	void OnDialogCharHook(wxKeyEvent& evt);
	DECLARE_EVENT_TABLE()

public:

	void SetPositionOrder(COrderServiceThread::POSITIONORDER& tPositionOrder) {
		m_tPositionOrder = tPositionOrder;
	};
	COrderServiceThread::POSITIONORDER& GetPositionOrder() {
		return m_tPositionOrder;
	};
	BOOL ReadReq(PlatformStru_InputOrder& req);

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	
private:
	void ShowTradeInfoDlg(const wxString& Title, const wxString& errormessage, BOOL bIsShow);
	void ShowOrderReq();
	void ShowOrderPosition();

public:
	static wxWindow*	sm_poDailog;
	static HHOOK		sm_hDailogKeyBoard;

private:
	//COrderServiceThread*	m_poOrderServiceThread;
	COrderInputPanel*	m_poOrderInputPanel;
	COrderPositionPanel*	m_poOrderPositionPanel;
    std::vector<unsigned int>	m_TabOrderVector;
	COrderServiceThread::POSITIONORDER		m_tPositionOrder;
	COrderServiceThread::POSITIONORDER&		m_refPositionOrder;
	double	m_fPriceTick;

};