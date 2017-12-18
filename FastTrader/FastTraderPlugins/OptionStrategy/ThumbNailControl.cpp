#include "stdafx.h"
#include "OptionStrategy.h"
#include <math.h>
#include "ThumbNailControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RGBTHUMBCTRLBKGD_LIGHT RGB(156,187,212)
#define RGBTHUMBCTRLBKGD_THICK RGB(239,239,239)
#define RGBTHUMBCTRLBORDER RGB( 0, 0, 0 )

#define CTHUMBNAILCONTROL_CLASSNAME _T("CThumbNailControl")

/////////////////////////////////////////////////////////////////////////////
// CThumbNailControl

CBrush  CThumbNailControl::m_bkBrush;
CThumbNailControl::CThumbNailControl():m_ColorMode(Color_Light)
{
    if( !RegisterWindowClass() )
        return;

    m_arPtrData.RemoveAll();

    m_nCol = 0;
    m_nRow = 1;

    m_nStartX = 0;
    m_nStartY = 0;

    m_nThumbWidth  = 0;
    m_nThumbHeight = 0;
}

CThumbNailControl::~CThumbNailControl()
{

    for( int i=m_arPtrData.GetSize()-1; i>=0; i-- )
    {
        CThumbnailButton *pBtn = (CThumbnailButton*) m_arPtrData.GetAt(i);
        m_arPtrData.RemoveAt(i);

        if( pBtn != NULL )
            delete pBtn;
    }

    m_arPtrData.RemoveAll();
}


BEGIN_MESSAGE_MAP(CThumbNailControl, CWnd)
    //{{AFX_MSG_MAP(CThumbNailControl)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL() 
    ON_WM_SIZE()
    ON_REGISTERED_MESSAGE( UWM_ON_TNB_LCLICKED, OnTnbLClicked )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThumbNailControl message handlers

//---------------------------------------------------------------------------
//*************************
//*  InitializeVariables	*
//*************************
// 
// INPUTS       : none
// 
// RETURNS      : none
// 
void CThumbNailControl::InitializeVariables( int cX, int cY)
{
    m_arPtrData.RemoveAll();

    m_nThumbWidth     = cX;
    m_nThumbHeight    = cY;


    CString m_csSystemPath;
    char szLocalPath[MAX_PATH];
    memset(szLocalPath, 0, sizeof(szLocalPath));
    ::GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath) );
    CString csSystemPath( szLocalPath );
    int nPos = csSystemPath.ReverseFind( '\\' );
    if ( -1 != nPos )
    {
        m_csSystemPath = csSystemPath.Left( nPos );

        m_csSystemPath += _T("\\img");
    }


    LoadThumbNail(m_csSystemPath,0);
}

//---------------------------------------------------------------------------
//********************
//*  RecalButtonPos  *
//********************
// 
// DESCRIPTION  : Calculate button positions
// 
// INPUTS       : none
// 
// RETURNS      : none
// 
void CThumbNailControl::RecalButtonPos()
{
    CRect rect;
    GetClientRect( &rect );

    int nH = rect.Height();
    ASSERT(m_nThumbHeight<=nH);

    int nSeperator = (nH - m_nThumbHeight)/2;

    int nX=0;
    int nY=0;
    m_nRow=1;

    for( int i=0; i<m_arPtrData.GetSize(); i++ )
    {
        CThumbnailButton *pBtn = (CThumbnailButton*) m_arPtrData.GetAt(i);

        ASSERT( AfxIsValidAddress(pBtn, sizeof(CThumbnailButton), TRUE) );
        ASSERT( ::IsWindow( pBtn->m_hWnd ) );

        pBtn->SetWindowPos( NULL, 
                            m_nStartX + nSeperator + nX, 
                            m_nStartY + nSeperator+nY, 
                            0, 0, SWP_NOSIZE|SWP_NOZORDER );

        nX += m_nThumbWidth + nSeperator;
    }

}

