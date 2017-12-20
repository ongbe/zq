#include "stdafx.h"
#include <io.h>
#include <memory>
#include "tinyxml.h"
#include "ConfigManager.h"

using namespace std;

const char CONFIG_FILE[] = "systemLocal.xml";
ConfigManager* ConfigManager::m_pInstance = NULL;

ConfigManager::ConfigManager(void)
: m_nPort( -1 )
, m_strDBUser( "" )
, m_strDBPwd( "" )
, m_strDBSid( "" )
, m_nTimeOutSec( -1 )
, m_bInit( false )
, m_nAccountID( -1 )
{
	memset(&m_logReq, 0, sizeof(m_logReq));
	m_vBrokerInfo.clear();
	m_vServerGroup.clear();
	m_vServerAddr.clear();
	m_vTradeAccount.clear();

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
		m_pInstance->SaveConfig();
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
	if( !pXmlDocment->LoadFile(TIXML_ENCODING_UNKNOWN) )
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

	//账号
	TiXmlElement* pAccount = pRoot->FirstChildElement("MAAccount");
	if ( NULL == pAccount )
	{
		return false;
	}
	else
	{
		m_nAccountID = atoi(pAccount->Attribute("ID"));
		strcpy(m_logReq.szAccount, pAccount->Attribute("Account"));
		strcpy(m_logReq.szPassword, pAccount->Attribute("Password"));
	}

	//服务器地址
	TiXmlElement* pBrokerList = pRoot->FirstChildElement("BrokerList");
	if ( NULL == pBrokerList )
	{
		return false;
	}

	for ( TiXmlElement* pBrokerInfo = pBrokerList->FirstChildElement("BrokerInfo"); 
		pBrokerInfo != NULL; 
		pBrokerInfo = pBrokerInfo->NextSiblingElement("BrokerInfo"))
	{
		//BrokerInfo
		BrokerInfo brokerInfo;
		memset(&brokerInfo, 0, sizeof(brokerInfo));
		brokerInfo.nBrokerID = atoi(pBrokerInfo->Attribute("BrokerID"));
		strcpy(brokerInfo.szCode, pBrokerInfo->Attribute("BrokerCode"));
		strcpy(brokerInfo.szCompany, pBrokerInfo->Attribute("Company"));
		brokerInfo.nOwnerAccountID = m_nAccountID;
		m_vBrokerInfo.push_back(brokerInfo);

		for ( TiXmlElement* pServerGroup = pBrokerInfo->FirstChildElement("ServerGroup"); 
			pServerGroup != NULL; 
			pServerGroup = pServerGroup->NextSiblingElement("ServerGroup"))
		{
			//ServerGroup
			ServerGroup serverGroup;
			memset(&serverGroup, 0, sizeof(serverGroup));
			serverGroup.nBrokerID = brokerInfo.nBrokerID;
			serverGroup.nServerNo = atoi(pServerGroup->Attribute("ServerNo"));
			strcpy(serverGroup.szServerName, pServerGroup->Attribute("ServerName"));
			m_vServerGroup.push_back(serverGroup);

			TiXmlElement* pTrading = pServerGroup->FirstChildElement("Trading");
			TiXmlElement* pMarketData = pServerGroup->FirstChildElement("MarketData");
			if ( NULL == pBrokerList || NULL == pMarketData )
			{
				continue;
			}

			for ( TiXmlElement* pTradeItem = pTrading->FirstChildElement("item"); 
				pTradeItem != NULL; 
				pTradeItem = pTradeItem->NextSiblingElement("item"))
			{
				ServerAddr serverAddr;
				memset(&serverAddr, 0, sizeof(serverAddr));
				serverAddr.nAddrID = atoi(pTradeItem->Attribute("AddrID"));
				serverAddr.nServerNo = serverGroup.nServerNo;
				serverAddr.nServerType = 0;
				
				string strAddr = pTradeItem->GetText();
				int nPos = strAddr.find(":");
				if ( nPos != -1 )
				{
					string strIP = strAddr.substr(0, nPos);
					string strPort = strAddr.substr(nPos+1, strAddr.length());
					strcpy(serverAddr.szServerAddress, strIP.c_str());
					serverAddr.nPort = atoi(strPort.c_str());
				}

				m_vServerAddr.push_back(serverAddr);
			}

			for ( TiXmlElement* pMarketDataItem = pMarketData->FirstChildElement("item"); 
				pMarketDataItem != NULL; 
				pMarketDataItem = pMarketDataItem->NextSiblingElement("item"))
			{
				ServerAddr serverAddr;
				memset(&serverAddr, 0, sizeof(serverAddr));
				serverAddr.nAddrID = atoi(pMarketDataItem->Attribute("AddrID"));
				serverAddr.nServerNo = serverGroup.nServerNo;
				serverAddr.nServerType = 1;

				string strAddr = pMarketDataItem->GetText();
				int nPos = strAddr.find(":");
				if ( nPos != -1 )
				{
					string strIP = strAddr.substr(0, nPos);
					string strPort = strAddr.substr(nPos+1, strAddr.length());
					strcpy(serverAddr.szServerAddress, strIP.c_str());
					serverAddr.nPort = atoi(strPort.c_str());
				}

				m_vServerAddr.push_back(serverAddr);
			}
		}
	}

	//交易地址
	TiXmlElement* pTradeAccountList = pRoot->FirstChildElement("TradeAccountList");
	if ( NULL == pTradeAccountList )
	{
		return false;
	}

	for ( TiXmlElement* pTradeAccount = pTradeAccountList->FirstChildElement("TradeAccount"); 
		pTradeAccount != NULL; 
		pTradeAccount = pTradeAccount->NextSiblingElement("TradeAccount"))
	{
		TradeAccount tradeAccount;
		memset(&tradeAccount, 0, sizeof(tradeAccount));
		strcpy(tradeAccount.szTradeAccount, pTradeAccount->Attribute("Account"));
		strcpy(tradeAccount.szPassword, pTradeAccount->Attribute("Password"));
		tradeAccount.nBrokerID = atoi(pTradeAccount->Attribute("BrokerID"));
		tradeAccount.nServerNo = atoi(pTradeAccount->Attribute("ServerNo"));
		strcpy(tradeAccount.szNickName, pTradeAccount->Attribute("NickName"));
		tradeAccount.nID = atoi(pTradeAccount->Attribute("ID"));
		tradeAccount.nRowNo = atoi(pTradeAccount->Attribute("RowNo"));
		m_vTradeAccount.push_back(tradeAccount);
	}

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

	TiXmlElement* pAccount = pRoot->FirstChildElement("MAAccount");
	pAccount->SetAttribute("Password", m_logReq.szPassword);

	TiXmlElement *pTradeAccountList = pRoot->FirstChildElement("TradeAccountList");
	if(pTradeAccountList)
		pRoot->RemoveChild(pTradeAccountList);

	pTradeAccountList = new TiXmlElement("TradeAccountList");
	for ( UINT i = 0; i < m_vTradeAccount.size(); i++ )
	{
		TradeAccount tradeAccount = m_vTradeAccount[i];

		TiXmlElement *pTradeAccount = new TiXmlElement("TradeAccount");
		pTradeAccount->SetAttribute("Account", tradeAccount.szTradeAccount);
		pTradeAccount->SetAttribute("Password", tradeAccount.szPassword);
		pTradeAccount->SetAttribute("BrokerID", tradeAccount.nBrokerID);
		pTradeAccount->SetAttribute("ServerNo", tradeAccount.nServerNo);
		pTradeAccount->SetAttribute("NickName", tradeAccount.szNickName);
		pTradeAccount->SetAttribute("ID", tradeAccount.nID);
		pTradeAccount->SetAttribute("RowNo", tradeAccount.nRowNo);
		pTradeAccountList->LinkEndChild(pTradeAccount);
	}

	if ( NULL != pTradeAccountList->FirstAttribute() || NULL != pTradeAccountList->FirstChild())
	{
		pRoot->LinkEndChild(pTradeAccountList);
	}
	else
	{
		delete pTradeAccountList;
		pTradeAccountList = NULL;
	}

	if( !pXmlDocment->SaveFile( strPath.c_str() ))
		return false;
	else
		return true;
}

