#pragma once



// CExtTabCtrl

class CExtTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CExtTabCtrl)

public:
	CExtTabCtrl(COLORREF ParentBkCol=RGB(240,240,240),
				COLORREF ClientBkCol=RGB(252,252,254),
				COLORREF ItemBkCol=RGB(240,240,235),
				COLORREF LineCol=RGB(0,0,0),
				COLORREF MainBorderCol=RGB(145,155,156),
				COLORREF MainBorderShadowCol=RGB(192,198,198),
				COLORREF ItemBorderCol=RGB(145,167,180),
				COLORREF TextCol=RGB(0,0,0),int FontSize=12,const char* FontName="ËÎÌו");
	virtual ~CExtTabCtrl();

	RECT GetPageRect(void);

private:
	COLORREF m_ParentBkCol;
	COLORREF m_ClientBkCol;
	COLORREF m_ItemBkCol;
	COLORREF m_LineCol;
	COLORREF m_MainBorderCol;
	COLORREF m_MainBorderShadowCol;
	COLORREF m_ItemBorderCol;
	COLORREF m_TextCol;
	int m_FontSize;
	char m_FontName[256];

	CBrush m_brush;
	CPen m_Pen;
	CFont m_Font;

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void DrawItem2(CDC& dc,int itemID);
	void DrawMainBorder(CDC& dc);
	
	void FillGradientRect(CDC& dc,const RECT& rect,COLORREF topcol,COLORREF bomcol);
	COLORREF Darker(COLORREF crBase, float fFactor);
	COLORREF Lighter(COLORREF crBase, float fFactor);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


