#include "stdafx.h"
#include "CMouseOpenOrderSetPanel.h"

BEGIN_EVENT_TABLE(CMouseOpenOrderSetPanel,wxPanel)
END_EVENT_TABLE()

CMouseOpenOrderSetPanel::CMouseOpenOrderSetPanel(wxWindow *parent,
												 wxWindowID winid ,
												 const wxPoint& pos ,
												 const wxSize& size ,
												 long style,
												 const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	wxArrayString str;
	str.Add(wxEmptyString);
	str.Add(LOADSTRING(SEND_ORDER_WITH__CONFIRM));
	str.Add(LOADSTRING(SEND_ORDER_NOT_WITH__CONFIRM));
	str.Add(LOADSTRING(DO_NOT_MOVE_FOCUS_AFTER_ORDER_SEND));
	str.Add(LOADSTRING(FOCUS_MOVE_TO_CONTRACTID_FIELD_AFTER_ORDER_SEND));
	str.Add(LOADSTRING(FOCUS_MOVE_TO_BUYSELL_FIELD_AFTER_ORDER_SEND));
	str.Add(LOADSTRING(FOCUS_MOVE_TO_OPENCLOSE_FIELD_AFTER_ORDER_SEND));
	str.Add(LOADSTRING(FOCUS_MOVE_TO_VOLUMN_FIELD_AFTER_ORDER_SEND));
	str.Add(LOADSTRING(FOCUS_MOVE_TO_PRICE_FIELD_AFTER_ORDER_SEND));
	str.Add(LOADSTRING(FOCUS_MOVE_TO_OPEN_ORDER_BUTTON_AFTER_ORDER_SEND));

	int yoff = 15;
	new wxStaticBox(this, wxID_ANY, LOADSTRING(MOUSE_CLICK_QUOTE), wxPoint(11, 0), wxSize(234, 270));
	new wxStaticBox(this, wxID_ANY, LOADSTRING(MOUSE_CLICK_POSITION), wxPoint(253, 1), wxSize(232, 105));
	new wxStaticBox(this, wxID_ANY, LOADSTRING(MOUSE_CLICK_TO_CHANGE_PRICE), wxPoint(253, 124), wxSize(231, 82));
	new wxStaticText(this, wxID_ANY, LOADSTRING(CLICK), wxPoint(18, 15+yoff), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(DBCLICK), wxPoint(19, 45+yoff), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(ORDER_DIRECTION), wxPoint(19, 133), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(CLICK), wxPoint(261, 15+yoff), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(DBCLICK), wxPoint(261, 40+yoff), wxDefaultSize, 0);

	m_cbxClickQuote = new wxOwnerDrawnComboBox(this, wxID_ANY, wxEmptyString, wxPoint(68, 13+yoff), wxSize(143, 20), str, wxCB_READONLY);
	m_cbxDoubleClickQuote = new wxOwnerDrawnComboBox(this, wxID_ANY, wxEmptyString, wxPoint(67, 39+yoff), wxSize(144, 20),str,wxCB_READONLY);
	m_cbxClickPosition = new wxOwnerDrawnComboBox(this, wxID_ANY, wxEmptyString, wxPoint(307, 14+yoff), wxSize(151, 20), str, wxCB_READONLY);
	m_cbxDoubleClickPosition = new wxOwnerDrawnComboBox(this, wxID_ANY, wxEmptyString, wxPoint(308, 39+yoff), wxSize(149, 20), str,wxCB_READONLY);

	m_radioReserveOpenOrder = new wxRadioButton(this, wxID_ANY, LOADSTRING(REVERSE), wxPoint(24, 157), wxSize(203, 17), 0);
	m_radioSyntropOpenOrder = new wxRadioButton(this, wxID_ANY, LOADSTRING(NORMAL), wxPoint(24, 176), wxSize(217, 17), 0);

	m_chkCtrlReserve = new wxCheckBox(this, wxID_ANY, LOADSTRING(PRESS_CTRL_TO_REVERSE_OPEN_ORDER), wxPoint(24, 197), wxSize(200, 17), 0);
	m_chkMouseClickOpenOrder = new wxCheckBox(this, wxID_ANY, LOADSTRING(MOUSE_CLICK_QUOTE_ALWAYS_OPEN_ORDER), wxPoint(24, 220), wxSize(200, 17), 0);

	new wxStaticText(this, wxID_ANY, LOADSTRING(CLICK_QUOTE_OR_POSITION_PRICE_CHANGE), wxPoint(261, 157), wxDefaultSize, 0);
	m_textPriceOffset = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxPoint(261, 176), wxSize(31, 19), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(TICKER_SIZE), wxPoint(295, 176), wxDefaultSize, 0);

	ReadCfg();
}

