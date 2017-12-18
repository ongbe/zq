// ColorText_Static.cpp : implementation file
//

#include "stdafx.h"
#include "ColorTextStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorTextStatic

CColorTextStatic::CColorTextStatic()
{
    b_FillBackColor = 0;
    m_Font = 0;
    m_DisBackColro = RGB(200, 200, 200);
    Set_BackColor(GetSysColor(COLOR_3DFACE));
    Set_TextColor(0);
}

CColorTextStatic::~CColorTextStatic()
{
    if(m_Font)
    {
        m_Font->DeleteObject();
        delete m_Font;
    }   

    m_BackBrush.DeleteObject();
    m_DisBackBrush.DeleteObject();
}

void CColorTextStatic::SetFont(LOGFONT *LogFont, BOOL bRedraw)
{
    if(m_Font)
        m_Font->DeleteObject();

    if(m_Font == NULL)
        m_Font = new CFont();

    if(m_Font)
    {
        if(!m_Font->CreatePointFontIndirect( LogFont ))
        {
            delete m_Font;
            m_Font = NULL;
        }
    }
}

void CColorTextStatic::SetFont(CFont* pFont, BOOL bRedraw)
{
    LOGFONT LogFont;
    pFont->GetLogFont(&LogFont);
    SetFont(&LogFont, bRedraw);
}

void CColorTextStatic::SetFont(int nHeight, LPCTSTR fontName, BOOL bRedraw)
{
    if(m_Font)
        m_Font->DeleteObject();

    if(m_Font == NULL)
        m_Font = new CFont();

    if(m_Font)
    {
        if(!m_Font->CreatePointFont(nHeight, fontName))
        {
            delete m_Font; 
            m_Font = NULL;
        }
    }
    if(bRedraw && GetSafeHwnd())
        Invalidate();
}

void CColorTextStatic::Set_BackColor(COLORREF cr) 
{ 
    m_BackColor = cr; 
    b_FillBackColor = true; 

    m_BackBrush.DeleteObject();
    m_BackBrush.CreateSolidBrush(m_BackColor);

    m_DisBackBrush.DeleteObject();
    m_DisBackBrush.CreateSolidBrush(m_DisBackColro);

    if(GetSafeHwnd())
        Invalidate(); 
}

BEGIN_MESSAGE_MAP(CColorTextStatic, CStatic)
    //{{AFX_MSG_MAP(CColorTextStatic)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorTextStatic message handlers

HBRUSH CColorTextStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    // TODO: Change any attributes of the DC here
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(m_TextColor);

    if(m_Font)
        pDC->SelectObject(m_Font);

    HBRUSH hBrush = (HBRUSH) m_BackBrush;
    if(!IsWindowEnabled())
        hBrush = (HBRUSH) m_DisBackBrush;
    return ( hBrush );	
    // TODO: Return a non-NULL brush if the parent's handler should not be called 	return NULL;
}

BOOL CColorTextStatic::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    return TRUE;
    return CStatic::OnEraseBkgnd(pDC);
}