//---------------------------------------------------------------------------
//*************************
//*  RegisterWindowClass	*
//*************************
// 
// DESCRIPTION  : Register the "CThumNailControl" class name into window.
// 
// INPUTS       : none
// 
// RETURNS      : BOOL - TRUE if success, otherwise FALSE
// 
BOOL CThumbNailControl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, CTHUMBNAILCONTROL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        m_bkBrush.CreateSolidBrush( (m_ColorMode==Color_Light)?RGBTHUMBCTRLBKGD_LIGHT:RGBTHUMBCTRLBKGD_THICK );

        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) m_bkBrush.GetSafeHandle();
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = CTHUMBNAILCONTROL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}


//---------------------------------------------------------------------------
//*********
//*  Add	*
//*********
// 
// SYNOPSIS     : 
// 
// DESCRIPTION  : [TODO]
// 
// INPUTS       : const CString& sPath - 
// 
// RETURNS      : [TODO]
// 
// NOTE         : [TODO]
// 
// TODO         : [TODO]
// 
void CThumbNailControl::Add( const CString& sPath )
{
    ASSERT( ::IsWindow(this->m_hWnd) );

    // Initialize Variables Not done!
    ASSERT( (m_nThumbWidth + m_nThumbHeight) > 0 );

    Invalidate( FALSE );

    //bmp size
    long nThumbImgWidth  = m_nThumbWidth;//(int)(0.8f*m_nThumbWidth);
    long nThumbImgHeight = m_nThumbHeight;//(int)(0.8f*m_nThumbHeight);

    CString strategyName = sPath.Mid(sPath.ReverseFind('/')+1, sPath.ReverseFind('.') - sPath.ReverseFind('/')-1);

    CThumbnailButton *pBtn = new CThumbnailButton(strategyName);
    pBtn->Create(   strategyName, 
                    WS_CHILD|WS_VISIBLE, 
                    CRect(0,0,m_nThumbWidth,m_nThumbHeight), this, 0 );


    pBtn->LoadBitmaps(sPath,2,0,0,0,0);

    if(pBtn)
    {
        VERIFY( ::IsWindow(pBtn->m_hWnd) );
        m_arPtrData.Add( (void*) pBtn );
    }
    else
    {
        pBtn->DestroyWindow();
        delete pBtn;
    }
}

//---------------------------------------------------------------------------
//***************
//*  OnHScroll	*
//***************
// 
// DESCRIPTION  : Perform Horizontal Scrolling
// 
// INPUTS       : UINT nSBCode - SB_* message
//		            UINT - Not used
//		            CScrollBar* - Not used
// 
// RETURNS      : none
// 
void CThumbNailControl::OnHScroll(UINT nSBCode, UINT /* nPos */ , CScrollBar* /* pScrollBar */) 
{
    int nScrollPos = GetScrollPos32(SB_HORZ);

    switch( nSBCode )
    {
    case SB_LEFT:
        break;

    case SB_ENDSCROLL:
        RedrawWindow();
        break;

    case SB_LINELEFT :
        SetScrollPos32(SB_HORZ, nScrollPos - 1 );
        break;

    case SB_LINERIGHT:
        SetScrollPos32(SB_HORZ, nScrollPos + 1);
        break;

    case SB_PAGELEFT :
        SetScrollPos32(SB_HORZ, nScrollPos - 20 );
        break;

    case SB_PAGERIGHT:
        SetScrollPos32(SB_HORZ, nScrollPos + 20);
        break;

    case SB_RIGHT:
        break;

    case SB_THUMBPOSITION:  // Go down...
    case SB_THUMBTRACK:
        SetScrollPos32( SB_HORZ, GetScrollPos32(SB_HORZ, TRUE) );
        break;

    default:
        break;
    }

    m_nStartX = -GetScrollPos32(SB_HORZ);
    RecalButtonPos();
}

