#pragma once
#include <map>
#include <vector>
#include <string>
#include "RiskManageStruct.h"

// CParameterTree

class CParameterTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CParameterTree)

public:
	CParameterTree();
	virtual ~CParameterTree();

	void SetPermissionModule( const std::map<int, PermissionNode>& mapNode,
		const std::map<int, PermissionAction>& mapAction);
	void ShowPermissionModule(PermissionType eType);
	void SetPermissionData(std::map<int, PermissionValue> mapData);
	void GetPermissionData(std::vector<PermissionValue>& vData);
	BOOL SetItemState( HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch=TRUE);
	void ClearItems();
	void ResetTreeData();
	PermissionType GetCurShowType();

protected:
    afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()

private:
	void TravelSiblingAndParent(HTREEITEM hItem, int nState);
	void TravelChild(HTREEITEM hItem,int nState);

private:
	UINT m_uFlags;
	PermissionType m_eType;
	std::map<int, PermissionNode> m_mapNode;
	std::map<int, PermissionAction> m_mapAction;
};


