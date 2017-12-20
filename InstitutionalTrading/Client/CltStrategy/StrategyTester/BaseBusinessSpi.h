#pragma once

#include <vector>
#include "Module-Misc2\KernelStruct.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonMacro.h"
#include "CommonStruct.h"


class CBaseBusinessSpi
{
public:
	enum {
		conNothing, 
		conQuotConnected,
		conQuotDisconnected,
		conTraderConnected,
		conTraderDisconnected,

		conQuotLogin,
		conTraderLogin,

		conQryInstrument,

		conDepthMarketData,
	
		conQryCommissionRate,
		conQryMarginRate,

		conItemCount
	};

public:
	virtual void OnQuotConnected()=0;
	virtual void OnQuotDisconnected()=0;
	virtual void OnTraderConnected()=0;
	virtual void OnTraderDisconnected()=0;

	virtual void OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)=0;
	virtual void OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)=0;

	virtual void OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum)=0;
	virtual void OnQueryQuotData(const vector<PlatformStru_DepthMarketData>& vec, int nErrorNum)=0;
	
	virtual void OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum)=0;
	virtual void OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum)=0;
	virtual void OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum)=0;

protected:
	CBaseBusinessSpi() {};
	virtual ~CBaseBusinessSpi() {};
    
};
