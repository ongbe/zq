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





class   CKLine : public CKCtrl
{
public:
	 CKLine(void);
	 ~CKLine(void);

private:
	//vector<SKLine *> m_vecKLine;        //由最近到远的数据
	wxColour  m_clBackGround;           //背景色
	wxColour  m_clBorder;               //边框色
	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;
	wxColour	m_clTipsBk;
	wxColour	m_clLineName;//线的性质
	wxColour    m_clSAR;//SAR技术指标的颜色

public:	
	bool InitCfg(TiXmlElement *root);
	virtual void SetCursel(SItem& sItem);
	virtual void DoMouseMove(wxPoint mousePos);
public://config专用
	void SetBackGround(unsigned long lValue);
	void SetBorder(unsigned long lValue);
	void SetRedLine(unsigned long lValue);
	void SetGreenLine(unsigned long lValue);
	void SetdashLine(unsigned long lValue);
	void SetCurSelLine(unsigned long lValue);
	void SetTipsBk(unsigned long lValue);
	void SetLineName(unsigned long lValue);
	void SetSARName(unsigned long lValue);
	void SetLeftpanelName(unsigned long lValue);
	void SetLeftpanelTime(unsigned long lValue);
	void SetLeftpanelzhang(unsigned long lValue);
	void SetLeftpaneldie(unsigned long lValue);
	void SetLeftpanelhengpan(unsigned long lValue);
	void SetTrangleUp(unsigned long lValue);
	void SetTrangleDown(unsigned long lValue);
	void SetLineColor(int nIndex, unsigned long lValue);

 private:
    DECLARE_DYNAMIC_CLASS (CKLine)
    DECLARE_EVENT_TABLE()


	virtual void CalcMaxMin();
	

	void OnPaint (wxPaintEvent & PaintEvent);
	void DrawDC(wxDC& dc);
	void DrawHead(wxDC& dc);
	void DrawBorder(wxDC& dc, wxRect& rt);
	void DrawItems(wxDC& dc);
	void DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4);
	void DrawScales(wxDC& dc);
	void DrawZuiGaoZuiDiJia(wxDC& dc);
	void DrawCurSelLine(wxDC& dc);
	void DrawTips(wxDC& dc);
	
	void DrawOneItemData(wxDC& dc, wxRect rtClient);//画左边价格提示框，不得已在k线模块里面画，其实这个应该单独一个模块
	void DrawSAR(wxDC& dc);	
private:
	void GetLineName(wxString& strLineName);
	

private://字体颜色，左侧上角提示框专用
	wxColour m_LeftpanelName; 
	wxColour m_LeftpanelTime; //时间
	wxColour m_Leftpanelzhang;  //涨
	wxColour m_Leftpaneldie;    //跌
	wxColour m_Leftpanelhengpan;//横盘

	void SetTextColor(wxDC& dc, double dbClosePrice);
	void GetTimeText(EnumPhrase	 enumPhrase, wxString& strText, wxString& strText2);

	int  m_nLeftPanelHeight;

private://图像画图用的
	virtual void DrawMarks(wxDC& dc);
	wxColour		m_clTrangleUp;			//画图用的，向上三角形的颜色
	wxColour		m_clTrangleUpBorder;    //边框颜色
	wxColour		m_clTrangleDown;		//画图用的，向下三角形的颜色
	wxColour		m_clTrangleDownBorder;  //边框颜色

public:
	wxColour m_clLineColor[COLOR_MAX];


};
