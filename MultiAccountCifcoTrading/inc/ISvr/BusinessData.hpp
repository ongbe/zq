#ifndef _BUSINESSDATA_HPP_
#define _BUSINESSDATA_HPP_

#include "..\\ctp\\ThostFtdcUserApiStruct.h" //引用CTP的数据结构
#include "../inc/Module-Misc/globalDefines.h"
#include "../inc/Module-Misc/tools_util.h"
#include "PlatformStruct.h"
#include "../inc/Module-Misc/tools_tools.h"
#include <string>


//当执行一些查询命令时，可能查询结果为空，
//但综合交易平台的API依然认为此查询成功，
//我们将这种情况"纠正"为查询失败，所以各GUI
//子模块需处理这个特殊的ErrorID
#define CustomErrorID 123456789
#define CustomErrorMsg "查询无记录"
#define THOST_FTDC_OST_ERROE  'd'
#define ORDER_INSERT_FRONTID  -1
#define ORDER_INSERT_SESSIONID -1
#define ORDER_INSERT_ORDERSTATUS 'N'

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
    BID_RspOrderInsert,             //报单录入回执(根据文档，一定是错误回执)
    BID_RspOrderAction1,            //报单操作回执(错误回执1，综合交易平台认为撤单无效)
    BID_RspOrderAction2,            //报单操作回执(错误回执2，交易所认为撤单无效)
    BID_RtnOrder,                   //报单回报(报单录入与报单操作都会产生该回报)
    BID_RtnTrade,                   //成交回报
    BID_RtnDepthMarketData,         //深度行情
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
    BID_RspQryExchange,             //请求查询交易所响应
    BID_RspQryInstrument,           //请求查询合约响应
    BID_RspQryDepthMarketData,      //请求查询行情响应
    BID_RspQrySettlementInfo,       //请求查询投资者结算结果响应
    BID_RspQryTransferBank,         //请求查询转帐银行响应
    BID_RspQryInvestorPositionDetail,//请求查询投资者持仓明细响应
    BID_RspQryNotice,               //请求查询客户通知响应
    BID_RspQrySettlementInfoConfirm,//请求查询结算信息确认响应
    BID_RspQryInvestorPositionComb, //请求查询投资者持仓明细响应
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

    //ADD NEW
    BID_FrontDisconnected,                  //与交易后台通信连接断开
    BID_FrontConnected,					//与交易后台通信连接成功


    BID_QryStart,                           //查询开始，参数为string，表示查询内容
    BID_ConnectivityStatus,
    BID_AlertMessage,			//Pats Alert Message
    BID_ExchangeRate,		
    BID_MAX
};

///标示一个报单记录
struct OrderKey
{
    OrderKey()
    {
        memset(this,0,sizeof(*this));
        FrontID=-1;
        SessionID=-1;
    };

