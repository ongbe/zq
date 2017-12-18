#pragma once

#include <wx/spinctrl.h>
#include "cfgMgr/CfgGlobal.h"
#include "ConfigPanelBase.h"

class J_WeiTuoCanShuDlg :public ConfigPanelBase
{
public:
	enum
	{
		ID_COM,
		ID_LIST,
		ID_BTSET,
		ID_List
	};

	J_WeiTuoCanShuDlg(wxWindow *parent,
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = wxPanelNameStr);

	void OnPanelCharHook(wxCommandEvent& evt);
	void Sel(wxListEvent& event);
	void OnSize( wxSizeEvent& event );
	void OnSaveAndReturn(wxCommandEvent& event);
	void OnCom(wxCommandEvent& event);
	void OnSet(wxCommandEvent& event);
	void OnReturn(wxCommandEvent& event);
	void BeginEdit(wxExtListEvent& event);
	void EndEdit(wxExtListEvent& event);
	void OnText(wxExtListEvent& event);
	void OnColClick(wxExtListEvent& event);
	void OnRadioButton(wxCommandEvent& event);
	void OnCheck( wxCommandEvent& event);
	void OnLanguageChanged(wxCommandEvent& event);
	void Init();
	void ReadCfg();
	void WriteCfg();
	void SetDefaultVolume();
	void ReadInstrumentList();

	DECLARE_EVENT_TABLE()

private:
	vector<VarietyInfo> m_vVarietyInfo;
	vector<InstrumentGroupInfo> m_vecGroupInfo;

	wxStaticBox* m_pStaticModifyOrderParam;
	wxStaticText* m_pStaticExchange;
	wxOwnerDrawnComboBox* m_pComGroup;
	wxCheckBox* m_pChkOrderConfirm;
	wxCheckBox* m_pChkOrderPrompt;
	wxCheckBox* m_pChkCancelOrder;
	wxStaticText* m_pStaticPriceTicker;
	wxStaticText* m_pStaticPriceMethod;

	wxRadioButton* m_pRadioNotClear;
	wxRadioButton* m_pRadioClearAll;
	wxRadioButton* m_pRadioClearPriceAndVol;
	wxRadioButton* m_pRadioClearHand;

	wxSpinCtrl* m_pSpinCtrl;
	wxExtListCtrl_Obsolete* m_pList;
	wxButton*  BtSet;

	bool m_bText;
	bool m_bChanged;

	wxTimer m_timer;
};


