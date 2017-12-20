#include "stdafx.h"
#include <io.h>
#include <memory>
#include "tinyxml.h"
#include "ConfigManager.h"

using namespace std;

const char CONFIG_FILE[] = "system.xml";
ConfigManager* ConfigManager::m_pInstance = NULL;

ConfigManager::ConfigManager(void)
: m_nPort( -1 )
, m_strDBUser( "" )
, m_strDBPwd( "" )
, m_strDBSid( "" )
, m_nTimeOutSec( -1 )
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

ConfigManager* ConfigManager::GetInstance()
{
	if ( NULL == m_pInstance )
	{
		m_pInstance = new ConfigManager();
	}

	return m_pInstance;
}

void ConfigManager::DestroyConfigManager()
{
	if ( NULL != m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

bool ConfigManager::LoadConfig()
{
	char szBuffer[MAX_PATH];
	GetModuleFileName(NULL, szBuffer, sizeof(szBuffer));
	string strPath(szBuffer);
	int nPos = strPath.rfind("\\");
	strPath = strPath.substr(0, nPos+1);
	strPath = strPath + CONFIG_FILE;
	if ( -1 == _access( strPath.c_str(), 0 ))
	{
		std::cout << "file not exsit" << endl;
		return false;
	}

	TiXmlDocument* pXmlDocment = new TiXmlDocument( strPath );
	if( NULL == pXmlDocment)
	{
		return false;
	}

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() )
	{
		return false;
	}

	TiXmlElement *pRoot = pXmlDocment->RootElement();
	if ( NULL == pRoot )
	{
		return false;
	}

	if( 0 != strcmp(pRoot->Value(), "system"))
	{
		return false;
	}

	//Port
	TiXmlElement* pElementPort = pRoot->FirstChildElement("Port");
	if ( NULL == pElementPort )
	{
		return false;
	}
	m_nPort = atoi(pElementPort->GetText());

	//DataBase
	TiXmlElement* pElementDataBase = pRoot->FirstChildElement("DataBase");
	if ( NULL == pElementDataBase )
	{
		return false;
	}

	TiXmlElement* pElementSid = pElementDataBase->FirstChildElement("SID");
	if ( NULL == pElementSid )
	{
		return false;
	}
	m_strDBSid = pElementSid->GetText();

	TiXmlElement* pElementUser = pElementDataBase->FirstChildElement("User");
	if ( NULL == pElementUser )
	{
		return false;
	}
	m_strDBUser = pElementUser->GetText();

	TiXmlElement* pElementPwd = pElementDataBase->FirstChildElement("Password");
	if ( NULL == pElementPwd )
	{
		return false;
	}
	m_strDBPwd = pElementPwd->GetText();

	//TimeOutSec
	TiXmlElement* pElementTimeOutSec = pRoot->FirstChildElement("TimeOutSec");
	if ( NULL == pElementTimeOutSec )
	{
		return false;
	}
	m_nTimeOutSec = atoi(pElementTimeOutSec->GetText());

	return true;
}

int ConfigManager::GetPort()
{
	return m_nPort;
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

int ConfigManager::GetTimeOutSec()
{
	return m_nTimeOutSec;
}

bool ConfigManager::IsInit()
{
	return m_bInit;
}

bool ConfigManager::SaveConfig()
{
	char szBuffer[MAX_PATH];
	GetModuleFileName(NULL, szBuffer, sizeof(szBuffer));
	string strPath(szBuffer);
	int nPos = strPath.rfind("\\");
	strPath = strPath.substr(0, nPos+1);
	strPath = strPath + CONFIG_FILE;
	if ( -1 == _access( strPath.c_str(), 0 ))
	{
		std::cout << "file not exsit" << endl;
		return false;
	}

	TiXmlDocument* pXmlDocment = new TiXmlDocument( strPath );
	if( NULL == pXmlDocment)
	{
		return false;
	}

	std::auto_ptr<TiXmlDocument> ptr( pXmlDocment );
	if( !pXmlDocment->LoadFile() )
	{
		return false;
	}

	TiXmlElement *pRoot = pXmlDocment->RootElement();
	if ( NULL == pRoot )
	{
		return false;
	}

	if( 0 != strcmp(pRoot->Value(), "system"))
	{
		return false;
	}

	//DataBase
	TiXmlElement* pElementDataBase = pRoot->FirstChildElement("DataBase");
	if ( NULL != pElementDataBase )
	{
		pRoot->RemoveChild(pElementDataBase);
	}

	pElementDataBase = new TiXmlElement("DataBase");
	pRoot->LinkEndChild(pElementDataBase);

	TiXmlElement* pElementSid = new TiXmlElement("SID");
	TiXmlText* pContent = new TiXmlText(m_strDBSid); 
	pElementSid->LinkEndChild(pContent);
	pElementDataBase->LinkEndChild(pElementSid);

	TiXmlElement* pElementUser = new TiXmlElement("User");
	pContent = new TiXmlText(m_strDBUser); 
	pElementUser->LinkEndChild(pContent);
	pElementDataBase->LinkEndChild(pElementUser);

	TiXmlElement* pElementPwd = new TiXmlElement("Password");
	pContent = new TiXmlText(m_strDBPwd); 
	pElementPwd->LinkEndChild(pContent);
	pElementDataBase->LinkEndChild(pElementPwd);

	if ( !pXmlDocment->SaveFile(strPath))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void ConfigManager::SetDBUser( const std::string& strUser )
{
	m_strDBUser = strUser;
}

void ConfigManager::SetDBPwd( const std::string& strPwd )
{
	m_strDBPwd = strPwd;
}

void ConfigManager::SetDBSID( const std::string& strSID )
{
	m_strDBSid = strSID;
}
