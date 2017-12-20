#pragma once
#include "WriteLog.h"
#include <string>
#include <map>
#include <set>
#include <vector>
#include "..\..\..\FastTrader\inc\ctp\ThostFtdcUserApiStruct.h"
#include "..\..\..\FastTrader\inc\Module-Misc2\KernelStruct.h"
#include "easymutex.h"
#include "..\CommonClasses\CMemCache.h"
#include "..\CommonClasses\DataEntity.h"

using namespace std;


#include "../SvrTradeData/Interface_SvrTradeData.h";
#pragma comment(lib, "SvrTradeData_Local.lib")		


class CTradeSimulation
{
public:
	CTradeSimulation(void);
	~CTradeSimulation(void);
	bool init(std::string& nUserID, CDataEntity*	pDataEntity ,int ntradeRule, CMemCache* SubCache);
	bool Release();
	//行情接口
	CF_ERROR Quot(const PlatformStru_DepthMarketData& quot);
	static DWORD ThreadWorkerSubscribe(LPVOID pParameter);
	static void AddCommandCallback(const std::string & nsCommand);
	bool SubscribeCacheChannel();
	void DealCommand();
	void AddCommand(const std::string & nsCommand);

	typedef std::map<std::string, PlatformStru_DepthMarketData>   MAPQUOT;

public: static CTradeSimulation& getObj(void);
//private:static auto_ptr<CTradeSimulation> m_pObj;
       static CTradeSimulation* m_pObj;


private:

	static void ConvInput2Order(const PlatformStru_InputOrder &input,PlatformStru_OrderInfo& order);
	static void ConvOrder2Trade(const PlatformStru_OrderInfo &order, 
		PlatformStru_TradeInfo& trade);
    bool  IsInvalidValue(double fVal);  
	void GenerateCancelFail(const PlatformStru_InputOrderAction& action, PlatformStru_OrderInfo& order);
	bool Orderaction2String(std::string strRedis, PlatformStru_InputOrderAction& action);
	bool OrderInput2String(std::string strRedis,PlatformStru_InputOrder& orderInput);

	void LockObject();
	void UnlockObject();

    //推送报单
	void PushOrder(PlatformStru_OrderInfo& order);
	//推送成交
	void PushTrade(PlatformStru_TradeInfo& trade);
	void CancelOrderDeal(PlatformStru_OrderInfo & orderInfo,CF_ERROR nErrorCode);
	

	//下单接口
	//返回值：0 下单成功；1 用户未登录；2 重复的报单
	CF_ERROR InsertOrder(const PlatformStru_InputOrder &input);

	//撤单接口
	CF_ERROR CancelOrder(const PlatformStru_InputOrderAction& action);

	int GetNextSessionID() {
		n_nSessionID++;
		return n_nSessionID;
	};
	int GetNextSeqID() {
		m_nSeqID++;
		return m_nSeqID;
	};
	int GetNextOrderID() {
		m_nOrderID++;
		return m_nOrderID;
	};
	int GetNextTradeID() {
		m_nTradeID++;
		return m_nTradeID;
	};
	bool MarkOrderBase(PlatformStru_OrderInfo& order);
	bool MarkOrderSys(PlatformStru_OrderInfo& order);
	bool MarkTrade(PlatformStru_TradeInfo& trade,  const PlatformStru_DepthMarketData& quot,int tradeRule);





protected:

	CDataEntity*	m_pDataEntity;//记录测试数据
	std::string     m_nUserID;//测试使用的交易员账号
	int             m_nTradeRule;//成交规则：1-最新价，2-买卖价，3-按买卖价触发，触发的按买卖量成交

	//互斥锁
	Ceasymutex			m_mutex;

	std::string			m_strTradingDay;
	std::string			m_strRspOrderTime;

	MAPQUOT				m_mapQuot;


	int					n_nSessionID;
	int					m_nSeqID;
	int					m_nOrderID;
	int					m_nTradeID;

	//CMemCache           mCache;
	
	std::queue<std::string> mCommandList;


	//线程参数
	HANDLE				m_hThreadSubscribe;



	CMemCache*           mSubCache;

	double               m_preDynamicProfit;
	int                  m_TotalTradingTime;
	
};
