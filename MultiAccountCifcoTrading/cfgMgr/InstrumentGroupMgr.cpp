#include "StdAfx.h"

#include "Module-Misc/globalDefines.h"
#include "Module-Misc/GLobalFunc.h"
#include "InstrumentGroup.h"
#include "InstrumentGroupMgr.h"

InstrumentGroupMgr::InstrumentGroupMgr(void)
{
	m_dwCurrentGroupId = 0;
	m_nMaxGroupId = 0;
	m_InstrumentGroupVector.clear();
}

InstrumentGroupMgr::~InstrumentGroupMgr(void)
{
	ClearInstrumentGroupVector();
}

int InstrumentGroupMgr::GetCurrentId()
{
	return m_dwCurrentGroupId;
}

std::string InstrumentGroupMgr::GetCurrentGroupName()
{
	for ( int i = 0; i < (int)m_InstrumentGroupVector.size(); i++)
	{
		CInstrumentGroup* pGroup = m_InstrumentGroupVector[i];
		if ( m_dwCurrentGroupId == pGroup->GetGroupId())
		{
			return pGroup->GetGroupName();
		}
	}

	return "";
}

void InstrumentGroupMgr::SetCurrentId( int id )
{
	m_dwCurrentGroupId = id;
}

bool InstrumentGroupMgr::InitCfg()
{
	std::string strFilePath = "InstrumentGroup.xml";
	GlobalFunc::GetPath( strFilePath );
	if ( !GlobalFunc::IsFileExists(strFilePath))
	{
		return false;
	}

	TiXmlDocument* pXmlDocment = new TiXmlDocument( strFilePath.c_str());
	if( NULL == pXmlDocment)
	{
		return false;
	}

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() )
	{
		return false;
	}

	TiXmlElement *root = pXmlDocment->RootElement();
	if ( NULL == root )
	{
		return false;
	}

	if( std::string(root->Value()) != "InstrumentGroup")
	{
		return false;
	}
  
	const char *szId = root->Attribute("id");
    if(szId)
	{
		m_dwCurrentGroupId = atoi(szId);
	}

	for(TiXmlElement *GroupItem = root->FirstChildElement("Group"); 
		GroupItem; 
		GroupItem = GroupItem->NextSiblingElement("Group"))
	{
		const char *pGroupName = GroupItem->Attribute("name");
		if(!pGroupName) 
		{
			continue;
		}
	
		const char *pGroupId = GroupItem->Attribute("id");
		if(!pGroupId)
		{
			continue;
		}

		int nGroupId = atoi(pGroupId);
		if(nGroupId<0)
		{
			continue;
		}

		int nVisibility = 0;
		const char *pVisibility = GroupItem->Attribute("Visibility");
		if(pVisibility)
		{
			nVisibility = atoi(pVisibility);
		}

		bool bVisible = ( nVisibility == 1?true:false );
		int nGroupIndex = AddInstrumentGroup( pGroupName, bVisible, nGroupId);
		if( nGroupIndex != -1 )
		{
			for(TiXmlElement *InstrumentItem = GroupItem->FirstChildElement("Instrument"); 
				InstrumentItem; 
				InstrumentItem = InstrumentItem->NextSiblingElement("Instrument"))
			{	
				const char * pInstrumentId = InstrumentItem->Attribute("id");
				if(!pInstrumentId) continue;
				AddInstrument(nGroupIndex, pInstrumentId);
			}
		}

	}

	return true;
}

