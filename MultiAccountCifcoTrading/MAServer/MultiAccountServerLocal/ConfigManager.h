#pragma once

#include <vector>
#include "MultiAccountStruct.h"

class ConfigManager
{
public:
	static ConfigManager* GetInstance();

	static void DestroyConfigManager();

	bool LoadConfig();

	bool SaveConfig();

	bool IsInit();

	int GetPort();
	std::string GetDBUser();
	std::string GetDBPwd();
	std::string GetDBSID();
	int GetTimeOutSec();

	bool Logon(const LoginReq& req);
	std::vector<BrokerInfo> GetBrokerInfo();
	std::vector<ServerGroup> GetServerGroup( int nBrokerID );
	std::vector<ServerAddr>	GetServerAddr(int nServerNo);
	std::vector<TradeAccount> GetTradeAccount();
	void SetTradeAccount( const std::vector<TradeAccount>& vTradeAccount);
	bool ModifyPwd( const std::string& strNewPwd );

private:
	ConfigManager(void);
	~ConfigManager(void);

private:
	static ConfigManager* m_pInstance;
	int m_nPort;
	std::string m_strDBUser;
	std::string m_strDBPwd;
	std::string m_strDBSid;
	int m_nTimeOutSec;
	bool m_bInit;

	int m_nAccountID;
	LoginReq m_logReq;
	std::vector<BrokerInfo> m_vBrokerInfo;
	std::vector<ServerGroup> m_vServerGroup;
	std::vector<ServerAddr>	m_vServerAddr;
	std::vector<TradeAccount> m_vTradeAccount;
};
