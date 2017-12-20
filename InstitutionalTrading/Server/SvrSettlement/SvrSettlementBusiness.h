#pragma once
#include "Interface_SvrSettlement.h"
#include "SettlementCalc.h"
#include <vector>
#include <map>
#include "CommonErrorCode.h"
#include "WriteLog.h"


class SvrSettlementBusiness : public CInterface_SvrSettlement
{

public:
	static SvrSettlementBusiness& getObj(void);
	static void deleteObj();
	/*virtual CF_ERROR StratSettlement(void);
	virtual CF_ERROR FinishSettlement(void);*/
	CF_ERROR QuerySettlementPrice( std::vector<SettlementPriceField>& outData,int nbHistorySettlement);
	CF_ERROR ModifySettlementPrice(const SettlementPriceField& indata,int userId);
	CF_ERROR QueryCommissionRate( std::map<RateKey,CommissionMarginRate>& outData);
	CF_ERROR ModifyCommissionRate( CommissionMarginRate& outData,int userId);
	CF_ERROR QueryTrade(std::vector<PlatformStru_TradeInfo>& outData); 
	CF_ERROR AddTrade(PlatformStru_TradeInfo& indata,int userId);
	CF_ERROR DeleteTrade(PlatformStru_TradeInfo& indata,int userId);
	CF_ERROR ModifyTrade(PlatformStru_TradeInfo&  originaldata, PlatformStru_TradeInfo&  newdata,int userId);
	CF_ERROR QueryPositionDetail(std::vector<PlatformStru_PositionDetail>& outData);
	CF_ERROR AddPositionDetail(PlatformStru_PositionDetail& indata);
	CF_ERROR DeletePositionDetail(PlatformStru_PositionDetail& indata);
	CF_ERROR ModifyPositionDetail(PlatformStru_PositionDetail& indata);
	CF_ERROR QueryPosition(std::vector<PlatformStru_Position>& outData);
	//CF_ERROR AddPosition(PlatformStru_Position& indata,int userId);
	//CF_ERROR DeletePosition(PlatformStru_Position& indata,int userId);
	//CF_ERROR ModifyPosition(PlatformStru_Position* indata,int userId);
	CF_ERROR SetSettltmentTime(_SettlementDate * indata, int Flag);
	CF_ERROR ModifyFund(PlatformStru_TradingAccountInfo&  inData,int userId);
	CF_ERROR QueryFund(std::vector<PlatformStru_TradingAccountInfo>& outData);
	
	CF_ERROR FundCalc(void);
	CF_ERROR PositionDetailCalc(void);
	CF_ERROR PositionCalc(void);
	CF_ERROR SaveData(void);
	CF_ERROR loadData(eSettlementLoadOption nOption,const string& strDate,int nbHistorySettlement);
	CF_ERROR SettlementCalc(void);
	CF_ERROR SaveAction2DB(SettlementOperation& inData);
	CF_ERROR ConfirmForCaculate(void);
	CF_ERROR loadTradeData();
	void ModifiedRateClear();
	CF_ERROR SetModifiedRate();
	CF_ERROR ConfirmTrade();
	void     LoadFund();
	CF_ERROR ConfirmSettlementPrice();
	CF_ERROR QueryTradingday(std::string &pData);
	CF_ERROR LoadLastSettlementPrice(const std::string & strTime, std::map<std::string,SettlementPriceField>  & nInfo) ;
	CF_ERROR QuerySettlementInfo(PlatformStru_QrySettlementInfo&  inData,string&  outData);
	void LoadLatestSettlementDay(std::string& strSettlementDay);
	CF_ERROR CheckCloseVolume(PlatformStru_TradeInfo&  indata) ;

	void SetSettlementDay(const string SetDate);
	bool IsSettled();
	void AutoSettlement();
	bool LoadSettltTime(string& szSettleTime);
	string Double2String(double field);
    string fundDouble2String(double field);
	string Price2String(double field);
	string Volume2String(int field);
	string Risk2String(double field);
	//设置结算模块状态
	void SetSettlementStatus(eSettlementStatus eStatus);
	void GetTrade();
	void SetSettleSocket(int nSocket);
	void RemoveSettleSocket(int nSocket);
	int GetSettleSocket();
	CF_ERROR GetCTPLoadStatus(std::string &pData);
	CF_ERROR QueryHistoryCommissionRate(const string& strDate,std::vector<RateKey>& vecRate,std::map<RateKey,CommissionMarginRate>& outData);
	CF_ERROR LoadHistoryRate(const string& strDate,RateKey& key,PlatformStru_InstrumentCommissionRate& Commission,PlatformStru_InstrumentMarginRate& MarginRate);
    CF_ERROR  ImmediatelyAutoSettlement();


private:
	int time2Mins(char* strTime);
	//	判断一个时间是否在指定的时间段内。时间用一天的分钟数的整形表示
	bool IsInTimePeriod(int iMins,int StartMins,int EndMins);
	bool IsToday(const string& OpenDate);
	//string GetTradingDay(void);
	bool getSettlementPrice( const string instrumentId, double& price );
	bool GetMarginRate( const string userId, const string instrumentId, CommissionMarginRate& rate );
	//获取结算模块状态
	eSettlementStatus GetSettlementStatus() ;
	void Mapclear();



public:
    CSettlementCalc* m_pSettlementCalc;

private:
	SvrSettlementBusiness(void);
	~SvrSettlementBusiness(void);
	static SvrSettlementBusiness* m_pObj;
	std::map<std::string,PlatformStru_DepthMarketData>  m_mapSettlementPrice;
	std::map<std::string,SettlementPriceField>  m_mapSettlePriceInternal;
	
	std::map<std::string,std::vector<PlatformStru_TradeInfo>> m_mapTradeData;
	std::map<TradeKey,PlatformStru_TradeInfo> m_mapTradeID2TradeInfo;
	std::map<std::string,std::vector<PlatformStru_PositionDetail>> m_mapPositionDetail;
	std::map<std::string,std::vector<PlatformStru_Position>> m_mapPosition;
	std::map<std::string,PlatformStru_TradingAccountInfo> m_mapFundData;
	std::string m_strautoSettltmentTime;
	double InvalideValue;
    std::map<RateKey,CommissionMarginRate> m_mapCommissionMarginRate;
	std::string   msTodayString; //年-月-日
	std::vector<PlatformStru_InstrumentCommissionRate> m_vecModifideCommissionRate;
	std::vector<PlatformStru_InstrumentMarginRate> m_vecModifideMarginRate;
	bool m_bSettlement;
	bool m_bModifiedTrade;
	CWriteLog	*m_pWriteLog;	//日志接口
	eSettlementStatus m_SettlementStatus;
    int m_SettleSocket;
	//std::vector<PlatformStru_PositionDetail> m_vecPositionDetail;
	//std::vector<PlatformStru_Position> m_vecPosition;

	
};
