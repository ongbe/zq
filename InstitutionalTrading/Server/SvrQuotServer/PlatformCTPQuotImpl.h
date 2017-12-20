#pragma once
#include "PlatformCTPQuot.h"
#include "Tools_Win32.h"
#include <queue>
using std::queue;

#define SAVE_TO_DB_INTERVAL 5*60//5 minute
#define LAST_SETTLE_TIME 15*60 //15minute
    

class CPlatformCTPQuotImpl :  public CPlatformCTPQuot
{
public:
    static CPlatformCTPQuotImpl& GetInstance(void);
    static void DeleteInstance();

public:
    CPlatformCTPQuotImpl(const Stru_CTPConnectionParam& ConnParam=Stru_CTPConnectionParam(),
                        bool bPushCache=true,
                        bool bUseComb=false,
                        bool bCalcuCombQuotOnLegQuot=false);
    virtual ~CPlatformCTPQuotImpl(void);

public:
    bool ReInitCtpConnect();

    int  SubscribeClientMarketData(const string& InstrumentID,int hSocket);
    int  UnSubscribeClientMarketData(const string& InstrumentID, int hSocket);
    void DeleteClientSocketFromMap(int nSocket);

    void Clear();

    void SetQuit(int nVal);
	bool GetQuit();
	bool GetQueueQuot(std::vector<PlatformStru_DepthMarketData>& vecData);
	bool GetQueueQuotForKLine( PlatformStru_DepthMarketData& outData, int& nRemainCount);
	void GetSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec);
    void SetSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec);
	void GetAppSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec);
	void SetAppSubscribedInst2Socket(std::map<std::string,std::set<SOCKET>>& vec);

    void GetCTPConnectParam(BrokerInfo& brokerInfo,std::map<std::string, std::vector<std::string>>& mapBrokerCode2AddrParam);
    void SetCTPConnectParam(BrokerInfo& brokerInfo,std::map<std::string, std::vector<std::string>>& mapBrokerCode2AddrParam);
    ///获取成功订阅行情列表
    vector<string> GetSubscribedInstrumentIDs(void) const;

	int IsValidUser(const std::string& strAccout, const std::string& strPwd);
	void SetValidSocket(int hSocket);
	bool IsValidSocket( int hSocket);
	bool IsValidInstrument( std::string strIns);
	int PushLastQuot(const string& InstrumentID,int hSocket);
	int UnSubscribeAppClientMarketData(const string& InstrumentID, int hSocket);
	time_t GetLoginQuotTime();
	void SetIns(map<std::string,PlatformStru_InstrumentInfo>& data);
	/*CThostFtdcTraderApi& GetSHSpi();*/

	void SetDataInTime(PlatformStru_DepthMarketData* pData);
	string GetTradingDay();


protected:
	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    ///深度行情通知
    void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
    ///订阅行情应答
    void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    void OnFrontDisconnected(int nReason);

