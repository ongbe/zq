#include "stdafx.h"
#include "../inc/Quotation/KScales.h"





IMPLEMENT_CLASS(CKScales, CKCtrl)







CKScales::CKScales(void)//:m_timer1(this, 1111),m_timer2(this, 1112)
{ 
	m_clBackGround	= wxColour(RGB(0,0,0));
	m_clBorder		= wxColour(RGB(192,0,0));

	m_clRedLine		= wxColour(RGB(192,106,105));
	m_clGreenLine	= wxColour(RGB(80,255,255));
	m_cldashLine	= wxColour(RGB(76,40,40));
	m_clCurSelLine  = wxColour(RGB(200,200,200));

	m_clTipsBk		= wxColour(RGB(0,0,128));
	m_clTriangle    = wxColour(RGB(255,255,0));
	m_clText		= wxColour(RGB(192,0,0));
	
	m_nBorderWidth	= 1;

	m_nShowCount	= 100;
	GetShowCount(m_nShowCount);

	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue = MAX_VALUE_DEFAULT;

	m_nBegin = 0;
	m_nEnd   = 0;
	m_RectOffSet =  wxRect(66,16,2,5);
	m_nCurSel  = -1;
	
	m_nLeftPos  = 1;
	m_nRightPos = 1;

	m_uTimerarr[0] = -1;
	m_uTimerarr[1] = -1;
	m_bMaxMinChange		= TRUE;



	m_pMsgWnd = NULL;

	m_bIsMouse		= false;
	m_bIsKeyBoard	= false;

	m_bShowHeight   = false;
	m_bShowTime     = true;
    
	m_enumPhrase    = PHRASE_USERDEFINE;

	m_ptMouseMoveLast = wxPoint(-1,-1);

	m_bShowCross	   = false;
	m_bMouseIn		   = false;

	m_strTechIndexName = wxT("");
	m_KType = KTYPE_SCALES;
}
CKScales::~CKScales(void)
{
	m_vecKLine.clear();
}
BEGIN_EVENT_TABLE(CKScales, CKCtrl)
	EVT_PAINT(CKScales::OnPaint)
	EVT_LEFT_DOWN(CKScales::OnLeftDown)
	EVT_LEFT_UP(CKScales::OnLeftUp)
	EVT_RIGHT_UP(CKScales::OnRightUp)
END_EVENT_TABLE()


void CKScales::CalcMaxMin()
{
}
void CKScales::OnPaint (wxPaintEvent & PaintEvent)
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
void CKScales::DrawDC(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxBrush brush(m_clBackGround);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.x, size.y);


	wxRect rt(0, 0, size.x, size.y);
	DrawBorder(dc, rt);
	DrawLeftText(dc);
	
	switch(m_enumPhrase)
	{		
	case PHRASE_1MIN:
		DrawItems_MINS(dc);
		break;
	case PHRASE_5MIN:
		DrawItems_MINS(dc);
		
		break;
	case PHRASE_15MIN:
		DrawItems_MINS(dc);
		
		break;
	case PHRASE_30MIN:
		DrawItems_MINS(dc);
		
		break;
	case PHRASE_60MIN:
		DrawItems_MINS(dc);		
		break;
	case PHRASE_DAY:
		DrawItems_Day(dc);
		
		break;
	case PHRASE_WEEK:
		DrawItems_MINS(dc);
		
		break;
	case PHRASE_MONTH:
		DrawItems_MINS(dc);
		
		break;
	}
	if(m_bMouseIn)
		DrawTips(dc);
	if(m_bMouseIn && m_bShowCross)
		DrawCurSelLine(dc);	
}

