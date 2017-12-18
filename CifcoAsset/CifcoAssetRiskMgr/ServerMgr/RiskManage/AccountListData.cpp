#include "StdAfx.h"
#include "AccountListData.h"

CAccountListData::CAccountListData(void)
: m_nKey(0)
{
	m_vCol.clear();
}

CAccountListData::~CAccountListData(void)
{
}

CString CAccountListData::GetValue( int nCol ) const
{
	if ( nCol >= 0 && nCol < static_cast<int>(m_vCol.size()))
	{
		return m_vCol[nCol];
	}
	else
	{
		return _T("");
	}
}