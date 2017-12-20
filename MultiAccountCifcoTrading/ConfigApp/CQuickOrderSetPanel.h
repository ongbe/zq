
#pragma once

class CQuickOrderSetPanel : public wxPanel
{
public:
	enum
	{
		ID_OPPOSITE_POSITION1,
		ID_OPPOSITE_POSITION2,
		ID_OPPOSITE_POSITION3,
		ID_MOUSE_CLOSE_POSITION,
		ID_ACTION_BEFORE_OPEN_ORDER,
	};

	CQuickOrderSetPanel(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~CQuickOrderSetPanel();

	bool WriteCfg();
	void ReadCfg();

	DECLARE_EVENT_TABLE()

private:
	void InitControls();

	wxString GetStrategyString(int nQuickOrderStrategy);
	int GetStrategyInt( const wxString& strLable );

private:
	wxOwnerDrawnComboBox* m_pComNoOppositePosition;
	wxOwnerDrawnComboBox* m_pComSomeHaveOppositePosition;
	wxOwnerDrawnComboBox* m_pComAllHaveOppositePosition;
	wxOwnerDrawnComboBox* m_pComMouseClosePosition;

	wxCheckBox* m_pChkActionBeforeOpenOrder;
};