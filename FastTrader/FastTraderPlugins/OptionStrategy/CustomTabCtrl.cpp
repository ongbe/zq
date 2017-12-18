#include "StdAfx.h"
#include "CustomTabCtrl.h"

//////////////////////////////////////////////////////////////////////////
CCustomTabItem::CCustomTabItem(CString sText, LPARAM lParam) : 
m_sText(sText), 
m_lParam(lParam),	
m_bShape(TAB_INVISIBLE),
m_fSelected(FALSE),
m_fHighlighted(FALSE),
m_fHighlightChanged(FALSE)
{
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabItem::operator=(const CCustomTabItem &other)
{
    m_sText = other.m_sText;
    m_lParam = other.m_lParam;
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabItem::Draw(CDC& dc, CFont& font, TabItemColors *pColors, BOOL fOnTop, BOOL fNoLines)
{
    COLORREF bgColor, fgColor;

    if(m_fSelected || m_fHighlighted)
    {
        bgColor=pColors->crBkgActive;
        fgColor=pColors->crTxtActive;
    }
    else
    {
        bgColor=pColors->crBkgInactive;
        fgColor=pColors->crTxtInactive;
    }

    CBrush brush(bgColor);

    CPen blackPen(PS_SOLID, 1, pColors->crDarkLine);
    CPen shadowPen(PS_SOLID, 1, pColors->crLightLine);

    POINT *points = new POINT[4];
    points[0].x = m_rect.left;
    points[0].y = m_rect.top;
    points[1].x = m_rect.left + m_rect.Width()*4/5;
    points[1].y = m_rect.top;
    points[2].x = m_rect.right;
    points[2].y = m_rect.bottom;
    points[3].x = m_rect.left;
    points[3].y = m_rect.bottom;

    CRgn rgn;
    rgn.CreatePolygonRgn(points,4,ALTERNATE);

    delete []points;

    CRect rc = m_rect;

    // Paint item background
    //dc.FillRect(&m_rect, &brush);	
    dc.FillRgn(&rgn,&brush);

    CPen* pOldPen = dc.SelectObject(&blackPen);

    if(!fNoLines)
    {
        if(fOnTop)
        {
            dc.MoveTo(rc.left, rc.bottom);
            dc.LineTo(rc.left, rc.top);
            dc.LineTo(rc.right, rc.top);
            dc.LineTo(rc.right, rc.bottom);
        }
        else
        {
            dc.MoveTo(rc.left, rc.top);
            dc.LineTo(rc.left, rc.bottom-1);
            dc.LineTo(rc.right, rc.bottom-1);
            dc.LineTo(rc.right, rc.top);
        }
    }

    // draw item text
    COLORREF bgOldColor = dc.SetBkColor(bgColor);
    COLORREF fgOldColor = dc.SetTextColor(fgColor);

    rc.DeflateRect(rc.Height()/2, 2, rc.Height()/2, 2);
    CFont* pOldFont = dc.SelectObject(&font);

    dc.DrawText(m_sText, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    dc.SelectObject(pOldFont);
    dc.SetTextColor(fgOldColor);
    dc.SetBkColor(bgOldColor);

    dc.SelectObject(pOldPen);
}

//////////////////////////////////////////////////////////////////////////
#ifdef _WIN32_WCE
LOGFONT CCustomTabCtrl::lf_default = {12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS,
DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Tahoma") };

LOGFONT CCustomTabCtrl::lf_default_vga = {14, 0, 0, 0, FW_NORMAL, 0, 0, 0,
DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS,
DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Tahoma") };
#else
LOGFONT CCustomTabCtrl::lf_default = {12, 0, 0, 0, FW_NORMAL, 0, 0, 0,
DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS,
DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Microsoft Sans Serif") };
#endif

BYTE CCustomTabCtrl::m_bBitsGlyphs[] = {
    0xBD,0xFB,0xDF,0xBD,0x3C,0x00,
    0xB9,0xF3,0xCF,0x9D,0x99,0x00,
    0xB1,0xE3,0xC7,0x8D,0xC3,0x00,
    0xA3,0xC7,0xE3,0xC5,0xE7,0x00,
    0xB1,0xE3,0xC7,0x8D,0xC3,0x00,
    0xB9,0xF3,0xCF,0x9D,0x99,0x00,
    0xBD,0xFB,0xDF,0xBD,0x3C,0x00
};

IMPLEMENT_DYNAMIC(CCustomTabCtrl, CWnd)

//////////////////////////////////////////////////////////////////////////
CCustomTabCtrl::CCustomTabCtrl() :
m_nButtonIDDown(CTCID_NOBUTTON),
m_nPrevState(BNST_INVISIBLE),
m_nNextState(BNST_INVISIBLE),
m_nFirstState(BNST_INVISIBLE),
m_nLastState(BNST_INVISIBLE),
m_nCloseState(BNST_INVISIBLE),
m_nItemSelected(-1),
m_nFirstVisibleItem(0),
m_dwLastRepeatTime(0),
m_nItemDragDest(0)
{
    RegisterWindowClass();
    m_bmpGlyphsMono.CreateBitmap(48, 7, 1, 1, m_bBitsGlyphs);

    //initialize tab/tabitem colors
    m_ticColors.crBkgInactive=GetSysColor(COLOR_3DFACE);
    m_ticColors.crBkgActive=GetSysColor(COLOR_WINDOW);
    m_ticColors.crTxtInactive=GetSysColor(COLOR_BTNTEXT);
    m_ticColors.crTxtActive=GetSysColor(COLOR_WINDOWTEXT);
    m_ticColors.crDarkLine=GetSysColor(COLOR_BTNTEXT);
    m_ticColors.crLightLine=GetSysColor(COLOR_3DSHADOW);
    m_ticColors.crWndBkg=GetSysColor(COLOR_WINDOW);

    if(GetSystemMetrics(SM_CYSCREEN) > 320 || GetSystemMetrics(SM_CXSCREEN) > 320)
    {
        //vga device?
        m_nTabsHeight=28;
        SetControlFont(lf_default_vga);
    }
    else
    {
        m_nTabsHeight=25;
        SetControlFont(lf_default);
    }
}

// Register the window class if it has not already been registered.
//////////////////////////////////////////////////////////////////////////
BOOL CCustomTabCtrl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst;

    hInst = AfxGetInstanceHandle();

    if(!(::GetClassInfo(hInst, CustomTabCtrl_CLASSNAME, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = CustomTabCtrl_CLASSNAME;

        if(!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
CCustomTabCtrl::~CCustomTabCtrl()
{
    int i;

    for(i=0; i< m_aItems.GetSize(); i++)
    {
        delete m_aItems[i];
    }
    m_aItems.RemoveAll();
}

BEGIN_MESSAGE_MAP(CCustomTabCtrl, CWnd)
    //{{AFX_MSG_MAP(CCustomTabCtrl)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_TIMER()
    ON_EN_UPDATE(CTCID_EDITCTRL, OnUpdateEdit)
#ifndef _WIN32_WCE
    ON_WM_RBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
#endif
    ON_MESSAGE(TCM_ADJUSTRECT, OnTcmAdjustRect)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::AdjustRect(BOOL bLarger, LPRECT lpRect)
{
    if(IsWindow(m_hWnd))
    {
        ::SendMessage(m_hWnd, TCM_ADJUSTRECT, bLarger, (LPARAM)lpRect);
    }
}

//////////////////////////////////////////////////////////////////////////
LRESULT CCustomTabCtrl::OnTcmAdjustRect(WPARAM wParam, LPARAM lParam)
{
    LPRECT pRect;

    pRect=(LPRECT)lParam;
    if(pRect == NULL)
    {
        return 0;
    }

    if(IsContainer())
    {
        if(wParam)
        {
            //return entire window rect
            GetWindowRect(pRect);
        }
        else
        {
            if(GetStyle() & CTCS_TOP)
            {
                pRect->top = m_nTabsHeight;
            }
            else
            {
                pRect->bottom -= m_nTabsHeight;
            }
        }
    }
    else
    {
        //not a container, AdjustRect not supported
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
//get tabs height (makes sense if has container style)
int CCustomTabCtrl::GetTabsHeight()
{
    CRect rc;

    if(IsContainer())
    {
        return m_nTabsHeight;
    }
    else
    {
        GetClientRect(&rc);
        if(IsVertical())
        {
            return rc.Width();
        }
        else
        {
            return rc.Height();
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//set tabs height (makes sense if has container style)
void CCustomTabCtrl::SetTabsHeight(int nHeight)
{
    m_nTabsHeight=nHeight;
    RecalcLayout(RECALC_RESIZED, m_nItemSelected);
}

//////////////////////////////////////////////////////////////////////////
//set control colors, and optionally redraw.
void CCustomTabCtrl::SetColors(TabItemColors *pColorsIn, BOOL fRedraw)
{
    if(pColorsIn != NULL)
    {
        m_ticColors.crBkgActive = pColorsIn->crBkgActive;
        m_ticColors.crBkgInactive = pColorsIn->crBkgInactive;
        m_ticColors.crDarkLine = pColorsIn->crDarkLine;
        m_ticColors.crLightLine = pColorsIn->crLightLine;
        m_ticColors.crTxtActive = pColorsIn->crTxtActive;
        m_ticColors.crTxtInactive = pColorsIn->crTxtInactive;
        m_ticColors.crWndBkg = pColorsIn->crWndBkg;
    }

    if(fRedraw)
    {
        Invalidate(FALSE);
    }
}

//////////////////////////////////////////////////////////////////////////
//return RECT where tabs are: the entire window rect if not a container, tabs only otherwise.
void CCustomTabCtrl::GetTabsRect(LPRECT pRect)
{
    CRect rc;

    if(pRect == NULL)
    {
        return;
    }

    GetClientRect(&rc);

    if(IsContainer())
    {
        if(GetStyle() & CTCS_TOP)
        {
            rc.bottom=rc.top + m_nTabsHeight;
        }
        else
        {
            rc.top=rc.bottom - m_nTabsHeight;
        }
    }
    *pRect = rc;
}

//////////////////////////////////////////////////////////////////////////
//get control colors
void CCustomTabCtrl::GetColors(TabItemColors *pColorsOut)
{
    if(pColorsOut != NULL)
    {
        pColorsOut->crBkgActive = m_ticColors.crBkgActive;
        pColorsOut->crBkgInactive = m_ticColors.crBkgInactive;
        pColorsOut->crDarkLine = m_ticColors.crDarkLine;
        pColorsOut->crLightLine = m_ticColors.crLightLine;
        pColorsOut->crTxtActive = m_ticColors.crTxtActive;
        pColorsOut->crTxtInactive = m_ticColors.crTxtInactive;
        pColorsOut->crWndBkg = m_ticColors.crWndBkg;
    }
}

//////////////////////////////////////////////////////////////////////////
BOOL CCustomTabCtrl::Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID)
{
    return CWnd::Create(CustomTabCtrl_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}

//////////////////////////////////////////////////////////////////////////
BOOL CCustomTabCtrl::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::OnPaint()
{
    int i, nBtns;
    CRect rCl, rWnd;
    CDC dcMem;
    CBitmap bmpMem;

    CPaintDC dc(this);

    {
        m_rgbGlyph[0] = GetSysColor(COLOR_BTNTEXT);
        m_rgbGlyph[1] = GetSysColor(COLOR_BTNTEXT);
        m_rgbGlyph[2] = GetSysColor(COLOR_BTNTEXT);
        m_rgbGlyph[3] = GetSysColor(COLOR_BTNTEXT);
    }

    GetTabsRect(&rCl);
    GetClientRect(&rWnd);

    if(IsVertical())
    {
        rCl.SetRect(0, 0, rCl.Height(), rCl.Width());
    }

    CPen blackPen(PS_SOLID, 1, m_ticColors.crDarkLine);

    CBitmap* pOldBmp=NULL;

    if(dcMem.CreateCompatibleDC(&dc))
    {
        if(bmpMem.CreateCompatibleBitmap(&dc, rWnd.Width(), rWnd.Height()))
        {
            pOldBmp = dcMem.SelectObject(&bmpMem);
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }

    // clear background
    dcMem.FillSolidRect(&rCl, m_ticColors.crWndBkg);

    nBtns = 0;
    if(m_nCloseState)
    {
        nBtns++;
    }
    if(m_nPrevState)
    {
        nBtns += 2;
    }
    if(m_nFirstState)
    {
        nBtns += 2;
    }

    BOOL fAfter = (BOOL)GetStyle()&CTCS_BUTTONSAFTER;

    BOOL fTop = (BOOL)GetStyle()&CTCS_TOP;

    int nA = rCl.Height()-2;

    CRect rAll;
    if(fTop)
    {
        rAll.SetRect(0, 0, nBtns*nA+3, rCl.Height()-1);
    }
    else
    {
        rAll.SetRect(0, rCl.top, nBtns*nA+3, rCl.bottom);
    }

    if(nBtns == 0)
    {
        rAll.SetRectEmpty();
    }

    int nCloseOffset = 0;

    if(fAfter)
    {
        nCloseOffset = rCl.Width() - rAll.Width();
        rAll.OffsetRect(nCloseOffset, 0);
    }

    // draw tab items visible and not selected	
    for(i=0; i<m_aItems.GetSize(); i++)
    {
        if(m_aItems[i]->m_bShape && !m_aItems[i]->m_fSelected)
        {
            if(m_aItems[i]->m_fHighlighted)
            {
                m_aItems[i]->Draw(dcMem, m_FontSelected, &m_ticColors, GetStyle()&CTCS_TOP, GetStyle()/*&CTCS_NOLINES*/);
            }
            else
            {
                m_aItems[i]->Draw(dcMem, m_Font, &m_ticColors, GetStyle()&CTCS_TOP, GetStyle()/*&CTCS_NOLINES*/);
            }
        }
    }

    // draw selected tab item	
    if(m_nItemSelected!=-1 && m_aItems[m_nItemSelected]->m_bShape)
    {
        m_aItems[m_nItemSelected]->Draw(dcMem, m_FontSelected, &m_ticColors, GetStyle()&CTCS_TOP, GetStyle()/*&CTCS_NOLINES*/);
    }

    if(m_nCloseState || m_nPrevState)
    {
        CPen* pOldPen = dcMem.SelectObject(&blackPen);
        dcMem.Rectangle(rAll);
        dcMem.SelectObject(pOldPen);
    }

    // draw buttons
    if(m_nCloseState)
    {
        BOOL fMirrored = TRUE;
        if(!fAfter)
        {
            fMirrored = FALSE;
        }
        CRect rClose;
        if(fAfter)
        {
            if(fTop)
            {
                rClose.SetRect(rCl.Width()-nA-2, 1, rCl.Width()-1, rCl.Height());
            }
            else
            {
                rClose.SetRect(rCl.Width()-nA-2, rCl.top, rCl.Width()-1, rCl.bottom-1);
            }
        }
        else
        {
            nCloseOffset = nA;
            if(fTop)
            {
                rClose.SetRect(1, 1, nA+1, rCl.Height());
            }
            else
            {
                rClose.SetRect(1, rCl.top, nA+1, rCl.bottom-1);
            }
        }

        CPoint ptClose(rClose.left + (rClose.Width()-8)/2 + rClose.Width()%2, rClose.top + (rClose.Height()-7)/2);

        /*
        if(fMirrored && m_hBmpBkRightSpin)
        {
        DrawBk(dcMem, rClose, m_hBmpBkRightSpin, m_fIsRightImageHorLayout, m_mrgnRight, m_nCloseState-1);
        }
        else if(m_hBmpBkLeftSpin)
        {
        DrawBk(dcMem, rClose, m_hBmpBkLeftSpin, m_fIsLeftImageHorLayout, m_mrgnLeft, m_nCloseState-1);
        }
        else
        */
        {
            if(m_nCloseState == BNST_PRESSED)
            {
                dcMem.DrawFrameControl(rClose, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
            }
            else
            {
                dcMem.DrawFrameControl(rClose, DFC_BUTTON, DFCS_BUTTONPUSH);
            }
        }
        DrawGlyph(dcMem, ptClose, 4, m_nCloseState-1);
    }

    if(m_nPrevState)
    {
        CRect rFirst, rPrev, rNext, rLast;

        if(fTop)
        {
            if(nBtns<4)
            {
                rPrev.SetRect(nCloseOffset+1, 1, nCloseOffset+nA+1, rCl.Height());
                rNext.SetRect(nCloseOffset+nA+2, 1, nCloseOffset+2*nA+2, rCl.Height());
            }
            else
            {
                rFirst.SetRect(nCloseOffset+1, 1, nCloseOffset+nA+1, rCl.Height());
                rPrev.SetRect(nCloseOffset+nA+1, 1, nCloseOffset+2*nA+1, rCl.Height());
                rNext.SetRect(nCloseOffset+2*nA+2, 1, nCloseOffset+3*nA+2, rCl.Height());
                rLast.SetRect(nCloseOffset+3*nA+2, 1, nCloseOffset+4*nA+2, rCl.Height());
            }
        }
        else
        {
            if(nBtns<4)
            {
                rPrev.SetRect(nCloseOffset+1, rCl.top, nCloseOffset+nA+1, rCl.bottom);
                rNext.SetRect(nCloseOffset+nA+2, rCl.top, nCloseOffset+2*nA+2, rCl.bottom);
            }
            else
            {
                rFirst.SetRect(nCloseOffset+1, rCl.top, nCloseOffset+nA+1, rCl.bottom);
                rPrev.SetRect(nCloseOffset+nA+1, rCl.top, nCloseOffset+2*nA+1, rCl.bottom);
                rNext.SetRect(nCloseOffset+2*nA+2, rCl.top, nCloseOffset+3*nA+2, rCl.bottom);
                rLast.SetRect(nCloseOffset+3*nA+2, rCl.top, nCloseOffset+4*nA+2, rCl.bottom);
            }
        }

        if(nBtns>=4)
        {
            CPoint ptFirst(rFirst.left + (rFirst.Width()-8)/2, rFirst.top + (rFirst.Height()-7)/2);
            /*
            if(m_hBmpBkLeftSpin)
            {
            DrawBk(dcMem, rFirst, m_hBmpBkLeftSpin, m_fIsLeftImageHorLayout, m_mrgnLeft, m_nFirstState-1);
            }
            else
            */
            {
                if(m_nFirstState == BNST_PRESSED)
                {
                    dcMem.DrawFrameControl(rFirst, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
                }
                else
                {
                    dcMem.DrawFrameControl(rFirst, DFC_BUTTON, DFCS_BUTTONPUSH);
                }
            }
            DrawGlyph(dcMem, ptFirst, 0, m_nFirstState-1);

            CPoint ptLast(rLast.left + (rLast.Width()-8)/2, rLast.top+(rLast.Height()-7)/2);
            /*
            if(m_hBmpBkRightSpin)
            {
            DrawBk(dcMem, rLast, m_hBmpBkRightSpin, m_fIsRightImageHorLayout, m_mrgnRight, m_nLastState-1);
            }
            else
            */
            {
                if(m_nLastState == BNST_PRESSED)
                {
                    dcMem.DrawFrameControl(rLast, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
                }
                else
                {
                    dcMem.DrawFrameControl(rLast, DFC_BUTTON, DFCS_BUTTONPUSH);
                }
            }
            DrawGlyph(dcMem,ptLast, 3, m_nLastState-1);
        }

        CPoint ptPrev(rPrev.left + (rPrev.Width()-8)/2, rPrev.top+(rPrev.Height()-7)/2);
        /*
        if(m_hBmpBkLeftSpin)
        {
        DrawBk(dcMem, rPrev, m_hBmpBkLeftSpin, m_fIsLeftImageHorLayout, m_mrgnLeft,m_nPrevState-1);
        }
        else
        */
        {
            if(m_nPrevState == BNST_PRESSED)
            {
                dcMem.DrawFrameControl(rPrev, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
            }
            else
            {
                dcMem.DrawFrameControl(rPrev, DFC_BUTTON, DFCS_BUTTONPUSH);
            }
        }
        DrawGlyph(dcMem, ptPrev, 1, m_nPrevState-1);

        CPoint ptNext(rNext.left + (rNext.Width()-8)/2, rNext.top + (rNext.Height()-7)/2);
        /*
        if(m_hBmpBkRightSpin)
        {
        DrawBk(dcMem,  rNext, m_hBmpBkRightSpin, m_fIsRightImageHorLayout, m_mrgnRight, m_nNextState-1);
        }
        else
        */
        {
            rNext.left -= 1;
            if(m_nNextState == BNST_PRESSED)
            {
                dcMem.DrawFrameControl(rNext, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
            }
            else
            {
                dcMem.DrawFrameControl(rNext, DFC_BUTTON, DFCS_BUTTONPUSH);
            }
        }
        DrawGlyph(dcMem, ptNext, 2, m_nNextState-1);
    }

    // draw lines around tabs
    int nOffsetX = rAll.Width();

    if(! ((BOOL) (GetStyle()&CTCS_NOLINES)) )
    {
        CPoint pts[4];
        if(fAfter)
        {
            //buttons are after tabs.
            if(GetStyle()&CTCS_TOP)
            {
                //nothing selected?
                if(m_nItemSelected==-1)
                {
                    pts[0] = CPoint(0, rCl.bottom-1); 
                    pts[1] = CPoint(0, rCl.bottom-1); 
                    pts[2] = CPoint(0, rCl.bottom-1); 
                    pts[3] = CPoint(rCl.right-nOffsetX, rCl.bottom-1);
                }
                else
                {
                    //something selected.

                    //is selected tab visible?
                    if(m_aItems[m_nItemSelected]->m_bShape)
                    {
                        pts[0] = CPoint(0, rCl.bottom-1); 
                        pts[1] = CPoint(m_aItems[m_nItemSelected]->m_rect.left, rCl.bottom-1); 
                        pts[2] = CPoint(m_aItems[m_nItemSelected]->m_rect.right, rCl.bottom-1); 
                        pts[3] = CPoint(rCl.right-nOffsetX, rCl.bottom-1);
                    }
                    else
                    {
                        //no it is not.
                        pts[0] = CPoint(0, rCl.bottom-1); 
                        pts[1] = CPoint(0, rCl.bottom-1); 
                        pts[2] = CPoint(0, rCl.bottom-1); 
                        pts[3] = CPoint(rCl.right-nOffsetX, rCl.bottom-1);
                    }
                }
            }//if top
            else
            {
                //buttons after tabs, and BOTTOM style.		

                //nothing selected?
                if(m_nItemSelected==-1)
                {
                    pts[0] = CPoint(rCl.left, rCl.top); 
                    pts[1] = CPoint(rCl.left, rCl.top); 
                    pts[2] = CPoint(rCl.left, rCl.top); 
                    pts[3] = CPoint(rCl.right-nOffsetX, rCl.top);
                }
                else
                {
                    if(m_aItems[m_nItemSelected]->m_bShape)
                    {
                        pts[0] = CPoint(rCl.left, rCl.top); 
                        pts[1] = CPoint(m_aItems[m_nItemSelected]->m_rect.left, rCl.top); 
                        pts[2] = CPoint(m_aItems[m_nItemSelected]->m_rect.right, rCl.top); 
                        pts[3] = CPoint(rCl.right-nOffsetX, rCl.top);
                    }
                    else
                    {
                        pts[0] = CPoint(rCl.left, rCl.top); 
                        pts[1] = CPoint(rCl.left, rCl.top); 
                        pts[2] = CPoint(rCl.left, rCl.top); 
                        pts[3] = CPoint(rCl.right-nOffsetX, rCl.top);
                    }
                }
            }//if bottom
        }//if after
        else
        {
            //buttons before tabs.

            if(GetStyle()&CTCS_TOP)
            {
                if(m_nItemSelected==-1)
                {
                    pts[0] = CPoint(nOffsetX, rCl.bottom-1); 
                    pts[1] = CPoint(nOffsetX, rCl.bottom-1); 
                    pts[2] = CPoint(nOffsetX, rCl.bottom-1); 
                    pts[3] = CPoint(rCl.right, rCl.bottom-1);
                }
                else
                {
                    if(m_aItems[m_nItemSelected]->m_bShape)
                    {
                        pts[0] = CPoint(nOffsetX, rCl.bottom-1); 
                        pts[1] = CPoint(m_aItems[m_nItemSelected]->m_rect.left, rCl.bottom-1); 
                        pts[2] = CPoint(m_aItems[m_nItemSelected]->m_rect.right, rCl.bottom-1); 
                        pts[3] = CPoint(rCl.right, rCl.bottom-1);
                    }
                    else
                    {
                        pts[0] = CPoint(nOffsetX, rCl.bottom-1); 
                        pts[1] = CPoint(nOffsetX, rCl.bottom-1); 
                        pts[2] = CPoint(nOffsetX, rCl.bottom-1); 
                        pts[3] = CPoint(rCl.right, rCl.bottom-1);
                    }
                }
            }//if top
            else
            {
                if(m_nItemSelected==-1)
                {
                    pts[0] = CPoint(nOffsetX, rCl.top); 
                    pts[1] = CPoint(nOffsetX, rCl.top); 
                    pts[2] = CPoint(nOffsetX, rCl.top); 
                    pts[3] = CPoint(rCl.right, rCl.top);
                }
                else
                {
                    if(m_aItems[m_nItemSelected]->m_bShape)
                    {
                        pts[0] = CPoint(nOffsetX, rCl.top); 
                        pts[1] = CPoint(m_aItems[m_nItemSelected]->m_rect.left, rCl.top); 
                        pts[2] = CPoint(m_aItems[m_nItemSelected]->m_rect.right, rCl.top); 
                        pts[3] = CPoint(rCl.right, rCl.top);
                    }
                    else
                    {
                        pts[0] = CPoint(nOffsetX, rCl.top); 
                        pts[1] = CPoint(nOffsetX, rCl.top); 
                        pts[2] = CPoint(nOffsetX, rCl.top); 
                        pts[3] = CPoint(rCl.right, rCl.top);
                    }
                }
            }//if bottom
        }

        CPen* pOldPen = dcMem.SelectObject(&blackPen);

        //draw lines	
        dcMem.MoveTo(pts[0]);
        dcMem.LineTo(pts[1]);
        dcMem.MoveTo(pts[2]);
        dcMem.LineTo(pts[3]);

        dcMem.SelectObject(pOldPen);
    }//if ! NOLINES

    //ok all drawing done, now display control
    if(IsVertical())
    {
        //need to rotate
#ifndef _WIN32_WCE
        //on nt/2000/xp, very simple
        POINT pts[3];
        pts[0].x = 0;
        pts[0].y = rCl.Width();
        pts[1].x = 0;
        pts[1].y = 0;
        pts[2].x = rCl.Height();
        pts[2].y = rCl.Width();
        ::PlgBlt(dc.m_hDC, pts, dcMem.m_hDC, 0, 0, rCl.Width(), rCl.Height(), NULL, 0, 0);
#else
        //on PocketPC/Wince, a bit more complicated...
        {
            register int x, y, w, h;
            register COLORREF cr;

            w=rCl.Width();
            h=rCl.Height();

            for(x=0; x<w; x++)
            {
                for(y=0; y<h; y++)
                {
                    cr=dcMem.GetPixel(x, y);
                    dc.SetPixel(y, w-x, cr);
                }
            }
        }
#endif
    }
    else
    {
        //just display
        dc.BitBlt(rCl.left, rCl.top, rCl.Width(), rCl.Height(), &dcMem, rCl.left, rCl.top, SRCCOPY);
        //		dc.BitBlt(rWnd.left, rWnd.top, rWnd.Width(), rWnd.Height(), &dcMem, rWnd.left, rWnd.top, SRCCOPY);
    }
    dcMem.SelectObject(pOldBmp);
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
    CWnd::OnSize(nType, cx, cy);
    if(cx && cy)
    {
        RecalcLayout(RECALC_RESIZED, m_nItemSelected);
        Display();
    }
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    int nHitTest;

    nHitTest = HitTest(point);

    NotifyParent(CTCN_CLICK, nHitTest, point);
    ProcessLButtonDown(nHitTest, nFlags, point);

    //avoid gesture recognition
#ifndef _WIN32_WCE
    CWnd::OnLButtonDown(nFlags,point);
#else
    Default();
#endif
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    int nHTRet;

    nHTRet = ProcessLButtonDown(HitTest(point), nFlags, point);
    if(nHTRet>=0)
    {
        m_nButtonIDDown = CTCID_NOBUTTON;
        if(nHTRet==HitTest(point))
        {
            EditLabel(nHTRet, TRUE);
        }
    }
    NotifyParent(CTCN_DBLCLK, HitTest(point), point);
    CWnd::OnLButtonDblClk(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::ProcessLButtonDown(int nHitTest, UINT nFlags, CPoint point)
{
    int i;

    SetCapture();
    switch(nHitTest)
    {
    case CTCHT_NOWHERE:
        {
            m_nButtonIDDown = CTCID_NOBUTTON;
            break;
        }
    case CTCHT_ONFIRSTBUTTON:
        {
            m_nButtonIDDown = CTCID_FIRSTBUTTON;
            m_nFirstState = BNST_PRESSED;
            RecalcLayout(RECALC_FIRST_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            m_dwLastRepeatTime = ::GetTickCount();
            SetTimer(CTC_TIMER_ID, CTC_TIMER_TIMEOUT, NULL);
            break;
        }
    case CTCHT_ONPREVBUTTON:
        {
            m_nButtonIDDown = CTCID_PREVBUTTON;
            m_nPrevState = BNST_PRESSED;
            RecalcLayout(RECALC_PREV_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            m_dwLastRepeatTime = ::GetTickCount();
            SetTimer(CTC_TIMER_ID, CTC_TIMER_TIMEOUT, NULL);
            break;
        }
    case CTCHT_ONNEXTBUTTON:
        {
            m_nButtonIDDown = CTCID_NEXTBUTTON;
            m_nNextState = BNST_PRESSED;
            RecalcLayout(RECALC_NEXT_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            m_dwLastRepeatTime = ::GetTickCount();
            SetTimer(CTC_TIMER_ID, CTC_TIMER_TIMEOUT, NULL);
            break;
        }
    case CTCHT_ONLASTBUTTON:
        {
            m_nButtonIDDown = CTCID_LASTBUTTON;
            m_nLastState = BNST_PRESSED;
            RecalcLayout(RECALC_LAST_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            m_dwLastRepeatTime = ::GetTickCount();
            SetTimer(CTC_TIMER_ID, CTC_TIMER_TIMEOUT, NULL);
            break;
        }
    case CTCHT_ONCLOSEBUTTON:
        {
            m_nButtonIDDown = CTCID_CLOSEBUTTON;
            m_nCloseState = BNST_PRESSED;
            RecalcLayout(RECALC_CLOSE_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            break;
        }
    default:
        {
            DWORD dwStyle = GetStyle();
            m_nButtonIDDown = CTCID_NOBUTTON;

            if((GetStyle()&CTCS_MULTIHIGHLIGHT) && (nFlags&MK_CONTROL))
            {
                HighlightItem(nHitTest, TRUE, nFlags&MK_CONTROL);
            }
            else
            {
                BOOL bNotify = nHitTest!=m_nItemSelected;
                SetCurSel(nHitTest, TRUE, nFlags&MK_CONTROL);
                if(bNotify)
                {
                    NotifyParent(CTCN_SELCHANGE, m_nItemSelected, point);
                }
            }
            for(i=0; i<m_aItems.GetSize(); i++)
            {
                if(m_aItems[i]->m_fHighlightChanged)
                {
                    NotifyParent(CTCN_HIGHLIGHTCHANGE, i, point);
                }
            }
        }
        break;
    }
    return nHitTest;
}

//////////////////////////////////////////////////////////////////////////
BOOL CCustomTabCtrl::NotifyParent(UINT code, int nItem, CPoint pt)
{
    CTC_NMHDR nmh;

    memset(&nmh, 0, sizeof(CTC_NMHDR));
    nmh.hdr.hwndFrom = GetSafeHwnd();
    nmh.hdr.idFrom = GetDlgCtrlID();
    nmh.hdr.code = code;
    nmh.nItem = nItem;
    nmh.ptHitTest = pt;
    if(nItem>=0)
    {
        _tcscpy(nmh.pszText, m_aItems[nItem]->m_sText);
        nmh.lParam = m_aItems[nItem]->m_lParam;
        nmh.rItem = m_aItems[nItem]->m_rectText;
        nmh.fSelected = m_aItems[nItem]->m_fSelected;
        nmh.fHighlighted = m_aItems[nItem]->m_fHighlighted;
    }
    return (BOOL)GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmh);
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::OnLButtonUp(UINT /*nFlags*/, CPoint /*point*/)
{
    if(m_nPrevState || m_nNextState || m_nFirstState || m_nLastState || m_nCloseState)
    {
        if(m_nCloseState)
        {
            m_nCloseState = BNST_NORMAL;
        }
        if(m_nPrevState)
        {
            m_nPrevState = BNST_NORMAL;
            m_nNextState = BNST_NORMAL;
        }
        if(m_nFirstState)
        {
            m_nFirstState = BNST_NORMAL;
            m_nLastState = BNST_NORMAL;
        }
        Invalidate(FALSE);
        KillTimer(1);
    }
    m_nButtonIDDown = CTCID_NOBUTTON;
    m_nItemDragDest = CTCID_NOBUTTON;
    ReleaseCapture();
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::OnUpdateEdit() 
{
    if(m_ctrlEdit.m_hWnd)
    {
        m_ctrlEdit.GetWindowText(m_aItems[m_nItemSelected]->m_sText);
        RecalcLayout(RECALC_EDIT_RESIZED, m_nItemSelected);
        Invalidate(FALSE);
    }
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::OnTimer(UINT nIDEvent) 
{
    CWnd::OnTimer(nIDEvent);

    if(nIDEvent == CTC_TIMER_ID)
    {
        if(m_nFirstState==BNST_PRESSED && ::GetTickCount()-m_dwLastRepeatTime >= REPEAT_TIMEOUT)
        {
            m_nFirstState = BNST_PRESSED;
            RecalcLayout(RECALC_FIRST_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            m_dwLastRepeatTime = ::GetTickCount();
            return;

        }
        if(m_nPrevState==BNST_PRESSED && ::GetTickCount()-m_dwLastRepeatTime >= REPEAT_TIMEOUT)
        {
            m_nPrevState = BNST_PRESSED;
            RecalcLayout(RECALC_PREV_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            m_dwLastRepeatTime = ::GetTickCount();
            return;

        }
        if(m_nNextState==BNST_PRESSED && ::GetTickCount()-m_dwLastRepeatTime >= REPEAT_TIMEOUT)
        {
            m_nNextState = BNST_PRESSED;
            RecalcLayout(RECALC_NEXT_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            m_dwLastRepeatTime = ::GetTickCount();
            return;
        }
        if(m_nLastState==BNST_PRESSED && ::GetTickCount()-m_dwLastRepeatTime >= REPEAT_TIMEOUT)
        {
            m_nLastState = BNST_PRESSED;
            RecalcLayout(RECALC_LAST_PRESSED, m_nItemSelected);
            Invalidate(FALSE);
            m_dwLastRepeatTime = ::GetTickCount();
            return;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::SetControlFont(const LOGFONT& lf, BOOL fRedraw)
{
    LOGFONT lfSel;

    if(m_Font.m_hObject)
    {
        DeleteObject(m_Font);
        m_Font.m_hObject = NULL;
    }

    if(m_FontSelected.m_hObject)
    {
        DeleteObject(m_FontSelected);
        m_FontSelected.m_hObject = NULL;
    }

    if(!m_Font.CreateFontIndirect(&lf))
    {
        m_Font.CreateFontIndirect(&lf_default);
    }

    m_Font.GetLogFont(&lfSel);
    lfSel.lfWeight = FW_BOLD;
    m_FontSelected.CreateFontIndirect(&lfSel);

    if(fRedraw)
    {
        RecalcLayout(RECALC_RESIZED, m_nItemSelected);
        Invalidate();
    }
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::InsertItem(int nItem, CString sText, LPARAM lParam)
{
    if(nItem<0 || nItem > m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }

    if(sText.GetLength() > MAX_LABEL_LENGTH-1)
    {
        return CTCERR_TEXTTOOLONG;
    }

    CCustomTabItem* pItem = new CCustomTabItem(sText, lParam);
    if(pItem==NULL)
    {
        return CTCERR_OUTOFMEMORY;
    }

    m_aItems.InsertAt(nItem, pItem);

    if(m_nItemSelected >= nItem)
    {
        m_nItemSelected++;
    }

    RecalcLayout(RECALC_RESIZED, m_nItemSelected);
    Invalidate(FALSE);

    return nItem;
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::MoveItem(int nItemSrc, int nItemDst)
{
    return MoveItem(nItemSrc, nItemDst, FALSE);
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::MoveItem(int nItemSrc, int nItemDst, BOOL fMouseSel)
{
    if(nItemSrc<0||nItemSrc>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }
    if(nItemDst<0||nItemDst>m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }

    if(nItemSrc==nItemDst || nItemSrc==nItemDst-1)
    {
        return nItemSrc;
    }

    CCustomTabItem *pItem = m_aItems[nItemSrc];

    // remove item from old place
    m_aItems.RemoveAt(nItemSrc);

    // insert item in new place
    if(nItemDst>nItemSrc)
    {
        nItemDst--;
    }

    m_aItems.InsertAt(nItemDst, pItem);

    m_nItemSelected = nItemDst;

    RecalcLayout(RECALC_ITEM_SELECTED, m_nItemSelected);
    Invalidate(FALSE);
    if(fMouseSel)
    {
        NotifyParent(CTCN_ITEMMOVE, m_nItemSelected, CPoint(0, 0));
    }
    return nItemDst;
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::CopyItem(int nItemSrc, int nItemDst)
{
    return CopyItem(nItemSrc, nItemDst, FALSE);
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::CopyItem(int nItemSrc, int nItemDst, BOOL fMouseSel)
{
    int i;
    CString sDst;

    if(nItemSrc<0||nItemSrc>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }
    if(nItemDst<0||nItemDst>m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }

    BOOL bAppendFlag=TRUE;
    int n = m_aItems[nItemSrc]->m_sText.GetLength();
    if(n>=4)
    {
        if(m_aItems[nItemSrc]->m_sText[n-1]==_T(')') && 
            m_aItems[nItemSrc]->m_sText[n-2]>_T('1') &&
            m_aItems[nItemSrc]->m_sText[n-2]<=_T('9') &&
            m_aItems[nItemSrc]->m_sText[n-3]==_T('('))
        {
            n = m_aItems[nItemSrc]->m_sText.GetLength()-3;
            bAppendFlag = FALSE;
        }
        else if(m_aItems[nItemSrc]->m_sText[n-1]==_T(')') && 
            m_aItems[nItemSrc]->m_sText[n-2]>=_T('0') &&
            m_aItems[nItemSrc]->m_sText[n-2]<=_T('9') &&
            m_aItems[nItemSrc]->m_sText[n-3]>=_T('1') &&
            m_aItems[nItemSrc]->m_sText[n-3]<=_T('9') &&
            m_aItems[nItemSrc]->m_sText[n-4]==_T('('))
        {
            n = m_aItems[nItemSrc]->m_sText.GetLength()-4;
            bAppendFlag = FALSE;
        }
    }
    int ndx = 1;
    while(1)
    {
        ndx++;
        if(bAppendFlag)
        {
            sDst.Format(_T("%s (%d)"), (LPCTSTR)m_aItems[nItemSrc]->m_sText, ndx);
        }
        else
        {
            sDst.Format(_T("%s(%d)"), (LPCTSTR)m_aItems[nItemSrc]->m_sText.Left(n), ndx);
        }
        for(i=0; i<m_aItems.GetSize(); i++)
        {
            if(m_aItems[i]->m_sText==sDst)
            {
                break;
            }
        }
        if(i == m_aItems.GetSize())
        {
            break;
        }
    }

    int nRetItem = InsertItem(nItemDst,sDst,m_aItems[nItemSrc]->m_lParam);
    if(nRetItem>=0)
    {
        SetCurSel(nRetItem);
        if(fMouseSel)
        {
            NotifyParent(CTCN_ITEMCOPY, nRetItem, CPoint(0, 0));
        }
    }
    else if(fMouseSel && nRetItem==CTCERR_OUTOFMEMORY)
    {
        NotifyParent(CTCN_OUTOFMEMORY, nRetItem, CPoint(0, 0));
    }

    return nRetItem;
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::DeleteItem(int nItem)
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }

    if(m_aItems.GetSize()==1)
    {
        m_nItemSelected = -1;
    }
    else if(m_nItemSelected==nItem)
    {
        if(m_nItemSelected==m_aItems.GetSize()-1) // last item
        {
            m_nItemSelected--;
            m_aItems[m_nItemSelected]->m_fSelected = TRUE;	
        }
        else
        {
            m_aItems[m_nItemSelected+1]->m_fSelected = TRUE;	
        }
    }
    else if(m_nItemSelected>nItem)
    {
        m_nItemSelected--;
    }

    delete m_aItems[nItem];
    m_aItems.RemoveAt(nItem);

    RecalcLayout(RECALC_RESIZED, m_nItemSelected);
    Invalidate(FALSE);
    return CTCERR_NOERROR;
}

void CCustomTabCtrl::DeleteAllItems()
{
    {
        for(int i=0; i< m_aItems.GetSize(); i++)
            delete m_aItems[i];
    }

    m_aItems.RemoveAll();

    m_nItemSelected = -1;

    RecalcLayout(RECALC_RESIZED,m_nItemSelected);
    Invalidate(FALSE);
}

int CCustomTabCtrl::SetCurSel(int nItem)
{
    return SetCurSel(nItem,FALSE,FALSE);
}

int CCustomTabCtrl::HighlightItem(int nItem, BOOL fHighlight)
{
    if(!(GetStyle()&CTCS_MULTIHIGHLIGHT))
        return CTCERR_NOMULTIHIGHLIGHTSTYLE;
    if(nItem<0 || nItem>=m_aItems.GetSize())
        return CTCERR_INDEXOUTOFRANGE;
    if(m_nItemSelected==-1 && !fHighlight)
        return CTCERR_NOERROR;
    if(m_nItemSelected==-1)
    {
        SetCurSel(nItem);
        return CTCERR_NOERROR;
    }
    if(fHighlight==m_aItems[nItem]->m_fHighlighted || nItem==m_nItemSelected)
        return CTCERR_NOERROR;

    m_aItems[nItem]->m_fHighlighted = fHighlight;
    return CTCERR_NOERROR;
}

int CCustomTabCtrl::GetItemText(int nItem, CString& sText)
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
        return CTCERR_INDEXOUTOFRANGE;
    sText = m_aItems[nItem]->m_sText;
    return CTCERR_NOERROR;
}

int CCustomTabCtrl::SetItemText(int nItem, CString sText)
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
        return CTCERR_INDEXOUTOFRANGE;
    m_aItems[nItem]->m_sText = sText;
    RecalcLayout(RECALC_RESIZED,m_nItemSelected);
    Invalidate(FALSE);
    return CTCERR_NOERROR;
}


int CCustomTabCtrl::GetItemData(int nItem, DWORD& dwData)
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }
    dwData = (DWORD)m_aItems[nItem]->m_lParam;
    return CTCERR_NOERROR;
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::GetItemRect(int nItem, CRect& rect) const
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }
    rect = m_aItems[nItem]->m_rectText;
    return CTCERR_NOERROR;
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::SetItemData(int nItem, DWORD dwData)
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }
    m_aItems[nItem]->m_lParam = dwData;
    return CTCERR_NOERROR;
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::IsItemHighlighted(int nItem)
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }
    return (m_aItems[nItem]->m_fHighlighted)?1:0;
}

//////////////////////////////////////////////////////////////////////////
int	CCustomTabCtrl::HitTest(CPoint pt)
{
    CRect rCl;
    int i, nBtns;

    GetTabsRect(&rCl);
    if(IsVertical())
    {
        rCl.SetRect(0, 0, rCl.Height(), rCl.Width());
        pt = CPoint(rCl.Width()-pt.y, pt.x);
    }

    nBtns = 0;
    if(m_nCloseState)
    {
        nBtns++;
    }
    if(m_nPrevState)
    {
        nBtns += 2;
    }
    if(m_nFirstState)
    {
        nBtns += 2;
    }
    int nA = rCl.Height()-2;

    int nCloseOffset = 0;
    if(m_nCloseState)
    {
        nCloseOffset = nA;
        CRect rClose(0, rCl.top, nA+1, rCl.bottom);
        if(GetStyle()&CTCS_BUTTONSAFTER)
        {
            rClose.SetRect(rCl.Width()-nA-1, rCl.top, rCl.Width(), rCl.bottom);
        }
        if(rClose.PtInRect(pt))
        {
            return CTCHT_ONCLOSEBUTTON;
        }
    }

    CRect rFirst, rPrev, rNext, rLast;

    if(GetStyle()&CTCS_BUTTONSAFTER)
    {
        if(nBtns==2 || nBtns==3)
        {
            rNext.SetRect(rCl.Width()-nCloseOffset-nA-1, rCl.top, rCl.Width()-nCloseOffset, rCl.bottom);
            rPrev.SetRect(rCl.Width()-nCloseOffset-2*nA-3, rCl.top, rCl.Width()-nCloseOffset-nA-1, rCl.bottom);
        }
        else if(nBtns==4 || nBtns==5)
        {
            rLast.SetRect(rCl.Width()-nCloseOffset-nA-1, rCl.top, rCl.Width()-nCloseOffset, rCl.bottom);
            rNext.SetRect(rCl.Width()-nCloseOffset-2*nA-1, rCl.top, rCl.Width()-nCloseOffset-nA-1, rCl.bottom);
            rPrev.SetRect(rCl.Width()-nCloseOffset-3*nA-2, rCl.top, rCl.Width()-nCloseOffset-2*nA-1, rCl.bottom);
            rFirst.SetRect(rCl.Width()-nCloseOffset-4*nA-3, rCl.top, rCl.Width()-nCloseOffset-3*nA-2, rCl.bottom);
        }
    }
    else
    {
        if(nBtns==2 || nBtns==3)
        {
            rPrev.SetRect(nCloseOffset, rCl.top, nCloseOffset+nA+1, rCl.bottom);
            rNext.SetRect(nCloseOffset+nA+1, rCl.top, nCloseOffset+2*nA+3, rCl.bottom);
        }
        else if(nBtns==4 || nBtns==5)
        {
            rFirst.SetRect(nCloseOffset, rCl.top, nCloseOffset+nA+1, rCl.bottom);
            rPrev.SetRect(nCloseOffset+nA+1, rCl.top, nCloseOffset+2*nA+1, rCl.bottom);
            rNext.SetRect(nCloseOffset+2*nA+1, rCl.top, nCloseOffset+3*nA+2, rCl.bottom);
            rLast.SetRect(nCloseOffset+3*nA+2, rCl.top, nCloseOffset+4*nA+3, rCl.bottom);
        }
    }

    if(nBtns>=4 && m_nFirstState && rFirst.PtInRect(pt))
    {
        return CTCHT_ONFIRSTBUTTON;
    }

    if(m_nPrevState && rPrev.PtInRect(pt))
    {
        return CTCHT_ONPREVBUTTON;
    }

    if(m_nNextState && rNext.PtInRect(pt))
    {
        return CTCHT_ONNEXTBUTTON;
    }

    if(nBtns>=4 && m_nLastState && rLast.PtInRect(pt))
    {
        return CTCHT_ONLASTBUTTON;
    }

    for(i=0; i<m_aItems.GetSize(); i++)
    {
        if(m_aItems[i]->HitTest(pt))
        {
            return i;
        }
    }
    return CTCHT_NOWHERE;
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::HighlightItem(int nItem, BOOL fMouseSel, BOOL fCtrlPressed)
{
    int i;

    if(!(GetStyle()&CTCS_MULTIHIGHLIGHT))
    {
        return CTCERR_NOMULTIHIGHLIGHTSTYLE;
    }

    for(i=0; i<m_aItems.GetSize();i++)
    {
        m_aItems[i]->m_fHighlightChanged = FALSE;
    }

    if(fCtrlPressed)
    {
        if(nItem!=m_nItemSelected)
        {
            m_aItems[nItem]->m_fHighlighted = !m_aItems[nItem]->m_fHighlighted;
            if(fMouseSel)
            {
                m_aItems[nItem]->m_fHighlightChanged = TRUE;
            }
        }
    }
    else
        if(!m_aItems[nItem]->m_fHighlighted)
        {
            m_aItems[nItem]->m_fHighlighted = TRUE;
            m_aItems[nItem]->m_fHighlightChanged = TRUE;
            for(i=0; i<m_aItems.GetSize(); i++)
            {
                if(i!=m_nItemSelected)
                {
                    if(m_aItems[i]->m_fHighlighted)
                    {
                        m_aItems[i]->m_fHighlighted = FALSE;
                        if(fMouseSel)
                        {
                            m_aItems[i]->m_fHighlightChanged = TRUE;
                        }
                    }
                }
            }//for items
        }

        if(fMouseSel)
        {
            RecalcLayout(RECALC_ITEM_SELECTED,nItem);
        }
        Invalidate(FALSE);
        return CTCERR_NOERROR;
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::SetCurSel(int nItem, BOOL fMouseSel, BOOL fCtrlPressed)
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }

    if(m_nItemSelected!=-1)
    {
        m_aItems[m_nItemSelected]->m_fSelected = FALSE;
    }

    m_nItemSelected = nItem;

    if(m_nItemSelected!=-1)
    {
        m_aItems[m_nItemSelected]->m_fSelected = TRUE;
    }

    if(fMouseSel)
    {
        RecalcLayout(RECALC_ITEM_SELECTED, m_nItemSelected);
    }
    else
    {
        m_nFirstVisibleItem = nItem;
        RecalcLayout(RECALC_RESIZED, m_nItemSelected);
    }
    Invalidate(FALSE);
    HighlightItem(nItem, fMouseSel, fCtrlPressed);
    return CTCERR_NOERROR;
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::RecalcLayout(int nRecalcType, int nItem)
{
    CRect rCl;
    int nCloseOffset, nA, nBnWidth, nBtns;

    GetTabsRect(&rCl);

    if(IsVertical())
    {
        rCl.SetRect(0, 0, rCl.Height(), rCl.Width());
    }

    nCloseOffset = 0;
    nA = rCl.Height()-3;
    nBnWidth = 0;
    nBtns = 0;

    if(GetStyle()&CTCS_CLOSEBUTTON)
    {
        nBtns++;
        nCloseOffset = nA;
        nBnWidth = nA+3;
        //show close button
        if(m_nCloseState == BNST_INVISIBLE)
        {
            m_nCloseState = BNST_NORMAL;
        }
    }
    else
    {
        m_nCloseState = BNST_INVISIBLE;
    }

    int nWidth = RecalcRectangles();

    if((GetStyle()&CTCS_AUTOHIDEBUTTONS) && (m_aItems.GetSize()<2 || nWidth <= rCl.Width()-nBnWidth))
    {
        //hide buttons
        m_nFirstState = BNST_INVISIBLE;
        m_nPrevState = BNST_INVISIBLE;
        m_nNextState = BNST_INVISIBLE;
        m_nLastState = BNST_INVISIBLE;
        m_nFirstVisibleItem = 0;

        RecalcOffset(nBnWidth);
        if(nRecalcType == RECALC_EDIT_RESIZED)
        {
            RecalcEditResized(0, nItem);
        }
        return;
    }

    if(m_nPrevState==BNST_INVISIBLE)
    {
        m_nPrevState = BNST_NORMAL;
        m_nNextState = BNST_NORMAL;
    }

    if(GetStyle()&CTCS_FOURBUTTONS)
    {
        nBtns += 4;
        if(m_nFirstState==BNST_INVISIBLE)
        {
            m_nFirstState = BNST_NORMAL;
            m_nLastState = BNST_NORMAL;
        }
    }
    else
    {
        nBtns += 2;
        m_nFirstState = BNST_INVISIBLE;
        m_nLastState = BNST_INVISIBLE;
    }

    if(m_aItems.GetSize()==0)
    {
        return;
    }

    nBnWidth = nBtns*nA+3;	
    if(GetStyle()&CTCS_BUTTONSAFTER)
    {
        rCl.right -= nBnWidth;
    }

    switch(nRecalcType)
    {
    case RECALC_CLOSE_PRESSED:
        {
            RecalcRectangles();
            RecalcOffset(nBnWidth);
            break;
        }
    case RECALC_FIRST_PRESSED:
        {
            m_nFirstVisibleItem=0;
            RecalcRectangles();
            RecalcOffset(nBnWidth);
            break;
        }
    case RECALC_PREV_PRESSED:
        {	
            RecalcOffset(nBnWidth);
            if(m_nFirstVisibleItem > 0)
            {
                m_nFirstVisibleItem--;
                RecalcRectangles();
                RecalcOffset(nBnWidth);
            }
            break;
        }
    case RECALC_NEXT_PRESSED:
        {	
            RecalcOffset(nBnWidth);
            if(m_aItems[m_aItems.GetSize()-1]->m_rect.right > rCl.Width() && m_nFirstVisibleItem != m_aItems.GetSize()-1)
            {
                m_nFirstVisibleItem++;
                RecalcRectangles();
                RecalcOffset(nBnWidth);
            }
            break;
        }
    case RECALC_ITEM_SELECTED:
        {
            RecalcOffset(nBnWidth);
            if(m_aItems[nItem]->m_bShape==TAB_BEFORE_FIRST_VISIBLE)
            {
                m_nFirstVisibleItem--;
                RecalcRectangles();
                RecalcOffset(nBnWidth);
            }
            else
            {
                while(m_nFirstVisibleItem < nItem && 
                    m_aItems[nItem]->m_bShape==TAB_VISIBLE && 
                    m_aItems[nItem]->m_rect.right > rCl.Width() && 
                    m_aItems[nItem]->m_rect.left > ((GetStyle()&CTCS_BUTTONSAFTER)? 0:nBnWidth))
                {
                    m_nFirstVisibleItem++;
                    RecalcRectangles();
                    RecalcOffset(nBnWidth);
                }
            }
            break;
        }
    case RECALC_EDIT_RESIZED:
        {
            RecalcOffset(nBnWidth);
            RecalcEditResized(nBnWidth,nItem);
            break;
        }
    case RECALC_LAST_PRESSED:
        {
            m_nFirstVisibleItem=(int)m_aItems.GetSize()-1;

            //no break here?
        }
    default:	// window resized
        {
            BOOL bNdxOffsetChanged = FALSE;
            RecalcOffset(nBnWidth);
            while(m_nFirstVisibleItem>=0 && m_aItems[m_aItems.GetSize()-1]->m_rect.right < rCl.Width())
            {
                m_nFirstVisibleItem--;
                if(m_nFirstVisibleItem>=0)
                {
                    RecalcRectangles();
                    RecalcOffset(nBnWidth);
                }
                bNdxOffsetChanged = TRUE;
            }
            if(bNdxOffsetChanged)
            {
                m_nFirstVisibleItem++;
                RecalcRectangles();
                RecalcOffset(nBnWidth);
            }
            break;
        }//default
    }//switch recalc type
}

///////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::RecalcEditResized(int nOffset, int nItem)
{
    CRect rCl;

    GetTabsRect(rCl);

    if(GetStyle() & CTCS_BUTTONSAFTER)
    {
        rCl.right -= nOffset;
    }

    do
    {
        CRect r;
        CDC* pDC = GetDC();
        CFont* pOldFont = pDC->SelectObject(&m_FontSelected);
        int h = pDC->DrawText(m_aItems[nItem]->m_sText+_T("X"), r, DT_CALCRECT);
        pDC->SelectObject(pOldFont);
        ReleaseDC(pDC);
        r = m_aItems[nItem]->m_rectText;
        if(r.Height()>h)
        {
            r.top += (r.Height()-h)/2;
            r.bottom -= (r.Height()-h)/2;
        }
        r.left += 2;
        if(r.right>rCl.right && m_nItemSelected>m_nFirstVisibleItem)
        {
            m_nFirstVisibleItem++;
            RecalcRectangles();
            RecalcOffset(nOffset);
        }
        else
        {
            if(r.right>rCl.right)
            {
                r.right = rCl.right;
            }
            m_ctrlEdit.MoveWindow(r);
            int n = m_aItems[nItem]->m_sText.GetLength();
            int nStart, nEnd;
            m_ctrlEdit.GetSel(nStart, nEnd);
            if(nStart == nEnd && nStart == n)
            {
                m_ctrlEdit.SetSel(0, 0);
                m_ctrlEdit.SetSel(n, n);
            }
            return;
        }
    } 
    while(1);
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::RecalcOffset(int nOffset)
{
    CRect rCl;
    int i;

    GetTabsRect(&rCl);

    if(IsVertical())
    {
        rCl.SetRect(0, 0, rCl.Height(), rCl.Width());
    }

    int rightAdjusment = 0;

    if(GetStyle() & CTCS_BUTTONSAFTER)
    {
        rightAdjusment = nOffset;
        nOffset = 0;
    }

    for(i = 0; i<m_aItems.GetSize(); i++)
    {
        if(i < m_nFirstVisibleItem-1)
        {
            m_aItems[i]->m_bShape = TAB_INVISIBLE;
            //nOffset -= m_aItems[i]->m_rect.Width()-rCl.Height()/2;
            nOffset -= m_aItems[i]->m_rect.Width();
            m_aItems[i]->m_rectText.SetRectEmpty();
        }
        else if(i == m_nFirstVisibleItem-1)
        {
            int nBtns = 2;

            if(GetStyle() & CTCS_FOURBUTTONS)
            {
                nBtns = 4;
            }

            if(GetStyle() & CTCS_CLOSEBUTTON)
            {
                nBtns++;
            }

            int nBnWidth = nBtns*(rCl.Height() - 3) + 3;

            m_aItems[i]->m_bShape = TAB_BEFORE_FIRST_VISIBLE;

            //nOffset -= m_aItems[i]->m_rect.Width()-rCl.Height()/2;
            //no overlapping
            nOffset -= m_aItems[i]->m_rect.Width();

            //overlap here
            m_aItems[i]->m_rect.SetRect(0, 1, rCl.Height()/2, rCl.Height()-1);
            if(!(GetStyle() & CTCS_BUTTONSAFTER))
            {
                m_aItems[i]->m_rect.OffsetRect(nBnWidth, 0);
            }

            //this prevents some overlapping in case of no BUTTONSAFTER style
            m_aItems[i]->m_rect.SetRectEmpty();
            //text rect empty anyways
            m_aItems[i]->m_rectText.SetRectEmpty();
        }
        else
        {
            m_aItems[i]->m_bShape = TAB_VISIBLE;
            m_aItems[i]->m_rect.OffsetRect(nOffset, 0);
            m_aItems[i]->m_rectText.OffsetRect(nOffset, 0);
        }
    }//for tabs
}

//////////////////////////////////////////////////////////////////////////
//recalculate tab item rectangles (item rect and item text rect)
int CCustomTabCtrl::RecalcRectangles()
{
    CRect rCl;
    int i, w, h;	
    BOOL fTop;
    int nWidth;

    GetTabsRect(&rCl);

    if(IsVertical())
    {
        rCl.SetRect(0, 0, rCl.Height(), rCl.Width());
    }

    fTop = GetStyle()&CTCS_TOP;
    nWidth = 0;
    {
        //calculate widths
        int nOffset = 0;
        CRect rcText;
        CDC* pDC = GetDC();
        CFont* pOldFont = pDC->SelectObject(&m_FontSelected);

        if(GetStyle()&CTCS_FIXEDWIDTH)
        {
            int nMaxWidth=0;

            //get max width of items, based on item text
            for(i=0; i<m_aItems.GetSize(); i++)
            {
                w=0;
                h = pDC->DrawText(m_aItems[i]->m_sText, rcText, DT_CALCRECT);

                if(h>0)
                {
                    w = rcText.Width();
                }
                if(w>nMaxWidth)
                {
                    nMaxWidth = w;
                }
            }//for items

            for(i=0; i<m_aItems.GetSize(); i++)
            {
                if(fTop)
                {
                    m_aItems[i]->m_rect = CRect(0, 0, nMaxWidth+rCl.Height()+4, rCl.bottom);
                    m_aItems[i]->m_rectText = CRect(rCl.Height()/2, 0, nMaxWidth+rCl.Height()/2+4, rCl.Height()-1);
                }
                else
                {
                    m_aItems[i]->m_rect = CRect(0, rCl.top, nMaxWidth+rCl.Height()+4, rCl.bottom);
                    m_aItems[i]->m_rectText = CRect(rCl.Height()/2, rCl.top, nMaxWidth+rCl.Height()/2+4, rCl.bottom-1);
                }
                m_aItems[i]->m_rect += CPoint(nOffset, 0);
                m_aItems[i]->m_rectText += CPoint(nOffset, 0);

                //horizontal overlapping with height/2
                //nOffset += m_aItems[i]->m_rect.Width()-rCl.Height()/2;
                nOffset += m_aItems[i]->m_rect.Width();
                nWidth = m_aItems[i]->m_rect.right;
            }//for items
        }//if fixed width
        else
        {
            //variable width, calculate width based on individual text widths
            for(i=0; i<m_aItems.GetSize(); i++)
            {
                w=0;
                h = pDC->DrawText(m_aItems[i]->m_sText, rcText, DT_CALCRECT);
                if(h>0)
                {
                    w = rcText.Width();
                }
                if(fTop)
                {
                    m_aItems[i]->m_rect = CRect(0, 0, w+rCl.Height()+4, rCl.bottom);
                    m_aItems[i]->m_rectText = CRect(rCl.Height()/2, 0, w+rCl.Height()/2+4, rCl.Height()-1);
                }
                else
                {
                    m_aItems[i]->m_rect = CRect(0, rCl.top, w+rCl.Height()+4, rCl.bottom);
                    m_aItems[i]->m_rectText = CRect(rCl.Height()/2, rCl.top, w+rCl.Height()/2+4, rCl.bottom-1);
                }

                m_aItems[i]->m_rect += CPoint(nOffset, 0);
                m_aItems[i]->m_rectText += CPoint(nOffset, 0);

                //horizontal overlapping with height/2
                //nOffset += m_aItems[i]->m_rect.Width()-rCl.Height()/2;
                nOffset += m_aItems[i]->m_rect.Width();

                nWidth = m_aItems[i]->m_rect.right;
            }//for items
        }//if not fixed width
        pDC->SelectObject(pOldFont);
        ReleaseDC(pDC);
    }

    return nWidth;
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::DrawGlyph(CDC& dc, CPoint& pt, int nImageNdx, int nColorNdx)
{
    CDC dcMem, dcMemMono;
    CBitmap bmpGlyphColor;

    dcMem.CreateCompatibleDC(&dc);
    dcMemMono.CreateCompatibleDC(&dc);

    CBitmap* pOldBmpGlyphMono = dcMemMono.SelectObject(&m_bmpGlyphsMono);

    bmpGlyphColor.CreateCompatibleBitmap(&dc, 8, 7);

    CBitmap* pOldBmpGlyphColor = dcMem.SelectObject(&bmpGlyphColor);

    COLORREF rgbOldTextGlyph =  dcMem.SetTextColor(m_rgbGlyph[nColorNdx]);

    dcMem.BitBlt(0, 0, 8, 7, &dcMemMono, nImageNdx*8, 0, SRCCOPY);
    dcMem.SetTextColor(rgbOldTextGlyph);

    COLORREF rgbOldBk = dc.SetBkColor(RGB(255, 255, 255));
    COLORREF rgbOldText = dc.SetTextColor(RGB(0, 0, 0));

    dc.BitBlt(pt.x, pt.y, 8, 7, &dcMem, 0, 0, SRCINVERT);
    dc.BitBlt(pt.x, pt.y, 8, 7, &dcMemMono, nImageNdx*8, 0, SRCAND);
    dc.BitBlt(pt.x, pt.y, 8, 7, &dcMem, 0, 0, SRCINVERT);

    dcMem.SelectObject(pOldBmpGlyphColor);
    dcMemMono.SelectObject(pOldBmpGlyphMono);
    dc.SetBkColor(rgbOldBk);
    dc.SetTextColor(rgbOldText);
}

//////////////////////////////////////////////////////////////////////////
BOOL CCustomTabCtrl::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
    int i;

    if(dwRemove & CTCS_MULTIHIGHLIGHT)
    {
        //remove highlights
        for(i=0; i<m_aItems.GetSize(); i++)
        {
            m_aItems[i]->m_fHighlighted = FALSE;
        }
    }
    if(dwAdd & CTCS_MULTIHIGHLIGHT)
    {
        //highlight all selected tabs
        for(i=0; i<m_aItems.GetSize(); i++)
        {
            if(i==m_nItemSelected)
            {
                m_aItems[i]->m_fHighlighted = TRUE;
            }
        }
    }

    //vertical styles removed
#ifdef _WIN32_WCE
    dwAdd &= ~CTCS_LEFT;
    dwAdd &= ~CTCS_RIGHT;
    CWnd::ModifyStyle(dwRemove | CTCS_LEFT | CTCS_RIGHT, dwAdd, nFlags);
#else
    CWnd::ModifyStyle(dwRemove, dwAdd, nFlags);
#endif
    RecalcLayout(RECALC_RESIZED, m_nItemSelected);
    Invalidate(FALSE);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
BOOL CCustomTabCtrl::ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
    CWnd::ModifyStyleEx(dwRemove, dwAdd, nFlags);
    RecalcLayout(RECALC_RESIZED, m_nItemSelected);
    Invalidate(FALSE);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::PreSubclassWindow() 
{
    CWnd::ModifyStyle(0, WS_CLIPCHILDREN);
    RecalcLayout(RECALC_RESIZED, m_nItemSelected);
    CWnd::PreSubclassWindow();
}

//////////////////////////////////////////////////////////////////////////
#ifndef _WIN32_WCE
void CCustomTabCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
    NotifyParent(CTCN_RCLICK, HitTest(point), point);
    CWnd::OnRButtonDown(nFlags, point);
}
#endif

//////////////////////////////////////////////////////////////////////////
#ifndef _WIN32_WCE
void CCustomTabCtrl::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
    NotifyParent(CTCN_RDBLCLK, HitTest(point), point);
    CWnd::OnRButtonDblClk(nFlags, point);
}
#endif

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::EditLabel(int nItem)
{
    return EditLabel(nItem, FALSE);
}

//////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::EditLabel(int nItem, BOOL fMouseSel)
{
    if(nItem<0 || nItem>=m_aItems.GetSize())
    {
        return CTCERR_INDEXOUTOFRANGE;
    }
    if(!(GetStyle()&CTCS_EDITLABELS))
    {
        return CTCERR_NOEDITLABELSTYLE;
    }
    if(nItem!=m_nItemSelected)
    {
        return CTCERR_ITEMNOTSELECTED;
    }
    if(m_ctrlEdit.m_hWnd)
    {
        return CTCERR_ALREADYINEDITMODE;
    }
    if(IsVertical())
    {
        return CTCERR_EDITNOTSUPPORTED;
    }

    CRect r;
    CDC* pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject(&m_FontSelected);
    int h = pDC->DrawText(m_aItems[nItem]->m_sText, r, DT_CALCRECT);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);
    r = m_aItems[nItem]->m_rectText;
    if(r.Height()>h)
    {
        r.top += (r.Height()-h)/2;
        r.bottom -= (r.Height()-h)/2;
    }
    r.left += 2;
    //create edit control
    if(m_ctrlEdit.Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, r, this, CTCID_EDITCTRL))
    {
        CString sOld = m_aItems[nItem]->m_sText;
        m_ctrlEdit.SetFont(&m_FontSelected, FALSE);
        m_ctrlEdit.SetLimitText(MAX_LABEL_LENGTH);
        m_ctrlEdit.SetWindowText(m_aItems[nItem]->m_sText);
        m_ctrlEdit.SetFocus();
        m_ctrlEdit.SetSel(0,-1);
        if(fMouseSel)
        {
            ReleaseCapture();
        }
        //process messages in edit mode
        for(;;) 
        {
            MSG msg;
            ::GetMessage(&msg, NULL, 0, 0);

            switch (msg.message) 
            {
            case WM_SYSKEYDOWN:
                {
                    if(msg.wParam == VK_F4 && msg.lParam&29)
                    {
                        break;
                    }
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    break;
                }
            case WM_KEYDOWN:
                {
                    if (msg.wParam == VK_ESCAPE)
                    {
                        m_aItems[nItem]->m_sText = sOld;
                        m_ctrlEdit.DestroyWindow();
                        RecalcLayout(RECALC_RESIZED,m_nItemSelected);
                        Invalidate(FALSE);
                        return CTCERR_NOERROR;
                    }
                    if(msg.wParam == VK_RETURN)
                    {
                        if(NotifyParent(CTCN_LABELUPDATE,nItem,CPoint(0,0)))
                            break;
                        m_ctrlEdit.GetWindowText(m_aItems[nItem]->m_sText);
                        m_ctrlEdit.DestroyWindow();
                        RecalcLayout(RECALC_RESIZED,nItem);
                        Invalidate(FALSE);
                        return CTCERR_NOERROR;
                    }
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    break;
                }
            case WM_LBUTTONDOWN:
                {
                    if(msg.hwnd == m_hWnd)
                    {
                        POINTS pt = MAKEPOINTS(msg.lParam);
                        if(HitTest(CPoint(pt.x, pt.y)) != m_nItemSelected)
                        {
                            if(NotifyParent(CTCN_LABELUPDATE, nItem, CPoint(0, 0)))
                            {
                                break;
                            }
                            m_ctrlEdit.GetWindowText(m_aItems[m_nItemSelected]->m_sText);
                            m_ctrlEdit.DestroyWindow();
                            TranslateMessage(&msg);
                            DispatchMessage(&msg);
                            return CTCERR_NOERROR;
                        }
                    }
                    else if(msg.hwnd == m_ctrlEdit.m_hWnd)
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                    else
                    {
                        if(NotifyParent(CTCN_LABELUPDATE, nItem, CPoint(0, 0)))
                        {
                            break;
                        }
                        m_ctrlEdit.GetWindowText(m_aItems[m_nItemSelected]->m_sText);
                        m_ctrlEdit.DestroyWindow();
                        return CTCERR_NOERROR;
                    }
                    break;
                }
            case WM_LBUTTONUP:
                {
                    if(msg.hwnd==m_ctrlEdit.m_hWnd)
                    {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }
                    break;
                }
            case WM_LBUTTONDBLCLK:
#ifndef _WIN32_WCE
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
#endif
                {
                    break;
                }				
            default:
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    break;
                }
            }//switch message
        }//for(ever)
    }//if edit control created
    return CTCERR_NOERROR;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CCustomTabContainer, CCustomTabCtrl)

CCustomTabContainer::CCustomTabContainer()
{
}

//////////////////////////////////////////////////////////////////////////
CCustomTabContainer::~CCustomTabContainer()
{
    int i;

    for(i=0; i<m_Dlgs.GetSize(); i++)
    {
        delete m_Dlgs[i];
    }
    m_Dlgs.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabContainer::FixStyle(HWND hWnd)
{
    LONG lStyle;

    lStyle=GetWindowLong(hWnd, GWL_STYLE);
    lStyle &= ~WS_CAPTION;
    lStyle &= ~DS_MODALFRAME;
    lStyle &= ~WS_POPUP;
    lStyle |= WS_CHILD;
    SetWindowLong(hWnd, GWL_STYLE, lStyle);
}

BEGIN_MESSAGE_MAP(CCustomTabContainer, CCustomTabCtrl)
    //{{AFX_MSG_MAP(MyTabCtrl)
    //ON_WM_SIZE()
    ON_NOTIFY_REFLECT(CTCN_SELCHANGE, OnSelchange)
    ON_NOTIFY_REFLECT(CTCN_CLICK, OnTabBtnClick)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MyTabCtrl message handlers

// void CCustomTabContainer::OnSize(UINT nType, int cx, int cy) 
// {
//     CCustomTabCtrl::OnSize(nType, cx, cy);
// 
//     int i;
//     for(i=0; i<m_Dlgs.GetSize(); i++)
//     {
//         m_Dlgs[i]->MoveWindow(0,0,cx,cy);
//     }
// }
//////////////////////////////////////////////////////////////////////////
void CCustomTabContainer::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
    Display();
    *pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabContainer::OnTabBtnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CTC_NMHDR *pCHdr;

    pCHdr=(CTC_NMHDR *)pNMHDR;
    if(pCHdr->nItem == CTCID_CLOSEBUTTON)
    {
        //close
        RemoveDialog(GetCurSel());		
    }
    Display();
    *pResult = 0;
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabContainer::AddDialog(int nIndex, CString strText, CDialog *pDlg)
{
    FixStyle(pDlg->GetSafeHwnd());

    m_Dlgs.InsertAt(nIndex, pDlg);
    if(strText != _T(""))
    {
        InsertItem(nIndex, strText);
    }
    else
    {
        InsertItem(nIndex, _T("<No title>"));
    }

    //nothing yet selected?
    if(GetCurSel() < 0)
    {
        //select once
        SetCurSel(nIndex);
        Display();
    }
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabContainer::RemoveDialog(int nIndex)
{
    if(nIndex >= 0 && nIndex < GetItemCount())
    {
        DeleteItem(nIndex);
        delete m_Dlgs[nIndex];
        m_Dlgs.RemoveAt(nIndex);
        Display();
    }
}

//////////////////////////////////////////////////////////////////////////
void CCustomTabContainer::Display()
{
    int i, nSel;
    CRect rcClient, rcWnd;

    GetClientRect(rcClient);
    GetWindowRect(rcWnd);

    AdjustRect(FALSE, rcClient);

    //no dialogs? then wipe background
    if(m_Dlgs.GetCount() == 0)
    {
        CDC *pDC=GetDC();
        if(pDC != NULL)
        {
            pDC->FillSolidRect(rcClient, m_ticColors.crWndBkg);		
            ReleaseDC(pDC);
        }
    }

    nSel=GetCurSel();
    if(nSel < 0 || nSel >= GetItemCount())
    {
        return;
    }

    if(m_Dlgs[nSel]->m_hWnd)
    {
        m_Dlgs[nSel]->ShowWindow(SW_HIDE);
    }

    GetParent()->ScreenToClient(rcWnd);

    rcClient.OffsetRect(rcWnd.left, rcWnd.top);

    for(i=0; i < m_Dlgs.GetCount(); i++)
    {
        m_Dlgs[i]->SetWindowPos(&wndTop, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_HIDEWINDOW);
    }
    m_Dlgs[nSel]->SetWindowPos(&wndTop, rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), SWP_SHOWWINDOW);
    m_Dlgs[nSel]->ShowWindow(SW_SHOW);

    if(GetStyle() & CTCS_NOLINES)
    {
        return;
    }

    //once displayed, draw a line around dialog display area
    /*
    GetClientRect(rcClient);
    AdjustRect(FALSE, rcClient);

    CDC *pDC=GetDC();
    if(pDC != NULL)
    {
    CPen pen(PS_SOLID, 1, m_ticColors.crDarkLine);
    HGDIOBJ hOldPen;

    hOldPen=pDC->SelectObject(pen);
    if(GetStyle() & CTCS_TOP)
    {
    pDC->MoveTo(0, rcClient.top);
    pDC->LineTo(0, rcClient.bottom);
    pDC->LineTo(rcClient.right-1, rcClient.bottom);
    pDC->LineTo(rcClient.right-1, rcClient.top);
    }
    else
    {
    pDC->MoveTo(0, rcClient.bottom);
    pDC->LineTo(0, rcClient.top);
    pDC->LineTo(rcClient.right-1, rcClient.top);
    pDC->LineTo(rcClient.right-1, rcClient.bottom);
    }
    pDC->SelectObject(hOldPen);
    ReleaseDC(pDC);
    }
    */
}
