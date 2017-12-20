#include "StdAfx.h"
#include "zqControl.h"
#include "wx/renderer.h"
#include "EventSet.h"
#include "GlobalDefines.h"
#include "constants.h"
BOOL zqPopupMenu(wxMenu *menu, int x, int y,wxWindow* owner)
{
    menu->SetInvokingWindow(owner);
    menu->UpdateUI();

    if ( x == wxDefaultCoord && y == wxDefaultCoord )
    {
		wxPoint mouse = owner->ScreenToClient(wxGetMousePosition());
        x = mouse.x; y = mouse.y;
    }

    HMENU hMenu = GetHmenuOf(menu);

    ///wxCurrentPopupMenu = menu;
#if defined(__WXWINCE__)
    static const UINT flags = 0;
#else // !__WXWINCE__
    UINT flags = TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY;
    // NT4 doesn't support TPM_RECURSE and simply doesn't show the menu at all
    // when it's use, I'm not sure about Win95/98 but prefer to err on the safe
    // side and not to use it there neither -- modify the test if it does work
    // on these systems
    if ( wxGetWinVersion() >= wxWinVersion_5 )
    {
        // using TPM_RECURSE allows us to show a popup menu while another menu
        // is opened which can be useful and is supported by the other
        // platforms, so allow it under Windows too
        flags |= TPM_RECURSE;
    }
#endif // __WXWINCE__/!__WXWINCE__

    BOOL retcmd=::TrackPopupMenu(hMenu, flags, x, y, 0, GetHwndOf(owner), NULL);

    // we need to do it right now as otherwise the events are never going to be
    // sent to wxCurrentPopupMenu from HandleCommand()
    //
    // note that even eliminating (ugly) wxCurrentPopupMenu global wouldn't
    // help and we'd still need wxYieldForCommandsOnly() as the menu may be
    // destroyed as soon as we return (it can be a local variable in the caller
    // for example) and so we do need to process the event immediately
    //wxYieldForCommandsOnly();

    //wxCurrentPopupMenu = NULL;

    menu->SetInvokingWindow(NULL);

    return retcmd;
}

IMPLEMENT_DYNAMIC_CLASS(zqStaticText, wxStaticText)

BEGIN_EVENT_TABLE (zqStaticText, wxStaticText)
    EVT_ERASE_BACKGROUND (zqStaticText::OnEraseBackground)
	EVT_LEFT_UP(zqStaticText::OnLeftUp)
    EVT_PAINT(zqStaticText::OnPaint)
END_EVENT_TABLE()

//-------------------------------------------------------------------------
zqStaticText::zqStaticText ()
{
}

bool zqStaticText::Create(wxWindow * ParentWindow,
                                     wxWindowID ControlIdent,
                                     const wxString& LabelString,
                                     const wxPoint& Position,
                                     const wxSize& Size,
                                     long Style,
                                     const wxString& Name)
{
    bool IsOk = wxStaticText::Create (ParentWindow, ControlIdent, LabelString,
                                      Position, Size, (Style|wxTRANSPARENT_WINDOW), Name);
	m_AlignRight=((Style&wxALIGN_MASK)==wxALIGN_RIGHT);
	m_pCheckBox = NULL;

    return IsOk;
}
void zqStaticText::OnEraseBackground (wxEraseEvent & WXUNUSED(EraseEvent))
{
}

void zqStaticText::OnLeftUp(wxMouseEvent& event)
{
	if (m_pCheckBox&&m_pCheckBox->IsEnabled())
	{
		wxCheckBox * p = static_cast<wxCheckBox*>(m_pCheckBox);
		if (p->IsChecked())
		{
			(static_cast<wxCheckBox*>(m_pCheckBox))->SetValue(false);
		}
		else
		{
			(static_cast<wxCheckBox*>(m_pCheckBox))->SetValue(true);

		}
		wxCommandEvent evt(wxEVT_COMMAND_CHECKBOX_CLICKED,m_pCheckBox->GetId());
		m_pCheckBox->ProcessEvent(evt);	
	}
}

