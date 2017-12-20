#include "stdafx.h"
#include "../inc/Quotation/KVolume.h"
#include <time.h>
#include "../inc/Quotation/TechIndexConfigDlg.h"

IMPLEMENT_CLASS(CKVolume, CKCtrl)

CKVolume::CKVolume(void)//:m_timer1(this, 1111),m_timer2(this, 1112)
{
	m_clBackGround	= wxColour(RGB(0,0,0));
	m_clBorder		= wxColour(RGB(192,0,0));

	m_clRedLine		= wxColour(RGB(192,106,105));
	m_clGreenLine	= wxColour(RGB(80,255,255));
	m_cldashLine	= wxColour(RGB(76,40,40));
	m_clCurSelLine  = wxColour(RGB(200,200,200));

	m_clTipsBk		= wxColour(RGB(0,0,128));
	m_clLineName    = wxColour(RGB(192,192,192));
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


	m_enumMark = MARK_UNDEFINE;

	m_bShowHeight = true;
	m_bShowTime  = false;
	
	m_ptMouseMoveLast = wxPoint(-1,-1);
    
	m_clTrangleUp	= wxColour(255, 0, 0);
	m_clTrangleDown = wxColour(0, 128, 0);
	m_clTrangleUpBorder = wxColour(255, 255, 255);
	m_clTrangleDownBorder = wxColour(255, 255, 255);


	m_pTechIndex = NULL;
	m_strTechIndexName = wxT(VOL);

	m_clLineColor[0] = RGB(200, 200, 200);//白
	m_clLineColor[1] = RGB(255, 255, 255);//白	
	m_clLineColor[2] = RGB(74, 203, 76);  //绿
	m_clLineColor[3] = RGB(181, 52, 179); //粉
	m_clLineColor[4] = RGB(241, 240, 113);//黄
	m_clLineColor[5] = RGB(124, 192, 255);//蓝
	m_clLineColor[6] = RGB(100, 100, 100);//大红
	m_clLineColor[7] = RGB(194, 106, 105);//

	m_bShowCross	   = false;
	m_bMouseIn		   = false;

	m_KType = KTYPE_VOLUME;
}
CKVolume::~CKVolume(void)
{
	m_vecKLine.clear();
}
BEGIN_EVENT_TABLE(CKVolume, CKCtrl)
	EVT_PAINT(CKVolume::OnPaint)
	//EVT_LEFT_DOWN(OnLeftDown)
	//EVT_LEFT_DCLICK(OnLeftDBClick)
	//EVT_LEFT_UP(OnLeftUp)
	//EVT_RIGHT_DOWN(OnRightDown)
	//EVT_RIGHT_UP(OnRightUp)
END_EVENT_TABLE()

void CKVolume::CalcMaxMin()
{
	if(!m_bMaxMinChange)
		return;//主要优化速度
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue = 0;
	for(int i = m_nBegin; i< (int)m_vecKLine.size() && i>= m_nEnd  && i>=0; i--)
	{
		SKLine *pKLine = m_vecKLine[i];
		if(pKLine->dwVolume> m_fMaxValue)
		{
			m_fMaxValue = pKLine->dwVolume;
			m_nMaxPos = i;
		}
		if(pKLine->dwVolume < m_fMinValue)
		{
			m_nMinPos = i;
		}
	}
	if(IsDoubleSame(m_fMaxValue, MIN_VALUE_DEFAULT))
	{
		m_fMaxValue = 10.0;				
	}
}
void CKVolume::OnPaint (wxPaintEvent & PaintEvent)
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
void CKVolume::DrawDC(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxBrush brush(m_clBackGround);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.x, size.y);
	
	wxRect rt(0, 0, size.x, size.y);
	DrawScales(dc);
	DrawBorder(dc, rt);
	DrawHead(dc);
	
	DrawItems(dc);
	DrawTechIndex(dc);
	if(m_bMouseIn && m_bShowCross)
		DrawCurSelLine(dc);
	if(m_bMouseIn)
		DrawTips(dc);

	DrawMarks(dc);
	
}
void CKVolume::DrawMarks(wxDC& dc)
{
	MAPsmark::iterator it = m_mapMarks.begin();
	while(it != m_mapMarks.end())
	{
		MAPsmark::iterator itTemp = it;
		it++;

		EnumMark enumMark	= (EnumMark)itTemp->first;
		SMark	 *pTemp		= itTemp->second;		
		for(vector<SPoint *>::iterator it = pTemp->vecPt.begin(); it != pTemp->vecPt.end(); it++)
		{
			SPoint *p = *it;	
			if(p->enumType != m_KType)
				continue;
			if(enumMark == MARK_TRIANGLE_UP)
				DrawTriangleUp(dc, p->dwTime, p->dwPrice, p->bSelected);
			else if(enumMark == MARK_TRIANGLE_DOWN)
				DrawTriangleDown(dc, p->dwTime, p->dwPrice, p->bSelected);
		}
	}
}

