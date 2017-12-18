#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/popupwin.h"
#include "wx/spinctrl.h"

// IDs for the controls and the menu commands
enum
{
    ID_PopupButtonVolume = 1,
};


//----------------------------------------------------------------------------
// SimpleTransientPopup
//----------------------------------------------------------------------------
class MouseHandyInputPaneVolume: public wxPopupTransientWindow
{
private:
	enum { conMaxButtonNum = 9 };

public:
    MouseHandyInputPaneVolume( wxWindow *parent );
    virtual ~MouseHandyInputPaneVolume();

    // wxPopupTransientWindow virtual methods are all overridden to log them
    virtual void Popup(wxWindow *focus = NULL);
    virtual void OnDismiss();
    virtual bool ProcessLeftDown(wxMouseEvent& event);
    virtual bool Show( bool show = true );

    wxScrolledWindow* GetChild() { return m_panel; }

private:
    wxScrolledWindow	*m_panel;
    wxButton*			m_button[conMaxButtonNum];
	int					m_nVolumes[conMaxButtonNum];

private:
	void AsyncVolumes();

private:
    void OnSize( wxSizeEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );
    void OnButton( wxCommandEvent& event );

private:
    DECLARE_CLASS(MouseHandyInputPaneVolume)
    DECLARE_EVENT_TABLE()
};
