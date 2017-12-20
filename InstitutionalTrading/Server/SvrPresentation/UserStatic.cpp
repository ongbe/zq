#include "stdafx.h"
#include "UserStatic.h"



void CUserStatic::GetStatisticsEntryList(const AssetOrgProRelation& org,std::vector<StatisticsEntry>& vec)
{
	//加入initfund
	StatisticsEntry entry;
	memset(&entry,0,sizeof(StatisticsEntry));
	entry.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
	entry.mnFinancialProductID = org.mnFinancialProductID;
	entry.mnStatisticsType = ST_InitFund;
	entry.mdblValue = mdbInitFund;
	vec.push_back(entry);

	//加入posprofit
	StatisticsEntry entryProfit;
	memset(&entryProfit,0,sizeof(StatisticsEntry));
	entryProfit.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
	entryProfit.mnFinancialProductID = org.mnFinancialProductID;
	entryProfit.mnStatisticsType = ST_PosProfit;
	entryProfit.mdblValue = mdbPositionProfit;
	vec.push_back(entryProfit);

	//加入Balance
	StatisticsEntry entryBalance;
	memset(&entryBalance,0,sizeof(StatisticsEntry));
	entryBalance.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
	entryBalance.mnFinancialProductID = org.mnFinancialProductID;
	entryBalance.mnStatisticsType = ST_Balance;
	entryBalance.mdblValue = mdbStaticBalance;
	vec.push_back(entryBalance);

	//加入合约持仓量
	std::map<std::string,double>::iterator lIter = mmapInstrmentVolume.begin();
	for(;lIter != mmapInstrmentVolume.end(); lIter++)
	{
		StatisticsEntry entryInstrumentVolume;
		memset(&entryInstrumentVolume,0,sizeof(StatisticsEntry));
		entryInstrumentVolume.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
		entryInstrumentVolume.mnFinancialProductID = org.mnFinancialProductID;
		strcpy_s(entryInstrumentVolume.mInstrumentID,sizeof(_InstrumentID),lIter->first.c_str());
		entryInstrumentVolume.mnStatisticsType = ST_Position;
		entryInstrumentVolume.mdblValue = lIter->second;
		vec.push_back(entryInstrumentVolume);
	}

	if(mmapInstrmentVolume.size() == 0)
	{
		//加入Volume
		StatisticsEntry entryPosition;
		memset(&entryPosition,0,sizeof(StatisticsEntry));
		entryPosition.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
		entryPosition.mnFinancialProductID = org.mnFinancialProductID;
		entryPosition.mnStatisticsType = ST_Position;
		entryPosition.mdblValue = 0.0f;
		vec.push_back(entryPosition);

	}

	//加入合约持仓保证金
	std::map<std::string,double>::iterator lIterMargin = mmapInstrumentMargin.begin();
	for(;lIterMargin != mmapInstrumentMargin.end(); lIterMargin++)
	{
		StatisticsEntry entryInstrumentMargin;
		memset(&entryInstrumentMargin,0,sizeof(StatisticsEntry));
		entryInstrumentMargin.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
		entryInstrumentMargin.mnFinancialProductID = org.mnFinancialProductID;
		strcpy_s(entryInstrumentMargin.mInstrumentID,sizeof(_InstrumentID),lIterMargin->first.c_str());
		entryInstrumentMargin.mnStatisticsType = ST_UseMargin;
		entryInstrumentMargin.mdblValue = lIterMargin->second;
		vec.push_back(entryInstrumentMargin);
	}

	if(mmapInstrumentMargin.size() == 0)
	{
		StatisticsEntry entryMargin;
		memset(&entryMargin,0,sizeof(StatisticsEntry));
		entryMargin.mnAssetMgmtOrgID = org.mnAssetMgmtOrgID;
		entryMargin.mnFinancialProductID = org.mnFinancialProductID;
		entryMargin.mnStatisticsType = ST_UseMargin;
		entryMargin.mdblValue = 0.0f;
		vec.push_back(entryMargin);

	}
}