void CKVolume::DrawHead(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());

	wxFont wxfont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	dc.SetFont(wxfont);
	dc.SetTextForeground(m_clBorder); 

	wxString  strText;
	wxCoord w,h;
	wxPoint pt;

	strText = wxT(VOL);
	//strText.Printf(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());

	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + 5;
	pt.y = rtClient.GetTop() -h; 
	//dc.DrawText(strText, pt);


	if(m_pTechIndex == NULL)
		return;

	if(m_pTechIndex->m_nVecDataCount == 0 )
		return;

	int nLeft = pt.x;
	wxString strTechTitle;
	strTechTitle.Printf(wxT("%s("), m_pTechIndex->m_strTechName);
	for(int nConfig =0; nConfig < (int)m_pTechIndex->m_vecConfig.size(); nConfig++)
	{
		wxString strTemp;
		if(nConfig == 0)
			strTemp.Printf(wxT("%d"), m_pTechIndex->m_vecConfig[nConfig]);
		else
			strTemp.Printf(wxT(", %d"), m_pTechIndex->m_vecConfig[nConfig]);
		strTechTitle.Append(strTemp);
	}
	strTechTitle.Append(wxT(")"));
	dc.SetTextForeground(m_clLineColor[0]); 
	wxCoord wConfig,hConfig;
	dc.GetTextExtent(strTechTitle, &wConfig, &hConfig);	
	pt.x = nLeft;
	pt.y = rtClient.GetTop() -h; 
	dc.DrawText(strTechTitle, pt);
	nLeft = pt.x + wConfig + 5;			


	for(int k =0; k < m_pTechIndex->m_nVecDataCount; k++)
	{
		if(m_pTechIndex->m_vecData == NULL)
			return;
		if( m_pTechIndex->m_vecData[k].size() == 0)
			continue;
		dc.SetTextForeground(m_clLineColor[k+1]); 
		wxPen penBorder(wxColour(m_clLineColor[k+1]), m_nBorderWidth, wxSOLID);
		dc.SetPen(penBorder);
		wxString str;
		wxString strFormat;
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;		
		if(m_nCurSel >= 0 && m_nCurSel < (int)m_pTechIndex->m_vecData[k].size())
			sValue = m_pTechIndex->m_vecData[k][m_nCurSel];
		if(sValue.fValue - MIN_VALUE_DEFAULT > -0.00001 && sValue.fValue - MIN_VALUE_DEFAULT < 0.00001)	
		{
			if(m_pTechIndex->m_vecConfigText.size() > k)
				str.Printf(wxT("%s: "), m_pTechIndex->m_vecConfigText[k].c_str());
		}
		else
		{
			strFormat.Printf(wxT("%%s: %%.%df"), m_nNumDigits);	
			str.Printf(strFormat, sValue.strName.c_str(), sValue.fValue);
		}

		wxCoord w3,h3;
		dc.GetTextExtent(str, &w3, &h3);	
		pt.x = nLeft;
		pt.y = rtClient.GetTop() -h3; 
		dc.DrawText(str, pt);
		nLeft = pt.x + w3 + 5;		
	}
}

