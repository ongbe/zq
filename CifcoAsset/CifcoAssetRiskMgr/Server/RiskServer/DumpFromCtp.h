#pragma once
#include "FtdcRiskUserApiStruct.h"
class DumpFromCtp
{
public:
	static void DumpBrokerDepositField(std::string& buf,const CShfeFtdcBrokerDepositField& ctp);
	static void DumpBrokerInvestorField(std::string& buf,const CShfeFtdcBrokerInvestorField& ctp);
	static void DumpCurrentTimeField(std::string& buf,const CShfeFtdcCurrentTimeField& ctp);
	static void DumpDepartmentField(std::string& buf,const CShfeFtdcDepartmentField& ctp);
	static void DumpDepthMarketDataField(std::string& buf,const CShfeFtdcDepthMarketDataField& ctp);
	static void DumpExchangeField(std::string& buf,const CShfeFtdcExchangeField& ctp);
	static void DumpExchangeMarginRateField(std::string& buf,const CShfeFtdcExchangeMarginRateField& ctp);
	static void DumpExchangeMarginRateAdjustField(std::string& buf,const CShfeFtdcExchangeMarginRateAdjustField& ctp);
	static void DumpFullIndexNPPField(std::string& buf,const CShfeFtdcFullIndexNPPField& ctp);
	static void DumpIndexNPPField(std::string& buf,const CShfeFtdcIndexNPPField& ctp);
	static void DumpInputOrderField(std::string& buf,const CShfeFtdcInputOrderField& ctp);
	static void DumpInputOrderActionField(std::string& buf,const CShfeFtdcInputOrderActionField& ctp);
	static void DumpInstrumentField(std::string& buf,const CShfeFtdcInstrumentField& ctp);
	static void DumpInvestorDepartmentFlatField(std::string& buf,const CShfeFtdcInvestorDepartmentFlatField& ctp);
	static void DumpInvestorGroupField(std::string& buf,const CShfeFtdcInvestorGroupField& ctp);
	static void DumpInvestorHashField(std::string& buf,const CShfeFtdcInvestorHashField& ctp);
	static void DumpInvestorLinkManField(std::string& buf,const CShfeFtdcInvestorLinkManField& ctp);
	static void DumpInvestorMarginRateField(std::string& buf,const CShfeFtdcInvestorMarginRateField& ctp);
	static void DumpInvestorPatternField(std::string& buf,const CShfeFtdcInvestorPatternField& ctp);
	static void DumpInvestorPositionStaticField(std::string& buf,const CShfeFtdcInvestorPositionStaticField& ctp);
	static void DumpInvestorRiskAccountField(std::string& buf,const CShfeFtdcInvestorRiskAccountField& ctp);
	static void DumpInvestorSumInfoField(std::string& buf,const CShfeFtdcInvestorSumInfoField& ctp);
	static void DumpInvestorTradeStaticField(std::string& buf,const CShfeFtdcInvestorTradeStaticField& ctp);
	static void DumpNormalRiskQueryField(std::string& buf,const CShfeFtdcNormalRiskQueryField& ctp);
	static void DumpNoticePatternField(std::string& buf,const CShfeFtdcNoticePatternField& ctp);
	static void DumpNoticeTokenField(std::string& buf,const CShfeFtdcNoticeTokenField& ctp);
	static void DumpOrderStatField(std::string& buf,const CShfeFtdcOrderStatField& ctp);
	static void DumpPositionStatField(std::string& buf,const CShfeFtdcPositionStatField& ctp);
	static void DumpPredictRiskAccountField(std::string& buf,const CShfeFtdcPredictRiskAccountField& ctp);
	static void DumpPredictRiskPositionField(std::string& buf,const CShfeFtdcPredictRiskPositionField& ctp);
	static void DumpPriceVaryParamField(std::string& buf,const CShfeFtdcPriceVaryParamField& ctp);
	static void DumpProductField(std::string& buf,const CShfeFtdcProductField& ctp);
	static void DumpQueryBrokerDepositField(std::string& buf,const CShfeFtdcQueryBrokerDepositField& ctp);
	static void DumpRemoveRiskParkedOrderField(std::string& buf,const CShfeFtdcRemoveRiskParkedOrderField& ctp);
	static void DumpRiskInvestorParamField(std::string& buf,const CShfeFtdcRiskInvestorParamField& ctp);
	static void DumpRiskLoginInfoField(std::string& buf,const CShfeFtdcRiskLoginInfoField& ctp);
	static void DumpRiskNotifyTokenField(std::string& buf,const CShfeFtdcRiskNotifyTokenField& ctp);
	static void DumpRiskParamInfoField(std::string& buf,const CShfeFtdcRiskParamInfoField& ctp);
	static void DumpRiskParkedOrderField(std::string& buf,const CShfeFtdcRiskParkedOrderField& ctp);
	static void DumpRiskPatternField(std::string& buf,const CShfeFtdcRiskPatternField& ctp);
	static void DumpRiskSyncAccountField(std::string& buf,const CShfeFtdcRiskSyncAccountField& ctp);
	static void DumpRiskSyncInvestorField(std::string& buf,const CShfeFtdcRiskSyncInvestorField& ctp);
	static void DumpRiskUserEventField(std::string& buf,const CShfeFtdcRiskUserEventField& ctp);
	static void DumpRiskUserFunctionField(std::string& buf,const CShfeFtdcRiskUserFunctionField& ctp);
	static void DumpRspForceClosePositionField(std::string& buf,const CShfeFtdcRspForceClosePositionField& ctp);
	static void DumpRspInfoField(std::string& buf,const CShfeFtdcRspInfoField& ctp);
	static void DumpRspInstPositionRateField(std::string& buf,const CShfeFtdcRspInstPositionRateField& ctp);
	static void DumpRspProductPositionRateField(std::string& buf,const CShfeFtdcRspProductPositionRateField& ctp);
	static void DumpRspRiskUserLoginField(std::string& buf,const CShfeFtdcRspRiskUserLoginField& ctp);
	static void DumpRspSafePriceRangeField(std::string& buf,const CShfeFtdcRspSafePriceRangeField& ctp);
	static void DumpSeqBizNoticeField(std::string& buf,const CShfeFtdcSeqBizNoticeField& ctp);
	static void DumpSeqDepositField(std::string& buf,const CShfeFtdcSeqDepositField& ctp);
	static void DumpSeqPreRiskAccountField(std::string& buf,const CShfeFtdcSeqPreRiskAccountField& ctp);
	static void DumpSeqRiskNotifyBField(std::string& buf,const CShfeFtdcSeqRiskNotifyBField& ctp);
	static void DumpSeqRiskParkedOrderField(std::string& buf,const CShfeFtdcSeqRiskParkedOrderField& ctp);
	static void DumpSequencialBrokerUserEventField(std::string& buf,const CShfeFtdcSequencialBrokerUserEventField& ctp);
	static void DumpSequencialOrderField(std::string& buf,const CShfeFtdcSequencialOrderField& ctp);
	static void DumpSequencialPositionField(std::string& buf,const CShfeFtdcSequencialPositionField& ctp);
	static void DumpSequencialTradeField(std::string& buf,const CShfeFtdcSequencialTradeField& ctp);
	static void DumpSettlementSessionField(std::string& buf,const CShfeFtdcSettlementSessionField& ctp);
	static void DumpTradeParamField(std::string& buf,const CShfeFtdcTradeParamField& ctp);
	static void DumpTradeStatField(std::string& buf,const CShfeFtdcTradeStatField& ctp);
	static void DumpTradingCodeField(std::string& buf,const CShfeFtdcTradingCodeField& ctp);
	static void DumpUserPasswordUpdateField(std::string& buf,const CShfeFtdcUserPasswordUpdateField& ctp);
	static void DumpVaryMarketDataField(std::string& buf,const CShfeFtdcVaryMarketDataField& ctp);
private:
private:
	DumpFromCtp(void);
	virtual ~DumpFromCtp(void);
};