    OrderKey(const char* account,const char* instrumentID,int fid, int sid, const char* orderRef)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        FrontID=fid;
        SessionID=sid;
        CTools::mymemcpy_trim(OrderRef,sizeof(OrderRef),orderRef);
    };

    OrderKey(const PlatformStru_OrderInfo& Order)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),Order.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
        FrontID=Order.FrontID;
        SessionID=Order.SessionID;
        CTools::mymemcpy_trim(OrderRef,sizeof(OrderRef),Order.OrderRef);
    };

    OrderKey& operator= (const OrderKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));
        return *this;
    };

    bool operator< (const OrderKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(FrontID<r.FrontID)
            return true;
        if(FrontID>r.FrontID)
            return false;

        if(SessionID<r.SessionID)
            return true;
        if(SessionID>r.SessionID)
            return false;

        irlt=strncmp(OrderRef,r.OrderRef,sizeof(OrderRef));
        if(irlt<0) return true;
        if(irlt>0) return false;

        return false;
    }

    bool operator== (const OrderKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(FrontID!=r.FrontID) return false;

        if(SessionID!=r.SessionID) return false;

        irlt=strncmp(OrderRef,r.OrderRef,sizeof(OrderRef));
        if(irlt!=0) return false;

        return true;
    }

    bool operator> (const OrderKey& r) const
    {
        return !((*this) < r || (*this) == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)        { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Front%d,SessionD%d",FrontID,SessionID);
        ilen=strlen(buf);
        if(len+ilen<rltsize)             { strcat(prlt,buf);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(OrderRef);
        if(len+ilen+1<rltsize)       { strcat(prlt,OrderRef);  strcat(prlt,",");  len+=ilen+1; }
        return prlt;
    }
    
    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;       //合约ID
    int                     FrontID;            //前置机有许多，通过此ID标记不同的前置机
    int                     SessionID;          //只要重新登入，就会变化，
    CommonOrderRefType      OrderRef;           //(长度为12个字符，右对齐，左边补空格)报单在GUI中的ID,
                                                //只要FrontID与SessionID的组合唯一，
                                                //OrderRef就能重复使用    
};

///标示一个成交记录
struct TradeKey
{
//    TradeKey()
//    {
//    };

    TradeKey(const char* account, 
                const char* instrumentID, 
                const char* tradeID, 
                const char* tradeDate, 
                const char* ordersysID, 
                const char direction)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools::mymemcpy_trim(TradeDate,sizeof(TradeDate),tradeDate);
        CTools::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);
        CTools::mymemcpy_trim(OrderSysID,sizeof(OrderSysID),ordersysID);
        Direction=direction;
    };

    TradeKey(const PlatformStru_TradeInfo& Trade)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),Trade.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Trade.InstrumentID);
        CTools::mymemcpy_trim(TradeDate,sizeof(TradeDate),Trade.TradeDate);
        CTools::mymemcpy_trim(TradeID,sizeof(TradeID),Trade.TradeID);
        CTools::mymemcpy_trim(OrderSysID,sizeof(OrderSysID),Trade.OrderSysID);
        Direction=Trade.Direction;
    };

    TradeKey& operator= (const TradeKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };

    //注意，通过TradeDate和TradeID的比较顺序，保证set<TradeKey>对相同条件的记录，按时间排序
    bool operator< (const TradeKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(TradeDate,r.TradeDate,sizeof(TradeDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(OrderSysID,r.OrderSysID,sizeof(OrderSysID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction) return true;
        if(Direction>r.Direction) return false;
        
        return false;
    }

    bool operator== (const TradeKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(TradeDate,r.TradeDate,sizeof(TradeDate));
        if(irlt!=0) return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt!=0) return false;

        irlt=strncmp(OrderSysID,r.OrderSysID,sizeof(OrderSysID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;

        return true;
    }
    
    bool operator> (const TradeKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;

        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(TradeDate);
        if(len+ilen+1<rltsize)   { strcat(prlt,TradeDate);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,TradeID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(OrderSysID);
        if(len+ilen+1<rltsize)   { strcat(prlt,OrderSysID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d",Direction);
        ilen=strlen(buf);
        if(len+ilen<rltsize)      { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }

    CommonAccountType       Account;
    CommonInstrumentIDType  InstrumentID;      //合约ID
    CommonDateType          TradeDate;         //成交日期(长度为8个字符，如:20101023)
    CommonTradeIDType       TradeID;           //成交ID(长度为20个字符，右对齐，左边补空格)
    CommonOrderSysIDType    OrderSysID;        //报单ID(长度为20个字符，右对齐，左边补空格)
	char		            Direction;         //考虑模拟盘自成交情况，加上方向
};

///标示一个成交统计记录
struct TradeTotalKey
{
//    TradeTotalKey()
//    {
//    };

    TradeTotalKey(const char* account,
                    const char* exchangeID, 
                    const char* instrumentID, 
                    const char direction, 
                    const char offsetFlag)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),exchangeID);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        Direction=direction;
        OffsetFlag=offsetFlag;
    };

    TradeTotalKey(const PlatformStru_TradeTotalInfo& TradeTotal)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),TradeTotal.szAccount);
        CTools::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),TradeTotal.ExchangeID);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),TradeTotal.InstrumentID);
        Direction=TradeTotal.Direction;
        OffsetFlag=TradeTotal.OffsetFlag;
    };

	TradeTotalKey(const PlatformStru_TradeInfo& TradeInfo)
	{
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),TradeInfo.szAccount);
        CTools::mymemcpy_trim(ExchangeID,sizeof(ExchangeID),TradeInfo.ExchangeID);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),TradeInfo.InstrumentID);
        Direction=TradeInfo.Direction;
        OffsetFlag=TradeInfo.OffsetFlag;
    }


    TradeTotalKey& operator= (const TradeTotalKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };

    bool operator< (const TradeTotalKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(ExchangeID,r.ExchangeID,sizeof(ExchangeID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
            return true;
        if(Direction>r.Direction)
            return false;

        if(OffsetFlag<r.OffsetFlag)
            return true;
        if(OffsetFlag>r.OffsetFlag)
            return false;

        return false;
    }

    bool operator== (const TradeTotalKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(ExchangeID,r.ExchangeID,sizeof(ExchangeID));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;

        if(OffsetFlag!=r.OffsetFlag)
            return false;

        return true;
    }
    
    bool operator> (const TradeTotalKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(ExchangeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,ExchangeID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d,Offset%d",Direction,OffsetFlag);
        ilen=strlen(buf);
        if(len+ilen<rltsize)             { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }

    CommonAccountType           Account;
    CommonExchangeIDType        ExchangeID;
    CommonInstrumentIDType      InstrumentID;      //合约ID
	char			            Direction;
	char			            OffsetFlag;
};


///标示一个持仓明细记录，由InstrumentID、OpenDate、TradeID、Direction、hedgeFlag、TradeType定义唯一的持仓明细记录
struct PositionDetailKey
{
	PositionDetailKey(const char* account, 
                        const char* instrumentID, 
                        const char* openDate, 
                        const char* tradeID, 
                        const char& direction, 
                        const char& hedgeFlag, 
                        const char& tradeType)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools::mymemcpy_trim(OpenDate,sizeof(OpenDate),openDate);
        CTools::mymemcpy_trim(TradeID,sizeof(TradeID),tradeID);

        Direction=direction;
        HedgeFlag=hedgeFlag;
        TradeType=tradeType;
    };

    PositionDetailKey(const PlatformStru_PositionDetail& PositionDetail)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),PositionDetail.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),PositionDetail.InstrumentID);
        CTools::mymemcpy_trim(OpenDate,sizeof(OpenDate),PositionDetail.OpenDate);
        CTools::mymemcpy_trim(TradeID,sizeof(TradeID),PositionDetail.TradeID);

        Direction=PositionDetail.Direction;
        HedgeFlag=PositionDetail.HedgeFlag;
        TradeType=PositionDetail.TradeType;
    };

    PositionDetailKey& operator= (const PositionDetailKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };


    //注意，通过OpenDate和TradeID的比较顺序，保证set<PositionDetailKey>对相同条件的持仓明细，按时间排序
    bool operator< (const PositionDetailKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools::mycompare2_trynumeric(TradeID,r.TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
			return true;
		if(Direction>r.Direction)
			return false;

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag) 
			return false;

        if(TradeType<r.TradeType)
			return true;
		if(TradeType>r.TradeType) 
			return false;


        return false;
    }

    bool operator== (const PositionDetailKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt!=0) return false;

        irlt=strncmp(TradeID,r.TradeID,sizeof(TradeID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;
        if(HedgeFlag!=r.HedgeFlag)
            return false;
        if(TradeType!=r.TradeType)
            return false;

        return true;
    }
    
    bool operator!= (const PositionDetailKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionDetailKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(OpenDate);
        if(len+ilen+1<rltsize)   { strcat(prlt,OpenDate);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,TradeID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d,Hedge%d,TradeType%d",Direction,HedgeFlag,TradeType);
        ilen=strlen(buf);
        if(len+ilen<rltsize)     { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }


	CommonAccountType       Account;		    //账号
    CommonInstrumentIDType  InstrumentID;       //合约ID
    CommonDateType          OpenDate;           //开仓日期
    CommonTradeIDType       TradeID;            //成交ID(长度为20个字符，右对齐，左边补空格)
    char                    Direction;          //买卖方向
    char                    HedgeFlag;          //投机套保标志
    char                    TradeType;          //成交类型，'0'/0x0: 普通成交； '4': 组合衍生成交
};


///标示一个持仓记录，由InstrumentID、Direction、HedgeFlag定义唯一的持仓明细记录
struct PositionKey
{
//    PositionKey()
//    {
//    };

	PositionKey(const char* account, const char* instrumentID, const char& posiDirection, const char& hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        PosiDirection=posiDirection;
        HedgeFlag=hedgeFlag;
    };

    PositionKey(const PlatformStru_Position& Position)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),Position.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Position.InstrumentID);
        PosiDirection=Position.PosiDirection;
        HedgeFlag=Position.HedgeFlag;
    };

	PositionKey(const PlatformStru_OrderInfo& Order)
	{
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),Order.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),Order.InstrumentID);
		if(Order.CombOffsetFlag[0]==THOST_FTDC_OF_Open)
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		else if(Order.CombOffsetFlag[0]==THOST_FTDC_OF_Close||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_ForceClose||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_CloseToday||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_CloseYesterday||
				Order.CombOffsetFlag[0]==THOST_FTDC_OF_LocalForceClose)
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Short : THOST_FTDC_PD_Long;
		else 
			PosiDirection = Order.Direction==THOST_FTDC_D_Buy ? THOST_FTDC_PD_Long : THOST_FTDC_PD_Short;
		HedgeFlag=Order.CombHedgeFlag[0];
	};

    PositionKey& operator= (const PositionKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));
        return *this;
    };


    bool operator< (const PositionKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(PosiDirection<r.PosiDirection)
			return true;
		if(PosiDirection>r.PosiDirection)
			return false;

        if(HedgeFlag<r.HedgeFlag)
			return true;
		if(HedgeFlag>r.HedgeFlag) 
			return false;

        return false;
    }

    bool operator== (const PositionKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        if(PosiDirection!=r.PosiDirection)
            return false;

        if(HedgeFlag!=r.HedgeFlag)
            return false;

        return true;
    }
    
    bool operator!= (const PositionKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionKey& r) const
    {
        return !(*this < r ||*this == r);
    }
    bool IsSamePositionKey(const PlatformStru_PositionDetail& PositionDetail) const
    {
        int off,len;

        CTools::mygettrimpos(PositionDetail.szAccount,off,len);
        if(strlen(Account)!=len||memcmp(Account,PositionDetail.szAccount,len)!=0)
            return false;

        CTools::mygettrimpos(PositionDetail.InstrumentID,off,len);
        if(strlen(InstrumentID)!=len||memcmp(InstrumentID,PositionDetail.InstrumentID,len)!=0)
            return false;

        if( HedgeFlag != PositionDetail.HedgeFlag)
            return false;

        if( PosiDirection==THOST_FTDC_PD_Long && PositionDetail.Direction!=THOST_FTDC_D_Buy ||
            PosiDirection==THOST_FTDC_PD_Short && PositionDetail.Direction!=THOST_FTDC_D_Sell )
            return false;

        return true;
    }
    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d,Hedge%d",PosiDirection,HedgeFlag);
        ilen=strlen(buf);
        if(len+ilen<rltsize)     { strcat(prlt,buf);  len+=ilen; }
        return prlt;
    }

	CommonAccountType       Account;			//账号
    CommonInstrumentIDType  InstrumentID;      //合约ID
    char                    PosiDirection;     //买卖方向
    char                    HedgeFlag;         //投机套保标志
};

