// NCButton.cpp: implementation of the CNCButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void TransparentBlt2( HDC hdcDest,      // 目标DC
					 int nXOriginDest,   // 目标X偏移
					 int nYOriginDest,   // 目标Y偏移
					 int nWidthDest,     // 目标宽度
					 int nHeightDest,    // 目标高度
					 HDC hdcSrc,         // 源DC
					 int nXOriginSrc,    // 源X起点
					 int nYOriginSrc,    // 源Y起点
					 int nWidthSrc,      // 源宽度
					 int nHeightSrc,     // 源高度
					 UINT crTransparent  // 透明色,COLORREF类型
					 )
{
	HBITMAP hOldImageBMP, hImageBMP = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);	// 创建兼容位图
	HBITMAP hOldMaskBMP, hMaskBMP = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);			// 创建单色掩码位图
	HDC		hImageDC = CreateCompatibleDC(hdcDest);
	HDC		hMaskDC = CreateCompatibleDC(hdcDest);
	hOldImageBMP = (HBITMAP)SelectObject(hImageDC, hImageBMP);
	hOldMaskBMP = (HBITMAP)SelectObject(hMaskDC, hMaskBMP);
	
	// 将源DC中的位图拷贝到临时DC中
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
		BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY);
	else
		StretchBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, 
		hdcSrc, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	
	// 设置透明色
	SetBkColor(hImageDC, crTransparent);
	
	// 生成透明区域为白色，其它区域为黑色的掩码位图
	BitBlt(hMaskDC, 0, 0, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCCOPY);
	
	// 生成透明区域为黑色，其它区域保持不变的位图
	SetBkColor(hImageDC, RGB(0,0,0));
	SetTextColor(hImageDC, RGB(255,255,255));
	BitBlt(hImageDC, 0, 0, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// 透明部分保持屏幕不变，其它部分变成黑色
	SetBkColor(hdcDest,RGB(255,255,255));
	SetTextColor(hdcDest,RGB(0,0,0));
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hMaskDC, 0, 0, SRCAND);
	
	// "或"运算,生成最终效果
	BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hImageDC, 0, 0, SRCPAINT);
	
	// 清理、恢复	
	SelectObject(hImageDC, hOldImageBMP);
	DeleteDC(hImageDC);
	SelectObject(hMaskDC, hOldMaskBMP);
	DeleteDC(hMaskDC);
	DeleteObject(hImageBMP);
	DeleteObject(hMaskBMP);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNCButton::CNCButton()
{
	m_bCreated = FALSE;
	m_hParentWnd = NULL;
	m_hTooltip = NULL;
	m_strText = "";
	m_nAlign = ALIGN_RIGHT | ALIGN_TOP;
	m_nState = BTNSTATE_NORMAL;
	m_nBmpNormal = 0;
	m_nBmpHover = 0;
	m_nBmpDown = 0;
	m_hFont = NULL;
}

CNCButton::~CNCButton()
{
	if(m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}	
}

//------------------------------------------------
//	绘制按钮
//------------------------------------------------

void CNCButton::OnPaint()
{
	//CCriticalSection cs;
	//cs.Lock();

	HDC hdc = GetWindowDC(m_hParentWnd);

	POINT posOrg;
	SetViewportOrgEx(hdc, m_Rect.left, m_Rect.top, &posOrg);

	// 根据不同的状态画按钮
	if((m_nState & BTNSTATE_MOUSEOVER) == BTNSTATE_MOUSEOVER)
	{
		if((m_nState & BTNSTATE_LBUTTONDOWN) == BTNSTATE_LBUTTONDOWN)
		{
			OnPaintDown(hdc);
		}
		else
		{
			OnPaintHover(hdc);
		}
	}
	else
	{
		OnPaintNormal(hdc);
	}
	SetViewportOrgEx(hdc, posOrg.x, posOrg.y, NULL);
	ReleaseDC(m_hParentWnd, hdc);

}

//----------------------------------------------------------------
//	创建按钮
//	ptOffset为X坐标的位置与Y坐标的位置，具体的位置与对齐方式有关
//	sizeBtn是按钮的宽度与高度
//----------------------------------------------------------------

