#include "stdafx.h"
#include "CNumericTextCtrl.h"



CNumericTextCtrl::CNumericTextCtrl()
{

}

CNumericTextCtrl::CNumericTextCtrl(wxWindow *parent,
								 wxWindowID id, 
								 const wxString &value, 
								 const wxPoint &pos, 
								 const wxSize &size, 
								 long style, 
								 const wxValidator &validator, 
								 const wxString &name)
	 :wxTextCtrl(parent, id, value, pos, size, style, validator, name),m_pCharSet(NULL)
{
	Connect(wxEVT_CHAR,wxKeyEventHandler(CNumericTextCtrl::OnChar),NULL,this);
}

CNumericTextCtrl::~CNumericTextCtrl()
{

}

void CNumericTextCtrl::OnChar(wxKeyEvent& event)
{
	const int keyCode = event.GetKeyCode();

	if (!m_pCharSet)
	{
		if (keyCode >= '0' && keyCode <= '9')
		{
			// Accept a number
			event.Skip();
		}
	}
	else
	{
		for (unsigned int i=0; i<strlen(m_pCharSet); ++i)
		{
			if ((int)(m_pCharSet[i]) == keyCode)
				event.Skip();
		}

		
	}


	// Accept some other keys
	switch (keyCode)
	{
	case WXK_BACK :
	case WXK_LEFT :
	case WXK_RIGHT :
	case WXK_TAB :
	case WXK_DELETE :
	case WXK_END :
	case WXK_HOME :
		event.Skip();
		break;
	}
}