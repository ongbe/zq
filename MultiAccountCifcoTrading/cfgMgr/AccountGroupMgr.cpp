#include "StdAfx.h"
#include "Module-Misc/GLobalFunc.h"
#include "ShortCutKeysMgr.h"
#include "AccountGroupMgr.h"

AccountGroupMgr::AccountGroupMgr(void)
: m_nMaxGroupID(0)
, m_nGroupID(0)
{
	m_vAccountGroup.clear();
}

AccountGroupMgr::~AccountGroupMgr(void)
{
}

bool AccountGroupMgr::InitCfg()
{
	std::string strFilePath = "AccountGroup.xml";
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

	if( std::string(root->Value()) != "AccountGroup")
	{
		return false;
	}

	const char *szId = root->Attribute("id");
	if(szId)
	{
		m_nGroupID = atoi(szId);
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

		const char *pShortKey = GroupItem->Attribute("shortkey");
		if(!pShortKey) 
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

		if ( IsGroupExist(nGroupId))
		{
			continue;
		}

		if ( nGroupId > m_nMaxGroupID )
		{
			m_nMaxGroupID = nGroupId;
		}

		AccountGroup accountGroup;
		accountGroup.SetGroupAttr(nGroupId, pGroupName, pShortKey);
		for(TiXmlElement *AccountItem = GroupItem->FirstChildElement("Account"); 
			AccountItem; 
			AccountItem = AccountItem->NextSiblingElement("Account"))
		{	
			const char * pAccountID = AccountItem->Attribute("id");
			if(!pAccountID) continue;
			const char* pAccountName = AccountItem->Attribute("name");
			if ( !pAccountName ) continue;

			accountGroup.AddAccount(atoi(pAccountID), pAccountName);
		}

		m_vAccountGroup.push_back(accountGroup);
	}

	return true;
}

bool AccountGroupMgr::SaveCfg()
{
	TiXmlDocument* pDoc = new TiXmlDocument;
	if ( NULL == pDoc )
	{
		return false;
	}
	std::auto_ptr<TiXmlDocument> ptrDoc( pDoc );

	TiXmlElement *root = new TiXmlElement("AccountGroup");
	if ( NULL == root )
	{
		return false;
	}
	pDoc->LinkEndChild( root );

	root->SetAttribute("id",m_nGroupID);
	std::vector<AccountGroup>::iterator it = m_vAccountGroup.begin();
	for ( ; it != m_vAccountGroup.end(); it++ )
	{
		AccountGroup accountGroup = *it;
		TiXmlElement *pGroupItem = new TiXmlElement("Group");
		pGroupItem->SetAttribute("name", accountGroup.GetGroupName());
		pGroupItem->SetAttribute("id", accountGroup.GetGroupID());
		pGroupItem->SetAttribute("shortkey", accountGroup.GetShortKey());

		root->LinkEndChild(pGroupItem);

		int nAccountCount = accountGroup.GetAccountCount();
		for ( int i = 0; i < nAccountCount; i++)
		{
			AccountStru account;
			memset(&account, 0, sizeof(account));
			if ( !accountGroup.GetAccount( i, account))
			{
				continue;
			}

			TiXmlElement *pAccountItem = new TiXmlElement("Account");
			pAccountItem->SetAttribute("id", account.nID);
			pAccountItem->SetAttribute("name", account.szAccount);

			pGroupItem->LinkEndChild(pAccountItem);
		}	
	}

	std::string strFile = "AccountGroup.xml";
	GlobalFunc::GetPath( strFile );
	if( !pDoc->SaveFile( strFile.c_str() ))
		return false;

	return true;
}

void AccountGroupMgr::ClearCfg()
{
	m_vAccountGroup.clear();
	m_nMaxGroupID = 0;
}

bool AccountGroupMgr::IsGroupExist( int nGroupID )
{
	for ( UINT i = 0; i < m_vAccountGroup.size(); i++ )
	{
		if ( nGroupID == m_vAccountGroup[i].GetGroupID())
		{
			return true;
		}
	}

	return false;
}

int AccountGroupMgr::GetNextGroupID()
{
	return ++m_nMaxGroupID;
}

bool AccountGroupMgr::AddGroup( int nGroupID, std::string strGroupName )
{
	if ( IsGroupExist(nGroupID) )
	{
		return false;
	}

	AccountGroup accountGroup;
	accountGroup.SetGroupAttr(nGroupID, strGroupName, "");
	m_vAccountGroup.push_back(accountGroup);
	return true;
}

bool AccountGroupMgr::AddAccount( int nGroupID, int nAccountID, std::string strAccountName )
{
	for ( UINT i = 0; i < m_vAccountGroup.size(); i++ )
	{
		if ( nGroupID == m_vAccountGroup[i].GetGroupID())
		{
			m_vAccountGroup[i].AddAccount(nAccountID, strAccountName);
			return true;
		}
	}

	return false;
}

bool AccountGroupMgr::DelAccount( int nAccountID )
{
	for ( UINT i = 0; i < m_vAccountGroup.size(); i++ )
	{
		m_vAccountGroup[i].DelAccount(nAccountID);
	}

	return false;
}

std::vector<AccountGroup> AccountGroupMgr::GetAccountGroup()
{
	return m_vAccountGroup;
}

void AccountGroupMgr::SetAccountGroup( const std::vector<AccountGroup>& accountGroup )
{
	m_nMaxGroupID = 0;
	m_vAccountGroup = accountGroup;
	for ( UINT i = 0; i < m_vAccountGroup.size(); i++ )
	{
		if ( m_vAccountGroup[i].GetGroupID() > m_nMaxGroupID )
		{
			m_nMaxGroupID = m_vAccountGroup[i].GetGroupID();
		}
	}
}

void AccountGroupMgr::SetCurGroupID( int nGroupID )
{
	m_nGroupID = nGroupID;
}

int AccountGroupMgr::GetCurGroupID()
{
	return m_nGroupID;
}

void AccountGroupMgr::CalcUsableShortCutKeys( ShortCutKeysMgr* pShortCutKeyMgr )
{
	if ( NULL == pShortCutKeyMgr )
	{
		return;
	}

	for ( UINT i = 0; i < m_vAccountGroup.size(); i++ )
	{
		pShortCutKeyMgr->DelUsableShortCutKey(m_vAccountGroup[i].GetShortKey());
	}
}