BOOL CNCButton::Create(LPCTSTR lpszText, HWND hParentWnd, POINT& ptOffset, SIZE& sizeBtn, UINT nID)
{
	if(m_bCreated)
		return FALSE;

	m_hParentWnd = hParentWnd;
	m_strText = lpszText;
	m_ptOffset = ptOffset;
	m_sizeBtn = sizeBtn;
	m_nID = nID;

	if(m_hParentWnd)
	{	
		if(strlen(lpszText) > 0)
		{
			// 创建默认字体
			LOGFONT logFont;
			ZeroMemory((void*)&logFont, sizeof(logFont));
			strcpy(logFont.lfFaceName, "宋体");
			logFont.lfHeight = -12;
			logFont.lfWeight = 400;
			logFont.lfCharSet = GB2312_CHARSET;
			logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logFont.lfClipPrecision = OUT_DEFAULT_PRECIS;
			logFont.lfQuality = PROOF_QUALITY;
			logFont.lfPitchAndFamily = VARIABLE_PITCH|FF_ROMAN;
			m_hFont = CreateFontIndirect(&logFont);
		}

		RECT rectParent;
		GetWindowRect(hParentWnd, &rectParent);
		OnParentSize(0, &rectParent);
		m_bCreated = TRUE;
	}
	else
	{
		m_bCreated = FALSE;
	}

	return m_bCreated;
}

//-----------------------------------------------------
//	改变大小时调用
//	hParentWnd为父窗口, lpRect为父窗口大小
//-----------------------------------------------------

void CNCButton::OnParentSize(UINT nSide, LPRECT lpRect)
{
	RECT rectParent;

	rectParent.left = 0;
	rectParent.top = 0;
	rectParent.right = lpRect->right - lpRect->left;
	rectParent.bottom = lpRect->bottom - lpRect->top;

	if((m_nAlign & ALIGN_RIGHT) == ALIGN_RIGHT)
	{
		m_Rect.right = rectParent.right - m_ptOffset.x;
		m_Rect.left = m_Rect.right - m_sizeBtn.cx;
	}
	else
	{
		if((m_nAlign & ALIGN_LEFT) == ALIGN_LEFT)
		{
			m_Rect.left = rectParent.left + m_ptOffset.x;
			m_Rect.right = m_Rect.left + m_sizeBtn.cx;
		}
	}
	if((m_nAlign & ALIGN_BOTTOM) == ALIGN_BOTTOM)
	{
		m_Rect.bottom = rectParent.bottom - m_ptOffset.y;
		m_Rect.top = m_Rect.bottom - m_sizeBtn.cy;
	}
	else
	{
		if((m_nAlign & ALIGN_TOP) == ALIGN_TOP)
		{
			m_Rect.top = rectParent.top + m_ptOffset.y;
			m_Rect.bottom = m_Rect.top + m_sizeBtn.cy;
		}
	}
}

//------------------------------------------------
//	设置对齐方式
//------------------------------------------------

void CNCButton::SetAlign(UINT nAlign)
{
	m_nAlign = nAlign;
}

//------------------------------------------------
//	WM_HITTEST消息处理
//------------------------------------------------

BOOL CNCButton::OnHitTest(POINT point)
{
	//if(m_Rect.left == 272)
//	TRACE("%d,%d,%d,%d;%d,%d;()%d\r\n", m_Rect.left,m_Rect.top,m_Rect.right,m_Rect.bottom,
//		point.x,point.y,PtInRect(&m_Rect, point));
	
	if(PtInRect(&m_Rect, point))
	{
		m_nState |= BTNSTATE_MOUSEOVER;
		return TRUE;
	}
	else
	{
		m_nState &= ~BTNSTATE_MOUSEOVER;
		//TRACE("Hittest 结果:FALSE\r\n");
		return FALSE;
	}
}

//---------------------------------------------
//	鼠标移动的消息处理
//---------------------------------------------

void CNCButton::OnMouseMove(POINT point)
{
	SetState(GetState() | BTNSTATE_MOUSEOVER);
	if(GetCapture() != m_hParentWnd)
	{
		SetCapture(m_hParentWnd);

		OnPaint();
		OnSetCursor();
		ShowTooltip(TRUE);
	}
}

//---------------------------------------------
//	左键按下的消息处理
//---------------------------------------------

void CNCButton::OnMouseLButtonDown()
{
	SetState(GetState() | BTNSTATE_LBUTTONDOWN);
	SetCapture(m_hParentWnd);
	ShowTooltip(FALSE);
}

//---------------------------------------------
//	左键释放的消息处理
//---------------------------------------------

void CNCButton::OnMouseLButtonUp()
{
	SetState(GetState() & ~BTNSTATE_LBUTTONDOWN);
	if((GetState() & BTNSTATE_MOUSEOVER) != BTNSTATE_MOUSEOVER)
		ReleaseCapture();
}

//---------------------------------------------
//	鼠标离开按钮之上
//---------------------------------------------

void CNCButton::OnMouseLeave()
{
//	TRACE("MouseLeave\r\n");
	if((GetState() & BTNSTATE_LBUTTONDOWN) != BTNSTATE_LBUTTONDOWN)
		ReleaseCapture();
	ShowTooltip(FALSE);
}

void CNCButton::OnMouseClick()
{
	//MessageBox(m_hParentWnd, "Message", "VCKBASE", MB_OK);
}

