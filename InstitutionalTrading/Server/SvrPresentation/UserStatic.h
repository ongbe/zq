#pragma once
#include <map>
#include <string>
#include <set>
#include <vector>
#include "CommonStruct.h"
class CUserStatic
{
private:
	double mdbPositionProfit; //持仓盈亏汇总
	double mdbInitFund;       //用户初始权益
	double mdbStaticBalance;  //当前总权益汇总
	std::map<std::string,double>  mmapInstrumentMargin;//单合约持仓报证金
	std::map<std::string,double>  mmapInstrmentVolume;//单合约持仓量
	std::set<std::string> msetInstrument;

public:
	
	CUserStatic()
	{
		clear();
	}
	~CUserStatic()
	{
		clear();
	}

	void clear()
	{
		msetInstrument.clear();
		mdbStaticBalance = mdbInitFund = mdbPositionProfit = 0.0f;
		mmapInstrmentVolume.clear();
		mmapInstrumentMargin.clear();
	}
	double GetPositionProfit()
	{
		return mdbPositionProfit;
	}

	void SetPositionProfit(double ndbVal)
	{
		mdbPositionProfit = ndbVal;
	}

	double GetInitFund()
	{
		return mdbInitFund;
	}

	void SetInitFund(double ndbVal)
	{
		mdbInitFund = ndbVal;
	}

	double GetStaticBalance()
	{
		return mdbStaticBalance;
	}

	void SetStaticBalance(double ndbVal)
	{
		mdbStaticBalance = ndbVal;
	}

	void PushInstrumentUserMargin(const std::string& nsInstrumentID,double ndbVal)
	{
		msetInstrument.insert(nsInstrumentID);
		mmapInstrumentMargin[nsInstrumentID] += ndbVal;
	}

	double GetInstruemntUserMargin(const  std::string& nsInstrumentID)
	{
		if(mmapInstrumentMargin.find(nsInstrumentID) != mmapInstrumentMargin.end())
			return mmapInstrumentMargin[nsInstrumentID];
		else
			return 0.0f;
	}

	void PushInstrumentVolumn(const  std::string& nsInstrumentID,double ndbVal)
	{
		msetInstrument.insert(nsInstrumentID);
		mmapInstrmentVolume[nsInstrumentID] += ndbVal;
	}

	double GetInstruemntUserVolumn(const std::string& nsInstumentId)
	{
		if(mmapInstrmentVolume.find(nsInstumentId) != mmapInstrmentVolume.end())
			return mmapInstrmentVolume[nsInstumentId];
		else
			return 0.0f;
	}

	void GetInsturments(std::set<std::string>& lsetInstrument)
	{
		lsetInstrument = msetInstrument;
	}

	void GetStatisticsEntryList(const AssetOrgProRelation& org,std::vector<StatisticsEntry>& vec);

};