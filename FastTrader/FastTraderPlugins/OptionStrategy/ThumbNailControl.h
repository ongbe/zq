#if !defined(AFX_THUMBNAILCONTROL_H__EA2BDCEA_556B_46D0_9C14_B68E4BC7F4A1__INCLUDED_)
#define AFX_THUMBNAILCONTROL_H__EA2BDCEA_556B_46D0_9C14_B68E4BC7F4A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThumbNailControl.h : header file
//

#include "ThumbnailButton.h"
#include "bkcolor.h"


#define ON_STRATEGY_BTN_LCLICKED _T( "WM_ON_STRATEGY_BTN_LCLICKED_{D190C85B-217C-4a91-8684-0C523559C67D}" )
static const UINT UWM_ON_STRATEGY_BTN_LCLICKED = ::RegisterWindowMessage( ON_STRATEGY_BTN_LCLICKED );

/////////////////////////////////////////////////////////////////////////////
// CThumbNailControl window

class CThumbNailControl : public CWnd
{
    //****************
    //*  Attributes  *
    //****************
private:
    static CBrush     m_bkBrush;

    CPtrArray         m_arPtrData;

    long              m_nStartX;
    long              m_nStartY;

    long              m_nCol;
    long              m_nRow;

    long              m_nThumbWidth;
    long              m_nThumbHeight;

    //*********************************
    //*  Construction / Destruction 	*
    //*********************************
public:
    CThumbNailControl();
    virtual ~CThumbNailControl();

    //****************
    //*  Operations  *
    //****************
private:
    void RecalScrollBars( void );
    BOOL RegisterWindowClass();
    void RecalButtonPos( void );
    int UpdateSelected(CThumbnailButton *pBtnSelected);

protected:
    int GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE );
    BOOL SetScrollPos32( int nBar, int nPos, BOOL bRedraw = TRUE );

public:
    void Add( const CString& sPath );
    void InitializeVariables( int cX, int cY);

    //***************
    //*  Overrides	*
    //***************
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CThumbNailControl)
    //}}AFX_VIRTUAL

    //*************************************
    //*  Generated message map functions	*
    //*************************************
protected:
    //{{AFX_MSG(CThumbNailControl)
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);  
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnTnbLClicked( WPARAM wParam, LPARAM lParam );
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BK_COLOR_MODE m_ColorMode;
    void LoadThumbNail(CString strDir, int nDepth);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THUMBNAILCONTROL_H__EA2BDCEA_556B_46D0_9C14_B68E4BC7F4A1__INCLUDED_)