private:
    void CopyDepthMarketData( PlatformStru_DepthMarketData& data, const CThostFtdcDepthMarketDataField& depthMarketData );
	void SetExchangeID(CThostFtdcDepthMarketDataField *pDepthMarketData);
	bool SaveSettlementPrice2DB(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void VolatilityCalc(std::string strInstrumentID,string strTradingday);
	bool SaveVolatility2DB(Volatility* pVolatility);
	void GetCfgInfo();
	void split(const std::string& s, char c, std::vector<std::string>& v) ;
	
	BOOL SetInTimeData_KLine(PlatformStru_DepthMarketData* pData);
	//BOOL SetInTimeData_TLine(PlatformStru_DepthMarketData* pData);
	BOOL SetInTimeData_DiffSpan(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,std::string exchangeID);
	BOOL SetInTimeData_MINS(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine, int nTimeSpan ,vector<struct tm>& vecTimeScales);
	BOOL SetInTimeData_Month(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
	BOOL SetInTimeData_Week(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
	BOOL SetInTimeData_Day(string& strInstrumentID, vector<KLineBase>& vecKLinePara, KLineBase& pKLine,vector<struct tm>& vecTimeScales);
	bool IsOneMonth(struct tm tm1, struct tm tm2);
	bool IsOneWeek(struct tm tm1, struct tm tm2);
	struct tm GetTimeInSpan(struct tm tmPara, int nSpan,vector<struct tm>& vecTimeScales);
	//BOOL SetInTimeData_DiffSpan_TLine(TLineBase& pTLine,std::string exchangeID);
	//struct tm GetTimeInSpan_TLine(struct tm tmPara, int nSpan,vector<struct tm>& vecTimeScales);
	int GetDayOfYear(struct tm tm1);
	BOOL SaveKline2DB(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase,std::string& exchangeID);
	//BOOL SaveKLineData(string strName,KLineBase& pKLine, int nErrorCode);
	BOOL UpdateKLineData(string& strInstrumentID, EnumPhrase phrase, KLineBase& pKLine, bool bInsert, int& nErrorCode);

	BOOL SaveKline2Cache(string& strInstrumentID, KLineBase& pKLine, EnumPhrase enumPhrase);


	

	


private:
    static CPlatformCTPQuotImpl* m_pInstance;

    Ceasymutex      m_mutexImpl;
	Ceasymutex      m_mutexKLine;//k线计算的锁
	Ceasymutex      m_mutexKLineM1,m_mutexKLineM5,m_mutexKLineM15,m_mutexKLineM30,m_mutexKLineM60,m_mutexKLineDay,m_mutexKLineWeek,m_mutexKLineMonth;

    volatile int    m_nQuit;
    queue<PlatformStru_DepthMarketData>      m_queueQuot; //这里保存每条深度行情数据，每10分钟存一次数据库
	queue<PlatformStru_DepthMarketData>      m_queueForKline; //用于K线计算的queue
    map<string, set<SOCKET>>    m_SubscribedInstrumentID2Socket;//当前订阅行情的合约与socket关系
    map<SOCKET, set<string>>    m_SendedSocket2Inst;//保存已经发送过的Socket，避免与CTP断开又重连接后重复发送。
	std::set<SOCKET> m_ValidSockets;
	vector<sLoginReq>  m_vecUsers;//用户登录密码信息
	map<string, set<SOCKET>>    m_AppSubscribedInstrumentID2Socket;
	
	


    //线程参数
    HANDLE	m_hThread;
    DWORD	m_idThread; 
	HANDLE	m_hThreadKLine;
	DWORD	m_idThreadKLine; 

    BrokerInfo m_brokerInfo;
    std::map<std::string, std::vector<std::string>> m_mapBrokerCode2AddrParam; 
	map<std::string,PlatformStru_InstrumentInfo>   m_mapIns;
	std::map<std::string,std::vector<struct tm>> m_mapExchangeTime;			//各交易所交易时间 小到大排序
	std::map<std::string,std::vector<TLineBase>>  m_mapTLine;
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin1;         //所有合约一分钟K线
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin5;         //所有合约5分钟K线
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin15;         //所有合约15分钟K线
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin30;         //所有合约30分钟K线
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMin60;         //所有合约60分钟K线
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineDay;         //所有合约日K线
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineWeek;         //所有合约周K线
	std::map<std::string,std::vector<KLineBase>>		m_mapKLineMonth;         //所有合约月K线
	//std::map<std::string,std::map<time_t, KLineBase>>		m_mapKLineMin1;         //所有合约一分钟K线
    std::map<std::string,KLineBase>			  m_mapKLineLast;
	std::map<std::string,TLineBase>			  m_mapTLineLast;				//最后一秒种的实时行情
	map<std::string,KLineBase >                 m_maptmLast1;//上一个K线
	map<std::string,KLineBase >                 m_maptmLast5;
	map<std::string,KLineBase >                 m_maptmLast15;
	map<std::string,KLineBase >                 m_maptmLast30;
	map<std::string,KLineBase >                 m_maptmLast60;
	map<std::string,KLineBase >                 m_maptmLastDay;
	map<std::string,KLineBase >                 m_maptmLastWeek;
	map<std::string,KLineBase >                 m_maptmLastMonth;

	std::set<std::string>					m_setTableName;		// 已创建KLine存储表的名称

	//历史和今日数据	
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin1;         //所有合约一分钟K线
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin5;         //所有合约5分钟K线
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin15;         //所有合约15分钟K线
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin30;         //所有合约30分钟K线
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMin60;         //所有合约60分钟K线
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineDay;         //所有合约日K线
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineWeek;         //所有合约周K线
	std::map<std::string,map<time_t,KLineBase>>		m_vmapKLineMonth;         //所有合约月K线

    
public:
    void OnRtnDepthMarketDataSimulate(PlatformStru_DepthMarketData *pDepthMarketData); 
    void SetLastQuot(const PlatformStru_DepthMarketData& data);
    bool GetLastQuot(const string& InstrumentID, PlatformStru_DepthMarketData& outData);
	std::string MakeTableName(string& strInstrumentID, EnumPhrase phrase);

	bool RecalcWeekMonthKLine();

	//加载历史数据
	void SetKLineMin1(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMin5(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMin15(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMin30(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMin60(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineDay(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineWeek(std::map<std::string,map<time_t,KLineBase>> mapKLine);
	void SetKLineMonth(std::map<std::string,map<time_t,KLineBase>> mapKLine);

	bool GetKLineMin1(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMin5(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMin15(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMin30(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMin60(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineDay(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineWeek(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);
	bool GetKLineMonth(KLineQuery* pData,vector<KLineBase>& outData,vector<TLineBase>& outTLData);

private:
    map<string,PlatformStru_DepthMarketData> m_lastQuotImpl;	//这里保存每个模拟合约的最新行情
	string            m_strTradingDay;//保存交易日
	int               m_nTimeSpan;//日，周、月K线存储间隔
	struct           tm m_LastDBTimeDay;
	struct           tm m_LastDBTimeWeek;
	struct           tm m_LastDBTimeMonth;
	struct tm        m_tmCloseTime;
	bool			 m_bHasRecalcWeekMonthKLine;

public:
	time_t m_LoginQuotTime;
	/*static CThostFtdcTraderApi *m_pUserApi;*/
};
