/* Standard Disclaimer: 
Copyright (C) 2000  Dennis Howard
This file is free software; you can redistribute it and/or
modify it without any conditions. There is no warranty,
implied or expressed, as to validity or fitness for a particular purpose.
*/

// ComboTree.cpp : implementation file
//

#include "stdafx.h"
#include "ComboTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//non top level child control ids
#define IDC_COMBOTREE_BUTTON 4317         
#define IDC_COMBOTREE_EDIT   4318 
#define IDC_COMBOTREE_DROPDOWN   4319         




/////////////////////////////////////////////////////////////////////////////
// ComboTree

ComboTree::ComboTree()
{
	m_Tree.SetCombo (this);
	m_ComboWidth = 200;
	m_ComboHeight = 200;
	m_bDroppedState = FALSE;
	m_bCreateWithCheckboxes = FALSE;
}

ComboTree::~ComboTree()
{
}


BEGIN_MESSAGE_MAP(ComboTree, CWnd)
	//{{AFX_MSG_MAP(ComboTree)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED (IDC_COMBOTREE_BUTTON, OnDropdownButton )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// ComboTree message handlers

LRESULT ComboTree::SendParentComboMessage (UINT Action)
{
	CWnd* pParent = GetParent ();
	if (pParent && pParent->GetSafeHwnd ())
	{
		return pParent->SendMessage (WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(), Action),(LPARAM) GetSafeHwnd());
	}

	return 0L;
}

void ComboTree::SetWindowText (LPCTSTR Text)
{
	m_Edit.SetWindowText (Text);
	Invalidate ();
}


CString ComboTree::GetWindowText ()
{
	CString Text;
	m_Edit.GetWindowText(Text);
	return Text;
}

int ComboTree::GetLBText (HTREEITEM hItem, CString& rText)
{
  rText = _T("");
  int retval = CB_ERR;
  rText = m_Tree.GetItemText (hItem);
  if (rText.GetLength () > 0)
  {
	retval = 0;
  }
  return retval;
}

TCHAR ComboTree::GetPathDelimiter ()
{
	return m_Tree.GetPathDelimiter ();
}

void ComboTree::SetPathDelimiter (TCHAR Delimiter)
{
	m_Tree.SetPathDelimiter (Delimiter);
}

HTREEITEM ComboTree::FindChildItemData(DWORD SearchData, HTREEITEM hItem)
{
	return m_Tree.FindChildItemData(SearchData, hItem);
}

HTREEITEM ComboTree::FindChildItem (LPCTSTR Label, HTREEITEM hItem)
{
	return m_Tree.FindChildItem (Label, hItem);
}


HTREEITEM ComboTree::GetLastItem( HTREEITEM hItem )
{
	return m_Tree.GetLastItem(hItem );
}

HTREEITEM ComboTree::GetNextItem( HTREEITEM hItem )
{
	return m_Tree.GetNextItem( hItem);
}

HTREEITEM ComboTree::GetPrevItem( HTREEITEM hItem ) 
{
	return m_Tree.GetPrevItem( hItem );
}

HTREEITEM ComboTree::GetLastSibling( HTREEITEM hItem )
{
	return m_Tree.GetLastSibling( hItem );
}

void ComboTree::CollapseBranch( HTREEITEM hItem)
{
	 m_Tree.CollapseBranch( hItem);
}

void ComboTree::ExpandBranch( HTREEITEM hItem )
{
	 m_Tree.ExpandBranch( hItem );
}

void ComboTree::CollapseAllSiblings( HTREEITEM hNode )
{
	m_Tree.CollapseAllSiblings( hNode );
}

BOOL ComboTree::SetCheck( HTREEITEM hItem, BOOL bChecked)
{
	if (m_Tree.IsItemChecked(hItem))
	{
		if (bChecked)
		{
			//already checked
			return m_Tree.SetCheck( hItem, ComboTreeDropList::REFRESH);
		}
		else
		{
			//uncheck
			return m_Tree.SetCheck( hItem, ComboTreeDropList::UNCHECK);
		}
	}
	else
	{
		if (bChecked)
		{
			return m_Tree.SetCheck( hItem, ComboTreeDropList::CHECK);
		}
		else
		{
			//already unchecked
			return m_Tree.SetCheck( hItem, ComboTreeDropList::REFRESH);
		}

	}
}

