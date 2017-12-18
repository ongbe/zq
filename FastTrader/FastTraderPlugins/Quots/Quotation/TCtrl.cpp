#include "stdafx.h"
#include "../../../inc/Quotation/TCtrl.h"
#include <time.h>



DEFINE_EVENT_TYPE(wxEVT_TCtrl_KeyDown)
DEFINE_EVENT_TYPE(wxEVT_TCtrl_LBUTTONDOWN)
DEFINE_EVENT_TYPE(wxEVT_TCtrl_MOUSEMOVE)
DEFINE_EVENT_TYPE(wxEVT_TCtrl_RBUTTONUP)
DEFINE_EVENT_TYPE(wxEVT_TCtrl_MOUSELEAVE)
DEFINE_EVENT_TYPE(wxEVT_TCtrl_LEFTDBCLICK)
DEFINE_EVENT_TYPE(wxEVT_TCtrl_MOUSEENTER)



IMPLEMENT_CLASS(CTCtrl, wxPanel)

CTCtrl::CTCtrl(void)
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
bool CTCtrl::Create(wxWindow *parent,
			wxWindowID winid ,
			const wxPoint& pos ,
			const wxSize& size,
			long style,
			const wxString& name )
{
	style |= wxWANTS_CHARS|wxTE_PROCESS_ENTER|wxCLIP_CHILDREN|wxCLIP_SIBLINGS|wxTAB_TRAVERSAL;
	bool bReturn = wxPanel::Create(parent,winid,pos, size,	wxTE_PROCESS_ENTER|style, name);		
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	return bReturn;
}
void CTCtrl::SetLastClosePrice(double dbPrice)
{
	m_fLastClosePrice = dbPrice;	
}
void CTCtrl::SetFData(CFData *pFData)
{
	m_pFData = pFData;	
	if(m_pFData)
		m_nNumDigits = m_pFData->GetDotDigits();
}
CTCtrl::~CTCtrl(void)
{
}
BEGIN_EVENT_TABLE(CTCtrl, wxPanel)
	EVT_PAINT(CTCtrl::OnPaint)
	EVT_MOTION(CTCtrl::OnMouseMove)
	EVT_MOTION(CTCtrl::OnMouseMove)
	EVT_SIZE(CTCtrl::OnSize)
	EVT_LEAVE_WINDOW(CTCtrl::OnMouseLeave)
	EVT_ENTER_WINDOW(CTCtrl::OnMouseEnter)
	EVT_LEFT_DCLICK(OnLeftDBClick)
	EVT_RIGHT_UP(CTCtrl::OnRightUp)
	EVT_LEFT_DOWN(CTCtrl::OnLeftDown)
END_EVENT_TABLE()

