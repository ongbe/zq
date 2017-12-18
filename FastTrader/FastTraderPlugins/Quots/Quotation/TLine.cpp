#include "stdafx.h"
#include "../../../inc/Quotation/TLine.h"
#include <time.h>





IMPLEMENT_CLASS(CTLine, CTCtrl)

CTLine::CTLine(void)
{
	m_clBackGround	= wxColour(RGB(0,0,0));
	m_clBorder		= wxColour(RGB(192,0,0));
	//m_cldashLine	= wxColour(RGB(76,40,40));

	m_clPrice		= RGB(255, 255, 255);
	m_clAveragePrice   = RGB(255, 255, 0);

	m_clName		= wxColour(255, 255, 255);
	m_clTime		= wxColour(82, 255, 255);
	m_zhang			= wxColour(255, 82, 82);
	m_die			= wxColour(82, 255, 82);
	m_hengpan		= wxColour(255, 255, 255);
	m_clCurSelLine  = wxColour(RGB(200,200,200));
	m_clLineName    = wxColour(RGB(192,192,192));

	m_clTipsBk		= wxColour(RGB(0,0,128));

	m_nBegin = 0;
	m_nEnd   = 0;
	m_nDayCount = 1;
	
	m_RectOffSet =  wxRect(66,16,66,0);
	m_nBorderWidth	= 1;
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;

	m_bShowHeight = true;
	m_bShowTime   = false;
	m_ptMouseMoveLast = wxPoint(-1,-1);
	m_nLeftPanelHeight = 230;//左侧价格提示窗高度


	m_fLastClosePrice = MIN_VALUE_DEFAULT;

	m_nOneDayScalesCount = m_vecTimeScales.size();

	m_dbVolumeMultiple = 100.00;
	//m_bShowOneItemData = true;
	m_pFData = NULL;
	m_nNumDigits = 2;


	m_nSpanLineType = 1;		
	m_nSpanLineSize = 2;		
	m_nHourLineType = 1;		
	m_nHourLineSize = 1;		
	m_nHalfHourLineLineType = 0;
	m_nHalfHourLineSize = 1;	

	m_bShowCross	   = false;
	m_bMouseIn		   = false;
}
CTLine::~CTLine(void)
{
}
BEGIN_EVENT_TABLE(CTLine, CTCtrl)
	EVT_PAINT(CTLine::OnPaint)
	EVT_MOTION(CTLine::OnMouseMove)
	EVT_MOTION(CTLine::OnMouseMove)
	EVT_SIZE(CTLine::OnSize)
	EVT_LEAVE_WINDOW(CTLine::OnMouseLeave)
	EVT_ENTER_WINDOW(CTLine::OnMouseEnter)
	EVT_LEFT_DCLICK(OnLeftDBClick)
	EVT_RIGHT_UP(CTLine::OnRightUp)
	EVT_LEFT_DOWN(CTLine::OnLeftDown)
END_EVENT_TABLE()
BOOL CTLine::CalcMaxMinInit()
{
	for(int i = m_nBegin; i< (int)m_vecTLine.size() && i>= m_nEnd && i>=0; i--)
	{		
		STLine *pTLine = m_vecTLine[i];
		if(pTLine->fLastPrice > m_fMaxValue)
		{
			m_fMaxValue = pTLine->fLastPrice;
		}
		if(pTLine->fLastPrice < m_fMinValue)
		{
			m_fMinValue = pTLine->fLastPrice;
		}
	}
	
	return true;
}

