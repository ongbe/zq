#include "stdafx.h"
#include "../inc/Quotation/KCtrl.h"
#include <time.h>
#include "../inc/Quotation/TechIndexConfigDlg.h"


DEFINE_EVENT_TYPE(wxEVT_KCtrl_KeyDown)
DEFINE_EVENT_TYPE(wxEVT_KCtrl_LBUTTONDOWN)
DEFINE_EVENT_TYPE(wxEVT_KCtrl_MOUSEMOVE)
DEFINE_EVENT_TYPE(wxEVT_KCtrl_RBUTTONUP)
DEFINE_EVENT_TYPE(wxEVT_KCtrl_MOUSELEAVE)
DEFINE_EVENT_TYPE(wxEVT_KCtrl_LEFTDBCLICK)
DEFINE_EVENT_TYPE(wxEVT_KCtrl_MARKSCHANGE)
DEFINE_EVENT_TYPE(wxEVT_KCtrl_MOUSEENTER)
DEFINE_EVENT_TYPE(wxEVT_KCtrl_RBUTTONDOWN)


IMPLEMENT_CLASS(CKCtrl, wxPanel)

CKCtrl::CKCtrl(void):m_timer1(this, 1111),m_timer2(this, 1112)
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



	m_pMsgWnd = NULL;

	m_bIsMouse		= false;
	m_bIsKeyBoard	= false;

	m_bShowHeight = true;
	m_bShowTime  = false;
    

	m_LeftpanelName     = wxColour(255, 255, 255);
	m_LeftpanelTime		= wxColour(82, 255, 255);
	m_Leftpanelzhang		= wxColour(255, 82, 82);
	m_Leftpaneldie		= wxColour(82, 255, 82);
	m_Leftpanelhengpan	= wxColour(255, 255, 255);
	m_nLeftPanelHeight = 300;//左侧价格提示窗高度



	m_enumMark = MARK_UNDEFINE;

	m_clTrangleUp	= wxColour(255, 0, 0);
	m_clTrangleDown = wxColour(0, 128, 0);
	m_clTrangleUpBorder = wxColour(255, 255, 255);
	m_clTrangleDownBorder = wxColour(255, 255, 255);

	m_ptMouseMoveLast = wxPoint(-1,-1);

//	m_bShowOneItemData = true;
	m_bShowCross	   = false;
	m_bMouseIn		   = false;
	
	m_pFData = NULL;
	m_nNumDigits = 2;

	m_pTechIndex = NULL;
	m_strTechIndexName = wxT(MA);

	m_clLineColor[0] = RGB(200, 200, 200);//白
	m_clLineColor[1] = RGB(255, 255, 255);//白	
	m_clLineColor[2] = RGB(74, 203, 76);  //绿
	m_clLineColor[3] = RGB(181, 52, 179); //粉
	m_clLineColor[4] = RGB(241, 240, 113);//黄
	m_clLineColor[5] = RGB(124, 192, 255);//蓝
	m_clLineColor[6] = RGB(100, 100, 100);//大红
	m_clLineColor[7] = RGB(194, 106, 105);//

	m_KType = KTYPE_KLINE;

}
bool CKCtrl::Create(wxWindow *parent,
			wxWindowID winid ,
			const wxPoint& pos ,
			const wxSize& size,
			long style,
			const wxString& name )
{
	style |= wxWANTS_CHARS|wxTE_PROCESS_ENTER|wxCLIP_CHILDREN|wxCLIP_SIBLINGS;
	bool bReturn = wxPanel::Create(parent,winid,pos, size,	style, name);		
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);

	return bReturn;
}
void CKCtrl::SetFData(CFData *pFData)
{
	m_pFData = pFData;	
	if(m_pFData)
	{
		m_nNumDigits = m_pFData->GetDotDigits();		
	}
	m_vecKLine.clear();
}
CKCtrl::~CKCtrl(void)
{
	m_vecKLine.clear();
	m_mapMarks.clear();
}
BEGIN_EVENT_TABLE(CKCtrl, wxPanel)
	EVT_PAINT(CKCtrl::OnPaint)
	EVT_TIMER(wxID_ANY, CKCtrl::OnTimer1)
	EVT_LEFT_DOWN(CKCtrl::OnLeftDown)
	EVT_LEFT_DCLICK(CKCtrl::OnLeftDBClick)
	EVT_LEFT_UP(CKCtrl::OnLeftUp)
	EVT_RIGHT_DOWN(CKCtrl::OnRightDown)
	EVT_RIGHT_UP(CKCtrl::OnRightUp)
	EVT_MOTION (CKCtrl::OnMouseMove)
	EVT_SIZE(CKCtrl::OnSize)
	EVT_LEAVE_WINDOW(CKCtrl::OnMouseLeave)
	EVT_ENTER_WINDOW(CKCtrl::OnMouseEnter)
	EVT_COMMAND(wxID_ANY, wxEVT_CONFIG_CHANGE, CKCtrl::OnConfigChange)
END_EVENT_TABLE()

