#pragma once
#include "stdafx.h"
#include "CBaseInputPanel.h"
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Order-Common/CMouseHandyInputPane.h"
#include "../inc/Order-Common/MouseHandyInputPaneVolume.h"
#include "../inc/Order-Common/CMouseHandyInputPaneDigit.h"
#include "../inc/Module-Misc/orderDefines.h"


class CSubsForOrderRspDialog : public wxDialog
{
public:
// FAKFOK板块UIID
enum
{
	ID_LIST_ALL=1001,
	ID_LIST_SELECT,
	ID_APPEND,
	ID_REMOVE,
	ID_SUBSCRIBEALL,
	ID_SUBSCRIBE,
	ID_UNSUBSCRIBE,
	ID_UNSUBSCRIBEALL,
	ID_CANCEL,
	ID_STATIC,
	SUMNUM_ID	// 标志ID号的数量
};


public:
	CSubsForOrderRspDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CSubsForOrderRspDialog();
    
	static CSubsForOrderRspDialog* CreateDailog(wxWindow* parent);

	void OnBtnAppend(wxCommandEvent& evt);
	void OnBtnRemove(wxCommandEvent& evt);
	void OnBtnSubscribe(wxCommandEvent& evt);
	void OnBtnSubscribeAll(wxCommandEvent& evt);
	void OnBtnUnSubscribe(wxCommandEvent& evt);
	void OnBtnUnSubscribeAll(wxCommandEvent& evt);
	void OnBtnClose(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:
	void ResetLanguage();

private:

};