void CTCtrl::OnPaint (wxPaintEvent & PaintEvent)
{
/*	wxPaintDC dc(this);
	wxMemoryDC memdc;  
	memdc.SetLayoutDirection(dc.GetLayoutDirection());
	wxBitmap bmp;
	wxSize size = GetClientSize();
	bmp.Create(size.GetWidth(), size.GetHeight());
	memdc.SelectObject(bmp);

	DrawDC(memdc);
	dc.Blit(0, 0, size.GetWidth(),size.GetHeight(),&memdc, 0, 0);*/
}
void CTCtrl::DrawDC(wxDC& dc)
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
void CTCtrl::SetFuture(string ExchangeName,string ContractName,string ContractDate)
{
	m_ExchangeName = ExchangeName;
	m_ContractName = ContractName;
	m_ContractDate = ContractDate;	
}
BOOL CTCtrl::SetVectorData(vector<STLine *>& veCTCtrl)
{	
	m_vecTLine.clear();
	m_vecTLine = veCTCtrl;	
	GetBeginEnd();

	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;
	CalcMaxMin();
	return TRUE;
}
void CTCtrl::SetScales(vector<struct tm>& vecScales)
{
	if(vecScales.size() < 2)
		return;

	m_vecTimeScales.clear();
	m_vecTimeScales = vecScales;
	m_nOneDayScalesCount = m_vecTimeScales.size();

	m_vecTMWidthSpan.clear();
	m_TMWidthCount = 0;
	for(int i = 0; i< (int)m_vecTimeScales.size(); i++)
	{
		struct tm tm1 = m_vecTimeScales[i];
		struct tm tm2 = m_vecTimeScales[i+1];
		DWORD dw = abs(difftime(mktime(&tm1), mktime(&tm2)));
		m_vecTMWidthSpan.push_back(dw);
		m_TMWidthCount = m_TMWidthCount + dw;
		i++;
	}
}
void CTCtrl::GetBeginEnd()
{
	if(m_vecTimeScales.size() == 0)
		return;

	m_nEnd	 = -1;
	m_nBegin = -1;
	struct tm tmMax = m_vecTimeScales[m_vecTimeScales.size() -1];
	struct tm tmMin = m_vecTimeScales[0];
	
	bool bFindBegin = false, bFindEnd = false;
	for(int i = 0; i < (int)m_vecTLine.size(); i++)
	{
		STLine *pTLine = m_vecTLine[i];
		struct tm tmTime = pTLine->dwTime;
		
		if(!bFindEnd && (difftime(mktime(&tmMax), mktime(&tmTime)) >= 0) )
		{
			tmMax = m_vecTimeScales[m_vecTimeScales.size() -1];//mktime 会改变里面参数的值，所以用完后都得重新赋值
			tmTime = pTLine->dwTime;
			if((difftime(mktime(&tmMin), mktime(&tmTime)) <= 0))
			{
				tmMin = m_vecTimeScales[0];
				tmTime = pTLine->dwTime;
				m_nEnd = i;
				bFindEnd = true;
			}
		}
		if(difftime(mktime(&tmMin), mktime(&tmTime)) <= 0)
		{
			tmMin = m_vecTimeScales[0];
			tmTime = pTLine->dwTime;

			if((difftime(mktime(&tmMax), mktime(&tmTime)) >= 0))
			{
				tmMax = m_vecTimeScales[m_vecTimeScales.size() -1];
				tmTime = pTLine->dwTime;
				m_nBegin = i;				
			}
		}
	}
}
void CTCtrl::SetMaxMinValue(double& dbMaxValue, double& dbMinValue)
{
	if(IsDoubleSame(m_fMaxValue, MIN_VALUE_DEFAULT) && IsDoubleSame(m_fMinValue, MAX_VALUE_DEFAULT))
	{
		m_fMaxValue = m_fLastClosePrice *1.01;
		m_fMinValue = m_fLastClosePrice*0.99;	
		return;
	}
	
	m_fMaxValue = dbMaxValue;
	m_fMinValue = dbMinValue;	
	double dbMaxSpan = abs(m_fMaxValue - m_fLastClosePrice);
	double dbMax2	 = abs(m_fMinValue - m_fLastClosePrice);
	if(dbMax2 > dbMaxSpan)
		dbMaxSpan = dbMax2;

	m_fMaxValue = m_fLastClosePrice + dbMaxSpan;
	m_fMinValue = m_fLastClosePrice - dbMaxSpan;

	if(m_fMaxValue - m_fMinValue > -0.00001 && m_fMaxValue - m_fMinValue <0.00001)
	{
		m_fMaxValue = m_fMinValue *1.01;
		m_fMinValue = m_fMinValue*0.99;
	}
}
void CTCtrl::CalcMaxMin()
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
	if(abs(m_fMaxValue - MIN_VALUE_DEFAULT)<0.00001 && abs(m_fMinValue-MAX_VALUE_DEFAULT)<0.00001)
	{
		m_fMaxValue = m_fLastClosePrice *1.001;
		m_fMinValue = m_fLastClosePrice*0.999;

		if(abs(m_fMaxValue)<0.00001 && abs(m_fMaxValue)>-0.00001)
			m_fMaxValue = 1;//没有昨结算价很麻烦
	}
	else
	{
		double dbMaxSpan = abs(m_fMaxValue - m_fLastClosePrice);
		double dbMax2	 = abs(m_fMinValue - m_fLastClosePrice);
		if(dbMax2 > dbMaxSpan)
			dbMaxSpan = dbMax2;

		m_fMaxValue = m_fLastClosePrice + dbMaxSpan;
		m_fMinValue = m_fLastClosePrice - dbMaxSpan;

		if(m_fMaxValue - m_fMinValue > -0.00001 && m_fMaxValue - m_fMinValue <0.00001)
		{
			m_fMaxValue = m_fMinValue *1.01;
			m_fMinValue = m_fMinValue*0.99;
		}
	}

}
void CTCtrl::DrawBorder(wxDC& dc, wxRect& rt)
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
void CTCtrl::DateTime2Tm(wxDateTime dt ,struct tm& tmDt)
{
	tmDt.tm_year = dt.GetYear() -1900;
	tmDt.tm_mon  = dt.GetMonth();
	tmDt.tm_mday = dt.GetDay();
	tmDt.tm_hour = dt.GetHour();
	tmDt.tm_min  = dt.GetMinute();
	tmDt.tm_sec  = dt.GetSecond();
}
struct tm CTCtrl::GetDrawTime(vector<struct tm>& vecTimeScales, vector<DWORD>& vecTMWidthSpan, DWORD dwSpan)
{
	struct tm tmReturn;
	tmReturn.tm_year = 0;
	if(vecTimeScales.size() == 0)
		return tmReturn;

