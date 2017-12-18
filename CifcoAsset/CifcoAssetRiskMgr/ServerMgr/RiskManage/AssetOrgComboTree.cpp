#include "StdAfx.h"
#include "AssetOrgComboTree.h"
#include "ConfigMgr.h"

CAssetOrgComboTree::CAssetOrgComboTree(void)
{
}

CAssetOrgComboTree::~CAssetOrgComboTree(void)
{
}

void CAssetOrgComboTree::SetAssetMgmtOrg( std::map<int, std::map<int, AssetMgmtOrganization>> mapAssetMgmtOrgTree )
{
	m_mapAssetMgmtOrgTree = mapAssetMgmtOrgTree;
	if ( m_mapAssetMgmtOrgTree.empty() )
	{
		return;
	}

	int nAssetMgmtOrgID = ConfigManager::GetInstance()->GetAssetMgmtOrgID();
	std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it_owner = 
		m_mapAssetMgmtOrgTree.begin();
	for ( ; it_owner != m_mapAssetMgmtOrgTree.end(); it_owner++ )
	{
		std::map<int, AssetMgmtOrganization> tempOrgMap = it_owner->second;
		std::map<int, AssetMgmtOrganization>::iterator it_org = tempOrgMap.find(nAssetMgmtOrgID);
		if ( it_org != tempOrgMap.end() )
		{
			HTREEITEM hRoot = NULL;
			hRoot = m_Tree.InsertItem(it_org->second.szName);

			m_Tree.SetItemData(hRoot, it_org->second.nAssetMgmtOrgID);
			AddOrgItems(nAssetMgmtOrgID, hRoot);

			//默认选中根节点
			m_Tree.SelectItem(hRoot);
			OnSelection();

			return;
		}
	}
}

void CAssetOrgComboTree::AddOrgItems( int nUpperLevelOrgID, HTREEITEM hParent )
{
	//显示资管区域
	std::map<int, std::map<int, AssetMgmtOrganization>>::iterator it_owner = 
		m_mapAssetMgmtOrgTree.find(nUpperLevelOrgID);
	if ( it_owner != m_mapAssetMgmtOrgTree.end() )
	{
		std::map<int, AssetMgmtOrganization> tempOrgMap = it_owner->second;
		if ( tempOrgMap.empty())
		{
			return;
		}

		std::map<int, AssetMgmtOrganization>::iterator it_org = tempOrgMap.begin();
		for ( ; it_org != tempOrgMap.end(); it_org++ )
		{
			HTREEITEM hItem = m_Tree.InsertItem(it_org->second.szName, hParent);
			m_Tree.SetItemData(hItem, it_org->second.nAssetMgmtOrgID);
			AddOrgItems(it_org->second.nAssetMgmtOrgID, hItem);
		}
	}
}

HTREEITEM CAssetOrgComboTree::GetCurSelItem()
{
	return m_Tree.GetSelectedItem();
}

DWORD CAssetOrgComboTree::GetItemData( HTREEITEM hItem )
{
	return m_Tree.GetItemData(hItem);
}

DWORD CAssetOrgComboTree::GetSelectItemData()
{
	HTREEITEM hItem = m_Tree.GetSelectedItem();
	if ( NULL == hItem )
	{
		return -1;
	}
	else
	{
		return m_Tree.GetItemData(hItem);
	}
}

void CAssetOrgComboTree::SelectItem( DWORD dwItemData )
{
	HTREEITEM hItem = m_Tree.FindChildItemData(dwItemData);
	m_Tree.SelectItem(hItem);
	OnSelection();
}