BOOL CTLine::SetVectorData(vector<STLine *>& veCTCtrl)
{	
	m_vecTLine.clear();
	m_vecTLine = veCTCtrl;	
	GetBeginEnd();
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;
	CalcMaxMinInit();
	return TRUE;
}
void CTLine::OnPaint (wxPaintEvent & PaintEvent)
{
	wxPaintDC dc(this);
	wxMemoryDC memdc;  
	memdc.SetLayoutDirection(dc.GetLayoutDirection());
	wxBitmap bmp;
	wxSize size = GetClientSize();
	bmp.Create(size.GetWidth(), size.GetHeight());
	memdc.SelectObject(bmp);

	DrawDC(memdc);
	dc.Blit(0, 0, size.GetWidth(),size.GetHeight(),&memdc, 0, 0);
}
void CTLine::DrawDC(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxBrush brush(m_clBackGround);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.x, size.y);

	wxRect rt(0, 0, size.x, size.y);
	DrawScales(dc);
	DrawHead(dc);
	DrawBorder(dc, rt);
	DrawPriceLines(dc);
	DrawAveragePriceLines(dc);
	if(m_bMouseIn && m_bShowCross)
		DrawCurSelLine(dc);
	if(m_bMouseIn)
		DrawTips(dc);
	if(m_bMouseIn && m_bShowCross)
	{
		wxRect rtData = wxRect(1, m_RectOffSet.GetHeight(), m_RectOffSet.GetLeft(), m_nLeftPanelHeight);
		DrawOneItemData(dc, rtData);
	}
}
void CTLine::CalcMaxMin()
{

}
void CTLine::DrawBorder(wxDC& dc, wxRect& rt)
{
	wxPen penBorder(wxColour(m_clBorder), m_nBorderWidth, wxSOLID);
	dc.SetPen(penBorder);
	dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetRight(), rt.GetTop());
	dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetLeft(), rt.GetBottom());
	//dc.DrawLine(rt.GetLeft(), rt.GetBottom(), rt.GetRight(), rt.GetBottom());
	//dc.DrawLine(rt.GetRight(), rt.GetBottom(), rt.GetRight(), rt.GetTop());

	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() );
	dc.DrawLine(rtClient.GetLeft(), rtClient.GetTop(), rtClient.GetLeft(), rtClient.GetBottom());
	dc.DrawLine(rtClient.GetRight(), rtClient.GetTop(), rtClient.GetRight(), rtClient.GetBottom());

}
void CTLine::DrawScales(wxDC& dc)
{
	if(m_vecTimeScales.size() == 0)
		return;

	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop()	+ m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight()	- m_RectOffSet.GetWidth()	- m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()	- m_RectOffSet.GetTop());

	wxString  strText;
	wxCoord w,h;
	wxPoint pt;
	wxPen penBorder(wxColour(m_clBorder), 3, wxSOLID);
	dc.SetPen(penBorder);
	int nHeight = JiaGe2GaoDu(m_fLastClosePrice);
	dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
	SetTextColor(dc, m_fLastClosePrice);
