#pragma once
#include <map>
#include <vector>
#include <string>
#include "MultiAccountStruct.h"

// CParameterTree

class CParameterTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CParameterTree)

public:
	CParameterTree();
	virtual ~CParameterTree();

	void SetPermissionModule( std::map<int, PermissionModule> mapModuleData);
	void ShowPermissionModule(PermissionType eType);
	void SetPermissionData(std::map<int, PermissionData> mapData);
	void GetPermissionData(std::vector<PermissionData>& vData);
	BOOL SetItemState( HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch=TRUE);
	void ClearItems();
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
	std::map<int, PermissionModule> m_mapModuleData;
};


