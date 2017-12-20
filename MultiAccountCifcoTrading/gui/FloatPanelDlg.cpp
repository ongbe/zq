#include "Stdafx.h"
#include "FloatPanelDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
BEGIN_EVENT_TABLE(CFloatPanelDlg,wxDialog)
    EVT_SIZE(OnSize)
END_EVENT_TABLE()
CFloatPanelDlg::CFloatPanelDlg(wxWindow* parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(TRADING_NOTICE_TITLE),wxDefaultPosition, wxSize(700,400),wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER)
,m_panel(NULL)
{
	DIALOG_CREATE_LOG();
}
void CFloatPanelDlg::SetPanel(wxWindow* pPanel)
{
    m_panel=pPanel;
    DoSize();
}
void CFloatPanelDlg::DoSize()
{ 
    wxSize sz=GetSize();
    if(m_panel)
        m_panel->SetSize(sz.GetWidth(), sz.GetHeight()-30);
}
void CFloatPanelDlg::OnSize(wxSizeEvent& evt)
{
    DoSize();
}