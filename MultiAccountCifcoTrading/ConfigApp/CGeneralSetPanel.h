
#pragma once 

class CGeneralSetPanel:public wxPanel
{
public:
	enum
	{
		ID_CBX_LISTSTYLE,
		ID_CBX_MKTPRICEUSE,
		ID_CBX_OPENORDERBTNHOTKEY,
		ID_CBX_ORDERPARKEDBTNHOTKEY
	};

	CGeneralSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CGeneralSetPanel();

	void InitControls();
	bool WriteCfg();

	void OnChangeSel(wxCommandEvent& event);
	void OnCharBuy(wxKeyEvent& event);
	void OnCharSel(wxKeyEvent& event);
	void OnCharOpen(wxKeyEvent& event);
	void OnCharCloseToday(wxKeyEvent& event);
	void OnCharClose(wxKeyEvent& event);

	DECLARE_EVENT_TABLE()

private:
	wxTextCtrl* m_text1;
	wxTextCtrl* m_text2;
	wxTextCtrl* m_text3;
	wxTextCtrl* m_text4;
	wxTextCtrl* m_text5;
	wxTextCtrl* m_text6;
	wxTextCtrl* m_text7;
	wxTextCtrl* m_text8;
	wxTextCtrl* m_text9;

	wxTextCtrl* m_tcBuyHotKey;
	wxTextCtrl* m_tcSelHotKey;
	wxTextCtrl* m_tcOpenOrderHotKey;
	wxTextCtrl* m_tcCloseTodayVolumnHotKey;
	wxTextCtrl* m_tcCloseOrderHotKey;
	wxOwnerDrawnComboBox* m_cbxOpenOrderBtnHotKey;
	wxOwnerDrawnComboBox* m_cbxOrderParkedBtnHotKey;

	wxCheckBox* m_cbxMouseInputInstrumentId;
	wxCheckBox* m_cbxInputSimpleSpell;
	wxOwnerDrawnComboBox* m_cbxListStyle;
	wxCheckBox* m_cbxOrderConfirm;
	wxCheckBox* m_cbxAutoOpenClose;
	wxCheckBox* m_cbxRetrack;
	wxCheckBox* m_cbxSimulate;
	wxOwnerDrawnComboBox* m_cbxMKTPriceUse;
	wxCheckBox* m_cbxMouseInputPrice;
	wxCheckBox* m_cbxMouseInputHand;
	
	wxString m_strOpenOrderBtnHotKey;
	wxString m_strOrderParkedBtnHotKey;

	wxRadioButton* m_rbDefault;
	wxRadioButton* m_rbContract;
	wxRadioButton* m_rbDirection;
	wxRadioButton* m_rbOpenClose;
	wxRadioButton* m_rbPrice;
	wxRadioButton* m_rbVol;
};


