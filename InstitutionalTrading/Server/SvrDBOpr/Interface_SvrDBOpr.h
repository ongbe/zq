#pragma once

#include "Interface_SvrCommon.h"
#include <comutil.h>
#include <vector>
#include "CommonStruct.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "RiskManageStruct.h"
#pragma comment(lib, "comsuppw.lib") 

#ifndef SVRDBOPR_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRDBOPR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRDBOPR_EXPORTS
#define SVRDBOPR_API __declspec(dllexport)
#else
#define SVRDBOPR_API __declspec(dllimport)
#endif
#endif

class SVRDBOPR_API CInterface_SvrDBOpr : public CInterface_SvrCommon
{
public:
    static CInterface_SvrDBOpr& getObj(void);

	virtual void InitDB(const std::string& strUserName, const std::string& strPwd, const std::string& strDBName) = 0;
	virtual bool Conncect() = 0;

	//执行更新、删除SQL语句，返回影响的记录条数
	virtual bool ExcuteUpdate( const char* pSql, int& nNum, int& nErrorCode ) = 0;
	//执行任意SQL语句
	virtual bool Excute( const char* pSql, int& nErrorCode ) = 0;
	//执行查询SQL语句，返回查询的记录条数
	virtual bool ExcuteSelect( const char* pSql, int& nRecordNum, int& nErrorCode ) = 0;
	//插入一条数据，并返回插入记录的ID
	virtual bool InsertAndReturnID( const char* pSql, const char* pIDSql, int& nPKID, int& nErrorCode ) = 0;
	//执行查询SQL语句，并返回结果集
	virtual bool QueryData(const char* pSql, std::vector<std::vector<_variant_t>>& vec, int& nErrorCode ) = 0;

	//保存权限
	virtual bool SaveRolePrivilegeRelation(int nRoleID, std::vector<int>& vecPrivilegeID, int& nErrorCode) = 0;
	
	//保存用户角色
	virtual bool SaveUserRole( int nUserID, std::vector<int>& vRoleID, int& nErrorCode ) = 0;

    //保存理财产品和交易员关系
    virtual bool SaveProductTraderRelation( int nRelationType, int nID, std::vector<int>& vID, int& nErrorCode ) = 0;

    //保存交易策略和组织机构关系
    virtual bool SaveStrategyOrganizationRelation( int nStrategyID, std::vector<int>& vOrgID, int& nErrorCode ) = 0;

    //保存用户和组织机构关系
    virtual bool SaveUserOrganizationRelation( int nOrgID, int nUserType, int nUserID, int nRelationType, int& nErrorCode) = 0;
    virtual bool SaveUserOrganizationRelation( int nOrgID, int nUserType, std::vector<int>& vInID, std::vector<int>& vOutID, int nRelationType, int& nErrorCode) = 0;
    virtual bool SaveUserOrganizationRelation( int nRiskID, int nAttachOrg, std::vector<int>& vMonitorOrg, int& nErrorCode) = 0;

	//存结算价
	virtual bool SaveSettlementPrice2DB(std::string strTime, std::vector<SettlementPriceField>& vSettlementPrice ,int& nErrorCode)=0;

	//保存合约
	virtual bool SaveInstruments(const std::string& strTime, 
		const std::vector<PlatformStru_InstrumentInfo>& nInstruemnts ,
		int& nErrorCode ,
		const std::string& nsTableName ="TRADEDATA_INSTRUEMENTS") = 0;
	//动态创建每日合约表
	virtual bool CreatInsTable_PreDay(const std::string& nsTableName,
		int& nErrorCode ) = 0;

	//保存费率
	virtual bool SaveAccountCommissionRate(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_InstrumentCommissionRate>& nCommission ,
		int& nErrorCode ) = 0 ;


	//保证金率
	virtual bool SaveAccountMarginRate(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_InstrumentMarginRate>& nMargins ,
		int& nErrorCode ) = 0;