void CKCtrl::SetFuture(string ExchangeName,string ContractName,string ContractDate, EnumPhrase	enumPhrase)
{
	m_ExchangeName = ExchangeName;
	m_ContractName = ContractName;
	m_ContractDate = ContractDate;

	if(m_enumPhrase != enumPhrase)
	{
		m_nCurSel = 0;
	//	m_nDayWidth = 4;
	//	m_nDaySpan = 2;
	}
	m_enumPhrase   = enumPhrase;	
}
BOOL CKCtrl::SetVectorData(vector<SKLine *>& vecKLine)
{	
	m_vecKLine.clear();
	m_vecKLine = vecKLine;

	

	if(m_pFData && m_strTechIndexName!= "")
		m_pFData->CalcKTechIndex(m_vecKLine,"", m_strTechIndexName.c_str(), m_enumPhrase, m_pTechIndex);

	GetShowCount(0);
	GetBeginEnd();
	CalcMaxMin();
	return TRUE;
}
void CKCtrl::SetMarks(MAPsmark& mapMarks)
{
	m_mapMarks.clear();
	m_mapMarks = mapMarks;
	m_enumMark = MARK_UNDEFINE;
}
void CKCtrl::SetPhrase(EnumPhrase enumPhrase)
{
	if(m_enumPhrase != enumPhrase)
	{
		m_nCurSel = 0;
	//	m_nDayWidth = 4;
	//	m_nDaySpan = 2;
	}
	m_enumPhrase   = enumPhrase;
	
}
EnumPhrase CKCtrl::GetPhrase()
{
	return	m_enumPhrase;
}
void CKCtrl::SetMsgWnd(wxWindow *pMsgWnd)
{
	m_pMsgWnd = pMsgWnd;
}
void CKCtrl::CalcMaxMin()
{
	if(!m_bMaxMinChange)
		return;//主要优化速度
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue = MAX_VALUE_DEFAULT;
	for(int i = m_nBegin; i< (int)m_vecKLine.size() && i>= m_nEnd  && i>=0; i--)
	{
		SKLine *pKLine = m_vecKLine[i];
		if(pKLine->fHighPrice > m_fMaxValue)
		{
			m_fMaxValue = pKLine->fHighPrice;
			m_nMaxPos = i;
		}
		if(pKLine->fLowPrice < m_fMinValue)
		{
			m_fMinValue = pKLine->fLowPrice;
			m_nMinPos = i;
		}
	}

	if(!m_pTechIndex)
		return;
	for(int k =0; k < m_pTechIndex->m_nVecDataCount; k++)
	{
			for(int i = m_nBegin; i< (int)m_vecKLine.size() && i>= m_nEnd; i--)
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
int	CKCtrl::JiaGe2GaoDu(float fJiaGe)
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
float CKCtrl::GaoDu2JiaGe(int nHeight)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight() - m_RectOffSet.GetTop());

	float fJiaGe = (m_fMaxValue - m_fMinValue)*(rtClient.GetBottom() - nHeight )/rtClient.GetHeight() + m_fMinValue;
	return fJiaGe;
}
int	CKCtrl::WeiZhi2XuHao(int nWidthPos)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight() - m_RectOffSet.GetTop());

	int nWidth = nWidthPos - rtClient.GetLeft();
	if(nWidth <0)//左边空白向左，则去掉
		return -1;

	if(m_nDayWidth +m_nDaySpan == 0)
		return -1;
	int nCount = nWidth/(m_nDayWidth +m_nDaySpan);
	if(nWidth % (m_nDayWidth +m_nDaySpan) > m_nDayWidth)
		nCount ++;

	nCount = m_nBegin - nCount;

	return nCount;//超过部分，也为左边的Item
}
float CKCtrl::XuHao2WeiZhi(int nItem, int& nLeft, int& nRight)
{
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft() + 2);//2个像素为了好看一些
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft() -2);
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight() - m_RectOffSet.GetTop());

	int nKaiShi =  m_nBegin - nItem;
	if(nKaiShi == 0)
	{
		nLeft = rtClient.GetLeft();
		nRight = nLeft + m_nDayWidth;
	}
	else
	{
		nLeft = rtClient.GetLeft() + nKaiShi * (m_nDayWidth +m_nDaySpan);
		nRight = nLeft + m_nDayWidth;
	}
	return nLeft;
}
int  CKCtrl::time2WeiZhi(struct tm tmTime,int& nLeft, int& nRight)
{
	if(m_vecKLine.size() == 0)
		return -1;

	int nIndex = -1;
	for(int i = m_nEnd; i<= m_nBegin; i++)
	{
		SKLine *pKline = m_vecKLine[i];
		int dwTimeSpan = difftime(mktime(&tmTime), mktime(&pKline->dwTime));
		if(dwTimeSpan == 0)
		{
			nIndex = i;
			break;
		}
	}
	if(nIndex == -1)
		return -1;
	return XuHao2WeiZhi(nIndex, nLeft, nRight);
}
void CKCtrl::GetBeginEnd()
{
	int nBegin = 0, nEnd =0;
	wxRect rtClient = GetClientRect();

	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());

	int nWith = rtClient.GetWidth();
	if(nWith <= 0)
		return;

	if(m_nDayWidth +m_nDaySpan == 0)
		return;

	int nCount = nWith/(m_nDayWidth +m_nDaySpan);
	int nSize = m_vecKLine.size();
	if(nCount > nSize)
	{
		nBegin = nSize -1;
		nEnd   = 0;
	}
	else
	{
		nBegin = nCount-1;
		nEnd   = 0;
	}


	if(m_nCurSel >= m_nEnd && m_nCurSel <= m_nBegin && (nCount-1 == m_nBegin - m_nEnd) && m_nBegin < (int)m_vecKLine.size())
	{

	}
	else if(nBegin - nEnd >= (int)m_vecKLine.size() )
	{
		m_nBegin = m_vecKLine.size()-1;
		m_nEnd   = 0 ;
		if(m_nCurSel!= -1 &&( m_nCurSel > m_nBegin || m_nCurSel < m_nEnd) )
			m_nCurSel = 0;
	}	
	else if(m_nCurSel>=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		int nShow = nBegin - nEnd;
		int nOffset =0;
		m_nEnd   = m_nCurSel - nShow/2;
		if(m_nEnd < 0)
		{
			nOffset = -m_nEnd;
			m_nEnd = 0;
		}
		m_nBegin = m_nCurSel + nShow - nShow/2 + nOffset;
		if(m_nBegin >= (int)m_vecKLine.size())
		{
			m_nBegin = m_vecKLine.size() - 1;
			m_nEnd = m_nBegin - nShow;
		}
	}
	else
	{
		m_nBegin = nBegin;
		m_nEnd   = nEnd;
	}
}
void CKCtrl::OnPaint (wxPaintEvent & PaintEvent)
{
/*	wxPaintDC dc(this);
	wxMemoryDC memdc;  
	memdc.SetLayoutDirection(dc.GetLayoutDirection());
    wxBitmap bmp;
	wxSize size = GetClientSize();
    bmp.Create(size.GetWidth(), size.GetHeight());
    memdc.SelectObject(bmp);

	DrawDC(memdc);
	dc.Blit(0, 0, size.GetWidth(),size.GetHeight(),&memdc, 0, 0);
   */
}
void CKCtrl::DrawDC(wxDC& dc)
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
	DrawZuiGaoZuiDiJia(dc);


	
	DrawMarks(dc);
	DrawTechIndex(dc);
	if(m_bMouseIn && m_bShowCross)
		DrawCurSelLine(dc);
	if(m_bMouseIn)
		DrawTips(dc);

	if(m_bMouseIn && m_bShowCross)
	{
		wxRect rtData = wxRect(2, m_RectOffSet.GetHeight(), m_RectOffSet.GetLeft(), m_nLeftPanelHeight);
		DrawOneItemData(dc, rtData);
	}
}
void CKCtrl::DrawHead(wxDC& dc)
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

	if(m_ContractDate == "")
		strText =  m_ContractName;
	else
		strText.Printf(_T("%s_%s"), m_ContractName.c_str(), m_ContractDate.c_str());
