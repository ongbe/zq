// TLineView.cpp : implementation file
//

#include "stdafx.h"
#include "OptionQuation.h"
#include "TLineView.h"


// CTLineView

IMPLEMENT_DYNAMIC(CTLineView, CWnd)

CTLineView::CTLineView()
{
	m_clBackGround	= RGB(0,0,0);
	m_clBorder		= RGB(192,0,0);
	m_cldashLine	= RGB(76,40,40);

	m_clPrice		= RGB(255, 255, 255);
	m_clAveragePrice   = RGB(255, 255, 0);

	m_clName		= RGB(255, 255, 255);
	m_clTime		= RGB(82, 255, 255);
	m_zhang			= RGB(255, 82, 82);
	m_die			= RGB(82, 255, 82);
	m_hengpan		= RGB(255, 255, 255);
	m_clCurSelLine  = RGB(200,200,200);
	m_clLineName    = RGB(192,192,192);
	
	m_clTipsBk		= RGB(0,0,128);
	m_nBegin = 0;
	m_nEnd   = 0;
	m_nDayCount = 1;

	m_RectOffSet =  CRect(66,16,66,0);
	m_nBorderWidth	= 1;
	m_fMaxValue  = MIN_VALUE_DEFAULT;
	m_fMinValue  = MAX_VALUE_DEFAULT;

	m_bShowHeight = true;
	m_bShowTime   = true;
	m_ptMouseMoveLast = CPoint(-1,-1);
	m_nLeftPanelHeight = 230;//左侧价格提示窗高度


	m_fLastClosePrice = MIN_VALUE_DEFAULT;

	m_nOneDayScalesCount = m_vecTimeScales.size();

	m_dbVolumeMultiple = 100.00;
	//m_bShowOneItemData = true;
//	m_pFData = NULL;
	m_nNumDigits = 2;


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
	lf.lfHeight = 15;
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
	m_hSysFontBottomScales = ::CreateFontIndirect(&lf);
	if(!m_FontBottomScales.m_hObject)
	{
		CFont font;
		LOGFONT lf;
		HFONT hSysFont; 
		hSysFont = m_hSysFontBottomScales;
		font.Attach( hSysFont );
		font.GetLogFont(&lf);
		font.Detach();
		m_FontBottomScales.CreateFontIndirect(&lf);
	}

	m_bShowCross	   = false;
	m_bMouseIn		   = false;
	m_nCurSel   = -1;

	m_uTimerarr[TIMER_1] = -1;
	m_uTimerarr[TIMER_2] = -1;

	m_ExchangeName = "";
	m_ContractName = "";	

	m_nTViewCount	= 2;
	m_rtTLine		= CRect(0,0,0,0);
	m_rtTVolume		= CRect(0,0,0,0);
	m_rtScales		= CRect(0,0,0,0);

	//////////////////////////////////////////////////////////////////////////
	m_fMaxValue_TVolume  = MIN_VALUE_DEFAULT;
	m_fMinValue_TVolume  = 0;

	m_LbuttonDownPoint = CPoint(0, 0);
	m_dbTLine		   = 1.0;
	m_dbTVolume		   = 1.0;
	m_nResizeRange     = 2;
	m_nMaxSpan		   = 15;
	m_bLbuttonDown		= false;
	
}

CTLineView::~CTLineView()
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


BEGIN_MESSAGE_MAP(CTLineView, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDD_MENU_TOKLINE, OnToKLine)
END_MESSAGE_MAP()



BOOL CTLineView::Create(CRect rtClient, CWnd* pParentWnd, UINT nID)
{
	return	CWnd::Create(NULL,  NULL, WS_CHILD|WS_VISIBLE, rtClient, pParentWnd, nID, NULL);
}
void CTLineView::OnPaint() 
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
	DrawBitmap_TLine(&dcMem);
	DrawBitmap_TVolume(&dcMem);
	DrawBitmap_TScales(&dcMem);
	DrawCommonLast(&dcMem);
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(OldBmp);
	dcMem.DeleteDC();	

}
void CTLineView::DrawCommonLast(CDC* pDC)
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
			if(Height < m_rtTLine.top+m_nMaxSpan)
				Height = m_rtTLine.top+m_nMaxSpan;
			if(Height > m_rtTVolume.bottom - m_nMaxSpan)
				Height = m_rtTVolume.bottom -m_nMaxSpan;
			
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
void CTLineView::DrawBitmap_TLine(CDC *pDC)
{
	CRect rtClient,rect;
	GetClientRect(&rtClient);		
	pDC->FillSolidRect(rtClient, m_clBackGround);

	DrawScales(pDC);
	DrawHead(pDC);
	DrawBorder(pDC);	
	DrawPriceLines(pDC);
	DrawAveragePriceLines(pDC);
	
}
void CTLineView::DrawHead(CDC *pDC)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	rtClient.left   = (rtClient.left + m_RectOffSet.left);
	rtClient.top	= (rtClient.top + m_RectOffSet.top);
	rtClient.right	= (rtClient.right - m_RectOffSet.Width() -m_RectOffSet.left);
	rtClient.bottom = (rtClient.bottom - m_RectOffSet.Height()- m_RectOffSet.top);

	CFont *pOldFont = pDC->SelectObject(&m_FontLeft);
//	wxFont wxfont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false); 	
//	dc.SetTextForeground(m_clLineName); 
	pDC->SetTextColor(m_clLineName); 

	CString  strText;
	CSize sz;
	CPoint pt;

	//if(m_ContractDate == "")
	strText.Format(_T("%s"), m_ContractName.c_str());
	//strText =  m_ContractName;
	//else
	//	strText.Format(_T("%s_%s"), m_ContractName.c_str(), m_ContractDate.c_str());
	//	strText.Format(_T("%s_%s_%s"), m_ExchangeName.c_str(), m_ContractName.c_str(), m_ContractDate.c_str());


	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.left + 5;
	pt.y = rtClient.top - sz.cy; 

	CRect rt2;
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	
	pDC->SetTextColor(m_clLineName); 

//	dc.SetTextForeground(m_clLineName); 
//	wxCoord w2,h2;
	CString strLineName = _T("分时走势");

	sz = pDC->GetTextExtent(strLineName);	
	pt.x = pt.x + sz.cx + 2;
	pt.y = rtClient.top -sz.cy; 
	
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;

	pDC->DrawText(strLineName, rt2, DT_LEFT|DT_TOP);
	//dc.DrawText(strLineName, pt);

	pDC->SelectObject(pOldFont);
}
void CTLineView::DrawOneItemData(CDC *pDC,  CRect rtClient)
{
	if(m_nBegin <0 || m_nBegin >= (int)m_vecTLine.size())
		return;
	//	if(m_nBegin == -1)
	//		return;
	CRect rtAll;
	GetClientRect(&rtAll);
	int nHeightMax = rtAll.Height()-2;
	int nHeight = rtClient.Height()-2;
	if(nHeight > nHeightMax)
		nHeight = nHeightMax;
	
	CBrush brush(m_clBackGround);		
	pDC->FillRect(CRect(1, 1, rtClient.Width()-1, nHeight), &brush);

	CBrush brush2(m_zhang);	
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
	pDC->SetTextColor(m_clName); 
	
	CRect rt2;
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

	nTop = rtClient.top + 1 + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine pTLine = m_vecTLine[m_nCurSel];
		//	GetTimeText(m_enumPhrase, strText, strText2);
		strText.Format(_T("%2.2d:%2.2d"), pTLine.dwTime.tm_hour, pTLine.dwTime.tm_min);
		pDC->SetTextColor(m_clTime); 
		//dc.SetTextForeground(m_clTime); 
	}
	else 
	{
		strText  = _T("----");
		strText2 = _T("----");
	}
	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.right - sz.cx - 2;
	pt.y = nTop ;

	pDC->SetTextColor(m_clTime); 
	
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	//dc.DrawText(strText, pt);
	nTop = nTop + sz.cy + 3;
	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	strText = _T("价    格");
	sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 
	pDC->SetTextColor(m_clName); 
