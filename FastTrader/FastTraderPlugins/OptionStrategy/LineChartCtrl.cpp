// LineChartCtrl.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "LineChartCtrl.h"
#include "CustomMemDC.h"
#include "Resource.h"
#include "ProfitChartDialog.h"
#include "TabProbaChartDialog.h"

#include <gdiplus.h>
using namespace Gdiplus;

///////////////////////////////////////////////////////////////////////////////////////////////
//Line
CLine::CLine(int iStyle, COLORREF color, int iThick)
{
	m_bDraw = TRUE;
	m_iStyle = iStyle;
	m_color = color;
	m_iThick = iThick;
	InitializeCriticalSection ( & g_cs ) ;
}
CLine::CLine(const CLine &line)
{
	m_bDraw = line.m_bDraw;
	m_iStyle = line.m_iStyle;
	m_color = line.m_color;
	m_iThick = line.m_iThick;
	//
	m_dXValue.clear();
	m_dXValue=line.m_dXValue;
	m_dYValue.clear();
	m_dYValue=line.m_dYValue;
	InitializeCriticalSection ( & g_cs ) ;
}
CLine& CLine::operator=(const CLine& line)
{
	m_bDraw = line.m_bDraw;
	m_iStyle = line.m_iStyle;
	m_color = line.m_color;
	m_iThick = line.m_iThick;
	//
	m_dXValue.clear();
	m_dXValue=line.m_dXValue;
	m_dYValue.clear();
	m_dYValue=line.m_dYValue;

	return *this;
}
CLine::~CLine()
{
	DeleteCriticalSection ( & g_cs ) ;
}
void CLine::AddPoint(double x, double y)
{
	EnterCriticalSection ( & g_cs ) ;
	m_dXValue.push_back(x);
	m_dYValue.push_back(y);
	LeaveCriticalSection ( & g_cs ) ;
	
}
double CLine::GetPointX(int nIndex)
{
	return m_dXValue[nIndex];
}
double CLine::GetPointY(int nIndex)
{
	return m_dYValue[nIndex];
}
int CLine::GetPointCount()
{
	return (int)m_dXValue.size();
}
void CLine::RemoveAllPoints()
{
	EnterCriticalSection ( & g_cs ) ;
	m_dXValue.clear();
	m_dYValue.clear();
	LeaveCriticalSection ( & g_cs ) ;
}
void CLine::RemoveUselessPoints()
{
	EnterCriticalSection ( & g_cs ) ;
	for(int i=0;i<MAX_POINTS;i++)
	{
		vector<double>::iterator itx = m_dXValue.begin();
		m_dXValue.erase(itx);
		vector<double>::iterator ity = m_dYValue.begin();
		m_dYValue.erase(ity);
	}
	LeaveCriticalSection ( & g_cs ) ;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// CLineChartCtrl

IMPLEMENT_DYNAMIC(CLineChartCtrl, CStatic)
CLineChartCtrl::CLineChartCtrl():m_ColorMode(Color_Light)
{
}

CLineChartCtrl::~CLineChartCtrl()
{
}


BEGIN_MESSAGE_MAP(CLineChartCtrl, CStatic)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CLineChartCtrl message handlers

void CLineChartCtrl::OnTimer(UINT nIDEvent)
{
	Invalidate(TRUE);
	CStatic::OnTimer(nIDEvent);
}

void CLineChartCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting

    CDataInfo::NewInstance()->SetChartInfo(m_ChartType,m_topRectCtrl);

    CustomMemDC memDC(  &dc, 
                        &m_topRectCtrl,
                        &m_bottomRectCtrl,
                        m_ColorMode,
                        m_ChartType);
}

BOOL CLineChartCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}
void CLineChartCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default


    CRect client;
    GetClientRect(&client);

    bool bMouseIn;
    if(m_topRectCtrl.PtInRect(point))
        bMouseIn = true;
    else
        bMouseIn = false;


    CDataInfo::NewInstance()->SetMouseIn(bMouseIn);
    CDataInfo::NewInstance()->SetMousePt(point);

    Invalidate();    
    CStatic::OnMouseMove(nFlags, point);  

    if(m_ChartType == eProfitChart)
    {
        CProfitChartDialog* pWnd = (CProfitChartDialog*)(GetParent());
        if(pWnd && ::IsWindow(pWnd->m_hWnd))
            if(pWnd->m_pTabProbaChartDialog)
                ::SendMessage(pWnd->m_pTabProbaChartDialog->m_hWnd,UPDATE_MESSAGE,0,0);
    }
    else if(m_ChartType == eProbaChart)
    {
        CTabProbaChartDialog* pWnd = (CTabProbaChartDialog*)(GetParent());
        if(pWnd && ::IsWindow(pWnd->m_hWnd))
            if(pWnd->m_pProfitChartDialog)
                ::SendMessage(pWnd->m_pProfitChartDialog->m_hWnd,UPDATE_MESSAGE,0,0);
    }
    
}
void CLineChartCtrl::Update()
{    
    Invalidate();
}
void CLineChartCtrl::InvalidateCross()
{
    Invalidate();
}
void CLineChartCtrl::SetChartType(CHART_TYPE type)
{
    m_ChartType = type;
}

//
void CLineChartCtrl::CalcLayout()
{
	GetClientRect(&m_rectCtrl);

    CopyRect(&m_rectPlot,&m_rectCtrl);
    CopyRect(&m_bottomRectCtrl,&m_rectCtrl);
    CopyRect(&m_topRectCtrl,&m_rectCtrl);
    m_topRectCtrl.DeflateRect(8,0);
    m_topRectCtrl.DeflateRect(0,30);
    m_topRectCtrl.top -= 20;

    CDataInfo::NewInstance()->SetChartInfo(m_ChartType,m_topRectCtrl);
    CDataInfo::NewInstance()->Calculate();
}
void CLineChartCtrl::AddLine(int iStyle, COLORREF color, int iThick)
{
	m_lines.push_back(CLine(iStyle,color,iThick));
}
CLine& CLineChartCtrl::GetLine(int nIndex)
{
	return m_lines[nIndex];
}
int CLineChartCtrl::GetLineCount()
{
	return (int)m_lines.size();
}

//
CAxis& CLineChartCtrl::GetAxisY()
{
	return m_axisY;
}
CAxis& CLineChartCtrl::GetAxisX()
{
	return m_axisX;
}

void CLineChartCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	CalcLayout();
	Invalidate();
}

void CLineChartCtrl::SetBKColorMode(BK_COLOR_MODE mode)
{
    m_ColorMode = mode;
}
