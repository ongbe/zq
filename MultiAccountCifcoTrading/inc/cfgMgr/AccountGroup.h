#pragma once

#include "CfgGlobal.h"

class AccountGroup
{
public:
	AccountGroup(void);
	~AccountGroup(void);

	void SetGroupAttr( int nGroupID, const std::string& strGroupName, const std::string& strShortKey );
	bool AddAccount( int nAccountID, std::string strAccountName );
	bool DelAccount( int nAccountID );
	void ClearAccounts();
	bool IsAccountExist( const std::string& strAccount );

	std::string GetGroupName();
	int GetGroupID();

	void SetShortKey( const std::string& strShortKey);
	std::string GetShortKey();

	int GetAccountCount();
	bool GetAccount( int nIndex, AccountStru& account );

private:
	int m_nGroupID;
	std::string m_strGroupName;
	std::string m_strShortKey;
	std::vector<AccountStru> m_vAccount;
};
