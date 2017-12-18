#include "StdAfx.h"

#include "ISvr/PlatformSvrMgr.h"
#include "ListCfg.h"

extern PlatformSvrMgr* g_pPlatformMgr;

CListCfg::CListCfg(void)
{
	ZeroMemory(&m_ListCfg, sizeof(LIST_TABLE_CFG));
	m_vAllCol.clear();
	m_ColVector.clear();
	m_VisibleColVector.clear();
}

CListCfg::~CListCfg(void)
{
	ClearColumnCfg();
}

void CListCfg::AddColCfgInfo(LPLIST_COLUMN_CFG pColCfg)
{
	if(pColCfg)
	{
		m_vAllCol.push_back( pColCfg );
		//判断字段在该平台有效性，保存有效的字段
		if ( DEFAULT_SVR()&&DEFAULT_SVR()->IsFieldValid( m_ListCfg.Id, pColCfg->id ) )
		{
			m_ColVector.push_back(pColCfg);
			if(pColCfg->Visibility)
				m_VisibleColVector.push_back(pColCfg);
		}
	}
}

void CListCfg::AddColCfgInfo( int id, int Width, 
							 unsigned long TextColor, int Visibility)
{
	LPLIST_COLUMN_CFG pColumnCfg = new LIST_COLUMN_CFG;
	pColumnCfg->id = id;
	pColumnCfg->Width = Width;
	pColumnCfg->TextColor = TextColor;
	pColumnCfg->Visibility = Visibility;

	AddColCfgInfo( pColumnCfg );
}



void CListCfg::ClearColumnCfg()
{
	std::vector<LPLIST_COLUMN_CFG>::iterator iter;
	for(iter= m_vAllCol.begin(); iter != m_vAllCol.end(); ++iter)
	{
		LPLIST_COLUMN_CFG p = *iter;
		SAFE_DELETE(p);
	}

	m_vAllCol.clear();
	m_ColVector.clear();
	m_VisibleColVector.clear();
}

LPLIST_COLUMN_CFG CListCfg::GetListColCfg( int index )
{
	if(index < (int)m_ColVector.size())
		return m_ColVector[index];	
	return NULL;
}

int CListCfg::GetListColCount()
{
	return m_ColVector.size();
}
LPLIST_COLUMN_CFG CListCfg::GetALLListColCfg( int index )
{
	if(index < (int)m_vAllCol.size())
		return m_vAllCol[index];	
	return NULL;
}

int CListCfg::GetALLListColCount()
{
	return m_vAllCol.size();
}

LPLIST_COLUMN_CFG CListCfg::GetListVisibleColCfg( int index )
{
	if(index < (int)m_VisibleColVector.size())
		return m_VisibleColVector[index];	
	return NULL;
}

std::set<int> CListCfg::GetValidListColId()
{
	std::set<int> allColSet;
	for ( size_t i = 0; i < m_ColVector.size(); i++ )
	{
		if ( NULL != m_ColVector[i])
		{
			allColSet.insert(m_ColVector[i]->id);
		}
	}

	return allColSet;
}

bool CListCfg::CompareValidColList( CListCfg* pListCfg )
{
	if ( NULL == pListCfg )
	{
		return false;
	}

	std::set<int> listCfgSet1 = GetValidListColId();
	std::set<int> listCfgSet2 = pListCfg->GetValidListColId();
	if ( listCfgSet1.size() != listCfgSet2.size() )
	{
		return false;
	}

	std::set<int>::iterator it1 = listCfgSet1.begin();
	std::set<int>::iterator it2 = listCfgSet2.begin();
	for ( ; it1 != listCfgSet1.end() && it2 != listCfgSet2.end(); it1++, it2++)
	{
		if ( (*it1) != (*it2) )
		{
			return false;
		}
	}

	return true;
}

