#include "stdafx.h"
#include "ModifyPwdDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

static ModifyPwdDlg* pModifyPwdDlg = NULL;

static int TradePwdChange(const GUIModuleID GID,const AbstractBusinessData& data)
{
   	if(data.BID!= BID_RspUserPasswordUpdate ||data.Size!=sizeof(DataRspUserPasswordUpdate))
		return 0;

	DataRspUserPasswordUpdate* pTmp = new DataRspUserPasswordUpdate();
	memcpy(pTmp, &data, sizeof(DataRspUserPasswordUpdate));

	wxCommandEvent evt(wxEVT_TradePwdChange, wxID_ANY);
	evt.SetClientData(pTmp);
	if( pModifyPwdDlg)
		 pModifyPwdDlg->AddPendingEvent(evt);
	return  0;
}

static int FundPwdChange(const GUIModuleID GID,const AbstractBusinessData& data)
{

	if(data.BID!= BID_RspTradingAccountPasswordUpdate ||data.Size!=sizeof(DataRspTradingAccountPasswordUpdate))
	return 0;

	DataRspTradingAccountPasswordUpdate* pTmp = new DataRspTradingAccountPasswordUpdate();
	memcpy(pTmp, &data, sizeof(DataRspTradingAccountPasswordUpdate));

	wxCommandEvent evt(wxEVT_FundPwdChange, wxID_ANY);
	evt.SetClientData(pTmp);
	if( pModifyPwdDlg )
		 pModifyPwdDlg->AddPendingEvent(evt);

	 return 0;
}


ModifyPwdDlg::ModifyPwdDlg(wxWindow *parent):
			wxDialog(parent, wxID_ANY,LOADSTRING(CHANGEPASSWORD),wxDefaultPosition, wxSize(350,250),wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxDIALOG_MODELESS )

{
	m_pSvr=NULL;
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();	
	SetBackgroundColour(DEFAULT_COLOUR);
	InitionInterface();
    pModifyPwdDlg = this;
}