//	strText.Printf(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());
	
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + 5;
	pt.y = rtClient.GetTop() -h; 
	dc.DrawText(strText, pt);

	dc.SetTextForeground(m_clBorder); 
	wxCoord w2,h2;
	wxString strLineName;
	GetLineName(strLineName);
	dc.GetTextExtent(strLineName, &w2, &h2);	
	pt.x = pt.x + w + 2;
	pt.y = rtClient.GetTop() -h2; 
	dc.DrawText(strLineName, pt);

	
	if(m_pTechIndex == NULL)
		return;
	
	if(m_pTechIndex->m_nVecDataCount == 0 )
		return;

	int nLeft = pt.x + w2 + 2;
	wxString strTechTitle;
	strTechTitle.Printf(wxT("%s("), m_pTechIndex->m_strTechName);
	for(int nConfig =0; nConfig< (int)m_pTechIndex->m_vecConfig.size(); nConfig++)
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
	pt.y = rtClient.GetTop() -h2; 
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
		pt.y = rtClient.GetTop() -h2; 
		dc.DrawText(str, pt);
		nLeft = pt.x + w3 + 5;		
	}

}
void CKCtrl::GetLineName(wxString& strLineName)
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

void CKCtrl::DrawBorder(wxDC& dc, wxRect& rt)
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
void  CKCtrl::DrawItems(wxDC& dc)
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
void CKCtrl::DrawOneItem(wxDC& dc, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4)
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
void CKCtrl::DrawScales(wxDC& dc)
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
//	strText = wxString::Format(wxT("%.2f"), m_fMaxValue);
	strText = Double2String(m_fMaxValue, m_nNumDigits, true);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);


	nTop = rtClient.GetTop() -10 + rtClient.GetHeight()/3;
//	strText = wxString::Format(wxT("%.2f"), m_fMaxValue - (m_fMaxValue- m_fMinValue)/3);
	strText = Double2String(m_fMaxValue - (m_fMaxValue- m_fMinValue)/3, m_nNumDigits, true);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);

	nTop = rtClient.GetTop() -10 + 2*rtClient.GetHeight()/3;
//	strText = wxString::Format(wxT("%.2f"), m_fMaxValue - 2*(m_fMaxValue- m_fMinValue)/3);
	strText = Double2String(m_fMaxValue - 2*(m_fMaxValue- m_fMinValue)/3, m_nNumDigits, true);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);


	nTop = rtClient.GetBottom() -15 ;
//	strText = wxString::Format(wxT("%.2f"), m_fMinValue );
	strText = Double2String(m_fMinValue, m_nNumDigits, true);
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() - w-4;
	pt.y = nTop; 
	dc.DrawText(strText, pt);
}
void CKCtrl::DrawZuiGaoZuiDiJia(wxDC& dc)
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

void CKCtrl::DrawCurSelLine(wxDC& dc)
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
	if(m_ptMouseMoveLast.y >= rtClient.GetTop() && m_ptMouseMoveLast.y <= rtClient.GetBottom())
		dc.DrawLine(rtClient.GetLeft(), m_ptMouseMoveLast.y, rtClient.GetRight(), m_ptMouseMoveLast.y);	

}
void CKCtrl::DrawTips(wxDC& dc)
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

