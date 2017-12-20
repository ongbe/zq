#pragma once

#include "Interface_SvrDBOpr.h"

#include <comutil.h>
#include <vector>
#include "FileOpr.h"
#pragma comment(lib, "comsuppw.lib") 

#include "occi.h"

class CWriteLog;
#if 0
#define RISKDATA_LOG(fmt, ...)  ;
#else
#define RISKDATA_LOG(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","RiskDataTemporaryStorage", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);
#endif
struct ColumeData
{
	oracle::occi::Type eColumeType;
	int nColumeMaxLen;
	int nOffset;
};

struct ColumeDataEx
{
	oracle::occi::Type eColumeType;
	int nColumeMaxLen;
	int nOffset;
	std::string msColumeName;
};

struct DeleteStruct
{
	InstrumentIDType InstrumentID;
	TThostFtdcExchangeIDType ExchangeID;
};

class CSvrDBOprImpl : public CInterface_SvrDBOpr
{
public:
	static CSvrDBOprImpl& getObj(void);
	//static void deleteObj(void);

	void InitDB(const std::string& strUserName, const std::string& strPwd, const std::string& strDBName);
	bool Conncect();
	static int NotifyFn(oracle::occi::Environment *env, oracle::occi::Connection *conn, void *ctx,
		oracle::occi::Connection::FailOverType foType, 
		oracle::occi::Connection::FailOverEventType foEvent);

public:
	//执行更新、删除SQL语句，返回影响的记录条数
	bool ExcuteUpdate( const char* pSql, int& nNum, int& nErrorCode );
	//执行任意SQL语句
	bool Excute( const char* pSql, int& nErrorCode );
	//执行查询SQL语句，返回查询的记录条数
	bool ExcuteSelect( const char* pSql, int& nRecordNum, int& nErrorCode );
	//插入一条数据，并返回插入记录的ID
	bool InsertAndReturnID( const char* pSql, const char* pIDSql, int& nPKID, int& nErrorCode );
	//执行查询SQL语句，并返回结果集
	bool QueryData(const char* pSql, std::vector<std::vector<_variant_t>>& vec, int& nErrorCode );

	//保存权限
	bool SaveRolePrivilegeRelation(int nRoleID, std::vector<int>& vecPrivilegeID, int& nErrorCode);

	//保存用户角色
	bool SaveUserRole( int nProductID, std::vector<int>& vTraderID, int& nErrorCode );

    //保存理财产品和交易员关系
    bool SaveProductTraderRelation(int nRelationType, int nID, std::vector<int>& vID, int& nErrorCode );

    //保存交易策略和组织机构关系
    bool SaveStrategyOrganizationRelation(int nStrategyID, std::vector<int>& vOrgID, int& nErrorCode );

    //保存用户和组织机构关系
    bool SaveUserOrganizationRelation( int nOrgID, int nUserType, int nUserID, int nRelationType, int& nErrorCode);
    bool SaveUserOrganizationRelation( int nOrgID, int nUserType, std::vector<int>& vInID, std::vector<int>& vOutID, int nRelationType, int& nErrorCode);
    bool SaveUserOrganizationRelation( int nRiskID, int nAttachOrg, std::vector<int>& vMonitorOrg, int& nErrorCode);


	//存结算价
	bool SaveSettlementPrice2DB(std::string strTime, std::vector<SettlementPriceField>& vSettlementPrice ,int& nErrorCode);

	//保存合约
	bool SaveInstruments(
		const std::string& strTime, 
		const std::vector<PlatformStru_InstrumentInfo>& nInstruemnts ,
		int& nErrorCode ,
		const std::string& nsTableName);


	//保存费率
	virtual bool SaveAccountCommissionRate(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_InstrumentCommissionRate>& nCommission ,
		int& nErrorCode ) ;


	//保证金率
	virtual bool SaveAccountMarginRate(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_InstrumentMarginRate>& nMargins ,
		int& nErrorCode ) ;



