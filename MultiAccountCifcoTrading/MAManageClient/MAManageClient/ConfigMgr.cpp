// ConfigMgr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#include <io.h>
#include "ConfigMgr.h"

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
, m_nAccountType(0)
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
	m_nAccountType = nAccountType;
}

int ConfigManager::GetPermissonID()
{
	return m_nAccountType;
}
