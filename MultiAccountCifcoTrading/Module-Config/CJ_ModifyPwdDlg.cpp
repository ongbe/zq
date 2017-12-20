#include "stdafx.h"
#include "CJ_ModifyPwdDlg.h"
#include "../inc/cfgMgr/CfgMgr.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

static CJ_ModifyPwdDlg* pCJ_ModifyPwdDlg = NULL;

static int TradePwdChange(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(data.BID!= BID_RspUserPasswordUpdate ||data.Size!=sizeof(DataRspUserPasswordUpdate))
		return 0;

	DataRspUserPasswordUpdate* pTmp = new DataRspUserPasswordUpdate();
	memcpy(pTmp, &data, sizeof(DataRspUserPasswordUpdate));

	wxCommandEvent evt(wxEVT_TradePwdChange, wxID_ANY);
	evt.SetClientData(pTmp);
	if( pCJ_ModifyPwdDlg)
		pCJ_ModifyPwdDlg->AddPendingEvent(evt);
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
	if( pCJ_ModifyPwdDlg )
		pCJ_ModifyPwdDlg->AddPendingEvent(evt);

	return 0;
}

BEGIN_EVENT_TABLE(CJ_ModifyPwdDlg,ConfigPanelBase)
	EVT_COMMAND(wxID_ANY, wxEVT_TradePwdChange, CJ_ModifyPwdDlg::OnRcvTradePwdChange)
	EVT_COMMAND(wxID_ANY, wxEVT_FundPwdChange,  CJ_ModifyPwdDlg::OnRcvTFundPwdChange)
	EVT_BUTTON( ID_BTN_OK,OnOk)
	EVT_BUTTON( ID_BTN_CLEAR,OnClear)
	EVT_COMBOBOX(ID_COM, OnCom) 
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
	EVT_CHOICE(ID_CHOICEUSER,OnUserChoice)
END_EVENT_TABLE()

CJ_ModifyPwdDlg::CJ_ModifyPwdDlg(wxWindow *parent,
								 wxWindowID winid ,
								 const wxPoint& pos ,
								 const wxSize& size ,
								 long style,
								 const wxString& name)
								 : ConfigPanelBase(parent,winid,pos,size,style,name),
								 m_pSvr(NULL)
{
	ConfigPanelBase::CreateHeader("CHANGEPASSWORD");
	ConfigPanelBase::EnableSaveAndReturn( false );
	InitionInterface();
	pCJ_ModifyPwdDlg = this;
}

