#pragma once
#include "DataBase.h"
class DBAccess;
class CWriteLog;
class CDataCenter:CDataBase
{
public:
	typedef std::map<std::string,std::map<TSequenceNoType,SequencialPositionField>> POSITIONSTRUCT;
	typedef std::map<std::string,std::map<TSequenceNoType,SequencialTradeField>> TRADESTRUCT;
	typedef std::map<std::string,std::map<TSequenceNoType,SequencialOrderField>> ORDERSTRUCT; 
	typedef std::map<RiskIndModuleType,std::map<RiskLevelType,RiskResponse>> RISKWARNINGSTRUCT; 
public:
	CDataCenter(void);
	virtual ~CDataCenter(void);
	static void Init(void);
	static void Release(void);
	static CDataCenter* Get();
	void MakeRiskWarningMap(RiskIndicatorType type,RiskIndModuleType module,
		int level,const RiskResponse& warning,RISKWARNINGSTRUCT* pMapVec);
	void MakeRiskWarningMap(RiskIndicatorType type,std::string strAcc,
		std::string ProductID,int level,const RiskResponse& warning,
		std::map<std::string,std::map<RiskLevelType,RiskResponse>>& tmap);
	void ReadRiskWarning();
	void ReadConfigXml(const std::string strfilename);
	void AddProduct(const ProductField& Product );
	void AddInstrument(const InstrumentField& Instrument);
	void AddMarketData(const DepthMarketDataField& MarketData);
	void AddRiskFundAccount(const RiskSyncAccountField& RiskSyncAccount);
	void AddPreRiskFundAccount(const SeqPreRiskAccountField& PreRiskAccount);
	void AddPosition(const SequencialPositionField& Position);
	void AddTrade(const SequencialTradeField& Trade);
	void AddOrder(const SequencialOrderField& Order);
	void AddSeqDeposit(const SeqDepositField& Deposit);
	void OnTimerStatistics();