	int nIndex = 0;
	DWORD dwCount =0;
	for(int i= 0; i<(int)vecTMWidthSpan.size(); i++)
	{
		int n = dwSpan - vecTMWidthSpan[i];
		if(n < 0)
		{
			nIndex = i;
			dwCount = n + vecTMWidthSpan[i];
			break;
			
		}		
	}

	time_t t1 = mktime( &vecTimeScales[nIndex*2]);
	t1 =t1 + dwCount;	
	tmReturn = *localtime(&t1);
	return tmReturn;	
}
void CTCtrl::DrawScales(wxDC& dc)
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

	wxPen penBorder2(wxColour(m_clBorder), 2, wxSOLID);
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
		dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
		nHeight = rtClient.GetTop() + i*rtClient.GetHeight()/(nDrawScalesCount*2) ;
		dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
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
int	CTCtrl::GetDrawScalesCount(wxDC& dc)
{
	int nCount = 0;
	
	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop()	+ m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight()	- m_RectOffSet.GetWidth()	- m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()	- m_RectOffSet.GetTop());

	wxCoord w,h;	
	wxString strText = wxT("0");
	dc.GetTextExtent(strText, &w, &h);
	int nHeight = rtClient.GetHeight();

	nCount = nHeight/(h*2);

	if(nCount == 0)
		nCount =1;
	else if(nCount > 7)
		nCount = 7;

	
	return nCount;
}
void CTCtrl::DrawPriceLines(wxDC& dc)
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
bool CTCtrl::DrawPriceLines_NoData(wxDC& dc, STLine *&pTLineEnd1, STLine *&pTLineBegin1)//end时间近
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
void CTCtrl::DrawAveragePriceLines(wxDC& dc)
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
		dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXEnd, nYEnd));		

		pTLineBegin			=  pTLineEnd;
	}
}
int CTCtrl::Time2X(struct tm tmTime)
{
	if(m_vecTimeScales.size() == 0)
		return -1;
	tmTime.tm_sec = 0;

	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;
	rtClient.SetLeft(rtClient.GetLeft()     + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop()       + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight()   - m_RectOffSet.GetWidth()  - m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight() - m_RectOffSet.GetTop());
	
	struct tm tm1 = m_vecTimeScales[0];
	struct tm tm2 = tmTime;

	if(difftime(mktime(&tm1), mktime(&tm2)) > 0)
		return -1;

	tm1 = m_vecTimeScales[m_vecTimeScales.size() -1];
	tm2 = tmTime;
	if(difftime(mktime(&tm1), mktime(&tm2)) < 0 )
		return -1;//不在刻度范围内

	int nIndex = 0;
	for(int i= 0; i< (int)m_vecTimeScales.size(); i++)
	{
		struct tm tm1 = m_vecTimeScales[i];
		struct tm tm2 = tmTime;
		if(difftime(mktime(&tm1), mktime(&tm2)) >= 0) 
		{
			nIndex = i;
			break;
		}
	}

	if(nIndex!=0 && nIndex%2 == 0 && nIndex != m_vecTimeScales.size()-1)
		return -1;

	//DWORD nWidthCount = 0;
	DWORD nLeft = 0;
	for(int i=0; i< (int)m_vecTMWidthSpan.size(); i++)
	{
	//	nWidthCount += m_vecTMWidthSpan[i];
		if(i < nIndex/2)
			nLeft += m_vecTMWidthSpan[i];
	}
	int nx = 0;
	if(nIndex == 0)
		nx = rtClient.GetLeft();
	else
	{
		struct tm tm1 = m_vecTimeScales[nIndex-1];
		struct tm tm2 = tmTime;
		struct tm tm3 = m_vecTimeScales[nIndex];
		struct tm tm4 = m_vecTimeScales[nIndex-1];
		double dbPercent = abs(difftime(mktime(&tm1), mktime(&tm2)) /difftime(mktime(&tm3), mktime(&tm4)));
		nx = rtClient.GetLeft() + (nLeft + dbPercent * m_vecTMWidthSpan[nIndex/2])/m_TMWidthCount * rtClient.GetWidth();	
	}

/*//以下为区间等长模式
	int nQuDuan = m_vecTimeScales.size()/2;		//总共分为几个区段
	int nWidth  = rtClient.GetWidth()/nQuDuan;	//每一个区段的宽度
	
	int nx = 0;
	if(nIndex == 0)
		nx = rtClient.GetLeft();
	else
	{
		double dbPercent = abs(difftime(mktime(&m_vecTimeScales[nIndex-1]), mktime(&tmTime)) /difftime(mktime(&m_vecTimeScales[nIndex]), mktime(&m_vecTimeScales[nIndex-1])));
		
		nx = rtClient.GetLeft() + nIndex/2*nWidth + dbPercent * nWidth;
	}
	*/
	return nx;
}
struct tm CTCtrl::X2Time(int nx)
{
	struct tm tTime;
	tTime.tm_year = 0;
	
