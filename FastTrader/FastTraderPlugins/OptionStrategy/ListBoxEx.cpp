// ListBoxEx.cpp : implementation file
//

#include "stdafx.h"
#include "OptionStrategy.h"
#include "ListBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx

CListBoxEx::CListBoxEx()
{
}

CListBoxEx::~CListBoxEx()
{
	m_BkBrush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	//{{AFX_MSG_MAP(CListBoxEx)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBoxEx message handlers

void CListBoxEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_bOver = FALSE;
	m_ItemHeight=SCROLL_HIGHT; 
	m_crTextHlt=GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crTextClr=GetSysColor(COLOR_WINDOWTEXT);
	m_HBkColor=GetSysColor(COLOR_HIGHLIGHT);
    m_BkColor=GetSysColor(COLOR_BACKGROUND);
	m_BmpWidth=SCROLL_HIGHT;
	m_BmpHeight=SCROLL_HIGHT;


	CListBox::PreSubclassWindow();

}

void CListBoxEx::DrawBorders()
{
    return;

	//Gets the Controls device context
	CDC *pDC=GetDC();
	
	//Gets the size of the control's client area
	CRect rect;
	GetClientRect(rect);
	
	/*
	Inflates the size of rect by the size of the default border
	Suppose rect is (0,0,100,200) and the default border is 2 pixels,
	after InflateRect, rect should be (-2,-2, 102,202) and the border will be drawn
	from -2 to 0, -2 -> 0, 102->100, 202->200.
	*/
	rect.InflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));
	
	//Draws the edge of the border depending on whether the mouse is over or not
	
	if (m_bOver)pDC->DrawEdge(rect,EDGE_BUMP ,BF_RECT );
	else pDC->DrawEdge(rect,EDGE_SUNKEN,BF_RECT );
	
	ReleaseDC(pDC); //Frees the DC
}

void CListBoxEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//If m_bOver==FALSE, and this function is called, it means that the mouse entered.
	
	if (!m_bOver){ 
		m_bOver=TRUE; //Now the mouse is over
		DrawBorders(); //Self explanatory
	
		TRACKMOUSEEVENT track; //Declares structure
		track.cbSize=sizeof(track);
		track.dwFlags=TME_LEAVE; //Notify us when the mouse leaves
		track.hwndTrack=m_hWnd; //Assigns this window's hwnd
		TrackMouseEvent(&track); //Tracks the events like WM_MOUSELEAVE
	
	}
	
	CListBox::OnMouseMove(nFlags, point);
}

LRESULT CListBoxEx::OnMouseLeave(WPARAM wParam, LPARAM lParam){

	m_bOver=FALSE;
	DrawBorders();
	
	return 0;
}



HBRUSH CListBoxEx::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	if (!IsWindowEnabled()){
	CBrush br(GetSysColor(COLOR_INACTIVEBORDER));
	return br;
	}
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return m_BkBrush;
}
void CListBoxEx::SetBkColor(COLORREF crBkColor,COLORREF crSelectedColor)
{
    m_BkColor = crBkColor;
	m_HBkColor=crSelectedColor;
	m_BkBrush.DeleteObject(); //Deletes previous brush. Must do it or there would be a run-time error.
	m_BkBrush.CreateSolidBrush(m_BkColor); //Sets the brush the specified background color
	Invalidate(); //Forces Redraw
}

void CListBoxEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	lpMeasureItemStruct->itemHeight=m_ItemHeight;

}

void CListBoxEx::SetItemHeight(int newHeight)
{
	m_ItemHeight=newHeight;
	Invalidate();
}


void CListBoxEx::AddItem(UINT IconID, LPCTSTR lpszText)
{
	//Adds a string ans assigns nIndex the index of the current item
	int nIndex=AddString(lpszText);
	//If no error, associates the index with the bitmap ID
	if (nIndex!=LB_ERR&&nIndex!=LB_ERRSPACE)
		SetItemData(nIndex, IconID);
}


void CListBoxEx::InsertItem(int nIndex, UINT nID, LPCTSTR lpszText)
{
	int result=InsertString(nIndex,lpszText); //Inserts the string
	if (result!=LB_ERR||result!=LB_ERRSPACE) SetItemData(nIndex,nID); //Associates the ID with the index
}

void CListBoxEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
		// TODO: Add your message handler code here
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC); //Gets the item DC
	UINT nID=(UINT)lpDrawItemStruct->itemData; //Retrieves the ID we set using SetItemData
	CRect rect=lpDrawItemStruct->rcItem; //Gets the rect of the item
	UINT action=lpDrawItemStruct->itemAction; //What it wants to do
	UINT state=lpDrawItemStruct->itemState; //The item current state
	COLORREF TextColor=m_crTextClr; //Text color that we'll use
	
	//Action statements
	if ((state & ODS_SELECTED) &&
		(action & ODA_SELECT))
		//Used when an item need to be selected
	{
		//Since it will be highlighted, we create a brush with the highlighted color
		CBrush brush(m_HBkColor); 
		//Draws the highlighted rect	
		pDC->FillRect(rect, &brush);

	}
	
	if (!(state & ODS_SELECTED) &&	(action & ODA_SELECT))
		//The item needs to be deselected
	{
		//We draw the background color	
		pDC->FillRect(rect, &m_BkBrush);
	
	}
	
	if ((action & ODA_FOCUS) && (state & ODS_FOCUS)&&(state&ODS_SELECTED)){
		//It has the focus,
		//Draws a 3D focus rect	
		pDC->Draw3dRect(rect,RGB(255,255,255),RGB(0,0,0)); 
		TextColor=m_crTextHlt;	

	}
	
	if ((action & ODA_FOCUS) && !(state & ODS_FOCUS)&&(state&ODS_SELECTED)){
		
		//If the focus needs to be removed.
		CBrush brush(m_HBkColor); 
		pDC->FillRect(rect, &brush);
		TextColor=m_crTextHlt;	
	}

	if (state&ODS_DISABLED) TextColor=GetSysColor(COLOR_3DSHADOW); //If disabled
	CString text;
	GetText(lpDrawItemStruct->itemID, text); //Gets the item text
	pDC->SetTextColor(TextColor); //Mo need to explain
	pDC->SetBkMode(TRANSPARENT); //Sets text background transparent

	
	
	if (nID!=NO_BMP_ITEM){ //If the item has a bitmap
		
		CDC dcMem; //New device context used as the source DC
		//Creates a deice context compatible to pDC
		dcMem.CreateCompatibleDC(pDC); 
		
		CBitmap bmp; //Bitmap object
		//Loads the bitmap with the specified resource ID
		bmp.LoadBitmap(nID); 
		//Saves the old bitmap object so that the GDI resources are not depleted
		CBitmap* oldbmp=dcMem.SelectObject(&bmp); 
		
		if (nID!=BLANK_BMP) //Draws the bitmap if it is not blank
			//Copies the bitmap to the screen
			pDC->BitBlt(rect.left+5,rect.top,m_BmpWidth,m_BmpHeight,&dcMem,0,0,SRCCOPY); 
		
		//Selects the saved bitmap object
		dcMem.SelectObject(oldbmp); 
		
		bmp.DeleteObject(); //Deletes the bitmap
		
		//Displays the text 
		pDC->TextOut(rect.left+10+m_BmpWidth,rect.top,text); 
	}
	//Displays the text without identing it
	else pDC->TextOut(rect.left+5,rect.top,text); 
	
	
	
}

void CListBoxEx::SetTextColor(COLORREF crTextColor, COLORREF crHighlight)
{
	m_crTextClr=crTextColor;
	m_crTextHlt=crHighlight;
	Invalidate();
}

void CListBoxEx::SetBMPSize(int Height, int Width)
{
	m_BmpHeight=Height;
	m_BmpWidth=Width;
	Invalidate();
}

void CListBoxEx::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	
	lpncsp->rgrc[0].top += SCROLL_HIGHT; //Top
	lpncsp->rgrc[0].bottom -= SCROLL_HIGHT; //Bottom

	CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CListBoxEx::OnNcPaint() 
{
	// TODO: Add your message handler code here

	static BOOL before=FALSE;
	if (!before) { //If first time, the OnNcCalcSize function will be called
			SetWindowPos(NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE); 
			before=TRUE;
	}
	
	DrawBorders();
	DrawScrolls(SC_UP,SC_NORMAL);
	DrawScrolls(SC_DOWN,SC_NORMAL);

	// Do not call CListBox::OnNcPaint() for painting messages
}