//	strText = wxString::Format(wxT("%.3f"), m_fLastClosePrice);
	strText = Double2String(m_fLastClosePrice, m_nNumDigits);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nHeight - h/2; 
	if(nHeight >=0)
	  dc.DrawText(strText, pt);

	strText = wxT("0.00%");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() + 4;
	pt.y = nHeight - h/2; 
	if(nHeight >=0)
		dc.DrawText(strText, pt);

	wxPen penBorder2(wxColour(m_clBorder), 1, wxSOLID);
	//////////////////////////////////////////////////////////////////////////
	if(m_nSpanLineType == 0)
	{//虚线的时候粗度必须为1,1以上的时候有时候会很难看，有时候会出现锯齿状
		penBorder2.SetStyle(wxDOT);
		penBorder2.SetWidth(1);
	}
	else
	{
		penBorder2.SetStyle(wxSOLID);
		penBorder2.SetWidth(m_nSpanLineSize);
	}
	//////////////////////////////////////////////////////////////////////////
	dc.SetPen(penBorder2);
	for(int i = 1; i< (int)m_vecTimeScales.size()-1; i = i + 2)//-1是不希望最后一根画竖线
	{  
		int nX = Time2X(m_vecTimeScales[i]);		
		dc.DrawLine(nX, rtClient.GetTop(), nX, rtClient.GetBottom());		
	}
	struct tm tmBegin = m_vecTimeScales[0];
	struct tm tmEnd   = m_vecTimeScales[m_vecTimeScales.size() - 1];
	wxDateTime dateBegin(tmBegin);
	wxDateTime dateEnd(tmEnd);

	if(m_TMWidthCount >= 43200)//超过12小时的交易时间；执行不同画法;主要针对外盘和模拟行情
	{
		for(int i = 0; i <4; i++)
		{
			int nSpan1 = m_TMWidthCount/8;
			int nSpan2 = m_TMWidthCount/4;
			struct tm tmOneSpan = GetDrawTime(m_vecTimeScales, m_vecTMWidthSpan, nSpan1+i*nSpan2);
			wxPen penBorder(wxColour(m_clBorder), 1, wxDOT);
			//////////////////////////////////////////////////////////////////////////
			if(m_nHalfHourLineLineType == 0)
				penBorder.SetStyle(wxDOT);
			else
				penBorder.SetStyle(wxSOLID);
			penBorder.SetWidth(m_nHalfHourLineSize);	
			//////////////////////////////////////////////////////////////////////////
			dc.SetPen(penBorder);
			int nX = Time2X(tmOneSpan);
			dc.DrawLine(nX, rtClient.GetTop(), nX, rtClient.GetBottom());	

			struct tm tmOneSpan2 = GetDrawTime(m_vecTimeScales, m_vecTMWidthSpan, nSpan2+i*nSpan2);
			wxPen penBorder2(wxColour(m_clBorder), 1, wxSOLID);
			//////////////////////////////////////////////////////////////////////////
			if(m_nHourLineType == 0)
			{//虚线的时候粗度必须为1,1以上的时候有时候会很难看，有时候会出现锯齿状
				penBorder2.SetStyle(wxDOT);
				penBorder2.SetWidth(1);
			}
			else
			{
				penBorder2.SetStyle(wxSOLID);
				penBorder2.SetWidth(m_nHourLineSize);
			}
		  //////////////////////////////////////////////////////////////////////////
			dc.SetPen(penBorder2);
			int nX2 = Time2X(tmOneSpan2);
			dc.DrawLine(nX2, rtClient.GetTop(), nX2, rtClient.GetBottom());	
		}		
	}
	else
	{
		for(wxDateTime dt = dateBegin; dt<dateEnd; dt += wxTimeSpan(0, 15, 0, 0))
		{
			int nMin = dt.GetMinute();
			if(nMin == 30)
			{
				struct tm tmDt;
				DateTime2Tm(dt, tmDt);
				wxPen penBorder2(wxColour(m_clBorder), 1, wxDOT);
				//////////////////////////////////////////////////////////////////////////
				if(m_nHalfHourLineLineType == 0)
				{//虚线的时候粗度必须为1,1以上的时候有时候会很难看，有时候会出现锯齿状
					penBorder2.SetStyle(wxDOT);
					penBorder2.SetWidth(1);
				}
				else
				{
					penBorder2.SetStyle(wxSOLID);
					penBorder2.SetWidth(m_nHalfHourLineSize);	
				}
				//////////////////////////////////////////////////////////////////////////
				dc.SetPen(penBorder2);
				int nX = Time2X(tmDt);
				dc.DrawLine(nX, rtClient.GetTop(), nX, rtClient.GetBottom());		
			}
			else if(nMin == 0)
			{
				struct tm tmDt;
				DateTime2Tm(dt, tmDt);
				wxPen penBorder2(wxColour(m_clBorder), 1, wxSOLID);
				//////////////////////////////////////////////////////////////////////////
				if(m_nHourLineType == 0)
				{//虚线的时候粗度必须为1,1以上的时候有时候会很难看，有时候会出现锯齿状
					penBorder2.SetStyle(wxDOT);
					penBorder2.SetWidth(1);
				}
				else
				{
					penBorder2.SetStyle(wxSOLID);
					penBorder2.SetWidth(m_nHourLineSize);
				}
				//////////////////////////////////////////////////////////////////////////
				dc.SetPen(penBorder2);
				int nX = Time2X(tmDt);
				dc.DrawLine(nX, rtClient.GetTop(), nX, rtClient.GetBottom());
			}
		}
	}	

	int nDrawScalesCount = GetDrawScalesCount(dc);
	wxPen penBorderN(wxColour(m_clBorder), 1, wxSOLID);
	dc.SetPen(penBorderN);
	for(int i = 0; i<= nDrawScalesCount; i++)
	{
		int nHeight = rtClient.GetBottom() - i*rtClient.GetHeight()/(nDrawScalesCount*2) ;
		dc.DrawLine(rtClient.GetLeft(), nHeight, rtClient.GetRight(), nHeight);
		nHeight = rtClient.GetTop() + i*rtClient.GetHeight()/(nDrawScalesCount*2) ;
		dc.DrawLine(rtClient.GetLeft(), nHeight, rtClient.GetRight(), nHeight);
	}
	for(int i = 1; i<= nDrawScalesCount; i++)
	{
		double dbPrice = m_fLastClosePrice + i*( m_fMaxValue - m_fLastClosePrice)/nDrawScalesCount;
		int nHeight = JiaGe2GaoDu(dbPrice);
	//	dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
		if(nHeight <0)
			continue;
		SetTextColor(dc, dbPrice);
	//	strText = wxString::Format(wxT("%.3f"), dbPrice);
		strText = Double2String(dbPrice, m_nNumDigits, true);
		dc.GetTextExtent(strText, &w, &h);	
		pt.x = rtClient.GetLeft() - w-4;
		pt.y = nHeight - h/2; 
		dc.DrawText(strText, pt);

		strText = wxString::Format(wxT("%.2f%%"), (dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);
	//	strText = Double2String((dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);
		dc.GetTextExtent(strText, &w, &h);	
		pt.x = rtClient.GetRight() + 4;
		pt.y = nHeight - h/2; 
		dc.DrawText(strText, pt);

		if( i != nDrawScalesCount)
		{
			dbPrice = m_fLastClosePrice - i*( m_fLastClosePrice - m_fMinValue)/nDrawScalesCount;
			nHeight = JiaGe2GaoDu(dbPrice);
		//	dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);

			SetTextColor(dc, dbPrice);
		//	strText = wxString::Format(wxT("%.3f"), dbPrice);
			strText = Double2String(dbPrice, m_nNumDigits, true);
			dc.GetTextExtent(strText, &w, &h);	
			pt.x = rtClient.GetLeft() - w-4;
			pt.y = nHeight - h/2; 
			dc.DrawText(strText, pt);

			strText = wxString::Format(wxT("%.2f%%"), (dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);
		//	strText = Double2String((dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);
			dc.GetTextExtent(strText, &w, &h);	
			pt.x = rtClient.GetRight() + 4;
			pt.y = nHeight - h/2; 
			dc.DrawText(strText, pt);
		}
	}
}
void CTLine::DrawPriceLines(wxDC& dc)
{
	if(m_nBegin < 0 || m_nBegin >=  (int)m_vecTLine.size()
		|| m_nEnd < 0 || m_nEnd >=  (int)m_vecTLine.size())
		return;

	wxPen penLine(m_clPrice, 1, wxSOLID);
	dc.SetPen(penLine);

	STLine *pTLineEnd1 = m_vecTLine[m_nEnd];
	for( int i = m_nEnd+1; i<= m_nBegin; i++ )
	{
		int nXBegin = Time2X(pTLineEnd1->dwTime);
		int nYBegin = JiaGe2GaoDu(pTLineEnd1->fLastPrice);
		STLine *pTLineBegin1 = m_vecTLine[i];
		int nXEnd = Time2X(pTLineBegin1->dwTime);
		int nYEnd = JiaGe2GaoDu(pTLineBegin1->fLastPrice);

		
		if(nXBegin == -1 || nXEnd == -1)
			continue;
		if(!DrawPriceLines_NoData(dc, pTLineEnd1, pTLineBegin1))//画停牌或者没有数据的时候
			dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXEnd, nYEnd));		

		pTLineEnd1 = pTLineBegin1;
		
	}

	if(m_vecTimeScales.size() == 0)
		return;

	STLine *pTLineTemp = m_vecTLine[m_nBegin];
	if(difftime(mktime(&m_vecTimeScales[0]), mktime(&pTLineTemp->dwTime)) < 0 )
	{
		int nXBegin = Time2X(pTLineTemp->dwTime);
		int nYBegin = JiaGe2GaoDu(m_fLastClosePrice);
		
		int nXEnd = Time2X(m_vecTimeScales[0]);
		int nYEnd = JiaGe2GaoDu(pTLineTemp->fLastPrice);
		dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXEnd, nYBegin));		
		dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXBegin, nYEnd));
	}
}
bool CTLine::DrawPriceLines_NoData(wxDC& dc, STLine *&pTLineEnd1, STLine *&pTLineBegin1)//end时间近
{	
	struct tm tmBegin = pTLineEnd1->dwTime;
	tmBegin.tm_sec = 0;
	struct tm tmEnd   = pTLineBegin1->dwTime;
	tmEnd.tm_sec = 0;
	for(int i =0; i< (int)m_vecTimeScales.size(); i++)
	{
		if(difftime(mktime(&m_vecTimeScales[i]), mktime(&tmBegin)) == -60 )
		{
			if(i-1>=0 && i-1< (int)m_vecTimeScales.size() && difftime(mktime(&m_vecTimeScales[i-1]), mktime(&tmEnd)) == 0 )
				return false;
		}
	}
	

	if(difftime(mktime(&tmBegin), mktime(&tmEnd)) == 60 )
		return false;
	int nXBegin = Time2X(pTLineEnd1->dwTime);
	int nYBegin = JiaGe2GaoDu(pTLineEnd1->fLastPrice);	
	int nXEnd = Time2X(pTLineBegin1->dwTime);
	int nYEnd = JiaGe2GaoDu(pTLineBegin1->fLastPrice);

	dc.DrawLine(wxPoint(nXBegin, nYEnd), wxPoint(nXEnd, nYEnd));		
	dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXBegin, nYEnd));
	pTLineEnd1 = pTLineBegin1;
	
	return true;
}
void CTLine::DrawAveragePriceLines(wxDC& dc)
{
	if(m_nBegin < 0 || m_nBegin >=  (int)m_vecTLine.size()
		|| m_nEnd < 0 || m_nEnd >=  (int)m_vecTLine.size())
		return;

	wxPen penLine(m_clAveragePrice, 1, wxSOLID);
	dc.SetPen(penLine);

	STLine *pTLineBegin = m_vecTLine[m_nBegin];
	double dbVolumeCountBegin  = 0;
	double dbdwHoldVolumeBegin = 0;
	double dbVolumeCountEnd  = pTLineBegin->dwVolume;
	double dbdwHoldVolumeEnd = pTLineBegin->dwVolumeMoney;
	for( int i = m_nBegin-1; i>= m_nEnd; i-- )
	{
		int nXBegin = Time2X(pTLineBegin->dwTime);
		int nYBegin = JiaGe2GaoDu(dbdwHoldVolumeEnd/(m_dbVolumeMultiple*dbVolumeCountEnd));

		STLine *pTLineEnd = m_vecTLine[i];
		dbdwHoldVolumeBegin += pTLineEnd->dwVolumeMoney;
		dbVolumeCountBegin  += pTLineEnd->dwVolume;

		int nXEnd = Time2X(pTLineEnd->dwTime);
		int nYEnd = JiaGe2GaoDu(dbdwHoldVolumeBegin/(m_dbVolumeMultiple*dbVolumeCountBegin));
	
		dbdwHoldVolumeEnd	=  dbdwHoldVolumeBegin;
		dbVolumeCountEnd	=  dbVolumeCountBegin;

		

		if(nXBegin == -1 || nXEnd == -1)
			continue;

		if(dbVolumeCountBegin < 0.00001 && dbVolumeCountBegin> -0.00001)
			continue;
		if(dbVolumeCountEnd < 0.00001 && dbVolumeCountEnd> -0.00001)
			continue;
		dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXEnd, nYEnd));		

		pTLineBegin			=  pTLineEnd;
	}
}
void CTLine::SetTextColor(wxDC& dc, double dbClosePrice)
{
	double dbPrevious = m_fLastClosePrice;
	
	if(dbClosePrice - dbPrevious > -0.00001 && dbClosePrice - dbPrevious < 0.00001)
		dc.SetTextForeground(m_hengpan); 
	else if(dbClosePrice > dbPrevious)
		dc.SetTextForeground(m_zhang); 
	else if(dbClosePrice < dbPrevious)
		dc.SetTextForeground(m_die); 
}
void CTLine::DrawCurSelLine(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + 1);
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() -2);

	wxPen penBorder(wxColour(m_clCurSelLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	if(m_ptMouseMoveLast.x >= rtClient.GetLeft()	&& m_ptMouseMoveLast.x <= rtClient.GetRight()+1)
	{		
		dc.DrawLine(m_ptMouseMoveLast.x, rtClient.GetTop(), m_ptMouseMoveLast.x, rtClient.GetBottom());
	}
	if(m_ptMouseMoveLast.y >= rtClient.GetTop() && m_ptMouseMoveLast.y <= rtClient.GetBottom())
		dc.DrawLine(rtClient.GetLeft(), m_ptMouseMoveLast.y, rtClient.GetRight(), m_ptMouseMoveLast.y);	

}
void CTLine::DrawTips(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + 1);
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() -2);

	wxPen penBorder(wxColour(m_clCurSelLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	if(m_ptMouseMoveLast.x >= rtClient.GetLeft()	&& m_ptMouseMoveLast.x <= rtClient.GetRight())
	{		
		if(m_bShowTime)
		{
			DrawTips_Time(dc, rtClient, m_ptMouseMoveLast.x, m_bIsMouse);
		}
	}
	if(m_ptMouseMoveLast.y >= rtClient.GetTop() && m_ptMouseMoveLast.y <= rtClient.GetBottom())
	{
		if(m_bShowHeight)
		{
			double dbPrice = 0.0;
			if(m_bIsKeyBoard)
			{
				if(m_nCurSel < m_nEnd || m_nCurSel >= (int)m_vecTLine.size())
					return;
				STLine *pKLine = m_vecTLine[m_nCurSel];
				dbPrice = pKLine->fLastPrice;
			}
			if(m_bIsMouse)
				dbPrice = GaoDu2JiaGe(m_ptMouseMoveLast.y);

			wxFont wxfont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 
			dc.SetFont(wxfont);
			wxString  strText;
			wxCoord w,h;
			wxPoint pt;
			int nTop = rtClient.GetTop() - 10;
		//	strText = wxString::Format(wxT("%.3f"), dbPrice);
			
			strText = Double2String_Tip(dbPrice);
			//dc.GetTextExtent(strText, &w, &h);	
			wxString strLength = wxT("1234.56");
			dc.GetTextExtent(strLength, &w, &h);	
			w = w + 4;

			int nLeft =  rtClient.GetRight()+1;
			int nRectTop  = m_ptMouseMoveLast.y - h+1;
			if(nRectTop <= 0)
				nRectTop = 0;
			wxBrush brush(m_clTipsBk);
			dc.SetBrush(brush);
			wxPen penBorder(wxColour(m_zhang), 1, wxSOLID);
			dc.SetPen(penBorder);
			dc.DrawRectangle(nLeft, nRectTop, w, h);

			dc.SetTextForeground(m_clCurSelLine); 
			pt.x = nLeft+2;
			pt.y = nRectTop; 
			dc.DrawText(strText, pt);

			nLeft		=  rtClient.GetLeft() - w-2;			
			pt.x = nLeft+2;
			pt.y = nRectTop; 
			dc.DrawRectangle(nLeft, nRectTop, w, h);
			dc.DrawText(strText, pt);
		}
	}

}

void CTLine::DrawTips_Time(wxDC& dc, wxRect rtClient, int nX, bool bIsMouse)
{
	wxString strTextBottom;
	if(bIsMouse)
	{//主要是盘中停盘，没有数据
		struct tm dwTime = X2Time(nX);
		strTextBottom.Printf(_T("%.2d:%.2d"), dwTime.tm_hour, dwTime.tm_min);
		if (strTextBottom.Length() < 4)
			return;
	}
	else
	{
		if(m_nCurSel< m_nEnd || m_nCurSel > m_nBegin || m_nCurSel <0 || m_nCurSel>= (int)m_vecTLine.size())
			return;
		STLine *pTLine = m_vecTLine[m_nCurSel];	
		strTextBottom.Printf(_T("%.2d:%.2d"), pTLine->dwTime.tm_hour, pTLine->dwTime.tm_min);
		if (strTextBottom.Length() < 4)
			return;
	}
	wxFont wxfont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 
	dc.SetFont(wxfont);
	wxCoord w,h;
	wxPoint pt;
	int nTop = rtClient.GetTop() - 10;	
	dc.GetTextExtent(strTextBottom, &w, &h);	

	int nLeft =  nX+1;
	if(nX + w > rtClient.GetRight())
		nLeft = nX -w-1;
	int nRectTop  = rtClient.GetBottom() - h;
	wxBrush brush(m_clTipsBk);
	dc.SetBrush(brush);
	dc.DrawRectangle(nLeft, nRectTop, w, h);


	dc.SetTextForeground(m_clCurSelLine); 
	pt.x = nLeft;
	pt.y = nRectTop; 
	dc.DrawText(strTextBottom, pt);

}
void CTLine::DrawOneItemData(wxDC& dc,  wxRect rtClient)
{
	if(m_nBegin <0 || m_nBegin >= (int)m_vecTLine.size())
		return;
//	if(m_nBegin == -1)
//		return;
	wxRect rtAll = GetClientRect();
	int nHeightMax = rtAll.GetHeight()-2;
	int nHeight = rtClient.GetHeight()-2;
	if(nHeight > nHeightMax)
		nHeight = nHeightMax;
	wxBrush brush(m_clBackGround);	
	dc.SetBrush(brush);
	dc.DrawRectangle(1, 1, rtClient.GetWidth()-1, nHeight);

	rtClient.SetLeft(rtClient.GetLeft() -1);
	rtClient.SetRight(rtClient.GetRight() -1);

	wxFont wxfont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	dc.SetFont(wxfont);

	int nTop = 0;
	wxString  strText, strText2;
	wxCoord w,h;
	wxPoint pt;
	strText = wxT("时    间");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = rtClient.GetTop() + 1; 
	dc.SetTextForeground(m_clName); 
	dc.DrawText(strText, pt);
	nTop = rtClient.GetTop() + 1 + h;

	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine *pTLine = m_vecTLine[m_nCurSel];
	//	GetTimeText(m_enumPhrase, strText, strText2);
		strText.Printf(_T("%2.2d:%2.2d"), pTLine->dwTime.tm_hour, pTLine->dwTime.tm_min);
		dc.SetTextForeground(m_clTime); 
	}
	else 
	{
		strText  = wxT("----");
		strText2 = wxT("----");
	}
	if(nHeightMax <nTop + h*2 + 2 )
		return;

	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.SetTextForeground(m_clTime); 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 3;
/*
	dc.GetTextExtent(strText2, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.SetTextForeground(m_clTime); 
	dc.DrawText(strText2, pt);
	nTop = nTop + h + 1;*/
	//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + h*2 + 2 )
		return;

	strText = wxT("价    格");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_clName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine *pTLine = m_vecTLine[m_nCurSel];
	//	strText.Printf(_T("%.2f"), pTLine->fLastPrice);
		strText = Double2String( pTLine->fLastPrice, m_nNumDigits);
		SetTextColor(dc, pTLine->fLastPrice);
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 3;
//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + h*2 + 2 )
		return;

	strText = wxT("均    价");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_clName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine *pTLineBegin = m_vecTLine[m_nBegin];
		double dbVolumeCountBegin  = pTLineBegin->dwVolume;
		double dbdwHoldVolumeBegin = pTLineBegin->dwVolumeMoney;
		double dbVolumeCountEnd  = 0;
		double dbdwHoldVolumeEnd = 0;
		for( int i = m_nBegin-1; i>= m_nCurSel; i-- )
		{
			STLine *pTLineEnd = m_vecTLine[i];
			dbdwHoldVolumeBegin += pTLineEnd->dwVolumeMoney;
			dbVolumeCountBegin  += pTLineEnd->dwVolume;
			dbdwHoldVolumeEnd	=  dbdwHoldVolumeBegin;
			dbVolumeCountEnd	=  dbVolumeCountBegin;
			pTLineBegin			=  pTLineEnd;
		}
		if(dbVolumeCountBegin<0.00001)
			strText = wxT("0.00");
		else
			strText = Double2String(dbdwHoldVolumeBegin/(m_dbVolumeMultiple*dbVolumeCountBegin), m_nNumDigits);
		//	strText.Printf(_T("%.2f"), dbdwHoldVolumeBegin/(m_dbVolumeMultiple*dbVolumeCountBegin));
		dc.SetTextForeground(m_clTime); 
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 3;

	//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + h*2 + 2 )
		return;

	strText = wxT("涨    跌");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_clName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine *pTLine = m_vecTLine[m_nCurSel];
	//	strText.Printf(wxT("%.2f"), pTLine->fLastPrice - m_fLastClosePrice);	
		strText = Double2String( pTLine->fLastPrice - m_fLastClosePrice, m_nNumDigits);
		SetTextColor(dc, pTLine->fLastPrice);
			
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 3;

	//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + h*2 + 2 )
		return;

	strText = wxT("幅    度");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_clName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine *pTLine = m_vecTLine[m_nCurSel];

		if(m_fLastClosePrice - MIN_VALUE_DEFAULT > -0.00001 && m_fLastClosePrice - MIN_VALUE_DEFAULT < 0.00001)	
		{
			strText = wxT("----");				
		}
		else
		{
			strText.Printf(wxT("%.2f%%"), 100*(pTLine->fLastPrice - m_fLastClosePrice)/m_fLastClosePrice);	
			SetTextColor(dc, pTLine->fLastPrice);
		}				
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 3;

	if(nHeightMax <nTop + h*2 + 2 )
		return;

	strText = wxT("成 交 量");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_clName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine *pTLine = m_vecTLine[m_nCurSel];
		strText.Printf(_T("%d"), pTLine->dwVolume);
		dc.SetTextForeground(m_clTime); 
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 3;

}
void CTLine::DrawHead(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());

	wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 	
	dc.SetTextForeground(m_clLineName); 

	wxString  strText;
	wxCoord w,h;
	wxPoint pt;

	if(m_ContractDate == "")
		strText =  m_ContractName;
	else
		strText.Printf(_T("%s_%s"), m_ContractName.c_str(), m_ContractDate.c_str());
	//	strText.Printf(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());
	

	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + 5;
	pt.y = rtClient.GetTop() -h; 
	dc.DrawText(strText, pt);


	dc.SetTextForeground(m_clLineName); 
	wxCoord w2,h2;
	wxString strLineName = wxT("分时走势");
	
	dc.GetTextExtent(strLineName, &w2, &h2);	
	pt.x = pt.x + w + 2;
	pt.y = rtClient.GetTop() -h2; 
	dc.DrawText(strLineName, pt);
}
void CTLine::SetCursel(STItem& sItem)
{
	if(sItem.nItem >=0 && sItem.nItem < (int)m_vecTLine.size())
		m_nCurSel = sItem.nItem;

	m_bIsMouse		= sItem.bIsMouse;
	m_bIsKeyBoard	= sItem.bIsKeyBoard;
	//	CalcMaxMin();		
	m_ptMouseMoveLast.x = sItem.wxPt.x;
	m_ptMouseMoveLast.y = -1;
	if(m_bIsKeyBoard)
	{
		if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
		{
			STLine *pTLine = m_vecTLine[m_nCurSel];
			float fClosePrice = pTLine->fLastPrice;
			int nHeight = JiaGe2GaoDu(fClosePrice);				
			m_ptMouseMoveLast.y =  nHeight;
		}
	}
	Refresh(false);

}
bool CTLine::InitCfg(TiXmlElement *root)
{
	TiXmlElement *tList = root->FirstChildElement("tline");
	if(tList == NULL)
		return false;

	TiXmlNode *leftspanwidth = tList->FirstChild("leftspanwidth");
	if(leftspanwidth == NULL)
		return false;
	const char *strleftspanwidth = leftspanwidth->ToElement()->GetText();
	unsigned long valueSpan = ColorStr2Long(strleftspanwidth);
	SetLeftSpanWidth(valueSpan);

	TiXmlNode *pline = tList->FirstChild("line");
	if(pline == NULL)
		return false;

	TiXmlElement* spanline = pline->FirstChildElement("spanline");
	if(spanline == NULL)
		return false;
	const char *ptype = spanline->Attribute("type");
	unsigned long type = ColorStr2Long(ptype);
	ptype = spanline->Attribute("size");
	unsigned long size = ColorStr2Long(ptype);
	SetSpanLine(type, size);

	spanline = pline->FirstChildElement("hourline");
	if(spanline == NULL)
		return false;
	ptype = spanline->Attribute("type");
	type = ColorStr2Long(ptype);
	ptype = spanline->Attribute("size");
	size = ColorStr2Long(ptype);
	SetHourLineLine(type, size);

	spanline = pline->FirstChildElement("halfhourline");
	if(spanline == NULL)
		return false;
	ptype = spanline->Attribute("type");
	type = ColorStr2Long(ptype);
	ptype = spanline->Attribute("size");
	size = ColorStr2Long(ptype);
	SetHalfHourLine(type, size);


	TiXmlNode *pColor = tList->FirstChild("color");
	if(pColor == NULL)
		return false;

	TiXmlNode *ListItem = pColor->FirstChild("background");
	if(ListItem == NULL)
		return false;
	const char *strValue = ListItem->ToElement()->GetText();
	unsigned long value = ColorStr2Long(strValue);
	SetBackGround(value);


	ListItem = pColor->FirstChild("border");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetBorder(value);


	ListItem = pColor->FirstChild("price");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetPriceColor(value);


	ListItem = pColor->FirstChild("averageprice");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetAveragePriceColor(value);

	

	ListItem = pColor->FirstChild("name");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetNameColor(value);


	ListItem = pColor->FirstChild("time");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetTimeColor(value);


	ListItem = pColor->FirstChild("zhang");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetzhangColor(value);


	ListItem = pColor->FirstChild("die");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetdieColor(value);


	ListItem = pColor->FirstChild("hengpan");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SethengpanColor(value);



	ListItem = pColor->FirstChild("curselline");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetCurSelLineColor(value);

	ListItem = pColor->FirstChild("linename");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineNameColor(value);

	ListItem = pColor->FirstChild("tipsbk");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetTipsBkColor(value);

	return true;
}
void CTLine::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CTLine::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CTLine::SetdashLine(unsigned long lValue)
{
//	m_cldashLine = lValue;
}
void CTLine::SetPriceColor(unsigned long lValue)
{
	m_clPrice = lValue;
}
void CTLine::SetAveragePriceColor(unsigned long lValue)
{
	m_clAveragePrice = lValue;
}
void CTLine::SetCurSelLineColor(unsigned long lValue)
{
	m_clCurSelLine = lValue;
}
void CTLine::SetTipsBkColor(unsigned long lValue)
{
	m_clTipsBk = lValue;
}
void CTLine::SetNameColor(unsigned long lValue)
{
	m_clName = lValue;
}
void CTLine::SetTimeColor(unsigned long lValue)
{
	m_clTime = lValue;
}
void CTLine::SetzhangColor(unsigned long lValue)
{
	m_zhang = lValue;
}
void CTLine::SetdieColor(unsigned long lValue)
{
	m_die = lValue;
}
void CTLine::SethengpanColor(unsigned long lValue)
{
	m_hengpan = lValue;
}
void CTLine::SetLineNameColor(unsigned long lValue)
{
	m_clLineName = lValue;
}
void CTLine::SetSpanLine(int nLineType, int	nLineSize)
{
	m_nSpanLineType = nLineType;
	m_nSpanLineSize = nLineSize;
}
void CTLine::SetHourLineLine(int nLineType, int	nLineSize)
{
	m_nHourLineType = nLineType;
	m_nHourLineSize = nLineSize;
}
void CTLine::SetHalfHourLine(int nLineType, int	nLineSize)
{
	m_nHalfHourLineLineType = nLineType;
	m_nHalfHourLineSize = nLineSize;
}