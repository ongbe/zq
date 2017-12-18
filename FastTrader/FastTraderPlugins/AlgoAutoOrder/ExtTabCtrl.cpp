// ExtTabCtrl.cpp : implementation file
//
//#pragma warning(disable : 4996)

#include "stdafx.h"
#include "AlgoAutoOrder.h"
#include "ExtTabCtrl.h"


enum { PADDING = 3, EDGE = 20};

#pragma comment(lib,"msimg32.lib")

// CExtTabCtrl

IMPLEMENT_DYNAMIC(CExtTabCtrl, CTabCtrl)

CExtTabCtrl::CExtTabCtrl(	COLORREF ParentBkCol,
							COLORREF ClientBkCol,
							COLORREF ItemBkCol,
							COLORREF LineCol,
							COLORREF MainBorderCol,
							COLORREF MainBorderShadowCol,
							COLORREF ItemBorderCol,
							COLORREF TextCol,int FontSize,const char* FontName)
{
	m_ParentBkCol=ParentBkCol;
	m_ClientBkCol=ClientBkCol;
	m_ItemBkCol=ItemBkCol;
	m_LineCol=LineCol;
	m_MainBorderCol=MainBorderCol;
	m_MainBorderShadowCol=MainBorderShadowCol;
	m_ItemBorderCol=ItemBorderCol;
	m_TextCol=TextCol;
	m_FontSize=FontSize;
	memset(m_FontName,0,sizeof(m_FontName));
	if(FontName) strncpy_s(m_FontName,sizeof(m_FontName),FontName,sizeof(m_FontName)-1);

	m_brush.CreateSolidBrush(RGB(240,240,240));
	m_Pen.CreatePen(PS_SOLID,1,m_LineCol);
	m_Font.CreateFontA(m_FontSize,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_FontName);

}

CExtTabCtrl::~CExtTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CExtTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
//	ON_WM_SIZE()
//	ON_WM_CREATE()
END_MESSAGE_MAP()



// CExtTabCtrl message handlers



void CExtTabCtrl::PreSubclassWindow()
{
	CTabCtrl::PreSubclassWindow();
	ModifyStyle(0, TCS_OWNERDRAWFIXED|WS_CLIPCHILDREN);
}

BOOL CExtTabCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	ModifyStyle(0, TCS_OWNERDRAWFIXED|WS_CLIPCHILDREN);
	return CTabCtrl::PreCreateWindow(cs);
}

void CExtTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	TC_ITEM     tci;
	CDC			*pDC = CDC::FromHandle(lpdis->hDC);

	DrawItem2(*pDC,lpdis->itemID);
	return;
	CRect rItem(lpdis->rcItem);

	if(lpdis->itemID == (UINT)GetCurSel())
	{
		rItem.bottom -= 1;

		pDC->FillSolidRect(rItem, m_ClientBkCol);

		rItem.left += PADDING;
		rItem.top += PADDING + 1;
		rItem.right -= PADDING;

		char buf[256];
		tci.mask        = TCIF_TEXT ;
		tci.pszText     = buf;
		tci.cchTextMax  = sizeof(buf)-1;
		GetItem(lpdis->itemID, &tci);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_TextCol);
		pDC->DrawText(buf, rItem, DT_CENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
	}
	else
	{
		rItem.bottom += 2;

		COLORREF topcol=Lighter(m_ItemBkCol,1.05f);
		COLORREF bomcol=Darker(m_ItemBkCol,0.95f);
		FillGradientRect(*pDC,rItem,topcol,bomcol);

		rItem.left += PADDING;
		rItem.top += PADDING ;
		rItem.right -= PADDING;

		char buf[256];
		tci.mask        = TCIF_TEXT ;
		tci.pszText     = buf;
		tci.cchTextMax  = sizeof(buf)-1;
		GetItem(lpdis->itemID, &tci);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_TextCol);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->DrawText(buf, rItem, DT_CENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX);
	}
}

