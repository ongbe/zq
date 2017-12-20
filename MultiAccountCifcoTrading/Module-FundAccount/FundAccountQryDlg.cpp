#include "StdAfx.h"
#include "FundAccountQryDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(FundAccountQryDlg,wxDialog)
	EVT_CLOSE(FundAccountQryDlg::OnClose)
	EVT_BUTTON(wxID_OK,FundAccountQryDlg::WxButton6Click)
END_EVENT_TABLE()

FundAccountQryDlg::FundAccountQryDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,LOADSTRING(FUNDACCOUNTQRYDLG_TEXT),wxDefaultPosition, wxSize(600,500))
{
	DIALOG_CREATE_LOG();
	SetSize(GetSize().x,GetSize().y+wxSystemSettings::GetMetric(wxSYS_CAPTION_Y)-26);
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_pTextCtrl= new wxTextCtrl( this, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(580,400), wxTE_MULTILINE|wxTE_READONLY );
    itemBoxSizer3->Add(m_pTextCtrl, 0, wxGROW|wxALL, 5);

    wxButton* itemButton6 = new wxButton( this, wxID_OK, LOADSTRING(ASKNAMEDLG_OK), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    //SetContent();
    itemButton6->SetDefault();
    itemButton6->SetFocus();
}

FundAccountQryDlg::~FundAccountQryDlg(void)
{

}
void FundAccountQryDlg::SetContent()
{
	std::string prestr="";
    m_pTextCtrl->Clear();
	prestr.empty();
    if(DEFAULT_SVR()) DEFAULT_SVR()->GetAccountText(prestr,DEFAULT_SVR()->GetServerParamCfg().LanguageId);
	m_pTextCtrl->Replace(-1,-1,prestr);
}

void FundAccountQryDlg::WxButton6Click(wxCommandEvent& event)
{
    Hide();
}
void FundAccountQryDlg::OnClose(wxCloseEvent& event)
{
    Hide();
}