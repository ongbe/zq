#pragma once
#include "FtdcRiskUserApiStruct.h"
class Convert2Ctp
{
public:
	static void  GetBizNoticeField(const BizNoticeField& risk,CShfeFtdcBizNoticeField& ctp);
	static void  GetBrokerInvestorField(const BrokerInvestorField& risk,CShfeFtdcBrokerInvestorField& ctp);
	static void  GetForceCloseListField(const ForceCloseListField& risk,CShfeFtdcForceCloseListField& ctp);
	static void  GetForceClosePositionField(const ForceClosePositionField& risk,CShfeFtdcForceClosePositionField& ctp);
	static void  GetForceClosePositionRuleField(const ForceClosePositionRuleField& risk,CShfeFtdcForceClosePositionRuleField& ctp);
	static void  GetForceCloseStandardField(const ForceCloseStandardField& risk,CShfeFtdcForceCloseStandardField& ctp);
	static void  GetIndexNPPField(const IndexNPPField& risk,CShfeFtdcIndexNPPField& ctp);
	static void  GetInputOrderActionField(const InputOrderActionField& risk,CShfeFtdcInputOrderActionField& ctp);
	static void  GetInstrumentPriceField(const InstrumentPriceField& risk,CShfeFtdcInstrumentPriceField& ctp);
	static void  GetInvestorIDRangeField(const InvestorIDRangeField& risk,CShfeFtdcInvestorIDRangeField& ctp);
	static void  GetInvestorPatternField(const InvestorPatternField& risk,CShfeFtdcInvestorPatternField& ctp);
	static void  GetNormalRiskQueryField(const NormalRiskQueryField& risk,CShfeFtdcNormalRiskQueryField& ctp);
	static void  GetNoticePatternField(const NoticePatternField& risk,CShfeFtdcNoticePatternField& ctp);
	static void  GetNotifySequenceField(const NotifySequenceField& risk,CShfeFtdcNotifySequenceField& ctp);
	static void  GetPredictRiskParamField(const PredictRiskParamField& risk,CShfeFtdcPredictRiskParamField& ctp);
	static void  GetPriceRangeField(const PriceRangeField& risk,CShfeFtdcPriceRangeField& ctp);
	static void  GetPriceVaryParamField(const PriceVaryParamField& risk,CShfeFtdcPriceVaryParamField& ctp);
	static void  GetProductLimitsField(const ProductLimitsField& risk,CShfeFtdcProductLimitsField& ctp);
	static void  GetQryInstPositionRateField(const QryInstPositionRateField& risk,CShfeFtdcQryInstPositionRateField& ctp);
	static void  GetQryInvestorMarginRateField(const QryInvestorMarginRateField& risk,CShfeFtdcQryInvestorMarginRateField& ctp);
	static void  GetQryPriceVaryEffectField(const QryPriceVaryEffectField& risk,CShfeFtdcQryPriceVaryEffectField& ctp);
	static void  GetQryProductPositionRateField(const QryProductPositionRateField& risk,CShfeFtdcQryProductPositionRateField& ctp);
	static void  GetQrySafePriceRangeField(const QrySafePriceRangeField& risk,CShfeFtdcQrySafePriceRangeField& ctp);
	static void  GetQryStatField(const QryStatField& risk,CShfeFtdcQryStatField& ctp);
	static void  GetQueryBrokerDepositField(const QueryBrokerDepositField& risk,CShfeFtdcQueryBrokerDepositField& ctp);
	static void  GetRemoveRiskParkedOrderField(const RemoveRiskParkedOrderField& risk,CShfeFtdcRemoveRiskParkedOrderField& ctp);
	static void  GetReqRiskUserLoginField(const ReqRiskUserLoginField& risk,CShfeFtdcReqRiskUserLoginField& ctp);
	static void  GetRiskForceCloseOrderField(const RiskForceCloseOrderField& risk,CShfeFtdcRiskForceCloseOrderField& ctp);
	static void  GetRiskInvestorParamField(RiskInvestorParamField& risk,CShfeFtdcRiskInvestorParamField& ctp);
	static void  GetRiskLoginInfoField(RiskLoginInfoField& risk,CShfeFtdcRiskLoginInfoField& ctp);
	static void  GetRiskPatternField(RiskPatternField& risk,CShfeFtdcRiskPatternField& ctp);
	static void  GetSTPriceField(STPriceField& risk,CShfeFtdcSTPriceField& ctp);
	static void  GetRiskNotifyAField(const RiskNotifyAField& risk,CShfeFtdcRiskNotifyAField& ctp);
	static void  GetRiskNotifyCommandField(const RiskNotifyCommandField& risk,CShfeFtdcRiskNotifyCommandField& ctp);
	static void  GetRiskNtfSequenceField(const RiskNtfSequenceField& risk,CShfeFtdcRiskNtfSequenceField& ctp);
	static void  GetRiskParkedOrderField(const RiskParkedOrderField& risk,CShfeFtdcRiskParkedOrderField& ctp);
	static void  GetRiskUserEventField(const RiskUserEventField& risk,CShfeFtdcRiskUserEventField& ctp);
	static void  GetSTMarginRateField(const STMarginRateField& risk,CShfeFtdcSTMarginRateField& ctp);
	static void  GetSubMarketDataField(const SubMarketDataField& risk,CShfeFtdcSubMarketDataField& ctp);
	static void  GetUserPasswordUpdateField(const UserPasswordUpdateField& risk,CShfeFtdcUserPasswordUpdateField& ctp);

private:
	Convert2Ctp(void);
	virtual ~Convert2Ctp(void);

};
