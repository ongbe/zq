#pragma once

class ChangePWDDlg : public wxDialog
{
	enum
	{
		ID_OLD_PWD,
		ID_NEW_PWD,
		ID_NEW_PWD_CONFIRM
	};
public:
	ChangePWDDlg( const wxString& strOldPwd );
	~ChangePWDDlg(void);

	wxString GetOldPwd();
	wxString GetNewPwd();
	bool IsValid( const wxString& strPwd );

	void OnOK( wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	wxTextCtrl* m_pOldPwdText;
	wxTextCtrl* m_pNewPwdText;
	wxTextCtrl* m_pNewPwdConfirmText;
	wxString m_strOldPwd;
	wxString m_strNewPwd;
};
