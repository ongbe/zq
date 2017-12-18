#include "stdafx.h"
#include "../../../inc/Quotation/KTechIndex.h"
#include <time.h>
#include "../../../inc/Quotation/TechIndexConfigDlg.h"





IMPLEMENT_CLASS(CKTechIndex, CKCtrl)

CKTechIndex::CKTechIndex(void)//:m_timer1(this, 1111),m_timer2(this, 1112)
{
	m_clBackGround	= wxColour(RGB(0,0,0));
	m_clBorder		= wxColour(RGB(192,0,0));
	m_clRedLine		= wxColour(RGB(240,0,0));
	m_clGreenLine	= wxColour(RGB(0,255,255));
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
	m_RectOffSet =  wxRect(66,16,30,5);
	m_nCurSel  = -1;
	
	m_nLeftPos  = 1;
	m_nRightPos = 1;

	m_uTimerarr[0] = -1;
	m_uTimerarr[1] = -1;
	m_bMaxMinChange		= TRUE;



	m_pMsgWnd = NULL;

	m_bIsMouse		= false;
	m_bIsKeyBoard	= false;

	m_bShowHeight = true;
	m_bShowTime  = false;
    
	m_enumMark = MARK_UNDEFINE;

	m_clTrangleUp	= wxColour(255, 0, 0);
	m_clTrangleDown = wxColour(0, 128, 0);
	m_clTrangleUpBorder = wxColour(255, 255, 255);
	m_clTrangleDownBorder = wxColour(255, 255, 255);

	m_ptMouseMoveLast = wxPoint(-1,-1);

	m_bShowCross	   = false;
	m_bMouseIn		   = false;
	
	m_pFData = NULL;
	m_nNumDigits = 2;

	m_pTechIndex = NULL;
	m_strTechIndexName = wxT("");
	SetTechIndex(wxT(KDJ));

	m_clLineColor[0] = RGB(200, 200, 200);//白
	m_clLineColor[1] = RGB(255, 255, 255);//白	
	m_clLineColor[2] = RGB(74, 203, 76);  //绿
	m_clLineColor[3] = RGB(181, 52, 179); //粉
	m_clLineColor[4] = RGB(241, 240, 113);//黄
	m_clLineColor[5] = RGB(124, 192, 255);//蓝
	m_clLineColor[6] = RGB(100, 100, 100);//大红
	m_clLineColor[7] = RGB(194, 106, 105);//
	m_clMACD_ZHANG   = RGB(255, 84, 84);
	m_clMACD_DIE     = RGB(84, 255, 255);

	m_KType = KTYPE_TECHINDEX;
}
CKTechIndex::~CKTechIndex(void)
{
	m_vecKLine.clear();
	//m_mapMarks.clear();
}
BEGIN_EVENT_TABLE(CKTechIndex, CKCtrl)
	EVT_PAINT(CKTechIndex::OnPaint)
END_EVENT_TABLE()

