#include "stdafx.h"
#include "DataCenter.h"
#include "RiskProcess.h"
#include "ThreadSharedData.h"

CRiskProcess::CRiskProcess(CThreadSharedData* pThreadSharedData )
   :m_pThreadSharedData(pThreadSharedData)
{
}

CRiskProcess::~CRiskProcess(void)
{
}
void CRiskProcess::GetMaxPositionRisk(const std::string & BrokerID, const string& InvestorID,  std::set<string>& setInstrumentID)
{
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>::const_iterator it;
	m_mutex_MaxPositionLevel.read_lock();	
	for(it = m_MaxPositionLevel.begin(); it!= m_MaxPositionLevel.end(); it++)
	{
		SRiskEventKey temp = it->first;
		if(strcmp(BrokerID.c_str(), temp.baKey.BrokerID) == 0
			&& strcmp(InvestorID.c_str(), temp.baKey.AccountID) == 0)
		{
			const std::map<std::string,EventLevelID>& nestmap=it->second;
			std::map<std::string,EventLevelID>::const_iterator nestit=nestmap.begin();
			while(nestit!=nestmap.end())
			{
				if(nestit->second.LevelID>0)
					setInstrumentID.insert(nestit->first);

				nestit++;
			}
		}
	}
	m_mutex_MaxPositionLevel.read_unlock();	
}
void CRiskProcess::GetMarketShockRisk(const std::string & BrokerID, const string& InvestorID,  std::set<string>& setInstrumentID)
{
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>::const_iterator it;
	m_mutex_MarketShockLevel.read_lock();
	for(it = m_MarketShockLevel.begin(); it!= m_MarketShockLevel.end(); it++)
	{
		SRiskEventKey temp = it->first;
		if(strcmp(BrokerID.c_str(), temp.baKey.BrokerID) == 0
			&& strcmp(InvestorID.c_str(), temp.baKey.AccountID) == 0)
		{
			const std::map<std::string,EventLevelID>& nestmap=it->second;
			std::map<std::string,EventLevelID>::const_iterator nestit=nestmap.begin();
			while(nestit!=nestmap.end())
			{
				if(nestit->second.LevelID>0)
					setInstrumentID.insert(nestit->first);

				nestit++;
			}
		}
	}
	m_mutex_MarketShockLevel.read_unlock();	
}
//////////////////////////////////////////////////////////////////////////
bool CRiskProcess::HasMaxPositionRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID) const
{
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>::const_iterator it;
	bool bRet=false;
	m_mutex_MaxPositionLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	it=m_MaxPositionLevel.find(reKey);
	if(it!=m_MaxPositionLevel.end())
	{
		const std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::const_iterator nestit=nestmap.find(InstrumentID);
		if(nestit!=nestmap.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MaxPositionLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasMarketShockRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID) const
{
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>::const_iterator it;
	bool bRet=false;
	m_mutex_MarketShockLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	it=m_MarketShockLevel.find(reKey);
	if(it!=m_MarketShockLevel.end())
	{
		const std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::const_iterator nestit=nestmap.find(InstrumentID);
		if(nestit!=nestmap.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MarketShockLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasAccountLossRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd)const
{
	bool bRet=false;
	m_mutex_AccountLossLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_AccountLossLevel.find(reKey);
	if(nestit!=m_AccountLossLevel.end())
	{
		if(nestit!=m_AccountLossLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_AccountLossLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasTodayLossRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd)const
{
	bool bRet=false;
	m_mutex_TodayLossLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_TodayLossLevel.find(reKey);
	if(nestit!=m_TodayLossLevel.end())
	{
		if(nestit!=m_TodayLossLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_TodayLossLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasGappedMarketRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_GappedMarketLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_GappedMarketLevel.find(reKey);
	if(nestit!=m_GappedMarketLevel.end())
	{
		if(nestit!=m_GappedMarketLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_GappedMarketLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasMarginRatioRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_MarginRatioLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_MarginRatioLevel.find(reKey);
	if(nestit!=m_MarginRatioLevel.end())
	{
		if(nestit!=m_MarginRatioLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MarginRatioLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasFundNetValueRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_FundNetValueLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_FundNetValueLevel.find(reKey);
	if(nestit!=m_FundNetValueLevel.end())
	{
		if(nestit!=m_FundNetValueLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_FundNetValueLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasTradeForbidRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID) const
{
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>::const_iterator it;
	bool bRet=false;
	m_mutex_TradeForbidLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	it=m_TradeForbidLevel.find(reKey);
	if(it!=m_TradeForbidLevel.end())
	{
		const std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::const_iterator nestit=nestmap.find(InstrumentID);
		if(nestit!=nestmap.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_TradeForbidLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasMarginForbidRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_MarginForbidLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_MarginForbidLevel.find(reKey);
	if(nestit!=m_MarginForbidLevel.end())
	{
		if(nestit!=m_MarginForbidLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MarginForbidLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasLossForbidRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_LossForbidLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_LossForbidLevel.find(reKey);
	if(nestit!=m_LossForbidLevel.end())
	{
		if(nestit!=m_LossForbidLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_LossForbidLevel.read_unlock();
	return bRet;
}

bool CRiskProcess::HasOnedayLargetsLossRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_OnedayLargetsLossLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_OnedayLargetsLossLevel.find(reKey);
	if(nestit!=m_OnedayLargetsLossLevel.end())
	{
		if(nestit!=m_OnedayLargetsLossLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_OnedayLargetsLossLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasContractsValuesRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_ContractsValuesLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_ContractsValuesLevel.find(reKey);
	if(nestit!=m_ContractsValuesLevel.end())
	{
		if(nestit!=m_ContractsValuesLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_ContractsValuesLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasMaxRetraceRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_MaxRetraceLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_MaxRetraceLevel.find(reKey);
	if(nestit!=m_MaxRetraceLevel.end())
	{
		if(nestit!=m_ContractsValuesLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MaxRetraceLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasBullBearValuesRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_BullBearValuesLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_BullBearValuesLevel.find(reKey);
	if(nestit!=m_BullBearValuesLevel.end())
	{
		if(nestit!=m_BullBearValuesLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_BullBearValuesLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasLossContinueDaysRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const
{
	bool bRet=false;
	m_mutex_LossContinueDaysLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_LossContinueDaysLevel.find(reKey);
	if(nestit!=m_LossContinueDaysLevel.end())
	{
		if(nestit!=m_LossContinueDaysLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_LossContinueDaysLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasLossMaxRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd)const
{
	bool bRet=false;
	m_mutex_MaxRiskLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_MaxRiskLevel.find(reKey);
	if(nestit!=m_MaxRiskLevel.end())
	{
		if(nestit!=m_MaxRiskLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MaxRiskLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasLossPercentRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd)const
{
	bool bRet=false;
	m_mutex_LossPercentLevel.read_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::const_iterator nestit=m_LossPercentLevel.find(reKey);
	if(nestit!=m_LossPercentLevel.end())
	{
		if(nestit!=m_LossPercentLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_LossPercentLevel.read_unlock();
	return bRet;
}
//检查相关持仓明细最大持仓时间风险是否存在
bool CRiskProcess::HasMaxTime_PosiDetailRisk(const string& InvestorID, const PositionDetailKey& posiDetailKey)
{
	std::map<string, std::map<PositionDetailKey, RiskEventLevelID>>::const_iterator it;
	std::map<PositionDetailKey, RiskEventLevelID>::const_iterator itmap;
	bool bRet=false;

	m_mutex_MaxTimePosiDetailLevel.read_lock();	
	it=m_mapMaxTimePosiDetailLevel.find(InvestorID);
	if(it!=m_mapMaxTimePosiDetailLevel.end()) {
		itmap=it->second.find(posiDetailKey);
		if(itmap!=it->second.end())
			bRet = itmap->second.LevelID>0;
	}

	m_mutex_MaxTimePosiDetailLevel.read_unlock();

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
RiskEventIDType CRiskProcess::GetMaxPositionEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID, int levelID, int& nEventOrgID)
{
	RiskEventIDType EventID=0;
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>::iterator it;
	m_mutex_MaxPositionLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	it=m_MaxPositionLevel.find(reKey);
	if(it!=m_MaxPositionLevel.end())
	{
		std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::iterator nestit=nestmap.find(InstrumentID);
		if(nestit!=nestmap.end())
		{
			EventLevelID& evtlvl=nestit->second;
			EventID=FillEventLevelID(evtlvl,levelID);
			nEventOrgID = evtlvl.EventID;//已有的风险事件ID
		}
		else
		{
			SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
			EventLevelID evtlvl;
			evtlvl.EventID=EventID;
			evtlvl.LevelID=levelID;
			nestmap[InstrumentID]=evtlvl;
		}
	}
	else
	{
		std::map<std::string,EventLevelID> nestmap;
		EventLevelID evtlvl;
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		nestmap[InstrumentID]=evtlvl;
		m_MaxPositionLevel[reKey]=nestmap;
	}
	m_mutex_MaxPositionLevel.write_unlock();
	return EventID;
}

RiskEventIDType CRiskProcess::GetMarketShockEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>::iterator it;
	m_mutex_MarketShockLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	it=m_MarketShockLevel.find(reKey);
	if(it!=m_MarketShockLevel.end())
	{
		std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::iterator nestit=nestmap.find(InstrumentID);
		if(nestit!=nestmap.end())
		{
			EventLevelID& evtlvl=nestit->second;
			EventID = FillEventLevelID(evtlvl,levelID);
			nEventOrgID = evtlvl.EventID;//已有的风险事件ID
		}
		else
		{
			SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
			EventLevelID evtlvl;
			evtlvl.EventID=EventID;
			evtlvl.LevelID=levelID;
			nestmap[InstrumentID]=evtlvl;
		}
	}
	else
	{
		std::map<std::string,EventLevelID> nestmap;
		EventLevelID evtlvl;
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		nestmap[InstrumentID]=evtlvl;
		m_MarketShockLevel[reKey]=nestmap;
	}
	m_mutex_MarketShockLevel.write_unlock();
	return EventID;
}

RiskEventIDType CRiskProcess::GetAccountLossEventID(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_AccountLossLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_AccountLossLevel.find(reKey);
	if(nestit!=m_AccountLossLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_AccountLossLevel[reKey]=evtlvl;
	}
	m_mutex_AccountLossLevel.write_unlock();
	return EventID;
}

RiskEventIDType CRiskProcess::GetTodayLossEventID(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_TodayLossLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_TodayLossLevel.find(reKey);
	if(nestit!=m_TodayLossLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_TodayLossLevel[reKey]=evtlvl;
	}
	m_mutex_TodayLossLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetGappedMarketEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID  )
{
	RiskEventIDType EventID=0;
	m_mutex_GappedMarketLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_GappedMarketLevel.find(reKey);
	if(nestit!=m_GappedMarketLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_GappedMarketLevel[reKey]=evtlvl;
	}
	m_mutex_GappedMarketLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetMarginRatioEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_MarginRatioLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_MarginRatioLevel.find(reKey);
	if(nestit!=m_MarginRatioLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_MarginRatioLevel[reKey]=evtlvl;
	}
	m_mutex_MarginRatioLevel.write_unlock();
	return EventID;
}

RiskEventIDType CRiskProcess::GetFundNetValueEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_FundNetValueLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_FundNetValueLevel.find(reKey);
	if(nestit!=m_FundNetValueLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_FundNetValueLevel[reKey]=evtlvl;
	}
	m_mutex_FundNetValueLevel.write_unlock();
	return EventID;

}
RiskEventIDType CRiskProcess::GetTradeForbidEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>::iterator it;
	m_mutex_TradeForbidLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;
	it=m_TradeForbidLevel.find(reKey);
	if(it!=m_TradeForbidLevel.end())
	{
		std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::iterator nestit=nestmap.find(InstrumentID);
		if(nestit!=nestmap.end())
		{
			EventLevelID& evtlvl=nestit->second;
			EventID=FillEventLevelID(evtlvl,levelID);
			nEventOrgID = evtlvl.EventID;//已有的风险事件ID
		}
		else
		{
			SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
			EventLevelID evtlvl;
			evtlvl.EventID=EventID;
			evtlvl.LevelID=levelID;
			nestmap[InstrumentID]=evtlvl;
		}
	}
	else
	{
		std::map<std::string,EventLevelID> nestmap;
		EventLevelID evtlvl;
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		nestmap[InstrumentID]=evtlvl;
		m_TradeForbidLevel[reKey]=nestmap;
	}
	m_mutex_TradeForbidLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetMarginForbidEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_MarginForbidLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_MarginForbidLevel.find(reKey);
	if(nestit!=m_MarginForbidLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_MarginForbidLevel[reKey]=evtlvl;
	}
	m_mutex_MarginForbidLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetLossForbidEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_LossForbidLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_LossForbidLevel.find(reKey);
	if(nestit!=m_LossForbidLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_LossForbidLevel[reKey]=evtlvl;
	}
	m_mutex_LossForbidLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetContractsValuesEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_ContractsValuesLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_ContractsValuesLevel.find(reKey);
	if(nestit!=m_ContractsValuesLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_ContractsValuesLevel[reKey]=evtlvl;
	}
	m_mutex_ContractsValuesLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetBullBearValuesEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_BullBearValuesLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_BullBearValuesLevel.find(reKey);
	if(nestit!=m_BullBearValuesLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_BullBearValuesLevel[reKey]=evtlvl;
	}
	m_mutex_BullBearValuesLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetLossContinueDaysEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_LossContinueDaysLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_LossContinueDaysLevel.find(reKey);
	if(nestit!=m_LossContinueDaysLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_LossContinueDaysLevel[reKey]=evtlvl;
	}
	m_mutex_LossContinueDaysLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetLossMaxEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_MaxRiskLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_MaxRiskLevel.find(reKey);
	if(nestit!=m_MaxRiskLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_MaxRiskLevel[reKey]=evtlvl;
	}
	m_mutex_MaxRiskLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetMaxRetraceEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_MaxRetraceLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_MaxRetraceLevel.find(reKey);
	if(nestit!=m_MaxRetraceLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_MaxRetraceLevel[reKey]=evtlvl;
	}
	m_mutex_MaxRetraceLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetLossPercentEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID )
{
	RiskEventIDType EventID=0;
	m_mutex_LossPercentLevel.write_lock();
	BrokerAccountKey BrokerKey(BrokerID.c_str(), InvestorID.c_str());
	SRiskEventKey reKey;
	reKey.baKey			= BrokerKey;
	reKey.strTimeBegin	= strTimeBegin;
	reKey.strTimeEnd	= strTimeEnd;	
	std::map<SRiskEventKey,EventLevelID>::iterator nestit=m_LossPercentLevel.find(reKey);
	if(nestit!=m_LossPercentLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
		nEventOrgID = evtlvl.EventID;//已有的风险事件ID
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_LossPercentLevel[reKey]=evtlvl;
	}
	m_mutex_LossPercentLevel.write_unlock();
	return EventID;
}

RiskEventIDType CRiskProcess::GetMaxTime_PosiDetailRisk(const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const PositionDetailKey& posiDetailKey, int levelID, double fRiskVal, int& nEventOrgID)
{
	RiskEventIDType EventID=0;

	m_mutex_MaxTimePosiDetailLevel.write_lock();
	std::map<string, std::map<PositionDetailKey, RiskEventLevelID>>::iterator it = 
			m_mapMaxTimePosiDetailLevel.find(InvestorID);
	if(it != m_mapMaxTimePosiDetailLevel.end()) {
		std::map<PositionDetailKey, RiskEventLevelID>::iterator itmap = it->second.find(posiDetailKey);
		if(itmap != it->second.end()) {
			RiskEventLevelID& evtlvl = itmap->second;
			nEventOrgID = evtlvl.EventID;//已有的风险事件ID
			if(evtlvl.LevelID==0 && levelID>0) {
				EventLevelID evtlvl2;
				memset(&evtlvl2, 0, sizeof(evtlvl2));
				evtlvl2.EventID = evtlvl.EventID;
				evtlvl2.LevelID = evtlvl.LevelID;
				EventID = FillEventLevelID(evtlvl2, levelID);
				if(EventID != -1) {
					evtlvl.EventID = evtlvl2.EventID;
					evtlvl.LevelID = evtlvl2.LevelID;
				}
			}
			else if(evtlvl.LevelID!=levelID) {
				EventID = evtlvl.EventID;
				evtlvl.LevelID = levelID;
				evtlvl.RiskVal = fRiskVal;
			}
			else if(levelID!=0 && evtlvl.LevelID==levelID && !util::equals(itmap->second.RiskVal, fRiskVal)) {
				EventID = evtlvl.EventID;
				evtlvl.RiskVal = fRiskVal;
			}
			else {
				EventID = -1;
			}
		}
		else {
			SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
			RiskEventLevelID evtlvl;
			evtlvl.EventID = EventID;
			evtlvl.LevelID = levelID;
			evtlvl.RiskVal = fRiskVal;
			it->second.insert(make_pair(posiDetailKey, evtlvl));
		}
	}
	else {
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		RiskEventLevelID evtlvl;
		evtlvl.EventID = EventID;
		evtlvl.LevelID = levelID;
		evtlvl.RiskVal = fRiskVal;
		std::map<PositionDetailKey, RiskEventLevelID> mapTemp;
		mapTemp.insert(make_pair(posiDetailKey, evtlvl));
		m_mapMaxTimePosiDetailLevel.insert(make_pair(InvestorID, mapTemp));
	}
	
	m_mutex_MaxTimePosiDetailLevel.write_unlock();
	
	return EventID;
}

std::map<std::string, std::map<PositionDetailKey, RiskEventLevelID>>& CRiskProcess::GetEfficaciousRisk_MaxTimerPosiDetail_Keys()
{
	return m_mapMaxTimePosiDetailLevel;
}

bool CRiskProcess::GetEfficaciousRisk_MaxTimerPosiDetail_Keys(const std::string& InvestorID, 
															  std::set<PositionDetailKey>& setRet)
{
	setRet.clear();

	m_mutex_MaxTimePosiDetailLevel.read_lock();
	std::map<string, std::map<PositionDetailKey, RiskEventLevelID>>::iterator it = 
			m_mapMaxTimePosiDetailLevel.find(InvestorID);
	if(it != m_mapMaxTimePosiDetailLevel.end()) {
		std::map<PositionDetailKey, RiskEventLevelID>::iterator itmap;
		for(itmap = it->second.begin(); itmap != it->second.end(); itmap++) {
			if(itmap->second.LevelID>0)
				setRet.insert(itmap->first);
		}
	}
	
	m_mutex_MaxTimePosiDetailLevel.read_unlock();

	return setRet.size()>0;
}

//////////////////////////////////////////////////////////////////////////
void CRiskProcess::AddMaxPositionEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,const std::string& InstrumentID,
									   double dblValue,int nVolume, int levelID, int& nEventID, SResponse& vecThisRisk)
{

	int nEventOrgID = 0;
	nEventID= GetMaxPositionEventID(BrokerID, InvestorID, strTimeBegin, strTimeEnd, InstrumentID,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			SendRiskEvent_Same(RI_MaxPosition,BrokerID, InvestorID, strTimeBegin, strTimeEnd,InstrumentID,nEventOrgID,nVolume,levelID, 0);
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_MaxPosition,BrokerID, InvestorID, strTimeBegin, strTimeEnd,InstrumentID,nEventID,nVolume,levelID, nResponseType, 0);

	if(levelID == 0)//风险消失
		CDataCenter::Get()->AddResponse(InvestorID, RI_MaxPosition, InstrumentID, NULL, vecThisRisk, false);	
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_MaxPosition, InstrumentID, NULL, vecThisRisk, true);	
}
void CRiskProcess::AddMarketShockEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,const std::string& InstrumentID,
									   double dblValue,int nVolume,int levelID, int& nEventID,SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetMarketShockEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,InstrumentID,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			SendRiskEvent_Same(RI_MarketShock,BrokerID,InvestorID, strTimeBegin, strTimeEnd,InstrumentID,nEventOrgID,dblValue,levelID, 0);
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_MarketShock,BrokerID,InvestorID, strTimeBegin, strTimeEnd,InstrumentID,nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_MarketShock, InstrumentID, NULL, vecThisRisk, false);	
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_MarketShock, InstrumentID, NULL, vecThisRisk, true);	
}
void CRiskProcess::AddAccountLossEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk, bool bDoResponse)
{
	int nEventOrgID = 0;
	nEventID= GetAccountLossEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(bDoResponse)//主要是账户限亏，新开仓的也要强平掉，所以重复做这个动作
			CDataCenter::Get()->AddResponse(InvestorID, RI_AccountLoss, "", NULL, vecThisRisk, true);	
		if(levelID != 0)
			SendRiskEvent_Same(RI_AccountLoss,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventOrgID,dblValue,levelID, 0);
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_AccountLoss,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_AccountLoss, "", NULL, vecThisRisk, false);	
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_AccountLoss, "", NULL, vecThisRisk, true);	
}
void CRiskProcess::AddTodayLossEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk, bool bDoResponse)
{
	int nEventOrgID = 0;
	nEventID= GetTodayLossEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(bDoResponse)//主要是今日限亏，新开仓的也要强平掉，所以重复做这个动作
			CDataCenter::Get()->AddResponse(InvestorID, RI_TodayLoss, "", NULL, vecThisRisk, true);	
		if(levelID != 0)
			SendRiskEvent_Same(RI_TodayLoss,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventOrgID,dblValue,levelID, 0);
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_TodayLoss,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_TodayLoss, "", NULL, vecThisRisk, false);		
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_TodayLoss, "", NULL, vecThisRisk, true);	
}

void CRiskProcess::AddGappedMarketEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetGappedMarketEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			SendRiskEvent_Same(RI_GappedMarket,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventOrgID,dblValue,levelID, 0);
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_GappedMarket,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_GappedMarket, "", NULL, vecThisRisk, false);		
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_GappedMarket, "", NULL, vecThisRisk, true);	
}
void CRiskProcess::AddMarginRatioEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetMarginRatioEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			SendRiskEvent_Same(RI_MarginRatio,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventOrgID,dblValue,levelID, 0);
		OutputDebugString("AddMarginRatioEvent1\n");
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_MarginRatio,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	OutputDebugString("AddMarginRatioEvent2\n");
	if(levelID == 0)
		OutputDebugString("AddMarginRatioEvent3\n");
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_MarginRatio, "", NULL, vecThisRisk, false);		
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_MarginRatio, "", NULL, vecThisRisk, true);	
}
void CRiskProcess::AddFundNetValueEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk, bool bDoResponse)
{
	int nEventOrgID = 0;
	nEventID= GetFundNetValueEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(bDoResponse)//主要是基金净值，新开仓的也要强平掉，所以重复做这个动作
			CDataCenter::Get()->AddResponse(InvestorID, RI_FundNetValue, "", NULL, vecThisRisk, true);	
		if(levelID != 0)
			SendRiskEvent_Same(RI_FundNetValue,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventOrgID,dblValue,levelID, 0);
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_FundNetValue,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_FundNetValue, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_FundNetValue, "", NULL, vecThisRisk, true);	
}
void CRiskProcess::AddTradeForbidEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetTradeForbidEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,InstrumentID, levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			SendRiskEvent_Same(RI_TRADE_FORBID,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventOrgID,dblValue,levelID, 0);
		//	OutputDebugString("AddMarginRatioEvent1\n");
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_TRADE_FORBID,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	//OutputDebugString("AddMarginRatioEvent2\n");
	if(levelID == 0)
		OutputDebugString("AddMarginRatioEvent3\n");
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_TRADE_FORBID, "", NULL, vecThisRisk, false);		
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_TRADE_FORBID, "", NULL, vecThisRisk, true);

}
void CRiskProcess::AddMarginForbidEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue, double dblValue2,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetMarginForbidEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_MARGIN_FORBID, "", NULL, vecThisRisk, true);	
	
		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_MARGIN_FORBID,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, dblValue2);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_MARGIN_FORBID, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_MARGIN_FORBID, "", NULL, vecThisRisk, true);	
}
void CRiskProcess::AddLossForbidEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue, double dblValue2,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetLossForbidEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_FORBID, "", NULL, vecThisRisk, true);	

		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_LOSS_FORBID,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, dblValue2);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_FORBID, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_FORBID, "", NULL, vecThisRisk, true);	
}
void CRiskProcess::AddOnedayLargetsLossEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetLossForbidEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_ONEDAY_LARGESTLOSS, "", NULL, vecThisRisk, true);	

		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_ONEDAY_LARGESTLOSS,BrokerID,InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_ONEDAY_LARGESTLOSS, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_ONEDAY_LARGESTLOSS, "", NULL, vecThisRisk, true);	
}
void CRiskProcess::AddContractsValuesEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetContractsValuesEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_CONTRACTS_VALUES, "", NULL, vecThisRisk, true);	

		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_CONTRACTS_VALUES, BrokerID, InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_CONTRACTS_VALUES, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_CONTRACTS_VALUES, "", NULL, vecThisRisk, true);
}
void CRiskProcess::AddBullBearValuesEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetBullBearValuesEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_BULLBEAR_VALUES, "", NULL, vecThisRisk, true);	

		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_BULLBEAR_VALUES, BrokerID, InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_BULLBEAR_VALUES, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_BULLBEAR_VALUES, "", NULL, vecThisRisk, true);
}
void CRiskProcess::AddLossContinueDaysEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetLossContinueDaysEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_CONTINUEDAYS, "", NULL, vecThisRisk, true);	

		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_LOSS_CONTINUEDAYS, BrokerID, InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_CONTINUEDAYS, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_CONTINUEDAYS, "", NULL, vecThisRisk, true);
}
void CRiskProcess::AddLossMaxEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetLossMaxEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_LOSSMAXVALUE, "", NULL, vecThisRisk, true);	

		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_LOSSMAXVALUE, BrokerID, InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_LOSSMAXVALUE, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_LOSSMAXVALUE, "", NULL, vecThisRisk, true);
}
void CRiskProcess::AddMaxRetraceEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetMaxRetraceEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_MAXRETRACE, "", NULL, vecThisRisk, true);	

		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_MAXRETRACE, BrokerID, InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_MAXRETRACE, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_MAXRETRACE, "", NULL, vecThisRisk, true);
}
void CRiskProcess::AddLossPercentEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk)
{
	int nEventOrgID = 0;
	nEventID= GetLossPercentEventID(BrokerID,InvestorID, strTimeBegin, strTimeEnd,levelID, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_PERCENT, "", NULL, vecThisRisk, true);	

		return;//重复事件，不发送
	}
	int  nResponseType = GetResponseType(vecThisRisk);
	SendRiskEvent(RI_LOSS_PERCENT, BrokerID, InvestorID, strTimeBegin, strTimeEnd,"",nEventID,dblValue,levelID, nResponseType, 0);
	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_PERCENT, "", NULL, vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_LOSS_PERCENT, "", NULL, vecThisRisk, true);
}

