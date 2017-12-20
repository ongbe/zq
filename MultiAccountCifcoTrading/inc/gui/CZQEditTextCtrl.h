#pragma once
#include "stdafx.h"


class CZQEditTextCtrl : public wxStaticText
{
public:
	CZQEditTextCtrl(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxStaticTextNameStr);
	~CZQEditTextCtrl();
    
    void OnLeftDown(wxMouseEvent& event);
	void OnSetCursor(wxSetCursorEvent& event);
	
	void OnPaint(wxPaintEvent& event);

	DECLARE_EVENT_TABLE()

public:
	void SetViewMode(int nViewMode);

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	double GetPriceTick() {
		return m_fPriceTick;
	};

	void ResetDepthMarketData(wxString& strInstrumentID) {
		ZeroMemory(&m_tDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
		strcpy(m_tDepthMarketData.InstrumentID, strInstrumentID.c_str());
	}

	void SetDepthMarketData(CThostFtdcDepthMarketDataField& tDepthMarketData) {
		m_tDepthMarketData = tDepthMarketData;
	};
	CThostFtdcDepthMarketDataField& GetDepthMarketData() {
		return m_tDepthMarketData;
	};

protected:
	wxString Number2String(int num);
	wxString Double2String(double num);

private:
	double				m_fPriceTick;
	CThostFtdcDepthMarketDataField	m_tDepthMarketData;

};