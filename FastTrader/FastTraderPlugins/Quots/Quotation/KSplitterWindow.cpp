#include "stdafx.h"
#include "wx/splitter.h"
#include "../../../inc/Quotation/KSplitterWindow.h"
#include "wx/renderer.h"
// ----------------------------------------------------------------------------
// KSplitterWindow
// ----------------------------------------------------------------------------
DEFINE_EVENT_TYPE(wxEVT_KSplitter_Change)
BEGIN_EVENT_TABLE(KSplitterWindow, wxSplitterWindow)
EVT_SPLITTER_SASH_POS_CHANGED(wxID_ANY, KSplitterWindow::OnPositionChanged)

END_EVENT_TABLE()
KSplitterWindow::KSplitterWindow(wxWindow *parent, wxWindowID id,
								   const wxPoint& pos,
								   const wxSize& size,
								   long style)
{
	m_clBorder		= wxColour(RGB(192,0,0));
	m_fixed=(style&wxSP_NOSASH)>0;
	if(m_fixed)
	{
	//	style&=~wxSP_NOSASH;
	}
	Create(parent, id, pos, size, style|wxCLIP_CHILDREN|wxCLIP_SIBLINGS);
	SetMinimumPaneSize(20);
}
bool KSplitterWindow::OnSashPositionChange(int WXUNUSED(newSashPosition))
{
	return !m_fixed;
}
void KSplitterWindow::OnPositionChanged(wxSplitterEvent& event)
{
	//	wxLogStatus(m_frame, _T("Position has changed, now = %d (or %d)"),
	//		event.GetSashPosition(), GetSashPosition());
	wxWindow *pWindow1 = GetWindow1();
	if(pWindow1)
		pWindow1->Refresh(false);

	wxWindow *pWindow2 = GetWindow2();
	if(pWindow2)
		pWindow2->Refresh(false);
/*	消息暂时不发了，暂时上层也不需要这个消息
	wxWindow* window= GetParent();
	wxCommandEvent myEvent(wxEVT_KSplitter_Change);
	myEvent.SetClientData(this);
	window->ProcessEvent(myEvent);
*/
	event.Skip();
}
void KSplitterWindow::DrawSash(wxDC& dc)
{//重载该函数，需要注意里面有些内容没有重载过来
	const wxCoord SASH_WIDTH = 4;
	wxSize size = GetClientSize();
	wxOrientation orient = (m_splitMode == wxSPLIT_VERTICAL )? wxVERTICAL:wxHORIZONTAL;
	wxCoord position = m_sashPosition;
	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(wxBrush(wxColour(m_clBorder)));
	if ( orient == wxVERTICAL )
	{
		dc.DrawRectangle(position, 0, SASH_WIDTH, size.y);
	}
	else // wxHORIZONTAL
	{
		dc.DrawRectangle(0, position, size.x, SASH_WIDTH);
	}
}