void CKCtrl::DrawTips_Time(wxDC& dc, wxRect rtClient, int nX)
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

	wxString strTextBottom;
	strTextBottom.Printf(_T("%d/%d%d"), pKLine->dwTime.tm_year+1900, pKLine->dwTime.tm_mon+1, pKLine->dwTime.tm_mday);
	if (strTextBottom.Length() < 10)
		return;
	wxFont wxfont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 
	dc.SetFont(wxfont);
	wxCoord w,h;
	wxPoint pt;
	int nTop = rtClient.GetTop() - 10;	
	dc.GetTextExtent(strTextBottom, &w, &h);	
	
	w= w+4;
	int nLeft =  nX;
	if(nX + w > rtClient.GetRight())
		nLeft = nX -w;
	int nRectTop  = rtClient.GetBottom() - h;
	wxBrush brush(m_clTipsBk);
	dc.SetBrush(brush);
	wxPen penBorder(wxColour(m_clRedLine), 1, wxSOLID);
	dc.SetPen(penBorder);
	dc.DrawRectangle(nLeft, nRectTop, w, h);


	dc.SetTextForeground(m_clCurSelLine); 
	pt.x = nLeft + 2;
	pt.y = nRectTop; 
	dc.DrawText(strTextBottom, pt);

}
void   CKCtrl::SItemGetValue(bool	bIsMouse, bool bIsKeyBoard)
{
	m_sItem.nBegin		= m_nBegin;
	m_sItem.nEnd		= m_nEnd;
	m_sItem.nItem		= m_nCurSel;
	m_sItem.nDayWidth	= m_nDayWidth;
	m_sItem.nDaySpan	= m_nDaySpan;
	m_sItem.nShowCount  = m_nShowCount;

	if(m_nCurSel >= 0 && m_nCurSel < (int)m_vecKLine.size())
	{
		m_sItem.pItem = m_vecKLine[m_nCurSel];
	}
	else
		m_sItem.pItem  = NULL; 

	m_sItem.pMsg = NULL;

	m_sItem.bIsMouse		= bIsMouse;
	m_sItem.bIsKeyBoard	= bIsKeyBoard;
	m_sItem.wxPt		= m_ptMouseMoveLast;

}
bool  CKCtrl::GetShowCount(int  nMax)
{
	int nSize = m_vecKLine.size();
	if(nSize == 0)
		return false;
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop());
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop());
	int nWith = rtClient.GetWidth();

	if(nWith <=0 )
		return false;
	int nShowCount = m_nShowCount;
	if(nMax == -1)
	{
		if(nShowCount> nWith)
			return false;
		nShowCount = m_nShowCount *1.5 + 0.5;
	}
	else if(nMax == 1)
	{//放大
		if(nShowCount< 11)
			return false;
		nShowCount = m_nShowCount*2/3;
		if(m_nShowCount <11)
			nShowCount = 10;
	}

	if(nShowCount > nWith)
		nShowCount = nWith;
	//if(nShowCount == m_nShowCount)
	//	return false;

	m_nShowCount = nShowCount;
	int nOneDay = nWith/nShowCount;
	m_nDaySpan  = nOneDay *1/4;
	m_nDayWidth = nOneDay - m_nDaySpan;

	return true;
}
void CKCtrl::OnChar(int nkeyCode)
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
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;
	
			bool bShowChange = GetShowCount(1);
			if(!bShowChange)
				return;
			GetBeginEnd();

			CalcMaxMin();

			int nLeft =0, nRight =0;
			XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
			{
				SKLine *pKLine = m_vecKLine[m_nCurSel];
				float fClosePrice = pKLine->fClosePrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = wxPoint((nLeft+nRight)/2, nHeight);
			}
			SItemGetValue(false, true);

			Refresh(false);

			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);


			
			return;
		}
		break;
	case VK_DOWN:
		{
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;
		
			bool bShowChange = GetShowCount(-1);
			if(!bShowChange)
				return;

			GetBeginEnd();
			CalcMaxMin();
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
			{
				SKLine *pKLine = m_vecKLine[m_nCurSel];
				float fClosePrice = pKLine->fClosePrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = wxPoint((nLeft+nRight)/2, nHeight);
			}
			SItemGetValue(false, true);

			Refresh(false);

			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);
			
			return;
		}
		break;
	case VK_LEFT:
		{
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;
		//	if(m_timer1.IsRunning())
		//		m_timer1.Stop();
			
		//	m_timer1.Start(100);
	
			if(m_nCurSel== m_vecKLine.size()-1)
			{
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
				if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
				{
					SKLine *pKLine = m_vecKLine[m_nCurSel];
					float fClosePrice = pKLine->fClosePrice;
					int nHeight = JiaGe2GaoDu(fClosePrice);				
					m_ptMouseMoveLast = wxPoint((nLeft+nRight)/2, nHeight);
				}
				SItemGetValue(false, true);
				wxWindow* window= GetParent();
				wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
				myEvent.SetInt(nkeyCode);
				myEvent.SetClientData((void*)&m_sItem);
				window->ProcessEvent(myEvent);
				return ;
			}
			m_nCurSel  += m_nLeftPos;
			if(m_nCurSel > (int)m_vecKLine.size()-1)
				m_nCurSel = m_vecKLine.size()-1;
			if(m_nCurSel > m_nBegin)
			{
				m_bMaxMinChange = TRUE;
				int nCount = m_nBegin - m_nEnd;
				m_nBegin = m_nCurSel;
				m_nEnd   = m_nBegin - nCount;				
			}
			else if(m_nCurSel < m_nEnd)
			{
				m_bMaxMinChange = TRUE;
				m_nCurSel = m_nEnd;
			}
			CalcMaxMin();
		//	m_bShowOneItemData = true;
			m_bShowCross = true;
			Refresh(false);

		//	m_nLeftPos++;
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
			{				
				SKLine *pKLine = m_vecKLine[m_nCurSel];
				float fClosePrice = pKLine->fClosePrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = wxPoint((nLeft+nRight)/2, nHeight);
			}
			SItemGetValue(false, true);

			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);

			return ;
		}
		break;
	case VK_RIGHT:
		{
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;
		//	if(m_timer2.IsRunning())
		//		m_timer2.Stop();

		//	m_timer2.Start(100);

			if(m_nCurSel==0)
			{
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
				if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
				{
					SKLine *pKLine = m_vecKLine[m_nCurSel];
					float fClosePrice = pKLine->fClosePrice;
					int nHeight = JiaGe2GaoDu(fClosePrice);				
					m_ptMouseMoveLast = wxPoint((nLeft+nRight)/2, nHeight);
				}
				SItemGetValue(false, true);
				wxWindow* window= GetParent();
				wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
				myEvent.SetInt(nkeyCode);
				myEvent.SetClientData((void*)&m_sItem);
				window->ProcessEvent(myEvent);
				return;
			}
			m_nCurSel  -= m_nRightPos;
			if(m_nCurSel < 0)
				m_nCurSel = 0;
			if(m_nCurSel < m_nEnd)
			{
				m_bMaxMinChange = TRUE;
				int nCount = m_nBegin - m_nEnd;				
				m_nEnd  = m_nCurSel;
				m_nBegin = m_nEnd + nCount;
			}
			else if(m_nCurSel > m_nBegin)
			{
				m_bMaxMinChange = TRUE;
				m_nCurSel = m_nBegin;
			}
			CalcMaxMin();
			m_bShowCross = true;
			Refresh(false);

		//	m_nRightPos++;
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
			{
				SKLine *pKLine = m_vecKLine[m_nCurSel];
				float fClosePrice = pKLine->fClosePrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = wxPoint((nLeft+nRight)/2, nHeight);
			}
			SItemGetValue(false, true);

			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);
		
			return ;
		}
		break;
	case VK_PRIOR:
		{

		}
		break;
	case VK_NEXT:
		{

		}
		break;
	case VK_DELETE:
		{
			MAPsmark::iterator it = m_mapMarks.begin();
			while(it != m_mapMarks.end())
			{
				MAPsmark::iterator itTemp = it;
				it++;

				EnumMark enumMark	= (EnumMark)itTemp->first;
				SMark	 *pTemp		= itTemp->second;
				for(vector<SPoint *>::iterator it = pTemp->vecPt.begin(); it != pTemp->vecPt.end(); )
				{
					SPoint *p = *it;
					if(p->bSelected)
					{
						delete p;
						it = pTemp->vecPt.erase(it);
						Refresh(false);
						wxWindow* window= GetParent();
						wxCommandEvent myEvent(wxEVT_KCtrl_MARKSCHANGE);
						myEvent.SetClientData(&m_mapMarks);
						myEvent.SetExtraLong(reinterpret_cast<long>(this));
						window->ProcessEvent(myEvent);
					}
					else 
						it++;
				}
			}
		}
		break;
	default:
		break;
	}

	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
	myEvent.SetInt(nkeyCode);
	if(m_pMsgWnd)
		m_pMsgWnd->ProcessEvent(myEvent);
	else
		window->ProcessEvent(myEvent);
}
void CKCtrl::OnTimer1(wxTimerEvent& event)
{
	int nID = event.GetId();
	if(nID == 1111 )
	{
		m_nLeftPos  = 1;
	}
	if(nID == 1112 )
	{
		m_nRightPos = 1;		
		return;
	}
}
void CKCtrl::OnLeftDown(wxMouseEvent& event)
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
void CKCtrl::OnLeftUp(wxMouseEvent& event)
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
void CKCtrl::OnRightDown(wxMouseEvent& event)
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

}
void CKCtrl::OnRightUp(wxMouseEvent& event)
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
	if(m_pMsgWnd)
		m_pMsgWnd->ProcessEvent(myEvent);
	else
		window->ProcessEvent(myEvent);
}
void CKCtrl::OnMouseMove(wxMouseEvent &event)
{
	m_bIsMouse		= true;
	m_bIsKeyBoard	= false;

	m_ptMouseMoveLast = event.GetPosition();
	int nCursel= WeiZhi2XuHao(m_ptMouseMoveLast.x);
	if(nCursel < m_nEnd || nCursel > m_nBegin)
		return;

	m_nCurSel = nCursel;
	event.Skip();	
}

