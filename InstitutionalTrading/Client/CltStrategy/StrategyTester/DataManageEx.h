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

	bool Init();
	void Release();

	void SubscribeEvent(UINT nEventID, HWND hWnd);
	void UnsubscribeEvent(UINT nEventID, HWND hWnd);
	void UnsubscribeAllEvent(HWND hWnd);

	bool LoginQuot(const std::string& strUserID, const std::string& strPasswd);
	bool LoginTrader(const std::string& strUserID, const std::string& strPasswd);
	bool LogoutQuot();
	bool LogoutTrader();

public:
	void OnQuotConnected();
	void OnQuotDisconnected();
	void OnTraderConnected();
	void OnTraderDisconnected();

	void OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg);
	void OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg);

	void OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum);

	void OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum);
	void OnQueryQuotData(const vector<PlatformStru_DepthMarketData>& vec, int nErrorNum) {};
	
	void OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum);
	void OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum);
	
protected:
	bool PostEventToWnd(int nEventID);

protected:
	HANDLE					m_mutex;
	CStrategyTraderApi*		m_pTraderApi;
	//CStrategyMdApi*			m_pMDApi;

	bool					m_bIsConnectedQuot;
	bool					m_bIsConnectedTrader;

	int						m_nLoginQuotStatus;
	int						m_nLoginTraderStatus;

	std::string				m_strLoginTraderFailMsg;

	std::map<UINT, std::set<HWND>>	m_mapSubs;


};
