#include "stdafx.h"
#include "CAdvancedSetPanel.h"

BEGIN_EVENT_TABLE(CAdvancedSetPanel,wxPanel)
	EVT_BUTTON(ID_BTN_ADD,OnAdd)
	EVT_BUTTON(ID_BTN_DEL,OnDel)
	EVT_COMBOBOX(ID_CBX_HOTKEY, OnChangeSel)
END_EVENT_TABLE()

CAdvancedSetPanel::CAdvancedSetPanel(wxWindow *parent,
									 wxWindowID winid ,
									 const wxPoint& pos ,
									 const wxSize& size ,
									 long style,
									 const wxString& name ):wxPanel(parent,winid,pos,size,style,name)
{
	int nOffSet_y = 10;
	new wxStaticBox(this, wxID_ANY, LOADSTRING(DEFAULE_MULTIPLE), wxPoint(0, 0), wxSize(485, 312+5));
	new wxStaticText(this, wxID_ANY, LOADSTRING(IF_CHOOSE_LOTS), 
		wxPoint(7, 12+nOffSet_y), wxDefaultSize, 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(CAUTION_THIS_FUNCTION), 
		wxPoint(7, 32+nOffSet_y), wxDefaultSize, 0);

	m_listCtrl = new wxExtListCtrl(this,wxID_ANY, wxPoint(8, 52 + nOffSet_y), 
		wxSize(470, 200), wxLCEXT_REPORT|wxLCEXT_SINGLE_SEL|wxBORDER_SIMPLE);
	m_listCtrl->InsertColumn( 0,LOADSTRING(ACCELERATOR));
	m_listCtrl->InsertColumn( 1,LOADSTRING(MULTIPLE));
	m_listCtrl->SetColumnCtrlType( 1, COLUMN_CTRL_TYPE_EDIT );

	new wxButton(this, ID_BTN_ADD, LOADSTRING(ASKNAMEDLG_ADD), wxPoint(272, 274+nOffSet_y), 
		wxSize(75, 25), 0);
	new wxButton(this, ID_BTN_DEL, LOADSTRING(ASKNAMEDLG_DEL), wxPoint(356, 274+nOffSet_y), 
		wxSize(75, 25), 0);

	new wxStaticText(this, wxID_ANY, LOADSTRING(ACCELERATOR), wxPoint(8, 274+nOffSet_y), 
		wxDefaultSize, 0);
	m_cbxHotKey = new wxOwnerDrawnComboBox(this, ID_CBX_HOTKEY, wxEmptyString, 
		wxPoint(75, 274+nOffSet_y), wxSize(115, 20),0, wxCB_READONLY);

	InitControls();
}

CAdvancedSetPanel::~CAdvancedSetPanel()
{ 
}

void CAdvancedSetPanel::OnAdd( wxCommandEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	wxString strHotKey = m_cbxHotKey->GetValue();
	int nRowNum = m_listCtrl->GetItemCount();
	if( nRowNum == 0)
	{
		m_listCtrl->InsertItem( nRowNum ,strHotKey);
		m_listCtrl->SetItem( nRowNum, 1,"1" );
		pCfgMgr->DelUsableShortCutKey(strHotKey.c_str());
		m_listCtrl->SetItemState( nRowNum,wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);
		return;
	}

	for( int i=0; i<nRowNum; ++i)
	{
		if( m_listCtrl->IsSelected(i))
			m_listCtrl->SetItemState( i, ~wxLISTEXT_STATE_SELECTED,wxLISTEXT_MASK_STATE);
	}

	if( !m_listCtrl->GetSubItemText(nRowNum-1,0).IsEmpty())
	{
		if ( !pCfgMgr->IsShortKeyUsable(strHotKey.c_str()))
		{
			m_listCtrl->InsertItem( nRowNum ,wxEmptyString);
		}
		else
		{
			m_listCtrl->InsertItem( nRowNum ,strHotKey);
			pCfgMgr->DelUsableShortCutKey(strHotKey.c_str());
		}
		
		m_listCtrl->SetItem( nRowNum, 1, "1" );
		m_listCtrl->SetItemState( nRowNum, wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED);
	}
	else
	{
		m_listCtrl->SetItemState(nRowNum-1,wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);
	}

}

