#pragma once

#include "BaseBusinessSpi.h"
#include "DataManage.h"
#include "StrategyMDApi.h"
#include "StrategyTraderApi.h"
#include <map>

class CTcpConnectSpi;
class CStrategyMdApi;
class CStrategyTraderApi;

class CDataManageEx : public CDataManage, public CBaseBusinessSpi
{
public:
	enum {	conLogout, conLogon, conLoginning, conLoginFail };

public:
	static CDataManageEx* NewInstance();
	static void DestroyInstance();
	static CDataManageEx* m_pInstance;
	static int sm_nCount;

public:
	CDataManageEx(void);
	virtual ~CDataManageEx(void);

	void LockObject() {
		//::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnLockObject() {
		//::ReleaseMutex(m_mutex);
	};

	CStrategyTraderApi* GetTraderApi() {
		return m_pTraderApi;
	};
	CStrategyMdApi* GetMDApi() {
		return m_pMDApi;
	};

	void ResetDTEObject(LPVOID pDTE) {
		m_pDTE = pDTE;
	};

	bool IsConnectedQuot() {
		return m_bIsConnectedQuot;
	};
	bool IsConnectedTrader() {
		return m_bIsConnectedTrader;
	};

	int GetLoginQuotStatus() {
		return m_nLoginQuotStatus;
	};
	int GetLoginTraderStatus() {
		return m_nLoginTraderStatus;
	};

	string GetLoginRspMsg() {
		return m_strLoginTraderFailMsg;
	};

	const std::map<std::string, PlatformStru_InstrumentInfo>& 
	GetInstrument() {
		return m_mapInstrument;
	};
	const std::map<std::string, PlatformStru_DepthMarketData>& 
	GetMarketDepth() {
		return m_mapMD;
	}
	const std::map<PositionKey, PlatformStru_Position>& 
	GetPositions() {
		return m_mapPositions;
	};
	const std::map<OrderKey, PlatformStru_OrderInfo>&
	GetOrders() {
		return m_mapOrders;
	};
	const std::map<TradeKey, PlatformStru_TradeInfo>& 
	GetTrades() {
		return m_mapTrades;
	};
	const PlatformStru_TradingAccountInfo& GetTradingAccountInfo() {
		return m_tradingAccount;
	};	
	const std::map<std::string, SStrategyInstance>& 
	GetStrategyInstances() {
		return m_mapStrategyInstances;
	};
	//const std::map<std::string, SStrategy>& GetDownloadStrategys() {
	//	return m_mapStrategys;
	//};
	// 通过交易所ID获得相关的品种
	bool GetProductID(const std::string& strExchangID, std::set<std::string>& setRetProductID);
	// 通过品种ID获得相关的合约信息
	bool GetInstrument(const std::string& strProductID, 
			std::map<std::string, PlatformStru_InstrumentInfo>& mapRetInstrument);

	bool Init();
	void Release();
	bool Reload(const std::string& strProjectPath);
	bool SaveList(const std::string& strProjectPath);

	void SubscribeEvent(UINT nEventID, HWND hWnd);
	void UnsubscribeEvent(UINT nEventID, HWND hWnd);
	void UnsubscribeAllEvent(HWND hWnd);

	bool LoginQuot(const std::string& strUserID, const std::string& strPasswd);
	bool LoginTrader(const std::string& strUserID, const std::string& strPasswd);
	bool LogoutQuot();
	bool LogoutTrader();

	bool ExportCodes(const std::set<std::string>& setStrategy, 
			const std::set<std::string>& setIndicator, const std::string& strExportFile);
	bool ImportCodes(const std::string& strImportFile, bool bReplaceAll);

	bool GetProjectFiles(const std::string& strPath, const std::set<std::string>& strProjects, 
			std::set<std::string>& setFiles);

	bool GetStrategyInstance(const std::string& strStrategy, SStrategyInstance& instance);
	bool ResetStrategyInstance(const std::string& strStrategy, SStrategyInstance& instance);
	bool DeleteStrategyInstance(const std::string& strStrategy);
	bool GetExchangeID(const std::string& strInstrumentID, std::string& strExchangeID);
	bool GetProductID(const std::string& strInstrumentID, std::string& strProductID);

	void ClearDownloadDatas();

public:
	void OnQuotConnected();
	void OnQuotDisconnected();
	void OnTraderConnected();
	void OnTraderDisconnected();

