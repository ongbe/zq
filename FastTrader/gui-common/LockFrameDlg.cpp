#include "StdAfx.h"
#include "LockFrameDlg.h"
#include "Module-Misc2/SimpleWriteLog.h"
#include "Module-Misc2/GlobalConfigMgr.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

LockFrameDlg::LockFrameDlg(wxWindow *parent):
            wxDialog(parent, wxID_ANY,LOADSTRING(LOCKFRAMEDLG_TITLE),wxDefaultPosition, wxSize(280,135),wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX)
{ 
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();
	string strInvestorID="",strBrokerID="";
	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
	wxString strTitle=LOADSTRING(LOCKFRAMEDLG_TITLE);

	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT(); 
	if(pCfgMgr != NULL && !pCfgMgr->GetSystemParamInfo()->bHideAccount) {
		strTitle+="  -  ";
		strTitle+=strInvestorID.c_str();
	}
	SetTitle(strTitle);

    new wxStaticText(this, wxID_STATIC,LOADSTRING(LOCKFRAMEDLG_STATICTEXT1),wxPoint(5,5),wxSize(260,20));


    new wxStaticText(this, wxID_STATIC,LOADSTRING(LOCKFRAMEDLG_STATICTEXT2),wxPoint(9,35),wxSize(60,20));
    new wxTextCtrl(this,wxID_TEXTCTRL,wxEmptyString,wxPoint(70,35),wxSize(180,20),wxTE_PASSWORD);
    wxButton* pBtn=new wxButton(this, wxID_OK, LOADSTRING(LOCKFRAMEDLG_BUTTON),wxPoint(100,72),wxSize(74,22));
    pBtn->SetDefault();
}
BEGIN_EVENT_TABLE(LockFrameDlg,wxDialog)
EVT_BUTTON(wxID_OK,OnOK)
EVT_CLOSE( LockFrameDlg::OnClose)
END_EVENT_TABLE()
LockFrameDlg::~LockFrameDlg(void)
{
}
void LockFrameDlg::OnOK(wxCommandEvent& evt)
{
    wxString strText=((wxTextCtrl*)FindWindowById(wxID_TEXTCTRL,this))->GetValue();
    IPlatformSingleSvr* pSvr = IS_MULTIACCOUNT_VERSION ? g_pPlatformMgr->GetFirstPlatformSvr() : g_pPlatformMgr->GetDefaultPlatformSvr();
    if(IS_MULTIACCOUNT_VERSION&&pSvr&&pSvr->IsValidUserPW(strText.c_str()))
    {
        evt.Skip();
    }
    else if(!IS_MULTIACCOUNT_VERSION&&pSvr&&pSvr->IsValidInvestorPW(strText.c_str()))
    {
        evt.Skip();
    }
	else
	{
		wxMessageBox(LOADSTRING(PASSWORD_ERROR),LOADSTRING(OIP_ERROR),wxOK|wxICON_ERROR);
    }
}
void LockFrameDlg::OnClose(wxCloseEvent& evt)
{

}