void CKCtrl::OnSize(wxSizeEvent& event)
{
	m_bMaxMinChange = TRUE;

	
	GetShowCount(0);
	GetBeginEnd();
	CalcMaxMin();
	Refresh(false);
}


void CKCtrl::timeToSting(time_t time1, char *szTime)
{
	struct tm tm1;
#ifdef WIN32
	tm1 = *localtime(&time1);
#else
	localtime_r(time1, tm1 );
#endif
	sprintf( szTime, "%4.4d/%2.2d/%2.2d",
		tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday
		);
}

void CKCtrl::SetCursel(SItem& sItem)
{
	if(m_nBegin  != sItem.nBegin
		||m_nEnd    != sItem.nEnd	  
		||m_nDayWidth != sItem.nDayWidth
		||m_nDaySpan != sItem.nDaySpan)
		m_bMaxMinChange = TRUE;


	if(sItem.nItem >=0 && sItem.nItem < (int)m_vecKLine.size())
		m_nCurSel = sItem.nItem;

	m_nBegin  = sItem.nBegin;
	m_nEnd    = sItem.nEnd;
	
	m_nDayWidth = sItem.nDayWidth;
	m_nDaySpan = sItem.nDaySpan;
	m_nShowCount = sItem.nShowCount;
	m_bIsMouse		= sItem.bIsMouse;
	m_bIsKeyBoard	= sItem.bIsKeyBoard;

	CalcMaxMin();		
	m_ptMouseMoveLast.x = sItem.wxPt.x;
	m_ptMouseMoveLast.y = -1;
	
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);
}
void CKCtrl::DrawOneItemData(wxDC& dc,  wxRect rtClient)
{	
	wxRect rtAll = GetClientRect();
	int nHeightMax = rtAll.GetHeight()-2;

	int nHeight = rtClient.GetHeight()-2;
	wxBrush brush(m_clBackGround);	
	dc.SetBrush(brush);
	
	if(nHeight > nHeightMax)
		nHeight = nHeightMax;
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
	dc.SetTextForeground(m_LeftpanelName); 
	dc.DrawText(strText, pt);
	nTop = rtClient.GetTop() + 1 + h;

	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[m_nCurSel];
		GetTimeText(m_enumPhrase, strText, strText2);
		dc.SetTextForeground(m_LeftpanelTime); 
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
	dc.SetTextForeground(m_LeftpanelTime); 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 1;

	dc.GetTextExtent(strText2, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.SetTextForeground(m_LeftpanelTime); 
	dc.DrawText(strText2, pt);
	nTop = nTop + h + 1;


	//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + h*2 + 2 )
		return;

	strText = wxT("开    盘");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_LeftpanelName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[m_nCurSel];
		//strText.Printf(_T("%.2f"), pKLine->fOpenPrice);
		strText = Double2String( pKLine->fOpenPrice, m_nNumDigits);
		SetTextColor(dc, pKLine->fOpenPrice);
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 1;

	if(nHeightMax <nTop + h*2 + 2 )
		return;
	strText = wxT("最    高");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_LeftpanelName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[m_nCurSel];
	//	strText.Printf(_T("%.2f"), pKLine->fHighPrice);
		strText = Double2String( pKLine->fHighPrice, m_nNumDigits);
		SetTextColor(dc, pKLine->fHighPrice);
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 1;

	if(nHeightMax <nTop + h*2 + 2 )
		return;
	strText = wxT("最    低");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_LeftpanelName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[m_nCurSel];
	//	strText.Printf(_T("%.2f"), pKLine->fLowPrice);
		strText = Double2String( pKLine->fLowPrice, m_nNumDigits);
		SetTextColor(dc, pKLine->fLowPrice);
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 1;

	if(nHeightMax <nTop + h*2 + 2 )
		return;
	strText = wxT("收    盘");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_LeftpanelName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[m_nCurSel];
	//	strText.Printf(_T("%.2f"), pKLine->fClosePrice);
		strText = Double2String( pKLine->fClosePrice, m_nNumDigits);
		SetTextColor(dc, pKLine->fClosePrice);
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 1;

	if(nHeightMax <nTop + h*2 + 2 )
		return;
	strText = wxT("成 交 量");

	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_LeftpanelName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[m_nCurSel];
		strText.Printf(_T("%d"), pKLine->dwVolume);
		dc.SetTextForeground(m_LeftpanelTime); 
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 1;
	//////////////////////////////////////////////////////////////////////////

	if(nHeightMax <nTop + h*2 + 2 )
		return;
	strText = wxT("涨    跌");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_LeftpanelName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel <(int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[m_nCurSel];

		int nPrevious = m_nCurSel +1;
		if(nPrevious >=0 && nPrevious < (int)m_vecKLine.size())	
		{
			SKLine *pKLinePrevious = m_vecKLine[nPrevious];
			//strText.Printf(wxT("%.2f"), pKLine->fClosePrice - pKLinePrevious->fClosePrice);		
			strText = Double2String( pKLine->fClosePrice - pKLinePrevious->fClosePrice, m_nNumDigits, true);
			SetTextColor(dc, pKLine->fClosePrice);
		}
		else
			strText = wxT("----");		
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 1;


	//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + h*2 + 2 )
		return;
	strText = wxT("幅    度");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetLeft() + (rtClient.GetWidth() - w)/2;
	pt.y = nTop; 
	dc.SetTextForeground(m_LeftpanelName); 
	dc.DrawText(strText, pt);
	nTop = nTop + h;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine *pKLine = m_vecKLine[m_nCurSel];

		int nPrevious = m_nCurSel +1;
		if(nPrevious >=0 && nPrevious < (int)m_vecKLine.size())	
		{
			SKLine *pKLinePrevious = m_vecKLine[nPrevious];
			strText.Printf(wxT("%.2f%%"), 100*(pKLine->fClosePrice - pKLinePrevious->fClosePrice)/pKLinePrevious->fClosePrice);	
			SetTextColor(dc, pKLine->fClosePrice);
		}
		else
			strText = wxT("----");		
	}
	else 
		strText = wxT("----");
	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.GetRight() - w - 2;
	pt.y = nTop ; 
	dc.DrawText(strText, pt);
	nTop = nTop + h + 1;

}
void CKCtrl::SetTextColor(wxDC& dc, double dbClosePrice)
{
	double dbPrevious = 0.0;
	int nPrevious = m_nCurSel +1;
	if(nPrevious >=0 && nPrevious < (int)m_vecKLine.size())	
	{
		SKLine *pKLinePrevious = m_vecKLine[nPrevious];
		dbPrevious = pKLinePrevious->fClosePrice;	
	}
	
	if(dbClosePrice - dbPrevious > -0.00001 && dbClosePrice - dbPrevious < 0.00001)
	  	dc.SetTextForeground(m_Leftpanelhengpan); 
	else if(dbClosePrice > dbPrevious)
		dc.SetTextForeground(m_Leftpanelzhang); 
	else if(dbClosePrice < dbPrevious)
		dc.SetTextForeground(m_Leftpaneldie); 
}
void CKCtrl::GetTimeText(EnumPhrase	 enumPhrase, wxString& strText, wxString& strText2)
{
	if(m_nCurSel <0 || m_nCurSel >= (int)m_vecKLine.size())
		return;
   
	SKLine *pKLine = m_vecKLine[m_nCurSel];
	switch(m_enumPhrase)
	{
	case PHRASE_1MIN:
	case PHRASE_5MIN:		
	case PHRASE_15MIN:
	case PHRASE_30MIN:
	case PHRASE_60MIN:		
		strText.Printf(_T("%2.2d/%2.2d"),  pKLine->dwTime.tm_mon+1, pKLine->dwTime.tm_mday);
		strText2.Printf(_T("%2.2d:%2.2d"), pKLine->dwTime.tm_hour, pKLine->dwTime.tm_min);

		break;
	case PHRASE_DAY:		
	case PHRASE_WEEK:
	case PHRASE_MONTH:
		strText.Printf(_T("%4.4d"), pKLine->dwTime.tm_year+1900);
		strText2.Printf(_T("%2.2d/%2.2d"), pKLine->dwTime.tm_mon+1, pKLine->dwTime.tm_mday );
		break;
	}
}
void CKCtrl::SetMarkType(EnumMark enumMark)
{
//	SetCapture((HWND)m_hWnd);
	m_enumMark = enumMark;
	if(MARK_UNDEFINE != m_enumMark)
		::SetCursor(LoadCursor(NULL,IDC_HAND)); 		
}
void CKCtrl::DrawMarks(wxDC& dc)
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

/*
   /\
  / .\  pt这个点位于三角形的中间
 /____\
*/
void CKCtrl::DrawTriangleUp(wxDC& dc, struct tm tm1, double dbPrice, bool bSelected)
{

	int nLeft =0, nRight =0;
	if(-1== time2WeiZhi(tm1, nLeft, nRight))
		return;
	int x =	(nLeft+nRight)/2;
    int y = JiaGe2GaoDu(dbPrice);

	wxPoint points[3];
	points[0]= wxPoint(x, y - 3);
	points[1]= wxPoint(x - 3,y + 3);
	points[2]= wxPoint(x + 3, y + 3);
	wxPen penBorder(wxColour(m_clTrangleUp), 1, wxSOLID);
	dc.SetPen(penBorder);
	wxBrush brushHatch(wxColour(m_clTrangleUp), wxSOLID);
	dc.SetBrush(brushHatch);
	dc.DrawPolygon(WXSIZEOF(points), points, 0, 0, wxWINDING_RULE);
	
	if(bSelected)
	{
		wxPen penBorder(wxColour(m_clTrangleUpBorder), m_nBorderWidth, wxSOLID);
		dc.SetPen(penBorder);
		dc.DrawLine(x-5, y-5, x+5, y-5);
		dc.DrawLine(x+5, y-5, x+5, y+5);
		dc.DrawLine(x-5, y-5, x-5, y+5);
		dc.DrawLine(x-5, y+5, x+5, y+5);
	}

}
void CKCtrl::DrawTriangleDown(wxDC& dc, struct tm tm1, double dbPrice, bool bSelected)
{
	int nLeft =0, nRight =0;
	if(-1== time2WeiZhi(tm1, nLeft, nRight))
		return;
	int x =	(nLeft+nRight)/2;
	int y = JiaGe2GaoDu(dbPrice);

	wxPoint points[3];
	points[0]= wxPoint(x, y + 3);
	points[1]= wxPoint(x - 3,y - 3);
	points[2]= wxPoint(x + 3, y - 3);
	wxPen penBorder(wxColour(m_clTrangleDown), 1, wxSOLID);
	dc.SetPen(penBorder);
	wxBrush brushHatch(wxColour(m_clTrangleDown), wxSOLID);
	dc.SetBrush(brushHatch);
	dc.DrawPolygon(WXSIZEOF(points), points, 0, 0, wxWINDING_RULE);
	if(bSelected)
	{
		wxPen penBorder(wxColour(m_clTrangleDownBorder), m_nBorderWidth, wxSOLID);
		dc.SetPen(penBorder);
		dc.DrawLine(x-5, y-5, x+5, y-5);
		dc.DrawLine(x+5, y-5, x+5, y+5);
		dc.DrawLine(x-5, y-5, x-5, y+5);
		dc.DrawLine(x-5, y+5, x+5, y+5);		
	}
}
void CKCtrl::DoMouseMove(wxPoint mousePos)
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
		int nCurSel = WeiZhi2XuHao(mousePos.x);	
		if(nCurSel >= 0 && nCurSel < (int)m_vecKLine.size())
			m_nCurSel = nCurSel;
	}
	
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);

}
void CKCtrl::OnMouseLeave(wxMouseEvent &event)
{
	DoMouseLeave();
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KCtrl_MOUSELEAVE);
	myEvent.SetClientData(this);
	window->ProcessEvent(myEvent);

	event.Skip();
}
void CKCtrl::DoMouseLeave()
{
	m_ptMouseMoveLast = wxPoint(-1,-1);
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);

	if(MARK_UNDEFINE != m_enumMark)
		::SetCursor(LoadCursor(NULL,IDC_ARROW)); 		

}
void CKCtrl::OnMouseEnter(wxMouseEvent &event)
{
	if(MARK_UNDEFINE != m_enumMark)
		::SetCursor(LoadCursor(NULL,IDC_HAND)); 
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KCtrl_MOUSEENTER);
	myEvent.SetClientData(this);
	window->ProcessEvent(myEvent);
}

