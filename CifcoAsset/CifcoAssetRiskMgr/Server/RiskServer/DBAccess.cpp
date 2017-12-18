#include "stdafx.h"
#include <iostream>
#include "Tools/WriteLog.h"
#include "DBAccess.h"
#include "Guard.h"

using namespace std;
using namespace oracle::occi;

const int nMaxInsertRow = 10000;
static bool s_bConn = false;

DBAccess::DBAccess()
: m_pEnvironment(NULL)
, m_pCon(NULL)
, m_pStmt(NULL)
, m_pRes(NULL)
, m_strLastError("")
, m_nErrorID(-1)
, m_strUserName("")
, m_strPwd("")
, m_strDBName("")
, m_pWriteLog(NULL)
{
	CGuard guard(&m_mutex);
	m_pWriteLog = new CWriteLog(WriteLogMode_LOCALFILE, "DBAccess.log");
}

DBAccess::~DBAccess(void)
{
	CGuard guard(&m_mutex);
	DisConnected();

	if ( m_pWriteLog != NULL )
	{
		delete m_pWriteLog;
		m_pWriteLog = NULL;
	}
}

void DBAccess::InitDB( const string& strUserName, const string& strPwd, const string& strDBName )
{
	CGuard guard(&m_mutex);
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
		else
		{
			m_pCon->setTAFNotify(&NotifyFn, NULL);
		}

	}catch(SQLException &e){
		std::cout<<e.what()<<endl;
		m_strLastError = e.what();
		m_nErrorID = 0;
		WriteLog("", m_strLastError);
		return false;
	}
	
	s_bConn = true;
	return true;
}

bool DBAccess::IsConnected()
{
	return s_bConn;
}

void DBAccess::DisConnected()
{
	if ( !s_bConn )
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

	s_bConn = false;
}

std::string DBAccess::GetLastErrorString()
{
	CGuard guard(&m_mutex);
	return m_strLastError;
}

int DBAccess::GetLastErrorCode()
{
	CGuard guard(&m_mutex);
	return m_nErrorID;
}

int DBAccess::NotifyFn(oracle::occi::Environment *env, oracle::occi::Connection *conn, 
					   void *ctx, oracle::occi::Connection::FailOverType foType, 
					   oracle::occi::Connection::FailOverEventType foEvent)
{
	cout << "TAF callback FailOverEventType " << foEvent << endl;
	switch(foEvent)
	{
	case Connection::FO_BEGIN:
		cout << "TAF callback start reconnecting..." << endl;
		s_bConn = false;
		break;
	case Connection::FO_END:
	case Connection::FO_ABORT:
		cout << "TAF callback reconnected successful" << endl;
		s_bConn = true;
		break;
	case Connection::FO_REAUTH:
		break;
	case Connection::FO_ERROR:
		cout << "Retrying" << endl;
		return FO_RETRY;
	default:
		break;
	}

	return 0; //continue failover

}

