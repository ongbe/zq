#pragma once

enum
{
	ID_ORDERPARKEDSELECTDLG_YES=5100,
	ID_ORDERPARKEDSELECTDLG_NO,
	ID_ORDERPARKEDSELECTDLG_MARKETMSG=1852,
	ID_ORDERPARKEDSELECTDLG_LOCAL_HANDY,
	ID_ORDERPARKEDSELECTDLG_LOCAL_AUTO,
	ID_ORDERPARKEDSELECTDLG_SERVER,
	ORDERPARKEDSELECTDLGSUMNUM_ID	// 标志ID号的数量
};

class COrderParkedSelectDlg : public wxDialog
{
public:
	COrderParkedSelectDlg(wxWindow *parent, bool bIsMarketPrice=false);
	virtual ~COrderParkedSelectDlg();

	void OnBtnYes(wxCommandEvent& event);
	void OnBtnNo(wxCommandEvent& event);
	void OnCfgChanged(wxCommandEvent& evt);

	virtual bool Show(bool show = true);

	// 预埋单的预埋模式，0 本地手动，1 本地自动，2 服务器
	int GetParkedMode() {
		wxRadioButton* window=NULL;
		window=(wxRadioButton*)FindWindowById(ID_ORDERPARKEDSELECTDLG_LOCAL_HANDY,this);
		if(window) 
			if(window->GetValue()) return 0;
		window=(wxRadioButton*)FindWindowById(ID_ORDERPARKEDSELECTDLG_LOCAL_AUTO,this);
		if(window) 
			if(window->GetValue()) return 1;
		window=(wxRadioButton*)FindWindowById(ID_ORDERPARKEDSELECTDLG_SERVER,this);
		if(window) 
			if(window->GetValue()) return 2;
		return -1;
	};

	wxString GetUserInputText();

private:
	void OnClose(wxCloseEvent& event);
	void CreateGUIControls();
	int ComputeLineNum(char* pStr);

private:
    wxStaticText* m_poMarketType_Message;
    bool    m_bIsMarketPrice;
    DECLARE_EVENT_TABLE();
};
