#include "StdAfx.h"

#include "wx/tooltip.h"
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/Module-Misc/EventSet.h"
#include "../inc/Module-Misc/GLobalFunc.h"
#include "../inc/cfgMgr/CfgMgr.h"
#include "PlusInMgrDlg.h"
using namespace std;

BEGIN_EVENT_TABLE(PlusInMgrDlg, wxDialog)
	EVT_BUTTON(wxID_OK, OnClose)
	EVT_BUTTON(ID_OPEN, OnAdd)
	EVT_BUTTON(ID_DEL, OnDel)
	EVT_LIST_EXT_ITEM_SELECTED(ID_LIST, OnItemSelected)
END_EVENT_TABLE()

PlusInMgrDlg::PlusInMgrDlg(wxWindow *parent, 
						   map<wxString, PLUGWIN2>& mapPlusIn, 
						   CPlusin& rPlusin) :
						   wxDialog(parent, wxID_ANY,
							   wxEmptyString,wxDefaultPosition, 
							   wxSize(550,350),wxDEFAULT_DIALOG_STYLE)
							, m_rPlusin(rPlusin),
							m_pListCtrl(NULL)
{
	DIALOG_CREATE_LOG();
	SetTitle( LOADSTRING(GUI_PLUSIN_MANAGER) );
	new wxStaticText( this, wxID_ANY, LOADSTRING(GUI_REGISRTERED_PLUSIN), wxPoint(20, 20),wxSize( 200, 20));
	m_pListCtrl = new wxExtListCtrl( this, ID_LIST, wxPoint(20, 50), wxSize(510, 190), 
		wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE );

	m_pStaticText = new wxStaticText( this, wxID_ANY, "", wxPoint(20, 250),wxSize( 510, 20));

	new wxButton( this, ID_OPEN, LOADSTRING(GUI_PLUSIN_REGISTER ), wxPoint(330, 280), wxSize(60, 25));
	new wxButton( this, ID_DEL, LOADSTRING(GUI_PLUSIN_UNREGISTER), wxPoint(400, 280), wxSize(60, 25));
	new wxButton( this, wxID_OK, LOADSTRING(BTN_CLOSE_TEXT), wxPoint(470, 280), wxSize(60, 25));
	m_pListCtrl->InsertColumn( 0, LOADSTRING(GUI_PLUSIN_NAME), wxLIST_FORMAT_LEFT, 100 );
	m_pListCtrl->InsertColumn( 1, LOADSTRING(GUI_PLUSIN_PATH), wxLIST_FORMAT_LEFT, 250 );
	m_pListCtrl->InsertColumn( 2, LOADSTRING(GUI_PLUSIN_STATUS), wxLIST_FORMAT_LEFT, 150 );
	map<wxString, PLUGWIN2>::iterator it = mapPlusIn.begin();
	for ( int i = 0; it != mapPlusIn.end(); ++it, ++i)
	{
		m_pListCtrl->InsertItem( i, it->first.c_str());
		m_pListCtrl->SetItem( i, 1, it->second.strPath.c_str() );
		if ( it->second.bStarted )
		{
			m_pListCtrl->SetItem( i, 2, LOADSTRING(GUI_PLUSIN_STATUS_STARTED));
		}
		else
		{
			m_pListCtrl->SetItem( i, 2, LOADSTRING(GUI_PLUSIN_STATUS_NOT_STARTED));
		}
		m_pListCtrl->SetToolTip(it->second.strDesc);
	}
}

PlusInMgrDlg::~PlusInMgrDlg(void)
{
}

void PlusInMgrDlg::OnAdd( wxCommandEvent& event )
{
	std::string strPlusinPath = "plusin";
	GlobalFunc::GetPath(strPlusinPath);
	wxFileDialog dlg( this, LOADSTRING(GUI_CHOOSE_FILE), strPlusinPath.c_str(), wxT(""), wxT("*.exe"), wxFD_OPEN );
	dlg.ShowModal();
	wxString strPath = dlg.GetPath();
	if ( !strPath.empty())
	{
		//读取同名配置信息
		m_rPlusin.RegisterPlusin( strPath, true );
	}
}

void PlusInMgrDlg::OnDel( wxCommandEvent& event )
{
	int nCount = m_pListCtrl->GetItemCount();
	for ( int i = 0; i < nCount; i++ )
	{
		if( m_pListCtrl->IsSelectedItem( i ))
		{
			wxString strAppID = m_pListCtrl->GetSubItemText(i, 0);
			if(m_rPlusin.UnRegisterPlusin( strAppID ))
			{
				m_pListCtrl->DeleteItem( i );
				nCount--;
				i--;
			}
			else
			{
				wxMessageBox( LOADSTRING(GUI_PLUSIN_CANNOT_DEL_SYS_PLUSIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
			}
		}
	}
}

void PlusInMgrDlg::OnClose( wxCommandEvent& event )
{
	event.Skip();
}

void PlusInMgrDlg::UpdatePlusIn( const wxString& strAppID, const PLUGWIN2& plusin )
{
	if(m_pListCtrl)
	{
		int nCount = m_pListCtrl->GetItemCount();
		bool bExsit = false;
		//已经存在该记录，刷新
		for ( int i = 0; i < nCount; i++ )
		{
			if ( strAppID == m_pListCtrl->GetItemText(i))
			{
				bExsit = true;
				if ( plusin.bStarted )
				{
					m_pListCtrl->SetItem( i, 2, LOADSTRING(GUI_PLUSIN_STATUS_STARTED));
				}
				else
				{
					m_pListCtrl->SetItem( i, 2, LOADSTRING(GUI_PLUSIN_STATUS_NOT_STARTED));
				}
			}
		}

		//不存在该记录，添加
		if ( !bExsit )
		{
			m_pListCtrl->InsertItem( nCount, strAppID );
			m_pListCtrl->SetItem( nCount, 1, plusin.strPath );
			if ( plusin.bStarted )
			{
				m_pListCtrl->SetItem( nCount, 2, LOADSTRING(GUI_PLUSIN_STATUS_STARTED));
			}
			else
			{
				m_pListCtrl->SetItem( nCount, 2, LOADSTRING(GUI_PLUSIN_STATUS_NOT_STARTED));
			}
			m_pListCtrl->SetToolTip(plusin.strDesc);
		}
	}
}

void PlusInMgrDlg::DelPlusIn( const wxString& strAppID )
{
	if(m_pListCtrl)
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
}

void PlusInMgrDlg::OnItemSelected( wxExtListEvent& event )
{
	wxString strAppID = event.GetItem().GetText();
	PLUGWIN2 plusin;
	if( m_rPlusin.GetPlusinByAppID(strAppID, plusin))
	{
		m_pStaticText->SetLabel(plusin.strSummary.c_str());
	}
}

