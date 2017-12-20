#include "StdAfx.h"
#include "PlusInMgrDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
using namespace std;

BEGIN_EVENT_TABLE(PlusInMgrDlg, wxDialog)
	EVT_BUTTON(wxID_OK, OnClose)
	EVT_BUTTON(ID_OPEN, OnAdd)
	EVT_BUTTON(ID_DEL, OnDel)
END_EVENT_TABLE()

PlusInMgrDlg::PlusInMgrDlg(wxWindow *parent, map<wxString, PLUGWIN2>& mapPlusIn):
	wxDialog(parent, wxID_ANY,wxEmptyString,wxDefaultPosition, wxSize(550,350),wxDEFAULT_DIALOG_STYLE)
{
	DIALOG_CREATE_LOG();
	SetTitle( LOADSTRING(GUI_PLUSIN_MANAGER) );
	new wxStaticText( this, wxID_ANY, LOADSTRING(GUI_STARTED_PLUSIN), wxPoint(20, 20),wxSize( 200, 20));
	m_pListCtrl = new wxExtListCtrl( this, ID_LIST, wxPoint(20, 50), wxSize(510, 220), 
		wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE );
	new wxButton( this, ID_OPEN, LOADSTRING(ASKNAMEDLG_ADD ), wxPoint(330, 280), wxSize(60, 25));
	new wxButton( this, ID_DEL, LOADSTRING(BTN_QP_BUTTONDELETE), wxPoint(400, 280), wxSize(60, 25));
	new wxButton( this, wxID_OK, LOADSTRING(BTN_CLOSE_TEXT), wxPoint(470, 280), wxSize(60, 25));

	m_pListCtrl->InsertColumn( 0, LOADSTRING(GUI_PLUSIN_NAME), wxLIST_FORMAT_LEFT, 200 );
	m_pListCtrl->InsertColumn( 1, LOADSTRING(GUI_PLUSIN_PATH), wxLIST_FORMAT_LEFT, 300 );
	map<wxString, PLUGWIN2>::iterator it = mapPlusIn.begin();
	for ( int i = 0; it != mapPlusIn.end(); ++it, ++i)
	{
		m_pListCtrl->InsertItem( i, it->first.c_str());
		m_pListCtrl->SetItem( i, 1, it->second.strPath.c_str() );
	}
}

PlusInMgrDlg::~PlusInMgrDlg(void)
{
}

void PlusInMgrDlg::OnAdd( wxCommandEvent& event )
{
	wxFileDialog dlg( this, LOADSTRING(GUI_CHOOSE_FILE), wxT(""), wxT(""), wxT("*.exe"), wxFD_OPEN );
	dlg.ShowModal();
	wxString strPath = dlg.GetPath();
	if ( !strPath.empty())
	{
		wxCommandEvent myEvent( wxEVT_OPEN_PLUSIN );
//		myEvent.SetString(strPath.c_str());
        if(1)
        {
            DWORD EvtParamID;
            std::string EvtParamStr(strPath.c_str());
            CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
            myEvent.SetInt((int)EvtParamID);
        }
		m_parent->AddPendingEvent( myEvent );
	}
}

void PlusInMgrDlg::OnDel( wxCommandEvent& event )
{
	int nCount = m_pListCtrl->GetItemCount();
	for ( int i = 0; i < nCount; i++ )
	{
		if( m_pListCtrl->IsSelectedItem( i ))
		{
			wxCommandEvent myEvent( wxEVT_CLOSE_PLUSIN );
//			myEvent.SetString( m_pListCtrl->GetItemText( i ).c_str());
            if(1)
            {
	            DWORD EvtParamID;
	            std::string EvtParamStr(m_pListCtrl->GetItemText( i ).c_str());
	            CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0);
                myEvent.SetInt((int)EvtParamID);
            }
			m_parent->AddPendingEvent( myEvent );
		}
	}
}

void PlusInMgrDlg::OnClose( wxCommandEvent& event )
{
	event.Skip();
}

void PlusInMgrDlg::AddPlusIn( const wxString& strAppID, const wxString& strPath )
{
	int nCount = m_pListCtrl->GetItemCount();
	m_pListCtrl->InsertItem( nCount, strAppID );
	m_pListCtrl->SetItem( nCount, 1, strPath );
}

void PlusInMgrDlg::DelPlusIn( const wxString& strAppID )
{
	int nCount = m_pListCtrl->GetItemCount();
	for ( int i = 0; i < nCount; i++ )
	{
		if( strAppID == m_pListCtrl->GetItemText( i ))
		{
			m_pListCtrl->DeleteItem( i );
			break;
		}
	}
}

