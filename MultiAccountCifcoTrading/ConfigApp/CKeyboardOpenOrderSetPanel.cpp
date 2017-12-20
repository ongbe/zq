#include "stdafx.h"
#include "../inc/Module-Misc/GLobalFunc.h"
#include "CKeyboardOpenOrderSetPanel.h"

BEGIN_EVENT_TABLE(CKeyboardOpenOrderSetPanel,wxPanel)
	EVT_BUTTON(ID_BtAdd,OnAdd)
	EVT_BUTTON(ID_BtSub,OnDel)
	EVT_COMBOBOX(ID_Com1, OnSelc) 
	EVT_COMBOBOX(ID_Com2, OnSelc)
	EVT_COMBOBOX(ID_Com3, OnSelc)
	EVT_COMBOBOX(ID_Com4, OnSelc)
	EVT_COMBOBOX(ID_Com5, OnSelc)
	EVT_COMBOBOX(ID_Com6, OnSelc)
	EVT_CHECKBOX(ID_Check1,OnCheck)
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY,OnListItemSel)
END_EVENT_TABLE()

CKeyboardOpenOrderSetPanel::CKeyboardOpenOrderSetPanel(wxWindow *parent,
													   wxWindowID winid ,
													   const wxPoint& pos ,
													   const wxSize& size ,
													   long style,
													   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	new wxStaticBox(this, wxID_ANY, wxT("                        "), wxPoint(7, 3), wxSize(478, 313));
	new wxStaticText(this, wxID_ANY, LOADSTRING(CAUTION_USE_QUICK_ORDER), wxPoint(9, 318), wxDefaultSize, 0);

	m_chkEnableKsyBoardOpenOrder = new wxCheckBox(this, ID_Check1, LOADSTRING(ENABLE_KEYBOARD_QUICK_ORDER), wxPoint(18, 3), wxSize(110, 17), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(OPERATE_STEP), wxPoint(16, 18), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(PRESS_F1_TO_ENTER_QUOTE), wxPoint(42, 36), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(USE_KC_SELECT_A_CONSTRACT), wxPoint(42, 53), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(USE_THE_KEYS_ORDER), wxPoint(42, 67), wxDefaultSize, 0);

	m_listCtrl = new wxExtListCtrl(this, wxID_ANY, wxPoint(23, 86), wxSize(446, 130), wxLCEXT_REPORT|wxBORDER_SIMPLE);

	new wxStaticText(this, wxID_ANY, LOADSTRING(ACCELERATOR), wxPoint(25, 228), wxSize(50, 20), 0);
	m_cbxHotKey = new wxOwnerDrawnComboBox(this, ID_Com1, wxEmptyString, wxPoint(80, 228), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_DIRECTION), wxPoint(170, 228), wxSize(50, 20), 0);
	m_cbxBuySel = new wxOwnerDrawnComboBox(this, ID_Com2, wxEmptyString, wxPoint(235, 228), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticText(this, wxID_ANY, LOADSTRING(OLS_PRICE), wxPoint(335, 228), wxSize(30, 20), 0, wxEmptyString);
	m_cbxPriceType = new wxOwnerDrawnComboBox(this, ID_Com3, wxEmptyString, wxPoint(365, 228), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticText(this, wxID_ANY, LOADSTRING(ACTION), wxPoint(25, 253), wxSize(50, 20), 0, wxEmptyString);
	m_cbxAction = new wxOwnerDrawnComboBox(this, ID_Com4, wxEmptyString, wxPoint(80, 253), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticText(this, wxID_ANY, LOADSTRING(ACTIONZONE), wxPoint(170, 253), wxSize(50, 20), 0, wxEmptyString);
	m_cbxValidArea = new wxOwnerDrawnComboBox(this, ID_Com5, wxEmptyString, wxPoint(235, 253), wxSize(80, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	m_chkFillOrderPlate = new wxCheckBox(this, wxID_ANY, LOADSTRING(WTO_AFTER_ORDER_SEND), wxPoint(25, 275), wxSize(300, 17), 0);
	m_chkFillOrderPlate->Show(false);
	m_chkMouseReturnToQuoteTable = new wxCheckBox(this, wxID_ANY, LOADSTRING(SFTQL_AFTER_ORDER_SEND), wxPoint(25, 285), wxSize(300, 17), 0);

	m_btnAdd = new wxButton(this, ID_BtAdd, LOADSTRING(ASKNAMEDLG_ADD), wxPoint(325, 285), wxSize(75, 25), 0);
	m_btnDelete = new wxButton(this, ID_BtSub, LOADSTRING(ASKNAMEDLG_DEL), wxPoint(404, 285), wxSize(75, 25), 0);

	SetListData();
	ReadCfg();
}

CKeyboardOpenOrderSetPanel::~CKeyboardOpenOrderSetPanel()
{

}
void CKeyboardOpenOrderSetPanel::OnListItemSel(wxExtListEvent& event)
{
	wxString str;
	str=m_listCtrl->GetSubItemText(event.GetIndex(),0);//快捷键
	if(!m_cbxHotKey->SetStringSelection(str))
	{
		m_cbxHotKey->SetSelection(-1);
	}
	str=m_listCtrl->GetSubItemText(event.GetIndex(),1);//买卖
	if(!m_cbxBuySel->SetStringSelection(str))
	{
		m_cbxBuySel->SetSelection(-1);
	}
	str=m_listCtrl->GetSubItemText(event.GetIndex(),2);//价格
	if(!m_cbxPriceType->SetStringSelection(str))
	{
		m_cbxPriceType->SetSelection(-1);
	}
	str=m_listCtrl->GetSubItemText(event.GetIndex(),4);//动作
	if(!m_cbxAction->SetStringSelection(str))
	{
		m_cbxAction->SetSelection(-1);
	}
	str=m_listCtrl->GetSubItemText(event.GetIndex(),5);//作用区域
	if(!m_cbxValidArea->SetStringSelection(str))
	{
		m_cbxValidArea->SetSelection(-1);
	}
}

void CKeyboardOpenOrderSetPanel::SetListData()
{   
	m_listCtrl->InsertColumn(0,LOADSTRING(ACCELERATOR),wxLISTEXT_FORMAT_LEFT,50);
	m_listCtrl->InsertColumn(1,LOADSTRING(OLS_DIRECTION),wxLISTEXT_FORMAT_LEFT,60);
	m_listCtrl->InsertColumn(2,LOADSTRING(OLS_PRICE),wxLISTEXT_FORMAT_LEFT,60);
	m_listCtrl->InsertColumn(3,LOADSTRING(OVER_PRICE),wxLISTEXT_FORMAT_LEFT,60);
	m_listCtrl->InsertColumn(4,LOADSTRING(ACTION),wxLISTEXT_FORMAT_LEFT,140);
	m_listCtrl->InsertColumn(5,LOADSTRING(ACTIONZONE),wxLISTEXT_FORMAT_LEFT,60);
	m_listCtrl->SetColumnCtrlType(3,COLUMN_CTRL_TYPE_EDIT);
}

void CKeyboardOpenOrderSetPanel::OnAdd(wxCommandEvent& event)
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int rownum = m_listCtrl->GetItemCount();
	string str;
	if( rownum != 0)
	{
		str = m_listCtrl->GetSubItemText(rownum-1,0);
		if( str.empty())
		{
			m_listCtrl->SetItemState(rownum-1,wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);
			return;
		}

	}

	m_listCtrl->InsertItem( rownum ,str);
	str = m_cbxBuySel->GetValue();
	m_listCtrl->SetItem(rownum,1,str);

	str = m_cbxPriceType->GetValue();
	m_listCtrl->SetItem(rownum,2,str);

	m_listCtrl->SetItem(rownum,3,"0");

	str = m_cbxAction->GetValue();
	m_listCtrl->SetItem(rownum,4,str);

	str = m_cbxValidArea->GetValue();
	m_listCtrl->SetItem(rownum,5,str);

	str = m_cbxHotKey->GetValue();
	if ( !pCfgMgr->IsShortKeyUsable(str))
	{
		m_listCtrl->SetItem(rownum, 0, wxEmptyString);
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
	}
	else
	{
		m_listCtrl->SetItem(rownum,0,str);
		pCfgMgr->DelUsableShortCutKey(str);
	}
}

void CKeyboardOpenOrderSetPanel::OnDel(wxCommandEvent& event)
{
	int rownum = m_listCtrl->GetItemCount();
	for( int i=0; i< rownum; i++ )
	{
		if( m_listCtrl->IsSelectedItem( i) )
		{	
			wxString strOldKey = m_listCtrl->GetSubItemText(i, 0);
			CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
			if ( NULL != pCfgMgr && !strOldKey.empty())
			{
				pCfgMgr->AddUsableShortCutKey(strOldKey.c_str());
			}

			m_listCtrl->DeleteItem( i );
			rownum--;
			i--;
		}
	}
}

void CKeyboardOpenOrderSetPanel::ReadCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPORDER_CFG p = pMgr->GetOrderCfg();
	if(!p->bOpenQuotKeyBoardOrder)
	{
		m_chkEnableKsyBoardOpenOrder->SetValue(false);
		m_listCtrl->Disable();
		m_btnAdd->Disable();
		m_btnDelete->Disable();
		m_cbxHotKey->Disable();
		m_cbxBuySel->Disable();
		m_cbxPriceType->Disable();
		m_cbxAction->Disable();
		m_cbxValidArea->Disable();
		m_chkFillOrderPlate->Disable();
		m_chkMouseReturnToQuoteTable->Disable();
	}
	else
		m_chkEnableKsyBoardOpenOrder->SetValue(true);

	if( p->bSyncQuotPanel)
		m_chkFillOrderPlate->SetValue(true);
	else
		m_chkFillOrderPlate->SetValue(false);

	if(p->bReturnQuotTable)
		m_chkMouseReturnToQuoteTable->SetValue(true);
	else
		m_chkMouseReturnToQuoteTable->SetValue(false);

	for( int i=0; i<p->KeyOrderDataNum; ++i )
	{
		m_listCtrl->InsertItem( i,p->KeyOrderData[i].ShortKey);
		if ( p->KeyOrderData[i].nBuySell == 0)
		{
			m_listCtrl->SetItem(i,1,LOADSTRING(OLS_BUY));
		}
		else
		{
			m_listCtrl->SetItem(i,1,LOADSTRING(OLS_SELL));
		}

		string strPrice;
		switch( p->KeyOrderData[i].nPrice )
		{
		case 0:
			strPrice = LOADSTRING(OIP_LASTPRICE);
			break;
		case 1:
			strPrice = LOADSTRING(OIP_BIDPRICE);
			break;
		case 2:
			strPrice = LOADSTRING(OIP_ASKPRICE);
			break;
		default:
			break;
		}

		string strAction;
		switch( p->KeyOrderData[i].nAction )
		{
		case 0:
			strAction = "";
			break;
		case 1:
			strAction = LOADSTRING(SEND_ORDER_WITH__CONFIRM);
			break;
		case 2:
			strAction = LOADSTRING(SEND_ORDER_NOT_WITH__CONFIRM);
			break;
		case 3:
			strAction = LOADSTRING(DO_NOT_MOVE_FOCUS_AFTER_ORDER_SEND);
			break;
		case 4:
			strAction = LOADSTRING(FOCUS_MOVE_TO_CONTRACTID_FIELD_AFTER_ORDER_SEND);
			break;
		case 5:
			strAction = LOADSTRING(FOCUS_MOVE_TO_BUYSELL_FIELD_AFTER_ORDER_SEND);
			break;
		case 6:
			strAction = LOADSTRING(FOCUS_MOVE_TO_OPENCLOSE_FIELD_AFTER_ORDER_SEND);
			break;
		case 7:
			strAction = LOADSTRING(FOCUS_MOVE_TO_VOLUMN_FIELD_AFTER_ORDER_SEND);
			break;
		case 8:
			strAction = LOADSTRING(FOCUS_MOVE_TO_PRICE_FIELD_AFTER_ORDER_SEND);
			break;
		case 9:
			strAction = LOADSTRING(FOCUS_MOVE_TO_OPEN_ORDER_BUTTON_AFTER_ORDER_SEND);
			break;
		default:
			break;
		}

		string strActionArea;
		switch(p->KeyOrderData[i].nActionArea)
		{
		case 0:
			strActionArea = LOADSTRING(GLOBAL);
			break;
		case 1:
			strActionArea = LOADSTRING(IDT_QUOTETABLE);
			break;
		default:
			break;
		}
		m_listCtrl->SetItem(i,2,strPrice);
		m_listCtrl->SetItem(i,3, GlobalFunc::ConvertToString(p->KeyOrderData[i].nOverPrice));
		m_listCtrl->SetItem(i,4,strAction);
		m_listCtrl->SetItem(i,5,strActionArea);
	}

	map<int,string> addr = pMgr->GetShortCutKeyNameMap();
	map<int,string>::iterator it;
	wxArrayString arr;
	for( it= addr.begin(); it!= addr.end(); ++it)
	{
		arr.Add(it->second);
	}

	m_cbxHotKey->AppendString(wxEmptyString);
	m_cbxHotKey->Append(arr);

	m_cbxBuySel->AppendString(LOADSTRING(OLS_BUY));
	m_cbxBuySel->AppendString(LOADSTRING(OLS_SELL));

	m_cbxPriceType->AppendString(LOADSTRING(OIP_LASTPRICE));
	m_cbxPriceType->AppendString(LOADSTRING(OIP_BIDPRICE));
	m_cbxPriceType->AppendString(LOADSTRING(OIP_ASKPRICE));

	wxArrayString arr2;
	arr2.Add(wxEmptyString);
	arr2.Add(LOADSTRING(SEND_ORDER_WITH__CONFIRM));
	arr2.Add(LOADSTRING(SEND_ORDER_NOT_WITH__CONFIRM));
	arr2.Add(LOADSTRING(DO_NOT_MOVE_FOCUS_AFTER_ORDER_SEND));
	arr2.Add(LOADSTRING(FOCUS_MOVE_TO_CONTRACTID_FIELD_AFTER_ORDER_SEND));
	arr2.Add(LOADSTRING(FOCUS_MOVE_TO_BUYSELL_FIELD_AFTER_ORDER_SEND));
	arr2.Add(LOADSTRING(FOCUS_MOVE_TO_OPENCLOSE_FIELD_AFTER_ORDER_SEND));
	arr2.Add(LOADSTRING(FOCUS_MOVE_TO_VOLUMN_FIELD_AFTER_ORDER_SEND));
	arr2.Add(LOADSTRING(FOCUS_MOVE_TO_PRICE_FIELD_AFTER_ORDER_SEND));
	arr2.Add(LOADSTRING(FOCUS_MOVE_TO_OPEN_ORDER_BUTTON_AFTER_ORDER_SEND));
	m_cbxAction->Append(arr2);

	m_cbxValidArea->AppendString(LOADSTRING(GLOBAL));
	m_cbxValidArea->AppendString(LOADSTRING(IDT_QUOTETABLE));

	m_cbxHotKey->SetSelection(0);
	m_cbxBuySel->SetSelection(0);
	m_cbxPriceType->SetSelection(0);
	m_cbxAction->SetSelection(1);
	m_cbxValidArea->SetSelection(0);
}

bool CKeyboardOpenOrderSetPanel::WriteCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	bool bChanged = false;
	LPORDER_CFG  p = pMgr->GetOrderCfg();
	if( m_chkEnableKsyBoardOpenOrder->GetValue()!= p->bOpenQuotKeyBoardOrder )
	{
		p->bOpenQuotKeyBoardOrder = m_chkEnableKsyBoardOpenOrder->GetValue();
		bChanged = true;
	}

	if ( m_chkFillOrderPlate->GetValue() != p->bSyncQuotPanel )
	{
		p->bSyncQuotPanel = m_chkFillOrderPlate->GetValue();
		bChanged = true;
	}

	if ( m_chkMouseReturnToQuoteTable->GetValue() != p->bReturnQuotTable )
	{
		p->bReturnQuotTable = m_chkMouseReturnToQuoteTable->GetValue();
		bChanged = true;
	}

	int nBackOrderDateNum = p->KeyOrderDataNum;
	p->KeyOrderDataNum=0;
	int row = m_listCtrl->GetItemCount();
	if ( row > 100 )
	{
		row = 100;
	}
	for( int i=0; i<row; i++)
	{
		KeyOrderInfo info;
		ZeroMemory(&info, sizeof(info));
		string str;
		str = m_listCtrl->GetSubItemText(i,0);
		if(str.empty())
			continue;
		strncpy(info.ShortKey,str.c_str(), sizeof(info.ShortKey)-1);

		str = m_listCtrl->GetSubItemText(i,1);
		if ( str == LOADSTRING(OLS_BUY) )
		{
			info.nBuySell = 0;
		}
		else
		{
			info.nBuySell = 1;
		}

		str = m_listCtrl->GetSubItemText(i,2);
		if ( str == LOADSTRING(OIP_LASTPRICE))
		{
			info.nPrice = 0;
		}
		else if ( str == LOADSTRING(OIP_BIDPRICE))
		{
			info.nPrice = 1;
		}
		else
		{
			info.nPrice = 2;
		}

		str =m_listCtrl->GetSubItemText(i,3);
		info.nOverPrice = atoi( str.c_str());

		str = m_listCtrl->GetSubItemText(i,4);
		if ( str == wxEmptyString )
		{
			info.nAction = 0;
		}
		else if ( str == LOADSTRING(SEND_ORDER_WITH__CONFIRM))
		{
			info.nAction = 1;
		}
		else if ( str == LOADSTRING(SEND_ORDER_NOT_WITH__CONFIRM))
		{
			info.nAction = 2;
		}
		else if ( str == LOADSTRING(DO_NOT_MOVE_FOCUS_AFTER_ORDER_SEND))
		{
			info.nAction = 3;
		}
		else if ( str == LOADSTRING(FOCUS_MOVE_TO_CONTRACTID_FIELD_AFTER_ORDER_SEND))
		{
			info.nAction = 4;
		}
		else if ( str == LOADSTRING(FOCUS_MOVE_TO_BUYSELL_FIELD_AFTER_ORDER_SEND))
		{
			info.nAction = 5;
		}
		else if ( str == LOADSTRING(FOCUS_MOVE_TO_OPENCLOSE_FIELD_AFTER_ORDER_SEND))
		{
			info.nAction = 6;
		}
		else if ( str == LOADSTRING(FOCUS_MOVE_TO_VOLUMN_FIELD_AFTER_ORDER_SEND))
		{
			info.nAction = 7;
		}
		else if ( str == LOADSTRING(FOCUS_MOVE_TO_PRICE_FIELD_AFTER_ORDER_SEND))
		{
			info.nAction = 8;
		}
		else
		{
			info.nAction = 9;
		}

		str = m_listCtrl->GetSubItemText(i,5);
		m_cbxValidArea->AppendString(LOADSTRING(GLOBAL));
		m_cbxValidArea->AppendString(LOADSTRING(IDT_QUOTETABLE));
		if ( str == LOADSTRING(GLOBAL))
		{
			info.nActionArea = 0;
		}
		else
		{
			info.nActionArea = 1;
		}

		if ( p->KeyOrderData[p->KeyOrderDataNum].nAction != info.nAction ||
			p->KeyOrderData[p->KeyOrderDataNum].nActionArea != info.nActionArea ||
			0 != strcmp(p->KeyOrderData[p->KeyOrderDataNum].ShortKey, info.ShortKey) ||
			p->KeyOrderData[p->KeyOrderDataNum].nBuySell != info.nBuySell ||
			p->KeyOrderData[p->KeyOrderDataNum].nPrice != info.nPrice ||
			p->KeyOrderData[p->KeyOrderDataNum].nOverPrice != info.nOverPrice)
		{
			memcpy( &p->KeyOrderData[i],&info, sizeof(info) );
			bChanged = true;
		}
		p->KeyOrderDataNum++;
	}

	if ( p->KeyOrderDataNum != nBackOrderDateNum )
	{
		memset(&p->KeyOrderData[p->KeyOrderDataNum], 0, 
			(100 - p->KeyOrderDataNum)* sizeof(KeyOrderInfo));
		bChanged = true;
	}

	return bChanged;
}

void CKeyboardOpenOrderSetPanel::OnSelc(wxCommandEvent& event)
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int ncount = m_listCtrl->GetSelectedItemCount();
	if( ncount ==0 )
		return;

	string str;
	int id = event.GetId();
	switch(id)
	{
	case ID_Com1:
		str = m_cbxHotKey->GetValue();
		if ( !pCfgMgr->IsShortKeyUsable(str))
		{
			wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
			return;
		}

		for( int i=0; i<m_listCtrl->GetItemCount(); ++i )
		{ 
			if( m_listCtrl->IsSelectedItem( i ) )
			{
				wxString strOldKey = m_listCtrl->GetSubItemText(i, 0);
				m_listCtrl->SetItem( i,0,str);
				if ( !strOldKey.IsEmpty())
				{
					pCfgMgr->AddUsableShortCutKey(strOldKey.c_str());
				}
				pCfgMgr->DelUsableShortCutKey(str);
			}
		}

		break;
	case ID_Com2:
		str = m_cbxBuySel->GetValue();
		for( int i=0; i<m_listCtrl->GetItemCount(); ++i )
		{
			if ( m_listCtrl->IsSelectedItem( i ) )
			{
				m_listCtrl->SetItem( i,1,str);
			}
		}
		break;
	case  ID_Com3:
		str = m_cbxPriceType->GetValue();
		for( int i=0; i<m_listCtrl->GetItemCount(); ++i )
		{
			if ( m_listCtrl->IsSelectedItem( i ) )
			{
				m_listCtrl->SetItem( i,2,str);
			}
		}
		break;
	case ID_Com4: 
		str = m_cbxAction->GetValue();
		for( int i=0; i<m_listCtrl->GetItemCount(); ++i )
		{
			if ( m_listCtrl->IsSelectedItem( i ) )
			{
				m_listCtrl->SetItem( i,4,str);
			}
		}
		break;
	case ID_Com5:
		str = m_cbxValidArea->GetValue();
		for( int i=0; i<m_listCtrl->GetItemCount(); ++i )
		{
			if ( m_listCtrl->IsSelectedItem( i ) )
			{
				m_listCtrl->SetItem( i,5,str);
			}
		}
		break;
	}
}

void CKeyboardOpenOrderSetPanel::OnCheck(wxCommandEvent& event)
{
	if( m_chkEnableKsyBoardOpenOrder->GetValue()== false) 
	{
		m_listCtrl->Disable();
		m_btnAdd->Disable();
		m_btnDelete->Disable();
		m_cbxHotKey->Disable();
		m_cbxBuySel->Disable();
		m_cbxPriceType->Disable();
		m_cbxAction->Disable();
		m_cbxValidArea->Disable();
		m_chkFillOrderPlate->Disable();
		m_chkMouseReturnToQuoteTable->Disable();
	}
	else
	{
		m_listCtrl->Enable();
		m_btnAdd->Enable();
		m_btnDelete->Enable();
		m_cbxHotKey->Enable();
		m_cbxBuySel->Enable();
		m_cbxPriceType->Enable();
		m_cbxAction->Enable();
		m_cbxValidArea->Enable();
		m_chkFillOrderPlate->Enable();
		m_chkMouseReturnToQuoteTable->Enable();
	}
}
