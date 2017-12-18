#include "StdAfx.h"
#include "cfgmgr/InstrumentGroup.h"

CInstrumentGroup::CInstrumentGroup(void)
{
	ZeroMemory( &m_InstrumentGroupCfg, sizeof(m_InstrumentGroupCfg));
	m_InstrumentVector.clear();
}

CInstrumentGroup::~CInstrumentGroup(void)
{
}

bool CInstrumentGroup::IsExitGroup(const char *pInstrumentId)
{
	std::vector<std::string>::iterator iter = m_InstrumentVector.begin();
	for (iter; iter != m_InstrumentVector.end(); ++iter)
	{
		std::string strId = *iter;
		if (strId == pInstrumentId)
			return true;
	}
	return false;
}

void CInstrumentGroup::AddInstrument(const char *pInstrumentId)						//添加合约
{
	if (pInstrumentId==NULL || IsExitGroup(pInstrumentId))
		return;

	m_InstrumentVector.push_back(pInstrumentId);
}

void CInstrumentGroup::DelInstrument(const char *pInstrumentId)						//删除合约
{
	if (pInstrumentId==NULL)
		return;
	std::vector<std::string>::iterator iter = m_InstrumentVector.begin();
	for (iter; iter!=m_InstrumentVector.end();++iter)
	{
		std::string tInstrumentId = *iter;
		if (tInstrumentId == pInstrumentId)
		{
			m_InstrumentVector.erase(iter);
			return;
		}		
	}	
}

void CInstrumentGroup::ClearInstrument()
{
	ZeroMemory(&m_InstrumentGroupCfg, sizeof(m_InstrumentGroupCfg));
	m_InstrumentVector.clear();
}

int CInstrumentGroup::GetGroupMemberCount()
{
	return m_InstrumentVector.size();
}

std::string CInstrumentGroup::GetInstrumentId( int nIndex )
{
	return m_InstrumentVector[nIndex];
}

std::vector<std::string>* CInstrumentGroup::GetGroupMember()
{
	return &m_InstrumentVector;
}

LPINSTRUMENT_GROUP_CFG CInstrumentGroup::GetGroupInfo()
{
	return &m_InstrumentGroupCfg;
}

std::string CInstrumentGroup::GetGroupName()
{
	return m_InstrumentGroupCfg.GroupName;
}

int CInstrumentGroup::GetGroupId()
{
	return m_InstrumentGroupCfg.GroupId;
}

void CInstrumentGroup::SetGroupName( const char *name )
{
	strncpy(m_InstrumentGroupCfg.GroupName,name,GROUPNAME_LENGTH);
}

void CInstrumentGroup::SetGroupId( DWORD id )
{
	m_InstrumentGroupCfg.GroupId = id;
}

int CInstrumentGroup::IsVisible()
{
	return m_InstrumentGroupCfg.IsVisibility;
}

void CInstrumentGroup::SetVisible( bool bVisible )
{
	m_InstrumentGroupCfg.IsVisibility = bVisible;
}
