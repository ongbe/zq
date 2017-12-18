#pragma once

#include "ComboTree.h"
#include "RiskManageStruct.h"

class CAssetOrgComboTree : public ComboTree
{
public:
	CAssetOrgComboTree(void);
	~CAssetOrgComboTree(void);

	void SetAssetMgmtOrg(std::map<int, std::map<int, AssetMgmtOrganization>> mapAssetMgmtOrgTree);

	HTREEITEM GetCurSelItem();
	DWORD GetItemData(HTREEITEM hItem);
	DWORD GetSelectItemData();
	void SelectItem(DWORD dwItemData);

private:
	void AddOrgItems( int nUpperLevelOrgID, HTREEITEM hParent );

private:
	std::map<int, std::map<int, AssetMgmtOrganization>> m_mapAssetMgmtOrgTree;
};