void CExtTabCtrl::DrawItem2(CDC& dc,int itemID)
{
	RECT ItemRect;
	GetItemRect(itemID,&ItemRect);
	int wid=ItemRect.right-ItemRect.left;
	int hei=ItemRect.bottom-ItemRect.top;
	int memhei=ItemRect.bottom;

	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(&dc,wid,memhei);
	CBitmap *pOldBmp=memdc.SelectObject(&MemBmp);
	memdc.FillSolidRect(0,0,wid,memhei,m_ParentBkCol);
	CRect txtRect;

	if (itemID == (int)GetCurSel())
	{
		memdc.FillSolidRect(0,ItemRect.top+1,wid,hei-1,m_ClientBkCol);

		CPen pen;
		pen.CreatePen(PS_SOLID,1,m_MainBorderCol);
		CPen *poldpen=memdc.SelectObject(&pen);
		memdc.MoveTo(1,memhei);
		memdc.LineTo(1,ItemRect.top);
		memdc.MoveTo(wid-1,memhei);
		memdc.LineTo(wid-1,ItemRect.top);
		memdc.SelectObject(poldpen);
		pen.DeleteObject();

		pen.CreatePen(PS_SOLID,1,RGB(255,199,60));
		memdc.SelectObject(&pen);
		memdc.MoveTo(1,ItemRect.top);
		memdc.LineTo(wid,ItemRect.top);
		memdc.MoveTo(2,ItemRect.top-1);
		memdc.LineTo(wid-1,ItemRect.top-1);
		memdc.SelectObject(poldpen);
		pen.DeleteObject();

		pen.CreatePen(PS_SOLID,1,RGB(230,139,44));
		memdc.SelectObject(&pen);
		memdc.MoveTo(3,ItemRect.top-2);
		memdc.LineTo(wid-2,ItemRect.top-2);
		memdc.SelectObject(poldpen);
		pen.DeleteObject();

		txtRect.left=PADDING;
		txtRect.right=wid-PADDING;
		txtRect.top=ItemRect.top+PADDING+1;
		txtRect.bottom=ItemRect.bottom-1;
	}
	else
	{
		RECT rect2={1,ItemRect.top+1,wid-1,ItemRect.bottom};
		COLORREF topcol=Lighter(m_ItemBkCol,1.05f);
		COLORREF bomcol=Darker(m_ItemBkCol,0.95f);
		FillGradientRect(memdc,rect2,topcol,bomcol);

		CPen pen;
		pen.CreatePen(PS_SOLID,1,m_ItemBorderCol);
		CPen *poldpen=memdc.SelectObject(&pen);
		memdc.MoveTo(1,memhei);
		memdc.LineTo(1,ItemRect.top+2);
		memdc.LineTo(3,ItemRect.top);
		memdc.LineTo(wid-3,ItemRect.top);
		memdc.LineTo(wid-1,ItemRect.top+2);
		memdc.LineTo(wid-1,memhei);
		memdc.SelectObject(poldpen);
		pen.DeleteObject();

		txtRect.left=PADDING;
		txtRect.right=wid-PADDING;
		txtRect.top=ItemRect.top+PADDING;
		txtRect.bottom=ItemRect.bottom-1;
	}

	TC_ITEM     tci;
	char		buf[256];
	tci.mask        = TCIF_TEXT ;
	tci.pszText     = buf;
	tci.cchTextMax  = sizeof(buf)-1;
	GetItem(itemID, &tci);
	memdc.SetBkMode(TRANSPARENT);
	memdc.SetTextColor(m_TextCol);
	CFont font;
	font.CreateFont(m_FontSize,0,0,0,FW_NORMAL,FALSE,FALSE,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,0,m_FontName);
	CFont* poldFont=memdc.SelectObject(&font);
	memdc.DrawText(buf, txtRect, DT_CENTER | DT_SINGLELINE);// | DT_END_ELLIPSIS | DT_NOPREFIX);
	memdc.SelectObject(poldFont);
	font.DeleteObject();

	dc.BitBlt(ItemRect.left,0,wid,memhei,&memdc,0,0,SRCCOPY);
	memdc.DeleteDC();
	MemBmp.DeleteObject();
}

