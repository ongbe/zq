
#ifndef _BANKTRANFERDLG_H_
#define _BANKTRANFERDLG_H_

#include <wx/statline.h>
#include "BankCore.h"

class CBankTransferDlg:public wxPanel,public CBankCore
{
public:
	CBankTransferDlg(wxWindow *parent,
            wxWindowID winid = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	~CBankTransferDlg();

	void OnBtnSend(wxCommandEvent& event);
	void OnBtnQryDetail(wxCommandEvent& event);
    void OnSelected(wxExtListEvent& event);
    void OnSize( wxSizeEvent& event );
	void OnReturn(wxCommandEvent& event);
	void OnUserChoice(wxCommandEvent& event);
	void OnRcvRspTransfer(wxCommandEvent& event);
	DECLARE_EVENT_TABLE();
public:
	void SetSungardStyle( bool bFlag );
	wxString GetWindowCaption();
private:
	void SelectLisrRow( const int row );
	void OnDelayTimer(wxTimerEvent& event);
	virtual void DetailListInsertData(int FutureSerial,const char* BankAccount,const char* TradeCode,
		double TradeAmount,const char* TradeTime,const char* ErrorMsg,const char* BankID);
	virtual wxString GetUIValue(const wxString flag);
	virtual void CreateGUIControls();
private:
	wxTextCtrl *m_BankPassword;
	wxTextCtrl *m_Amount;
	wxTextCtrl *m_AccountPassword; 
	wxStaticText *m_strPromptText;
    wxComboBox *m_cmbType;
    wxTextCtrl *m_WithdrawQuota;
	wxTextCtrl *m_BankName;
	wxTextCtrl *m_BankId;
    wxTextCtrl *WxEdit2;
    wxStaticBox *m_GroupBox;
    wxButton *WxButton3;
    wxExtListCtrl *m_BankList;

	enum
	{
		ID_WXLISTCTRL2=1000,
		ID_BTN_QRY_DETAIL,
		ID_WXLISTCTRL1,
		ID_PROMPT_TEXT,
		ID_BTN_SEND,
		ID_WXEDIT7,
		ID_WXSTATICLINE1,
		ID_WXSTATICTEXT9,
		ID_WXEDIT6,
		ID_WXSTATICTEXT8,
		ID_WXEDIT5,
		ID_WXSTATICTEXT7,
		ID_WXCOMBOBOX1,
		ID_WXSTATICTEXT6,
		ID_WXEDIT4,
		ID_WXSTATICTEXT5,
		ID_WXSTATICTEXT4,
		ID_WXSTATICTEXT3,
		ID_WXEDIT1,
		ID_WXSTATICTEXT2,
		ID_WXSTATICTEXT1,
		ID_WXEDIT3,
		ID_WXEDIT2,
		ID_WXSTATICBOX1,
		ID_DELAY_TIMER,
		ID_CHOICEUSER,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

protected:
	bool m_bSungardStyle;
};

#endif