#include "StdAfx.h"
#include "wx/richtext/richtextctrl.h"
#include "../../../inc/Quotation/wxRichTextCtrlEx.h"






IMPLEMENT_DYNAMIC_CLASS(wxRichTextCtrlEx, wxRichTextCtrl)
DEFINE_EVENT_TYPE(wxEVT_RichTextCtrlEx_KeyDown)


BEGIN_EVENT_TABLE(wxRichTextCtrlEx, wxRichTextCtrl)
EVT_CHAR(wxRichTextCtrlEx::OnChar)
EVT_KEY_DOWN(wxRichTextCtrlEx::OnChar)
EVT_LEFT_DCLICK(wxRichTextCtrlEx::OnLeftDClick)
END_EVENT_TABLE()
wxRichTextCtrlEx::wxRichTextCtrlEx(wxWindow* parent,
							   wxWindowID id,
							   const wxString& value,
							   const wxPoint& pos,
							   const wxSize& size,
							   long style,
							   const wxValidator& validator,
							   const wxString& name):wxRichTextCtrl(parent, id, value, pos, size, style, validator, name)
							  
{
	
}
void wxRichTextCtrlEx::OnLeftDClick(wxMouseEvent& event)
{
//	SelectWord(GetCaretPosition()+1);
//	event.Skip();
}
void wxRichTextCtrlEx::OnChar(wxKeyEvent& event)
{
	long keycode = event.GetKeyCode();
	switch ( keycode )
	{
	case WXK_ESCAPE:
		{
			wxWindow* window= GetParent();
			wxCommandEvent myEvent(wxEVT_RichTextCtrlEx_KeyDown);
			myEvent.SetInt(keycode);
			window->ProcessEvent(myEvent);
		}
		break;
	default:break;
	}
	wxRichTextCtrl::OnChar(event);
}