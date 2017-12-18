#pragma once
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include "wx/popupwin.h"
#include "wx/spinctrl.h"



//----------------------------------------------------------------------------
// SimpleTransientPopup
//----------------------------------------------------------------------------
class CMouseHandyInputPaneDigit : public wxDialog
{
public:
	enum { conMaxButtonNum = 18 };

	// IDs for the controls and the menu commands
	enum
	{
		ID_PopupButtonDigit = 1,
	};

public:
    CMouseHandyInputPaneDigit(wxWindow *parent, wxWindowID id,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxNO_BORDER,
             const wxString& name = wxDialogNameStr);
    virtual ~CMouseHandyInputPaneDigit();

    virtual bool Show( bool show = true );

    wxScrolledWindow* GetChild() { return m_panel; }
	void TransMouseHook(wxPoint mousePos);
	void SetOwnerUIID(int nOwnerUIID) {
		m_nOwnerUIID = nOwnerUIID;
	};
	int SetOwnerUIID() {
		return m_nOwnerUIID;
	};

private:
    void OnSize( wxSizeEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );
    void OnButton( wxCommandEvent& event );
	void OnTimer(wxTimerEvent& event);
private:
    DECLARE_CLASS(MouseHandyInputPaneDigit)
    DECLARE_EVENT_TABLE()

private:
	int					m_nOwnerUIID;
    wxScrolledWindow*	m_panel;
    wxButton*			m_button[conMaxButtonNum];
	int					m_nInputIndex;
	BOOL				m_bDotIsPress;
	char				m_strInputDigit[32];
	wxTimer m_timer;
};
