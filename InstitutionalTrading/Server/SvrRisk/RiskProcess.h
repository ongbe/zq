#pragma once
class CThreadSharedData;
struct SRiskEventKey
{
	BrokerAccountKey baKey;
	std::string      strTimeBegin;
	std::string		 strTimeEnd;

	SRiskEventKey& operator= (const SRiskEventKey& other)
	{
		if (this == &other)
			return *this;

		memcpy((char*)this,(char*)&other,sizeof(*this));
		return *this;
	};

	bool operator< (const SRiskEventKey& r) const
	{	
		if(baKey < r.baKey)
			return true;
		else if(baKey > r.baKey)
			return false;

		int irlt;
		irlt=strncmp(strTimeBegin.c_str(), r.strTimeBegin.c_str(),sizeof(strTimeBegin));
		if(irlt<0) return true;
		if(irlt>0) return false;

		irlt=strncmp(strTimeEnd.c_str(), r.strTimeEnd.c_str(),sizeof(strTimeEnd));
		if(irlt<0) return true;
		if(irlt>0) return false;

		return false;
	}

	bool operator== (const SRiskEventKey& r) const
	{
		int irlt = (baKey == r.baKey);
		if(irlt!=0) return false;

		irlt=strncmp(strTimeBegin.c_str(), r.strTimeBegin.c_str(),sizeof(strTimeBegin));
		if(irlt!=0) return false;

		irlt=strncmp(strTimeEnd.c_str(), r.strTimeEnd.c_str(),sizeof(strTimeEnd));
		if(irlt!=0) return false;		

		return true;
	}
};
class CRiskProcess
{
public:
	CRiskProcess(CThreadSharedData* pThreadSharedData);
	~CRiskProcess(void);

	//得到目前有风险的合约
	void GetMaxPositionRisk(const std::string & BrokerID, const string& InvestorID,  std::set<string>& setInstrumentID);
	void GetMarketShockRisk(const std::string & BrokerID, const string& InvestorID,  std::set<string>& setInstrumentID);

	//判断当前是否存在风险事件
	bool HasMaxPositionRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID) const;
	bool HasMarketShockRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID) const;
	bool HasAccountLossRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasTodayLossRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasGappedMarketRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasMarginRatioRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasFundNetValueRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasTradeForbidRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID) const;
	bool HasMarginForbidRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasLossForbidRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasOnedayLargetsLossRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasContractsValuesRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasBullBearValuesRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasLossContinueDaysRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasLossMaxRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasMaxRetraceRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;
	bool HasLossPercentRisk(const std::string & BrokerID,  const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd) const;


	//检查相关持仓明细最大持仓时间风险是否存在
	bool HasMaxTime_PosiDetailRisk(const string& InvestorID, const PositionDetailKey& posiDetailKey);



	void AddMaxPositionEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID,
		double dblValue,int nVolume,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddMarketShockEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,const std::string& InstrumentID,
		double dblValue,int nVolume,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddAccountLossEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,	double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk, bool bDoResponse);
	void AddTodayLossEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,	double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk, bool bDoResponse);
	void AddGappedMarketEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddMarginRatioEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddFundNetValueEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk, bool bDoResponse);
	void AddTradeForbidEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID,
		double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddMarginForbidEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue, double dblValue2,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddLossForbidEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue, double dblValue2,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddOnedayLargetsLossEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddContractsValuesEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddBullBearValuesEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddLossContinueDaysEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddLossMaxEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddMaxRetraceEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);
	void AddLossPercentEvent(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, double dblValue,int levelID, int& nEventID, SResponse& vecThisRisk);

	

	void AddMaxTime_PosiDetailRisk(const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const PositionDetailKey& posiDetailKey, 
			int levelID, int& nEventID, double fRiskVal, double fRealVal, SResponse& vecThisRisk);
	
	std::map<string, std::map<PositionDetailKey, RiskEventLevelID>>& GetEfficaciousRisk_MaxTimerPosiDetail_Keys();
	bool GetEfficaciousRisk_MaxTimerPosiDetail_Keys(const std::string& InvestorID, 
			std::set<PositionDetailKey>& setRet);


