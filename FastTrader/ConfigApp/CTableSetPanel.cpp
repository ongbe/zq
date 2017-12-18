#include "stdafx.h"

#include "Module-Misc2/globalDefines.h"
#include "CFontAndColorSetPanel.h"
#include "CTableSetPanel.h"

BEGIN_EVENT_TABLE(CTableSetPanel,wxPanel)
	EVT_BUTTON(ID_BtUp,OnUp)
	EVT_BUTTON(ID_BtDown,OnDown)
	EVT_BUTTON(ID_BtSet,OnSetFont)
END_EVENT_TABLE()

CTableSetPanel::CTableSetPanel(wxWindow *parent,
							 wxWindowID winid ,
							 const wxPoint& pos ,
							 const wxSize& size ,
							 int nModuleType,
							 bool bShowCheckBox,
							 long style,
							 const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	m_ModuleType = nModuleType;
	m_List = new wxExtListCtrl(this, ID_List, wxPoint(15, 3), wxSize(242, 328), wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE);  
	m_BtUp = new wxBitmapButton(this, ID_BtUp, wxBitmap("zqBITMAP_UP"), wxPoint(302, 83));
	m_BtDown = new wxBitmapButton(this, ID_BtDown,wxBitmap("zqBITMAP_DOWN"), wxPoint(302, 176));
	m_BtSetTable = new wxButton(this, ID_BtSet, LOADSTRING(SELECT_FONT_AND_COLOR), wxPoint(300, 293), wxSize(121, 25), 0);

	wxExtListItem iteminfo;
	iteminfo.m_mask = wxLISTEXT_MASK_TEXT ;
	iteminfo.m_text = LOADSTRING(TITLE);
	m_List->InsertColumn(0,iteminfo );
	m_List->SetColumnWidth( 0, 100 );
	
	if ( bShowCheckBox )
	{
		iteminfo.m_text =LOADSTRING(IDT_SHOW);
		m_List->InsertColumn(1,iteminfo);
		m_List->SetColumnWidth( 1, 100 );
		m_List->SetColumnCtrlType(1,COLUMN_CTRL_TYPE_CHECKBOX);
	}

	SetListData();
}

CTableSetPanel::~CTableSetPanel()
{
}

void CTableSetPanel::SetListData()
{   
	m_List->DeleteAllItems();
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return ;

	LPLIST_TABLE_CFG pTable = pMgr->GetListCfg(m_ModuleType);
	if(pMgr == NULL) return;

	int colnum = pMgr->GetListColCount(m_ModuleType );
	for( int i=0; i<colnum; i++ )
	{
		LPLIST_COLUMN_CFG pColCfg = pMgr->GetListColCfg(m_ModuleType, i);
		std::string strName =pMgr->GetColCfgCaption(m_ModuleType,pColCfg->id);
		m_List->InsertItem(i,strName);
		m_List->SetItemData( i, pColCfg->id );
		if( pColCfg->Visibility ==1 )
			m_List->SetItemChecked(i,true);
	}
}


void CTableSetPanel::OnUp(wxCommandEvent& event)
{
	int count = m_List->GetSelectedItemCount();
	int j = 0;
	if( count == 0 )
		return;

	int row;
	int rownum = m_List->GetItemCount();
	wxExtListItem item;
	item.SetMask(wxLISTEXT_MASK_TEXT|wxLISTEXT_MASK_STATE|wxLISTEXT_SET_ITEM);
	for(int i=0; i< rownum; i++)
	{
		if( j >count )
			break;

		if( m_List->IsSelectedItem( i ) )
		{
			row = i;
			if(row == 0) {
				j++;
				continue;
			}

			m_List->ChangeRow(row, true);
		}

		if(j == count)
			break;
	}
}

void CTableSetPanel::OnDown(wxCommandEvent& event)
{
	int count = m_List->GetSelectedItemCount();
	if( count == 0 )
		return;

	int row = 0;
	int rownum = m_List->GetItemCount();
	int j = 0;

	wxExtListItem item;
	item.SetMask(wxLISTEXT_MASK_TEXT);

	for(int i = rownum-1; i >= 0; i--)
	{
		if( m_List->IsSelectedItem( i ) )
		{
			row = i;
			if(row == rownum -1) {
				j++;
				continue;
			}

			m_List->ChangeRow(row, false);
		}
	}
}

bool CTableSetPanel::WriteCfg()
{
	CfgMgr * pMgr = CFG_MGR_DEFAULT();  
	if( pMgr == NULL) return false;

	LPLIST_TABLE_CFG pTable = pMgr->GetListCfg(m_ModuleType);
	if(pTable == NULL) return false;

	bool bChanged = false;
	int ListCount = m_List->GetItemCount();
	string name;
	LPLIST_COLUMN_CFG ColCfg;

	std::vector<LPLIST_COLUMN_CFG>* raw = pMgr->GetListColCfg(m_ModuleType);

	for( int i=0; i<ListCount; i++ )
	{
		long lID = m_List->GetItemData( i );
		int nChecked = static_cast<int>(m_List->GetItemChecked( i ));

		for( unsigned int j=0; j<raw->size(); j++ )
		{
			if( (*raw)[j]->id == lID )
			{
				if ( nChecked != (*raw)[j]->Visibility)
				{
					(*raw)[j]->Visibility = nChecked;
					bChanged = true;
				}

				if ( i != j )
				{
					ColCfg  = (*raw)[j];
					(*raw)[j]  = (*raw)[i];
					(*raw)[i]  = ColCfg;
					bChanged = true;
				}

				break;
			}
		}
	}

	std::vector<LPLIST_COLUMN_CFG> vec( * raw);

	pMgr->SetListColCfg(m_ModuleType, pTable,vec);

	return bChanged;
}


void CTableSetPanel::OnSetFont(wxCommandEvent& event)
{
	wxCommandEvent myEvent( wxEVT_SHOW_PANEL );
	myEvent.SetInt( ID_ITEM_FONTANDCOLOR );
	GetParent()->AddPendingEvent( myEvent );
}