//	dc.SetTextForeground(m_clName); 
	
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
//	dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine pTLine = m_vecTLine[m_nCurSel];
		//	strText.Format(_T("%.2f"), pTLine->fLastPrice);
		strText = Double2String( pTLine.fLastPrice, m_nNumDigits);
		SetTextColor(pDC, pTLine.fLastPrice);
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
	nTop = nTop + sz.cy + 3;
	//////////////////////////////////////////////////////////////////////////
	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	strText = _T("均    价");
	sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 
	pDC->SetTextColor(m_clName); 
	//dc.SetTextForeground(m_clName); 

	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	//dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine pTLineBegin = m_vecTLine[m_nBegin];
		double dbVolumeCountBegin  = pTLineBegin.dwVolume;
		double dbdwHoldVolumeBegin = pTLineBegin.dwVolumeMoney;
		double dbVolumeCountEnd  = 0;
		double dbdwHoldVolumeEnd = 0;
		for( int i = m_nBegin-1; i>= m_nCurSel; i-- )
		{
			STLine pTLineEnd = m_vecTLine[i];
			dbdwHoldVolumeBegin += pTLineEnd.dwVolumeMoney;
			dbVolumeCountBegin  += pTLineEnd.dwVolume;
			dbdwHoldVolumeEnd	=  dbdwHoldVolumeBegin;
			dbVolumeCountEnd	=  dbVolumeCountBegin;
			pTLineBegin			=  pTLineEnd;
		}
		if(dbVolumeCountBegin<0.00001)
			strText = _T("0.00");
		else
			strText = Double2String(dbdwHoldVolumeBegin/(m_dbVolumeMultiple*dbVolumeCountBegin), m_nNumDigits);
		//	strText.Format(_T("%.2f"), dbdwHoldVolumeBegin/(m_dbVolumeMultiple*dbVolumeCountBegin));
		//dc.SetTextForeground(m_clTime); 
		pDC->SetTextColor(m_clTime); 
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
	nTop = nTop + sz.cy + 3;

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

	pDC->SetTextColor(m_clName); 
	//dc.SetTextForeground(m_clName); 

	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	//dc.DrawText(strText, pt);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine pTLine = m_vecTLine[m_nCurSel];
		//	strText.Format(_T("%.2f"), pTLine->fLastPrice - m_fLastClosePrice);	
		strText = Double2String( pTLine.fLastPrice - m_fLastClosePrice, m_nNumDigits);
		SetTextColor(pDC, pTLine.fLastPrice);

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
	nTop = nTop + sz.cy + 3;

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
	pDC->SetTextColor(m_clName);
	//dc.SetTextForeground(m_clName); 
	//dc.DrawText(strText, pt);

	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine pTLine = m_vecTLine[m_nCurSel];

		if(m_fLastClosePrice - MIN_VALUE_DEFAULT > -0.00001 && m_fLastClosePrice - MIN_VALUE_DEFAULT < 0.00001)	
		{
			strText = _T("----");				
		}
		else
		{
			strText.Format(_T("%.2f%%"), 100*(pTLine.fLastPrice - m_fLastClosePrice)/m_fLastClosePrice);	
			SetTextColor(pDC, pTLine.fLastPrice);
		}				
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
	nTop = nTop + sz.cy + 3;

	if(nHeightMax <nTop + sz.cy*2 + 2 )
	{
		pDC->SelectObject(pOldFont);
		return;
	}

	strText = _T("成 交 量");
	sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.left + (rtClient.Width() - sz.cx)/2;
	pt.y = nTop; 
	pDC->SetTextColor(m_clName);
	//dc.SetTextForeground(m_clName); 
