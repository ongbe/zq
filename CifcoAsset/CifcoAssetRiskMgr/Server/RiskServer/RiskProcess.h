#pragma once
#include "DataBase.h"

//class UserApi;
class CRiskProcess:CDataBase
{
public:
	CRiskProcess();
	~CRiskProcess();

	//判断当前是否存在风险事件
	bool HasMaxPositionRisk( const std::string& InvestorID, const std::string& InstrumentID) const;
	bool HasFundLossRisk( const std::string& InvestorID) const;
	bool HasGappedMarketRisk( const std::string& InvestorID) const;
	bool HasMarginRatioRisk( const std::string& InvestorID) const;
	bool HasMaxProductPositionRisk( const std::string& InvestorID, const std::string& ProductID) const;
	bool HasFundNetValueRisk( const std::string& InvestorID) const;

	void AddMaxPositionEvent(const std::string& InvestorID,const std::string& InstrumentID,
		double dblValue,int nVolume,int levelID);
	void AddFundLossEvent(const std::string& InvestorID,double dblValue,int levelID);
	void AddGappedMarketEvent(const std::string& InvestorID,double dblValue,int levelID);
	void AddMarginRatioEvent(const std::string& InvestorID,double dblValue,int levelID);
	void AddMaxProductPositionEvent(const std::string& InvestorID,const std::string& ProductID,
		double dblValue,int levelID);
	void AddFundNetValueEvent(const std::string& InvestorID,
		double dblValue,int levelID);
protected:
	RiskEventIDType GetMaxPositionEventID( const std::string& InvestorID, const std::string& InstrumentID, int levelID );
	RiskEventIDType GetFundLossEventID( const std::string& InvestorID, int levelID );
	RiskEventIDType GetGappedMarketEventID( const std::string& InvestorID, int levelID );
	RiskEventIDType GetMarginRatioEventID( const std::string& InvestorID, int levelID );
	RiskEventIDType GetMaxProductPositionEventID( const std::string& InvestorID, const std::string& ProductID, int levelID );
	RiskEventIDType GetFundNetValueEventID( const std::string& InvestorID, int levelID );

	RiskEventIDType FillEventLevelID(EventLevelID& evtlvl,int levelID );
	void SendRiskEvent(RiskIndicatorType type,const std::string& InvestorID,
		const std::string& InstrumentID,RiskEventIDType EventID,
		double dblValue,int levelID);

protected:
	//判断是否需要强平
	bool NeedForceClose(const std::string& InvestorID, double dbValue);
	//强平操作
	void ForceClose(const std::string& InvestorID);
private:
	std::map<std::string,std::map<std::string,EventLevelID>>	m_MaxPositionLevel;
	CReadWriteLock		m_mutex_MaxPositionLevel;
	std::map<std::string,EventLevelID>						m_FundLossLevel;
	CReadWriteLock		m_mutex_FundLossLevel;
	std::map<std::string,EventLevelID>						m_GappedMarketLevel;
	CReadWriteLock		m_mutex_GappedMarketLevel;
	std::map<std::string,EventLevelID>						m_MarginRatioLevel;
	CReadWriteLock		m_mutex_MarginRatioLevel;

	std::map<std::string,std::map<std::string,EventLevelID>>	m_MaxProductPositionLevel;
	CReadWriteLock		m_mutex_MaxProductPositionLevel;
	
	std::map<std::string,EventLevelID>						m_FundNetValueLevel;
	CReadWriteLock		m_mutex_FundNetValueLevel;
};
