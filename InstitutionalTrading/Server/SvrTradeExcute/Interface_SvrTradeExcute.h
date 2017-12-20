#pragma once

#include "Interface_SvrCommon.h"

#ifndef SVRTRADEEXCUTE_API
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SVRUSERORG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TESTTCPSVR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SVRTRADEEXCUTE_EXPORTS
#define SVRTRADEEXCUTE_API __declspec(dllexport)
#else
#define SVRTRADEEXCUTE_API __declspec(dllimport)
#endif
#endif

#include <map>
#include <vector>
#include "CommonStruct.h"
#include "RiskManageStruct.h"
class SVRTRADEEXCUTE_API CInterface_SvrTradeExcute : public CInterface_SvrCommon
{
public:
    static CInterface_SvrTradeExcute& getObj(void);

public:
	//tss 调用，设置交易方向
	virtual void SetOneStrategyOrderLimit(const std::string &nUserName,
		const std::string & nInstrumentID,
		StrategyOrderLimit nCtrl) = 0;	

	//风控调用
	virtual bool RiskControl(const std::string &nUserName, 
		std::multimap<RiskWarningType, SRiskControl>& vecRiskControl) = 0;


	//风控下强平单
	virtual CF_ERROR RiskForceCloseOrder(PlatformStru_InputOrder & nInputOrder,
		const InputOrderKey& lKey,
		int nUserID,
		const std::string & nsForceReason) = 0;

	//风控撤挂单
	virtual CF_ERROR RiskForceActionOrder(PlatformStru_InputOrderAction & nInputOrderAction,
		int nUserID,
		int nRequestID,
		const std::string & nsActionReson) = 0;


	//审核报单
	virtual  CF_ERROR SetVerifyOrder(OrderKey& orderKey, bool bPassVerify) = 0;
};







