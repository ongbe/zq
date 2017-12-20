#pragma once 

class CMouseOpenOrderSetPanel:public wxPanel
{
public:
	CMouseOpenOrderSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CMouseOpenOrderSetPanel();

	void ReadCfg();
	bool WriteCfg();

	DECLARE_EVENT_TABLE()

private:
	wxOwnerDrawnComboBox* m_cbxClickQuote;
	wxOwnerDrawnComboBox* m_cbxDoubleClickQuote;
	wxOwnerDrawnComboBox* m_cbxClickPosition;
	wxOwnerDrawnComboBox* m_cbxDoubleClickPosition;

	wxRadioButton* m_radioReserveOpenOrder;
	wxRadioButton* m_radioSyntropOpenOrder;

	wxCheckBox* m_chkCtrlReserve;
	wxCheckBox* m_chkMouseClickOpenOrder;

	wxTextCtrl* m_textPriceOffset;
};
