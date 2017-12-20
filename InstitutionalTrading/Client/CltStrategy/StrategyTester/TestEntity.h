#pragma once

#include <queue>
#include "..\CommonClasses\DataEntity.h"
#include "..\CommonClasses\CMemCache.h"
#include "BaseBusinessSpi.h"
#include "TradeSimulation.h"
#include "..\SvrTradeData\Interface_SvrTradeData.h"

using namespace std;


#include "../SvrTradeData/Interface_SvrTradeData.h";
#pragma comment(lib, "SvrTradeData_Local.lib")	

// 类CTestEntity，用集成数据管理模块、模拟交易模块，并进行初始化运行release等工作，
// 读取配置文件，并实现行情推送，加载RunSingleStrategy.exe。
// 主要功能：
// 1.  持有CDataEntity，并负责测试完成后保存工作。
// 2.  创建管理测试线程。
// 3.  读取与模拟交易有关的配置文件。
// 4.  创建初始化数据管理模块。
// 5.  创建初始化模拟交易模块。
// 6.  向数据管理模块、模拟交易模块推送行情。
// 7.  关闭测试线程，卸载数据管理模块、模拟测试模块。
// 8.  保存生产的数据。
// 9.  暂停、启动、终止未完成的测试过程。


class CTestEntity : public CBaseBusinessSpi
{
public:
	enum {	conRunNone,			// 未运行
			conRunning,			// 运行中
			conRunPause,		// 运行暂停
			conRunFinish		// 运行完成
	};

	static DWORD WINAPI Proc_Thread(LPVOID lpParam);
	static void GetNextDateTime(int& nNextDate, int& nNextMinute);
	static int ConvTime(char* pcsTime);

public:
	CTestEntity(void);
	virtual ~CTestEntity(void);

	void LockObject() {
		::WaitForSingleObject(m_mutex, INFINITE);
	};
	void UnlockObject() {
		::ReleaseMutex(m_mutex);
	};

public:
	int  GetRunStatus() {
		return m_nRunStatus;
	};

	void SetDataEntity(CDataEntity* pDataEntity) {
		m_pDataEntity = pDataEntity;
	};
	CDataEntity* GetDataEntity() {
		return m_pDataEntity;
	};

	void SetInstrumentIDs(std::string& strIDs) {
		m_strInstrumentIDs = strIDs;
	};
	const std::string& GetInstrumentIDs() {
		return m_strInstrumentIDs;
	};
	void SetRunPath(const std::string& strPath) {
		m_strRunPath = strPath;
	};
	const std::string& GetRunPath() {
		return m_strRunPath;
	};
	void SetQuotFromFile(bool bHas) {
		m_bQuotFromFile = bHas;
	};
	bool GetQuotFromFile() {
		return m_bQuotFromFile;
	};
	void ResetQuotRange(int nStartDate, int nEndDate) {
		m_nStartDate = nStartDate;
		m_nEndDate = nEndDate;
	};
	void GetQuotRange(int& nStartDate, int& nEndDate) {
		nStartDate = m_nStartDate;
		nEndDate = m_nEndDate;
	};
	void SetQuotFile(const std::string& strFile) {
		m_strQuotFile = (std::string&)strFile;
	};
	const std::string& GetQuotFile() {
		return m_strQuotFile;
	};
	int GetCurrQuotPos() {
		return m_nCurrQuotPos;
	};

	bool Init(const std::vector<std::string>& vecInstrument, 
			CStrategyDataEx& strategy, const StrategyTestParams& testParams);
	void Start();
	void Pause();
	void Stop();
	void Release();


public:
	void OnQuotConnected();
	void OnQuotDisconnected();
	void OnTraderConnected();
	void OnTraderDisconnected();

	void OnQuotLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg);
	void OnTraderLogin(const sTraderLoginRsp& rsp, int nErrorNum, const std::string& strErrorMsg);

	void OnInstrument(const std::vector<PlatformStru_InstrumentInfo>& vec, int nErrorNum);

	void OnDepthMarketData(const PlatformStru_DepthMarketData& rsp, int nErrorNum);
	void OnQueryQuotData(const vector<PlatformStru_DepthMarketData>& vec, int nErrorNum);
	
	void OnCommissionRate(const PlatformStru_InstrumentCommissionRate& rsp, int nErrorNum);
	void OnMarginRate(const PlatformStru_InstrumentMarginRate& rsp, int nErrorNum);


protected:
	bool loadXMLString(std::string& strStraID, std::string& strStraParams, 
						std::string& strInstruments, std::string& strStraRunPath, 
						StrategyTestParams& testParams, std::string& strXML);
	bool CreateRunStrategyExe();

	// 执行策略测试过程。只能在Proc_Thread函数中调用。
	void Run();

	// 只能在Run函数中调用。
	const bool GetNextQuotItem(PlatformStru_DepthMarketData& quot, bool& bEnd);
	// 只能在Run函数中调用。
	void PushQuotToSimulate(const PlatformStru_DepthMarketData& quot);

protected:
	HANDLE			m_mutex;
	HANDLE			m_hThread;
	DWORD			m_nThreadID;
	HANDLE			m_hRunStrategyProcess;
	CMemCache		m_memCache;
	CMemCache		m_memSubCache;
	CDataEntity*	m_pDataEntity;
	CStrategyMdApi*	m_pMDApi;
	CTradeSimulation	m_oTradeSimu;
	CInterface_SvrTradeData*	m_pTradeData;

	HANDLE			m_hQuotFile;
	int				m_nRunStatus;
	std::string		m_strRunPath;
	std::string		m_strInstrumentIDs;
	bool			m_bQuotFromFile;
	int				m_nStartDate;
	int				m_nEndDate;
	int				m_nNextDate;
	int				m_nNextMinute;
	std::string		m_strQuotFile;

	bool			m_bIsConnectedQuot;
	int				m_nCurrQuotPos;
	int				m_nQuotUpdateTime;
	bool			m_bWaitQuotQuery;
	
	std::list<PlatformStru_DepthMarketData>::iterator 
					m_itDMQuotList;
	std::queue<PlatformStru_DepthMarketData>
					m_queueQuot;
	std::vector<std::string>	m_vecInstrument;
	CStrategyDataEx	m_strategy;
	StrategyTestParams m_testParams;

};
