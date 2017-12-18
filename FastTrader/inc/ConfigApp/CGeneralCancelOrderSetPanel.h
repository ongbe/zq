#pragma once 

#include "Module-Misc2/globalDefines.h"

class CGeneralCancelOrderSetPanel : public wxPanel
{
public:
	enum
	{
		ID_MOUSECANCELORDER,
		ID_CANCELORDERCONFIRM,
		ID_HIDECANCELORDERBTN,
		ID_ENABLEMODIFYORDER,
		ID_MODIFYORDERCONFIRM,
		ID_CANCELORDERHOTKEY,
		ID_CANCELALLORDERHOTKEY
	};

public:
	CGeneralCancelOrderSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CGeneralCancelOrderSetPanel();

	void InitControls();
	bool WriteCfg();

	void OnChangeSel( wxCommandEvent& event );

	DECLARE_EVENT_TABLE()

private:
	wxCheckBox* m_pChkBoxMouseCancelOrder;
	wxCheckBox* m_pChkBoxCancelOrderConfirm;
	wxCheckBox* m_pChkBoxHideCancelOrderBtn;
	wxCheckBox* m_pChkEnableModifyOrder;
	wxCheckBox* m_pChkBoxModifyOrderConfirm;
	wxOwnerDrawnComboBox* m_pCbxCancelOrderHotKey;
	wxOwnerDrawnComboBox* m_pCbxCancelAllOrderHotKey;

	wxString m_strCancelOrderHotKey;
	wxString m_strCancelAllOrderHotKey;
};