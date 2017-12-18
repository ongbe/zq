#pragma once
#include "stdafx.h"


class CStaticFivePriceList : public wxPanel
{
public:
	CStaticFivePriceList(wxWindow *parent,
                 wxWindowID id,
                 //const wxString& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxEmptyString);
	~CStaticFivePriceList();
    
    void OnLeftDown(wxMouseEvent& event);
	void OnSetCursor(wxSetCursorEvent& event);
	
	void OnPaint(wxPaintEvent& event);
    DECLARE_CLASS(CStaticFivePriceList)
	DECLARE_EVENT_TABLE()

public:
	void SetViewMode(int nViewMode);

	void SetPriceTick(double fPriceTick) {
		m_fPriceTick = fPriceTick;
	};
	double GetPriceTick() {
		return m_fPriceTick;
	};

	//void ResetDepthMarketData(wxString& strInstrumentID) {
	//	ZeroMemory(&m_tDepthMarketData, sizeof(PlatformStru_DepthMarketData));
	//	strcpy(m_tDepthMarketData.InstrumentID, strInstrumentID.c_str());
	//}
	void ResetDepthMarketData() {
		ZeroMemory(&m_tDepthMarketData, sizeof(PlatformStru_DepthMarketData));
	}

	BOOL ResetDepthMarketData(PlatformStru_DepthMarketData& tDepthMarketData); 
//	void SetDepthMarketData(PlatformStru_DepthMarketData& tDepthMarketData) {
//		m_tDepthMarketData = tDepthMarketData;
//	};
	PlatformStru_DepthMarketData& GetDepthMarketData() {
		return m_tDepthMarketData;
	};

private:
	double				m_fPriceTick;
	PlatformStru_DepthMarketData	m_tDepthMarketData;

};