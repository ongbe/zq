#include "stdafx.h"
#include "CVarietyInfoSetPanel.h"
#include "ConfigAppDlg.h"

BEGIN_EVENT_TABLE(CVarietyInfoSetPanel,wxPanel)
	EVT_BUTTON(ID_BtAdd,OnAdd)
	EVT_BUTTON(ID_BtSub,OnDel)
	EVT_COMBOBOX(ID_Com, OnSelc) 
	EVT_LIST_EXT_ITEM_SELECTED(ID_List,ItemSel)
	EVT_LIST_CHAR_HOOK(OnKeyFun)
	EVT_LIST_EXT_END_LABEL_EDIT(wxID_ANY,OnLabelEndEdit)
END_EVENT_TABLE()

CVarietyInfoSetPanel::CVarietyInfoSetPanel(wxWindow *parent,
										   wxWindowID winid ,
										   const wxPoint& pos ,
										   const wxSize& size ,
										   long style,
										   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{

	new wxStaticText(this, wxID_ANY, LOADSTRING(SET_DEFAULT_PARAMETER), wxPoint(20, 4), wxDefaultSize, 0);
	m_listCtrl = new wxExtListCtrl(this, ID_List, wxPoint(25, 20), wxSize(452, 284), wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE);

	new wxButton(this, ID_BtAdd, LOADSTRING(ASKNAMEDLG_ADD), wxPoint(307, 308), wxSize(75, 25), 0);
	new wxButton(this, ID_BtSub, LOADSTRING(ASKNAMEDLG_DEL), wxPoint(404, 308), wxSize(75, 25), 0);
	new wxStaticText(this, wxID_ANY, LOADSTRING(ACCELERATOR), wxPoint(25, 310), wxDefaultSize, 0);

	m_cbx = new wxOwnerDrawnComboBox(this, ID_Com, wxEmptyString, wxPoint(90, 308), wxSize(119, 20), 0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	ReadCfg();
	SetListData();
}

CVarietyInfoSetPanel::~CVarietyInfoSetPanel()
{
}

void CVarietyInfoSetPanel::SetListData()
{
	wxExtListItem iteminfo;
	iteminfo.m_mask = wxLISTEXT_MASK_TEXT;
	iteminfo.m_text = LOADSTRING(CONTRACT_CODE);
	m_listCtrl->InsertColumn(0,iteminfo );
	iteminfo.m_text = LOADSTRING(DEFAULT_VOLUME);
	m_listCtrl->InsertColumn(1,iteminfo);
	iteminfo.m_text = LOADSTRING(ACCELERATOR);
	m_listCtrl->InsertColumn(2,iteminfo );
	m_listCtrl->InsertColumn(3,LOADSTRING(STOP_PROFIT_PRICE_OFFSET));
	m_listCtrl->InsertColumn(4,LOADSTRING(STOP_LOSE_PRICE_OFFSET));
	m_listCtrl->InsertColumn(5,LOADSTRING(REVERSE_LIMIT_PRICE_OFFSET));


	for(int i=0; i<5; i++)
	{
		m_listCtrl->SetColumnWidth(i,80);
	}

	m_listCtrl->SetColumnCtrlType(0,COLUMN_CTRL_TYPE_EDIT);
	m_listCtrl->SetColumnCtrlType(1,COLUMN_CTRL_TYPE_EDIT);
	m_listCtrl->SetColumnCtrlType(3,COLUMN_CTRL_TYPE_EDIT);
	m_listCtrl->SetColumnCtrlType(4,COLUMN_CTRL_TYPE_EDIT);
	m_listCtrl->SetColumnCtrlType(5,COLUMN_CTRL_TYPE_EDIT);

	char buf[10];
	for( unsigned int i=0; i<m_varietyInfo->size(); i++)
	{
		memset(buf, 0, sizeof(buf));
		m_listCtrl->InsertItem(i,(*m_varietyInfo)[i].InstruName);
		if((*m_varietyInfo)[i].volume!=-1)
			sprintf(buf,"%d",(*m_varietyInfo)[i].volume);
		m_listCtrl->SetItem( i,1,buf);
		m_listCtrl->SetItem( i,2,(*m_varietyInfo)[i].ShortKey);
		m_listCtrl->SetItem(i,3,(*m_varietyInfo)[i].profit);
		m_listCtrl->SetItem(i,4,(*m_varietyInfo)[i].loss);
		m_listCtrl->SetItem(i,5,(*m_varietyInfo)[i].reverse);
	}

}

void CVarietyInfoSetPanel::OnAdd(wxCommandEvent& event)
{
	int rownum = m_listCtrl->GetItemCount();
	if( rownum ==0)
	{
		m_listCtrl->InsertItem( rownum ,wxEmptyString);
		m_listCtrl->SetItem( rownum,1,"1" );
		m_listCtrl->SetItemState(rownum,wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);
		return;
	}

	for( int i=0; i<rownum; ++i)
	{
		if( m_listCtrl->IsSelected(i))
			m_listCtrl->SetItemState(i,~wxLISTEXT_STATE_SELECTED,wxLISTEXT_MASK_STATE);
	}
	if( m_listCtrl->GetSubItemText(rownum-1,0) != "")
	{
		m_listCtrl->InsertItem( rownum ,wxEmptyString);
		m_listCtrl->SetItem( rownum,1,"1" );
		m_listCtrl->SetItemState(rownum,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED);
		return;
	}
	m_listCtrl->SetItemState(rownum-1,wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);  
}

void CVarietyInfoSetPanel::OnDel(wxCommandEvent& event)
{
	int rownum = m_listCtrl->GetItemCount();
	for( int i=0; i< rownum; i++ )
	{
		if( m_listCtrl->IsSelectedItem( i) )
		{
			wxString strOldKey = m_listCtrl->GetSubItemText(i, 2);
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

void CVarietyInfoSetPanel::ReadCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	m_varietyInfo = pMgr->GetVarietyInfoVector();
	std::map<int, std::string> shortCutKeyMap = pMgr->GetShortCutKeyNameMap();
	map<int ,string>::iterator it = shortCutKeyMap.begin();
	wxArrayString arr;
	for( ; it!= shortCutKeyMap.end(); ++it )
	{
		arr.Add(it->second);
	}

	m_cbx->Append(wxEmptyString);
	m_cbx->Append(arr);
}

void CVarietyInfoSetPanel::OnSelc( wxCommandEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	wxString str = m_cbx->GetValue();
	if ( !pCfgMgr->IsShortKeyUsable(str.c_str()))
	{
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
		return;
	}

	int ncount = m_listCtrl->GetSelectedItemCount();
	if( ncount ==0 )
		return;

	for( int i=0; i<m_listCtrl->GetItemCount(); ++i )
	{
		if ( m_listCtrl->IsSelectedItem( i ) )
		{
			wxString strOldKey = m_listCtrl->GetSubItemText(i, 2);
			m_listCtrl->SetItem( i,2,str);
			if ( !strOldKey.IsEmpty())
			{
				pCfgMgr->AddUsableShortCutKey(strOldKey.c_str());
			}
			pCfgMgr->DelUsableShortCutKey(str.c_str());
		}
	}
}

bool CVarietyInfoSetPanel::IsChanged()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL)
	{
		return false;
	}

	m_varietyInfo = pMgr->GetVarietyInfoVector();
	int row = m_listCtrl->GetItemCount();
	string str;
	vector<VarietyInfo> vecTemp;
	for( int i=0; i<row; i++)
	{  
		VarietyInfo info;
		str = m_listCtrl->GetSubItemText( i,0 );
		//(*m_VarietyInfo)[i].InstruName = str;
		if( str.empty())continue;
		strncpy(info.InstruName, str.c_str(), sizeof(info.InstruName)-1);

		str = m_listCtrl->GetSubItemText( i,1 );
		//(*m_VarietyInfo)[i].volume = atoi(str.c_str());
		info.volume = atoi(str.c_str());
		if(info.volume==0) info.volume=-1;
		str = m_listCtrl->GetSubItemText( i,2 );
		//(*m_VarietyInfo)[i].ShortKey = str;  
		strncpy(info.ShortKey, str.c_str(), sizeof(info.ShortKey)-1);

		str = m_listCtrl->GetSubItemText(i,3);
		strncpy(info.profit, str.c_str(), sizeof(info.profit)-1);

		str = m_listCtrl->GetSubItemText(i,4);
		strncpy(info.loss, str.c_str(), sizeof(info.loss)-1);

		str = m_listCtrl->GetSubItemText(i,5);
		strncpy(info.reverse, str.c_str(), sizeof(info.reverse)-1);

		vecTemp.push_back( info);
	}

	if ( m_varietyInfo->size() == vecTemp.size() )
	{
		m_varietyInfo->clear();
		m_varietyInfo->assign( vecTemp.begin(), vecTemp.end());
		return true;
	}
	else
	{
        if ( vecTemp.empty())
		{
			return false;
		}

		if( 0 == memcmp( reinterpret_cast<const VOID *>(m_varietyInfo),
			reinterpret_cast<const VOID *>(&vecTemp), 
			sizeof(VarietyInfo)*vecTemp.size()))
		{
			return false;
		}

		m_varietyInfo->clear();
		m_varietyInfo->assign( vecTemp.begin(), vecTemp.end());
		return true;
	}
}

bool CVarietyInfoSetPanel::WriteCfg()
{
	CfgMgr * pMgr =  CFG_MGR_DEFAULT();  
	if( pMgr == NULL)
	{
		return false;
	}

	m_varietyInfo = pMgr->GetVarietyInfoVector();
	int row = m_listCtrl->GetItemCount();
	string str;
	vector<VarietyInfo> vecTemp;
	for( int i=0; i<row; i++)
	{  
		VarietyInfo info;
		str = m_listCtrl->GetSubItemText( i,0 );
		//(*m_VarietyInfo)[i].InstruName = str;
		if( str=="")continue;
		strncpy(info.InstruName, str.c_str(), sizeof(info.InstruName)-1);

		str = m_listCtrl->GetSubItemText( i,1 );
		//(*m_VarietyInfo)[i].volume = atoi(str.c_str());
		info.volume = atoi(str.c_str());
		if(info.volume==0) info.volume=-1;
		str = m_listCtrl->GetSubItemText( i,2 );
		//(*m_VarietyInfo)[i].ShortKey = str;  
		strncpy(info.ShortKey, str.c_str(), sizeof(info.ShortKey)-1);

		str = m_listCtrl->GetSubItemText(i,3);
		strncpy(info.profit, str.c_str(), sizeof(info.profit)-1);

		str = m_listCtrl->GetSubItemText(i,4);
		strncpy(info.loss, str.c_str(), sizeof(info.loss)-1);

		str = m_listCtrl->GetSubItemText(i,5);
		strncpy(info.reverse, str.c_str(), sizeof(info.reverse)-1);

		vecTemp.push_back( info);
	}

	if ( m_varietyInfo->size() != vecTemp.size() )
	{
		m_varietyInfo->clear();
		m_varietyInfo->assign( vecTemp.begin(), vecTemp.end());
		return true;
	}
	else
	{
        if ( vecTemp.empty())
		{
			return false;
		}

		bool bChanged = false;
		for ( unsigned int i = 0; i < vecTemp.size(); i++)
		{
			if ( m_varietyInfo->at(i).volume != vecTemp.at(i).volume ||
				0 != strcmp(m_varietyInfo->at(i).InstruName, vecTemp.at(i).InstruName) ||
				0 != strcmp(m_varietyInfo->at(i).ShortKey, vecTemp.at(i).ShortKey) ||
				0 != strcmp(m_varietyInfo->at(i).profit, vecTemp.at(i).profit) ||
				0 != strcmp(m_varietyInfo->at(i).loss, vecTemp.at(i).loss) ||
				0 != strcmp(m_varietyInfo->at(i).reverse, vecTemp.at(i).reverse) )
			{
				(*m_varietyInfo)[i] = vecTemp.at(i);
				bChanged = true;
				break;
			}
		}

		return bChanged;
	}
}

void CVarietyInfoSetPanel::ItemSel(wxExtListEvent& event)
{
	//int rowindex = event.GetIndex();
	//int colindex = event.GetColumn();
	int rownum = m_listCtrl->GetItemCount();
	if(rownum==0)
		return;
	string str = m_listCtrl->GetSubItemText(rownum-1,0);
	for( int i=0; i<m_listCtrl->GetItemCount()-1; ++i)
	{
		string name = m_listCtrl->GetSubItemText(i,0);
		if( string( strupr((char*)name.c_str())) == string(strupr((char*)str.c_str())) )
		{
			wxMessageBox(LOADSTRING(DUPLICATE_FIELD),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			m_listCtrl->SetItem(rownum-1,0,"");
			break;
		}
	}

	event.Skip();
}

void CVarietyInfoSetPanel::OnKeyFun(wxCommandEvent&  event)
{ 
	wxKeyEvent* pEvt=(wxKeyEvent*)event.GetClientData();
	//int key = pEvt->GetKeyCode();
	//if( key != WXK_NUMPAD_ENTER ||key !=WXK_RETURN)
	//{
	//	int x =0;
	//}

	int rownum = m_listCtrl->GetItemCount();
	if(rownum==0)
		return;
	string str = m_listCtrl->GetSubItemText(rownum-1,0);
	for( int i=0; i<m_listCtrl->GetItemCount()-1; ++i)
	{
		string name = m_listCtrl->GetSubItemText(i,0);
		if( string( strupr((char*)name.c_str())) == string(strupr((char*)str.c_str())) )
		{
			wxMessageBox(LOADSTRING(DUPLICATE_FIELD),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			m_listCtrl->SetItem(rownum-1,0,"");
			break;
		}
	}
	delete pEvt;
}

void CVarietyInfoSetPanel::OnLabelEndEdit(wxExtListEvent& event)
{
	int rownum = m_listCtrl->GetItemCount();
	if(rownum==0)
		return;
	string str = event.GetLabel();
	for( int i=0; i<m_listCtrl->GetItemCount()-1; ++i)
	{
		string name = m_listCtrl->GetSubItemText(i,0);
		if( strupr((char*)name.c_str()) == string(strupr((char*)str.c_str())))
		{
			wxMessageBox(LOADSTRING(DUPLICATE_FIELD),LOADSTRING(USERLOGINDLG_ERROR),wxOK|wxICON_QUESTION);
			event.Veto();
			break;
		}
	}
}