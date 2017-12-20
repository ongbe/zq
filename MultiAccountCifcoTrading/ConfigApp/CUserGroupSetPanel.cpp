#include "stdafx.h"

#include <vector>
#include <string>
#include "..\\inc\\cfgMgr\\CfgMgr.h"
#include "CUserGroupSetPanel.h"
#include "../inc/MultiAccountCore/MultiAccountCore.h"
#include "../inc/Module-Misc/GLobalFunc.h"


BEGIN_EVENT_TABLE(CUserGroupSetPanel, wxPanel)
	EVT_BUTTON(ID_ADDGROUP, OnAddGroup)
	EVT_BUTTON(ID_DELGROUP, OnDelGroup)
	EVT_BUTTON(ID_ADDALLACCOUNT, OnAddAllAccount)
	EVT_BUTTON(ID_ADDACCOUNT, OnAddAccount)
	EVT_BUTTON(ID_DELACCOUNT, OnDelAccount)
	EVT_BUTTON(ID_DELALLACCOUNT, OnDelAllAccount)
	EVT_LIST_EXT_BEGIN_LABEL_EDIT(ID_USERGROUPLIST, OnLabelBeginEdit)
	EVT_LIST_EXT_END_LABEL_EDIT(ID_USERGROUPLIST, OnLabelEndEdit)
	EVT_LIST_EXT_ITEM_SELECTED(ID_USERGROUPLIST, OnGroupSelected)
	EVT_COMBOBOX(ID_SHORTKEY, OnSelChanged) 
END_EVENT_TABLE()

CUserGroupSetPanel::CUserGroupSetPanel(wxWindow *parent,
							   wxWindowID winid ,
							   const wxPoint& pos ,
							   const wxSize& size ,
							   long style,
							   const wxString& name ):
wxPanel(parent,winid,pos,size,style,name)
{
	const int nLeftStart = 5;
	const int nMidStart = 155;
	const int nGroupOperatorButtonStart = 300;
	const int nRightStart = 355;
	const int nTop = 5;
	const int nListWidth = 140;
	const int nListHeight = 275;
	const int nButtonWidth = 50;
	const int nButtonHeight = 25;

	//左侧用户组列表
	new wxStaticText(this, wxID_ANY, "用户组设置", wxPoint(nLeftStart, nTop), wxSize(60, 20));
	m_userGroupList = new wxExtListCtrl( this, ID_USERGROUPLIST, wxPoint(nLeftStart, nTop+25), 
		wxSize(nListWidth, nListHeight-30), wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE|wxLCEXT_SINGLE_SEL);
	m_userGroupList->InsertColumn(0, "组名", wxALIGN_LEFT, 80);
	m_userGroupList->InsertColumn(1, "快捷键", wxALIGN_LEFT, 60);
	m_userGroupList->SetColumnCtrlType(0,COLUMN_CTRL_TYPE_EDIT);
	m_userGroupList->SetColumnCtrlType(1,COLUMN_CTRL_TYPE_COMBOBOX);

	new wxStaticText(this, wxID_ANY, "快捷键", wxPoint(nLeftStart, nTop+280), wxSize(40, 20));
	m_pCbxShortKey = new wxOwnerDrawnComboBox(this, ID_SHORTKEY, "", wxPoint(nLeftStart+45, nTop+280), 
		wxSize(90, 20),0, wxCB_READONLY|wxST_NO_AUTORESIZE);

	new wxButton(this, ID_ADDGROUP, "新建", wxPoint(nLeftStart+10, nTop+305), wxSize(nButtonWidth, nButtonHeight), 0);
	new wxButton(this, ID_DELGROUP, "删除", wxPoint(nLeftStart+80, nTop+305), wxSize(nButtonWidth, nButtonHeight), 0);

	//中间组内账号列表
	new wxStaticText(this, wxID_ANY, "组内成员列表", wxPoint(nMidStart, nTop), wxSize(80, 20));
	m_groupInfoList = new wxExtListCtrl( this, ID_GROUPINFOLIST, wxPoint(nMidStart, nTop+25), 
		wxSize(nListWidth, nListHeight), wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE);
	m_groupInfoList->InsertColumn(0, "ID", wxALIGN_LEFT, 40);
	m_groupInfoList->InsertColumn(1, "用户名", wxALIGN_LEFT, 100);

	new wxStaticText(this, wxID_ANY, "组内客户数：", wxPoint(nMidStart, nTop+308), wxSize(75, 20));
	m_pAccountNumInGroup = new wxStaticText(this, ID_ACCOUNTNUMBERIN, "0", wxPoint(nMidStart+75, nTop+308), wxSize(40, 20));

	//操作按钮列表
	new wxButton(this, ID_ADDALLACCOUNT, "<<", wxPoint(nGroupOperatorButtonStart, nTop+50), wxSize(nButtonWidth, nButtonHeight), 0);
	new wxButton(this, ID_ADDACCOUNT, "<", wxPoint(nGroupOperatorButtonStart, nTop+100), wxSize(nButtonWidth, nButtonHeight), 0);
	new wxButton(this, ID_DELACCOUNT, ">", wxPoint(nGroupOperatorButtonStart, nTop+150), wxSize(nButtonWidth, nButtonHeight), 0);
	new wxButton(this, ID_DELALLACCOUNT, ">>", wxPoint(nGroupOperatorButtonStart, nTop+200), wxSize(nButtonWidth, nButtonHeight), 0);

	//右侧账号列表
	new wxStaticText(this, wxID_ANY, "组外成员列表", wxPoint(nRightStart, nTop), wxSize(80, 20));
	m_accountList = new wxExtListCtrl( this, ID_GROUPINFOLIST, wxPoint(nRightStart, nTop+25), 
		wxSize(nListWidth, nListHeight), wxLCEXT_REPORT|wxLCEXT_TOGGLE_COLOUR|wxBORDER_SIMPLE);
	m_accountList->InsertColumn(0, "ID", wxALIGN_LEFT, 40);
	m_accountList->InsertColumn(1, "用户名", wxALIGN_LEFT, 100);

	new wxStaticText(this, wxID_ANY, "组外客户数：", wxPoint(nRightStart, nTop+308), wxSize(75, 20));
	m_pAccountNumOutOfGroup = new wxStaticText(this, ID_ACCOUNTNUMBEROUT, "0", wxPoint(nRightStart+75, nTop+308), wxSize(40, 20));

	ReadCfg();
	InitControls();
}

