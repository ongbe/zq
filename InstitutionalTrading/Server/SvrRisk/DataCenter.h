#pragma once
#include "StdAfx.h"

struct RiskKey
{
	int nOrgID;//组织机构ID
	RiskIndicatorType RiskType;//风险类型
	bool operator< (const RiskKey& r) const
	{
		if(nOrgID < r.nOrgID)
			return true;
		else if(nOrgID > r.nOrgID)
			return false;
		
		if(RiskType < r.RiskType)
			return true;
		else if(RiskType > r.RiskType)
			return false;

		return false;
	}
};

typedef struct {
	UINT nStart;
	UINT nEnd;
	UINT nSubNum;
} EXCHANGERESTTIME;


typedef std::multimap<RiskKey, SResponse> RISKWARNINGSTRUCT;
class CDataCenter
{
public:
	CDataCenter(void);
	~CDataCenter(void);
	static void Init(void);
	static void Release(void);
	static CDataCenter* Get();

	void split(const string& s, char c, vector<string>& v);
	void ReadDB();	
	void ReadRiskWarning();
	void ReadRiskIndicator();
	void ReadNetFundParam();
	void ReadAccountLossDays();

	void SaveAccountLossDays(std::string strBrokerID, std::string strAccountID, double dblBalance);
	bool GetAccountLossDays(BrokerAccountKey& bakey, AccountLossDay& accountLossDay);
	bool SetAccountLossDays(BrokerAccountKey& bakey, AccountLossDay& accountLossDay);

	void ReadEventMessageTemplate();//消息模板暂时用不上了
	void LoadExchangeRestTime();

	std::map<string, std::vector<EXCHANGERESTTIME>>& GetExchangeRestTime() 
	{
		return m_mapRestTime;
	};

	void SaveAllData();
	void Reset();//重启设置

	std::multimap<RiskKey, SResponse>		m_RiskWarning;
	CReadWriteLock						m_Mutex_RiskWarning;

	std::map<int, RiskIndicator>		m_mapRiskIndicator;
	CReadWriteLock						m_Mutex_multimapRiskIndicator;

	std::multimap<int, RiskPlan>		m_multimapOrgid2RiskPlan;//组织机构和风控方案的对应关系
	CReadWriteLock						m_Mutex_multimapOrgid2RiskPlan;

	//基金净值参数
	std::map<int,NetFundParam>				m_mapOrgID2NetFundParam;
	CReadWriteLock							m_Mutex_OrgID2NetFundParam;

	//连续亏损的天数（其实是总共亏损的天数）
	std::map<BrokerAccountKey, AccountLossDay>		m_mapAccountDay;
	CReadWriteLock									m_Mutex_AccountDay;
public:
	
	bool GetRiskIndicatorName(int nID, std::string& strName);
	bool GetRiskPlanByOrgID(int nOrgID, vector<RiskPlan>& vec);
	bool GetParentOrgID(int nOrgID, int& nParentID);
	int  GetWarningMapLevel(RiskKey& rKey, std::string strTime, double value,  SResponse& vecResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);
	bool GetAllMaxPositonZeroWarningMapLevel(RiskKey& rKey, std::string strTime, SResponse& vecResponse, std::string& strTimeBegin, std::string& strTimeEnd);//最大持仓有0值的时候，所有的maplevel

	bool GetFundParam(RiskKey& rKey, std::string strTime, NetFundParam& netFundParam);
	int  GetPosiDetailLevel(RiskKey& rKey, double value, const string& strInstID, SResponse& retRsp, int& LevelID);

	//最大持仓和市场冲击风险的等级判断
	int  GetMaxPositionWarningMapLevel(RiskKey& rKey, std::string strTime, std::string InstrumentID, double value, SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);

	
	int  GetIDPre();
	void GetRiskEventID( int& EventID );
	RiskEventSubIDType GetRiskEventSubID(RiskEventIDType EventID);
	void NewActiveEvent( const RiskEvent& evt );
	void ChangeActiveEvent(const RiskEvent& evt );
	void DeleteActiveEvent( const RiskEvent& evt );
	bool GetActiveEvent(int nRiskEventID, RiskEvent&	riskEvent);
	void GetAllActiveEvent(std::map<RiskEventIDType,std::vector<RiskEvent>>&  ActiveEvent);


	bool GetEventMessageTemplate(RiskIndicatorType nRiskIndicatorID,
		RiskLevelType nRiskLevelID,EventMessageTemplate& msgtemplate) const;

	void GetRiskEventHandlingID( int& HandlingID );
	
	void AddHandling(const RiskEventHandling& r);

