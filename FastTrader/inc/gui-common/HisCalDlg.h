#pragma once
#include "stdafx.h"

#include <wx/datectrl.h>
//#define wxUSE_DATEPICKCTRL 1

class HisCalDlg :
    public wxDialog
{
	enum
	{
		ID_CALCTRL,
		ID_BtDay,
		ID_BtMon,
		ID_BtSave,
		ID_BtPrint,
		ID_BtClose,
        ID_MyEdit
    };
public:
    HisCalDlg(wxWindow *parent);
    void  InitionInterface();
	void OnHisCal(wxCommandEvent& evt);
    void OnSaveFile(wxCommandEvent& event);
    void OnSize(wxSizeEvent& evt);
    ~ HisCalDlg();
     void OnQryDayInfo(wxCommandEvent& event);
     wxString m_timestring;
	 wxString m_AllContent;
	DECLARE_EVENT_TABLE()
};