void CExtTabCtrl::DrawMainBorder(CDC& dc)
{
	CRect rPage;
	RECT clientrect;

	GetClientRect(&clientrect);
	rPage = clientrect;
	AdjustRect(FALSE, rPage);
	CRect rBorder(clientrect);
	rBorder.top=rPage.top - 2;

	if(rBorder.Width()<=0||rBorder.Height()<=0) return;

	RECT PageRect=GetPageRect();
	int leftspace=PageRect.left-rBorder.left-1;
	int rightspace=rBorder.right-1-PageRect.right;
	int topspace=PageRect.top-rBorder.top-1;
	int bomspace=rBorder.bottom-1-PageRect.bottom;
	if(leftspace>0&&rBorder.Height()-2>0)
		dc.FillSolidRect(rBorder.left+1,rBorder.top+1,leftspace,rBorder.Height()-2,m_ClientBkCol);
	if(rightspace>0&&rBorder.Height()-2>0)
		dc.FillSolidRect(PageRect.right,rBorder.top+1,rightspace,rBorder.Height()-2,m_ClientBkCol);
	if(topspace>0&&PageRect.right-PageRect.left>0)
		dc.FillSolidRect(PageRect.left,rBorder.top+1,PageRect.right-PageRect.left,topspace,m_ClientBkCol);
	if(bomspace>0&&PageRect.right-PageRect.left>0)
		dc.FillSolidRect(PageRect.left,PageRect.bottom,PageRect.right-PageRect.left,bomspace,m_ClientBkCol);

	CPen pen;
	pen.CreatePen(PS_SOLID,1,m_MainBorderCol);
	CPen *poldpen=dc.SelectObject(&pen);
	dc.MoveTo(rBorder.left,rBorder.top);
	dc.LineTo(rBorder.right-2,rBorder.top);
	dc.LineTo(rBorder.right-2,rBorder.bottom-2);
	dc.LineTo(rBorder.left,rBorder.bottom-2);
	dc.LineTo(rBorder.left,rBorder.top);
	dc.SelectObject(poldpen);
	pen.DeleteObject();

	pen.CreatePen(PS_SOLID,1,m_MainBorderShadowCol);
	dc.SelectObject(&pen);
	dc.MoveTo(rBorder.right-1,rBorder.top+1);
	dc.LineTo(rBorder.right-1,rBorder.bottom-1);
	dc.LineTo(rBorder.left+1,rBorder.bottom-1);
	dc.SelectObject(poldpen);
	pen.DeleteObject();

	dc.SetPixel(rBorder.right-1,rBorder.top,m_ParentBkCol);
	dc.SetPixel(rBorder.left,rBorder.bottom-1,m_ParentBkCol);

	int nsel=GetCurSel();
	if(nsel>=0)
	{
		RECT ItemRect;
		GetItemRect(nsel,&ItemRect);
		pen.CreatePen(PS_SOLID,1,m_ClientBkCol);
		poldpen=dc.SelectObject(&pen);
		dc.MoveTo(ItemRect.left+2,ItemRect.bottom);
		dc.LineTo(ItemRect.right-1,ItemRect.bottom);
		dc.SelectObject(poldpen);
		pen.DeleteObject();
	}
}

//Ω•±‰ÃÓ≥‰
void CExtTabCtrl::FillGradientRect(CDC& dc,const RECT& rect,COLORREF topcol,COLORREF bomcol)
{
	int hei=rect.bottom-rect.top;
	int wid=rect.right-rect.left;

	if(hei<=0) return;
	else if(hei==1) 
	{
		dc.FillSolidRect(rect.left,rect.top,wid,1,topcol);
		return;
	}

	int topR=(int)GetRValue(topcol);
	int topG=(int)GetGValue(topcol);
	int topB=(int)GetBValue(topcol);
	int bomR=(int)GetRValue(bomcol);
	int bomG=(int)GetGValue(bomcol);
	int bomB=(int)GetBValue(bomcol);
	float deltaR=(float)(bomR-topR)/(float)(hei-1);
	float deltaG=(float)(bomG-topG)/(float)(hei-1);
	float deltaB=(float)(bomB-topB)/(float)(hei-1);
	float iR=(float)topR;
	float iG=(float)topG;
	float iB=(float)topB;

	for(int i=0;i<hei;i++,iR+=deltaR,iG+=deltaG,iB+=deltaB)
	{
		dc.FillSolidRect(rect.left,rect.top+i,wid,1,RGB((int)iR,(int)iG,(int)iB));
	}
}