	if( m_vecTimeScales.size() == 0)
		return tTime;

	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;
	rtClient.SetLeft(rtClient.GetLeft()     + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop()       + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight()   - m_RectOffSet.GetWidth()  - m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight() - m_RectOffSet.GetTop());
	
	if(nx < rtClient.GetLeft() || nx > rtClient.GetRight()+1)
		return tTime;

	double dbPercent = (nx- rtClient.GetLeft())*1.0/rtClient.GetWidth();
	double dbPercentWidth = 0.0;
	DWORD  dwWithCount = 0;
	int nIndex = 0;
	for(int i=0; i< (int)m_vecTMWidthSpan.size(); i++)
	{		
		dwWithCount += m_vecTMWidthSpan[i];
		if(dwWithCount*1.0/ m_TMWidthCount >= dbPercent)
		{
			nIndex = i;//在第几个时间区间之内
			dbPercentWidth = dbPercent - (dwWithCount - m_vecTMWidthSpan[i])*1.0/m_TMWidthCount;//在最后一个区间之内的百分比（按时间均匀刻度,）
			
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	struct tm tm1 = m_vecTimeScales[nIndex*2];
	struct tm tm2 = m_vecTimeScales[nIndex*2 + 1];
	time_t t1 =  mktime(&tm1);
	time_t t2 =  mktime(&tm2);//mktime可能返回-1  timecube.tm_year=y-1900; timecube.tm_mon=m-1; 

	dbPercentWidth = dbPercentWidth * m_TMWidthCount/(t2-t1);//相对于整个区间的百分比转化为区间内的百分比
	time_t t3 = t1+ dbPercentWidth *(t2 - t1);  
	struct tm tReturn = *localtime(&t3);
	tReturn.tm_sec = 0;

	int nIndexCount = m_nOneDayScalesCount/2;
	if(nIndex%nIndexCount != 0)
	{
		struct tm tm1 = m_vecTimeScales[nIndex*2];
		struct tm tm2 = tReturn;
		if(difftime(mktime(&tm1), mktime(&tm2)) == 0)
		{//13:00点显示为11:30，参考大智慧
			return m_vecTimeScales[nIndex*2 - 1];
		}
	}
	return tReturn;	


	//////////////////////////////////////////////////////////////////////////
//以下为区间等长模式
/*	int nQuDuan = m_vecTimeScales.size()/2;				//总共分为几个区段
	int nWidth  = rtClient.GetWidth()/nQuDuan;			//每一个区段的宽度
	int nIndex  = (nx - rtClient.GetLeft())/nWidth;		//位于第几个区段
	double dbPercent = (nx - rtClient.GetLeft() - nIndex*nWidth)/(double)nWidth;//百分比
	
	time_t t1 =  mktime(&m_vecTimeScales[nIndex*2]);
	time_t t2 =  mktime(&m_vecTimeScales[nIndex*2 + 1]);//mktime可能返回-1  timecube.tm_year=y-1900; timecube.tm_mon=m-1; 

	time_t t3 = t1+ dbPercent *(t2 - t1);  
	struct tm tReturn = *localtime(&t3);
	tReturn.tm_sec = 0;

	int nIndexCount = m_nOneDayScalesCount/2;
	if(nIndex%nIndexCount != 0)
	{
		if(difftime(mktime(&m_vecTimeScales[nIndex*2]), mktime(&tReturn)) == 0)
		{//13:00点显示为11:30，参考大智慧
			return m_vecTimeScales[nIndex*2 - 1];
		}
	}
	return tReturn;	*/
}
int  CTCtrl::WeiZhi2XuHao(int nx)
{
	if(m_vecTLine.size() == 0)
		return -1;
	struct tm tmSel = X2Time(nx);
	if(tmSel.tm_year == 0)
		return -1;
	for(int i = m_nEnd; i<= m_nBegin && i < (int)m_vecTLine.size() && i>=0; i++)
	{
		STLine *pTLine = m_vecTLine[i];
		pTLine->dwTime.tm_sec = 0;
		struct tm tm1 = pTLine->dwTime;
		struct tm tm2 = tmSel;
		if(difftime(mktime(&tm1), mktime(&tm2)) == 0)
		{
			return i;			
		}
	}	
	return -1;
}
int  CTCtrl::XuHao2WeiZhi(int nItem)
{
	if(nItem > m_nBegin || nItem < m_nEnd)
		return -1;
	STLine *pTLine = m_vecTLine[nItem];
	int nLeft = Time2X(pTLine->dwTime);
	return nLeft;
}
int	CTCtrl::JiaGe2GaoDu(float fJiaGe)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight() - m_RectOffSet.GetTop());
	
	
	float ftemp = (fJiaGe - m_fMinValue)*rtClient.GetHeight()/(m_fMaxValue - m_fMinValue)  ;
	int nn = rtClient.GetBottom();
	float fWeiZhi =rtClient.GetBottom() - (fJiaGe - m_fMinValue)*rtClient.GetHeight()/(m_fMaxValue - m_fMinValue)  ;
	int nWeiZhi = (int)fWeiZhi;
	if(fWeiZhi - nWeiZhi >0.5)
		nWeiZhi += 1;

