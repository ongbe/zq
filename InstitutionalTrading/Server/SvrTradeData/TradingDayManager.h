
#pragma once
#include <set>
#include <map>
#include "easymutex.h"
#include <string>
enum eTradingDayInitStatus
{
	TRADING_DAY_INIT_STATUS_NOTHING = 0,       
	TRADING_DAY_INIT_STATUS_INSTRUMENT = 1
};

class CTradingDayManager
{
	
public:
	CTradingDayManager();
	~CTradingDayManager();

	void SetNeedSaveTODB(bool nbNeed) { mbNeedSaveDB = nbNeed;}
	//装置历史的交易日
	void LoadHistoryTradingDay();

	//装置历史的结算日
	void LoadHistorySettmentedDay();

	//判断是否有盘中初始化过的交易日未结算
	bool HasInitTradingDayWithNoSettlement(std::string & nsOldestInitTradingDayWithNoSettlement);

	//获取上一个交易日
	bool GetLastTradingData(const std::string & nDay,std::string & nLastTradingDay);

	//添加一个交易日
	bool AddTradingDay(const std::string & nDay);

	//设置一个交易日初始化完毕了
	bool SetTradingDayInit(const std::string & nDay);

	//是不是系统运行的第一天
	bool IsFirstDay();

	//是否是交易日
	bool IsTradingDay(const std::string & nsDay);

	//是否结算过
	bool IsSetttemnted(const std::string & nsDay);


	//添加一个结算日
	bool AddSettlementedDay(const std::string & nDay); 

	//获取最新的已结算的交易日
	bool GetNewestSettlementDay( std::string & nDay); 
private:
	Ceasymutex mMutex;
	std::map<std::string,eTradingDayInitStatus> mmapAllTradingDay;//所有的交易日

	std::set<std::string> msetAllSettlementedDay;//所有已做过的结算日

	bool mbNeedSaveDB;
};