///标示一个组合持仓明细记录
struct PositionCombKey
{
    PositionCombKey(const char* account, 
                    const char* instrumentID, 
                    const char* openDate, 
                    const char* leg1TradeID, 
                    const char* leg2TradeID, 
                    const char& direction, 
                    const char& hedgeFlag)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),account);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),instrumentID);
        CTools::mymemcpy_trim(OpenDate,sizeof(OpenDate),openDate);
        CTools::mymemcpy_trim(Leg1TradeID,sizeof(Leg1TradeID),leg1TradeID);
        CTools::mymemcpy_trim(Leg2TradeID,sizeof(Leg2TradeID),leg2TradeID);

        Direction=direction;
        HedgeFlag=hedgeFlag;
    };

    PositionCombKey(const PlatformStru_PositionComb& PositionComb)
    {
        memset(this,0,sizeof(*this));

        CTools::mymemcpy_trim(Account,sizeof(Account),PositionComb.szAccount);
        CTools::mymemcpy_trim(InstrumentID,sizeof(InstrumentID),PositionComb.InstrumentID);
        CTools::mymemcpy_trim(OpenDate,sizeof(OpenDate),PositionComb.OpenDate);
        CTools::mymemcpy_trim(Leg1TradeID,sizeof(Leg1TradeID),PositionComb.Leg1TradeID);
        CTools::mymemcpy_trim(Leg2TradeID,sizeof(Leg2TradeID),PositionComb.Leg2TradeID);

        Direction=PositionComb.Direction;
        HedgeFlag=PositionComb.HedgeFlag;
    };

    PositionCombKey& operator= (const PositionCombKey& other)
    {
        if (this == &other)
            return *this;

        memcpy((char*)this,(char*)&other,sizeof(*this));

        return *this;
    };

    //注意，通过OpenDate和TradeID的比较顺序，保证set<PositionCombKey>对相同条件的记录，按时间排序
    bool operator< (const PositionCombKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt<0) return true;
        if(irlt>0) return false;
		
        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools::mycompare2_trynumeric(Leg1TradeID,r.Leg1TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        irlt=CTools::mycompare2_trynumeric(Leg2TradeID,r.Leg2TradeID);
        if(irlt<0) return true;
        if(irlt>0) return false;

        if(Direction<r.Direction)
		    return true;
	    else if(Direction>r.Direction)
		    return false;

        if(HedgeFlag<r.HedgeFlag)
		    return true;
	    else if(HedgeFlag>r.HedgeFlag) 
		    return false;
	     
		return false;
    }

    bool operator== (const PositionCombKey& r) const
    {
        int irlt;

        irlt=strncmp(Account,r.Account,sizeof(Account));
        if(irlt!=0) return false;

        irlt=strncmp(InstrumentID,r.InstrumentID,sizeof(InstrumentID));
        if(irlt!=0) return false;

        irlt=strncmp(OpenDate,r.OpenDate,sizeof(OpenDate));
        if(irlt!=0) return false;

        irlt=strncmp(Leg1TradeID,r.Leg1TradeID,sizeof(Leg1TradeID));
        if(irlt!=0) return false;

        irlt=strncmp(Leg2TradeID,r.Leg2TradeID,sizeof(Leg2TradeID));
        if(irlt!=0) return false;

        if(Direction!=r.Direction)
            return false;

        if(HedgeFlag!=r.HedgeFlag)
            return false;

        return true;
    }
    
    bool operator!= (const PositionCombKey& r) const
    {
        return !(*this == r);
    }
    
    bool operator> (const PositionCombKey& r) const
    {
        return !(*this < r ||*this == r);
    }

    char* tostring(char*prlt,int rltsize) const
    {
        if(!prlt||!rltsize) return NULL;
        int len=0,ilen;
        prlt[len]=0;

        ilen=strlen(Account);
        if(len+ilen+1<rltsize)   { strcat(prlt,Account);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(InstrumentID);
        if(len+ilen+1<rltsize)   { strcat(prlt,InstrumentID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(OpenDate);
        if(len+ilen+1<rltsize)   { strcat(prlt,OpenDate);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(Leg1TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,Leg1TradeID);  strcat(prlt,",");  len+=ilen+1; }

        ilen=strlen(Leg2TradeID);
        if(len+ilen+1<rltsize)   { strcat(prlt,Leg2TradeID);  strcat(prlt,",");  len+=ilen+1; }

        char buf[256];
        sprintf(buf,"Direct%d,Hedge%d",Direction,HedgeFlag);
        ilen=strlen(buf);
        if(len+ilen<rltsize)     { strcat(prlt,buf);  len+=ilen; }

        return prlt;
    }

	CommonAccountType       Account;			//账号
    CommonInstrumentIDType  InstrumentID;      //组合合约ID
    CommonDateType          OpenDate;          //开仓日期
    CommonTradeIDType       Leg1TradeID;       //Leg1的成交编号
    CommonTradeIDType       Leg2TradeID;       //Leg2的成交编号
    char                    Direction;         //买卖方向
    char                    HedgeFlag;         //投机套保标志
};






//下面定义一个合约不同挂单量的数据结构
struct WaitOrderVolumeStru
{
    int         OpenVolume_Buy_Speculation;                 //开仓-买-投机
    int         CloseVolume_Buy_Speculation;                //平仓-买-投机
    int         CloseTodayVolume_Buy_Speculation;           //平今-买-投机
    int         CloseYesterdayVolume_Buy_Speculation;       //平昨-买-投机

    int         OpenVolume_Sell_Speculation;                //开仓-卖-投机
    int         CloseVolume_Sell_Speculation;               //平仓-卖-投机
    int         CloseTodayVolume_Sell_Speculation;          //平今-卖-投机
    int         CloseYesterdayVolume_Sell_Speculation;      //平昨-卖-投机

    int         OpenVolume_Buy_Hedge;                       //开仓-买-套保
    int         CloseVolume_Buy_Hedge;                      //平仓-买-套保
    int         CloseTodayVolume_Buy_Hedge;                 //平今-买-套保
    int         CloseYesterdayVolume_Buy_Hedge;             //平昨-买-套保

    int         OpenVolume_Sell_Hedge;                      //开仓-卖-套保
    int         CloseVolume_Sell_Hedge;                     //平仓-卖-套保
    int         CloseTodayVolume_Sell_Hedge;                //平今-卖-套保
    int         CloseYesterdayVolume_Sell_Hedge;            //平昨-卖-套保
};

struct ExchangeTime
{
    ExchangeTime()
    {
        CurTickCount=0;
        memset(&TradingDay, 0, sizeof(TradingDay));
        memset(&SHFETime, 0, sizeof(SHFETime));
        memset(&DCETime, 0, sizeof(DCETime));
        memset(&CZCETime, 0, sizeof(CZCETime));
        memset(&FFEXTime, 0, sizeof(FFEXTime));
    };
    
    unsigned long             CurTickCount;//本地电脑开机后的毫秒数据
    TThostFtdcDateType	TradingDay;
    ///上期所时间
	TThostFtdcTimeType	SHFETime;
	///大商所时间
	TThostFtdcTimeType	DCETime;
	///郑商所时间
	TThostFtdcTimeType	CZCETime;
	///中金所时间
	TThostFtdcTimeType	FFEXTime;
};

struct AbstractBusinessData
{
    BusinessID                 BID;
	void*					   pPlatformSvr;
    unsigned int               Size;//肯定大于等于sizeof(AbstractBusinessData)
};

typedef int (*GuiCallBackFunc)(const GUIModuleID,const AbstractBusinessData&);

//登入请求响应
struct DataRspUserLogin
{
    DataRspUserLogin()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspUserLogin;
        Head.Size = sizeof(*this);
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
    DataRspUserLogout()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspUserLogout;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspUserPasswordUpdate;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspTradingAccountPasswordUpdate;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspParkedOrderInsert;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspParkedOrderAction;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspRemoveParkedOrder;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspRemoveParkedOrderAction;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryParkedOrder;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryParkedOrderAction;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspOrderInsert;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInputOrderField InputOrderField;
    CThostFtdcRspInfoField    RspInfoField;    
};



//报单操作回执(错误回执1，综合交易平台认为撤单无效)
struct DataRspOrderAction1
{
    DataRspOrderAction1()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspOrderAction1;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInputOrderActionField InputOrderActionField;
    CThostFtdcRspInfoField     RspInfoField;    
};

//报单操作回执(错误回执2，交易所认为撤单无效)
struct DataRspOrderAction2
{
    DataRspOrderAction2()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspOrderAction2;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    CThostFtdcOrderActionField OrderActionField;
    CThostFtdcRspInfoField     RspInfoField;    
};

//报单回报(下单无误，会产生此消息，撤单无误，也会产生此消息)
struct DataRtnOrder
{
    DataRtnOrder()
    {
        //memset(this, 0, sizeof(*this));PlatformStru_OrderInfo有默认构造函数
        Head.BID = BID_RtnOrder;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    PlatformStru_OrderInfo      OrderField; 
};

//成交回报
struct DataRtnTrade
{
    DataRtnTrade()
    {
        //memset(this, 0, sizeof(*this));PlatformStru_TradeInfo有默认构造函数
        Head.BID = BID_RtnTrade;
        Head.Size = sizeof(*this);
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
    bool                            bHavePosition;  //表示是否有持仓
};

//请求查询投资者持仓响应
struct DataRspQryInvestorPosition
{
    DataRspQryInvestorPosition()
    {
        Head.BID = BID_RspQryInvestorPosition;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        InvestorPositionField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));
        memset(&QryInstrumentID, 0, sizeof(TThostFtdcInstrumentIDType));
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
    {
        Head.BID = BID_RspQryInvestorPositionDetail;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        InvestorPositionDetailField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));
        memset(&QryInstrumentID, 0, sizeof(TThostFtdcInstrumentIDType));
    };

    AbstractBusinessData		Head;
    int							nRequestID;
    bool						bIsLast;
    PlatformStru_PositionDetail	InvestorPositionDetailField;
    CThostFtdcRspInfoField		RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //查询的合约ID
};

//请求查询投资者持仓组合合约明细响应
struct DataRspQryInvestorPositionComb
{
    DataRspQryInvestorPositionComb()
    {
        Head.BID = BID_RspQryInvestorPositionComb;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        InvestorPositionCombineDetailField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));
        memset(&QryInstrumentID, 0, sizeof(TThostFtdcInstrumentIDType));
    };

    AbstractBusinessData		Head;
    int							nRequestID;
    bool						bIsLast;
    PlatformStru_PositionComb	InvestorPositionCombineDetailField;
    CThostFtdcRspInfoField		RspInfoField;    
  	TThostFtdcInstrumentIDType	QryInstrumentID;        //查询的合约ID
};