CUserGroupSetPanel::~CUserGroupSetPanel()
{

}

void CUserGroupSetPanel::InitControls()
{
	for ( UINT i = 0; i < m_vAccountGroup.size(); i++ )
	{
		AccountGroup group = m_vAccountGroup[i];
		int nCount = m_userGroupList->GetItemCount();
		m_userGroupList->InsertItem(nCount, group.GetGroupName());
		m_userGroupList->SetItemData( nCount, group.GetGroupID());
		m_userGroupList->SetItem(nCount, 1, group.GetShortKey());
	}

	int nAccountCount = MA_CORE()->GetTradeAccountCount();
	for ( int i = 0; i < nAccountCount; i++ )
	{
		const CTPTradeAccount* pTradeAccount = MA_CORE()->GetTradeAccount(i);
		if ( NULL == pTradeAccount )
		{
			continue;
		}

		int nCount = m_accountList->GetItemCount();
		m_accountList->InsertItem(nCount, "");
		m_accountList->SetItem(nCount, 0, 
			GlobalFunc::ConvertToString(pTradeAccount->t.nID));
		m_accountList->SetItem(nCount, 1, pTradeAccount->t.szTradeAccount);
	}

	wxString strNumber = wxString::Format("%d", m_accountList->GetItemCount());
	m_pAccountNumOutOfGroup->SetLabel(strNumber);
}

void  CUserGroupSetPanel::OnAddGroup(wxCommandEvent& event)
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCount = m_userGroupList->GetItemCount();
	if ( nCount > static_cast<int>(m_vAccountGroup.size()) )
	{
		SelectGroup( nCount - 1  );
		m_userGroupList->Select(nCount-1);
		m_userGroupList->Edit(nCount-1, 0);
		return;
	}

	m_userGroupList->InsertItem( nCount, wxEmptyString);
	m_userGroupList->SetItemData( nCount, pCfgMgr->GetNextAccountGroupID());
	m_userGroupList->Select(nCount);
	m_userGroupList->Edit(nCount, 0);
}

