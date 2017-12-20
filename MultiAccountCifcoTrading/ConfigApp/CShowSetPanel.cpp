#include "stdafx.h"
#include <algorithm>
#include "CShowSetPanel.h"
#include "CFontAndColorSetPanel.h"
#include "ConfigAppDlg.h"
#include "../inc/gui/MainFrame.h"

extern wxPanel* g_BiaoGeZiTiYanSe;
#include "CShowSetPanel.h"
#define Row_Name      0
#define Row_ShortKey  1
#define Row_AliasName 2

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(CShowSetPanel,wxPanel)
	EVT_LIST_EXT_ITEM_SELECTED( ID_MYLIST,OnListItemSel)  
    EVT_COMBOBOX(ID_ComShortKey, OnSelShortkey) 
	EVT_BUTTON(ID_BtSet,OnSet)
	EVT_COMBOBOX(ID_ComColor, OnSelColor)  
END_EVENT_TABLE()

CShowSetPanel::CShowSetPanel(wxWindow *parent,
				   wxWindowID winid ,
				   const wxPoint& pos ,
				   const wxSize& size ,
				   long style,
				   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;
	vector<PanelInfo>* pPanelInfo = pMgr->GetPanelSetVector();
	LPMAIN_WINDOW_CFG p = pMgr->GetMainWindowCfg();

	new wxStaticBox(this, wxID_ANY, LOADSTRING(LIST_PANEL), wxPoint(18, 0), wxSize(467, 217));
	wxCheckBox* Box1 = new wxCheckBox(this, ID_FIXPANEL, LOADSTRING(FIX_LIST_POS), wxPoint(200, 9), wxSize(97, 17));
	Box1->SetValue( p->bFixPanel );
	wxCheckBox*  Box2 = new wxCheckBox(this, ID_SHOWKEY, LOADSTRING(SHOW_SHORTCUTKEY_ON_TITLE), wxPoint(310, 9), wxSize(160, 17));
	Box2->SetValue(p->bShowPanelShortKey );
	wxExtListCtrl* List = new wxExtListCtrl(this, ID_MYLIST, wxPoint(29, 29), wxSize(445, 154), wxLIST_CFG_VIEW|wxBORDER_SIMPLE);
	List->InsertColumn(Row_Name,LOADSTRING(LIST_NAME),wxLISTEXT_FORMAT_LEFT,170);
	//List->SetColumnReadOnly(Row_Name);
	List->InsertColumn(Row_ShortKey,LOADSTRING(ACCELERATOR),wxLISTEXT_FORMAT_LEFT,100);
	List->InsertColumn(Row_AliasName,LOADSTRING(ANOTHER_NAME));
	List->SetColumnCtrlType(Row_AliasName,COLUMN_CTRL_TYPE_EDIT);
//	wxCommandEvent MyEvent(wxEVT_GET_PANE_CAPTION);
	for( int i=0; i<(int)pPanelInfo->size(); i++)
	{
        std::string strName=((MainFrame*)GETTOPWINDOW())->GetPanelCaption((*pPanelInfo)[i].id,1);

//		MyEvent.SetId((*pPanelInfo)[i].id);
//		MyEvent.SetInt(1);
//		((wxApp*)wxApp::GetInstance())->GetTopWindow()->ProcessEvent(MyEvent);
//		std::string strName = MyEvent.GetString();
	  	List->InsertItem(i, strName);
		List->SetItem( i,Row_ShortKey,(*pPanelInfo)[i].ShortKey );
		List->SetItem( i,Row_AliasName,(*pPanelInfo)[i].AliasName);
	}	
	new wxStaticText(this, wxID_ANY, LOADSTRING(CHOOSE_SHORTCUTKEY), wxPoint(30, 190), wxDefaultSize);
	map<int,string> shortCutKeyMap = pMgr->GetShortCutKeyNameMap();
	map<int,string>::iterator it;
	wxArrayString choices1;
	choices1.Add(wxEmptyString);
	for( it= shortCutKeyMap.begin(); it!= shortCutKeyMap.end(); ++it )
		choices1.Add(it->second.c_str()); 

	new wxOwnerDrawnComboBox(this, ID_ComShortKey, wxEmptyString, wxPoint(200, 188), wxSize(100, 20), choices1,wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxStaticBox(this, wxID_ANY, LOADSTRING(OTHER), wxPoint(18, 223), wxSize(467, 60));

	wxCheckBox* Box4 = new wxCheckBox(this, ID_MIN2TRAY, LOADSTRING(MINIMIZE_TO_TRAY), wxPoint(30, 245), wxSize(130, 17));
	Box4->SetValue( p->bIsMinTray );

	new wxButton(this,ID_BtSet, LOADSTRING(SELECT_FONT_AND_COLOR), wxPoint(359, 295), wxSize(126, 25));
}

void CShowSetPanel::OnSelShortkey( wxCommandEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	wxOwnerDrawnComboBox* pCombo = ((wxOwnerDrawnComboBox*)FindWindowById(ID_ComShortKey,this)); 
	wxString str=pCombo->GetString(pCombo->GetSelection());
	if ( !pCfgMgr->IsShortKeyUsable(str.c_str()))
	{
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
		return;
	}

	wxExtListCtrl* List =(wxExtListCtrl*)FindWindowById(ID_MYLIST,this);
	if(List->GetSelectedItemCount()>0)
	{
		long lIndex = List->GetFirstSelected();
		wxString strOldKey = List->GetSubItemText(lIndex, Row_ShortKey);
		List->SetItem(lIndex,Row_ShortKey,str);
		if ( !strOldKey.IsEmpty())
		{
			pCfgMgr->AddUsableShortCutKey(strOldKey.c_str());
		}
		pCfgMgr->DelUsableShortCutKey(str.c_str());
	}
}

void CShowSetPanel::OnSelColor( wxCommandEvent& event )
{

}


void CShowSetPanel::OnSet(wxCommandEvent& event)
{
	wxCommandEvent myEvent( wxEVT_SHOW_PANEL );
	myEvent.SetInt( ID_ITEM_FONTANDCOLOR );
	GetParent()->AddPendingEvent( myEvent );
}


CShowSetPanel::~CShowSetPanel()
{

}
bool CShowSetPanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	bool bChanged = false;
    wxExtListCtrl* List =(wxExtListCtrl*)FindWindowById(ID_MYLIST,this);
	vector<PanelInfo>* pPanelInfo = pMgr->GetPanelSetVector();
	int row = List->GetItemCount();
	if ( pPanelInfo->size() != row )
	{
		return false;
	}
	else
	{
		for( int i=0; i<row; i++)
		{
			PanelInfo info;
			strncpy(info.ShortKey, List->GetSubItemText(i,Row_ShortKey ).c_str(), sizeof(info.ShortKey)-1);
			strncpy(info.AliasName, List->GetSubItemText( i,Row_AliasName).c_str(), sizeof(info.AliasName)-1);
			if ( 0 != strcmp((*pPanelInfo)[i].ShortKey, info.ShortKey) ||
				0 != strcmp((*pPanelInfo)[i].AliasName, info.AliasName) )
			{
				strncpy((*pPanelInfo)[i].ShortKey, info.ShortKey, sizeof((*pPanelInfo)[i].ShortKey)-1);
				strncpy((*pPanelInfo)[i].AliasName, info.AliasName, sizeof((*pPanelInfo)[i].AliasName)-1);
				bChanged = true;
			}   
		}
	}
    
	LPMAIN_WINDOW_CFG p = pMgr->GetMainWindowCfg();
    wxCheckBox* Box1=(wxCheckBox*)FindWindowById(ID_FIXPANEL,this);
    wxCheckBox* Box2=(wxCheckBox*)FindWindowById(ID_SHOWKEY,this);
    wxCheckBox* Box4=(wxCheckBox*)FindWindowById(ID_MIN2TRAY,this);
	
	if ( p->bFixPanel != Box1->GetValue())
	{
		p->bFixPanel = Box1->GetValue();
		bChanged = true;
	}
	
	if ( p->bShowPanelShortKey != Box2->GetValue())
	{
		p->bShowPanelShortKey =Box2->GetValue();
		bChanged = true;
	}

	if ( p->bIsMinTray != Box4->GetValue())
	{
		p->bIsMinTray = Box4->GetValue();
		bChanged = true;
	}

	return bChanged;
}

void CShowSetPanel::OnListItemSel(wxExtListEvent& event)
{
	wxOwnerDrawnComboBox* pCombo = ((wxOwnerDrawnComboBox*)FindWindowById(ID_ComShortKey,this)); 
	wxExtListCtrl* List =(wxExtListCtrl*)FindWindowById(ID_MYLIST,this);
	wxString str=List->GetSubItemText(event.GetIndex(),Row_ShortKey);
	if(!pCombo->SetStringSelection(str))
	{
		pCombo->SetSelection(-1);
	}
}

