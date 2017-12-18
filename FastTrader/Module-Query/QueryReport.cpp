#include "stdafx.h"
#include "../inc/ISvr/PlatformSvrMgr.h"
#include "QueryReport.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(CQueryReportDlg,wxDialog)
	EVT_COMBOBOX(ID_COMBOX, OnReportTypeChanged)
	EVT_SIZE(OnSize)
END_EVENT_TABLE()

CQueryReportDlg::CQueryReportDlg(wxWindow *parent)
:wxDialog(parent, wxID_ANY,LOADSTRING(ID_QUERY_REPORT),wxDefaultPosition, wxSize(1000,600),wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER)
{
	m_mapReport.clear();
    CreateGUIControls();
}

CQueryReportDlg::~CQueryReportDlg()
{
   map<DWORD, CTPReport>::iterator it = m_mapReport.begin();
   for ( ; it != m_mapReport.end(); ++it )
   {
	   if ( it->second.pReport != NULL )
	   {
		   delete [] it->second.pReport;
		   it->second.pReport = NULL;
	   }
   }

   m_mapReport.clear();
}

void CQueryReportDlg::CreateGUIControls()
{
	new wxStaticText(this, wxID_ANY, LOADSTRING(ID_REPORT_TYPE), wxPoint(5, 10), wxSize(70, 20));
    m_pComReportType = new wxComboBox(this, ID_COMBOX, "", wxPoint(80, 10) ,wxSize(120, 20));
	wxRect rc = GetClientRect();
	m_textReport = new wxTextCtrl(this, ID_TEXT, "", wxPoint(5, 35), wxSize(rc.width-10, rc.height-40), wxTE_MULTILINE|wxTE_READONLY|wxVSCROLL);
    m_textReport->SetFont(wxFont(9, wxDEFAULT ,wxNORMAL, wxNORMAL, FALSE,"Fixedsys"));
	m_mapReport.clear();
	if ( DEFAULT_SVR()->GetReports(m_mapReport) < 0)
	{
		return;
	}

	map<DWORD, CTPReport>::reverse_iterator it = m_mapReport.rbegin();
	for ( ; it != m_mapReport.rend(); ++it )
	{
		m_pComReportType->AppendString( it->second.strReportType );
		if ( it == m_mapReport.rbegin() )
		{
			m_pComReportType->SetSelection(0);
			m_textReport->AppendText( (char*)(it->second.pReport) );
		}
	}
}

void CQueryReportDlg::OnSize( wxSizeEvent& evt )
{
	wxRect rc = GetClientRect();
	m_textReport->SetSize( wxSize(rc.width-10, rc.height-40) );
}

void CQueryReportDlg::OnReportTypeChanged( wxCommandEvent& event )
{
	m_textReport->Clear();
	string strType = m_pComReportType->GetValue().c_str();
	if ( strType.empty() || m_mapReport.empty())
	{
		return;
	}

	map<DWORD, CTPReport>::iterator it = m_mapReport.begin();
	for ( ; it != m_mapReport.end(); ++it )
	{
		if ( strType == it->second.strReportType )
		{
			m_textReport->AppendText( (char*)(it->second.pReport) );
		}
	}
}