bool InstrumentGroupMgr::SaveCfg()
{
	TiXmlDocument* pDoc = new TiXmlDocument;
	if ( NULL == pDoc )
	{
		return false;
	}
	std::auto_ptr<TiXmlDocument> ptrDoc( pDoc );

	TiXmlElement *root = new TiXmlElement("InstrumentGroup");
	if ( NULL == root )
	{
		return false;
	}
	pDoc->LinkEndChild( root );
 
	root->SetAttribute("id",m_dwCurrentGroupId);
	std::vector<CInstrumentGroup *>::iterator iter = m_InstrumentGroupVector.begin();
	for (iter; iter != m_InstrumentGroupVector.end(); iter++)
	{
		CInstrumentGroup *p = *iter;
		TiXmlElement *GroupItem = new TiXmlElement("Group");
		GroupItem->SetAttribute("name", p->GetGroupName().c_str());
		GroupItem->SetAttribute("id", GlobalFunc::ConvertToString(p->GetGroupId()).c_str());
		GroupItem->SetAttribute("Visibility", GlobalFunc::ConvertToString(p->IsVisible()).c_str());

		root->LinkEndChild(GroupItem);

		int nInstrumentCount = p->GetGroupMemberCount();
		for (int i = 0; i < nInstrumentCount; i++)
		{
			TiXmlElement *InstrumentItem = new TiXmlElement("Instrument");
			InstrumentItem->SetAttribute("id", p->GetInstrumentId(i).c_str());

			GroupItem->LinkEndChild(InstrumentItem);
		}	
	}

	std::string strFile = "InstrumentGroup.xml";
	GlobalFunc::GetPath( strFile );
	if( !pDoc->SaveFile( strFile.c_str() ))
		return false;

	return true;
}

void InstrumentGroupMgr::ClearCfg()
{
	m_dwCurrentGroupId=0;
	m_nMaxGroupId = 0;
	ClearInstrumentGroupVector();
}

bool InstrumentGroupMgr::IsExitGroup(const std::string strGroupName)
{
	std::vector<CInstrumentGroup *>::iterator iter = m_InstrumentGroupVector.begin();
	for (iter; iter != m_InstrumentGroupVector.end(); iter++)
	{
		CInstrumentGroup *p = *iter;
		if (p->GetGroupName() == strGroupName)
			return true;
	}
	return false;
}

bool InstrumentGroupMgr::IsExitGroup(int nGroupId)
{
	std::vector<CInstrumentGroup *>::iterator iter = m_InstrumentGroupVector.begin();
	for (iter; iter != m_InstrumentGroupVector.end(); iter++)
	{
		CInstrumentGroup *p = *iter;
		if (p->GetGroupId() == nGroupId)
			return true;
	}
	return false;
}

int InstrumentGroupMgr::GetGroupIndexByGroupId( int nGroupId )
{
	int nGroupIndex = 0;
	std::vector<CInstrumentGroup *>::iterator iter = m_InstrumentGroupVector.begin();
	for (iter; iter != m_InstrumentGroupVector.end(); iter++)
	{
		CInstrumentGroup *p = *iter;
		if (p->GetGroupId() == nGroupId)
		{
			return nGroupIndex;
		}

		nGroupIndex++;
	}

	return -1;
}

int InstrumentGroupMgr::AddInstrumentGroup(std::string strName, bool bVisible, DWORD id)
{
	if(IsExitGroup(strName))
		return -1;

	if( m_nMaxGroupId < id )
	{
		m_nMaxGroupId = id;
	}

	CInstrumentGroup *pGroup = new CInstrumentGroup;
	pGroup->SetGroupName(strName.c_str());
	pGroup->SetGroupId( id );
	pGroup->SetVisible( bVisible );
	m_InstrumentGroupVector.push_back(pGroup);

	return m_InstrumentGroupVector.size() - 1;
}

int InstrumentGroupMgr::ReplaceInstrumentGroup( int nGroupIndex, 
												const char * szName, 
												bool bVisible, 
												DWORD id /* = -1  */)
{
	if ( nGroupIndex >= static_cast<int>(m_InstrumentGroupVector.size()) )
	{
		return -1;
	}

	if ( NULL == szName )
	{
		return -1;
	}

	CInstrumentGroup *pGroup = m_InstrumentGroupVector[nGroupIndex];
	pGroup->ClearInstrument();
	pGroup->SetGroupName( szName );
	if(id == -1)
	{
		m_nMaxGroupId++;
		id = m_nMaxGroupId;
	}
	pGroup->SetGroupId( id );
	pGroup->SetVisible(bVisible);

	return id;
}