bool ConfigManager::Logon( const LoginReq& req )
{
	if ( strcmp(m_logReq.szAccount, req.szAccount) == 0 &&
		strcmp(m_logReq.szPassword, req.szPassword) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::vector<BrokerInfo> ConfigManager::GetBrokerInfo()
{
	return m_vBrokerInfo;
}

std::vector<ServerGroup> ConfigManager::GetServerGroup( int nBrokerID )
{
	std::vector<ServerGroup> vGroup;
	for ( UINT i = 0; i < m_vServerGroup.size(); i++ )
	{
		if ( nBrokerID == m_vServerGroup[i].nBrokerID )
		{
			vGroup.push_back(m_vServerGroup[i]);
		}
	}

	return vGroup;
}

std::vector<ServerAddr> ConfigManager::GetServerAddr(int nServerNo)
{
	std::vector<ServerAddr> vAddr;
	for ( UINT i = 0; i < m_vServerAddr.size(); i++ )
	{
		if ( nServerNo == m_vServerAddr[i].nServerNo )
		{
			vAddr.push_back(m_vServerAddr[i]);
		}
	}

	return vAddr;
}

std::vector<TradeAccount> ConfigManager::GetTradeAccount()
{
	return m_vTradeAccount;
}

void ConfigManager::SetTradeAccount( const std::vector<TradeAccount>& vTradeAccount )
{
	m_vTradeAccount = vTradeAccount;
}

bool ConfigManager::ModifyPwd( const std::string& strNewPwd )
{
	strcpy(m_logReq.szPassword, strNewPwd.c_str());
	return SaveConfig();
}


