#pragma once
#include "stdafx.h"
#include <wx/spinctrl.h>
#include <vector>
#include "../inc/Module-Misc/orderDefines.h"


class COrderFAKFOKPanel :public wxPanel
{
public:
	enum {	conROW_HEIGHT=13,
			conROW_WIDTH=12
	};

public:

    COrderFAKFOKPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
   ~COrderFAKFOKPanel();

	void OnRadioButton(wxCommandEvent& event);
    void OnPanelCharHook(wxCommandEvent& evt);
	DECLARE_EVENT_TABLE()
public:
    static COrderFAKFOKPanel* CreatePane(wxWindow* parent);

	int GetType() {
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_FAKFOK_RADIOFAK,this);
		if(window->GetValue()) return 0;
		window=(wxRadioButton*)FindWindowById(ID_FAKFOK_RADIOFOK,this);
		if(window->GetValue()) return 1;
		return -1;
	};
	int GetMinVolume() {
		wxSpinCtrl* window=NULL;
		window=(wxSpinCtrl*)FindWindowById(ID_FAKFOK_SPINCTRLMINVOLUME,this);
		if(window) return window->GetValue();
		return 0;
	};

	void ResetLanguage();
	wxString GetUserInputText();

private:
    std::vector<unsigned int> m_TabOrderVector;

};