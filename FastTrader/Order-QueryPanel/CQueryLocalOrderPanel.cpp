#include "StdAfx.h"

#include <stdlib.h>
#include <string.h>
#include "CQueryLocalOrderPanel.h"
#include "../ConfigApp/Const.h"
#include "../inc/Module-Misc/zqControl.h"

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;


BEGIN_EVENT_TABLE(CQueryLocalOrderPanel, wxPanel)
	EVT_SIZE(OnSize )
    EVT_TIMER(ID_QUERYLOCAL_ASYNCTIMER, OnTimer)
    EVT_RADIOBUTTON(ID_QUERYLOCAL_CONDITION_RADIO,CQueryLocalOrderPanel::OnRadioButton)
    EVT_RADIOBUTTON(ID_QUERYLOCAL_PROFIT_RADIO,CQueryLocalOrderPanel::OnRadioButton)
    EVT_RADIOBUTTON(ID_QUERYLOCAL_POSIPL_RADIO,CQueryLocalOrderPanel::OnRadioButton)
    EVT_RADIOBUTTON(ID_QUERYLOCAL_PARKED_RADIO,CQueryLocalOrderPanel::OnRadioButton)
    EVT_RADIOBUTTON(ID_QUERYLOCAL_BATCH_RADIO,CQueryLocalOrderPanel::OnRadioButton)

	EVT_CHECKBOX(ID_QUERYLOCAL_CONDITION_SHOWDELETE_CHECK, CQueryLocalOrderPanel::OnConditionCheck)
	EVT_CHECKBOX(ID_QUERYLOCAL_CONDITION_SHOWSENDED_CHECK, CQueryLocalOrderPanel::OnConditionCheck)
	EVT_CHECKBOX(ID_QUERYLOCAL_CONDITION_SHOWPROFIT_CHECK, CQueryLocalOrderPanel::OnConditionCheck)
	EVT_BUTTON(ID_QUERYLOCAL_CONDITION_QUERY_BUTTON, CQueryLocalOrderPanel::OnConditionQueryButton)
	EVT_BUTTON(ID_QUERYLOCAL_CONDITION_DELETE_BUTTON, CQueryLocalOrderPanel::OnConditionDeleteButton)
	EVT_BUTTON(ID_QUERYLOCAL_CONDITION_DONOW_BUTTON, CQueryLocalOrderPanel::OnConditionDoNowButton)
	EVT_CHECKBOX(ID_QUERYLOCAL_PROFIT_SHOWDELETE_CHECK, CQueryLocalOrderPanel::OnProfitCheck)
	EVT_BUTTON(ID_QUERYLOCAL_PROFIT_QUERY_BUTTON, CQueryLocalOrderPanel::OnProfitQueryButton)
	EVT_BUTTON(ID_QUERYLOCAL_PROFIT_DELETE_BUTTON, CQueryLocalOrderPanel::OnProfitDeleteButton)
	EVT_BUTTON(ID_QUERYLOCAL_PROFIT_DETAIL_BUTTON, CQueryLocalOrderPanel::OnProfitDetailButton)
	EVT_CHECKBOX(ID_QUERYLOCAL_POSIPL_SHOWDELETE_CHECK, CQueryLocalOrderPanel::OnPosiPLCheck)
	EVT_BUTTON(ID_QUERYLOCAL_POSIPL_QUERY_BUTTON, CQueryLocalOrderPanel::OnPosiPLQueryButton)
	EVT_BUTTON(ID_QUERYLOCAL_POSIPL_DELETE_BUTTON, CQueryLocalOrderPanel::OnPosiPLDeleteButton)
	EVT_CHECKBOX(ID_QUERYLOCAL_PARKED_SHOWDELETE_CHECK, CQueryLocalOrderPanel::OnParkedCheck)
	EVT_CHECKBOX(ID_QUERYLOCAL_PARKED_SHOWSENDED_CHECK, CQueryLocalOrderPanel::OnParkedCheck)
	EVT_BUTTON(ID_QUERYLOCAL_PARKED_QUERY_BUTTON, CQueryLocalOrderPanel::OnParkedQueryButton)
	EVT_BUTTON(ID_QUERYLOCAL_PARKED_DELETE_BUTTON, CQueryLocalOrderPanel::OnParkedDeleteButton)
	EVT_BUTTON(ID_QUERYLOCAL_PARKED_DONOW_BUTTON, CQueryLocalOrderPanel::OnParkedDoNowButton)
	EVT_BUTTON(ID_QUERYLOCAL_BATCH_QUERY_BUTTON, CQueryLocalOrderPanel::OnBatchQueryButton)

	EVT_CFG_CHANGE(OnCfgChanged)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChange)
END_EVENT_TABLE()