bool InstrumentGroupMgr::DelInstrumentGroup(const char *name)
{
	if (name == NULL)
	{
		return false;
	}

	std::vector<CInstrumentGroup *>::iterator iter = m_InstrumentGroupVector.begin();
	for (iter; iter != m_InstrumentGroupVector.end(); iter++)
	{
		CInstrumentGroup *p = *iter;
		if (p->GetGroupName() == name)
		{
			delete p;
			p=NULL;
			m_InstrumentGroupVector.erase(iter);
			return true;
		}
	}

	return false;
}

bool InstrumentGroupMgr::DelInstrumentGroup( int nGroupId )
{
	std::vector<CInstrumentGroup *>::iterator iter = m_InstrumentGroupVector.begin();
	for (iter; iter != m_InstrumentGroupVector.end(); iter++)
	{
		CInstrumentGroup *p = *iter;
		if (p->GetGroupId() == nGroupId)
		{
			delete p;
			p=NULL;
			m_InstrumentGroupVector.erase(iter);
			return true;
		}
	}

	return false;
}

void InstrumentGroupMgr::ClearInstrumentGroupVector()
{
	std::vector<CInstrumentGroup *>::iterator iter = m_InstrumentGroupVector.begin();
	for (iter; iter != m_InstrumentGroupVector.end(); iter++)
	{
		CInstrumentGroup *p = *iter;
		SAFE_DELETE(p)
	}

	m_InstrumentGroupVector.clear();
}

LPINSTRUMENT_GROUP_CFG InstrumentGroupMgr::GetGroupInfo(int GroupIndex)
{
	if(GroupIndex>=(int)m_InstrumentGroupVector.size())
		return NULL; 
	return m_InstrumentGroupVector[GroupIndex]->GetGroupInfo();
}

std::string InstrumentGroupMgr::GetGroupName(int GroupIndex)
{
	if(GroupIndex>=(int)m_InstrumentGroupVector.size())
		return "";
	return m_InstrumentGroupVector[GroupIndex]->GetGroupName();
}

int  InstrumentGroupMgr::GetGroupMemberCount(int GroupIndex)
{
	if(GroupIndex>=(int)m_InstrumentGroupVector.size())
		return -1;
	return m_InstrumentGroupVector[GroupIndex]->GetGroupMemberCount();
}

std::string InstrumentGroupMgr::GetGroupInstrumentId(int GroupIndex, int InstrumentIndex)
{
	if(GroupIndex>=(int)m_InstrumentGroupVector.size())
		return "";
	return m_InstrumentGroupVector[GroupIndex]->GetInstrumentId(InstrumentIndex);
}

std::vector<std::string> *InstrumentGroupMgr::GetGroupMember(int GroupIndex)
{
	if(GroupIndex>=(int)m_InstrumentGroupVector.size())
		return NULL;
	return m_InstrumentGroupVector[GroupIndex]->GetGroupMember();
}

std::vector<std::string> *InstrumentGroupMgr::GetCurrentGroupMember()
{
	for ( int i = 0; i < (int)m_InstrumentGroupVector.size(); i++)
	{
		CInstrumentGroup* pGroup = m_InstrumentGroupVector[i];
		if ( m_dwCurrentGroupId == pGroup->GetGroupId())
		{
			return pGroup->GetGroupMember();
		}
	}

	return NULL;
}

void InstrumentGroupMgr::AddInstrument(int GroupIndex, const char *pInstrumentId)
{
	if(GroupIndex<(int)m_InstrumentGroupVector.size())
	{
		m_InstrumentGroupVector[GroupIndex]->AddInstrument(pInstrumentId);
	}
}

void InstrumentGroupMgr::DelInstrument(int GroupIndex)
{
	if(GroupIndex<(int)m_InstrumentGroupVector.size())
	{
		m_InstrumentGroupVector[GroupIndex]->ClearInstrument();
	}
}

void InstrumentGroupMgr::DelInstrument(int GroupIndex, const char *pInstrumentId)
{
	if(GroupIndex<(int)m_InstrumentGroupVector.size())
	{
		m_InstrumentGroupVector[GroupIndex]->DelInstrument(pInstrumentId);
	}
}

int	InstrumentGroupMgr::GetGroupCount()
{
	return m_InstrumentGroupVector.size();
}

int InstrumentGroupMgr::GetNextGroupId()
{
	return ++m_nMaxGroupId;
}