	//报单
	virtual bool SaveUserOrderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_OrderInfo>& nMargins ,
		int& nErrorCode ) ;
	//成交
	virtual bool SaveUserTraderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_TradeInfo>& nMargins ,
		int& nErrorCode ) ;

	//持仓
	virtual bool SaveUserPositionInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_Position>& nPositions ,
		int& nErrorCode );

	//持仓明细
	virtual bool SaveUserPositionDetailInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_PositionDetail>& nPositionDetails ,
		int& nErrorCode );

	//资金
	virtual bool SaveUserFundInfos(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsUserName,
		const PlatformStru_TradingAccountInfo& nFundInfos,
		int& nErrorCode );



	//报单
	virtual bool SaveAccountOrderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_OrderInfo>& nMargins ,
		int& nErrorCode ) ;
	//成交
	virtual bool SaveAccountTraderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_TradeInfo>& nMargins ,
		int& nErrorCode ) ;

	//持仓
	virtual bool SaveAccountPositionInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_Position>& nPositions ,
		int& nErrorCode );

	//持仓明细
	virtual bool SaveAccountPositionDetailInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_PositionDetail>& nPositionDetails ,
		int& nErrorCode );

	//资金
	virtual bool SaveAccountFundInfos(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const PlatformStru_TradingAccountInfo& nFundInfos,
		int& nErrorCode );


	//写出入金记录
	virtual bool SaveFundInOut(const sFundInOut & s);

	//写交易日
	virtual bool SaveOneTradingDay(const std::string& nsTableName,
		const std::string& strTradingDay,
		int nInitStatus,
		int& nErrorCode );

	//更新交易日状态
	virtual bool UpdateTradingDayInitStatus(const std::string& nsTableName,
		const std::string& strTradingDay,
		int nInitStatus,
		int& nErrorCode ) ;

	//写结算日
	virtual bool SaveOneSettlementDay(const std::string& nsTableName,
		const std::string& strTradingDay,	
		int& nErrorCode ) ;

	virtual bool SaveDepthMarketDataWithDelete( const std::string& nsTableName,
		const std::string& strTime,
		const std::vector<PlatformStru_DepthMarketData>& vecData, 
		int& nErrorCode );


	//报单
	virtual bool MergeOrderInfos(
		const std::string& nsTableName,
		const std::string& strTime,		
		const std::vector<PlatformStru_OrderInfo>& nOrders ,
		int& nErrorCode ) ;

public://报盘
	virtual bool SaveOrderTransfer(const std::vector<SOrderTransfer>& vecOrderTransfer,	int& nErrorCode );
	virtual bool GetOrderTransfer(std::string strTime,  std::vector<SOrderTransfer>& vecOrderTransfer); 
	virtual bool UpdateOrderTransfer(SOrderTransfer& sOrderTransfer);