BOOL ComboTree::IsItemChecked(HTREEITEM hItem)
{
	return m_Tree.IsItemChecked(hItem);
}

HTREEITEM ComboTree::GetFirstCheckedItem()
{
	return m_Tree.GetFirstCheckedItem();
}

HTREEITEM ComboTree::GetNextCheckedItem( HTREEITEM hItem )
{
	return m_Tree.GetNextCheckedItem(hItem );
}

HTREEITEM ComboTree::GetPrevCheckedItem( HTREEITEM hItem )
{
	return m_Tree.GetPrevCheckedItem(hItem );
}

void ComboTree::HideTree ()
{
	SetDroppedState (FALSE);
	m_Tree.ShowWindow( SW_HIDE );

	//Tree is no longer displayed, stop message interception
	m_ComboTreeHook.UnhookParent();

	m_Edit.Invalidate ();
	m_DropDownButton.Invalidate();
	Invalidate();
}

void ComboTree::ShowTree ()
{
	//Must set focus to edit here or dropdown will still have it and act on
	//arrow key messages
		
	m_Edit.SetFocus();	
	DisplayTree ();
	SendParentComboMessage (CBN_DROPDOWN);

	//Must call this to intercept parent window messsages
	m_ComboTreeHook.HookParent();


	m_Edit.Invalidate ();
	m_DropDownButton.Invalidate();
	Invalidate();
}


void ComboTree::OnDropdownButton ()
{
	
    if (GetDroppedState())
	{
		OnSelection ();
	}
	else
	{
		ShowTree();
	}
}


void ComboTree::CalculateDroppedRect(LPRECT lpDroppedRect)
{
	_ASSERTE (lpDroppedRect);

	if (!lpDroppedRect)
	{
		return;
	}

	CRect rectCombo;
	GetWindowRect(&rectCombo);

	//adjust to either the top or bottom
	int DropTop = rectCombo.bottom;
	int ScreenHeight = GetSystemMetrics (SM_CYSCREEN);
	if ((DropTop + m_ComboHeight) > ScreenHeight)
	{
		DropTop = rectCombo.top - m_ComboHeight;
	}

	//adjust to either the right or left
	int DropLeft = rectCombo.left;
	int ScreenWidth = GetSystemMetrics (SM_CXSCREEN);
	if ((DropLeft + m_ComboWidth) > ScreenWidth)
	{
		DropLeft = rectCombo.right - m_ComboWidth;
	}

	lpDroppedRect->left  = DropLeft;
	lpDroppedRect->top   = DropTop;
	lpDroppedRect->bottom = DropTop + m_ComboHeight;
	lpDroppedRect->right  = DropLeft + m_ComboWidth;

}

void ComboTree::DisplayTree()
{
	CRect rect(0,0,200,200);

	CalculateDroppedRect (&rect);


	m_Tree.SetWindowPos (&wndTopMost, rect.left, rect.top,
		                 rect.Width (), rect.Height (), SWP_SHOWWINDOW );

	m_BeginPath = m_Tree.GetCurrentTreePath ();

	SetDroppedState (TRUE);

	
	m_Tree.PostMessage (WM_SETFOCUS);
	m_Tree.SetActiveWindow ();
	m_Tree.SetForegroundWindow();
}


BOOL ComboTree::SetHasCheckboxes (BOOL bHasCheckboxes)
{
	//can't call this tree has no checkboxes
	if (m_bCreateWithCheckboxes == bHasCheckboxes)
	{
		//already set to this value
		return TRUE;
	}

	
	if (m_Tree.GetSafeHwnd())
	{
		_ASSERTE(("Checkbox style must be set before tree is created!", FALSE));
		return FALSE;
	}

	m_bCreateWithCheckboxes = bHasCheckboxes;


	return TRUE;
}

BOOL ComboTree::GetHasCheckboxes ()
{
	return m_bCreateWithCheckboxes;
}


BOOL ComboTree::CreateTree ()
{	
	CWnd* pParent = GetParent ();
	if (!pParent && pParent->GetSafeHwnd ())
	{
		return FALSE;
	}

	CRect treeRect (0,0, m_ComboWidth, m_ComboHeight);

	//can't have a control Id with WS_POPUP style
	CWnd* pDesktop = GetDesktopWindow ();
    if (! m_Tree.CreateEx ( WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR | WS_EX_TOOLWINDOW,
						   	WS_CHILD | WS_BORDER | TVS_SINGLEEXPAND | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_NOTOOLTIPS,
							treeRect, pDesktop, IDC_COMBOTREE_DROPDOWN))
	{
		return FALSE;
	}

	m_ComboTreeHook.Init(::GetParent(m_hWnd), this);	 // initialize


	return TRUE;
}


