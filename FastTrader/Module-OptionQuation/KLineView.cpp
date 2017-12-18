#include "stdafx.h"
#include "OptionQuation.h"
#include "KLineView.h"


CKLineView::CKLineView(void)
{
	m_ExchangeName  = "";
	m_ContractName  = "";

	m_clBackGround	= RGB(0,0,0);
	m_clBackGround	= RGB(0,0,0);
	m_clBorder		= RGB(192,0,0);
	m_clRedLine		= RGB(240,0,0);
	m_clGreenLine	= RGB(0,255,255);
	m_cldashLine	= RGB(76,40,40);
	m_clCurSelLine  = RGB(200,200,200);
	m_clTipsBk		= RGB(0,0,128);
	m_clLineName    = RGB(192,192,192);
	m_clSAR			= RGB(255,255,255);
	m_clTriangle    = RGB(255,255,0);
	m_clText		= RGB(192,0,0);

	m_nBorderWidth	= 1;
	m_nShowCount	= 100;
	GetShowCount(m_nShowCount);

	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue = MAX_VALUE_DEFAULT;

	m_nBegin = 0;
	m_nEnd   = 0;
	m_RectOffSet =  CRect(66,16,0,5);
	m_nCurSel  = -1;

	m_nLeftPos  = 1;
	m_nRightPos = 1;

	m_uTimerarr[0] = -1;
	m_uTimerarr[1] = -1;
	m_bMaxMinChange		= TRUE;




	m_bIsMouse		= false;
	m_bIsKeyBoard	= false;
	m_bShowHeight = true;
	m_bShowTime  = true;


	m_LeftpanelName     = RGB(255, 255, 255);
	m_LeftpanelTime		= RGB(82, 255, 255);
	m_Leftpanelzhang	= RGB(255, 82, 82);
	m_Leftpaneldie		= RGB(82, 255, 82);
	m_Leftpanelhengpan	= RGB(255, 255, 255);
	m_nLeftPanelHeight = 300;//左侧价格提示窗高度


/*
	m_enumMark = MARK_UNDEFINE;

	m_clTrangleUp			= RGB(255, 0, 0);
	m_clTrangleDown			= RGB(0, 128, 0);
	m_clTrangleUpBorder		= RGB(255, 255, 255);
	m_clTrangleDownBorder	= RGB(255, 255, 255);
*/
	m_ptMouseMoveLast = CPoint(-1,-1);

	//	m_bShowOneItemData = true;
	m_bShowCross	   = true;
	m_bMouseIn		   = true;


	m_nNumDigits = 2;

	m_nKViewCount	= 2;
	
	m_rtKLine		= CRect(0,0,0,0);
	m_rtKVolume		= CRect(0,0,0,0);
	m_rtScales		= CRect(0,0,0,0);

	//////////////////////////////////////////////////////////////////////////
	m_fMaxValue_KVolume  = MIN_VALUE_DEFAULT;
	m_fMinValue_KVolume  = 0;

	m_LbuttonDownPoint = CPoint(0, 0);
	m_dbKLine		   = 1.0;
	m_dbKVolume		   = 0.5;
	m_nResizeRange     = 2;
	m_nMaxSpan		   = 15;
	m_bLbuttonDown		= false;



	LOGFONT lf;
	memset(&lf,0,sizeof(lf));
	lf.lfHeight = 13;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfWeight = FW_NORMAL;
	lf.lfQuality = PROOF_QUALITY;
	strcpy(lf.lfFaceName,_T("Tahoma"));
	lf.lfHeight = 13;
	m_hSysFontHead = ::CreateFontIndirect(&lf);

	if(!m_FontHead.m_hObject)
	{
		CFont font;
		LOGFONT lf;
		HFONT hSysFont; 
		hSysFont = m_hSysFontHead;
		font.Attach( hSysFont );
		font.GetLogFont(&lf);
		font.Detach();
		m_FontHead.CreateFontIndirect(&lf);
	}

	lf.lfHeight = 14;
	lf.lfItalic = FALSE;
	lf.lfWeight = FW_BOLD;
	m_hSysFontTip = ::CreateFontIndirect(&lf);
	if(!m_FontTip.m_hObject)
	{
		CFont font;
		LOGFONT lf;
		HFONT hSysFont; 
		hSysFont = m_hSysFontTip;
		font.Attach( hSysFont );
		font.GetLogFont(&lf);
		font.Detach();
		m_FontTip.CreateFontIndirect(&lf);
	}
	lf.lfHeight = 13;
	lf.lfItalic = FALSE;
	lf.lfWeight = FW_NORMAL;
	m_hSysFontLeft = ::CreateFontIndirect(&lf);
	if(!m_FontLeft.m_hObject)
	{
		CFont font;
		LOGFONT lf;
		HFONT hSysFont; 
		hSysFont = m_hSysFontLeft;
		font.Attach( hSysFont );
		font.GetLogFont(&lf);
		font.Detach();
		m_FontLeft.CreateFontIndirect(&lf);
	}

	lf.lfHeight = 15;
	lf.lfItalic = FALSE;
	lf.lfWeight |= FW_BOLD;
	m_hSysFont1 = ::CreateFontIndirect(&lf);
	if(!m_FontScales.m_hObject)
	{
		CFont font;
		LOGFONT lf;
		HFONT hSysFont; 
		hSysFont = m_hSysFont1;
		font.Attach( hSysFont );
		font.GetLogFont(&lf);
		font.Detach();
		m_FontScales.CreateFontIndirect(&lf);
	}

	lf.lfHeight = 15;
	lf.lfItalic = FALSE;
	lf.lfWeight |= FW_BOLD;
	m_hSysFontDownScalesLeft = ::CreateFontIndirect(&lf);
	if(!m_FontDownScalesLeft.m_hObject)
	{
		CFont font;
		LOGFONT lf;
		HFONT hSysFont; 
		hSysFont = m_hSysFontDownScalesLeft;
		font.Attach( hSysFont );
		font.GetLogFont(&lf);
		font.Detach();
		m_FontDownScalesLeft.CreateFontIndirect(&lf);
	}
	m_fMaxValue_KVolume = MIN_VALUE_DEFAULT;
	m_fMinValue_KVolume = MAX_VALUE_DEFAULT;

	m_strTechIndexName[0] = MA;
	m_strTechIndexName[1] = VOL;
	//m_strTechIndexName[2] = KDJ;

	m_clLineColor[0] = RGB(200, 200, 200);//白
	m_clLineColor[1] = RGB(255, 255, 255);//白	
	m_clLineColor[2] = RGB(74, 203, 76);  //绿
	m_clLineColor[3] = RGB(181, 52, 179); //粉
	m_clLineColor[4] = RGB(241, 240, 113);//黄
	m_clLineColor[5] = RGB(124, 192, 255);//蓝
	m_clLineColor[6] = RGB(100, 100, 100);//大红
	m_clLineColor[7] = RGB(194, 106, 105);//
}

CKLineView::~CKLineView(void)
{
	if(m_FontHead.m_hObject) 
		m_FontHead.DeleteObject(); 
	if(m_hSysFontHead)
	{
		::DeleteObject((HGDIOBJ)(m_hSysFontHead)); 
		m_hSysFontHead = NULL; 
	}

	if(m_FontScales.m_hObject) 
		m_FontScales.DeleteObject(); 
	if(m_hSysFont1)
	{
		::DeleteObject((HGDIOBJ)(m_hSysFont1)); 
		m_hSysFont1 = NULL; 
	}

	if(m_FontTip.m_hObject) 
		m_FontTip.DeleteObject(); 
	if(m_hSysFontTip)
	{
		::DeleteObject((HGDIOBJ)(m_hSysFontTip)); 
		m_hSysFontTip = NULL; 
	}
}
// CKLineView

IMPLEMENT_DYNAMIC(CKLineView, CWnd)


