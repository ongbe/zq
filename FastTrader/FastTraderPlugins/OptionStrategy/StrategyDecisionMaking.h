#pragma once
#include "CommPublic.h"
#include "KernelStruct.h"
class StrategyDecisionMaking
{
public:
	StrategyDecisionMaking(void);
	~StrategyDecisionMaking(void);

public:
	//*************************************************************************************************
	//	获取策略建议报单
	//	strategyID:					                [in]	    策略ID
	//	strUnderlyingInstrumentID: 	            	[in]	    标的物合约ID
	//  vector<PlatformStru_InstrumentInfo>         [in]        当前所有期权期货合约
	//	map<string,PlatformStru_DepthMarketData>    [in]        当前所有期权期货的行情
	//	outOrders:					               [out]	    建议报单列表
	//*************************************************************************************************
	bool GetOptionStrategyOrders(eStrategyID     strategyID,
								 const string      strUnderlyingInstrumentID,
		vector<PlatformStru_InstrumentInfo>  vecOptionIns,
		map<string,PlatformStru_DepthMarketData>   maplastQuot,

		vector<PlatformStru_OrderInfo>&      outOrders);

private:
	void GetMiniPriceIns(vector<PlatformStru_InstrumentInfo>& vecTempIns,PlatformStru_InstrumentInfo& outData);
	void GetMaxPriceIns(vector<PlatformStru_InstrumentInfo>& vecTempIns,PlatformStru_InstrumentInfo& outData);

};
