#ifndef _BUSINESSDATA_HPP_
#define _BUSINESSDATA_HPP_

#include "KernelStruct.h" 
#include "globalDefines.h"
#include "Tools_Ansi.h"
#include <string>

#ifdef WIN32
#pragma managed(push,off)
#endif 





#define PATSYSTEM_FTDC_OPT_Market              'G'
#define PATSYSTEM_FTDC_OPT_Limit               'H'
#define PATSYSTEM_FTDC_OPT_LimitFAK            'I'
#define PATSYSTEM_FTDC_OPT_LimitFOK            'J'
#define PATSYSTEM_FTDC_OPT_Stop                'K'
#define PATSYSTEM_FTDC_OPT_SynthStop           'L'
#define PATSYSTEM_FTDC_OPT_SynthStopLimit      'M'
#define PATSYSTEM_FTDC_OPT_MIT                 'N'
#define PATSYSTEM_FTDC_OPT_SynthMIT            'O'
#define PATSYSTEM_FTDC_OPT_MarketFOK           'P'
#define PATSYSTEM_FTDC_OPT_MOO                 'Q'
#define PATSYSTEM_FTDC_OPT_CrossingBatchType   'R'
#define PATSYSTEM_FTDC_OPT_BasisBatchType      'S'
#define PATSYSTEM_FTDC_OPT_BlockBatchType      'T'
#define PATSYSTEM_FTDC_OPT_AABatchType         'U'
#define PATSYSTEM_FTDC_OPT_IOC                 'V'
#define PATSYSTEM_FTDC_OPT_StopRise            'W'
#define PATSYSTEM_FTDC_OPT_StopFall            'X'
#define PATSYSTEM_FTDC_OPT_RFQ                 'Y'
#define PATSYSTEM_FTDC_OPT_StopLoss            'Z'

enum BusinessID
{
    BID_MIN = 0,
    BID_RspUserLogin = BID_MIN,     //登录请求响应,分为交易登入,行情登入
    BID_RspUserLogout,              //登出请求响应,分为交易登出,行情登出
    BID_RspUserPasswordUpdate,      //用户口令更新请求响应
    BID_RspTradingAccountPasswordUpdate,  //资金账户口令更新请求响应
    BID_RspOrderInsert,            //报单录入请求响应(错误回执)(OnRspOrderInsert 根据“综合交易平台API开发FAQ”:CTP在检查委托发现错误时，会给发出委托的投资者发出OnRspInsertOrder，同时发出OnErrRtnOrder给相关的交易员，所以，作为投资者可以不关心OnErrRtnOrder)
    BID_ErrRtnOrderInsert,			//BID_RspOrderInsert2, //报单录入回执((错误回执2，交易所认为录入无效)(OnErrRtnOrder)
    BID_RspOrderAction,            //报单操作请求响应
    BID_ErrRtnOrderAction,          //报单操作回执(错误回执2，交易所认为撤单无效)
    BID_RtnOrder,                   //报单回报(报单录入与报单操作都会产生该回报)
    BID_RtnTrade,                   //成交回报
    BID_RtnDepthMarketData,         //深度行情
    BID_RspQryDepthMarketData,      //查询深度行情响应
    BID_RspParkedOrderInsert,       //预埋单录入请求响应
    BID_RspParkedOrderAction,       //预埋撤单录入请求响应
    BID_RspRemoveParkedOrder,       //删除预埋单响应    
    BID_RspRemoveParkedOrderAction, //删除预埋撤单响应
    BID_RspQryParkedOrder,          //请求查询预埋单响应
    BID_RspQryParkedOrderAction,    //请求查询预埋撤单响应
    BID_RspQueryMaxOrderVolume,     //查询最大报单数量响应
    BID_RspSettlementInfoConfirm,   //投资者结算结果确认响应
    BID_RspQryOrder,                //请求查询报单响应
    BID_RspQryTrade,                //请求查询成交响应
    BID_RspQryInvestorPosition,     //请求查询投资者持仓响应
    BID_RspQryTradingAccount,       //请求查询资金账户响应
    BID_RspQryInvestor,             //请求查询投资者响应
    BID_RspQryTradingCode,          //请求查询交易编码响应
    BID_RspQryInstrumentMarginRate, //请求查询合约保证金率响应   
    BID_RspQryInstrumentCommissionRate,//请求查询合约手续费率响应
    BID_RspQryOptionInstrCommRate,  //请求查询期权合约手续费响应
    BID_RspQryOptionInstrTradeCost, //请求查询期权交易成本响应
    BID_RspQryExchange,             //请求查询交易所响应
    BID_RspQryProduct,              //请求查询产品响应
    BID_RspQryInstrument,           //请求查询合约响应
    BID_RspQrySettlementInfo,       //请求查询投资者结算结果响应
    BID_RspQryTransferBank,         //请求查询转帐银行响应
    BID_RspQryInvestorPositionDetail,//请求查询投资者持仓明细响应
    BID_RspQryNotice,               //请求查询客户通知响应
    BID_RspQrySettlementInfoConfirm,//请求查询结算信息确认响应
    BID_RspQryInvestorPositionCombDetail, //请求查询投资者持仓明细响应
    BID_RtnInstrumentStatus,                 //合约交易状态通知
    BID_RtnTradingNotice,                    //交易通知
    BID_RspQryContractBank,                  //请求查询签约银行响应
    BID_RspQryTradingNotice,                 //请求查询交易通知响应
    BID_RspQryBrokerTradingParams,           //请求查询经纪公司交易参数响应
    BID_RspQryBrokerTradingAlgos,            //请求查询经纪公司交易算法响应