void DBAccess::RollBack()
{
	try
	{
		if ( NULL != m_pCon  )
		{
			m_pCon->rollback();
			if ( NULL != m_pStmt )
			{
				m_pCon->terminateStatement(m_pStmt);
			}
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
		m_pWriteLog->WriteLog_Fmt("", WriteLogLevel_DEBUGINFO, szBuffer);
	}
}

bool DBAccess::ExcuteUpdate( const char* pSql, int& nNum )
{
	CGuard guard(&m_mutex);
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
	CGuard guard(&m_mutex);
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
	CGuard guard(&m_mutex);
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
	CGuard guard(&m_mutex);
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
	CGuard guard(&m_mutex);
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
		"select t.riskmgmtuserid, t.permissionid, t.assetmgmtorgid, t.financialproductid, \
		t.status from RISKMGMTUSERINFO t where \
		t.account = '%s' and t.password = '%s' and t.roletype=%d",
		pUserName, pPwd, nAccountType);

	try
	{
		m_pStmt = m_pCon->createStatement( szBuffer );
		m_pRes = m_pStmt->executeQuery();
		if ( m_pRes->next())
		{
			rsp.nAccountID = m_pRes->getInt(1);
			rsp.nPermissionID = m_pRes->getInt(2);
			rsp.nAssetMgmtOrgID = m_pRes->getInt(3);
			rsp.nFinancialProductID = m_pRes->getInt(4);
			int nAccountStatus = m_pRes->getInt(5);
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

bool DBAccess::QueryData( const char* pSql, std::vector<MessageInfo>& vec )
{
	CGuard guard(&m_mutex);
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
			MessageInfo info;
			memset(&info, 0, sizeof(info));

			info.nMessageID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy(info.szTitle, strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy(info.szContent, strValue.c_str());
			strValue = m_pRes->getString(4);
			strcpy(info.szExpiredDate, strValue.c_str());
			info.nOwner = m_pRes->getInt(5);
			strValue = m_pRes->getString(6);
			strcpy(info.szCreateDate, strValue.c_str());
			strValue = m_pRes->getString(7);
			strcpy(info.szOwner, strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<MsgSendStatus>& vec )
{
	CGuard guard(&m_mutex);
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
			MsgSendStatus info;
			memset(&info, 0, sizeof(info));
			info.nRiskMgmtUserID = m_pRes->getInt(1);
			info.nMessageID = m_pRes->getInt(2);
			std::string strValue = m_pRes->getString(3);
			strcpy(info.szAccount, strValue.c_str());
			info.nSendStatus = m_pRes->getInt(4);

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

bool DBAccess::AddMessage( const MessageInfo& msgInfo, const std::vector<TargetAccount> vAccount, int& nPKID )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		char szOwner[50];
		memset(szOwner, 0, sizeof(szOwner));
		if ( msgInfo.nOwner == 0 )
		{
			strcpy(szOwner, "NULL");
		}
		else
		{
			sprintf(szOwner, "%d", msgInfo.nOwner);
		}

		sprintf(szBuffer, "insert into MESSAGEINFO values(SEQ_MESSAGEID.NEXTVAL, \
				'%s','%s', to_date('%s','YYYY-MM-DD'), %s, sysdate,'%s')", 
				msgInfo.szTitle, msgInfo.szContent, msgInfo.szExpiredDate, 
				szOwner, msgInfo.szOwner);

		m_pStmt = m_pCon->createStatement();
		m_pStmt->execute(szBuffer);

		m_pRes = m_pStmt->executeQuery( "select SEQ_MESSAGEID.currval from dual" );
		if ( m_pRes->next())
		{
			nPKID = m_pRes->getInt(1);
		}
		m_pStmt->closeResultSet(m_pRes);

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

bool DBAccess::SaveMessageSendStatus( std::vector<MsgSendStatus> vMsgStatus )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		m_pStmt = m_pCon->createStatement();
		for ( UINT i = 0; i < vMsgStatus.size(); i++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into MESSAGETARGET values(%d,%d,'%s',%d)",
				vMsgStatus[i].nRiskMgmtUserID, vMsgStatus[i].nMessageID, 
				vMsgStatus[i].szAccount, vMsgStatus[i].nSendStatus);
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

bool DBAccess::QueryData( const char* pSql, vector<RiskMgmtUserInfo>& vec )
{
	CGuard guard(&m_mutex);
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
			RiskMgmtUserInfo info;
			memset(&info, 0, sizeof(info));

			info.nRiskMgmtUserID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szAccount, sizeof(info.szAccount), strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy_s(info.szPassword, sizeof(info.szPassword), strValue.c_str());
			info.nRoleType = (RoleType)m_pRes->getInt(4);
			info.nAssetMgmtOrgID = m_pRes->getInt(5);
			info.nFinancialProductID = m_pRes->getInt(6);
			info.nStatus = m_pRes->getInt(7);
			strValue = m_pRes->getString(8);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			strValue = m_pRes->getString(9);
			strcpy_s(info.szContactInfo, sizeof(info.szContactInfo), strValue.c_str());
			info.nPermissionID = m_pRes->getInt(10);

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

bool DBAccess::QueryData( const char* pSql, std::vector<TrustTradeAccount>& vec )
{
	CGuard guard(&m_mutex);
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
			TrustTradeAccount info;
			memset(&info, 0, sizeof(info));

			info.nTradeAccountID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szAccount, sizeof(info.szAccount), strValue.c_str());
			info.nAssetMgmtOrgID = m_pRes->getInt(3);
			info.nFinancialProductID = m_pRes->getInt(4);
			info.nRiskIndModuleID = m_pRes->getInt(5);
			info.nTraderID = m_pRes->getInt(6);
			info.dProfit = m_pRes->getDouble(7);
			strValue = m_pRes->getString(8);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			strValue = m_pRes->getString(9);
			strcpy_s(info.szContactInfo, sizeof(info.szContactInfo), strValue.c_str());
			info.nIDCardType =(IDCardType) m_pRes->getInt(10);
			strValue = m_pRes->getString(11);
			strcpy_s(info.szIDCardNo, sizeof(info.szIDCardNo), strValue.c_str());
			strValue = m_pRes->getString(12);
			strcpy_s(info.szPwd, sizeof(info.szPwd), strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<TraderInfo>& vec )
{
	CGuard guard(&m_mutex);
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
			TraderInfo info;
			memset(&info, 0, sizeof(info));

			info.nTraderID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			info.nFinancialProductID = m_pRes->getInt(3);
			info.nAssetMgmtOrgID = m_pRes->getInt(4);
			strValue = m_pRes->getString(5);
			strcpy_s(info.szTel, sizeof(info.szTel), strValue.c_str());
			strValue = m_pRes->getString(6);
			strcpy_s(info.szMobile, sizeof(info.szMobile), strValue.c_str());
			strValue = m_pRes->getString(7);
			strcpy_s(info.szOtherContactInfo, sizeof(info.szOtherContactInfo), strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<AssetMgmtOrganization>& vec )
{
	CGuard guard(&m_mutex);
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
			AssetMgmtOrganization info;
			memset(&info, 0, sizeof(info));

			info.nAssetMgmtOrgID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			info.nUpperLevelOrgID = m_pRes->getInt(3);
			info.nScaleLevel = m_pRes->getInt(4);
			info.dLongitude = m_pRes->getDouble(5);
			info.dLatitude = m_pRes->getDouble(6);

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

bool DBAccess::QueryData( const char* pSql, std::vector<FinancialProduct>& vec )
{
	CGuard guard(&m_mutex);
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
			FinancialProduct info;
			memset(&info, 0, sizeof(info));

			info.nFinancialProductID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy_s(info.szScopeDesc, sizeof(info.szScopeDesc), strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<AssetOrgProRelation>& vec )
{
	CGuard guard(&m_mutex);
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
			AssetOrgProRelation info;
			memset(&info, 0, sizeof(info));

			info.nFinancialProductID = m_pRes->getInt(1);
			info.nAssetMgmtOrgID = m_pRes->getInt(2);

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

bool DBAccess::QueryData( const char* pSql, std::vector<RiskIndicatorModule>& vec )
{
	CGuard guard(&m_mutex);
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
			RiskIndicatorModule info;
			memset(&info, 0, sizeof(info));

			info.nRiskIndModuleID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<PermissionName>& vec )
{
	CGuard guard(&m_mutex);
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
			PermissionName info;
			memset(&info, 0, sizeof(info));

			info.nPermissionID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<PermissionNameEx>& vec )
{
	CGuard guard(&m_mutex);
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
			PermissionNameEx info;
			memset(&info, 0, sizeof(info));

			info.nPermissionID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			info.nPermissionType = m_pRes->getInt(3);

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

bool DBAccess::QueryData( const char* pSql, std::vector<PermissionNode>& vec )
{
	CGuard guard(&m_mutex);
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
			PermissionNode info;
			memset(&info, 0, sizeof(info));

			info.nNodeID = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());
			info.nNodeType = m_pRes->getInt(3);

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

bool DBAccess::QueryData( const char* pSql, std::vector<PermissionAction>& vec )
{
	CGuard guard(&m_mutex);
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
			PermissionAction info;
			memset(&info, 0, sizeof(info));

			info.nActionID = m_pRes->getInt(1);
			info.nNodeID = m_pRes->getInt(2);
			string strValue = m_pRes->getString(3);
			strcpy_s(info.szName, sizeof(info.szName), strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<PermissionValue>& vec )
{
	CGuard guard(&m_mutex);
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
			PermissionValue info;
			memset(&info, 0, sizeof(info));

			info.nPermissionID = m_pRes->getInt(1);
			info.nActionID = m_pRes->getInt(2);
			info.nValue = m_pRes->getInt(3);

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

bool DBAccess::QueryData( const char* pSql, std::vector<RiskIndicator>& vec )
{
	CGuard guard(&m_mutex);
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
			RiskIndicator info;
			memset(&info, 0, sizeof(info));

			info.nRiskIndicatorID = (RiskIndicatorType)m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy(info.szName, strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy(info.szAlgorithmDesc, strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<RiskWarning>& vec )
{
	CGuard guard(&m_mutex);
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
			RiskWarning info;
			memset(&info, 0, sizeof(info));

			info.nRiskIndModuleID = m_pRes->getInt(1);
			info.nRiskIndicatorID = (RiskIndicatorType)m_pRes->getInt(2);
			info.nRiskLevelID = m_pRes->getInt(3);
			info.fThresholdValue = m_pRes->getFloat(4);
			info.nResponseType = (RiskWarningType)m_pRes->getInt(5);
			info.nColor = m_pRes->getInt(6);

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

bool DBAccess::QueryData( const char* pSql, std::vector<MaxRiskLevel>& vec )
{
	CGuard guard(&m_mutex);
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
			MaxRiskLevel info;
			memset(&info, 0, sizeof(info));

			info.nRiskIndicatorID = (RiskIndicatorType)m_pRes->getInt(1);
			info.nMaxLevel = m_pRes->getInt(2);

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

bool DBAccess::QueryData( const char* pSql, std::vector<EventMessageTemplate>& vec )
{
	CGuard guard(&m_mutex);
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
			EventMessageTemplate info;
			memset(&info, 0, sizeof(info));

			info.nRiskIndicatorID = (RiskIndicatorType)m_pRes->getInt(1);
			info.nRiskLevelID = m_pRes->getInt(2);
			string strValue = m_pRes->getString(3);
			strcpy(info.szTitle, strValue.c_str());
			strValue = m_pRes->getString(4);
			strcpy(info.szContent, strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<ProductRiskWarning>& vec )
{
	CGuard guard(&m_mutex);
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
			ProductRiskWarning info;
			memset(&info, 0, sizeof(info));
			int col=0;
			info.nTradeAccountID=m_pRes->getInt(++col);	
			info.nRiskIndicatorID = (RiskIndicatorType)m_pRes->getInt(++col);
			info.nRiskLevelID = m_pRes->getInt(++col);			
			string strValue = m_pRes->getString(++col);
			strcpy(info.szProductID, strValue.c_str());
			info.fThresholdValue = m_pRes->getFloat(++col);
			info.nResponseType = (RiskWarningType)m_pRes->getInt(++col);
			info.nColor = m_pRes->getInt(++col);

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

bool DBAccess::QueryData( const char* pSql, std::vector<RiskEvent>& vec )
{
	CGuard guard(&m_mutex);
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
			RiskEvent info;
			memset(&info, 0, sizeof(info));

			info.nRiskEventID = m_pRes->getInt(1);	
			info.nRiskEventSubID = m_pRes->getInt(2);
			info.nTradeAccountID = m_pRes->getInt(3);
			info.lEventTime = m_pRes->getInt(4);
			info.nRiskIndModuleID = m_pRes->getInt(5);	
			info.nRiskIndicatorID = (RiskIndicatorType)m_pRes->getInt(6);
			info.nRiskLevelID = m_pRes->getInt(7);
			info.dblIndicatorValue = m_pRes->getDouble(8);
			info.nMsgSendStatus = (MsgStatusType)m_pRes->getInt(9);
			info.nIsValid = (RiskEventType)m_pRes->getInt(10);
			string strValue = m_pRes->getString(11);
			strcpy(info.InstrumentID, strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<RiskEventHandling>& vec )
{
	CGuard guard(&m_mutex);
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
			RiskEventHandling info;
			memset(&info, 0, sizeof(info));

			info.nRiskEventHandlingID = m_pRes->getInt(1);	
			info.nRiskEventID = m_pRes->getInt(2);
			info.nRiskEventSubID = m_pRes->getInt(3);
			info.lHandlingTime = m_pRes->getInt(4);
			string strValue = m_pRes->getString(5);	
			strcpy(info.szHandler, strValue.c_str());
			strValue = m_pRes->getString(6);	
			strcpy(info.szAccepter, strValue.c_str());
			info.nAction = (EventHandlingActionType)m_pRes->getInt(7);
			info.nProcessStatus = (MsgStatusType)m_pRes->getInt(8);
			strValue = m_pRes->getString(9);	
			strcpy(info.szContent, strValue.c_str());
			info.nTradeAccountID = m_pRes->getInt(10);

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


bool DBAccess::QueryData( const char* pSql, std::vector<RiskEventHandlingEx>& vec )
{
	CGuard guard(&m_mutex);
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
			RiskEventHandlingEx info;
			memset(&info, 0, sizeof(info));

			info.nRiskEventHandlingID = m_pRes->getInt(1);	
			info.nRiskEventID = m_pRes->getInt(2);
			info.nRiskEventSubID = m_pRes->getInt(3);
			info.lHandlingTime = m_pRes->getInt(4);
			string strValue = m_pRes->getString(5);	
			strcpy(info.szHandler, strValue.c_str());
			strValue = m_pRes->getString(6);	
			strcpy(info.szAccepter, strValue.c_str());
			info.nAction = (EventHandlingActionType)m_pRes->getInt(7);
			info.nProcessStatus = (MsgStatusType)m_pRes->getInt(8);
			strValue = m_pRes->getString(9);	
			strcpy(info.szContent, strValue.c_str());
			info.nTradeAccountID = m_pRes->getInt(10);
			info.nRiskIndicatorID = (RiskIndicatorType)m_pRes->getInt(11);
			info.nRiskLevelID = m_pRes->getInt(12);
			info.dblIndicatorValue = m_pRes->getDouble(13);
			strValue = m_pRes->getString(14);	
			strcpy(info.InstrumentID, strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<int>& vec )
{
	CGuard guard(&m_mutex);
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
			vec.push_back(m_pRes->getInt(1));
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

bool DBAccess::SavePermissionValue( int nPermissionID, std::vector<PermissionValue>& vec )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "delete PERMISSIONVALUE t where t.permissionid = %d", nPermissionID);

		m_pStmt = m_pCon->createStatement();
		m_pStmt->execute(szBuffer);

		std::vector<PermissionValue>::iterator it = vec.begin();
		for ( ; it != vec.end(); it++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into PERMISSIONVALUE values(%d, %d,%d)",
				nPermissionID, (*it).nActionID, (*it).nValue);
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

bool DBAccess::SaveFinOrgRelation( int nFinID, std::vector<int>& vec )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "select t.assetmgmtorgid from PRODUCTANDORGRELATION t where t.financialproductid = %d", nFinID);
		m_pStmt = m_pCon->createStatement();
		m_pRes = m_pStmt->executeQuery(szBuffer);
		std::vector<int> vOld;
		while ( m_pRes->next())
		{
			int nOrgID = m_pRes->getInt(1);
			vOld.push_back(nOrgID);
		}
		m_pStmt->closeResultSet(m_pRes);

		//新的数据与原来的数据对比
		//原来有，现在没有的数据，要删除
		for ( UINT i = 0; i < vOld.size(); i++ )
		{
			int nOrgID = vOld[i];
			bool bFind = false;
			for ( UINT j = 0; j < vec.size(); j++ )
			{
				if ( nOrgID == vec[j])
				{
					bFind = true;
					break;
				}
			}

			if ( !bFind )
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "delete PRODUCTANDORGRELATION t where \
								  t.financialproductid = %d and t.assetmgmtorgid = %d", 
								  nFinID, nOrgID);
				m_pStmt->executeQuery(szBuffer);
			}
		}

		//原来没有，现在有的数据，要添加
		for ( UINT i = 0; i < vec.size(); i++ )
		{
			int nOrgID = vec[i];
			bool bFind = false;
			for ( UINT j = 0; j < vOld.size(); j++ )
			{
				if ( nOrgID == vOld[j])
				{
					bFind = true;
					break;
				}
			}

			if ( !bFind )
			{
				memset(szBuffer, 0, sizeof(szBuffer));
				sprintf(szBuffer, "insert into PRODUCTANDORGRELATION values(%d, %d)", 
								  nFinID, nOrgID);
				m_pStmt->executeQuery(szBuffer);
			}
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

bool DBAccess::SaveRiskWarning( int nModuleID, std::vector<IndLevelWarning>& vec )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "delete RISKWARNING t where t.riskindmoduleid = %d", 
			nModuleID);

		m_pStmt = m_pCon->createStatement();
		m_pStmt->execute(szBuffer);

		std::vector<IndLevelWarning>::iterator it = vec.begin();
		for ( ; it != vec.end(); it++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into RISKWARNING values(%d,%d,%d,%.2f,%d,%d)",
				nModuleID, (*it).nRiskIndicatorID, (*it).nRiskLevelID, 
				(*it).fThresholdValue, (*it).nResponseType, (*it).nColor);
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

bool DBAccess::SaveMsgTemplate( int nIndicatorID, std::vector<EventMessageTemplate>& vec )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "delete EVENTMESSAGETEMPLATE t where t.riskindicatorid = %d", nIndicatorID);

		m_pStmt = m_pCon->createStatement();
		m_pStmt->execute(szBuffer);

		std::vector<EventMessageTemplate>::iterator it = vec.begin();
		for ( ; it != vec.end(); it++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into EVENTMESSAGETEMPLATE values(%d, %d,'%s','%s')",
				nIndicatorID, (*it).nRiskLevelID, (*it).szTitle, (*it).szContent);
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

bool DBAccess::SaveProductRiskWarning( const ProducrRiskWarningKey& key, std::vector<IndLevelWarning>& vec )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];

	try
	{
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "delete PRODUCTTHRESHOLD t where t.tradeaccountid = %d and t.productid = '%s'", 
			key.nTraderID, key.szProductID);

		m_pStmt = m_pCon->createStatement();
		m_pStmt->execute(szBuffer);

		std::vector<IndLevelWarning>::iterator it = vec.begin();
		for ( ; it != vec.end(); it++ )
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "insert into PRODUCTTHRESHOLD values(%d,%d,%d,'%s',%.2f,%d,%d)",
				key.nTraderID, (*it).nRiskIndicatorID, (*it).nRiskLevelID, key.szProductID,
				(*it).fThresholdValue, (*it).nResponseType, (*it).nColor);
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

bool DBAccess::SavePreRiskAccount( std::string strTime, 
								  std::vector<SeqPreRiskAccountField>& vPreRiskAccount )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	TSequenceNoType* seqNo = new TSequenceNoType[nMaxInsertRow];
	TBrokerIDType* brokerID = new TBrokerIDType[nMaxInsertRow];
	TInvestorIDType* investorID = new TInvestorIDType[nMaxInsertRow];
	TAccountIDType*	accountID = new TAccountIDType[nMaxInsertRow];
	TMoneyType* preMortgage = new TMoneyType[nMaxInsertRow];
	TMoneyType* preCredit = new TMoneyType[nMaxInsertRow];
	TMoneyType* preDeposit = new TMoneyType[nMaxInsertRow];
	TMoneyType* preBalance = new TMoneyType[nMaxInsertRow];
	TMoneyType*	preMargin = new TMoneyType[nMaxInsertRow];
	TMoneyType*	reserve = new TMoneyType[nMaxInsertRow];
	TMoneyType*	preExchMargin = new TMoneyType[nMaxInsertRow];
	TVolumeType* forceCloseStat = new TVolumeType[nMaxInsertRow];
	TMoneyType*	deliveryMargin = new TMoneyType[nMaxInsertRow];
	TMoneyType*	exchangeDeliveryMargin = new TMoneyType[nMaxInsertRow];
	TTimeType* recvTime = new TTimeType[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from PRERISKACCOUNT t where t.RECVTIME = '%s'",
			strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into PRERISKACCOUNT(SEQUENCENO,BROKERID,INVESTORID,ACCOUNTID,\
						  PREMORTGAGE,PRECREDIT,PREDEPOSIT,PREBALANCE,PREMARGIN,RESERVE,\
						  PREEXCHMARGIN,FORCECLOSESTAT,DELIVERYMARGIN,EXCHANGEDELIVERYMARGIN,\
						  RECVTIME) values(:v1,:v2,:v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10,:v11,\
						  :v12,:v13,:v14,:v15)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vPreRiskAccount.size() / nMaxInsertRow;
		if ( vPreRiskAccount.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_double[nMaxInsertRow];
			ub2 len_brokerID[nMaxInsertRow];
			ub2 len_investorID[nMaxInsertRow];
			ub2 len_accountID[nMaxInsertRow];
			ub2 len_recvTime[nMaxInsertRow];
			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vPreRiskAccount.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				SeqPreRiskAccountField preRiskAccount = vPreRiskAccount[j];
				seqNo[nRecordNum] = preRiskAccount.UniqSequenceNo;
				len_int[nRecordNum] = sizeof(int);
				strcpy(brokerID[nRecordNum], preRiskAccount.BrokerID);
				len_brokerID[nRecordNum] = strlen(brokerID[nRecordNum]) + 1;
				strcpy(investorID[nRecordNum], preRiskAccount.InvestorID);
				len_investorID[nRecordNum] = strlen(investorID[nRecordNum]) + 1;
				strcpy(accountID[nRecordNum], preRiskAccount.AccountID);
				len_accountID[nRecordNum] = strlen(accountID[nRecordNum]) + 1;
				preMortgage[nRecordNum] = preRiskAccount.PreMortgage;
				len_double[nRecordNum] = sizeof(TMoneyType);
				preCredit[nRecordNum] = preRiskAccount.PreCredit;
				preDeposit[nRecordNum] = preRiskAccount.PreDeposit;
				preBalance[nRecordNum] = preRiskAccount.PreBalance;
				preMargin[nRecordNum] = preRiskAccount.PreMargin;
				reserve[nRecordNum] = preRiskAccount.Reserve;
				preExchMargin[nRecordNum] = preRiskAccount.PreExchMargin;
				forceCloseStat[nRecordNum] = preRiskAccount.ForceCloseStat;
				deliveryMargin[nRecordNum] = preRiskAccount.DeliveryMargin;
				exchangeDeliveryMargin[nRecordNum] = preRiskAccount.ExchangeDeliveryMargin;
				strcpy(recvTime[nRecordNum], preRiskAccount.RecvTime);
				len_recvTime[nRecordNum] = strlen(recvTime[nRecordNum]) + 1;

				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, seqNo, OCCIINT, sizeof(TSequenceNoType), len_int); 
			m_pStmt->setDataBuffer(2, brokerID, OCCI_SQLT_STR, sizeof(TBrokerIDType), len_brokerID); 
			m_pStmt->setDataBuffer(3, investorID, OCCI_SQLT_STR, sizeof(TInvestorIDType), len_investorID); 
			m_pStmt->setDataBuffer(4, accountID, OCCI_SQLT_STR, sizeof(TAccountIDType), len_accountID); 
			m_pStmt->setDataBuffer(5, preMortgage, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(6, preCredit, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(7, preDeposit, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(8, preBalance, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(9, preMargin, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(10, reserve, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(11, preExchMargin, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(12, forceCloseStat, OCCIINT, sizeof(TVolumeType), len_int); 
			m_pStmt->setDataBuffer(13, deliveryMargin, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(14, exchangeDeliveryMargin, OCCIFLOAT, sizeof(TMoneyType), len_double); 
			m_pStmt->setDataBuffer(15, recvTime, OCCI_SQLT_STR, sizeof(TTimeType), len_recvTime); 

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] seqNo;
	delete [] brokerID;
	delete [] investorID;
	delete [] accountID;
	delete [] preMortgage;
	delete [] preCredit;
	delete [] preDeposit;
	delete [] preBalance;
	delete [] preMargin;
	delete [] reserve;
	delete [] preExchMargin;
	delete [] forceCloseStat;
	delete [] deliveryMargin;
	delete [] exchangeDeliveryMargin;
	delete [] recvTime;

	return bRet;
}

bool DBAccess::SaveRiskSyncAccount( std::string strTime, std::vector<RiskSyncAccountField>& vRiskSyncAccount )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	TBrokerIDType* BrokerID = new TBrokerIDType[nMaxInsertRow];
	TInvestorIDType* InvestorID = new TInvestorIDType[nMaxInsertRow];
	TMoneyType* Withdraw = new TMoneyType[nMaxInsertRow];
	TMoneyType* CurrMargin = new TMoneyType[nMaxInsertRow];
	TMoneyType* Commission = new TMoneyType[nMaxInsertRow];
	TMoneyType* CloseProfit = new TMoneyType[nMaxInsertRow];
	TMoneyType* PositionProfit = new TMoneyType[nMaxInsertRow];
	TMoneyType* Balance = new TMoneyType[nMaxInsertRow];
	TMoneyType* Available = new TMoneyType[nMaxInsertRow];
	TMoneyType* Mortgage = new TMoneyType[nMaxInsertRow];
	TMoneyType* ExchangeMargin = new TMoneyType[nMaxInsertRow];
	TMoneyType* WithdrawQuota = new TMoneyType[nMaxInsertRow];
	TMoneyType* Credit = new TMoneyType[nMaxInsertRow];
	TMoneyType* FrozenMargin = new TMoneyType[nMaxInsertRow];
	TMoneyType* FrozenCommission = new TMoneyType[nMaxInsertRow];
	TTimeType* RecvTime = new TTimeType[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from RISKSYNCACCOUNT t where t.RECVTIME = '%s'",
			strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into RISKSYNCACCOUNT(BROKERID,INVESTORID,WITHDRAW,\
						  CURRMARGIN,COMMISSION,CLOSEPROFIT,POSITIONPROFIT,BALANCE,AVAILABLE,\
						  MORTGAGE,EXCHANGEMARGIN,WITHDRAWQUOTA,CREDIT,FROZENMARGIN,FROZENCOMMISSION,\
						  RECVTIME) values(:v1,:v2,:v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10,\
						  :v11,:v12,:v13,:v14,:v15,:v16)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vRiskSyncAccount.size() / nMaxInsertRow;
		if ( vRiskSyncAccount.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_double[nMaxInsertRow];
			ub2 len_brokerID[nMaxInsertRow];
			ub2 len_investorID[nMaxInsertRow];
			ub2 len_recvTime[nMaxInsertRow];
			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vRiskSyncAccount.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				RiskSyncAccountField field = vRiskSyncAccount[j];

				len_int[nRecordNum] = sizeof(int);
				len_double[nRecordNum] = sizeof(double);
				strcpy(BrokerID[nRecordNum], field.BrokerID);
				len_brokerID[nRecordNum] = strlen(BrokerID[nRecordNum]) + 1;
				strcpy(InvestorID[nRecordNum], field.InvestorID);
				len_investorID[nRecordNum] = strlen(InvestorID[nRecordNum]) + 1;
				Withdraw[nRecordNum] = field.Withdraw;
				CurrMargin[nRecordNum] = field.CurrMargin;
				Commission[nRecordNum] = field.Commission;
				CloseProfit[nRecordNum] = field.CloseProfit;
				PositionProfit[nRecordNum] = field.PositionProfit;
				Balance[nRecordNum] = field.Balance;
				Available[nRecordNum] = field.Available;
				Mortgage[nRecordNum] = field.Mortgage;
				ExchangeMargin[nRecordNum] = field.ExchangeMargin;
				WithdrawQuota[nRecordNum] = field.WithdrawQuota;
				Credit[nRecordNum] = field.Credit;
				FrozenMargin[nRecordNum] = field.FrozenMargin;
				FrozenCommission[nRecordNum] = field.FrozenCommission;
				strcpy(RecvTime[nRecordNum], field.RecvTime);
				len_recvTime[nRecordNum] = strlen(RecvTime[nRecordNum]) + 1;

				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, BrokerID, OCCI_SQLT_STR, sizeof(TBrokerIDType), len_brokerID); 
			m_pStmt->setDataBuffer(2, InvestorID, OCCI_SQLT_STR, sizeof(TInvestorIDType), len_investorID); 
			m_pStmt->setDataBuffer(3, Withdraw, OCCIFLOAT, sizeof(double), len_double);
			m_pStmt->setDataBuffer(4, CurrMargin, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(5, Commission, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(6, CloseProfit, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(7, PositionProfit, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(8, Balance, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(9, Available, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(10, Mortgage, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(11, ExchangeMargin, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(12, WithdrawQuota, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(13, Credit, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(14, FrozenMargin, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(15, FrozenCommission, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(16, RecvTime, OCCI_SQLT_STR, sizeof(TTimeType), len_recvTime); 

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] BrokerID;
	delete [] InvestorID;
	delete [] Withdraw;
	delete [] CurrMargin;
	delete [] Commission;
	delete [] CloseProfit;
	delete [] PositionProfit;
	delete [] Balance;
	delete [] Available;
	delete [] Mortgage;
	delete [] ExchangeMargin;
	delete [] WithdrawQuota;
	delete [] Credit;
	delete [] FrozenMargin;
	delete [] FrozenCommission;
	delete [] RecvTime;

	return bRet;
}

bool DBAccess::QueryRiskAccount( const char* pSql, std::vector<RiskAllAccountField>& vec )
{
	CGuard guard(&m_mutex);
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
			RiskAllAccountField field;
			memset(&field, 0, sizeof(field));

			field.pre.UniqSequenceNo = m_pRes->getInt(1);
			string strValue = m_pRes->getString(2);
			strcpy(field.pre.BrokerID, strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy(field.pre.InvestorID, strValue.c_str());
			strValue = m_pRes->getString(4);
			strcpy(field.pre.AccountID, strValue.c_str());
			field.pre.PreMortgage = m_pRes->getDouble(5);
			field.pre.PreCredit = m_pRes->getDouble(6);
			field.pre.PreDeposit = m_pRes->getDouble(7);
			field.pre.PreBalance = m_pRes->getDouble(8);
			field.pre.PreMargin = m_pRes->getDouble(9);
			field.pre.Reserve = m_pRes->getDouble(10);
			field.pre.PreExchMargin = m_pRes->getDouble(11);
			field.pre.ForceCloseStat = m_pRes->getInt(12);
			field.pre.DeliveryMargin = m_pRes->getDouble(13);
			field.pre.ExchangeDeliveryMargin = m_pRes->getDouble(14);
			strValue = m_pRes->getString(15);
			strcpy(field.pre.RecvTime, strValue.c_str());

			strValue = m_pRes->getString(16);
			strcpy(field.cur.BrokerID, strValue.c_str());
			strValue = m_pRes->getString(17);
			strcpy(field.cur.InvestorID, strValue.c_str());
			field.cur.Withdraw = m_pRes->getDouble(18);
			field.cur.CurrMargin = m_pRes->getDouble(19);
			field.cur.Commission = m_pRes->getDouble(20);
			field.cur.CloseProfit = m_pRes->getDouble(21);
			field.cur.PositionProfit = m_pRes->getDouble(22);
			field.cur.Balance = m_pRes->getDouble(23);
			field.cur.Available = m_pRes->getDouble(24);
			field.cur.Mortgage = m_pRes->getDouble(25);
			field.cur.ExchangeMargin = m_pRes->getDouble(26);
			field.cur.WithdrawQuota = m_pRes->getDouble(27);
			field.cur.Credit = m_pRes->getDouble(28);
			field.cur.FrozenMargin = m_pRes->getDouble(29);
			field.cur.FrozenCommission = m_pRes->getDouble(30);
			strValue = m_pRes->getString(31);
			strcpy(field.cur.RecvTime, strValue.c_str());

			vec.push_back(field);
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

bool DBAccess::SaveTradeRecord( std::string strTime, std::vector<SequencialTradeField>& vTrade )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	TBrokerIDType* BrokerID = new TBrokerIDType[nMaxInsertRow];
	TInvestorIDType* InvestorID = new TInvestorIDType[nMaxInsertRow];
	TInstrumentIDType* InstrumentID = new TInstrumentIDType[nMaxInsertRow];
	TOrderRefType* OrderRef = new TOrderRefType[nMaxInsertRow];
	TUserIDType* UserID = new TUserIDType[nMaxInsertRow];
	TExchangeIDType* ExchangeID = new TExchangeIDType[nMaxInsertRow];
	TTradeIDType* TradeID = new TTradeIDType[nMaxInsertRow];
	TDirectionType* Direction = new TDirectionType[nMaxInsertRow];
	TOrderSysIDType* OrderSysID = new TOrderSysIDType[nMaxInsertRow];
	TParticipantIDType* ParticipantID = new TParticipantIDType[nMaxInsertRow];
	TClientIDType* ClientID = new TClientIDType[nMaxInsertRow];
	TTradingRoleType* TradingRole = new TTradingRoleType[nMaxInsertRow];
	TExchangeInstIDType* ExchangeInstID = new TExchangeInstIDType[nMaxInsertRow];
	TOffsetFlagType* OffsetFlag = new TOffsetFlagType[nMaxInsertRow];
	THedgeFlagType* HedgeFlag = new THedgeFlagType[nMaxInsertRow];
	TPriceType* Price = new TPriceType[nMaxInsertRow];
	TVolumeType* Volume = new TVolumeType[nMaxInsertRow];
	TDateType* TradeDate = new TDateType[nMaxInsertRow];
	TTimeType* TradeTime = new TTimeType[nMaxInsertRow];
	TTradeTypeType* TradeType = new TTradeTypeType[nMaxInsertRow];
	TPriceSourceType* PriceSource = new TPriceSourceType[nMaxInsertRow];
	TTraderIDType* TraderID = new TTraderIDType[nMaxInsertRow];
	TOrderLocalIDType* OrderLocalID = new TOrderLocalIDType[nMaxInsertRow];
	TParticipantIDType* ClearingPartID = new TParticipantIDType[nMaxInsertRow];
	TBusinessUnitType* BusinessUnit = new TBusinessUnitType[nMaxInsertRow];
	TSequenceNoType* SequenceNo = new TSequenceNoType[nMaxInsertRow];
	TDateType* TradingDay = new TDateType[nMaxInsertRow];
	TSettlementIDType* SettlementID = new TSettlementIDType[nMaxInsertRow];
	TSequenceNoType* BrokerOrderSeq = new TSequenceNoType[nMaxInsertRow];
	TTradeSourceType* TradeSource = new TTradeSourceType[nMaxInsertRow];
	int* UniqSequenceNo = new int[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from SEQUENCIALTRADE t where t.TRADEDATE = '%s'",
			strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into SEQUENCIALTRADE(BROKERID,INVESTORID,INSTRUMENTID,\
						  ORDERREF,USERID,EXCHANGEID,TRADEID,DIRECTION,ORDERSYSID,\
						  PARTICIPANTID,CLIENTID,TRADINGROLE,EXCHANGEINSTID,OFFSETFLAG,HEDGEFLAG,\
						  PRICE,VOLUME,TRADEDATE,TRADETIME,TRADETYPE,PRICESOURCE,TRADERID,\
						  ORDERLOCALID,CLEARINGPARTID,BUSINESSUNIT,SEQUENCENO,TRADINGDAY,\
						  SETTLEMENTID,BROKERORDERSEQ,TRADESOURCE,UNIQSEQUENCENO) values(:v1,:v2,\
						  :v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10,:v11,:v12,:v13,:v14,\
						  :v15,:v16,:v17,:v18,:v19,:v20,:v21,:v22,:v23,:v24,:v25,\
						  :v26,:v27,:v28,:v29,:v30,:v31)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vTrade.size() / nMaxInsertRow;
		if ( vTrade.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_double[nMaxInsertRow];
			ub2 len_char[nMaxInsertRow];
			ub2 len_brokerID[nMaxInsertRow];
			ub2 len_investorID[nMaxInsertRow];
			ub2 len_InstrumentID[nMaxInsertRow];
			ub2 len_OrderRef[nMaxInsertRow];
			ub2 len_UserID[nMaxInsertRow];
			ub2 len_ExchangeID[nMaxInsertRow];
			ub2 len_TradeID[nMaxInsertRow];
			ub2 len_OrderSysID[nMaxInsertRow];
			ub2 len_ParticipantID[nMaxInsertRow];
			ub2 len_ClientID[nMaxInsertRow];
			ub2 len_ExchangeInstID[nMaxInsertRow];
			ub2 len_TradeDate[nMaxInsertRow];
			ub2 len_TradeTime[nMaxInsertRow];
			ub2 len_TraderID[nMaxInsertRow];
			ub2 len_OrderLocalID[nMaxInsertRow];
			ub2 len_ClearingPartID[nMaxInsertRow];
			ub2 len_BusinessUnit[nMaxInsertRow];
			ub2 len_TradingDay[nMaxInsertRow];

			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vTrade.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				SequencialTradeField field = vTrade[j];

				len_int[nRecordNum] = sizeof(int);
				len_double[nRecordNum] = sizeof(double);
				len_char[nRecordNum] = sizeof(char);

				strcpy(BrokerID[nRecordNum], field.BrokerID);
				len_brokerID[nRecordNum] = strlen(BrokerID[nRecordNum]) + 1;
				strcpy(InvestorID[nRecordNum], field.InvestorID);
				len_investorID[nRecordNum] = strlen(InvestorID[nRecordNum]) + 1;
				strcpy(InstrumentID[nRecordNum], field.InstrumentID);
				len_InstrumentID[nRecordNum] = strlen(InstrumentID[nRecordNum]) + 1;
				strcpy(OrderRef[nRecordNum], field.OrderRef);
				len_OrderRef[nRecordNum] = strlen(OrderRef[nRecordNum]) + 1;
				strcpy(UserID[nRecordNum], field.UserID);
				len_UserID[nRecordNum] = strlen(UserID[nRecordNum]) + 1;
				strcpy(ExchangeID[nRecordNum], field.ExchangeID);
				len_ExchangeID[nRecordNum] = strlen(ExchangeID[nRecordNum]) + 1;
				strcpy(TradeID[nRecordNum], field.TradeID);
				len_TradeID[nRecordNum] = strlen(TradeID[nRecordNum]) + 1;
				Direction[nRecordNum] = field.Direction;
				strcpy(OrderSysID[nRecordNum], field.OrderSysID);
				len_OrderSysID[nRecordNum] = strlen(OrderSysID[nRecordNum]) + 1;
				strcpy(ParticipantID[nRecordNum], field.ParticipantID);
				len_ParticipantID[nRecordNum] = strlen(ParticipantID[nRecordNum]) + 1;
				strcpy(ClientID[nRecordNum], field.ClientID);
				len_ClientID[nRecordNum] = strlen(ClientID[nRecordNum]) + 1;
				TradingRole[nRecordNum] = field.TradingRole;
				strcpy(ExchangeInstID[nRecordNum], field.ExchangeInstID);
				len_ExchangeInstID[nRecordNum] = strlen(ExchangeInstID[nRecordNum]) + 1;
				OffsetFlag[nRecordNum] = field.OffsetFlag;
				HedgeFlag[nRecordNum] = field.HedgeFlag;
				Price[nRecordNum] = field.Price;
				Volume[nRecordNum] = field.Volume;
				strcpy(TradeDate[nRecordNum], field.TradeDate);
				len_TradeDate[nRecordNum] = strlen(TradeDate[nRecordNum]) + 1;
				strcpy(TradeTime[nRecordNum], field.TradeTime);
				len_TradeTime[nRecordNum] = strlen(TradeTime[nRecordNum]) + 1;
				TradeType[nRecordNum] = field.TradeType;
				PriceSource[nRecordNum] = field.PriceSource;
				strcpy(TraderID[nRecordNum], field.TraderID);
				len_TraderID[nRecordNum] = strlen(TraderID[nRecordNum]) + 1;
				strcpy(OrderLocalID[nRecordNum], field.OrderLocalID);
				len_OrderLocalID[nRecordNum] = strlen(OrderLocalID[nRecordNum]) + 1;
				strcpy(ClearingPartID[nRecordNum], field.ClearingPartID);
				len_ClearingPartID[nRecordNum] = strlen(ClearingPartID[nRecordNum]) + 1;
				strcpy(BusinessUnit[nRecordNum], field.BusinessUnit);
				len_BusinessUnit[nRecordNum] = strlen(BusinessUnit[nRecordNum]) + 1;
				SequenceNo[nRecordNum] = field.SequenceNo;
				strcpy(TradingDay[nRecordNum], field.TradingDay);
				len_TradingDay[nRecordNum] = strlen(TradingDay[nRecordNum]) + 1;
				SettlementID[nRecordNum] = field.SettlementID;
				BrokerOrderSeq[nRecordNum] = field.BrokerOrderSeq;
				TradeSource[nRecordNum] = field.TradeSource;
				UniqSequenceNo[nRecordNum] = field.UniqSequenceNo;

				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, BrokerID, OCCI_SQLT_STR, sizeof(TBrokerIDType), len_brokerID); 
			m_pStmt->setDataBuffer(2, InvestorID, OCCI_SQLT_STR, sizeof(TInvestorIDType), len_investorID); 
			m_pStmt->setDataBuffer(3, InstrumentID, OCCI_SQLT_STR, sizeof(TInstrumentIDType), len_InstrumentID);
			m_pStmt->setDataBuffer(4, OrderRef, OCCI_SQLT_STR, sizeof(TOrderRefType), len_OrderRef); 
			m_pStmt->setDataBuffer(5, UserID, OCCI_SQLT_STR, sizeof(TUserIDType), len_UserID); 
			m_pStmt->setDataBuffer(6, ExchangeID, OCCI_SQLT_STR, sizeof(TExchangeIDType), len_ExchangeID); 
			m_pStmt->setDataBuffer(7, TradeID, OCCI_SQLT_STR, sizeof(TTradeIDType), len_TradeID); 
			m_pStmt->setDataBuffer(8, Direction, OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(9, OrderSysID, OCCI_SQLT_STR, sizeof(TOrderSysIDType), len_OrderSysID); 
			m_pStmt->setDataBuffer(10, ParticipantID, OCCI_SQLT_STR, sizeof(TParticipantIDType), len_ParticipantID); 
			m_pStmt->setDataBuffer(11, ClientID, OCCI_SQLT_STR, sizeof(TClientIDType), len_ClientID); 
			m_pStmt->setDataBuffer(12, TradingRole, OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(13, ExchangeInstID, OCCI_SQLT_STR, sizeof(TExchangeInstIDType), len_ExchangeInstID); 
			m_pStmt->setDataBuffer(14, OffsetFlag, OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(15, HedgeFlag, OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(16, Price, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(17, Volume, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(18, TradeDate, OCCI_SQLT_STR, sizeof(TDateType), len_TradeDate); 
			m_pStmt->setDataBuffer(19, TradeTime, OCCI_SQLT_STR, sizeof(TTimeType), len_TradeTime);
			m_pStmt->setDataBuffer(20, TradeType, OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(21, PriceSource, OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(22, TraderID, OCCI_SQLT_STR, sizeof(TTraderIDType), len_TraderID); 
			m_pStmt->setDataBuffer(23, OrderLocalID, OCCI_SQLT_STR, sizeof(TOrderLocalIDType), len_OrderLocalID); 
			m_pStmt->setDataBuffer(24, ClearingPartID, OCCI_SQLT_STR, sizeof(TParticipantIDType), len_ClearingPartID); 
			m_pStmt->setDataBuffer(25, BusinessUnit, OCCI_SQLT_STR, sizeof(TBusinessUnitType), len_BusinessUnit); 
			m_pStmt->setDataBuffer(26, SequenceNo, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(27, TradingDay, OCCI_SQLT_STR, sizeof(TDateType), len_TradingDay); 
			m_pStmt->setDataBuffer(28, SettlementID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(29, BrokerOrderSeq, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(30, TradeSource, OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(31, UniqSequenceNo, OCCIINT, sizeof(int), len_int); 

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] BrokerID;
	delete [] InvestorID;
	delete [] InstrumentID;
	delete [] OrderRef;
	delete [] UserID;
	delete [] ExchangeID;
	delete [] TradeID;
	delete [] Direction;
	delete [] OrderSysID;
	delete [] ParticipantID;
	delete [] ClientID;
	delete [] TradingRole;
	delete [] ExchangeInstID;
	delete [] OffsetFlag;
	delete [] HedgeFlag;
	delete [] Price;

	delete [] Volume;
	delete [] TradeDate;
	delete [] TradeTime;
	delete [] TradeType;
	delete [] PriceSource;
	delete [] TraderID;
	delete [] OrderLocalID;
	delete [] ClearingPartID;
	delete [] BusinessUnit;
	delete [] SequenceNo;
	delete [] TradingDay;
	delete [] SettlementID;
	delete [] BrokerOrderSeq;
	delete [] TradeSource;
	delete [] UniqSequenceNo;

	return bRet;
}

bool DBAccess::QueryTradeRecord( const char* pSql, 
								std::vector<SequencialTradeField>& vec )
{
	CGuard guard(&m_mutex);
	if ( NULL == pSql)
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
			SequencialTradeField info;
			memset(&info, 0, sizeof(info));

			string strValue = m_pRes->getString(1);
			strcpy(info.BrokerID, strValue.c_str());
			strValue = m_pRes->getString(2);
			strcpy(info.InvestorID, strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy(info.InstrumentID, strValue.c_str());
			strValue = m_pRes->getString(4);
			strcpy(info.OrderRef, strValue.c_str());
			strValue = m_pRes->getString(5);
			strcpy(info.UserID, strValue.c_str());
			strValue = m_pRes->getString(6);
			strcpy(info.ExchangeID, strValue.c_str());
			strValue = m_pRes->getString(7);
			strcpy(info.TradeID, strValue.c_str());
			strValue = m_pRes->getString(8);
			if(!strValue.empty()) info.Direction = strValue[0];
			strValue = m_pRes->getString(9);
			strcpy(info.OrderSysID, strValue.c_str());
			strValue = m_pRes->getString(10);
			strcpy(info.ParticipantID, strValue.c_str());
			strValue = m_pRes->getString(11);
			strcpy(info.ClientID, strValue.c_str());
			strValue = m_pRes->getString(12);
			if(!strValue.empty()) info.TradingRole = strValue[0];
			strValue = m_pRes->getString(13);
			strcpy(info.ExchangeInstID, strValue.c_str());
			strValue = m_pRes->getString(14);
			if(!strValue.empty()) info.OffsetFlag = strValue[0];
			strValue = m_pRes->getString(15);
			if(!strValue.empty()) info.HedgeFlag = strValue[0];
			info.Price = m_pRes->getDouble(16);
			info.Volume = m_pRes->getInt(17);
			strValue = m_pRes->getString(18);
			strcpy(info.TradeDate, strValue.c_str());
			strValue = m_pRes->getString(19);
			strcpy(info.TradeTime, strValue.c_str());
			strValue = m_pRes->getString(20);
			if(!strValue.empty()) info.TradeType = strValue[0];
			strValue = m_pRes->getString(21);
			if(!strValue.empty()) info.PriceSource = strValue[0];
			strValue = m_pRes->getString(22);
			strcpy(info.TraderID, strValue.c_str());
			strValue = m_pRes->getString(23);
			strcpy(info.OrderLocalID, strValue.c_str());
			strValue = m_pRes->getString(24);
			strcpy(info.ClearingPartID, strValue.c_str());
			strValue = m_pRes->getString(25);
			strcpy(info.BusinessUnit, strValue.c_str());
			info.SequenceNo = m_pRes->getInt(26);
			strValue = m_pRes->getString(27);
			strcpy(info.TradingDay, strValue.c_str());
			info.SettlementID = m_pRes->getInt(28);
			info.BrokerOrderSeq = m_pRes->getInt(29);
			strValue = m_pRes->getString(30);
			if(!strValue.empty()) info.TradeSource = strValue[0];
			info.UniqSequenceNo = m_pRes->getInt(31);

			vec.push_back(info);
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

bool DBAccess::SaveOrderRecord( std::string strTime, std::vector<SequencialOrderField>& vOrder )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	TBrokerIDType* BrokerID = new TBrokerIDType[nMaxInsertRow];
	TInvestorIDType* InvestorID = new TInvestorIDType[nMaxInsertRow];
	TInstrumentIDType* InstrumentID = new TInstrumentIDType[nMaxInsertRow];
	TOrderRefType* OrderRef = new TOrderRefType[nMaxInsertRow];
	TUserIDType* UserID = new TUserIDType[nMaxInsertRow];
	TOrderPriceTypeType* OrderPriceType = new TOrderPriceTypeType[nMaxInsertRow];
	TDirectionType* Direction = new TDirectionType[nMaxInsertRow];
	TCombOffsetFlagType* CombOffsetFlag = new TCombOffsetFlagType[nMaxInsertRow];
	TCombHedgeFlagType* CombHedgeFlag = new TCombHedgeFlagType[nMaxInsertRow];
	TPriceType* LimitPrice = new TPriceType[nMaxInsertRow];
	TVolumeType* VolumeTotalOriginal = new TVolumeType[nMaxInsertRow];
	TTimeConditionType* TimeCondition = new TTimeConditionType[nMaxInsertRow];
	TDateType* GTDDate = new TDateType[nMaxInsertRow];
	TVolumeConditionType* VolumeCondition = new TVolumeConditionType[nMaxInsertRow];
	TVolumeType* MinVolume = new TVolumeType[nMaxInsertRow];
	TContingentConditionType* ContingentCondition = new TContingentConditionType[nMaxInsertRow];
	TPriceType* StopPrice = new TPriceType[nMaxInsertRow];
	TForceCloseReasonType* ForceCloseReason = new TForceCloseReasonType[nMaxInsertRow];
	TBoolType* IsAutoSuspend = new TBoolType[nMaxInsertRow];
	TBusinessUnitType* BusinessUnit = new TBusinessUnitType[nMaxInsertRow];
	TRequestIDType* RequestID = new TRequestIDType[nMaxInsertRow];
	TOrderLocalIDType* OrderLocalID = new TOrderLocalIDType[nMaxInsertRow];
	TExchangeIDType* ExchangeID = new TExchangeIDType[nMaxInsertRow];
	TParticipantIDType* ParticipantID = new TParticipantIDType[nMaxInsertRow];
	TClientIDType* ClientID = new TClientIDType[nMaxInsertRow];
	TExchangeInstIDType* ExchangeInstID = new TExchangeInstIDType[nMaxInsertRow];
	TTraderIDType* TraderID = new TTraderIDType[nMaxInsertRow];
	TInstallIDType* InstallID = new TInstallIDType[nMaxInsertRow];
	TOrderSubmitStatusType* OrderSubmitStatus = new TOrderSubmitStatusType[nMaxInsertRow];
	TSequenceNoType* NotifySequence = new TSequenceNoType[nMaxInsertRow];
	TDateType* TradingDay = new TDateType[nMaxInsertRow];
	TSettlementIDType* SettlementID = new TSettlementIDType[nMaxInsertRow];
	TOrderSysIDType* OrderSysID = new TOrderSysIDType[nMaxInsertRow];
	TOrderSourceType* OrderSource = new TOrderSourceType[nMaxInsertRow];
	TOrderStatusType* OrderStatus = new TOrderStatusType[nMaxInsertRow];
	TOrderTypeType* OrderType = new TOrderTypeType[nMaxInsertRow];
	TVolumeType* VolumeTraded = new TVolumeType[nMaxInsertRow];
	TVolumeType* VolumeTotal = new TVolumeType[nMaxInsertRow];
	TDateType* InsertDate = new TDateType[nMaxInsertRow];
	TTimeType* InsertTime = new TTimeType[nMaxInsertRow];
	TTimeType* ActiveTime = new TTimeType[nMaxInsertRow];
	TTimeType* SuspendTime = new TTimeType[nMaxInsertRow];
	TTimeType* UpdateTime = new TTimeType[nMaxInsertRow];
	TTimeType* CancelTime = new TTimeType[nMaxInsertRow];
	TTraderIDType* ActiveTraderID = new TTraderIDType[nMaxInsertRow];
	TParticipantIDType* ClearingPartID = new TParticipantIDType[nMaxInsertRow];
	TSequenceNoType* SequenceNo = new TSequenceNoType[nMaxInsertRow];
	TFrontIDType* FrontID = new TFrontIDType[nMaxInsertRow];
	TSessionIDType* SessionID = new TSessionIDType[nMaxInsertRow];
	TProductInfoType* UserProductInfo = new TProductInfoType[nMaxInsertRow];
	TErrorMsgType* StatusMsg = new TErrorMsgType[nMaxInsertRow];
	TBoolType* UserForceClose = new TBoolType[nMaxInsertRow];
	TUserIDType* ActiveUserID = new TUserIDType[nMaxInsertRow];
	TSequenceNoType* BrokerOrderSeq = new TSequenceNoType[nMaxInsertRow];
	TOrderSysIDType* RelativeOrderSysID = new TOrderSysIDType[nMaxInsertRow];
	TVolumeType* ZCETotalTradedVolume = new TVolumeType[nMaxInsertRow];
	int* UniqSequenceNo = new int[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from SEQUENCIALORDER t where t.TRADINGDAY = '%s'",
			strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into SEQUENCIALORDER values(:v1,:v2,\
						  :v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10,:v11,:v12,:v13,:v14,\
						  :v15,:v16,:v17,:v18,:v19,:v20,:v21,:v22,:v23,:v24,:v25,\
						  :v26,:v27,:v28,:v29,:v30,:v31,:v32,:v33,:v34,:v35,:v36,\
						  :v37,:v38,:v39,:v40,:v41,:v42,:v43,:v44,:v45,:v46,:v47,\
						  :v48,:v49,:v50,:v51,:v52,:v53,:v54,:v55,:v56,:v57)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vOrder.size() / nMaxInsertRow;
		if ( vOrder.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_double[nMaxInsertRow];
			ub2 len_char[nMaxInsertRow];
			ub2 len_BrokerID[nMaxInsertRow];
			ub2 len_InvestorID[nMaxInsertRow];
			ub2 len_InstrumentID[nMaxInsertRow];
			ub2 len_OrderRef[nMaxInsertRow];
			ub2 len_UserID[nMaxInsertRow];
			ub2 len_CombOffsetFlag[nMaxInsertRow];
			ub2 len_CombHedgeFlag[nMaxInsertRow];
			ub2 len_GTDDate[nMaxInsertRow];
			ub2 len_BusinessUnit[nMaxInsertRow];
			ub2 len_OrderLocalID[nMaxInsertRow];
			ub2 len_ExchangeID[nMaxInsertRow];
			ub2 len_ParticipantID[nMaxInsertRow];
			ub2 len_ClientID[nMaxInsertRow];
			ub2 len_ExchangeInstID[nMaxInsertRow];
			ub2 len_TraderID[nMaxInsertRow];
			ub2 len_TradingDay[nMaxInsertRow];
			ub2 len_OrderSysID[nMaxInsertRow];
			ub2 len_InsertDate[nMaxInsertRow];
			ub2 len_InsertTime[nMaxInsertRow];
			ub2 len_ActiveTime[nMaxInsertRow];
			ub2 len_SuspendTime[nMaxInsertRow];
			ub2 len_UpdateTime[nMaxInsertRow];
			ub2 len_CancelTime[nMaxInsertRow];
			ub2 len_ActiveTraderID[nMaxInsertRow];
			ub2 len_ClearingPartID[nMaxInsertRow];
			ub2 len_UserProductInfo[nMaxInsertRow];
			ub2 len_StatusMsg[nMaxInsertRow];
			ub2 len_ActiveUserID[nMaxInsertRow];
			ub2 len_RelativeOrderSysID[nMaxInsertRow];

			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vOrder.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				SequencialOrderField field = vOrder[j];

				len_int[nRecordNum] = sizeof(int);
				len_double[nRecordNum] = sizeof(double);
				len_char[nRecordNum] = sizeof(char);

				strcpy(BrokerID[nRecordNum], field.BrokerID);
				len_BrokerID[nRecordNum] = strlen(BrokerID[nRecordNum]) + 1;
				strcpy(InvestorID[nRecordNum], field.InvestorID);
				len_InvestorID[nRecordNum] = strlen(InvestorID[nRecordNum]) + 1;
				strcpy(InstrumentID[nRecordNum], field.InstrumentID);
				len_InstrumentID[nRecordNum] = strlen(InstrumentID[nRecordNum]) + 1;
				strcpy(OrderRef[nRecordNum], field.OrderRef);
				len_OrderRef[nRecordNum] = strlen(OrderRef[nRecordNum]) + 1;
				strcpy(UserID[nRecordNum], field.UserID);
				len_UserID[nRecordNum] = strlen(UserID[nRecordNum]) + 1;
				strcpy(CombOffsetFlag[nRecordNum], field.CombOffsetFlag);
				len_CombOffsetFlag[nRecordNum] = strlen(CombOffsetFlag[nRecordNum]) + 1;
				strcpy(CombHedgeFlag[nRecordNum], field.CombHedgeFlag);
				len_CombHedgeFlag[nRecordNum] = strlen(CombHedgeFlag[nRecordNum]) + 1;
				strcpy(GTDDate[nRecordNum], field.GTDDate);
				len_GTDDate[nRecordNum] = strlen(GTDDate[nRecordNum]) + 1;
				strcpy(BusinessUnit[nRecordNum], field.BusinessUnit);
				len_BusinessUnit[nRecordNum] = strlen(BusinessUnit[nRecordNum]) + 1;
				strcpy(OrderLocalID[nRecordNum], field.OrderLocalID);
				len_OrderLocalID[nRecordNum] = strlen(OrderLocalID[nRecordNum]) + 1;
				strcpy(ExchangeID[nRecordNum], field.ExchangeID);
				len_ExchangeID[nRecordNum] = strlen(ExchangeID[nRecordNum]) + 1;
				strcpy(ParticipantID[nRecordNum], field.ParticipantID);
				len_ParticipantID[nRecordNum] = strlen(ParticipantID[nRecordNum]) + 1;
				strcpy(ClientID[nRecordNum], field.ClientID);
				len_ClientID[nRecordNum] = strlen(ClientID[nRecordNum]) + 1;
				strcpy(ExchangeInstID[nRecordNum], field.ExchangeInstID);
				len_ExchangeInstID[nRecordNum] = strlen(ExchangeInstID[nRecordNum]) + 1;
				strcpy(TraderID[nRecordNum], field.TraderID);
				len_TraderID[nRecordNum] = strlen(TraderID[nRecordNum]) + 1;
				strcpy(TradingDay[nRecordNum], field.TradingDay);
				len_TradingDay[nRecordNum] = strlen(TradingDay[nRecordNum]) + 1;
				strcpy(OrderSysID[nRecordNum], field.OrderSysID);
				len_OrderSysID[nRecordNum] = strlen(OrderSysID[nRecordNum]) + 1;
				strcpy(InsertDate[nRecordNum], field.InsertDate);
				len_InsertDate[nRecordNum] = strlen(InsertDate[nRecordNum]) + 1;
				strcpy(InsertTime[nRecordNum], field.InsertTime);
				len_InsertTime[nRecordNum] = strlen(InsertTime[nRecordNum]) + 1;
				strcpy(ActiveTime[nRecordNum], field.ActiveTime);
				len_ActiveTime[nRecordNum] = strlen(ActiveTime[nRecordNum]) + 1;
				strcpy(SuspendTime[nRecordNum], field.SuspendTime);
				len_SuspendTime[nRecordNum] = strlen(SuspendTime[nRecordNum]) + 1;
				strcpy(UpdateTime[nRecordNum], field.UpdateTime);
				len_UpdateTime[nRecordNum] = strlen(UpdateTime[nRecordNum]) + 1;
				strcpy(CancelTime[nRecordNum], field.CancelTime);
				len_CancelTime[nRecordNum] = strlen(CancelTime[nRecordNum]) + 1;
				strcpy(ActiveTraderID[nRecordNum], field.ActiveTraderID);
				len_ActiveTraderID[nRecordNum] = strlen(ActiveTraderID[nRecordNum]) + 1;
				strcpy(ClearingPartID[nRecordNum], field.ClearingPartID);
				len_ClearingPartID[nRecordNum] = strlen(ClearingPartID[nRecordNum]) + 1;
				strcpy(UserProductInfo[nRecordNum], field.UserProductInfo);
				len_UserProductInfo[nRecordNum] = strlen(UserProductInfo[nRecordNum]) + 1;
				strcpy(StatusMsg[nRecordNum], field.StatusMsg);
				len_StatusMsg[nRecordNum] = strlen(StatusMsg[nRecordNum]) + 1;
				strcpy(ActiveUserID[nRecordNum], field.ActiveUserID);
				len_ActiveUserID[nRecordNum] = strlen(ActiveUserID[nRecordNum]) + 1;
				strcpy(RelativeOrderSysID[nRecordNum], field.RelativeOrderSysID);
				len_RelativeOrderSysID[nRecordNum] = strlen(RelativeOrderSysID[nRecordNum]) + 1;

				OrderPriceType[nRecordNum] = field.OrderPriceType;
				Direction[nRecordNum] = field.Direction;
				LimitPrice[nRecordNum] = field.LimitPrice;
				VolumeTotalOriginal[nRecordNum] = field.VolumeTotalOriginal;
				TimeCondition[nRecordNum] = field.TimeCondition;
				VolumeCondition[nRecordNum] = field.VolumeCondition;
				MinVolume[nRecordNum] = field.MinVolume;
				ContingentCondition[nRecordNum] = field.ContingentCondition;
				StopPrice[nRecordNum] = field.StopPrice;
				ForceCloseReason[nRecordNum] = field.ForceCloseReason;
				IsAutoSuspend[nRecordNum] = field.IsAutoSuspend;
				RequestID[nRecordNum] = field.RequestID;
				InstallID[nRecordNum] = field.InstallID;
				OrderSubmitStatus[nRecordNum] = field.OrderSubmitStatus;
				NotifySequence[nRecordNum] = field.NotifySequence;
				SettlementID[nRecordNum] = field.SettlementID;
				OrderSource[nRecordNum] = field.OrderSource;
				OrderStatus[nRecordNum] = field.OrderStatus;
				OrderType[nRecordNum] = field.OrderType;
				VolumeTraded[nRecordNum] = field.VolumeTraded;
				VolumeTotal[nRecordNum] = field.VolumeTotal;
				SequenceNo[nRecordNum] = field.SequenceNo;
				FrontID[nRecordNum] = field.FrontID;
				SessionID[nRecordNum] = field.SessionID;
				UserForceClose[nRecordNum] = field.UserForceClose;
				BrokerOrderSeq[nRecordNum] = field.BrokerOrderSeq;
				ZCETotalTradedVolume[nRecordNum] = field.ZCETotalTradedVolume;
				UniqSequenceNo[nRecordNum] = field.UniqSequenceNo;

				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, BrokerID, OCCI_SQLT_STR, sizeof(TBrokerIDType), len_BrokerID);
			m_pStmt->setDataBuffer(2, InvestorID, OCCI_SQLT_STR, sizeof(TInvestorIDType), len_InvestorID);
			m_pStmt->setDataBuffer(3, InstrumentID, OCCI_SQLT_STR, sizeof(TInstrumentIDType), len_InstrumentID);
			m_pStmt->setDataBuffer(4, OrderRef, OCCI_SQLT_STR, sizeof(TOrderRefType), len_OrderRef);
			m_pStmt->setDataBuffer(5, UserID, OCCI_SQLT_STR, sizeof(TUserIDType), len_UserID);
			m_pStmt->setDataBuffer(6, OrderPriceType, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(7, Direction, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(8, CombOffsetFlag, OCCI_SQLT_STR, sizeof(TCombOffsetFlagType), len_CombOffsetFlag);
			m_pStmt->setDataBuffer(9, CombHedgeFlag, OCCI_SQLT_STR, sizeof(TCombHedgeFlagType), len_CombHedgeFlag);
			m_pStmt->setDataBuffer(10, LimitPrice, OCCIFLOAT, sizeof(double), len_double);
			m_pStmt->setDataBuffer(11, VolumeTotalOriginal, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(12, TimeCondition, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(13, GTDDate, OCCI_SQLT_STR, sizeof(TDateType), len_GTDDate);
			m_pStmt->setDataBuffer(14, VolumeCondition, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(15, MinVolume, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(16, ContingentCondition, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(17, StopPrice, OCCIFLOAT, sizeof(double), len_double);
			m_pStmt->setDataBuffer(18, ForceCloseReason, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(19, IsAutoSuspend, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(20, BusinessUnit, OCCI_SQLT_STR, sizeof(TBusinessUnitType), len_BusinessUnit);
			m_pStmt->setDataBuffer(21, RequestID, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(22, OrderLocalID, OCCI_SQLT_STR, sizeof(TOrderLocalIDType), len_OrderLocalID);
			m_pStmt->setDataBuffer(23, ExchangeID, OCCI_SQLT_STR, sizeof(TExchangeIDType), len_ExchangeID);
			m_pStmt->setDataBuffer(24, ParticipantID, OCCI_SQLT_STR, sizeof(TParticipantIDType), len_ParticipantID);
			m_pStmt->setDataBuffer(25, ClientID, OCCI_SQLT_STR, sizeof(TClientIDType), len_ClientID);
			m_pStmt->setDataBuffer(26, ExchangeInstID, OCCI_SQLT_STR, sizeof(TExchangeInstIDType), len_ExchangeInstID);
			m_pStmt->setDataBuffer(27, TraderID, OCCI_SQLT_STR, sizeof(TTraderIDType), len_TraderID);
			m_pStmt->setDataBuffer(28, InstallID, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(29, OrderSubmitStatus, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(30, NotifySequence, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(31, TradingDay, OCCI_SQLT_STR, sizeof(TDateType), len_TradingDay);
			m_pStmt->setDataBuffer(32, SettlementID, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(33, OrderSysID, OCCI_SQLT_STR, sizeof(TOrderSysIDType), len_OrderSysID);
			m_pStmt->setDataBuffer(34, OrderSource, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(35, OrderStatus, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(36, OrderType, OCCI_SQLT_CHR, sizeof(char), len_char);
			m_pStmt->setDataBuffer(37, VolumeTraded, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(38, VolumeTotal, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(39, InsertDate, OCCI_SQLT_STR, sizeof(TDateType), len_InsertDate);
			m_pStmt->setDataBuffer(40, InsertTime, OCCI_SQLT_STR, sizeof(TTimeType), len_InsertTime);
			m_pStmt->setDataBuffer(41, ActiveTime, OCCI_SQLT_STR, sizeof(TTimeType), len_ActiveTime);
			m_pStmt->setDataBuffer(42, SuspendTime, OCCI_SQLT_STR, sizeof(TTimeType), len_SuspendTime);
			m_pStmt->setDataBuffer(43, UpdateTime, OCCI_SQLT_STR, sizeof(TTimeType), len_UpdateTime);
			m_pStmt->setDataBuffer(44, CancelTime, OCCI_SQLT_STR, sizeof(TTimeType), len_CancelTime);
			m_pStmt->setDataBuffer(45, ActiveTraderID, OCCI_SQLT_STR, sizeof(TTraderIDType), len_ActiveTraderID);
			m_pStmt->setDataBuffer(46, ClearingPartID, OCCI_SQLT_STR, sizeof(TParticipantIDType), len_ClearingPartID);
			m_pStmt->setDataBuffer(47, SequenceNo, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(48, FrontID, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(49, SessionID, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(50, UserProductInfo, OCCI_SQLT_STR, sizeof(TProductInfoType), len_UserProductInfo);
			m_pStmt->setDataBuffer(51, StatusMsg, OCCI_SQLT_STR, sizeof(TErrorMsgType), len_StatusMsg);
			m_pStmt->setDataBuffer(52, UserForceClose, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(53, ActiveUserID, OCCI_SQLT_STR, sizeof(TUserIDType), len_ActiveUserID);
			m_pStmt->setDataBuffer(54, BrokerOrderSeq, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(55, RelativeOrderSysID, OCCI_SQLT_STR, sizeof(TOrderSysIDType), len_RelativeOrderSysID);
			m_pStmt->setDataBuffer(56, ZCETotalTradedVolume, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(57, UniqSequenceNo, OCCIINT, sizeof(int), len_int);

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] BrokerID;
	delete [] InvestorID;
	delete [] InstrumentID;
	delete [] OrderRef;
	delete [] UserID;
	delete [] OrderPriceType;
	delete [] Direction;
	delete [] CombOffsetFlag;
	delete [] CombHedgeFlag;
	delete [] LimitPrice;
	delete [] VolumeTotalOriginal;
	delete [] TimeCondition;
	delete [] GTDDate;
	delete [] VolumeCondition;
	delete [] MinVolume;
	delete [] ContingentCondition;
	delete [] StopPrice;
	delete [] ForceCloseReason;
	delete [] IsAutoSuspend;
	delete [] BusinessUnit;
	delete [] RequestID;
	delete [] OrderLocalID;
	delete [] ExchangeID;
	delete [] ParticipantID;
	delete [] ClientID;
	delete [] ExchangeInstID;
	delete [] TraderID;
	delete [] InstallID;
	delete [] OrderSubmitStatus;
	delete [] NotifySequence;
	delete [] TradingDay;
	delete [] SettlementID;
	delete [] OrderSysID;
	delete [] OrderSource;
	delete [] OrderStatus;
	delete [] OrderType;
	delete [] VolumeTraded;
	delete [] VolumeTotal;
	delete [] InsertDate;
	delete [] InsertTime;
	delete [] ActiveTime;
	delete [] SuspendTime;
	delete [] UpdateTime;
	delete [] CancelTime;
	delete [] ActiveTraderID;
	delete [] ClearingPartID;
	delete [] SequenceNo;
	delete [] FrontID;
	delete [] SessionID;
	delete [] UserProductInfo;
	delete [] StatusMsg;
	delete [] UserForceClose;
	delete [] ActiveUserID;
	delete [] BrokerOrderSeq;
	delete [] RelativeOrderSysID;
	delete [] ZCETotalTradedVolume;
	delete [] UniqSequenceNo;

	return bRet;
}

bool DBAccess::QueryOrderRecord( const char* pSql, 
								std::vector<SequencialOrderField>& vec )
{
	CGuard guard(&m_mutex);
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
			SequencialOrderField info;
			memset(&info, 0, sizeof(info));

			string strValue = m_pRes->getString(1);
			strcpy(info.BrokerID, strValue.c_str());
			strValue = m_pRes->getString(2);
			strcpy(info.InvestorID, strValue.c_str());
			strValue = m_pRes->getString(3);
			strcpy(info.InstrumentID, strValue.c_str());
			strValue = m_pRes->getString(4);
			strcpy(info.OrderRef, strValue.c_str());
			strValue = m_pRes->getString(5);
			strcpy(info.UserID, strValue.c_str());
			strValue = m_pRes->getString(6);
			if(!strValue.empty()) info.OrderPriceType = strValue[0];
			strValue = m_pRes->getString(7);
			if(!strValue.empty()) info.Direction = strValue[0];
			strValue = m_pRes->getString(8);
			strcpy(info.CombOffsetFlag, strValue.c_str());
			strValue = m_pRes->getString(9);
			strcpy(info.CombHedgeFlag, strValue.c_str());
			info.LimitPrice = m_pRes->getDouble(10);
			info.VolumeTotalOriginal = m_pRes->getInt(11);
			strValue = m_pRes->getString(12);
			if(!strValue.empty()) info.TimeCondition = strValue[0];
			strValue = m_pRes->getString(13);
			strcpy(info.GTDDate, strValue.c_str());
			strValue = m_pRes->getString(14);
			if(!strValue.empty()) info.VolumeCondition = strValue[0];
			info.MinVolume = m_pRes->getInt(15);
			strValue = m_pRes->getString(16);
			if(!strValue.empty()) info.ContingentCondition = strValue[0];
			info.StopPrice = m_pRes->getDouble(17);
			strValue = m_pRes->getString(18);
			if(!strValue.empty()) info.ForceCloseReason = strValue[0];
			info.IsAutoSuspend = m_pRes->getInt(19);
			strValue = m_pRes->getString(20);
			strcpy(info.BusinessUnit, strValue.c_str());
			info.RequestID = m_pRes->getInt(21);
			strValue = m_pRes->getString(22);
			strcpy(info.OrderLocalID, strValue.c_str());
			strValue = m_pRes->getString(23);
			strcpy(info.ExchangeID, strValue.c_str());
			strValue = m_pRes->getString(24);
			strcpy(info.ParticipantID, strValue.c_str());
			strValue = m_pRes->getString(25);
			strcpy(info.ClientID, strValue.c_str());
			strValue = m_pRes->getString(26);
			strcpy(info.ExchangeInstID, strValue.c_str());
			strValue = m_pRes->getString(27);
			strcpy(info.TraderID, strValue.c_str());
			info.InstallID = m_pRes->getInt(28);
			strValue = m_pRes->getString(29);
			if(!strValue.empty()) info.OrderSubmitStatus = strValue[0];
			info.NotifySequence = m_pRes->getInt(30);
			strValue = m_pRes->getString(31);
			strcpy(info.TradingDay, strValue.c_str());
			info.SettlementID = m_pRes->getInt(32);
			strValue = m_pRes->getString(33);
			strcpy(info.OrderSysID, strValue.c_str());
			strValue = m_pRes->getString(34);
			if(!strValue.empty()) info.OrderSource = strValue[0];
			strValue = m_pRes->getString(35);
			if(!strValue.empty()) info.OrderStatus = strValue[0];
			strValue = m_pRes->getString(36);
			if(!strValue.empty()) info.OrderType = strValue[0];
			info.VolumeTraded = m_pRes->getInt(37);
			info.VolumeTotal = m_pRes->getInt(38);
			strValue = m_pRes->getString(39);
			strcpy(info.InsertDate, strValue.c_str());
			strValue = m_pRes->getString(40);
			strcpy(info.InsertTime, strValue.c_str());
			strValue = m_pRes->getString(41);
			strcpy(info.ActiveTime, strValue.c_str());
			strValue = m_pRes->getString(42);
			strcpy(info.SuspendTime, strValue.c_str());
			strValue = m_pRes->getString(43);
			strcpy(info.UpdateTime, strValue.c_str());
			strValue = m_pRes->getString(44);
			strcpy(info.CancelTime, strValue.c_str());
			strValue = m_pRes->getString(45);
			strcpy(info.ActiveTraderID, strValue.c_str());
			strValue = m_pRes->getString(46);
			strcpy(info.ClearingPartID, strValue.c_str());
			info.SequenceNo = m_pRes->getInt(47);
			info.FrontID = m_pRes->getInt(48);
			info.SessionID = m_pRes->getInt(49);
			strValue = m_pRes->getString(50);
			strcpy(info.UserProductInfo, strValue.c_str());
			strValue = m_pRes->getString(51);
			strcpy(info.StatusMsg, strValue.c_str());
			info.UserForceClose = m_pRes->getInt(52);
			strValue = m_pRes->getString(53);
			strcpy(info.ActiveUserID, strValue.c_str());
			info.BrokerOrderSeq = m_pRes->getInt(54);
			strValue = m_pRes->getString(55);
			strcpy(info.RelativeOrderSysID, strValue.c_str());
			info.ZCETotalTradedVolume = m_pRes->getInt(56);
			info.UniqSequenceNo = m_pRes->getInt(57);

			vec.push_back(info);
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

bool DBAccess::SavePositionRecord( std::string strTime, std::vector<SequencialPositionField>& vOrder )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	NEW_ARRAY_PTR(TSequenceNoType,UniqSequenceNo);///流中唯一的序列号
	NEW_ARRAY_PTR(TInstrumentIDType,InstrumentID);///合约代码
	NEW_ARRAY_PTR(TBrokerIDType,BrokerID);///经纪公司代码
	NEW_ARRAY_PTR(TInvestorIDType,InvestorID);///投资者代码
	NEW_ARRAY_PTR(TPosiDirectionType,PosiDirection);///持仓多空方向
	NEW_ARRAY_PTR(THedgeFlagType,HedgeFlag);///投机套保标志
	NEW_ARRAY_PTR(TPositionDateType,PositionDate);///持仓日期
	NEW_ARRAY_PTR(TVolumeType,YdPosition);///上日持仓
	NEW_ARRAY_PTR(TVolumeType,Position);///今日持仓
	NEW_ARRAY_PTR(TVolumeType,LongFrozen);///多头冻结
	NEW_ARRAY_PTR(TVolumeType,ShortFrozen);///空头冻结
	NEW_ARRAY_PTR(TMoneyType,LongFrozenAmount);///开仓冻结金额
	NEW_ARRAY_PTR(TMoneyType,ShortFrozenAmount);///开仓冻结金额
	NEW_ARRAY_PTR(TVolumeType,OpenVolume);///开仓量
	NEW_ARRAY_PTR(TVolumeType,CloseVolume);///平仓量
	NEW_ARRAY_PTR(TMoneyType,OpenAmount);///开仓金额
	NEW_ARRAY_PTR(TMoneyType,CloseAmount);///平仓金额
	NEW_ARRAY_PTR(TMoneyType,PositionCost);///持仓成本
	NEW_ARRAY_PTR(TMoneyType,PreMargin);///上次占用的保证金
	NEW_ARRAY_PTR(TMoneyType,UseMargin);///占用的保证金
	NEW_ARRAY_PTR(TMoneyType,FrozenMargin);///冻结的保证金
	NEW_ARRAY_PTR(TMoneyType,FrozenCash);///冻结的资金
	NEW_ARRAY_PTR(TMoneyType,FrozenCommission);///冻结的手续费
	NEW_ARRAY_PTR(TMoneyType,CashIn);///资金差额
	NEW_ARRAY_PTR(TMoneyType,Commission);///手续费
	NEW_ARRAY_PTR(TMoneyType,CloseProfit);///平仓盈亏
	NEW_ARRAY_PTR(TMoneyType,PositionProfit);///持仓盈亏
	NEW_ARRAY_PTR(TPriceType,PreSettlementPrice);///上次结算价
	NEW_ARRAY_PTR(TPriceType,SettlementPrice);///本次结算价
	NEW_ARRAY_PTR(TDateType,TradingDay);///交易日
	NEW_ARRAY_PTR(TSettlementIDType,SettlementID);///结算编号
	NEW_ARRAY_PTR(TMoneyType,OpenCost);///开仓成本
	NEW_ARRAY_PTR(TMoneyType,ExchangeMargin);///交易所保证金
	NEW_ARRAY_PTR(TVolumeType,CombPosition);///组合成交形成的持仓
	NEW_ARRAY_PTR(TVolumeType,CombLongFrozen);///组合多头冻结
	NEW_ARRAY_PTR(TVolumeType,CombShortFrozen);///组合空头冻结
	NEW_ARRAY_PTR(TMoneyType,CloseProfitByDate);///逐日盯市平仓盈亏
	NEW_ARRAY_PTR(TMoneyType,CloseProfitByTrade);///逐笔对冲平仓盈亏
	NEW_ARRAY_PTR(TVolumeType,TodayPosition);///今日持仓
	NEW_ARRAY_PTR(TRatioType,MarginRateByMoney);///保证金率
	NEW_ARRAY_PTR(TRatioType,MarginRateByVolume);///保证金率(按手数)

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from SEQUENCIALPOSITION t where t.TRADINGDAY = '%s'",
			strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into SEQUENCIALPOSITION values(:v1,:v2,\
						  :v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10,:v11,:v12,:v13,:v14,\
						  :v15,:v16,:v17,:v18,:v19,:v20,:v21,:v22,:v23,:v24,:v25,\
						  :v26,:v27,:v28,:v29,:v30,:v31,:v32,:v33,:v34,:v35,:v36,\
						  :v37,:v38,:v39,:v40,:v41,sysdate)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vOrder.size() / nMaxInsertRow;
		if ( vOrder.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_double[nMaxInsertRow];
			ub2 len_char[nMaxInsertRow];
			ub2 len_BrokerID[nMaxInsertRow];
			ub2 len_InvestorID[nMaxInsertRow];
			ub2 len_InstrumentID[nMaxInsertRow];
			ub2 len_TradingDay[nMaxInsertRow];

			int index=0,nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vOrder.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				SequencialPositionField& field = vOrder[j];

				len_int[nRecordNum] = sizeof(int);
				len_double[nRecordNum] = sizeof(double);
				len_char[nRecordNum] = sizeof(char);

				FILLDB_EQUVAL(UniqSequenceNo);	//唯一ID
				FILLDB_STR(InstrumentID,len_InstrumentID);	///合约代码
				FILLDB_STR(BrokerID,len_BrokerID);	///经纪公司代码
				FILLDB_STR(InvestorID,len_InvestorID);	///投资者代码
				FILLDB_EQUVAL(PosiDirection);	///持仓多空方向
				FILLDB_EQUVAL(HedgeFlag);	///投机套保标志
				FILLDB_EQUVAL(PositionDate);	///持仓日期
				FILLDB_EQUVAL(YdPosition);	///上日持仓
				FILLDB_EQUVAL(Position);	///今日持仓
				FILLDB_EQUVAL(LongFrozen);	///多头冻结
				FILLDB_EQUVAL(ShortFrozen);	///空头冻结
				FILLDB_EQUVAL(LongFrozenAmount);	///开仓冻结金额
				FILLDB_EQUVAL(ShortFrozenAmount);	///开仓冻结金额
				FILLDB_EQUVAL(OpenVolume);	///开仓量
				FILLDB_EQUVAL(CloseVolume);	///平仓量
				FILLDB_EQUVAL(OpenAmount);	///开仓金额
				FILLDB_EQUVAL(CloseAmount);	///平仓金额
				FILLDB_EQUVAL(PositionCost);	///持仓成本
				FILLDB_EQUVAL(PreMargin);	///上次占用的保证金
				FILLDB_EQUVAL(UseMargin);	///占用的保证金
				FILLDB_EQUVAL(FrozenMargin);	///冻结的保证金
				FILLDB_EQUVAL(FrozenCash);	///冻结的资金
				FILLDB_EQUVAL(FrozenCommission);	///冻结的手续费
				FILLDB_EQUVAL(CashIn);	///资金差额
				FILLDB_EQUVAL(Commission);	///手续费
				FILLDB_EQUVAL(CloseProfit);	///平仓盈亏
				FILLDB_EQUVAL(PositionProfit);	///持仓盈亏
				FILLDB_EQUVAL(PreSettlementPrice);	///上次结算价
				FILLDB_EQUVAL(SettlementPrice);	///本次结算价
				FILLDB_STR(TradingDay,len_TradingDay);	///交易日
				FILLDB_EQUVAL(SettlementID);	///结算编号
				FILLDB_EQUVAL(OpenCost);	///开仓成本
				FILLDB_EQUVAL(ExchangeMargin);	///交易所保证金
				FILLDB_EQUVAL(CombPosition);	///组合成交形成的持仓
				FILLDB_EQUVAL(CombLongFrozen);	///组合多头冻结
				FILLDB_EQUVAL(CombShortFrozen);	///组合空头冻结
				FILLDB_EQUVAL(CloseProfitByDate);	///逐日盯市平仓盈亏
				FILLDB_EQUVAL(CloseProfitByTrade);	///逐笔对冲平仓盈亏
				FILLDB_EQUVAL(TodayPosition);	///今日持仓
				FILLDB_EQUVAL(MarginRateByMoney);	///保证金率
				FILLDB_EQUVAL(MarginRateByVolume);	///保证金率(按手数)

				nRecordNum++;
			}
			SETDBBUF_INT(UniqSequenceNo);	//唯一ID
			SETDBBUF_STR(InstrumentID,TInstrumentIDType,len_InstrumentID);	///合约代码
			SETDBBUF_STR(BrokerID,TBrokerIDType, len_BrokerID);	///经纪公司代码
			SETDBBUF_STR(InvestorID,TInvestorIDType, len_InvestorID);	///投资者代码
			SETDBBUF_CHAR(PosiDirection);	///持仓多空方向
			SETDBBUF_CHAR(HedgeFlag);	///投机套保标志
			SETDBBUF_CHAR(PositionDate);	///持仓日期
			SETDBBUF_INT(YdPosition);	///上日持仓
			SETDBBUF_INT(Position);	///今日持仓
			SETDBBUF_INT(LongFrozen);	///多头冻结
			SETDBBUF_INT(ShortFrozen);	///空头冻结
			SETDBBUF_DOUBLE(LongFrozenAmount);	///开仓冻结金额
			SETDBBUF_DOUBLE(ShortFrozenAmount);	///开仓冻结金额
			SETDBBUF_INT(OpenVolume);	///开仓量
			SETDBBUF_INT(CloseVolume);	///平仓量
			SETDBBUF_DOUBLE(OpenAmount);	///开仓金额
			SETDBBUF_DOUBLE(CloseAmount);	///平仓金额
			SETDBBUF_DOUBLE(PositionCost);	///持仓成本
			SETDBBUF_DOUBLE(PreMargin);	///上次占用的保证金
			SETDBBUF_DOUBLE(UseMargin);	///占用的保证金
			SETDBBUF_DOUBLE(FrozenMargin);	///冻结的保证金
			SETDBBUF_DOUBLE(FrozenCash);	///冻结的资金
			SETDBBUF_DOUBLE(FrozenCommission);	///冻结的手续费
			SETDBBUF_DOUBLE(CashIn);	///资金差额
			SETDBBUF_DOUBLE(Commission);	///手续费
			SETDBBUF_DOUBLE(CloseProfit);	///平仓盈亏
			SETDBBUF_DOUBLE(PositionProfit);	///持仓盈亏
			SETDBBUF_DOUBLE(PreSettlementPrice);	///上次结算价
			SETDBBUF_DOUBLE(SettlementPrice);	///本次结算价
			SETDBBUF_STR(TradingDay,TDateType, len_TradingDay);	///交易日
			SETDBBUF_INT(SettlementID);	///结算编号
			SETDBBUF_DOUBLE(OpenCost);	///开仓成本
			SETDBBUF_DOUBLE(ExchangeMargin);	///交易所保证金
			SETDBBUF_INT(CombPosition);	///组合成交形成的持仓
			SETDBBUF_INT(CombLongFrozen);	///组合多头冻结
			SETDBBUF_INT(CombShortFrozen);	///组合空头冻结
			SETDBBUF_DOUBLE(CloseProfitByDate);	///逐日盯市平仓盈亏
			SETDBBUF_DOUBLE(CloseProfitByTrade);	///逐笔对冲平仓盈亏
			SETDBBUF_INT(TodayPosition);	///今日持仓
			SETDBBUF_DOUBLE(MarginRateByMoney);	///保证金率
			SETDBBUF_DOUBLE(MarginRateByVolume);	///保证金率(按手数)
			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}
	return bRet;
}

bool DBAccess::QueryPositionRecord( const char* pSql, 
								   std::vector<SequencialPositionField>& vec )
{
	CGuard guard(&m_mutex);
	if (pSql==NULL && !IsConnected() && !Conncect())
	{
		return false;
	}

	try
	{
		m_pStmt = m_pCon->createStatement( pSql );
		m_pRes = m_pStmt->executeQuery();
		while ( m_pRes->next())
		{
			SequencialPositionField info;
			memset(&info, 0, sizeof(info));
			int index=0;

			GETDB_INT(UniqSequenceNo);	//唯一ID
			GETDB_STR(InstrumentID);	///合约代码
			GETDB_STR(BrokerID);	///经纪公司代码
			GETDB_STR(InvestorID);	///投资者代码
			GETDB_CHAR(PosiDirection);	///持仓多空方向
			GETDB_CHAR(HedgeFlag);	///投机套保标志
			GETDB_CHAR(PositionDate);	///持仓日期
			GETDB_INT(YdPosition);	///上日持仓
			GETDB_INT(Position);	///今日持仓
			GETDB_INT(LongFrozen);	///多头冻结
			GETDB_INT(ShortFrozen);	///空头冻结
			GETDB_DOUBLE(LongFrozenAmount);	///开仓冻结金额
			GETDB_DOUBLE(ShortFrozenAmount);	///开仓冻结金额
			GETDB_INT(OpenVolume);	///开仓量
			GETDB_INT(CloseVolume);	///平仓量
			GETDB_DOUBLE(OpenAmount);	///开仓金额
			GETDB_DOUBLE(CloseAmount);	///平仓金额
			GETDB_DOUBLE(PositionCost);	///持仓成本
			GETDB_DOUBLE(PreMargin);	///上次占用的保证金
			GETDB_DOUBLE(UseMargin);	///占用的保证金
			GETDB_DOUBLE(FrozenMargin);	///冻结的保证金
			GETDB_DOUBLE(FrozenCash);	///冻结的资金
			GETDB_DOUBLE(FrozenCommission);	///冻结的手续费
			GETDB_DOUBLE(CashIn);	///资金差额
			GETDB_DOUBLE(Commission);	///手续费
			GETDB_DOUBLE(CloseProfit);	///平仓盈亏
			GETDB_DOUBLE(PositionProfit);	///持仓盈亏
			GETDB_DOUBLE(PreSettlementPrice);	///上次结算价
			GETDB_DOUBLE(SettlementPrice);	///本次结算价
			GETDB_STR(TradingDay);	///交易日
			GETDB_INT(SettlementID);	///结算编号
			GETDB_DOUBLE(OpenCost);	///开仓成本
			GETDB_DOUBLE(ExchangeMargin);	///交易所保证金
			GETDB_INT(CombPosition);	///组合成交形成的持仓
			GETDB_INT(CombLongFrozen);	///组合多头冻结
			GETDB_INT(CombShortFrozen);	///组合空头冻结
			GETDB_DOUBLE(CloseProfitByDate);	///逐日盯市平仓盈亏
			GETDB_DOUBLE(CloseProfitByTrade);	///逐笔对冲平仓盈亏
			GETDB_INT(TodayPosition);	///今日持仓
			GETDB_DOUBLE(MarginRateByMoney);	///保证金率
			GETDB_DOUBLE(MarginRateByVolume);	///保证金率(按手数)

			vec.push_back(info);
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

bool DBAccess::SaveRiskEvent( std::string strTime, std::vector<RiskEvent>& vRiskEvent )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	int* RiskEventID = new int[nMaxInsertRow];
	int* RiskEventSubID = new int[nMaxInsertRow];
	INT64* EventTime = new INT64[nMaxInsertRow];
	int* TradeAccountID = new int[nMaxInsertRow];
	int* RiskIndModuleID = new int[nMaxInsertRow];
	int* RiskIndicatorID = new int[nMaxInsertRow];
	int* RiskLevelID = new int[nMaxInsertRow];
	double* IndicatorValue = new double[nMaxInsertRow];
	int* MsgSendStatus = new int[nMaxInsertRow];
	int* IsValid = new int[nMaxInsertRow];
	InstrumentIDType* InstrumentID = new InstrumentIDType[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from RISKEVENT t where \
						  to_char(to_date('1970-01-01','yyyy-MM-dd')+t.eventtime/86400,'YYYYMMDD') = '%s'",
			strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into RISKEVENT values(:v1,:v2,:v3,:v4,:v5,:v6, \
						  :v7,:v8,:v9,:v10,:v11)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vRiskEvent.size() / nMaxInsertRow;
		if ( vRiskEvent.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_double[nMaxInsertRow];
			ub2 len_int64[nMaxInsertRow];
			ub2 len_InstrumentID[nMaxInsertRow];

			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vRiskEvent.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				RiskEvent info = vRiskEvent[j];
				len_int[nRecordNum] = sizeof(int);
				len_int64[nRecordNum] = sizeof(INT64);
				len_double[nRecordNum] = sizeof(double);

				RiskEventID[nRecordNum] = info.nRiskEventID;
				RiskEventSubID[nRecordNum] = info.nRiskEventSubID;
				EventTime[nRecordNum] = info.lEventTime;
				TradeAccountID[nRecordNum] = info.nTradeAccountID;
				RiskIndModuleID[nRecordNum] = info.nRiskIndModuleID;
				RiskIndicatorID[nRecordNum] = info.nRiskIndicatorID;
				RiskLevelID[nRecordNum] = info.nRiskLevelID;
				IndicatorValue[nRecordNum] = info.dblIndicatorValue;
				MsgSendStatus[nRecordNum] = info.nMsgSendStatus;
				IsValid[nRecordNum] = info.nIsValid;
				strcpy(InstrumentID[nRecordNum], info.InstrumentID);
				len_InstrumentID[nRecordNum] = strlen(InstrumentID[nRecordNum]) + 1;

				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, RiskEventID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(2, RiskEventSubID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(3, TradeAccountID, OCCIINT, sizeof(int), len_int);
			m_pStmt->setDataBuffer(4, EventTime, OCCIINT, sizeof(INT64), len_int64); 
			m_pStmt->setDataBuffer(5, RiskIndModuleID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(6, RiskIndicatorID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(7, RiskLevelID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(8, IndicatorValue, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(9, MsgSendStatus, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(10, IsValid, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(11, InstrumentID, OCCI_SQLT_STR, sizeof(InstrumentIDType), len_InstrumentID); 

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] RiskEventID;
	delete [] RiskEventSubID;
	delete [] EventTime;
	delete [] TradeAccountID;
	delete [] RiskIndModuleID;
	delete [] RiskIndicatorID;
	delete [] RiskLevelID;
	delete [] IndicatorValue;
	delete [] MsgSendStatus;
	delete [] IsValid;
	delete [] InstrumentID;

	return bRet;
}

bool DBAccess::SaveRiskEventHandling( std::string strTime, std::vector<RiskEventHandling>& vHandling )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	int* RiskEventHandlingID = new int[nMaxInsertRow];
	int* RiskEventID = new int[nMaxInsertRow];
	int* RiskEventSubID = new int[nMaxInsertRow];
	INT64* HandlingTime = new INT64[nMaxInsertRow];
	USERNAMESTRING* Handler = new USERNAMESTRING[nMaxInsertRow];
	USERNAMESTRING* Accepter = new USERNAMESTRING[nMaxInsertRow];
	int* ActionType = new int[nMaxInsertRow];
	int* ProcessStatus = new int[nMaxInsertRow];
	CommentType* EventComment = new CommentType[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from RISKEVENTHANDLING t where \
						  to_char(to_date('1970-01-01','yyyy-MM-dd')+t.HandlingTime/86400,'YYYYMMDD') = '%s'",
						  strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into RISKEVENTHANDLING values(\
						  :v1,:v2,:v3,:v4,:v5,:v6,:v7,:v8,:v9)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vHandling.size() / nMaxInsertRow;
		if ( vHandling.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_Handler[nMaxInsertRow];
			ub2 len_Accepter[nMaxInsertRow];
			ub2 len_EventComment[nMaxInsertRow];
			ub2 len_int64[nMaxInsertRow];

			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vHandling.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				RiskEventHandling info = vHandling[j];
				len_int[nRecordNum] = sizeof(int);
				len_int64[nRecordNum] = sizeof(INT64);

				RiskEventHandlingID[nRecordNum] = info.nRiskEventHandlingID;
				RiskEventID[nRecordNum] = info.nRiskEventID;
				RiskEventSubID[nRecordNum] = info.nRiskEventSubID;
				HandlingTime[nRecordNum] = info.lHandlingTime;
				strcpy(Handler[nRecordNum], info.szHandler);
				len_Handler[nRecordNum] = strlen(Handler[nRecordNum]) + 1;
				strcpy(Accepter[nRecordNum], info.szAccepter);
				len_Accepter[nRecordNum] = strlen(Accepter[nRecordNum]) + 1;
				ActionType[nRecordNum] = info.nAction;
				ProcessStatus[nRecordNum] = info.nProcessStatus;
				strcpy(EventComment[nRecordNum], info.szContent);
				len_EventComment[nRecordNum] = strlen(EventComment[nRecordNum]) + 1;

				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, RiskEventHandlingID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(2, RiskEventID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(3, RiskEventSubID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(4, HandlingTime, OCCIINT, sizeof(INT64), len_int64); 
			m_pStmt->setDataBuffer(5, Handler, OCCI_SQLT_STR, sizeof(USERNAMESTRING), len_Handler); 
			m_pStmt->setDataBuffer(6, Accepter, OCCI_SQLT_STR, sizeof(USERNAMESTRING), len_Accepter); 
			m_pStmt->setDataBuffer(7, ActionType, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(8, ProcessStatus, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(9, EventComment, OCCI_SQLT_STR, sizeof(CommentType), len_EventComment); 

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] RiskEventHandlingID;
	delete [] RiskEventID;
	delete [] RiskEventSubID;
	delete [] HandlingTime;
	delete [] Handler;
	delete [] Accepter;
	delete [] ActionType;
	delete [] ProcessStatus;
	delete [] EventComment;

	return bRet;
}
/*
nQueryType:0 按部门查询； 1 按理财产品查询 ； 2 按某一账户查询
*/
bool DBAccess::QueryData( std::string strTime, std::vector<SGatherData>& vGatherData )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));
	try
	{
		sprintf(szBuffer, "select t1.id,t1.idtype,t1.initprofit,t1.curprofit,t1.offsetprofit,\
						  t1.annualprofitrate,t1.marginrate,to_char(t1.updatedate, 'YYYY-MM-DD'), t1.rjvalue, t1.daycount\
						  from GATHERDATA t1 where to_char(t1.updatedate, 'YYYY-MM-DD') = \
						  (select max(to_char(t.updatedate, 'YYYY-MM-DD')) from GATHERDATA t \
						  where to_char(t.updatedate, 'YYYY-MM-DD') != '%s')",
						  strTime.c_str());
		m_pStmt = m_pCon->createStatement( szBuffer );
		m_pRes = m_pStmt->executeQuery();
		while ( m_pRes->next())
		{
			SGatherData data;
			memset(&data, 0, sizeof(data));

			data.nID = m_pRes->getInt(1);
			data.nIDType = m_pRes->getInt(2);
			data.SumPreBalance = m_pRes->getDouble(3);
			data.SumBalance = m_pRes->getDouble(4);
			data.OffBalance = m_pRes->getDouble(5);
			data.YearEarnRate = m_pRes->getDouble(6);
			data.MarginRate = m_pRes->getDouble(7);
			std::string strValue = m_pRes->getString(8);
			strcpy(data.szUpdateDate, strValue.c_str());
			data.dbValue = m_pRes->getDouble(9);
			data.nDayCount = m_pRes->getInt(10);
			vGatherData.push_back(data);
		}

		m_pStmt->closeResultSet(m_pRes);
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

bool DBAccess::QueryData( const char* pSql, std::vector<NetFundParam>& vec )
{
	CGuard guard(&m_mutex);
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
			NetFundParam info;
			memset(&info, 0, sizeof(info));

			info.nTradeAccountID = m_pRes->getInt(1);
			info.dInnerVolumn = m_pRes->getDouble(2);
			info.dOuterVolumn = m_pRes->getDouble(3);
			info.dOuterNetAsset = m_pRes->getDouble(4);
			info.nInnerNetAssetOption = m_pRes->getInt(5);

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

bool DBAccess::QueryData( const char* pSql, std::vector<NetFundCalcResult>& vec )
{
	CGuard guard(&m_mutex);
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
			NetFundCalcResult info;
			memset(&info, 0, sizeof(info));

			info.nTradeAccountID = m_pRes->getInt(1);
			info.dInnerVolumn = m_pRes->getDouble(2);
			info.dOuterVolumn = m_pRes->getDouble(3);
			info.dInnerNetAsset = m_pRes->getDouble(4);
			info.dOuterNetAsset = m_pRes->getDouble(5);
			info.dInnerPerNet = m_pRes->getDouble(6);
			info.dOuterPerNet = m_pRes->getDouble(7);
			info.dTotalNetAsset = m_pRes->getDouble(8);
			string strValue = m_pRes->getString(9);
			strcpy(info.szUpdateDate, strValue.c_str());

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

bool DBAccess::QueryData( const char* pSql, std::vector<ForceCloseParam>& vec )
{
	CGuard guard(&m_mutex);
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
			ForceCloseParam info;
			memset(&info, 0, sizeof(info));

			info.nTradeAccountID = m_pRes->getInt(1);
			info.nForceCloseInd = m_pRes->getInt(2);
			info.fThreshold = m_pRes->getFloat(3);
			info.nCloseStrategy = m_pRes->getInt(4);
			info.nForceClosePriceOption = m_pRes->getInt(5);

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

bool DBAccess::SaveNetFundCalcResult( std::string strTime, std::vector<NetFundCalcResult>& vResult )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	int* TradeAccountID = new int[nMaxInsertRow];
	double* InnerVolumn = new double[nMaxInsertRow];
	double* OuterVolumn = new double[nMaxInsertRow];
	double* InnerNetAsset = new double[nMaxInsertRow];
	double* OuterNetAsset = new double[nMaxInsertRow];
	double* InnerPerNet = new double[nMaxInsertRow];
	double* OuterPerNet = new double[nMaxInsertRow];
	double* TotalNetAsset = new double[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from FUNDNETCALCRESULT t where \
						  to_char(t.updatedate,'YYYY-MM-DD') = '%s'",
						  strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into FUNDNETCALCRESULT values(:v1,:v2,:v3,:v4,:v5,:v6, \
						  :v7,:v8,sysdate)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vResult.size() / nMaxInsertRow;
		if ( vResult.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_double[nMaxInsertRow];

			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vResult.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				NetFundCalcResult info = vResult[j];
				len_int[nRecordNum] = sizeof(int);
				len_double[nRecordNum] = sizeof(double);

				TradeAccountID[nRecordNum] = info.nTradeAccountID;
				InnerVolumn[nRecordNum] = info.dInnerVolumn;
				OuterVolumn[nRecordNum] = info.dOuterVolumn;
				InnerNetAsset[nRecordNum] = info.dInnerNetAsset;
				OuterNetAsset[nRecordNum] = info.dOuterNetAsset;
				InnerPerNet[nRecordNum] = info.dInnerPerNet;
				OuterPerNet[nRecordNum] = info.dOuterPerNet;
				TotalNetAsset[nRecordNum] = info.dTotalNetAsset;

				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, TradeAccountID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(2, InnerVolumn, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(3, OuterVolumn, OCCIFLOAT, sizeof(double), len_double);
			m_pStmt->setDataBuffer(4, InnerNetAsset, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(5, OuterNetAsset, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(6, InnerPerNet, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(7, OuterPerNet, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(8, TotalNetAsset, OCCIFLOAT, sizeof(double), len_double); 

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] TradeAccountID;
	delete [] InnerVolumn;
	delete [] OuterVolumn;
	delete [] InnerNetAsset;
	delete [] OuterNetAsset;
	delete [] InnerPerNet;
	delete [] OuterPerNet;
	delete [] TotalNetAsset;

	return bRet;
}

bool DBAccess::SaveGatherData( std::string strTime, std::vector<SGatherData>& vGatherData )
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	int* ID = new int[nMaxInsertRow];
	int* IDType = new int[nMaxInsertRow];
	double* InitProfit = new double[nMaxInsertRow];
	double* CurProfit = new double[nMaxInsertRow];
	double* OffsetProfit = new double[nMaxInsertRow];
	double* AnnualProfitRate = new double[nMaxInsertRow];
	double* MarginRate = new double[nMaxInsertRow];
	double* RJValue = new double[nMaxInsertRow];
	int* DayCount = new int[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from GATHERDATA t where \
						  to_char(t.updatedate,'YYYY-MM-DD') = '%s'",
						  strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into GATHERDATA values(:v1,:v2,:v3,:v4,:v5,:v6,:v7,sysdate,:v8,:v9)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vGatherData.size() / nMaxInsertRow;
		if ( vGatherData.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];
			ub2 len_double[nMaxInsertRow];

			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vGatherData.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				SGatherData info = vGatherData[j];
				len_int[nRecordNum] = sizeof(int);
				len_double[nRecordNum] = sizeof(double);

				ID[nRecordNum] = info.nID;
				IDType[nRecordNum] = info.nIDType;
				InitProfit[nRecordNum] = info.SumPreBalance;
				CurProfit[nRecordNum] = info.SumBalance;
				OffsetProfit[nRecordNum] = info.OffBalance;
				AnnualProfitRate[nRecordNum] = info.YearEarnRate;
				MarginRate[nRecordNum] = info.MarginRate;

				RJValue[nRecordNum]   = info.dbValue;
				DayCount[nRecordNum]  = info.nDayCount;
				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, ID, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(2, IDType, OCCIINT, sizeof(int), len_int); 
			m_pStmt->setDataBuffer(3, InitProfit, OCCIFLOAT, sizeof(double), len_double);
			m_pStmt->setDataBuffer(4, CurProfit, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(5, OffsetProfit, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(6, AnnualProfitRate, OCCIFLOAT, sizeof(double), len_double); 
			m_pStmt->setDataBuffer(7, MarginRate, OCCIFLOAT, sizeof(double), len_double); 

			m_pStmt->setDataBuffer(8, RJValue, OCCIFLOAT, sizeof(double), len_double);
			m_pStmt->setDataBuffer(9, DayCount, OCCIINT, sizeof(int), len_int); 

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] ID;
	delete [] IDType;
	delete [] InitProfit;
	delete [] CurProfit;
	delete [] OffsetProfit;
	delete [] AnnualProfitRate;
	delete [] MarginRate;
	delete [] RJValue;
	delete [] DayCount;

	return bRet;
}
bool DBAccess::QueryData( const char* pSql, std::vector<SGather>& vGather )
{
	CGuard guard(&m_mutex);
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
			SGather info;
			memset(&info, 0, sizeof(info));		
			info.nID			= m_pRes->getInt(1);
			info.nIDType		= m_pRes->getInt(2);
			info.SumPreBalance	= m_pRes->getDouble(3);
			info.SumBalance		= m_pRes->getDouble(4);
			info.OffBalance     = m_pRes->getDouble(5);
			info.YearEarnRate   = m_pRes->getDouble(6);
			info.MarginRate		= m_pRes->getDouble(7);

			vGather.push_back(info);
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}
	catch(oracle::occi::SQLException &e)
	{
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

bool DBAccess::QueryData( const char* pSql,  std::multimap<std::string, SProfit>& sProfit)
{
	CGuard guard(&m_mutex);
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
			SProfit info;
			memset(&info, 0, sizeof(info));		
			info.strTradeAccount = m_pRes->getString(1);
			info.dbProfit        = m_pRes->getDouble(2);
			info.dbBalance		 = m_pRes->getDouble(3);
			info.dbOffsetProfit  = m_pRes->getDouble(4);
			info.strDate		 = m_pRes->getString(5);		

			sProfit.insert(make_pair(info.strDate, info));
		}

		m_pStmt->closeResultSet(m_pRes);
		m_pCon->terminateStatement(m_pStmt);
	}
	catch(oracle::occi::SQLException &e)
	{
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

bool DBAccess::SaveSeqDepositField(std::string strTime, std::vector<SeqDepositField>& vecSeqDepositField)
{
	CGuard guard(&m_mutex);
	if ( !IsConnected() && !Conncect())
	{
		return false;
	}

	char szBuffer[MAX_SQL_LENGTH];
	memset(szBuffer, 0, sizeof(szBuffer));

	int* UniqSequenceNo					= new int[nMaxInsertRow];
	TFundIOTypeType* Type				= new TFundIOTypeType[nMaxInsertRow];	
	TAvailabilityFlagType* Flag			= new TAvailabilityFlagType[nMaxInsertRow];
	TFundDirectionType* Direction		= new TFundDirectionType[nMaxInsertRow];
	TDateType* 	TradingDay				= new TDateType[nMaxInsertRow];
	TDepositSeqNoType* SequenceNo		= new TDepositSeqNoType[nMaxInsertRow];
	TTimeType* 		Time				= new TTimeType[nMaxInsertRow];
	TBrokerIDType* 	BrokerID			= new TBrokerIDType[nMaxInsertRow];
	TInvestorIDType* InvestorID			= new TInvestorIDType[nMaxInsertRow];
	TMoneyType* Amount					= new TMoneyType[nMaxInsertRow];

	bool bRet = true;
	try
	{
		m_pStmt = m_pCon->createStatement();
		//先删除今天已存储的记录
		sprintf(szBuffer, "delete from SEQDEPOSITFIELD t where \
						  TRADINGDAY = '%s'",
						  strTime.c_str());
		m_pStmt->execute( szBuffer );

		//批量存储，每次最多存储10000条
		memset(szBuffer, 0, sizeof(szBuffer));
		sprintf(szBuffer, "insert into SEQDEPOSITFIELD values(:v1,:v2,:v3,:v4,:v5,:v6,:v7,:v8,:v9,:v10)");
		m_pStmt->setSQL(szBuffer);
		int nCount = vecSeqDepositField.size() / nMaxInsertRow;
		if ( vecSeqDepositField.size() % nMaxInsertRow != 0 )
		{
			nCount++;
		}

		for ( int i = 0; i < nCount; i++ )
		{
			ub2 len_int[nMaxInsertRow];	
			ub2 len_char[nMaxInsertRow];
			ub2 len_Type[nMaxInsertRow];
			ub2 len_Flag[nMaxInsertRow];
			ub2 len_Direction[nMaxInsertRow];
			ub2 len_TradingDay[nMaxInsertRow];
			ub2 len_SequenceNo[nMaxInsertRow];
			ub2 len_Time[nMaxInsertRow];
			ub2 len_BrokerID[nMaxInsertRow];
			ub2 len_InvestorID[nMaxInsertRow];
			ub2 len_Amount[nMaxInsertRow];

			int nRecordNum = 0;
			for ( int j = i*nMaxInsertRow; j < static_cast<int>(vecSeqDepositField.size()) && nRecordNum < nMaxInsertRow; j++)
			{
				SeqDepositField& info = vecSeqDepositField[j];
				UniqSequenceNo[nRecordNum]		= info.UniqSequenceNo;
				len_int[nRecordNum]				= sizeof(int);
				len_char[nRecordNum]			= sizeof(char);

				Type[nRecordNum] = info.Type;
				len_Type[nRecordNum]			= 1;

				Flag[nRecordNum] = info.Flag;
				len_Flag[nRecordNum]			= 1;

				Direction[nRecordNum] = info.Direction;
				len_Direction[nRecordNum]       = 1;		

				strcpy(TradingDay[nRecordNum], info.TradingDay);
				len_TradingDay[nRecordNum]      = strlen(TradingDay[nRecordNum]) +1;

				strcpy(SequenceNo[nRecordNum], info.SequenceNo);
				len_SequenceNo[nRecordNum]      = strlen(SequenceNo[nRecordNum]) +1;

				strcpy(Time[nRecordNum], info.Time);
				len_Time[nRecordNum]			= strlen(Time[nRecordNum]) +1;

				strcpy(BrokerID[nRecordNum], info.BrokerID);
				len_BrokerID[nRecordNum]		= strlen(BrokerID[nRecordNum]) +1;

				strcpy(InvestorID[nRecordNum], info.InvestorID);
				len_InvestorID[nRecordNum]      = strlen(InvestorID[nRecordNum]) +1;

				Amount[nRecordNum]  =  info.Amount;
				len_Amount[nRecordNum] = sizeof(double);
				nRecordNum++;
			}

			m_pStmt->setDataBuffer(1, UniqSequenceNo,	OCCIINT,	   sizeof(int), len_int); 
			m_pStmt->setDataBuffer(2, Type,				OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(3, Flag,				OCCI_SQLT_CHR, sizeof(char), len_char); 			
			m_pStmt->setDataBuffer(4, Direction,		OCCI_SQLT_CHR, sizeof(char), len_char); 
			m_pStmt->setDataBuffer(5, TradingDay,		OCCI_SQLT_STR, sizeof(TDateType), len_TradingDay); 
			m_pStmt->setDataBuffer(6, SequenceNo,		OCCI_SQLT_STR, sizeof(TDepositSeqNoType), len_SequenceNo); 
			m_pStmt->setDataBuffer(7, Time,				OCCI_SQLT_STR, sizeof(TTimeType), len_Time); 
			m_pStmt->setDataBuffer(8, BrokerID,			OCCI_SQLT_STR, sizeof(TBrokerIDType), len_BrokerID); 
			m_pStmt->setDataBuffer(9, InvestorID,		OCCI_SQLT_STR, sizeof(TInvestorIDType), len_InvestorID); 
			m_pStmt->setDataBuffer(10, Amount,			OCCIFLOAT,	   sizeof(double), len_Amount); 

			m_pStmt->executeArrayUpdate(nRecordNum); 
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
		bRet = false;
	}

	delete [] UniqSequenceNo;
	delete [] Type;
	delete [] Flag;
	delete [] Direction;
	delete [] TradingDay;
	delete [] SequenceNo;
	delete [] Time;
	delete [] BrokerID;
	delete [] InvestorID;
	delete [] Amount;

	return bRet;


}