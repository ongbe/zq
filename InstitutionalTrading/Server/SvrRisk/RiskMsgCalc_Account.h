#pragma once
class CRiskProcess;
class CThreadSharedData;
class CRiskMsgCalc_Account
{
public:
	CRiskMsgCalc_Account(CThreadSharedData* pThreadSharedData);
	~CRiskMsgCalc_Account(void);
	
	//资金到来
	void NewFundAccount(const std::string & BrokerID, const string& InvestorID );
	//持仓到来
	void NewPosition(const std::string & BrokerID, const string& InvestorID, bool bDoResponse = false);//bDoResponse 对于重复出现的事件也要做动作
	//成交到来
	void NewTrade(const std::string & BrokerID, const string& InvestorID, const string&  InstrumentID );
	//报单到来
	void NewOrder(const std::string & BrokerID, const string& InvestorID, const string&  InstrumentID );
	//一个新的Timer到达时，用来判断持仓明细的持仓时间
	void NewTimer_PosiDetailTime();
	//交易数据管理数据准备好，持仓判断
	void TradeDataInit();

	//风控方案改变
	void ChangeRiskPlan();

public:
	//激活计算单合约最大持仓
	void ActiveCalcMaxPositionRisk(const std::string & BrokerID, string InvestorID, std::string strTime, const std::string& InstrumentID);
	
	//市场冲击风险
	void ActiveCalcMarketShockRisk(const std::string & BrokerID, const string& InvestorID, std::string strTime, const std::string& InstrumentID);
	
	//今日限亏
	void ActiveCalcTodayLossRisk(const std::string & BrokerID, const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse 对于重复出现的事件也要做动作

	//保证金比例风险
	void ActiveCalcMarginRatioRisk(const std::string & BrokerID, const string& InvestorID, std::string strTime);

	//基金净值风险 
	void ActiveCalcFundNetValueRisk(const std::string & BrokerID, const string& InvestorID, std::string strTime, bool bDoResponse = false);//bDoResponse 对于重复出现的事件也要做动作

	//持仓明细的持仓时间风险
//	void ActiveCalcPosiDetalMaxTimeRisk(const string& InvestorID, 
//				std::vector<PlatformStru_PositionDetail>& vecPosiDetails, 
//				vector<SResponse>& vecResponse);
	//单独处理持仓明细的持仓时间风险消失情况
	void ActivePosiDetalMaxTimeRiskLose(const string& InvestorID, std::string strTime, 
				std::map<PositionDetailKey, RiskEventLevelID>& mapDetailKey, std::string& strTimeBegin, std::string& strTimeEnd);

	//处理风险


public://判断有没有风险，以及风险的详细信息
	int  GetMaxPositionRisk_Level(const std::string & BrokerID, string InvestorID, std::string strTime,const std::string& InstrumentID, double&	dblRisk, int& nVolume, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarketShockRisk_Level(const std::string & BrokerID,const string& InvestorID, std::string strTime,const std::string& InstrumentID, double& dblRisk, int& nVolume,  double& total, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetTodayLossRisk_Level(const std::string & BrokerID,const string& InvestorID, std::string strTime,double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetMarginRatioRisk_Level(const std::string & BrokerID,const string& InvestorID, std::string strTime,double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	int  GetFundNetValueRisk_Level(const std::string & BrokerID,const string& InvestorID, std::string strTime,double& dblRisk, SResponse& vecThisRisk, int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);

private:
	int GetRiskLevel(const std::string & BrokerID, string InvestorID,std::string strTime, RiskIndicatorType RiskType, double value, SResponse& vecResponse,int& LevelID, std::string& strTimeBegin, std::string& strTimeEnd);
	int GetMaxPositionRiskLevel(const std::string & BrokerID, string InvestorID,std::string strTime, RiskIndicatorType RiskType, std::string InstrumentID, double dblRisk, SResponse& vecResponse,int& nLevel, std::string& strTimeBegin, std::string& strTimeEnd);
	bool CalcGetFundParam(const std::string & BrokerID, const string& InvestorID, NetFundParam& netFundParam);
	int CalcOverTime_DealerPosiDetail(const string& InvestorID, const string& strProdID, 
				SResponse& retRsp, int nOpenTime, int& nLevel);
	int CalcRemainTime(std::string& strInstID, std::string& strOpenTime);
	int CalcRealHoldingTime(std::string& strExchangeID, int nTime);
	int GetRemainTime(std::string& strOpenTime);
	void DoPosiDetailRisk(std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail, 
					SResponse& warnings, int nLevel, time_t currTime, std::string& strTimeBegin, std::string& strTimeEnd);

	

private:
	CRiskProcess* m_RiskProcess;
	CThreadSharedData* m_pThreadShareData;


};
