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

	std::string GetDBUser();
	std::string GetDBPwd();
	std::string GetDBSID();

private:
	ConfigManager(void);
	~ConfigManager(void);

private:
	static ConfigManager* m_pInstance;
	std::string m_strDBUser;
	std::string m_strDBPwd;
	std::string m_strDBSid;
	bool m_bInit;
};
