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
	//vector<SKLine *> m_vecKLine;        //�������Զ������
	wxColour  m_clBackGround;           //����ɫ
	wxColour  m_clBorder;               //�߿�ɫ

	wxColour	m_clRedLine;
	wxColour	m_clGreenLine;
	wxColour	m_cldashLine;
	wxColour	m_clCurSelLine;

	wxColour	m_clTipsBk;


	
public:	
	bool InitCfg(TiXmlElement *root);
	void SetTechIndex(wxString strTechIndex);

public://configר��
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
	//void SetLeftSpanWidth(int nWidth);//�������ҿ̶����Ŀ���

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
	

	void DrawOneItemData(wxDC& dc, wxRect rtClient);//����߼۸���ʾ�򣬲�������k��ģ�����滭����ʵ���Ӧ�õ���һ��ģ��
	

	//void OnLeftDown(wxMouseEvent& event);
	//void OnRightDown(wxMouseEvent& event);	
	//void OnLeftUp(wxMouseEvent& event);


	//void OnRightUp(wxMouseEvent& event);
	


	void GetLineName(wxString& strLineName);
	



private://ͼ��ͼ�õ�
	void DrawMarks(wxDC& dc);


	wxColour		m_clTrangleUp;			//��ͼ�õģ����������ε���ɫ
	wxColour		m_clTrangleUpBorder;    //�߿���ɫ
	wxColour		m_clTrangleDown;		//��ͼ�õģ����������ε���ɫ
	wxColour		m_clTrangleDownBorder;  //�߿���ɫ


	public:
	


public:
	void DrawMACD(wxDC& dc);//MACD���⻭������ͬ��һ���ԵĻ���
	wxColour m_clLineColor[COLOR_MAX];
	wxColour m_clMACD_ZHANG;
	wxColour m_clMACD_DIE;

private:
	vector<float> m_vecLeftScales;

};