    BID_RspFromBankToFutureByFuture,        //期货发起银行资金转期货应答
    BID_RspFromFutureToBankByFuture,        //期货发起期货资金转银行应答
    BID_RspQueryBankAccountMoneyByFuture,   //期货发起查询银行余额应答
    BID_RspQryTransferSerial,               //请求查询转帐流水响应
    BID_RspQryAccountregister,              //请求查询银期签约关系响应

    BID_RtnFromBankToFutureByFuture,        //期货发起银行资金转期货回报
    BID_RtnFromFutureToBankByFuture,        //期货发起期货资金转银行回报
    BID_RtnQueryBankAccountMoneyByFuture,   //期货发起查询银行余额回报

    BID_RspQryExchangeRate,                 //请求查询汇率返回

    BID_RspForQuoteInsert,                  //询价录入请求响应
    BID_RspExecOrderInsert,                 //执行宣告录入请求响应
    BID_RspExecOrderAction,                 //执行宣告操作请求响应
    BID_RspQryExecOrder,                    //请求查询执行宣告响应
    BID_ErrRtnForQuoteInsert,               //询价录入错误回报
    BID_ErrRtnExecOrderInsert,              //执行宣告录入错误回报
    BID_ErrRtnExecOrderAction,              //执行宣告操作错误回报
    BID_RtnExecOrder,                       //执行宣告通知
    BID_RspSubForQuoteRsp,                  //订阅询价应答
    BID_RspUnSubForQuoteRsp,                //退订询价应答
    BID_RtnForQuoteRsp,                     //询价通知

    BID_FrontDisconnected,                  //与交易后台通信连接断开
    BID_FrontConnected,					    //与交易后台通信连接成功

    BID_QryStart,                           //查询开始，参数为string，表示查询内容
    BID_ConnectivityStatus,
    BID_AlertMessage,			            //Pats Alert Message
    BID_ExchangeRate,		
    BID_MAX
};

struct AbstractBusinessData
{
    AbstractBusinessData()
    :   BID(BID_MIN),
        PlatformID(0),
        Size(0)
    {
    };

    AbstractBusinessData(BusinessID bID,unsigned int size)
    :   BID(bID),
        PlatformID(0),
        Size(size)
    {
    };

    BusinessID                 BID;
	int						   PlatformID;
    unsigned int               Size;//肯定大于等于sizeof(AbstractBusinessData)
};

typedef int (*GuiCallBackFunc)(const GUIModuleID,const AbstractBusinessData&);