//------------------------------------------------
// 画3D方框
//------------------------------------------------

void CNCButton::Draw3dRect(HDC hdc, LPRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	int nCX = lpRect->right - lpRect->left;
	int nCY = lpRect->bottom - lpRect->top;
	FillSolidRect(hdc, lpRect->left, lpRect->top, nCX - 1, 1, clrTopLeft);
	FillSolidRect(hdc, lpRect->left, lpRect->top, 1, nCY - 1, clrTopLeft);
	FillSolidRect(hdc, lpRect->left + nCX, lpRect->top, -1, nCY, clrBottomRight);
	FillSolidRect(hdc, lpRect->left, lpRect->top + nCY, nCX, -1, clrBottomRight);
}

//-------------------------------------------------
//	填充方框
//-------------------------------------------------

void CNCButton::FillSolidRect(HDC hdc, int x, int y, int cx, int cy, COLORREF clr)
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + cx;
	rect.bottom = y + cy;

	SetBkColor(hdc, clr);
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

//------------------------------------------
//	正常状态按钮
//------------------------------------------

void CNCButton::OnPaintNormal(HDC hdc)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_sizeBtn.cx;
	rect.bottom = m_sizeBtn.cy;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DDKSHADOW));
	
	rect.left++;
	rect.top++;
	rect.right--;
	rect.bottom--;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNSHADOW));
	FillSolidRect(hdc, rect.left + 1, rect.top + 1, rect.right - 3, rect.bottom - 3, GetSysColor(COLOR_BTNFACE));

	// 画位图
	UINT nBmpID = m_nBmpNormal;
	if(nBmpID != 0)
		DrawTransparentBmp(hdc, 2, 2, rect.right - 3, rect.bottom - 3, nBmpID, RGB(0xD8, 0xD4, 0xD0));

	// 输出文字
	HFONT hOldFont = NULL;
	if(m_hFont)
		hOldFont = (HFONT)SelectObject(hdc, m_hFont);

	SetBkMode(hdc, TRANSPARENT);
	//SetBkColor(hdc, RGB(0xff, 0x99, 0));
	DrawText(hdc, m_strText.c_str(), m_strText.size(), &rect, DT_CENTER | DT_VCENTER);

	if(hOldFont)
		SelectObject(hdc, hOldFont);
}

//------------------------------------------
//	鼠标在上面时的按钮
//------------------------------------------

void CNCButton::OnPaintHover(HDC hdc)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_sizeBtn.cx;
	rect.bottom = m_sizeBtn.cy;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_3DHIGHLIGHT), GetSysColor(COLOR_3DDKSHADOW));
	
	rect.left++;
	rect.top++;
	rect.right--;
	rect.bottom--;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_BTNFACE), GetSysColor(COLOR_BTNSHADOW));
	
	FillSolidRect(hdc, rect.left + 1, rect.top + 1, rect.right - 3, rect.bottom - 3, GetSysColor(COLOR_BTNFACE));
	
	// 画位图
	UINT nBmpID;
	if(m_nBmpHover != 0)
		nBmpID = m_nBmpHover;
	else
		nBmpID = m_nBmpNormal;

	if(nBmpID != 0)
		DrawTransparentBmp(hdc, 2, 2, rect.right - 3, rect.bottom - 3, nBmpID, RGB(0xD8, 0xD4, 0xD0));
		//DrawTransparentBmp(hdc, 2, 1, 10, 10, nBmpID, RGB(0xD4, 0xD0, 0xC8));

	// 输出文字
	HFONT hOldFont = NULL;
	if(m_hFont)
		hOldFont = (HFONT)SelectObject(hdc, m_hFont);
	
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0xf0));
	DrawText(hdc, m_strText.c_str(), m_strText.size(), &rect, DT_CENTER | DT_VCENTER);
	
	if(hOldFont)
		SelectObject(hdc, hOldFont);
}

//------------------------------------------
//	鼠标按下时的按钮
//------------------------------------------

void CNCButton::OnPaintDown(HDC hdc)
{
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = m_sizeBtn.cx;
	rect.bottom = m_sizeBtn.cy;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_3DDKSHADOW), GetSysColor(COLOR_3DHIGHLIGHT));
	
	rect.left++;
	rect.top++;
	rect.right--;
	rect.bottom--;
	Draw3dRect(hdc, &rect, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNFACE));
	
	FillSolidRect(hdc, rect.left + 1, rect.top + 1, rect.right - 3, rect.bottom - 3, GetSysColor(COLOR_BTNFACE));	

	// 画位图
	UINT nBmpID;
	if(m_nBmpDown != 0)
		nBmpID = m_nBmpDown;
	else
		nBmpID = m_nBmpHover;

	if(nBmpID != 0)
		DrawTransparentBmp(hdc, 3, 2, rect.right - 3, rect.bottom - 3, nBmpID, RGB(0xD8, 0xD4, 0xD0));

	// 输出文字
	HFONT hOldFont = NULL;
	if(m_hFont)
		hOldFont = (HFONT)SelectObject(hdc, m_hFont);
	
	rect.left += 2;
	rect.top += 2;
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0xf0));
	DrawText(hdc, m_strText.c_str(), m_strText.size(), &rect, DT_CENTER | DT_VCENTER);
	
	if(hOldFont)
		SelectObject(hdc, hOldFont);
}