//请求查询资金账户响应
struct DataRspQryTradingAccount
{
    DataRspQryTradingAccount()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryTradingAccount;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryInvestor;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryTradingCode;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryInstrumentMarginRate;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryInstrumentCommissionRate;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentCommissionRate   InstrumentCommissionRateField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询交易所响应
struct DataRspQryExchange
{
    DataRspQryExchange()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryExchange;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcExchangeField   ExchangeField;
    CThostFtdcRspInfoField    RspInfoField;    
};

//请求查询合约响应
struct DataRspQryInstrument
{
    DataRspQryInstrument()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryInstrument;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    PlatformStru_InstrumentInfo InstrumentField;
    CThostFtdcRspInfoField     RspInfoField;    
};


//请求查询报单响应
struct DataRspQryOrder
{
    DataRspQryOrder()
    {
        Head.BID = BID_RspQryOrder;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        OrderField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));
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
    DataRspQryTrade()
    {
        Head.BID = BID_RspQryTrade;
        Head.Size = sizeof(*this);
        nRequestID=0;
        bIsLast=false;
        TradeField.Clear();
        memset(&RspInfoField, 0, sizeof(CThostFtdcRspInfoField));  
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQrySettlementInfo;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspFromBankToFutureByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspFromFutureToBankByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQueryBankAccountMoneyByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryTransferSerial;
        Head.Size = sizeof(*this);
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
    DataRspQryAccountregister()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryAccountregister;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnFromBankToFutureByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnFromFutureToBankByFuture;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnQueryBankAccountMoneyByFuture;
        Head.Size = sizeof(*this);
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
    DataRspQryContractBank()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryContractBank;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcContractBankField      ContractBankField;
    CThostFtdcRspInfoField    RspInfoField;    
};

///合约交易状态
struct DataInstrumentStatus
{
    DataInstrumentStatus()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnInstrumentStatus;
        Head.Size = sizeof(*this);
    };