void CUserGroupSetPanel::OnDelGroup( wxCommandEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	int nCount = m_userGroupList->GetItemCount();
	if( nCount == 0) return;

	for( int i = 0; i< nCount; i++ )
	{
		if( m_userGroupList->IsSelectedItem( i) )
		{
			int nGroupID = m_userGroupList->GetItemData(i);
			std::vector<AccountGroup>::iterator it = m_vAccountGroup.begin();
			for ( ; it != m_vAccountGroup.end(); it++ )
			{
				if ( nGroupID == (*it).GetGroupID())
				{
					m_vAccountGroup.erase(it);
					break;
				}
			}

			wxString strShortKey = m_userGroupList->GetSubItemText(i, 1);
			if ( !strShortKey.IsEmpty())
			{
				pCfgMgr->AddUsableShortCutKey(strShortKey.c_str());
			}

			m_userGroupList->DeleteItem(i);
			m_pAccountNumInGroup->SetLabel("0");
			break;
		}
	}

	m_groupInfoList->DeleteAllItems();
	m_pAccountNumInGroup->SetLabel("0");

	//组外客户
	m_accountList->DeleteAllItems();
	int nAccountCount = MA_CORE()->GetTradeAccountCount();
	for ( int i = 0; i < nAccountCount; i++ )
	{
		const CTPTradeAccount* pTradeAccount = MA_CORE()->GetTradeAccount(i);
		if ( NULL == pTradeAccount )
		{
			continue;
		}

		int nCount = m_accountList->GetItemCount();
		m_accountList->InsertItem(nCount, "");
		m_accountList->SetItem(nCount, 0, 
			GlobalFunc::ConvertToString(pTradeAccount->t.nID));
		m_accountList->SetItem(nCount, 1, pTradeAccount->t.szTradeAccount);
	}

	wxString strNumber = wxString::Format("%d", m_accountList->GetItemCount());
	m_pAccountNumOutOfGroup->SetLabel(strNumber);
}

void CUserGroupSetPanel::OnLabelBeginEdit( wxExtListEvent& event )
{
}

void CUserGroupSetPanel::OnLabelEndEdit(wxExtListEvent& event)
{
	wxString strGroupName = event.GetLabel();
	if ( strGroupName.empty() )
	{
		event.Veto();
		return;
	}

	if ( strGroupName.Length() > 40 )
	{
		wxMessageBox("用户组名不能超过20个汉字或40个字符");
		event.Veto();
		m_userGroupList->Edit(event.GetIndex(), 0);
		return;
	}

	if ( IsGroupExist(strGroupName) )
	{
		wxMessageBox("组名重复，请重新输入");
		event.Veto();
		m_userGroupList->Edit(event.GetIndex(), 0);
		return;
	}

	int nIndex = event.GetIndex();
	int nGroupID = m_userGroupList->GetItemData(nIndex);
	wxString strShortKey = m_userGroupList->GetSubItemText(nIndex, 1);
	if ( nIndex >= static_cast<int>(m_vAccountGroup.size()) )
	{
		//新增
		AccountGroup group;
		group.SetGroupAttr(nGroupID, strGroupName.c_str(), strShortKey.c_str());
		m_vAccountGroup.push_back(group);
	}
	else
	{
		//修改
		for ( UINT i = 0; i < m_vAccountGroup.size(); i++ )
		{
			if ( nGroupID == m_vAccountGroup[i].GetGroupID())
			{
				m_vAccountGroup[i].SetGroupAttr(nGroupID, strGroupName.c_str(), strShortKey.c_str());
				break;
			}
		}
	}
}

