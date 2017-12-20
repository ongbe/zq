#pragma once
///////////////////////////////////////////////////////////////////////////
class CProxySvrSetupDlg :
	public wxDialog
{
	enum
	{
		ID_TEXTCTRL=100,
		ID_CHOICE,
		ID_EDIT_ADDR,
		ID_EDIT_PORT,
		ID_EDIT_USER,
		ID_EDIT_PWD,
		ID_SVRLIST,
		ID_TRADE_LIST,
		ID_QUOT_LIST,
		ID_ADD_SVR,
		ID_DEL_SVR,
		ID_ADD_TRADE,
		ID_DEL_TRADE,
		ID_ADD_QUOT,
		ID_DEL_QUOT,
	};

public:
	CProxySvrSetupDlg(wxWindow *parent=NULL);
	~CProxySvrSetupDlg(void);
	void OnTypeChoice(wxCommandEvent& event);
	void OnCellLeftDoubleClick(wxGridEvent& evt);
	void ChangeSvr();
	void OnAddSvr(wxCommandEvent& evt);
	void OnDelSvr(wxCommandEvent& evt);
	void OnAddTrade(wxCommandEvent& evt);
	void OnDelTrade(wxCommandEvent& evt);
	void OnAddQuot(wxCommandEvent& evt);
	void OnDelQuot(wxCommandEvent& evt);
	void OnOK(wxCommandEvent& evt);
	void OnCellChanged(wxGridEvent& evt);
	DECLARE_EVENT_TABLE()
private:
	wxTextCtrl* peditAddr,*peditPort,*peditUser,*peditPwd;
	std::vector<SERVER_PARAM_CFG_ITEM> m_vec;
	int m_SelRow;
	int m_CTPSimulate;
};