void zqStaticText::OnPaint (wxPaintEvent & PaintEvent)
{
    wxPaintDC PaintWxDC (this);

    wxRect WindowRect = GetClientRect();

    // WARN required to set these values back?
    PaintWxDC.SetBackgroundMode (wxTRANSPARENT);
    PaintWxDC.SetTextForeground (GetForegroundColour ());
    PaintWxDC.SetTextBackground (GetBackgroundColour ());
    PaintWxDC.SetFont (GetFont ());

    wxString WindowTextString = GetLabel();

    // const char * WindowTextCharPtr = WindowTextString.c_str();
    int CharCount = wxStrlen (WindowTextString);

    // GetPixel returns a COLORREF in Win32
    COLORREF ForegroundColorRef = GetForegroundColour().GetPixel();

    HDC PaintDC = (HDC) PaintWxDC.GetHDC();
    COLORREF PreviousForegroundColorRef = SetTextColor (PaintDC, ForegroundColorRef);
    int PreviousBkMode = SetBkMode (PaintDC, TRANSPARENT);
 
    RECT Rect = {WindowRect.GetX(), WindowRect.GetY(), 
                 WindowRect.GetWidth(), WindowRect.GetHeight() };
	
    ::DrawText (PaintDC, WindowTextString,
		CharCount, & Rect, m_AlignRight?DT_RIGHT:0 );

    SetBkMode (PaintDC, PreviousBkMode);
    SetTextColor (PaintDC, PreviousForegroundColorRef);

}

IMPLEMENT_DYNAMIC_CLASS(ToggleStaticText, wxStaticText)
BEGIN_EVENT_TABLE (ToggleStaticText, wxStaticText)
   EVT_LEFT_UP(ToggleStaticText::OnLeftUp)
END_EVENT_TABLE()
void ToggleStaticText::OnLeftUp(wxMouseEvent& event)
{
   SetNormal(!m_normal);
   wxCommandEvent myEvent(wxEVT_STATIC_LEFT_UP,GetId());
   GetParent()->ProcessEvent(myEvent);
}
bool ToggleStaticText::IsNormal()
{
    return m_normal;
}
void ToggleStaticText::GetLabel2(wxString& label1,wxString& label2)
{
	label1=m_label1;
	label2=m_label2;
}
void ToggleStaticText::SetLabel2(const wxString& label1,const wxString& label2)
{
	m_label1=label1;
	m_label2=label2;
}
void ToggleStaticText::SetNormal(bool status)
{
   m_normal=status;
   SetLabel(m_normal?m_label1:m_label2);
   SetForegroundColour(m_normal?(*wxBLACK):(*wxRED));
   Refresh();
}
zqRadioButton::zqRadioButton()
{

}
zqRadioButton::zqRadioButton(wxWindow *parent,
                  wxWindowID id,
                  const wxString& label,
                  const wxPoint& pos ,
                  const wxSize& size ,
                  long style ,
                  const wxValidator& validator ,
                  const wxString& name ):
wxControl(parent,id,pos,size, (style|wxTRANSPARENT_WINDOW),validator,name)
{
    m_label=label;
    m_checked=false;
}
IMPLEMENT_DYNAMIC_CLASS(zqRadioButton, wxControl)
BEGIN_EVENT_TABLE (zqRadioButton, wxControl)
    EVT_ERASE_BACKGROUND (zqRadioButton::OnEraseBackground)
    EVT_PAINT(zqRadioButton::OnPaint)
    EVT_LEFT_UP(zqRadioButton::OnLeftUp)
