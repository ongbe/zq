#include "StdAfx.h"
#include "CCustomSpinCtrl.h"


CCustomSpinCtrl::CCustomSpinCtrl(wxWindow *parent, wxWindowID tid, wxWindowID sid, const wxString &value, const wxPoint &pos, const wxSize &size, 
								 long style, int min, int max, int initial, const wxString &name, const double & fStep)
{
	wxPoint ctlPos(0, 0);
	wxSize ctlSz = size;

	m_fStep = fStep;
	m_nMin = min;
	m_nMax = max;
	ctlPos = pos;

	m_pTextCtrl = new CNumericTextCtrl( parent, tid, wxEmptyString, ctlPos, ctlSz, 0 );
	m_pTextCtrl->SetLabel(value);
	

	ctlPos.x = ctlPos.x + size.GetX();
	ctlSz.Set(16, size.GetY());
	m_pSpinButton = new wxSpinButton( parent, sid, ctlPos, ctlSz, style );
	m_pSpinButton->SetRange(min, max);
	m_pSpinButton->SetValue(initial);
}

CCustomSpinCtrl::~CCustomSpinCtrl()
{

}

void CCustomSpinCtrl::SpinUpPress(wxSpinEvent& event)
{
	double fPrice=GetPrice();
	if (fPrice+m_fStep >= m_nMax)
	{
		SetPrice(m_nMax);
	}
	else
		SetPrice(fPrice+m_fStep);
}

void CCustomSpinCtrl::SpinDownPress(wxSpinEvent& event)
{
	double fPrice=GetPrice();
	if (fPrice-m_fStep <= m_nMin)
	{
		SetPrice(m_nMin);
	}
	else
		SetPrice(fPrice-m_fStep);
}


double CCustomSpinCtrl::GetPrice()
{
	wxString str;
	if (m_pTextCtrl) str = m_pTextCtrl->GetLabel();
	double t = 0;
	str.ToDouble(&t);
	return t;
}

void CCustomSpinCtrl::SetPrice(double fPrice)
{
	wxString str;
	if(fPrice == -1||util::isInvalidValue(fPrice)) 
		str = "-";
	if (m_pTextCtrl) m_pTextCtrl->SetLabel(Double2String(fPrice));
}

wxString CCustomSpinCtrl::Double2String(double num)
{
	wxString strPriceText="-";
	if(num!=-1 && (!util::isInvalidValue(num))) {
		//一位，二位，没有小数位
		int percentnum=(int)(m_fStep*100);
		if(percentnum%10!=0) {			//二位小数位
			strPriceText.Printf("%.2f",num);
		}
		else if(percentnum%100!=0) {	//一位小数位
			strPriceText.Printf("%.1f",num);
		}
		else {							//没有小数位
			strPriceText.Printf("%.0f",num);
		}
	}
	return strPriceText;
}