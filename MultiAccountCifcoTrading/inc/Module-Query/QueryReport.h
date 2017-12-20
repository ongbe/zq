#ifndef _QUERYREPORT_H_
#define _QUERYREPORT_H_

#include "stdafx.h"
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <map>
#include "../inc/Module-Misc/ExtListCtrl.h"

class CQueryReportDlg : public wxDialog
{
public:
	enum
	{	
		ID_COMBOX,
		ID_TEXT,
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

	CQueryReportDlg(wxWindow *parent);

	~CQueryReportDlg();

	void OnSize(wxSizeEvent& evt);
	void OnReportTypeChanged(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	void CreateGUIControls();

private:
	wxComboBox* m_pComReportType;
	wxTextCtrl* m_textReport;
	map<DWORD, Report> m_mapReport;
};

#endif