void CKScales::DrawLeftText(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 
	dc.SetTextForeground(m_clText); 

	wxString  strText;
	wxCoord w,h;
	wxPoint pt;
	switch(m_enumPhrase)
	{		
		case PHRASE_1MIN:
			strText = wxT("1分钟");
			break;
		case PHRASE_5MIN:
			strText = wxT("5分钟");
			break;
		case PHRASE_15MIN:
			strText = wxT("15分钟");
			break;
		case PHRASE_30MIN:
			strText = wxT("30分钟");
			break;
		case PHRASE_60MIN:
			strText = wxT("60分钟");
			break;
		case PHRASE_DAY:
			strText = _T("日线");
			break;
		case PHRASE_WEEK:
			strText = wxT("周线");
			break;
		case PHRASE_MONTH:
			strText = wxT("月线");
			break;
	}
	
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() +2;
	pt.y = (rtClient.GetHeight() -h)/2; 
	dc.DrawText(strText, pt);

	wxPoint points[3];
	points[0]= wxPoint(m_RectOffSet.GetLeft() -3, rtClient.GetBottom()-3);
	points[1]= wxPoint(m_RectOffSet.GetLeft() -20, rtClient.GetBottom()-3);
	points[2]= wxPoint(m_RectOffSet.GetLeft() -12, rtClient.GetTop()+3);
	wxPen penBorder(wxColour(m_clTriangle), 1, wxSOLID);
	dc.SetPen(penBorder);
	wxBrush brushHatch(wxColour(m_clTriangle), wxSOLID);
	dc.SetBrush(brushHatch);
	dc.DrawPolygon(WXSIZEOF(points), points, 0, 0, wxWINDING_RULE);

}
void  CKScales::DrawItems_Day(wxDC& dc)
{
	if(m_vecKLine.size() == 0)
		return;

	wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 
	dc.SetTextForeground(m_clBorder); 
	wxPen penBorder(wxColour(m_clBorder), 1, wxSOLID);
	dc.SetPen(penBorder);

	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());

	int nLastYear = 0, nLastMonth = 0, nLastDay = 0;
	wxDateTime::WeekDay nLastWeek = wxDateTime::WeekDay::Mon;
	wxCoord w,h;
	wxPoint pt;
	int nFirstEnd =0;
	for(int i = m_nBegin; i>= m_nEnd; i--)
	{
		SKLine * pKLine = m_vecKLine[i];
		wxASSERT(pKLine != NULL);
		
		wxString strText;
		if(i == m_nBegin)
		{
			strText.Printf(wxT("%4.4d/%2.2d"), pKLine->dwTime.tm_year+1900, pKLine->dwTime.tm_mon+1);
			dc.GetTextExtent(strText, &w, &h);	
			pt.x = rtClient.GetLeft();
			pt.y = (rtClient.GetHeight() -h)/2; 			
			dc.DrawText(strText, pt);

			nLastYear = pKLine->dwTime.tm_year;
			nLastMonth =  pKLine->dwTime.tm_mon;
			nLastDay =  pKLine->dwTime.tm_mday;
					
			wxDateTime date(pKLine->dwTime.tm_mday, wxDateTime::Month(pKLine->dwTime.tm_mon), pKLine->dwTime.tm_year+1900, 0, 0, 0, 0);
			nLastWeek = date.GetWeekDay();	
			nFirstEnd = pt.x + w;
			continue;
		}	
		
	
		wxDateTime date(pKLine->dwTime.tm_mday, wxDateTime::Month(pKLine->dwTime.tm_mon), pKLine->dwTime.tm_year+1900, 0, 0, 0, 0);
		wxDateTime::WeekDay week = date.GetWeekDay();	
		if(week <= nLastWeek ||week == wxDateTime::WeekDay::Mon)
		{
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(i, nLeft, nRight);
			if((nLeft+nRight)/2 > nFirstEnd)
				dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetTop()+4);	
		}	
		
		if(pKLine->dwTime.tm_year > nLastYear)
		{
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(i, nLeft, nRight);		
			if((nLeft+nRight)/2 > nFirstEnd)
				dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
		
			strText.Printf(wxT("%4.4d/%2.2d"), pKLine->dwTime.tm_year+1900, pKLine->dwTime.tm_mon+1);
			dc.GetTextExtent(strText, &w, &h);	
			pt.x = (nLeft+nRight)/2;
			pt.y = (rtClient.GetHeight() -h)/2; 
			if(pt.x > nFirstEnd)			
				dc.DrawText(strText, pt);
		}
		else if(pKLine->dwTime.tm_mon > nLastMonth)
		{
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(i, nLeft, nRight);		
			if((nLeft+nRight)/2 > nFirstEnd)
				dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());

			strText.Printf(wxT("%2.2d"), pKLine->dwTime.tm_mon+1);
			dc.GetTextExtent(strText, &w, &h);	
			pt.x = (nLeft+nRight)/2;
			pt.y = (rtClient.GetHeight() -h)/2; 	
			if(pt.x > nFirstEnd)
				dc.DrawText(strText, pt);
		}
		nLastYear = pKLine->dwTime.tm_year;
		nLastMonth = pKLine->dwTime.tm_mon;
		nLastDay = pKLine->dwTime.tm_mday;
		nLastWeek = week;
	}

}
void CKScales::DrawItems_MINS(wxDC& dc)
{
	if(m_vecKLine.size() == 0)
		return;

	wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 
	dc.SetTextForeground(m_clBorder); 
	wxPen penBorder(wxColour(m_clBorder), 1, wxSOLID);
	dc.SetPen(penBorder);

	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());

	struct tm tmPrevious;
	wxCoord w,h;
	wxPoint pt;

	int nFirstEnd = 0;
	int nItemSpan = 0;//间隔次数
	for(int i = m_nBegin; i>= m_nEnd; i--)
	{
		SKLine * pKLine = m_vecKLine[i];
		wxASSERT(pKLine != NULL);
		wxString strText;
		if(i == m_nBegin)
		{
			strText.Printf(wxT("%.2d/%.2d/%2.2d"), pKLine->dwTime.tm_year + 1900, pKLine->dwTime.tm_mon+1, pKLine->dwTime.tm_mday);
			dc.GetTextExtent(strText, &w, &h);	
			pt.x = rtClient.GetLeft();
			pt.y = (rtClient.GetHeight() -h)/2; 			
			dc.DrawText(strText, pt);

			tmPrevious = pKLine->dwTime;
			nFirstEnd = pt.x +w;
			continue;
		}	
		
		ProcessMins(dc, m_enumPhrase, i, nItemSpan, pKLine->dwTime, tmPrevious, nFirstEnd);
	}
}
void CKScales::ProcessMins(wxDC& dc, EnumPhrase enumPhrase, int i, int& nItemSpan, struct tm tm33, struct tm& tmPrevious, int nFirstEnd)
{
	wxCoord w,h;
	wxPoint pt;
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	wxString strText;
	switch(enumPhrase)
	{		
	case PHRASE_1MIN:
		{
			struct tm tm1(tm33);
		//	tm1.tm_year -= 1900; //不减去1900就返回-1
			time_t t1 = mktime( &tm1 );
			struct tm tm2(tmPrevious);
		//	tm2.tm_year -= 1900; 
			time_t t2 = mktime( &tm2);
			int dw = difftime(t1, t2);		
			if((dw%300 == 0 && m_nDayWidth >8)
				||(dw%600 == 0 && m_nDayWidth >4)
				||(dw%1200 == 0 && (m_nDayWidth <= 4 && m_nDayWidth >= 2) )
				||(dw%2400 == 0 && m_nDayWidth ==1)
				|| dw>3600)//每5分钟画一次，5*60秒
			{
				tmPrevious = tm33;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);		
				if((nLeft+nRight)/2 > nFirstEnd)
					dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				strText.Printf(wxT("%2.2d:%2.2d"), tm1.tm_hour, tm1.tm_min);
				dc.GetTextExtent(strText, &w, &h);	
				pt.x = (nLeft+nRight)/2;
				pt.y = (rtClient.GetHeight() -h)/2; 	
				if(pt.x > nFirstEnd)
					dc.DrawText(strText, pt);
			}
			
		}
		break;
		
	case PHRASE_5MIN:
		{
			struct tm tm1(tm33);
			struct tm tm2(tmPrevious);
			if(tm2.tm_mday != tm1.tm_mday)
			{
				tmPrevious = tm33;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
					dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				strText.Printf(wxT("%2.2d/%2.2d"), tm33.tm_mon, tm33.tm_mday);
				dc.GetTextExtent(strText, &w, &h);	
				pt.x = (nLeft+nRight)/2;
				pt.y = (rtClient.GetHeight() -h)/2; 
				if(pt.x > nFirstEnd)
					dc.DrawText(strText, pt);
			}
		}
		break;
	case PHRASE_15MIN:
		{
			struct tm tm1(tm33);
			struct tm tm2(tmPrevious);
			if((tm2.tm_mday != tm1.tm_mday && m_nDayWidth >1) || nItemSpan>= 3)
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
					dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				strText.Printf(wxT("%2.2d/%2.2d"), tm33.tm_mon, tm33.tm_mday);
				dc.GetTextExtent(strText, &w, &h);	
				pt.x = (nLeft+nRight)/2;
				pt.y = (rtClient.GetHeight() -h)/2; 
				if(pt.x > nFirstEnd)
					dc.DrawText(strText, pt);
			}
			else
			{
				if(tm2.tm_mday != tm1.tm_mday) 
					nItemSpan++;
			}
			tmPrevious = tm33;
		}
		break;
	case PHRASE_30MIN:
		{
			struct tm tm1(tm33);
			struct tm tm2(tmPrevious);
			if((tm2.tm_mday != tm1.tm_mday && m_nDayWidth >=4) 
				|| (nItemSpan>= 3 && m_nDayWidth <4 && m_nDayWidth >1)
				||  (nItemSpan>= 5 && m_nDayWidth == 1))
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
					dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				strText.Printf(wxT("%2.2d/%2.2d"), tm33.tm_mon, tm33.tm_mday);
				dc.GetTextExtent(strText, &w, &h);	
				pt.x = (nLeft+nRight)/2;
				pt.y = (rtClient.GetHeight() -h)/2; 	
				if(pt.x > nFirstEnd)
					dc.DrawText(strText, pt);
			}
			else
			{
				if(tm2.tm_mday != tm1.tm_mday) 
					nItemSpan++;
			}
			tmPrevious = tm33;
		}
		break;
	case PHRASE_60MIN:
		{
			struct tm tm1(tm33);
			struct tm tm2(tmPrevious);
			if((nItemSpan>= 3 && m_nDayWidth >=4) 
				|| (nItemSpan>= 6 && m_nDayWidth <4 && m_nDayWidth >1)
				||  (nItemSpan>= 10 && m_nDayWidth == 1))
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
					dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				strText.Printf(wxT("%2.2d/%2.2d"), tm33.tm_mon, tm33.tm_mday);
				dc.GetTextExtent(strText, &w, &h);	
				pt.x = (nLeft+nRight)/2;
				pt.y = (rtClient.GetHeight() -h)/2; 
				if(pt.x > nFirstEnd)
					dc.DrawText(strText, pt);
			}
			else
			{
				if(tm2.tm_mday != tm1.tm_mday) 
					nItemSpan++;
			}
			tmPrevious = tm33;
		}
		break;
	case PHRASE_DAY:
		{

		}
		break;
	case PHRASE_WEEK:
		{			
			if(tm33.tm_mon%2 == 1&& (tmPrevious.tm_year != tm33.tm_year|| (tmPrevious.tm_year == tm33.tm_year && tmPrevious.tm_mon != tm33.tm_mon)) && m_nDayWidth>=6 
				|| (tm33.tm_mon%3 == 1&& (tmPrevious.tm_year != tm33.tm_year|| (tmPrevious.tm_year == tm33.tm_year && tmPrevious.tm_mon != tm33.tm_mon)) && m_nDayWidth < 6 && m_nDayWidth >1)
				|| (tm33.tm_mon == 1&& (tmPrevious.tm_year != tm33.tm_year|| (tmPrevious.tm_year == tm33.tm_year && tmPrevious.tm_mon != tm33.tm_mon)) && m_nDayWidth == 1 ))
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
					dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				if(tm33.tm_mon == 1)
					strText.Printf(wxT("%4.4d/%2.2d"), tm33.tm_year, tm33.tm_mon);
				else
					strText.Printf(wxT("%2.2d"), tm33.tm_mon);
				dc.GetTextExtent(strText, &w, &h);	
				pt.x = (nLeft+nRight)/2;
				pt.y = (rtClient.GetHeight() -h)/2; 
				if(pt.x > nFirstEnd)
					dc.DrawText(strText, pt);
			}
			
			tmPrevious = tm33;
		}
		break;
	case PHRASE_MONTH:
		{			
			if(tmPrevious.tm_year != tm33.tm_year)
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);
				if((nLeft+nRight)/2 > nFirstEnd)
					dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());				
				strText.Printf(wxT("%4.4d/%2.2d"), tm33.tm_year, tm33.tm_mon);
				dc.GetTextExtent(strText, &w, &h);	
				pt.x = (nLeft+nRight)/2;
				pt.y = (rtClient.GetHeight() -h)/2; 
				if(pt.x > nFirstEnd)
					dc.DrawText(strText, pt);
			}
			tmPrevious = tm33;
		}
		break;
	}


}

