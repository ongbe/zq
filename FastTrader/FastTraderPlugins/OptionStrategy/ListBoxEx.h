#if !defined(AFX_LISTBOXEX_H__E503C832_836A_11D5_AD87_8686F2D82715__INCLUDED_)
#define AFX_LISTBOXEX_H__E503C832_836A_11D5_AD87_8686F2D82715__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ListBoxEx.h : header file
//
//the bottom and top scrolls
#define SCROLL_HIGHT    13 

#define NO_BMP_ITEM 0
#define BLANK_BMP 1

#define SC_UP 2 //Up scroll
#define SC_DOWN 3 //Down Scroll

#define SC_NORMAL NULL //Normal scroll
#define SC_PRESSED DFCS_PUSHED //The scroll is pressed
#define SC_DISABLED DFCS_INACTIVE //The scroll is disabled
/////////////////////////////////////////////////////////////////////////////
// CListBoxEx window

class CListBoxEx : public CListBox
{
// Construction
public:
	CListBoxEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxEx)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBMPSize(int Height, int Width);
	void SetTextColor(COLORREF crTextColor, COLORREF crHighlight);
	void InsertItem(int nIndex, UINT nID, LPCTSTR lpszText);
	void AddItem(UINT IconID, LPCTSTR lpszText);
	void SetItemHeight (int newHeight);
	virtual ~CListBoxEx();
	void SetBkColor( COLORREF crBkColor, COLORREF crSelectedColor=GetSysColor(COLOR_HIGHLIGHT));
	// Generated message map functions
protected:
	void DrawScrolls(UINT WhichOne,UINT State);
	void DrawBorders();
	BOOL m_bOver;
	
	short m_ItemHeight; //Height of each item
	COLORREF m_crTextHlt; //Color of the text when highlighted
	COLORREF m_crTextClr; //Color of the text
	COLORREF m_HBkColor; //Color of the highlighted item background
    COLORREF m_BkColor;  //Color of the item background
	int m_BmpWidth; //Width of the bitmap
	int m_BmpHeight; //Height of the bitmap

	CBrush m_BkBrush;
	//{{AFX_MSG(CListBoxEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXEX_H__E503C832_836A_11D5_AD87_8686F2D82715__INCLUDED_)
