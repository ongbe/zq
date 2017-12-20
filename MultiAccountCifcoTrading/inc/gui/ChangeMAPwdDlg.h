#pragma once
#include "wx\msw\dialog.h"
#include <map>
#include "../MAServer/include/MAAccount/MultiAccountStruct.h"

class ChangeMAPwdDlg :
	public wxDialog
{
public:
	enum
	{
		ID_OLD_PWD,
		ID_NEW_PWD,
		ID_NEW_PWD_CONFIRM
	};

public:
	ChangeMAPwdDlg(wxWindow *parent);
	~ChangeMAPwdDlg(void);

	void OnOK( wxCommandEvent& event);
	wxString GetOldPwd();
	wxString GetNewPwd();

	DECLARE_EVENT_TABLE()

private:
	wxTextCtrl* m_pOldPwdText;
	wxTextCtrl* m_pNewPwdText;
	wxTextCtrl* m_pNewPwdConfirmText;
	wxString m_strOldPwd;
	wxString m_strNewPwd;
};
