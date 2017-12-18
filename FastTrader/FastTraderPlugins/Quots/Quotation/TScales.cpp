#include "stdafx.h"
#include "../../../inc/Quotation/TScales.h"




IMPLEMENT_CLASS(CTScales, CTCtrl)







CTScales::CTScales(void)
{
	m_clBackGround	= wxColour(RGB(0,0,0));
	m_clBorder		= wxColour(RGB(192,0,0));
	m_cldashLine	= wxColour(RGB(76,40,40));

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

	m_RectOffSet =  wxRect(66,0,66,0);
	m_nBorderWidth	= 1;
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;

	m_bShowHeight = false;
	m_bShowTime   = true;
	m_ptMouseMoveLast = wxPoint(-1,-1);
	m_nLeftPanelHeight = 230;//左侧价格提示窗高度


	m_fLastClosePrice = 8.880;

	m_bShowCross	   = false;
	m_bMouseIn		   = false;
	
}
CTScales::~CTScales(void)
{
	
}
BEGIN_EVENT_TABLE(CTScales, CTCtrl)
EVT_PAINT(CTScales::OnPaint)
EVT_MOTION(CTScales::OnMouseMove)
EVT_MOTION(CTScales::OnMouseMove)
EVT_SIZE(CTScales::OnSize)
EVT_LEAVE_WINDOW(CTScales::OnMouseLeave)
EVT_ENTER_WINDOW(CTScales::OnMouseEnter)
EVT_LEFT_DCLICK(OnLeftDBClick)
EVT_RIGHT_UP(OnRightUp)
EVT_LEFT_DOWN(OnLeftDown)
END_EVENT_TABLE()

