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

		conSubscribeQuot,
		conUnSubscribeQuot,
		conDepthMarketData,
	
		conOrder,
		conTrade,
		conOrderInsert,
		conOrderError,
		conOrderActionFail,
		conOrderActionError,
		conQryTradingAccount,
		conQryOrders,
		conQryTrades,
		conQryPositions,
		conQryPositionDetails,
		conQryPositionDetailCombs,
		conQryCommissionRate,
		conQryMarginRate,

		conUploadStartRsp, 
		conUploadEndRsp, 

		conUploadIndicatorInfoRsp,
		conUploadIndicatorFileRsp,
	
		conUploadStrategyInfoRsp,
		conUploadStrategyFileRsp,

		conDownloadIndicatorListRsp,
		conDownloadIndicatorInfoRsp,
		conDownloadIndicatorFileRsp,
	
		conDownloadStrategyListRsp,
		conDownloadStrategyInfoRsp,
		conDownloadStrategyFileRsp,

		conResetStrategyUseStatusRsp,
		conDeleteStrategyRsp,

		conDownloadStrategyInstanceListRsp,
		conStrategyInstanceAddRsp,
		conStrategyInstanceResetRsp,
		conStrategyInstanceDeleteRsp,
		conStrategyInstanceRunStopRsp,

		conItemCount
	};

public:
	virtual void OnQuotConnected()=0;
	virtual void OnQuotDisconnected()=0;
	virtual void OnTraderConnected()=0;
	virtual void OnTraderDisconnected()=0;

	virtual void OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)=0;
	virtual void OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg)=0;

	virtual void OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum)=0;

	virtual void OnSubscribeQuot(const std::string& strInstrumentID, int nErrorNum)=0;
	virtual void OnUnSubscribeQuot(const std::string& strInstrumentID, int nErrorNum)=0;
	virtual void OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum)=0;
	
	virtual void OnOrder(const PlatformStru_OrderInfo& rsp, int nErrorNum)=0;
	virtual void OnTrade(const PlatformStru_TradeInfo& rsp, int nErrorNum)=0;
	virtual void OnOrderInsert(const PlatformStru_InputOrder& rsp, int nErrorNum)=0;
	virtual void OnOrderError(const PlatformStru_InputOrder& rsp, int nErrorNum)=0;
	virtual void OnOrderActionFail(const PlatformStru_InputOrderAction& rsp, int nErrorNum)=0;
	virtual void OnOrderActionError(const CThostFtdcOrderActionField& rsp, int nErrorNum)=0;
	virtual void OnTradingAccount(const PlatformStru_TradingAccountInfo& rsp, int nErrorNum)=0;
	virtual void OnOrders(const std::vector<PlatformStru_OrderInfo>& vec, int nErrorNum)=0;
	virtual void OnTrades(const std::vector<PlatformStru_TradeInfo>& vec, int nErrorNum)=0;
	virtual void OnPositions(const std::vector<PlatformStru_Position>& vec, int nErrorNum)=0;
	virtual void OnPositionDetails(const std::vector<PlatformStru_PositionDetail>& vec, int nErrorNum)=0;
	virtual void OnPositionDetailCombs(const std::vector<PlatformStru_PositionDetailComb>& vec, int nErrorNum)=0;
	virtual void OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum)=0;
	virtual void OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum)=0;

	virtual void OnUploadStartRsp(int nErrorNum, const std::string& strMsg)=0;
	virtual void OnUploadEndRsp(int nErrorNum, const std::string& strMsg)=0;

	virtual void OnUploadIndicatorInfoRsp(int nErrorNum, const std::string& strMsg)=0;
	virtual void OnUploadIndicatorFileRsp(int nErrorNum, int nFileType, const std::string& strMsg)=0;
	
	virtual void OnUploadStrategyInfoRsp(int nErrorNum, const std::string& strMsg)=0;
	virtual void OnUploadStrategyFileRsp(int nErrorNum, int nFileType, const std::string& strMsg)=0;

	virtual void OnDownloadIndicatorListRsp(const std::vector<SIndex>& vec, int nErrorNum, const std::string& strMsg)=0;
	virtual void OnDownloadIndicatorInfoRsp(const SIndex rsp, int nErrorNum, const std::string& strMsg)=0;
	virtual void OnDownloadIndicatorFileRsp(const std::string& strIndicatorID, int nFileType, LPVOID pFileData, int nFileLen, int nErrorNum, const std::string& strMsg)=0;
	
	virtual void OnDownloadStrategyListRsp(const std::vector<SStrategy>& vec, int nErrorNum, const std::string& strMsg)=0;
	virtual void OnDownloadStrategyInfoRsp(const SStrategy& rsp, int nErrorNum, const std::string& strMsg)=0;
	virtual void OnDownloadStrategyFileRsp(const std::string& strStrategyID, int nFileType, LPVOID pFileData, int nFileLen, int nErrorNum, const std::string& strMsg)=0;
	
	virtual void OnResetStrategyUseStatusRsp(int nErrorNum, const std::string& strMsg)=0;
	virtual void OnDeleteStrategyRsp(int nErrorNum, const std::string& strMsg)=0;

	virtual void OnDownloadStrategyInstanceListRsp(const std::vector<SStrategyInstance>& vec, int nErrorNum, const std::string& strMsg)=0;
	virtual void OnStrategyInstanceAddRsp(int nErrorNum, const std::string& strMsg)=0;
	virtual void OnStrategyInstanceResetRsp(int nErrorNum, const std::string& strMsg)=0;
	virtual void OnStrategyInstanceDeleteRsp(int nErrorNum, const std::string& strMsg)=0;
	virtual void OnStrategyInstanceRunStopRsp(int nErrorNum, const std::string& strMsg)=0;

protected:
	CBaseBusinessSpi() {};
	virtual ~CBaseBusinessSpi() {};
    
};
