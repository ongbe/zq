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



class   CKVolume : public CKCtrl
{
public:
	 CKVolume(void);
	 ~CKVolume(void);
private:
	wxColour  m_clBackGround;           //背景色
	wxColour  m_clBorder;               //边框色

	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;
	wxColour	m_clLineName;
	wxColour	m_clTipsBk;

	wxColour		m_clTrangleUp;			//画图用的，向上三角形的颜色
	wxColour		m_clTrangleUpBorder;    //边框颜色
	wxColour		m_clTrangleDown;		//画图用的，向下三角形的颜色
	wxColour		m_clTrangleDownBorder;  //边框颜色
	

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
	void SetLineName(unsigned long lValue);
	void SetTrangleUp(unsigned long lValue);
	void SetTrangleDown(unsigned long lValue);
	void SetLineColor(int nIndex, unsigned long lValue);
	//void SetLeftSpanWidth(int nWidth);//设置左右刻度区的宽度
 private:
    DECLARE_DYNAMIC_CLASS (CKVolume)
    DECLARE_EVENT_TABLE()

	void CalcMaxMin();

	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void DrawHead(wxDC& dc);
	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawItems(wxDC& dc);
	void DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, bool IsZhang);
	void DrawScales(wxDC& dc);
	void DrawZuiGaoZuiDiJia(wxDC& dc);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);

private://图像画图用的
	void				DrawMarks(wxDC& dc);
	
//	void OnLeftDown(wxMouseEvent& event);
//	void OnLeftUp(wxMouseEvent& event);
//	void OnRightDown(wxMouseEvent& event);	
//	void OnRightUp(wxMouseEvent& event);

public:
	wxColour m_clLineColor[COLOR_MAX];

};