CQueryLocalOrderPanel::CQueryLocalOrderPanel(wxWindow *parent,
            wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name):
	wxPanel(parent,winid, pos,size, style, name)
    , m_timer(this, ID_QUERYLOCAL_ASYNCTIMER)
{
	
	m_nCurrRadioId = -1;
	m_poCurrPanel = NULL;

	SetBackgroundColour(DEFAULT_COLOUR);

	int nNextPosX=0;

	m_poRadioCondition = NULL;
	m_poConditionPanel = NULL;
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_CONDITION ))
	{
		m_poRadioCondition = new wxRadioButton(this, ID_QUERYLOCAL_CONDITION_RADIO, LOADSTRING(QUERY_LOCALORDER_CONDITION), wxPoint(5, 1), wxSize(43, CommonBtnHeight));	//条件单
		m_poConditionPanel = new CQueryOrderConditionPanel(this);
		m_poConditionPanel->Show(false);
		nNextPosX += 45;
	}
	m_poRadioPosiPL = NULL;
	m_poPosiPLPanel = NULL;
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_POSIPL ))
	{
		m_poRadioPosiPL = new wxRadioButton(this, ID_QUERYLOCAL_POSIPL_RADIO, LOADSTRING(QUERY_LOCALORDER_POSIPL), wxPoint(5+nNextPosX, 1), wxSize(43+39, CommonBtnHeight));			//盈损单
		m_poPosiPLPanel = new CQueryOrderPosiPLPanel(this);
		m_poPosiPLPanel->Show(false);
		nNextPosX += 45+39;
	}
	m_poRadioProfit = NULL;
	m_poProfitPanel = NULL;
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_POSITION ))
	{
		m_poRadioProfit = new wxRadioButton(this, ID_QUERYLOCAL_PROFIT_RADIO, LOADSTRING(QUERY_LOCALORDER_PROFIT), wxPoint(5+nNextPosX, 1), wxSize(43, CommonBtnHeight));			//盈损单
		m_poProfitPanel = new CQueryOrderPositionPanel(this);
		m_poProfitPanel->Show(false);
		nNextPosX += 45;
	}
	m_poRadioParked = NULL;
	m_poParkedPanel = NULL;
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_PARKED ))
	{
		m_poRadioParked = new wxRadioButton(this, ID_QUERYLOCAL_PARKED_RADIO, LOADSTRING(QUERY_LOCALORDER_PARKED), wxPoint(5+nNextPosX, 1), wxSize(43, CommonBtnHeight));			//预埋单
		m_poParkedPanel = new CQueryOrderParkedPanel(this);
		m_poParkedPanel->Show(false);
		nNextPosX += 45;
	}
	m_poRadioBatch = NULL;
	m_poBatchPanel = NULL;
	if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsModuleValid( GID_QUERY_ORDER_PARKED ))
	{
		m_poRadioBatch = new wxRadioButton(this, ID_QUERYLOCAL_BATCH_RADIO, LOADSTRING(QUERY_LOCALORDER_BATCH), wxPoint(5+nNextPosX, 1), wxSize(43, CommonBtnHeight));				//批量单
		m_poBatchPanel = new CQueryOrderBatchPanel(this);
		m_poBatchPanel->Show(false);
		nNextPosX += 45;
	}
	//m_poCurrPanel = m_poConditionPanel;

	m_poStaticText = new wxStaticText(this, ID_QUERYLOCAL_STATICTEXT, "|", wxPoint(5+nNextPosX, 1), wxSize(15, CommonBtnHeight));

	nNextPosX+=10+10;

	m_poCondition_showdelete_check = NULL;
	m_poCondition_showsended_check = NULL;
	m_poCondition_showprofit_check = NULL;
	m_poCondition_query_button = NULL;
	m_poCondition_delete_button = NULL;
	m_poCondition_donow_button = NULL;
	if(m_poRadioCondition)
	{
		m_poCondition_showdelete_check = new wxCheckBox( this, ID_QUERYLOCAL_CONDITION_SHOWDELETE_CHECK, 
				LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(nNextPosX, 1), wxSize(14*5, CommonBtnHeight));//, 0 );
		m_poCondition_showsended_check = new wxCheckBox( this, ID_QUERYLOCAL_CONDITION_SHOWSENDED_CHECK, 
				LOADSTRING(OIP_SENDED), wxPoint(nNextPosX+14*5, 1),  wxSize(14*4, CommonBtnHeight));//, 0 );//18
		// 保留创建控件代码，方便重新开启
		//m_poCondition_showprofit_check = new wxCheckBox( this, ID_QUERYLOCAL_CONDITION_SHOWPROFIT_CHECK, 
		//		LOADSTRING(BTN_QP_HIDESTEOPCONDITION), wxPoint(nNextPosX+14*5+14*4, 1),  wxSize(14*8, CommonBtnHeight));//, 0 );//18

		m_poCondition_query_button = new wxButton(this, ID_QUERYLOCAL_CONDITION_QUERY_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(nNextPosX+14*5+14*4, 1), wxSize(14*3, CommonBtnHeight));//240
		m_poCondition_delete_button = new wxButton(this, ID_QUERYLOCAL_CONDITION_DELETE_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONDELETE), wxPoint(nNextPosX+14*5+14*4+14*3, 1), wxSize(14*3, CommonBtnHeight));
		m_poCondition_donow_button = new wxButton(this, ID_QUERYLOCAL_CONDITION_DONOW_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONDONOW), wxPoint(nNextPosX+14*5+14*4+14*3+14*3, 1), wxSize(14*4, CommonBtnHeight));
		// 保留创建控件代码，方便重新开启
		//m_poCondition_query_button = new wxButton(this, ID_QUERYLOCAL_CONDITION_QUERY_BUTTON, 
		//		LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(nNextPosX+14*5+14*4+14*8, 1), wxSize(14*3, CommonBtnHeight));//240
		//m_poCondition_delete_button = new wxButton(this, ID_QUERYLOCAL_CONDITION_DELETE_BUTTON, 
		//		LOADSTRING(BTN_QP_BUTTONDELETE), wxPoint(nNextPosX+14*5+14*4+14*8+14*3, 1), wxSize(14*3, CommonBtnHeight));
		//m_poCondition_donow_button = new wxButton(this, ID_QUERYLOCAL_CONDITION_DONOW_BUTTON, 
		//		LOADSTRING(BTN_QP_BUTTONDONOW), wxPoint(nNextPosX+14*5+14*4+14*8+14*3+14*3, 1), wxSize(14*4, CommonBtnHeight));
	}

	m_poProfit_showdelete_check = NULL;
	m_poProfit_query_button = NULL;
	m_poProfit_delete_button = NULL;
	m_poProfit_detail_button = NULL;
	if(m_poRadioProfit)
	{
		//m_poProfit_showdelete_check = new wxCheckBox( this, ID_QUERYLOCAL_PROFIT_SHOWDELETE_CHECK, 
		//		LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(nNextPosX, 1), wxSize(14*5, CommonBtnHeight));//, 0 );
		//m_poProfit_showdelete_check->Show(false);

		//m_poProfit_query_button = new wxButton(this, ID_QUERYLOCAL_PROFIT_QUERY_BUTTON, 
		//		LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(nNextPosX+14*5, 1), wxSize(14*3, CommonBtnHeight));
		//m_poProfit_delete_button = new wxButton(this, ID_QUERYLOCAL_PROFIT_DELETE_BUTTON, 
		//		LOADSTRING(BTN_QP_BUTTONDELETE), wxPoint(nNextPosX+14*5+14*3, 1), wxSize(14*3, CommonBtnHeight));
		m_poProfit_query_button = new wxButton(this, ID_QUERYLOCAL_PROFIT_QUERY_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(nNextPosX, 1), wxSize(14*3, CommonBtnHeight));
		m_poProfit_delete_button = new wxButton(this, ID_QUERYLOCAL_PROFIT_DELETE_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONDELETE), wxPoint(nNextPosX+14*3, 1), wxSize(14*3, CommonBtnHeight));
		//m_poProfit_detail_button = new wxButton(this, ID_QUERYLOCAL_PROFIT_DETAIL_BUTTON, 
		//		LOADSTRING(BTN_QP_BUTTONDETAIL), wxPoint(nNextPosX+14*3+14*3, 1), wxSize(14*4, CommonBtnHeight));
	}

	m_poPosiPL_showdelete_check = NULL;
	m_poPosiPL_query_button = NULL;
	m_poPosiPL_delete_button = NULL;
	if(m_poRadioPosiPL)
	{

		//m_poPosiPL_showdelete_check = new wxCheckBox( this, ID_QUERYLOCAL_POSIPL_SHOWDELETE_CHECK, 
		//		LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(nNextPosX, 1), wxSize(14*5, CommonBtnHeight));//, 0 );
		//m_poPosiPL_showdelete_check->Show(false);

		//m_poPosiPL_query_button = new wxButton(this, ID_QUERYLOCAL_POSIPL_QUERY_BUTTON, 
		//		LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(nNextPosX+14*5, 1), wxSize(14*3, CommonBtnHeight));
		//m_poPosiPL_delete_button = new wxButton(this, ID_QUERYLOCAL_POSIPL_DELETE_BUTTON, 
		//		LOADSTRING(BTN_QP_BUTTONDELETE), wxPoint(nNextPosX+14*5+14*3, 1), wxSize(14*3, CommonBtnHeight));
		m_poPosiPL_query_button = new wxButton(this, ID_QUERYLOCAL_POSIPL_QUERY_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(nNextPosX, 1), wxSize(14*3, CommonBtnHeight));
		m_poPosiPL_delete_button = new wxButton(this, ID_QUERYLOCAL_POSIPL_DELETE_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONDELETE), wxPoint(nNextPosX+14*3, 1), wxSize(14*3, CommonBtnHeight));
	}

	m_poParked_showdelete_check = NULL;
	m_poParked_showsended_check = NULL;
	m_poParked_query_button = NULL;
	m_poParked_delete_button = NULL;
	m_poParked_donow_button = NULL;
	if(m_poRadioParked)
	{
		m_poParked_showdelete_check = new wxCheckBox( this, ID_QUERYLOCAL_PARKED_SHOWDELETE_CHECK, 
				LOADSTRING(BTN_QP_SHOWDELETED), wxPoint(nNextPosX, 1), wxSize(14*5, CommonBtnHeight));
		m_poParked_showsended_check = new wxCheckBox(this, ID_QUERYLOCAL_PARKED_SHOWSENDED_CHECK, 
				LOADSTRING(OIP_SENDED), wxPoint(nNextPosX+14*5, 1),  wxSize(14*4, CommonBtnHeight));

		m_poParked_query_button = new wxButton(this, ID_QUERYLOCAL_PARKED_QUERY_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(nNextPosX+14*5+14*4, 1), wxSize(14*3, CommonBtnHeight));
		m_poParked_delete_button = new wxButton(this, ID_QUERYLOCAL_PARKED_DELETE_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONDELETE), wxPoint(nNextPosX+14*5+14*4+14*3, 1), wxSize(14*3, CommonBtnHeight));
		m_poParked_donow_button = new wxButton(this, ID_QUERYLOCAL_PARKED_DONOW_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONDONOW), wxPoint(nNextPosX+14*5+14*4+14*3+14*3, 1), wxSize(14*4, CommonBtnHeight));
	}

	m_poBatch_query_button = NULL;
	if(m_poRadioBatch)
	{
		m_poBatch_query_button = new wxButton(this, ID_QUERYLOCAL_BATCH_QUERY_BUTTON, 
				LOADSTRING(BTN_QP_BUTTONQUERY), wxPoint(nNextPosX, 1), wxSize(14*3, CommonBtnHeight));
	}

	int nCurrID = -1;
	if(	m_poRadioCondition)
	{
		m_poRadioCondition->SetValue(true);
		nCurrID = ID_QUERYLOCAL_CONDITION_RADIO;
	}
	else if(m_poRadioProfit)
	{
		m_poRadioProfit->SetValue(true);
		nCurrID = ID_QUERYLOCAL_PROFIT_RADIO;
	}
	else if(m_poRadioParked)
	{
		m_poRadioParked->SetValue(true);
		nCurrID = ID_QUERYLOCAL_PARKED_RADIO;
	}
	else if(m_poRadioBatch)
	{
		m_poRadioBatch->SetValue(true);
		nCurrID = ID_QUERYLOCAL_BATCH_RADIO;
	}

	Init();
	ShowPanel(nCurrID);
    DoInitQryStep();

    m_timer.Start(25);

}