BOOL ComboTree::CreateButton ()
{	
	CRect btnRect;
	GetClientRect (&btnRect);

	int width = GetSystemMetrics (SM_CXVSCROLL);
	btnRect.left = 	btnRect.right - width;
	
     m_DropDownButton.CreateEx( 0, 
      _T("BUTTON"), NULL,
      WS_TABSTOP | BS_PUSHBUTTON | BS_NOTIFY | WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
      btnRect, this, IDC_COMBOTREE_BUTTON);

	m_DropDownButton.MoveWindow (&btnRect);

	return TRUE;
}


//Requires that dropdown button be created first, to size it properly
BOOL ComboTree::CreateEdit ()
{	
	if (!m_DropDownButton.GetSafeHwnd ())
	{
		return FALSE;
	}

	CRect clientRect;
	GetClientRect (&clientRect);

	CRect btnRect;
	m_DropDownButton.GetWindowRect (&btnRect);
	ScreenToClient (&btnRect);

	CRect editRect;
	editRect.left = clientRect.left;
	editRect.top = clientRect.top;
	editRect.bottom = clientRect.bottom;
	editRect.right = btnRect.left;

	BOOL bCreate = m_Edit.Create(WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER ,  editRect, this, IDC_COMBOTREE_EDIT);


	if (bCreate)
	{
		CWnd* pParent = GetParent ();
		CFont* pFont = pParent->GetFont ();
		m_Edit.SetFont (pFont);
	}

	return bCreate;
}


//Creates ComboTree window, used by SubclassDlgItem

BOOL ComboTree::Create( const RECT& rect, int DroppedWidth, int DroppedHeight, CWnd* pParentWnd, UINT nID )
{
	ASSERT (pParentWnd != NULL);
	if (!pParentWnd)
	{
		return FALSE;
	}

	m_ComboWidth = DroppedWidth;
	m_ComboHeight = DroppedHeight;


   CString className = AfxRegisterWndClass(   CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
						::LoadCursor(NULL, IDC_ARROW),
						(HBRUSH) ::GetStockObject(WHITE_BRUSH));


   BOOL bCreate =  CreateEx( WS_EX_CLIENTEDGE, // 3D  client edge 
		 className, NULL,
		 WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		 rect, pParentWnd, nID);

	_ASSERTE (bCreate);

	if (!bCreate)
	{
		return FALSE;
	}

	UpdateWindow ();

	return TRUE;
}

//Removes the original ComboBox control instead of subclassing it
BOOL ComboTree::SubclassDlgItem (UINT nID, CWnd* pParent)
{
	CComboBox* pCombo = static_cast<CComboBox*>(pParent->GetDlgItem (nID));
	if (!pCombo)
	{
		return FALSE;
	}

	CRect DroppedRect;
	pCombo->GetDroppedControlRect (&DroppedRect);

 	CRect rect;
	pCombo->GetWindowRect (&rect);
	pParent->ScreenToClient (&rect);

	if (!Create (rect, DroppedRect.Width(), DroppedRect.Height(), pParent, nID))
	{
		return FALSE;
	}

	//Set Z Order to follow after original combo
	SetWindowPos (pCombo, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

	pCombo->DestroyWindow (); //remove original window

	return TRUE; 
}

int ComboTree::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!CreateTree())
	{
		return -1;
	}

	if (!CreateButton())
	{
		return -1;
	}

	if (!CreateEdit())
	{
		return -1;
	}

	return 0;
}


CString ComboTree::GetTreePath (HTREEITEM hItem)
{
	return m_Tree.GetTreePath (hItem);
}

CString ComboTree::GetCurrentTreePath ()
{
	return m_Tree.GetCurrentTreePath ();
}


HTREEITEM ComboTree::AddString ( LPCTSTR lpszString)
{
	return m_Tree.AddString (lpszString);
}

HTREEITEM ComboTree::FindString ( LPCTSTR lpszString, HTREEITEM hParent /*=NULL*/)
{
	return m_Tree.FindString (lpszString, hParent);
}


