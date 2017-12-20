#pragma once
#include <string>
#include <map>
#include <set>
#include <list>
#include "StrategyDataEx.h"
#include "KernelStruct.h"
#include "CSVOperator.h"
#include "ConvEnum.h"
using namespace std;


// 类CDataEntity，用来保存管理策略测试时依赖或生产的所有数据，
// 包括策略的运行参数xml、合约、行情、保证金率、手续费率、资金、持仓、报单、成交。
// 主要功能：
// 1.  持有和管理策略的运行参数xml、合约、行情、保证金率、手续费率、资金、持仓、报单、成交数据。
// 2.  负责保存策略的运行参数xml、合约、行情、保证金率、手续费率、资金、持仓、报单、成交数据。
// 3.  负责加载历史测试的策略的运行参数xml、合约、行情、保证金率、手续费率、资金、持仓、报单、成交数据。

class CDataEntity
{
public:
	CDataEntity(void);
	virtual ~CDataEntity(void);

public:

	void SetSaveData(bool bHas) {
		m_bSaveData = bHas;
	};
	bool GetSaveData() {
		return m_bSaveData;
	};
	void SetSavePath(const std::string& strPath) {
		m_strSavePath = strPath;
	};
	const std::string& GetSavePath() {
		return m_strSavePath;
	};
	void SetStrategyCreateTime(std::string& strTime) {
		strncpy(m_testIndicator.szCreatTime, strTime.c_str(), sizeof(m_testIndicator.szCreatTime)-1);
	};
	void SetStrategyEditTime(std::string& strTime) {
		strncpy(m_testIndicator.szModifyTime, strTime.c_str(), sizeof(m_testIndicator.szModifyTime)-1);
	};
	void SetTestStartTime(std::string& strTime) {
		strncpy(m_testIndicator.szStartTime, strTime.c_str(), sizeof(m_testIndicator.szStartTime)-1);
	};
	void SetTestEndTime(std::string& strTime) {
		strncpy(m_testIndicator.szEndTime, strTime.c_str(), sizeof(m_testIndicator.szEndTime)-1);
	};
	void SetQuoteSource(std::string& strSource) {
		strncpy(m_testIndicator.szQuoteSource, strSource.c_str(), sizeof(m_testIndicator.szQuoteSource)-1);
	};

	bool LoadAll();
	bool SaveAll();
	bool LoadSimple(TestSimpleData& data);
	
	void SetStrategy(CStrategyDataEx& strategy) {
		m_strategy = strategy;
	};
	CStrategyDataEx& GetStrategy() {
		return m_strategy;
	};
	bool SaveStrategy();
	bool LoadStrategy();

	void SetInstrument(const std::map<std::string, PlatformStru_InstrumentInfo>& mapInstrument) {
		m_mapInstrument = mapInstrument;
	};
	const std::map<std::string, PlatformStru_InstrumentInfo>& 
	GetInstrument() {
		return m_mapInstrument;
	};
	bool SaveInstrument();
	bool LoadInstrument();
	
	void SetMDQuotList(const std::list<PlatformStru_DepthMarketData>& lstQuot) {
		m_lstQuot = lstQuot;
	};
	const std::list<PlatformStru_DepthMarketData>& 
	GetMDQuotList() {
		return m_lstQuot;
	};
	bool AppendQuot(const PlatformStru_DepthMarketData& quot);
	void ClearQuotList();
	bool SaveMDQuotList();
	bool LoadMDQuotList();

	void SetMarginRate(const std::map<std::string, PlatformStru_InstrumentMarginRate>& mapMarginRate) {
		m_mapMarginRate = mapMarginRate;
	};
	const std::map<std::string, PlatformStru_InstrumentMarginRate>& 
	GetMarginRate() {
		return m_mapMarginRate;
	};
	bool SaveMarginRate();
	bool LoadMarginRate();