CQueryLocalOrderPanel::~CQueryLocalOrderPanel(void)
{
	m_timer.Stop();
}

int CQueryLocalOrderPanel::GetRadioSelected()
{
	if(m_poRadioCondition)
	{
		if(m_poRadioCondition->GetValue())
			return m_poRadioCondition->GetId();
	}
	if(m_poRadioProfit)
	{
		if(m_poRadioProfit->GetValue())
			return m_poRadioProfit->GetId();
	}
	if(m_poRadioPosiPL)
	{
		if(m_poRadioPosiPL->GetValue())
			return m_poRadioPosiPL->GetId();
	}
	if(m_poRadioParked)
	{
		if(m_poRadioParked->GetValue())
			return m_poRadioParked->GetId();
	}
	if(m_poRadioBatch)
	{
		if(m_poRadioBatch->GetValue())
			return m_poRadioBatch->GetId();
	}
	return -1;
}

bool CQueryLocalOrderPanel::Init()
{
    PROCESS_LANGUAGE_EVENT();
    return true;
}

void CQueryLocalOrderPanel::ShowPanel(int nID)
{
	if(nID<0 || nID==m_nCurrRadioId)
		return;

	wxWindow* pWin = NULL;

	if(m_poCurrPanel)
		m_poCurrPanel->Show(false);
	for(int i=ID_QUERYLOCAL_FIRST; i<ID_QUERYLOCAL_SUM; i++)
	{
		pWin = FindWindowById(i, this);
		if(pWin)
			pWin->Show(false);
	}
	switch(nID)
	{
	    case ID_QUERYLOCAL_CONDITION_RADIO:
			m_poCurrPanel = m_poConditionPanel;
			if(m_poCondition_showdelete_check)
				m_poCondition_showdelete_check->Show();
			if(m_poCondition_showsended_check)
				m_poCondition_showsended_check->Show();
			if(m_poCondition_showprofit_check)
				m_poCondition_showprofit_check->Show();

			if(m_poCondition_query_button)
				m_poCondition_query_button->Show();
			if(m_poCondition_delete_button)
				m_poCondition_delete_button->Show();
			if(m_poCondition_donow_button)
				m_poCondition_donow_button->Show();
		    break;
	    case ID_QUERYLOCAL_PROFIT_RADIO:
			m_poCurrPanel = m_poProfitPanel;
			if(m_poProfit_showdelete_check)
				m_poProfit_showdelete_check->Show();

			if(m_poProfit_query_button)
				m_poProfit_query_button->Show();
			if(m_poProfit_delete_button)
				m_poProfit_delete_button->Show();
			if(m_poProfit_detail_button)
				m_poProfit_detail_button->Show();
		    break;
	    case ID_QUERYLOCAL_POSIPL_RADIO:
			m_poCurrPanel = m_poPosiPLPanel;
			if(m_poPosiPL_showdelete_check)
				m_poPosiPL_showdelete_check->Show();

			if(m_poPosiPL_query_button)
				m_poPosiPL_query_button->Show();
			if(m_poPosiPL_delete_button)
				m_poPosiPL_delete_button->Show();
		    break;
	    case ID_QUERYLOCAL_PARKED_RADIO:
			m_poCurrPanel = m_poParkedPanel;
			if(m_poParked_query_button)
				m_poParked_query_button->Show();
			if(m_poParked_delete_button)
				m_poParked_delete_button->Show();
			if(m_poParked_donow_button)
				m_poParked_donow_button->Show();

			if(m_poParked_showdelete_check)
				m_poParked_showdelete_check->Show();
			if(m_poParked_showsended_check)
				m_poParked_showsended_check->Show();
		    break;
	    case ID_QUERYLOCAL_BATCH_RADIO:
			m_poCurrPanel = m_poBatchPanel;
			if(m_poBatch_query_button)
				m_poBatch_query_button->Show();
		    break;
	    default:
		    break;
	}
	m_nCurrRadioId = nID;
	if(m_poCurrPanel)
		m_poCurrPanel->Show(true);

}