void CKVolume::DrawBorder(wxDC& dc, wxRect& rt)
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
void  CKVolume::DrawItems(wxDC& dc)
{
	if(m_vecKLine.size() == 0)
		return;

	for(int i = m_nBegin; i>= m_nEnd; i--)
	{
		SKLine * pKLine = m_vecKLine[i];
		wxASSERT(pKLine != NULL);

		int nLeft =0, nRight =0;
		XuHao2WeiZhi(i, nLeft, nRight);
		int top1 = JiaGe2GaoDu(pKLine->dwVolume);
		
		if(pKLine->fOpenPrice > pKLine->fClosePrice)
			DrawOneItem(dc, nLeft, nRight, top1,FALSE);
		else
			DrawOneItem(dc, nLeft, nRight, top1,TRUE);		
	}
}
void CKVolume::DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, bool IsZhang)
{//int nTop1 最高, int nTop2 开盘, int nTop3 收盘, int nTop4 最低

	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());
	if(IsZhang)
	{
		wxPen penBorder(wxColour(m_clRedLine), 1, wxSOLID);
		dc.SetPen(penBorder);
	
		dc.DrawLine(nLeft, rtClient.GetBottom(),nLeft, nTop1);
		dc.DrawLine(nLeft, nTop1,nRight, nTop1);
		dc.DrawLine(nRight, nTop1,nRight, rtClient.GetBottom());
		dc.DrawLine(nRight, rtClient.GetBottom(),nLeft, rtClient.GetBottom());
		
	}
	else
	{
		wxPen penBorder(wxColour(m_clGreenLine), 1, wxSOLID);
		dc.SetPen(penBorder);
		wxBrush brush(m_clGreenLine);
		dc.SetBrush(brush);
		dc.DrawRectangle(nLeft, nTop1, nRight - nLeft,rtClient.GetBottom()- nTop1);	
	}
}
void CKVolume::DrawScales(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;

	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());


	wxPen penBorder(wxColour(m_cldashLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	dc.DrawLine(rtClient.GetLeft(), rtClient.GetBottom() - rtClient.GetHeight()/3, rtClient.GetRight(), rtClient.GetBottom() - rtClient.GetHeight()/3);
	dc.DrawLine(rtClient.GetLeft(), rtClient.GetBottom() - 2*rtClient.GetHeight()/3, rtClient.GetRight(), rtClient.GetBottom() - 2*rtClient.GetHeight()/3);
	dc.DrawLine(rtClient.GetLeft(), rtClient.GetTop(), rtClient.GetRight(), rtClient.GetTop());
	dc.DrawLine(rtClient.GetLeft(), rtClient.GetBottom(), rtClient.GetRight(), rtClient.GetBottom());


	wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 
	dc.SetTextForeground(m_clBorder); 

	if(m_fMaxValue  == MIN_VALUE_DEFAULT ||	m_fMinValue == MAX_VALUE_DEFAULT)
		return;

	wxString  strText;
	wxCoord w,h;
	wxPoint pt;

	int nTop = rtClient.GetTop() -10;

	double dbPrice = m_fMaxValue;
	if(dbPrice <= 10 && dbPrice>0)
		strText = wxString::Format(wxT("%.2f"), dbPrice);
	else
		strText = wxString::Format(wxT("%.0f"), dbPrice);	
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);


	nTop = rtClient.GetTop() -10 + rtClient.GetHeight()/3;

	dbPrice = m_fMaxValue - (m_fMaxValue- m_fMinValue)/3;
	if(dbPrice <= 10 && dbPrice>0)
		strText = wxString::Format(wxT("%.2f"), dbPrice);
	else
		strText = wxString::Format(wxT("%.0f"), dbPrice);	

	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);

	nTop = rtClient.GetTop() -10 + 2*rtClient.GetHeight()/3;

	dbPrice =  m_fMaxValue - 2*(m_fMaxValue- m_fMinValue)/3;
	if(dbPrice <= 10 && dbPrice>0)
		strText = wxString::Format(wxT("%.2f"), dbPrice);
	else
		strText = wxString::Format(wxT("%.0f"), dbPrice);	

//	strText = wxString::Format(wxT("%.0f"), m_fMaxValue - 2*(m_fMaxValue- m_fMinValue)/3);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);


	nTop = rtClient.GetBottom() -15 ;
	dbPrice =  m_fMinValue;
	if(dbPrice <= 10 && dbPrice>0)
		strText = wxString::Format(wxT("%.2f"), dbPrice);
	else
		strText = wxString::Format(wxT("%.0f"), dbPrice);
//	strText = wxString::Format(wxT("%.0f"), m_fMinValue );
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);
}
void CKVolume::DrawZuiGaoZuiDiJia(wxDC& dc)
{
}

