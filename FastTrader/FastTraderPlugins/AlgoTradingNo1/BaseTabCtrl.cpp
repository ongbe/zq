// BaseTabCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "tabcontrol.h"
#include "BaseTabCtrl.h"
#include "crm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl

CBaseTabCtrl::CBaseTabCtrl(int nType)
{
	m_crBack = (COLORREF)-1; // use default color
	m_nDrawType = BTC_NONE;

	EnableDraw(nType);
}

CBaseTabCtrl::~CBaseTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CBaseTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CBaseTabCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseTabCtrl message handlers

void CBaseTabCtrl::SetBkgndColor(COLORREF color)
{
	// set new color
	m_crBack = color;

	// redraw
	if (GetSafeHwnd())
		Invalidate();
}

BOOL CBaseTabCtrl::EnableDraw(int nType)
{
	ASSERT (nType >= BTC_NONE && nType <= BTC_ALL);

	if (nType < BTC_NONE || nType > BTC_ALL)
		return FALSE;

	m_nDrawType = nType;

	if (GetSafeHwnd())
	{
		if (m_nDrawType != BTC_NONE) 
			ModifyStyle(0, TCS_OWNERDRAWFIXED);
		else
			ModifyStyle(TCS_OWNERDRAWFIXED, 0);

		Invalidate();
	}

	return TRUE;
}

void CBaseTabCtrl::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

void CBaseTabCtrl::DrawItemBorder(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

void CBaseTabCtrl::DrawMainBorder(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	ASSERT (0);
}

COLORREF CBaseTabCtrl::GetTabColor(BOOL /*bSelected*/)
{
	return ::GetSysColor(COLOR_3DFACE);
}

COLORREF CBaseTabCtrl::GetTabTextColor(BOOL /*bSelected*/)
{
	return ::GetSysColor(COLOR_WINDOWTEXT);
}

void CBaseTabCtrl::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();

	if (m_nDrawType != BTC_NONE) 
		ModifyStyle(0, TCS_OWNERDRAWFIXED);
}

BOOL CBaseTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CRect rClient, rTab, rTotalTab, rBkgnd, rEdge;
	COLORREF crBack;
	int nTab, nTabHeight = 0;

															//add by l. 20071109
	if(1)
	{
		CRect	ClientRect,SelTabRect,TmpRect;
		int		SelOrd;
		CDC		iMemDC;
//		HWND	iMainWnd;
//		POINT	Pt;
		
		GetClientRect(ClientRect);
		SelOrd=GetCurSel();
		if(SelOrd<0) SelOrd=0;
		GetItemRect(SelOrd,SelTabRect);

		if(theApp.m_ViewWnd)												
		{
			theApp.PaintWith_m_mainfrmbackground(pDC->GetSafeHdc(),m_hWnd);	
			if(SelTabRect.bottom>0) ClientRect.top=SelTabRect.bottom;
			else ClientRect.top=19;
		}

		pDC->FillSolidRect(ClientRect, theApp.m_clrBack);//RGB(218,238,235));	//modify by l. 20071121


//		TmpRect=ClientRect;
//		if(SelTabRect.bottom>0) TmpRect.bottom=SelTabRect.bottom;
//		else TmpRect.bottom=TmpRect.top+19;
//		iMainWnd=GetParent()->GetParent()->m_hWnd;
//		if(iMainWnd)
//		{

//			Pt.x=TmpRect.left;
//			Pt.y=TmpRect.top;
//			::ClientToScreen(m_hWnd,&Pt);
//			::ScreenToClient(iMainWnd,&Pt);
//			iMemDC.CreateCompatibleDC(pDC);
//			iMemDC.SelectObject(theApp.m_Bmp_L01_mainfrmbackground);
//			pDC->BitBlt(TmpRect.left,TmpRect.top,TmpRect.right-TmpRect.left,TmpRect.bottom-TmpRect.top,
//						 &iMemDC,Pt.x,Pt.y,SRCCOPY);
//			iMemDC.DeleteDC();
//		}

		return TRUE;
	}
															//end add by l. 20071109

	CTabCtrl::OnEraseBkgnd(pDC);							

	// calc total tab width
	GetClientRect(rClient);
	nTab = GetItemCount();
	rTotalTab.SetRectEmpty();

	DWORD style = GetStyle();
