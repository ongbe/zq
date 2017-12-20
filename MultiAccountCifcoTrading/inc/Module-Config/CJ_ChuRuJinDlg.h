#pragma once 
#include "stdafx.h"
#include "wx/odcombo.h"
#include <string >
#include <vector>
#include <wx/datectrl.h>

#include <wx/listctrl.h>
#include "ConfigPanelBase.h"

using namespace std;

typedef struct
{
	string date;
	string serial;
	string direction;
	string in;
	string out;
	string name;
	string type;
	string bankid;
	string comment;

} SerialInfo;

typedef struct
{
	string in;
}InInfo;

typedef struct
{
	string out;
}OutInfo;

class CJ_ChuRuJinDlg : public ConfigPanelBase
{
public:
	enum
	{
		ID_BTN_QUERY,
		ID_BTN_CANCEL,
		ID_ChoiceUser,
		ID_Com
	};

	CJ_ChuRuJinDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxPoint(0,0),
		const wxSize& size = wxSize(800,280),
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);
	~CJ_ChuRuJinDlg();

	void OnSize(wxSizeEvent& evt);
	void OnCom(wxCommandEvent& event);

	void OnQuery(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);

	void OnRcvRspTransfer(wxCommandEvent& evt);

	void OnClose(wxCloseEvent& event);
	void OnLanguageChanged(wxCommandEvent& event);
	void OnUserChoice(wxCommandEvent& event);
	DECLARE_EVENT_TABLE();

private:
	wxButton *m_pBtnQuery;	

	wxComboBox* m_pCombox;
	wxStaticText* m_pStaticDirection;
	wxStaticText* m_pStaticPrompt;
	wxExtListCtrl_Obsolete* m_pList;

	vector<SerialInfo> m_vec;
	IPlatformSingleSvr* m_pSvr;
	InInfo  m_InInfo;
	OutInfo m_OutInfo;
};