	return nWeiZhi;

}
float CTCtrl::GaoDu2JiaGe(int nHeight)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight() - m_RectOffSet.GetTop());

	float fJiaGe = (m_fMaxValue - m_fMinValue)*(rtClient.GetBottom() - nHeight )/rtClient.GetHeight() + m_fMinValue;
	return fJiaGe;
}
void CTCtrl::SetTextColor(wxDC& dc, double dbClosePrice)
{
	double dbPrevious = m_fLastClosePrice;
	
	if(dbClosePrice - dbPrevious > -0.00001 && dbClosePrice - dbPrevious < 0.00001)
		dc.SetTextForeground(m_hengpan); 
	else if(dbClosePrice > dbPrevious)
		dc.SetTextForeground(m_zhang); 
	else if(dbClosePrice < dbPrevious)
		dc.SetTextForeground(m_die); 
}
void CTCtrl::DrawCurSelLine(wxDC& dc)
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
void CTCtrl::DrawTips(wxDC& dc)
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

void CTCtrl::DrawTips_Time(wxDC& dc, wxRect rtClient, int nX, bool bIsMouse)
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
void CTCtrl::OnMouseMove(wxMouseEvent &event)
{
	m_bIsMouse		= true;
	m_bIsKeyBoard	= false;

	m_ptMouseMoveLast = event.GetPosition();
	int nCursel= WeiZhi2XuHao(m_ptMouseMoveLast.x);
	if(nCursel < m_nEnd || nCursel > m_nBegin)
		return;

	m_nCurSel = nCursel;
	//Refresh(false);
	event.Skip();	
}