//登入请求响应
struct DataRspUserLogin
{
    DataRspUserLogin(bool btrader,int requestID,bool bisLast,unsigned long curTickCount,const CThostFtdcRspUserLoginField& userLoginField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspUserLogin,(unsigned int)sizeof(*this)),
        bTrader(btrader),
        nRequestID(requestID),
        bIsLast(bisLast),
        CurTickCount(curTickCount),
        UserLoginField(userLoginField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    bool                      bTrader;//是否交易
    int                       nRequestID;
    bool                      bIsLast;
    unsigned long             CurTickCount;//本地电脑开机后的毫秒数据
    CThostFtdcRspUserLoginField UserLoginField;
    CThostFtdcRspInfoField    RspInfoField; 
};

//登出请求响应
struct DataRspUserLogout
{
    DataRspUserLogout(int type,int requestID,bool bisLast,const CThostFtdcUserLogoutField& userLogoutField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspUserLogout,(unsigned int)sizeof(*this)),
        Type(type),
        nRequestID(requestID),
        bIsLast(bisLast),
        UserLogoutField(userLogoutField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       Type;//0:交易, 1:行情
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcUserLogoutField UserLogoutField;
    CThostFtdcRspInfoField    RspInfoField; 
};


//用户口令更新请求响应
struct DataRspUserPasswordUpdate
{
    DataRspUserPasswordUpdate()
    :   Head(BID_RspUserPasswordUpdate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&UserPasswordUpdateField,0,sizeof(UserPasswordUpdateField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspUserPasswordUpdate(int requestID,bool bisLast,const CThostFtdcUserPasswordUpdateField& userPasswordUpdateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspUserPasswordUpdate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        UserPasswordUpdateField(userPasswordUpdateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcUserPasswordUpdateField UserPasswordUpdateField;
    CThostFtdcRspInfoField    RspInfoField; 
};

//资金账户口令更新请求响应
struct DataRspTradingAccountPasswordUpdate
{
    DataRspTradingAccountPasswordUpdate()
    :   Head(BID_RspTradingAccountPasswordUpdate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TradingAccountPasswordUpdateField,0,sizeof(TradingAccountPasswordUpdateField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspTradingAccountPasswordUpdate(int requestID,bool bisLast,const CThostFtdcTradingAccountPasswordUpdateField& tradingAccountPasswordUpdateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspTradingAccountPasswordUpdate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TradingAccountPasswordUpdateField(tradingAccountPasswordUpdateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcTradingAccountPasswordUpdateField TradingAccountPasswordUpdateField;
    CThostFtdcRspInfoField    RspInfoField; 
};

//投资者结算结果确认响应
struct DataRspSettlementInfoConfirm
{
    DataRspSettlementInfoConfirm()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspSettlementInfoConfirm;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcSettlementInfoConfirmField      SettlementInfoConfirmField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//预埋单录入回执(分为成功回执与错误回执)
struct DataRspParkedOrderInsert
{
    DataRspParkedOrderInsert()
    :   Head(BID_RspParkedOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&ParkedOrder,0,sizeof(ParkedOrder));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspParkedOrderInsert(int requestID,bool bisLast,const CThostFtdcParkedOrderField& parkedOrder,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspParkedOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ParkedOrder(parkedOrder),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcParkedOrderField ParkedOrder;
    CThostFtdcRspInfoField    RspInfoField;    
};

//预埋撤单录入请求响应
struct DataRspParkedOrderAction
{
    DataRspParkedOrderAction()
    :   Head(BID_RspParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&ParkedOrderAction,0,sizeof(ParkedOrderAction));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspParkedOrderAction(int requestID,bool bisLast,const CThostFtdcParkedOrderActionField& parkedOrderAction,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ParkedOrderAction(parkedOrderAction),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcParkedOrderActionField ParkedOrderAction;
    CThostFtdcRspInfoField     RspInfoField;    
};


//删除预埋单响应
struct DataRspRemoveParkedOrder
{
    DataRspRemoveParkedOrder()
    :   Head(BID_RspRemoveParkedOrder,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RemoveParkedOrder,0,sizeof(RemoveParkedOrder));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspRemoveParkedOrder(int requestID,bool bisLast,const CThostFtdcRemoveParkedOrderField& removeParkedOrder,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspRemoveParkedOrder,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        RemoveParkedOrder(removeParkedOrder),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcRemoveParkedOrderField RemoveParkedOrder;
    CThostFtdcRspInfoField     RspInfoField;    
};

//删除预埋撤单响应
struct DataRspRemoveParkedOrderAction
{
    DataRspRemoveParkedOrderAction()
    :   Head(BID_RspRemoveParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RemoveParkedOrderAction,0,sizeof(RemoveParkedOrderAction));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspRemoveParkedOrderAction(int requestID,bool bisLast,const CThostFtdcRemoveParkedOrderActionField& removeParkedOrderAction,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspRemoveParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        RemoveParkedOrderAction(removeParkedOrderAction),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcRemoveParkedOrderActionField RemoveParkedOrderAction;
    CThostFtdcRspInfoField     RspInfoField;    
};

//请求查询预埋单响应
struct DataRspQryParkedOrder
{
    DataRspQryParkedOrder()
    :   Head(BID_RspQryParkedOrder,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&ParkedOrder,0,sizeof(ParkedOrder));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryParkedOrder(int requestID,bool bisLast,const CThostFtdcParkedOrderField& parkedOrder,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryParkedOrder,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ParkedOrder(parkedOrder),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcParkedOrderField      ParkedOrder;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询预埋撤单响应
struct DataRspQryParkedOrderAction
{
    DataRspQryParkedOrderAction()
    :   Head(BID_RspQryParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&ParkedOrderAction,0,sizeof(ParkedOrderAction));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryParkedOrderAction(int requestID,bool bisLast,const CThostFtdcParkedOrderActionField& parkedOrderAction,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryParkedOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ParkedOrderAction(parkedOrderAction),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcParkedOrderActionField      ParkedOrderAction;
    CThostFtdcRspInfoField    RspInfoField;    
};


//报单录入回执(分为成功回执与错误回执)
struct DataRspOrderInsert
{
    DataRspOrderInsert()
    :   Head(BID_RspOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InputOrderField,0,sizeof(InputOrderField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspOrderInsert(int requestID,bool bisLast,const CThostFtdcInputOrderField& OrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputOrderField(OrderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInputOrderField InputOrderField;
    CThostFtdcRspInfoField    RspInfoField;    
};
struct DataErrRtnOrderInsert
{
	DataErrRtnOrderInsert()
    :   Head(BID_ErrRtnOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
	{
        memset(&InputOrderField,0,sizeof(InputOrderField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
	};
	DataErrRtnOrderInsert(int requestID,bool bisLast,const CThostFtdcInputOrderField& OrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputOrderField(OrderField),
        RspInfoField(RspInfo)
	{
	};

	AbstractBusinessData      Head;
	int                       nRequestID;
	bool                      bIsLast;
	CThostFtdcInputOrderField InputOrderField;
	CThostFtdcRspInfoField    RspInfoField;    
};


//报单操作回执(错误回执1，综合交易平台认为撤单无效)
struct DataRspOrderAction
{
    DataRspOrderAction()
    :   Head(BID_RspOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InputOrderActionField,0,sizeof(InputOrderActionField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspOrderAction(int requestID,bool bisLast,const CThostFtdcInputOrderActionField& OrderActionField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputOrderActionField(OrderActionField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInputOrderActionField InputOrderActionField;
    CThostFtdcRspInfoField     RspInfoField;    
};

//报单操作回执(错误回执2，交易所认为撤单无效)
struct DataErrRtnOrderAction
{
    DataErrRtnOrderAction()
    :   Head(BID_ErrRtnOrderAction,(unsigned int)sizeof(*this))
    {
        memset(&OrderActionField,0,sizeof(OrderActionField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataErrRtnOrderAction(const CThostFtdcOrderActionField& orderActionField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnOrderAction,(unsigned int)sizeof(*this)),
        OrderActionField(orderActionField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    CThostFtdcOrderActionField OrderActionField;
    CThostFtdcRspInfoField     RspInfoField;    
};

//报单回报(下单无误，会产生此消息，撤单无误，也会产生此消息)
struct DataRtnOrder
{
    DataRtnOrder()
    :   Head(BID_RtnOrder,(unsigned int)sizeof(*this))
    {
        memset(&OrderField,0,sizeof(OrderField));
    };
    DataRtnOrder(const PlatformStru_OrderInfo& orderField)
    :   Head(BID_RtnOrder,(unsigned int)sizeof(*this)),
        OrderField(orderField)
    {
    };

    AbstractBusinessData      Head;
    PlatformStru_OrderInfo      OrderField; 
};

//成交回报
struct DataRtnTrade
{
    DataRtnTrade()
    :   Head(BID_RtnTrade,(unsigned int)sizeof(*this))
    {
        memset(&TradeField,0,sizeof(TradeField));
    };
    DataRtnTrade(const PlatformStru_TradeInfo& tradeField)
    :   Head(BID_RtnTrade,(unsigned int)sizeof(*this)),
        TradeField(tradeField)
    {
    };

    AbstractBusinessData      Head;
    PlatformStru_TradeInfo      TradeField; 
};

//深度行情通知
struct DataRtnDepthMarketData
{
    DataRtnDepthMarketData()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnDepthMarketData;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData                      Head;
    PlatformStru_DepthMarketData    DepthMarketDataField; 
    bool                            bHavePosition;  //表示是否有有效持仓
};

//请求查询投资者持仓响应
struct DataRspQryInvestorPosition
{
    DataRspQryInvestorPosition()
    :   Head(BID_RspQryInvestorPosition,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InvestorPositionField,0,sizeof(InvestorPositionField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
		memset(QryInstrumentID,0,sizeof(QryInstrumentID));
    };
    DataRspQryInvestorPosition(int requestID,bool bisLast,const PlatformStru_Position& investorPositionField,const CThostFtdcRspInfoField& RspInfo,const TThostFtdcInstrumentIDType& qryInstrumentID)
    :   Head(BID_RspQryInvestorPosition,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InvestorPositionField(investorPositionField),
        RspInfoField(RspInfo)
    {
		memcpy(QryInstrumentID,qryInstrumentID,sizeof(QryInstrumentID));
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_Position       InvestorPositionField;
    CThostFtdcRspInfoField      RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //查询的合约ID
};


//请求查询投资者持仓明细响应
struct DataRspQryInvestorPositionDetail
{
    DataRspQryInvestorPositionDetail()
    :   Head(BID_RspQryInvestorPositionDetail,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InvestorPositionDetailField,0,sizeof(InvestorPositionDetailField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
		memset(QryInstrumentID,0,sizeof(QryInstrumentID));
    };
    DataRspQryInvestorPositionDetail(int requestID,bool bisLast,const PlatformStru_PositionDetail& investorPositionDetailField,const CThostFtdcRspInfoField& RspInfo,const TThostFtdcInstrumentIDType& qryInstrumentID)
    :   Head(BID_RspQryInvestorPositionDetail,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InvestorPositionDetailField(investorPositionDetailField),
        RspInfoField(RspInfo)
    {
		memcpy(QryInstrumentID,qryInstrumentID,sizeof(QryInstrumentID));
    };

    AbstractBusinessData		Head;
    int							nRequestID;
    bool						bIsLast;
    PlatformStru_PositionDetail	InvestorPositionDetailField;
    CThostFtdcRspInfoField		RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //查询的合约ID
};

//请求查询投资者持仓组合合约明细响应
struct DataRspQryInvestorPositionCombDetail
{
    DataRspQryInvestorPositionCombDetail()
    :   Head(BID_RspQryInvestorPositionCombDetail,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InvestorPositionCombineDetailField,0,sizeof(InvestorPositionCombineDetailField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
		memset(QryInstrumentID,0,sizeof(QryInstrumentID));
    };
    DataRspQryInvestorPositionCombDetail(int requestID,bool bisLast,const PlatformStru_PositionDetailComb& investorPositionCombineDetailField,const CThostFtdcRspInfoField& RspInfo,const TThostFtdcInstrumentIDType& qryInstrumentID)
    :   Head(BID_RspQryInvestorPositionCombDetail,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InvestorPositionCombineDetailField(investorPositionCombineDetailField),
        RspInfoField(RspInfo)
    {
		memcpy(QryInstrumentID,qryInstrumentID,sizeof(QryInstrumentID));
    };

    AbstractBusinessData		Head;
    int							nRequestID;
    bool						bIsLast;
    PlatformStru_PositionDetailComb	InvestorPositionCombineDetailField;
    CThostFtdcRspInfoField		RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //查询的合约ID
};

//请求查询资金账户响应
struct DataRspQryTradingAccount
{
    DataRspQryTradingAccount()
    :   Head(BID_RspQryTradingAccount,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TradingAccountField,0,sizeof(TradingAccountField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryTradingAccount(int requestID,bool bisLast,const PlatformStru_TradingAccountInfo& tradingAccountField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryTradingAccount,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TradingAccountField(tradingAccountField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_TradingAccountInfo TradingAccountField;
    CThostFtdcRspInfoField     RspInfoField;    
};

struct DataRspQryTradingAccountEx
{
	DataRspQryTradingAccountEx()
	{
		memset(this, 0, sizeof(*this));
		Head.BID = BID_RspQryTradingAccount;
		Head.Size = sizeof(*this);
	};
	AbstractBusinessData      Head;
	int                       nRequestID;
	bool                      bIsLast;
	CThostFtdcTradingAccountFieldEx TradingAccountField;
	CThostFtdcRspInfoField     RspInfoField;    
};

//请求查询投资者响应
struct DataRspQryInvestor
{
    DataRspQryInvestor()
    :   Head(BID_RspQryInvestor,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InvestorField,0,sizeof(InvestorField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryInvestor(int requestID,bool bisLast,const CThostFtdcInvestorField& investorField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryInvestor,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InvestorField(investorField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInvestorField   InvestorField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询交易编码响应
struct DataRspQryTradingCode
{
    DataRspQryTradingCode()
    :   Head(BID_RspQryTradingCode,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TradingCodeField,0,sizeof(TradingCodeField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryTradingCode(int requestID,bool bisLast,const CThostFtdcTradingCodeField& tradingCodeField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryTradingCode,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TradingCodeField(tradingCodeField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcTradingCodeField   TradingCodeField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询合约保证金率响应
struct DataRspQryInstrumentMarginRate
{
    DataRspQryInstrumentMarginRate()
    :   Head(BID_RspQryInstrumentMarginRate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InstrumentMarginRateField,0,sizeof(InstrumentMarginRateField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryInstrumentMarginRate(int requestID,bool bisLast,const PlatformStru_InstrumentMarginRate& instrumentMarginRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryInstrumentMarginRate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentMarginRateField(instrumentMarginRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentMarginRate   InstrumentMarginRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询合约手续费率响应
struct DataRspQryInstrumentCommissionRate
{
    DataRspQryInstrumentCommissionRate()
    :   Head(BID_RspQryInstrumentCommissionRate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InstrumentCommissionRateField,0,sizeof(InstrumentCommissionRateField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryInstrumentCommissionRate(int requestID,bool bisLast,const PlatformStru_InstrumentCommissionRate& instrumentCommissionRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryInstrumentCommissionRate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentCommissionRateField(instrumentCommissionRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentCommissionRate   InstrumentCommissionRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询期权合约手续费率响应
struct DataRspQryOptionInstrCommRate
{
    DataRspQryOptionInstrCommRate()
    :   Head(BID_RspQryOptionInstrCommRate,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryOptionInstrCommRate(int requestID,bool bisLast,const PlatformStru_InstrumentCommissionRate& instrumentCommissionRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryOptionInstrCommRate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentCommissionRateField(instrumentCommissionRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentCommissionRate   InstrumentCommissionRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};
//请求查询期权交易成本响应
struct DataRspQryOptionInstrTradeCost
{
    DataRspQryOptionInstrTradeCost()
    :   Head(BID_RspQryOptionInstrTradeCost,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryOptionInstrTradeCost(int requestID,bool bisLast,const PlatformStru_InstrumentMarginRate& instrumentMarginRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryOptionInstrTradeCost,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentMarginRateField(instrumentMarginRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentMarginRate   InstrumentMarginRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询交易所响应
struct DataRspQryExchange
{
    DataRspQryExchange(int requestID,bool bisLast,const CThostFtdcExchangeField& exchangeField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryExchange,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ExchangeField(exchangeField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcExchangeField   ExchangeField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询经纪公司交易参数响应
struct DataRspQryBrokerTradingParams
{
    DataRspQryBrokerTradingParams(int requestID,bool bisLast,const PlatformStru_BrokerTradingParams& brokerTradingParams,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryBrokerTradingParams,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        BrokerTradingParams(brokerTradingParams),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData                Head;
    int                                 nRequestID;
    bool                                bIsLast;
    PlatformStru_BrokerTradingParams    BrokerTradingParams;
    CThostFtdcRspInfoField              RspInfoField;    
};

//请求查询产品响应
struct DataRspQryProduct
{
    DataRspQryProduct(int requestID,bool bisLast,const PlatformStru_ProductInfo& productField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryProduct,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ProductField(productField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_ProductInfo    ProductField;
    CThostFtdcRspInfoField      RspInfoField;    
};

//请求查询合约响应
struct DataRspQryInstrument
{
    DataRspQryInstrument(int requestID,bool bisLast,const PlatformStru_InstrumentInfo& instrumentField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryInstrument,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentField(instrumentField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentInfo InstrumentField;
    CThostFtdcRspInfoField     RspInfoField;    
};
//请求查询行情响应
struct DataRspQryDepthMarketData
{
    DataRspQryDepthMarketData()
    :   Head(BID_RspQryDepthMarketData,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&DepthMarketDataField,0,sizeof(DepthMarketDataField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryDepthMarketData(int requestID,bool bisLast,const PlatformStru_DepthMarketData& depthMarketDataField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryDepthMarketData,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        DepthMarketDataField(depthMarketDataField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_DepthMarketData DepthMarketDataField;
    CThostFtdcRspInfoField     RspInfoField;    
};


//请求查询报单响应
struct DataRspQryOrder
{
    DataRspQryOrder(int requestID,bool bisLast,const PlatformStru_OrderInfo& orderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryOrder,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        OrderField(orderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_OrderInfo    OrderField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询成交响应
struct DataRspQryTrade
{
    DataRspQryTrade(int requestID,bool bisLast,const PlatformStru_TradeInfo& tradeField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryTrade,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TradeField(tradeField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_TradeInfo    TradeField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询投资者结算结果响应
struct DataRspQrySettlementInfo
{
    DataRspQrySettlementInfo()
    :   Head(BID_RspQrySettlementInfo,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&SettlementInfoField,0,sizeof(SettlementInfoField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQrySettlementInfo(int requestID,bool bisLast,const CThostFtdcSettlementInfoField& settlementInfoField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQrySettlementInfo,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        SettlementInfoField(settlementInfoField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcSettlementInfoField      SettlementInfoField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//期货发起银行资金转期货应答
struct DataRspFromBankToFutureByFuture
{
    DataRspFromBankToFutureByFuture()
    :   Head(BID_RspFromBankToFutureByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TransferField,0,sizeof(TransferField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspFromBankToFutureByFuture(int requestID,bool bisLast,const CThostFtdcReqTransferField& transferField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspFromBankToFutureByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TransferField(transferField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcReqTransferField      TransferField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//期货发起期货资金转银行应答
struct DataRspFromFutureToBankByFuture
{
    DataRspFromFutureToBankByFuture()
    :   Head(BID_RspFromFutureToBankByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TransferField,0,sizeof(TransferField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspFromFutureToBankByFuture(int requestID,bool bisLast,const CThostFtdcReqTransferField& transferField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspFromFutureToBankByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TransferField(transferField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcReqTransferField      TransferField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//期货发起查询银行余额应答
struct DataRspQueryBankAccountMoneyByFuture
{
    DataRspQueryBankAccountMoneyByFuture()
    :   Head(BID_RspQueryBankAccountMoneyByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&QueryAccountField,0,sizeof(QueryAccountField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQueryBankAccountMoneyByFuture(int requestID,bool bisLast,const CThostFtdcReqQueryAccountField& queryAccountField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQueryBankAccountMoneyByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        QueryAccountField(queryAccountField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcReqQueryAccountField      QueryAccountField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询转帐流水响应
struct DataRspQryTransferSerial
{
    DataRspQryTransferSerial()
    :   Head(BID_RspQryTransferSerial,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&TransferSerialField,0,sizeof(TransferSerialField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryTransferSerial(int requestID,bool bisLast,const CThostFtdcTransferSerialField& transferSerialField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryTransferSerial,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        TransferSerialField(transferSerialField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcTransferSerialField      TransferSerialField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询银期签约关系响应
struct DataRspQryAccountregister
{
    DataRspQryAccountregister(int requestID,bool bisLast,const CThostFtdcAccountregisterField& accountregister,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryAccountregister,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        Accountregister(accountregister),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcAccountregisterField      Accountregister;
    CThostFtdcRspInfoField    RspInfoField;    
};

//期货发起银行资金转期货回报
struct DataRtnFromBankToFutureByFuture
{
    DataRtnFromBankToFutureByFuture()
    :   Head(BID_RtnFromBankToFutureByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RspTransferField,0,sizeof(RspTransferField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRtnFromBankToFutureByFuture(int requestID,bool bisLast,const CThostFtdcRspTransferField& rspTransferField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RtnFromBankToFutureByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        RspTransferField(rspTransferField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcRspTransferField      RspTransferField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//期货发起期货资金转银行回报
struct DataRtnFromFutureToBankByFuture
{
    DataRtnFromFutureToBankByFuture()
    :   Head(BID_RtnFromFutureToBankByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&RspTransferField,0,sizeof(RspTransferField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRtnFromFutureToBankByFuture(int requestID,bool bisLast,const CThostFtdcRspTransferField& rspTransferField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RtnFromFutureToBankByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        RspTransferField(rspTransferField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcRspTransferField      RspTransferField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//期货发起查询银行余额回报
struct DataRtnQueryBankAccountMoneyByFuture
{
    DataRtnQueryBankAccountMoneyByFuture()
    :   Head(BID_RtnQueryBankAccountMoneyByFuture,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&NotifyQueryAccountField,0,sizeof(NotifyQueryAccountField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRtnQueryBankAccountMoneyByFuture(int requestID,bool bisLast,const CThostFtdcNotifyQueryAccountField& notifyQueryAccountField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RtnQueryBankAccountMoneyByFuture,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        NotifyQueryAccountField(notifyQueryAccountField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcNotifyQueryAccountField      NotifyQueryAccountField;
    CThostFtdcRspInfoField    RspInfoField;    
};
//请求查询签约银行响应
struct DataRspQryContractBank
{
    DataRspQryContractBank(int requestID,bool bisLast,const CThostFtdcContractBankField& contractBankField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryContractBank,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ContractBankField(contractBankField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcContractBankField      ContractBankField;
    CThostFtdcRspInfoField    RspInfoField;    
};
//请求查询汇率响应
struct DataRspQryExchangeRate
{
    DataRspQryExchangeRate(int requestID,bool bisLast,const CThostFtdcExchangeRateField& exchangeRateField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryExchangeRate,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ExchangeRateField(exchangeRateField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    CThostFtdcExchangeRateField ExchangeRateField;
    CThostFtdcRspInfoField      RspInfoField;    
};

//询价录入请求响应
struct DataRspForQuoteInsert
{
	DataRspForQuoteInsert(void) {
		memset(this, 0, sizeof(*this));
	};
    DataRspForQuoteInsert(int requestID,bool bisLast,const PlatformStru_InputForQuoteField& inputForQuoteField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspForQuoteInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputForQuoteField(inputForQuoteField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_InputForQuoteField InputForQuoteField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//执行宣告录入请求响应
struct DataRspExecOrderInsert
{
	DataRspExecOrderInsert(void)
	:   Head(BID_RspExecOrderInsert,(unsigned int)sizeof(*this)) {
		nRequestID = 0;
		bIsLast = false;
		memset(&InputExecOrderField, 0, sizeof(InputExecOrderField));
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspExecOrderInsert(int requestID,bool bisLast,const PlatformStru_InputExecOrderField& inputExecOrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspExecOrderInsert,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputExecOrderField(inputExecOrderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_InputExecOrderField InputExecOrderField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//执行宣告操作请求响应
struct DataRspExecOrderAction
{
	DataRspExecOrderAction(void) 
    :   Head(BID_RspExecOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(false)
	{
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspExecOrderAction(int requestID,bool bisLast,const PlatformStru_InputExecOrderActionField& inputExecOrderActionField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspExecOrderAction,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InputExecOrderActionField(inputExecOrderActionField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_InputExecOrderActionField InputExecOrderActionField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//请求查询执行宣告响应
struct DataRspQryExecOrder
{
	DataRspQryExecOrder(void) 
	:   Head(BID_RspQryExecOrder,(unsigned int)sizeof(*this))
	{
        nRequestID = 0;
        bIsLast = false;
        memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspQryExecOrder(int requestID,bool bisLast,const PlatformStru_ExecOrderField& execOrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryExecOrder,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        ExecOrderField(execOrderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    int                         nRequestID;
    bool                        bIsLast;
    PlatformStru_ExecOrderField ExecOrderField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//询价录入错误回报
struct DataErrRtnForQuoteInsert
{
	DataErrRtnForQuoteInsert(void) {
		memset(this, 0, sizeof(*this));
	};
    DataErrRtnForQuoteInsert(const PlatformStru_InputForQuoteField& inputForQuoteField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnForQuoteInsert,(unsigned int)sizeof(*this)),
        InputForQuoteField(inputForQuoteField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_InputForQuoteField InputForQuoteField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//执行宣告录入错误回报
struct DataErrRtnExecOrderInsert
{
	DataErrRtnExecOrderInsert(void) {
		memset(this, 0, sizeof(*this));
	};
	DataErrRtnExecOrderInsert(const PlatformStru_InputExecOrderField& inputExecOrderField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnExecOrderInsert,(unsigned int)sizeof(*this)),
        InputExecOrderField(inputExecOrderField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_InputExecOrderField InputExecOrderField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//执行宣告操作错误回报
struct DataErrRtnExecOrderAction
{
	DataErrRtnExecOrderAction(void)
    :   Head(BID_ErrRtnExecOrderAction,(unsigned int)sizeof(*this))
	{
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataErrRtnExecOrderAction(const PlatformStru_ExecOrderActionField& execOrderActionField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_ErrRtnExecOrderAction,(unsigned int)sizeof(*this)),
        ExecOrderActionField(execOrderActionField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_ExecOrderActionField ExecOrderActionField;
    CThostFtdcRspInfoField      RspInfoField;    
};
//执行宣告通知
struct DataRtnExecOrder
{
    DataRtnExecOrder()
    :   Head(BID_RtnExecOrder,(unsigned int)sizeof(*this))
    {
    };
    DataRtnExecOrder(const PlatformStru_ExecOrderField& execOrderField)
    :   Head(BID_RtnExecOrder,(unsigned int)sizeof(*this)),
        ExecOrderField(execOrderField)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_ExecOrderField ExecOrderField;
};
//订阅询价应答
struct DataRspSubForQuoteRsp
{
	DataRspSubForQuoteRsp(void) 
	:	Head(BID_RspSubForQuoteRsp,(unsigned int)sizeof(*this)) {
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspSubForQuoteRsp(const char* instrumentID,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspSubForQuoteRsp,(unsigned int)sizeof(*this)),
        RspInfoField(RspInfo)
    {
        memset(InstrumentID,0,sizeof(InstrumentID));
        strncpy(InstrumentID,instrumentID,sizeof(InstrumentID)-1);
    };

    AbstractBusinessData        Head;
    CommonInstrumentIDType      InstrumentID;
    CThostFtdcRspInfoField      RspInfoField;    
};
//退订询价应答
struct DataRspUnSubForQuoteRsp
{
	DataRspUnSubForQuoteRsp(void) 
	:   Head(BID_RspUnSubForQuoteRsp,(unsigned int)sizeof(*this)) {
		memset(&RspInfoField, 0, sizeof(RspInfoField));
	};
    DataRspUnSubForQuoteRsp(const char* instrumentID,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspUnSubForQuoteRsp,(unsigned int)sizeof(*this)),
        RspInfoField(RspInfo)
    {
        strncpy(InstrumentID,instrumentID,sizeof(InstrumentID)-1);
    };

    AbstractBusinessData        Head;
    CommonInstrumentIDType      InstrumentID;
    CThostFtdcRspInfoField      RspInfoField;    
};
//询价通知
struct DataRtnForQuoteRsp
{
    DataRtnForQuoteRsp()
    :   Head(BID_RtnForQuoteRsp,(unsigned int)sizeof(*this))
    {
    };
    DataRtnForQuoteRsp(const PlatformStru_ForQuoteRspField& forQuoteRsp)
    :   Head(BID_RtnForQuoteRsp,(unsigned int)sizeof(*this)),
        ForQuoteRsp(forQuoteRsp)
    {
    };

    AbstractBusinessData        Head;
    PlatformStru_ForQuoteRspField ForQuoteRsp;
};


///合约交易状态
struct DataInstrumentStatus
{
    DataInstrumentStatus()
    :   Head(BID_RtnInstrumentStatus,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&InstrumentStatusField,0,sizeof(InstrumentStatusField));
    };
    DataInstrumentStatus(int requestID,bool bisLast,const CThostFtdcInstrumentStatusField& instrumentStatusField)
    :   Head(BID_RtnInstrumentStatus,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        InstrumentStatusField(instrumentStatusField)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInstrumentStatusField      InstrumentStatusField; 
};
//与交易后台通信连接成功
struct DataFrontConnected
{
    DataFrontConnected(bool btrader)
    :   Head(BID_FrontConnected,(unsigned int)sizeof(*this)),
        bTrader(btrader)
    {
    };

    AbstractBusinessData      Head;
	bool                      bTrader;
};
//与交易后台通信连接断开
struct DataFrontDisconnected
{
    DataFrontDisconnected(bool btrader,int nreason)
    :   Head(BID_FrontDisconnected,(unsigned int)sizeof(*this)),
        bTrader(btrader),
        nReason(nreason)
    {
    };

    AbstractBusinessData      Head;
	bool                      bTrader;
    int                       nReason; 
};
struct DataRspQrySettlementInfoConfirm
{
	DataRspQrySettlementInfoConfirm(int requestID,bool bisLast,const CThostFtdcSettlementInfoConfirmField& settlementInfoConfirm,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQrySettlementInfoConfirm,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        SettlementInfoConfirm(settlementInfoConfirm),
        RspInfoField(RspInfo)
	{
	};

    AbstractBusinessData      Head;
	int                       nRequestID;
	bool                      bIsLast;
	CThostFtdcSettlementInfoConfirmField      SettlementInfoConfirm;
	CThostFtdcRspInfoField    RspInfoField;    
};
struct DataTradingNoticeInfoField
{
    DataTradingNoticeInfoField(const CThostFtdcTradingNoticeInfoField& tradingNoticeInfoField)
    :   Head(BID_RtnTradingNotice,(unsigned int)sizeof(*this)),
        TradingNoticeInfoField(tradingNoticeInfoField)
    {
    };

    AbstractBusinessData      Head;
    CThostFtdcTradingNoticeInfoField      TradingNoticeInfoField; 
};

struct DataRspQryTradingNotice
{
    DataRspQryTradingNotice()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryTradingNotice;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    CThostFtdcTradingNoticeField      TradingNoticeInfoField; 
};
struct DataConnectivityStatus
{
    DataConnectivityStatus()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_ConnectivityStatus;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    ConnectivityStatus      StatusField; 
};
struct DataMessageStruct
{
    DataMessageStruct()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_AlertMessage;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    PlatformStru_MessageStruct  MessageStructField; 
};
struct DataExchangeRate
{
    DataExchangeRate()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_ExchangeRate;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    char			RateField[128]; 
};
///查询用户通知响应
struct DataRspQryNotice
{
    DataRspQryNotice()
    :   Head(BID_RspQryNotice,(unsigned int)sizeof(*this)),
        nRequestID(0),
        bIsLast(true)
    {
        memset(&NoticeField,0,sizeof(NoticeField));
        memset(&RspInfoField,0,sizeof(RspInfoField));
    };
    DataRspQryNotice(int requestID,bool bisLast,const CThostFtdcNoticeField& noticeField,const CThostFtdcRspInfoField& RspInfo)
    :   Head(BID_RspQryNotice,(unsigned int)sizeof(*this)),
        nRequestID(requestID),
        bIsLast(bisLast),
        NoticeField(noticeField),
        RspInfoField(RspInfo)
    {
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcNoticeField     NoticeField; 
    CThostFtdcRspInfoField    RspInfoField;   
};
// 成交统计结构
typedef struct
{
	std::string Instrument;
	std::string EcChangeID;
	double TradeCommission;
	std::string BuySell;
	std::string OpenClose;
	double price;
	int volume;
} TotalInfo;
typedef enum  _tagSubscribeMarketDataOper
{
	eSubscribeMarketData,
	eUnsubscribeMarketData,
}SubscribeMarketDataOper;

//表示是哪个模块订阅的行情
typedef enum  _tagSPanel
{
	S_STATUS_TEMP=(1<<2),
	S_STATUS_QUOT=(1<<3),
	S_STATUS_ORDER=(1<<4),
	S_STATUS_ORDERINSERT=(1<<5),
	S_STATUS_POS=(1<<6),
	S_STATUS_PLUS=(1<<7),
    S_STATUS_PlatformSvr=(1<<8),
    S_STATUS_MovePosition=(1<<9)
}S_Panel;


#ifdef WIN32
#pragma managed(pop)
#endif 





#endif