void CTScales::OnPaint (wxPaintEvent & PaintEvent)
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
void CTScales::DrawDC(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxBrush brush(m_clBackGround);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.x, size.y);

	wxRect rt(0, 0, size.x, size.y);
	DrawScales(dc);
	DrawBorder(dc, rt);
	if(m_bMouseIn && m_bShowCross)
		DrawCurSelLine(dc);
	if(m_bMouseIn)
		DrawTips(dc);
}
void CTScales::CalcMaxMin()
{
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;
	for(int i = m_nBegin; i< (int)m_vecTLine.size() && i>= m_nEnd  && i>=0; i--)
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
	double dbMaxSpan = abs(m_fMaxValue - m_fLastClosePrice);
	double dbMax2	 = abs(m_fMinValue - m_fLastClosePrice);
	if(dbMax2 > dbMaxSpan)
		dbMaxSpan = dbMax2;

	m_fMaxValue = m_fLastClosePrice + dbMaxSpan;
	m_fMinValue = m_fLastClosePrice - dbMaxSpan;

}
void CTScales::DrawBorder(wxDC& dc, wxRect& rt)
{
	wxPen penBorder(wxColour(m_clBorder), m_nBorderWidth, wxSOLID);
	dc.SetPen(penBorder);
	//dc.DrawLine(rt.GetLeft(), rt.GetTop(), rt.GetRight(), rt.GetTop());
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
void CTScales::DrawScales(wxDC& dc)
{
	if(m_vecTimeScales.size() == 0)
		return;

	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop()	+ m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight()	- m_RectOffSet.GetWidth()	- m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()	- m_RectOffSet.GetTop());

	struct tm tmBegin = m_vecTimeScales[0];
	struct tm tmEnd   = m_vecTimeScales[m_vecTimeScales.size() - 1];
	wxDateTime dateBegin(tmBegin);
	wxDateTime dateEnd(tmEnd);

	wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 	
	dc.SetTextForeground(m_clName); 

	wxString  strText;
	wxPoint pt;

	for(int i = 1; i< (int)m_vecTimeScales.size(); i = i + 2)//-1是不希望最后一根画竖线
	{  
		int nX = Time2X(m_vecTimeScales[i]);		
		struct tm tm1 = m_vecTimeScales[i];
		wxString  strText;
		wxCoord w,h;
		wxPoint pt;
		strText.Printf(_T("%.2d:%.2d"), tm1.tm_hour, tm1.tm_min);	
		dc.GetTextExtent(strText, &w, &h);	

		pt.x = nX -w/2;
		if(pt.x < rtClient.GetLeft() - m_RectOffSet.GetLeft())
			pt.x = rtClient.GetLeft() - m_RectOffSet.GetLeft();
		pt.y = rtClient.GetTop() + (rtClient.GetHeight() -h)/2 ; 
		dc.DrawText(strText, pt);

	}
	int nFirstEnd = 0;
	for(wxDateTime dt = dateBegin; dt<dateEnd; dt += wxTimeSpan(0, 15, 0, 0))
	{
		int nMin = dt.GetMinute();
		if(dt == dateBegin)
		{//开始
			struct tm tmDt;
			DateTime2Tm(dt, tmDt);
			int nX = Time2X(tmDt);
			wxString  strText;
			wxCoord w,h;
			wxPoint pt;
			strText.Printf(_T("%.4d/%.2d/%.2d,%.2d:%.2d"), dt.GetYear(), dt.GetMonth()+1, dt.GetDay(), dt.GetHour(), dt.GetMinute());	
			dc.GetTextExtent(strText, &w, &h);	

			pt.x = rtClient.GetLeft() -w/2;
			if(pt.x < rtClient.GetLeft() - m_RectOffSet.GetLeft())
				pt.x = rtClient.GetLeft() - m_RectOffSet.GetLeft();
			pt.y = rtClient.GetTop() + (rtClient.GetHeight() -h)/2 ; 
			dc.DrawText(strText, pt);
			
			nFirstEnd = pt.x +w;

		}
		else if(m_TMWidthCount >= 43200)//超过12小时的交易时间；执行不同画法
		{
			for(int i = 0; i <4; i++)
			{			
				int nSpan2 = m_TMWidthCount/4;
				struct tm tmOneSpan2 = GetDrawTime(m_vecTimeScales, m_vecTMWidthSpan, nSpan2+i*nSpan2);
				int nX = Time2X(tmOneSpan2);
				if(nX == -1)
					continue;
				wxString  strText;
				wxCoord w,h;
				wxPoint pt;
				strText.Printf(_T("%.2d:%.2d"), tmOneSpan2.tm_hour, tmOneSpan2.tm_min);	
				dc.GetTextExtent(strText, &w, &h);	

				pt.x = nX -w/2;
				if(pt.x < rtClient.GetLeft() - m_RectOffSet.GetLeft())
					pt.x = rtClient.GetLeft() - m_RectOffSet.GetLeft();
				pt.y = rtClient.GetTop() + (rtClient.GetHeight() -h)/2 ; 

				if(pt.x > nFirstEnd)
					dc.DrawText(strText, pt);
			}		
		}
		else if(nMin == 0 && dt != dateBegin)
		{//整点
			struct tm tmDt;
			DateTime2Tm(dt, tmDt);
			int nX = Time2X(tmDt);
			if(nX == -1)
				continue;
			wxString  strText;
			wxCoord w,h;
			wxPoint pt;
			strText.Printf(_T("%.2d:%.2d"), dt.GetHour(), dt.GetMinute());	
			dc.GetTextExtent(strText, &w, &h);	

			pt.x = nX -w/2;
			if(pt.x < rtClient.GetLeft() - m_RectOffSet.GetLeft())
				pt.x = rtClient.GetLeft() - m_RectOffSet.GetLeft();
			pt.y = rtClient.GetTop() + (rtClient.GetHeight() -h)/2 ; 

			if(pt.x > nFirstEnd)
			dc.DrawText(strText, pt);
		}
	}	
}
void CTScales::SetTextColor(wxDC& dc, double dbClosePrice)
{
	double dbPrevious = m_fLastClosePrice;

	if(dbClosePrice - dbPrevious > -0.00001 && dbClosePrice - dbPrevious < 0.00001)
		dc.SetTextForeground(m_hengpan); 
	else if(dbClosePrice > dbPrevious)
		dc.SetTextForeground(m_zhang); 
	else if(dbClosePrice < dbPrevious)
		dc.SetTextForeground(m_die); 
}
void CTScales::DrawCurSelLine(wxDC& dc)
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

}
void CTScales::DrawTips(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + 1);
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() -2);

	wxFont wxfont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 
	dc.SetFont(wxfont);
	wxPen penBorder(wxColour(m_clCurSelLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	if(m_ptMouseMoveLast.x >= rtClient.GetLeft()	&& m_ptMouseMoveLast.x <= rtClient.GetRight()+1)
	{		
		if(m_bShowTime)
		{
			DrawTips_Time(dc, rtClient, m_ptMouseMoveLast.x, m_bIsMouse);
		}
	}
}

void CTScales::DrawTips_Time(wxDC& dc, wxRect rtClient, int nX, bool bIsMouse)
{
	wxString strTextBottom;
	if(bIsMouse)
	{//主要是盘中停盘，没有数据
		struct tm dwTime = X2Time(nX);
		if(dwTime.tm_year == 0)
			return;
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
	w = w +4;

	int nLeft =  nX+1;
	if(nX + w > rtClient.GetRight())
		nLeft = nX -w-1;
	int nRectTop  = rtClient.GetBottom() - h;
	wxBrush brush(m_clTipsBk);
	dc.SetBrush(brush);
	wxPen penBorder(wxColour(m_zhang), 1, wxSOLID);
	dc.SetPen(penBorder);
	dc.DrawRectangle(nLeft, nRectTop, w, h);


	dc.SetTextForeground(m_clCurSelLine); 
	pt.x = nLeft + 2;
	pt.y = nRectTop; 
	dc.DrawText(strTextBottom, pt);

}
bool CTScales::InitCfg(TiXmlElement *root)
{
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
	}
	TiXmlElement *tList = root->FirstChildElement("tscales");
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

	

	



	ListItem = pColor->FirstChild("name");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetNameColor(value);


	

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

	
	ListItem = pColor->FirstChild("tipsbk");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetTipsBkColor(value);

	return true;
}
void CTScales::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CTScales::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CTScales::SetdashLine(unsigned long lValue)
{
	m_cldashLine = lValue;
}
void CTScales::SetPriceColor(unsigned long lValue)
{
	m_clPrice = lValue;
}
void CTScales::SetAveragePriceColor(unsigned long lValue)
{
	m_clAveragePrice = lValue;
}
void CTScales::SetCurSelLineColor(unsigned long lValue)
{
	m_clCurSelLine = lValue;
}
void CTScales::SetTipsBkColor(unsigned long lValue)
{
	m_clTipsBk = lValue;
}
void CTScales::SetNameColor(unsigned long lValue)
{
	m_clName = lValue;
}
void CTScales::SetTimeColor(unsigned long lValue)
{
	m_clTime = lValue;
}
void CTScales::SetzhangColor(unsigned long lValue)
{
	m_zhang = lValue;
}
void CTScales::SetdieColor(unsigned long lValue)
{
	m_die = lValue;
}
void CTScales::SethengpanColor(unsigned long lValue)
{
	m_hengpan = lValue;
}
void CTScales::SetLineNameColor(unsigned long lValue)
{
	m_clLineName = lValue;
}