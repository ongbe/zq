#pragma once
#include "interface_svrquotserver.h"
#include "../SvrDBOpr/Interface_SvrDBOpr.h"
#include "ThreadManage.h"
#include "ThreadPoolSelfDefine.h"


// enum WEEKDAY{Sunday=0, Monday,Tuesday,Wednesday,Thursday,Friday,Saturday};

class CSvrQuotServerImpl : public CInterface_SvrQuotServer
{
public:
    static CSvrQuotServerImpl& getObj(void);
    static void deleteObj();


public:
    bool LoadConnectCtpParamFromDB(BrokerInfo& brokerInfo,std::map<std::string,std::vector<std::string>>& mapBrokerCode2AddrParam);
    bool LoadInstrumentsFromDB(const TThostFtdcDateType pTradeDay,std::set<std::string>& setInstrument);

    CF_ERROR GetReinitTime(sReinitTime& sTime);
    CF_ERROR SetReinitTime(const sReinitTime& sTime);
 
    void CreateDeleteThread();
    //清除2个交易日前的行情数据
    bool DeleteTradingDayData_ex();
	////删除QuotTrigger表中旧数据
	//bool DeleteQuotTriggerData_ex();
	//类初始化时借助线程池创建加载K线数据线程
	void CreateLoadKLineDataThread();

    //查询“某年某月某日某时某分”历史行情接口
    //nErrorCode -100 时间格式不正确
    bool QueryQuotData( const std::string strDateTime, 
        std::vector<PlatformStru_DepthMarketData_EX>& vMarketData ,
        int& nErrorCode);
	bool QueryKlineData(KLineQuery* pData ,int&lError,int nsocket,int nSeq);
	bool QuerySettlementPrice(KLineQuery* pData, int& lError, vector<SettlementPrice>& outData);
	static DWORD  WINAPI QueryKlineDBData(LPVOID lp);
	static DWORD  WINAPI QueryTimelineDBData(LPVOID lp);
	static DWORD  WINAPI LoadKLineDataThreadProc(void* lpParam);
	static DWORD  WINAPI LoadKLineData();

	CF_ERROR QueryVolatilityData(Volatility& pData);

	int GetRestLoadkLineDataThread();
private:
    //加载服务器自动重启时间
    void LoadServerReinitTime();

    //加载行情历史数据保存截止时间
    void LoadQuotDataSaveTime();

	static int GetParamFromString(std::string Str, std::vector<std::string>& StringVec, char Delim = ';');
	static void RespKlineData(int nsocket,int nSeq,std::vector<KLineBase>& vecKlineData);
	static void RespTlineData(int nsocket,int nSeq,std::vector<TLineBase>& vecTlineData);
	bool  GetKLineData(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
private:
    CSvrQuotServerImpl(void);
    virtual ~CSvrQuotServerImpl(void);
    static CSvrQuotServerImpl* m_pObj;

private:
    sReinitTime m_sReinitTime;
    _ExchangDateType m_HistoryDataSaveTime;

    //线程参数
    HANDLE	m_hThread;
    DWORD	m_idThread;

	HANDLE	m_hLoadKLineDataThread;
	DWORD	m_idLoadKLineDataThread;
	

private:
	CThreadManage* manage;
public:
	/*static std::vector<KLineBase> m_vecKlineData;
	static std::vector<TLineBase> m_vecTlineData;*/
	//互斥锁
	//Ceasymutex			m_mutex;
	//std::vector<KLineData>    m_vKlineData;
	bool	m_bKLineDataThreadLoading;//加载历史K线历史数据的线程是否完成
};
