#ifndef _QUERYEXCHANGERATE_H_
#define _QUERYEXCHANGERATE_H_

#include "stdafx.h"
#include <vector>
#include "../inc/Module-Misc/ExtListCtrl.h"

class CQueryExchangeRateDlg : public wxDialog
{
public:
	enum
	{	
		ID_LIST,
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

	CQueryExchangeRateDlg(wxWindow *parent);

	~CQueryExchangeRateDlg();

	static int ExchangeRateCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data);

	void OnSize(wxSizeEvent& evt);
	void OnReportTypeChanged(wxCommandEvent& event);
	void OnExchangeRateChanged(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	void CreateGUIControls();

private:
	wxExtListCtrl* m_pListCtrl;
};

#endif