public://风控
	//风险事件发生后，消息通知
	virtual bool QueryData( const char* pSql, std::vector<RiskIndicator>& vec, int& nErrorCode );
	virtual bool QueryData( const char* pSql, std::vector<RiskResponse>& vec, int& nErrorCode );
	virtual bool QueryData( const char* pSql, std::vector<RiskEvent>& vec, int& nErrorCode );
	virtual bool QueryData( const char* pSql, std::vector<MessageInfo>& vec, int& nErrorCode);
	virtual bool QueryData( const char* pSql, std::vector<MsgSendStatus>& vec, int& nErrorCode );
	virtual bool QueryData( const char* pSql, std::vector<NetFundCalcResult>& vec, int& nErrorCode ); 
	virtual bool AddMessage( const MessageInfo& msgInfo, const std::vector<TargetAccount> vAccount, int& nPKID, int& nErrorCode);
	virtual bool AddMessage( const MessageInfo& msgInfo, int& nPKID, int& nErrorCode );
	bool SaveMessageSendStatus( std::vector<MsgSendStatus> vMsgStatus );
	//只限同一个组织机构的同一种风险
	bool InsertRiskPlan(std::vector<RiskPlan>& vecRiskPlan, int& nErrorCode);
	bool GetRiskPlan(const char* pSql, std::vector<RiskPlan>& vecRiskPlan, int& nErrorCode );
	bool QueryData( const char* pSql, std::vector<EventMessageTemplate>& vec );
	bool SaveMsgTemplate( int nIndicatorID, std::vector<EventMessageTemplate>& vec );	
	bool QueryData( const char* pSql, std::vector<NetFundParam>& vec );

	bool SaveRiskEvent( std::string strTime, std::vector<RiskEvent>& vRiskEvent, int& nErrorCode, bool deleteToday = true );
	bool SaveNetFundCalcResult(std::string strTime, std::vector<NetFundCalcResult>& vResult, int& nErrorCode);
	bool SaveDepthMarketData( const std::string& nsTableName,
		const std::string& strTime,
		const std::vector<PlatformStru_DepthMarketData>& vecData, int& nErrorCode );
    bool SaveDepthMarketData_PerDay( const std::string& nsTableName,
        const std::string& strTime,
        const std::vector<PlatformStru_DepthMarketData>& vecData, int& nErrorCode );

	//风控强平下单记录 bForceCloseType:true自动强平，false手头强平
	bool SaveForceCloseOrder(PlatformStru_InputOrder& order, InputOrderKey& lKey, bool bForceCloseType, std::string strRiskName, int& nErrorCode);

	//风控强平撤单
	bool SaveForceCloseOrderAction(PlatformStru_InputOrderAction& order, bool bForceCloseType, std::string strRiskName, int& nErrorCode);
	//写初始资金
	virtual bool SaveInitFund(const std::string & nsTableName,
		const std::string & nsUserName,
		const double & ndbVal) ;

    //基础数据操作入库
    bool WriteLogToDB( const LogBaseData& logData, int& nErrorCode );

	//判断数据库没有连接上
	bool IsConnected();
public://策略
	void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst );
	void GetFolderFileName(std::string& strFilePath,const std::string& folder);
	void ReadBlobToFile(oracle::occi::Blob& blob, std::string strFileName);
	void WriteFileToBlob(oracle::occi::Blob& blob, char *pData, int nLength);

	bool DB_GetUserStrategys(std::vector<SStrategy>& vecStrategy, std::string strUser ="");
	bool DB_AddStrategy(const SStrategy& strategy, int& nErrorCode );
	bool DB_DelStrategy(std::string strName, int& nErrorCode );
	bool DB_SetStragegyStatus(std::string strName, int nUse, int& nErrorCode );
	bool DB_WriteStragegyFile(std::string strName, int nType, char *pData, int nLength, int& nErrorCode);//nType:2写入dll文件， nType：3 写入zip文件

	bool DB_AddStrategy2IndexRelation(std::string strName, std::string strIndex, int& nErrorCode);
	bool DB_DelStrategy2IndexRelation(std::string strName, int& nErrorCode);
	bool DB_GetStrategy2IndexRelation(std::vector<SStrategy2Index>& vec);

	bool DB_GetUserInstance(std::vector<SStrategyInstance>& vecInstance, std::string strUser ="");
	bool DB_AddInstance(const SStrategyInstance& strategy, int& nErrorCode);
	bool DB_DelInstance(std::string strName, int& nErrorCode);
	bool DB_DelInstance_ByTrader(std::string strName, std::string strUploader, int& nErrorCode);	
	bool DB_SetInstanceStatus(std::string strName, std::string strUploader, int nUse, int& nErrorCode);

	bool DB_GetUserIndex(std::vector<SIndex>& vecIndex, std::string strUser ="");
	bool DB_AddIndex(const SIndex& strategy, int& nErrorCode );
	bool DB_DelIndex(std::string strName, int& nErrorCode);
	bool DB_WriteIndexFile(std::string strName, int nType, char *pData, int nLength, int& nErrorCode);
	
	bool DB_WriteLogToDB( const LogStrategy& logData, int& nErrorCode );

	//当与数据库断开连接时，需要将入库数据 存 文件
	//风险事件
	void SaveRiskEvent2File( std::vector<RiskEvent>& vRiskEvent);
	//基金净值
	void SaveNetFundCalcResult2File(std::vector<NetFundCalcResult>& vResult);
	//风控强平下单记录
	void SaveForceCloseOrder2File(PlatformStru_InputOrder& order, InputOrderKey& lKey, bool bForceCloseType, std::string strRiskName);
	//风控强平撤单
	void SaveForceCloseOrderAction2File(PlatformStru_InputOrderAction& order, bool bForceCloseType, std::string strRiskName);
	//报单审核
	void SaveVertifyOrder2File(SVerisyOrder& order);
	std::string Int2String(int field);
	std::string Double2String(double field);
	//动态创建每日合约表
	bool CreatInsTable_PreDay(const std::string& nsTableName,
		int& nErrorCode );
    //动态创建每日行情表
	bool CreatDepthMarketTable_PerDay(const std::string& nsTableName);
	//动态创建K线表
	bool CreatKLineTable_PerDay(const std::string& strDate);

    
    bool SaveStockQuot( const int nType,
                        const std::string& nsTableName,
                        const std::string& strTime,
                        const vector< PlatformStru_DepthMarketData >& vData,
                        int& nErrorCode );
	

