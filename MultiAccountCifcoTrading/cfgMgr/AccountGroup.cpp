#include "StdAfx.h"
#include "AccountGroup.h"

AccountGroup::AccountGroup(void)
: m_nGroupID(0)
, m_strGroupName("")
{
	m_vAccount.clear();
}

AccountGroup::~AccountGroup(void)
{
}

void AccountGroup::SetGroupAttr( int nGroupID, const std::string& strGroupName, const std::string& strShortKey )
{
	m_nGroupID = nGroupID;
	m_strGroupName = strGroupName;
	m_strShortKey = strShortKey;
}

bool AccountGroup::AddAccount( int nAccountID, std::string strAccountName )
{
	AccountStru account;
	memset(&account, 0, sizeof(account));
	account.nID = nAccountID;
	strcpy_s(account.szAccount, sizeof(account.szAccount), strAccountName.c_str());
	if ( IsAccountExist( account.szAccount ))
	{
		return false;
	}

	m_vAccount.push_back(account);
	return true;
}

bool AccountGroup::DelAccount( int nAccountID )
{
	std::vector<AccountStru>::iterator it = m_vAccount.begin();
	for ( ; it != m_vAccount.end(); ++it )
	{
		if ( nAccountID == (*it).nID )
		{
			m_vAccount.erase(it);
			return true;
		}
	}

	return false;
}

bool AccountGroup::IsAccountExist( const std::string& strAccount )
{
	for ( UINT i = 0; i < m_vAccount.size(); i++ )
	{
		if ( strAccount == m_vAccount[i].szAccount )
		{
			return true;
		}
	}

	return false;
}

void AccountGroup::ClearAccounts()
{
	m_vAccount.clear();
}

std::string AccountGroup::GetGroupName()
{
	return m_strGroupName;
}

int AccountGroup::GetGroupID()
{
	return m_nGroupID;
}

int AccountGroup::GetAccountCount()
{
	return m_vAccount.size();
}

bool AccountGroup::GetAccount( int nIndex, AccountStru& account )
{
	if ( nIndex >= static_cast<int>(m_vAccount.size()) || nIndex < 0 )
	{
		return false;
	}
	else
	{
		account = m_vAccount[nIndex];
		return true;
	}
}

void AccountGroup::SetShortKey( const std::string& strShortKey )
{
	m_strShortKey = strShortKey;
}

std::string AccountGroup::GetShortKey()
{
	return m_strShortKey;
}
