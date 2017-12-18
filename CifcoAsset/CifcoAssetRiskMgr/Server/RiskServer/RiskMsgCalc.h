#pragma once
class CRiskProcess;
class UserApi;
class CRiskMsgCalc
{
public:
	CRiskMsgCalc(UserApi* pApi);
	virtual ~CRiskMsgCalc();
	//行情到来
	void NewDepthMarketData(const  std::string& InstrumentID );
	//资金到来
	void NewFundAccount(const  std::string& InvestorID );
	//持仓到来
	void NewPosition(const std::string& InvestorID,const std::string& InstrumentID);
	//成交到来
	void NewTrade(const std::string& InvestorID );
	//报单到来
	void NewOrder(const std::string& InvestorID );
public:
	//激活计算单合约最大持仓
	void ActiveCalcMaxPositionRisk( const std::string& InvestorID, const std::string& InstrumentID );
	//激活计算限亏预警
	void ActiveCalcFundLossRisk( const std::string& InvestorID );
	//激活计算持仓风险敞口预警
	void ActiveCalcGappedMarketRisk(const std::string& InvestorID);

	//激活保证金比例预警
	void ActiveCalcMarginRatoRisk(const std::string& InvestorID);

	//激活产品最大持仓风险
	void ActiveCalcProductMaxPositionRisk( const std::string& InvestorID);

	//激活基金净值风险
	void ActiveCalcFundNetValueRisk( const std::string& InvestorID);
private:
	//计算单合约最大持仓
	double CalcMaxPositionRisk(const std::string& InvestorID,const std::string& InstrumentID, int& nVolume);
	//计算限亏预警
	double CalcFundLossRisk(const std::string& InvestorID);
	//市场冲击风险
	double CalcGappedMarketRisk(const std::string& InvestorID);
	//保证金百分比预警
	double CalcMarginRatoRisk(const std::string& InvestorID);
	//品种最大持仓预警
	void CalcProductMaxPositionRisk(const std::string& InvestorID, std::map<std::string, int>& mapProductCount);
public://涉及基金净值实时推送，所以外部需要调用
	//基金净值预警
	double CalcFundNetValueRisk( const std::string& InvestorID);
private:
	UserApi*			 m_pApi;
	CRiskProcess*		  m_RiskProcess;
};
