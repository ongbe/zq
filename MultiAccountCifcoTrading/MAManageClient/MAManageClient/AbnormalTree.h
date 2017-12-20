#pragma once
#include <map>
#include <vector>
#include <set>
#include <string>
#include "MultiAccountStruct.h"

// CParameterTree

class CAbnormalTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CAbnormalTree)

public:
	struct Node
	{
		int nAccountID;
		int nAccountType;
	};

	CAbnormalTree();
	virtual ~CAbnormalTree();

	void SetMessageType(MessageType eType);
	void SetSelectChildOnly(bool bFlag);
	void DrawData();
	void SetAccountData(int nRootAccountID, 
		std::map<int, AccountInfoEx> mapAccount,
		std::map<int, std::map<int, AccountInfoEx>> mapAccountTree);

	BOOL SetItemCheck( HTREEITEM hItem, BOOL bCheck );
	void ClearItems();

	void SelectAll();
	void UnSelectAll();

	void SetInitSelectAccount(std::set<int> setSelectAccount);
	void RefreshCheckStatus();

protected:
    afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnStateIconClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	DECLARE_MESSAGE_MAP()

private:
	void TravelChild(HTREEITEM hItem, BOOL bCheck);

	void AddAccountItems( int nOwnerID, HTREEITEM hParent, int nViewPermission2 );
	void PostCheckStatusChangeMsg( HTREEITEM hItem, BOOL bCheck );

	void RefreshItemCheckStatus( HTREEITEM hItem);
	void SetCheckIfNeed( HTREEITEM hItem );

private:
	UINT m_uFlags;
	MessageType m_eType;
	bool m_bSelectChildOnly;
	int m_nRootAccountID;
	std::map<int, AccountInfoEx> m_mapAccount;
	std::map<int, std::map<int, AccountInfoEx>> m_mapAccountTree;
	std::set<int> m_setInitSelectAccount;

	std::vector<Node*> m_vNodes;
};


