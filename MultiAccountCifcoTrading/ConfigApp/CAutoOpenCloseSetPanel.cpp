#include "stdafx.h"
#include "CAutoOpenCloseSetPanel.h"

BEGIN_EVENT_TABLE(CAutoOpenCloseSetPanel,wxPanel)
	EVT_BUTTON(ID_BTN_SET,OnSet)
END_EVENT_TABLE()

CAutoOpenCloseSetPanel::CAutoOpenCloseSetPanel(wxWindow *parent,
											   wxWindowID winid ,
											   const wxPoint& pos ,
											   const wxSize& size ,
											   long style,
											   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	new wxStaticText(this, wxID_ANY, LOADSTRING(BY_MOUSE_KEYBOARD_USE_OPTIONS), 
		wxPoint(25, 11), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(NO_POSITION_DEFAULT_VOLUME), 
		wxPoint(54, 47), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(HAVE_POSITION_USE_OPTION), wxPoint(54, 74), 
		wxDefaultSize, 0);

	m_radioBtn1 = new wxRadioButton(this, ID_RADIO1, LOADSTRING(CLOSE_ALL_POSITION), wxPoint(54, 96), 
		wxSize(113, 17), 0);
	m_radioBtn2 = new wxRadioButton(this, ID_RADIO2, LOADSTRING(CLOSE_BY_DEFAULT_DO_NOT_DEAL), 
		wxPoint(54, 123), wxSize(380, 17), 0);
	m_radioBtn3 = new wxRadioButton(this, ID_RADIO3, LOADSTRING(CLOSE_BY_DEFAULT_REVERSE), 
		wxPoint(54, 148), wxSize(380, 17), 0);

	new wxButton(this, ID_BTN_SET, LOADSTRING(SET_DEFAULT_VOLUMES), wxPoint(318, 36), 
		wxSize(120, 32), 0);

	InitControls();
}

CAutoOpenCloseSetPanel::~CAutoOpenCloseSetPanel()
{ 
}

void CAutoOpenCloseSetPanel::OnSet( wxCommandEvent& event )
{
	wxCommandEvent myEvent( wxEVT_SHOW_PANEL );
	myEvent.SetInt( ID_ITEM_VARIETYINFOSET );
	GetParent()->AddPendingEvent( myEvent );
}

void CAutoOpenCloseSetPanel::InitControls()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();
	switch( pOrderCfg->ProcessPositionType)
	{
	case 1:
		m_radioBtn1->SetValue(true);
		break;
	case 2:
		m_radioBtn2->SetValue(true);
		break;
	case 3:
		m_radioBtn3->SetValue(true);
		break;
	}
}

bool CAutoOpenCloseSetPanel::WriteCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	bool bChanged = false;
	LPORDER_CFG  p = pMgr->GetOrderCfg();

	int nPosType = -1;
	if( m_radioBtn1->GetValue())
		nPosType = 1;
	else if( m_radioBtn2->GetValue())
		nPosType = 2;
	else if( m_radioBtn3->GetValue())
		nPosType = 3;
	else
		nPosType = -1;

	if( p->ProcessPositionType != nPosType )
	{
		p->ProcessPositionType = nPosType;
		bChanged = true;
	}

	return bChanged;
}