void CAdvancedSetPanel::OnDel( wxCommandEvent& event )
{
	int nRowNum = m_listCtrl->GetItemCount();
	for( int i=0; i< nRowNum; i++ )
	{
		if( m_listCtrl->IsSelectedItem(i) )
		{
			wxString strOldKey = m_listCtrl->GetSubItemText(i, 0);
			CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
			if ( NULL != pCfgMgr && !strOldKey.empty())
			{
				pCfgMgr->AddUsableShortCutKey(strOldKey.c_str());
			}

			m_listCtrl->DeleteItem( i );
			nRowNum--;
			i--;
		}
	}
}

void CAdvancedSetPanel::InitControls()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	wxArrayString arr;
	map<int,string> shortCutKeyMap = pMgr->GetShortCutKeyNameMap();
	map<int,string>::iterator it = shortCutKeyMap.begin();
	for ( ; it != shortCutKeyMap.end() ;++it)
	{
		arr.Add(it->second);
	}
	m_cbxHotKey->Append(wxEmptyString);
	m_cbxHotKey->Append( arr );

	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();
	for( int i= 0; i< pOrderCfg->OrderMultiDataNum; ++i)
	{
		char buf[20];
		memset( buf, 0, sizeof(buf));
		m_listCtrl->InsertItem(i, pOrderCfg->OrderMultiData[i].ShortKey);
		sprintf( buf, "%d", pOrderCfg->OrderMultiData[i].Volume);
		m_listCtrl->SetItem( i, 1, buf );
	}

}

bool CAdvancedSetPanel::WriteCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	LPORDER_CFG pOrderCfg = pMgr->GetOrderCfg();
	bool bChanged = false;
	int nBackOrderMultiDataNum = pOrderCfg->OrderMultiDataNum;
	pOrderCfg->OrderMultiDataNum =0;

	int nCount = m_listCtrl->GetItemCount();
	if ( nCount > 100 )
	{
		nCount = 100;
	}
	for( int i=0; i< nCount; ++i)
	{
		string str;
		OrderMultiply info;
		ZeroMemory(&info, sizeof(info));
		str = m_listCtrl->GetSubItemText( i,0 );
		strncpy(info.ShortKey,str.c_str(), sizeof(info.ShortKey)-1);
		info.Volume = atoi(m_listCtrl->GetSubItemText( i,1 ));
		if ( (0 != strcmp( pOrderCfg->OrderMultiData[i].ShortKey, info.ShortKey)) ||
			(pOrderCfg->OrderMultiData[i].Volume != info.Volume) )
		{
			memcpy(&pOrderCfg->OrderMultiData[i],&info,sizeof(info));
			bChanged = true;
		}

		pOrderCfg->OrderMultiDataNum++;
	}

	if ( pOrderCfg->OrderMultiDataNum != nBackOrderMultiDataNum )
	{
		memset( &pOrderCfg->OrderMultiData[pOrderCfg->OrderMultiDataNum], 0, 
			(100 - pOrderCfg->OrderMultiDataNum)*sizeof(OrderMultiply) );
		bChanged = true;
	}

	return bChanged;
}

void CAdvancedSetPanel::OnChangeSel(wxCommandEvent& event)
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	string strHotKey = m_cbxHotKey->GetValue();
	if ( !pCfgMgr->IsShortKeyUsable(strHotKey))
	{
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
		m_cbxHotKey->SetValue(wxEmptyString);
		return;
	}

	if( m_listCtrl->GetSelectedItemCount() == 0 )
	{
		return;
	}

	for( int i=0; i<m_listCtrl->GetItemCount(); ++i )
	{
		if ( m_listCtrl->IsSelectedItem( i ) )
		{
			wxString strOldKey = m_listCtrl->GetSubItemText(i, 0);
			m_listCtrl->SetItem( i,0,strHotKey);
			if ( !strOldKey.empty())
			{
				pCfgMgr->AddUsableShortCutKey(strOldKey.c_str());
			}
			pCfgMgr->DelUsableShortCutKey(strHotKey);
		}
	}

}
