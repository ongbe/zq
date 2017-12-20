
#include "stdafx.h"
#include "cfgMgr/CfgMgr.h"
#include "GroupSetDlg.h"
#include "J_ZiXuanHeYueDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(GroupSetDlg,wxDialog)
	EVT_BUTTON(ID_ADD,OnAdd)
	EVT_BUTTON(ID_DEL,OnDel)
	EVT_BUTTON(ID_UP,OnUp)
	EVT_BUTTON(ID_DOWN,OnDown)
	EVT_BUTTON(ID_OK,OnReturn)
	EVT_LIST_EXT_ITEM_SELECTED(wxID_ANY, OnSelected)
END_EVENT_TABLE()

GroupSetDlg::GroupSetDlg(J_ZiXuanHeYueDlg *parent)
: wxDialog((wxWindow*)parent, wxID_ANY, LOADSTRING(IDT_GROUPSET),wxDefaultPosition, wxSize(800,280))
, m_pParent(parent)
{
	DIALOG_CREATE_LOG();
	m_pComGroup = new wxOwnerDrawnComboBox(this, wxID_ANY, wxEmptyString, wxPoint(64, 22), wxSize(100, 20), 0, 0, wxCB_SIMPLE);
											//this, ID_CBX_INSTRUMENT_GROUP,wxEmptyString, wxPoint(308, 6), wxSize(119, 20),0,0,wxCB_READONLY);

	new wxButton(this, ID_ADD, LOADSTRING(ASKNAMEDLG_ADD), wxPoint(179, 22), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton1"));
	new wxButton(this, ID_DEL, LOADSTRING(ASKNAMEDLG_DEL), wxPoint(179, 52), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton2"));

	m_pList = new wxExtListCtrl(this, ID_LIST, wxPoint(23, 96), wxSize(134, 212), wxLCEXT_REPORT, wxDefaultValidator, wxT("WxListCtrl1"));

	new wxBitmapButton(this, ID_UP, wxBitmap("zqBITMAP_UP"), wxPoint(178, 129));
	new wxBitmapButton(this, ID_DOWN, wxBitmap("zqBITMAP_DOWN"), wxPoint(178, 213));
	new wxButton(this, ID_OK, LOADSTRING(ASKNAMEDLG_OK), wxPoint(190, 284), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton5"));

	new wxStaticText(this, ID_COM, LOADSTRING(GROUP), wxPoint(21, 25), wxDefaultSize, 0, wxT("WxStaticText1"));
	new wxStaticLine(this, wxID_ANY, wxPoint(10, 82), wxSize(269, -1), wxLI_HORIZONTAL);
	new wxStaticBox(this, wxID_ANY, wxT(""), wxPoint(8, 13), wxSize(270, 308));

	SetIcon(wxNullIcon);
	SetSize(103,10,294,363);
	Center();
}

void GroupSetDlg::InitCtrols(vector<INSTRUMENT_GROUP_CFG>& vGroup)
{
	m_vGroup = vGroup;
	for(int unsigned i=0; i<m_vGroup.size(); i++)
	{
		m_pComGroup->AppendString(m_vGroup[i].GroupName);
	}

	m_pList->InsertColumn(0,LOADSTRING(GROUP));
	m_pList->InsertColumn(1, LOADSTRING(SHOW_OR_NOT));
	m_pList->SetColumnCtrlType( 1,COLUMN_CTRL_TYPE_CHECKBOX );

	for( unsigned int i=0;i<m_vGroup.size(); i++)
	{
		m_pList->InsertItem(i,m_vGroup[i].GroupName);
		m_pList->SetItemChecked(i, m_vGroup[i].IsVisibility);
	}

}

GroupSetDlg::~GroupSetDlg()
{

}

void GroupSetDlg::OnSelected(wxExtListEvent& event)
{
	int row = event.GetIndex();
	string str = m_pList->GetSubItemText(row,0);

	m_pComGroup->SetValue(str);
	event.Skip();
}

void GroupSetDlg::OnAdd(wxCommandEvent& event)
{
	string str = m_pComGroup->GetValue();
	if(str.length()==0)
	{
		wxMessageBox(LOADSTRING(GROUP_NAME_CANNOT_BE_EMPTY), LOADSTRING(ABOUTDLG_TITLE));
		return;
	}
	if(str.length()>10)
	{
		wxMessageBox( LOADSTRING(CONFIG_GROUP_MAX_LEN),LOADSTRING(ABOUTDLG_TITLE));
		return;
	}
	//if(!CURRENT_SVR()||!CURRENT_SVR()->ExchangeName2ID(str).empty())
	//	return;

	// int ncount = m_vGroup.size();
	if(m_pList->GetItemCount()>=8)
	{
		wxMessageBox(LOADSTRING(MAX_GROUP_IS_EIGHT),LOADSTRING(ABOUTDLG_TITLE));
		return;
	}

	int row = m_pList->GetItemCount();

	for( int i=0; i<row; i++)
	{
		string name = m_pList->GetSubItemText(i,0);
		if( str == name )
		{
			wxMessageBox(LOADSTRING(DUPLICATE_FIELD),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
			m_pComGroup->SetValue( wxEmptyString );
			return;
		}
	}

	m_pComGroup->AppendString(str);
	m_pList->InsertItem(0,str);
	m_pList->SetItemChecked(0,true);	

	INSTRUMENT_GROUP_CFG info;
	strncpy(info.GroupName,str.c_str(),GROUPNAME_LENGTH);
	info.IsVisibility = true;
	info.GroupId = 0;
	m_vGroup.insert(m_vGroup.begin(),info);
}


void GroupSetDlg::OnDel(wxCommandEvent& event)
{
	string str = m_pComGroup->GetValue();
	if(m_pList->GetItemCount()==1)
	{
		wxMessageBox(LOADSTRING(CANNOT_LAST_GROUP),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION);
		return;
	}
	if(str.empty())
		return;


	int index = m_pComGroup->FindString(str);
	if(index!= -1)
		m_pComGroup->Delete(index);
	else 
	{
		return;
	}

	int row = m_pList->GetItemCount();

	string name;
	for( int i=0; i<row; i++)
	{
		name = m_pList->GetSubItemText(i,0);
		if(name == str)
		{
			m_pList->DeleteItem(i);
			return;
		}
	}

	m_vGroup.clear();
	INSTRUMENT_GROUP_CFG info;
	int count = m_pList->GetItemCount();
	for( int i=0; i<count;i++)
	{
        strncpy(info.GroupName,m_pList->GetSubItemText(i,0).c_str(),GROUPNAME_LENGTH);
		info.IsVisibility = m_pList->GetItemChecked(i);
		m_vGroup.push_back(info);
	}
	m_pComGroup->SetValue("");
}


void GroupSetDlg::OnUp(wxCommandEvent& event)
{
	int count = m_pList->GetSelectedItemCount();
	int j = 0;
	if( count == 0 )
		return;

	string ZuStr;

	int row;
	int rownum = m_pList->GetItemCount();
	wxExtListItem item;
	item.SetMask(wxLISTEXT_MASK_TEXT|wxLISTEXT_MASK_STATE|wxLISTEXT_SET_ITEM);
	bool a;
	for(int i=0; i< rownum; i++)
	{
		if( j >count )
			break;

		if( m_pList->IsSelectedItem( i ) )
		{
			row = i;
			if(row == 0) {
				j++;
				continue;
			}

			item.SetId(row);
			item.SetColumn(0);

			m_pList->GetItem( item );
			a = m_pList->GetItemChecked( row );
			m_pList->DeleteItem( item );

			m_pList->InsertItem( row-1,item.GetText() );
			m_pList->SetItemChecked( row-1,a );
			item.SetId( row-1 );
			m_pList->SetItemState( row-1,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);

		}

		if(j == count)
			break;
	}
}


void GroupSetDlg::OnDown(wxCommandEvent& event)
{
	int count = m_pList->GetSelectedItemCount();
	if( count == 0 )
		return;

	string ZuStr;
	int row = 0;
	int rownum = m_pList->GetItemCount();
	int j = 0;

	wxExtListItem item;
	item.SetMask(wxLISTEXT_MASK_TEXT);

	for(int i = rownum-1; i >= 0; i--)
	{
		if( m_pList->IsSelectedItem( i ) )
		{
			row = i;
			if(row == rownum -1) {
				j++;
				continue;
			}

			item.SetId(row);
			item.SetColumn(0);

			m_pList->GetItem( item );
			bool a = m_pList->GetItemChecked( row );
			m_pList->DeleteItem( item );
			m_pList->InsertItem( row+1,item.GetText() );
			m_pList->SetItemChecked( row+1,a );
			m_pList->SetItemState( row+1,wxLISTEXT_STATE_FOCUSED|wxLISTEXT_STATE_SELECTED,wxLISTEXT_STATE_SELECTED);
		}
	}
}

void GroupSetDlg::OnReturn(wxCommandEvent& event)
{
	vector<INSTRUMENT_GROUP_CFG> temp = m_vGroup;

	m_vGroup.clear();
	INSTRUMENT_GROUP_CFG info;
	info.GroupId = 0;
	for( int i=0; i< m_pList->GetItemCount() ;i++)
	{
        strncpy(info.GroupName,m_pList->GetSubItemText(i,0).c_str(),GROUPNAME_LENGTH);
		info.IsVisibility = m_pList->GetItemChecked(i);
		for( size_t j = 0; j < temp.size(); j++)
		{
			if( info.GroupName == temp[j].GroupName)
			{
				info.GroupId = temp[j].GroupId;
				break;
			}

		}
		m_vGroup.push_back(info);
	}

	if ( NULL != m_pParent )
	{
		m_pParent->SetGroupVec( m_vGroup );
	}

	this->EndDialog(wxID_OK);
}