//	dc.DrawText(strText, pt);
	rt2.left   = pt.x;
	rt2.top    = pt.y;
	rt2.right  = rt2.left + sz.cx;
	rt2.bottom = rt2.top + sz.cy;
	pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);
	nTop = nTop + sz.cy;
	if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
	{
		STLine pTLine = m_vecTLine[m_nCurSel];
		strText.Format(_T("%d"), pTLine.dwVolume);
		//dc.SetTextForeground(m_clTime); 
		pDC->SetTextColor(m_clTime);
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
	nTop = nTop + sz.cy + 3;

	pDC->SelectObject(pOldFont);

}
void CTLineView::DrawCurSelLine(CDC *pDC)
{	
	SHitTest hitTest;
	HitTest(m_ptMouseMoveLast, hitTest);
	if(hitTest.eType == enumSplitBar)
		return;

	CRect rtClient;
	GetClientRect(&rtClient);
	rtClient.left = (rtClient.left + m_RectOffSet.left);
	rtClient.top =  m_rtTLine.top;
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
void CTLineView::DrawBorder(CDC *pDC)
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

	//T线窗口右边
	rtClient.DeflateRect(0, 0, m_RectOffSet.right, 0);
	pDC->MoveTo(rtClient.right, rtClient.top);
	pDC->LineTo(rtClient.right, rtClient.bottom- m_rtScales.Height());

	//画刻度上轨
	pDC->MoveTo(m_rtScales.left, m_rtScales.top);
	pDC->LineTo(m_rtScales.right, m_rtScales.top);

	//分时图下轨
	pDC->MoveTo(rtAll.left, m_rtTLine.bottom+1);
	pDC->LineTo(rtAll.right, m_rtTLine.bottom+1);
	pDC->SelectObject(oldPen);
}
void CTLineView::DrawScales(CDC* pDC)
{
	if(m_vecTimeScales.size() == 0)
		return;

	CRect rtClient,rect;
	rtClient = m_rtTLine;
	rect = rtClient;

	CFont *pOldFont=pDC->SelectObject(&m_FontScales);

	CString  strText;
	CPoint pt;
	CRect rt;
	CPen penBorder(PS_SOLID,3,m_clBorder);
	CPen *oldPen = pDC->SelectObject(&penBorder);
	int nHeight = JiaGe2GaoDu(m_fLastClosePrice);

	pDC->MoveTo(rtClient.left+1, nHeight);
	pDC->LineTo(rtClient.right-1, nHeight);
	
	SetTextColor(pDC, m_fLastClosePrice);
	pDC->SetBkMode(TRANSPARENT);
	strText = Double2String(m_fLastClosePrice, m_nNumDigits);
	CSize sz = pDC->GetTextExtent(strText);
	pt.x = rtClient.left - sz.cx-4;
	pt.y = nHeight - sz.cy/2; 
	rt.left   = pt.x;
	rt.top    = pt.y;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	if(nHeight >=0)
		pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

	strText = _T("0.00%");
	sz = pDC->GetTextExtent(strText);	
	pt.x = rtClient.right + 4;
	pt.y = nHeight - sz.cy/2; 
	rt.left   = pt.x;
	rt.top    = pt.y;
	rt.right  = rt.left + sz.cx;
	rt.bottom = rt.top + sz.cy;
	if(nHeight >=0)
		pDC->DrawText(strText, rt, DT_RIGHT|DT_TOP);

	pDC->SelectObject(oldPen);

	CPen penBorder2(PS_SOLID,2,m_clBorder);
	oldPen = pDC->SelectObject(&penBorder2);
	for(int i = 1; i< (int)m_vecTimeScales.size()-1; i = i + 2)//-1是不希望最后一根画竖线
	{  
		int nX = Time2X(m_vecTimeScales[i]);	
		pDC->MoveTo(nX, rtClient.top);
		pDC->LineTo(nX, rtClient.bottom);				
	}
	pDC->SelectObject(oldPen);

	struct tm tmBegin = m_vecTimeScales[0];
	struct tm tmEnd   = m_vecTimeScales[m_vecTimeScales.size() - 1];
	COleDateTime dateBegin(tmBegin.tm_year+1900, tmBegin.tm_mon+1, tmBegin.tm_mday, tmBegin.tm_hour, tmBegin.tm_min, tmBegin.tm_sec);
	COleDateTime dateEnd(tmEnd.tm_year+1900, tmEnd.tm_mon+1, tmEnd.tm_mday, tmEnd.tm_hour, tmEnd.tm_min, tmEnd.tm_sec);

	for(COleDateTime dt = dateBegin; dt<dateEnd; dt += COleDateTimeSpan(0, 0, 15, 0))
	{
		int nMin = dt.GetMinute();
		int sss = dt.GetYear();
		if(nMin == 30)
		{
			struct tm tmDt;
			DateTime2Tm(dt, tmDt);
			CPen penBorder2(PS_DOT,1,m_clBorder);
			//////////////////////////////////////////////////////////////////////////
		/*	if(m_nHalfHourLineLineType == 0)
			{//虚线的时候粗度必须为1,1以上的时候有时候会很难看，有时候会出现锯齿状
				penBorder2.SetStyle(wxDOT);
				penBorder2.SetWidth(1);
			}
			else
			{
				penBorder2.SetStyle(wxSOLID);
				penBorder2.SetWidth(m_nHalfHourLineSize);	
			}*/
			//////////////////////////////////////////////////////////////////////////
			oldPen = pDC->SelectObject(&penBorder2);
			int nX = Time2X(tmDt);
			pDC->MoveTo(nX, rtClient.top);
			pDC->LineTo(nX, rtClient.bottom);	
			pDC->SelectObject(oldPen);
		}
		else if(nMin == 0)
		{
			struct tm tmDt;
			DateTime2Tm(dt, tmDt);		
			CPen penBorder2(PS_SOLID,1,m_clBorder);
			//////////////////////////////////////////////////////////////////////////
	/*		if(m_nHourLineType == 0)
			{//虚线的时候粗度必须为1,1以上的时候有时候会很难看，有时候会出现锯齿状
				penBorder2.SetStyle(wxDOT);
				penBorder2.SetWidth(1);
			}
			else
			{
				penBorder2.SetStyle(wxSOLID);
				penBorder2.SetWidth(m_nHourLineSize);
			}*/
			//////////////////////////////////////////////////////////////////////////
			oldPen = pDC->SelectObject(&penBorder2);
			int nX = Time2X(tmDt);
			pDC->MoveTo(nX, rtClient.top);
			pDC->LineTo(nX, rtClient.bottom);	
			pDC->SelectObject(oldPen);
		}
	}

//	pDC->SelectObject(oldPen);

	int nDrawScalesCount = GetDrawScalesCount(pDC);
	CPen penBorderN(PS_SOLID,1,m_clBorder);
	oldPen = pDC->SelectObject(&penBorderN);
	for(int i = 0; i<= nDrawScalesCount; i++)
	{
		int nHeight = rtClient.bottom - i*rtClient.Height()/(nDrawScalesCount*2) ;
		pDC->MoveTo(rtClient.left+1, nHeight);
		pDC->LineTo(rtClient.right-1, nHeight);
		//dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
		nHeight = rtClient.top + i*rtClient.Height()/(nDrawScalesCount*2) ;
		pDC->MoveTo(rtClient.left+1, nHeight);
		pDC->LineTo(rtClient.right-1, nHeight);
		//dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
	}
	for(int i = 1; i<= nDrawScalesCount; i++)
	{
		double dbPrice = m_fLastClosePrice + i*( m_fMaxValue - m_fLastClosePrice)/nDrawScalesCount;
		int nHeight = JiaGe2GaoDu(dbPrice);
		//	dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
		if(nHeight <0)
			continue;
		SetTextColor(pDC, dbPrice);
		//	strText = CString::Format(wxT("%.3f"), dbPrice);
		strText = Double2String(dbPrice, m_nNumDigits, true);
		CSize cz = pDC->GetTextExtent(strText);	
		pt.x = rtClient.left - cz.cx-4;
		pt.y = nHeight - cz.cy/2; 
		rt.left   = pt.x;
		rt.top    = pt.y;
		rt.right  = rt.left + sz.cx;
		rt.bottom = rt.top + sz.cy;
		pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

		strText.Format(_T("%.2f%%"), (dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);//CString::Format(_T("%.2f%%"), (dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);
		//	strText = Double2String((dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);
		cz = pDC->GetTextExtent(strText);	
		pt.x = rtClient.right + 4;
		pt.y = nHeight - cz.cy/2; 
		rt.left   = pt.x;
		rt.top    = pt.y;
		rt.right  = rt.left + sz.cx;
		rt.bottom = rt.top + sz.cy;
		pDC->DrawText(strText, rt, DT_RIGHT|DT_TOP);

		if( i != nDrawScalesCount)
		{
			dbPrice = m_fLastClosePrice - i*( m_fLastClosePrice - m_fMinValue)/nDrawScalesCount;
			nHeight = JiaGe2GaoDu(dbPrice);
			//	dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);

			SetTextColor(pDC, dbPrice);
			//	strText = CString::Format(_T("%.3f"), dbPrice);
			strText = Double2String(dbPrice, m_nNumDigits, true);
			cz = pDC->GetTextExtent(strText);	
			pt.x = rtClient.left - cz.cx-4;
			pt.y = nHeight - cz.cy/2; 
			rt.left   = pt.x;
			rt.top    = pt.y;
			rt.right  = rt.left + sz.cx;
			rt.bottom = rt.top + sz.cy;
			pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);

			strText.Format(_T("%.2f%%"), (dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);
			//	strText = Double2String((dbPrice-m_fLastClosePrice)/m_fLastClosePrice*100);
			cz = pDC->GetTextExtent(strText);	
			pt.x = rtClient.right + 4;
			pt.y = nHeight - cz.cy/2; 
			rt.left   = pt.x;
			rt.top    = pt.y;
			rt.right  = rt.left + sz.cx;
			rt.bottom = rt.top + sz.cy;
			pDC->DrawText(strText, rt, DT_RIGHT|DT_TOP);
		}
	}

	pDC->SelectObject(oldPen);
	pDC->SelectObject(pOldFont);
}
void CTLineView::DrawPriceLines(CDC* pDC)
{
	if(m_nBegin < 0 || m_nBegin >=  (int)m_vecTLine.size()
		|| m_nEnd < 0 || m_nEnd >=  (int)m_vecTLine.size())
		return;

	CPen penLine(PS_SOLID, 1, m_clPrice);	
	CPen* pOld = pDC->SelectObject(&penLine);

	STLine pTLineEnd1 = m_vecTLine[m_nEnd];
	for( int i = m_nEnd+1; i<= m_nBegin; i++ )
	{
		int nXBegin = Time2X(pTLineEnd1.dwTime);
		int nYBegin = JiaGe2GaoDu(pTLineEnd1.fLastPrice);
		STLine pTLineBegin1 = m_vecTLine[i];
		int nXEnd = Time2X(pTLineBegin1.dwTime);
		int nYEnd = JiaGe2GaoDu(pTLineBegin1.fLastPrice);


		if(nXBegin == -1 || nXEnd == -1)
			continue;
		if(!DrawPriceLines_NoData(pDC, pTLineEnd1, pTLineBegin1))//画停牌或者没有数据的时候
		{
			pDC->MoveTo(nXBegin, nYBegin);
			pDC->LineTo(nXEnd, nYEnd);
			//dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXEnd, nYEnd));	
		}

		pTLineEnd1 = pTLineBegin1;
	}

	if(m_vecTimeScales.size() == 0)
	{
		pDC->SelectObject(pOld);
		return;
	}

	//画开始
	STLine pTLineTemp = m_vecTLine[m_nBegin];
	if(difftime(mktime(&m_vecTimeScales[0]), mktime(&pTLineTemp.dwTime)) < 0 )
	{
		int nXBegin = Time2X(pTLineTemp.dwTime);
		int nYBegin = JiaGe2GaoDu(m_fLastClosePrice);

		int nXEnd = Time2X(m_vecTimeScales[0]);
		int nYEnd = JiaGe2GaoDu(pTLineTemp.fLastPrice);
		pDC->MoveTo(nXBegin, nYBegin);
		pDC->LineTo(nXEnd, nYBegin);

		pDC->MoveTo(nXBegin, nYBegin);
		pDC->LineTo(nXBegin, nYEnd);
		//dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXEnd, nYBegin));		
		//dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXBegin, nYEnd));
	}
	pDC->SelectObject(pOld);
}
bool CTLineView::DrawPriceLines_NoData(CDC* pDC, STLine& pTLineEnd1, STLine& pTLineBegin1)//end时间近
{	
	struct tm tmBegin = pTLineEnd1.dwTime;
	tmBegin.tm_sec = 0;
	struct tm tmEnd   = pTLineBegin1.dwTime;
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
	int nXBegin = Time2X(pTLineEnd1.dwTime);
	int nYBegin = JiaGe2GaoDu(pTLineEnd1.fLastPrice);	
	int nXEnd = Time2X(pTLineBegin1.dwTime);
	int nYEnd = JiaGe2GaoDu(pTLineBegin1.fLastPrice);

	pDC->MoveTo(nXBegin, nYEnd);
	pDC->LineTo(nXEnd, nYEnd);
	pDC->MoveTo(nXBegin, nYBegin);
	pDC->LineTo(nXBegin, nYEnd);
	//dc.DrawLine(wxPoint(nXBegin, nYEnd), wxPoint(nXEnd, nYEnd));		
	//dc.DrawLine(wxPoint(nXBegin, nYBegin), wxPoint(nXBegin, nYEnd));
	pTLineEnd1 = pTLineBegin1;

	return true;
}
void CTLineView::SetTextColor(CDC* pDC, double dbClosePrice)
{
	double dbPrevious = m_fLastClosePrice;

	if(dbClosePrice - dbPrevious > -0.00001 && dbClosePrice - dbPrevious < 0.00001)
		pDC->SetTextColor(m_hengpan); 
	else if(dbClosePrice > dbPrevious)
		pDC->SetTextColor(m_zhang); 
	else if(dbClosePrice < dbPrevious)
		pDC->SetTextColor(m_die); 
}
void CTLineView::GetBeginEnd()
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
		STLine pTLine = m_vecTLine[i];
		struct tm tmTime = pTLine.dwTime;

		if(!bFindEnd && (difftime(mktime(&tmMax), mktime(&tmTime)) >= 0) )
		{
			tmMax = m_vecTimeScales[m_vecTimeScales.size() -1];//mktime 会改变里面参数的值，所以用完后都得重新赋值
			tmTime = pTLine.dwTime;
			if((difftime(mktime(&tmMin), mktime(&tmTime)) <= 0))
			{
				tmMin = m_vecTimeScales[0];
				tmTime = pTLine.dwTime;
				m_nEnd = i;
				bFindEnd = true;
			}
		}
		if(difftime(mktime(&tmMin), mktime(&tmTime)) <= 0)
		{
			tmMin = m_vecTimeScales[0];
			tmTime = pTLine.dwTime;

			if((difftime(mktime(&tmMax), mktime(&tmTime)) >= 0))
			{
				tmMax = m_vecTimeScales[m_vecTimeScales.size() -1];
				tmTime = pTLine.dwTime;
				m_nBegin = i;				
			}
		}
	}
}
void CTLineView::SetMaxMinValue(double& dbMaxValue, double& dbMinValue)
{
	if(IsDoubleSame(m_fMaxValue, MIN_VALUE_DEFAULT) && IsDoubleSame(m_fMinValue, MAX_VALUE_DEFAULT))
	{
		m_fMaxValue = m_fLastClosePrice *1.01;
		m_fMinValue = m_fLastClosePrice*0.99;	
		return;
	}

	m_fMaxValue = dbMaxValue;
	m_fMinValue = dbMinValue;	
	double dbMaxSpan = fabs(m_fMaxValue - m_fLastClosePrice);
	double dbMax2	 = fabs(m_fMinValue - m_fLastClosePrice);
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
void CTLineView::CalcMaxMin()
{
	for(int i = m_nBegin; i< (int)m_vecTLine.size() && i>= m_nEnd && i>=0; i--)
	{		
		STLine pTLine = m_vecTLine[i];
		if(pTLine.fLastPrice > m_fMaxValue)
		{
			m_fMaxValue = pTLine.fLastPrice;
		}
		if(pTLine.fLastPrice < m_fMinValue)
		{
			m_fMinValue = pTLine.fLastPrice;
		}
	}
	if(fabs(m_fMaxValue - MIN_VALUE_DEFAULT)<0.00001 && fabs(m_fMinValue-MAX_VALUE_DEFAULT)<0.00001)
	{
		m_fMaxValue = m_fLastClosePrice *1.001;
		m_fMinValue = m_fLastClosePrice*0.999;

		if(fabs(m_fMaxValue)<0.00001 && fabs(m_fMaxValue)>-0.00001)
			m_fMaxValue = 1;//没有昨结算价很麻烦
	}
	else
	{
		double dbMaxSpan = fabs(m_fMaxValue - m_fLastClosePrice);
		double dbMax2	 = fabs(m_fMinValue - m_fLastClosePrice);
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
int	CTLineView::JiaGe2GaoDu(double fJiaGe)
{
	CRect rtClient ;
	rtClient = m_rtTLine;
/*	GetClientRect(&rtClient);
	rtClient.left = rtClient.left + m_RectOffSet.left;
	rtClient.top  = rtClient.top + m_RectOffSet.top;
	rtClient.right = rtClient.right - m_RectOffSet.Width() -m_RectOffSet.left;
	rtClient.bottom = rtClient.bottom - m_RectOffSet.Height() - m_RectOffSet.top;
*/
	double ftemp = (fJiaGe - m_fMinValue)*rtClient.Height()/(m_fMaxValue - m_fMinValue)  ;
	int nn = rtClient.bottom;
	double fWeiZhi =rtClient.bottom - (fJiaGe - m_fMinValue)*rtClient.Height()/(m_fMaxValue - m_fMinValue)  ;
	int nWeiZhi = (int)fWeiZhi;
	if(fWeiZhi - nWeiZhi >0.5)
		nWeiZhi += 1;
	
	return nWeiZhi;
}
double CTLineView::GaoDu2JiaGe(int nHeight)
{
	CRect rtClient ;
	rtClient = m_rtTLine;
/*	GetClientRect(&rtClient);
	rtClient.left  = rtClient.left + m_RectOffSet.left;
	rtClient.top   = rtClient.top + m_RectOffSet.top;
	rtClient.right = rtClient.right - m_RectOffSet.Width() -m_RectOffSet.left;
	rtClient.bottom = rtClient.bottom - m_RectOffSet.Height() - m_RectOffSet.top;
*/
	double fJiaGe = (m_fMaxValue - m_fMinValue)*(rtClient.bottom - nHeight )/rtClient.Height() + m_fMinValue;
	return fJiaGe;
}
int CTLineView::Time2X(struct tm tmTime)
{
	if(m_vecTimeScales.size() == 0)
		return -1;
	tmTime.tm_sec = 0;

	CRect rtClient;
	rtClient = m_rtTLine;
	CRect rect = rtClient;
	/*GetClientRect(&rtClient);
	CRect rect = rtClient;
	rtClient.left = (rtClient.left     + m_RectOffSet.left);
	rtClient.top = (rtClient.top      + m_RectOffSet.top);
	rtClient.right = (rtClient.right   - m_RectOffSet.Width()  - m_RectOffSet.left);
	rtClient.bottom = (rtClient.bottom - m_RectOffSet.Height() - m_RectOffSet.top);
*/
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
		nx = rtClient.left;
	else
	{
		struct tm tm1 = m_vecTimeScales[nIndex-1];
		struct tm tm2 = tmTime;
		struct tm tm3 = m_vecTimeScales[nIndex];
		struct tm tm4 = m_vecTimeScales[nIndex-1];

		double db1 = difftime(mktime(&tm1), mktime(&tm2));
		double db2	= difftime(mktime(&tm3), mktime(&tm4));
		double dbPercent = db1/db2;
		if(dbPercent <0)
			dbPercent = -dbPercent;//abs函数有问题
		//double dbPercent = abs(difftime(mktime(&tm1), mktime(&tm2))*1.0 /difftime(mktime(&tm3), mktime(&tm4)));
		nx = rtClient.left + (nLeft + dbPercent * m_vecTMWidthSpan[nIndex/2])/m_TMWidthCount * rtClient.Width();	
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
struct tm CTLineView::X2Time(int nx)
{
	struct tm tTime;
	tTime.tm_year = 0;

	if( m_vecTimeScales.size() == 0)
		return tTime;

	CRect rtClient;
	rtClient = m_rtTLine;
	CRect rect = rtClient;
	/*
	GetClientRect(&rtClient);
	CRect rect = rtClient;
	rtClient.left = rtClient.left     + m_RectOffSet.left;
	rtClient.top = rtClient.top       + m_RectOffSet.top;
	rtClient.right =(rtClient.right   - m_RectOffSet.Width()  - m_RectOffSet.left);
	rtClient.bottom = (rtClient.bottom - m_RectOffSet.Height() - m_RectOffSet.top);
*/
	if(nx < rtClient.left || nx > rtClient.right+1)
		return tTime;

	double dbPercent = (nx- rtClient.left)*1.0/rtClient.Width();
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
void CTLineView::DateTime2Tm(COleDateTime dt ,struct tm& tmDt)
{
	tmDt.tm_year = dt.GetYear()-1900;
	tmDt.tm_mon  = dt.GetMonth()-1;
	tmDt.tm_mday = dt.GetDay();
	tmDt.tm_hour = dt.GetHour();
	tmDt.tm_min  = dt.GetMinute();
	tmDt.tm_sec  = dt.GetSecond();
}
void CTLineView::SetFuture(string ExchangeName,string ContractName)
{
	m_ExchangeName = ExchangeName;
	m_ContractName = ContractName;	
}
BOOL CTLineView::SetVectorData(vector<STLine>& vec)
{
	m_vecTLine.clear();
	m_vecTLine = vec;	
	GetBeginEnd();
	CalcMaxMin();
	CalcMaxMin_TVolume();
	Invalidate();

	return true;
}
void CTLineView::SetScales(vector<struct tm>& vecScales)
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
void CTLineView::SetLastClosePrice(double dbPrice)
{
	m_fLastClosePrice = dbPrice;	
}
void CTLineView::SetVolumeMultiple(double dbVolumeMultiple)
{
	m_dbVolumeMultiple = dbVolumeMultiple;
}
int	CTLineView::GetDrawScalesCount(CDC* pDC)
{
	int nCount = 0;

	CRect rtClient; 
	rtClient = m_rtTLine;
	CRect rect = rtClient;
	/*
	GetClientRect(&rtClient);
	CRect rect = rtClient;
	rtClient.left = (rtClient.left + m_RectOffSet.left);
	rtClient.top = (rtClient.top	+ m_RectOffSet.top);
	rtClient.right = (rtClient.right	- m_RectOffSet.Width()	- m_RectOffSet.left);
	rtClient.bottom = (rtClient.bottom - m_RectOffSet.Height()	- m_RectOffSet.top);
*/
	CSize sz;	
	CString strText = _T("0");
	sz = pDC->GetTextExtent(strText);
	int nHeight = rtClient.Height();

	nCount = nHeight/(sz.cy*2);

	if(nCount == 0)
		nCount =1;
	else if(nCount > 7)
		nCount = 7;


	return nCount;
}
void CTLineView::DrawAveragePriceLines(CDC* pDC)
{
	if(m_nBegin < 0 || m_nBegin >=  (int)m_vecTLine.size()
		|| m_nEnd < 0 || m_nEnd >=  (int)m_vecTLine.size())
		return;

	CPen penLine(PS_SOLID,1,m_clAveragePrice);
	CPen *oldPen = pDC->SelectObject(&penLine);

	STLine pTLineBegin = m_vecTLine[m_nBegin];
	double dbVolumeCountBegin  = 0;
	double dbdwHoldVolumeBegin = 0;
	double dbVolumeCountEnd  = pTLineBegin.dwVolume;
	double dbdwHoldVolumeEnd = pTLineBegin.dwVolumeMoney;
	for( int i = m_nBegin-1; i>= m_nEnd; i-- )
	{
		int nXBegin = Time2X(pTLineBegin.dwTime);
		int nYBegin = JiaGe2GaoDu(dbdwHoldVolumeEnd/(m_dbVolumeMultiple*dbVolumeCountEnd));

		STLine pTLineEnd = m_vecTLine[i];
		dbdwHoldVolumeBegin += pTLineEnd.dwVolumeMoney;
		dbVolumeCountBegin  += pTLineEnd.dwVolume;

		int nXEnd = Time2X(pTLineEnd.dwTime);
		int nYEnd = JiaGe2GaoDu(dbdwHoldVolumeBegin/(m_dbVolumeMultiple*dbVolumeCountBegin));

		dbdwHoldVolumeEnd	=  dbdwHoldVolumeBegin;
		dbVolumeCountEnd	=  dbVolumeCountBegin;



		if(nXBegin == -1 || nXEnd == -1)
			continue;

		if(dbVolumeCountBegin < 0.00001 && dbVolumeCountBegin> -0.00001)
			continue;
		if(dbVolumeCountEnd < 0.00001 && dbVolumeCountEnd> -0.00001)
			continue;
		pDC->MoveTo(nXBegin, nYBegin);
		pDC->LineTo(nXEnd, nYEnd);	

		pTLineBegin			=  pTLineEnd;
	}
	pDC->SelectObject(oldPen);
}
int  CTLineView::WeiZhi2XuHao(int nx)
{
	if(m_vecTLine.size() == 0)
		return -1;
	struct tm tmSel = X2Time(nx);
	if(tmSel.tm_year == 0)
		return -1;
	for(int i = m_nEnd; i<= m_nBegin && i < (int)m_vecTLine.size() && i>=0; i++)
	{
		STLine pTLine = m_vecTLine[i];
		pTLine.dwTime.tm_sec = 0;
		struct tm tm1 = pTLine.dwTime;
		struct tm tm2 = tmSel;
		if(difftime(mktime(&tm1), mktime(&tm2)) == 0)
		{
			return i;			
		}
	}	
	return -1;
}
int  CTLineView::XuHao2WeiZhi(int nItem)
{
	if(nItem > m_nBegin || nItem < m_nEnd)
		return -1;
	STLine pTLine = m_vecTLine[nItem];
	int nLeft = Time2X(pTLine.dwTime);
	return nLeft;
}
void CTLineView::OnMouseMove(UINT nFlags, CPoint point)
{
	SHitTest hitTest;
	HitTest(point, hitTest);
	if(hitTest.eType == enumSplitBar)		
	{	
		if(m_nTViewCount != 1)
			SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		if(m_MouseMode != MOUSE_PREPARE_DRAGITEM)
		{
			m_MouseMode = MOUSE_OVER_ITEM_DIVIDE;			
		}
		//else
		//	OutputDebugString(_T("OnLButtonDown\n"));
	}
	else
	{
		if(!(nFlags & MK_LBUTTON))	
		{
			m_MouseMode = MOUSE_NOTHING;
			SetCursor(::LoadCursor(NULL, IDC_ARROW));
		//	OutputDebugString(_T("OnMouseMove\n"));
		}
		else
		{
		//	OutputDebugString(_T("1111\n"));
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
void CTLineView::OnMouseHover(UINT nFlags, CPoint point)
{
	CWnd::OnMouseHover(nFlags, point);
}
void CTLineView::OnMouseLeave()
{
	m_bMouseIn = false;
	Invalidate();
/*	mousePos = ScreenToClient(mousePos);
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
	UpdateWindow((HWND)m_hWnd);*/
	CWnd::OnMouseLeave();
}

void CTLineView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	SetCapture();
	m_bMouseIn = true;
	m_bShowCross = true;
	m_bLbuttonDown = true;
	m_LbuttonDownPoint = point;

	SHitTest hitTest;
	HitTest(point, hitTest);
	if(hitTest.eType == enumSplitBar)		
	{			
		if(m_MouseMode == MOUSE_OVER_ITEM_DIVIDE)
		{
			m_MouseMode = MOUSE_PREPARE_DRAGITEM;	
			//OutputDebugString(_T("MOUSE_PREPARE_DRAGITEM\n"));
			SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		}
		else
		{
			//OutputDebugString(_T("222\n"));
		}	
	}
	else
	{
		m_MouseMode = MOUSE_NOTHING;
	//	OutputDebugString(_T("OnLButtow222222\n"));
	}
	CWnd::OnLButtonDown(nFlags, point);
}
void CTLineView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLbuttonDown = false;
	
	ReleaseCapture();
	if(m_MouseMode == MOUSE_PREPARE_DRAGITEM)
	{
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
		if(m_LbuttonDownPoint != point) 
		{
			if(m_nTViewCount == 2)
			{
				int nChange =  point.y - m_LbuttonDownPoint.y;
				if(nChange > 0)
				{//往下移
					if(nChange> m_rtTVolume.Height() -m_nMaxSpan)
						nChange = m_rtTVolume.Height() -m_nMaxSpan;

					m_rtTLine.bottom += nChange;
					m_rtTVolume.top  += nChange;
				}
				else				
				{//往上移
					if(nChange> m_rtTLine.Height() -m_nMaxSpan)
						nChange = m_rtTLine.Height() -m_nMaxSpan;

					m_rtTLine.bottom += nChange;
					m_rtTVolume.top  += nChange;
				}
				Invalidate();
			}
		}
		m_MouseMode = MOUSE_NOTHING;
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void  CTLineView::SItemGetValue(bool	bIsMouse, bool bIsKeyBoard)
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
void CTLineView::DrawTips(CDC* pDC)
{
	SHitTest hitTest;
	HitTest(m_ptMouseMoveLast, hitTest);
	if(hitTest.eType == enumSplitBar)
		return;

	CRect rtClient;
	rtClient = m_rtTLine;

	CPen penBorder(PS_SOLID,1,m_clCurSelLine);
	CPen *oldPen = pDC->SelectObject(&penBorder);
	
	if(m_ptMouseMoveLast.x >= rtClient.left	&& m_ptMouseMoveLast.x <= rtClient.right)
	{		
		if(m_bShowTime)
		{
			DrawTips_Time(pDC, m_rtScales, m_ptMouseMoveLast.x, m_bIsMouse);
		}
	}
	if(m_ptMouseMoveLast.y >= m_rtTLine.top && m_ptMouseMoveLast.y <= m_rtTLine.bottom)
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
				if(m_nCurSel < m_nEnd || m_nCurSel >= (int)m_vecTLine.size())
					return;
				STLine pKLine = m_vecTLine[m_nCurSel];
				dbPrice = pKLine.fLastPrice;
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

			int nLeft =  rtClient.right+1;
			int nRectTop  = m_ptMouseMoveLast.y - cz.cy + 1;
			if(nRectTop <= 0)
				nRectTop = 0;
			
			CRect rc(nLeft, nRectTop, nLeft + cz.cx, nRectTop+cz.cy);
			CBrush brush(m_clTipsBk);
			pDC->FillRect(rc, &brush);
			CBrush brush2(m_zhang);
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
			CRect rt2;
			rt2.left   = nLeft;
			rt2.top    = nRectTop;
			rt2.right  = rt2.left + sz.cx;
			rt2.bottom = rt2.top + sz.cy;
			
			pDC->FillRect(rt2, &brush);				
			pDC->FrameRect(rt2, &brush2);
			rt2.left   = nLeft+2;
			pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);

			
			pDC->SelectObject(pOldFont);

		}
	}
	else if(m_ptMouseMoveLast.y >= m_rtTVolume.top && m_ptMouseMoveLast.y <= m_rtTVolume.bottom)
	{
		if(m_bShowHeight)
		{
			if((m_fMaxValue_TVolume- MIN_VALUE_DEFAULT > -0.00001 && m_fMaxValue_TVolume- MIN_VALUE_DEFAULT< 0.00001)
				||(m_fMinValue_TVolume- MAX_VALUE_DEFAULT > -0.00001 && m_fMinValue_TVolume- MAX_VALUE_DEFAULT< 0.00001))
			{
				pDC->SelectObject(oldPen);
				return;
			}

			double dbPrice = 0.0;
			if(m_bIsKeyBoard)
			{
				if(m_nCurSel < m_nEnd || m_nCurSel >= (int)m_vecTLine.size())
				{
					pDC->SelectObject(oldPen);
					return;
				}
				STLine pKLine = m_vecTLine[m_nCurSel];
				dbPrice = pKLine.dwVolume;
			}
			if(m_bIsMouse)
				dbPrice = GaoDu2JiaGe_TVolume(m_ptMouseMoveLast.y);

			CFont *pOldFont=pDC->SelectObject(&m_FontTip);

			CSize cz;
			int nTop = rtClient.top - 10;	

			CString strText = Double2String_Tip(dbPrice);
			cz = pDC->GetTextExtent(strText);	

			CString strLength = _T("1234.56");
			cz = pDC->GetTextExtent(strLength);	
			cz.cx = cz.cx + 4;

			int nLeft =  rtClient.right+1;
			int nRectTop  = m_ptMouseMoveLast.y - cz.cy + 1;
			if(nRectTop <= 0)
				nRectTop = 0;

			CRect rc(nLeft, nRectTop, nLeft + cz.cx, nRectTop+cz.cy);
			CBrush brush(m_clTipsBk);
			pDC->FillRect(rc, &brush);
			CBrush brush2(m_zhang);
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
			CRect rt2;
			rt2.left   = nLeft;
			rt2.top    = nRectTop;
			rt2.right  = rt2.left + sz.cx;
			rt2.bottom = rt2.top + sz.cy;

			pDC->FillRect(rt2, &brush);				
			pDC->FrameRect(rt2, &brush2);
			rt2.left   = nLeft+2;
			pDC->DrawText(strText, rt2, DT_LEFT|DT_TOP);


			pDC->SelectObject(pOldFont);
		}
	}
	pDC->SelectObject(oldPen);
}

void CTLineView::DrawTips_Time(CDC* pDC, CRect rtClient, int nX, bool bIsMouse)
{
	CString strTextBottom;
	if(bIsMouse)
	{//主要是盘中停盘，没有数据
		struct tm dwTime = X2Time(nX);
		strTextBottom.Format(_T("%.2d:%.2d"), dwTime.tm_hour, dwTime.tm_min);
		if (strTextBottom.GetLength() < 4)
			return;
	}
	else
	{
		if(m_nCurSel< m_nEnd || m_nCurSel > m_nBegin || m_nCurSel <0 || m_nCurSel>= (int)m_vecTLine.size())
			return;
		STLine pTLine = m_vecTLine[m_nCurSel];	
		strTextBottom.Format(_T("%.2d:%.2d"), pTLine.dwTime.tm_hour, pTLine.dwTime.tm_min);
		if (strTextBottom.GetLength() < 4)
			return;
	}
	CFont *pOldFont=pDC->SelectObject(&m_FontTip);	
	CSize cz;
	int nTop = rtClient.top;	
	cz = pDC->GetTextExtent(strTextBottom);	

	int nLeft =  nX+1;
	if(nX + cz.cx > rtClient.right)
		nLeft = nX -cz.cx-1;
	int nRectTop  = nTop;

	CRect rc(nLeft, nRectTop+1, nLeft + cz.cx, rtClient.bottom);
	CBrush brush(m_clTipsBk);
	pDC->FillRect(rc, &brush);

	CBrush brush2(m_zhang);
	pDC->FrameRect(rc, &brush2);
	pDC->SetTextColor(m_clCurSelLine); 
	//CSize sz = pDC->GetTextExtent(strTextBottom);
	
	pDC->DrawText(strTextBottom, rc, DT_LEFT|DT_VCENTER);
	pDC->SelectObject(pOldFont);
}
LRESULT CTLineView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_KEYDOWN  )
	{
		if(wParam == VK_LEFT)
		{	
			if(m_vecTLine.size() ==0 )
				return CWnd::DefWindowProc(message,  wParam,  lParam); 
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;

			if(m_nCurSel== m_vecTLine.size()-1)
			{
				int nLeft =0;
				nLeft = XuHao2WeiZhi(m_nCurSel);			
				if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
				{
					STLine pKLine = m_vecTLine[m_nCurSel];
					double fClosePrice = pKLine.fLastPrice;
					int nHeight = JiaGe2GaoDu(fClosePrice);				
					m_ptMouseMoveLast = CPoint(nLeft, nHeight);
				}
				SItemGetValue(false, true);
				/*	wxWindow* window= GetParent();
				wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
				myEvent.SetInt(nkeyCode);
				myEvent.SetClientData((void*)&m_sItem);
				window->ProcessEvent(myEvent);
				*/	return CWnd::DefWindowProc(message,  wParam,  lParam); 
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
			Invalidate();
			int nLeft =0, nRight =0;
			nLeft = XuHao2WeiZhi(m_nCurSel);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
			{
				STLine pTLine = m_vecTLine[m_nCurSel];
				double fClosePrice = pTLine.fLastPrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = CPoint(nLeft, nHeight);
			}
			SItemGetValue(false, true);
			/*	wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);
			*/
			return  CWnd::DefWindowProc(message,  wParam,  lParam); 
		}
		if(wParam == VK_RIGHT)
		{
			if(m_vecTLine.size() ==0 )
				return CWnd::DefWindowProc(message,  wParam,  lParam); 
			m_bIsMouse		= false;
			m_bIsKeyBoard	= true;	
			if(m_nCurSel==0)
			{
				int nLeft =0;
				nLeft = XuHao2WeiZhi(m_nCurSel);			
				if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
				{
					STLine pKLine = m_vecTLine[m_nCurSel];
					double fClosePrice = pKLine.fLastPrice;
					int nHeight = JiaGe2GaoDu(fClosePrice);				
					m_ptMouseMoveLast = CPoint(nLeft, nHeight);
				}
				SItemGetValue(false, true);
				/*		wxWindow* window= GetParent();
				wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
				myEvent.SetInt(nkeyCode);
				myEvent.SetClientData((void*)&m_sItem);
				window->ProcessEvent(myEvent);
				*/		return CWnd::DefWindowProc(message,  wParam,  lParam); 
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
				return CWnd::DefWindowProc(message,  wParam,  lParam); 
			if(m_nCurSel == -1)
				return CWnd::DefWindowProc(message,  wParam,  lParam); 
			//CalcMaxMin();
			m_bShowCross = true;
			Invalidate();

			int nLeft =0;
			nLeft = XuHao2WeiZhi(m_nCurSel);			
			if(m_nCurSel >=0 && m_nCurSel < (int)m_vecTLine.size())
			{
				STLine pKLine = m_vecTLine[m_nCurSel];
				double fClosePrice = pKLine.fLastPrice;
				int nHeight = JiaGe2GaoDu(fClosePrice);				
				m_ptMouseMoveLast = CPoint(nLeft, nHeight);
			}
			SItemGetValue(false, true);
			/*	wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_TCtrl_KeyDown);
			myEvent.SetInt(nkeyCode);
			myEvent.SetClientData((void*)&m_sItem);
			window->ProcessEvent(myEvent);
			*/	
			return TRUE;
		}
		if(wParam == VK_DOWN)
		{
			return TRUE;
		}
		else if(wParam == VK_UP)
		{	
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
			int n = 0;
			GetParent()->SendMessage(MESSAGE_RETURN,(WPARAM)&n,0);			
			return 1L;
			//return CWnd::DefWindowProc(message,  wParam,  lParam); ;
		}	
		else
		{
			KillTimer(m_uTimerarr[TIMER_2]);
			KillTimer(m_uTimerarr[TIMER_1]);
			/*	BYTE bCtrl = HIBYTE(GetKeyState(VK_CONTROL));
			if (pMsg->wParam == VK_MENU &&bCtrl)
			{
			SItemGetValue();
			GetParent()->SendMessage(Message_JiShuZhiBiao,(WPARAM)&m_sItem, NULL);
			SetFocus();
			}
			else				
			GetParent()->SendMessage(Message_JiangPanTiShi,(WPARAM)&m_sItem,(LPARAM) pMsg);
			*/

		}
	}
	LRESULT lr = CWnd::DefWindowProc(message, wParam, lParam);
	return lr;
}

BOOL CTLineView::PreTranslateMessage(MSG* pMsg) 
{
	return CWnd::PreTranslateMessage(pMsg);
}
void CTLineView::OnSize(UINT nType, int cx, int cy)
{
	if(m_nTViewCount == 1)
		OnView1();
	else if(m_nTViewCount == 2)
		OnView2();

	CWnd::OnSize(nType, cx, cy);
}
void CTLineView::OnView1()
{
	CRect rtClient;
	GetClientRect(&rtClient);	

	//刻度矩形区域
	m_rtScales = rtClient;
	m_rtScales.top = m_rtScales.bottom - 18;//刻度高度

	rtClient.DeflateRect(m_RectOffSet);

	//分时图矩形区域
	m_rtTLine = rtClient;
	m_rtTLine.bottom = m_rtScales.top;

}
void CTLineView::OnView2()
{
	CRect rtClient;
	GetClientRect(&rtClient);
	

	//刻度矩形区域
	m_rtScales = rtClient;
	m_rtScales.top = m_rtScales.bottom - 18; //刻度高度

	rtClient.DeflateRect(m_RectOffSet);

	//分时图矩形区域
	m_rtTLine = rtClient;
	m_rtTLine.bottom = m_rtScales.top;
	m_rtTLine.bottom = m_rtTLine.top + m_rtTLine.Height()*m_dbTLine/(m_dbTLine + m_dbTVolume);
	
	//量能图矩形区域

	m_rtTVolume = rtClient;
	m_rtTVolume.bottom = m_rtScales.top;
	m_rtTVolume.top = m_rtTVolume.top + m_rtTVolume.Height()*m_dbTLine/(m_dbTLine + m_dbTVolume);

}
void CTLineView::DrawBitmap_TVolume(CDC *pDC)
{	
	DrawScales_TVolume(pDC);
	DrawVolumeLine_TVolume(pDC);
}
void CTLineView::DrawScales_TVolume(CDC* pDC)
{
	if(m_vecTimeScales.size() == 0)
		return;

	CRect rtClient,rect;
	//GetClientRect(&rtClient);	
	rtClient = m_rtTVolume;

	CFont *pOldFont=pDC->SelectObject(&m_FontScales);

	CString  strText;
	CPoint pt;
	CRect rt;
	//CPen penBorder(PS_SOLID,3,m_clBorder);
	
	//oldPen = pDC->SelectObject(&penBorder);

	CPen penBorder2(PS_SOLID,2,m_clBorder);
	CPen* pOldPen = pDC->SelectObject(&penBorder2);
	for(int i = 1; i< (int)m_vecTimeScales.size()-1; i = i + 2)//-1是不希望最后一根画竖线
	{  
		int nX = Time2X(m_vecTimeScales[i]);	
		pDC->MoveTo(nX, rtClient.top);
		pDC->LineTo(nX, rtClient.bottom);				
	}
	pDC->SelectObject(pOldPen);

	struct tm tmBegin = m_vecTimeScales[0];
	struct tm tmEnd   = m_vecTimeScales[m_vecTimeScales.size() - 1];
	COleDateTime dateBegin(tmBegin.tm_year+1900, tmBegin.tm_mon+1, tmBegin.tm_mday, tmBegin.tm_hour, tmBegin.tm_min, tmBegin.tm_sec);
	COleDateTime dateEnd(tmEnd.tm_year+1900, tmEnd.tm_mon+1, tmEnd.tm_mday, tmEnd.tm_hour, tmEnd.tm_min, tmEnd.tm_sec);

	for(COleDateTime dt = dateBegin; dt<dateEnd; dt += COleDateTimeSpan(0, 0, 15, 0))
	{
		int nMin = dt.GetMinute();
		int sss = dt.GetYear();
		if(nMin == 30)
		{
			struct tm tmDt;
			DateTime2Tm(dt, tmDt);
			CPen penBorder2(PS_DOT,1,m_clBorder);
			//////////////////////////////////////////////////////////////////////////
			/*	if(m_nHalfHourLineLineType == 0)
			{//虚线的时候粗度必须为1,1以上的时候有时候会很难看，有时候会出现锯齿状
			penBorder2.SetStyle(wxDOT);
			penBorder2.SetWidth(1);
			}
			else
			{
			penBorder2.SetStyle(wxSOLID);
			penBorder2.SetWidth(m_nHalfHourLineSize);	
			}*/
			//////////////////////////////////////////////////////////////////////////
			CPen* pOldPen = pDC->SelectObject(&penBorder2);
			int nX = Time2X(tmDt);
			pDC->MoveTo(nX, rtClient.top);
			pDC->LineTo(nX, rtClient.bottom);	
			pDC->SelectObject(pOldPen);
		}
		else if(nMin == 0)
		{
			struct tm tmDt;
			DateTime2Tm(dt, tmDt);		
			CPen penBorder2(PS_SOLID,1,m_clBorder);
			//////////////////////////////////////////////////////////////////////////
			/*		if(m_nHourLineType == 0)
			{//虚线的时候粗度必须为1,1以上的时候有时候会很难看，有时候会出现锯齿状
			penBorder2.SetStyle(wxDOT);
			penBorder2.SetWidth(1);
			}
			else
			{
			penBorder2.SetStyle(wxSOLID);
			penBorder2.SetWidth(m_nHourLineSize);
			}*/
			//////////////////////////////////////////////////////////////////////////
			CPen* pOldPen = pDC->SelectObject(&penBorder2);
			int nX = Time2X(tmDt);
			pDC->MoveTo(nX, rtClient.top);
			pDC->LineTo(nX, rtClient.bottom);	
			pDC->SelectObject(pOldPen);
		}
	}
/*
	int nDrawScalesCount = GetDrawScalesCount(pDC);
	CPen penBorderN(PS_SOLID,1,m_clBorder);
	pDC->SelectObject(&penBorderN);
	for(int i = 0; i<= nDrawScalesCount; i++)
	{
		int nHeight = rtClient.bottom - i*rtClient.Height()/(nDrawScalesCount*2) ;
		pDC->MoveTo(rtClient.left+1, nHeight);
		pDC->LineTo(rtClient.right-1, nHeight);
		//dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
		nHeight = rtClient.top + i*rtClient.Height()/(nDrawScalesCount*2) ;
		pDC->MoveTo(rtClient.left+1, nHeight);
		pDC->LineTo(rtClient.right-1, nHeight);
		//dc.DrawLine(rtClient.GetLeft()+1, nHeight, rtClient.GetRight()-1, nHeight);
	}*/
	int nDrawScalesCount = GetDrawScalesCount(pDC);
	CPen penBorderN(PS_SOLID,1,m_clBorder);
	CPen* oldPen = pDC->SelectObject(&penBorderN);		
	pDC->SetTextColor(m_clBorder); 
	for(int i = 1; i< nDrawScalesCount; i++)
	{
		double dbPrice = i* m_fMaxValue_TVolume /nDrawScalesCount;
		int nHeight = JiaGe2GaoDu_TVolume(dbPrice);
		
		pDC->MoveTo(rtClient.left+1, nHeight);
		pDC->LineTo(rtClient.right-1, nHeight);
		

		//SetTextColor(dc, dbPrice);
		if(dbPrice <= 10 && dbPrice>0)
			strText.Format(_T("%.2f"), dbPrice);
		else
			strText.Format(_T("%.0f"), dbPrice);
		CSize cz = pDC->GetTextExtent(strText);	
		pt.x = rtClient.left - cz.cx-4;
		if(i == nDrawScalesCount)
			pt.y = nHeight; 
		else
			pt.y = nHeight - cz.cy/2; 

		CRect rt;
		rt.left   = pt.x;
		rt.top    = pt.y;
		rt.right  = rt.left + cz.cx;
		rt.bottom = rt.top + cz.cy;

		pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);		
	}
	

	pDC->SelectObject(oldPen);
	pDC->SelectObject(pOldFont);
}
void CTLineView::CalcMaxMin_TVolume()
{
	m_fMaxValue_TVolume  = MIN_VALUE_DEFAULT;
	m_fMinValue_TVolume  = 0;
	for(int i = m_nBegin; i< (int)m_vecTLine.size() && i>= m_nEnd  && i>=0; i--)
	{
		STLine pTLine = m_vecTLine[i];
		if(pTLine.dwVolume > m_fMaxValue_TVolume)
		{
			m_fMaxValue_TVolume = pTLine.dwVolume;
		}		
	}
	if(m_fMaxValue_TVolume - MIN_VALUE_DEFAULT > -0.00001 && m_fMaxValue_TVolume - MIN_VALUE_DEFAULT < 0.00001 && m_fMinValue_TVolume  == 0)
	{
		m_fMaxValue_TVolume = 1;		
	}	
}
int	CTLineView::JiaGe2GaoDu_TVolume(double fJiaGe)
{
	CRect rtClient ;
	rtClient = m_rtTVolume;

	double ftemp = (fJiaGe - m_fMinValue_TVolume)*rtClient.Height()/(m_fMaxValue_TVolume - m_fMinValue_TVolume)  ;
	int nn = rtClient.bottom;
	double fWeiZhi =rtClient.bottom - (fJiaGe - m_fMinValue_TVolume)*rtClient.Height()/(m_fMaxValue_TVolume - m_fMinValue_TVolume)  ;
	int nWeiZhi = (int)fWeiZhi;
	if(fWeiZhi - nWeiZhi >0.5)
		nWeiZhi += 1;

	return nWeiZhi;
}
double	CTLineView::GaoDu2JiaGe_TVolume(int nHeight)
{
	CRect rtClient ;
	rtClient = m_rtTVolume;

	double fJiaGe = (m_fMaxValue_TVolume - m_fMinValue_TVolume)*(rtClient.bottom - nHeight )/rtClient.Height() + m_fMinValue_TVolume;
	return fJiaGe;
}
void CTLineView::DrawVolumeLine_TVolume(CDC* pDC)
{
	if(m_vecTLine.size() == 0
		|| m_nEnd < 0 || m_nEnd >= (int)m_vecTLine.size() 
		|| m_nBegin < 0 || m_nBegin >= (int)m_vecTLine.size() )
		return;
	CRect rtClient = m_rtTVolume;

	CPen penBorderN(PS_SOLID,1,m_clAveragePrice);
	CPen *oldPen = pDC->SelectObject(&penBorderN);	
	for( int i = m_nEnd; i<= m_nBegin; i++ )
	{
		STLine pTLineEnd = m_vecTLine[i];
		int nXEnd = Time2X(pTLineEnd.dwTime);
		int nYEnd = JiaGe2GaoDu_TVolume(pTLineEnd.dwVolume);
		pDC->MoveTo(nXEnd, nYEnd);
		pDC->LineTo(nXEnd, rtClient.bottom);			
	}

	pDC->SelectObject(oldPen);
}
void CTLineView::DrawBitmap_TScales(CDC* pDC)
{
	if(m_vecTimeScales.size() == 0)
		return;

	CRect rtClient = m_rtScales;

	struct tm tmBegin = m_vecTimeScales[0];
	struct tm tmEnd   = m_vecTimeScales[m_vecTimeScales.size() - 1];
	COleDateTime dateBegin(tmBegin.tm_year+1900, tmBegin.tm_mon+1, tmBegin.tm_mday, tmBegin.tm_hour, tmBegin.tm_min, tmBegin.tm_sec);
	COleDateTime dateEnd(tmEnd.tm_year+1900, tmEnd.tm_mon+1, tmEnd.tm_mday, tmEnd.tm_hour, tmEnd.tm_min, tmEnd.tm_sec);

	CFont *pOldFont = pDC->SelectObject(&m_FontBottomScales);	
	pDC->SetTextColor(m_clName); 	

	CString  strText;
	CPoint pt;
	for(int i = 1; i< (int)m_vecTimeScales.size(); i = i + 2)//-1是不希望最后一根画竖线
	{  		
		int nX = Time2X(m_vecTimeScales[i]);		
		struct tm tm1 = m_vecTimeScales[i];
		CString  strText;
		CSize cz;
		CPoint pt;
		strText.Format(_T("%.2d:%.2d"), tm1.tm_hour, tm1.tm_min);	
		cz = pDC->GetTextExtent(strText);	

		pt.x = nX -cz.cx/2;
		if(pt.x < rtClient.left + m_RectOffSet.left)
			pt.x = rtClient.left+ m_RectOffSet.left;
		pt.y = rtClient.top + (rtClient.Height() -cz.cy)/2 ; 

		CRect rt;
		rt.left   = pt.x;
		rt.top    = pt.y;
		rt.right  = rt.left + cz.cx;
		rt.bottom = rt.top + cz.cy;
		pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);		

	}
	int nFirstEnd = 0;
	for(COleDateTime dt = dateBegin; dt<dateEnd; dt += COleDateTimeSpan(0, 0, 15, 0))
	{
		int nMin = dt.GetMinute();
		if(dt == dateBegin)
		{//开始
			struct tm tmDt;
			DateTime2Tm(dt, tmDt);
			int nX = Time2X(tmDt);
			CString  strText;
			CSize cz;
			CPoint pt;
			strText.Format(_T("%.4d/%.2d/%.2d,%.2d:%.2d"), dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute());	
			cz = pDC->GetTextExtent(strText);	

			pt.x = rtClient.left -cz.cx/2;
			if(pt.x < rtClient.left)
				pt.x = rtClient.left;
			pt.y = rtClient.top + (rtClient.Height() -cz.cy)/2 ; 
			CRect rt;
			rt.left   = pt.x;
			rt.top    = pt.y;
			rt.right  = rt.left + cz.cx;
			rt.bottom = rt.top + cz.cy;
			pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);		

			nFirstEnd = pt.x +cz.cx;

		}
		else if(m_TMWidthCount >= 43200)//超过12小时的交易时间;执行不同画法
		{
			for(int i = 0; i <4; i++)
			{			
				int nSpan2 = m_TMWidthCount/4;
				struct tm tmOneSpan2 = GetDrawTime(m_vecTimeScales, m_vecTMWidthSpan, nSpan2+i*nSpan2);
				int nX = Time2X(tmOneSpan2);
				if(nX == -1)
					continue;
				CString  strText;
				CSize cz;
				CPoint pt;
				strText.Format(_T("%.2d:%.2d"), tmOneSpan2.tm_hour, tmOneSpan2.tm_min);	
				cz = pDC->GetTextExtent(strText);	

				pt.x = nX -cz.cx/2;
				if(pt.x < rtClient.left)
					pt.x = rtClient.left;
				pt.y = rtClient.top + (rtClient.Height() - cz.cy)/2 ; 

				if(pt.x > nFirstEnd)
				{
					CRect rt;
					rt.left   = pt.x;
					rt.top    = pt.y;
					rt.right  = rt.left + cz.cx;
					rt.bottom = rt.top + cz.cy;
					pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);		

				}
			}		
		}
		else if(nMin == 0 && dt != dateBegin)
		{//整点
			struct tm tmDt;
			DateTime2Tm(dt, tmDt);
			int nX = Time2X(tmDt);
			if(nX == -1)
				continue;
			CString  strText;
			CSize cz;
			CPoint pt;
			strText.Format(_T("%.2d:%.2d"), dt.GetHour(), dt.GetMinute());	
			cz = pDC->GetTextExtent(strText);	

			pt.x = nX -cz.cx/2;
			if(pt.x < rtClient.left + m_RectOffSet.left)
				pt.x = rtClient.left + m_RectOffSet.left;
			pt.y = rtClient.top + (rtClient.Height() -cz.cy)/2 ; 

			if(pt.x > nFirstEnd)
			{
				CRect rt;
				rt.left   = pt.x;
				rt.top    = pt.y;
				rt.right  = rt.left + cz.cx;
				rt.bottom = rt.top + cz.cy;
				pDC->DrawText(strText, rt, DT_LEFT|DT_TOP);		
			}
		}
	}	
	pDC->SelectObject(pOldFont);	
}
struct tm CTLineView::GetDrawTime(vector<struct tm>& vecTimeScales, vector<DWORD>& vecTMWidthSpan, DWORD dwSpan)
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
bool  CTLineView::HitTest(CPoint pt, SHitTest& hitTest)
{
	CRect rtClient;
	GetClientRect(&rtClient);
	if(pt.x > rtClient.left && pt.x < rtClient.right
		&& pt.y >= m_rtTLine.bottom && pt.y <= m_rtTLine.bottom+m_nResizeRange)
	{
		hitTest.eType = enumSplitBar;
		hitTest.nIndex = 0;
		return true;
	}
	if(m_rtTLine.PtInRect(pt))
	{
		hitTest.eType = enumTLine;
		return true;
	}
	if(m_rtTVolume.PtInRect(pt))
	{
		hitTest.eType = enumTVolume;
		return true;
	}
	if(m_rtScales.PtInRect(pt))
	{
		hitTest.eType = enumScales;
		return true;
	}	
	return false;
}
BOOL CTLineView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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
void CTLineView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int n = 0;
	GetParent()->SendMessage(MESSAGE_LEFTDBCLICK,(WPARAM)&n,0);
	CWnd::OnLButtonDblClk(nFlags, point);
}
BOOL CTLineView::SetInTimeData(CFData& fData)
{
	SetMaxMinValue(fData.m_fMaxValue, fData.m_fMinValue);
	std::vector<STLine> vec;
	fData.GetVecTLine(vec);
	SetVectorData(vec);	
	
	return true;
}
void CTLineView::OnRButtonUp(UINT nFlags, CPoint point)
{
	CMenu menu1;
	menu1.CreatePopupMenu();     //动态创建弹出式菜单对象
	menu1.AppendMenu(MF_STRING,IDD_MENU_TOKLINE,"K线视图");	

	CPoint pt;
	GetCursorPos(&pt);
	menu1.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	menu1.DestroyMenu();

	return CWnd::OnRButtonUp(nFlags, point);
}
void CTLineView::OnToKLine()
{
	int n = 0;
	GetParent()->SendMessage(MESSAGE_CHANGEVIEW,(WPARAM)&n,0);
	
}