void CKScales::DrawBorder(wxDC& dc, wxRect& rt)
{
	wxPen penBorder(wxColour(m_clBorder), m_nBorderWidth, wxSOLID);
	dc.SetPen(penBorder);
	dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetRight(), rt.GetTop());
	dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetLeft(), rt.GetBottom());
	dc.DrawLine(rt.GetLeft(), rt.GetBottom(), rt.GetRight(), rt.GetBottom());
	dc.DrawLine(rt.GetRight(), rt.GetBottom(), rt.GetRight(), rt.GetTop());

	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() );
	dc.DrawLine(rtClient.GetLeft(), rtClient.GetTop(), rtClient.GetLeft(), rtClient.GetBottom());
}
void CKScales::DrawCurSelLine(wxDC& dc)
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
		dc.DrawLine(m_ptMouseMoveLast.x, rtClient.GetTop(), m_ptMouseMoveLast.x, rtClient.GetBottom());
	}


	
}
void CKScales::DrawTips(wxDC& dc)
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
			DrawTips_Time(dc, rtClient, m_ptMouseMoveLast.x);
			
		}
	}
	if(m_ptMouseMoveLast.y >= rtClient.GetTop() && m_ptMouseMoveLast.y <= rtClient.GetBottom())
	{
		if(m_bShowHeight)
		{
			double dbPrice = 0.0;
			if(m_bIsKeyBoard)
			{
				if(m_nCurSel < 0 || m_nCurSel >= (int)m_vecKLine.size())
					return;
				SKLine *pKLine = m_vecKLine[m_nCurSel];
				dbPrice = pKLine->fClosePrice;
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
			strText = Double2String(dbPrice, 2);
			dc.GetTextExtent(strText, &w, &h);	

			int nLeft =  rtClient.GetRight() - w;
			int nRectTop  = m_ptMouseMoveLast.y - h;
			if(nRectTop <= 0)
				nRectTop = 0;
			wxBrush brush(m_clTipsBk);
			dc.SetBrush(brush);
			dc.DrawRectangle(nLeft, nRectTop, w, h);

		
			dc.SetTextForeground(m_clCurSelLine); 
			pt.x = nLeft;
			pt.y = nRectTop; 
			dc.DrawText(strText, pt);
		}
	}	
}
void CKScales::DrawTips_Time(wxDC& dc, wxRect rtClient, int nX)
{
	if(m_nCurSel< m_nEnd || m_nCurSel > m_nBegin||m_nCurSel < 0 || m_nCurSel >= (int)m_vecKLine.size())
		return;

	SKLine *pKLine =  NULL;
	if(m_bIsKeyBoard)
	{
		pKLine = m_vecKLine[m_nCurSel];
	}
	if(m_bIsMouse)
	{
		int nCurSel = WeiZhi2XuHao(nX);
		if(nCurSel < 0 || nCurSel >= (int)m_vecKLine.size())
			return;
		pKLine = m_vecKLine[m_nCurSel];
	}	
	wxASSERT(pKLine != NULL);

	wxString strTextBottom;
	switch(m_enumPhrase)
	{		
	case PHRASE_1MIN:
	case PHRASE_5MIN:
	case PHRASE_15MIN:
	case PHRASE_30MIN:
	case PHRASE_60MIN:
		strTextBottom.Printf(wxT("%2.2d/%2.2d,%2.2d:%2.2d"), pKLine->dwTime.tm_mon+1, pKLine->dwTime.tm_mday, pKLine->dwTime.tm_hour, pKLine->dwTime.tm_min);
		break;
	case PHRASE_DAY:
	case PHRASE_WEEK:
	case PHRASE_MONTH:
		{
			wxDateTime date(pKLine->dwTime.tm_mday, wxDateTime::Month(pKLine->dwTime.tm_mon), pKLine->dwTime.tm_year+1900, 0, 0, 0, 0);
			wxDateTime::WeekDay week = date.GetWeekDay();	
			wxString strText = GetWeekName(week);
			strTextBottom.Printf(wxT("%4.4d/%2.2d/%2.2d(%s)"), pKLine->dwTime.tm_year+1900, pKLine->dwTime.tm_mon+1, pKLine->dwTime.tm_mday, strText);
			if (strTextBottom.Length() < 10)
				return;
		}
		break;
	}	
	wxFont wxfont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 
	dc.SetFont(wxfont);
	wxCoord w,h;
	wxPoint pt;
	int nTop = rtClient.GetTop() - 10;	
	dc.GetTextExtent(strTextBottom, &w, &h);	

	w= w+4;
	int nLeft =  nX;
	if(nX + w > rtClient.GetRight())
		nLeft = nX -w+1;
	int nRectTop  = rtClient.GetBottom() - h;
	wxBrush brush(m_clTipsBk);
	dc.SetBrush(brush);
	wxPen penBorder(wxColour(m_clRedLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	dc.DrawRectangle(nLeft, nRectTop, w, h);

	
	dc.SetTextForeground(m_clCurSelLine); 
	pt.x = nLeft +2;
	pt.y = nRectTop; 
	dc.DrawText(strTextBottom, pt);

}
void CKScales::OnLeftDown(wxMouseEvent& event)
{
	SetFocus();
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KCtrl_LBUTTONDOWN);
	myEvent.SetExtraLong(reinterpret_cast<long>(this));
	myEvent.SetClientData(NULL);
	window->ProcessEvent(myEvent);
}
void CKScales::OnLeftUp(wxMouseEvent& event)
{
}
wxString CKScales::GetWeekName(wxDateTime::WeekDay week)
{
	wxString strText = wxT("");
	switch(week)
	{
	case wxDateTime::WeekDay::Mon:
		strText = wxT("一");
		break;
	case wxDateTime::WeekDay::Tue:
		strText = wxT("二");
		break;
	case wxDateTime::WeekDay::Wed:
		strText = wxT("三");
		break;
	case wxDateTime::WeekDay::Thu:
		strText = wxT("四");
		break;
	case wxDateTime::WeekDay::Fri:
		strText = wxT("五");
		break;
	case wxDateTime::WeekDay::Sat:
		strText = wxT("六");
		break;
	case wxDateTime::WeekDay::Sun:
		strText = wxT("日");
		break;

	}
	return strText;
}
void CKScales::OnRightUp(wxMouseEvent& event)
{

}
bool CKScales::InitCfg(TiXmlElement *root)
{
	{//这部分代码和下面部分读取配置文件的代码线保留，如果以后改成分开读取公共部分配置信息，则还原即可。
		TiXmlElement *tList = root->FirstChildElement("kline");
		if(tList == NULL)
			return false;

		TiXmlNode *leftspanwidth = tList->FirstChild("leftspanwidth");
		if(leftspanwidth == NULL)
			return false;
		const char *strleftspanwidth = leftspanwidth->ToElement()->GetText();
		unsigned long valueSpan = ColorStr2Long(strleftspanwidth);
		SetLeftSpanWidth(valueSpan);
	}
	TiXmlElement *tList = root->FirstChildElement("kscales");
	if(tList == NULL)
		return false;
/*
	TiXmlNode *leftspanwidth = tList->FirstChild("leftspanwidth");
	if(leftspanwidth == NULL)
		return false;
	const char *strleftspanwidth = leftspanwidth->ToElement()->GetText();
	unsigned long valueSpan = ColorStr2Long(strleftspanwidth);
	SetLeftSpanWidth(valueSpan);
*/
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

	ListItem = pColor->FirstChild("redline");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();	
	value = ColorStr2Long(strValue);
	SetRedLine(value);

	ListItem = pColor->FirstChild("greenline");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetGreenLine(value);

	
	ListItem = pColor->FirstChild("curselline");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetCurSelLine(value);

	ListItem = pColor->FirstChild("tipbk");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetTipsBk(value);

	ListItem = pColor->FirstChild("linename");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineName(value);


	ListItem = pColor->FirstChild("cltrangle");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetTrangle(value);


	return true;
}
void CKScales::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CKScales::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CKScales::SetRedLine(unsigned long lValue)
{
	m_clRedLine = lValue;
}
void CKScales::SetGreenLine(unsigned long lValue)
{
	m_clGreenLine = lValue;
}
void CKScales::SetdashLine(unsigned long lValue)
{
	m_cldashLine = lValue;
}
void CKScales::SetCurSelLine(unsigned long lValue)
{
	m_clCurSelLine = lValue;
}
void CKScales::SetTipsBk(unsigned long lValue)
{
	m_clTipsBk = lValue;
}

void CKScales::SetTrangle(unsigned long lValue)
{
	m_clTriangle = lValue;
}
void CKScales::SetLineName(unsigned long lValue)
{
	m_clText = lValue;
}
