#pragma once

#include "ConfigPanelBase.h"

class J_ShuZiKuaiJieJianDlg : public ConfigPanelBase
{
public:
	enum
	{
		ID_List,
		ID_ADD,
		ID_DEL,
		ID_Text1
	};

	bool Legal( string str );
	void OnReturn(wxCommandEvent& event);
	//void OnKey(wxKeyEvent& evt);
	void OnKey(wxKeyEvent& evt);
	void OnContextMenu(wxContextMenuEvent& event);
	void ColClick(wxExtListEvent& event);
	void RightClick(wxExtListEvent& event);
	void Sel(wxExtListEvent& event);

	void OnSize( wxSizeEvent& event );

	void OnDel(wxCommandEvent& event);
	void OnSaveAndReturn(wxCommandEvent& event);

	void OnAdd(wxCommandEvent& event);
	void TextChange(wxCommandEvent& event);
	void OnLanguageChanged(wxCommandEvent& event);
	void OnLabelEndEdit(wxExtListEvent& event);

	J_ShuZiKuaiJieJianDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	void ReadCfg();
	void WriteCfg();

	DECLARE_EVENT_TABLE()

private:
	wxStaticBox* m_pStaticBox;
	wxStaticText* m_pStaticAccelerator;
	wxStaticText* m_pStaticAcceIndicate;
	wxStaticText* m_pStaticAcceIntro;
	wxStaticBox* m_pBoxEgg;
	wxStaticText* m_pStaticEggAcce;
	wxStaticText* m_pStaticEggAcceIndicate;

	wxButton* m_pBtnAdd;
	wxButton* m_pBtnDel;

	wxExtListCtrl* m_pList;
	wxTextCtrl* Text1;
	wxTextCtrl* Text2;
	wxTextCtrl* Text3;

	bool m_bChanged;
};


