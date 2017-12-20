#pragma once
#include "stdafx.h"
#include "../inc/Module-Misc/orderDefines.h"


class COrderRealMsgPanel : public wxPanel
{
public:
	COrderRealMsgPanel(wxWindow *parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxEmptyString);
	~COrderRealMsgPanel();
    	
	void OnSize(wxSizeEvent& event);
//	void OnPaint(wxPaintEvent& event);
    DECLARE_CLASS(COrderRealMsgPanel)
	DECLARE_EVENT_TABLE()

public:
	int GetMKTOpenVolumeMode(); 
	void ResetMsg(wxString& strCode, BOOL bIsBuy, BOOL bIsHedge, double fPrice, 
				int nDefaultVolume, int nMultiplyVolume, bool bIsMarketPrice, wxString strAccount);
    bool GetIMC(const std::string& InstrumentID,PlatformStru_InstrumentInfo& InstrumentData,
                                  PlatformStru_InstrumentMarginRate& InstrumentMarginRateData,
                                  PlatformStru_InstrumentCommissionRate& InstrumentCommissionRateData);
};