void CListBoxEx::DrawScrolls(UINT WhichOne, UINT State)
{

	CDC *pDC=GetDC();
	CRect rect;
	GetClientRect(rect); //Gets the dimensions
	
	if (!IsWindowEnabled())State=SC_DISABLED; //If the window is not enabled, set state to disabled
	
	rect.left-=GetSystemMetrics(SM_CYEDGE); //Expands the so that it does not draw over the borders
	rect.right+=GetSystemMetrics(SM_CXEDGE);
	
	if (WhichOne==SC_UP){ //The one to draw is the up one
		
		//Calculates the rect of the up scroll
		rect.bottom=rect.top-GetSystemMetrics(SM_CXEDGE);
		rect.top=rect.top-SCROLL_HIGHT-GetSystemMetrics(SM_CXEDGE);
		
		//Draws the scroll up
		pDC->DrawFrameControl(rect,DFC_SCROLL,State|DFCS_SCROLLUP);
        //pDC->DrawFrameControl(rect,DFC_SCROLL,State|DFCS_SCROLLUP);
        pDC->FillSolidRect(&rect,m_BkColor);
	}
	else{ //Needs to draw down
		
		rect.top=rect.bottom+GetSystemMetrics(SM_CXEDGE);;
		rect.bottom=rect.bottom+SCROLL_HIGHT+GetSystemMetrics(SM_CXEDGE);
		
		pDC->DrawFrameControl(rect,DFC_SCROLL,State|DFCS_SCROLLDOWN);
        //pDC->DrawFrameControl(rect,DFC_SCROLL,State|DFCS_SCROLLUP);
        pDC->FillSolidRect(&rect,m_BkColor);

		
	}	
	ReleaseDC(pDC);
}

LRESULT CListBoxEx::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect rect,top,bottom; 
	//Gets the windows rect, relative to the parent, so rect.left and rect.top might not be 0.
	GetWindowRect(rect); 
	ScreenToClient(rect); //Converts the rect to the client
	
	//Calculates the rect of the bottom and top scrolls
	top=bottom=rect;
	top.bottom=rect.top+SCROLL_HIGHT;
	bottom.top=rect.bottom-SCROLL_HIGHT;
	
	
	UINT where = CListBox::OnNcHitTest(point); //Obtains where the mouse is
	
	ScreenToClient(&point); //Converts the point so its relative to the client area
	
	if (where == HTNOWHERE) //If mouse is not in a place it recognizes
		if (top.PtInRect(point)) //Check to see if the mouse is on the top
			where = HTVSCROLL;
		else if (bottom.PtInRect(point)) where=HTHSCROLL; //Check to see if its on the bottom
		
		return where; //Returns where it is
}


void CListBoxEx::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if (nHitTest==HTVSCROLL) //Up scroll Pressed
	{
		DrawScrolls(SC_UP,SC_PRESSED);
		SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),0); //Scroll up 1 line
		
		
		SetTimer(1,100,NULL); //Sets the timer ID 1
	}
	else if (nHitTest==HTHSCROLL) //Down scroll Pressed
	{
		DrawScrolls(SC_DOWN,SC_PRESSED);
		SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),0); //Scroll down 1 line
		
		SetTimer(2,100,NULL); //Sets the timer ID 2
	}
	
	CListBox::OnNcLButtonDown(nHitTest, point);
}

void CListBoxEx::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	//Gets the state of the left button to see if it is pressed
	short result=GetKeyState(VK_LBUTTON);
	
	if (nIDEvent==1){ //Up timer
		
		//If it returns negative then it is pressed
		if (result<0){
			SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),0);	
		}
		else { //No longer pressed
			
			KillTimer(1);
			DrawScrolls(SC_UP,SC_NORMAL);		
		}
	}
	else { //Down timer
		
		//If it returns negative then it is pressed
		if (result<0){
			SendMessage(WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),0);	
		}
		else {
			KillTimer(2);
			DrawScrolls(SC_DOWN,SC_NORMAL);		
		}
	}
	
	CListBox::OnTimer(nIDEvent);
}

void CListBoxEx::OnEnable(BOOL bEnable) 
{
	CListBox::OnEnable(bEnable);
	
	// TODO: Add your message handler code here
	//SC_NORMAL will be changed to SC_DISABLED if the window is disabled
	DrawScrolls(SC_UP,SC_NORMAL);
	DrawScrolls(SC_DOWN,SC_NORMAL);
	Invalidate();
}