void CTCtrl::OnSize(wxSizeEvent& event)
{	
	GetBeginEnd();
	CalcMaxMin();
	Refresh(false);
}
void CTCtrl::DrawOneItemData(wxDC& dc,  wxRect rtClient)
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
	dc.DrawRectangle(1, 1, rtClient.GetWidth()-2, nHeight);

	rtClient.SetLeft(rtClient.GetLeft() -1);
	rtClient.SetRight(rtClient.GetRight() -2);

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
void CTCtrl::DrawHead(wxDC& dc)
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
void CTCtrl::OnMouseLeave(wxMouseEvent &event)
{
	DoMouseLeave();
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_TCtrl_MOUSELEAVE);
	myEvent.SetClientData(this);
	window->ProcessEvent(myEvent);
	event.Skip();
}
void CTCtrl::DoMouseLeave()
{
	m_ptMouseMoveLast = wxPoint(-1,-1);
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);
}
void CTCtrl::OnMouseEnter(wxMouseEvent &event)
{
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_TCtrl_MOUSEENTER);
	myEvent.SetClientData(this);
	window->ProcessEvent(myEvent);
}
void CTCtrl::DoMouseMove(wxPoint mousePos)
{
	mousePos = ScreenToClient(mousePos);
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + 1);
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() -2);

	m_ptMouseMoveLast = mousePos;

	if(mousePos.x >= rtClient.GetLeft()	&& mousePos.x <= rtClient.GetRight())
	{
		m_bIsMouse		 = true;
		m_bIsKeyBoard	 = false;
		int nCursel= WeiZhi2XuHao(m_ptMouseMoveLast.x);
		if(nCursel >= m_nEnd && nCursel <= m_nBegin)
		{
			m_nCurSel = nCursel;
		}
		
	}

	Refresh(false);
	UpdateWindow((HWND)m_hWnd);
}
void CTCtrl::OnChar(int nkeyCode)
{
	switch ( nkeyCode )
	{
	case VK_RETURN:
		{
		}
		break;
	case VK_ESCAPE:
		{
		}
		break;
	case VK_UP:
		{				
		}
		break;
	case VK_DOWN:
		{			
		}
		break;
	case VK_LEFT:
		{
			if(m_vecTLine.size() ==0 )
				return;
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;

			if(m_nCurSel== m_vecTLine.size()-1)
			{
				int nLeft =0;
				nLeft = XuHao2WeiZhi(m_nCurSel);			
				if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
				{
					STLine *pKLine = m_vecTLine[m_nCurSel];
					float fClosePrice = pKLine->fLastPrice;
					int nHeight = JiaGe2GaoDu(fClosePrice);				
					m_ptMouseMoveLast = wxPoint(nLeft, nHeight);
				}
				SItemGetValue(false, true);
				wxWindow* window= GetParent();
				wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
				myEvent.SetInt(nkeyCode);
				myEvent.SetClientData((void*)&m_sItem);
				window->ProcessEvent(myEvent);
				return ;
			}
			m_nCurSel  += 1;
			if(m_nCurSel > (int)m_vecTLine.size()-1)
				m_nCurSel = m_vecTLine.size()-1;
			if(m_nCurSel > m_nBegin)
			{
				m_nCurSel = m_nBegin;					
			}
			else if(m_nCurSel < m_nEnd)
			{
				m_nCurSel = m_nEnd;
			}
			m_bShowCross = true;
			Refresh(false);
			int nLeft =0, nRight =0;
			nLeft = XuHao2WeiZhi(m_nCurSel);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
			{
				STLine *pTLine = m_vecTLine[m_nCurSel];
				float fClosePrice = pTLine->fLastPrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = wxPoint(nLeft, nHeight);
			}
			SItemGetValue(false, true);
			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);
			return;
		}
		break;
	case VK_RIGHT:
		{
			if(m_vecTLine.size() ==0 )
				return;
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;	
			if(m_nCurSel==0)
			{
				int nLeft =0;
				nLeft = XuHao2WeiZhi(m_nCurSel);			
				if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
				{
					STLine *pKLine = m_vecTLine[m_nCurSel];
					float fClosePrice = pKLine->fLastPrice;
					int nHeight = JiaGe2GaoDu(fClosePrice);				
					m_ptMouseMoveLast = wxPoint(nLeft, nHeight);
				}
				SItemGetValue(false, true);
				wxWindow* window= GetParent();
				wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
				myEvent.SetInt(nkeyCode);
				myEvent.SetClientData((void*)&m_sItem);
				window->ProcessEvent(myEvent);
				return;
			}
			m_nCurSel  -= 1;
			if(m_nCurSel < 0)
				m_nCurSel = 0;
			if(m_nCurSel < m_nEnd)
			{
				m_nCurSel = m_nEnd;
			}
			else if(m_nCurSel > m_nBegin)
			{
				m_nCurSel = m_nBegin;
			}
			if(m_vecTLine.size() == 0)
				return ;
			if(m_nCurSel == -1)
				return;

			//CalcMaxMin();
			m_bShowCross = true;
			Refresh(false);

			int nLeft =0;
			nLeft = XuHao2WeiZhi(m_nCurSel);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
			{
				STLine *pKLine = m_vecTLine[m_nCurSel];
				float fClosePrice = pKLine->fLastPrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = wxPoint(nLeft, nHeight);
			}
			SItemGetValue(false, true);
			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);
			return;
		}
		break;
	case VK_PRIOR://上一页
		{
		}
		break;
	case VK_NEXT://下一页
		{
		}
		break;
	case VK_DELETE:
		{			
		}
		break;
	default:
		break;
	}

	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
	myEvent.SetInt(nkeyCode);
	window->ProcessEvent(myEvent);
}
void   CTCtrl::SItemGetValue(bool	bIsMouse, bool bIsKeyBoard)
{
	m_sItem.nItem		= m_nCurSel;
	if(m_nCurSel >= 0 && m_nCurSel < (int)m_vecTLine.size())
	{
	}
	else
		m_sItem.pItem  = NULL; 

	m_sItem.pMsg = NULL;
	m_sItem.bIsMouse		= bIsMouse;
	m_sItem.bIsKeyBoard	= bIsKeyBoard;
	m_sItem.wxPt		= m_ptMouseMoveLast;

}
void CTCtrl::SetCursel(STItem& sItem)
{
	if(sItem.nItem >=0 && sItem.nItem < (int)m_vecTLine.size())
		m_nCurSel = sItem.nItem;

	m_bIsMouse		= sItem.bIsMouse;
	m_bIsKeyBoard	= sItem.bIsKeyBoard;
	//	CalcMaxMin();		
	m_ptMouseMoveLast.x = sItem.wxPt.x;
	m_ptMouseMoveLast.y = -1;
	
	Refresh(false);

}
BOOL CTCtrl::SetVectorData_InTime(vector<STLine *>& veCTCtrl)
{
	if(veCTCtrl.size() == 0)
		return TRUE;

	m_vecTLine = veCTCtrl;
	GetBeginEnd();
	CalcMaxMin();
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);
	return TRUE;
}
void CTCtrl::OnLeftDBClick(wxMouseEvent& event)
{
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_TCtrl_LEFTDBCLICK);
	myEvent.SetClientData(this);
	window->ProcessEvent(myEvent);
}
WXLRESULT CTCtrl::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{//wxpanel不支持return escape，所以在这里处理onchar	
	switch (message) 
	{ 
	case WM_KEYDOWN: 			
		{
			OnChar((int)wParam);
		}
		break;
	case WM_SYSKEYDOWN: 			
		{
			OnChar((int)wParam);
		}
		break;
	default:
		break;
	} 
	return wxPanel::MSWWindowProc(message,wParam,lParam); 
}
void CTCtrl::SetVolumeMultiple(double dbVolumeMultiple)
{
	m_dbVolumeMultiple = dbVolumeMultiple;
}
bool CTCtrl::InitCfg(TiXmlElement *root)
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

	ListItem = pColor->FirstChild("dashline");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetdashLine(value);

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
void CTCtrl::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CTCtrl::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CTCtrl::SetdashLine(unsigned long lValue)
{
	m_cldashLine = lValue;
}
void CTCtrl::SetPriceColor(unsigned long lValue)
{
	m_clPrice = lValue;
}
void CTCtrl::SetAveragePriceColor(unsigned long lValue)
{
	m_clAveragePrice = lValue;
}
void CTCtrl::SetCurSelLineColor(unsigned long lValue)
{
	m_clCurSelLine = lValue;
}
void CTCtrl::SetTipsBkColor(unsigned long lValue)
{
	m_clTipsBk = lValue;
}
void CTCtrl::SetNameColor(unsigned long lValue)
{
	m_clName = lValue;
}
void CTCtrl::SetTimeColor(unsigned long lValue)
{
	m_clTime = lValue;
}
void CTCtrl::SetzhangColor(unsigned long lValue)
{
	m_zhang = lValue;
}
void CTCtrl::SetdieColor(unsigned long lValue)
{
	m_die = lValue;
}
void CTCtrl::SethengpanColor(unsigned long lValue)
{
	m_hengpan = lValue;
}
void CTCtrl::SetLineNameColor(unsigned long lValue)
{
	m_clLineName = lValue;
}
void CTCtrl::SetSpanLine(int nLineType, int	nLineSize)
{
	m_nSpanLineType = nLineType;
	m_nSpanLineSize = nLineSize;
}
void CTCtrl::SetHourLineLine(int nLineType, int	nLineSize)
{
	m_nHourLineType = nLineType;
	m_nHourLineSize = nLineSize;
}
void CTCtrl::SetHalfHourLine(int nLineType, int	nLineSize)
{
	m_nHalfHourLineLineType = nLineType;
	m_nHalfHourLineSize = nLineSize;
}
void CTCtrl::SetShowCross(bool bShowCross)
{
	m_bShowCross = bShowCross;
}
void CTCtrl::SetMouseIn(bool bMouseIn)
{
	m_bMouseIn = bMouseIn;
}
void CTCtrl::OnRightUp(wxMouseEvent& event)
{
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_TCtrl_RBUTTONUP);
	
	window->ProcessEvent(myEvent);
}
void CTCtrl::OnLeftDown(wxMouseEvent& event)
{
	SetFocus();

	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_TCtrl_LBUTTONDOWN);
	myEvent.SetExtraLong(reinterpret_cast<long>(this));
	window->ProcessEvent(myEvent);
	Refresh(false);	

}
void CTCtrl::SetLeftSpanWidth(int nWidth)
{
	m_RectOffSet.SetLeft(nWidth);
}