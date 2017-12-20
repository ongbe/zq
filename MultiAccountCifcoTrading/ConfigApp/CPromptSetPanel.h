#pragma once 

class CPromptSetPanel : public wxPanel
{
public:
	CPromptSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CPromptSetPanel();

	void InitControls();
	bool WriteCfg();

	DECLARE_EVENT_TABLE()

private:
	wxCheckBox* m_chkOpenOrderSuccess;
	wxOwnerDrawnComboBox* m_cmbOpenOrderSuccessSound;
	wxCheckBox* m_chkOpenOrderFailed;
	wxOwnerDrawnComboBox* m_cmbOpenOrderFailedSound;
	wxCheckBox* m_chkOrderDeal;
	wxOwnerDrawnComboBox* m_cmbOrderDealSound;
	wxCheckBox* m_chkCancelOrderSuccess;
	wxOwnerDrawnComboBox* m_cmbCancelOrderSuccessSound;
	wxCheckBox* m_chkCancelOrderFailed;
	wxOwnerDrawnComboBox* m_cmbCancelOrderFailedSound;

	wxOwnerDrawnComboBox* m_cmbTradeTimeSound;
	wxTextCtrl* m_pBeforeSecText;
	wxOwnerDrawnComboBox* m_cmbBeforeTradeEndSound;

	wxCheckBox* m_chkDeleteInstrument;
};