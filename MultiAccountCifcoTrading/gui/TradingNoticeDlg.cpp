#include "StdAfx.h"
#include "TradingNoticeDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#define ID_TEXTCTRL 540
TradingNoticeDlg::TradingNoticeDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(TRADING_NOTICE_TITLE),wxDefaultPosition, wxSize(700,400))
{ 
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();
    SetBackgroundColour(DEFAULT_COLOUR);
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    SetSizer(itemBoxSizer2);

    m_itemTextCtrl= new wxTextCtrl( this, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(680,320), wxTE_MULTILINE );
    itemBoxSizer2->Add(m_itemTextCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    m_itemTextCtrl->SetFont(wxFont(14, wxDEFAULT ,wxNORMAL, wxNORMAL, FALSE,"Fixedsys"));
    wxButton* itemButton4 = new wxButton( this, wxID_OK, LOADSTRING(ASKNAMEDLG_OK), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    itemButton4->SetFocus();
}
TradingNoticeDlg::~TradingNoticeDlg()
{
}
void TradingNoticeDlg::SetContent(wxString content)
{
    m_itemTextCtrl->SetLabel(content);
}
