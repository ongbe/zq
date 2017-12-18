#pragma once
#include <vector>
#include "bkcolor.h"
#include "CommPublic.h"


//Message
#define UPDATE_MESSAGE  WM_USER + 200

#define MAX_POINTS 2048 //1 pixel per point, 2048 is the ordinary screen pixels count
#define PI     3.141592657

using namespace std;
class CLine
{
public:
	BOOL        m_bDraw;     //plot or not
	COLORREF	m_color;     //
	int         m_iStyle;     //solid,dash
	int         m_iThick;     //1pixel,2pixel
	CLine()
	{
		m_bDraw = TRUE;
		m_color = RGB(0,0,0);
		m_iStyle = PS_SOLID;
		m_iThick = 1;
		InitializeCriticalSection ( & g_cs ) ;
	}
	CLine(int iStyle, COLORREF color, int iThick);
	CLine(const CLine& line);
	CLine& operator = (const CLine& line);
	~CLine();
	void AddPoint(double x, double y);
	double GetPointX(int nIndex);
	double GetPointY(int nIndex);
	int    GetPointCount();
	void RemoveAllPoints();
	void RemoveUselessPoints();

private:
	vector<double> m_dXValue;
	vector<double> m_dYValue;
	CRITICAL_SECTION g_cs ;

};
class CAxis
{
public:
	BOOL        m_bDraw;     //plot or not
	BOOL        m_bGrid;     //Draw grid line or not
	double      m_dMinValue;
	double      m_dMaxValue;
	double	    m_dValuePerPixel; //how much a pixel represented

	COLORREF	m_color;     //
	int         m_iStyle;     //
	int         m_iThick;     //
	CAxis()
	{
		m_bDraw = TRUE;
		m_bGrid = TRUE;
		m_dMinValue = 0;
		m_dMaxValue = 10000;
		m_color = RGB(255,0,0);
		m_iStyle = PS_SOLID;
		m_iThick = 1;
	}
};
// CLineChartCtrl

class CLineChartCtrl : public CStatic
{
	DECLARE_DYNAMIC(CLineChartCtrl)

public:
	CLineChartCtrl();
	virtual ~CLineChartCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);

    void SetChartType(CHART_TYPE type);

	void CalcLayout();
	void AddLine(int iStyle, COLORREF color, int iThick = 1);
	CLine& GetLine(int nIndex);
	int    GetLineCount();

	CAxis& GetAxisY();
	CAxis& GetAxisX();


private:
    CHART_TYPE  m_ChartType;

    CRect m_rectCtrl;                       // the static rect of chart control
	CRect m_topRectCtrl;                    // the static top rect of chart control
    CRect m_bottomRectCtrl;                 // the static bottom rect of chart control
	CRect m_rectAxisY;		                // Y axis rect
	CRect m_rectAxisX;		                // X axis rect
	CRect m_rectPlot;                       // m_rectCtrl - margin - axisRect
	CPoint m_ptInLine[MAX_POINTS];          // points used to draw a line


	CAxis m_axisY;       //Y axis
	CAxis m_axisX;     //X axis

	vector<CLine> m_lines;
	
public:
    void SetBKColorMode(BK_COLOR_MODE mode);
private:
    BK_COLOR_MODE m_ColorMode;
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
    CPoint m_ptMouse;
public:
    void Update();
    void InvalidateCross();//接收的消息，更新图表
};


