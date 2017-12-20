#include "stdafx.h"
#include "zq.h"
#include "AskNameDlg.h"
#include "../inc/Module-Misc/ImportExport.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
BEGIN_EVENT_TABLE(AskNameDlg,wxDialog)
EVT_BUTTON(ID_ASKNAMEDEL,OnDel)
EVT_BUTTON(wxID_OK,OnOK)
END_EVENT_TABLE()
AskNameDlg::AskNameDlg(wxWindow* parent,CImportExport* pImportExport):
wxDialog(parent,wxID_ANY,LOADSTRING(ASKNAMEDLG_TITLE),wxDefaultPosition,wxSize(400,120))
,m_pImportExport(pImportExport)
{
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();
	wxArrayString ComboBoxStrings;
	m_pImportExport->GetLayoutArray(ComboBoxStrings);

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( this, wxID_STATIC,LOADSTRING(ASKNAMEDLG_STATIC), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_cmbName= new wxComboBox( this, ID_ASKNAMECHOICE, wxEmptyString, wxDefaultPosition, wxSize(150,20), ComboBoxStrings, wxCB_DROPDOWN|wxST_NO_AUTORESIZE );
    itemBoxSizer3->Add(m_cmbName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton6 = new wxButton( this, ID_ASKNAMEDEL, LOADSTRING(ASKNAMEDLG_DEL), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton8 = new wxButton( this, wxID_OK,LOADSTRING(ASKNAMEDLG_OK), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton( this, wxID_CANCEL,LOADSTRING(ASKNAMEDLG_CANCEL), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	itemButton8->SetDefault();
}
AskNameDlg::~AskNameDlg()
{

}
void AskNameDlg::OnDel(wxCommandEvent& event)
{
    int cur=m_cmbName->GetCurrentSelection();
    if(cur<0) return;
    //禁止删除当前布局
    wxString strText=m_cmbName->GetString(cur);
	wxCommandEvent MyEvent(wxEVT_GET_STYLE);
	((wxApp*)wxApp::GetInstance())->GetTopWindow()->ProcessEvent(MyEvent);
    wxString *p=(wxString*)MyEvent.GetClientData();
    if(p&&!p->IsEmpty()&&strText==*p)
    {
        wxMessageBox(LOADSTRING(DONOT_DELCUR),LOADSTRING(OIP_ERROR),wxOK|wxICON_ERROR);
    }
    else
    {
        m_pImportExport->DoDeleteConfig(strText);
        m_cmbName->Delete(cur);  
    }
    SAFE_DELETE(p);
}
void AskNameDlg::OnOK(wxCommandEvent& event)
{
    wxString str=GetCaption();
    if(!str.IsEmpty())
        event.Skip();

}
wxString AskNameDlg::GetCaption()
{
    wxString str=m_cmbName->GetLabel();
    str=str.Trim(true).Trim(false);
	if ( str.Length() > 20 )
	{
		str = str.Left( 20 );
	}

    return str;
}