BOOL CKCtrl::SetVectorData_InTime(vector<SKLine *>& vecKLine)
{
	if(vecKLine.size() == 0)
		return TRUE;
/*	if(m_vecKLine.size() < vecKLine.size())
	{
		SKLine *pKLine = vecKLine[0];
		m_vecKLine.insert(m_vecKLine.begin(), pKLine);	
		if(m_pFData)
			m_pFData->CalcKTechIndex(m_vecKLine, "", m_strTechIndexName.c_str(), m_enumPhrase, m_pTechIndex);
		GetBeginEnd();
		CalcMaxMin();
		Refresh(false);
		UpdateWindow((HWND)m_hWnd);
		return TRUE;
	}
	if(m_vecKLine.size() == 0)
		return FALSE;

	SKLine *pLast = m_vecKLine[0];
	SKLine *pKLine = vecKLine[0];
	struct tm tm1 = pLast->dwTime;
	struct tm tm2 = pKLine->dwTime;
	int dwTimeSpan = difftime(mktime(&tm1), mktime(&tm2));
	if(dwTimeSpan == 0)
	{
		*pLast = *pKLine; 
	}*/
	bool bRecalc = false;
	if(m_vecKLine.size() != vecKLine.size())
		bRecalc = true;
	m_vecKLine = vecKLine;
	if(m_pFData && bRecalc && m_strTechIndexName!= "")
		m_pFData->CalcKTechIndex(m_vecKLine, "", m_strTechIndexName.c_str(),m_enumPhrase, m_pTechIndex);

	GetShowCount(0);
	GetBeginEnd();
	CalcMaxMin();
	
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);
	return TRUE;
}
void CKCtrl::OnLeftDBClick(wxMouseEvent& event)
{
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KCtrl_LEFTDBCLICK);
	myEvent.SetClientData(this);
	window->ProcessEvent(myEvent);
}
WXLRESULT CKCtrl::MSWWindowProc(WXUINT   message,   WXWPARAM   wParam,   WXLPARAM   lParam) 
{//wxpanel不支持return escape，所以在这里处理onchar	
	switch (message) 
	{ 
	case WM_KEYDOWN: 			
		{
			OnChar((int)wParam);
		}
		break;
	default:
		break;
	} 
	return wxPanel::MSWWindowProc(message,wParam,lParam); 
}
bool CKCtrl::InitCfg(TiXmlElement *root)
{
	TiXmlElement *tList = root->FirstChildElement("kline");
	if(tList == NULL)
		return false;
	
	TiXmlNode *leftspanwidth = tList->FirstChild("leftspanwidth");
	if(leftspanwidth == NULL)
		return false;
	const char *strleftspanwidth = leftspanwidth->ToElement()->GetText();
	unsigned long valueSpan = ColorStr2Long(strleftspanwidth);
	SetLeftSpanWidth(valueSpan);

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


	ListItem = pColor->FirstChild("leftpanelname");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLeftpanelName(value);

	ListItem = pColor->FirstChild("leftpaneltime");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLeftpanelTime(value);


	ListItem = pColor->FirstChild("leftpanelzhang");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLeftpanelzhang(value);

	ListItem = pColor->FirstChild("leftpaneldie");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLeftpaneldie(value);

	ListItem = pColor->FirstChild("leftpanelhengpan");
	if(ListItem == NULL)
		return false;
	strValue = ListItem->ToElement()->GetText();
	value = ColorStr2Long(strValue);
	SetLeftpanelhengpan(value);

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
void CKCtrl::SetBackGround(unsigned long lValue)
{
	m_clBackGround = lValue;
}
void CKCtrl::SetBorder(unsigned long lValue)
{
	m_clBorder = lValue;
}
void CKCtrl::SetRedLine(unsigned long lValue)
{
	m_clRedLine = lValue;
}
void CKCtrl::SetGreenLine(unsigned long lValue)
{
	m_clGreenLine = lValue;
}
void CKCtrl::SetdashLine(unsigned long lValue)
{
	m_cldashLine = lValue;
}
void CKCtrl::SetCurSelLine(unsigned long lValue)
{
	m_clCurSelLine = lValue;
}
void CKCtrl::SetTipsBk(unsigned long lValue)
{
	m_clTipsBk = lValue;
}
void CKCtrl::SetLineName(unsigned long lValue)
{
	m_clLineName = lValue;
}
void CKCtrl::SetLeftpanelName(unsigned long lValue)
{
	m_LeftpanelName = lValue;
}
void CKCtrl::SetLeftpanelTime(unsigned long lValue)
{
	m_LeftpanelTime = lValue;
}
void CKCtrl::SetLeftpanelzhang(unsigned long lValue)
{
	m_Leftpanelzhang = lValue;
}
void CKCtrl::SetLeftpaneldie(unsigned long lValue)
{
	m_Leftpaneldie = lValue;
}
void CKCtrl::SetLeftpanelhengpan(unsigned long lValue)
{
	m_Leftpanelhengpan = lValue;
}
void CKCtrl::SetTrangleUp(unsigned long lValue)
{
	m_clTrangleUp = lValue;
}
void CKCtrl::SetTrangleDown(unsigned long lValue)
{
	m_clTrangleDown = lValue;
}
void CKCtrl::SetLineColor(int nIndex, unsigned long lValue)
{
	if(nIndex <0 || nIndex >=COLOR_MAX)
		return;
	m_clLineColor[nIndex] = lValue;
}/*
void CKCtrl::SetShowOneItemData(bool bShowOneItemData)
{
	if(m_bShowOneItemData == bShowOneItemData)
		return;
	m_bShowOneItemData = bShowOneItemData;
//	Refresh(false);
}*/
void CKCtrl::DrawTechIndex(wxDC& dc)
{
	if(m_pTechIndex == NULL)
		return;
	if(m_pTechIndex->m_nVecDataCount == 0 )
		return;	
	
	wxRect rtClient = GetClientRect();
	rtClient.SetLeft(rtClient.GetLeft() + m_RectOffSet.GetLeft());
	rtClient.SetTop(rtClient.GetTop() + m_RectOffSet.GetTop()-1);
	rtClient.SetRight(rtClient.GetRight() - m_RectOffSet.GetWidth() -m_RectOffSet.GetLeft());
	rtClient.SetBottom(rtClient.GetBottom() - m_RectOffSet.GetHeight()- m_RectOffSet.GetTop()+1);

	
	dc.SetClippingRegion(rtClient);
	for(int k =0; k < m_pTechIndex->m_nDrawBody; k++)
	{
		if(m_pTechIndex->m_vecData == NULL)
		{
			dc.DestroyClippingRegion();
			return;
		}
		if( m_pTechIndex->m_vecData[k].size() == 0)
			continue;
		for(int i = m_nBegin; i> m_nEnd; i--)
		{
			if(i>= (int)m_pTechIndex->m_vecData[k].size() )
				continue;
			int nLeftBegin =0, nRightBegin =0, nLeftEnd = 0, nRightEnd = 0;
			XuHao2WeiZhi(i, nLeftBegin, nRightBegin);
			XuHao2WeiZhi(i-1, nLeftEnd, nRightEnd);
			if((m_pTechIndex->m_vecData[k][i].fValue - MIN_VALUE_DEFAULT > -0.00001 && m_pTechIndex->m_vecData[k][i].fValue - MIN_VALUE_DEFAULT < 0.00001) 
				|| (m_pTechIndex->m_vecData[k][i-1].fValue - MIN_VALUE_DEFAULT > -0.00001 && m_pTechIndex->m_vecData[k][i-1].fValue - MIN_VALUE_DEFAULT < 0.00001) )
				continue;
			int nBegin = JiaGe2GaoDu(m_pTechIndex->m_vecData[k][i].fValue);
			int nEnd   = JiaGe2GaoDu(m_pTechIndex->m_vecData[k][i-1].fValue);
			wxPen penBorder(wxColour(m_clLineColor[k+1]), m_nBorderWidth, wxSOLID);
			dc.SetPen(penBorder);
			dc.DrawLine((nLeftBegin+nRightBegin)/2, nBegin, (nLeftEnd+nRightEnd)/2, nEnd);
		}
	}
	dc.DestroyClippingRegion();
}
void CKCtrl::OnConfigChange(wxCommandEvent& event)
{
	CInitConfig& cInitConfig = *(CInitConfig*)event.GetClientData();
	vector<int> vecCofig;
	for(int i=0; i<MAX_CONFIG_ITEMS; i++)
	{
		if(cInitConfig.bConfigShow[i])
			vecCofig.push_back(cInitConfig.nConfig[i]);		
	}
	if(m_pFData)
		m_pFData->CalcKTechIndex(m_vecKLine,"", m_strTechIndexName.c_str(), m_enumPhrase, m_pTechIndex, &vecCofig);

	CalcMaxMin();
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);

}
void CKCtrl::SetTechIndex(wxString strTechIndex)
{
	if(m_strTechIndexName == strTechIndex)
		return;

/*有左边刻度规定的指标的话，以后再考虑把，目前至少没有这样的需求，到时候再参考KtechIndex模块
m_vecLeftScales.clear();
	if(strTechIndex == KDJ)
	{
		m_vecLeftScales.push_back(0.0);
		m_vecLeftScales.push_back(20.0);
		m_vecLeftScales.push_back(50.0);
		m_vecLeftScales.push_back(80.0);
		m_vecLeftScales.push_back(100.0);
	}*/
	m_strTechIndexName = strTechIndex;
	if(m_pFData)
		m_pFData->CalcKTechIndex(m_vecKLine,"", m_strTechIndexName.c_str(), m_enumPhrase, m_pTechIndex, NULL);
	CalcMaxMin();
	Refresh(false);
	UpdateWindow((HWND)m_hWnd);
}
void CKCtrl::SetShowCross(bool bShowCross)
{
	m_bShowCross = bShowCross;
}
void CKCtrl::SetMouseIn(bool bMouseIn)
{
	m_bMouseIn = bMouseIn;
}
void CKCtrl::SetLeftSpanWidth(int nWidth)
{
	m_RectOffSet.SetLeft(nWidth);
}
void CKCtrl::SetKType(int nKType)
{ 
	m_KType = nKType; 
}