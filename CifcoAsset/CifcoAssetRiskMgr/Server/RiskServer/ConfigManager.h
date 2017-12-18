#pragma once

class ConfigManager
{
public:
	static ConfigManager* Get();
	static void Init();
	static void Release();

	bool LoadConfig();

	bool SaveConfig();

	bool IsInit();

	int GetPort();
	std::string GetDBUser();
	std::string GetDBPwd();
	std::string GetDBSID();
	int GetTimeOutSec();

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
};