bool CUserGroupSetPanel::WriteCfg()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return false;
	}

	std::vector<AccountGroup> vTemp;
	pCfgMgr->GetAccountGroup(vTemp);
	pCfgMgr->SetAccountGroup(m_vAccountGroup);

	if ( vTemp.size() != m_vAccountGroup.size())
	{
		return true;
	}

	for ( UINT i = 0; i < vTemp.size(); i++ )
	{
		if ( vTemp[i].GetGroupID() != m_vAccountGroup[i].GetGroupID() ||
			vTemp[i].GetGroupName() != m_vAccountGroup[i].GetGroupName() ||
			vTemp[i].GetAccountCount() != m_vAccountGroup[i].GetAccountCount() )
		{
			return true;
		}
		else
		{
			int nAccountCount = vTemp[i].GetAccountCount();
			for ( int j = 0; j < nAccountCount; j++ )
			{
				AccountStru account1, account2;
				memset(&account1, 0, sizeof(account1));
				memset(&account2, 0, sizeof(account2));
				vTemp[i].GetAccount(j, account1);
				m_vAccountGroup[i].GetAccount(j, account2);
				if ( memcmp(&account1, &account2, sizeof(account1)) != 0 )
				{
					return true;
				}
			}
		}
	}

	return false;
}

void CUserGroupSetPanel::ReadCfg()
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	std::map<int, std::string> shortCutKeyMap = pCfgMgr->GetShortCutKeyNameMap();
	map<int ,string>::iterator it = shortCutKeyMap.begin();
	wxArrayString arr;
	for( ; it!= shortCutKeyMap.end(); ++it )
	{
		arr.Add(it->second);
	}

	m_pCbxShortKey->Append(arr);

	pCfgMgr->GetAccountGroup(m_vAccountGroup);
}

void CUserGroupSetPanel::OnGroupSelected( wxExtListEvent& event )
{
	int nIndex = event.GetIndex();
	SelectGroup(nIndex);
}

void CUserGroupSetPanel::SelectGroup( int nIndex )
{
	m_groupInfoList->DeleteAllItems();
	m_pAccountNumInGroup->SetLabel("0");

	if ( nIndex >= static_cast<int>(m_vAccountGroup.size()))
	{
		//组外客户
		m_accountList->DeleteAllItems();
		int nAccountCount = MA_CORE()->GetTradeAccountCount();
		for ( int i = 0; i < nAccountCount; i++ )
		{
			const CTPTradeAccount* pTradeAccount = MA_CORE()->GetTradeAccount(i);
			if ( NULL == pTradeAccount )
			{
				continue;
			}

			int nCount = m_accountList->GetItemCount();
			m_accountList->InsertItem(nCount, "");
			m_accountList->SetItem(nCount, 0, 
				GlobalFunc::ConvertToString(pTradeAccount->t.nID));
			m_accountList->SetItem(nCount, 1, pTradeAccount->t.szTradeAccount);
		}

		wxString strNumber = wxString::Format("%d", m_accountList->GetItemCount());
		m_pAccountNumOutOfGroup->SetLabel(strNumber);

		return;
	}

	//组内客户
	AccountGroup group = m_vAccountGroup[nIndex];
	for ( int i = 0; i < group.GetAccountCount(); i++ )
	{
		AccountStru accountInfo;
		memset(&accountInfo, 0, sizeof(accountInfo));
		if ( !group.GetAccount(i, accountInfo))
		{
			continue;
		}

		int nCount = m_groupInfoList->GetItemCount();
		m_groupInfoList->InsertItem(nCount, "");
		m_groupInfoList->SetItem(nCount, 0, 
			GlobalFunc::ConvertToString(accountInfo.nID));
		m_groupInfoList->SetItem(nCount, 1, accountInfo.szAccount);
	}

	wxString strNumber = wxString::Format("%d", m_groupInfoList->GetItemCount());
	m_pAccountNumInGroup->SetLabel(strNumber);

	//组外客户
	m_accountList->DeleteAllItems();
	int nAccountCount = MA_CORE()->GetTradeAccountCount();
	for ( int i = 0; i < nAccountCount; i++ )
	{
		const CTPTradeAccount* pTradeAccount = MA_CORE()->GetTradeAccount(i);
		if ( NULL == pTradeAccount )
		{
			continue;
		}

		if ( !IsExistInCurList(pTradeAccount->t.szTradeAccount))
		{
			int nCount = m_accountList->GetItemCount();
			m_accountList->InsertItem(nCount, "");
			m_accountList->SetItem(nCount, 0, 
				GlobalFunc::ConvertToString(pTradeAccount->t.nID));
			m_accountList->SetItem(nCount, 1, pTradeAccount->t.szTradeAccount);
		}
	}

	strNumber = wxString::Format("%d", m_accountList->GetItemCount());
	m_pAccountNumOutOfGroup->SetLabel(strNumber);
}

