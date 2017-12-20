#include "StdAfx.h"
#include "ChangePWDDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
BEGIN_EVENT_TABLE(ChangePWDDlg,wxDialog)
	EVT_BUTTON(wxID_OK, OnOK)
END_EVENT_TABLE()

ChangePWDDlg::ChangePWDDlg( const wxString& strOldPwd )
:wxDialog( NULL, wxID_ANY, LOADSTRING(CHANGEPASSWORD), wxDefaultPosition, wxSize(250,200),wxDEFAULT_DIALOG_STYLE )
, m_strOldPwd( strOldPwd )
, m_strNewPwd( wxEmptyString )
{
	DIALOG_CREATE_LOG();
	SetBackgroundColour(DEFAULT_COLOUR);
	int nStaticLeft = 20;
	int nStaticWidth = 80;
	int nTextLeft = nStaticLeft + nStaticWidth + 2;
	int nTextWidth = 120;
	int nRowOffset = 10;
	int nTop = 20;
	int nControlHight = 20;
	new wxStaticText( this, wxID_STATIC, LOADSTRING(OLDPWD), wxPoint(nStaticLeft, nTop), wxSize( nStaticWidth, nControlHight));
	m_pOldPwdText = new wxTextCtrl( this, ID_OLD_PWD, wxEmptyString, wxPoint(nTextLeft, nTop), wxSize( nTextWidth, nControlHight), wxTE_PASSWORD );
	m_pOldPwdText->SetValue( strOldPwd );
	new wxStaticText( this, wxID_STATIC, LOADSTRING(NEWPWD), wxPoint(nStaticLeft, nTop + nControlHight + nRowOffset), wxSize( nStaticWidth, nControlHight));
	m_pNewPwdText = new wxTextCtrl( this, ID_NEW_PWD, wxEmptyString, wxPoint(nTextLeft, nTop + nControlHight + nRowOffset), wxSize( nTextWidth, nControlHight), wxTE_PASSWORD );
	new wxStaticText( this, wxID_STATIC, LOADSTRING(PWDCONFIRM), wxPoint(nStaticLeft, nTop + 2 * (nControlHight + nRowOffset)), wxSize( nStaticWidth, nControlHight));
	m_pNewPwdConfirmText = new wxTextCtrl( this, ID_NEW_PWD_CONFIRM, wxEmptyString, wxPoint(nTextLeft, nTop + 2 * (nControlHight + nRowOffset)), wxSize( nTextWidth, nControlHight), wxTE_PASSWORD );
	new wxButton( this, wxID_OK, LOADSTRING(OK), wxPoint( 60, nTop + 3 * (nControlHight + nRowOffset)), wxSize( 50, 25));
	new wxButton( this, wxID_CANCEL, LOADSTRING(CANCEL), wxPoint( 140, nTop + 3 * (nControlHight + nRowOffset)), wxSize( 50, 25));
}

ChangePWDDlg::~ChangePWDDlg(void)
{
}

void ChangePWDDlg::OnOK( wxCommandEvent& event )
{
	if ( m_pOldPwdText->GetValue().IsEmpty())
	{
		wxMessageBox( LOADSTRING(PWDNOTEMPTY) );
		m_pOldPwdText->SetFocus();
		return;
	}

	if ( m_pNewPwdText->GetValue().IsEmpty())
	{
		wxMessageBox( LOADSTRING(PWDNOTEMPTY) );
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
	
	if ( !IsValid( m_pNewPwdText->GetValue() ))
	{
		wxMessageBox( LOADSTRING(PWNNOTALPHA) );
		m_pNewPwdText->Clear();
		m_pNewPwdConfirmText->Clear();
		m_pNewPwdText->SetFocus();
		return;
	}
	 
	m_strOldPwd = m_pOldPwdText->GetValue();
	m_strNewPwd = m_pNewPwdText->GetValue();
	event.Skip();
}

wxString ChangePWDDlg::GetNewPwd()
{
	return m_strNewPwd;
}

wxString ChangePWDDlg::GetOldPwd()
{
	return m_strOldPwd;
}

bool ChangePWDDlg::IsValid( const wxString& strPwd )
{
	for ( int i = 0; i < (int)strPwd.Length(); i++)
	{
		char szCh = strPwd[i];
		if ( (szCh >= 'a' && szCh <= 'z') || (szCh >= 'A' && szCh <= 'Z'))
		{
		}
		else if ( szCh >= '0' && szCh <= '9')
		{
			if ( i == 0 )
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}
