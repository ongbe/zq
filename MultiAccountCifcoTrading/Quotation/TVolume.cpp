#include "stdafx.h"
#include "../inc/Quotation/TVolume.h"
#include <time.h>



IMPLEMENT_CLASS(CTVolume, CTCtrl)

CTVolume::CTVolume(void)
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

	m_RectOffSet =  wxRect(66,1,66,0);
	m_nBorderWidth	= 1;
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;

	m_bShowHeight = true;
	m_bShowTime   = false;
	m_ptMouseMoveLast = wxPoint(-1,-1);
	m_nLeftPanelHeight = 230;//左侧价格提示窗高度


	m_fLastClosePrice = 8.880;
	m_nOneDayScalesCount = m_vecTimeScales.size();
	
	m_nSpanLineType = 1;		
	m_nSpanLineSize = 2;		
	m_nHourLineType = 1;		
	m_nHourLineSize = 1;		
	m_nHalfHourLineLineType = 0;
	m_nHalfHourLineSize = 1;	
	
	m_bShowCross	   = false;
	m_bMouseIn		   = false;

	m_pFData = NULL;
	m_nNumDigits = 2;
}
CTVolume::~CTVolume(void)
{

}
BEGIN_EVENT_TABLE(CTVolume, CTCtrl)
EVT_PAINT(CTVolume::OnPaint)
EVT_MOTION(CTVolume::OnMouseMove)
EVT_MOTION(CTVolume::OnMouseMove)
EVT_SIZE(CTVolume::OnSize)
EVT_LEAVE_WINDOW(CTVolume::OnMouseLeave)
EVT_ENTER_WINDOW(CTVolume::OnMouseEnter)
EVT_LEFT_DCLICK(OnLeftDBClick)
EVT_RIGHT_UP(OnRightUp)
EVT_LEFT_DOWN(OnLeftDown)
END_EVENT_TABLE()