	//报单
	virtual bool SaveUserOrderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_OrderInfo>& nMargins ,
		int& nErrorCode ) = 0;
	//成交
	virtual bool SaveUserTraderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_TradeInfo>& nMargins ,
		int& nErrorCode ) = 0;

	//持仓
	virtual bool SaveUserPositionInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_Position>& nPositions ,
		int& nErrorCode )= 0;

	//持仓明细
	virtual bool SaveUserPositionDetailInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsUserName,
		const std::vector<PlatformStru_PositionDetail>& nPositionDetails ,
		int& nErrorCode )= 0;

	//资金
	virtual bool SaveUserFundInfos(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsUserName,
		const PlatformStru_TradingAccountInfo& nFundInfos,
		int& nErrorCode )= 0;


	//报单
	virtual bool SaveAccountOrderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_OrderInfo>& nMargins ,
		int& nErrorCode ) = 0;
	//成交
	virtual bool SaveAccountTraderInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_TradeInfo>& nMargins ,
		int& nErrorCode ) = 0;

	//持仓
	virtual bool SaveAccountPositionInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_Position>& nPositions ,
		int& nErrorCode )= 0;

	//持仓明细
	virtual bool SaveAccountPositionDetailInfos(
		const std::string& nsTableName,
		bool nbDelete,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const std::vector<PlatformStru_PositionDetail>& nPositionDetails ,
		int& nErrorCode )= 0;

	//资金
	virtual bool SaveAccountFundInfos(
		const std::string& nsTableName,
		const std::string& strTime,
		const std::string& nsBrokerID,
		const std::string& nsAccountID,
		const PlatformStru_TradingAccountInfo& nFundInfos,
		int& nErrorCode )= 0;

	//写出入金记录
	virtual bool SaveFundInOut(const sFundInOut & s) = 0;

	//写交易日
	virtual bool SaveOneTradingDay(const std::string& nsTableName,
		const std::string& strTradingDay,
		int nInitStatus,
		int& nErrorCode ) = 0;

	//更新交易日状态
	virtual bool UpdateTradingDayInitStatus(const std::string& nsTableName,
		const std::string& strTradingDay,
		int nInitStatus,
		int& nErrorCode ) = 0;

	//写结算日
	virtual bool SaveOneSettlementDay(const std::string& nsTableName,
		const std::string& strTradingDay,	
		int& nErrorCode ) = 0;

	//报盘orderfref转化
	virtual bool SaveOrderTransfer(const std::vector<SOrderTransfer>& vecOrderTransfer ,
		int& nErrorCode ) = 0;

	virtual bool GetOrderTransfer(std::string strTime, std::vector<SOrderTransfer>& vecOrderTransfer) = 0;

	virtual bool UpdateOrderTransfer(SOrderTransfer& sOrderTransfer) = 0;

	//风险事件发生后，消息通知
	virtual bool QueryData( const char* pSql, std::vector<RiskIndicator>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<RiskResponse>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<RiskEvent>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<MessageInfo>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<MsgSendStatus>& vec, int& nErrorCode ) =0;
	virtual bool QueryData( const char* pSql, std::vector<NetFundCalcResult>& vec, int& nErrorCode ) =0;

	virtual bool AddMessage( const MessageInfo& msgInfo, const std::vector<TargetAccount> vAccount, int& nPKID, int& nErrorCode) = 0;
	virtual bool AddMessage( const MessageInfo& msgInfo, int& nPKID, int& nErrorCode ) = 0;
    virtual bool SaveMessageSendStatus( std::vector<MsgSendStatus> vMsgStatus ) = 0;

	virtual bool InsertRiskPlan(std::vector<RiskPlan>& vecRiskPlan, int& nErrorCode) = 0;
	virtual bool GetRiskPlan(const char* pSql,std::vector<RiskPlan>& vecRiskPlan, int& nErrorCode ) = 0;
	virtual bool SaveMsgTemplate( int nIndicatorID, std::vector<EventMessageTemplate>& vec ) =0;
	virtual bool QueryData( const char* pSql, std::vector<EventMessageTemplate>& vec ) =0;
	virtual bool QueryData( const char* pSql, std::vector<NetFundParam>& vec ) =0;

	virtual bool SaveRiskEvent( std::string strTime, std::vector<RiskEvent>& vRiskEvent, int& nErrorCode , bool deleteToday = true ) = 0;
	virtual bool SaveNetFundCalcResult(std::string strTime, std::vector<NetFundCalcResult>& vResult, int& nErrorCode)=0;
	//得到风险指标设置
	
	//风控强平下单记录
	virtual bool SaveForceCloseOrder(PlatformStru_InputOrder& order, InputOrderKey& lKey, bool bForceCloseType, std::string strRiskName, int& nErrorCode) = 0;
	//风控强平撤单
	virtual bool SaveForceCloseOrderAction(PlatformStru_InputOrderAction& order, bool bForceCloseType, std::string strRiskName, int& nErrorCode) =0;