BEGIN_MESSAGE_MAP(CKLineView, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
//	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDD_MENU_TOKLINE, OnToKLine)
	ON_COMMAND(IDD_MENU_K1, OnToK1)
	ON_COMMAND(IDD_MENU_K5, OnToK5)
	ON_COMMAND(IDD_MENU_K15, OnToK15)
	ON_COMMAND(IDD_MENU_K30, OnToK30)
	ON_COMMAND(IDD_MENU_K60, OnToK60)
	ON_COMMAND(IDD_MENU_KDAY, OnToKDay)
	ON_COMMAND(IDD_MENU_KWEEK, OnToKWeek)
	ON_COMMAND(IDD_MENU_KMONTH, OnToKMonth)
	ON_COMMAND(IDD_MENU_INDEXEXPRESS, OnIndexExpress)
	ON_COMMAND(IDD_MENU_INDEXCONFIG, OnIndexConfig)
END_MESSAGE_MAP()



// CKLineView message handlers

BOOL CKLineView::Create(CRect rtClient, CWnd* pParentWnd, UINT nID)
{
	return	CWnd::Create(NULL,  NULL, WS_CHILD|WS_VISIBLE, rtClient, pParentWnd, nID, NULL);
}
void CKLineView::SetFuture(string ExchangeName,string ContractName)
{
	m_ExchangeName = ExchangeName;
	m_ContractName = ContractName;	
}
void CKLineView::SetPhrase(EnumPhrase enumPhrase)
{
	if(m_enumPhrase != enumPhrase)
	{
		m_nCurSel = 0;
	}
	m_enumPhrase   = enumPhrase;	
}
void CKLineView::OnPaint() 
{
	CPaintDC dc(this); 

	CRect rect;
	GetClientRect(rect);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	if(m_Bmp.m_hObject != NULL)
		m_Bmp.DeleteObject();
	m_Bmp.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	CBitmap* OldBmp = dcMem.SelectObject(&m_Bmp);
	DrawBitmap(&dcMem);
	DrawBitmap_Volume(&dcMem);
	DrawBitmap_Scales(&dcMem);
	DrawCommonLast(&dcMem);

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(OldBmp);
	dcMem.DeleteDC();	

}
void CKLineView::DrawBitmap(CDC *pDC)
{
	CRect rtClient,rect;
	GetClientRect(&rtClient);		
	pDC->FillSolidRect(rtClient, m_clBackGround);
	DrawScales(pDC);
	DrawBorder(pDC);
	DrawHead(pDC);
	DrawItems(pDC);
	DrawTechIndex(pDC);
	DrawTechIndex_Volume(pDC);
	DrawZuiGaoZuiDiJia(pDC);
	
}
void CKLineView::DrawTips(CDC *pDC)
{
	SHitTestKView hitTest;
	HitTest(m_ptMouseMoveLast, hitTest);
	if(hitTest.eType == enumKSplitBar)
		return;

	CRect rtClient;
	rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	CPen penBorder(PS_SOLID,1,m_clCurSelLine);
	CPen *oldPen = pDC->SelectObject(&penBorder);

	if(m_ptMouseMoveLast.x >= rtClient.left	&& m_ptMouseMoveLast.x <= rtClient.right)
	{		
		if(m_bShowTime)
		{			
			DrawTips_Time(pDC, m_rtScales, m_ptMouseMoveLast.x);
		}
	}
	if(m_ptMouseMoveLast.y >= m_rtKLine.top && m_ptMouseMoveLast.y <= m_rtKLine.bottom)
	{
		if(m_bShowHeight)
		{
			if((m_fMaxValue- MIN_VALUE_DEFAULT > -0.00001 && m_fMaxValue- MIN_VALUE_DEFAULT< 0.00001)
				||(m_fMinValue- MAX_VALUE_DEFAULT > -0.00001 && m_fMaxValue- MAX_VALUE_DEFAULT< 0.00001))
			{
				pDC->SelectObject(oldPen);
				return;
			}
			  
			double dbPrice = 0.0;
			if(m_bIsKeyBoard)
			{
				if(m_nCurSel < 0 || m_nCurSel >= (int)m_vecKLine.size())
				{
					pDC->SelectObject(oldPen);
					return;
				}
				SKLine pKLine = m_vecKLine[m_nCurSel];
				dbPrice = pKLine.fClosePrice;
			}
			if(m_bIsMouse)
				dbPrice = GaoDu2JiaGe(m_ptMouseMoveLast.y);

			CFont *pOldFont=pDC->SelectObject(&m_FontTip);

			CSize cz;
			int nTop = rtClient.top - 10;	

			CString strText = Double2String_Tip(dbPrice);
			cz = pDC->GetTextExtent(strText);	

			CString strLength = _T("1234.56");
			cz = pDC->GetTextExtent(strLength);	
			cz.cx = cz.cx + 4;

			int nLeft =  rtClient.right - cz.cx;
			int nRectTop  = m_ptMouseMoveLast.y - cz.cy-1;
			if(nRectTop <= 0)
				nRectTop = 0;

			CRect rc(nLeft, nRectTop, nLeft + cz.cx, nRectTop+cz.cy);
			rc.InflateRect(1,1,0,1);

			CBrush brush(m_clTipsBk);
			pDC->FillRect(rc, &brush);
			CBrush brush2(m_clRedLine);				
			pDC->FrameRect(rc, &brush2);

			pDC->SetTextColor(m_clCurSelLine); 

			CSize sz = pDC->GetTextExtent(strText);
			CRect rt;
			rt.left   = nLeft;
			rt.top    = nRectTop;
			rt.right  = rt.left + sz.cx;
			rt.bottom = rt.top + sz.cy;
			pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

			nLeft		=  rtClient.left - sz.cx - 2;		
			CRect rt2   = rt;
			rt2.left    = nLeft;
			rt2.right   = rt2.left + sz.cx;
			
			rt2.InflateRect(2,1,0,1);
			pDC->FillRect(rt2, &brush);				
			pDC->FrameRect(rt2, &brush2);
			rt2.left += 1;
			rt2.top += 1;
			pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

			pDC->SelectObject(pOldFont);
		}
	}
	if(m_ptMouseMoveLast.y >= m_rtKVolume.top && m_ptMouseMoveLast.y <= m_rtKVolume.bottom)
	{
		if(m_bShowHeight)
		{
			if((m_fMaxValue_KVolume- MIN_VALUE_DEFAULT > -0.00001 && m_fMaxValue_KVolume- MIN_VALUE_DEFAULT< 0.00001)
				||(m_fMinValue_KVolume- MAX_VALUE_DEFAULT > -0.00001 && m_fMinValue_KVolume- MAX_VALUE_DEFAULT< 0.00001))
			{
				pDC->SelectObject(oldPen);
				return;
			}

			double dbPrice = 0.0;
			if(m_bIsKeyBoard)
			{
				if(m_nCurSel < 0 || m_nCurSel >= (int)m_vecKLine.size())
				{
					pDC->SelectObject(oldPen);
					return;
				}
				pDC->SelectObject(oldPen);
				return;
			}
			if(m_bIsMouse)
				dbPrice = GaoDu2JiaGe_Volume(m_ptMouseMoveLast.y);

			CFont *pOldFont=pDC->SelectObject(&m_FontTip);

			CSize cz;
			int nTop = rtClient.top - 10;	

			CString strText = Double2String_Tip(dbPrice);
			cz = pDC->GetTextExtent(strText);	

			CString strLength = _T("1234.56");
			cz = pDC->GetTextExtent(strLength);	
			cz.cx = cz.cx + 4;

			int nLeft =  rtClient.right - cz.cx;
			int nRectTop  = m_ptMouseMoveLast.y - cz.cy-1;
			if(nRectTop <= 0)
				nRectTop = 0;

			CRect rc(nLeft, nRectTop, nLeft + cz.cx, nRectTop+cz.cy);
			rc.InflateRect(0,1,0,1);

			CBrush brush(m_clTipsBk);
			pDC->FillRect(rc, &brush);
			CBrush brush2(m_clRedLine);				
			pDC->FrameRect(rc, &brush2);

			pDC->SetTextColor(m_clCurSelLine); 

			CSize sz = pDC->GetTextExtent(strText);
			CRect rt;
			rt.left   = nLeft+2;
			rt.top    = nRectTop;
			rt.right  = rt.left + sz.cx;
			rt.bottom = rt.top + sz.cy;
			pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

			nLeft		=  rtClient.left - sz.cx - 2;		
			CRect rt2   = rt;
			rt2.left    = nLeft;
			rt2.right   = rt2.left + sz.cx;

			rt2.InflateRect(2,1,0,1);
			pDC->FillRect(rt2, &brush);				
			pDC->FrameRect(rt2, &brush2);
			rt2.left += 1;
			rt2.top += 1;
			pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

			pDC->SelectObject(pOldFont);
		}
	}
	pDC->SelectObject(oldPen);
}
void CKLineView::DrawTips_Time(CDC* pDC, CRect rtClient, int nX)
{
	if(m_nCurSel< m_nEnd || m_nCurSel > m_nBegin||m_nCurSel < 0 || m_nCurSel >= (int)m_vecKLine.size())
		return;

	SKLine pKLine;
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

	CString strTextBottom;
	switch(m_enumPhrase)
	{		
	case PHRASE_1MIN:
	case PHRASE_5MIN:
	case PHRASE_15MIN:
	case PHRASE_30MIN:
	case PHRASE_60MIN:
		strTextBottom.Format(_T("%2.2d/%2.2d,%2.2d:%2.2d"), pKLine.dwTime.tm_mon+1, pKLine.dwTime.tm_mday, pKLine.dwTime.tm_hour, pKLine.dwTime.tm_min);
		break;
	case PHRASE_DAY:
	case PHRASE_WEEK:
	case PHRASE_MONTH:
		{
			COleDateTime dateBegin(pKLine.dwTime.tm_year+1900, pKLine.dwTime.tm_mon+1, pKLine.dwTime.tm_mday, pKLine.dwTime.tm_hour, pKLine.dwTime.tm_min, pKLine.dwTime.tm_sec);
			int nDay = dateBegin.GetDayOfWeek();
			CString strText = GetWeekName(nDay);
			strTextBottom.Format(_T("%4.4d/%2.2d/%2.2d(%s)"), pKLine.dwTime.tm_year+1900, pKLine.dwTime.tm_mon+1, pKLine.dwTime.tm_mday, strText);
			if (strTextBottom.GetLength() < 10)
				return;
		}
		break;
	}	
	CFont *pOldFont=pDC->SelectObject(&m_FontTip);

	CSize cz;
	CPoint pt;
	int nTop = rtClient.top;	
	cz = pDC->GetTextExtent(strTextBottom);	

	cz.cx = cz.cx + 4;
	int nLeft =  nX;
	if(nX + cz.cx > rtClient.right)
		nLeft = nX -cz.cx+1;
	int nRectTop  = rtClient.bottom - cz.cy-2;
	
	CBrush brush(m_clTipsBk);	

	CPen penBorder(PS_SOLID,1,m_clRedLine);
	CPen *oldPen = pDC->SelectObject(&penBorder);
	
	pDC->SetTextColor(m_clRedLine); 
	CRect rt;
	rt.left   = nLeft;
	rt.top    = rtClient.top;
	rt.right  = rt.left + cz.cx + 2;
	rt.bottom = rtClient.bottom;
	CBrush brush2(m_clRedLine);		
	
	pDC->FrameRect(rt, &brush2);

	pDC->SetTextColor(m_clCurSelLine); 

	rt.left   = nLeft+1;
	rt.top    = rtClient.top+1;
	rt.right  = rt.left + cz.cx;
	rt.bottom = rtClient.bottom;
	pDC->DrawText(strTextBottom, rt, DT_LEFT|DT_CENTER);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(pOldFont);

}
CString CKLineView::GetWeekName(int week)
{
	CString strText = _T("");
	switch(week)
	{
	case 1:
		strText =_T("日");
		break;
	case 2:
		strText = _T("一");
		break;
	case 3:
		strText = _T("二");
		break;
	case 4:
		strText = _T("三");
		break;
	case 5:
		strText = _T("四");
		break;
	case 6:
		strText = _T("五");
		break;
	case 7:
		strText = _T("六");
		break;
	

	}
	return strText;
}
void CKLineView::DrawCurSelLine(CDC *pDC)
{	
	SHitTestKView hitTest;
	HitTest(m_ptMouseMoveLast, hitTest);
	if(hitTest.eType == enumKSplitBar)
		return;

	CRect rtClient;
	GetClientRect(&rtClient);
	rtClient.left = (rtClient.left + m_RectOffSet.left);
	rtClient.top =  m_rtKLine.top;
	rtClient.right = (rtClient.right - m_RectOffSet.Width() -m_RectOffSet.left);
	rtClient.bottom = m_rtScales.bottom;

	CPen penBorder(PS_SOLID,1,m_clCurSelLine);
	CPen *oldPen = pDC->SelectObject(&penBorder);	

	if(m_ptMouseMoveLast.x >= rtClient.left	&& m_ptMouseMoveLast.x <= rtClient.right+1)
	{		
		pDC->MoveTo(m_ptMouseMoveLast.x, rtClient.top);
		pDC->LineTo(m_ptMouseMoveLast.x, rtClient.bottom);
	}
	if(m_ptMouseMoveLast.y >= rtClient.top && m_ptMouseMoveLast.y <= rtClient.bottom)
	{
		pDC->MoveTo(rtClient.left, m_ptMouseMoveLast.y);
		pDC->LineTo(rtClient.right, m_ptMouseMoveLast.y);	
	}	

	pDC->SelectObject(oldPen);
}
void CKLineView::DrawCommonLast(CDC* pDC)
{
	if(m_MouseMode != MOUSE_PREPARE_DRAGITEM && m_MouseMode != MOUSE_OVER_ITEM_DIVIDE)
	{
		if(m_bMouseIn && m_bShowCross)
			DrawCurSelLine(pDC);
		if(m_bMouseIn)
			DrawTips(pDC);
	}
	else
	{//拖拽的线条
		if(m_MouseMode == MOUSE_PREPARE_DRAGITEM)
		{
			CPen pPen(PS_SOLID,2,RGB(119,124,125));
			CPen *pOldPen = pDC->SelectObject(&pPen);
			int nOldMode = pDC->SetROP2(R2_XORPEN);
			//caculate the height position
			CRect rcClient;
			GetClientRect(rcClient);
			//xor draw
			int Height = m_ptMouseMoveLast.y;
			if(Height < m_rtKLine.top+m_nMaxSpan+m_RectOffSet.top)
				Height = m_rtKLine.top+m_nMaxSpan+m_RectOffSet.top;
			if(Height > m_rtKVolume.bottom - m_nMaxSpan)
				Height = m_rtKVolume.bottom -m_nMaxSpan;

			pDC->MoveTo(rcClient.left,  Height);
			pDC->LineTo(rcClient.right, Height);
			//restore
			pDC->SetROP2(nOldMode);
			pDC->SelectObject(pOldPen);		
		}

	}
	if(m_bMouseIn && m_bShowCross)
	{
		//CRect rtData = CRect(1, m_RectOffSet.top, m_RectOffSet.left, m_nLeftPanelHeight);
		CRect rtData = CRect(1, 5, m_RectOffSet.left, m_nLeftPanelHeight);
		DrawOneItemData(pDC, rtData);
	}
}
void CKLineView::DrawOneItemData(CDC* pDC,  CRect rtClient)
{	
	CRect rtAll;
	GetClientRect(&rtAll);
	int nHeightMax = rtAll.Height()-2;
	int nHeight = rtClient.Height()-2;
	if(nHeight > nHeightMax)
		nHeight = nHeightMax;

	CBrush brush(m_clBackGround);		
	pDC->FillRect(CRect(1, 1, rtClient.Width()-1, nHeight), &brush);

	CBrush brush2(m_clRedLine);	
	pDC->FrameRect(CRect(1, 1, rtClient.Width()-1, nHeight), &brush2);
	rtClient.left  = rtClient.left -1;
	rtClient.right = rtClient.right -1;

	CFont *pOldFont=pDC->SelectObject(&m_FontLeft);

	int nTop = 0;
	CString  strText, strText2;
	CSize sz;
	CPoint pt;
	strText = _T("时    间");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = rtClient.top + 1; 
	pDC->SetTextColor(m_LeftpanelTime); 
	
	CRect rt2;
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	
	nTop = rt2.top + sz.cy + 1;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine pKLine = m_vecKLine[m_nCurSel];
		GetTimeText(m_enumPhrase, strText, strText2);
		pDC->SetTextColor(m_LeftpanelTime); 
		//dc.SetTextForeground(m_LeftpanelTime); 
	}
	else 
	{
		strText  = _T("----");
		strText2 = _T("----");
	}

	if(nHeightMax <nTop + sz.cy * 2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ; 
	pDC->SetTextColor(m_LeftpanelTime); 
//	dc.SetTextForeground(m_LeftpanelTime); 
	//dc.DrawText(strText, pt);
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

	nTop = nTop + sz.cy + 1;

	sz = pDC->GetTextExtent(strText2);	
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ; 
	pDC->SetTextColor(m_LeftpanelTime); 
//	dc.SetTextForeground(m_LeftpanelTime); 
//	dc.DrawText(strText2, pt);
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText2, rt2, DT_LEFT|DT_TOP);

	nTop = nTop + sz.cy + 1;


	//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	strText = _T("开    盘");
	sz = pDC->GetTextExtent(strText);
//	dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 
	pDC->SetTextColor(m_LeftpanelName); 
//	dc.SetTextForeground(m_LeftpanelName); 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

	//dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine  pKLine = m_vecKLine[m_nCurSel];
		//strText.Format(_T("%.2f"), pKLine.fOpenPrice);
		strText = Double2String( pKLine.fOpenPrice, m_nNumDigits);
		SetTextColor(pDC, pKLine.fOpenPrice);
	}
	else 
		strText = _T("----");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ; 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	//dc.DrawText(strText, pt);
	nTop = nTop + sz.cy + 1;

	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}
	strText = _T("最    高");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 
	pDC->SetTextColor(m_LeftpanelName); 
//	dc.SetTextForeground(m_LeftpanelName); 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
//	dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine pKLine = m_vecKLine[m_nCurSel];
		//	strText.Format(_T("%.2f"), pKLine.fHighPrice);
		strText = Double2String( pKLine.fHighPrice, m_nNumDigits);
		SetTextColor(pDC, pKLine.fHighPrice);
	}
	else 
		strText = _T("----");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ; 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

	nTop = nTop + sz.cy + 1;

	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}
	strText = _T("最    低");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left + (rtClient.Width() -sz.cx)/2;
	pt.y = nTop; 
	pDC->SetTextColor(m_LeftpanelName); 
	//dc.SetTextForeground(m_LeftpanelName); 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
//	dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine pKLine = m_vecKLine[m_nCurSel];
		//	strText.Format(_T("%.2f"), pKLine.fLowPrice);
		strText = Double2String( pKLine.fLowPrice, m_nNumDigits);
		SetTextColor(pDC, pKLine.fLowPrice);
	}
	else 
		strText = _T("----");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ; 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

	nTop = nTop + sz.cy + 1;

	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}
	strText = _T("收    盘");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 
	pDC->SetTextColor(m_LeftpanelName); 
	//dc.SetTextForeground(m_LeftpanelName); 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
