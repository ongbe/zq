//////////////////////////////////////////////////////////////////////////
//
// CCustomSlider - Win32/WinCE/PocketPC compatible Tab control
// Based on Andrzej Markowski's CCustomTabControl
// http://www.codeproject.com/tabctrl/AMCustomTabCtrlDemo.asp
//
// This version was slapped together by dzolee
// http://dzolee.blogspot.com
//
//////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif

#include <afxwin.h>
#include <afxtempl.h>

#ifndef MAKEPOINTS
#define MAKEPOINTS(l) (*((POINTS FAR *)&(l)))
#endif

#define _WIN32_WCE

//tab states - not visible, before the first visible, or visible
#define TAB_INVISIBLE		0
#define TAB_BEFORE_FIRST_VISIBLE		1
#define TAB_VISIBLE		3

//reason of recalculation
#define RECALC_PREV_PRESSED			0
#define RECALC_NEXT_PRESSED			1
#define RECALC_ITEM_SELECTED		2
#define RECALC_RESIZED				3
#define RECALC_FIRST_PRESSED		4
#define RECALC_LAST_PRESSED			5
#define RECALC_EDIT_RESIZED			6
#define RECALC_CLOSE_PRESSED		7

#define MAX_LABEL_LENGTH				30

struct CTC_NMHDR
{
    NMHDR hdr;
    int	nItem;
    TCHAR pszText[MAX_LABEL_LENGTH];
    LPARAM lParam;
    RECT rItem;
    POINT ptHitTest;
    BOOL fSelected;
    BOOL fHighlighted;
};

//tab item colors
struct TabItemColors
{
	COLORREF crWndBkg;			//window background color
	COLORREF crBkgInactive;		//background color for inactive tab item
    COLORREF crBkgActive;		// .. active tab item
    COLORREF crTxtInactive;		//text color for active tab item
    COLORREF crTxtActive;		// .. active tab item
    COLORREF crDarkLine;		//darker line
    COLORREF crLightLine;		//lighter line
};

//////////////////////////////////////////////////////////////////////////
struct CCustomTabItem
{
    CCustomTabItem(CString sText, LPARAM lParam);
    void Draw(CDC& dc, CFont& font, TabItemColors *pColors, BOOL fOnTop, BOOL fNoLines);
    BOOL HitTest(CPoint pt)
    {
        return (m_bShape && m_rect.PtInRect(pt)) ? TRUE : FALSE;
    }
    void operator=(const CCustomTabItem &other);

    CString	m_sText;
    LPARAM m_lParam;
    CRect m_rect;
    CRect m_rectText;
    BYTE m_bShape;
    BOOL m_fSelected;
    BOOL m_fHighlighted;
    BOOL m_fHighlightChanged;
};

// CCustomTabCtrl

// styles
#define CTCS_FIXEDWIDTH			1		// Makes all tabs the same width.
#define CTCS_FOURBUTTONS		2		// Four buttons (First, Prev, Next, Last)
#define CTCS_AUTOHIDEBUTTONS	4		// Auto hide buttons
#define CTCS_MULTIHIGHLIGHT		16		// Multi highlighted items
#define CTCS_EDITLABELS			32		// Allows item text to be edited in place
#define CTCS_NOLINES			64		// No lines drawn around tabs
#define CTCS_CONTAINER			128		// Is not only a bar of buttons, but also a container for dialogs, like a regular CTabCtrl
#define CTCS_CLOSEBUTTON		256     // Close button
#define CTCS_BUTTONSAFTER		512		// Button after items
#define CTCS_TOP				1024    // Location on top	0x400
#define CTCS_RIGHT				2048    // Location on right 0x800
#define CTCS_LEFT				4096	// Location on left 0xc00

// hit test results
#define CTCHT_ONFIRSTBUTTON		-1
#define CTCHT_ONPREVBUTTON		-2
#define CTCHT_ONNEXTBUTTON		-3
#define CTCHT_ONLASTBUTTON		-4
#define CTCHT_ONCLOSEBUTTON		-5
#define CTCHT_NOWHERE			-6

// notification messages
#define CTCN_CLICK				NM_CLICK
#define CTCN_RCLICK				NM_RCLICK
#define CTCN_DBLCLK				NM_DBLCLK
#define CTCN_RDBLCLK			NM_RDBLCLK
#define CTCN_OUTOFMEMORY		NM_OUTOFMEMORY

#define CTCN_SELCHANGE			NM_FIRST
#define CTCN_HIGHLIGHTCHANGE	NM_FIRST + 1
#define CTCN_ITEMMOVE			NM_FIRST + 2
#define CTCN_ITEMCOPY			NM_FIRST + 3
#define CTCN_LABELUPDATE		NM_FIRST + 4

#define CTCID_FIRSTBUTTON		-1
#define CTCID_PREVBUTTON		-2
#define CTCID_NEXTBUTTON		-3
#define CTCID_LASTBUTTON		-4
#define CTCID_CLOSEBUTTON		-5
#define CTCID_NOBUTTON			-6

//id of label edit control
#define CTCID_EDITCTRL			1

//button click repeat
#define REPEAT_TIMEOUT			500

#define CTC_TIMER_ID			1
#define CTC_TIMER_TIMEOUT		100

