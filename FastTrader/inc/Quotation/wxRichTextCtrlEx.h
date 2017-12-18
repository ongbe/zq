#pragma once
#include "wx\msw\dialog.h"
#include "KSplitterWindow.h"
#include <string>



DECLARE_EVENT_TYPE(wxEVT_RichTextCtrlEx_KeyDown, 9900)

class wxRichTextCtrlEx :public wxRichTextCtrl
{
public:
	wxRichTextCtrlEx(){};
	wxRichTextCtrlEx( wxWindow* parent, wxWindowID id = -1, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		   long style = wxRE_MULTILINE, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxTextCtrlNameStr);
	DECLARE_DYNAMIC_CLASS (wxRichTextCtrlEx)
	DECLARE_EVENT_TABLE()
	void OnChar(wxKeyEvent& event);
	void OnLeftDClick(wxMouseEvent& event);

	

};
