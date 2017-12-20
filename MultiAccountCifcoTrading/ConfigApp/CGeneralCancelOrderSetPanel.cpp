#include "stdafx.h"
#include "CGeneralCancelOrderSetPanel.h"

BEGIN_EVENT_TABLE(CGeneralCancelOrderSetPanel,wxPanel)
	EVT_COMBOBOX(ID_CANCELORDERHOTKEY, OnChangeSel)
	EVT_COMBOBOX(ID_CANCELALLORDERHOTKEY, OnChangeSel)
END_EVENT_TABLE()

CGeneralCancelOrderSetPanel::CGeneralCancelOrderSetPanel(wxWindow *parent,
					   wxWindowID winid ,
					   const wxPoint& pos ,
					   const wxSize& size ,
					   long style,
					   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	new wxStaticBox(this, wxID_ANY, LOADSTRING(CONTEXTMENU_REMOVE), wxPoint(22, 2), wxSize(255, 150));
	wxStaticBox* pStaticBox = new wxStaticBox(this, wxID_ANY, LOADSTRING(CHANGE_ORDER), wxPoint(20, 121), wxSize(257, 105));
	pStaticBox->Show(false);

	m_pChkBoxMouseCancelOrder = new wxCheckBox(this, ID_MOUSECANCELORDER, LOADSTRING(MOUSE_DOUBLE_CLICK_CANCEL_ORDER), 
		wxPoint(35, 18), wxSize(97, 17), 0);
	m_pChkBoxCancelOrderConfirm = new wxCheckBox(this, ID_CANCELORDERCONFIRM, LOADSTRING(CONFIRM_BEFORE_CANCEL_ORDER), 
		wxPoint(35, 38), wxSize(97, 17), 0);
	m_pChkBoxHideCancelOrderBtn = new wxCheckBox(this, ID_HIDECANCELORDERBTN, LOADSTRING(HIDE_CANCEL_BUTTON), 
		wxPoint(35, 60), wxSize(97, 17), 0);
	m_pChkEnableModifyOrder = new wxCheckBox(this, ID_ENABLEMODIFYORDER, LOADSTRING(ENABLE_CHANGE_ORDER), 
		wxPoint(29, 139), wxSize(97, 17), 0);
	m_pChkBoxModifyOrderConfirm = new wxCheckBox(this, ID_MODIFYORDERCONFIRM, LOADSTRING(CONFIRM_BEFORE_CHANGE_ORDER), 
		wxPoint(29, 161), wxSize(97, 17), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(CANCEL_BTN_SHORTCUTKEY), wxPoint(35, 90 ), wxDefaultSize, 0);
	m_pCbxCancelOrderHotKey = new wxOwnerDrawnComboBox(this, ID_CANCELORDERHOTKEY, wxEmptyString, 
		wxPoint(170, 90), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);
	new wxStaticText(this, wxID_ANY, LOADSTRING(CANCEL_ALL_BTN_SHORTCUTKEY), wxPoint(35, 120 ), wxDefaultSize, 0);
	m_pCbxCancelAllOrderHotKey = new wxOwnerDrawnComboBox(this, ID_CANCELALLORDERHOTKEY, wxEmptyString, 
		wxPoint(170, 120), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	m_pChkBoxHideCancelOrderBtn->Show(false);
	m_pChkEnableModifyOrder->Show(false);
	m_pChkBoxModifyOrderConfirm->Show(false);

	InitControls();
}

CGeneralCancelOrderSetPanel::~CGeneralCancelOrderSetPanel()
{

}

void CGeneralCancelOrderSetPanel::InitControls()
{
	CANCEL_ORDER_CFG cancelOrderCfg = CFG_MGR_DEFAULT()->GetCancelOrderCfg();
	m_pChkBoxMouseCancelOrder->SetValue( cancelOrderCfg.bDoubleClickMouseCancel );
	m_pChkBoxCancelOrderConfirm->SetValue( cancelOrderCfg.bCancelConfirm );
	m_pChkBoxHideCancelOrderBtn->SetValue( cancelOrderCfg.bHideCancelButton );
	m_pChkEnableModifyOrder->SetValue( cancelOrderCfg.bOpenModifyOrder );
	m_pChkBoxModifyOrderConfirm->SetValue( cancelOrderCfg.bModifyOrderConfirm );

	map<int,string> addr = CFG_MGR_DEFAULT()->GetShortCutKeyNameMap();
	map<int,string>::iterator it;
	wxArrayString arr;
	for( it= addr.begin(); it != addr.end(); ++it)
	{
		arr.Add(it->second);
	}

	m_pCbxCancelOrderHotKey->Append(arr);
	m_pCbxCancelOrderHotKey->SetValue( cancelOrderCfg.szCancelOrderBtnHotKey.operator const char*() );
	m_strCancelOrderHotKey = cancelOrderCfg.szCancelOrderBtnHotKey;

	m_pCbxCancelAllOrderHotKey->Append( arr );
	m_pCbxCancelAllOrderHotKey->SetValue( cancelOrderCfg.szCancelAllOrderBtnHotKey.operator const char*() );
	m_strCancelAllOrderHotKey = cancelOrderCfg.szCancelAllOrderBtnHotKey;
}

bool CGeneralCancelOrderSetPanel::WriteCfg()
{
	bool bChanged = false;
	CANCEL_ORDER_CFG cancelOrderCfg = CFG_MGR_DEFAULT()->GetCancelOrderCfg();

	if ( m_pChkBoxMouseCancelOrder->GetValue() != cancelOrderCfg.bDoubleClickMouseCancel )
	{
		cancelOrderCfg.bDoubleClickMouseCancel = m_pChkBoxMouseCancelOrder->GetValue();
		bChanged = true;
	}

	if ( m_pChkBoxCancelOrderConfirm->GetValue() != cancelOrderCfg.bCancelConfirm )
	{
		cancelOrderCfg.bCancelConfirm = m_pChkBoxCancelOrderConfirm->GetValue();
		bChanged = true;
	}

	if ( m_pChkBoxHideCancelOrderBtn->GetValue() != cancelOrderCfg.bHideCancelButton )
	{
		cancelOrderCfg.bHideCancelButton = m_pChkBoxHideCancelOrderBtn->GetValue();
		bChanged = true;
	}
	if ( m_pChkBoxHideCancelOrderBtn->GetValue() != cancelOrderCfg.bHideCancelAllButton )
	{
		cancelOrderCfg.bHideCancelAllButton = m_pChkBoxHideCancelOrderBtn->GetValue();
		bChanged = true;
	}

	wxString strCancelOrderHotKey = m_pCbxCancelOrderHotKey->GetValue();
	if ( strCancelOrderHotKey != wxString(cancelOrderCfg.szCancelOrderBtnHotKey))
	{
		cancelOrderCfg.szCancelOrderBtnHotKey.copy(strCancelOrderHotKey.c_str());
		bChanged = true;
	}

	wxString strCancelAllOrderHotKey = m_pCbxCancelAllOrderHotKey->GetValue();
	if ( strCancelAllOrderHotKey != wxString(cancelOrderCfg.szCancelOrderBtnHotKey))
	{
		cancelOrderCfg.szCancelAllOrderBtnHotKey.copy(strCancelAllOrderHotKey.c_str());
		bChanged = true;
	}

	if ( bChanged )
	{
		CFG_MGR_DEFAULT()->SetCancelOrderCfg( cancelOrderCfg );
	}
	
	return bChanged;
}

void CGeneralCancelOrderSetPanel::OnChangeSel( wxCommandEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nId = event.GetId();
	switch( nId )
	{
	case ID_CANCELORDERHOTKEY:
		{
			wxString strCancelOrderHotKey = m_pCbxCancelOrderHotKey->GetValue();
			if ( strCancelOrderHotKey == m_strCancelOrderHotKey )
			{
				return;
			}

			if ( !pCfgMgr->IsShortKeyUsable(strCancelOrderHotKey.c_str()) )
			{
				wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN) ,LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
				m_pCbxCancelOrderHotKey->SetValue( m_strCancelOrderHotKey );
			}
			else
			{
				pCfgMgr->AddUsableShortCutKey(m_strCancelOrderHotKey.c_str());
				m_strCancelOrderHotKey = strCancelOrderHotKey;	
				pCfgMgr->DelUsableShortCutKey(strCancelOrderHotKey.c_str());
			}
		}
		break;
	case ID_CANCELALLORDERHOTKEY:
		{
			wxString strCancelAllOrderHotKey = m_pCbxCancelAllOrderHotKey->GetValue();
			if ( strCancelAllOrderHotKey == m_strCancelAllOrderHotKey )
			{
				return;
			}

			if ( !pCfgMgr->IsShortKeyUsable(strCancelAllOrderHotKey.c_str()) )
			{
				wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN) ,LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
				m_pCbxCancelAllOrderHotKey->SetValue( m_strCancelAllOrderHotKey );
			}
			else
			{
				pCfgMgr->AddUsableShortCutKey(m_strCancelAllOrderHotKey.c_str());
				m_strCancelAllOrderHotKey = strCancelAllOrderHotKey;	
				pCfgMgr->DelUsableShortCutKey(strCancelAllOrderHotKey.c_str());
			}
		}
		break;
	default:
		break;
	}
}