// 	ModifyStyle()

	while (nTab--)
	{
		GetItemRect(nTab, rTab);
		rTotalTab.UnionRect(rTab, rTotalTab);
	}

	nTabHeight = rTotalTab.Height();

	// add a bit
	rTotalTab.InflateRect(2, 3);
	rEdge = rTotalTab;

	// then if background color is set, paint the visible background
	// area of the tabs in the bkgnd color
	// note: the mfc code for drawing the tabs makes all sorts of assumptions
	// about the background color of the tab control being the same as the page
	// color - in some places the background color shows thru' the pages!!
	// so we must only paint the background color where we need to, which is that
	// portion of the tab area not excluded by the tabs themselves
	crBack = (m_crBack == -1) ? ::GetSysColor(COLOR_3DFACE) : m_crBack;
	
	// full width of tab ctrl above top of tabs
	rBkgnd = rClient;
	rBkgnd.bottom = rTotalTab.top + 3;
	pDC->SetBkColor(crBack);
//	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);
	
	// width of tab ctrl visible bkgnd including bottom pixel of tabs to left of tabs
	rBkgnd = rClient;
	rBkgnd.right = 2;
	rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
//	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);
	
	// to right of tabs
	rBkgnd = rClient;
	rBkgnd.left += rTotalTab.Width() - 2;
	rBkgnd.bottom = rBkgnd.top + (nTabHeight + 2);
//	pDC->ExtTextOut(rBkgnd.left, rBkgnd.top, ETO_CLIPPED | ETO_OPAQUE, rBkgnd, "", NULL);

	return TRUE;
}



void CBaseTabCtrl::OnPaint() 
{
	if (m_nDrawType == BTC_NONE)
	{
		Default();
	}
	else if (m_nDrawType == BTC_TABS)
	{
		ASSERT (GetStyle() & TCS_OWNERDRAWFIXED);
		Default();

															//add by l. 20071109
		if(1)
		{
			int SelOrd;

			SelOrd=GetCurSel();
			if(SelOrd>=0)
			{
				CRect		iiRect;
				HDC			hDC;
				HPEN		hPen,OldPen;
				hDC=::GetDC(m_hWnd);
				GetItemRect(SelOrd,&iiRect);
				hPen=::CreatePen(PS_SOLID,1,theApp.m_clrBack);//	RGB(218,238,235));	//rem by l. 20071121
				OldPen=(HPEN)::SelectObject(hDC,hPen);
				::MoveToEx(hDC,iiRect.left,iiRect.bottom+1,NULL);
				::LineTo(hDC,iiRect.right,iiRect.bottom+1);
				::SelectObject(hDC,OldPen);
				::ReleaseDC(m_hWnd,hDC);
				::DeleteObject(hPen);
			}
		}

															//end add by l. 20071109

	}
	else // all
	{
		CPaintDC dc(this); // device context for painting

		// prepare dc
		dc.SelectObject(GetFont());

		DRAWITEMSTRUCT dis;
		dis.CtlType = ODT_TAB;
		dis.CtlID = GetDlgCtrlID();
		dis.hwndItem = GetSafeHwnd();
		dis.hDC = dc.GetSafeHdc();
		dis.itemAction = ODA_DRAWENTIRE;

		// draw the rest of the border
		CRect rClient, rPage;
		GetClientRect(&dis.rcItem);
		rPage = dis.rcItem;
		AdjustRect(FALSE, rPage);
		dis.rcItem.top = rPage.top - 2;

//		DrawMainBorder(&dis);

		// paint the tabs first and then the borders
		int nTab = GetItemCount();
		int nSel = GetCurSel();

		if (!nTab) // no pages added
			return;

		while (nTab--)
		{
			if (nTab != nSel)
			{
				dis.itemID = nTab;
				dis.itemState = 0;

				VERIFY(GetItemRect(nTab, &dis.rcItem));

				dis.rcItem.bottom -= 2;
				DrawItem(&dis);
				DrawItemBorder(&dis);
			}
		}

		// now selected tab
		dis.itemID = nSel;
		dis.itemState = ODS_SELECTED;

		VERIFY(GetItemRect(nSel, &dis.rcItem));

		dis.rcItem.bottom += 2+4;
		dis.rcItem.top -= 2;
		DrawItem(&dis);
		DrawItemBorder(&dis);


	}
}

