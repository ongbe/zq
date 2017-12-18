#pragma once
#include "wx/defs.h"

#include "wx/window.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
//#include "KItemData.h"

#include <time.h>
#include "wx\msw\font.h"
#include "../inc/Quotation/KCtrl.h"
#include "../inc/Quotation/FData.h"








class   CKTechIndex : public CKCtrl
{
public:
	 CKTechIndex(void);
	 ~CKTechIndex(void);

private:
	//vector<SKLine *> m_vecKLine;        //由最近到远的数据
	wxColour	m_clBackGround;           //背景色
	wxColour	m_clBorder;               //边框色
	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;
	wxColour	m_clTipsBk;
	
public:	
	bool InitCfg(TiXmlElement *root);
	void SetTechIndex(wxString strTechIndex);

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
	void SetMACDZHANG(unsigned long lValue);
	void SetMACDDIE(unsigned long lValue);	

 private:
    DECLARE_DYNAMIC_CLASS (CKTechIndex)
    DECLARE_EVENT_TABLE()

	void CalcMaxMin();


	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void DrawHead(wxDC& dc);
	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawItems(wxDC& dc);
	void DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4);
	void DrawScales(wxDC& dc);
	void DrawLeftScales(wxDC& dc);
	void DrawZuiGaoZuiDiJia(wxDC& dc);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);
	

	void DrawOneItemData(wxDC& dc, wxRect rtClient);//画左边价格提示框，不得已在k线模块里面画，其实这个应该单独一个模块
	void GetLineName(wxString& strLineName);

private://图像画图用的
	void DrawMarks(wxDC& dc);


	wxColour		m_clTrangleUp;			//画图用的，向上三角形的颜色
	wxColour		m_clTrangleUpBorder;    //边框颜色
	wxColour		m_clTrangleDown;		//画图用的，向下三角形的颜色
	wxColour		m_clTrangleDownBorder;  //边框颜色

public:
	void DrawMACD(wxDC& dc);//MACD特殊画法，不同于一般性的画法
	wxColour m_clLineColor[COLOR_MAX];
	wxColour m_clMACD_ZHANG;
	wxColour m_clMACD_DIE;

private:
	vector<float> m_vecLeftScales;

};