void CKTechIndex::CalcMaxMin()
{
	if(!m_bMaxMinChange)
		return;//主要优化速度
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;
	if(!m_pTechIndex)
		return;
	if(m_vecLeftScales.size())
	{
		m_fMinValue = m_vecLeftScales[0];
		m_fMaxValue = m_vecLeftScales[m_vecLeftScales.size()-1];
	}
	else
	{
		for(int k =0; k < m_pTechIndex->m_nVecDataCount; k++)
		{
			for(int i = m_nBegin; i< (int)m_vecKLine.size() && i>= m_nEnd  && i>=0; i--)
			{
				STechIndexValue sValue;
				sValue.fValue = MIN_VALUE_DEFAULT;		
				if(i >= 0 && i < (int)m_pTechIndex->m_vecData[k].size())
					sValue = m_pTechIndex->m_vecData[k][i];
				if(sValue.fValue - MIN_VALUE_DEFAULT > -0.00001 && sValue.fValue - MIN_VALUE_DEFAULT < 0.00001)	
				{
					
				}
				else
				{
					if(sValue.fValue > m_fMaxValue)
					{
						m_fMaxValue = sValue.fValue;
						m_nMaxPos = i;
					}
					if(sValue.fValue < m_fMinValue)
					{
						m_fMinValue = sValue.fValue;
						m_nMinPos = i;
					}
				}
			}
		}
	}

	if(m_fMaxValue - MIN_VALUE_DEFAULT > -0.00001 && m_fMaxValue - MIN_VALUE_DEFAULT < 0.00001)	
		m_fMaxValue =10.0;
	if(m_fMinValue - MAX_VALUE_DEFAULT > -0.00001 && m_fMinValue - MAX_VALUE_DEFAULT < 0.00001)	
		m_fMinValue =0;
}
void CKTechIndex::OnPaint (wxPaintEvent & PaintEvent)
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
void CKTechIndex::DrawDC(wxDC& dc)
{
	wxSize size = GetClientSize();
	wxBrush brush(m_clBackGround);
	dc.SetBrush(brush);
	dc.DrawRectangle(0, 0, size.x, size.y);
	
	wxRect rt(0, 0, size.x, size.y);
	if(m_vecLeftScales.size() == 0)
		DrawScales(dc);
	else
		DrawLeftScales(dc);
	DrawBorder(dc, rt);
	DrawHead(dc);

	DrawMarks(dc);
	DrawTechIndex(dc);
	DrawMACD(dc);
	if(m_bMouseIn && m_bShowCross)
		DrawCurSelLine(dc);
	if(m_bMouseIn)
		DrawTips(dc);
}
void CKTechIndex::DrawHead(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());

	wxFont wxfont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
	dc.SetFont(wxfont);
	dc.SetTextForeground(m_clLineName); 

	wxString  strText;
	wxCoord w,h;
	wxPoint pt;

	if(m_pFData != NULL)
	{
		if(m_pFData->m_ContractDate == "")
			strText =  m_pFData->m_ContractName;
		else
			strText.Printf(_T("%s_%s"), m_pFData->m_ContractName.c_str(), m_pFData->m_ContractDate.c_str());
	}
	
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + 5;
	pt.y = rtClient.GetTop() -h; 

	wxCoord w2,h2;
	wxString strLineName = "T";
	GetLineName(strLineName);
	dc.GetTextExtent(strLineName, &w2, &h2);	
	
	if(m_pTechIndex == NULL)
		return;
	
	if(m_pTechIndex->m_nVecDataCount == 0 )
		return;

	int nLeft = pt.x;
	wxString strTechTitle;
	strTechTitle.Printf(wxT("%s"), m_pTechIndex->m_strTechName);
	for(int nConfig =0; nConfig < (int)m_pTechIndex->m_vecConfig.size(); nConfig++)
	{
		if(nConfig == 0)
			strTechTitle.Append("(");
		wxString strTemp;
		if(nConfig == 0)
			strTemp.Printf(wxT("%d"), m_pTechIndex->m_vecConfig[nConfig]);
		else
			strTemp.Printf(wxT(", %d"), m_pTechIndex->m_vecConfig[nConfig]);

		strTechTitle.Append(strTemp);
		if(nConfig == m_pTechIndex->m_vecConfig.size()-1)
			strTechTitle.Append(")");
	}
	dc.SetTextForeground(m_clLineColor[0]); 
	wxCoord wConfig,hConfig;
	dc.GetTextExtent(strTechTitle, &wConfig, &hConfig);	
	pt.x = nLeft;
	pt.y = rtClient.GetTop() -h2; 
	dc.DrawText(strTechTitle, pt);

	nLeft = pt.x + wConfig + 5;			
	for(int k =0; k < m_pTechIndex->m_nVecDataCount; k++)
	{
		dc.SetTextForeground(m_clLineColor[k+1]); 
		wxPen penBorder(wxColour(m_clLineColor[k+1]), m_nBorderWidth, wxSOLID);
		dc.SetPen(penBorder);
		wxString str;
		wxString strFormat;
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;		
		if(m_pTechIndex->m_vecData && m_nCurSel >= 0 && m_nCurSel < (int)m_pTechIndex->m_vecData[k].size())
			sValue = m_pTechIndex->m_vecData[k][m_nCurSel];
		if(sValue.fValue - MIN_VALUE_DEFAULT > -0.00001 && sValue.fValue - MIN_VALUE_DEFAULT < 0.00001)	
		{
			if((int)m_pTechIndex->m_vecConfigText.size() > k)
				str.Printf(wxT("%s: "), m_pTechIndex->m_vecConfigText[k].c_str());
		}
		else
		{
			strFormat.Printf(wxT("%%s: %%.%df"), 3);	
			str.Printf(strFormat, sValue.strName.c_str(), sValue.fValue);
		}
		
		wxCoord w3,h3;
		dc.GetTextExtent(str, &w3, &h3);	
		pt.x = nLeft;
		pt.y = rtClient.GetTop() -h2; 
		dc.DrawText(str, pt);
		nLeft = pt.x + w3 + 5;		
	}

}
void CKTechIndex::GetLineName(wxString& strLineName)
{
	switch(m_enumPhrase)
	{
	case PHRASE_1MIN:
		strLineName = "1分钟";
		break;
	case PHRASE_5MIN:
		strLineName = "5分钟";
		break;
	case PHRASE_15MIN:
		strLineName = "15分钟";
		break;
	case PHRASE_30MIN:
		strLineName = "30分钟";
		break;
	case PHRASE_60MIN:
		strLineName = "60分钟";
		break;
	case PHRASE_DAY:
		strLineName = "日线";
		break;
	case PHRASE_WEEK:
		strLineName = "周线";
		break;
	case PHRASE_MONTH:
		strLineName = "月线";
		break;
	}
}

