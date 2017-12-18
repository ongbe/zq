#pragma once
#include "stdafx.h"
#include "wx/defs.h"

#include "wx/window.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include <time.h>
#include "wx\msw\font.h"
#include "../inc/Quotation/KCtrl.h"
#include "../inc/Quotation/FData.h"



class   CKScales : public CKCtrl
{
public:
	 CKScales(void);
	 ~CKScales(void);

private:	
	wxColour	m_clBackGround;           //背景色
	wxColour	m_clBorder;               //边框色
	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;
	wxColour	m_clTipsBk;
	wxColour	m_clTriangle;
	wxColour	m_clText;
	

public:
	bool InitCfg(TiXmlElement *root);
public://config专用
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetRedLine(unsigned long lValue);
	void SetGreenLine(unsigned long lValue);
	void SetdashLine(unsigned long lValue);
	void SetCurSelLine(unsigned long lValue);
	void SetTipsBk(unsigned long lValue);
	void SetTrangle(unsigned long lValue);
	void SetLineName(unsigned long lValue);
 private:
    DECLARE_DYNAMIC_CLASS (CKScales)
    DECLARE_EVENT_TABLE()

	void CalcMaxMin();

	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawLeftText(wxDC& dc);
	void DrawCurSelLine(wxDC& dc);
	void DrawItems_Day(wxDC& dc);
	void DrawTips(wxDC& dc);	
	void DrawTips_Time(wxDC& dc, wxRect rtClient, int nX);
	void DrawItems_MINS(wxDC& dc);
	void ProcessMins(wxDC& dc, EnumPhrase enumPhrase, int i, int& nItemSpan, struct tm tm33, struct tm& tmPrevious, int& nFirstEnd);
	
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
	void OnRightUp(wxMouseEvent& event);
public:
	wxString GetWeekName(wxDateTime::WeekDay week);
	virtual void DoMouseMove(wxPoint mousePos);

};
