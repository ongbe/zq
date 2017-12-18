#include "StdAfx.h"
#include "ColorButton.h"

IMPLEMENT_DYNAMIC(CColorButton, CButton) 

CColorButton::CColorButton(void)
{
	m_fg = RGB(1, 1, 1);
	m_bg = RGB(192, 123, 192);
	m_bPressDown = FALSE;
}

CColorButton::~CColorButton(void)
{
}

//定义Attach（）函数 
BOOL CColorButton::Attach(const UINT nID, CWnd* pParent, const COLORREF BGColor, const COLORREF FGColor) 
{ 
	if(!SubclassDlgItem(nID, pParent)) 
		return FALSE; 
	m_fg = FGColor; 
	m_bg = BGColor; 
	return TRUE; 
} 

BOOL CColorButton::PreTranslateMessage(MSG* pMsg)
{
	BOOL bResult = FALSE;
	CRect rect;
	CPoint pos;

	switch(pMsg->message)
	{
		case WM_MOUSEMOVE:
			{
				::GetCursorPos(&pos);
				GetWindowRect(&rect);
				if(!rect.PtInRect(pos) && m_bPressDown)
				{
					m_bPressDown = FALSE;
					Invalidate();
					bResult = TRUE;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				::GetCursorPos(&pos);
				GetWindowRect(&rect);
				if(rect.PtInRect(pos))
				{
					m_bPressDown = TRUE;
					Invalidate();
					bResult = TRUE;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				::GetCursorPos(&pos);
				GetWindowRect(&rect);
				if(rect.PtInRect(pos))
				{
					m_bPressDown = FALSE;
					Invalidate();
					bResult = TRUE;
				}
			}
			break;
	}
	
	return __super::PreTranslateMessage(pMsg);
;
}

//重载DrawItem（） 
void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{ 
	CDC* pDC = CDC::FromHandle(lpDIS->hDC); 
	UINT state = lpDIS->itemState; 
	CRect focusRect, btnRect; 
	focusRect.CopyRect(&lpDIS->rcItem); //按纽的选中虚线框 
	btnRect.CopyRect(&lpDIS->rcItem); // 设置表示按纽被选中的虚线框 
	focusRect.left += 4; 
	focusRect.right -= 4; 
	focusRect.top += 4; 
	focusRect.bottom -= 4; 
	// 按纽标题 
	const int bufSize = 512; 
	TCHAR buffer[bufSize]; 
	GetWindowText(buffer, bufSize); 
	// 绘制并标志按纽 
	DrawFilledRect(pDC, btnRect, GetBGColor()); 
	if(m_bPressDown)
		DrawFrameSunken(pDC, btnRect); 
	else
		DrawFrame(pDC, btnRect); 
	DrawButtonText(pDC, btnRect, buffer, GetFGColor()); 
	// 如果按纽处于选中状态则在其上绘制选中虚线框 
	if (state & ODS_FOCUS) { 
		DrawFocusRect(lpDIS->hDC, (LPRECT)&focusRect); 
	} 
} 

void CColorButton::DrawFrame(CDC *DC, CRect R) 
{ 
	//绘制按纽，用户通过定制该函数可实现不同形状的按纽。 
	DrawLine(DC, R.left, R.top, R.right, R.top, RGB(255, 255, 255)); 
	DrawLine(DC, R.left, R.top, R.left, R.bottom, RGB(255, 255, 255)); 
	//以下绘制按纽的外围框线以使按纽有立体感 
	DrawLine(DC, R.left + 1, R.bottom - 1, R.right, R.bottom - 1, RGB(1, 1, 1)); 
	//绘制按纽左框线和上框线 
	DrawLine(DC, R.right - 1, R.top + 1, R.right - 1, R.bottom, RGB(1, 1, 1)); 
	//绘制按纽右框线和下框线 
} 

void CColorButton::DrawFrameSunken(CDC *DC, CRect R) 
{ 
	//绘制按纽，用户通过定制该函数可实现不同形状的按纽。 
	DrawLine(DC, R.left, R.top, R.right, R.top, RGB(1, 1, 1)); 
	DrawLine(DC, R.left, R.top, R.left, R.bottom, RGB(1, 1, 1)); 
	//以下绘制按纽的外围框线以使按纽有立体感 
	DrawLine(DC, R.left + 1, R.bottom - 1, R.right, R.bottom - 1, RGB(255, 255, 255)); 
	//绘制按纽左框线和上框线 
	DrawLine(DC, R.right - 1, R.top + 1, R.right - 1, R.bottom, RGB(255, 255, 255)); 
	//绘制按纽右框线和下框线 
} 

//用色彩填充按纽框 
void CColorButton::DrawFilledRect(CDC *DC, CRect R, COLORREF color) 
{ 
	CBrush B; 
	B.CreateSolidBrush(color); 
	DC->FillRect(R, &B); 
} 

// DrawLine用于绘制按纽，其为多态函数 
void CColorButton::DrawLine(CDC *DC, CRect EndPoints, COLORREF color)
{
} 

void CColorButton::DrawLine(CDC *DC, long left, long top, long right, long bottom, COLORREF color) 
{
	CPen penCurr;

	penCurr.CreatePen(PS_SOLID, 1, color);
	CPen* pOldPen = pOldPen = DC->SelectObject(&penCurr);
	DC->MoveTo(left, top);
	DC->LineTo(right, bottom);
	DC->SelectObject(pOldPen);
	penCurr.DeleteObject();
} 

//绘制按纽文本 
void CColorButton::DrawButtonText(CDC *DC, CRect R, const char *Buf, COLORREF TextColor) 
{ 
	COLORREF prevColor = DC->SetTextColor(TextColor); 
	DC->SetBkMode(TRANSPARENT); 
	DC->DrawText(Buf, strlen(Buf), R, DT_CENTER|DT_VCENTER|DT_SINGLELINE); 
	DC->SetTextColor(prevColor); 
} 
