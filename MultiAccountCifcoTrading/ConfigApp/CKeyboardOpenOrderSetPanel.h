
#pragma once 

class CKeyboardOpenOrderSetPanel:public wxPanel
{
public:
	enum
	{
		ID_Check1,
		ID_BtAdd,
		ID_BtSub,
		ID_Com1,
		ID_Com2,
		ID_Com3,
		ID_Com4,
		ID_Com5,
		ID_Com6,
	};

	CKeyboardOpenOrderSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CKeyboardOpenOrderSetPanel();

	void OnAdd(wxCommandEvent& event);
	void OnDel(wxCommandEvent& event);
	void OnListItemSel(wxExtListEvent& event);
	void OnSelc(wxCommandEvent& event);
	void OnCheck(wxCommandEvent& event);

	void SetListData();

	void ReadCfg();

	bool WriteCfg();

	DECLARE_EVENT_TABLE()

private:
	wxCheckBox* m_chkEnableKsyBoardOpenOrder;
	wxCheckBox* m_chkFillOrderPlate;
	wxCheckBox* m_chkMouseReturnToQuoteTable;

	wxOwnerDrawnComboBox* m_cbxHotKey;
	wxOwnerDrawnComboBox* m_cbxBuySel;
	wxOwnerDrawnComboBox* m_cbxPriceType;
	wxOwnerDrawnComboBox* m_cbxAction;
	wxOwnerDrawnComboBox* m_cbxValidArea;

	wxExtListCtrl*  m_listCtrl;

	wxButton* m_btnAdd;
	wxButton* m_btnDelete;
};


