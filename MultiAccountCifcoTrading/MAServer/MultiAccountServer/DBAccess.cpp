#include "stdafx.h"
#include <iostream>
#include "WriteLog.h"
#include "DBAccess.h"
using namespace std;
using namespace oracle::occi;

DBAccess::DBAccess()
: m_pEnvironment(NULL)
, m_pCon(NULL)
, m_pStmt(NULL)
, m_pRes(NULL)
, m_bConn(false)
, m_strLastError("")
, m_nErrorID(-1)
, m_strUserName("")
, m_strPwd("")
, m_strDBName("")
, m_pWriteLog(NULL)
{
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE, "DBAccess.log");
}

DBAccess::~DBAccess(void)
{
	DisConnected();

	if ( m_pWriteLog != NULL )
	{
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}
}

void DBAccess::Init( const string& strUserName, const string& strPwd, const string& strDBName )
{
	m_strUserName = strUserName;
	m_strPwd = strPwd;
	m_strDBName = strDBName;
}

bool DBAccess::Conncect()
{
	char szBuffer[1024];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf(szBuffer, "%s, %s, %s", m_strUserName.c_str(), m_strPwd.c_str(), m_strDBName.c_str());
	WriteLog("", szBuffer);
	if ( m_strUserName.empty() || m_strPwd.empty() || m_strDBName.empty() )
	{
		return false;
	}

	try{
		m_pEnvironment = Environment::createEnvironment(Environment::DEFAULT);
		if ( NULL == m_pEnvironment )
		{
			return false;
		}

		m_pCon = m_pEnvironment->createConnection(m_strUserName, m_strPwd, m_strDBName);
		if ( NULL == m_pCon )
		{
			return false;
		}

	}catch(SQLException &e){
		std::cout<<e.what()<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog("", m_strLastError);
		return false;
	}
	
	m_bConn = true;
	return true;
}

bool DBAccess::IsConnected()
{
	return m_bConn;
}

void DBAccess::DisConnected()
{
	if ( !m_bConn )
	{
		return;
	}

	try
	{
		if ( NULL != m_pCon && NULL != m_pEnvironment )
		{
			m_pEnvironment->terminateConnection(m_pCon);
		}
	}
	catch (SQLException& e)
	{
		std::cout << e.what() << endl;
	}

	m_bConn = false;
}

std::string DBAccess::GetLastErrorString()
{
	return m_strLastError;
}

int DBAccess::GetLastErrorCode()
{
	return m_nErrorID;
}

void DBAccess::RollBack()
{
	try
	{
		if ( NULL != m_pCon  )
		{
			m_pCon->rollback();
		}
	}
	catch (SQLException& e)
	{
		std::cout << e.what() << endl;
	}
}

void DBAccess::WriteLog(const string& strSql, const string& strError)
{
	if ( NULL != m_pWriteLog )
	{
		char szBuffer[MAX_SQL_LENGTH];
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf_s(szBuffer, MAX_USABLE_SQL_LENGTH, "%s\n%s", strError.c_str(), strSql.c_str());
		m_pWriteLog->WriteLog_Fmt("%s", WriteLogLevel_DEBUGINFO, szBuffer);
	}
}