void CKVolume::DrawCurSelLine(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + 1);
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() -2);
//	dc.SetLogicalFunction(wxXOR);
	wxPen penBorder(wxColour(m_clCurSelLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	if(m_ptMouseMoveLast.x >= rtClient.GetLeft()	&& m_ptMouseMoveLast.x <= rtClient.GetRight())
	{		
		dc.DrawLine(m_ptMouseMoveLast.x, rtClient.GetTop(), m_ptMouseMoveLast.x, rtClient.GetBottom());
	}
	if(m_ptMouseMoveLast.y >= rtClient.GetTop() && m_ptMouseMoveLast.y <= rtClient.GetBottom())
		dc.DrawLine(rtClient.GetLeft(), m_ptMouseMoveLast.y, rtClient.GetRight(), m_ptMouseMoveLast.y);	
	
}
void CKVolume::DrawTips(wxDC& dc)
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

			
			strText = Double2String_Tip(dbPrice);

			wxString strLength = wxT("1234.56");
			dc.GetTextExtent(strLength, &w, &h);	
			w = w + 4;
			int nLeft =  rtClient.GetRight() - w;
			int nRectTop  = m_ptMouseMoveLast.y - h;
			if(nRectTop <= 0)
				nRectTop = 0;
			wxBrush brush(m_clTipsBk);
			dc.SetBrush(brush);
			wxPen penBorder(wxColour(m_clRedLine), 1, wxSOLID);
			dc.SetPen(penBorder);
			dc.DrawRectangle(nLeft, nRectTop, w, h);

			dc.SetTextForeground(m_clCurSelLine); 
			pt.x = nLeft +2;
			pt.y = nRectTop; 
			dc.DrawText(strText, pt);

			nLeft		=  rtClient.GetLeft() - w-2;			
			pt.x = nLeft;
			pt.y = nRectTop; 
			dc.DrawRectangle(nLeft, nRectTop, w, h);

			pt.x = nLeft + 2;
			dc.DrawText(strText, pt);

		}
	}
}
/*
void CKVolume::OnLeftDown(wxMouseEvent& event)
{
	SetFocus();
	wxPoint pt = wxGetMousePosition(); 
	ScreenToClient(&pt.x, &pt.y);

	MAPsmark::iterator it = m_mapMarks.begin();
	while(it != m_mapMarks.end())
	{
		MAPsmark::iterator itTemp = it;
		it++;

		EnumMark enumMark	= (EnumMark)itTemp->first;
		SMark	 *pTemp		= itTemp->second;
		for(vector<SPoint *>::iterator it = pTemp->vecPt.begin(); it != pTemp->vecPt.end(); it++)
		{
			SPoint *p = *it;
			if(p->enumType != m_KType)
			{
				p->bSelected = false;
				continue;
			}

			if(enumMark == MARK_TRIANGLE_UP || enumMark == MARK_TRIANGLE_DOWN)
			{
				int nLeft =0, nRight =0;
				time2WeiZhi(p->dwTime, nLeft, nRight);
				int x =	(nLeft+nRight)/2;
				int y = JiaGe2GaoDu(p->dwPrice);
				if(pt.x >= x-5 && pt.x <= x+5 
					&& pt.y >= y-5 && pt.y <= y+5)
				{
					p->bSelected = true;
				}
				else if(p->bSelected)
					p->bSelected = false;					
			}
		}
	}
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KCtrl_LBUTTONDOWN); 
	myEvent.SetExtraLong(reinterpret_cast<long>(this));
	myEvent.SetClientData(&m_mapMarks);
	window->ProcessEvent(myEvent);
	Refresh(false);
}
void CKVolume::OnLeftUp(wxMouseEvent& event)
{
	wxPoint pt = wxGetMousePosition(); 
	ScreenToClient(&pt.x, &pt.y);
	int nCurSel =WeiZhi2XuHao(pt.x);
	if(nCurSel >=0 && nCurSel< (int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[nCurSel];
		double dbPrice = GaoDu2JiaGe(pt.y);	
		if(m_enumMark != MARK_UNDEFINE)
		{
			map<int, SMark *>::iterator it = m_mapMarks.find(m_enumMark);
			if(it != m_mapMarks.end())
			{
				SMark *pTemp	= it->second;
				SPoint *sp		= new SPoint;
				sp->enumType    = m_KType;
				sp->dwPrice		= dbPrice;
				sp->dwTime		= pKLine->dwTime;
				pTemp->vecPt.push_back(sp);
			}
			else
			{
				SMark *pTemp	 = new SMark;
				SPoint *sp		 = new SPoint;
				sp->enumType	 = m_KType;
				sp->dwPrice		 = dbPrice;
				sp->dwTime		 = pKLine->dwTime;
				pTemp->vecPt.push_back(sp);
				m_mapMarks.insert(MAPsmark::value_type(m_enumMark,  pTemp));						
			}	
			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_MARKSCHANGE);
			myEvent.SetClientData(&m_mapMarks);
			myEvent.SetExtraLong(reinterpret_cast<long>(this));
			window->ProcessEvent(myEvent);
			Refresh(false);
		}
	}

	m_enumMark = MARK_UNDEFINE;
}
void CKVolume::OnRightUp(wxMouseEvent& event)
{
	SPoint *pSel = NULL;
	MAPsmark::iterator it = m_mapMarks.begin();
	while(it != m_mapMarks.end())
	{
		MAPsmark::iterator itTemp = it;
		it++;

		EnumMark enumMark	= (EnumMark)itTemp->first;
		SMark	 *pTemp		= itTemp->second;
		for(vector<SPoint *>::iterator it = pTemp->vecPt.begin(); it != pTemp->vecPt.end(); it++)
		{
			SPoint *p = *it;
			if(p->bSelected == true)
			{
				pSel = p;
				break;
			}			
		}
	}

	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KCtrl_RBUTTONUP);
	
	myEvent.SetClientData(pSel);
	myEvent.SetExtraLong(reinterpret_cast<long>(this));

	if(m_pTechIndex)
		myEvent.SetString(m_pTechIndex->m_strTechName);

	window->ProcessEvent(myEvent);
}*/
bool CKVolume::InitCfg(TiXmlElement *root)
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

	TiXmlElement *tList = root->FirstChildElement("kvolume");
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

	ListItem = pColor->FirstChild("dashline");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetdashLine(value);

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

	ListItem = pColor->FirstChild("cltrangleup");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetTrangleUp(value);

	ListItem = pColor->FirstChild("cltrangledown");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetTrangleDown(value);

	ListItem = pColor->FirstChild("cllinecolor0");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineColor(0, value);

	ListItem = pColor->FirstChild("cllinecolor1");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineColor(1, value);

	ListItem = pColor->FirstChild("cllinecolor2");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineColor(2, value);

	ListItem = pColor->FirstChild("cllinecolor3");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineColor(3, value);

	ListItem = pColor->FirstChild("cllinecolor4");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineColor(4, value);

	ListItem = pColor->FirstChild("cllinecolor5");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineColor(5, value);

	ListItem = pColor->FirstChild("cllinecolor6");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineColor(6, value);

	ListItem = pColor->FirstChild("cllinecolor7");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLineColor(7, value);
	return true;
}
void CKVolume::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CKVolume::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CKVolume::SetRedLine(unsigned long lValue)
{
	m_clRedLine = lValue;
}
void CKVolume::SetGreenLine(unsigned long lValue)
{
	m_clGreenLine = lValue;
}
void CKVolume::SetdashLine(unsigned long lValue)
{
	m_cldashLine = lValue;
}
void CKVolume::SetCurSelLine(unsigned long lValue)
{
	m_clCurSelLine = lValue;
}
void CKVolume::SetTipsBk(unsigned long lValue)
{
	m_clTipsBk = lValue;
}
void CKVolume::SetLineName(unsigned long lValue)
{
	m_clLineName = lValue;
}