	// 获得单笔持仓时间风控配置指标
	bool GetPosiDetailWarning(int nOrgID, std::string strTime, SResponse& retWarning, std::string& strTimeBegin, std::string& strTimeEnd);


public:
	void ClearInstrumentInfo();
	int GetInstrumentInfo_DataCenter(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
	std::map<std::string, PlatformStru_InstrumentInfo>				m_mapID2InstrumentInfo;
	CReadWriteLock													m_Mutex_ID2InstrumentInfo;


	int GetInstrumentInfos_ByProduct(const std::string& ProductID, std::set<std::string>& outData);
	std::map<std::string, std::set<std::string>>					m_mapProduct2Instruments;//品种对应的合约
	CReadWriteLock													m_Mutex_Product2Instruments;
public:
	void SubscribeRiskEvent(int AccID);
	void SubscribeTrade(int AccID);
	void SubscribeQuot(const std::string& strInstrument);
	void SubscribeFund(int AccID);
	void SubscribePosition(int AccID);
	void SubscribeOrder(int AccID);

	void UnSubscribeRiskEvent(int AccID);
	void UnSubscribeTrade(int AccID);
	void UnSubscribeQuot(const std::string& strInstrument);
	void UnSubscribeFund(int AccID);
	void UnSubscribePosition(int AccID);
	void UnSubscribeOrder(int AccID);

	void GetRiskEventList( int AccID, std::vector<RiskEvent>& vecEvent ) const;
protected:
	static CDataCenter* m_DataCenter;
	
private:
	int	 m_nRiskEventID;

	CReadWriteLock												m_Mutex_RiskEvent;
	std::map<RiskEventIDType,std::vector<RiskEvent>>			m_ActiveEvent;
	std::map<RiskEventIDType,std::vector<RiskEvent>>			m_HistoryEvent;
	std::map<RiskEventIDType,std::vector<RiskEventHandling>>    m_Handling;
	int															m_nRiskEventHandlingID;

	std::map<int,EventMessageTemplate>							m_EventMessageTemplate;
	CReadWriteLock												m_Mutex_EventMessageTemplate;


private:
	std::map<std::string,SubscribeType>			m_SubscribeInfo;
	CReadWriteLock						m_Mutex_SubscribeInfo;	

	std::set<std::string>				m_SubscribeQuot;
	CReadWriteLock						m_Mutex_SubscribeQuot;
	
	bool GetNetFundParam(int nID, NetFundParam& netFundParam);
	//基金净值参数
	std::map<string,NetFundParam>				m_mapAccount2NetFundParam;
	CReadWriteLock								m_Mutex_Account2NetFundParam;
	std::map<string, std::vector<EXCHANGERESTTIME>>		m_mapRestTime;

public:
	double CalcFundNetValueRisk( const string& InvestorID);
private:


public:
	bool IsQutoSubscribed(PlatformStru_DepthMarketData& DepthMarketData);
	void IdleBusinessDataMain();
	std::set<int>										m_DayLoadSavedSet[2];
	
	//得到风险事件ID的最大值
	int GetMaxRiskEventID();
public:
	// 处理强平；风险不改变不需要处理
	CReadWriteLock										m_Mutex_AccountRisk;
	//map<账号, map<风险唯一性标识， 某一风险动作>>
	std::map<string, std::map<SRiskKey, SResponse>>		m_mapAccountRisk;
	//参数：账号；风险指标；合约；风险动作的数组（一般这个数组空或者有一个）
	void AddResponse(std::string strInvestor, RiskIndicatorType	nRiskIndicatorID, std::string strInstrument, PositionDetailKey*  PostionDetail, SResponse& sResponse, bool bNewRiskLevel);
	void GetAccountRisk(std::map<string, std::map<SRiskKey, SResponse>>& mapAccountRisk);
	bool EraseAccountRisk(std::string  strAccount);


	//处理账户限制开仓，限制下单，CInterface_SvrTradeExcute::getObj().RiskControl();	函数需要每次都赋值，不赋值则视为账户无限制
	CReadWriteLock										m_Mutex_AccountForbid;
	//map<账号, map<风险唯一性标识， 某一风险动作>>
	std::map<string, std::map<SRiskKey, SResponse>>		m_mapAccountForbid;
	//参数：账号；风险指标；合约；风险动作的数组（一般这个数组空或者有一个）
	void AddAccountForbid(std::string strInvestor, RiskIndicatorType	nRiskIndicatorID, std::string strInstrumen, PositionDetailKey*  PostionDetailt, SResponse& sResponse, bool bNewRiskLevel);
	void GetAccountForbid(std::map<string, std::map<SRiskKey, SResponse>>& mapAccountRisk);
	bool EraseAccountForbid(std::string  strAccount);


public:
	bool GetResponseOfRiskWarning(RiskKey& rKey, std::string strTime, SResponse& warnings);

	int  GetTradeForbidMapLevel(RiskKey& rKey, std::string strTime, std::string InstrumentID, double value,  SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarginForbidMapLevel(RiskKey& rKey, std::string strTime, double value, double dbMargin, SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetLossForbidMapLevel(RiskKey& rKey, std::string strTime, double value, double dbLoss, SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);

	int  GetLossPercentMapLevel(RiskKey& rKey, std::string strTime, double dynamic, double dbInit, SResponse& sResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);

};