CMouseOpenOrderSetPanel::~CMouseOpenOrderSetPanel()
{ 
}

void CMouseOpenOrderSetPanel::ReadCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPORDER_CFG  p = pMgr->GetOrderCfg();
	if(p->ClickQuotTableAction>=0&&
		p->ClickQuotTableAction<(int)m_cbxClickQuote->GetCount())
		m_cbxClickQuote->SetSelection(p->ClickQuotTableAction);
	else
		m_cbxClickQuote->SetSelection(-1);
	if(p->DoubleClickQuotTableAction>=0&&
		p->DoubleClickQuotTableAction<(int)m_cbxDoubleClickQuote->GetCount())
		m_cbxDoubleClickQuote->SetSelection(p->DoubleClickQuotTableAction);
	else
		m_cbxDoubleClickQuote->SetSelection(-1);
	if(p->ClickPositionAction>=0&&
		p->ClickPositionAction<(int)m_cbxClickPosition->GetCount())
		m_cbxClickPosition->SetSelection(p->ClickPositionAction);
	else
		m_cbxClickPosition->SetSelection(-1);
	if(p->DoubleClickPositionAction>=0&&
		p->DoubleClickPositionAction<(int)m_cbxDoubleClickPosition->GetCount())
		m_cbxDoubleClickPosition->SetSelection(p->DoubleClickPositionAction);
	else
		m_cbxDoubleClickPosition->SetSelection(-1);

	m_radioReserveOpenOrder->SetValue(p->MouseFastOrderDirection==0);
	m_radioSyntropOpenOrder->SetValue(p->MouseFastOrderDirection!=0);
	m_chkCtrlReserve->SetValue(p->bCtrlReverse );
	m_chkMouseClickOpenOrder->SetValue(p->bAlwaysOpen);

	wxString strVol;
	strVol.Printf("%d",p->PositionVolume);
	m_textPriceOffset->SetValue(strVol);
}

bool CMouseOpenOrderSetPanel::WriteCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	LPORDER_CFG  p = pMgr->GetOrderCfg();
	bool bChanged = false;
	int nClickQuotTableAction = m_cbxClickQuote->GetSelection()>=0?m_cbxClickQuote->GetSelection():10;
	if ( p->ClickQuotTableAction != nClickQuotTableAction )
	{
		p->ClickQuotTableAction = nClickQuotTableAction;
		bChanged = true;
	}

	int nDoubleClickQuotTableAction = m_cbxDoubleClickQuote->GetSelection()>=0?m_cbxDoubleClickQuote->GetSelection():10;
	if( p->DoubleClickQuotTableAction != nDoubleClickQuotTableAction )
	{
		p->DoubleClickQuotTableAction = nDoubleClickQuotTableAction;
		bChanged = true;
	}

	int nClickPositionAction = m_cbxClickPosition->GetSelection()>=0?m_cbxClickPosition->GetSelection():10;
	if ( p->ClickPositionAction != nClickPositionAction )
	{
		p->ClickPositionAction = nClickPositionAction;
		bChanged = true;
	}

	int nDoubleClickPositionAction = m_cbxDoubleClickPosition->GetSelection()>=0?m_cbxDoubleClickPosition->GetSelection():10;
	if ( p->DoubleClickPositionAction != nDoubleClickPositionAction )
	{
		p->DoubleClickPositionAction = nDoubleClickPositionAction;
		bChanged = true;
	}

	int nMouseFastOrderDirection = m_radioReserveOpenOrder->GetValue()?0:1;
	if( p->MouseFastOrderDirection != nMouseFastOrderDirection )
	{
		p->MouseFastOrderDirection = nMouseFastOrderDirection;
		bChanged = true;
	}

	if( p->bCtrlReverse != m_chkCtrlReserve->GetValue())
	{
		p->bCtrlReverse = m_chkCtrlReserve->GetValue();
		bChanged = true;
	}

	if ( p->bAlwaysOpen != m_chkMouseClickOpenOrder->GetValue())
	{
		p->bAlwaysOpen = m_chkMouseClickOpenOrder->GetValue();
		bChanged = true;
	}

	int nPositionVolume = atoi(m_textPriceOffset->GetValue());
	if ( p->PositionVolume != nPositionVolume )
	{
		p->PositionVolume = nPositionVolume;
		bChanged = true;
	}

	return bChanged;
}