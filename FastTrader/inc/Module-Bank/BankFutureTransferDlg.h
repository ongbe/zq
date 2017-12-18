/***************************************************************************//**
*   @file         BankFutureTransferDlg.h
*   @brief        银期转账对话框
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/10/11
********************************************************************************/
#ifndef _BANKFUTURETRANSFERDLG_H_
#define _BANKFUTURETRANSFERDLG_H_
#include "stdafx.h"
#include <wx/statline.h>
#include "BankCore.h"

class CBankFutureTransferDlg:public wxDialog,public CBankCore
{
public:
	CBankFutureTransferDlg(wxWindow *parent);
	~CBankFutureTransferDlg();

	void OnRcvRspTransfer(wxCommandEvent& event);
	void OnQueryBankAccount(wxCommandEvent& event);
	void WxButton2Click(wxCommandEvent& event);
	void OnQueryFromFutureToBank(wxCommandEvent& event);
	void OnQueryFromBankToFuture(wxCommandEvent& event);
	void OnBtnQryDetail(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnHide(wxCommandEvent& event);
    void OnSelected(wxCommandEvent& event);
	virtual void DetailListInsertData(int FutureSerial,const char* BankAccount,const char* TradeCode,
		double TradeAmount,const char* CurrencyID,const char* TradeTime,const char* ErrorMsg,const char* BankID);
	virtual wxString GetUIValue(const wxString flag);
	virtual void CreateGUIControls();
private:
	void OnClose(wxCloseEvent& event);

private:
	wxTextCtrl *m_BankPassword;
	wxTextCtrl *m_Amount;
	wxTextCtrl *m_AccountPassword; 
	wxStaticText *m_strPromptText;
	wxComboBox *m_cmbCurrency;
	wxComboBox *m_cmbBankName;

	enum
		{
			////GUI Enum Control ID Start
			ID_BTN_HIDE = 1018,
			ID_WXSTATICTEXT5 = 1017,
			ID_WXSTATICLINE1 = 1016,
			ID_WXBUTTON6 = 1015,
			ID_WXBUTTON5 = 1014,
			ID_WXBUTTON4 = 1013,
			ID_WXBUTTON3 = 1012,
			ID_WXBUTTON2 = 1011,
			ID_WXBUTTON1 = 1010,
			ID_WXCOMBOBOX2 = 1009,
			ID_WXEDIT2 = 1008,
			ID_WXSTATICTEXT4 = 1007,
			ID_WXSTATICTEXT3 = 1006,
			ID_WXEDIT1 = 1005,
			ID_WXSTATICTEXT2 = 1004,
			ID_WXSTATICTEXT1 = 1003,
            ID_WXSTATICTEXT6 = 1019,
            ID_WXEDIT3 = 1020,
			ID_WXCOMBOBOX1 = 1001,
            ID_LISTCTRL = 1021,
			ID_WXSTATICTEXT7 = 1022,
            //ID_TIMER,
			ID_TIMER2,
			ID_TIMER3,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

private:
	DECLARE_EVENT_TABLE();
};

#endif

