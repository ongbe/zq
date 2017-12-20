#pragma once
#include "stdafx.h"
#include "CNumericTextCtrl.h"
#include <wx/spinctrl.h>

class CCustomSpinCtrl
{
public:

	CCustomSpinCtrl(
		wxWindow *parent, 
		wxWindowID tid=wxID_ANY, 
		wxWindowID sid=wxID_ANY, 
		const wxString &value=wxEmptyString, 
		const wxPoint &pos=wxDefaultPosition, 
		const wxSize &size=wxDefaultSize, 
		long style=wxSP_ARROW_KEYS, 
		int min=0, int max=100, int initial=0, 
		const wxString &name="wxSpinCtrl", 
		const double & fStep=0.01);

	virtual ~CCustomSpinCtrl();

	void SpinUpPress(wxSpinEvent& event);
	void SpinDownPress(wxSpinEvent& event);

	void SetStep(double step)
	{
		m_fStep = step;
	}

	CNumericTextCtrl * GetTextCtrl()
	{
		return m_pTextCtrl;
	}

	wxSpinButton * GetSpinButton()
	{
		return m_pSpinButton;
	}

	void SetFilter(const char * pChar)
	{
		m_pTextCtrl->SetFilter(pChar);
	}

private:

	double GetPrice();
	void SetPrice(double fPrice);
	wxString Double2String(double num);

private:

	double m_fStep;
	CNumericTextCtrl* m_pTextCtrl;
	wxSpinButton* m_pSpinButton;
	int m_nMin;
	int m_nMax;

};
