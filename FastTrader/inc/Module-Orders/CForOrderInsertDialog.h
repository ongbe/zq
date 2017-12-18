#pragma once
#include "stdafx.h"
#include "CfgMgr/CfgGlobal.h"

#include "../inc/Module-Misc/orderDefines.h"


class CForOrderInsertDialog : public wxDialog
{
public:

	enum
	{
		ID_EXCHANGE_COMB=1001,
		ID_TARGETPRODUCT_COMB,
		ID_TARGETEXPAIREDATE_COMB,
		ID_OPTIONDIRECTION_COMB,
		ID_OPTIONINSTRUMENT_COMB,
		ID_OK,
		ID_CANCEL,
		ID_STATIC,
		SUMNUM_ID	// 标志ID号的数量
	};


public:
	CForOrderInsertDialog(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_DIALOG_STYLE,
             const wxString& name = wxDialogNameStr);
	~CForOrderInsertDialog();
    
	static CForOrderInsertDialog* CreateDailog(wxWindow* parent);

	void OnCmbExchange(wxCommandEvent& evt);
	void OnCmbTargetID(wxCommandEvent& evt);
	void OnCmbExpareDate(wxCommandEvent& evt);
	void OnCmbOptionDirection(wxCommandEvent& evt);

	void OnBtnOrder(wxCommandEvent& evt);
	void OnBtnClose(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()

public:
	void ResetLanguage();

private:

};