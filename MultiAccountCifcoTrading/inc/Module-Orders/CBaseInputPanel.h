#pragma once
#include "stdafx.h"


class CBaseInputPanel : public wxPanel
{
public:
	static int ConvertOwnerDrawnComboBoxID(int nID, wxWindow* pPanel);

public:
	CBaseInputPanel(wxWindow *parent,
			wxWindowID winid = wxID_ANY,
			const wxPoint& pos = wxDefaultPosition,
			const wxSize& size = wxDefaultSize,
			long style = wxTAB_TRAVERSAL | wxNO_BORDER,
			const wxString& name = wxPanelNameStr);
	virtual ~CBaseInputPanel();
    
	DECLARE_EVENT_TABLE()

public:

	//void LockForSetChange() {
	//	m_bIsLockForSetChange = TRUE;
	//};
	//void UnlockForSetChange() {
	//	m_bIsLockForSetChange = FALSE;
	//};

	void SetUpperLimitPrice(double fPrice) {
		m_fUpperLimitPrice = fPrice;
	};
	double GetUpperLimitPrice() {
		return m_fUpperLimitPrice;
	};
	void SetLowerLimitPrice(double fPrice) {
		m_fLowerLimitPrice = fPrice;
	};
	double GetLowerLimitPrice() {
		return m_fLowerLimitPrice;
	};

protected:

	int ConvertOwnerDrawnComboBoxID(int nID);

protected:
	double	m_fUpperLimitPrice;
	double	m_fLowerLimitPrice;

};