    AbstractBusinessData      Head;
    int                       nRequestID;
    bool                      bIsLast;
    CThostFtdcInstrumentStatusField      InstrumentStatusField; 
};
//与交易后台通信连接成功
struct DataFrontConnected
{
    DataFrontConnected()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_FrontDisconnected;
        Head.Size = sizeof(*this);
    };
    AbstractBusinessData      Head;
	bool                      bTrader;
};
//与交易后台通信连接断开
struct DataFrontDisconnected
{
    DataFrontDisconnected()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_FrontDisconnected;
        Head.Size = sizeof(*this);
    };
    AbstractBusinessData      Head;
	bool                      bTrader;
    int                       nReason; 
};
struct DataRspQrySettlementInfoConfirm
{
	DataRspQrySettlementInfoConfirm()
	{
		memset(this, 0, sizeof(*this));
		Head.BID = BID_RspQrySettlementInfoConfirm;
		Head.Size = sizeof(*this);
	};
	AbstractBusinessData      Head;
	int                       nRequestID;
	bool                      bIsLast;
	CThostFtdcSettlementInfoConfirmField      SettlementInfoConfirm;
	CThostFtdcRspInfoField    RspInfoField;    
};
//
struct DataTradingNoticeInfoField
{
    DataTradingNoticeInfoField()
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RtnTradingNotice;
        Head.Size = sizeof(*this);
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
    {
        memset(this, 0, sizeof(*this));
        Head.BID = BID_RspQryNotice;
        Head.Size = sizeof(*this);
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
typedef enum  _tagSStatus
{
	S_STATUS_ADD,
	S_STATUS_DEL,
	S_STATUS_EQU	
}S_Status;
typedef enum  _tagSPanel
{
	S_STATUS_TEMP=(1<<2),
	S_STATUS_QUOT=(1<<3),
	S_STATUS_ORDER=(1<<4),
	S_STATUS_TRADE=(1<<5),
	S_STATUS_POS=(1<<6),
	S_STATUS_PLUS=(1<<7)
}S_Panel;
/************服务器参数**begin********************************************************************************************************/
typedef struct tagServerParamCfgItem
{
	std::vector<std::string> TradeAddr;		//获取交易前置机地址
	std::vector<std::string> QuotAddr;		//获取行情前置机地址
	std::string BrokerID;					//获取经纪公司ID
	std::string InvestorID;					//登录的账户 
	std::string ServerName;					//中文
	int CTPSimulate;						//服务器是否CTP模拟服务器, 如是，则需要将合约对应的ExchangeID还原为真实的交易所ID(CTP都设为SHFE)
}SERVER_PARAM_CFG_ITEM, *LPSERVER_PARAM_CFG_ITEM;

typedef struct tagServerParamCfg
{
	std::vector<SERVER_PARAM_CFG_ITEM> vec;
	std::string UniserverAddr;
	std::string VersionStr;
	int LanguageId;
	int current;
	//下面代理服务器参数设置
	std::string ProxyType;//socks4,socks5
	std::string ProxyAddr;
	std::string ProxyPort;
	std::string ProxyUser;
	std::string ProxyPwd;
}SERVER_PARAM_CFG,* LPSERVER_PARAM_CFG;
/************服务器参数**end********************************************************************************************************/








#endif