void CJ_ModifyPwdDlg::InitionInterface()
{
	int offset = 50;
	new wxStaticBox(this, wxID_ANY, wxT(""), wxPoint(24, 4+offset), wxSize(264, 206));
	new wxStaticText( this, wxID_STATIC,"ÓÃ»§Ñ¡Ôñ", wxPoint(40,19+offset), wxSize(90, 20), wxALIGN_RIGHT, wxT(""));
	new wxChoice( this, ID_CHOICEUSER, wxPoint(135,16+offset), wxSize(90,20), MA_CORE()->GetAccountArray() );
	m_pStaticPwdType = new wxStaticText(this, wxID_ANY, LOADSTRING(CONFIG_PWD_TYPE), wxPoint(40, 48+offset), wxSize(98, 20), wxALIGN_RIGHT, wxT(""));
	wxArrayString arr;
	arr.Add( LOADSTRING(CONFIG_TRADE_PWD));
	arr.Add( LOADSTRING(CONFIG_ACCOUNT_PWD));
	m_pComPwdType = new wxOwnerDrawnComboBox(this, ID_COM, wxT(""), wxPoint(135, 44+offset), wxSize(98, 20), arr, wxCB_READONLY, wxDefaultValidator, wxT("WxComboBox1"));
	m_pComPwdType->SetSelection(0);

	m_pStaticOldPwd = new wxStaticText(this, wxID_ANY, LOADSTRING(OLDPWD), wxPoint(40, 78+offset), wxSize(90, 20), wxALIGN_RIGHT, wxT(""));
	Text1 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(135, 71+offset), wxSize(96, 19), wxTE_PASSWORD, wxDefaultValidator, wxT(""));

	m_pStaticNewPwd = new wxStaticText(this, wxID_ANY, LOADSTRING(NEWPWD), wxPoint(40, 105+offset), wxSize(90, 20), wxALIGN_RIGHT, wxT(""));
	Text2 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(135, 100+offset), wxSize(96, 19), wxTE_PASSWORD, wxDefaultValidator, wxT(""));
	m_pStaticPwdConfirm = new wxStaticText(this, wxID_ANY, LOADSTRING(PWDCONFIRM), wxPoint(40, 132+offset), wxSize(90, 20), wxALIGN_RIGHT, wxT(""));
	Text3 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxPoint(135, 126+offset), wxSize(96, 19), wxTE_PASSWORD, wxDefaultValidator, wxT(""));

	Text1->Clear();
	Text2->Clear();
	Text3->Clear();

	m_pBtnOK = new wxButton(this, ID_BTN_OK, LOADSTRING(OK), wxPoint(56, 174+offset), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton3"));
	m_pBtnClear = new wxButton(this, ID_BTN_CLEAR, LOADSTRING(CONFIG_CLEAR), wxPoint(150, 174+offset), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	m_Fund = false;
	m_Trade = true;

}

void CJ_ModifyPwdDlg::OnRcvTradePwdChange(wxCommandEvent& evt)
{
	DataRspUserPasswordUpdate* pTmp = (DataRspUserPasswordUpdate*)(evt.GetClientData());
	if(pTmp)
	{
		if(pTmp->RspInfoField.ErrorID!=0)
		{
			wxMessageBox(pTmp->RspInfoField.ErrorMsg, LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),wxOK|wxICON_QUESTION);
		}
		else
		{
			if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspUserPasswordUpdate, GID_MODIFYPWD);
			wxMessageBox( LOADSTRING(CONFIG_MODIFY_PWD_SUCCESSED), LOADSTRING(ABOUTDLG_TITLE) );
		}
		delete pTmp;
	}

	CLOSE_FLOAT_PANEL();
}

void CJ_ModifyPwdDlg::OnRcvTFundPwdChange(wxCommandEvent& evt)
{
	DataRspTradingAccountPasswordUpdate* pTmp = (DataRspTradingAccountPasswordUpdate*)(evt.GetClientData());
	if(pTmp)
	{
		if(pTmp->RspInfoField.ErrorID!=0)
		{
			wxMessageBox( pTmp->RspInfoField.ErrorMsg,LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),wxOK|wxICON_QUESTION);
		}
		else
		{
			if(m_pSvr) m_pSvr->UnSubscribeBusinessData(BID_RspTradingAccountPasswordUpdate, GID_MODIFYPWD);
			wxMessageBox( LOADSTRING(CONFIG_MODIFY_PWD_SUCCESSED), LOADSTRING(ABOUTDLG_TITLE) );
		}
		delete pTmp;
	}

	CLOSE_FLOAT_PANEL();
}

bool CJ_ModifyPwdDlg::IsLegal()
{
	if( Text1->GetValue().IsEmpty() ||
		Text2->GetValue().IsEmpty() ||
		Text3->GetValue().IsEmpty())
	{
		wxMessageBox(LOADSTRING(PWDNOTEMPTY), LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),wxOK|wxICON_ERROR);
		return false;
	}

	if( Text2->GetValue() != Text3->GetValue() )
	{
		wxMessageBox(LOADSTRING(PWDNOTSAME), LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),wxOK|wxICON_ERROR);
		return false;
	}

	if(Text2->GetValue() == Text1->GetValue())
	{
		wxMessageBox(LOADSTRING(PWDNOTCHANGE), LOADSTRING(MAINFRAME_INITIALIZE_ERROR_CAPTION),wxOK|wxICON_ERROR);
		return false;
	}
	return true;
}