//	dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine pKLine = m_vecKLine[m_nCurSel];
		//	strText.Format(_T("%.2f"), pKLine.fClosePrice);
		strText = Double2String( pKLine.fClosePrice, m_nNumDigits);
		SetTextColor(pDC, pKLine.fClosePrice);
	}
	else 
		strText = _T("----");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ; 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	//dc.DrawText(strText, pt);
	nTop = nTop + sz.cy + 1;

	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}
	strText = _T("成 交 量");

	sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 
	pDC->SetTextColor(m_LeftpanelName); 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->SetTextColor(m_LeftpanelName); 
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	//dc.SetTextForeground(m_LeftpanelName); 
	//dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine pKLine = m_vecKLine[m_nCurSel];
		strText.Format(_T("%d"), pKLine.dwVolume);
		//dc.SetTextForeground(m_LeftpanelTime); 
	}
	else 
		strText = _T("----");
	sz = pDC->GetTextExtent(strText);
	//dc.GetTextExtent(strText, &w, &h);	
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ;
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->SetTextColor(m_LeftpanelName); 
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	//dc.DrawText(strText, pt);
	nTop = nTop + sz.cy + 1;
	//////////////////////////////////////////////////////////////////////////

	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}
	strText = _T("涨    跌");
	sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->SetTextColor(m_LeftpanelName); 
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel <(int)m_vecKLine.size())
	{
		SKLine pKLine = m_vecKLine[m_nCurSel];

		int nPrevious = m_nCurSel +1;
		if(nPrevious >=0 && nPrevious < (int)m_vecKLine.size())	
		{
			SKLine  pKLinePrevious = m_vecKLine[nPrevious];
			//strText.Printf(wxT("%.2f"), pKLine.fClosePrice - pKLinePrevious->fClosePrice);		
			strText = Double2String( pKLine.fClosePrice - pKLinePrevious.fClosePrice, m_nNumDigits, true);
			SetTextColor(pDC, pKLine.fClosePrice);
		}
		else
			strText = _T("----");		
	}
	else 
		strText = _T("----");
	sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ; 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->SetTextColor(m_LeftpanelName); 
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	nTop = nTop + sz.cy + 1;


	//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}
	strText = _T("幅    度");
	sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 

	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->SetTextColor(m_LeftpanelName); 
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

