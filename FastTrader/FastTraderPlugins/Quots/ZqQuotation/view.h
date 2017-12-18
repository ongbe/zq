/////////////////////////////////////////////////////////////////////////////
// Name:        view.h
// Purpose:     View classes
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: view.h 35650 2005-09-23 12:56:45Z MR $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __VIEWSAMPLEH__
#define __VIEWSAMPLEH__

#include "wx/docview.h"

class DrawingView: public wxView
{
public:
    wxMDIChildFrame *frame;
  //  MyCanvas *canvas;
	MyFrame *canvas;
  
    DrawingView() { canvas = (MyFrame *) NULL; frame = (wxMDIChildFrame *) NULL; }
    ~DrawingView() {}

    bool OnCreate(wxDocument *doc, long flags);
    void OnDraw(wxDC *dc);
    void OnUpdate(wxView *sender, wxObject *hint = (wxObject *) NULL);
    bool OnClose(bool deleteWindow = true);

    void OnCut(wxCommandEvent& event);

private:
    DECLARE_DYNAMIC_CLASS(DrawingView)
    DECLARE_EVENT_TABLE()
};



#endif