void ModifyPwdDlg::InitionInterface()
{
    
	//wxBoxSizer*  BoxSizer = new wxBoxSizer(wxVERTICAL);
	//this->SetSizer(BoxSizer);

 //   wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
 //   BoxSizer->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);


  	m_RbTrade = new wxRadioButton( this, ID_RbtTrade, LOADSTRING(GUI_MODIFY_TRADE_PWD),  wxPoint(60,14), wxSize(120,17), 0 );
    m_RbTrade->SetValue(true);
    //itemBoxSizer3->Add(m_RbTrade, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_RbFund = new wxRadioButton( this, ID_RbtFund, LOADSTRING(GUI_MODIFY_ACCOUNT_PWD),  wxPoint(200,15), wxSize(130,17), 0 );
    m_RbFund->SetValue(false);
    //itemBoxSizer3->Add(m_RbFund, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);


    m_Trade  = true;
	m_Fund   = false;

	//wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    //BoxSizer->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( this, wxID_STATIC, LOADSTRING(GUI_OLD_PWD), wxPoint(94,51), wxDefaultSize, 0 );
    //itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);


	m_OldPwd = new wxTextCtrl( this, ID_EditOldPwd,wxEmptyString,  wxPoint(163,48), wxSize(121,19), wxTE_PASSWORD );
	m_OldPwd->SetMaxLength(40);
   // itemBoxSizer6->Add(m_OldPwd, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);



    //wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    //BoxSizer->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText10 = new wxStaticText( this, wxID_STATIC, LOADSTRING(GUI_NEW_PWD), wxPoint(95,79), wxDefaultSize, 0 );
    //itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    //m_NewPwd = new wxTextCtrl( this, ID_EditNewPwd,wxEmptyString,  wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	m_NewPwd = new wxTextCtrl( this, ID_EditNewPwd,wxEmptyString,  wxPoint(163,76), wxSize(121,19), wxTE_PASSWORD );
	m_NewPwd->SetMaxLength(40);
    //itemBoxSizer9->Add(m_NewPwd, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
     


	//wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    //BoxSizer->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( this, wxID_STATIC, LOADSTRING(PWDCONFIRM), wxPoint(84,112), wxDefaultSize, 0 );
    //itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_CfgPwd = new wxTextCtrl( this, ID_EditCfgPwd,wxEmptyString,  wxPoint(163,110), wxSize(121,19), wxTE_PASSWORD );
	m_CfgPwd->SetMaxLength(40);
    //itemBoxSizer12->Add(m_CfgPwd, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_items=MA_CORE()->GetAccountArray();
	new wxStaticText( this, wxID_STATIC,"ÓÃ»§Ñ¡Ôñ", wxPoint(84,138), wxDefaultSize, 0 );
	wxChoice* m_ChoiceCtrl = new wxChoice( this, ID_ChoiceUser, wxPoint(164,138), wxSize(120,-1), m_items  );
   // wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    //BoxSizer->Add(itemBoxSizer15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    m_BtOk   =  new wxButton( this, ID_MyOK, LOADSTRING(ASKNAMEDLG_OK),  wxPoint(82,166), wxSize(75,25), 0 );
    //itemBoxSizer15->Add(m_BtOk, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    m_BtCancel= new wxButton( this, wxID_CANCEL, LOADSTRING(ASKNAMEDLG_CANCEL), wxPoint(192,166), wxSize(75,25), 0 );
	//itemBoxSizer15->Add( m_BtCancel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

   
	SetSize(8,8,375,265);
	Center();

}
void ModifyPwdDlg::OnUserSel(wxCommandEvent& event)
{
	int sel=event.GetInt();
	if(sel>=0&&sel<(int)m_items.size())
	{
		m_pSvr=g_pPlatformMgr->GetPlatformSvr(m_items[sel].c_str());
	}
}
void ModifyPwdDlg::OnRcvTradePwdChange(wxCommandEvent& evt)
{

	DataRspUserPasswordUpdate* pTmp = (DataRspUserPasswordUpdate*)(evt.GetClientData());
	if(!pTmp)
	{     	
		return;
	}


	//if( pTmp->bIsLast == true)

	// if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspUserPasswordUpdate, GID_MODIFYPWD);

    if( pTmp->RspInfoField.ErrorID!=0  )
	{

		wxMessageBox(pTmp->RspInfoField.ErrorMsg,LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
		delete pTmp;
	    wxDialog::EndDialog(0);
		return;
	}

	CThostFtdcUserPasswordUpdateField& rawData= pTmp->UserPasswordUpdateField;


	wxMessageBox( LOADSTRING(CONFIG_MODIFY_PWD_SUCCESSED),LOADSTRING(USERLOGINDLG_INFORMATION));
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspUserPasswordUpdate, GID_MODIFYPWD);
	delete pTmp;

	wxDialog::EndDialog(0);
}

void ModifyPwdDlg::OnRcvTFundPwdChange(wxCommandEvent& evt)
{
    DataRspTradingAccountPasswordUpdate* pTmp = (DataRspTradingAccountPasswordUpdate*)(evt.GetClientData());
	if(!pTmp)
		return;

	//if( pTmp->bIsLast == true)

	// if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspTradingAccountPasswordUpdate, GID_MODIFYPWD);

    if( pTmp->RspInfoField.ErrorID!=0  )
	{
		
		wxMessageBox( pTmp->RspInfoField.ErrorMsg,LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);

		delete pTmp;
		wxDialog::EndDialog(0);
		return;
	}

	CThostFtdcTradingAccountPasswordUpdateField& rawData= pTmp->TradingAccountPasswordUpdateField;

	wxMessageBox(LOADSTRING(CONFIG_MODIFY_PWD_SUCCESSED),LOADSTRING(USERLOGINDLG_INFORMATION));
	
	if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspTradingAccountPasswordUpdate, GID_MODIFYPWD);
	delete pTmp;

    wxDialog::EndDialog(0);
}

