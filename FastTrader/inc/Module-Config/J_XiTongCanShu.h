#pragma once

#include "ConfigPanelBase.h"

class J_XiTongCanShuDlg : public ConfigPanelBase
{
public:
	enum
	{
		ID_COM,
		ID_LIST,
		ID_BTN_APPLY_DEFAULT,
		ID_BTOK,
		ID_CHECK1
	};

	void OnSaveAndReturn(wxCommandEvent& event);
	void OnHotkeySelChanged(wxCommandEvent& event);
	void OnApplyDefault(wxCommandEvent& event);
	void OnReturn(wxCommandEvent& event);
	void OnCheck(wxCommandEvent& event);
	void OnPanelCharHook(wxCommandEvent& evt);
	void OnRadioButton(wxCommandEvent& event);
	void OnLanguageChanged(wxCommandEvent& event);
	void ReadCfg(bool bDefault=false);
	void WriteCfg();

	J_XiTongCanShuDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);
	~J_XiTongCanShuDlg();

	DECLARE_EVENT_TABLE()

private:
	wxStaticBox* m_pStaticBox;
	wxCheckBox* m_pChkUseHideHotkey;
	wxOwnerDrawnComboBox* m_combox;

	wxCheckBox* m_chkAlt;
	wxCheckBox* m_chkCtrl;
	wxCheckBox* m_chkShift;
	wxCheckBox* m_chkWindow;

	wxRadioButton* Rb1;
	wxRadioButton* Rb2;
	wxRadioButton* Rb3;

	wxButton*  m_pBtnApplyDefault;

	bool m_bChanged ;
};