public://策略
	virtual	bool DB_GetUserStrategys(std::vector<SStrategy>& vecStrategy, std::string strUser ="") = 0;
	virtual	bool DB_AddStrategy(const SStrategy& strategy, int& nErrorCode ) = 0;
	virtual	bool DB_DelStrategy(std::string strName, int& nErrorCode) = 0;
	virtual	bool DB_SetStragegyStatus(std::string strName, int nUse, int& nErrorCode ) = 0;
	virtual bool DB_WriteStragegyFile(std::string strName, int nType, char *pData, int nLength, int& nErrorCode)=0;//nType:2写入dll文件， nType：3 写入zip文件
	virtual bool DB_AddStrategy2IndexRelation(std::string strName, std::string strIndex, int& nErrorCode) = 0;
	virtual bool DB_DelStrategy2IndexRelation(std::string strName, int& nErrorCode) = 0;
	virtual bool DB_GetStrategy2IndexRelation(std::vector<SStrategy2Index>& vec) = 0;

	virtual	bool DB_GetUserInstance(std::vector<SStrategyInstance>& vecInstance, std::string strUser ="") = 0;
	virtual	bool DB_AddInstance(const SStrategyInstance& strategy, int& nErrorCode) = 0;
	virtual	bool DB_DelInstance(std::string strName, int& nErrorCode) = 0;
	virtual	bool DB_DelInstance_ByTrader(std::string strName, std::string strUploader, int& nErrorCode) = 0;
	virtual	bool DB_SetInstanceStatus(std::string strName, std::string strUploader, int nUse, int& nErrorCode) = 0;

	virtual	bool DB_GetUserIndex(std::vector<SIndex>& vecIndex, std::string strUser ="") = 0;
	virtual	bool DB_AddIndex(const SIndex& strategy, int& nErrorCode ) = 0;
	virtual	bool DB_DelIndex(std::string strName, int& nErrorCode) = 0;
	virtual bool DB_WriteIndexFile(std::string strName, int nType, char *pData, int nLength, int& nErrorCode)=0;

	//策略数据操作入库
	virtual bool DB_WriteLogToDB( const LogStrategy& logData, int& nErrorCode ) = 0;


    //保存当日行情
	virtual bool SaveDepthMarketData( const std::string& nsTableName,
		const std::string& strTime,
		const std::vector<PlatformStru_DepthMarketData>& vecData, int& nErrorCode ) = 0;    

    //保存当日行情
    virtual bool SaveDepthMarketData_PerDay( const std::string& nsTableName,
        const std::string& strTime,
        const std::vector<PlatformStru_DepthMarketData>& vecData, int& nErrorCode ) = 0;  

	//写初始资金
	virtual bool SaveInitFund(const std::string & nsTableName,
		const std::string & nsUserName,
		const double & ndbVal) = 0;


	//存结算行情
	virtual bool SaveDepthMarketDataWithDelete( const std::string& nsTableName,
		const std::string& strTime,
		const std::vector<PlatformStru_DepthMarketData>& vecData, 
		int& nErrorCode ) = 0;

    //基础数据操作入库
    virtual bool WriteLogToDB( const LogBaseData& logData, int& nErrorCode ) = 0;

	//判断数据库没有连接上
	virtual bool IsConnected() = 0;

	//当与数据库断开连接时，需要将入库数据 存 文件
	//风险事件
	virtual void  SaveRiskEvent2File( std::vector<RiskEvent>& vRiskEvent) = 0;
	//基金净值
	virtual void  SaveNetFundCalcResult2File( std::vector<NetFundCalcResult>& vResult)=0;
	//风控强平下单记录
	virtual void  SaveForceCloseOrder2File(PlatformStru_InputOrder& order, InputOrderKey& lKey, bool bForceCloseType, std::string strRiskName) = 0;
	//风控强平撤单
	virtual void  SaveForceCloseOrderAction2File(PlatformStru_InputOrderAction& order, bool bForceCloseType, std::string strRiskName) =0;
	//报单审核
	virtual void  SaveVertifyOrder2File(SVerisyOrder& order) =0;
	//动态创建每日行情表
	virtual bool CreatDepthMarketTable_PerDay(const std::string& nsTableName)=0;
	//动态创建K线表
	virtual bool CreatKLineTable_PerDay(const std::string& strDate)=0;


    //股票入库，目前只存当前行情的交易日、股票代码、高开低收价格
    //nType 0：上交所   1：深交所
    virtual bool SaveStockQuot( const int nType,
                                const std::string& nsTableName,
                                const std::string& strTime,
                                const vector< PlatformStru_DepthMarketData >& vData,
                                int& nErrorCode )=0;
};