	void AddAcoount2NetValue(NetValueInfo& netValueInfo);
public:
	void NewActiveEvent( const RiskEvent& evt );
	void DeleteActiveEvent( const RiskEvent& evt );
	void AddHandling(const RiskEventHandling& r);
	void AddHandling(const RiskEventHandlingUserReq& r);
	DBAccess& GetDBAccess(){return m_dbAccess; }
	//获取用户登录请求结构体
	void GetUserLoginField(ReqRiskUserLoginField& LoginField) const;
	//获取前端服务器地址列表
	void GetrFrontAddr(std::vector<std::string>& strFrontAddr ) const;
	//获取交易服务器地址
	void GetTradeAddr(std::string& strTradeAddr) const;
	bool GetForceClosePermission(int AccID) const;
	bool GetEventMessageTemplate(RiskIndicatorType nRiskIndicatorID,
		RiskLevelType nRiskLevelID,EventMessageTemplate& msgtemplate) const;
	bool GetRiskMgmtUserInfo(AssetOrgProRelation org,std::vector<TargetAccount>& Accounts);
public:
	bool AccountListIsExist(const std::string& InvestorID);
	//获取管理的全部账户列表
	void GetAccountList( std::vector<std::string>& accounts );
	void CDataCenter::GetAccountList( std::map<std::string,TrustTradeAccount>& AccountList);
	//
	RiskEventSubIDType GetRiskEventSubID(RiskEventIDType EventID);
	//由InvestorID变成账户数据库ID
	TradeAccountIDType GetTradeAccountID( const std::string& InvestorID );
	//
	AssetMgmtOrgIDType GetAssetMgmtOrgID( const std::string& InvestorID );
	//
	FinancialProductIDType GetFinancialProductID( const std::string& InvestorID );	
	//
	std::string GetTradeAccountID( TradeAccountIDType aID ) const;
	//获取委托交易账户信息
	bool GetTradeAccount(const std::string& InvestorID, TrustTradeAccount& account);
	//由InvestorID变成账户数据库ID
	RiskIndModuleType GetRiskIndModuleID( const std::string& InvestorID ) const;
	//获取合约列表
	void GetInstrumentList(std::vector<std::string>& vec) const;
	//获取品种列表
	void GetProductIDList(std::vector<std::string>& vec);
	//从深度行情获取合约的市场总持仓
	double  GetTotalPostion(const std::string& Instrument);
	//获取一个账户特定的合约下总持仓
	int  GetInstrumentPostion(const std::string& Investor,const std::string& Instrument);
	bool GetWarningMapLevel(double value,std::map<int,RiskResponse>& warnings,int& LevelID) const;
	bool  GetMaxPositionRiskLevel(const std::string& Investor, const std::string& InstrumentID,double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;
	bool  GetFundLossRiskLevel(const std::string& Investor, double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;	
	bool  GetGappedMarketRiskLevel(const std::string& Investor,double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;	
	bool  GetMarginRatioRiskLevel(const std::string& Investor,double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;	

	bool  GetMaxProductPositionRiskLevel(const std::string& Investor, const std::string& ProductID,double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;

	bool  GetFundNetValueLevel(const std::string& Investor, double value,
		std::map<int,RiskResponse>& warnings,int& LevelID) const;	
	bool GetWarningMapLevel_FundNetValue(double value,std::map<int,RiskResponse>& warnings,int& LevelID) const;

	//获取一个账户特定的合约下总持仓
	void  GetProductPostion(const std::string& Investor, std::map<std::string, int>& mapProductCount);

	//得到当天某账户出入金
	bool GetWithDraw( const std::string& InvestorID, double& dbWithDraw);
	double GetCurrMargin( const std::string& InvestorID );
	bool GetBalance( const std::string& InvestorID, double& dbValue );
	//获得账户的初始权益
	double GetInitFund( const std::string& InvestorID );
	//获得账户的持仓盈亏
	double GetPositionProfit( const std::string& InvestorID );

	bool GetPreBalance(const std::string& InvestorID, double& dbValue);
	//获取最新行情
	bool GetDepthMarketData(const std::string& Instrument, DepthMarketDataField& MarketDataField);
	//获取合约详情
	bool GetInstrumentInfo(const std::string& strInstrument,InstrumentField& InstrumentInfo) const;
	//
	void GetInstrumentInfoList( std::vector<InstrumentField>& vec );
	double GetSimulateGappedLoss( const std::string& InvestorID );
	std::string GetSpecialIDFromProduct(const std::string& Investor, const std::string& ProductID) const;
	std::string GetSpecialIDFromInstrument(const std::string& Investor, const std::string& InstrumentID) const;

	//基金净值参数
	bool GetNetFundParam(const std::string& Investor, NetFundParam& netFundParam);
	//基金强平设置
	bool GetForceCloseParam(const std::string& Investor, ForceCloseParam& forceCloseParam);

	//基金净值强平权限
	bool HasForceClosePermission(const std::string& Investor);

	//获取未成交单
	void GetOpenOrder(std::string strInvestor, std::vector<SequencialOrderField>& vec);
	//获取持仓
	void GetPosition(std::string strInvestor, std::vector<SequencialPositionField>& vec);
	//通过合约获取交易所ID
	bool GetExchangeID(std::string strInstrumentID, std::string& strExchangeID);
	//取某个合约的涨跌停价
	bool GetLimitPrice(std::string strInstrumentID, double& dUpperLimit, double& dLowerLimit);
	//获取某个合约的挂单量
	void GetOpenOrderVolumn(std::string strInvestor, std::string strInstrumentID, char Direction,
		int& nTodayVolumn, int& nYestodayVolumn );

	//得到绩效汇总历史数据
	bool GetGatherData(int nIDType, int nID, SGatherData& sGathData);

public:
	//获取出入金
	void GetDepositList( int AccID, std::vector<SeqDepositField>& vec ) const;
	//获取成交记录
	void GetTradeList( int AccID, std::vector<SequencialTradeField>& vec) const;
	//获取报单
	void GetOrderList( int AccID, std::vector<SequencialOrderField>& vec ) const;
	//获取持仓
	void GetPositionList( int AccID, std::vector<SequencialPositionField>& vec) const;
	//获取账户昨日今日资金信息
	void GetAllAccountFundList( int AccID, std::vector<RiskAllAccountField>& vec ) const;
	//获取行情
	void GetQuotList( std::string InstrumentID, std::vector<DepthMarketDataField>& vec ) const;
	//风险事件
	void GetRiskEventList( int AccID, std::vector<RiskEvent>& vecEvent ) const;
	//风险事件处理
	void GetRiskEvtHandlingList( int AccID, std::vector<RiskEventHandling>& vec ) const;
	//统计
	void GetStatisticsEntryList(const AssetOrgProRelation& org, std::vector<StatisticsEntry>& vec,bool bProtected=true) const;
	
	//得到基金净值数据
	void GetAllNetValueInfo( int AccID, std::vector<NetValueInfo>& vec ) const;
public://各种订阅
	void SubscribeFund(int AccID);
	void SubscribePosition(int AccID);
	void SubscribeTrade(int AccID);
	void SubscribeOrder(int AccID);
	void SubscribeDeposit(int AccID);
	void SubscribeRiskEvent(int AccID);
	void SubscribeRiskEvtHandling(int AccID);
	void SubscribeQuot(const std::string& strInstrument);
	void SubscribeStatistics(const AssetOrgProRelation& org);
	void SubscribeNetValue(int AccID);

	void UnSubscribeFund(int AccID);
	void UnSubscribePosition(int AccID);
	void UnSubscribeTrade(int AccID);
	void UnSubscribeOrder(int AccID);
	void UnSubscribeDeposit(int AccID);
	void UnSubscribeRiskEvent(int AccID);
	void UnSubscribeRiskEvtHandling(int AccID);
	void UnSubscribeQuot(const std::string& strInstrument);
	void UnSubscribeStatistics(const AssetOrgProRelation& org);
	void UnSubscribeNetValue(int AccID);
public:
	void ReadAssetOrgProRelation();
	void ReadTrustTradeAccount();
	void ReadForceClosePermission();
	void ReadEventMessageTemplate();
	void ReadRiskMgmtUserInfo();
	void ReadNetFundParam();
	void ReadForceCloseParam();//基金强平设置
	void ReadGatherData();//得到绩效统计数据
	void GetUpperLevelOrgID(AssetMgmtOrgIDType id,std::set<AssetMgmtOrgIDType>& ups);
	void InsertStatisticsItem( const TrustTradeAccount&  acc );
	void SaveAllData();
	void SaveGatherData();
	bool GetLowerAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vOut );
	void CalcLowerData( std::map<int, std::map<int, AssetMgmtOrganization>>& mapAssetOrgTree, 
		int nOwnerID, std::vector<AssetMgmtOrganization>& vOutData );
	void FilterAssetOrg( int nCurOrgID, std::vector<AssetMgmtOrganization>& vAssetOrg, 
		std::vector<AssetMgmtOrganization>& vOut );
	bool ProcessGatherAccountVec(int nTypeID, int nID, std::vector<TrustTradeAccount>& vec, std::vector<SGatherData>& vGatherData);
	
	void FrontDisconnected( int nReason );
	void SuccessLogin();
	bool IsOK();
	std::map<std::string,double>& GetComplexMapIterator(
		std::map<AssetOrgProRelation,std::map<std::string,double>>& itemmap,AssetOrgProRelation org);
	void Reset();
	void IdleBusinessDataMain();
	void UpdateBaseData();
	void GetRiskEventID( int& EventID );
	void GetRiskEventHandlingID( int& HandlingID );
	template<typename K,typename V>
	bool IsSameMap(const std::map<K,V>& map1,const std::map<K,V>& map2);
	template<typename K>
	bool IsSameRiskWarning(const std::map<K,std::map<RiskLevelType,RiskResponse>>& map1,
		const std::map<K,std::map<RiskLevelType,RiskResponse>>& map2);
	bool IsRiskWarningChanged();
	void FirstPrintf(const char* pMsg,bool bClear=false);
	void WriteLogInfo(const char* pMsg, const char* pContent, int nID=-1);
	CWriteLog* m_pWriteLogTest;
public:
		//DIRTYITEM(RiskWarning)
		//DIRTYITEM(AssetOrgProRelation)
		//DIRTYITEM(TrustTradeAccount)
		//DIRTYITEM(ForceClosePermission)
protected:
	static CDataCenter* m_DataCenter;
	CWriteLog* m_pWriteLog;
private://重启可以重交易所获取全部新数据
	//风控资金账户昨日信息
	std::map<std::string,SeqPreRiskAccountField>	m_PreRiskFundAccount;
	CReadWriteLock									m_Mutex_PreRiskFundAccount;
	//风控同步投资者资金账户
	std::map<std::string,RiskSyncAccountField>		m_RiskFundAccount;
	CReadWriteLock									m_Mutex_RiskFundAccount;

	//基金净值实时数据
	std::map<std::string, NetValueInfo>					m_Accout2NetValue;
	CReadWriteLock									m_Mutex_Accout2NetValue;

	//持仓
	POSITIONSTRUCT									m_AllPosition;
	CReadWriteLock									m_Mutex_AllPosition;
	//交易
	TRADESTRUCT										m_AllTrade;
	CReadWriteLock									m_Mutex_AllTrade;
	//报单
	ORDERSTRUCT										m_AllOrder;
	CReadWriteLock									m_Mutex_AllOrder;
	//合约
	std::map<std::string,ProductField>				m_AllProduct;
	CReadWriteLock									m_Mutex_AllProduct;
	//合约
	std::map<std::string,InstrumentField>			m_AllInstrument;
	CReadWriteLock									m_Mutex_AllInstrument;
	//出入金
	std::map<std::string,std::map<int,SeqDepositField>> m_AllDeposit;
	CReadWriteLock									m_Mutex_AllDeposit;
	//行情
	std::map<std::string,DepthMarketDataField>		m_DepthMarketData;
	CReadWriteLock									m_Mutex_DepthMarketData;

	

private://重启后全部数据应该重置为空
	std::map<std::string,SubscribeType> m_SubscribeInfo;
	CReadWriteLock						m_Mutex_SubscribeInfo;
	std::set<std::string>				m_SubscribeQuot;
	CReadWriteLock						m_Mutex_SubscribeQuot;
	std::set<AssetOrgProRelation>		m_SubscribeStatistics;
	CReadWriteLock						m_Mutex_SubscribeStatistics;
private://重启后全部数据应该重读数据库
	std::map<std::string,TrustTradeAccount> m_AccountList;//账户列表<账户-初始权益>
	std::map<int,std::string>				m_AccountID2Str;//交易账户和账户ID的对应关系	
	CReadWriteLock							m_Mutex_AccountList;
	//强平权限<风控账户ID>
	std::set<int>							m_ForceClosePermission;
	CReadWriteLock							m_Mutex_ForceClosePermission;
	//
	std::map<int,EventMessageTemplate>		m_EventMessageTemplate;
	CReadWriteLock							m_Mutex_EventMessageTemplate;

	std::map<AssetOrgProRelation,std::vector<TargetAccount>>	m_RiskMgmtUserInfo;
	CReadWriteLock							m_Mutex_RiskMgmtUserInfo;

	//基金净值参数
	std::map<std::string,NetFundParam>				m_mapAccount2NetFundParam;
	CReadWriteLock									m_Mutex_Account2NetFundParam;

	//基金强平设置
	std::map<std::string,ForceCloseParam>			m_mapAccount2ForceCloseParam;
	CReadWriteLock									m_Mutex_Account2ForceCloseParam;

	//根据组织机构ID得到上一个交易日历史绩效汇总数据
	std::map<int,  SGatherData>						m_mapOrgID2GatherData;
	CReadWriteLock									m_Mutex_OrgID2GatherData;

	//根据理财产品得到上一个交易日历史绩效汇总数据
	std::map<int,  SGatherData>						m_mapFinancialID2GatherData;
	CReadWriteLock									m_Mutex_FinancialID2GatherData;

	//根据交易账户得到上一个交易日历史绩效汇总数据
	std::map<int,  SGatherData>						m_mapAccountID2GatherData;
	CReadWriteLock									m_Mutex_AccountID2GatherData;

private://重启后全部数据应该重读数据库
	//风险预警
	std::map<std::string,std::map<RiskLevelType,RiskResponse>>			m_SpecialMaxPositionRiskWarning;//<AccID+ProductID>
	RISKWARNINGSTRUCT	m_MaxPositionRiskWarning;	
	RISKWARNINGSTRUCT	m_FundLossRiskWarning;
	RISKWARNINGSTRUCT	m_GappedMarketRiskWarning;
	RISKWARNINGSTRUCT	m_MarginRatoRiskWarning;
	
	RISKWARNINGSTRUCT	m_ProductMaxPositionRiskWarning;	
	std::map<std::string,std::map<RiskLevelType,RiskResponse>>			m_SpecialProductMaxPositionRiskWarning;//<AccID+ProductID>

	RISKWARNINGSTRUCT	m_FundNetRiskWarning;
	bool				m_bRiskWarningChanged;
	CReadWriteLock														m_Mutex_RiskWarning;
	DISALLOW_COPY_AND_ASSIGN(CDataCenter);
private://重启后全部数据应该保存当天数据
	//风险事件记录列表
	std::map<RiskEventIDType,std::vector<RiskEvent>>			m_ActiveEvent;
	std::map<RiskEventIDType,std::vector<RiskEvent>>			m_HistoryEvent;
	std::map<RiskEventIDType,std::vector<RiskEventHandling>>    m_Handling;
	int															m_nRiskEventHandlingID;
	int															m_nRiskEventID;
	CReadWriteLock												m_Mutex_RiskEvent;
private:
	std::vector<std::string>							m_strFrontAddr;
	std::string											m_strTradeAddr;
	ReqRiskUserLoginField								m_LoginField;
	time_t												m_LoginTime;
	bool												m_lastStatus;
	std::set<int>										m_DayLoadSavedSet[2];
	DBAccess											m_dbAccess;
	//统计
	volatile long m_StatisticsUpdate;
	std::map<AssetMgmtOrgIDType,AssetMgmtOrgIDType>				m_OrgIDMap;	
	std::map<AssetOrgProRelation,std::set<std::string>>			m_StatisticsItem;
	std::map<AssetOrgProRelation,double>					   m_StatisticsTotalValue[3];
	std::map<AssetOrgProRelation,std::map<std::string,double>> m_StatisticsInstrValue[2];
	CReadWriteLock											   m_Mutex_Statistics;

public:
	//计算基金净值
	double CalcFundNetValueRisk( const std::string& InvestorID);


	int  getExpoBase2(const double& d);
	bool equals(const double& d1, const double& d2);
};
