#include "stdafx.h"
#include "VirtualKeyButton.h"

const wxColor BACKGROUND_BOTTON_NORMAL = wxColor(214,214,214);
const wxColor FOREGROUND_BOTTON_NORMAL = wxColor(0,0,0);
const wxColor BACKGROUND_BOTTON_PRESS = wxColor(0,255,255);
const wxColor FOREGROUND_BOTTON_PRESS = wxColor(0,0,0);
const wxColor BACKGROUND_BOTTON_OVER = wxColor(255,255,0);
const wxColor FOREGROUND_BOTTON_OVER = wxColor(255,0,0);

VirtualKeyButton::VirtualKeyButton(wxWindow *parent, wxWindowID id, const wxString& text,const wxPoint& pos,
								   const wxSize& size, long style, const wxValidator& validator, const wxString& name)
{

	Create(parent, id, text, pos, size, style, validator, name);
	m_bLocked = false;
	m_nButtonStyle = -1;
}

VirtualKeyButton::~VirtualKeyButton()
{
}

BEGIN_EVENT_TABLE(VirtualKeyButton,wxButton)
EVT_MOTION(VirtualKeyButton::OnMotion)
EVT_LEFT_DOWN(VirtualKeyButton::OnLeftDown)
EVT_LEFT_UP(VirtualKeyButton::OnLeftUp)
EVT_ENTER_WINDOW(VirtualKeyButton::OnEnterWindow)
EVT_LEAVE_WINDOW(VirtualKeyButton::OnLeaveWindow)
END_EVENT_TABLE()


void VirtualKeyButton::ChangeButtonBitmap( int state )
{

	if (BUTTON_NORMAL == state)
	{
		Change2Normal();
	}
	else if (BUTTON_PRESS == state)
	{
		Change2Press();
	}
	else if (BUTTON_OVER == state)
	{
		Change2Over();
	}

}

void VirtualKeyButton::OnMotion(wxMouseEvent& evt)
{	
	evt.Skip();
}

void VirtualKeyButton::OnLeftDown(wxMouseEvent& evt)
{
	if (BUTTON_STYLE_LOCK == m_nButtonStyle)
	{
		m_bLocked = !m_bLocked;
	}

	Change2Press();
	evt.Skip();
}

void VirtualKeyButton::OnLeftUp(wxMouseEvent& evt)
{
	if (BUTTON_STYLE_LOCK == m_nButtonStyle && m_bLocked)
	{
		Change2Press();
	}
	else
	{
		Change2Normal();
	}


	evt.Skip();
}

void VirtualKeyButton::OnEnterWindow(wxMouseEvent& evt)
{
	if (!m_bLocked)
	{
		Change2Over();	
	}

	evt.Skip();
}

void VirtualKeyButton::OnLeaveWindow(wxMouseEvent& evt)
{
	if (!m_bLocked)
	{
		Change2Normal();
	}

	evt.Skip();
}


void VirtualKeyButton::Change2Over()
{
	SetBackgroundColour(BACKGROUND_BOTTON_OVER);
	SetForegroundColour(FOREGROUND_BOTTON_OVER);
}

void VirtualKeyButton::Change2Normal()
{
	SetBackgroundColour(BACKGROUND_BOTTON_NORMAL);
	SetForegroundColour(FOREGROUND_BOTTON_NORMAL);
}

void VirtualKeyButton::Change2Press()
{
	SetBackgroundColour(BACKGROUND_BOTTON_PRESS);
	SetForegroundColour(FOREGROUND_BOTTON_PRESS);
}

