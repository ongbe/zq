#include "StdAfx.h"
#include "VirtualKeyboard.h"
#include "../inc/Module-Misc/constants.h"
#include "../inc/Module-Misc/GLobalFunc.h"
#include "../inc/cfgmgr/cfgmgr.h"
#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/dcmemory.h>
#include <wx/file.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include <algorithm>
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/gui/UserLoginDlg.h"
/*Notice*
* 虚拟键盘尺寸只能手动调整，当尺寸允许时，可任意添加多个按钮，并且这些按钮的位置是自动调整的。
* 获取密码框中信息时，要使用GetValue()，不要使用GetLabelText()，因为特殊字符可能会使结果不正确，
* 如输入"&="却返回"="
*/

const int VirtualKeyboardWidth = 335;
const int VirtualKeyboardHight = 185;
const wxString SPECIAL_CHAR = "&&";

void VirtualKeyboard::RandomList (wxChar * pList) 
{
	int len = strlen(pList);
	std::random_shuffle(pList, pList+len);
}

VirtualKeyboard::VirtualKeyboard(wxWindow *parent, wxTextCtrl* textctrl):
	wxDialog(parent, wxID_ANY,wxEmptyString,wxDefaultPosition, wxSize(VirtualKeyboardWidth,VirtualKeyboardHight),
	wxSTATIC_BORDER),	m_pPasswordTextCtrl(NULL),m_pParent(NULL),m_bIsCapslockButtonDown(false)
{
	DIALOG_CREATE_LOG();
	m_pParent = parent;
	m_pPasswordTextCtrl = textctrl;
    SetTitle(LOADSTRING(VIRTUAL_KEYBOARD));
	wxSize buttonSize = wxSize(25,25);


	wxChar numberlist[] = "1234567890";
	wxChar letterlist[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	wxChar otherlist[] = "`~!@#$%^*()&<>?-=/+_\\|,.[];:{}\"\'";

	RandomList(numberlist);
	RandomList(letterlist);
	RandomList(otherlist);

	wxString charlist(numberlist);
	charlist.Append(letterlist);
	charlist.Append(otherlist);

	const int gap = 1;
	const int xstep = buttonSize.GetWidth() + gap, ystep = buttonSize.GetWidth() + gap;
	const int nID = 100;
	int xpoint = gap;
	int ypoint = gap;

	const wxString special("&");

	for (size_t i = 0; i<charlist.Len(); ++i )
	{
		if (xpoint > VirtualKeyboardWidth-buttonSize.GetX()-gap)
		{
			xpoint = gap;
			ypoint += ystep;
		}

		if (ypoint > VirtualKeyboardHight-buttonSize.GetY()-gap)
		{
			break;
		}	
		
		wxString & item = charlist.SubString(i,i);

		if (special.CmpNoCase(item) == 0)
		{
			item = SPECIAL_CHAR;
		}


		VirtualKeyButton* itemButtonA = new VirtualKeyButton( this, nID+i, item, wxPoint(xpoint,ypoint), buttonSize, 0 );
		m_vecVirtualKeyButton.push_back(itemButtonA);
		itemButtonA->ChangeButtonBitmap(BUTTON_NORMAL);
		
		xpoint += xstep;

	}


	VirtualKeyButton * pButtonBackSpace = new VirtualKeyButton( this, ID_BACKSPACE,LOADSTRING(VK_BACKSPACE_TEXT), 
		wxPoint(xpoint,ypoint), wxSize(buttonSize.GetWidth()*2+gap,buttonSize.GetHeight()), 0 );
	m_vecVirtualKeyButton.push_back(pButtonBackSpace);
	pButtonBackSpace->ChangeButtonBitmap(BUTTON_NORMAL);

	VirtualKeyButton* pButtonClose = new VirtualKeyButton( this, ID_VIRTUAL_KEYBOARD_CLOSE,LOADSTRING(VK_ESC_TEXT), 
		wxPoint(xpoint + gap + pButtonBackSpace->GetSize().GetX(),ypoint), 
		wxSize(buttonSize.GetWidth()*2+gap,buttonSize.GetHeight()), 0 );
	m_vecVirtualKeyButton.push_back(pButtonClose);
	pButtonClose->ChangeButtonBitmap(BUTTON_NORMAL);

	xpoint = gap;
	ypoint += ystep;

	VirtualKeyButton * pButtonBlank = new VirtualKeyButton( this, ID_BLANK,LOADSTRING(VK_SPACE_TEXT), 
		wxPoint(xpoint,ypoint), wxSize(buttonSize.GetWidth()*8+gap*7,buttonSize.GetHeight()), 0 );
	m_vecVirtualKeyButton.push_back(pButtonBlank);
	pButtonBlank->ChangeButtonBitmap(BUTTON_NORMAL);

	VirtualKeyButton * pButtonCapslock = new VirtualKeyButton( this, ID_CAPSLOCK,LOADSTRING(VK_CAPS_TEXT), 
		wxPoint(xpoint + gap + pButtonBlank->GetSize().GetX(),ypoint), 
		wxSize(buttonSize.GetWidth()*4+gap*3,buttonSize.GetHeight()), 0 );
	pButtonCapslock->SetStyle(BUTTON_STYLE_LOCK);
	m_vecVirtualKeyButton.push_back(pButtonCapslock);
	pButtonCapslock->ChangeButtonBitmap(BUTTON_NORMAL);
	GET_JPG_FILE_RESOURCE("IDR_XML_KEYBG",_T("XML"),m_BackBitmap);
}

VirtualKeyboard::~VirtualKeyboard(void)
{
	std::vector<VirtualKeyButton*>::iterator iter = m_vecVirtualKeyButton.begin();
	for (; iter != m_vecVirtualKeyButton.end(); ++iter)
	{
		VirtualKeyButton* pItem = *iter;
		if (pItem)
		{
			pItem->Destroy();
		}
	}
}

BEGIN_EVENT_TABLE(VirtualKeyboard,wxDialog)
EVT_COMMAND_RANGE(ID_CHAR_1,ID_CHAR_68,wxEVT_COMMAND_BUTTON_CLICKED,VirtualKeyboard::OnVirtualButtonDown)
EVT_BUTTON(ID_BACKSPACE,VirtualKeyboard::OnBackspace)
EVT_BUTTON(ID_VIRTUAL_KEYBOARD_CLOSE,VirtualKeyboard::OnClose)
EVT_BUTTON(ID_BLANK,VirtualKeyboard::OnBlank)
EVT_BUTTON(ID_CAPSLOCK,VirtualKeyboard::OnCapsLock)
EVT_MOTION(VirtualKeyboard::OnMotion)
EVT_LEFT_DOWN(VirtualKeyboard::OnLeftDown)
EVT_LEFT_UP(VirtualKeyboard::OnLeftUp)
EVT_PAINT(VirtualKeyboard::OnPaint)
EVT_ERASE_BACKGROUND(VirtualKeyboard::OnEraseBackground)
END_EVENT_TABLE()


void VirtualKeyboard::OnVirtualButtonDown(wxCommandEvent& event)
{
	if (m_pPasswordTextCtrl)
	{
		VirtualKeyButton * pButton = NULL;
		pButton = (static_cast<VirtualKeyButton*>(GetWindowChild(event.GetId())));
		if (pButton)
		{
			//大写
			if (m_bIsCapslockButtonDown)
			{
				m_pPasswordTextCtrl->WriteText( (pButton->GetLabelText()).MakeUpper() );
			}
			//小写
			else
			{
				m_pPasswordTextCtrl->WriteText( (pButton->GetLabelText()).MakeLower() );
			}
		}
	}
	
}

// void VirtualKeyboard::ChangeButtonBackground(int nID, int flag)
// {
// 	wxWindow *pVerify = GetWindowChild(nID);
// 	(static_cast<VirtualKeyButton*>(pVerify))->ChangeButtonBitmap(flag);
// 
// }

void VirtualKeyboard::OnBackspace(wxCommandEvent& event)
{
	if (m_pPasswordTextCtrl)
	{
		wxString str = m_pPasswordTextCtrl->GetValue();
		str = str.Left(str.Len()-1);
		m_pPasswordTextCtrl->Clear();
		m_pPasswordTextCtrl->WriteText( str );
	}

}

void VirtualKeyboard::OnClose(wxCommandEvent& event)
{
	//EndModal(0);
	//Show(false);


	if (m_pParent)
	{
		wxWindow *pVerify = m_pParent->GetWindowChild(UserLoginDlg::ID_VERIFY);
		if (pVerify != NULL)
			pVerify->SetFocus();
	}

}

void VirtualKeyboard::OnMotion(wxMouseEvent& evt)
{
	if (evt.Dragging() && evt.LeftIsDown())
	{
		wxPoint pt = ClientToScreen(evt.GetPosition());
		int x = pt.x - m_MouseMoveDelta.x;
		int y = pt.y - m_MouseMoveDelta.y;
		Move(x, y);
	}

}

void VirtualKeyboard::OnLeftDown(wxMouseEvent& evt)
{
	CaptureMouse();
	wxPoint pt = ClientToScreen(evt.GetPosition());
	wxPoint origin = GetPosition();
	int dx = pt.x - origin.x;
	int dy = pt.y - origin.y;
	m_MouseMoveDelta = wxPoint(dx, dy);

}

void VirtualKeyboard::OnLeftUp(wxMouseEvent& evt)
{
	if (HasCapture())
	{
		ReleaseMouse();
	}

}

void VirtualKeyboard::OnPaint(wxPaintEvent& event)
{
	if(m_BackBitmap.IsOk())
	{
		wxPaintDC dc(this);
		dc.DrawBitmap(m_BackBitmap,0,0);
	}
 	else
	{
  		event.Skip();
	}
}

void VirtualKeyboard::OnEraseBackground(wxEraseEvent& event)
{

}

void VirtualKeyboard::OnBlank(wxCommandEvent& event)
{
	if (m_pPasswordTextCtrl)
		m_pPasswordTextCtrl->WriteText( _(" ") );
}

void VirtualKeyboard::OnCapsLock(wxCommandEvent& event)
{
	m_bIsCapslockButtonDown = !m_bIsCapslockButtonDown;
}