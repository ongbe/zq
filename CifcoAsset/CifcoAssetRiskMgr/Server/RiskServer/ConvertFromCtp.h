#pragma once
#include "FtdcRiskUserApiStruct.h"
class ConvertFromCtp
{
public:
	static void GetBrokerDepositField(const CShfeFtdcBrokerDepositField& ctp,BrokerDepositField& risk);
	static void GetBrokerInvestorField(const CShfeFtdcBrokerInvestorField& ctp,BrokerInvestorField& risk);
	static void GetCurrentTimeField(const CShfeFtdcCurrentTimeField& ctp,CurrentTimeField& risk);
	static void GetDepartmentField(const CShfeFtdcDepartmentField& ctp,DepartmentField& risk);
	static void GetDepthMarketDataField(const CShfeFtdcDepthMarketDataField& ctp,DepthMarketDataField& risk);
	static void GetExchangeField(const CShfeFtdcExchangeField& ctp,ExchangeField& risk);
	static void GetExchangeMarginRateField(const CShfeFtdcExchangeMarginRateField& ctp,ExchangeMarginRateField& risk);
	static void GetExchangeMarginRateAdjustField(const CShfeFtdcExchangeMarginRateAdjustField& ctp,ExchangeMarginRateAdjustField& risk);
	static void GetFullIndexNPPField(const CShfeFtdcFullIndexNPPField& ctp,FullIndexNPPField& risk);
	static void GetIndexNPPField(const CShfeFtdcIndexNPPField& ctp,IndexNPPField& risk);
	static void GetInputOrderField(const CShfeFtdcInputOrderField& ctp,InputOrderField& risk);
	static void GetInputOrderActionField(const CShfeFtdcInputOrderActionField& ctp,InputOrderActionField& risk);
	static void GetInstrumentField(const CShfeFtdcInstrumentField& ctp,InstrumentField& risk);
	static void GetInvestorDepartmentFlatField(const CShfeFtdcInvestorDepartmentFlatField& ctp,InvestorDepartmentFlatField& risk);
	static void GetInvestorGroupField(const CShfeFtdcInvestorGroupField& ctp,InvestorGroupField& risk);
	static void GetInvestorHashField(const CShfeFtdcInvestorHashField& ctp,InvestorHashField& risk);
	static void GetInvestorLinkManField(const CShfeFtdcInvestorLinkManField& ctp,InvestorLinkManField& risk);
	static void GetInvestorMarginRateField(const CShfeFtdcInvestorMarginRateField& ctp,InvestorMarginRateField& risk);
	static void GetInvestorPatternField(const CShfeFtdcInvestorPatternField& ctp,InvestorPatternField& risk);
	static void GetInvestorPositionStaticField(const CShfeFtdcInvestorPositionStaticField& ctp,InvestorPositionStaticField& risk);
	static void GetInvestorRiskAccountField(const CShfeFtdcInvestorRiskAccountField& ctp,InvestorRiskAccountField& risk);
	static void GetInvestorSumInfoField(const CShfeFtdcInvestorSumInfoField& ctp,InvestorSumInfoField& risk);
	static void GetInvestorTradeStaticField(const CShfeFtdcInvestorTradeStaticField& ctp,InvestorTradeStaticField& risk);
	static void GetNormalRiskQueryField(const CShfeFtdcNormalRiskQueryField& ctp,NormalRiskQueryField& risk);
	static void GetNoticePatternField(const CShfeFtdcNoticePatternField& ctp,NoticePatternField& risk);
	static void GetNoticeTokenField(const CShfeFtdcNoticeTokenField& ctp,NoticeTokenField& risk);
	static void GetOrderStatField(const CShfeFtdcOrderStatField& ctp,OrderStatField& risk);
	static void GetPositionStatField(const CShfeFtdcPositionStatField& ctp,PositionStatField& risk);
	static void GetPredictRiskAccountField(const CShfeFtdcPredictRiskAccountField& ctp,PredictRiskAccountField& risk);
	static void GetPredictRiskPositionField(const CShfeFtdcPredictRiskPositionField& ctp,PredictRiskPositionField& risk);
	static void GetPriceVaryParamField(const CShfeFtdcPriceVaryParamField& ctp,PriceVaryParamField& risk);
	static void GetProductField(const CShfeFtdcProductField& ctp,ProductField& risk);
	static void GetQueryBrokerDepositField(const CShfeFtdcQueryBrokerDepositField& ctp,QueryBrokerDepositField& risk);
	static void GetRemoveRiskParkedOrderField(const CShfeFtdcRemoveRiskParkedOrderField& ctp,RemoveRiskParkedOrderField& risk);
	static void GetRiskInvestorParamField(const CShfeFtdcRiskInvestorParamField& ctp,RiskInvestorParamField& risk);
	static void GetRiskLoginInfoField(const CShfeFtdcRiskLoginInfoField& ctp,RiskLoginInfoField& risk);
	static void GetRiskNotifyTokenField(const CShfeFtdcRiskNotifyTokenField& ctp,RiskNotifyTokenField& risk);
	static void GetRiskParamInfoField(const CShfeFtdcRiskParamInfoField& ctp,RiskParamInfoField& risk);
	static void GetRiskParkedOrderField(const CShfeFtdcRiskParkedOrderField& ctp,RiskParkedOrderField& risk);
	static void GetRiskPatternField(const CShfeFtdcRiskPatternField& ctp,RiskPatternField& risk);
	static void GetRiskSyncAccountField(const CShfeFtdcRiskSyncAccountField& ctp,RiskSyncAccountField& risk);
	static void GetRiskSyncInvestorField(const CShfeFtdcRiskSyncInvestorField& ctp,RiskSyncInvestorField& risk);
	static void GetRiskUserEventField(const CShfeFtdcRiskUserEventField& ctp,RiskUserEventField& risk);
	static void GetRiskUserFunctionField(const CShfeFtdcRiskUserFunctionField& ctp,RiskUserFunctionField& risk);
	static void GetRspForceClosePositionField(const CShfeFtdcRspForceClosePositionField& ctp,RspForceClosePositionField& risk);
	static void GetRspInfoField(const CShfeFtdcRspInfoField& ctp,RspInfoField& risk);
	static void GetRspInstPositionRateField(const CShfeFtdcRspInstPositionRateField& ctp,RspInstPositionRateField& risk);
	static void GetRspProductPositionRateField(const CShfeFtdcRspProductPositionRateField& ctp,RspProductPositionRateField& risk);
	static void GetRspRiskUserLoginField(const CShfeFtdcRspRiskUserLoginField& ctp,RspRiskUserLoginField& risk);
	static void GetRspSafePriceRangeField(const CShfeFtdcRspSafePriceRangeField& ctp,RspSafePriceRangeField& risk);
	static void GetSeqBizNoticeField(const CShfeFtdcSeqBizNoticeField& ctp,SeqBizNoticeField& risk);
	static void GetSeqDepositField(const CShfeFtdcSeqDepositField& ctp,SeqDepositField& risk);
	static void GetSeqPreRiskAccountField(const CShfeFtdcSeqPreRiskAccountField& ctp,SeqPreRiskAccountField& risk);
	static void GetSeqRiskNotifyBField(const CShfeFtdcSeqRiskNotifyBField& ctp,SeqRiskNotifyBField& risk);
	static void GetSeqRiskParkedOrderField(const CShfeFtdcSeqRiskParkedOrderField& ctp,SeqRiskParkedOrderField& risk);
	static void GetSequencialBrokerUserEventField(const CShfeFtdcSequencialBrokerUserEventField& ctp,SequencialBrokerUserEventField& risk);
	static void GetSequencialOrderField(const CShfeFtdcSequencialOrderField& ctp,SequencialOrderField& risk);
	static void GetSequencialPositionField(const CShfeFtdcSequencialPositionField& ctp,SequencialPositionField& risk);
	static void GetSequencialTradeField(const CShfeFtdcSequencialTradeField& ctp,SequencialTradeField& risk);
	static void GetSettlementSessionField(const CShfeFtdcSettlementSessionField& ctp,SettlementSessionField& risk);
	static void GetTradeParamField(const CShfeFtdcTradeParamField& ctp,TradeParamField& risk);
	static void GetTradeStatField(const CShfeFtdcTradeStatField& ctp,TradeStatField& risk);
	static void GetTradingCodeField(const CShfeFtdcTradingCodeField& ctp,TradingCodeField& risk);
	static void GetUserPasswordUpdateField(const CShfeFtdcUserPasswordUpdateField& ctp,UserPasswordUpdateField& risk);
	static void GetVaryMarketDataField(const CShfeFtdcVaryMarketDataField& ctp,VaryMarketDataField& risk);
private:
	ConvertFromCtp(void);
	virtual ~ConvertFromCtp(void);
};