void CKVolume::SetTrangleUp(unsigned long lValue)
{
	m_clTrangleUp = lValue;
}
void CKVolume::SetTrangleDown(unsigned long lValue)
{
	m_clTrangleDown = lValue;
}
void CKVolume::SetLineColor(int nIndex, unsigned long lValue)
{
	if(nIndex <0 || nIndex >=COLOR_MAX)
		return;
	m_clLineColor[nIndex] = lValue;
}/*
void CKVolume::OnRightDown(wxMouseEvent& event)
{
	SetFocus();
	wxPoint pt = wxGetMousePosition(); 
	ScreenToClient(&pt.x, &pt.y);

	MAPsmark::iterator it = m_mapMarks.begin();
	while(it != m_mapMarks.end())
	{
		MAPsmark::iterator itTemp = it;
		it++;

		EnumMark enumMark	= (EnumMark)itTemp->first;
		SMark	 *pTemp		= itTemp->second;
		for(vector<SPoint *>::iterator it = pTemp->vecPt.begin(); it != pTemp->vecPt.end(); it++)
		{
			SPoint *p = *it;
			if(p->enumType != m_KType)
			{
				p->bSelected = false;
				continue;
			}

			if(enumMark == MARK_TRIANGLE_UP || enumMark == MARK_TRIANGLE_DOWN)
			{
				int nLeft =0, nRight =0;
				time2WeiZhi(p->dwTime, nLeft, nRight);
				int x =	(nLeft+nRight)/2;
				int y = JiaGe2GaoDu(p->dwPrice);
				if(pt.x >= x-5 && pt.x <= x+5 
					&& pt.y >= y-5 && pt.y <= y+5)
				{
					p->bSelected = true;
				}
				else if(p->bSelected)
					p->bSelected = false;	

				
			}
			
		}
	}
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KCtrl_MARKSCHANGE);
	myEvent.SetExtraLong(reinterpret_cast<long>(this));
	myEvent.SetClientData(&m_mapMarks);
	window->ProcessEvent(myEvent);
	Refresh(false);		

}*/