//	dc.SetTextForeground(m_LeftpanelName); 
//	dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
	{
		SKLine pKLine = m_vecKLine[m_nCurSel];

		int nPrevious = m_nCurSel +1;
		if(nPrevious >=0 && nPrevious < (int)m_vecKLine.size())	
		{
			SKLine pKLinePrevious = m_vecKLine[nPrevious];
			strText.Format(_T("%.2f%%"), 100*(pKLine.fClosePrice - pKLinePrevious.fClosePrice)/pKLinePrevious.fClosePrice);	
			SetTextColor(pDC, pKLine.fClosePrice);
		}
		else
			strText = _T("----");		
	}
	else 
		strText = _T("----");
	sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ; 
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
//	dc.DrawText(strText, pt);
	nTop = nTop + sz.cy + 1;

	pDC->SelectObject(pOldFont);
}
void CKLineView::DrawZuiGaoZuiDiJia(CDC *pDC)
{
	if(m_vecKLine.size() == 0)
		return;

	ASSERT((int)m_vecKLine.size() > m_nMaxPos || (int)m_vecKLine.size() > m_nMinPos);

	CRect rtClient ;
	rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	int nLeft1 =0, nRight1 =0;
	XuHao2WeiZhi(m_nMaxPos, nLeft1, nRight1);
	SKLine pKLine = m_vecKLine[m_nMaxPos];
	int top1 = JiaGe2GaoDu(pKLine.fHighPrice);

	CPen penLine(PS_SOLID,1,m_clCurSelLine);
	CPen *oldPen = pDC->SelectObject(&penLine);

	if(rtClient.right > nRight1 +40)
	{
		pDC->MoveTo((nLeft1+nRight1)/2, top1);
		pDC->LineTo((nLeft1+nRight1)/2+20, top1+10);			
	}
	else
	{
		pDC->MoveTo((nLeft1+nRight1)/2, top1);
		pDC->LineTo((nLeft1+nRight1)/2-20, top1+10);	
	}
	int nLeft2 =0, nRight2 =0;
	XuHao2WeiZhi(m_nMinPos, nLeft2, nRight2);
	pKLine = m_vecKLine[m_nMinPos];
	int top2 = JiaGe2GaoDu(pKLine.fLowPrice);

	if(rtClient.right > nRight2 +40)
	{
		pDC->MoveTo((nLeft2+nRight2)/2, top2);
		pDC->LineTo((nLeft2+nRight2)/2+20, top2-10);	
		//dc.DrawLine((nLeft2+nRight2)/2, top2, (nLeft2+nRight2)/2+20, top2-10);		
	}
	else
	{
		pDC->MoveTo((nLeft2+nRight2)/2, top2);
		pDC->LineTo((nLeft2+nRight2)/2-20, top2-10);
	//	dc.DrawLine((nLeft2+nRight2)/2, top2, (nLeft2+nRight2)/2-20, top2-10);		
	}

	CFont *pOldFont = pDC->SelectObject(&m_FontHead);
	pDC->SetTextColor(m_clCurSelLine);
//	wxFont wxfont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 
//	dc.SetTextForeground(m_clCurSelLine); 

	if(rtClient.right > nRight1 +40)
	{
		CString strText;
		CPoint pt;
		//	strText = CString::Format(wxT("%.2f"), m_fMaxValue);
		strText = Double2String(m_fMaxValue, m_nNumDigits);
		CSize sz = pDC->GetTextExtent(strText);
		pt.x = (nLeft1+nRight1)/2+20;
		pt.y = top1+10; 

		CRect rt2;
		rt2.left   = pt.x;
		rt2.top    = pt.y;
		rt2.right  = rt2.left + sz.cx;
		rt2.bottom = rt2.top + sz.cy;
		pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
		//dc.DrawText(strText, pt);
	}
	else
	{
		CString strText;
		CPoint pt;
		//strText = CString::Format(wxT("%.2f"), m_fMaxValue);
		strText = Double2String(m_fMaxValue,m_nNumDigits);
		CSize sz = pDC->GetTextExtent(strText);
		pt.x = (nLeft1+nRight1)/2-40;
		pt.y = top1+10; 

		CRect rt2;
		rt2.left   = pt.x;
		rt2.top    = pt.y;
		rt2.right  = rt2.left + sz.cx;
		rt2.bottom = rt2.top + sz.cy;
		pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	//	dc.DrawText(strText, pt);
	}

	if(rtClient.right > nRight2 +40)
	{
		CString strText;
		CPoint pt;
		//	strText = CString::Format(wxT("%.2f"), m_fMinValue);
		strText = Double2String(m_fMinValue, m_nNumDigits);
		CSize sz = pDC->GetTextExtent(strText);
		pt.x = (nLeft2+nRight2)/2+20;
		pt.y = top2-20; 
		CRect rt2;
		rt2.left   = pt.x;
		rt2.top    = pt.y;
		rt2.right  = rt2.left + sz.cx;
		rt2.bottom = rt2.top + sz.cy;
		pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	}
	else
	{
		CString strText;
		CPoint pt;
		//	strText = CString::Format(wxT("%.2f"), m_fMinValue);
		strText = Double2String(m_fMinValue, m_nNumDigits);
		CSize sz = pDC->GetTextExtent(strText);
		pt.x = (nLeft2+nRight2)/2-40;
		pt.y = top2-20; 
		CRect rt2;
		rt2.left   = pt.x;
		rt2.top    = pt.y;
		rt2.right  = rt2.left + sz.cx;
		rt2.bottom = rt2.top + sz.cy;
		pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(pOldFont);
}

void CKLineView::DrawItems(CDC *pDC)
{
	if(m_vecKLine.size() == 0)
		return;

	for(int i = m_nBegin; i>= m_nEnd; i--)
	{
		SKLine  pKLine = m_vecKLine[i];
	

		int nLeft =0, nRight =0;
		XuHao2WeiZhi(i, nLeft, nRight);
		int top1 = JiaGe2GaoDu(pKLine.fHighPrice);
		int top2 = JiaGe2GaoDu(pKLine.fOpenPrice);
		int top3 = JiaGe2GaoDu(pKLine.fClosePrice);
		int top4 = JiaGe2GaoDu(pKLine.fLowPrice);

		DrawOneItem(pDC, nLeft, nRight, top1, top2, top3, top4);
	}
}
void CKLineView::DrawOneItem(CDC *pDC, int nLeft, int nRight, int nTop1, int nTop2, int nTop3, int nTop4)
{//int nTop1 最高, int nTop2 开盘, int nTop3 收盘, int nTop4 最低

	if(nTop2 >= nTop3)
	{
		CPen penLine(PS_SOLID,1,m_clRedLine);
		CPen *oldPen = pDC->SelectObject(&penLine);
		pDC->MoveTo(nLeft , nTop3);
		pDC->LineTo(nRight, nTop3);		
		pDC->MoveTo(nRight, nTop3);
		pDC->LineTo(nRight, nTop2);
		pDC->MoveTo(nRight, nTop2);
		pDC->LineTo(nLeft, nTop2);
		pDC->MoveTo(nLeft, nTop2);
		pDC->LineTo(nLeft, nTop3);
		
		pDC->MoveTo((nLeft+nRight)/2, nTop3);
		pDC->LineTo((nLeft+nRight)/2, nTop1);
		pDC->MoveTo((nLeft+nRight)/2, nTop2);
		pDC->LineTo((nLeft+nRight)/2, nTop4);

	
		pDC->SelectObject(oldPen);
	}
	else
	{
		CPen penLine(PS_SOLID,1,m_clGreenLine);
		CPen *oldPen = pDC->SelectObject(&penLine);
		
		pDC->MoveTo((nLeft+nRight)/2, nTop2);
		pDC->LineTo((nLeft+nRight)/2, nTop1);
	
		pDC->MoveTo((nLeft+nRight)/2, nTop3);
		pDC->LineTo((nLeft+nRight)/2, nTop4);
		
	


		CBrush brush(m_clGreenLine);
		pDC->FillRect(CRect(nLeft, nTop2, nRight, nTop3), &brush);
	//	dc.SetBrush(brush);
	//	dc.DrawRectangle(nLeft, nTop2, nRight - nLeft, nTop3 - nTop2);	

		pDC->SelectObject(oldPen);
	}
}
int	CKLineView::JiaGe2GaoDu(double fJiaGe)
{
	CRect rtClient ;
	rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	double ftemp = (fJiaGe - m_fMinValue)*rtClient.Height()/(m_fMaxValue - m_fMinValue)  ;
	int nn = rtClient.bottom;
	double fWeiZhi =rtClient.bottom - (fJiaGe - m_fMinValue)*rtClient.Height()/(m_fMaxValue - m_fMinValue)  ;
	int nWeiZhi = (int)fWeiZhi;
	if(fWeiZhi - nWeiZhi >0.5)
		nWeiZhi += 1;

	return nWeiZhi;
}
float CKLineView::XuHao2WeiZhi(int nItem, int& nLeft, int& nRight)
{
	CRect rtClient ;
	rtClient = m_rtKLine;

	int nKaiShi =  m_nBegin - nItem;
	if(nKaiShi == 0)
	{
		nLeft = rtClient.left;
		nRight = nLeft + m_dbDayWidth;
	}
	else
	{
		nLeft = rtClient.left + nKaiShi * (m_dbDayWidth +m_nDaySpan);
		nRight = nLeft + m_dbDayWidth;
	}
	return nLeft;
}
void CKLineView::DrawHead(CDC *pDC)
{
	CRect rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);
	
	CFont *pOldFont = pDC->SelectObject(&m_FontHead);
	pDC->SetTextColor(m_clLineName);
//	wxFont wxfont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT, false); 	
//	dc.SetFont(wxfont);
//	dc.SetTextForeground(m_clLineName); 

	CString  strText;
	CSize sz;
	CPoint pt;	
	strText.Format(_T("%s"), m_ContractName.c_str());

	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left + 5;
	pt.y = rtClient.top -sz.cy; 
	CRect rt2;
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

	pDC->SetTextColor(m_clBorder); 

//	dc.SetTextForeground(m_clBorder); 
//	wxCoord w2,h2;
	CString strLineName;
	GetLineName(strLineName);
	sz = pDC->GetTextExtent(strText);
	pt.x = pt.x + sz.cx + 2;
	pt.y = rtClient.top -sz.cy; 

	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx+2;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strLineName, rt2, DT_LEFT|DT_TOP);



	
	if(TECHINDEX_MAX <1)
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	if(m_pTechIndex[0].m_nVecDataCount == 0 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	int nLeft = pt.x + sz.cx + 2;
	CString strTechTitle;
	strTechTitle.Format(_T("%s("), m_pTechIndex[0].m_strTechName);
	for(int nConfig =0; nConfig< (int)m_pTechIndex[0].m_vecConfig.size(); nConfig++)
	{
		CString strTemp;
		if(nConfig == 0)
			strTemp.Format(_T("%d"), m_pTechIndex[0].m_vecConfig[nConfig]);
		else
			strTemp.Format(_T(", %d"), m_pTechIndex[0].m_vecConfig[nConfig]);

		strTechTitle.Append(strTemp);
	}
	strTechTitle.Append(_T(")"));
	pDC->SetTextColor(m_clLineColor[0]); 
//	dc.SetTextForeground(m_clLineColor[0]); 
	sz = pDC->GetTextExtent(strTechTitle);
//	wxCoord wConfig,hConfig;
//	dc.GetTextExtent(strTechTitle, &wConfig, &hConfig);	
	pt.x = nLeft;
	pt.y = rtClient.top - sz.cy; 

	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx+2;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strTechTitle, rt2, DT_LEFT|DT_TOP);

	//dc.DrawText(strTechTitle, pt);

	nLeft = pt.x + sz.cx + 5;			
	for(int k =0; k < m_pTechIndex[0].m_nVecDataCount; k++)
	{
		if(m_pTechIndex[0].m_vecData == NULL)
		{
			pDC->SelectObject(pOldFont);
			return;
		}
		if( m_pTechIndex[0].m_vecData[k].size() == 0)
			continue;
		pDC->SetTextColor(m_clLineColor[k+1]); 
	//	dc.SetTextForeground(m_clLineColor[k+1]); 

		CPen penBorder(PS_SOLID,m_nBorderWidth,m_clLineColor[k+1]);
		CPen *oldPen = pDC->SelectObject(&penBorder);

		//wxPen penBorder(wxColour(m_clLineColor[k+1]), m_nBorderWidth, wxSOLID);
		//dc.SetPen(penBorder);
		CString str;
		CString strFormat;
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;		
		if(m_nCurSel >= 0 && m_nCurSel < (int)m_pTechIndex[0].m_vecData[k].size())
			sValue = m_pTechIndex[0].m_vecData[k][m_nCurSel];
		if(sValue.fValue - MIN_VALUE_DEFAULT > -0.00001 && sValue.fValue - MIN_VALUE_DEFAULT < 0.00001)	
		{
			if((int)m_pTechIndex[0].m_vecConfigText.size() > k)
				str.Format(_T("%s: "), m_pTechIndex[0].m_vecConfigText[k].c_str());
		}
		else
		{
			strFormat.Format(_T("%%s: %%.%df"), m_nNumDigits);	
			str.Format(strFormat, sValue.strName.c_str(), sValue.fValue);
		}

		CSize szTemp = pDC->GetTextExtent(str);
	//	wxCoord w3,h3;
	//	dc.GetTextExtent(str, &w3, &h3);	
		pt.x = nLeft;
		pt.y = rtClient.top -szTemp.cy; 
		rt2.left   = pt.x;
		rt2.top    = pt.y;
		rt2.right  = rt2.left + sz.cx+2;
		rt2.bottom = rt2.top + sz.cy;
		pDC->DrawText(str, rt2, DT_LEFT|DT_TOP);

	//	dc.DrawText(str, pt);
		nLeft = pt.x + szTemp.cx + 5;	

		pDC->SelectObject(oldPen);
	}

	pDC->SelectObject(pOldFont);
	
}
void CKLineView::GetLineName(CString& strLineName)
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
void CKLineView::DrawBorder(CDC *pDC)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	CRect rtAll = rtClient;

	CPen PenR(PS_SOLID,2,m_clBorder);
	CPen *oldPen = pDC->SelectObject(&PenR);
	//整体边框
	pDC->MoveTo(rtClient.left, rtClient.top);
	pDC->LineTo(rtClient.left, rtClient.bottom);
	pDC->LineTo(rtClient.right, rtClient.bottom);
	pDC->LineTo(rtClient.right, rtClient.top);
	pDC->LineTo(rtClient.left, rtClient.top);

	//“左侧信息框”的右边
	rtClient.DeflateRect(m_RectOffSet.left, 0, 0, 0);
	pDC->MoveTo(rtClient.left, rtClient.top);
	pDC->LineTo(rtClient.left, rtClient.bottom - m_rtScales.Height());

	//K线窗口右边
	rtClient.DeflateRect(0, 0, m_RectOffSet.right, 0);
	pDC->MoveTo(rtClient.right, rtClient.top);
	pDC->LineTo(rtClient.right, rtClient.bottom- m_rtScales.Height());

	//画刻度上轨
	pDC->MoveTo(m_rtScales.left, m_rtScales.top);
	pDC->LineTo(m_rtScales.right, m_rtScales.top);

	//K线图下轨
	pDC->MoveTo(rtAll.left, m_rtKVolume.top);
	pDC->LineTo(rtAll.right,  m_rtKVolume.top);
	pDC->SelectObject(oldPen);
}
void CKLineView::DrawScales(CDC *pDC)
{
	CRect rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);
	
	CPen penBorder(PS_SOLID,1,m_cldashLine);
	CPen *oldPen = pDC->SelectObject(&penBorder);
	
	pDC->MoveTo(rtClient.left, rtClient.bottom - rtClient.Height()/3);
	pDC->LineTo(rtClient.right, rtClient.bottom - rtClient.Height()/3);
	pDC->MoveTo(rtClient.left, rtClient.bottom - 2*rtClient.Height()/3);
	pDC->LineTo(rtClient.right, rtClient.bottom - 2*rtClient.Height()/3);	
	pDC->MoveTo(rtClient.left, rtClient.top);
	pDC->LineTo(rtClient.right, rtClient.top);
	pDC->MoveTo(rtClient.left, rtClient.bottom);
	pDC->LineTo(rtClient.right, rtClient.bottom);

	if(m_bMouseIn && m_bShowCross)
	{
		pDC->SelectObject(oldPen);
		return;
	}

	//wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 
	//dc.SetTextForeground(m_clBorder); 

	if(m_fMaxValue  == MIN_VALUE_DEFAULT ||	m_fMinValue == MAX_VALUE_DEFAULT)
	{
		pDC->SelectObject(oldPen);
		return;
	}
	pDC->SetTextColor(m_clBorder); 
	CFont *pOldFont=pDC->SelectObject(&m_FontScales);
	CString  strText;
	CSize sz;
	CPoint pt;

	int nTop = rtClient.top -10;
	//	strText = CString::Format(wxT("%.2f"), m_fMaxValue);
	strText = Double2String(m_fMaxValue, m_nNumDigits, true);
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left - sz.cx - 4;
	pt.y = nTop; 
	CRect rt;
	rt.left   = pt.x;
	rt.top    = pt.y;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

	nTop = rtClient.top -10 + rtClient.Height()/3;
	//	strText = CString::Format(wxT("%.2f"), m_fMaxValue - (m_fMaxValue- m_fMinValue)/3);
	strText = Double2String(m_fMaxValue - (m_fMaxValue- m_fMinValue)/3, m_nNumDigits, true);
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left - sz.cx - 4;
	pt.y = nTop; 
	
	rt.left   = pt.x;
	rt.top    = pt.y;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);
	//dc.DrawText(strText, pt);

	nTop = rtClient.top -10 + 2*rtClient.Height()/3;
	//	strText = CString::Format(wxT("%.2f"), m_fMaxValue - 2*(m_fMaxValue- m_fMinValue)/3);
	strText = Double2String(m_fMaxValue - 2*(m_fMaxValue- m_fMinValue)/3, m_nNumDigits, true);
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left - sz.cx-4;
	pt.y = nTop; 

	rt.left   = pt.x;
	rt.top    = pt.y;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);


	nTop = rtClient.bottom -15 ;
	//	strText = CString::Format(wxT("%.2f"), m_fMinValue );
	strText = Double2String(m_fMinValue, m_nNumDigits, true);
	sz = pDC->GetTextExtent(strText);		
	pt.x = rtClient.left - sz.cx-4;
	pt.y = nTop; 
	rt.left   = pt.x;
	rt.top    = pt.y;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(pOldFont);
}
bool  CKLineView::GetShowCount(int  nMax)
{
	int nSize = m_vecKLine.size();
	if(nSize == 0)
		return false;
	CRect rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	int nWith = rtClient.Width();

	if(nWith <=0 )
		return false;
	int nShowCount = m_nShowCount;
	if(nMax == -1)
	{
		if(  (m_nDaySpan == 0 && m_dbDayWidth == 1))
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

	if(nShowCount > nWith && nMax != 0)//没有经过放大缩小的不改变显示的数目
		nShowCount = nWith;
	//if(nShowCount == m_nShowCount)
	//	return false;

	m_nShowCount = nShowCount;
	double nOneDay = 1.0*nWith/nShowCount;
	m_nDaySpan  = nOneDay *1/4;
	m_dbDayWidth = nOneDay - m_nDaySpan;

	return true;
}

void CKLineView::OnView1()
{
	CRect rtClient;
	GetClientRect(&rtClient);	
	
	
	//刻度矩形区域
	m_rtScales = rtClient;
	m_rtScales.top = m_rtScales.bottom - 18;//刻度高度

	rtClient.top = rtClient.top + m_RectOffSet.top;
	rtClient.left = rtClient.left + m_RectOffSet.left;

	//K线图矩形区域
	m_rtKLine = rtClient;
	m_rtKLine.bottom = m_rtScales.top;
}
void CKLineView::OnView2()
{
	CRect rtClient;
	GetClientRect(&rtClient);
	

	//刻度矩形区域
	m_rtScales = rtClient;
	m_rtScales.top = m_rtScales.bottom - 18; //刻度高度

//	rtClient.top = rtClient.top + m_RectOffSet.top;
	rtClient.left = rtClient.left + m_RectOffSet.left;

	//分时图矩形区域
	m_rtKLine = rtClient;
	m_rtKLine.bottom = m_rtScales.top;
	m_rtKLine.bottom = m_rtKLine.top + (rtClient.Height()-m_rtScales.Height())*m_dbKLine/(m_dbKLine + m_dbKVolume);

	

	//量能图矩形区域
	m_rtKVolume = rtClient;
	m_rtKVolume.bottom = m_rtScales.top;
	m_rtKVolume.top = m_rtKVolume.top + m_rtKVolume.Height()*m_dbKLine/(m_dbKLine + m_dbKVolume);

}
bool CKLineView::HitTest(CPoint pt, SHitTestKView& hitTest)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	if(pt.x > rtClient.left && pt.x < rtClient.right
		&& pt.y >= m_rtKLine.bottom && pt.y <= m_rtKLine.bottom+m_nResizeRange)
	{
		hitTest.eType = enumKSplitBar;
		hitTest.nIndex = 0;
		return true;
	}
	if(m_rtKLine.PtInRect(pt))
	{
		hitTest.eType = enumKLine;
		return true;
	}
	if(m_rtKVolume.PtInRect(pt))
	{
		hitTest.eType = enumKVolume;
		return true;
	}
	if(m_rtScales.PtInRect(pt))
	{
		hitTest.eType = enumKScales;
		return true;
	}	
	return false;
}
BOOL CKLineView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT)
	{
		switch (m_MouseMode) 
		{		
		case MOUSE_PREPARE_DRAGITEM:
			SetCursor(::LoadCursor(NULL, IDC_SIZENS)); 
			break;
		default:                    
			SetCursor(::LoadCursor(NULL, IDC_ARROW));
		}
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
void CKLineView::OnSize(UINT nType, int cx, int cy)
{
	if(m_nKViewCount == 1)
		OnView1();
	else if(m_nKViewCount == 2)
		OnView2();

	m_bMaxMinChange = TRUE;
	GetShowCount(0);
	GetBeginEnd();
	CalcMaxMin();

	Invalidate();
	CWnd::OnSize(nType, cx, cy);
}
void CKLineView::GetBeginEnd()
{
	int nBegin = 0, nEnd =0;
	CRect rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	int nWith = rtClient.Width();
	if(nWith <= 0)
		return;

	if(m_dbDayWidth +m_nDaySpan == 0)
		return;

	int nCount = nWith/(m_dbDayWidth +m_nDaySpan);
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
void CKLineView::CalcMaxMin()
{
	if(!m_bMaxMinChange)
		return;//主要优化速度
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue = MAX_VALUE_DEFAULT;
	for(int i = m_nBegin; i< (int)m_vecKLine.size() && i>= m_nEnd  && i>=0; i--)
	{
		SKLine pKLine = m_vecKLine[i];
		if(pKLine.fHighPrice > m_fMaxValue)
		{
			m_fMaxValue = pKLine.fHighPrice;
			m_nMaxPos = i;
		}
		if(pKLine.fLowPrice < m_fMinValue)
		{
			m_fMinValue = pKLine.fLowPrice;
			m_nMinPos = i;
		}
	}
/*
	if(m_pTechIndex[0].m_nVecDataCount ==0)
		return;
	for(int k =0; k < m_pTechIndex[0].m_nVecDataCount; k++)
	{
		for(int i = m_nBegin; i< (int)m_vecKLine.size() && i>= m_nEnd; i--)
		{
			STechIndexValue sValue;
			sValue.fValue = MIN_VALUE_DEFAULT;		
			if(i >= 0 && i < (int)m_pTechIndex[0].m_vecData[k].size())
				sValue = m_pTechIndex[0].m_vecData[k][i];
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
	}	*/
}
BOOL CKLineView::SetVectorData(vector<SKLine>& vecKLine)
{
//	if(vecKLine.size() == 0)
//		return TRUE;

	bool bRecalc = false;
	if(m_vecKLine.size() != vecKLine.size())
		bRecalc = true;
	m_vecKLine = vecKLine;
/*	if(m_pFData && bRecalc && m_strTechIndexName!= "")
	{
		STechCalcPara sTCPara;
		sTCPara.enumPhrase = m_enumPhrase;
		sTCPara.m_VolumeMultiple = m_pFData->m_VolumeMultiple;
		m_pFData->CalcKTechIndex(m_vecKLine, "", m_strTechIndexName.c_str(),sTCPara, m_pTechIndex);
	}
*/
	GetShowCount(0);
	GetBeginEnd();
	CalcMaxMin();
	CalcMaxMin_Volume();
	Invalidate();

	return true;
}
double CKLineView::GaoDu2JiaGe(int nHeight)
{
	CRect rtClient ;
	rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	double fJiaGe = (m_fMaxValue - m_fMinValue)*(rtClient.bottom - nHeight )/rtClient.Height() + m_fMinValue;
	return fJiaGe;
}
int	CKLineView::WeiZhi2XuHao(int nWidthPos)
{
	CRect rtClient ;
	rtClient = m_rtKLine;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	int nWidth = nWidthPos - rtClient.left;
	if(nWidth <0)//左边空白向左，则去掉
		return -1;

	if(m_dbDayWidth +m_nDaySpan == 0)
		return -1;
	int nCount = nWidth/(m_dbDayWidth +m_nDaySpan);
	if(m_dbDayWidth >1 && nWidth % ((int)m_dbDayWidth +m_nDaySpan) > m_dbDayWidth)
		nCount ++;

	nCount = m_nBegin - nCount;

	return nCount;//超过部分，也为左边的Item
}
void CKLineView::SetTextColor(CDC *pDC, double dbClosePrice)
{
	double dbPrevious = 0.0;
	int nPrevious = m_nCurSel +1;
	if(nPrevious >=0 && nPrevious < (int)m_vecKLine.size())	
	{
		SKLine pKLinePrevious = m_vecKLine[nPrevious];
		dbPrevious = pKLinePrevious.fClosePrice;	
	}

	if(dbClosePrice - dbPrevious > -0.00001 && dbClosePrice - dbPrevious < 0.00001)
		pDC->SetTextColor(m_Leftpanelhengpan); 
	else if(dbClosePrice > dbPrevious)
		pDC->SetTextColor(m_Leftpanelzhang); 
	else if(dbClosePrice < dbPrevious)
		pDC->SetTextColor(m_Leftpaneldie); 
}
void CKLineView::GetTimeText(EnumPhrase	 enumPhrase, CString& strText, CString& strText2)
{
	if(m_nCurSel <0 || m_nCurSel >= (int)m_vecKLine.size())
		return;

	SKLine pKLine = m_vecKLine[m_nCurSel];
	switch(m_enumPhrase)
	{
	case PHRASE_1MIN:
	case PHRASE_5MIN:		
	case PHRASE_15MIN:
	case PHRASE_30MIN:
	case PHRASE_60MIN:		
		strText.Format(_T("%2.2d/%2.2d"),  pKLine.dwTime.tm_mon+1, pKLine.dwTime.tm_mday);
		strText2.Format(_T("%2.2d:%2.2d"), pKLine.dwTime.tm_hour, pKLine.dwTime.tm_min);

		break;
	case PHRASE_DAY:		
	case PHRASE_WEEK:
	case PHRASE_MONTH:
		strText.Format(_T("%4.4d"), pKLine.dwTime.tm_year+1900);
		strText2.Format(_T("%2.2d/%2.2d"), pKLine.dwTime.tm_mon+1, pKLine.dwTime.tm_mday );
		break;
	}
}
void CKLineView::OnMouseMove(UINT nFlags, CPoint point)
{
	SHitTestKView hitTest;
	HitTest(point, hitTest);
	if(hitTest.eType == enumKSplitBar)		
	{	
		if(m_nKViewCount != 1)
			SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		if(m_MouseMode != MOUSE_PREPARE_DRAGITEM)
		{
			m_MouseMode = MOUSE_OVER_ITEM_DIVIDE;			
		}
		//else
			//OutputDebugString(_T("OnLButtonDown\n"));
	}
	else
	{
		if(!(nFlags & MK_LBUTTON))	
		{
			m_MouseMode = MOUSE_NOTHING;
			SetCursor(::LoadCursor(NULL, IDC_ARROW));
		//	OutputDebugString(_T("MouseMove\n"));
		}
		else
		{
			//OutputDebugString(_T("1111\n"));
		}
	}
	m_bMouseIn = true;	
	m_bIsMouse		= true;
	m_bIsKeyBoard	= false;

	m_ptMouseMoveLast = point;
	int nCursel= WeiZhi2XuHao(m_ptMouseMoveLast.x);
	if(nCursel < m_nEnd || nCursel > m_nBegin)
	{
		Invalidate();
		return;
	}

	m_nCurSel = nCursel;	

	TRACKMOUSEEVENT csTME;
	csTME.cbSize = sizeof (csTME);
	csTME.dwFlags = TME_LEAVE|TME_HOVER;
	csTME.hwndTrack = m_hWnd ;// 指定要 追踪 的窗口 
	csTME.dwHoverTime = 1;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
	::_TrackMouseEvent (&csTME); // 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持 

	Invalidate();

	CWnd::OnMouseMove(nFlags, point);
}
void CKLineView::OnMouseLeave()
{
	m_bMouseIn = false;
	Invalidate();

	CWnd::OnMouseLeave();
}
void CKLineView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	SetCapture();
	m_bMouseIn = true;
	m_bShowCross = true;
	m_bLbuttonDown = true;
	m_LbuttonDownPoint = point;

	SHitTestKView hitTest;
	HitTest(point, hitTest);
	if(hitTest.eType == enumKSplitBar)		
	{			
		if(m_MouseMode == MOUSE_OVER_ITEM_DIVIDE)
		{
			m_MouseMode = MOUSE_PREPARE_DRAGITEM;	
			//OutputDebugString(_T("MOUSE_PREPARE_DRAGITEM\n"));
			SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		}
		else
		{
		//	OutputDebugString(_T("222\n"));
		}	
	}
	else
	{
		m_MouseMode = MOUSE_NOTHING;
	//	OutputDebugString(_T("OnLButtow222222\n"));
	}
	CWnd::OnLButtonDown(nFlags, point);
}
void CKLineView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLbuttonDown = false;

	ReleaseCapture();
	if(m_MouseMode == MOUSE_PREPARE_DRAGITEM)
	{
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
		if(m_LbuttonDownPoint != point) 
		{
			if(m_nKViewCount == 2)
			{
				int nChange =  point.y - m_LbuttonDownPoint.y;
				if(nChange > 0)
				{//往下移
					if(nChange> m_rtKVolume.Height() -m_nMaxSpan)
						nChange = m_rtKVolume.Height() -m_nMaxSpan;

					m_dbKLine		= 1;
					m_dbKVolume		=  1.0*(m_rtKVolume.Height() -nChange)/(m_rtKLine.Height() +nChange);
					if(m_nKViewCount == 1)
						OnView1();
					else if(m_nKViewCount == 2)
						OnView2();
				}
				else				
				{//往上移
					int n1= m_rtKLine.Height() ;
					int n2 = m_rtKVolume.Height();
					nChange = -nChange;
					if(nChange> m_rtKLine.Height() -m_nMaxSpan)
						nChange = m_rtKLine.Height() -m_nMaxSpan;

					m_dbKLine		= 1;
					m_dbKVolume		=  1.0*(m_rtKVolume.Height() +nChange)/(m_rtKLine.Height() -nChange);
					if(m_nKViewCount == 1)
						OnView1();
					else if(m_nKViewCount == 2)
						OnView2();
				}
				Invalidate();
			}
		}
		m_MouseMode = MOUSE_NOTHING;
	}
	CWnd::OnLButtonUp(nFlags, point);
}
LRESULT CKLineView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_KEYDOWN  )
	{
		if(wParam == VK_LEFT)
		{	
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;
			
			if(m_nCurSel== m_vecKLine.size()-1)
			{
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
				if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
				{
					SKLine pKLine = m_vecKLine[m_nCurSel];
					float fClosePrice = pKLine.fClosePrice;
					int nHeight = JiaGe2GaoDu(fClosePrice);				
					m_ptMouseMoveLast = CPoint((nLeft+nRight)/2, nHeight);
				}
			/*	SItemGetValue(false, true);
				wxWindow* window= GetParent();
				wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
				myEvent.SetInt(nkeyCode);
				myEvent.SetClientData((void*)&m_sItem);
				window->ProcessEvent(myEvent);*/
				return CWnd::DefWindowProc(message,  wParam,  lParam); 
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
			CalcMaxMin_Volume();
			//	m_bShowOneItemData = true;
			m_bShowCross = true;
			//	Refresh(false);
			//	m_nLeftPos++;
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
			{				
				SKLine pKLine = m_vecKLine[m_nCurSel];
				float fClosePrice = pKLine.fClosePrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = CPoint((nLeft+nRight)/2, nHeight);
			}
			Invalidate();
		/*	SItemGetValue(false, true);

			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);
*/
			return CWnd::DefWindowProc(message,  wParam,  lParam); 
		}
		if(wParam == VK_RIGHT)
		{
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;
			if(m_nCurSel==0)
			{
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
				if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
				{
					SKLine pKLine = m_vecKLine[m_nCurSel];
					float fClosePrice = pKLine.fClosePrice;
					int nHeight = JiaGe2GaoDu(fClosePrice);				
					m_ptMouseMoveLast = CPoint((nLeft+nRight)/2, nHeight);
				}
			/*	SItemGetValue(false, true);
				wxWindow* window= GetParent();
				wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
				myEvent.SetInt(nkeyCode);
				myEvent.SetClientData((void*)&m_sItem);
				window->ProcessEvent(myEvent);*/
				return CWnd::DefWindowProc(message,  wParam,  lParam); 
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
			CalcMaxMin_Volume();
			m_bShowCross = true;
			//	Refresh(false);

			//	m_nRightPos++;
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
			{
				SKLine pKLine = m_vecKLine[m_nCurSel];
				float fClosePrice = pKLine.fClosePrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = CPoint((nLeft+nRight)/2, nHeight);
			}
			Invalidate();
		/*	SItemGetValue(false, true);

			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);*/
			return TRUE;
		}
		if(wParam == VK_DOWN)
		{
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;

			bool bShowChange = GetShowCount(-1);
			if(!bShowChange)
				return CWnd::DefWindowProc(message,  wParam,  lParam); 

			GetBeginEnd();
			CalcMaxMin();
			CalcMaxMin_Volume();
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
			{
				SKLine pKLine = m_vecKLine[m_nCurSel];
				float fClosePrice = pKLine.fClosePrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = CPoint((nLeft+nRight)/2, nHeight);
			}
			Invalidate();
		/*	SItemGetValue(false, true);

			Refresh(false);

			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);*/
			return TRUE;
		}
		else if(wParam == VK_UP)
		{	
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;

			bool bShowChange = GetShowCount(1);
			if(!bShowChange)
				return CWnd::DefWindowProc(message,  wParam,  lParam); 
			GetBeginEnd();

			CalcMaxMin();
			CalcMaxMin_Volume();

			int nLeft =0, nRight =0;
			XuHao2WeiZhi(m_nCurSel, nLeft, nRight);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecKLine.size())
			{
				SKLine pKLine = m_vecKLine[m_nCurSel];
				float fClosePrice = pKLine.fClosePrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = CPoint((nLeft+nRight)/2, nHeight);
			}
			Invalidate();
		/*	SItemGetValue(false, true);

			Refresh(false);

			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_KCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);*/

			return TRUE;
		}
		else if(wParam == VK_PRIOR)
		{	
			return TRUE;
		}
		else if(wParam == VK_NEXT)
		{				
			return TRUE;
		}
		else if(wParam == VK_HOME)
		{	
			return TRUE;
		}
		else if(wParam == VK_NEXT)
		{
			return TRUE;
		}

		else if(wParam == VK_END)
		{
			return TRUE;
		}	
		else  if(wParam == VK_CONTROL)
		{
			return TRUE;
		}
		else if(wParam == VK_SHIFT)
		{
			return TRUE;
		}
		else  if(wParam == VK_RETURN)
		{
			int n = 1;
			GetParent()->SendMessage(MESSAGE_RETURN,(WPARAM)&n,0);			
			return 1L;
			//return CWnd::DefWindowProc(message,  wParam,  lParam); 
		}	
		else
		{
	
		}
	}
	LRESULT lr = CWnd::DefWindowProc(message, wParam, lParam);
	return lr;
}
void CKLineView::DrawHead_Volume(CDC *pDC)
{
	CRect rtClient = m_rtKVolume;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);
	
	CFont *pOldFont = pDC->SelectObject(&m_FontHead);
	pDC->SetTextColor(m_clLineName);


	CString  strText;
	CSize sz(0,0);
	CPoint pt;	

	//strText = _T("VOL");
	//strText.Format(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());

	//sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left + 5;
	pt.y = rtClient.top-sz.cy; 
	CRect rt2;
	//rt2.left   = pt.x;
	//rt2.top    = pt.y;
	//rt2.right  = rt2.left + sz.cx;
