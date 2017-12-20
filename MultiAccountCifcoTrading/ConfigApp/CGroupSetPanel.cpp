#include "stdafx.h"

#include <vector>
#include <string>
#include "..\\inc\\cfgMgr\\CfgMgr.h"

#include "CGroupSetPanel.h"

#define MAX_GROUP_NUM 8

extern vector<GroupInfo2X> g_vecGroupInfo;

BEGIN_EVENT_TABLE(CGroupSetPanel,wxPanel)
	EVT_LIST_EXT_BEGIN_LABEL_EDIT(ID_GROUPLIST, BeginEdit)
	EVT_BUTTON(ID_ADD, OnAdd)
	EVT_BUTTON(ID_DEL, OnDel)
	EVT_BUTTON(ID_UP, OnUp)
	EVT_BUTTON(ID_DOWN, OnDown)
	EVT_LIST_EXT_END_LABEL_EDIT(wxID_ANY, OnLabelEndEdit)
END_EVENT_TABLE()

CGroupSetPanel::CGroupSetPanel(wxWindow *parent,
							   wxWindowID winid ,
							   const wxPoint& pos ,
							   const wxSize& size ,
							   long style,
							   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	new wxStaticBox(this, wxID_ANY, LOADSTRING(GROUP), wxPoint(9, 0), wxSize(423, 317));
	m_groupListCtrl = new wxExtListCtrl( this, ID_GROUPLIST, wxPoint(22, 18), 
		wxSize(288, 264), wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE);

	new wxButton(this, ID_ADD, LOADSTRING(ASKNAMEDLG_ADD), wxPoint(25, 287), wxSize(75, 25), 0);
	new wxButton(this, ID_DEL, LOADSTRING(ASKNAMEDLG_DEL), wxPoint(116, 287), wxSize(75, 25), 0);

	new wxBitmapButton(this, ID_UP, wxBitmap("zqBITMAP_UP"), wxPoint(320, 93));
	new wxBitmapButton(this, ID_DOWN, wxBitmap("zqBITMAP_DOWN"), wxPoint(320, 155));

	ReadCfg();
	InitControls();
}

void CGroupSetPanel::InitControls()
{
	m_groupListCtrl->SetTextCtrlMaxLength(5);
	wxExtListItem iteminfo;
	iteminfo.m_mask = wxLISTEXT_MASK_TEXT ;
	iteminfo.m_text = LOADSTRING(GROUP_NAME);
	m_groupListCtrl->InsertColumn(0,iteminfo );
	iteminfo.m_text = LOADSTRING(SHOW_OR_NOT);
	m_groupListCtrl->InsertColumn(1,iteminfo);
	m_groupListCtrl->SetColumnCtrlType(0,COLUMN_CTRL_TYPE_EDIT);
	m_groupListCtrl->SetColumnCtrlType(1,COLUMN_CTRL_TYPE_CHECKBOX);
	for(int i=0; i<2; i++)
	{
		m_groupListCtrl->SetColumnWidth(i,100);
	}

	for( unsigned int i=0; i<g_vecGroupInfo.size(); i++)
	{
		string str = g_vecGroupInfo[i].GroupName;
		m_groupListCtrl->InsertItem(i,str);
		m_groupListCtrl->SetItemChecked(i,g_vecGroupInfo[i].bChecked);
		m_groupListCtrl->SetItemData( i, g_vecGroupInfo[i].GroupId );
	}
}

void  CGroupSetPanel::OnAdd(wxCommandEvent& event)
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int rownum = m_groupListCtrl->GetItemCount();
	if ( rownum >= MAX_GROUP_NUM )
	{
		wxMessageBox(LOADSTRING(MAX_GROUP_IS_EIGHT),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
		return;
	}

	for ( int i = 0; i < rownum; i++)
	{
		if ( m_groupListCtrl->GetSubItemText(i, 0).IsEmpty())
		{
			//m_groupListCtrl->setf
			//m_groupListCtrl->SetItemState(i,wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);
			m_groupListCtrl->Select(i);
			m_groupListCtrl->Edit(i, 0);
			return;
		}
	}

	m_groupListCtrl->InsertItem( 0 ,wxEmptyString);
	m_groupListCtrl->SetItemChecked(0,true);
	m_groupListCtrl->SetItemData( 0, pCfgMgr->GetNextGroupId());

	vector<long> vec;
	for ( int i = 0; i < m_groupListCtrl->GetItemCount(); i++)
	{
		long lData = m_groupListCtrl->GetItemData( i );
		vec.push_back( lData );
	}
}

