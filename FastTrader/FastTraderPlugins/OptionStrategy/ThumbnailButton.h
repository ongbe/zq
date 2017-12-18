#if !defined(AFX_ThumbnailButton_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_)
#define AFX_ThumbnailButton_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommPublic.h"
//////////////////////////////////////////////////
// CThumbnailButton - Window Blinds like button class
//
// This class implements a Window Blinds like button

#define ON_TNB_LCLICKED _T( "WM_ON_TNB_LCLICKED_{D190C85B-217C-4a91-8684-0C523559C67D}" )
static const UINT UWM_ON_TNB_LCLICKED = ::RegisterWindowMessage( ON_TNB_LCLICKED );

class CThumbnailButton : public CButton
{

public:
	CThumbnailButton(CString strategyName);

// Methods
public:
    // Bitmap Loding and Initialize
    bool LoadBitmaps
    ( 
         CString ResourceName,// ResourceName
         int count,        // Num Of Pics
         int TopHeight,    // Top Merigin for Resizing
         int BottomHeight, // Bottom Merigin for Resizing
         int LeftWidth,    // Left Merigin for Resizing
         int RightWidth    // Right Merigin for Resizing
    );

protected:

    enum mode
    {
        normal  = 0,
        select  = 1,
        focus   = 2,
        disable = 3
    };

    enum state
    {
        notInited    = 0,
        FileLoaded   = 1,
        BitmapInited = 2
    };

    int m_State;

    // Bitmaps
    bool InitBitmap( CBitmap & src, CBitmap & dist, int index, int count);
    void DrawBitmap( CDC * pDC, int mode );

    UINT m_RcId;       // Resource ID
    int  m_NumofPics;  

    CBitmap NormalBitmap;
    CBitmap SelectBitmap;
    CBitmap FocusBitmap;
    CBitmap DisableBitmap;

    int m_TopHeight; //= 8;
    int m_BottomHeight; //= 8;
    int m_LeftWidth; //= 8;
    int m_RightWidth;// = 17;

    int m_dwWidth;
    int m_dwHeight;

private:
    // whether to be selected
    bool m_bClickSelected;
public:
    bool SetClickSelected(bool bSelected);
    bool GetClickSelected(void);

// Transpararent BackColor
protected:
    COLORREF m_BkColor;
public:
    void SetBackColor( COLORREF color ) { m_BkColor = color; }
    COLORREF GetBackColor() { return m_BkColor; }

	//{{AFX_VIRTUAL(CThumbnailButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

public:
	virtual ~CThumbnailButton();

	
protected:
	//{{AFX_MSG(CThumbnailButton)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
    virtual void PreSubclassWindow();
    //virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

    //CFont myFont; 
    CString m_StrategyName;
    CToolTipCtrl m_toolTip;
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _MEMDC_H_
#define _MEMDC_H_

//////////////////////////////////////////////////
// CBtnMemDC - memory DC
//
// This class implements a memory Device Context

class CBtnMemDC : public CDC {
private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
public:
	CBtnMemDC(CDC* pDC, const CRect& rcBounds) : CDC()
	{
		CreateCompatibleDC(pDC);
		m_bitmap = new CBitmap;
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap = SelectObject(m_bitmap);
		m_pDC = pDC;
		m_rcBounds = rcBounds;
        //For some reason the background color is not correct,
        //so we use the button face color.
        DWORD	color = ::GetSysColor( COLOR_BTNFACE );
        CBrush bkg(color);
        FillRect(rcBounds, &bkg);

	}
	~CBtnMemDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
					this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
		SelectObject(m_oldBitmap);
		if (m_bitmap != NULL) delete m_bitmap;
	}
	CBtnMemDC* operator->() {
		return this;
	}
};

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ÅB

#endif // !defined(AFX_ThumbnailButton_H__518122CF_358F_11D4_8F4F_00402656D980__INCLUDED_)
