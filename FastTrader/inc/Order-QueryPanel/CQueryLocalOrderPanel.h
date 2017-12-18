#pragma once

#include "../Module-Misc2/tools_util.h"
#include "Module-Misc/ExtListCtrl.h"
#include "cfgMgr/CfgMgr.h"
#include "../ISvr/PlatformSvrMgr.h"
#include "CQueryOrderBatchPanel.h"
#include "CQueryOrderConditionPanel.h"
#include "CQueryOrderParkedPanel.h"
#include "CQueryOrderPositionPanel.h"
#include "CQueryOrderPosiPLPanel.h"

class CQueryLocalOrderPanel: public wxPanel
{
public:
	enum {	//GUI Enum Control ID Start
			ID_QUERYLOCAL_CONDITION_RADIO = 1600,
			ID_QUERYLOCAL_PROFIT_RADIO,
			ID_QUERYLOCAL_POSIPL_RADIO,
			ID_QUERYLOCAL_PARKED_RADIO,
			ID_QUERYLOCAL_BATCH_RADIO,
			ID_QUERYLOCAL_STATICTEXT,
			ID_QUERYLOCAL_ASYNCTIMER, 
			ID_QUERYLOCAL_FIRST,
			ID_QUERYLOCAL_CONDITION_SHOWDELETE_CHECK,
			ID_QUERYLOCAL_CONDITION_SHOWSENDED_CHECK,
			ID_QUERYLOCAL_CONDITION_SHOWPROFIT_CHECK,
			ID_QUERYLOCAL_CONDITION_QUERY_BUTTON,
			ID_QUERYLOCAL_CONDITION_DELETE_BUTTON,
			ID_QUERYLOCAL_CONDITION_DONOW_BUTTON,
			ID_QUERYLOCAL_PROFIT_SHOWDELETE_CHECK,
			ID_QUERYLOCAL_PROFIT_QUERY_BUTTON,
			ID_QUERYLOCAL_PROFIT_DELETE_BUTTON,
			ID_QUERYLOCAL_PROFIT_DETAIL_BUTTON,
			ID_QUERYLOCAL_POSIPL_SHOWDELETE_CHECK,
			ID_QUERYLOCAL_POSIPL_QUERY_BUTTON,
			ID_QUERYLOCAL_POSIPL_DELETE_BUTTON,
			ID_QUERYLOCAL_PARKED_SHOWDELETE_CHECK,
			ID_QUERYLOCAL_PARKED_SHOWSENDED_CHECK,
			ID_QUERYLOCAL_PARKED_QUERY_BUTTON,
			ID_QUERYLOCAL_PARKED_DELETE_BUTTON,
			ID_QUERYLOCAL_PARKED_DONOW_BUTTON,
			ID_QUERYLOCAL_BATCH_QUERY_BUTTON,
			////GUI Enum Control ID End
			ID_QUERYLOCAL_SUM		//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

public:
    CQueryLocalOrderPanel(wxWindow *parent,
            wxWindowID winid = GID_QUERYLOCAL_ORDER,//
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
            const wxString& name = wxPanelNameStr);
	virtual ~CQueryLocalOrderPanel(void);

	void SetLocalOrderService(CLocalOrderService* poLocalOrderService) {
		//m_poLocalOrderService = poLocalOrderService;
	};

private:

	bool Init();
	void ShowPanel(int nID);
	int GetRadioSelected();
	void DoInitQryStep();

	void OnRadioButton(wxCommandEvent& evt);
	
	void OnConditionCheck(wxCommandEvent& evt);
	void OnConditionQueryButton(wxCommandEvent& evt);
	void OnConditionDeleteButton(wxCommandEvent& evt);
	void OnConditionDoNowButton(wxCommandEvent& evt);
	void OnProfitCheck(wxCommandEvent& evt);
	void OnProfitQueryButton(wxCommandEvent& evt);
	void OnProfitDeleteButton(wxCommandEvent& evt);
	void OnProfitDetailButton(wxCommandEvent& evt);
	void OnPosiPLCheck(wxCommandEvent& evt);
	void OnPosiPLQueryButton(wxCommandEvent& evt);
	void OnPosiPLDeleteButton(wxCommandEvent& evt);
	void OnParkedCheck(wxCommandEvent& evt);
	void OnParkedQueryButton(wxCommandEvent& evt);
	void OnParkedDeleteButton(wxCommandEvent& evt);
	void OnParkedDoNowButton(wxCommandEvent& evt);
	void OnBatchQueryButton(wxCommandEvent& evt);
	
    void OnTimer(wxTimerEvent& event);
	void OnSize( wxSizeEvent& evt );
	void OnCfgChanged(wxCommandEvent& evt);
	void OnLanguageChange(wxCommandEvent& evt);

private:
    wxTimer			m_timer;

	wxRadioButton*	m_poRadioCondition;
	wxRadioButton*	m_poRadioProfit;
	wxRadioButton*	m_poRadioPosiPL;
	wxRadioButton*	m_poRadioParked;
	wxRadioButton*	m_poRadioBatch;
	wxStaticText*	m_poStaticText;

	CQueryOrderConditionPanel*	m_poConditionPanel;
	CQueryOrderPositionPanel*	m_poProfitPanel;
	CQueryOrderPosiPLPanel*		m_poPosiPLPanel;
	CQueryOrderParkedPanel*		m_poParkedPanel;
	CQueryOrderBatchPanel*		m_poBatchPanel;

	wxCheckBox*		m_poCondition_showdelete_check;
	wxCheckBox*		m_poCondition_showsended_check;
	wxCheckBox*		m_poCondition_showprofit_check;
	wxButton*		m_poCondition_query_button;
	wxButton*		m_poCondition_delete_button;
	wxButton*		m_poCondition_donow_button;
	wxCheckBox*		m_poProfit_showdelete_check;
	wxButton*		m_poProfit_query_button;
	wxButton*		m_poProfit_delete_button;
	wxButton*		m_poProfit_detail_button;
	wxCheckBox*		m_poPosiPL_showdelete_check;
	wxButton*		m_poPosiPL_query_button;
	wxButton*		m_poPosiPL_delete_button;
	wxCheckBox*		m_poParked_showdelete_check;
	wxCheckBox*		m_poParked_showsended_check;
	wxButton*		m_poParked_query_button;
	wxButton*		m_poParked_delete_button;
	wxButton*		m_poParked_donow_button;
	wxButton*		m_poBatch_query_button;

	int				m_nCurrRadioId;
	wxWindow*		m_poCurrPanel;

	wxFont		m_Font;
	wxColor		m_HeadBackgroundColor;
	wxColor		m_HeadColor;
	wxColor		m_BackgroundColor;
	wxColor		m_TextColor;

DECLARE_EVENT_TABLE()
};