void CKTechIndex::DrawBorder(wxDC& dc, wxRect& rt)
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
}
void  CKTechIndex::DrawItems(wxDC& dc)
{
	if(m_vecKLine.size() == 0)
		return;

	for(int i = m_nBegin; i>= m_nEnd; i--)
	{
		SKLine * pKLine = m_vecKLine[i];
		wxASSERT(pKLine != NULL);

		int nLeft =0, nRight =0;
		XuHao2WeiZhi(i, nLeft, nRight);
		int top1 = JiaGe2GaoDu(pKLine->fHighPrice);
		int top2 = JiaGe2GaoDu(pKLine->fOpenPrice);
		int top3 = JiaGe2GaoDu(pKLine->fClosePrice);
		int top4 = JiaGe2GaoDu(pKLine->fLowPrice);

		DrawOneItem(dc, nLeft, nRight, top1, top2, top3, top4);
	}

}
void CKTechIndex::DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4)
{//int nTop1 最高, int nTop2 开盘, int nTop3 收盘, int nTop4 最低

	if(nTop2 >= nTop3)
	{
		wxPen penBorder(wxColour(m_clRedLine), 1, wxSOLID);
		dc.SetPen(penBorder);
		dc.DrawLine(nLeft , nTop3, nRight, nTop3);
		dc.DrawLine(nRight, nTop3, nRight, nTop2);
		dc.DrawLine(nRight, nTop2, nLeft, nTop2);
		dc.DrawLine(nLeft, nTop2, nLeft, nTop3);

		dc.DrawLine((nLeft+nRight)/2, nTop3, (nLeft+nRight)/2, nTop1);
		dc.DrawLine((nLeft+nRight)/2, nTop2, (nLeft+nRight)/2, nTop4);
	}
	else
	{
		wxPen penBorder(wxColour(m_clGreenLine), 1, wxSOLID);
		dc.SetPen(penBorder);
		dc.DrawLine((nLeft+nRight)/2, nTop2, (nLeft+nRight)/2, nTop1);
		dc.DrawLine((nLeft+nRight)/2, nTop3, (nLeft+nRight)/2, nTop4);


		wxBrush brush(m_clGreenLine);
		dc.SetBrush(brush);
		dc.DrawRectangle(nLeft, nTop2, nRight - nLeft, nTop3 - nTop2);	
	}
}
void CKTechIndex::DrawScales(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;

	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
//	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());


	wxPen penBorder(wxColour(m_cldashLine), 1, wxSOLID);
	dc.SetPen(penBorder);
//	dc.DrawLine(rtClient.GetLeft(), rtClient.GetBottom() - rtClient.GetHeight()/3, rtClient.GetRight(), rtClient.GetBottom() - rtClient.GetHeight()/3);
	dc.DrawLine(rtClient.GetLeft(), rtClient.GetBottom() - 1.0*rtClient.GetHeight()/2, rtClient.GetRight(), rtClient.GetBottom() - 1.0*rtClient.GetHeight()/2);
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
//	strText = wxString::Format(wxT("%.2f"), m_fMaxValue);
	strText = Double2String(m_fMaxValue, 1);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);


	nTop = rtClient.GetTop() -10 + 1.0*rtClient.GetHeight()/2;
