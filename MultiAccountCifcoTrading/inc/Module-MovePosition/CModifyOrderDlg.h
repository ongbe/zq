#pragma once
#include "stdafx.h"
#include <vector>
#include "CfgMgr/CfgGlobal.h"

#include <wx/listctrl.h>


class CModifyOrderDlg : public wxDialog
{
public:
	CModifyOrderDlg(wxWindow *parent);

	virtual ~CModifyOrderDlg();

	void SetOwner(wxDialog* pDlg, BOOL bIsMove) {
		m_pDlg = pDlg;
		m_bIsMove = bIsMove;
	};

	void InsertModify(char * pText);
	void Clear();

protected:
	void OnOK(wxCommandEvent& event);
	void OnCancle(wxCommandEvent& event);
	void OnClose(wxCloseEvent& evt);

private:
	void CreateGUIControls();

private:
	//Note: if you receive any error with these enum IDs, then you need to
	//change your old form code that are based on the #define control IDs.
	//#defines may replace a numeric value for the enum names.
	//Try copy and pasting the below block in your old form header files.
	enum
	{
		////GUI Enum Control ID Start
		ID_WXBUTTON1 = 10200,
		ID_WXBUTTON2,
		ID_WXLISTCTRL1,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

private:
	wxListCtrl * m_pListCtrl;
	wxDialog*	m_pDlg;
	BOOL		m_bIsMove;

	DECLARE_EVENT_TABLE();
};