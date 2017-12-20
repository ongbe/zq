// AbnormalTree.cpp : implementation file
//

#include "stdafx.h"
#include "AbnormalTree.h"


// CParameterTree

IMPLEMENT_DYNAMIC(CAbnormalTree, CTreeCtrl)

CAbnormalTree::CAbnormalTree()
{
	m_uFlags=0;
	m_eType = MESSAGE_COMMON;
	m_bSelectChildOnly = false;
	m_mapAccount.clear();
	m_mapAccountTree.clear();
	m_nRootAccountID = -1;
	m_vNodes.clear();
	m_setInitSelectAccount.clear();
}

CAbnormalTree::~CAbnormalTree()
{
	for ( UINT i = 0; i < m_vNodes.size(); i++ )
	{
		Node* pNode = m_vNodes[i];
		if ( NULL != pNode )
		{
			delete pNode;
			pNode = NULL;
		}
	}

	m_vNodes.clear();
}


BEGIN_MESSAGE_MAP(CAbnormalTree, CTreeCtrl)
    ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CAbnormalTree::OnStateIconClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(m_uFlags&TVHT_ONITEMSTATEICON) *pResult=1;
	else *pResult = 0;
}

void CAbnormalTree::OnLButtonDown( UINT nFlags, CPoint point )
{
	HTREEITEM hItem = HitTest(point, &m_uFlags);
	if ( (m_uFlags&TVHT_ONITEMSTATEICON ))
	{
		BOOL bCheck = GetCheck( hItem );
		bCheck = !bCheck;
		SetItemCheck( hItem, bCheck );
	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CAbnormalTree::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	//处理空格键
	//if(nChar==0x20)
	//{
	//	HTREEITEM hItem = GetSelectedItem();
	//	BOOL bCheck = CTreeCtrl::GetCheck( hItem );
	//	SetItemCheck(hItem, !bCheck);
	//}
	//else 
		CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAbnormalTree::OnKeydown( NMHDR* pNMHDR, LRESULT* pResult )
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

	*pResult = 0;
}

BOOL CAbnormalTree::SetItemCheck( HTREEITEM hItem, BOOL bCheck )
{
	BOOL bReturn = CTreeCtrl::SetCheck( hItem, bCheck );
	PostCheckStatusChangeMsg(hItem, bCheck);

	TravelChild(hItem, bCheck);

	return bReturn;
}

void CAbnormalTree::TravelChild( HTREEITEM hItem, BOOL bCheck )
{
	HTREEITEM hChildItem, hBrotherItem; // 子 兄

	//查找子节点，没有就结束
	hChildItem = GetChildItem(hItem);
	if( hChildItem != NULL )
	{
		//this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hChildItem);

		if ( !m_bSelectChildOnly )
		{
			//设置子节点的状态与当前节点的状态一致
			CTreeCtrl::SetCheck( hChildItem, bCheck );
			PostCheckStatusChangeMsg(hChildItem, bCheck);
			//再递归处理子节点的子节点和兄弟节点
			TravelChild(hChildItem, bCheck);

			//处理子节点的兄弟节点和其子节点
			hBrotherItem = GetNextSiblingItem(hChildItem);
			while (hBrotherItem)
			{
				//this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hBrotherItem);
				//设置子节点的兄弟节点状态与当前节点的状态一致
				CTreeCtrl::SetCheck( hBrotherItem, bCheck );
				PostCheckStatusChangeMsg(hBrotherItem, bCheck);

				//再递归处理子节点的兄弟节点的子节点和兄弟节点
				TravelChild(hBrotherItem, bCheck);
				hBrotherItem = GetNextSiblingItem(hBrotherItem);
			}
		}
		else
		{
			Node* pNode = (Node*)GetItemData( hChildItem );
			if ( NULL == pNode )
			{
				return;
			}

			if ( pNode->nAccountType == ACCOUNT_TYPE_LOGON)
			{
				CTreeCtrl::SetCheck(hChildItem, bCheck);
				PostCheckStatusChangeMsg(hChildItem, bCheck);
			}

			//处理子节点的兄弟节点和其子节点
			hBrotherItem = GetNextSiblingItem(hChildItem);
			while (hBrotherItem)
			{
				//this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hBrotherItem);
				//设置子节点的兄弟节点状态与当前节点的状态一致
				Node* pNode = (Node*)GetItemData( hBrotherItem );
				if ( NULL == pNode )
				{
					continue;
				}

				if ( pNode->nAccountType == ACCOUNT_TYPE_LOGON)
				{
					CTreeCtrl::SetCheck(hBrotherItem, bCheck);
					PostCheckStatusChangeMsg(hBrotherItem, bCheck);
				}

				//再递归处理子节点的兄弟节点的子节点和兄弟节点
				hBrotherItem = GetNextSiblingItem(hBrotherItem);
			}
		}
	}
}

void CAbnormalTree::ClearItems()
{
	DeleteAllItems();

	m_uFlags=0;

	for ( UINT i = 0; i < m_vNodes.size(); i++ )
	{
		Node* pNode = m_vNodes[i];
		if ( NULL != pNode )
		{
			delete pNode;
			pNode = NULL;
		}
	}

	m_vNodes.clear();
	m_setInitSelectAccount.clear();
}

void CAbnormalTree::SetMessageType( MessageType eType )
{
	m_eType = eType;
}

void CAbnormalTree::SetAccountData( int nRootAccountID, 
								   std::map<int, AccountInfoEx> mapAccount,
								   std::map<int, std::map<int, AccountInfoEx>> mapAccountTree )
{
	m_nRootAccountID = nRootAccountID;
	m_mapAccount = mapAccount;
	m_mapAccountTree = mapAccountTree;
}

void CAbnormalTree::SetSelectChildOnly( bool bFlag )
{
	m_bSelectChildOnly = bFlag;
}

void CAbnormalTree::DrawData()
{
	ClearItems();

	std::map<int, AccountInfoEx>::iterator it_account = m_mapAccount.find(m_nRootAccountID);
	if ( it_account == m_mapAccount.end() )
	{
		return;
	}
	else
	{
		CString csText;
		csText.Format(_T("管理账户_%s"), it_account->second.accountInfo.szAccount);
		HTREEITEM hRoot = InsertItem(csText, TVI_ROOT );

		Node* pNode = new Node();
		pNode->nAccountID = it_account->second.accountInfo.nAccountID;
		pNode->nAccountType = it_account->second.accountInfo.nAccountType;
		m_vNodes.push_back(pNode);
		SetItemData(hRoot, (DWORD)pNode);
		AddAccountItems(m_nRootAccountID, hRoot, it_account->second.accountInfo.nViewPermission2);
	}
}

void CAbnormalTree::AddAccountItems( int nOwnerID, HTREEITEM hParent, int nViewPermission2 )
{
	std::map<int, std::map<int, AccountInfoEx>>::iterator it_owner = 
		m_mapAccountTree.find(nOwnerID);
	if ( it_owner == m_mapAccountTree.end() )
	{
		return;
	}
	else
	{
		std::map<int, AccountInfoEx> tempAccountMap = it_owner->second;
		if ( tempAccountMap.empty())
		{
			return;
		}

		std::map<int, AccountInfoEx>::iterator it_account = tempAccountMap.begin();
		std::vector<AccountInfoEx> vForeAccounts;
		for ( ; it_account != tempAccountMap.end(); it_account++ )
		{
			if ( it_account->second.accountInfo.nAccountType == ACCOUNT_TYPE_MANAGE )
			{
				bool bAdd = false;
				if ( nViewPermission2 == 1)
				{
					bAdd = true;
				}
				else
				{
					if ( it_account->second.accountInfo.nViewPermission1 == 1)
					{
						bAdd = true;
					}
				}

				if ( bAdd )
				{
					Node* pNode = new Node();
					pNode->nAccountID = it_account->second.accountInfo.nAccountID;
					pNode->nAccountType = it_account->second.accountInfo.nAccountType;
					m_vNodes.push_back(pNode);

					CString csText;
					csText.Format(_T("管理账户_%s"), it_account->second.accountInfo.szAccount);
					HTREEITEM hItem = InsertItem(csText, hParent, TVI_LAST );
					SetItemData(hItem, (DWORD)pNode);

					AddAccountItems(it_account->second.accountInfo.nAccountID, hItem, nViewPermission2);
				}
			}
			else
			{
				if ( m_eType == MESSAGE_PERSONAL )
				{
					vForeAccounts.push_back(it_account->second);
				}
			}
		}

		//显示前台用户
		for ( UINT i = 0; i < vForeAccounts.size(); i++ )
		{
			AccountInfoEx info = vForeAccounts[i];
			Node* pNode = new Node();
			pNode->nAccountID = info.accountInfo.nAccountID;
			pNode->nAccountType = info.accountInfo.nAccountType;
			m_vNodes.push_back(pNode);

			CString csText;
			csText.Format(_T("前台账户_%s"), info.accountInfo.szAccount);
			HTREEITEM hItem = InsertItem(csText, hParent, TVI_LAST );
			SetItemData(hItem, (DWORD)pNode);
		}
	}

}

void CAbnormalTree::PostCheckStatusChangeMsg( HTREEITEM hItem, BOOL bCheck )
{
	if ( NULL == hItem )
	{
		return;
	}

	Node* pNode = (Node*)GetItemData(hItem);
	if ( NULL == pNode )
	{
		return;
	}

	if ( m_eType == MESSAGE_COMMON )
	{
		if ( pNode->nAccountType == ACCOUNT_TYPE_MANAGE )
		{
			this->GetParent()->PostMessage(MSG_TARGETACCOUNT_CHANGE, bCheck, pNode->nAccountID);
		}
	}
	else
	{
		if ( pNode->nAccountType == ACCOUNT_TYPE_LOGON )
		{
			this->GetParent()->PostMessage(MSG_TARGETACCOUNT_CHANGE, bCheck, pNode->nAccountID);
		}
	}
}

void CAbnormalTree::SelectAll()
{
	HTREEITEM hRoot = GetRootItem();
	if ( NULL == hRoot )
	{
		return;
	}

	bool bBack = m_bSelectChildOnly;
	m_bSelectChildOnly = false;
	SetItemCheck( hRoot, TRUE );
	m_bSelectChildOnly = bBack;
}

void CAbnormalTree::UnSelectAll()
{
	HTREEITEM hRoot = GetRootItem();
	if ( NULL == hRoot )
	{
		return;
	}

	bool bBack = m_bSelectChildOnly;
	m_bSelectChildOnly = false;
	SetItemCheck( hRoot, FALSE );
	m_bSelectChildOnly = bBack;
}

void CAbnormalTree::SetInitSelectAccount( std::set<int> setSelectAccount )
{
	m_setInitSelectAccount = setSelectAccount;
}

void CAbnormalTree::RefreshCheckStatus()
{
	HTREEITEM hRoot = GetRootItem();
	SetCheckIfNeed(hRoot);

	RefreshItemCheckStatus(hRoot);
}

void CAbnormalTree::RefreshItemCheckStatus( HTREEITEM hItem )
{
	//自身节点
	if ( NULL == hItem )
	{
		return;
	}

	HTREEITEM hChildItem = GetChildItem(hItem);
	if ( NULL != hChildItem )
	{
		//子节点
		SetCheckIfNeed(hChildItem);
		RefreshItemCheckStatus(hChildItem);

		//处理子节点的兄弟节点和其子节点
		HTREEITEM hBrotherItem = GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			SetCheckIfNeed(hBrotherItem);
			RefreshItemCheckStatus(hBrotherItem);

			hBrotherItem = GetNextSiblingItem(hBrotherItem);
		}
	}
}

void CAbnormalTree::SetCheckIfNeed( HTREEITEM hItem )
{
	if ( hItem == NULL )
	{
		return;
	}

	Node* pNode = (Node*)GetItemData(hItem);
	if ( NULL != pNode )
	{
		std::set<int>::iterator it = m_setInitSelectAccount.find(pNode->nAccountID);
		if ( it != m_setInitSelectAccount.end() )
		{
			CTreeCtrl::SetCheck(hItem, TRUE);
			PostCheckStatusChangeMsg(hItem, TRUE);
		}
		else
		{
			CTreeCtrl::SetCheck(hItem, FALSE);
		}
	}
}
