#pragma once
#include "stdafx.h"
#include <wx/spinctrl.h>
#include <vector>
#include "../inc/Module-Misc/orderDefines.h"


class COrderBatchPanel :public wxPanel
{
public:
	enum {	conROW_HEIGHT=13,
			conROW_WIDTH=12
	};

public:

    COrderBatchPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
   ~COrderBatchPanel();

	void OnRadioButton(wxCommandEvent& event);
    void OnPanelCharHook(wxCommandEvent& evt);
	DECLARE_EVENT_TABLE()
public:
    static COrderBatchPanel* CreatePane(wxWindow* parent);

	int GetPriceType() {
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_OBATCH_RADIOINPUTPRICE,this);
		if(window->GetValue()) return 0;
		window=(wxRadioButton*)FindWindowById(ID_OBATCH_RADIONEWPRICE,this);
		if(window->GetValue()) return 1;
		window=(wxRadioButton*)FindWindowById(ID_OBATCH_RADIOBSPRICE,this);
		if(window->GetValue()) return 2;
		return -1;
	};
	int GetBatchEachNum() {
		wxSpinCtrl* window=NULL;
		window=(wxSpinCtrl*)FindWindowById(ID_OBATCH_SPINCTRLVOLUME,this);
		if(window) return window->GetValue();
		return 0;
	};
	int GetTimerInterval() {
		wxSpinCtrl* window=NULL;
		window=(wxSpinCtrl*)FindWindowById(ID_OBATCH_SPINCTRLTIMER,this);
		if(window) return window->GetValue();
		return 0;
	};

	void ResetLanguage();
	wxString GetUserInputText();

private:
    std::vector<unsigned int> m_TabOrderVector;

};