//////////////////////////////////////////////////////////////////////////////
// helpers
COLORREF CExtTabCtrl::Darker(COLORREF crBase, float fFactor)
{
	fFactor = min(fFactor, 1.0f);
	fFactor = max(fFactor, 0.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedShadow, bBlueShadow, bGreenShadow;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedShadow = (BYTE)(bRed * fFactor);
	bBlueShadow = (BYTE)(bBlue * fFactor);
	bGreenShadow = (BYTE)(bGreen * fFactor);

	return RGB(bRedShadow, bGreenShadow, bBlueShadow);
}

COLORREF CExtTabCtrl::Lighter(COLORREF crBase, float fFactor) 
{
	fFactor = max(fFactor, 1.0f);

	BYTE bRed, bBlue, bGreen;
	BYTE bRedHilite, bBlueHilite, bGreenHilite;

	bRed = GetRValue(crBase);
	bBlue = GetBValue(crBase);
	bGreen = GetGValue(crBase);

	bRedHilite = (BYTE)min((int)(bRed * fFactor), 255);
	bBlueHilite = (BYTE)min((int)(bBlue * fFactor), 255);
	bGreenHilite = (BYTE)min((int)(bGreen * fFactor), 255);

	return RGB(bRedHilite, bGreenHilite, bBlueHilite);
}

RECT CExtTabCtrl::GetPageRect(void)
{
	RECT PageRect;
	GetClientRect(&PageRect);
	CRect rPage = PageRect;
	AdjustRect(FALSE, rPage);
	PageRect.top=rPage.top - 2;
	PageRect.left+=4;
	PageRect.top+=4;
	PageRect.right-=4;
	PageRect.bottom-=4;
	if(PageRect.right<PageRect.left) PageRect.right=PageRect.left;
	if(PageRect.bottom<PageRect.top) PageRect.bottom=PageRect.top;
	return PageRect;
}

BOOL CExtTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(1)
	{
		CRect	ClientRect,SelTabRect,TmpRect;
		int		SelOrd;
		CDC		iMemDC;
		
		GetClientRect(ClientRect);
		SelOrd=GetCurSel();
		if(SelOrd<0) SelOrd=0;
		GetItemRect(SelOrd,SelTabRect);

		pDC->FillSolidRect(ClientRect, m_ParentBkCol);

		return TRUE;
	}

	return CTabCtrl::OnEraseBkgnd(pDC);
}


void CExtTabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	dc.SelectObject(GetFont());
	DRAWITEMSTRUCT dis;
	dis.CtlType = ODT_TAB;
	dis.CtlID = GetDlgCtrlID();
	dis.hwndItem = GetSafeHwnd();
	dis.hDC = dc.GetSafeHdc();
	dis.itemAction = ODA_DRAWENTIRE;

	DrawMainBorder(dc);

	// paint the tabs first and then the borders
	int nTab = GetItemCount();
	int nSel = GetCurSel();

	while (nTab>0)
	{
		dis.itemID = nTab-1;
		if (nTab-1 != nSel)
		{
			dis.itemState = 0;
			GetItemRect(nTab-1, &dis.rcItem);
			dis.rcItem.bottom -= 2;
			DrawItem(&dis);
		}
		nTab--;
	}
	if(nSel>=0)
	{
		dis.itemID = nSel;
		dis.itemState = ODS_SELECTED;
		GetItemRect(nSel, &dis.rcItem);
		dis.rcItem.bottom += 2+4;
		dis.rcItem.top -= 2;
		DrawItem(&dis);
	}
}


