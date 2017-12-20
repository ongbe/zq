#include "stdafx.h"
#include "../inc/Module-Misc/GLobalFunc.h"
#include "CPromptSetPanel.h"

BEGIN_EVENT_TABLE(CPromptSetPanel,wxPanel)
END_EVENT_TABLE()

CPromptSetPanel::CPromptSetPanel(wxWindow *parent,
								 wxWindowID winid ,
								 const wxPoint& pos ,
								 const wxSize& size ,
								 long style,
								 const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	wxArrayString arryStr;
	arryStr.push_back(LOADSTRING(NO_SOUND));
	arryStr.push_back(LOADSTRING(HARDDISK_SOUND));

	//下单反馈
	new wxStaticBox(this, wxID_ANY, LOADSTRING(ORDER_PROMPT), wxPoint(13, 0), wxSize(420, 110));
	new wxStaticText(this, wxID_ANY, LOADSTRING(SEND_ORDER_OK), wxPoint(20, 22), wxDefaultSize, 0);
	m_chkOpenOrderSuccess = new wxCheckBox(this, wxID_ANY, LOADSTRING(POP_UP_PROMPT_DIALOG), wxPoint(110, 22), wxSize(97, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(SOUND_PROMPT), wxPoint(230, 22), wxDefaultSize, 0);
	m_cmbOpenOrderSuccessSound = new wxOwnerDrawnComboBox(this, wxID_ANY, "", wxPoint(300, 22), wxSize(100, 20), arryStr, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_SEND_ORDER_FAIL), wxPoint(20, 52), wxDefaultSize, 0);
	m_chkOpenOrderFailed = new wxCheckBox(this, wxID_ANY, LOADSTRING(POP_UP_PROMPT_DIALOG), wxPoint(110, 52), wxSize(97, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(SOUND_PROMPT), wxPoint(230, 52), wxDefaultSize, 0);
	m_cmbOpenOrderFailedSound = new wxOwnerDrawnComboBox(this, wxID_ANY, "", wxPoint(300, 52), wxSize(100, 20), arryStr, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticText(this, wxID_ANY, LOADSTRING(ORDER_DEALED), wxPoint(20, 82), wxDefaultSize, 0);
	m_chkOrderDeal = new wxCheckBox(this, wxID_ANY, LOADSTRING(POP_UP_PROMPT_DIALOG), wxPoint(110, 82), wxSize(97, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(SOUND_PROMPT), wxPoint(230, 82), wxDefaultSize, 0);
	m_cmbOrderDealSound = new wxOwnerDrawnComboBox(this, wxID_ANY, "", wxPoint(300, 82), wxSize(100, 20), arryStr, wxCB_READONLY|wxST_NO_AUTORESIZE);

	//撤单反馈
	new wxStaticBox(this, wxID_ANY, LOADSTRING(CANCEL_ORDER_PROMPT), wxPoint(13, 114), wxSize(420, 82));
	new wxStaticText(this, wxID_ANY, LOADSTRING(CANCEL_ORDER_OK), wxPoint(20, 136), wxDefaultSize, 0);
	m_chkCancelOrderSuccess = new wxCheckBox(this,wxID_ANY, LOADSTRING(POP_UP_PROMPT_DIALOG), wxPoint(110, 136), wxSize(97, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(SOUND_PROMPT), wxPoint(230, 136), wxDefaultSize, 0);
	m_cmbCancelOrderSuccessSound = new wxOwnerDrawnComboBox(this, wxID_ANY, "", wxPoint(300, 136), wxSize(100, 20), arryStr, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_CANCEL_ORDER_FAIL), wxPoint(20, 166), wxDefaultSize, 0);
	m_chkCancelOrderFailed = new wxCheckBox(this, wxID_ANY, LOADSTRING(POP_UP_PROMPT_DIALOG), wxPoint(110, 166), wxSize(97, 20), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(SOUND_PROMPT), wxPoint(230, 166), wxDefaultSize, 0);
	m_cmbCancelOrderFailedSound = new wxOwnerDrawnComboBox(this, wxID_ANY, "", wxPoint(300, 166), wxSize(100, 20), arryStr, wxCB_READONLY|wxST_NO_AUTORESIZE);

	//系统
	new wxStaticBox(this, wxID_ANY, LOADSTRING(SYSTEM_PROMPT), wxPoint(13, 204), wxSize(420, 110));
	new wxStaticText(this, wxID_ANY, LOADSTRING(TRADE_TIME_START_AND_END_PROMPT), wxPoint(20, 226), wxDefaultSize, 0);
	m_cmbTradeTimeSound = new wxOwnerDrawnComboBox(this, wxID_ANY, "", wxPoint(300, 226), wxSize(100, 20), arryStr, wxCB_READONLY|wxST_NO_AUTORESIZE);
	//new wxStaticText(this, wxID_ANY, LOADSTRING(CONTRACT_PROMPOT_BEFORE_END_TRADE), wxPoint(20, 256), wxDefaultSize, 0);
	m_pBeforeSecText = new wxTextCtrl(this, wxID_ANY, "", wxPoint(155, 256), wxSize(40, 20), 0);
	//new wxStaticText(this, wxID_ANY, LOADSTRING(PROMPT_BEFORE_SECONDS), wxPoint(200, 256), wxDefaultSize, 0);
	m_cmbBeforeTradeEndSound = new wxOwnerDrawnComboBox(this, wxID_ANY, "", wxPoint(300, 256), wxSize(100, 20), arryStr, wxCB_READONLY|wxST_NO_AUTORESIZE);
	m_pBeforeSecText->Show(false);
	m_cmbBeforeTradeEndSound->Show(false);
	//自选合约设置
	//new wxStaticBox(this, wxID_ANY, LOADSTRING(SELFFROUP_SET), wxPoint(13, 204), wxSize(420, 40));
	new wxStaticText(this, wxID_ANY, LOADSTRING(DELETE_SELFGROUP_INSTRUMENT), wxPoint(20, 256), wxDefaultSize, 0);
	m_chkDeleteInstrument = new wxCheckBox(this,wxID_ANY, LOADSTRING(POP_UP_PROMPT_DIALOG), wxPoint(110, 256), wxSize(97, 20), 0);

	InitControls();
}

CPromptSetPanel::~CPromptSetPanel()
{

}

void CPromptSetPanel::InitControls()
{   
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	LPORDER_CFG pOrderCfg = pCfgMgr->GetOrderCfg();

	m_chkOpenOrderSuccess->SetValue(pOrderCfg->bOrderSuccessDlg);
	int nOrderSuccessSound = pOrderCfg->bOrderSuccessSound? 1 : 0;
	m_cmbOpenOrderSuccessSound->SetSelection(nOrderSuccessSound);

	m_chkOpenOrderFailed->SetValue(pOrderCfg->bOrderFailDlg);
	int nOrderFailSound = pOrderCfg->bOrderFailSound? 1 : 0;
	m_cmbOpenOrderFailedSound->SetSelection(nOrderFailSound);

	m_chkOrderDeal->SetValue(pOrderCfg->bTradeDlg);
	int nbTradeSound = pOrderCfg->bTradeSound? 1 : 0;
	m_cmbOrderDealSound->SetSelection(nbTradeSound);

	CANCEL_ORDER_CFG cancelOrderCfg = pCfgMgr->GetCancelOrderCfg();
	m_chkCancelOrderSuccess->SetValue( cancelOrderCfg.bCancelSuccessDlg );
	int nCancelSuccessSound = cancelOrderCfg.bCancelSuccessSound ? 1 : 0;
	m_cmbCancelOrderSuccessSound->SetSelection(nCancelSuccessSound);

	m_chkCancelOrderFailed->SetValue( cancelOrderCfg.bCancelFailDlg );
	int nCancelFailSound = cancelOrderCfg.bCancelFailSound ? 1 : 0;
	m_cmbCancelOrderFailedSound->SetSelection(nCancelFailSound);

	SystemParamInfo* pSystemParamInfo = pCfgMgr->GetSystemParamInfo();
	m_cmbTradeTimeSound->SetSelection(pSystemParamInfo->nTradeTimeSoundPrompt);
	m_pBeforeSecText->SetValue(GlobalFunc::ConvertToString(pSystemParamInfo->nSoundPromptBeforeTradeEndSec.operator int()));
	m_cmbBeforeTradeEndSound->SetSelection(pSystemParamInfo->nTradeEndSoundPrompt);

	MAIN_WINDOW_CFG* pMainCfg = pCfgMgr->GetMainWindowCfg();
	m_chkDeleteInstrument->SetValue( pMainCfg->bDelInstrumentWarning );
}

bool CPromptSetPanel::WriteCfg()
{ 
	LPORDER_CFG pOrderCfg = CFG_MGR_DEFAULT()->GetOrderCfg();
	pOrderCfg->bOrderSuccessDlg = m_chkOpenOrderSuccess->GetValue();
	if ( m_cmbOpenOrderSuccessSound->GetSelection() == 0)
	{
		pOrderCfg->bOrderSuccessSound = false;
	}
	else
	{
		pOrderCfg->bOrderSuccessSound = true;
	}
	
	pOrderCfg->bOrderFailDlg = m_chkOpenOrderFailed->GetValue();
	if ( m_cmbOpenOrderFailedSound->GetSelection() == 0 )
	{
		pOrderCfg->bOrderFailSound = false;
	}
	else
	{
		pOrderCfg->bOrderFailSound = true;
	}

	pOrderCfg->bTradeDlg = m_chkOrderDeal->GetValue();
	if ( m_cmbOrderDealSound->GetSelection() == 0 )
	{
		pOrderCfg->bTradeSound = false;
	}
	else
	{
		pOrderCfg->bTradeSound = true;
	}

	CANCEL_ORDER_CFG cancelOrderCfg = CFG_MGR_DEFAULT()->GetCancelOrderCfg();
	cancelOrderCfg.bCancelSuccessDlg = m_chkCancelOrderSuccess->GetValue();
	if ( m_cmbCancelOrderSuccessSound->GetSelection() == 0)
	{
		cancelOrderCfg.bCancelSuccessSound = false;
	}
	else
	{
		cancelOrderCfg.bCancelSuccessSound = true;
	}

	cancelOrderCfg.bCancelFailDlg = m_chkCancelOrderFailed->GetValue();	
	if ( m_cmbCancelOrderFailedSound->GetSelection() == 0)
	{
		cancelOrderCfg.bCancelFailSound = false;
	}
	else
	{
		cancelOrderCfg.bCancelFailSound = true;
	}

	CFG_MGR_DEFAULT()->SetCancelOrderCfg( cancelOrderCfg );

	SystemParamInfo* pSystemParamInfo = CFG_MGR_DEFAULT()->GetSystemParamInfo();
	pSystemParamInfo->nTradeTimeSoundPrompt = m_cmbTradeTimeSound->GetSelection();
	if ( !m_pBeforeSecText->GetValue().IsEmpty())
	{
		pSystemParamInfo->nSoundPromptBeforeTradeEndSec = atoi(m_pBeforeSecText->GetValue().c_str());
	}
	pSystemParamInfo->nTradeEndSoundPrompt = m_cmbBeforeTradeEndSound->GetSelection();

	MAIN_WINDOW_CFG* pMainCfg = CFG_MGR_DEFAULT()->GetMainWindowCfg();
	pMainCfg->bDelInstrumentWarning = m_chkDeleteInstrument->GetValue();
	return true;
}


