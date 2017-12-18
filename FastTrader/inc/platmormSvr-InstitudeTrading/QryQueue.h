#pragma once


#include <map>
#include <list>
#include <set>
#include <string>
#include "Module-Misc2/BusinessData.hpp"
#include "Module-Misc2/packagecache.h"
//#include "Module-Misc2/platformstruct.h"


class CPackageCache;
class zqWriteLog;


//下面定义每种查询的宏
#define QryID_ReqQryInstrument                      1001            //查询合约信息
#define QryID_ReqQryTradingAccount                  1002            //查询资金
#define QryID_ReqQryTrade                           1003            //查询成交
#define QryID_ReqQryOrder                           1004            //查询报单
#define QryID_ReqQryInvestorPositionDetail          1005            //查询持仓明细
#define QryID_ReqQryInvestorPosition                1006            //查询持仓汇总
#define QryID_ReqQryInstrumentCommissionRate        1007            //查询合约手续费率
#define QryID_ReqQryInstrumentMarginRate            1008            //请求查询合约保证金率
#define QryID_ReqQrySettlementInfo                  1009            //请求查询投资者结算结果
#define QryID_ReqQryDepthMarketData                 1010            //请求查询行情

#define QryID_ReqQryInvestorPositionCombineDetail   1011            //请求查询投资者持仓明细(组合)
#define QryID_ReqQryNotice                          1012            //请求查询客户通知
#define QryID_ReqQrySettlementInfoConfirm           1013            //请求查询结算信息确认
#define QryID_ReqQryTransferBank                    1014            //请求查询转帐银行
#define QryID_ReqQryTransferSerial                  1015            //请求查询转帐流水
#define QryID_ReqQryContractBank                    1016            //请求查询签约银行
#define QryID_ReqQryAccountregister                 1017            //请求查询银期签约关系
#define QryID_ReqQryParkedOrder                     1018            //请求查询预埋单
#define QryID_ReqQryParkedOrderAction               1019            //请求查询预埋撤单
#define QryID_ReqQueryBankAccountMoneyByFuture      1020            //期货发起查询银行余额请求
#define QryID_ReqQryExchangeRate                    1021            //请求查询汇率

#define QryID_ReqQryCFMMCTradingAccountKey          1030            //请求查询保证金监管系统经纪公司资金账户密钥

class CPlatFormService;
class CPlatFormParam;
class CQryQueue
{
public:
    CQryQueue();
    ~CQryQueue(void);
    void StopThread();
	void SetPlatFormService(CPlatFormService* pPlatFormService){m_pPlatFormService=pPlatFormService;}
    //是否需要线程退出
    bool IsThreadNeedExit(void);                                  

    //是否到达查询时刻。ctp规定每秒只能有一次查询 
    bool IsQryTime(void);
	//
	bool TradeStatusIsConnected(void);
    //更新查询时刻
    void UpdateQryTime(void);

    //发送查询请求，返回值表示发送是否成功
    //ReqRlt表示如果进行了查询，查询返回值，<0表示查询失败
    bool SendQryCmd(int& ReqRlt);

    //检查ReqChache是否为空
    bool IsCacheEmpty(void);
    bool IsCache2Empty(void);
    //请求查询合约
    int ReqQryInstrument(PlatformStru_QryInstrument &QryInstrument, int nRequestID);

    //请求查询资金账户
    int ReqQryTradingAccount(PlatformStru_QryTradingAccount &QryTradingAccount, int nRequestID);

    //请求查询成交
    int ReqQryTrade(PlatformStru_QryTrade &QryTrade, int nRequestID);

    //请求查询报单
    int ReqQryOrder(PlatformStru_QryOrder &QryOrder, int nRequestID);

    //请求查询投资者持仓明细
    int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &QryInvestorPositionDetail, int nRequestID);

    //请求查询投资者持仓
    int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &QryInvestorPosition, int nRequestID);

    //请求查询合约手续费率
    int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField &QryInstrumentCommissionRate, int nRequestID);

    //请求查询合约保证金率
    int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField &QryInstrumentMarginRate, int nRequestID) ;

    //请求查询投资者结算结果
    int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &QrySettlementInfo, int nRequestID);

    //请求查询保证金监管系统经纪公司资金账户密钥
    int ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID);

    //请求查询行情
    int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &QryDepthMarketData, int nRequestID);

    ///请求查询投资者持仓明细(组合)
    int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &pQryInvestorPositionCombineDetail, int nRequestID=0) ;

    ///请求查询客户通知
    int ReqQryNotice(PlatformStru_QryNotice &pQryNotice, int nRequestID=0) ;

    ///请求查询结算信息确认
    int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &pQrySettlementInfoConfirm, int nRequestID=0) ;

    ///请求查询转帐银行
    int ReqQryTransferBank(PlatformStru_QryTransferBank &pQryTransferBank, int nRequestID=0) ;

    ///请求查询转帐流水
    int ReqQryTransferSerial(PlatformStru_QryTransferSerial &pQryTransferSerial, int nRequestID=0) ;

    ///请求查询签约银行
    int ReqQryContractBank(PlatformStru_QryContractBank &pQryContractBank, int nRequestID=0) ;
        
    ///请求查询银期签约关系
	int ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID=0);

    ///请求查询汇率
    int ReqQryExchangeRate(PlatformStru_QryExchangeRate &QryExchangeRate, int nRequestID=0);

    ///请求查询预埋单
    int ReqQryParkedOrder(PlatformStru_QryParkedOrder &pQryParkedOrder, int nRequestID=0) ;

    ///请求查询预埋撤单
    int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &pQryParkedOrderAction, int nRequestID=0) ;

    ///期货发起查询银行余额请求
    int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &pReqQueryAccount, int nRequestID=0) ;

    //清除查询请求队列中指定类型的查询请求，用于TCP重连后的重查前的处理
    //ReqCmdID:1:资金查询；2:成交查询；4:报单查询；8:持仓明细查询；16:持仓查询；32:组合持仓明细查询
    //ReqCmdID可以或处理
    void ClearReqInQryQueue(int ReqCmdID);


    static void ClearbInQry(void) { m_bInQry=false; }
    //static void NotifyStartStopInitQry(bool bStart) { m_bDoingInitQry=bStart?true:false; }


private:

    CPackageCache *m_pReqQueue;                             //优先查询队列。队列按先进先出顺序查询
    CPackageCache *m_pReqQueue2;                            //次级查询队列，放费率查询请求。队列按先进先出顺序查询    
    bool    m_bNeedExit;                                    //线程是否需要退出
    DWORD   m_LastQryTime;                                  //上次查询的时间
    HANDLE  m_hThread;                                      //线程句柄
    
    //线程函数
    static DWORD ThreadProc(void *arg);

    CPlatFormService *m_pPlatFormService;

    static bool m_bInQry;                                   //是否在查询途中
    //static bool m_bDoingInitQry;                            //正在进行初始化查询，此时不能查费率

    zqWriteLog*           m_pWriteLog;
    std::map<std::string,unsigned int> m_QryCommissionRateTime;
    std::map<std::string,int> m_QryCommissionRateCount;
    std::map<std::string,unsigned int> m_QryMarginRateTime;
    std::map<std::string,int> m_QryMarginRateCount;

};

