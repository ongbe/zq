#include "StdAfx.h"
#include "TradingNoticeDlg.h"
#include "../inc/Module-Misc2/SimpleWriteLog.h"
#define ID_TEXTCTRL 540
#define ID_PREVITEMBUTTON 541
#define ID_NEXTITEMBUTTON 542
#define ID_OKBUTTON 543


BEGIN_EVENT_TABLE(TradingNoticeDlg,wxDialog)
	EVT_CLOSE(TradingNoticeDlg::OnClose)
	EVT_BUTTON(ID_PREVITEMBUTTON,TradingNoticeDlg::OnBtnPrev)
	EVT_BUTTON(ID_NEXTITEMBUTTON,TradingNoticeDlg::OnBtnNext)
	EVT_SIZE(TradingNoticeDlg::OnSize)
END_EVENT_TABLE()


TradingNoticeDlg::TradingNoticeDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(TRADING_NOTICE_TITLE),wxDefaultPosition, wxSize(700,400))
{ 
	m_current = 0;

	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();
    SetBackgroundColour(DEFAULT_COLOUR);
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    SetSizer(itemBoxSizer2);

    m_itemTextCtrl= new wxTextCtrl( this, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(680,320), wxTE_MULTILINE );
    itemBoxSizer2->Add(m_itemTextCtrl, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    m_itemTextCtrl->SetFont(wxFont(14, wxDEFAULT ,wxNORMAL, wxNORMAL, FALSE,"Fixedsys"));
    wxButton* itemButton1 = new wxButton( this, ID_PREVITEMBUTTON, LOADSTRING(ID_INFODLG_PREV), wxPoint(10, 336), wxSize(70, 24), 0 );
    wxButton* itemButton2 = new wxButton( this, ID_NEXTITEMBUTTON, LOADSTRING(ID_INFODLG_NEXT), wxPoint(90, 336), wxSize(70, 24), 0 );
    wxButton* itemButton4 = new wxButton( this, wxID_OK, LOADSTRING(ASKNAMEDLG_OK), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton4, 0, wxALIGN_RIGHT|wxALL, 5);
    itemButton4->SetFocus();
}

TradingNoticeDlg::~TradingNoticeDlg()
{
}

void TradingNoticeDlg::SetContent(wxString content)
{
	m_vecContent.push_back(string(content.c_str()));
	m_current = m_vecContent.size()-1;
	Reload();
}

void TradingNoticeDlg::OnClose(wxCloseEvent& /*event*/)
{
	m_vecContent.clear();
    Hide();
}

void TradingNoticeDlg::Reload()
{
    int count=m_vecContent.size();
    wxString strAllTitle;
    strAllTitle.Printf("(%d/%d)", m_current+1, count);
    m_itemTextCtrl->SetLabel(m_vecContent[m_current]);
    FindWindowById(ID_PREVITEMBUTTON,this)->Show(m_current!=0);
    FindWindowById(ID_NEXTITEMBUTTON,this)->Show(m_current!=count-1);
}

void TradingNoticeDlg::OnBtnNext(wxCommandEvent& event)
{
    m_current++;
    Reload();
}

void TradingNoticeDlg::OnBtnPrev(wxCommandEvent& event)
{
    m_current--;
    Reload();
}
