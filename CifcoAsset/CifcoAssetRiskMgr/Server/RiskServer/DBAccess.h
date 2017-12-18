#pragma once

#include "occi.h"
#include "Tools\easymutex.h"

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
	bool ExcuteLogon( const char* pUserName, const char* pPwd, 
		int nAccountType, LoginRsp& rsp);

	bool QueryData( const char* pSql, std::vector<RiskMgmtUserInfo>& vec );
	bool QueryData( const char* pSql, std::vector<TrustTradeAccount>& vec );
	bool QueryData( const char* pSql, std::vector<TraderInfo>& vec );
	bool QueryData( const char* pSql, std::vector<AssetMgmtOrganization>& vec );
	bool QueryData( const char* pSql, std::vector<FinancialProduct>& vec );
	bool QueryData( const char* pSql, std::vector<AssetOrgProRelation>& vec );
	bool QueryData( const char* pSql, std::vector<RiskIndicatorModule>& vec );
	bool QueryData( const char* pSql, std::vector<PermissionName>& vec );
	bool QueryData( const char* pSql, std::vector<PermissionNameEx>& vec );
	bool QueryData( const char* pSql, std::vector<PermissionNode>& vec );
	bool QueryData( const char* pSql, std::vector<PermissionAction>& vec );
	bool QueryData( const char* pSql, std::vector<PermissionValue>& vec );
	bool QueryData( const char* pSql, std::vector<RiskIndicator>& vec );
	bool QueryData( const char* pSql, std::vector<RiskWarning>& vec );
	bool QueryData( const char* pSql, std::vector<MaxRiskLevel>& vec );
	bool QueryData( const char* pSql, std::vector<EventMessageTemplate>& vec );
	bool QueryData( const char* pSql, std::vector<ProductRiskWarning>& vec );
	bool QueryData( const char* pSql, std::vector<RiskEvent>& vec );
	bool QueryData( const char* pSql, std::vector<RiskEventHandling>& vec );
	bool QueryData( const char* pSql, std::vector<RiskEventHandlingEx>& vec );
	bool QueryData( const char* pSql, std::vector<int>& vec );
	bool QueryData( const char* pSql, std::vector<MessageInfo>& vec );
	bool QueryData( const char* pSql, std::vector<MsgSendStatus>& vec );

	bool QueryData( const char* pSql, std::vector<NetFundParam>& vec );
	bool QueryData( const char* pSql, std::vector<NetFundCalcResult>& vec );
	bool QueryData( const char* pSql, std::vector<ForceCloseParam>& vec );

	bool QueryData( const char* pSql, std::multimap<std::string, SProfit>& sProfit );
	bool QueryData( const char* pSql, std::vector<SGather>& vGather );
	bool QueryData( std::string strTime, std::vector<SGatherData>& vGatherData );
	bool SaveGatherData( std::string strTime, std::vector<SGatherData>& vGatherData );

	bool SavePermissionValue(int nPermissionID, std::vector<PermissionValue>& vec);
	bool SaveFinOrgRelation(int nFinID, std::vector<int>& vec);
	bool SaveRiskWarning(int nModuleID, std::vector<IndLevelWarning>& vec);
	bool SaveMsgTemplate(int nIndicatorID, std::vector<EventMessageTemplate>& vec);
	bool SaveProductRiskWarning(const ProducrRiskWarningKey& key, std::vector<IndLevelWarning>& vec);

	bool AddMessage( const MessageInfo& msgInfo, const std::vector<TargetAccount> vAccount, int& nPKID);

	bool SaveMessageSendStatus( std::vector<MsgSendStatus> vMsgStatus );

	//交易数据存储、查询
	bool SavePreRiskAccount( std::string strTime, std::vector<SeqPreRiskAccountField>& vPreRiskAccount);
	bool SaveRiskSyncAccount( std::string strTime, std::vector<RiskSyncAccountField>& vRiskSyncAccount);

	bool QueryRiskAccount( const char* pSql, std::vector<RiskAllAccountField>& vec);

	bool SaveTradeRecord( std::string strTime, std::vector<SequencialTradeField>& vTrade);
	bool QueryTradeRecord(const char* pSql, 
		std::vector<SequencialTradeField>& vec);

	bool SaveOrderRecord( std::string strTime, std::vector<SequencialOrderField>& vOrder);
	bool QueryOrderRecord(const char* pSql, 
		std::vector<SequencialOrderField>& vec);

	bool SavePositionRecord( std::string strTime, std::vector<SequencialPositionField>& vOrder);
	bool QueryPositionRecord(const char* pSql, 
		std::vector<SequencialPositionField>& vec);

	//批量存储风险事件
	bool SaveRiskEvent( std::string strTime, std::vector<RiskEvent>& vRiskEvent);
	bool SaveRiskEventHandling(std::string strTime, std::vector<RiskEventHandling>& vHandling);

	bool SaveNetFundCalcResult(std::string strTime, std::vector<NetFundCalcResult>& vResult);

	bool SaveSeqDepositField(std::string strTime, std::vector<SeqDepositField>& vecSeqDepositField);
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