void CJ_ModifyPwdDlg::OnOk(wxCommandEvent& event)
{

	if( IsLegal() )
	{
		if(m_Trade)
		{

            if(m_pSvr) m_pSvr->SubscribeBusinessData( BID_RspUserPasswordUpdate, GID_MODIFYPWD, TradePwdChange, true);

			PlatformStru_UserPasswordUpdate  temp1;
			memset(&temp1,0,sizeof(temp1));

			memcpy( temp1.Thost.OldPassword, Text1->GetValue().c_str(),sizeof(temp1.Thost.OldPassword) );
			memcpy( temp1.Thost.NewPassword, Text2->GetValue().c_str(),sizeof(temp1.Thost.NewPassword) );

            if(m_pSvr) m_pSvr->ReqUserPasswordUpdate(temp1);
		}

		if(m_Fund)
		{
            if(m_pSvr) m_pSvr->SubscribeBusinessData( BID_RspTradingAccountPasswordUpdate, GID_MODIFYPWD, FundPwdChange, true);

			PlatformStru_TradingAccountPasswordUpdate temp2;
			memset(&temp2,0,sizeof(temp2));

			memcpy( temp2.Thost.OldPassword, Text1->GetValue().c_str(),sizeof(temp2.Thost.OldPassword) );
			memcpy( temp2.Thost.NewPassword, Text2->GetValue().c_str(),sizeof(temp2.Thost.NewPassword) );

            if(m_pSvr) m_pSvr->ReqTradingAccountPasswordUpdate(temp2);
		}

	}
}

void CJ_ModifyPwdDlg::OnClear(wxCommandEvent& event)
{
	Text1->Clear();
	Text2->Clear();
	Text3->Clear();

}


void  CJ_ModifyPwdDlg::OnCom(wxCommandEvent& event)
{
	std::string str = m_pComPwdType->GetValue();
	if( str == LOADSTRING(CONFIG_TRADE_PWD))
	{
		m_Trade = true;
		m_Fund  = false;
	}
	else
	{
		m_Trade = false;
		m_Fund =  true ;
	}
}

void CJ_ModifyPwdDlg::OnLanguageChanged( wxCommandEvent& event )
{
	ConfigPanelBase::OnLanguageChanged();

	m_pStaticPwdType->SetLabel(LOADSTRING(CONFIG_PWD_TYPE));

	int nSel = m_pComPwdType->GetSelection();
	m_pComPwdType->Clear();
	m_pComPwdType->AppendString(LOADSTRING(CONFIG_TRADE_PWD));
	m_pComPwdType->AppendString(LOADSTRING(CONFIG_ACCOUNT_PWD));
	m_pComPwdType->SetSelection(nSel);

	m_pStaticOldPwd->SetLabel(LOADSTRING(OLDPWD));
	m_pStaticNewPwd->SetLabel(LOADSTRING(NEWPWD));
	m_pStaticPwdConfirm->SetLabel(LOADSTRING(PWDCONFIRM));

	m_pBtnOK->SetLabel( LOADSTRING(OK));
	m_pBtnClear->SetLabel(LOADSTRING(CONFIG_CLEAR));
}

CJ_ModifyPwdDlg::~ CJ_ModifyPwdDlg()
{ 
	pCJ_ModifyPwdDlg = NULL;
}
void CJ_ModifyPwdDlg::OnUserChoice(wxCommandEvent& event)
{
	int sel=event.GetInt();
	wxChoice* pChoice=(wxChoice*)FindWindowById(ID_CHOICEUSER,this);
	if(pChoice)
	{
		m_pSvr=g_pPlatformMgr->GetPlatformSvr(pChoice->GetString(sel).c_str());
	}
}