int ComboTree::DeleteString( HTREEITEM hItem )
{
	HTREEITEM hSelected = m_Tree.GetSelectedItem ();
	if (m_Tree.DeleteItem (hItem))
	{
		if (hItem == hSelected)
		{
			//get the new item
			hItem = m_Tree.GetSelectedItem ();
			if (hItem)
			{
				CString NodeText = m_Tree.GetItemText (hItem);
				SetWindowText (NodeText);
				SendParentComboMessage (CBN_SELCHANGE);
			}
			else
			{
				SetWindowText (_T(""));
			}
		}

		return 0;
	}
	else
	{
		return CB_ERR;
	}
}


HTREEITEM ComboTree::SelectString( LPCTSTR lpszString, HTREEITEM hParent /*=NULL*/)
{
	HTREEITEM hMatch = m_Tree.SelectString (lpszString, hParent);
	CString NodeText = m_Tree.GetItemText (hMatch);
	SetWindowText (NodeText);

	SendParentComboMessage (CBN_SELCHANGE);

	return hMatch;
}

HTREEITEM ComboTree::GetCurSel ()
{
	return m_Tree.GetSelectedItem ();
}

int ComboTree::SetItemData (HTREEITEM hItem, DWORD dwItemData)
{
	if (!m_Tree.SetItemData (hItem, dwItemData))
	{
		return CB_ERR;
	}
	else 
	{
		return 0;
	}
}

DWORD ComboTree::GetItemData (HTREEITEM hItem)
{
	return m_Tree.GetItemData (hItem);
}

void ComboTree::ShowDropDown( BOOL bShowIt)
{
	if (bShowIt)
	{
		if (!GetDroppedState ())
		{
			ShowTree ();
		}
	}
	else 
	{
		if (!GetDroppedState())
		{
			return; //there is no active dropdown, can't hide
		}
	
		HideTree ();
	}
}

void ComboTree::ResetContent ()
{
	m_Tree.DeleteAllItems( );
}


int ComboTree::SetDroppedWidth( UINT nWidth )
{
	int retval = 0;
	if (nWidth >= 0)
	{
		m_ComboWidth = nWidth;
		if (GetDroppedState ())
		{
			DisplayTree ();
		}
	}
	else
	{
		retval = CB_ERR;
	}
	return retval;
}

int ComboTree::GetDroppedWidth( )
{
	return m_ComboWidth;
}

int ComboTree::SetDroppedHeight (UINT nHeight)
{
	int retval = 0;
	if (nHeight >= 0)
	{
		m_ComboHeight = nHeight;
		if (GetDroppedState ())
		{
			DisplayTree ();
		}
	}
	else
	{
		retval = CB_ERR;
	}
	return retval;
}

int ComboTree::GetDroppedHeight ()
{
	return m_ComboHeight;
}

BOOL ComboTree::GetDroppedState( )
{
	return m_bDroppedState;
}

void ComboTree::GetDroppedControlRect (LPRECT pRect)
{
	_ASSERTE (pRect);
	if (pRect)
	{
		CalculateDroppedRect (pRect);
	}
}

void ComboTree::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	if (::GetFocus() != m_DropDownButton.GetSafeHwnd())
	{
		m_Edit.Invalidate ();
	}
}

BOOL ComboTree::IsMouseOnButton() 
{
	BOOL bMouseOn = FALSE;
	CPoint point;
	if (GetCursorPos (&point))
	{
		CRect rect;
		m_DropDownButton.GetWindowRect (&rect);
		if (rect.PtInRect (point))
		{
			bMouseOn = TRUE;
		}
	}
	return bMouseOn;
}

BOOL ComboTree::IsMouseOnEdit() 
{
	BOOL bMouseOn = FALSE;
	CPoint point;
	if (GetCursorPos (&point))
	{
		CRect rect;
		m_Edit.GetWindowRect (&rect);
		if (rect.PtInRect (point))
		{
			bMouseOn = TRUE;
		}
	}
	return bMouseOn;
}

BOOL ComboTree::IsEditHighlightOn () 
{
	BOOL bHighlightOn = FALSE;
	HWND hFocus = ::GetFocus ();

	if (hFocus == GetSafeHwnd () || 
		hFocus == m_Edit.GetSafeHwnd  () ||
		hFocus == m_DropDownButton.GetSafeHwnd  ())
	{
		bHighlightOn = TRUE;
	}

	return bHighlightOn;
}


