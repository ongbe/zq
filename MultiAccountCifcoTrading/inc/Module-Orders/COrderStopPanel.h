#pragma once
#include "stdafx.h"
#include <wx/spinctrl.h>
#include "../inc/Module-Misc/orderDefines.h"
#include "CBaseInputPanel.h"


class COrderStopPanel :public CBaseInputPanel
{
public:
	enum {	conROW_HEIGHT=13,
			conROW_WIDTH=12
	};

public:

    COrderStopPanel(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
   ~COrderStopPanel();

	void OnRadioStopSel(wxCommandEvent& event);
	void OnPriceSpinUp(wxSpinEvent& event);
	void OnPriceSpinDown(wxSpinEvent& event);
	void OnTriggerPriceSpinUp(wxSpinEvent& event);
	void OnTriggerPriceSpinDown(wxSpinEvent& event);
	void OnHookKeyPress(wxKeyEvent& evt);
	DECLARE_EVENT_TABLE()
public:
    static COrderStopPanel* CreatePane(wxWindow* parent);

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	void SetStopType(int nStopType) {
		wxRadioButton* window=NULL;
		switch(nStopType) {
		case 1:
			window=(wxRadioButton*)FindWindowById(ID_STOP_RADIOSTOP,this);
			if(window) window->SetValue(1);
			break;
		case 2:
			window=(wxRadioButton*)FindWindowById(ID_STOP_RADIOSTOPLIMIT,this);
			if(window) window->SetValue(1);
			break;
		};
	};
	int GetStopType() {
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_STOP_RADIOSTOP,this);
		if(window->GetValue()) return 1;
		window=(wxRadioButton*)FindWindowById(ID_STOP_RADIOSTOPLIMIT,this);
		if(window->GetValue()) return 2;
		return -1;
	};

	void SetPrice(double fPrice) {
		wxTextCtrl* window=NULL;
		window=(wxTextCtrl*)FindWindowById(ID_STOP_TEXTPRICE,this);
		wxString strValue;
		strValue.Printf("%0.2f", fPrice);
		if(window) window->SetLabel(strValue);
	};
	double GetPrice() {
		wxComboBox* window=NULL;
		window=(wxComboBox*)FindWindowById(ID_STOP_TEXTPRICE,this);
		return atof(window->GetLabel());
	};

	void SetTriggerPrice(double fPrice) {
		wxTextCtrl* window=NULL;
		window=(wxTextCtrl*)FindWindowById(ID_STOP_TEXTTRIGGERPRICE,this);
		wxString strValue;
		strValue.Printf("%0.2f", fPrice);
		if(window) window->SetLabel(strValue);
	};
	double GetTriggerPrice() {
		wxTextCtrl* window=NULL;
		window=(wxTextCtrl*)FindWindowById(ID_STOP_TEXTTRIGGERPRICE,this);
		return atof(window->GetLabel());
	};

	void ResetLanguage();
	wxString GetUserInputText();

private:
	double	m_fPriceTick;

};