void CRiskProcess::AddMaxTime_PosiDetailRisk(const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const PositionDetailKey& posiDetailKey, int levelID, int& nEventID, double fRiskVal, double fRealVal, SResponse& vecThisRisk)
{
	char strText[1024] = {0};
	sprintf_s(strText, sizeof(strText), "%s,%s,%s,%s", posiDetailKey.InstrumentID, posiDetailKey.OpenDate, 
		posiDetailKey.TradeID, posiDetailKey.Direction==THOST_FTDC_D_Buy ? "买" : "卖");

	int nEventOrgID = 0;
	nEventID= GetMaxTime_PosiDetailRisk(InvestorID, strTimeBegin, strTimeEnd, posiDetailKey, levelID, fRiskVal, nEventOrgID);
	if(nEventID<0)
	{
		if(levelID != 0)
			SendRiskEvent_Same(RI_SingleHoldTime,"",InvestorID, strTimeBegin, strTimeEnd,strText,nEventOrgID,fRealVal,levelID, 0);
		return;//重复事件，不发送
	}
	int nResponseType = GetResponseType(vecThisRisk);;//单笔持仓目前没有任何动作,需要重新开发
	SendRiskEvent(RI_SingleHoldTime,"",InvestorID, strTimeBegin, strTimeEnd,strText,nEventID,fRealVal,levelID, nResponseType, 0);

	if(levelID == 0)
		CDataCenter::Get()->AddResponse(InvestorID, RI_SingleHoldTime, "", const_cast<PositionDetailKey*>(&posiDetailKey), vecThisRisk, false);
	else
		CDataCenter::Get()->AddResponse(InvestorID, RI_SingleHoldTime, "", const_cast<PositionDetailKey*>(&posiDetailKey), vecThisRisk, true);	
}