void ComboTree::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	m_Edit.Invalidate ();	
}


BOOL ComboTree::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == BN_KILLFOCUS	)
	{
		m_Edit.Invalidate ();
	}

	return CWnd::OnCommand(wParam, lParam);
}

BOOL ComboTree::OnKeyDropdownInactive(UINT message, UINT nChar) 
{
	if(GetDroppedState())
	{
		return FALSE;
	}

	if ((message == WM_SYSKEYDOWN) )
	{
		if ((nChar == VK_DOWN))
		{
			ShowTree ();
			return FALSE;
		}
	}
	else if ((message == WM_KEYDOWN) && (nChar == VK_F4))
	{
		ShowTree ();
		return FALSE;
	}
	else if ((message == WM_KEYDOWN) && (nChar == VK_TAB))
	{
		
		BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
		
		CWnd* pComboParent = GetParent ();
		if (pComboParent && pComboParent->GetSafeHwnd ())
		{
			CWnd* pNext = pComboParent->GetNextDlgTabItem (this, bShift);
			if (pNext && pNext->GetSafeHwnd ())
			{
//				TRACE ("ComboTree::OnKeyDropdownInactive() -Moving focus\n");
				pNext->SetFocus ();
			}
		}
		return FALSE;
	}

	return TRUE;
}

BOOL ComboTree::OnKeyDropdownActive(UINT message, UINT nChar) 
{
	if(!GetDroppedState())
	{
		return FALSE;
	}

	if (message == WM_SYSKEYDOWN)
	{
		if ((nChar == VK_UP))
		{
			OnSelection ();
			return FALSE;
		}
	}
	else if ((message == WM_KEYDOWN) && (nChar == VK_F4))
	{
		OnSelection ();
		return FALSE;
	}
	else if (nChar == VK_RETURN)
	{
		OnSelection ();
		return FALSE;
	}
	else if (nChar == VK_ESCAPE)
	{
		OnCancel ();
		return FALSE;
	}
	else if ((message == WM_KEYDOWN) && (nChar == VK_TAB))
	{
		OnSelection ();
		
		BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
		
		CWnd* pComboParent = GetParent ();
		if (pComboParent && pComboParent->GetSafeHwnd ())
		{
			CWnd* pNext = pComboParent->GetNextDlgTabItem (this, bShift);
			if (pNext && pNext->GetSafeHwnd ())
			{
//				TRACE ("ComboTree::OnKeyDropdownActive(() Moving Focus\n");
				pNext->SetFocus ();
			}
		}
		return FALSE;
	}

	return TRUE;
}

BOOL ComboTree::PreTranslateMessage(MSG* pMsg) 
{
	if(GetDroppedState())
	{

		BOOL bContinue = TRUE;
		if ((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
		{
			bContinue = OnKeyDropdownActive(pMsg->message, pMsg->wParam);
			if (bContinue)
			{
				bContinue = m_Tree.SendMessage( pMsg->message, pMsg->wParam, pMsg->lParam);
			}
			if (!bContinue)
			{
				return FALSE;
			}
		}
	}
	else //not dropped down
	{
		BOOL bContinue = TRUE;
		if ((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
		{
			bContinue = OnKeyDropdownInactive(pMsg->message, pMsg->wParam);
			if (!bContinue)
			{
				return FALSE;
			}
		}
	}


	return CWnd::PreTranslateMessage(pMsg);
}

UINT ComboTree::OnGetDlgCode() 
{
	UINT result = CWnd::OnGetDlgCode();
	result |= DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTCHARS ;
	
	return result;
}


void ComboTree::OnSelection ()
{
	if ( m_Tree.GetSafeHwnd () )
	{
		HTREEITEM hItem = m_Tree.GetSelectedItem ();
		if (hItem)
		{
			CString Text;
			Text = m_Tree.GetItemText (hItem);
			SetWindowText(Text);
			SendParentComboMessage (CBN_SELENDOK);
//			TRACE ("ComboTree::OnSelection()\n");
		}
	}

	HideTree();
}

void ComboTree::OnCancel ()
{
	//restore inital path
	m_Tree.SelectString (m_BeginPath);
	SendParentComboMessage (CBN_SELENDCANCEL);
	HideTree();
}


