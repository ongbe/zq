#pragma once
#include <wx/button.h>
#include <wx/textctrl.h>

enum
{
	ID_ORDERMESSAGEDLG_YES=5200,
	ID_ORDERMESSAGEDLG_NO,
	ID_ORDERMESSAGEDLG_MESSAGETEXT,
	ID_ORDERMESSAGEDLG_CHECK_CONFIG,
	ORDERMESSAGEDLGSUMNUM_ID	// 标志ID号的数量
};

class COrderMessageDlg : public wxDialog
{
public:
	COrderMessageDlg(wxWindow *parent);
	virtual ~COrderMessageDlg();

	void OnBtnYes(wxCommandEvent& event);
	void OnBtnNo(wxCommandEvent& event);
	void OnHookKeyPress(wxKeyEvent& evt);
	void OnCfgChanged(wxCommandEvent& evt);

	virtual bool Show(bool show = true);

	void SetOrderInfo(const wxString& info) {
		wxTextCtrl* pText = (wxTextCtrl*)FindWindowById(ID_ORDERMESSAGEDLG_MESSAGETEXT, this); 
		if(pText!=NULL) {
			pText->SetLabel(info);
		}
	};

private:
	void OnClose(wxCloseEvent& event);
	void CreateGUIControls();
	int ComputeLineNum(char* pStr);

private:

	std::vector<UINT>	m_TabOrderVector;

private:
	DECLARE_EVENT_TABLE();
};