//---------------------------------------------------------------------------
//*********************
//*  RecalScrollBars	*
//*********************
// 
// DESCRIPTION  : Calculate scroll bars positions
// 
// INPUTS       : none
// 
// RETURNS      : none
// 
void CThumbNailControl::RecalScrollBars()
{
    CRect rect;
    GetClientRect( &rect );
    ClientToScreen( &rect );

    //ASSERT(m_nThumbHeight<=rect.Height());
    int nSeperator = (rect.Height() - m_nThumbHeight)/2;

    m_nCol = (int)m_arPtrData.GetSize();

    long nTotalWidth  = m_nThumbWidth * m_nCol + nSeperator * ( 1 + m_nCol );
    long nTotalHeight = m_nThumbHeight * m_nRow + nSeperator * ( 1 + m_nRow );

    long nWidDiff = nTotalWidth  - rect.Width();
    long nHeiDiff = nTotalHeight - rect.Height();

    if( nWidDiff > nSeperator && m_arPtrData.GetSize() >= m_nCol )
    {
        SCROLLINFO si;
        memset( &si, 0, sizeof(SCROLLINFO) );

        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_PAGE | SIF_RANGE;
        si.nPage = (int)(0.25*(nWidDiff+14));
        si.nMin = 0;
        si.nMax = (int)(1.25*(nWidDiff+14));

        SetScrollInfo( SB_HORZ, &si, TRUE );

        EnableScrollBarCtrl( SB_HORZ );
        EnableScrollBar( SB_HORZ );
    }

    if( nHeiDiff > nSeperator )
    {
        SCROLLINFO si;
        memset( &si, 0, sizeof(SCROLLINFO) );

        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_PAGE | SIF_RANGE;
        si.nPage = (int)(0.25*(nHeiDiff+14));
        si.nMin = 0;
        si.nMax = (int)(1.25*(nHeiDiff+14));

        SetScrollInfo( SB_VERT, &si, TRUE );

        EnableScrollBarCtrl( SB_VERT );
        EnableScrollBar( SB_VERT );
    }

}


//---------------------------------------------------------------------------
//********************
//*  SetScrollPos32  *
//********************
// 
// DESCRIPTION  : Set scroll bar position
// 
// INPUTS       : int nBar - SB_VERT / SB_HORZ
//		            int nPos - new position
//		            BOOL bRedraw - redraw flag
// 
// RETURNS      : [TODO]
// 
// NOTE         : [TODO]
// 
// TODO         : [TODO]
// 
BOOL CThumbNailControl::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

//---------------------------------------------------------------------------
//********************
//*  GetScrollPos32  *
//********************
// 
// DESCRIPTION  : Get scroll bar position
// 
// INPUTS       : int nBar - SB_VERT / SB_HORZ
//		            BOOL bGetTrackPos - 
// 
// RETURNS      : int - position at scroll bar
// 
int CThumbNailControl::GetScrollPos32( int nBar, BOOL bGetTrackPos )
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else
    {
        if (GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}

//---------------------------------------------------------------------------
//***************
//*  OnVScroll	*
//***************
// 
// DESCRIPTION  : Perform Vertical Scrolling
// 
// INPUTS       : UINT nSBCode - SB_* message
//		            UINT - Not used
//		            CScrollBar* - Not used
// 
// RETURNS      : none
// 
void CThumbNailControl::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar */) 
{
    int nScrollPos = GetScrollPos32(SB_VERT);
    switch( nSBCode )
    {
    case SB_BOTTOM:
        break;

    case SB_ENDSCROLL:
        RedrawWindow();
        break;

    case SB_LINEDOWN:
        SetScrollPos32(SB_VERT, nScrollPos + 1 );
        break;

    case SB_LINEUP:
        SetScrollPos32(SB_VERT, nScrollPos - 1 );
        break;

    case SB_PAGEDOWN:
        SetScrollPos32(SB_VERT, nScrollPos + 20 );
        break;

    case SB_PAGEUP:
        SetScrollPos32(SB_VERT, nScrollPos - 20 );
        break;

    case SB_THUMBPOSITION: // Go down..
    case SB_THUMBTRACK:
        SetScrollPos32( SB_VERT, GetScrollPos32(SB_VERT, TRUE) );
        break;

    case SB_TOP:
        break;

    default:
        break;
    }

    m_nStartY = -GetScrollPos32(SB_VERT);
    RecalButtonPos();
}

