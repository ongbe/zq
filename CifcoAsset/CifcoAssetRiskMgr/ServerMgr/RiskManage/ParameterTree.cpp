// ParameterTree.cpp : implementation file
//

#include "stdafx.h"
#include "ParameterTree.h"


// CParameterTree

IMPLEMENT_DYNAMIC(CParameterTree, CTreeCtrl)

CParameterTree::CParameterTree()
{
	m_uFlags=0;
	m_eType = PERMISSION_MANAGE;
	m_mapNode.clear();
	m_mapAction.clear();
}

CParameterTree::~CParameterTree()
{
}


BEGIN_MESSAGE_MAP(CParameterTree, CTreeCtrl)
    ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CParameterTree::OnStateIconClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(m_uFlags&TVHT_ONITEMSTATEICON) *pResult=1;
	else *pResult = 0;
}

void CParameterTree::OnLButtonDown( UINT nFlags, CPoint point )
{
	HTREEITEM hItem =HitTest(point, &m_uFlags);
	if ( (m_uFlags&TVHT_ONITEMSTATEICON ))
	{
		//nState: 0->无选择钮 1->没有选择 2->部分选择 3->全部选择
		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		nState=(nState==3)?1:3;
		(this->GetParent())->PostMessage(WM_USER + 12,0,(LPARAM)hItem);
		SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );

	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CParameterTree::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	//处理空格键
	if(nChar==0x20)
	{
		HTREEITEM hItem =GetSelectedItem();
		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		if(nState!=0)
		{
			nState=(nState==3)?1:3;
			SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		}
	}
	else CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CParameterTree::OnKeydown( NMHDR* pNMHDR, LRESULT* pResult )
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

	*pResult = 0;
}