// error codes
#define CTCERR_NOERROR					0
#define CTCERR_OUTOFMEMORY				-1
#define CTCERR_INDEXOUTOFRANGE			-2
#define CTCERR_NOEDITLABELSTYLE			-3
#define CTCERR_NOMULTIHIGHLIGHTSTYLE	-4
#define CTCERR_ITEMNOTSELECTED			-5
#define CTCERR_ALREADYINEDITMODE		-6
#define CTCERR_TEXTTOOLONG				-7
#define CTCERR_EDITNOTSUPPORTED			-8

// button states
#define BNST_INVISIBLE			0
#define BNST_NORMAL				1
#define BNST_HOT				2
#define BNST_PRESSED			3

#define CustomTabCtrl_CLASSNAME    _T("CCustomTabCtrl")  // Window class name

class CCustomTabCtrl : public CWnd
{
	DECLARE_DYNAMIC(CCustomTabCtrl)
public:

    // Construction

    CCustomTabCtrl();
    virtual ~CCustomTabCtrl();
    BOOL Create(UINT dwStyle, const CRect & rect, CWnd * pParentWnd, UINT nID);

    // Attributes

    int	GetItemCount()
    {
        return (int)m_aItems.GetSize();
    }
    int	GetCurSel()
    {
        return m_nItemSelected;
    }
    int	SetCurSel(int nItem);
    int	IsItemHighlighted(int nItem);
    int	HighlightItem(int nItem, BOOL fHighlight);
    int	GetItemData(int nItem, DWORD& dwData);
    int	SetItemData(int nItem, DWORD dwData);
    int	GetItemText(int nItem, CString& sText);
    int	SetItemText(int nItem, CString sText);
    int	GetItemRect(int nItem, CRect& rect) const;
    BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags=0);
    BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags=0);
    void SetControlFont(const LOGFONT& lf, BOOL fRedraw=FALSE);
    BOOL IsVertical()
    {
        return GetStyle()&CTCS_LEFT || GetStyle()&CTCS_RIGHT;
    }
	BOOL IsContainer()
	{
		return (GetStyle() & CTCS_CONTAINER);
	}
    // Operations

    int InsertItem(int nItem, CString sText, LPARAM lParam=0);
    int	DeleteItem(int nItem);
    void DeleteAllItems();
    int	MoveItem(int nItemSrc, int nItemDst);
    int	CopyItem(int nItemSrc, int nItemDst);
    int	HitTest(CPoint pt);
    int	EditLabel(int nItem);
	
	void SetColors(TabItemColors *pColorsIn, BOOL fRedraw=FALSE);
	void GetColors(TabItemColors *pColorsOut);
	int GetTabsHeight();
	void SetTabsHeight(int nHeight);
	void GetTabsRect(LPRECT pRect);

	void AdjustRect(BOOL bLarger, LPRECT lpRect);

    virtual void Display(){};

	// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCustomTabCtrl)
protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CCustomTabCtrl)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnUpdateEdit();
#ifndef _WIN32_WCE
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
#endif
	afx_msg LRESULT OnTcmAdjustRect(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
	void RecalcLayout(int nRecalcType,int nItem);
	void RecalcEditResized(int nOffset, int nItem);
	void RecalcOffset(int nOffset);
	int	RecalcRectangles();
	BOOL RegisterWindowClass();
	int	ProcessLButtonDown(int nHitTest, UINT nFlags, CPoint point);
	int	MoveItem(int nItemSrc, int nItemDst, BOOL fMouseSel);
	int	CopyItem(int nItemSrc, int nItemDst, BOOL fMouseSel);
	int	SetCurSel(int nItem, BOOL fMouseSel, BOOL fCtrlPressed);
	int	HighlightItem(int nItem, BOOL fMouseSel, BOOL fCtrlPressed);
	void DrawGlyph(CDC& dc, CPoint& pt, int nImageNdx, int nColorNdx);
	BOOL NotifyParent(UINT code, int nItem, CPoint pt);
	int	EditLabel(int nItem, BOOL fMouseSel);

protected:
	static LOGFONT lf_default;
#ifdef _WIN32_WCE
	static LOGFONT lf_default_vga;
#endif
	static BYTE	m_bBitsGlyphs[];
	CFont m_Font;
	CFont m_FontSelected;
	int	m_nItemSelected;
	int	m_nFirstVisibleItem;
	int	m_nItemDragDest;
	//button states for prev, next, first, last, close buttons
	int	m_nPrevState;
	int	m_nNextState;
	int	m_nFirstState;
	int	m_nLastState;
	int	m_nCloseState;
	
	int	m_nButtonIDDown;

	DWORD m_dwLastRepeatTime;

	COLORREF m_rgbGlyph[4];
	CBitmap	m_bmpGlyphsMono;
	CEdit m_ctrlEdit;
    CArray <CCustomTabItem*, CCustomTabItem*> m_aItems;
    TabItemColors m_ticColors;	//tab item colors
	int m_nTabsHeight;			//tabs height
};

//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
class CCustomTabContainer : public CCustomTabCtrl
{
	DECLARE_DYNAMIC(CCustomTabContainer)

	// Construction
public:
	CCustomTabContainer();

	// Attributes
public:
	void AddDialog(int nIndex, CString strText, CDialog *pDlg);
	void RemoveDialog(int nIndex);
	void Display();

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyTabCtrl)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CCustomTabContainer();

	// Generated message map functions
protected:
	void FixStyle(HWND hWnd);

	CArray <CDialog*, CDialog*> m_Dlgs;


	//{{AFX_MSG(MyTabCtrl)
    //afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTabBtnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
