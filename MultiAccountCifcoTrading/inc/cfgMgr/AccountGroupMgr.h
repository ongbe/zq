#pragma once

#include "AccountGroup.h"

class ShortCutKeysMgr;

class AccountGroupMgr
{
public:
	AccountGroupMgr(void);
	~AccountGroupMgr(void);

	bool InitCfg();
	bool SaveCfg();
	void ClearCfg();

	bool IsGroupExist( int nGroupID );
	int GetNextGroupID();
	bool AddGroup( int nGroupID, std::string strGroupName );
	bool AddAccount( int nGroupID, int nAccountID, std::string strAccountName );
	bool DelAccount( int nAccountID );

	void SetCurGroupID( int nGroupID );
	int GetCurGroupID();

	std::vector<AccountGroup> GetAccountGroup();
	void SetAccountGroup( const std::vector<AccountGroup>& accountGroup);

	void CalcUsableShortCutKeys(ShortCutKeysMgr* pShortCutKeyMgr);

private:
	int m_nMaxGroupID;
	int m_nGroupID;
	std::vector<AccountGroup> m_vAccountGroup;
};
