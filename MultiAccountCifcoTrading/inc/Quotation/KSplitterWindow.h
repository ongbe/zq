#pragma once
#include "wx/splitter.h"
class KSplitterWindow: public wxSplitterWindow
{
public:
	KSplitterWindow(wxWindow *parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxSP_3D| wxNO_BORDER);
	bool OnSashPositionChange(int WXUNUSED(newSashPosition)); 
	bool m_fixed;
	wxColour  m_clBorder;               //±ß¿òÉ«
	void OnPositionChanged(wxSplitterEvent& event);
	virtual void DrawSash(wxDC& dc);

	DECLARE_EVENT_TABLE()
	DECLARE_NO_COPY_CLASS(KSplitterWindow)
};