BOOL CParameterTree::SetItemState( HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch/*=TRUE*/ )
{
	BOOL bReturn=CTreeCtrl::SetItemState( hItem, nState, nStateMask );

	UINT iState = nState >> 12;
	if(iState!=0)
	{
		if(bSearch) TravelChild(hItem, iState);
		TravelSiblingAndParent(hItem,iState);
	}
	return bReturn;
}
void CParameterTree::TravelChild( HTREEITEM hItem,int nState )
{
	HTREEITEM hChildItem,hBrotherItem; // 子 兄

	//查找子节点，没有就结束
	hChildItem=GetChildItem(hItem);
	if(hChildItem!=NULL)
	{
		//this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hChildItem);

		//设置子节点的状态与当前节点的状态一致
		CTreeCtrl::SetItemState( hChildItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		//再递归处理子节点的子节点和兄弟节点
		TravelChild(hChildItem, nState);

		//处理子节点的兄弟节点和其子节点
		hBrotherItem=GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			//this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hBrotherItem);
			//设置子节点的兄弟节点状态与当前节点的状态一致
			int nState1 = GetItemState( hBrotherItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				CTreeCtrl::SetItemState( hBrotherItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//再递归处理子节点的兄弟节点的子节点和兄弟节点
			TravelChild(hBrotherItem, nState);
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
		}
	}
}


void CParameterTree::TravelSiblingAndParent( HTREEITEM hItem, int nState )
{
	HTREEITEM hNextSiblingItem,hPrevSiblingItem,hParentItem;  //下一个姐妹兄弟 上一个姐妹兄弟  父亲节点

	//查找父节点，没有就结束
	hParentItem=GetParentItem(hItem);
	if(hParentItem!=NULL)
	{
		int nState1=nState;//设初始值，防止没有兄弟节点时出错

		//查找当前节点下面的兄弟节点的状态
		hNextSiblingItem=GetNextSiblingItem(hItem);
		while(hNextSiblingItem!=NULL)
		{
			nState1 = GetItemState( hNextSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=nState && nState1!=0) break;
			else hNextSiblingItem=GetNextSiblingItem(hNextSiblingItem);
		}

		if(nState1==nState)
		{
			//查找当前节点上面的兄弟节点的状态
			hPrevSiblingItem=GetPrevSiblingItem(hItem);
			while(hPrevSiblingItem!=NULL)
			{
				nState1 = GetItemState( hPrevSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=nState && nState1!=0) break;
				else hPrevSiblingItem=GetPrevSiblingItem(hPrevSiblingItem);
			}
		}

		if(nState1==nState || nState1==0)
		{
			nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				//如果状态一致，则父节点的状态与当前节点的状态一致
				CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//再递归处理父节点的兄弟节点和其父节点
			TravelSiblingAndParent(hParentItem,nState);
		}
		else
		{
			//状态不一致，则当前节点的父节点、父节点的父节点……状态均为第三态
			hParentItem=GetParentItem(hItem);
			while(hParentItem!=NULL)
			{
				nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=0)
				{
					CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
				}
				hParentItem=GetParentItem(hParentItem);
			}
		}
	}	
}

void CParameterTree::ClearItems()
{
	DeleteAllItems();

	m_uFlags=0;
	m_eType = PERMISSION_MANAGE;
	m_mapNode.clear();
	m_mapAction.clear();
}

void CParameterTree::SetPermissionModule( const std::map<int, PermissionNode>& mapNode,
										 const std::map<int, PermissionAction>& mapAction )
{
	m_mapNode = mapNode;
	m_mapAction = mapAction;
}

void CParameterTree::ShowPermissionModule(PermissionType eType)
{
	DeleteAllItems();

	InsertItem( _T( "全选" ), TVI_ROOT );
	m_eType = eType;
	std::map<int, PermissionNode>::iterator it = m_mapNode.begin();
	for( ; it != m_mapNode.end(); it++ )
	{
		PermissionNode node = it->second;
		if ( node.nNodeType != m_eType )
		{
			continue;
		}

		HTREEITEM hRootItem = GetRootItem();
		HTREEITEM hModuleItem = InsertItem( node.szName, hRootItem, TVI_LAST);
		SetItemData(hModuleItem, node.nNodeID);

		HTREEITEM hPermissionItem = NULL;
		std::map<int, PermissionAction>::iterator it_action = m_mapAction.begin();
		for ( ; it_action != m_mapAction.end(); ++it_action )
		{
			if ( node.nNodeID == it_action->second.nNodeID )
			{
				HTREEITEM hPermissionItem = InsertItem( it_action->second.szName, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, it_action->second.nActionID);
			}
		}
	}
}

void CParameterTree::SetPermissionData( std::map<int, PermissionValue> mapData )
{
	if ( mapData.empty())
	{
		return;
	}

	HTREEITEM hRootItem = GetRootItem();
	HTREEITEM hChildItem = GetChildItem( hRootItem );
	while ( NULL != hChildItem )
	{
		HTREEITEM hSubItem = GetChildItem( hChildItem );
		while( NULL != hSubItem )
		{
			int nActionID = GetItemData(hSubItem);
			std::map<int, PermissionValue>::iterator it = mapData.find(nActionID);
			if ( it != mapData.end() && (it->second.nValue == 1))
			{
				int nState = 3;
				SetItemState(hSubItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
			}

			hSubItem = GetNextSiblingItem(hSubItem);
		}

		hChildItem = GetNextSiblingItem(hChildItem);
	}
}

void CParameterTree::GetPermissionData( std::vector<PermissionValue>& vData )
{
	vData.clear();

	HTREEITEM hRootItem = GetRootItem();
	HTREEITEM hChildItem = GetChildItem( hRootItem );
	while ( NULL != hChildItem )
	{
		//获取节点选中状态
		int nState = GetItemState( hChildItem, TVIS_STATEIMAGEMASK ) >> 12;
		if ( 2 == nState || 3 == nState )
		{
			HTREEITEM hSubItem = GetChildItem( hChildItem );
			while( NULL != hSubItem )
			{
				int nActionID = GetItemData(hSubItem);
				int nSubState = GetItemState( hSubItem, TVIS_STATEIMAGEMASK ) >> 12;
				if ( 2 == nSubState || 3 == nSubState)
				{
					PermissionValue permissionValue;
					permissionValue.nActionID = nActionID;
					permissionValue.nPermissionID = 0;
					permissionValue.nValue = 1;
					vData.push_back(permissionValue);
				}

				hSubItem = GetNextSiblingItem(hSubItem);
			}
		}

		hChildItem = GetNextSiblingItem(hChildItem);
	}
}

PermissionType CParameterTree::GetCurShowType()
{
	return m_eType;
}

void CParameterTree::ResetTreeData()
{
	HTREEITEM hRootItem = GetRootItem();
	int nStatus = 1;
	SetItemState(hRootItem, INDEXTOSTATEIMAGEMASK(nStatus), TVIS_STATEIMAGEMASK);
}
