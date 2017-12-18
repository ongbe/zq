// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#include <io.h>
#include "ConfigMgr.h"
#include "tinyxml.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ConfigManager* ConfigManager::m_pInstance = NULL;

ConfigManager::ConfigManager(void)
: m_csSystemPath(_T(""))
, m_csIPAddr(_T(""))
, m_nPort(0)
, m_csAccount(_T(""))
, m_bRememberAccount(false)
, m_csPwd(_T(""))
, m_nAccountID(0)
, m_nPermissionID(0)
, m_nAssetMgmtOrgID(0)
, m_bMultiInstance(false)
{
	char szLocalPath[MAX_PATH];
	memset(szLocalPath, 0, sizeof(szLocalPath));
	::GetModuleFileName( NULL, szLocalPath, sizeof(szLocalPath) );
	CString csSystemPath( szLocalPath );
	int nPos = csSystemPath.ReverseFind( '\\' );
	if ( -1 != nPos )
	{
		m_csSystemPath = csSystemPath.Left( nPos );
	}

	LoadConfig();
    LoadXMlConfig();
}

ConfigManager::~ConfigManager(void)
{
	SaveConfig();
}

ConfigManager* ConfigManager::GetInstance()
{
	if ( NULL == m_pInstance )
	{
		m_pInstance = new ConfigManager();
	}

	return m_pInstance;
}

