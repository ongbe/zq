#pragma once

#include <wx/button.h>

enum
{
	BUTTON_NORMAL = 1,
	BUTTON_PRESS,
	BUTTON_OVER
};

enum
{
	BUTTON_STYLE_LOCK=1
};

class VirtualKeyButton : public wxButton
{

public:

	VirtualKeyButton() {};

	VirtualKeyButton(wxWindow *parent,
		wxWindowID id,
		const wxString& text,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxValidator& validator = wxDefaultValidator,
		const wxString& name = wxButtonNameStr);

	virtual ~VirtualKeyButton();

	void ChangeButtonBitmap( int state );
	inline void SetStyle(int nStyle)
	{
		m_nButtonStyle = nStyle;
	}

	DECLARE_EVENT_TABLE()


protected:
	void OnMotion(wxMouseEvent& evt);
	void OnLeftDown(wxMouseEvent& evt);
	void OnLeftUp(wxMouseEvent& evt);
	void OnEnterWindow(wxMouseEvent& evt);
	void OnLeaveWindow(wxMouseEvent& evt);

private:
	void Change2Over();
	void Change2Normal();
	void Change2Press();

private:
	int m_nButtonStyle;
	bool m_bLocked;
};