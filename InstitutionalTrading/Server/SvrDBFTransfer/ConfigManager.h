#pragma once

#include <string>

class ConfigManager
{
public:
	static ConfigManager* Get();
	static void Init();
	static void Release();

	bool LoadConfig();

	bool IsInit();

	std::string GetDBUser();
	std::string GetDBPwd();
	std::string GetDBSID();
	std::string GeDBFShow2003Path();
	std::string GetDBFSJSHQPath();


private:
	ConfigManager(void);
	~ConfigManager(void);

private:
	static ConfigManager* m_pInstance;
	std::string m_strDBUser;
	std::string m_strDBPwd;
	std::string m_strDBSid;
	std::string m_strDBFShow2003Path;
	std::string m_strDBFSJSHQPath;

	bool m_bInit;
};
