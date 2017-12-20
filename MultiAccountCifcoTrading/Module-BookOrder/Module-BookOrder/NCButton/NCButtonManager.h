// NCButtonManager.h: interface for the CNCButtonManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NCBUTTONMANAGER_H__61246928_E0C2_443B_86C3_0907CC73A75E__INCLUDED_)
#define AFX_NCBUTTONMANAGER_H__61246928_E0C2_443B_86C3_0907CC73A75E__INCLUDED_

using namespace std;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNCButton;

class CNCButtonManager  
{
private:
	HWND m_hParentWnd;
	BOOL m_bWindowPosChanging;
	vector<CNCButton *> m_TitleButtonVector;
	CNCButton* m_pHoverButton;					// 鼠标在按钮之上
	CNCButton* m_pPressButton;					// 鼠标按下的按钮
	
public:
	CNCButtonManager();

	void	ResetAllButtons();
	void	OnNcActivate(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	OnActivate(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	OnActivateApp(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	RemoveButton(CNCButton *pButton);
	void	OnWindowPosChanged(HWND hwnd);
	void	OnWindowPosChanging(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	OnMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	OnLButtonUp(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	OnLButtonDown(HWND hwnd, WPARAM wParam, LPARAM lParam);
	BOOL	OnRButtonUp(HWND hwnd, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcHitTest(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	OnParentSize(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	OnParentSizing(HWND hwnd, WPARAM wParam, LPARAM lParam);
	void	OnPaint(HWND hwnd);
	BOOL	AddButton(CNCButton *pButton);
	CNCButton* EnumActiveButton(UINT& nIdx, HWND hwnd);

	virtual LRESULT DefWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual ~CNCButtonManager();

};

#endif // !defined(AFX_NCBUTTONMANAGER_H__61246928_E0C2_443B_86C3_0907CC73A75E__INCLUDED_)
