// NCButton.h: interface for the CNCButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCBUTTON_H__F74B28E4_0F45_4E80_B6C4_59FD5968D2AA__INCLUDED_)
#define AFX_NCBUTTON_H__F74B28E4_0F45_4E80_B6C4_59FD5968D2AA__INCLUDED_

using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 对齐方式

#define ALIGN_LEFT				0x0001
#define ALIGN_TOP				0x0002
#define ALIGN_RIGHT				0x0004
#define ALIGN_BOTTOM			0x0008

// 按钮状态
#define BTNSTATE_NORMAL			0x0000		// 正常
#define BTNSTATE_MOUSEOVER		0x0001		// 鼠标在上面
#define BTNSTATE_LBUTTONDOWN	0x0002		// 鼠标左键按下

// 按钮位图
#define BTNBMP_NORMAL			0x0000
#define BTNBMP_MOUSEOVER		0x0001
#define BTNBMP_LBUTTONDOWN		0x0002

class CNCButton
{
private:
	HWND	m_hParentWnd;			// 父窗口句柄
	UINT	m_nID;					// 按钮ID
	RECT	m_Rect;					// 按钮具体位置
	SIZE	m_sizeBtn;				// 尺寸
	POINT	m_ptOffset;				// 位置
	BOOL	m_bCreated;				// 创建成功的标志
	string	m_strText;				// 按钮文字
	string  m_strTooltip;			// 工具提示
	UINT	m_nAlign;				// 对齐方式
	UINT	m_nState;				// 状态
	UINT	m_nBmpNormal;			// 正常时候的位图ID
	UINT	m_nBmpHover;			// 热点位图ID
	UINT	m_nBmpDown;				// 按下时的位图ID
	HFONT	m_hFont;				// 字体
	HWND	m_hTooltip;				// 工具提示

public:
	void	SetTooltip(LPCTSTR lpszText);
	void	ShowTooltip(BOOL bShow);
	void	SetText(LPCTSTR lpszText, LPCTSTR lpszFont, int nSize, BYTE lfCharSet = GB2312_CHARSET);
	void	SetButtonBitmap(UINT nBmpID, UINT nCase);
	void	DrawTransparentBmp(HDC hdc, int nXOriginDest, int nYOriginDest, int nWidth, int nHeight, WORD wBmpID, COLORREF crTransparent);
	void	FillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr);
	void	Draw3dRect(HDC hdc, LPRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight );
	UINT	GetButtonID() { return m_nID; }
	UINT	GetState() { return m_nState; }
	void	SetState(UINT nState) { m_nState = nState;	}
	HWND	GetParentWnd() { return m_hParentWnd; }
	BOOL	OnHitTest(POINT point);
	void	SetAlign(UINT nAlign);
	BOOL	Create(LPCTSTR lpszText, HWND hParentWnd, POINT& ptOffset, SIZE& sizeBtn, UINT nID);
	BOOL	IsCreated() { return m_bCreated; }
	void	OnPaint();

	CNCButton();

	virtual void OnSetCursor();
	virtual void OnParentSize(UINT nSide, LPRECT lpRect );
	virtual void OnPaintDown(HDC hdc);
	virtual void OnPaintHover(HDC hdc);
	virtual void OnPaintNormal(HDC hdc);
	virtual void OnMouseMove(POINT point);
	virtual void OnMouseLButtonDown();
	virtual void OnMouseLButtonUp();
	virtual void OnMouseClick();
	virtual void OnMouseLeave();
	virtual ~CNCButton();

};

#endif // !defined(AFX_NCBUTTON_H__F74B28E4_0F45_4E80_B6C4_59FD5968D2AA__INCLUDED_)