void ConfigManager::DestroyConfigerManager()
{
	if ( NULL != m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
bool ConfigManager::LoadXMlConfig()
{
    CString csSysIniFullPath = 
        m_csSystemPath + _T("\\OptionStrategy.xml");
    if( !IsFileExsit( csSysIniFullPath ))
    {
        return false;
    }

    TiXmlDocument doc(csSysIniFullPath.GetBuffer(0));
    bool loadOk = doc.LoadFile();
    if (!loadOk)
        return false;


    TiXmlNode* child = NULL;
    TiXmlElement* element = NULL;
    TiXmlAttribute *attr = NULL;
    const char* pValue;
    //int ct;
    while(child = doc.FirstChild()->IterateChildren(child))
    {
        if(strcmp(child->ValueStr().c_str(),"MultiInstance")!=0)
            continue;

        //根据不同的节点类型做相应处理  
//         switch(ct)
//         {
//         case TiXmlNode::TINYXML_TEXT:
//             break;
//         case TiXmlNode::TINYXML_ELEMENT:
            element = child->ToElement();
            pValue = element->FirstChild()->ValueStr().c_str();
            m_bMultiInstance = atoi(pValue)?true:false;
            return true;
//         }
    }

    return true;
}

bool ConfigManager::LoadConfig()
{
	CString csSysIniFullPath = 
		m_csSystemPath + _T("\\Client.ini");
	if( !IsFileExsit( csSysIniFullPath ))
	{
		return false;
	}

	CString csSectionName( _T("Server"));
	char szBuffer[MAX_PATH];
	memset( szBuffer, 0, MAX_PATH );
	int nRetLength = MAX_PATH;
	GetPrivateProfileString( csSectionName, _T("IPAddr"), _T(""), 
		szBuffer, nRetLength, csSysIniFullPath );
	m_csIPAddr = szBuffer;

	m_nPort = GetPrivateProfileInt( csSectionName, 
		_T("Port"), 0, csSysIniFullPath );

	csSectionName = _T("Account");
	memset( szBuffer, 0, MAX_PATH );
	GetPrivateProfileString( csSectionName, _T("Account"), _T(""), 
		szBuffer, nRetLength, csSysIniFullPath );
	m_csAccount = szBuffer;

	m_bRememberAccount = GetPrivateProfileInt( csSectionName, 
		_T("RememberAccount"), 0, csSysIniFullPath ) > 0 ? true : false;

    csSectionName = _T("ColorMode");
    m_nColorMode = GetPrivateProfileInt( csSectionName, 
        _T("Mode"), 0, csSysIniFullPath ) > 0 ? true : false;

    csSectionName = _T("Resolution");
    m_nResolutionWidth = GetPrivateProfileInt( csSectionName, _T("Width"), 800,csSysIniFullPath );
    m_nResolutionHight = GetPrivateProfileInt( csSectionName, _T("Hight"), 600,csSysIniFullPath );

    csSectionName = _T("Splitter");
    memset( szBuffer, 0, MAX_PATH );
    GetPrivateProfileString( csSectionName, _T("SplitterRateH"), _T("0.6"), 
        szBuffer, nRetLength, csSysIniFullPath );
    m_fSplitterH = atof(szBuffer);

    memset( szBuffer, 0, MAX_PATH );
    GetPrivateProfileString( csSectionName, _T("SplitterRateV"), _T("0.76"), 
        szBuffer, nRetLength, csSysIniFullPath );
    m_fSplitterV = atof(szBuffer);

	return true;
}

bool ConfigManager::SaveConfig()
{
	CString csSysIniFullPath = 
		m_csSystemPath + _T("\\Client.ini");
	if( !IsFileExsit( csSysIniFullPath ))
	{
		return false;
	}

	CString csSectionName( _T("Account"));
	if ( m_bRememberAccount )
	{
		WritePrivateProfileString( csSectionName, _T("Account"),
			m_csAccount, csSysIniFullPath );
	}
	else
	{
		WritePrivateProfileString( csSectionName, _T("Account"),
			_T(""), csSysIniFullPath );
	}

	CString csTemp;
	csTemp.Format( _T("%d"), static_cast<int>( m_bRememberAccount ));
	WritePrivateProfileString( csSectionName, _T("RememberAccount"),
		csTemp, csSysIniFullPath );

	return true;
}

CString ConfigManager::GetAccount() const
{
	return m_csAccount;
}

void ConfigManager::SetAccount( const CString& csAccount )
{
	m_csAccount = csAccount;
}

bool ConfigManager::GetRememberAccountFlag() const
{
	return m_bRememberAccount;
}

void ConfigManager::SetRememberAccountFlag( const bool bFlag )
{
	m_bRememberAccount = bFlag;
}

const CString& ConfigManager::GetSystemPath()
{
	return m_csSystemPath;
}

bool ConfigManager::IsFileExsit( const CString& csFileName )
{
	if ( -1 != _access( csFileName, 0 ))
	{
		return true;
	}
	else
	{
		return false;
	}
}

const CString ConfigManager::GetServerIPAddress()
{
	return m_csIPAddr;
}

int ConfigManager::GetServerPort()
{
	return m_nPort;
}

void ConfigManager::SetPassword( const CString& csPwd )
{
	m_csPwd = csPwd;
}

CString ConfigManager::GetPassword() const
{
	return m_csPwd;
}

void ConfigManager::SetAccountID( int nAccountID )
{
	m_nAccountID = nAccountID;
}

int ConfigManager::GetAccountID()
{
	return m_nAccountID;
}

void ConfigManager::SetPermissonID( int nAccountType )
{
	m_nPermissionID = nAccountType;
}

int ConfigManager::GetPermissonID()
{
	return m_nPermissionID;
}

void ConfigManager::SetAssetMgmtOrgID( int nOrgID )
{
	m_nAssetMgmtOrgID = nOrgID;
}

int ConfigManager::GetAssetMgmtOrgID()
{
	return m_nAssetMgmtOrgID;
}

int ConfigManager::GetResolutionWidth()
{
    return m_nResolutionWidth;
}

int ConfigManager::GetResolutionHight()
{
    return m_nResolutionHight;
}

float ConfigManager::GetSplitterH()
{
    return m_fSplitterH;
}
float ConfigManager::GetSplitterV()
{
    return m_fSplitterV;
}
int ConfigManager::GetColorMode()
{
    return m_nColorMode;
}
bool ConfigManager::GetMultiInstance()
{
    return m_bMultiInstance;
}