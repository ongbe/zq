#include "stdafx.h"
#include <io.h>
#include <memory>
#include "ConfigManager.h"
#include "MyXML.h"

using namespace std;

ConfigManager* ConfigManager::m_pInstance = NULL;

ConfigManager::ConfigManager(void)
: m_strDBUser( "" )
, m_strDBPwd( "" )
, m_strDBSid( "" )
, m_strDBFShow2003Path( "")
, m_strDBFSJSHQPath( "" )
, m_bInit( false )
{
	if( LoadConfig())
	{
		m_bInit = true;
	}
}

ConfigManager::~ConfigManager(void)
{
}

ConfigManager* ConfigManager::Get()
{
	return m_pInstance;
}
void ConfigManager::Init()
{
	if(m_pInstance)
	{
		Release();
	}
	m_pInstance = new ConfigManager();
}
void ConfigManager::Release()
{
	if ( NULL != m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool ConfigManager::LoadConfig()
{
	CMyXML myxml("InstitutionalTrading.xml");
	if(!myxml.m_bvalid) return false;
	m_strDBFShow2003Path = myxml.GetNodeContent2("","root","DBFFilePath","path_show2003");
	m_strDBFSJSHQPath = myxml.GetNodeContent2("","root","DBFFilePath","path_sjshq");
	m_strDBSid = myxml.GetNodeContent2("","root","Database","SID");
	m_strDBUser = myxml.GetNodeContent2("","root","Database","User");
	m_strDBPwd = myxml.GetNodeContent2("","root","Database","Password");
	return true;
}

string ConfigManager::GetDBUser()
{
	return m_strDBUser;
}

string ConfigManager::GetDBPwd()
{
	return m_strDBPwd;
}

string ConfigManager::GetDBSID()
{
	return m_strDBSid;
}
string ConfigManager::GeDBFShow2003Path()
{
	return m_strDBFShow2003Path;
}
string ConfigManager::GetDBFSJSHQPath()
{ 
	return m_strDBFSJSHQPath;
}

bool ConfigManager::IsInit()
{
	return m_bInit;
}
