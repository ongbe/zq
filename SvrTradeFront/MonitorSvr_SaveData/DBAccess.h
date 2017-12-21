#pragma once
#include <comutil.h>
#include "occi.h"
#include "easymutex.h"
#pragma comment(lib, "comsuppw.lib") 

#define MAX_SQL_LENGTH			5000	//SQL语句最大长度
#define MAX_USABLE_SQL_LENGTH	4999
class CWriteLog;

class DBAccess
{
public:
	DBAccess();
	~DBAccess(void);

	void InitDB(const std::string& strUserName, const std::string& strPwd, const std::string& strDBName);

	std::string GetLastErrorString();
	int GetLastErrorCode();

	static int NotifyFn(oracle::occi::Environment *env, oracle::occi::Connection *conn, void *ctx,
		oracle::occi::Connection::FailOverType foType, 
		oracle::occi::Connection::FailOverEventType foEvent);

public:
	bool ExcuteUpdate( const char* pSql, int& nNum );
	bool Excute( const char* pSql );
	bool ExcuteSelect( const char* pSql, int& nRecordNum );
	bool InsertAndReturnID( const char* pSql, const char* pIDSql, int& nPKID );
	
	bool QueryData(const char* pSql, std::vector<std::vector<_variant_t>>& vec/*, int& nErrorCode */);


private:
	bool IsConnected();
	bool Conncect();
	void DisConnected();
	void RollBack();
	void WriteLog(const std::string& strSql, const std::string& strError);


private:
	template<class T>
	class CSafeArrayPtr
	{
	public:
		CSafeArrayPtr(T* p):m_ptr(p){}
		~CSafeArrayPtr(){if(m_ptr) delete[] m_ptr;}
	private:
		T* m_ptr;
	};
private:
	oracle::occi::Environment* m_pEnvironment;
	oracle::occi::Connection * m_pCon;
	oracle::occi::Statement* m_pStmt;
	oracle::occi::ResultSet* m_pRes;

	std::string m_strLastError;
	int m_nErrorID;

	std::string m_strUserName;
	std::string m_strPwd;
	std::string m_strDBName;
	CWriteLog* m_pWriteLog;

	Ceasymutex m_mutex;
};