void CQueryLocalOrderPanel::OnRadioButton(wxCommandEvent& evt)
{
	ShowPanel(evt.GetId());
}

void CQueryLocalOrderPanel::OnSize( wxSizeEvent& event )
{
	wxRect rt=GetRect();

	if(m_poConditionPanel)
		m_poConditionPanel->SetSize(rt.width, rt.height-CommonBtnHeight-2);
	if(m_poProfitPanel)
		m_poProfitPanel->SetSize(rt.width, rt.height-CommonBtnHeight-2);
	if(m_poPosiPLPanel)
		m_poPosiPLPanel->SetSize(rt.width, rt.height-CommonBtnHeight-2);
	if(m_poParkedPanel)
		m_poParkedPanel->SetSize(rt.width, rt.height-CommonBtnHeight-2);
	if(m_poBatchPanel)
		m_poBatchPanel->SetSize(rt.width, rt.height-CommonBtnHeight-2);

	wxWindow* pWin = NULL;

	for(int i=ID_QUERYLOCAL_CONDITION_RADIO; i<ID_QUERYLOCAL_SUM; i++)
	{
		pWin = FindWindowById(i, this);
		if(pWin)
		{
			wxPoint pos = pWin->GetPosition();
			pos.y = rt.height-2-CommonBtnHeight;
			if(i==ID_QUERYLOCAL_STATICTEXT)
				pos.y+=2;
			pWin->SetPosition(pos);
		}
	}

	event.Skip();
}

