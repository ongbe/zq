#include "stdafx.h"
#include "MainTaskBarIcon.h"
MainTaskBarIcon::MainTaskBarIcon(wxFrame* parent):
m_parent(parent)
{
}
MainTaskBarIcon::~MainTaskBarIcon()
{
}

BEGIN_EVENT_TABLE(MainTaskBarIcon, wxTaskBarIcon)
    EVT_TASKBAR_LEFT_DOWN(MainTaskBarIcon::OnClick)
    EVT_TASKBAR_RIGHT_DOWN(MainTaskBarIcon::OnClick)
END_EVENT_TABLE()

void MainTaskBarIcon::OnClick(wxTaskBarIconEvent&)
{
    m_parent->Show(true);
	m_parent->Restore();
	RemoveIcon();
}
