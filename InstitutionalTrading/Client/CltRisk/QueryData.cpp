#include "StdAfx.h"
#include "QueryData.h"
CQueryData* CQueryData::m_pInstance = NULL;
int g_QueryDataCount = 0;
CQueryData::CQueryData(void)
{
}
CQueryData* CQueryData::NewInstance()
{
	g_QueryDataCount ++;
	if( m_pInstance == NULL ) 
	{
		m_pInstance = new CQueryData();
	}

	return m_pInstance;
}

void CQueryData::DestroyInstance()
{
	g_QueryDataCount --;
	if(g_QueryDataCount == 0 && m_pInstance != NULL) 
	{		
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
void  CQueryData::SetSendSeq(int nSeq)
{

	m_vecSeq.push_back(nSeq);
}
void  CQueryData::GetSendSeq(std::vector<int>&   vecSeq)
{

	vecSeq = m_vecSeq;
}
void  CQueryData::ReceiveSeq(int nSeq)
{
	std::vector<int>::iterator it= m_vecSeq.begin();
	while (it!=m_vecSeq.end())
	{
		if (*it == nSeq)
		{
			it=m_vecSeq.erase(it);
		}
		else
			++it;
	}
}
bool  CQueryData::IsEmpty()
{
	if (m_vecSeq.empty())
	{
		return true;
	}
	else
		return false;
}
void CQueryData::ClearSendSeq()
{
	m_vecSeq.clear();
}