//	rt2.bottom = rt2.top + sz.cy;
//	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

	//pDC->SelectObject(pOldFont);
	//dc.DrawText(strText, pt);

	if(TECHINDEX_MAX <2)
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	if(m_pTechIndex[1].m_nVecDataCount == 0 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}


	int nLeft = pt.x + sz.cx + 2;
	CString strTechTitle;
	strTechTitle.Format(_T("%s("), m_pTechIndex[1].m_strTechName);
	for(int nConfig =0; nConfig< (int)m_pTechIndex[1].m_vecConfig.size(); nConfig++)
	{
		CString strTemp;
		if(nConfig == 0)
			strTemp.Format(_T("%d"), m_pTechIndex[1].m_vecConfig[nConfig]);
		else
			strTemp.Format(_T(", %d"), m_pTechIndex[1].m_vecConfig[nConfig]);

		strTechTitle.Append(strTemp);
	}
	strTechTitle.Append(_T(")"));
	pDC->SetTextColor(m_clLineColor[0]); 
	//	dc.SetTextForeground(m_clLineColor[0]); 
	sz = pDC->GetTextExtent(strTechTitle);
	//	wxCoord wConfig,hConfig;
	//	dc.GetTextExtent(strTechTitle, &wConfig, &hConfig);	
	pt.x = nLeft;
	pt.y = rtClient.top-sz.cy; 

	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx+2;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strTechTitle, rt2, DT_LEFT|DT_TOP);

	//dc.DrawText(strTechTitle, pt);

	nLeft = pt.x + sz.cx + 5;			
	for(int k =0; k < m_pTechIndex[1].m_nVecDataCount; k++)
	{
		if(m_pTechIndex[1].m_vecData == NULL)
		{
			pDC->SelectObject(pOldFont);
			return;
		}

		if( m_pTechIndex[1].m_vecData[k].size() == 0)
			continue;
		pDC->SetTextColor(m_clLineColor[k+1]); 
		//	dc.SetTextForeground(m_clLineColor[k+1]); 

		CPen penBorder(PS_SOLID,m_nBorderWidth,m_clLineColor[k+1]);
		CPen *oldPen = pDC->SelectObject(&penBorder);

		//wxPen penBorder(wxColour(m_clLineColor[k+1]), m_nBorderWidth, wxSOLID);
		//dc.SetPen(penBorder);
		CString str;
		CString strFormat;
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;		
		if(m_nCurSel >= 0 && m_nCurSel < (int)m_pTechIndex[1].m_vecData[k].size())
			sValue = m_pTechIndex[1].m_vecData[k][m_nCurSel];
		if(sValue.fValue - MIN_VALUE_DEFAULT > -0.00001 && sValue.fValue - MIN_VALUE_DEFAULT < 0.00001)	
		{
			if((int)m_pTechIndex[1].m_vecConfigText.size() > k)
				str.Format(_T("%s: "), m_pTechIndex[1].m_vecConfigText[k].c_str());
		}
		else
		{
			strFormat.Format(_T("%%s: %%.%df"), m_nNumDigits);	
			str.Format(strFormat, sValue.strName.c_str(), sValue.fValue);
		}

		CSize szTemp = pDC->GetTextExtent(str);
		//	wxCoord w3,h3;
		//	dc.GetTextExtent(str, &w3, &h3);	
		pt.x = nLeft;
		pt.y = rtClient.top -szTemp.cy; 
		rt2.left   = pt.x;
		rt2.top    = pt.y;
		rt2.right  = rt2.left + sz.cx+2;
		rt2.bottom = rt2.top + sz.cy;
		pDC->DrawText(str, rt2, DT_LEFT|DT_TOP);

		//	dc.DrawText(str, pt);
		nLeft = pt.x + szTemp.cx + 5;	

		pDC->SelectObject(oldPen);
	}
	pDC->SelectObject(pOldFont);
}
void  CKLineView::DrawItems_Volume(CDC* pDC)
{
	if(m_vecKLine.size() == 0)
		return;

	for(int i = m_nBegin; i>= m_nEnd; i--)
	{
		SKLine  pKLine = m_vecKLine[i];
		//wxASSERT(pKLine != NULL);

		int nLeft =0, nRight =0;
		XuHao2WeiZhi(i, nLeft, nRight);
		int top1 = JiaGe2GaoDu_Volume(pKLine.dwVolume);

		if(pKLine.fOpenPrice > pKLine.fClosePrice)
			DrawOneItem_Volume(pDC, nLeft, nRight, top1,FALSE);
		else
			DrawOneItem_Volume(pDC, nLeft, nRight, top1,TRUE);		
	}
}
void CKLineView::DrawOneItem_Volume(CDC* pDC, int nLeft, int nRight, int nTop1, bool IsZhang)
{//int nTop1 最高, int nTop2 开盘, int nTop3 收盘, int nTop4 最低

	CRect rtClient = m_rtKVolume;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);
	if(IsZhang)
	{
		CPen penBorder(PS_SOLID,1,m_clRedLine);
		CPen *oldPen = pDC->SelectObject(&penBorder);
		pDC->MoveTo(nLeft, rtClient.bottom);
		pDC->LineTo(nLeft, nTop1);	
		pDC->MoveTo(nLeft, nTop1);
		pDC->LineTo(nRight, nTop1);	
		pDC->MoveTo(nRight, nTop1);
		pDC->LineTo(nRight, rtClient.bottom);	
		pDC->MoveTo(nRight, rtClient.bottom);
		pDC->LineTo(nLeft, rtClient.bottom);	
		pDC->SelectObject(oldPen);
	}
	else
	{
		CBrush brush(m_clGreenLine);
		CPen penBorder(PS_SOLID,1,m_clGreenLine);
		CPen *oldPen = pDC->SelectObject(&penBorder);
		pDC->FillRect(CRect(nLeft, nTop1, nRight,rtClient.bottom), &brush);
		pDC->SelectObject(oldPen);
	}
}
void CKLineView::DrawScales_Volume(CDC *pDC)
{
	CRect rtClient,rect;
	rtClient = m_rtKVolume;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	CPen penBorder(PS_SOLID,1,m_cldashLine);
	CPen *oldPen = pDC->SelectObject(&penBorder);

	pDC->MoveTo(rtClient.left, rtClient.bottom - rtClient.Height()/3);
	pDC->LineTo(rtClient.right, rtClient.bottom - rtClient.Height()/3);
	pDC->MoveTo(rtClient.left, rtClient.bottom - 2*rtClient.Height()/3);
	pDC->LineTo(rtClient.right, rtClient.bottom - 2*rtClient.Height()/3);	
	pDC->MoveTo(rtClient.left, rtClient.top);
	pDC->LineTo(rtClient.right, rtClient.top);
	pDC->MoveTo(rtClient.left, rtClient.bottom);
	pDC->LineTo(rtClient.right, rtClient.bottom);

	
	pDC->SetTextColor(m_clBorder); 
	//wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 
	//dc.SetTextForeground(m_clBorder); 

	if(m_fMaxValue_KVolume  == MIN_VALUE_DEFAULT ||	m_fMinValue_KVolume == MAX_VALUE_DEFAULT)
	{
		pDC->SelectObject(oldPen);
		return;
	}
	
	CFont *pOldFont=pDC->SelectObject(&m_FontScales);

	CString  strText;
	CSize sz;
	CPoint pt;

	int nTop = rtClient.top;
	double dbPrice = m_fMaxValue_KVolume;
	if(dbPrice <= 10 && dbPrice>0)
		strText.Format(_T("%.2f"), dbPrice);
	else
		strText.Format(_T("%.0f"), dbPrice);	
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left - sz.cx-4;
	pt.y = nTop; 
	CRect rt;
	rt.left   = pt.x;
	rt.top    = pt.y-sz.cy/2;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

	nTop = rtClient.top  + rtClient.Height()/3;
	dbPrice = m_fMaxValue_KVolume - (m_fMaxValue_KVolume- m_fMinValue_KVolume)/3;
	if(dbPrice <= 10 && dbPrice>0)
		strText.Format(_T("%.2f"), dbPrice);
	else
		strText.Format(_T("%.0f"), dbPrice);	

	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left - sz.cx-4;
	pt.y = nTop; 
	rt.left   = pt.x;
	rt.top    = pt.y-sz.cy/2;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

	nTop = rtClient.top  + 2*rtClient.Height()/3;
	dbPrice =  m_fMaxValue_KVolume - 2*(m_fMaxValue_KVolume- m_fMinValue_KVolume)/3;
	if(dbPrice <= 10 && dbPrice>0)
		strText.Format(_T("%.2f"), dbPrice);
	else
		strText.Format(_T("%.0f"), dbPrice);		

	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left - sz.cx - 4;
	pt.y = nTop; 
	rt.left   = pt.x;
	rt.top    = pt.y-sz.cy/2;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

	nTop = rtClient.bottom -15 ;
	dbPrice =  m_fMinValue_KVolume;
	if(dbPrice <= 10 && dbPrice>0)
		strText.Format(_T("%.2f"), dbPrice);
	else
		strText.Format(_T("%.0f"), dbPrice);		
	
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left - sz.cx-4;
	pt.y = nTop; 
	rt.left   = pt.x;
	rt.top    = pt.y-sz.cy/2;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(pOldFont);
}
void CKLineView::DrawBitmap_Volume(CDC *pDC)
{
	DrawHead_Volume(pDC);
	DrawScales_Volume(pDC);
	DrawItems_Volume(pDC);
}
void CKLineView::CalcMaxMin_Volume()
{
	if(!m_bMaxMinChange)
		return;//主要优化速度
	m_fMaxValue_KVolume  = MIN_VALUE_DEFAULT;
	m_fMinValue_KVolume = 0;
	for(int i = m_nBegin; i< (int)m_vecKLine.size() && i>= m_nEnd  && i>=0; i--)
	{
		SKLine pKLine = m_vecKLine[i];
		if(pKLine.dwVolume> m_fMaxValue_KVolume)
		{
			m_fMaxValue_KVolume = pKLine.dwVolume;
			//m_nMaxPos = i;
		}
		if(pKLine.dwVolume < m_fMinValue_KVolume)
		{
			//m_nMinPos = i;
		}
	}
	if(IsDoubleSame(m_fMaxValue, MIN_VALUE_DEFAULT))
	{
		m_fMaxValue_KVolume = 10.0;				
	}
}
int	CKLineView::JiaGe2GaoDu_Volume(double fJiaGe)
{
	CRect rtClient ;
	rtClient = m_rtKVolume;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	double ftemp = (fJiaGe - m_fMinValue_KVolume)*rtClient.Height()/(m_fMaxValue_KVolume - m_fMinValue_KVolume)  ;
	int nn = rtClient.bottom;
	double fWeiZhi =rtClient.bottom - (fJiaGe - m_fMinValue_KVolume)*rtClient.Height()/(m_fMaxValue_KVolume - m_fMinValue_KVolume)  ;
	int nWeiZhi = (int)fWeiZhi;
	if(fWeiZhi - nWeiZhi >0.5)
		nWeiZhi += 1;

	return nWeiZhi;
}
double	CKLineView::GaoDu2JiaGe_Volume(int nHeight)
{
	CRect rtClient ;
	rtClient = m_rtKVolume;
	rtClient.DeflateRect(0, m_RectOffSet.top, 0, m_RectOffSet.bottom);

	double fJiaGe = (m_fMaxValue_KVolume - m_fMinValue_KVolume)*(rtClient.bottom - nHeight )/rtClient.Height() + m_fMinValue_KVolume;
	return fJiaGe;
}
void CKLineView::DrawBitmap_Scales(CDC *pDC)
{
	DrawLeftText(pDC);
	DrawItems_Scales(pDC);
}
void CKLineView::DrawLeftText(CDC *pDC)
{
	CFont *pOldFont = pDC->SelectObject(&m_FontDownScalesLeft);
	pDC->SetTextColor(m_clText);
	

	CString  strText;
	CSize sz;
	CPoint pt;
	switch(m_enumPhrase)
	{		
	case PHRASE_1MIN:
		strText = _T("1分钟");
		break;
	case PHRASE_5MIN:
		strText = _T("5分钟");
		break;
	case PHRASE_15MIN:
		strText = _T("15分钟");
		break;
	case PHRASE_30MIN:
		strText = _T("30分钟");
		break;
	case PHRASE_60MIN:
		strText = _T("60分钟");
		break;
	case PHRASE_DAY:
		strText = _T("日线");
		break;
	case PHRASE_WEEK:
		strText = _T("周线");
		break;
	case PHRASE_MONTH:
		strText = _T("月线");
		break;
	}

	sz = pDC->GetTextExtent(strText);	
		
	CRect rt;
	rt.left   = m_rtScales.left + 5;
	rt.top    = m_rtScales.top  + 1;
	rt.right  = rt.left + sz.cx + 2;
	rt.bottom = m_rtScales.bottom;
	pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);
	


	POINT points[3];
	points[0].x = m_RectOffSet.left -3;
	points[0].y = m_rtScales.bottom-3;
	
	points[1].x = m_RectOffSet.left -20;
	points[1].y = m_rtScales.bottom-3;

	points[2].x = m_RectOffSet.left -12;
	points[2].y = m_rtScales.top+3;

	CPen penBorder(PS_SOLID,1,m_clTriangle);
	CPen *oldPen = pDC->SelectObject(&penBorder);
	
	
	CBrush brushHatch(m_clTriangle);
	CBrush* pOldBrush = pDC->SelectObject(&brushHatch);
	pDC->Polygon(points, 3);

		
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(oldPen);
	pDC->SelectObject(pOldFont);

}
void CKLineView::DrawItems_Scales(CDC *pDC)
{
	switch(m_enumPhrase)
	{		
	case PHRASE_1MIN:
		DrawItems_MINS(pDC);
		break;
	case PHRASE_5MIN:
		DrawItems_MINS(pDC);

		break;
	case PHRASE_15MIN:
		DrawItems_MINS(pDC);

		break;
	case PHRASE_30MIN:
		DrawItems_MINS(pDC);

		break;
	case PHRASE_60MIN:
		DrawItems_MINS(pDC);		
		break;
	case PHRASE_DAY:
		DrawItems_Day(pDC);

		break;
	case PHRASE_WEEK:
		DrawItems_MINS(pDC);

		break;
	case PHRASE_MONTH:
		DrawItems_MINS(pDC);

		break;
	}
}
void CKLineView::DrawItems_Day(CDC *pDC)
{
	if(m_vecKLine.size() == 0)
		return;

	CFont *pOldFont = pDC->SelectObject(&m_FontDownScalesLeft);
	
	CPen penBorder(PS_SOLID,1,m_clBorder);
	CPen *oldPen = pDC->SelectObject(&penBorder);	
	pDC->SetTextColor(m_clBorder);

	CRect rtClient = m_rtScales;
	rtClient.left = rtClient.left + m_RectOffSet.left;

	int nLastYear = 0, nLastMonth = 0, nLastDay = 0;
	int nLastWeek = 1;
	CSize cz;
	CPoint pt;
	int nFirstEnd =0;
	for(int i = m_nBegin; i>= m_nEnd; i--)
	{
		SKLine  pKLine = m_vecKLine[i];

		CString strText;
		if(i == m_nBegin)
		{	
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(i, nLeft, nRight);		
			if((nLeft+nRight)/2 > nFirstEnd)
			{
				pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
				pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
			}

			strText.Format(_T("%4.4d/%2.2d"), pKLine.dwTime.tm_year+1900, pKLine.dwTime.tm_mon+1);
			cz = pDC->GetTextExtent(strText);	
			pt.x = rtClient.left;
			pt.y = m_rtScales.top + (rtClient.Height() - cz.cy)/2; 	

			CRect rt;
			rt.left   = pt.x+4;
			rt.top    = pt.y;
			rt.right  = rt.left + cz.cx + 2;
			rt.bottom = m_rtScales.bottom;
			pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);

			nLastYear = pKLine.dwTime.tm_year;
			nLastMonth =  pKLine.dwTime.tm_mon;
			nLastDay =  pKLine.dwTime.tm_mday;
			
			COleDateTime dateBegin(pKLine.dwTime.tm_year+1900, pKLine.dwTime.tm_mon+1, pKLine.dwTime.tm_mday, pKLine.dwTime.tm_hour, pKLine.dwTime.tm_min, pKLine.dwTime.tm_sec);
			nLastWeek = dateBegin.GetDayOfWeek();
			strText = GetWeekName(nLastWeek);			
			nFirstEnd = pt.x + cz.cx;

			continue;
		}	

		COleDateTime dateBegin(pKLine.dwTime.tm_year+1900, pKLine.dwTime.tm_mon+1, pKLine.dwTime.tm_mday, pKLine.dwTime.tm_hour, pKLine.dwTime.tm_min, pKLine.dwTime.tm_sec);
		int week = dateBegin.GetDayOfWeek();

		if(week <= nLastWeek ||week == 2)//周一
		{
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(i, nLeft, nRight);
			if((nLeft+nRight)/2 > nFirstEnd)
			{
				pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
				pDC->LineTo((nLeft+nRight)/2, rtClient.top +4);
			}
		}	

		if(pKLine.dwTime.tm_year > nLastYear)
		{
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(i, nLeft, nRight);		
			if((nLeft+nRight)/2 > nFirstEnd)
			{
				pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
				pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
			}

			strText.Format(_T("%4.4d/%2.2d"), pKLine.dwTime.tm_year+1900, pKLine.dwTime.tm_mon+1);
			cz = pDC->GetTextExtent(strText);	
			pt.x = (nLeft+nRight)/2;
			pt.y = m_rtScales.top+(rtClient.Height() -cz.cy)/2; 
			if(pt.x > nFirstEnd)			
			{
				CRect rt;
				rt.left   = pt.x+2;
				rt.top    = pt.y;
				rt.right  = rt.left + cz.cx + 2;
				rt.bottom = m_rtScales.bottom;
				pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);			
			}
		}
		else if(pKLine.dwTime.tm_mon > nLastMonth)
		{
			int nLeft =0, nRight =0;
			XuHao2WeiZhi(i, nLeft, nRight);		
			if((nLeft+nRight)/2 > nFirstEnd)
			{
				pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
				pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
				//dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
			}

			strText.Format(_T("%2.2d"), pKLine.dwTime.tm_mon+1);
			cz = pDC->GetTextExtent(strText);	
			pt.x = (nLeft+nRight)/2;
			pt.y = m_rtScales.top +(rtClient.Height() -cz.cy)/2; 	
			if(pt.x > nFirstEnd)
			{
				CRect rt;
				rt.left   = pt.x+2;
				rt.top    = pt.y;
				rt.right  = rt.left + cz.cx + 2;
				rt.bottom = m_rtScales.bottom;
				pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);			
				//dc.DrawText(strText, pt);
			}
		}
		nLastYear = pKLine.dwTime.tm_year;
		nLastMonth = pKLine.dwTime.tm_mon;
		nLastDay = pKLine.dwTime.tm_mday;
		nLastWeek = week;
	}

	pDC->SelectObject(oldPen);	
	pDC->SelectObject(pOldFont);
}
void CKLineView::DrawItems_MINS(CDC *pDC)
{
	if(m_vecKLine.size() == 0)
		return;

	CFont *pOldFont = pDC->SelectObject(&m_FontDownScalesLeft);

	CPen penBorder(PS_SOLID,1,m_clBorder);
	CPen *oldPen = pDC->SelectObject(&penBorder);	
	pDC->SetTextColor(m_clBorder);

	CRect rtClient = m_rtScales;
	rtClient.left = rtClient.left + m_RectOffSet.left;

	struct tm tmPrevious;
	CSize sz;
	CPoint pt;

	int nFirstEnd = 0;
	int nItemSpan = 0;//间隔次数
	for(int i = m_nBegin; i>= m_nEnd; i--)
	{
		SKLine  pKLine = m_vecKLine[i];
		
		CString strText;
		if(i == m_nBegin)
		{
			strText.Format(_T("%.2d/%.2d/%2.2d"), pKLine.dwTime.tm_year + 1900, pKLine.dwTime.tm_mon+1, pKLine.dwTime.tm_mday);
			sz = pDC->GetTextExtent(strText);	
			pt.x = rtClient.left;
			pt.y = m_rtScales.top + (rtClient.Height() -sz.cy)/2; 			
			CRect rt;
			rt.left   = pt.x+2;
			rt.top    = pt.y;
			rt.right  = rt.left + sz.cx + 2;
			rt.bottom = m_rtScales.bottom;
			pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);			
		
			tmPrevious = pKLine.dwTime;
			nFirstEnd = pt.x + sz.cx;
			continue;
		}	

		ProcessMins(pDC, m_enumPhrase, i, nItemSpan, pKLine.dwTime, tmPrevious, nFirstEnd);
	}
	pDC->SelectObject(oldPen);	
	pDC->SelectObject(pOldFont);
}
void CKLineView::ProcessMins(CDC *pDC, EnumPhrase enumPhrase, int i, int& nItemSpan, struct tm tm33, struct tm& tmPrevious, int& nFirstEnd)
{
	CSize sz;
	CPoint pt;

	CRect rtClient = m_rtScales;
	rtClient.left = rtClient.left + m_RectOffSet.left;
	CString strText;
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
			if((dw%300 == 0 && m_dbDayWidth >8)
				||(dw%600 == 0 && m_dbDayWidth >4)
				||(dw%1200 == 0 && (m_dbDayWidth <= 4 && m_dbDayWidth >= 2) )
				||(dw%2400 == 0 && m_dbDayWidth ==1)
				|| dw>3600)//每5分钟画一次，5*60秒
			{
				tmPrevious = tm33;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);		
				if((nLeft+nRight)/2 > nFirstEnd)
				{
					pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
					pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
					//dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				}
				strText.Format(_T("%2.2d:%2.2d"), tm1.tm_hour, tm1.tm_min);
				sz = pDC->GetTextExtent(strText);	
				pt.x = (nLeft+nRight)/2;
				pt.y = m_rtScales.top + (rtClient.Height() -sz.cy)/2; 	
				if(pt.x > nFirstEnd)
				{		
					CRect rt;
					rt.left   = pt.x+2;
					rt.top    = pt.y;
					rt.right  = rt.left + sz.cx + 2;
					rt.bottom = m_rtScales.bottom;
					pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);			
					//dc.DrawText(strText, pt);
					sz = pDC->GetTextExtent(strText);	
					nFirstEnd = pt.x +sz.cx;
				}
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
				{
					pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
					pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
					//dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				}
				strText.Format(_T("%2.2d/%2.2d"), tm33.tm_mon, tm33.tm_mday);
				sz = pDC->GetTextExtent(strText);	
				pt.x = (nLeft+nRight)/2;
				pt.y = m_rtScales.top + (rtClient.Height() - sz.cy)/2; 
				if(pt.x > nFirstEnd)
				{
					CRect rt;
					rt.left   = pt.x+2;
					rt.top    = pt.y;
					rt.right  = rt.left + sz.cx + 2;
					rt.bottom = m_rtScales.bottom;
					pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);			
					//dc.DrawText(strText, pt);
					sz = pDC->GetTextExtent(strText);	
					nFirstEnd = pt.x + sz.cx;
				}
			}
		}
		break;
	case PHRASE_15MIN:
		{
			struct tm tm1(tm33);
			struct tm tm2(tmPrevious);
			if((tm2.tm_mday != tm1.tm_mday && m_dbDayWidth >1) || nItemSpan>= 3)
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
				{
					pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
					pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
					//dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				}
				strText.Format(_T("%2.2d/%2.2d"), tm33.tm_mon, tm33.tm_mday);
				sz = pDC->GetTextExtent(strText);	
				pt.x = (nLeft+nRight)/2;
				pt.y = m_rtScales.top + (rtClient.Height() - sz.cy)/2; 
				if(pt.x > nFirstEnd)
				{
					CRect rt;
					rt.left   = pt.x+2;
					rt.top    = pt.y;
					rt.right  = rt.left + sz.cx + 2;
					rt.bottom = m_rtScales.bottom;
					pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);	
					sz = pDC->GetTextExtent(strText);
					nFirstEnd = pt.x +sz.cx;
				}
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
			if((tm2.tm_mday != tm1.tm_mday && m_dbDayWidth >=4) 
				|| (nItemSpan>= 3 && m_dbDayWidth <4 && m_dbDayWidth >1)
				||  (nItemSpan>= 5 && m_dbDayWidth == 1))
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
				{
					pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
					pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
					//dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				}
				strText.Format(_T("%2.2d/%2.2d"), tm33.tm_mon, tm33.tm_mday);
				sz = pDC->GetTextExtent(strText);	
				pt.x = (nLeft+nRight)/2;
				pt.y = m_rtScales.top + (rtClient.Height() -sz.cy)/2; 	
				if(pt.x > nFirstEnd)
				{
					CRect rt;
					rt.left   = pt.x+2;
					rt.top    = pt.y;
					rt.right  = rt.left + sz.cx + 2;
					rt.bottom = m_rtScales.bottom;
					pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);	
					//dc.DrawText(strText, pt);
					//dc.GetTextExtent(strText, &w, &h);	
					sz = pDC->GetTextExtent(strText);
					nFirstEnd = pt.x +sz.cx;
				}
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
			if((nItemSpan>= 3 && m_dbDayWidth >=4) 
				|| (nItemSpan>= 6 && m_dbDayWidth <4 && m_dbDayWidth >1)
				||  (nItemSpan>= 10 && m_dbDayWidth == 1))
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
				{
					pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
					pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
					//dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				}
				strText.Format(_T("%2.2d/%2.2d"), tm33.tm_mon, tm33.tm_mday);
				sz = pDC->GetTextExtent(strText);	
				pt.x = (nLeft+nRight)/2;
				pt.y = m_rtScales.top + (rtClient.Height() -sz.cy)/2; 
				if(pt.x > nFirstEnd)
				{
					CRect rt;
					rt.left   = pt.x+2;
					rt.top    = pt.y;
					rt.right  = rt.left + sz.cx + 2;
					rt.bottom = m_rtScales.bottom;
					pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);	

					sz = pDC->GetTextExtent(strText);
					nFirstEnd = pt.x +sz.cx;
					//dc.DrawText(strText, pt);
					//dc.GetTextExtent(strText, &w, &h);	
					//nFirstEnd = pt.x +w;
				}
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
			if(tm33.tm_mon%2 == 1&& (tmPrevious.tm_year != tm33.tm_year|| (tmPrevious.tm_year == tm33.tm_year && tmPrevious.tm_mon != tm33.tm_mon)) && m_dbDayWidth>=6 
				|| (tm33.tm_mon%3 == 1&& (tmPrevious.tm_year != tm33.tm_year|| (tmPrevious.tm_year == tm33.tm_year && tmPrevious.tm_mon != tm33.tm_mon)) && m_dbDayWidth < 6 && m_dbDayWidth >1)
				|| (tm33.tm_mon == 1&& (tmPrevious.tm_year != tm33.tm_year|| (tmPrevious.tm_year == tm33.tm_year && tmPrevious.tm_mon != tm33.tm_mon)) && m_dbDayWidth == 1 ))
			{				
				nItemSpan = 0;
				int nLeft =0, nRight =0;
				XuHao2WeiZhi(i, nLeft, nRight);	
				if((nLeft+nRight)/2 > nFirstEnd)
				{
					pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
					pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
					//dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());
				}
				if(tm33.tm_mon == 1)
					strText.Format(_T("%4.4d/%2.2d"), tm33.tm_year, tm33.tm_mon);
				else
					strText.Format(_T("%2.2d"), tm33.tm_mon);
				sz = pDC->GetTextExtent(strText);
				pt.x = (nLeft+nRight)/2;
				pt.y = m_rtScales.top + (rtClient.Height() - sz.cy)/2; 
				if(pt.x > nFirstEnd)
				{
					CRect rt;
					rt.left   = pt.x+2;
					rt.top    = pt.y;
					rt.right  = rt.left + sz.cx + 2;
					rt.bottom = m_rtScales.bottom;
					pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);	

					sz = pDC->GetTextExtent(strText);
					nFirstEnd = pt.x +sz.cx;

				//	dc.DrawText(strText, pt);
				//	dc.GetTextExtent(strText, &w, &h);	
				//	nFirstEnd = pt.x +w;
				}
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
				{
					pDC->MoveTo((nLeft+nRight)/2, rtClient.top);
					pDC->LineTo((nLeft+nRight)/2, rtClient.bottom);
				//	dc.DrawLine((nLeft+nRight)/2, rtClient.GetTop(), (nLeft+nRight)/2, rtClient.GetBottom());	
				}
				strText.Format(_T("%4.4d/%2.2d"), tm33.tm_year, tm33.tm_mon);
				sz = pDC->GetTextExtent(strText);
				pt.x = (nLeft+nRight)/2;
				pt.y = m_rtScales.top + (rtClient.Height() - sz.cy)/2; 
				if(pt.x > nFirstEnd)
				{
					CRect rt;
					rt.left   = pt.x+2;
					rt.top    = pt.y;
					rt.right  = rt.left + sz.cx + 2;
					rt.bottom = m_rtScales.bottom;
					pDC->DrawText(strText, rt, DT_LEFT|DT_VCENTER);	

					sz = pDC->GetTextExtent(strText);
					nFirstEnd = pt.x +sz.cx;
					//dc.DrawText(strText, pt);
					//dc.GetTextExtent(strText, &w, &h);	
					//nFirstEnd = pt.x +w;
				}
			}
			tmPrevious = tm33;
		}
		break;
	}


}
void CKLineView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int n = 1;
	GetParent()->SendMessage(MESSAGE_LEFTDBCLICK,(WPARAM)&n,0);
	CWnd::OnLButtonDblClk(nFlags, point);
}
BOOL CKLineView::SetInTimeData(CFData& fData)
{
	vector<SKLine> vecKLine;
	bool bGet = fData.GetVecKLine(m_enumPhrase, vecKLine);	

	if(bGet)
	{
		std::vector<std::string> vecTechNames;
		EnumPhrase enumPhrase;
		GetTechIndexNames(vecTechNames, enumPhrase);					
		for(int i =0; i< (int)vecTechNames.size(); i++)
		{
			STechCalcPara sTCPara;
			sTCPara.enumPhrase = m_enumPhrase;
			sTCPara.m_VolumeMultiple = fData.m_VolumeMultiple;
			fData.CalcKTechIndex(vecKLine,"", vecTechNames[i], sTCPara, m_pTechIndex[i]);
		}		
	}

	SetVectorData(vecKLine);
	return true;
}
void CKLineView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(!HitTest(point, m_hitTestLast))
		return;
	
	if(m_hitTestLast.eType != enumKLine
		&& m_hitTestLast.eType != enumKVolume)
		return;

	CMenu menu1;
	menu1.CreatePopupMenu();     //动态创建弹出式菜单对象
	menu1.AppendMenu(MF_STRING,IDD_MENU_TOKLINE,"分时图");	
	menu1.AppendMenu(MF_SEPARATOR,IDD_MENU_TOKLINE,"");	
	
	menu1.AppendMenu(MF_STRING,IDD_MENU_K1,"1分钟");	
	menu1.AppendMenu(MF_STRING,IDD_MENU_K5,"5分钟");	
	menu1.AppendMenu(MF_STRING,IDD_MENU_K15,"15分钟");	
	menu1.AppendMenu(MF_STRING,IDD_MENU_K30,"30分钟");	
	menu1.AppendMenu(MF_STRING,IDD_MENU_K60,"60分钟");

	menu1.AppendMenu(MF_SEPARATOR,IDD_MENU_TOKLINE,"");	

	menu1.AppendMenu(MF_STRING,IDD_MENU_KDAY,"日线");	
	menu1.AppendMenu(MF_STRING,IDD_MENU_KWEEK,"周线");	
	menu1.AppendMenu(MF_STRING,IDD_MENU_KMONTH,"月线");	
	
	menu1.AppendMenu(MF_SEPARATOR,IDD_MENU_TOKLINE,"");	



	
	CMenu pTechMenu;
	pTechMenu.CreatePopupMenu();
	pTechMenu.AppendMenu(MF_STRING,IDD_MENU_INDEXEXPRESS,"指标用法注释");	
	pTechMenu.AppendMenu(MF_STRING,IDD_MENU_INDEXCONFIG,"调整指标参数");	

	menu1.AppendMenu(MF_STRING|MF_POPUP, (UINT_PTR)pTechMenu.m_hMenu, "指标"); 

	CPoint pt;
	GetCursorPos(&pt);
	menu1.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	menu1.DestroyMenu();
	pTechMenu.DestroyMenu();

	return CWnd::OnRButtonUp(nFlags, point);
}
void CKLineView::OnToKLine()
{
	int n = 1;
	GetParent()->SendMessage(MESSAGE_CHANGEVIEW,(WPARAM)&n,0);
}
void CKLineView::OnToK1()
{
   EnumPhrase phrase = PHRASE_1MIN;
   GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN,(WPARAM)&phrase,0);
}
void CKLineView::OnToK5()
{
	EnumPhrase phrase = PHRASE_5MIN;
	GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN,(WPARAM)&phrase,0);
}
void CKLineView::OnToK15()
{
	EnumPhrase phrase = PHRASE_15MIN;
	GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN,(WPARAM)&phrase,0);
}
void CKLineView::OnToK30()
{
	EnumPhrase phrase = PHRASE_30MIN;
	GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN,(WPARAM)&phrase,0);
}
void CKLineView::OnToK60()
{
	EnumPhrase phrase = PHRASE_60MIN;
	GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN,(WPARAM)&phrase,0);
}
void CKLineView::OnToKDay()
{
	EnumPhrase phrase = PHRASE_DAY;
	GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN,(WPARAM)&phrase,0);
}
void CKLineView::OnToKWeek()
{
	EnumPhrase phrase = PHRASE_WEEK;
	GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN,(WPARAM)&phrase,0);
}
void CKLineView::OnToKMonth()
{
	EnumPhrase phrase = PHRASE_MONTH;
	GetParent()->SendMessage(MESSAGE_CHANGETimeSPAN,(WPARAM)&phrase,0);
}
void CKLineView::DrawTechIndex(CDC* pDC)
{	
	if(m_pTechIndex[0].m_nVecDataCount == 0 )
		return;	

	CRect rtClient = m_rtKLine;
	rtClient.left = rtClient.left;
	rtClient.top = rtClient.top + m_RectOffSet.top-1;
	rtClient.right = rtClient.right - m_RectOffSet.Width() -m_RectOffSet.left;
	rtClient.bottom = rtClient.bottom - m_RectOffSet.Height()- m_RectOffSet.top+1;
	
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rtClient);
	pDC->SelectClipRgn(&rgn);