//	strText = wxString::Format(wxT("%.2f"), m_fMaxValue - 2*(m_fMaxValue- m_fMinValue)/3);
	strText = Double2String(m_fMaxValue - (m_fMaxValue- m_fMinValue)/2, 1);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);


	nTop = rtClient.GetBottom() -15 ;
//	strText = wxString::Format(wxT("%.2f"), m_fMinValue );
	strText = Double2String(m_fMinValue, 1);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);
}
void CKTechIndex::DrawLeftScales(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	wxRect rect = rtClient;

	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
//	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());


	wxPen penBorder(wxColour(m_cldashLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	int nSize = m_vecLeftScales.size();
	for(int i=0; i< nSize; i++)
	{
		dc.DrawLine(rtClient.GetLeft(), rtClient.GetBottom() - rtClient.GetHeight()*(m_vecLeftScales[i]-m_fMinValue)/(m_fMaxValue - m_fMinValue)
			,rtClient.GetRight(), rtClient.GetBottom()- rtClient.GetHeight()*(m_vecLeftScales[i]-m_fMinValue)/(m_fMaxValue - m_fMinValue));
	}

	wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 
	dc.SetTextForeground(m_clBorder); 

	if(m_fMaxValue  == MIN_VALUE_DEFAULT ||	m_fMinValue == MAX_VALUE_DEFAULT)
		return;

	wxString  strText;
	wxCoord w,h;

	wxPoint pt;

	int nTop = rtClient.GetTop() -10;
	//	strText = wxString::Format(wxT("%.2f"), m_fMaxValue);
	strText = Double2String(m_fMaxValue, 1);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);

	for(int i=1; i< nSize-1; i++)
	{
		int nHeight = rtClient.GetBottom() - rtClient.GetHeight()*(m_vecLeftScales[i]-m_fMinValue)/(m_fMaxValue - m_fMinValue);
		strText = Double2String(m_vecLeftScales[i], 1);
		dc.GetTextExtent(strText, &w, &h);	
		nTop = nHeight - h/2;
		pt.x = rtClient.GetLeft() - w-4;
		pt.y = nTop; 
		dc.DrawText(strText, pt);
	}
	nTop = rtClient.GetBottom() -15 ;
	//	strText = wxString::Format(wxT("%.2f"), m_fMinValue );
	strText = Double2String(m_fMinValue, 1);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);
}
void CKTechIndex::DrawZuiGaoZuiDiJia(wxDC& dc)
{
	if(m_vecKLine.size() == 0)
		return;

	wxASSERT((int)m_vecKLine.size() > m_nMaxPos || (int)m_vecKLine.size() > m_nMinPos);

	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());


	int nLeft1 =0, nRight1 =0;
	XuHao2WeiZhi(m_nMaxPos, nLeft1, nRight1);
	SKLine *pKLine = m_vecKLine[m_nMaxPos];
	int top1 = JiaGe2GaoDu(pKLine->fHighPrice);

	wxPen penBorder(wxColour(m_clCurSelLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	if(rtClient.GetRight() > nRight1 +40)
	{
		dc.DrawLine((nLeft1+nRight1)/2, top1, (nLeft1+nRight1)/2+20, top1+10);		
	}
	else
	{
		dc.DrawLine((nLeft1+nRight1)/2, top1, (nLeft1+nRight1)/2-20, top1+10);
	}
	int nLeft2 =0, nRight2 =0;
	XuHao2WeiZhi(m_nMinPos, nLeft2, nRight2);
	pKLine = m_vecKLine[m_nMinPos];
	int top2 = JiaGe2GaoDu(pKLine->fLowPrice);

	if(rtClient.GetRight() > nRight2 +40)
	{
		dc.DrawLine((nLeft2+nRight2)/2, top2, (nLeft2+nRight2)/2+20, top2-10);		
	}
	else
	{
		dc.DrawLine((nLeft2+nRight2)/2, top2, (nLeft2+nRight2)/2-20, top2-10);		
	}

	wxFont wxfont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 
	dc.SetTextForeground(m_clCurSelLine); 

	if(rtClient.GetRight() > nRight1 +40)
	{
		wxString strText;
		wxPoint pt;
	//	strText = wxString::Format(wxT("%.2f"), m_fMaxValue);
		strText = Double2String(m_fMaxValue, m_nNumDigits);

		pt.x = (nLeft1+nRight1)/2+20;
		pt.y = top1+10; 
		dc.DrawText(strText, pt);
	}
	else
	{
		wxString strText;
		wxPoint pt;
		//strText = wxString::Format(wxT("%.2f"), m_fMaxValue);
		strText = Double2String(m_fMaxValue,m_nNumDigits);
		pt.x = (nLeft1+nRight1)/2-40;
		pt.y = top1+10; 
		dc.DrawText(strText, pt);
	}

	if(rtClient.GetRight() > nRight2 +40)
	{
		wxString strText;
		wxPoint pt;
	//	strText = wxString::Format(wxT("%.2f"), m_fMinValue);
		strText = Double2String(m_fMinValue, m_nNumDigits);
		pt.x = (nLeft2+nRight2)/2+20;
		pt.y = top2-20; 
		dc.DrawText(strText, pt);
	}
	else
	{
		wxString strText;
		wxPoint pt;
	//	strText = wxString::Format(wxT("%.2f"), m_fMinValue);
		strText = Double2String(m_fMinValue, m_nNumDigits);
		pt.x = (nLeft2+nRight2)/2-40;
		pt.y = top2-20; 
		dc.DrawText(strText, pt);
	}

}

void CKTechIndex::DrawCurSelLine(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + 1);
	//rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetRight(rtClient.GetRight() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() -2);

	wxPen penBorder(wxColour(m_clCurSelLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	dc.SetLogicalFunction(wxXOR);
	if(m_ptMouseMoveLast.x >= rtClient.GetLeft()	&& m_ptMouseMoveLast.x <= rtClient.GetRight())
	{		
		dc.DrawLine(m_ptMouseMoveLast.x, rtClient.GetTop(), m_ptMouseMoveLast.x, rtClient.GetBottom());
	}
	if(m_ptMouseMoveLast.y >= rtClient.GetTop() && m_ptMouseMoveLast.y <= rtClient.GetBottom())
		dc.DrawLine(rtClient.GetLeft(), m_ptMouseMoveLast.y, rtClient.GetRight(), m_ptMouseMoveLast.y);	

}
void CKTechIndex::DrawTips(wxDC& dc)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + 1);
	//rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetRight(rtClient.GetRight() -m_RectOffSet.GetLeft());
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
			strText = Double2String(dbPrice, 3);
			//dc.GetTextExtent(strText, &w, &h);	
			
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
void CKTechIndex::DrawMarks(wxDC& dc)
{
	if(m_pFData == NULL)
		return;
	MAPsmark*  pMarks = m_pFData->GetMarksCommon(m_enumPhrase);
	if(pMarks == NULL)
		return;
	MAPsmark::iterator it = pMarks->begin();
	while(it != pMarks->end())
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

bool CKTechIndex::InitCfg(TiXmlElement *root)
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
	TiXmlElement *tList = root->FirstChildElement("ktechindex");
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


	ListItem = pColor->FirstChild("macdzhang");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetMACDZHANG(value);

	ListItem = pColor->FirstChild("macddie");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetMACDDIE(value);
	return true;
}
void CKTechIndex::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CKTechIndex::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CKTechIndex::SetRedLine(unsigned long lValue)
{
	m_clRedLine = lValue;
}
void CKTechIndex::SetGreenLine(unsigned long lValue)
{
	m_clGreenLine = lValue;
}
void CKTechIndex::SetdashLine(unsigned long lValue)
{
	m_cldashLine = lValue;
}
void CKTechIndex::SetCurSelLine(unsigned long lValue)
{
	m_clCurSelLine = lValue;
}
void CKTechIndex::SetTipsBk(unsigned long lValue)
{
	m_clTipsBk = lValue;
}
void CKTechIndex::SetLineName(unsigned long lValue)
{
	m_clLineName = lValue;
}
void CKTechIndex::SetTrangleUp(unsigned long lValue)
{
	m_clTrangleUp = lValue;
}
void CKTechIndex::SetTrangleDown(unsigned long lValue)
{
	m_clTrangleDown = lValue;
}
void CKTechIndex::SetMACDZHANG(unsigned long lValue)
{
	m_clMACD_ZHANG = lValue;
}
void CKTechIndex::SetMACDDIE(unsigned long lValue)
{
	m_clMACD_DIE = lValue;
}
void CKTechIndex::SetLineColor(int nIndex, unsigned long lValue)
{
	if(nIndex <0 || nIndex >=COLOR_MAX)
		return;
	m_clLineColor[nIndex] = lValue;
}
void CKTechIndex::SetTechIndex(wxString strTechIndex)
{
	if(m_strTechIndexName == strTechIndex)
		return;

	m_vecLeftScales.clear();
	if(strTechIndex == KDJ)
	{
		m_vecLeftScales.push_back(0.0);
		m_vecLeftScales.push_back(20.0);
		m_vecLeftScales.push_back(50.0);
		m_vecLeftScales.push_back(80.0);
		m_vecLeftScales.push_back(100.0);
	}
	m_strTechIndexName = strTechIndex;
	if(m_pFData)
	{
		STechCalcPara sTCPara;
		sTCPara.enumPhrase = m_enumPhrase;
		sTCPara.m_VolumeMultiple = m_pFData->m_VolumeMultiple;
		m_pFData->CalcKTechIndex(m_vecKLine,"", m_strTechIndexName.c_str(), sTCPara, m_pTechIndex, NULL);
	}
	CalcMaxMin();
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);
}

