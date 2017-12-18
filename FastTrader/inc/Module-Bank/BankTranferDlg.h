
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
	void OnRcvRspTransfer(wxCommandEvent& event);
	DECLARE_EVENT_TABLE();
public:
	void SetSungardStyle( bool bFlag );
	wxString GetWindowCaption();
private:
	void SelectLisrRow( const int row );
	void OnDelayTimer(wxTimerEvent& event);
	virtual void DetailListInsertData(int FutureSerial,const char* BankAccount,const char* TradeCode,
		double TradeAmount,const char* CurrencyID,const char* TradeTime,const char* ErrorMsg,const char* BankID);
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
	wxComboBox *m_CurrencyID;

	enum
	{
		////GUI Enum Control ID Start
		ID_WXSTATICTEXT10 = 1028,
		ID_WXCOMBOBOX2 = 1027,
		ID_WXLISTCTRL2 = 1026,
		ID_BTN_QRY_DETAIL = 1025,
		ID_WXLISTCTRL1 = 1024,
		ID_PROMPT_TEXT = 1023,
		ID_BTN_SEND = 1022,
		ID_WXEDIT7 = 1021,
		ID_WXSTATICLINE1 = 1020,
		ID_WXSTATICTEXT9 = 1019,
		ID_WXEDIT6 = 1018,
		ID_WXSTATICTEXT8 = 1017,
		ID_WXEDIT5 = 1016,
		ID_WXSTATICTEXT7 = 1015,
		ID_WXCOMBOBOX1 = 1014,
		ID_WXSTATICTEXT6 = 1013,
		ID_WXEDIT4 = 1012,
		ID_WXSTATICTEXT5 = 1011,
		ID_WXSTATICTEXT4 = 1010,
		ID_WXSTATICTEXT3 = 1009,
		ID_WXEDIT1 = 1008,
		ID_WXSTATICTEXT2 = 1007,
		ID_WXSTATICTEXT1 = 1006,
		ID_WXEDIT3 = 1005,
		ID_WXEDIT2 = 1004,
		ID_WXSTATICBOX1 = 1003,
		ID_DELAY_TIMER,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

protected:
	bool m_bSungardStyle;
};

#endif