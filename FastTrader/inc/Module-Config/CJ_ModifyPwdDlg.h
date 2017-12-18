#pragma once
#include "stdafx.h"
#include "wx/odcombo.h"
#include "ConfigPanelBase.h"

class CJ_ModifyPwdDlg : public ConfigPanelBase
{
public:
	enum
    {
        ID_COM,
        ID_BTN_OK,
		ID_BTN_CLEAR,
		ID_COM2
    };
   
	CJ_ModifyPwdDlg(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
    ~ CJ_ModifyPwdDlg();

    void InitionInterface();
	bool IsLegal();
	void OnRcvTradePwdChange(wxCommandEvent& evt);
    void OnRcvTFundPwdChange(wxCommandEvent& evt);

	void OnCom(wxCommandEvent& event);
	void OnOk(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);

	void OnLanguageChanged(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	wxStaticText* m_pStaticPwdType;
    wxOwnerDrawnComboBox* m_pComPwdType;
	wxStaticText* m_pStaticOldPwd;
	wxStaticText* m_pStaticNewPwd;
	wxStaticText* m_pStaticPwdConfirm;
	wxStaticText* m_pStaticCurrency;
	wxOwnerDrawnComboBox* m_pCurrency;

	wxButton* m_pBtnOK;
    wxButton* m_pBtnClear;

	wxTextCtrl* Text1;
	wxTextCtrl* Text2;
	wxTextCtrl* Text3;
	
    bool  m_Trade ;
	bool  m_Fund;
};

