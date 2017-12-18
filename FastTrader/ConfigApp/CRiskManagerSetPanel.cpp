#include "stdafx.h"
#include "CRiskManagerSetPanel.h"

BEGIN_EVENT_TABLE(CRiskManagerSetPanel,wxPanel)
END_EVENT_TABLE()

CRiskManagerSetPanel::CRiskManagerSetPanel(wxWindow *parent,
										   wxWindowID winid ,
										   const wxPoint& pos ,
										   const wxSize& size ,
										   long style,
										   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	new wxStaticBox(this, wxID_ANY, LOADSTRING(ORDER_LIMIT), wxPoint(10, 4), wxSize(210, 149));
	new wxStaticText(this, wxID_ANY, LOADSTRING(VOLUME_LIMIT), wxPoint(20, 23), wxDefaultSize, 0);
	m_textHandLimit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(21, 39), wxSize(88, 19), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(VOLUME_S), wxPoint(116, 41), wxDefaultSize, 0);

	new wxStaticText(this, wxID_ANY, LOADSTRING(PRICE_LIMIT_PROMPT_CANNOT_OVER), wxPoint(19, 85), wxDefaultSize, 0);
	m_textPriceLimit = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(20, 106), wxSize(88, 19), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(TICKER_SIZE), wxPoint(116, 108), wxDefaultSize, 0);

	new wxStaticBox(this, wxID_ANY, LOADSTRING(CLEAR_ORDER_AREA), wxPoint(239, 4), wxSize(186, 149));
	new wxStaticText(this, wxID_ANY, LOADSTRING(CLEAR_ORDER_AREA_AFTER_ORDER), wxPoint(245, 25), wxDefaultSize, 0);
	m_radioNotReset = new wxRadioButton(this, wxID_ANY, LOADSTRING(DO_NOT_CLEAR), wxPoint(252, 45), wxSize(170, 17), 0);
	m_radioResetAll = new wxRadioButton(this, wxID_ANY, LOADSTRING(CLEAR_ALL), wxPoint(252, 66), wxSize(170, 17), 0);
	m_radioResetHandAndPrice = new wxRadioButton(this, wxID_ANY, LOADSTRING(CLEAR_PRICE_AND_VOLUME), wxPoint(252, 88), wxSize(170, 17), 0);
	m_radioResetHand = new wxRadioButton(this, wxID_ANY, LOADSTRING(ONLY_CLEAR_VOLUME), wxPoint(252, 109), wxSize(170, 17), 0 );

	ReadCfg();
}

CRiskManagerSetPanel::~CRiskManagerSetPanel()
{
}

void CRiskManagerSetPanel::ReadCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();

	char buf[20];
	sprintf(buf,"%d",pOrderCfg->VolumeLimit);
	m_textHandLimit->SetValue( buf);

	sprintf(buf,"%d",pOrderCfg->DifferPrice);
	m_textPriceLimit->SetValue(buf);

	switch( pOrderCfg->ActionAfterOrder)
	{
	case 1:
		m_radioNotReset->SetValue(true);
		break;
	case 2:
		m_radioResetAll->SetValue(true);
		break;
	case 3:
		m_radioResetHandAndPrice->SetValue(true);
		break;
	case 4:
		m_radioResetHand->SetValue(true);
		break;
	}
}

bool CRiskManagerSetPanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	bool bChanged = false;
	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();

	int nVolumeLimit = atoi(m_textHandLimit->GetValue());
	int nDifferPrice = atoi(m_textPriceLimit->GetValue());
	int nActionAfterOrder = 0;
	if(m_radioNotReset->GetValue())
		nActionAfterOrder = 1;
	else if( m_radioResetAll->GetValue())
		nActionAfterOrder =2;
	else if( m_radioResetHandAndPrice->GetValue())
		nActionAfterOrder =3;
	else
		nActionAfterOrder =4;

	if ( pOrderCfg->VolumeLimit != nVolumeLimit )
	{
		pOrderCfg->VolumeLimit = nVolumeLimit;
		bChanged = true;
	}

	if ( pOrderCfg->DifferPrice != nDifferPrice )
	{
		pOrderCfg->DifferPrice = nDifferPrice;
		bChanged = true;
	}

	if ( pOrderCfg->ActionAfterOrder != nActionAfterOrder )
	{
		pOrderCfg->ActionAfterOrder = nActionAfterOrder;
		bChanged = true;
	}

	return bChanged;
}