//////////////////////////////////////////////////////////////////////////
RiskEventIDType CRiskProcess::FillEventLevelID(EventLevelID& evtlvl,int levelID )
{
	RiskEventIDType EventID=0;
	if(levelID==evtlvl.LevelID)
		EventID=-1;//相同风控事件,不重新发送
	else
	{
		if(evtlvl.LevelID==0)
		{
			SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
			evtlvl.EventID=EventID;
		}
		else
			EventID=evtlvl.EventID;//以相同ID更新状态
		evtlvl.LevelID=levelID;
	}
	return EventID;
}
void CRiskProcess::SendRiskEvent(RiskIndicatorType type, const std::string & BrokerID, const string& InvestorID,std::string& strTimeBegin, std::string& strTimeEnd,
								 const std::string& InstrumentID,RiskEventIDType EventID,
								 double dblValue,int levelID, int& nResponseType, double dblValue2)
{
	RiskEvent evt;
	memset(&evt,0,sizeof(RiskEvent));
	RiskEventHandling handle;
	memset(&handle,0,sizeof(RiskEventHandling));
	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));

	bool bSendMsg=false;
	//具体内容	
	evt.nRiskEventID = EventID;
	evt.nRiskEventSubID=CDataCenter::Get()->GetRiskEventSubID(EventID);
	evt.lEventTime=time(NULL);

	UserInfo userInfo;
	CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(InvestorID, userInfo);	
	evt.nTradeInvestorID = userInfo.nUserID;	
	strcpy(evt.BrokerID, BrokerID.c_str());
	strcpy(evt.cTimeBegin, strTimeBegin.c_str());
	strcpy(evt.cTimeEnd, strTimeEnd.c_str());
	
	evt.nRiskIndicatorID=type;
	safestrcpy(evt.InstrumentID,sizeof(evt.InstrumentID),InstrumentID.c_str());
	evt.nRiskLevelID=levelID;
	evt.dblIndicatorValue=dblValue;//触发值	
	evt.dblIndicatorCurrentValue=dblValue;//触发值	
	evt.nIsValid=RiskEvent_Realtime;
	evt.nResponseType = nResponseType;	

	evt.dblIndicatorValue2=dblValue2;//触发值	
	evt.dblIndicatorCurrentValue2=dblValue2;//触发值	
	if(levelID>0)
	{
		if((WarningType_ShowMsg & nResponseType) == WarningType_ShowMsg)
			bSendMsg = true;
		evt.nMsgSendStatus=evt.nRiskEventSubID==0?MsgStatus_New:MsgStatus_ChangeLevel;
		SAFE_GET_DATACENTER()->NewActiveEvent(evt);
	}
	else
	{
		evt.nMsgSendStatus=MsgStatus_Delete;
		SAFE_GET_DATACENTER()->DeleteActiveEvent(evt);
	}

	//notice
	if(bSendMsg)
	{
		safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"风险事件提示");
		std::string strName;
		SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)type, strName);
		sprintf(MsgInfo.szContent,"交易员:%s;\n 触发风险类型:%s;\n 风险等级:%d", InvestorID.c_str(), strName.c_str(), levelID);
	}
	
	char AccIDOrInst[256];
	sprintf_s(AccIDOrInst,256,"%d",userInfo.nUserID);	

	const RiskEvent& Data= evt;
	int RspCmdID= Cmd_RM_RiskEvent_Push;
	std::set<SOCKET>::const_iterator it;
	std::set<SOCKET> sset;
	m_pThreadSharedData->GetSubscribeData(AccIDOrInst, SubscribeID_RiskEvent, sset);
	for (it=sset.begin();it!=sset.end();++it)
	{//发送风险事件
		CInterface_SvrTcp::getObj().SendPkgData(*it, RspCmdID,(void*)&Data,sizeof(Data), 0, 0, CF_ERROR_SUCCESS, 0, 0);
	}	    
	if(bSendMsg)
	{
		int nOrgID = -1;
		if(!CInterface_SvrUserOrg::getObj().GetOrgIDByAccount(InvestorID, nOrgID))
			return;
	
		set<int> setRealUsers;
		CInterface_SvrUserOrg::getObj().GetMonitorUserIDByOrgID(nOrgID, setRealUsers);
		
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);		
		int nExpiredDate = sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;

		CInterface_SvrMsg::getObj().SendMsg(0, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
	}
}
void CRiskProcess::SendRiskEvent_Same(RiskIndicatorType type, const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,
								 const std::string& InstrumentID,RiskEventIDType EventID,
								 double dblValue,int levelID, double dblValue2)
{
	RiskEvent evtExist;
	bool bGet = CDataCenter::Get()->GetActiveEvent(EventID, evtExist);

	RiskEvent evt;
	memset(&evt,0,sizeof(RiskEvent));	

	//具体内容	
	evt.nRiskEventID = EventID;
	evt.nRiskEventSubID=CDataCenter::Get()->GetRiskEventSubID(EventID);
	if(evt.nRiskEventSubID >=1)
		evt.nRiskEventSubID= evt.nRiskEventSubID - 1;
	if(bGet)
		evt.lEventTime=evtExist.lEventTime;
	else
		evt.lEventTime=time(NULL);

	UserInfo userInfo;
	CInterface_SvrUserOrg::getObj().GetUserInfoByAccount(InvestorID, userInfo);	
	evt.nTradeInvestorID = userInfo.nUserID;	
	strcpy(evt.BrokerID, BrokerID.c_str());
	strcpy(evt.cTimeBegin, strTimeBegin.c_str());
	strcpy(evt.cTimeEnd, strTimeEnd.c_str());

	evt.nRiskIndicatorID=type;
	safestrcpy(evt.InstrumentID,sizeof(evt.InstrumentID),InstrumentID.c_str());
	evt.nRiskLevelID=levelID;
	evt.dblIndicatorValue=dblValue;//触发值	
	evt.dblIndicatorCurrentValue=dblValue;//触发值	
	evt.nIsValid=RiskEvent_Realtime;
	evt.nResponseType = 0;

	if(levelID>0)
	{		
		if(bGet)
			evt.nMsgSendStatus==evtExist.nMsgSendStatus;		
		SAFE_GET_DATACENTER()->ChangeActiveEvent(evt);
	}

	char AccIDOrInst[256];
	sprintf_s(AccIDOrInst,256,"%d",userInfo.nUserID);

	const RiskEvent& Data= evt;
	int RspCmdID= Cmd_RM_RiskEvent_Push;
	std::set<SOCKET>::const_iterator it;
	std::set<SOCKET> sset;
	m_pThreadSharedData->GetSubscribeData(AccIDOrInst, SubscribeID_RiskEvent, sset);
	for (it=sset.begin();it!=sset.end();++it)
	{//发送风险事件
		CInterface_SvrTcp::getObj().SendPkgData(*it, RspCmdID, (void*)&Data,sizeof(Data), 0, 0, CF_ERROR_SUCCESS, 0, 0);
	}
}
void CRiskProcess::SendRiskEvent(RiskEvent& rE)
{	
	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));
	bool bSendMsg=false;	
	rE.lEventTime=time(NULL);
	if(rE.nRiskLevelID>0)
	{
		if((WarningType_ShowMsg & rE.nResponseType) == WarningType_ShowMsg)
			bSendMsg = true;
		rE.nMsgSendStatus=rE.nRiskEventSubID==0?MsgStatus_New:MsgStatus_ChangeLevel;
		SAFE_GET_DATACENTER()->NewActiveEvent(rE);
	}
	else
	{
		rE.nMsgSendStatus=MsgStatus_Delete;
		SAFE_GET_DATACENTER()->DeleteActiveEvent(rE);
	}

	UserInfo userInfo;
	CInterface_SvrUserOrg::getObj().GetUserByID(rE.nTradeInvestorID, userInfo);	
	//notice
	if(bSendMsg)
	{
		safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),"风险事件提示");
		std::string strName;
		SAFE_GET_DATACENTER()->GetRiskIndicatorName((int)rE.nRiskIndicatorID, strName);
		sprintf(MsgInfo.szContent,"交易员:%s;\n 触发风险类型:%s;\n 风险等级:%d", userInfo.szAccount, strName.c_str(), rE.nRiskLevelID);
	}

	char AccIDOrInst[256];
	sprintf_s(AccIDOrInst,256,"%d",rE.nTradeInvestorID);	

	const RiskEvent& Data= rE;
	int RspCmdID= Cmd_RM_RiskEvent_Push;
	std::set<SOCKET>::const_iterator it;
	std::set<SOCKET> sset;
	m_pThreadSharedData->GetSubscribeData(AccIDOrInst, SubscribeID_RiskEvent, sset);
	for (it=sset.begin();it!=sset.end();++it)
	{//发送风险事件
		CInterface_SvrTcp::getObj().SendPkgData(*it, RspCmdID,(void*)&Data,sizeof(Data), 0, 0, CF_ERROR_SUCCESS, 0, 0);
	}	    
	if(bSendMsg)
	{
		int nOrgID = -1;
		if(!CInterface_SvrUserOrg::getObj().GetOrgIDByUserID(rE.nTradeInvestorID, nOrgID))
			return;

		set<int> setRealUsers;
		CInterface_SvrUserOrg::getObj().GetMonitorUserIDByOrgID(nOrgID, setRealUsers);

		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);		
		int nExpiredDate = sysTime.wYear*10000 + sysTime.wMonth*100 + sysTime.wDay;

		CInterface_SvrMsg::getObj().SendMsg(0, setRealUsers, conAutoMsg, nExpiredDate, 
			MsgInfo.szTitle, MsgInfo.szContent);
	}
}
int CRiskProcess::GetResponseType(SResponse& response)
{
	int nResponse = 0;//风控动作
	std::map<int,RiskPlan>::iterator it = response.mapResponse.begin();
	if(it != response.mapResponse.end())
	{
		nResponse = it->second.WaringLevel.nResponseType;
	}
	return nResponse;
}