bool ModifyPwdDlg::IsLegal()
{
	wxString str = m_NewPwd->GetValue();
    wxString strOld=m_OldPwd->GetValue();
	if( m_NewPwd->GetValue() != m_CfgPwd->GetValue() )
	{
        wxMessageBox(LOADSTRING(PWDNOTSAME),LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),wxOK|wxICON_ERROR);
		return false;
	}
    if(str==strOld)
    {
        wxMessageBox(LOADSTRING(PWDNOTCHANGE),LOADSTRING(PWDNOTCHANGE),wxOK|wxICON_ERROR);
		return false;
    }
	return true;
}



void ModifyPwdDlg::OnMyOK(wxCommandEvent& event)
{

	if( IsLegal() )
	{
		if(m_Trade)
		{
            if(m_pSvr) m_pSvr->SubscribeBusinessData( BID_RspUserPasswordUpdate, GID_MODIFYPWD, TradePwdChange, true);

			PlatformStru_UserPasswordUpdate  temp1;
			memset(&temp1,0,sizeof(temp1));

            memcpy( temp1.Thost.OldPassword, m_OldPwd->GetValue().c_str(),sizeof(temp1.Thost.OldPassword) );
            memcpy( temp1.Thost.NewPassword, m_NewPwd->GetValue().c_str(),sizeof(temp1.Thost.NewPassword) );


            if(m_pSvr) m_pSvr->ReqUserPasswordUpdate(temp1);
		}

		if(m_Fund)
		{
            if(m_pSvr)m_pSvr->SubscribeBusinessData( BID_RspTradingAccountPasswordUpdate, GID_MODIFYPWD, FundPwdChange, true);

			PlatformStru_TradingAccountPasswordUpdate temp2;
			memset(&temp2,0,sizeof(temp2));

            memcpy( temp2.Thost.OldPassword, m_OldPwd->GetValue().c_str(),sizeof(temp2.Thost.OldPassword) );
            memcpy( temp2.Thost.NewPassword, m_NewPwd->GetValue().c_str(),sizeof(temp2.Thost.NewPassword) );

			if(m_pSvr) m_pSvr->ReqTradingAccountPasswordUpdate(temp2);
		}
       
	}
}
    
void ModifyPwdDlg::OnExit(wxCommandEvent& event)
{
	wxDialog::EndDialog(ID_MyCancel);

}


void  ModifyPwdDlg::OnRadioButton(wxCommandEvent& event)
{
     int id=event.GetId();

	switch(id)
    {
    case ID_RbtTrade:
        m_Trade = true;
		m_Fund  = false;
        break;

    case ID_RbtFund:

		m_Trade = false;
		m_Fund =  true ;
        break;

	default:
        break;
    }
}


ModifyPwdDlg::~ ModifyPwdDlg()
{ 
   if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspTradingAccountPasswordUpdate, GID_MODIFYPWD);
   if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspUserPasswordUpdate, GID_MODIFYPWD);
   pModifyPwdDlg = NULL;
}


BEGIN_EVENT_TABLE(ModifyPwdDlg,wxDialog)


	EVT_COMMAND(wxID_ANY, wxEVT_TradePwdChange, ModifyPwdDlg::OnRcvTradePwdChange)
	EVT_COMMAND(wxID_ANY, wxEVT_FundPwdChange,  ModifyPwdDlg::OnRcvTFundPwdChange)

	EVT_BUTTON(ID_MyOK,OnMyOK)
	EVT_BUTTON(wxID_CANCEL,OnExit)

    EVT_RADIOBUTTON(ID_RbtTrade,OnRadioButton)
    EVT_RADIOBUTTON(ID_RbtFund,OnRadioButton)
	EVT_CHOICE(ID_ChoiceUser,OnUserSel)
END_EVENT_TABLE()