/*	bool IsStrategyExist(std::string strName);	
	bool UpdateStrategy(SStrategy& strategy, int nType);
	bool SelectStrategy(std::vector<SStrategy>& vecStrategy);
	
	void GetFolderFileName(std::string& strFilePath,const std::string& folder);
	void ReadBlobToFile(oracle::occi::Blob& blob, std::string strFileName);

	void WriteFileToBlob(oracle::occi::Blob& blob, char *pData, int nLength);

	//指标
	bool IsIndexExist(std::string strName);
	bool SaveIndex(SStrategyInstance& strategy, int& nErrorCode );
	bool UpdateIndex(SStrategyInstance& strategy, int nType);
	bool SelectIndex(std::vector<SStrategyInstance>& vecStrategy);
	*/
private:
	template <class T>
	bool BatchInsert(							
		const std::string& nsTableName,
		const std::string& nDeleteSql,
		const std::string& nValidateDate,
		std::vector<ColumeData>& nVecColume ,
		const std::vector<T>& nVecData ,
		int& nErrorCode );

    template <class T>
    bool BatchInsert_PerDay(							
        const std::string& nsTableName,
        const std::string& nDeleteSql,
        const std::string& nValidateDate,
        std::vector<ColumeData>& nVecColume ,
        const std::vector<T>& nVecData ,
        int& nErrorCode );

	template <class T>
	bool BatchMerge(							
		const std::string& nsTableName,
		const std::string& nsConditionStr,
		const std::string& nValidateDate,
		std::vector<ColumeDataEx>& nVecColume ,
		const std::vector<T>& nVecData ,
		int& nErrorCode );

	template <class T>
	bool BatchDelete(							
		const std::string& nsTableName,
		const std::string& nsConditionStr,
		std::vector<ColumeDataEx>& nVecColume ,
		const std::vector<T>& nVecData ,
		int& nErrorCode );
	
private:
	
	void DisConnected();
	void RollBack();
	void WriteLog(const std::string& strSql, const std::string& strError);
	int GetErrorCode(const std::string& strError );

private:
	oracle::occi::Environment* m_pEnvironment;
	oracle::occi::Connection * m_pCon;
	oracle::occi::Statement* m_pStmt;
	oracle::occi::ResultSet* m_pRes;

	std::string m_strUserName;
	std::string m_strPwd;
	std::string m_strDBName;
	volatile bool m_bConn;
	CWriteLog* m_pWriteLog;

private:
	CSvrDBOprImpl();
	virtual ~ CSvrDBOprImpl();
	static CSvrDBOprImpl* m_pObj;
	int m_nQuotSeq;
};
template<class T>
class CSafeArrayPtr
{
public:
	CSafeArrayPtr(T* p):m_ptr(p){}
	~CSafeArrayPtr(){if(m_ptr) delete[] m_ptr;}
private:
	T* m_ptr;
	
};