END_EVENT_TABLE()
void zqRadioButton::OnLeftUp(wxMouseEvent& evt)
{
    if(!m_checked)
    {
        m_checked=true;

        const wxWindowList& siblings = GetParent()->GetChildren();
        wxWindowList::compatibility_iterator nodeThis = siblings.Find(this);
        for ( wxWindowList::compatibility_iterator nodeAfter = nodeThis->GetNext();
            nodeAfter;
            nodeAfter = nodeAfter->GetNext())
        {
            zqRadioButton *btn = wxDynamicCast(nodeAfter->GetData(),
                zqRadioButton);
            if ( !btn )
                continue;
            btn->SetValue(false);
        }
        for ( wxWindowList::compatibility_iterator nodeBefore = nodeThis->GetPrevious();
              nodeBefore;
              nodeBefore = nodeBefore->GetPrevious() )
        {
            zqRadioButton *btn = wxDynamicCast(nodeBefore->GetData(),
                                               zqRadioButton);
            if ( !btn )
                continue;
            btn->SetValue(false);
        }
        GetParent()->Refresh();
    }



}
void zqRadioButton::OnEraseBackground (wxEraseEvent & WXUNUSED(EraseEvent))
{
}
void zqRadioButton::SetValue(bool val)
{
    m_checked=val;
}
bool zqRadioButton::GetValue() const
{
    return m_checked;
}
void zqRadioButton::SetLabel(wxString val)
{
    m_label=val;
}
wxString zqRadioButton::Getlabel() const
{
    return m_label;
}
void zqRadioButton::OnPaint (wxPaintEvent & PaintEvent)
{
    wxPaintDC PaintWxDC (this); 
    wxRect WindowRect = GetClientRect(); 
    if(m_checked)
    {
       PaintWxDC.GradientFillLinear(WindowRect,wxColour(249,231,218),wxColour(249,231,218));
    }
    wxRenderer_DrawRadioButton(this,PaintWxDC,wxRect(1,3,13,13),m_checked?wxCONTROL_CHECKED:0);


    WindowRect.x+=15;
    WindowRect.width-=15;
    //if (m_TransparentStaticTextFlags & IsOpaque)
    //  {
    //    PaintWxDC.SetBrush (wxBrush (GetBackgroundColour(), wxSOLID));
    //    PaintWxDC.SetPen (* wxTRANSPARENT_PEN);
    //    PaintWxDC.DrawRectangle (WindowRect);
    //  }

    // WARN required to set these values back?
    PaintWxDC.SetBackgroundMode (wxTRANSPARENT);
    PaintWxDC.SetTextForeground (GetForegroundColour ());
    PaintWxDC.SetTextBackground (GetBackgroundColour ());
    PaintWxDC.SetFont (GetFont ());

    wxString WindowTextString = m_label;

    // const char * WindowTextCharPtr = WindowTextString.c_str();
    int CharCount = wxStrlen (WindowTextString);

    // GetPixel returns a COLORREF in Win32
    COLORREF ForegroundColorRef = GetForegroundColour().GetPixel();

    HDC PaintDC = (HDC) PaintWxDC.GetHDC();
    COLORREF PreviousForegroundColorRef = SetTextColor (PaintDC, ForegroundColorRef);
    int PreviousBkMode = SetBkMode (PaintDC, TRANSPARENT);
 
    RECT Rect = {WindowRect.GetX(), WindowRect.GetY(), 
                 WindowRect.GetWidth(), WindowRect.GetHeight() };

    ::DrawText (PaintDC, WindowTextString,
                CharCount, & Rect, DT_WORDBREAK/*DT_SINGLELINE | DT_CENTER */);

    SetBkMode (PaintDC, PreviousBkMode);
    SetTextColor (PaintDC, PreviousForegroundColorRef);
}
zqNCButton::zqNCButton(int id,wxString text,bool checked,int offset,bool IsCheckbox):
m_id(id),m_strText(text),m_checked(checked),m_width(offset),
m_IsCheckbox(IsCheckbox),m_bitmap_normal(NULL),m_bitmap_press(NULL)
{

}
zqNCButton::~zqNCButton()
{
	wxDELETE(m_bitmap_normal);
	wxDELETE(m_bitmap_press);
}
wxString zqNCButton::GetText()
{
	return m_strText;
}
void zqNCButton::SetBitmap(wxBitmap* bitmap1,wxBitmap* bitmap2)
{
	m_bitmap_normal=bitmap1;
	m_bitmap_press=bitmap2;
}
zqMenu::zqMenu(const wxString& title, long style):
wxMenu(title,style)
{
}
zqMenu::zqMenu(long style):
wxMenu(style)
{
}
wxMenuItem* zqMenu::Append(int itemid,
                       const wxString& text ,
                       const wxString& help ,
                       wxItemKind kind)
{
	return wxMenu::Append(itemid,text,help,kind);
}