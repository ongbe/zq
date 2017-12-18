#pragma once


#include <map>
#include <list>
#include <vector>
using std::map;
using std::string;
using std::vector;

#include "../Module-Misc2/BusinessData.hpp"
#include "../Module-Misc2/packagecache.h"
//#include "../Module-Misc2/platformstruct.h"


class CPackageCache;
class zqWriteLog;


//下面定义每种查询的宏
enum
{
    QryID_ReqBrokerTradingParam=1001,           //查询经纪公司交易参数
    QryID_ReqQryProduct,                        //查询合约信息
    QryID_ReqQryInstrument,                     //查询合约信息
    QryID_ReqQryTradingAccount,                 //查询资金
    QryID_ReqQryTrade,                          //查询成交
    QryID_ReqQryOrder,                          //查询报单
    QryID_ReqQryInvestorPositionDetail,         //查询持仓明细
    QryID_ReqQryInvestorPosition,               //查询持仓汇总
    QryID_ReqQryInstrumentCommissionRate,       //查询合约手续费率
    QryID_ReqQryInstrumentMarginRate,           //请求查询合约保证金率
    QryID_ReqQrySettlementInfo,                 //请求查询投资者结算结果
    QryID_ReqQryDepthMarketData,                //请求查询行情

    QryID_ReqQryInvestorPositionCombineDetail,  //请求查询投资者持仓明细(组合)
    QryID_ReqQryNotice,                         //请求查询客户通知
    QryID_ReqQrySettlementInfoConfirm,          //请求查询结算信息确认
    QryID_ReqQryTransferBank,                   //请求查询转帐银行
    QryID_ReqQryTransferSerial,                 //请求查询转帐流水
    QryID_ReqQryContractBank,                   //请求查询签约银行
    QryID_ReqQryAccountregister,                //请求查询银期签约关系
    QryID_ReqQryParkedOrder,                    //请求查询预埋单
    QryID_ReqQryParkedOrderAction,              //请求查询预埋撤单
    QryID_ReqQueryBankAccountMoneyByFuture,     //期货发起查询银行余额请求
    QryID_ReqQryExchangeRate,                   //请求查询汇率
    QryID_ReqQryExecOrder,                      //请求查询执行宣告
    QryID_ReqQueryMaxOrderVolume,               //请求查询最大报单数量请求

    QryID_ReqQryCFMMCTradingAccountKey,         //请求查询保证金监管系统经纪公司资金账户密钥
};

class CPlatFormService;
class CPlatFormParam;
class CQryQueue
{
public:
    CQryQueue(CPlatFormService& PlatFormService);
    ~CQryQueue(void);
    void StopThread();
    //是否需要线程退出
    bool IsThreadNeedExit(void);                                  

    //是否到达查询时刻。ctp规定每秒只能有一次查询 
    bool IsQryTime(void);
	//
	bool TradeStatusIsConnected(void);
    //更新查询时刻
    void UpdateQryTime(void);

    //发送查询请求，返回值表示发送是否成功
    bool SendQryCmd();

    //检查ReqChache是否为空
    bool IsCacheEmpty(void);
    bool IsCache2Empty(void);

    //请求查询经纪公司交易参数
    void ReqQryBrokerTradingParams(int nRequestID);

    //请求查询品种
    int ReqQryProduct(PlatformStru_QryProduct &QryProduct, int nRequestID);

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

    //请求查询执行宣告
	int ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID);

    //查询最大报单数量请求
    int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &QueryMaxOrderVolume, int nRequestID);

    //清除查询请求队列中指定类型的查询请求，用于TCP重连后的重查前的处理
    //ReqCmdID:1:资金查询；2:成交查询；4:报单查询；8:持仓明细查询；16:持仓查询；32:组合持仓明细查询
    //ReqCmdID可以或处理
    void ClearReqInQryQueue(int ReqCmdID);



private:
    //判断Cache中的请求查询的数据包是否正确
    bool IsReqQryPackageInCacheValid(const PackageHeadStru& PackageHead,const void* pPackageData);



private:

    CPackageCache *m_pReqQueue;                             //优先查询队列。队列按先进先出顺序查询
    CPackageCache *m_pReqQueue2;                            //次级查询队列，放费率查询请求。队列按先进先出顺序查询    
    bool    m_bNeedExit;                                    //线程是否需要退出
    DWORD   m_LastQryTime;                                  //上次查询的时间
    HANDLE  m_hThread;                                      //线程句柄
    
    //线程函数
    static DWORD ThreadProc(void *arg);

    CPlatFormService& m_RefPlatFormService;


    zqWriteLog*             m_pWriteLog;
    map<string,time_t>      m_QryCommissionRateTime;
    map<string,int>         m_QryCommissionRateCount;
    map<string,time_t>      m_QryMarginRateTime;
    map<string,int>         m_QryMarginRateCount;

    bool                    m_bStartQryRate;                //是否已经开始查询费率。在m_rServiceProxy.m_bInitQrySucc为true之后，再开始查询费率
    vector<string>          m_InstrumentIDsNeedQry;         //需要查询费率的合约。查询时按此顺序进行。
    vector<string>          m_InstrumentIDsNeedQry2;        //需要查询费率的合约。优先级较低。如果某合约查询无费率，则同品种其它合约移到此队列，最后再查。
    int                     m_QryRateCount;                 //查询费率的次数。头4次，用InstrumentID为空的方式查询手续费率、保证金率、期权手续费率、期权交易成本，以得到有持仓的费率数据

};

