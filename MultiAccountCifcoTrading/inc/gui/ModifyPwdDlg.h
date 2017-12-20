#pragma once
#include "stdafx.h"

class ModifyPwdDlg :
    public wxDialog
{
enum
{
	ID_EditOldPwd,
	ID_EditNewPwd,
	ID_EditCfgPwd,
	ID_ChoiceUser,
	ID_MyOK,
	ID_MyCancel,
	ID_RbtTrade,
	ID_RbtFund
};

public:



	wxTextCtrl*  m_OldPwd;
    wxTextCtrl*  m_NewPwd;
    wxTextCtrl*  m_CfgPwd;

	wxButton*    m_BtOk;
	wxButton*    m_BtCancel;

   	wxRadioButton* m_RbTrade;
	wxRadioButton* m_RbFund;

    bool  m_Trade ;
	bool  m_Fund;

    ModifyPwdDlg(wxWindow *parent);
	~ ModifyPwdDlg();
    void  InitionInterface();
	bool IsLegal();
	void  OnRcvTradePwdChange(wxCommandEvent& evt);
    void  OnRcvTFundPwdChange(wxCommandEvent& evt);


    void OnMyOK(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

	void OnRadioButton(wxCommandEvent& event);
	void OnUserSel(wxCommandEvent& event);
public:
	wxArrayString m_items;
    IPlatformSingleSvr* m_pSvr;

	DECLARE_EVENT_TABLE()
};
