
#pragma once

enum OrderStrategy;

class COrderAccountTypePanel : public wxPanel
{
public:
	enum
	{
		ID_COMBOX,
	};

	COrderAccountTypePanel( OrderStrategy eStrategy, wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~COrderAccountTypePanel();

	bool WriteCfg();
	void ReadCfg();

	DECLARE_EVENT_TABLE()

private:
	void InitControls();

private:
	wxOwnerDrawnComboBox* m_pCbxAccountType;
	OrderStrategy m_eStrategy;
};