bool DBAccess::ExcuteUpdate( const char* pSql, int& nNum )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement();
		nNum = m_pStmt->executeUpdate( pSql );
		m_pCon->commit();

		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::Excute( const char* pSql )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement();
		m_pStmt->execute( pSql );
		m_pCon->commit();

		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::ExcuteSelect( const char* pSql, int& nRecordNum )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		if ( m_pRes->next())
		{
			nRecordNum = m_pRes->getInt(1);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::InsertAndReturnID( const char* pSql, const char* pIDSql, int& nPKID )
{
	if ( NULL == pSql || (NULL != pSql && strlen(pSql) == 0) )
	{
		return false;
	}

	if ( NULL == pIDSql || (NULL != pIDSql && strlen(pIDSql) == 0) )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( );
		m_pStmt->executeUpdate( pSql );
		m_pRes = m_pStmt->executeQuery( pIDSql );
		if ( m_pRes->next())
		{
			nPKID = m_pRes->getInt(1);
		}

		m_pCon->commit();
		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::ExcuteLogon( const char* pUserName, const char* pPwd, 
						   int nAccountType, LoginRsp& rsp )
{
	if ( NULL == pUserName || NULL == pPwd )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	bool bRet = false;
	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	sprintf_s(szBuffer, MAX_SQL_LENGTH, 
		"select t.accountid, t.permissionid, t.accountstatus from ACCOUNT t where \
		t.account = '%s' and t.password = '%s' and t.accounttype=%d",
		pUserName, pPwd, nAccountType);

	try
	{
		m_pStmt = m_pCon->createStatement( szBuffer );
		m_pRes = m_pStmt->executeQuery();
		if ( m_pRes->next())
		{
			rsp.nAccountID = m_pRes->getInt(1);
			rsp.nPermissionID = m_pRes->getInt(2);
			int nAccountStatus = m_pRes->getInt(3);
			if ( nAccountStatus == 0 )
			{
				m_strLastError = "该账号已被停用，如有问题，请与账号创建者联系！";
			}
			else
			{
				bRet = true;
			}
		}
		else
		{
			m_strLastError = "用户名或密码错误";
		}
		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<szBuffer<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(szBuffer, m_strLastError);
		return false;
	}

	return bRet;
}

bool DBAccess::QueryData( const char* pSql, vector<AccountInfo>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			AccountInfo info;
			memset(&info, 0, sizeof(info));

			info.nAccountID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szAccount, sizeof(info.szAccount), strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy_s(info.szPassword, sizeof(info.szPassword), strValue.c_str());
			info.nAccountType = m_pRes->getInt(4);
			info.nOwnerAccountID = m_pRes->getInt(5);
			strValue = m_pRes->getString(6);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			strValue = m_pRes->getString(7);
			strcpy_s(info.szContactInfo, sizeof(info.szContactInfo), strValue.c_str());
			info.nIDCardType = m_pRes->getInt(8);
			strValue = m_pRes->getString(9);
			strcpy_s(info.szIDCardNo, sizeof(info.szIDCardNo), strValue.c_str());
			info.nAccountStatus = m_pRes->getInt(10);
			info.nLogonStatus = m_pRes->getInt(11);
			info.nViewPermission1 = m_pRes->getInt(12);
			info.nViewPermission2 = m_pRes->getInt(13);
			info.nPermissionID = m_pRes->getInt(14);

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, vector<AccountInfoEx>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			AccountInfoEx info;
			memset(&info, 0, sizeof(info));

			info.accountInfo.nAccountID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.accountInfo.szAccount, sizeof(info.accountInfo.szAccount), strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy_s(info.accountInfo.szPassword, sizeof(info.accountInfo.szPassword), strValue.c_str());
			info.accountInfo.nAccountType = m_pRes->getInt(4);
			info.accountInfo.nOwnerAccountID = m_pRes->getInt(5);
			strValue = m_pRes->getString(6);
			strcpy_s(info.accountInfo.szName, sizeof(info.accountInfo.szName), strValue.c_str());
			strValue = m_pRes->getString(7);
			strcpy_s(info.accountInfo.szContactInfo, sizeof(info.accountInfo.szContactInfo), strValue.c_str());
			info.accountInfo.nIDCardType = m_pRes->getInt(8);
			strValue = m_pRes->getString(9);
			strcpy_s(info.accountInfo.szIDCardNo, sizeof(info.accountInfo.szIDCardNo), strValue.c_str());
			info.accountInfo.nAccountStatus = m_pRes->getInt(10);
			info.accountInfo.nLogonStatus = m_pRes->getInt(11);
			info.accountInfo.nViewPermission1 = m_pRes->getInt(12);
			info.accountInfo.nViewPermission2 = m_pRes->getInt(13);
			info.accountInfo.nPermissionID = m_pRes->getInt(14);
			strValue = m_pRes->getString(15);
			strcpy_s(info.accountInfo.szDate, sizeof(info.accountInfo.szDate), strValue.c_str());

			//附加字段
			strValue = m_pRes->getString(16);
			strcpy_s(info.szOwnerAccount, sizeof(info.szOwnerAccount), strValue.c_str());
			strValue = m_pRes->getString(17);
			strcpy_s(info.szPermissionName, sizeof(info.szPermissionName), strValue.c_str());
			strValue = m_pRes->getString(18);
			strcpy_s(info.szIDCardName, sizeof(info.szIDCardName), strValue.c_str());

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<IDCardInfo>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			IDCardInfo info;
			memset(&info, 0, sizeof(info));

			info.nIDCardType = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szIDCardName, sizeof(info.szIDCardName), strValue.c_str());

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<ServerGroup>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			ServerGroup info;
			memset(&info, 0, sizeof(info));

			info.nServerNo = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szServerName, sizeof(info.szServerName), strValue.c_str());
			info.nBrokerID = m_pRes->getInt(3);
			info.nShare = m_pRes->getInt(4);

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<BrokerInfo>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		while ( m_pRes->next())
		{
			BrokerInfo info;
			memset(&info, 0, sizeof(info));

			info.nBrokerID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szCode, sizeof(info.szCode), strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy_s(info.szCompany, sizeof(info.szCompany), strValue.c_str());
			info.nShareWithLowerLever = m_pRes->getInt(4);

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<TradeAccount>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			TradeAccount info;
			memset(&info, 0, sizeof(info));

			string strValue = m_pRes->getString(1);
			strcpy_s(info.szTradeAccount, sizeof(info.szTradeAccount), strValue.c_str());
			strValue = m_pRes->getString(2);
			strcpy_s(info.szPassword, sizeof(info.szPassword), strValue.c_str());
			info.nOwnerAccountID = m_pRes->getInt(3);
			info.nBrokerID = m_pRes->getInt(4);
			info.nServerNo = m_pRes->getInt(5);
			strValue = m_pRes->getString(6);
			strcpy_s(info.szNickName, sizeof(info.szNickName), strValue.c_str());
			info.nID = m_pRes->getInt(7);
			info.nRowNo = m_pRes->getInt(8);

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<PermissionModule>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			PermissionModule info;
			memset(&info, 0, sizeof(info));

			info.nModuleID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szModuleName, sizeof(info.szModuleName), strValue.c_str());
			info.nCommonOperators = m_pRes->getInt(3);
			info.nAdditionalOperators = m_pRes->getInt(4);
			info.nModuleType = m_pRes->getInt(5);

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<Permission>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			Permission info;
			memset(&info, 0, sizeof(info));

			info.nPermissionID = m_pRes->getInt(1);
			info.nOwnerAccountID = m_pRes->getInt(2);
			string strValue = m_pRes->getString(3);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			info.nPermissionType = m_pRes->getInt(4);
			Blob blob = m_pRes->getBlob(5);
			Stream* pStream = blob.getStream();
			info.nPermissionDataSize = pStream->readBuffer(info.permissionData, sizeof(info.permissionData));

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<PermissionSimple>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			PermissionSimple info;
			memset(&info, 0, sizeof(info));

			info.nPermissionID = m_pRes->getInt(1);
			info.nOwnerAccountID = m_pRes->getInt(2);
			string strValue = m_pRes->getString(3);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			info.nPermissionType = m_pRes->getInt(4);

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<ServerAddr>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			ServerAddr info;
			memset(&info, 0, sizeof(info));

			info.nAddrID = m_pRes->getInt(1);
			info.nServerNo = m_pRes->getInt(2);
			string strValue = m_pRes->getString(3);
			strcpy_s(info.szServerAddress, sizeof(info.szServerAddress), strValue.c_str());
			info.nPort = m_pRes->getInt(4);
			info.nServerType = m_pRes->getInt(5);

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<MessageInfo>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			MessageInfo info;
			memset(&info, 0, sizeof(info));

			info.nMessageID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy(info.szTitle, strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy(info.szContent, strValue.c_str());
			strValue = m_pRes->getString(4);
			strcpy(info.szDate, strValue.c_str());
			info.nOwner = m_pRes->getInt(5);
			info.nType = m_pRes->getInt(6);

			vec.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QueryData( const char* pSql, std::vector<TargetAccount>& vec )
{
	if ( NULL == pSql )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		vec.clear();
		while ( m_pRes->next())
		{
			TargetAccount account;
			memset(&account, 0, sizeof(account));
			account.nAccountID = m_pRes->getInt(1);
			std::string strValue = m_pRes->getString(2);
			strcpy(account.szAccount, strValue.c_str());

			vec.push_back(account);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<pSql<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(pSql, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::AddPermissionData( const Permission* pData, int& nPKID )
{
	if ( NULL == pData )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		char szBuffer[MAX_SQL_LENGTH];
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into PERMISSION values(SEQ_PERMISSIONID.Nextval,%d, '%s', %d, :v1)",
			pData->nOwnerAccountID, pData->szName, pData->nPermissionType);
		
		m_pStmt = m_pCon->createStatement( szBuffer );
		Blob blob(m_pCon);
		blob.setEmpty();
		m_pStmt->setBlob(1, blob);
		m_pStmt->executeUpdate();

		m_pRes = m_pStmt->executeQuery( "select SEQ_PERMISSIONID.CURRVAL from dual" );
		if ( m_pRes->next())
		{
			nPKID = m_pRes->getInt(1);
		}

		m_pCon->commit();
		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(e.what(), m_strLastError);
		return false;
	}

	return true;

}

bool DBAccess::UpdatePermissionData( const Permission* pData, int& nNum )
{
	if ( NULL == pData )
	{
		return false;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		char szBuffer[MAX_SQL_LENGTH];
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, 
			"update PERMISSION t set t.name = '%s',t.permissiontype = %d \
			where t.permissionid = %d",
			pData->szName, pData->nPermissionType, pData->nPermissionID);

		m_pStmt = m_pCon->createStatement( szBuffer );
		nNum = m_pStmt->executeUpdate();
		m_pCon->commit();
		m_pCon->terminateStatement(m_pStmt);

		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "select t.permission from PERMISSION t where t.permissionid = %d FOR UPDATE",
			pData->nPermissionID);
		m_pStmt = m_pCon->createStatement( szBuffer );
		m_pRes = m_pStmt->executeQuery();
		if ( m_pRes->next())
		{
			Blob blob = m_pRes->getBlob(1);
			blob.trim(0);
			if ( pData->nPermissionDataSize != 0 )
			{
				blob.write(pData->nPermissionDataSize, (unsigned char*)pData->permissionData, pData->nPermissionDataSize);
			}

			m_pStmt->closeResultSet(m_pRes);
			m_pCon->terminateStatement(m_pStmt);
			m_pStmt = m_pCon->createStatement("update PERMISSION t set t.permission=:v1 where t.permissionid =:v2");
			m_pStmt->setBlob(1, blob);
			m_pStmt->setInt(2, pData->nPermissionID);
			nNum = m_pStmt->executeUpdate();
			m_pCon->commit();
		}
		else
		{
			m_pStmt->closeResultSet(m_pRes);
		}
		
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(e.what(), m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::DelAccountCaseCade( std::set<int>& setData )
{
	if ( setData.empty() )
	{
		return true;
	}

	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		m_pStmt = m_pCon->createStatement();
		std::set<int>::reverse_iterator it = setData.rbegin();
		for ( ; it != setData.rend(); it++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf_s(szBuffer, MAX_USABLE_SQL_LENGTH, 
				"delete from ACCOUNT t where t.accountid = %d or t.owner = %d", 
				*it, *it);
			m_pStmt->execute( szBuffer );
		}
		
		m_pCon->commit();
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<szBuffer<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(szBuffer, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::SaveTradeAccount( int nOwnerAccount, std::vector<TradeAccount> vAccount )
{
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "delete TRADEACCOUNT t where t.owner = %d", nOwnerAccount);

		m_pStmt = m_pCon->createStatement();
		m_pStmt->execute(szBuffer);

		std::vector<TradeAccount>::iterator it = vAccount.begin();
		for ( ; it != vAccount.end(); it++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into TRADEACCOUNT values('%s', '%s', %d, %d,%d, '%s',%d,%d)",
				(*it).szTradeAccount, (*it).szPassword,
				nOwnerAccount, (*it).nBrokerID,
				(*it).nServerNo, (*it).szNickName,
				(*it).nID, (*it).nRowNo);
			m_pStmt->execute( szBuffer );
		}

		m_pCon->commit();
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<szBuffer<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(szBuffer, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::AddMessage( const MessageInfo& msgInfo, const std::vector<TargetAccount> vAccount, 
						  std::vector<TargetAccount>& vForeAccount, int& nPKID )
{
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into MESSAGEINFO values(SEQ_MSGID.NEXTVAL, \
				'%s','%s', to_date('%s','YYYY-MM-DD'), %d, %d)", 
				msgInfo.szTitle, msgInfo.szContent, msgInfo.szDate, 
				msgInfo.nOwner, msgInfo.nType);

		m_pStmt = m_pCon->createStatement();
		m_pStmt->execute(szBuffer);

		m_pRes = m_pStmt->executeQuery( "select seq_msgid.currval from dual" );
		if ( m_pRes->next())
		{
			nPKID = m_pRes->getInt(1);
		}
		m_pStmt->closeResultSet(m_pRes);

		//往MessageTarget表中插入数据
		for ( UINT i = 0; i < vAccount.size(); i++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into messagetarget values(%d, %d, '%s')",
				vAccount[i].nAccountID, nPKID, vAccount[i].szAccount);
			m_pStmt->execute( szBuffer );
		}

		//计算接收消息的前台账户
		vForeAccount.clear();
		if ( msgInfo.nType == MESSAGE_COMMON)
		{
			//查询管理账号下属的前台账号
			for ( UINT i = 0; i < vAccount.size(); i++ )
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "select t.accountid, t.account from ACCOUNT t where t.owner = %d and t.accounttype = 1",
					vAccount[i]);
				m_pRes = m_pStmt->executeQuery(szBuffer);
				while ( m_pRes->next())
				{
					TargetAccount account;
					memset(&account, 0, sizeof(account));
					account.nAccountID = m_pRes->getInt(1);
					std::string strValue = m_pRes->getString(2);
					strcpy(account.szAccount, strValue.c_str());

					vForeAccount.push_back(account);
				}

				m_pStmt->closeResultSet(m_pRes);
			}
		}
		else
		{
			vForeAccount = vAccount;
		}

		m_pCon->commit();
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<szBuffer<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(szBuffer, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::SaveWaitForSendMessage( int nMessageID, std::vector<TargetAccount> vAccount )
{
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		m_pStmt = m_pCon->createStatement();
		for ( UINT i = 0; i < vAccount.size(); i++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into MESSAGEWAITFORSEND values(%d,%d,'%s')",
				vAccount[i].nAccountID, nMessageID, vAccount[i].szAccount);
			m_pStmt->execute( szBuffer );
		}

		m_pCon->commit();
		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		RollBack();
		std::cout<<e.what()<<endl;
		std::cout<<szBuffer<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(szBuffer, m_strLastError);
		return false;
	}

	return true;
}

bool DBAccess::QuerySharedBrokerInfo( int nForeAccountID, std::vector<BrokerInfo>& vec )
{
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	try
	{
		m_pStmt = m_pCon->createStatement();

		bool bFlag = true;
		int i = 0;
		int nAccountID = nForeAccountID;
		while(bFlag)
		{
			//查上一级别管理账号
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf( szBuffer, "select t.owner from ACCOUNT t where t.accountid = %d and t.owner != 0",
				nAccountID);
			m_pRes = m_pStmt->executeQuery(szBuffer);
			if ( !m_pRes->next())
			{
				m_pStmt->closeResultSet(m_pRes);
				bFlag = false;
			}
			else
			{
				nAccountID = m_pRes->getInt(1);
				m_pStmt->closeResultSet(m_pRes);

				memset(szBuffer, 0, sizeof(szBuffer));
				if ( i == 0 )
				{
					//查所属管理账户的经纪公司
					sprintf( szBuffer, "select * from BROKERINFO t where t.owner = %d",
						nAccountID);
				}
				else
				{
					//查共享的经纪公司
					sprintf( szBuffer, "select * from BROKERINFO t where t.owner = %d and t.sharewithlowerlever = 1",
						nAccountID);
				}

				m_pRes = m_pStmt->executeQuery(szBuffer);
				while ( m_pRes->next())
				{
					BrokerInfo info;
					memset(&info, 0, sizeof(info));

					info.nBrokerID = m_pRes->getInt(1);
					string strValue = m_pRes->getString(2);
					strcpy_s(info.szCode, sizeof(info.szCode), strValue.c_str());
					strValue = m_pRes->getString(3);
					strcpy_s(info.szCompany, sizeof(info.szCompany), strValue.c_str());
					info.nShareWithLowerLever = m_pRes->getInt(4);

					vec.push_back(info);
				}
				m_pStmt->closeResultSet(m_pRes);
			}

			i++;			
		}

		m_pCon->terminateStatement(m_pStmt);
	}catch(oracle::occi::SQLException &e){
		std::cout<<e.what()<<endl;
		std::cout<<szBuffer<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog(szBuffer, m_strLastError);
		return false;
	}

	return true;
}