	void SetCommissionRate(const std::map<std::string, PlatformStru_InstrumentCommissionRate>& mapCommissionRate) {
		m_mapCommissionRate = mapCommissionRate;
	};
	const std::map<std::string, PlatformStru_InstrumentCommissionRate>& 
	GetCommissionRate() {
		return m_mapCommissionRate;
	};
	bool SaveCommissionRate();
	bool LoadCommissionRate();

	const std::list<PlatformStru_TradingAccountInfo>& 
	GetTradingAccountList() {
		return m_lstTradingAccnt;
	};
	bool AppendTradingAccount(const PlatformStru_TradingAccountInfo& tradingAccnt);
	void ClearTradingAccountList();
	bool SaveTradingAccount();
	bool LoadTradingAccount();

	const std::list<PlatformStru_Position>&
	GetPositionList() {
		return m_lstPosition;
	};
	bool AppendPosition(const PlatformStru_Position& position);
	void ClearPositionList();
	bool SavePosition();
	bool LoadPosition();

	const std::list<PlatformStru_OrderInfo>& 
	GetOrderList() {
		return m_lstOrder;
	};
	bool AppendOrder(const PlatformStru_OrderInfo& order);
	void ClearOrderList();
	bool SaveOrder();
	bool LoadOrder();

	const std::list<PlatformStru_TradeInfo>&
	GetTradeList() {
		return m_lstTrade;
	};
	bool AppendTrade(const PlatformStru_TradeInfo& trade);
	void ClearTradeList();
	bool SaveTrade();
	bool LoadTrade();

	void CaculateIndicator();
	bool SaveTestIndicator();
	bool LoadTestIndicator();
	void SetInitFund( double fund);
	void SetIndiOpenOrder( int Cnt);
	int  GetMaxPosition();
	void SetMaxPosition( int Cnt);
	void SetTradingTime(int date);

	void GetQuot(std::list<PlatformStru_DepthMarketData>& outData);
	void GetInstrumentInfo(std::map<std::string, PlatformStru_InstrumentInfo>& outData);
	void GetMarginRate(std::map<std::string, PlatformStru_InstrumentMarginRate>& outData);
	void GetCommissionRate(std::map<std::string, PlatformStru_InstrumentCommissionRate>& outData);
	void GetTradingAccountInfo(std::list<PlatformStru_TradingAccountInfo>& outData);
	void GetPosition(std::list<PlatformStru_Position>& outData);
	void GetOrderInfo(std::list<PlatformStru_OrderInfo>& outData);
	void GetTradeInfo(std::list<PlatformStru_TradeInfo>& outData);
	void GetTestIndicator(TestIndicator& outData);

protected:
	//bool ConvToQuot(const std::string& strQuot, PlatformStru_DepthMarketData& ret);
	//bool ConvToInstrument(const std::string& strInstrument, PlatformStru_InstrumentInfo& ret);
	//bool ConvToMarginRate(const std::string& strInstrument, PlatformStru_InstrumentInfo& ret);
	//bool ConvToCommissionRate(const std::string& strInstrument, PlatformStru_InstrumentInfo& ret);

protected:
	bool					m_bSaveData;
	// 路径格式"E:\strategy\HistoryDatas\20140610 164447\"
	std::string				m_strSavePath;
	CStrategyDataEx			m_strategy;
	std::list<PlatformStru_DepthMarketData>
							m_lstQuot;
	std::map<std::string, PlatformStru_InstrumentInfo>
							m_mapInstrument;
	std::map<std::string, PlatformStru_InstrumentMarginRate>
							m_mapMarginRate;
	std::map<std::string, PlatformStru_InstrumentCommissionRate>
							m_mapCommissionRate;
	std::list<PlatformStru_TradingAccountInfo>
							m_lstTradingAccnt;
	std::list<PlatformStru_Position>
							m_lstPosition;
	std::list<PlatformStru_OrderInfo>
							m_lstOrder;
	std::list<PlatformStru_TradeInfo>
							m_lstTrade;
	TestIndicator       m_testIndicator;

};
