#pragma once
#include <wx/taskbar.h> 
class MainTaskBarIcon: public wxTaskBarIcon
{
public:
	MainTaskBarIcon(wxFrame* parent);
	~MainTaskBarIcon();
public:
	void OnClick(wxTaskBarIconEvent&);
	DECLARE_EVENT_TABLE();
private:
	wxFrame* m_parent;
};