	void OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg);
	void OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg);

	void OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum);

	void OnSubscribeQuot(const std::string& strInstrumentID, int nErrorNum);
	void OnUnSubscribeQuot(const std::string& strInstrumentID, int nErrorNum);
	void OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum);
	
	void OnOrder(const PlatformStru_OrderInfo& rsp, int nErrorNum);
	void OnTrade(const PlatformStru_TradeInfo& rsp, int nErrorNum);
	void OnOrderInsert(const PlatformStru_InputOrder& rsp, int nErrorNum);
	void OnOrderError(const PlatformStru_InputOrder& rsp, int nErrorNum);
	void OnOrderActionFail(const PlatformStru_InputOrderAction& rsp, int nErrorNum);
	void OnOrderActionError(const CThostFtdcOrderActionField& rsp, int nErrorNum);
	void OnOrders(const std::vector<PlatformStru_OrderInfo>& vec, int nErrorNum);
	void OnTrades(const std::vector<PlatformStru_TradeInfo>& vec, int nErrorNum);
	void OnTradingAccount(const PlatformStru_TradingAccountInfo& rsp, int nErrorNum);
	void OnPositions(const std::vector<PlatformStru_Position>& vec, int nErrorNum);
	void OnPositionDetails(const std::vector<PlatformStru_PositionDetail>& vec, int nErrorNum);
	void OnPositionDetailCombs(const std::vector<PlatformStru_PositionDetailComb>& vec, int nErrorNum);
	void OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum);
	void OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum);

	void OnUploadStartRsp(int nErrorNum, const std::string& strMsg);
	void OnUploadEndRsp(int nErrorNum, const std::string& strMsg);

	void OnUploadIndicatorInfoRsp(int nErrorNum, const std::string& strMsg);
	void OnUploadIndicatorFileRsp(int nErrorNum, int nFileType, const std::string& strMsg);
	
	void OnUploadStrategyInfoRsp(int nErrorNum, const std::string& strMsg);
	void OnUploadStrategyFileRsp(int nErrorNum, int nFileType, const std::string& strMsg);

	void OnDownloadIndicatorListRsp(const std::vector<SIndex>& vec, int nErrorNum, const std::string& strMsg);
	void OnDownloadIndicatorInfoRsp(const SIndex rsp, int nErrorNum, const std::string& strMsg);
	void OnDownloadIndicatorFileRsp(const std::string& strIndicatorID, int nFileType, LPVOID pFileData, int nFileLen, int nErrorNum, const std::string& strMsg);
	
	void OnDownloadStrategyListRsp(const std::vector<SStrategy>& vec, int nErrorNum, const std::string& strMsg);
	void OnDownloadStrategyInfoRsp(const SStrategy& rsp, int nErrorNum, const std::string& strMsg);
	void OnDownloadStrategyFileRsp(const std::string& strStrategyID, int nFileType, LPVOID pFileData, int nFileLen, int nErrorNum, const std::string& strMsg);

	void OnDownloadFileNext();
	
	void OnResetStrategyUseStatusRsp(int nErrorNum, const std::string& strMsg);
	void OnDeleteStrategyRsp(int nErrorNum, const std::string& strMsg);

	void OnDownloadStrategyInstanceListRsp(const std::vector<SStrategyInstance>& vec, int nErrorNum, const std::string& strMsg);
	void OnStrategyInstanceAddRsp(int nErrorNum, const std::string& strMsg);
	void OnStrategyInstanceResetRsp(int nErrorNum, const std::string& strMsg);
	void OnStrategyInstanceDeleteRsp(int nErrorNum, const std::string& strMsg);
	void OnStrategyInstanceRunStopRsp(int nErrorNum, const std::string& strMsg);

	
protected:
	bool PostEventToWnd(int nEventID, int nErrorNum=0);

protected:
	HANDLE					m_mutex;
	CStrategyTraderApi*		m_pTraderApi;
	CStrategyMdApi*			m_pMDApi;
	LPVOID					m_pDTE;

	bool					m_bIsConnectedQuot;
	bool					m_bIsConnectedTrader;

	int						m_nLoginQuotStatus;
	int						m_nLoginTraderStatus;

	std::string				m_strLoginTraderFailMsg;

	std::map<UINT, std::set<HWND>>	m_mapSubs;

	std::map<std::string, PlatformStru_InstrumentInfo>
							m_mapInstrument;
	std::map<std::string, PlatformStru_DepthMarketData>
							m_mapMD;
	std::map<PositionKey, PlatformStru_Position>
							m_mapPositions;
	std::map<OrderKey, PlatformStru_OrderInfo>
							m_mapOrders;
	std::map<TradeKey, PlatformStru_TradeInfo>
							m_mapTrades;

	std::map<std::string, SStrategyInstance>
							m_mapStrategyInstances;
	//std::map<std::string, SStrategy>
	//						m_mapStrategys;

	std::map<std::string, bool>
							m_mapDownloadIndicatorsNext;
	std::map<std::string, bool>
							m_mapDownloadStrategiesNext;

	PlatformStru_TradingAccountInfo
							m_tradingAccount;

};