void CUserGroupSetPanel::OnAddAllAccount( wxCommandEvent& event )
{
	if ( m_userGroupList->GetFirstSelected() < 0)
	{
		return;
	}

	m_groupInfoList->DeleteAllItems();

	int nAccountCount = MA_CORE()->GetTradeAccountCount();
	for ( int i = 0; i < nAccountCount; i++ )
	{
		const CTPTradeAccount* pTradeAccount = MA_CORE()->GetTradeAccount(i);
		if ( NULL == pTradeAccount )
		{
			continue;
		}

		int nCount = m_groupInfoList->GetItemCount();
		m_groupInfoList->InsertItem(nCount, "");
		m_groupInfoList->SetItem(nCount, 0, 
			GlobalFunc::ConvertToString(pTradeAccount->t.nID));
		m_groupInfoList->SetItem(nCount, 1, pTradeAccount->t.szTradeAccount);
	}

	m_accountList->DeleteAllItems();
	wxString strNumber = wxString::Format("%d", m_accountList->GetItemCount());
	m_pAccountNumOutOfGroup->SetLabel(strNumber);

	strNumber = wxString::Format("%d", m_groupInfoList->GetItemCount());
	m_pAccountNumInGroup->SetLabel(strNumber);

	RefreshCurGroupData();
}

void CUserGroupSetPanel::OnAddAccount( wxCommandEvent& event )
{
	if ( m_userGroupList->GetFirstSelected() < 0)
	{
		return;
	}

	int nAccountCount = m_accountList->GetItemCount();
	for ( int i = 0; i < nAccountCount; i++ )
	{
		if ( m_accountList->IsSelected(i) )
		{
			wxString strID = m_accountList->GetSubItemText(i, 0);
			wxString strName = m_accountList->GetSubItemText(i, 1);
			if ( !IsExistInCurList(strName))
			{
				int nCount = m_groupInfoList->GetItemCount();
				m_groupInfoList->InsertItem( nCount, "");
				m_groupInfoList->SetItem( nCount, 0, strID);
				m_groupInfoList->SetItem( nCount, 1, strName);

				m_accountList->DeleteItem(i);
				i--;
				nAccountCount--;
			}
		}
	}

	wxString strNumber = wxString::Format("%d", m_accountList->GetItemCount());
	m_pAccountNumOutOfGroup->SetLabel(strNumber);

	strNumber = wxString::Format("%d", m_groupInfoList->GetItemCount());
	m_pAccountNumInGroup->SetLabel(strNumber);

	RefreshCurGroupData();
}

void CUserGroupSetPanel::OnDelAccount( wxCommandEvent& event )
{
	if ( m_userGroupList->GetFirstSelected() < 0)
	{
		return;
	}

	int nCount = m_groupInfoList->GetItemCount();
	for ( int i = 0; i < nCount; i++ )
	{
		if ( m_groupInfoList->IsSelected(i))
		{
			wxString strID = m_groupInfoList->GetSubItemText(i, 0);
			wxString strName = m_groupInfoList->GetSubItemText(i, 1);

			int nAccountCount = m_accountList->GetItemCount();
			m_accountList->InsertItem( nAccountCount, "");
			m_accountList->SetItem( nAccountCount, 0, strID);
			m_accountList->SetItem( nAccountCount, 1, strName);

			m_groupInfoList->DeleteItem(i);
			nCount--;
			i--;
		}
	}

	wxString strNumber = wxString::Format("%d", m_accountList->GetItemCount());
	m_pAccountNumOutOfGroup->SetLabel(strNumber);

	strNumber = wxString::Format("%d", m_groupInfoList->GetItemCount());
	m_pAccountNumInGroup->SetLabel(strNumber);

	RefreshCurGroupData();
}

