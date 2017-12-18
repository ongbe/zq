#include "StdAfx.h"
#include "RiskMsgCalc.h"
#include "DBAccess.h"
#include "DataCenter.h"
#include "RiskProcess.h"
#include "UserApi.h"
CRiskMsgCalc::CRiskMsgCalc(UserApi* pApi)
:m_pApi(pApi)
{
	m_RiskProcess=new CRiskProcess();
}
CRiskMsgCalc::~CRiskMsgCalc()
{
	SAFE_DELETE(m_RiskProcess);
}
//行情到来
void CRiskMsgCalc::NewDepthMarketData(const  std::string& InstrumentID )
{
	std::vector<std::string> accounts;
	SAFE_GET_DATACENTER()->GetAccountList(accounts);
	std::vector<std::string>::const_iterator it;
	for(it=accounts.begin();it!=accounts.end();++it)
	{
		const std::string InvestorID=*it;
		ActiveCalcMaxPositionRisk(InvestorID,InstrumentID);
		ActiveCalcProductMaxPositionRisk(InvestorID);
		ActiveCalcFundNetValueRisk(InvestorID);	
	}
}
//资金到来
void CRiskMsgCalc::NewFundAccount(const  std::string& InvestorID )
{
	ActiveCalcFundLossRisk(InvestorID);
	ActiveCalcGappedMarketRisk(InvestorID);	
	ActiveCalcMarginRatoRisk(InvestorID);	
	ActiveCalcFundNetValueRisk(InvestorID);	
}
//持仓到来
void CRiskMsgCalc::NewPosition(const  std::string& InvestorID ,const std::string& InstrumentID)
{
	ActiveCalcMaxPositionRisk(InvestorID,InstrumentID);
	ActiveCalcGappedMarketRisk(InvestorID);
	ActiveCalcProductMaxPositionRisk(InvestorID);
	ActiveCalcFundNetValueRisk(InvestorID);	
}
//成交到来
void CRiskMsgCalc::NewTrade(const  std::string& InvestorID )
{
	ActiveCalcProductMaxPositionRisk(InvestorID);
	ActiveCalcFundNetValueRisk(InvestorID);	
}
//报单到来
void CRiskMsgCalc::NewOrder(const  std::string& InvestorID )
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//激活计算单合约最大持仓
void CRiskMsgCalc::ActiveCalcMaxPositionRisk( const std::string& InvestorID, const std::string& InstrumentID )
{
	double dblValue=0.0;
	int nVolume = 0;
	std::map<int,RiskResponse>  warnings;
	int levelID=0;
	if(CDataCenter::Get())
	{
		dblValue=CalcMaxPositionRisk(InvestorID,InstrumentID, nVolume);
		if(CDataCenter::Get()->GetMaxPositionRiskLevel(InvestorID,InstrumentID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddMaxPositionEvent(InvestorID,InstrumentID,dblValue,nVolume,levelID);
		}
		else if(m_RiskProcess->HasMaxPositionRisk(InvestorID,InstrumentID))
		{//levelID =0;风险事件消失
			m_RiskProcess->AddMaxPositionEvent(InvestorID,InstrumentID,dblValue,nVolume,0);
		}
	}	
}
//激活计算限亏预警
void CRiskMsgCalc::ActiveCalcFundLossRisk( const std::string& InvestorID )
{
	int levelID=0;
	double dblValue=0.0;
	std::map<int,RiskResponse>  warnings;
	if(CDataCenter::Get())
	{
		dblValue=CalcFundLossRisk(InvestorID);
		if(CDataCenter::Get()->GetFundLossRiskLevel(InvestorID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddFundLossEvent(InvestorID,dblValue,levelID);
		}
		else if(m_RiskProcess->HasFundLossRisk(InvestorID))
		{//levelID =0;风险事件消失
			m_RiskProcess->AddFundLossEvent(InvestorID,dblValue,0);
		}
	}	
}
//激活计算持仓风险敞口预警
void CRiskMsgCalc::ActiveCalcGappedMarketRisk(const std::string& InvestorID)
{
	int levelID=0;
	double dblValue=0.0;
	std::map<int,RiskResponse>  warnings;
	if(CDataCenter::Get())
	{
		dblValue=CalcGappedMarketRisk(InvestorID);
		if(CDataCenter::Get()->GetGappedMarketRiskLevel(InvestorID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddGappedMarketEvent(InvestorID,dblValue,levelID);
		}
		else if(m_RiskProcess->HasGappedMarketRisk(InvestorID))
		{//levelID =0;风险事件消失
			m_RiskProcess->AddGappedMarketEvent(InvestorID,dblValue,0);
		}


	}	
}

//激活保证金比例预警
void CRiskMsgCalc::ActiveCalcMarginRatoRisk(const std::string& InvestorID)
{
	int levelID=0;
	double dblValue=0.0;
	std::map<int,RiskResponse>  warnings;
	if(CDataCenter::Get())
	{
		dblValue=CalcMarginRatoRisk(InvestorID);
		if(CDataCenter::Get()->GetMarginRatioRiskLevel(InvestorID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddMarginRatioEvent(InvestorID,dblValue,levelID);
		}
		else if(m_RiskProcess->HasMarginRatioRisk(InvestorID))
		{//levelID =0;风险事件消失
			m_RiskProcess->AddMarginRatioEvent(InvestorID,dblValue,0);
		}

	}	
}
//激活计算产品最大持仓风险
void CRiskMsgCalc::ActiveCalcProductMaxPositionRisk( const std::string& InvestorID)
{
	std::map<int,RiskResponse>  warnings;
	int levelID=0;
	if(CDataCenter::Get())
	{
		std::map<std::string, int> mapProductCount;
		CalcProductMaxPositionRisk(InvestorID,mapProductCount);

		for(std::map<std::string, int>::iterator it= mapProductCount.begin(); it!= mapProductCount.end(); it++)
		{
			std::string strProduct = it->first;
			double dblValue		   = it->second;
			if(CDataCenter::Get()->GetMaxProductPositionRiskLevel(InvestorID,strProduct,dblValue,warnings,levelID))
			{
				m_RiskProcess->AddMaxProductPositionEvent(InvestorID,strProduct,dblValue,levelID);
			}
			else if(m_RiskProcess->HasMaxProductPositionRisk(InvestorID,strProduct))
			{//levelID =0;风险事件消失
				m_RiskProcess->AddMaxProductPositionEvent(InvestorID,strProduct,dblValue,0);
			}
		}
	}	
}
//激活基金净值风险
void CRiskMsgCalc::ActiveCalcFundNetValueRisk( const std::string& InvestorID )
{
	int levelID=0;
	double dblValue=0.0;
	std::map<int,RiskResponse>  warnings;
	if(CDataCenter::Get())
	{
		dblValue = CalcFundNetValueRisk(InvestorID);
		if(dblValue <0)
			return;//没有配置该账户的详细资金信息，无法进行风险判断

		if(CDataCenter::Get()->GetFundNetValueLevel(InvestorID,dblValue,warnings,levelID))
		{
			m_RiskProcess->AddFundNetValueEvent(InvestorID,dblValue,levelID);
		}
		else if(m_RiskProcess->HasFundNetValueRisk(InvestorID))
		{//levelID =0;风险事件消失
			m_RiskProcess->AddFundNetValueEvent(InvestorID,dblValue,0);
		}
	}	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//单合约最大持仓,响应行情改变和持仓改变
double CRiskMsgCalc::CalcMaxPositionRisk(const std::string& InvestorID,const std::string& InstrumentID, int& nVolume)
{
	double dblRisk=0.0;
	nVolume = 0;
	if(CDataCenter::Get())
	{
		double total=CDataCenter::Get()->GetTotalPostion(InstrumentID);
		if(total>0)
		{
			int pos=CDataCenter::Get()->GetInstrumentPostion(InvestorID,InstrumentID);
			nVolume = pos;
			dblRisk=pos/total;
		}
	}
	return dblRisk;
}
//限亏预警，响应资金改变
double CRiskMsgCalc::CalcFundLossRisk(const std::string& InvestorID)
{
	double dblRisk=0.0;
	if(CDataCenter::Get())
	{
		double dblMaxFundLoss=CDataCenter::Get()->GetInitFund(InvestorID);
		if(dblMaxFundLoss>0)
		{
			double dblBalance= 0.0;
			CDataCenter::Get()->GetBalance(InvestorID, dblBalance);//账户总权益
			dblRisk=1.00-dblBalance/dblMaxFundLoss;
		}
	}
	return dblRisk;
}
//持仓风险敞口预警
double CRiskMsgCalc::CalcGappedMarketRisk(const std::string& InvestorID)
{
	double dblRisk=0.0;
	if(CDataCenter::Get())
	{
		double dblMaxGappedLoss=CDataCenter::Get()->GetInitFund(InvestorID);
		if(dblMaxGappedLoss>0)
		{
			double dblBalance= 0.0;
			CDataCenter::Get()->GetBalance(InvestorID, dblBalance);//账户总权益
			double dblSimulateGappedLoss=CDataCenter::Get()->GetSimulateGappedLoss(InvestorID);
			dblRisk=(dblMaxGappedLoss-dblBalance+dblSimulateGappedLoss)/dblMaxGappedLoss;
		}
	}
	return dblRisk;
}
//保证金比例预警
// 风控指标 增加一个  保证金百分比   公式就是占用保证金/动态权益  大于等于 设置的阀值  就代表触发风险了
double CRiskMsgCalc::CalcMarginRatoRisk(const std::string& InvestorID)
{
	double dblRisk=0.0;
	if(CDataCenter::Get())
	{
		double dblMaxGappedLoss=CDataCenter::Get()->GetInitFund(InvestorID);
		if(dblMaxGappedLoss>0)
		{
			double dblCurrMargin=CDataCenter::Get()->GetCurrMargin(InvestorID);//占用保证金
			double dblBalance= 0.0;
			CDataCenter::Get()->GetBalance(InvestorID, dblBalance);//账户总权益		
			
			dblRisk=dblCurrMargin/dblBalance;
		}
	}
	return dblRisk;
}
//品种最大持仓：设置某个品种的汇总持仓手数预警。如果现有持仓数大于预警值，则触发相应风险。
void CRiskMsgCalc::CalcProductMaxPositionRisk(const std::string& InvestorID, std::map<std::string, int>& mapProductCount)
{	
	if(CDataCenter::Get())
	{
		CDataCenter::Get()->GetProductPostion(InvestorID, mapProductCount);		
	}
}
//基金净值预警
/*
基础值设置。  
					场外			场内（期货）
份额				Ao				Ai
资产净值			Qo				Qi
单位净值			Po = Qo / Ao	Pi = Qi / Ai
总资产净值			P = （Qo + Qi）/（Ao + Ai）	
其中。份额Ao，Ai 是手动设定的。  资产净值Qo是每日手动设置了。 Qi是每日盘中的动态权益。   场内单位净值和总资产净值是实时计算的。
P<设置的值。

*/
double CRiskMsgCalc::CalcFundNetValueRisk( const std::string& InvestorID)
{
/*	double dblBalance=CDataCenter::Get()->GetBalance(InvestorID);//账户总权益		
	NetFundParam netFundParam;
	if(!CDataCenter::Get()->GetNetFundParam(InvestorID, netFundParam))
		return -1;
	
	double dbP = 0.0;
	if(netFundParam.dOuterVolumn + netFundParam.dInnerVolumn > 0)
		dbP = (netFundParam.dOuterNetAsset + dblBalance)/(netFundParam.dOuterVolumn + netFundParam.dInnerVolumn);
*/
	double dbP = CDataCenter::Get()->CalcFundNetValueRisk(InvestorID);	
	return dbP;

	
}