private:
	RiskEventIDType GetMaxPositionEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID, int levelID, int& nEventOrgID);
	RiskEventIDType GetMarketShockEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID, int levelID, int& nEventOrgID );	
	RiskEventIDType GetAccountLossEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetTodayLossEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetGappedMarketEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetMarginRatioEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetFundNetValueEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetTradeForbidEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID, int levelID, int& nEventOrgID );
	RiskEventIDType GetMarginForbidEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetLossForbidEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetContractsValuesEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetBullBearValuesEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetLossContinueDaysEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetLossMaxEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetMaxRetraceEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );
	RiskEventIDType GetLossPercentEventID(const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, int levelID, int& nEventOrgID );

	
	
	RiskEventIDType GetMaxTime_PosiDetailRisk(const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd, 
			const PositionDetailKey& posiDetailKey, int levelID, double fRiskVal, int& nEventOrgID);


	RiskEventIDType FillEventLevelID(EventLevelID& evtlvl,int levelID );
	void SendRiskEvent(RiskIndicatorType type, const std::string & BrokerID, const string& InvestorID,std::string& strTimeBegin, std::string& strTimeEnd, const std::string& InstrumentID,RiskEventIDType EventID, double dblValue,int levelID, int& nResponseType, double dblValue2);
	void SendRiskEvent_Same(RiskIndicatorType type,const std::string & BrokerID, const string& InvestorID, std::string& strTimeBegin, std::string& strTimeEnd,
		const std::string& InstrumentID,RiskEventIDType EventID,double dblValue,int levelID, double dblValue2);
public:
	void SendRiskEvent(RiskEvent& rE);

public:
	CReadWriteLock		m_mutex_MaxTimePosiDetailLevel;

private:
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>	m_MaxPositionLevel;
	CReadWriteLock		m_mutex_MaxPositionLevel;
	
	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>	m_MarketShockLevel;
	CReadWriteLock		m_mutex_MarketShockLevel;

	std::map<SRiskEventKey, EventLevelID>							m_AccountLossLevel;
	CReadWriteLock		m_mutex_AccountLossLevel;

	std::map<SRiskEventKey, EventLevelID>							m_TodayLossLevel;
	CReadWriteLock		m_mutex_TodayLossLevel;
	
	std::map<SRiskEventKey, EventLevelID>							m_GappedMarketLevel;
	CReadWriteLock		m_mutex_GappedMarketLevel;

	std::map<SRiskEventKey, EventLevelID>							m_MarginRatioLevel;
	CReadWriteLock		m_mutex_MarginRatioLevel;	

	std::map<SRiskEventKey, EventLevelID>							m_FundNetValueLevel;
	CReadWriteLock		m_mutex_FundNetValueLevel;

	std::map<SRiskEventKey,std::map<std::string,EventLevelID>>		m_TradeForbidLevel;
	CReadWriteLock		m_mutex_TradeForbidLevel;

	std::map<SRiskEventKey, EventLevelID>							m_MarginForbidLevel;
	CReadWriteLock		m_mutex_MarginForbidLevel;

	std::map<SRiskEventKey, EventLevelID>							m_LossForbidLevel;
	CReadWriteLock		m_mutex_LossForbidLevel;

	std::map<SRiskEventKey, EventLevelID>							m_OnedayLargetsLossLevel;
	CReadWriteLock		m_mutex_OnedayLargetsLossLevel;

	std::map<SRiskEventKey, EventLevelID>							m_MaxRiskLevel;
	CReadWriteLock		m_mutex_MaxRiskLevel;

	std::map<SRiskEventKey, EventLevelID>							m_MaxRetraceLevel;
	CReadWriteLock		m_mutex_MaxRetraceLevel;


	std::map<SRiskEventKey, EventLevelID>							m_ContractsValuesLevel;
	CReadWriteLock		m_mutex_ContractsValuesLevel;

	std::map<SRiskEventKey, EventLevelID>							m_BullBearValuesLevel;
	CReadWriteLock		m_mutex_BullBearValuesLevel;

	std::map<SRiskEventKey, EventLevelID>							m_LossContinueDaysLevel;
	CReadWriteLock		m_mutex_LossContinueDaysLevel;
	
	std::map<SRiskEventKey, EventLevelID>							m_LossPercentLevel;
	CReadWriteLock		m_mutex_LossPercentLevel;

	std::map<string, std::map<PositionDetailKey, RiskEventLevelID>>	m_mapMaxTimePosiDetailLevel;

private:
	CThreadSharedData*  m_pThreadSharedData;	
	int GetResponseType(SResponse& response);
};