void CTVolume::OnPaint (wxPaintEvent & PaintEvent)
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
void CTVolume::DrawDC(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxBrush brush(m_clBackGround);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.x, size.y);

	wxRect rt(0, 0, size.x, size.y);
	DrawScales(dc);
	DrawBorder(dc, rt);
	DrawVolumeLine(dc);
	if(m_bMouseIn && m_bShowCross)
		DrawCurSelLine(dc);
	if(m_bMouseIn)
		DrawTips(dc);
	
}
void CTVolume::CalcMaxMin()
{
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = 0;
	for(int i = m_nBegin; i< (int)m_vecTLine.size() && i>= m_nEnd  && i>=0; i--)
	{
		STLine *pTLine = m_vecTLine[i];
		if(pTLine->dwVolume > m_fMaxValue)
		{
			m_fMaxValue = pTLine->dwVolume;
		}		
	}
	if(m_fMaxValue - MIN_VALUE_DEFAULT > -0.00001 && m_fMaxValue - MIN_VALUE_DEFAULT < 0.00001 && m_fMinValue  == 0)
	{
		m_fMaxValue = 1;		
	}	
}
void CTVolume::DrawBorder(wxDC& dc, wxRect& rt)
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
	dc.DrawLine(rtClient.GetRight(), rtClient.GetTop(), rtClient.GetRight(), rtClient.GetBottom());

}
void CTVolume::DrawScales(wxDC& dc)
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
	dc.SetTextForeground(m_clBorder); 
	for(int i = 1; i<= nDrawScalesCount; i++)
	{
		double dbPrice = i* m_fMaxValue /nDrawScalesCount;
		int nHeight = JiaGe2GaoDu(dbPrice);
		dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);

		//SetTextColor(dc, dbPrice);
		if(dbPrice <= 10 && dbPrice>0)
			strText = wxString::Format(wxT("%.2f"), dbPrice);
		else
			strText = wxString::Format(wxT("%.0f"), dbPrice);
		dc.GetTextExtent(strText, &w, &h);	
		pt.x = rtClient.GetLeft() - w-4;
		if(i == nDrawScalesCount)
			pt.y = nHeight; 
		else
			pt.y = nHeight - h/2; 
		dc.DrawText(strText, pt);
	}
}
void CTVolume::DrawVolumeLine(wxDC& dc)
{
	if(m_vecTLine.size() == 0
		|| m_nEnd < 0 || m_nEnd >= (int)m_vecTLine.size() 
		|| m_nBegin < 0 || m_nBegin >= (int)m_vecTLine.size() )
		return;
	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;
	rtClient.SetLeft(rtClient.GetLeft()     + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop()       + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight()   - m_RectOffSet.GetWidth()  - m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight() - m_RectOffSet.GetTop());

	wxPen penBorderN(wxColour(m_clAveragePrice), 1, wxSOLID);
	dc.SetPen(m_clAveragePrice);
	for( int i = m_nEnd; i<= m_nBegin; i++ )
	{
		STLine *pTLineEnd = m_vecTLine[i];
		int nXEnd = Time2X(pTLineEnd->dwTime);
		int nYEnd = JiaGe2GaoDu(pTLineEnd->dwVolume);
		dc.DrawLine(wxPoint(nXEnd, nYEnd), wxPoint(nXEnd, rtClient.GetBottom()));			
	}
}
void CTVolume::SetTextColor(wxDC& dc, double dbClosePrice)
{
	double dbPrevious = m_fLastClosePrice;

	if(dbClosePrice - dbPrevious > -0.00001 && dbClosePrice - dbPrevious < 0.00001)
		dc.SetTextForeground(m_hengpan); 
	else if(dbClosePrice > dbPrevious)
		dc.SetTextForeground(m_zhang); 
	else if(dbClosePrice < dbPrevious)
		dc.SetTextForeground(m_die); 
}
void CTVolume::DrawCurSelLine(wxDC& dc)
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
void CTVolume::DrawTips(wxDC& dc)
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
	if(m_ptMouseMoveLast.y >= rtClient.GetTop() && m_ptMouseMoveLast.y <= rtClient.GetBottom())
	{
		if(m_bShowHeight)
		{
			double dbPrice = 0.0;
			if(m_bIsKeyBoard)
			{
			//	if(m_nCurSel < m_nEnd || m_nCurSel >= m_vecTLine.size())
					return;
			//	STLine *pKLine = m_vecTLine[m_nCurSel];
			//	dbPrice = pKLine->fLastPrice;
			}
			if(m_bIsMouse)
				dbPrice = GaoDu2JiaGe(m_ptMouseMoveLast.y);

			wxString  strText;
			wxCoord w,h;
			wxPoint pt;
			int nTop = rtClient.GetTop() - 10;
		
			strText = Double2String_Tip(dbPrice);
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

void CTVolume::DrawTips_Time(wxDC& dc, wxRect rtClient, int nX, bool bIsMouse)
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
bool CTVolume::InitCfg(TiXmlElement *root)
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

	}
	TiXmlElement *tList = root->FirstChildElement("tvolume");
	if(tList == NULL)
		return false;
/*	TiXmlNode *leftspanwidth = tList->FirstChild("leftspanwidth");
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



	ListItem = pColor->FirstChild("averageprice");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetAveragePriceColor(value);



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
void CTVolume::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CTVolume::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CTVolume::SetdashLine(unsigned long lValue)
{
	m_cldashLine = lValue;
}
void CTVolume::SetPriceColor(unsigned long lValue)
{
	m_clPrice = lValue;
}
void CTVolume::SetAveragePriceColor(unsigned long lValue)
{
	m_clAveragePrice = lValue;
}
void CTVolume::SetCurSelLineColor(unsigned long lValue)
{
	m_clCurSelLine = lValue;
}
void CTVolume::SetTipsBkColor(unsigned long lValue)
{
	m_clTipsBk = lValue;
}
void CTVolume::SetNameColor(unsigned long lValue)
{
	m_clName = lValue;
}
void CTVolume::SetTimeColor(unsigned long lValue)
{
	m_clTime = lValue;
}
void CTVolume::SetzhangColor(unsigned long lValue)
{
	m_zhang = lValue;
}
void CTVolume::SetdieColor(unsigned long lValue)
{
	m_die = lValue;
}
void CTVolume::SethengpanColor(unsigned long lValue)
{
	m_hengpan = lValue;
}
void CTVolume::SetLineNameColor(unsigned long lValue)
{
	m_clLineName = lValue;
}
void CTVolume::SetSpanLine(int nLineType, int	nLineSize)
{
	m_nSpanLineType = nLineType;
	m_nSpanLineSize = nLineSize;
}
void CTVolume::SetHourLineLine(int nLineType, int	nLineSize)
{
	m_nHourLineType = nLineType;
	m_nHourLineSize = nLineSize;
}
void CTVolume::SetHalfHourLine(int nLineType, int	nLineSize)
{
	m_nHalfHourLineLineType = nLineType;
	m_nHalfHourLineSize = nLineSize;
}