//配置文件发生变化
void CQueryLocalOrderPanel::OnCfgChanged(wxCommandEvent& evt)
{
	//Init();
	if(m_poConditionPanel)
		m_poConditionPanel->OnCfgChanged(evt);
	if(m_poProfitPanel)
		m_poProfitPanel->OnCfgChanged(evt);
	if(m_poPosiPLPanel)
		m_poPosiPLPanel->OnCfgChanged(evt);
	if(m_poParkedPanel)
		m_poParkedPanel->OnCfgChanged(evt);
	if(m_poBatchPanel)
		m_poBatchPanel->OnCfgChanged(evt);
	//ShowPanel(GetRadioSelected());
}

void CQueryLocalOrderPanel::OnLanguageChange(wxCommandEvent& evt)
{

	if(m_poConditionPanel)
	{
		wxCommandEvent newEvt(wxEVT_LANGUAGE_CHANGE);
		m_poConditionPanel->OnLanguageChange(newEvt);
	}
	if(m_poProfitPanel)
	{
		wxCommandEvent newEvt(wxEVT_LANGUAGE_CHANGE);
		m_poProfitPanel->OnLanguageChange(newEvt);
	}
	if(m_poPosiPLPanel)
	{
		wxCommandEvent newEvt(wxEVT_LANGUAGE_CHANGE);
		m_poPosiPLPanel->OnLanguageChange(newEvt);
	}
	if(m_poParkedPanel)
	{
		wxCommandEvent newEvt(wxEVT_LANGUAGE_CHANGE);
		m_poParkedPanel->OnLanguageChange(newEvt);
	}
	if(m_poBatchPanel)
	{
		wxCommandEvent newEvt(wxEVT_LANGUAGE_CHANGE);
		m_poBatchPanel->OnLanguageChange(newEvt);
	}

	if(FindWindowById(ID_QUERYLOCAL_CONDITION_RADIO, this))
		FindWindowById(ID_QUERYLOCAL_CONDITION_RADIO, this)->SetLabel(LOADSTRING(QUERY_LOCALORDER_CONDITION));
	if(FindWindowById(ID_QUERYLOCAL_PROFIT_RADIO, this))
		FindWindowById(ID_QUERYLOCAL_PROFIT_RADIO, this)->SetLabel(LOADSTRING(QUERY_LOCALORDER_PROFIT));
	if(FindWindowById(ID_QUERYLOCAL_POSIPL_RADIO, this))
		FindWindowById(ID_QUERYLOCAL_POSIPL_RADIO, this)->SetLabel(LOADSTRING(QUERY_LOCALORDER_POSIPL));
	if(FindWindowById(ID_QUERYLOCAL_PARKED_RADIO, this))
		FindWindowById(ID_QUERYLOCAL_PARKED_RADIO, this)->SetLabel(LOADSTRING(QUERY_LOCALORDER_PARKED));
	if(FindWindowById(ID_QUERYLOCAL_BATCH_RADIO, this))
		FindWindowById(ID_QUERYLOCAL_BATCH_RADIO, this)->SetLabel(LOADSTRING(QUERY_LOCALORDER_BATCH));

	if(FindWindowById(ID_QUERYLOCAL_CONDITION_SHOWDELETE_CHECK, this))
		FindWindowById(ID_QUERYLOCAL_CONDITION_SHOWDELETE_CHECK, this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));
	if(FindWindowById(ID_QUERYLOCAL_CONDITION_SHOWSENDED_CHECK, this))
		FindWindowById(ID_QUERYLOCAL_CONDITION_SHOWSENDED_CHECK, this)->SetLabel(LOADSTRING(OIP_SENDED));
	if(FindWindowById(ID_QUERYLOCAL_CONDITION_SHOWPROFIT_CHECK, this))
		FindWindowById(ID_QUERYLOCAL_CONDITION_SHOWPROFIT_CHECK, this)->SetLabel(LOADSTRING(BTN_QP_HIDESTEOPCONDITION));

	if(FindWindowById(ID_QUERYLOCAL_CONDITION_QUERY_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_CONDITION_QUERY_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	if(FindWindowById(ID_QUERYLOCAL_CONDITION_DELETE_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_CONDITION_DELETE_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));
	if(FindWindowById(ID_QUERYLOCAL_CONDITION_DONOW_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_CONDITION_DONOW_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONDONOW));

	if(FindWindowById(ID_QUERYLOCAL_PROFIT_SHOWDELETE_CHECK, this))
		FindWindowById(ID_QUERYLOCAL_PROFIT_SHOWDELETE_CHECK, this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));

	if(FindWindowById(ID_QUERYLOCAL_PROFIT_QUERY_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_PROFIT_QUERY_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	if(FindWindowById(ID_QUERYLOCAL_PROFIT_DELETE_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_PROFIT_DELETE_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));
	if(FindWindowById(ID_QUERYLOCAL_PROFIT_DETAIL_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_PROFIT_DETAIL_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONDETAIL));

	if(FindWindowById(ID_QUERYLOCAL_POSIPL_SHOWDELETE_CHECK, this))
		FindWindowById(ID_QUERYLOCAL_POSIPL_SHOWDELETE_CHECK, this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));

	if(FindWindowById(ID_QUERYLOCAL_POSIPL_QUERY_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_POSIPL_QUERY_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	if(FindWindowById(ID_QUERYLOCAL_POSIPL_DELETE_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_POSIPL_DELETE_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));

	if(FindWindowById(ID_QUERYLOCAL_PARKED_SHOWDELETE_CHECK, this))
		FindWindowById(ID_QUERYLOCAL_PARKED_SHOWDELETE_CHECK, this)->SetLabel(LOADSTRING(BTN_QP_SHOWDELETED));
	if(FindWindowById(ID_QUERYLOCAL_PARKED_SHOWSENDED_CHECK, this))
		FindWindowById(ID_QUERYLOCAL_PARKED_SHOWSENDED_CHECK, this)->SetLabel(LOADSTRING(OIP_SENDED));

	if(FindWindowById(ID_QUERYLOCAL_PARKED_QUERY_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_PARKED_QUERY_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));
	if(FindWindowById(ID_QUERYLOCAL_PARKED_DELETE_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_PARKED_DELETE_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONDELETE));
	if(FindWindowById(ID_QUERYLOCAL_PARKED_DONOW_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_PARKED_DONOW_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONDONOW));

	if(FindWindowById(ID_QUERYLOCAL_BATCH_QUERY_BUTTON, this))
		FindWindowById(ID_QUERYLOCAL_BATCH_QUERY_BUTTON, this)->SetLabel(LOADSTRING(BTN_QP_BUTTONQUERY));

}

void CQueryLocalOrderPanel::DoInitQryStep()
{
    //ShowOrderBatchs();
}

void CQueryLocalOrderPanel::OnTimer(wxTimerEvent& event)
{
	if(m_poConditionPanel!=NULL && m_poRadioCondition!=NULL 
			&& m_poRadioCondition->GetValue())
	{
		wxButton* pButton = (wxButton*)FindWindowById(
				CQueryOrderConditionPanel::ID_QUERYCONDITION_BUTTONDONOW, 
				m_poConditionPanel);
		if(pButton!=NULL) 
		{
			bool bIsEnable = !(pButton->IsEnabled()==false || pButton->IsShown()==false);
			if(m_poCondition_donow_button!=NULL)
				m_poCondition_donow_button->Enable(bIsEnable);
		}
	}
	if(m_poParkedPanel!=NULL && m_poRadioParked!=NULL 
			&& m_poRadioParked->GetValue())
	{
		wxButton* pButton = (wxButton*)FindWindowById(
				CQueryOrderParkedPanel::ID_QUERYPARKED_BUTTONDONOW, 
				m_poParkedPanel);
		if(pButton!=NULL) 
		{
			bool bIsEnable = !(pButton->IsEnabled()==false || pButton->IsShown()==false);
			if(m_poParked_donow_button!=NULL)
				m_poParked_donow_button->Enable(bIsEnable);
		}
	}

}

void CQueryLocalOrderPanel::OnConditionCheck(wxCommandEvent& evt)
{
	if(m_poConditionPanel==NULL)
		return;
	switch(evt.GetId())
	{
		case ID_QUERYLOCAL_CONDITION_SHOWDELETE_CHECK:
			{
				wxCheckBox* poCheck = (wxCheckBox*)FindWindowById(CQueryOrderConditionPanel::ID_QUERYCONDITION_CHECKSHOWDELETE, this);
				if(poCheck) {
					poCheck->SetValue(evt.GetInt()!=0);
					wxCommandEvent newEvt(wxEVT_COMMAND_CHECKBOX_CLICKED, CQueryOrderConditionPanel::ID_QUERYCONDITION_CHECKSHOWDELETE);
					newEvt.SetInt(evt.GetInt());
					m_poConditionPanel->OnButtonQuery(newEvt);
				}
			}
			break;
		case ID_QUERYLOCAL_CONDITION_SHOWSENDED_CHECK:
			{
				wxCheckBox* poCheck = (wxCheckBox*)FindWindowById(CQueryOrderConditionPanel::ID_QUERYCONDITION_CHECKSHOWCONFIRM, this);
				if(poCheck) {
					poCheck->SetValue(evt.GetInt()!=0);
					wxCommandEvent newEvt(wxEVT_COMMAND_CHECKBOX_CLICKED, CQueryOrderConditionPanel::ID_QUERYCONDITION_CHECKSHOWCONFIRM);
					newEvt.SetInt(evt.GetInt());
					m_poConditionPanel->OnButtonQuery(newEvt);
				}
				//if(evt.GetInt()==0)
				//{
				//	wxRadioButton* poRadio = (wxRadioButton*)FindWindowById(CQueryOrderConditionPanel::ID_QUERYCONDITION_RADIOSHOWALL, this);
				//	if(poRadio) {
				//		poRadio->SetValue(1);
				//		wxCommandEvent newEvt(wxEVT_COMMAND_RADIOBUTTON_SELECTED, CQueryOrderConditionPanel::ID_QUERYCONDITION_RADIOSHOWALL);
				//		newEvt.SetInt(1);
				//		m_poConditionPanel->OnButtonQuery(newEvt);
				//	}
				//}
				//else 
				//{
				//	wxRadioButton* poRadio = (wxRadioButton*)FindWindowById(CQueryOrderConditionPanel::ID_QUERYCONDITION_RADIOSHOWCONFIRM, this);
				//	if(poRadio) {
				//		poRadio->SetValue(1);
				//		wxCommandEvent newEvt(wxEVT_COMMAND_RADIOBUTTON_SELECTED, CQueryOrderConditionPanel::ID_QUERYCONDITION_RADIOSHOWCONFIRM);
				//		newEvt.SetInt(1);
				//		m_poConditionPanel->OnButtonQuery(newEvt);
				//	}
				//}
			}
			break;
		case ID_QUERYLOCAL_CONDITION_SHOWPROFIT_CHECK:
			{
				wxCheckBox* poCheck = (wxCheckBox*)FindWindowById(CQueryOrderConditionPanel::ID_QUERYCONDITION_CHECKSHOWPOSITION, this);
				if(poCheck) {
					poCheck->SetValue(evt.GetInt()!=0);
					wxCommandEvent newEvt(wxEVT_COMMAND_CHECKBOX_CLICKED, CQueryOrderConditionPanel::ID_QUERYCONDITION_CHECKSHOWPOSITION);
					newEvt.SetInt(evt.GetInt());
					m_poConditionPanel->OnButtonQuery(newEvt);
				}
			}
			break;
		default:
			break;
	}
}

void CQueryLocalOrderPanel::OnConditionQueryButton(wxCommandEvent& evt)
{
	if(m_poConditionPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderConditionPanel::ID_QUERYCONDITION_BUTTONQUERY);
	newEvt.SetInt(evt.GetInt());
	m_poConditionPanel->OnButtonQuery(newEvt);
}

void CQueryLocalOrderPanel::OnConditionDeleteButton(wxCommandEvent& evt)
{
	if(m_poConditionPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderConditionPanel::ID_QUERYCONDITION_BUTTONDELETE);
	newEvt.SetInt(evt.GetInt());
	m_poConditionPanel->OnButtonDelete(newEvt);
}

void CQueryLocalOrderPanel::OnConditionDoNowButton(wxCommandEvent& evt)
{
	if(m_poConditionPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderConditionPanel::ID_QUERYCONDITION_BUTTONDONOW);
	newEvt.SetInt(evt.GetInt());
	m_poConditionPanel->OnButtonDoNow(newEvt);
}

void CQueryLocalOrderPanel::OnProfitCheck(wxCommandEvent& evt)
{
	if(m_poProfitPanel==NULL)
		return;
	switch(evt.GetId())
	{
		case ID_QUERYLOCAL_PROFIT_SHOWDELETE_CHECK:
			{
				wxCheckBox* poCheck = (wxCheckBox*)FindWindowById(CQueryOrderPositionPanel::ID_QUERYPOSITION_CHECKSHOWDELETE, this);
				if(poCheck) {
					poCheck->SetValue(evt.GetInt()!=0);
					wxCommandEvent newEvt(wxEVT_COMMAND_CHECKBOX_CLICKED, CQueryOrderPositionPanel::ID_QUERYPOSITION_CHECKSHOWDELETE);
					newEvt.SetInt(evt.GetInt());
					m_poProfitPanel->OnButtonQuery(newEvt);
				}
			}
			break;
		default:
			break;
	}
}

void CQueryLocalOrderPanel::OnProfitQueryButton(wxCommandEvent& evt)
{
	if(m_poProfitPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderPositionPanel::ID_QUERYPOSITION_BUTTONQUERY);
	newEvt.SetInt(evt.GetInt());
	m_poProfitPanel->OnButtonQuery(newEvt);
}

void CQueryLocalOrderPanel::OnProfitDeleteButton(wxCommandEvent& evt)
{
	if(m_poProfitPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderPositionPanel::ID_QUERYPOSITION_BUTTONDELETE);
	newEvt.SetInt(evt.GetInt());
	m_poProfitPanel->OnButtonDelete(newEvt);
}

void CQueryLocalOrderPanel::OnProfitDetailButton(wxCommandEvent& evt)
{
	if(m_poProfitPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderPositionPanel::ID_QUERYPOSITION_BUTTONPLDETAIL);
	newEvt.SetInt(evt.GetInt());
	m_poProfitPanel->OnButtonPLDetail(newEvt);
}

void CQueryLocalOrderPanel::OnPosiPLCheck(wxCommandEvent& evt)
{
	if(m_poProfitPanel==NULL)
		return;
	switch(evt.GetId())
	{
		case ID_QUERYLOCAL_POSIPL_SHOWDELETE_CHECK:
			{
				wxCheckBox* poCheck = (wxCheckBox*)FindWindowById(CQueryOrderPosiPLPanel::ID_QUERYPOSIPL_CHECKSHOWDELETE, this);
				if(poCheck) {
					poCheck->SetValue(evt.GetInt()!=0);
					wxCommandEvent newEvt(wxEVT_COMMAND_CHECKBOX_CLICKED, CQueryOrderPosiPLPanel::ID_QUERYPOSIPL_CHECKSHOWDELETE);
					newEvt.SetInt(evt.GetInt());
					m_poPosiPLPanel->OnButtonQuery(newEvt);
				}
			}
			break;
		default:
			break;
	}
}

void CQueryLocalOrderPanel::OnPosiPLQueryButton(wxCommandEvent& evt)
{
	if(m_poProfitPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderPosiPLPanel::ID_QUERYPOSIPL_BUTTONQUERY);
	newEvt.SetInt(evt.GetInt());
	m_poPosiPLPanel->OnButtonQuery(newEvt);
}

void CQueryLocalOrderPanel::OnPosiPLDeleteButton(wxCommandEvent& evt)
{
	if(m_poProfitPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderPosiPLPanel::ID_QUERYPOSIPL_BUTTONDELETE);
	newEvt.SetInt(evt.GetInt());
	m_poPosiPLPanel->OnButtonDelete(newEvt);
}

void CQueryLocalOrderPanel::OnParkedCheck(wxCommandEvent& evt)
{
	if(m_poParkedPanel==NULL)
		return;
	switch(evt.GetId())
	{
		case ID_QUERYLOCAL_PARKED_SHOWDELETE_CHECK:
			{
				wxCheckBox* poCheck = (wxCheckBox*)FindWindowById(CQueryOrderParkedPanel::ID_QUERYPARKED_CHECKSHOWDELETE, this);
				if(poCheck) {
					poCheck->SetValue(evt.GetInt()!=0);
					wxCommandEvent newEvt(wxEVT_COMMAND_CHECKBOX_CLICKED, CQueryOrderParkedPanel::ID_QUERYPARKED_CHECKSHOWDELETE);
					newEvt.SetInt(evt.GetInt());
					m_poParkedPanel->OnButtonQuery(newEvt);
				}
			}
			break;
		case ID_QUERYLOCAL_PARKED_SHOWSENDED_CHECK:
			{
				wxCheckBox* poCheck = (wxCheckBox*)FindWindowById(CQueryOrderParkedPanel::ID_QUERYPARKED_CHECKSHOWCONFIRM, this);
				if(poCheck) {
					poCheck->SetValue(evt.GetInt()!=0);
					wxCommandEvent newEvt(wxEVT_COMMAND_CHECKBOX_CLICKED, CQueryOrderParkedPanel::ID_QUERYPARKED_CHECKSHOWCONFIRM);
					newEvt.SetInt(evt.GetInt());
					m_poParkedPanel->OnButtonQuery(newEvt);
				}
				//if(evt.GetInt()==0)
				//{
				//	wxRadioButton* poRadio = (wxRadioButton*)FindWindowById(CQueryOrderParkedPanel::ID_QUERYPARKED_RADIOSHOWALL, this);
				//	if(poRadio) {
				//		poRadio->SetValue(1);
				//		wxCommandEvent newEvt(wxEVT_COMMAND_RADIOBUTTON_SELECTED, CQueryOrderParkedPanel::ID_QUERYPARKED_RADIOSHOWALL);
				//		newEvt.SetInt(1);
				//		m_poParkedPanel->OnButtonQuery(newEvt);
				//	}
				//}
				//else 
				//{
				//	wxRadioButton* poRadio = (wxRadioButton*)FindWindowById(CQueryOrderParkedPanel::ID_QUERYPARKED_RADIOSHOWCONFIRM, this);
				//	if(poRadio) {
				//		poRadio->SetValue(1);
				//		wxCommandEvent newEvt(wxEVT_COMMAND_RADIOBUTTON_SELECTED, CQueryOrderParkedPanel::ID_QUERYPARKED_RADIOSHOWCONFIRM);
				//		newEvt.SetInt(1);
				//		m_poParkedPanel->OnButtonQuery(newEvt);
				//	}
				//}
			}
			break;
		default:
			break;
	}
}

void CQueryLocalOrderPanel::OnParkedQueryButton(wxCommandEvent& evt)
{
	if(m_poParkedPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderParkedPanel::ID_QUERYPARKED_BUTTONQUERY);
	newEvt.SetInt(evt.GetInt());
	m_poParkedPanel->OnButtonQuery(newEvt);
}

void CQueryLocalOrderPanel::OnParkedDeleteButton(wxCommandEvent& evt)
{
	if(m_poParkedPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderParkedPanel::ID_QUERYPARKED_BUTTONDELETE);
	newEvt.SetInt(evt.GetInt());
	m_poParkedPanel->OnButtonDelete(newEvt);
}

void CQueryLocalOrderPanel::OnParkedDoNowButton(wxCommandEvent& evt)
{
	if(m_poParkedPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderParkedPanel::ID_QUERYPARKED_BUTTONDONOW);
	newEvt.SetInt(evt.GetInt());
	m_poParkedPanel->OnButtonDoNow(newEvt);
}

void CQueryLocalOrderPanel::OnBatchQueryButton(wxCommandEvent& evt)
{
	if(m_poBatchPanel==NULL)
		return;
	wxCommandEvent newEvt(wxEVT_COMMAND_BUTTON_CLICKED, 
			CQueryOrderBatchPanel::ID_QUERYBATCH_BUTTONQUERY);
	newEvt.SetInt(evt.GetInt());
	m_poBatchPanel->OnButtonQuery(newEvt);
}