void CKTechIndex::DrawMACD(wxDC& dc)
{
	if(m_strTechIndexName != MACD)
		return;
	if(m_pTechIndex == NULL || m_pTechIndex->m_nDrawBody == 0 )
		return;


	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop()-1);
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop()+1);

	
	if(m_pTechIndex->m_vecData == NULL)
		return;
	if( m_pTechIndex->m_vecData[2].size() == 0)
		return;
	dc.SetClippingRegion(rtClient);
	for(int i = m_nBegin; i> m_nEnd; i--)
	{
		if(i>= (int)m_pTechIndex->m_vecData[2].size() )
			continue;
		int nLeftBegin =0, nRightBegin =0;
		XuHao2WeiZhi(i, nLeftBegin, nRightBegin);
		int nBegin = JiaGe2GaoDu(m_pTechIndex->m_vecData[2][i].fValue);
		int nEnd   = JiaGe2GaoDu(0);
		wxColour clLine;
		if(m_pTechIndex->m_vecData[2][i].fValue > 0)
			clLine = m_clMACD_ZHANG;
		else
			clLine = m_clMACD_DIE;

		wxPen penBorder(clLine, 1, wxSOLID);
		dc.SetPen(penBorder);
		dc.DrawLine((nLeftBegin+nRightBegin)/2, nBegin, (nLeftBegin+nRightBegin)/2, nEnd);
	}
	
	dc.DestroyClippingRegion();
}