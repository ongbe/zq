/////////////////////////////////////////////////////////////////////////////
// Name:        view.cpp
// Purpose:     View classes
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: view.cpp 35650 2005-09-23 12:56:45Z MR $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "StdAfx.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_DOC_VIEW_ARCHITECTURE
#error You must set wxUSE_DOC_VIEW_ARCHITECTURE to 1 in setup.h!
#endif

#include "docview.h"
#include "doc.h"
#include "view.h"

IMPLEMENT_DYNAMIC_CLASS(DrawingView, wxView)

// For drawing lines in a canvas
float xpos = -1;
float ypos = -1;

BEGIN_EVENT_TABLE(DrawingView, wxView)
    
END_EVENT_TABLE()

// What to do when a view is created. Creates actual
// windows for displaying the view.
bool DrawingView::OnCreate(wxDocument *doc, long WXUNUSED(flags) )
{
    frame = wxGetApp().CreateChildFrame(doc, this, true);
    frame->SetTitle(_T("DrawingView"));

    canvas = GetMainFrame()->CreateKFrame(this, frame);
#ifdef __X__
    // X seems to require a forced resize
    int x, y;
    frame->GetSize(&x, &y);
    frame->SetSize(wxDefaultCoord, wxDefaultCoord, x, y);
#endif
    frame->Show(true);
    Activate(true);

    return true;
}

// Sneakily gets used for default print/preview
// as well as drawing on the screen.
void DrawingView::OnDraw(wxDC *dc)
{
  dc->SetFont(*wxNORMAL_FONT);
  dc->SetPen(*wxBLACK_PEN);

  wxList::compatibility_iterator node = ((DrawingDocument *)GetDocument())->GetDoodleSegments().GetFirst();
  while (node)
  {
    DoodleSegment *seg = (DoodleSegment *)node->GetData();
    seg->Draw(dc);
    node = node->GetNext();
  }
}

void DrawingView::OnUpdate(wxView *WXUNUSED(sender), wxObject *WXUNUSED(hint))
{
  if (canvas)
    canvas->Refresh();
}

// Clean up windows used for displaying the view.
bool DrawingView::OnClose(bool deleteWindow)
{
  if (!GetDocument()->Close())
    return false;

  // Clear the canvas in  case we're in single-window mode,
  // and the canvas stays.
  canvas->ClearBackground();
//  canvas->view = (wxView *) NULL;
  canvas = (MyFrame *) NULL;

  wxString s(wxTheApp->GetAppName());
  if (frame)
    frame->SetTitle(s);

  SetFrame((wxFrame*)NULL);

  Activate(false);

  if (deleteWindow)
  {
    delete frame;
    return true;
  }
  return true;
}