void CUserGroupSetPanel::OnDelAllAccount( wxCommandEvent& event )
{
	if ( m_userGroupList->GetFirstSelected() < 0)
	{
		return;
	}

	m_groupInfoList->DeleteAllItems();
	m_pAccountNumInGroup->SetLabel("0");

	m_accountList->DeleteAllItems();
	int nAccountCount = MA_CORE()->GetTradeAccountCount();
	for ( int i = 0; i < nAccountCount; i++ )
	{
		const CTPTradeAccount* pTradeAccount = MA_CORE()->GetTradeAccount(i);
		if ( NULL == pTradeAccount )
		{
			continue;
		}

		int nCount = m_accountList->GetItemCount();
		m_accountList->InsertItem(nCount, "");
		m_accountList->SetItem(nCount, 0, 
			GlobalFunc::ConvertToString(pTradeAccount->t.nID));
		m_accountList->SetItem(nCount, 1, pTradeAccount->t.szTradeAccount);
	}

	wxString strNumber = wxString::Format("%d", m_accountList->GetItemCount());
	m_pAccountNumOutOfGroup->SetLabel(strNumber);

	RefreshCurGroupData();
}

bool CUserGroupSetPanel::IsExistInCurList( const wxString& name)
{
	int nCount = m_groupInfoList->GetItemCount();
	for ( int i = 0; i < nCount; i++ )
	{
		wxString strName = m_groupInfoList->GetSubItemText(i, 1);
		if ( strName == name )
		{
			return true;
		}
	}

	return false;
}

bool CUserGroupSetPanel::IsGroupExist(const wxString& group)
{
	int nCount = m_userGroupList->GetItemCount();
	for ( int i = 0; i < nCount; i++ )
	{
		if ( !m_userGroupList->IsSelected(i))
		{
			wxString strGroup = m_userGroupList->GetSubItemText(i, 0);
			if ( group == strGroup)
			{
				return true;
			}
		}
	}

	return false;
}

void CUserGroupSetPanel::RefreshCurGroupData()
{
	int nSelItemCount = m_userGroupList->GetSelectedItemCount();
	if ( nSelItemCount == 0 )
	{
		return;
	}

	int nCount = m_userGroupList->GetItemCount();
	for ( int i = 0; i < nCount; i++ )
	{
		if ( m_userGroupList->IsSelected(i))
		{
			int nGroupID = m_userGroupList->GetItemData(i);
			for ( UINT j = 0; j < m_vAccountGroup.size(); j++ )
			{
				if ( nGroupID == m_vAccountGroup[j].GetGroupID() )
				{
					m_vAccountGroup[j].ClearAccounts();
					int nAccountCount = m_groupInfoList->GetItemCount();
					for ( int k = 0; k < nAccountCount; k++ )
					{
						wxString strID = m_groupInfoList->GetSubItemText(k, 0);
						int nID = atoi(strID.c_str());
						wxString strName = m_groupInfoList->GetSubItemText(k, 1);
						m_vAccountGroup[j].AddAccount(nID, strName.c_str());
					}

					break;
				}
			}

			break;
		}
	}
}

void CUserGroupSetPanel::OnSelChanged( wxCommandEvent& event )
{
	CfgMgr* pCfgMgr = CFG_MGR_DEFAULT();
	if ( NULL == pCfgMgr )
	{
		return;
	}

	wxString str = m_pCbxShortKey->GetValue();
	if ( !pCfgMgr->IsShortKeyUsable(str.c_str()))
	{
		wxMessageBox(LOADSTRING(DUPLICATE_SHORTCUTKEY_PLEASE_CHOOSE_AGAIN),LOADSTRING(OIP_ERROR),wxOK|wxICON_QUESTION );
		return;
	}

	int ncount = m_userGroupList->GetSelectedItemCount();
	if( ncount ==0 )
		return;

	for( int i=0; i< m_userGroupList->GetItemCount(); ++i )
	{
		if ( m_userGroupList->IsSelectedItem( i ) )
		{
			wxString strOldKey = m_userGroupList->GetSubItemText(i, 1);
			m_userGroupList->SetItem( i, 1, str);

			int nGroupID = m_userGroupList->GetItemData(i);
			for ( UINT j = 0; j < m_vAccountGroup.size(); j++ )
			{
				if ( nGroupID == m_vAccountGroup[j].GetGroupID())
				{
					m_vAccountGroup[i].SetShortKey(str.c_str());
					break;
				}
			}

			if ( !strOldKey.IsEmpty())
			{
				pCfgMgr->AddUsableShortCutKey(strOldKey.c_str());
			}
			pCfgMgr->DelUsableShortCutKey(str.c_str());
		}
	}
}