//---------------------------------------------------------------------------
//******************
//*  OnMouseWheel  *
//******************
// 
// DESCRIPTION  : Extra support on Mouse Wheel scrolling
// 
// INPUTS       : UINT nFlags -   passed to parent OnMouseWheel function
//		            short zDelta -  degree of rotation
//		            CPoint pt -     passed to parent OnMouseWheel function
// 
// RETURNS      : BOOL - returned from parent OnMouseWheel function
// 
BOOL CThumbNailControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    int nScrollPos = GetScrollPos32(SB_VERT);
    int nStep = zDelta/WHEEL_DELTA;

    SetScrollPos32(SB_VERT, nScrollPos - nStep*5 );

    m_nStartY = -GetScrollPos32(SB_VERT);
    RecalButtonPos();

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

int CThumbNailControl::UpdateSelected(CThumbnailButton *pBtnSelected)
{
    for( int i=m_arPtrData.GetSize()-1; i>=0; i-- )
    {
        CThumbnailButton *pBtn = (CThumbnailButton*) m_arPtrData.GetAt(i);

        //if(pBtn->GetClickSelected())
        //{
            pBtn->SetClickSelected(FALSE);
        //}            
        //else 
        //{
        //    if(pBtn == pBtnSelected)
        //        pBtn->SetClickSelected(TRUE);
        //}
    }

    pBtnSelected->SetClickSelected(TRUE);
    return 0;
}
LRESULT CThumbNailControl::OnTnbLClicked( WPARAM wParam, LPARAM lParam )
{
    CThumbnailButton *pBtn = (CThumbnailButton*) wParam;

    UpdateSelected(pBtn);
    Invalidate();

    if( ::IsWindow(pBtn->m_hWnd) )
    {
        int nID = FormatStrategyID((LPSTR)(LPCTSTR)pBtn->m_StrategyName);
        GetParent()->SendMessage(UWM_ON_STRATEGY_BTN_LCLICKED,(WPARAM)nID, (LPARAM)0);
     
        return 0;
    }

    return 1;
}

void CThumbNailControl::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    
    // TODO: Add your message handler code here
    RecalButtonPos();
    //RecalScrollBars();
}

void CThumbNailControl::LoadThumbNail(CString strDir, int nDepth)
{
    CFileFind filefind;                                     //声明CFileFind类型变量
    CString strWildpath = strDir + _T("//*.bmp");           //所有文件都列出。
    if(filefind.FindFile(strWildpath, 0))                   //开始检索文件
    {
        BOOL bRet = TRUE;
        while(bRet)
        {
            bRet = filefind.FindNextFile();                 //枚举一个文件
            if(filefind.IsDots())                                 //如果是. 或 .. 做下一个
                continue;

            for (int i = 0; i < nDepth; i ++)                 //层次空格打印
            {
                //TRACE(_T("    "));
            }

            if(!filefind.IsDirectory())                          //不是子目录，把文件名打印出来
            {
                CString strTextOut = strDir + CString(_T("//")) + filefind.GetFileName();
                //TRACE(_T("file = %s\n"), strTextOut);
                Add(strTextOut);
            }
            else                                                    //如果是子目录，递归调用该函数
            {
                CString strTextOut = strDir + CString(_T("//")) + filefind.GetFileName();
                //TRACE(_T("dir = %s\n"), strTextOut);
                LoadThumbNail(strTextOut, nDepth + 1);//递归调用该函数打印子目录里的文件
            }
        }

        filefind.Close();
    }
}