void CGroupSetPanel::OnLabelEndEdit(wxExtListEvent& event)
{
	wxString strGroupName = event.GetLabel();
	if( strGroupName.empty() )
	{
		return;
	}

	int nRow = event.GetIndex();
	for( int i=0; i< m_groupListCtrl->GetItemCount(); ++i)
	{
		if ( i == nRow )
		{
			continue;
		}

		if ( strGroupName == m_groupListCtrl->GetSubItemText(i,0) )
		{
			wxMessageBox(LOADSTRING(DUPLICATE_FIELD),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
			event.Veto();
			return;
		}
	}

	int nGroupId = m_groupListCtrl->GetItemData( nRow );
	bool bFind = false;
	for ( int i = 0; i < (int)g_vecGroupInfo.size(); i++ )
	{
		if ( nGroupId == g_vecGroupInfo[i].GroupId )
		{
			g_vecGroupInfo[i].GroupName = strGroupName.c_str();
			bFind = true;
			break;
		}
	}

	if ( !bFind )
	{
		GroupInfo2X info;
		info.GroupId = nGroupId;
		info.GroupName = strGroupName;
		g_vecGroupInfo.push_back( info );
	}

	wxCommandEvent myEvent( wxEVT_GROUP_CHANGE );
	GetParent()->AddPendingEvent( myEvent );
}

void CGroupSetPanel::OnDel(wxCommandEvent& event)
{
	int rownum = m_groupListCtrl->GetItemCount();
	if(rownum==0)return;

	int selcount=0,validcount=0;
	for( int i=0; i< rownum; i++ )
	{
		if(!m_groupListCtrl->GetSubItemText(i,0).IsEmpty())
		{
			validcount++;

			if( m_groupListCtrl->IsSelectedItem( i) )
				selcount++;
		}
	}
	if(selcount==validcount)
	{
		wxMessageBox(LOADSTRING(CANNOT_LAST_GROUP),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
		return;
	}
	for( int i=0; i< rownum; i++ )
	{
		if( m_groupListCtrl->IsSelectedItem( i) )
		{
			if( !m_groupListCtrl->GetSubItemText(i,0).IsEmpty())
			{
				int nGroupId = m_groupListCtrl->GetItemData( i );
				
				for( vector<GroupInfo2X>::iterator it =g_vecGroupInfo.begin(); it!= g_vecGroupInfo.end(); ++it)
				{
					if( (*it).GroupId == nGroupId )
					{
						g_vecGroupInfo.erase(it);
						break;
					}
				}
			}

			m_groupListCtrl->DeleteItem( i );
			rownum--;
			i--;
		}
	}

	wxCommandEvent myEvent( wxEVT_GROUP_CHANGE );
	GetParent()->AddPendingEvent( myEvent );
}

CGroupSetPanel::~CGroupSetPanel()
{

}

void CGroupSetPanel::WriteCfg()
{	
	vector<GroupInfo2X> vecBakGroupInfo = g_vecGroupInfo;
	g_vecGroupInfo.clear();
	for( int i=0; i<m_groupListCtrl->GetItemCount(); i++)
	{
		string strGroupName = m_groupListCtrl->GetSubItemText(i,0);
		bool bVisible = m_groupListCtrl->GetItemChecked(i);
		if( strGroupName.empty())
		{
			continue;
		}

		for ( unsigned int j = 0; j < vecBakGroupInfo.size(); j++)
		{
			if ( strGroupName == vecBakGroupInfo[j].GroupName )
			{
				if ( bVisible != vecBakGroupInfo[j].bChecked )
				{
					vecBakGroupInfo[j].bChecked = bVisible;
				}
				
				g_vecGroupInfo.push_back( vecBakGroupInfo[j]);
			}
		}
	}
}

void CGroupSetPanel::ReadCfg()
{
	CfgMgr *pCfgMgr = CFG_MGR_DEFAULT();
	if( !pCfgMgr) return;

	if( g_vecGroupInfo.size() !=0 )
		return;

	int count = pCfgMgr->GetInstrumentGroupCount();
	for( int i =0 ; i<count; i++ )
	{
		GroupInfo2X GroupInfo;
		GroupInfo.GroupId = 0;
		LPINSTRUMENT_GROUP_CFG info = pCfgMgr->GetInstrumentGroupInfo(i);
		std::string strname = info->GroupName;
		GroupInfo.GroupName = info->GroupName;
		GroupInfo.InstrVec = *pCfgMgr->GetGroupMember(i);

		LPINSTRUMENT_GROUP_CFG p =  pCfgMgr->GetInstrumentGroupInfo(i);	
		GroupInfo.bChecked = p->IsVisibility;
		GroupInfo.GroupId = p->GroupId;
		g_vecGroupInfo.push_back( GroupInfo ); 
	}
}

void  CGroupSetPanel::BeginEdit(wxExtListEvent& event)
{
	int index = event.GetIndex();
	string ZuStr = m_groupListCtrl->GetSubItemText(index,0);
	m_groupListCtrl->SetItem(index,0,ZuStr);
	m_groupListCtrl->SetItemState( index,wxLISTEXT_HITTEST_TORIGHT|~wxLISTEXT_STATE_FOCUSED|~wxLISTEXT_STATE_SELECTED,~wxLISTEXT_STATE_SELECTED);
}

void CGroupSetPanel::OnUp( wxCommandEvent& event )
{
	int count = m_groupListCtrl->GetSelectedItemCount();
	if( count == 0 )
		return;

	for(int i = 1; i < m_groupListCtrl->GetItemCount(); i++)
	{
		if( m_groupListCtrl->IsSelectedItem( i ) )
		{
			m_groupListCtrl->ChangeRow(i,true);
		}
	}
}

void CGroupSetPanel::OnDown( wxCommandEvent& event )
{
	int count = m_groupListCtrl->GetSelectedItemCount();
	if( count == 0 )
		return;

	int nRowNum = m_groupListCtrl->GetItemCount();
	for( int i = nRowNum-2; i >= 0; i--)
	{
		if( m_groupListCtrl->IsSelectedItem( i ) )
		{
			m_groupListCtrl->ChangeRow(i,false);
		}
	}
}
