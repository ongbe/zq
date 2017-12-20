#pragma once
class CRiskProcess;
class CThreadSharedData;
class CRiskMsgCalc
{
public:
	CRiskMsgCalc(CThreadSharedData* pThreadSharedData);
	~CRiskMsgCalc(void);
	
	//行情到来
	//void NewDepthMarketData(const  std::string& InstrumentID );
	//资金到来
	void NewFundAccount(const string& InvestorID );
	//持仓到来
	void NewPosition(const string& InvestorID, bool bDoResponse = false);//bDoResponse 对于重复出现的事件也要做动作
	//成交到来
	void NewTrade(const string& InvestorID, const string&  InstrumentID );
	//报单到来
	void NewOrder(const string& InvestorID, const string&  InstrumentID );
	//一个新的Timer到达时，用来判断持仓明细的持仓时间
	void NewTimer_PosiDetailTime();
	//交易数据管理数据准备好，持仓判断
	void TradeDataInit();

	//风控方案改变
	void ChangeRiskPlan();

public:
	//激活计算单合约最大持仓
	void ActiveCalcMaxPositionRisk( string InvestorID, std::string strTime, const std::string& InstrumentID);
	
	//市场冲击风险
	void ActiveCalcMarketShockRisk(const string& InvestorID, std::string strTime, const std::string& InstrumentID);
	
	//账户限亏
	void ActiveCalcAccountLossRisk(const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse 对于重复出现的事件也要做动作

	//今日限亏
	void ActiveCalcTodayLossRisk(const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse 对于重复出现的事件也要做动作

	//隔夜跳空
	void ActiveCalcGappedMarketRisk(const string& InvestorID, std::string strTime);

	//保证金比例风险
	void ActiveCalcMarginRatioRisk(const string& InvestorID, std::string strTime);

	//基金净值风险 
	void ActiveCalcFundNetValueRisk(const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse 对于重复出现的事件也要做动作

	//持仓明细的持仓时间风险
	void ActiveCalcPosiDetalMaxTimeRisk(const string& InvestorID, 
				std::vector<PlatformStru_PositionDetail>& vecPosiDetails, 
				vector<SResponse>& vecResponse);
	//单独处理持仓明细的持仓时间风险消失情况
	void ActivePosiDetalMaxTimeRiskLose(const string& InvestorID, std::string strTime, 
				std::map<PositionDetailKey, RiskEventLevelID>& mapDetailKey, std::string& strTimeBegin, std::string& strTimeEnd);

	//最大持仓风险对于0值需要额外处理
	void ActiveCalcMaxPosZero( string InvestorID, std::string strTime);
	//判断是不是设置了0风险的风控方案
	bool HasZeroRiskPlan(string InvestorID, std::string strTime, SResponse& vecResponse, std::string& strTimeBegin, std::string& strTimeEnd);
	
	void ProcessRisk(const string& InvestorID, bool bSetTradeStaus = true);// 处理强平；风险不改变不需要处理
	void ProcessForbid(const string& InvestorID);//处理账户限制开仓，限制下单，CInterface_SvrTradeExcute::getObj().RiskControl();	函数需要每次都赋值，不赋值则视为账户无限制

    
	

public://判断有没有风险，以及风险的详细信息
	int  GetMaxPositionRisk_Level( string InvestorID, std::string strTime, const std::string& InstrumentID, double&	dblRisk, int& nVolume, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarketShockRisk_Level(const string& InvestorID, std::string strTime, const std::string& InstrumentID, double& dblRisk, int& nVolume,  double& total, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetAccountLossRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetTodayLossRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetGappedMarketRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarginRatioRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetFundNetValueRisk_Level(const string& InvestorID, std::string strTime, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);


private:
	int GetRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, double value, SResponse& vecResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);
	int GetMaxPositionRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, std::string InstrumentID, double dblRisk, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	bool CalcGetFundParam( const string& InvestorID, std::string strTime, NetFundParam& netFundParam);
	int CalcOverTime_DealerPosiDetail(const string& InvestorID, const string& strProdID, 
				SResponse& retRsp, int nOpenTime, int& nLevel);
	int CalcRemainTime(std::string& strInstID, std::string& strOpenTime);
	int CalcRealHoldingTime(std::string& strExchangeID, int nTime);
	int GetRemainTime(std::string& strOpenTime);
	//bool GetPosiDetailWarning(int nOrgID, SResponse& retWarning, std::vector<std::string>& vecAccount);
	void DoPosiDetailRisk(std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail, 
					SResponse& warnings, int nLevel, time_t currTime, std::string& strTimeBegin, std::string& strTimeEnd);



private:
	CRiskProcess* m_RiskProcess;
	CThreadSharedData* m_pThreadShareData;

public:
	void SendQuto(PlatformStru_DepthMarketData& DepthMarketData);
	void SendFund(std::string strInvestor);
	void SendOrder(PlatformStru_OrderInfo* lpBuf);
	void SendTrader(PlatformStru_TradeInfo* lpBuf);
	void SendPosition(std::string strInvestor);


public:
	//风险事件失效，时间过期
	void  ActiveEventPassTime();
	//交易限制风险
	void ActiveCalcTradeForbid(const string& InvestorID, std::string strTime, const std::string& InstrumentID);
	int  GetTradeForbid_Level(const string& InvestorID, std::string strTime, const std::string& InstrumentID, double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetTradeForbidRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, std::string InstrumentID, double dblRisk, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//保证金限制
	void ActiveCalcMarginForbid(const string& InvestorID, std::string strTime);
	//dblRisk:保证金/动态权益比例不超过这个数 ; dbRiskMargin:保证金不超过这个数
	int  GetMarginForbid_Level(const string& InvestorID, std::string strTime,  double& dblRisk, double& dbRiskMargin, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarginForbidRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, double dblRisk, double& dbRiskMargin, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//限制亏损
	void ActiveCalcLossForbid(const string& InvestorID, std::string strTime);
	////dblRisk:亏损比例不超过这个数 ; dbRiskLoss:亏损总额不超过这个数
	int  GetLossForbid_Level(const string& InvestorID, std::string strTime,  double& dblRisk, double& dbRiskLoss, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetLossForbidRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, double dblRisk, double& dbRiskLoss, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//单日最大亏损
	void ActiveCalcOnedayLargetsLossForbid(const string& InvestorID, std::string strTime);
	int  GetOnedayLargetsLoss_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//亏损最大值
	void ActiveCalcLossMax(const string& InvestorID, std::string strTime);
	int  GetLossMax_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);


	//最大回撤值
	void ActiveCalcMaxRetrace(const string& InvestorID, std::string strTime);
	int  GetMaxRetrace_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//账户持仓合约价值
	void ActiveCalcContractsValuesForbid(const string& InvestorID, std::string strTime);
	int  GetContractsValues_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//多空轧差合约价值
	void ActiveBullBearValuesForbid(const string& InvestorID, std::string strTime);
	int  GetBullBearValues_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//连续亏损天数
	void ActiveLossContinueDays(const string& InvestorID, std::string strTime);
	int  GetLossContinueDays_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

	//限制亏损总量
	void ActiveLossPercent(const string& InvestorID, std::string strTime);
	int  GetLossPercent_Level(const string& InvestorID, std::string strTime,  double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetLossPercentRiskLevel(string InvestorID, std::string strTime, RiskIndicatorType RiskType, double dbdynamic, double& dbInit, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

};
