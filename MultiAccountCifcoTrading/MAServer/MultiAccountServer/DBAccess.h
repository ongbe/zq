#pragma once

#include <vector>
#include <set>
#include "occi.h"
#include "MultiAccountStruct.h"

class CWriteLog;

class DBAccess
{
public:
	DBAccess();
	~DBAccess(void);

	void Init(const std::string& strUserName, const std::string& strPwd, const std::string& strDBName);

	bool Conncect();

	bool IsConnected();

	void DisConnected();

	std::string GetLastErrorString();
	int GetLastErrorCode();

public:
	bool ExcuteUpdate( const char* pSql, int& nNum );
	bool Excute( const char* pSql );
	bool ExcuteSelect( const char* pSql, int& nRecordNum );
	bool InsertAndReturnID( const char* pSql, const char* pIDSql, int& nPKID );
	bool ExcuteLogon( const char* pUserName, const char* pPwd, 
		int nAccountType, LoginRsp& rsp);

	bool QueryData( const char* pSql, std::vector<AccountInfo>& vec );
	bool QueryData( const char* pSql, std::vector<AccountInfoEx>& vec );
	bool QueryData( const char* pSql, std::vector<IDCardInfo>& vec );
	bool QueryData( const char* pSql, std::vector<ServerGroup>& vec );
	bool QueryData( const char* pSql, std::vector<BrokerInfo>& vec );
	bool QueryData( const char* pSql, std::vector<TradeAccount>& vec );
	bool QueryData( const char* pSql, std::vector<PermissionModule>& vec );
	bool QueryData( const char* pSql, std::vector<Permission>& vec );
	bool QueryData( const char* pSql, std::vector<PermissionSimple>& vec );
	bool QueryData( const char* pSql, std::vector<ServerAddr>& vec );
	bool QueryData( const char* pSql, std::vector<MessageInfo>& vec );
	bool QueryData( const char* pSql, std::vector<TargetAccount>& vec );

	bool AddPermissionData( const Permission* pData, int& nPKID);
	bool UpdatePermissionData(const Permission* pData, int& nNum);

	bool DelAccountCaseCade( std::set<int>& setData);

	bool SaveTradeAccount( int nOwnerAccount, std::vector<TradeAccount> vAccount);

	bool AddMessage( const MessageInfo& msgInfo, const std::vector<TargetAccount> vAccount, 
		std::vector<TargetAccount>& vForeAccount, int& nPKID);

	bool SaveWaitForSendMessage( int nMessageID, std::vector<TargetAccount> vAccount );

	bool QuerySharedBrokerInfo( int nForeAccountID, std::vector<BrokerInfo>& vec);

private:
	void RollBack();
	void WriteLog(const std::string& strSql, const std::string& strError);

private:
	oracle::occi::Environment* m_pEnvironment;
	oracle::occi::Connection * m_pCon;
	oracle::occi::Statement* m_pStmt;
	oracle::occi::ResultSet* m_pRes;
	bool m_bConn;
	std::string m_strLastError;
	int m_nErrorID;

	std::string m_strUserName;
	std::string m_strPwd;
	std::string m_strDBName;
	CWriteLog* m_pWriteLog;
};