//--------------------------------------------------------
//	画透明位图
//--------------------------------------------------------

void CNCButton::DrawTransparentBmp(HDC hdc, int nXOriginDest, int nYOriginDest, int nWidth, int nHeight, WORD wBmpID, COLORREF crTransparent)
{
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(m_hParentWnd, GWL_HINSTANCE);
	HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(wBmpID));
	HDC hTempDC = CreateCompatibleDC(hdc);
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hTempDC, hBmp);
	
	TransparentBlt2(hdc, nXOriginDest, nYOriginDest, nWidth, nHeight, hTempDC, 0, 0, nWidth, nHeight, crTransparent);
	
	if(hOldBmp)
		SelectObject(hTempDC, hOldBmp);
	
	DeleteDC(hTempDC);
	DeleteObject(hBmp);
}

//------------------------------------------------------
//	设置按钮在各个状态下的位图
//------------------------------------------------------

void CNCButton::SetButtonBitmap(UINT nBmpID, UINT nCase)
{
	switch(nCase)
	{
	case BTNBMP_NORMAL:
		m_nBmpNormal = nBmpID;
		break;
	case BTNBMP_MOUSEOVER:
		m_nBmpHover = nBmpID;
		break;
	case BTNBMP_LBUTTONDOWN:
		m_nBmpDown = nBmpID;
		break;
	}
}

//---------------------------------------------------------
//	设置按钮文字
//---------------------------------------------------------

void CNCButton::SetText(LPCTSTR lpszText, LPCTSTR lpszFont, int nSize, BYTE lfCharSet /* = GB2312_CHARSET*/)
{
	if(m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	// 创建字体
	LOGFONT logFont;
	ZeroMemory((void*)&logFont, sizeof(logFont));
	strcpy(logFont.lfFaceName, lpszFont);
	logFont.lfHeight = -nSize;
	logFont.lfWeight = 400;
	logFont.lfCharSet = lfCharSet;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfQuality = PROOF_QUALITY;
	logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	m_hFont = CreateFontIndirect(&logFont);

	m_strText = lpszText;
}

void CNCButton::ShowTooltip(BOOL bShow)
{	
	if(m_strTooltip.size() <= 0)
		return;

	if(m_hTooltip == NULL)
	{
		HINSTANCE hInstance = (HINSTANCE)GetWindowLong(m_hParentWnd, GWL_HINSTANCE);			
		m_hTooltip = CreateWindowEx(
			WS_EX_TOPMOST,
			"Tooltips_class32",
			NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_hParentWnd,
			NULL,
			hInstance,
			NULL
			);
	}

	TOOLINFO ti;
	ti.cbSize = sizeof(TOOLINFO);
	ti.uFlags = TTF_TRACK;
	ti.hwnd = NULL;
	ti.hinst = NULL;
	ti.uId = (UINT)m_hTooltip;
	ti.lpszText = (char *)m_strTooltip.c_str();

	if(bShow == TRUE)
	{
		RECT rect;
		GetWindowRect(m_hParentWnd, &rect);
		POINT point;
		GetCursorPos(&point);
		
		::SendMessage(m_hTooltip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
		::SendMessage(m_hTooltip, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD) MAKELONG(point.x + 16, point.y + 16));
		::SendMessage(m_hTooltip, TTM_TRACKACTIVATE, TRUE, (LPARAM)(LPTOOLINFO) &ti);
	}
	else
		::SendMessage(m_hTooltip, TTM_TRACKACTIVATE, FALSE, (LPARAM)(LPTOOLINFO) &ti);
		
/*
		}
		else
		{
			SendMessage(m_hTooltip, TTM_UPDATETIPTEXT, 0, (LPARAM) (LPTOOLINFO) &ti);
			
			SendMessage(m_hTooltip, TTM_UPDATE, 0, (LPARAM) (LPTOOLINFO) &ti);
		}*/
	

}

//-------------------------------------------------
//	设置工具提示
//-------------------------------------------------

void CNCButton::SetTooltip(LPCTSTR lpszText)
{
	m_strTooltip = lpszText;
}

//-------------------------------------------------
//	设置光标
//-------------------------------------------------

void CNCButton::OnSetCursor()
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));	
}
