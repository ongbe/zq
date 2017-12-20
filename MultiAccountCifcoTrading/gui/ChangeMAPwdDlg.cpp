#include "StdAfx.h"
#include "ChangeMAPwdDlg.h"
#include "../ISvr/IPlatformSingleSvr.h"
#include "../ISvr/PlatformSvrMgr.h"

extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(ChangeMAPwdDlg,wxDialog)
	EVT_BUTTON(wxID_OK, OnOK)
END_EVENT_TABLE()

ChangeMAPwdDlg::ChangeMAPwdDlg(wxWindow *parent)
		: wxDialog(parent, wxID_ANY, "ÐÞ¸ÄµÇÂ¼ÃÜÂë", wxDefaultPosition, wxSize(250,200))
{
	int nStaticLeft = 20;
	int nStaticWidth = 80;
	int nTextLeft = nStaticLeft + nStaticWidth + 2;
	int nTextWidth = 120;
	int nRowOffset = 10;
	int nTop = 20;
	int nControlHight = 20;

	wxTextValidator validatorAlphaNum( wxFILTER_ALPHANUMERIC );

	new wxStaticText( this, wxID_STATIC, LOADSTRING(OLDPWD), wxPoint(nStaticLeft, nTop), wxSize( nStaticWidth, nControlHight));
	m_pOldPwdText = new wxTextCtrl( this, ID_OLD_PWD, wxEmptyString, wxPoint(nTextLeft, nTop), wxSize( nTextWidth, nControlHight), wxTE_PASSWORD, validatorAlphaNum );
	new wxStaticText( this, wxID_STATIC, LOADSTRING(NEWPWD), wxPoint(nStaticLeft, nTop + nControlHight + nRowOffset), wxSize( nStaticWidth, nControlHight));
	m_pNewPwdText = new wxTextCtrl( this, ID_NEW_PWD, wxEmptyString, wxPoint(nTextLeft, nTop + nControlHight + nRowOffset), wxSize( nTextWidth, nControlHight), wxTE_PASSWORD, validatorAlphaNum );
	new wxStaticText( this, wxID_STATIC, LOADSTRING(PWDCONFIRM), wxPoint(nStaticLeft, nTop + 2 * (nControlHight + nRowOffset)), wxSize( nStaticWidth, nControlHight));
	m_pNewPwdConfirmText = new wxTextCtrl( this, ID_NEW_PWD_CONFIRM, wxEmptyString, wxPoint(nTextLeft, nTop + 2 * (nControlHight + nRowOffset)), wxSize( nTextWidth, nControlHight), wxTE_PASSWORD, validatorAlphaNum );
	new wxButton( this, wxID_OK, LOADSTRING(OK), wxPoint( 60, nTop + 3 * (nControlHight + nRowOffset)), wxSize( 50, 25));
	new wxButton( this, wxID_CANCEL, LOADSTRING(CANCEL), wxPoint( 140, nTop + 3 * (nControlHight + nRowOffset)), wxSize( 50, 25));

	m_pOldPwdText->SetMaxLength(12);
	m_pNewPwdText->SetMaxLength(12);
	m_pNewPwdConfirmText->SetMaxLength(12);
}

ChangeMAPwdDlg::~ChangeMAPwdDlg(void)
{
}

void ChangeMAPwdDlg::OnOK( wxCommandEvent& event )
{
	if ( m_pOldPwdText->GetValue().IsEmpty())
	{
		wxMessageBox( LOADSTRING(PWDNOTEMPTY) );
		m_pOldPwdText->SetFocus();
		return;
	}

	IPlatformSingleSvr* pSvr = DEFAULT_SVR();
	if ( NULL != pSvr )
	{
		if ( 1 != pSvr->IsValidPW(m_pOldPwdText->GetValue().c_str()))
		{
			wxMessageBox("Ô­ÃÜÂëÊäÈë´íÎó");
			m_pOldPwdText->Clear();
			m_pOldPwdText->SetFocus();
			return;
		}
	}

	if ( m_pNewPwdText->GetValue().IsEmpty())
	{
		wxMessageBox( LOADSTRING(PWDNOTEMPTY) );
		m_pNewPwdText->SetFocus();
		return;
	}

	if ( m_pNewPwdText->GetValue().Length() < 6 )
	{
		wxMessageBox("ÃÜÂë±ØÐëÎª6~12Î»×Ö·û");
		m_pNewPwdText->SetFocus();
		return;
	}

	if ( m_pNewPwdConfirmText->GetValue().IsEmpty())
	{
		wxMessageBox( LOADSTRING(PWDNOTEMPTY) );
		m_pNewPwdConfirmText->SetFocus();
		return;
	}

	if ( m_pNewPwdText->GetValue() != m_pNewPwdConfirmText->GetValue())
	{
		wxMessageBox( LOADSTRING(PWDNOTSAME) );
		m_pNewPwdConfirmText->Clear();
		m_pNewPwdConfirmText->SetFocus();
		return;
	}

	if ( m_pOldPwdText->GetValue() == m_pNewPwdText->GetValue() )
	{
		wxMessageBox( LOADSTRING(PWDNOTCHANGE) );
		m_pNewPwdText->Clear();
		m_pNewPwdConfirmText->Clear();
		m_pNewPwdText->SetFocus();
		return;
	}

	m_strOldPwd = m_pOldPwdText->GetValue();
	m_strNewPwd = m_pNewPwdText->GetValue();
	event.Skip();
}

wxString ChangeMAPwdDlg::GetNewPwd()
{
	return m_strNewPwd;
}

wxString ChangeMAPwdDlg::GetOldPwd()
{
	return m_strOldPwd;
}