//	dc.SetClippingRegion(rtClient);
	for(int k =0; k < m_pTechIndex[0].m_nDrawBody; k++)
	{
		if(m_pTechIndex[0].m_vecData == NULL)
		{
			pDC->SelectClipRgn(NULL);
			return;
		}
		if( m_pTechIndex[0].m_vecData[k].size() == 0)
			continue;
		CPen penBorder(PS_SOLID,m_nBorderWidth,m_clLineColor[k+1]);
		CPen *oldPen = pDC->SelectObject(&penBorder);
		for(int i = m_nBegin; i> m_nEnd; i--)
		{
			if(i>= (int)m_pTechIndex[0].m_vecData[k].size() )
				continue;
			int nLeftBegin =0, nRightBegin =0, nLeftEnd = 0, nRightEnd = 0;
			XuHao2WeiZhi(i, nLeftBegin, nRightBegin);
			XuHao2WeiZhi(i-1, nLeftEnd, nRightEnd);
			if((m_pTechIndex[0].m_vecData[k][i].fValue - MIN_VALUE_DEFAULT > -0.00001 && m_pTechIndex[0].m_vecData[k][i].fValue - MIN_VALUE_DEFAULT < 0.00001) 
				|| (m_pTechIndex[0].m_vecData[k][i-1].fValue - MIN_VALUE_DEFAULT > -0.00001 && m_pTechIndex[0].m_vecData[k][i-1].fValue - MIN_VALUE_DEFAULT < 0.00001) )
				continue;
			int nBegin = JiaGe2GaoDu(m_pTechIndex[0].m_vecData[k][i].fValue);
			int nEnd   = JiaGe2GaoDu(m_pTechIndex[0].m_vecData[k][i-1].fValue);
		
			pDC->MoveTo((nLeftBegin+nRightBegin)/2, nBegin);
			pDC->LineTo((nLeftEnd+nRightEnd)/2, nEnd);			
			//dc.DrawLine((nLeftBegin+nRightBegin)/2, nBegin, (nLeftEnd+nRightEnd)/2, nEnd);
		}
		 pDC->SelectObject(oldPen);
	}
	pDC->SelectClipRgn(NULL);
	//dc.DestroyClippingRegion();
}
bool CKLineView::GetTechIndexNames(std::vector<std::string>& vecTechNames,  EnumPhrase& enumPhrase)
{
	for(int i=0; i<TECHINDEX_MAX; i++)
		vecTechNames.push_back(m_strTechIndexName[i].GetBuffer(m_strTechIndexName[i].GetLength()));
	enumPhrase = m_enumPhrase;

	return true;
}
void CKLineView::DrawTechIndex_Volume(CDC *pDC)
{
	if(m_pTechIndex[0].m_nVecDataCount == 0 )
		return;	

	CRect rtClient = m_rtKVolume;
	
	rtClient.left = rtClient.left + m_RectOffSet.left;
	rtClient.top = rtClient.top + m_RectOffSet.top-1;
	rtClient.right = rtClient.right - m_RectOffSet.Width() -m_RectOffSet.left;
	rtClient.bottom = rtClient.bottom - m_RectOffSet.Height()- m_RectOffSet.top+1;

	CRgn rgn;
	rgn.CreateRectRgnIndirect(rtClient);
	pDC->SelectClipRgn(&rgn);
	//	dc.SetClippingRegion(rtClient);
	for(int k =0; k < m_pTechIndex[1].m_nDrawBody; k++)
	{
		if(m_pTechIndex[1].m_vecData == NULL)
		{
			pDC->SelectClipRgn(NULL);
			return;
		}
		if( m_pTechIndex[1].m_vecData[k].size() == 0)
			continue;
		CPen penBorder(PS_SOLID,m_nBorderWidth,m_clLineColor[k+1]);
		CPen *oldPen = pDC->SelectObject(&penBorder);
		for(int i = m_nBegin; i> m_nEnd; i--)
		{
			if(i>= (int)m_pTechIndex[1].m_vecData[k].size() )
				continue;
			int nLeftBegin =0, nRightBegin =0, nLeftEnd = 0, nRightEnd = 0;
			XuHao2WeiZhi(i, nLeftBegin, nRightBegin);
			XuHao2WeiZhi(i-1, nLeftEnd, nRightEnd);
			if((m_pTechIndex[1].m_vecData[k][i].fValue - MIN_VALUE_DEFAULT > -0.00001 && m_pTechIndex[1].m_vecData[k][i].fValue - MIN_VALUE_DEFAULT < 0.00001) 
				|| (m_pTechIndex[1].m_vecData[k][i-1].fValue - MIN_VALUE_DEFAULT > -0.00001 && m_pTechIndex[1].m_vecData[k][i-1].fValue - MIN_VALUE_DEFAULT < 0.00001) )
				continue;
			int nBegin = JiaGe2GaoDu_Volume(m_pTechIndex[1].m_vecData[k][i].fValue);
			int nEnd   = JiaGe2GaoDu_Volume(m_pTechIndex[1].m_vecData[k][i-1].fValue);

			pDC->MoveTo((nLeftBegin+nRightBegin)/2, nBegin);
			pDC->LineTo((nLeftEnd+nRightEnd)/2, nEnd);			
			//dc.DrawLine((nLeftBegin+nRightBegin)/2, nBegin, (nLeftEnd+nRightEnd)/2, nEnd);
		}
		pDC->SelectObject(oldPen);
	}
	pDC->SelectClipRgn(NULL);
	//dc.DestroyClippingRegion();
}
void CKLineView::OnIndexExpress()
{	
	CString strTechName;	
	if(m_hitTestLast.eType == enumKLine)
		strTechName = m_strTechIndexName[0];
	else if(m_hitTestLast.eType == enumKVolume)
		strTechName = m_strTechIndexName[1];

	GetParent()->SendMessage(MESSAGE_INDEX_EXPRESS,(WPARAM)&strTechName, (LPARAM)m_enumPhrase);
}
void CKLineView::OnIndexConfig()
{
	CString strTechName;	
	int Index = 0;
	if(m_hitTestLast.eType == enumKLine)
	{
		strTechName = m_strTechIndexName[0];
		Index = 0;
	}
	else if(m_hitTestLast.eType == enumKVolume)
	{
		strTechName = m_strTechIndexName[1];
		Index = 1;
	}

	GetParent()->SendMessage(MESSAGE_INDEX_CONFIG,(WPARAM)&strTechName, (LPARAM)Index);
}
void CKLineView::GetFuture(string& ContractName)
{
	ContractName = m_ContractName;
}