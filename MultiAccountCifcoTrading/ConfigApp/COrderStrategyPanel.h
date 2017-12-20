
#pragma once

enum OrderStrategy;

class COrderStrategyPanel : public wxPanel
{
public:
	enum
	{
		ID_INPUT_CONTRACT,
		ID_ADD_CONTRACT,
		ID_DEL_CONTRACT,
		ID_LIST,
		ID_SET_DEFAULT_VOLUME,
	};

	COrderStrategyPanel( OrderStrategy eStrategy, wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	~COrderStrategyPanel();

	void OnAddContract(wxCommandEvent& event);
	void OnDelContract(wxCommandEvent& event);
	void OnSetDefaultVolume(wxCommandEvent& event);
	void OnLabelEndEdit(wxExtListEvent& event);

	bool IsExist( const wxString& strContract);

	bool WriteCfg();
	void ReadCfg();

	DECLARE_EVENT_TABLE()

private:
	void InitControls();

	bool MatchInstrument(const wxString& strOri, wxString& strTarget);

private:
	wxExtListCtrl* m_pList;
	wxTextCtrl* m_pContractText;

	std::vector<std::string> m_vContracts;

	OrderStrategy m_eStrategy;
};