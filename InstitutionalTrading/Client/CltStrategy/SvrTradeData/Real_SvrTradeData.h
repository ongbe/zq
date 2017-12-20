#pragma once

#include "Interface_SvrTradeData.h"
#include "PlatformDataMgr_Instruments.h"
#include "PlatformDataMgr_Quots.h"
#include "PlatformDataMgr_Rates.h"
#include "Interface_AccountOp.h"
#include "Interface_UserOp.h"
#include "TradingDayManager.h"
#include <map>
#include <queue>
#include "BufferDataStore.h"
#include "rwlock.h"
#include "WriteLog.h"




//#ifdef _DEBUG
//#pragma comment(lib, "PlatformDataMgr4dllD.lib")
//#pragma comment(lib, "tools4dllD.lib")
//#pragma comment(lib, "wsock32.lib")
//#else
//#pragma comment(lib, "PlatformDataMgr4dll.lib")
//#pragma comment(lib, "tools4dll.lib")
//#pragma comment(lib, "wsock32.lib")
//#endif

// #ifdef _DEBUG
//     #pragma comment(lib, "PlatformDataMgr4clrD.lib")
//     //#pragma comment(lib, "tools4dllD.lib")
//     #pragma comment(lib, "wsock32.lib")
// #else
//     #pragma comment(lib, "PlatformDataMgr4clr.lib")
//     //#pragma comment(lib, "tools4dll.lib")
//     #pragma comment(lib, "wsock32.lib")
// #endif

typedef std::map<BrokerAccountKey,IInterfaceAccountOp*>            _MapAccountData;
//typedef std::map<BrokerAccountKey,eCommissionMarginReady>          _MapAccountRateReady; 
typedef std::map<std::string,IInterfaceUserOp*>                    _MapUserTradeData;

class CReal_SvrTradeData : public CInterface_SvrTradeData
{

public:
    static CReal_SvrTradeData& getObj(void);
	CReal_SvrTradeData();
	~CReal_SvrTradeData();

public: //交易所状态通知	
	virtual void SetIntrumentsStatus(const CThostFtdcInstrumentStatusField & nStatus);


	//判断是否是交易日
	//字符串为日期格式2013-08-09
	virtual bool IsTradingDay(const std::string & nsDay);


	//判断交易日是否结算
	//字符串为日期格式2013-08-09
	virtual bool IsSettlemented(const std::string & nsDay) ;

	//获取一个日期的上一个交易日
	virtual bool GetLastTrading(const std::string & nsDay, std::string & nsLastDay) ;

	//设置当前交易日
	virtual void SetCurrentTradingDay(const std::string & nsCurrentTradingDay,bool nbSetByMySelf=false) ;

	//获取当前交易日
	virtual void GetCurrentTradingDay(std::string & nsCurrentTradingDay) ;


	//交易状态初始化1
	virtual void TradeDataInit(CMemCache* pCache,string szUser,string szBrokerCode,string szTradeAccount,bool nbForce = false) ;

	//报盘设置每个账户的链接CTP的状态
	virtual void SetAccountStatus(const std::string & nsAccoutnID,
		const std::string & nsBrokerID,
		const EnumAccountStatus& neUserStatu) ;

	//获取每个账户的ctp状态
	virtual void GetAccountStatus(const std::string & nsAccoutnID,
		const std::string & nsBrokerID,
		EnumAccountStatus& neCTPStatus,
		double & nStatusTime);

	//获取所有委托交易账号的状态
	virtual void GetAllAccountStatus(std::vector<sAccountStatus> & lStatus);

	//获取所有用户的初始化状态
	virtual void GetAlUserInitStatus(std::vector<sUserInitStatus> & lStatus);

	//获取用户的ctp链接状态
	virtual EnumAccountStatus GetUserCTPStatus(int nUserID,double & nDifftime) ;

	//获取用户的初始化状态
	virtual eUserInitStatus GetUserInitStatus(int nUserID) ;


	//获取用户的ctp链接状态
	virtual EnumAccountStatus GetUserCTPStatus(const std::string& nsUser,double & nDifftime) ;

	//获取用户的初始化状态
	virtual eUserInitStatus GetUserInitStatus(const std::string& nsUser) ;

	//获取交易数据管理模块状态
	virtual eTradeDataStatus GetTraderDataStatus() ;

	//获取交易数据管理模块状态
	virtual eExchangeStatus GetExchangeStatus() ;

	//登陆和初始化一个委托交易账号
	virtual CF_ERROR LoginAdnInitTraderAccount(const BrokerAccountKey & nKey,int nAccountID) ;

public: //合约操作


	//获取交易日
	virtual void GetExchangeData(ExchangeTime & nData) ;

	///设置指定合约信息
	virtual void SetInstrumentInfo(PlatformStru_InstrumentInfo& outData) ;

	///添加主力合约列表
	virtual void AddMainInstrumentList(std::string instrument) ;

	///获取合约列表
	virtual int GetInstrumentList(std::set<std::string> &outData,
		std::string ExchangeID="") ;

	///获取主力合约列表
	virtual int GetMainInstrumentList(std::set<std::string> &outData) ;

	///设置合约容差列表
	virtual void SetInstrumentVarietyMap(map<string, string>& inData);

	// 获取合约容差列表
	virtual int GetInstrumentVarietyMap(map<string, string>& outData) ;

	///获取指定品种的合约列表,返回合约个数
	virtual int GetInstrumentListByProductID(const std::string& ProductID,
		std::set<std::string> &outData) ;

	///获取指定合约信息
	virtual int GetInstrumentInfo(const std::string& InstrumentID, 
		PlatformStru_InstrumentInfo& outData) ;

	///获取合约的产品类型，失败返回-1
	virtual char GetProductClassType(const std::string& InstrumentID) ;

	///获取包含此单腿合约的组合合约列表
	virtual bool GetCombInstrumentIDs_IncludeLeg(const std::string& LegInstrument,
		std::vector<std::string>& vecCombInstruments);

	///获取合约品种代码
	virtual int GetProductID(std::map<std::string, std::set<std::string> >& outData,
		std::string ExchangeID="");    

	///获取合约乘数，成功返回合约乘数，失败返回-1
	virtual int GetInstrumentMultiple(const std::string& InstrumentID) ;

	//报盘调用，查询合约完毕
	virtual void EndQryInstrument() ;

	//获取合约数据是否可用
	eDataReady GetInstrumentDataReady();


public://行情操作
	//设置指定合约行情，设置的行情更新，则返回true；否则返回false（比原来的行情更老）
	virtual CF_ERROR SetQuotInfo(PlatformStru_DepthMarketData& inData,bool bSettlementManul = false);

	///获取指定合约行情
	virtual int GetQuotInfo(const std::string& InstrumentID,
		PlatformStru_DepthMarketData& outData);

	///获取指定合约旧行情
	virtual int GetOldQuotInfo(const std::string& InstrumentID, 
		PlatformStru_DepthMarketData& outData);

	///获取指定合约的现价，失败返回无效值
	virtual double GetCurPrice(const std::string& InstrumentID);


/***********************************涉及委托交易账号的操作******************************************************************************/
/*
//费率
//手续费率
//资金
//持仓
//持仓明细
*/
public: //费率操作
	///获取某个账户合约手续费率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
	virtual int GetCommissionRate(const string& nsBrokerID,
		const string& nsAccountID,
		const string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;

	///获取某个用户合约手续费率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
	//因为一个用户绑定一个账号，所以目前可以用这种方式来返回费率
	virtual int GetCommissionRate(const string& nsUserID,	
		const string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;


	///设置某个账户合约手续费率
	virtual void SetCommissionRate(PlatformStru_InstrumentCommissionRate& outData) ;

	//报盘调用，当某一个用户合约的保证金率查询完成后
	virtual void EndUserQryCommission(const std::string& nsBrokerID,
		const std::string &nAccountID,
        const std::string &nUserID) ;

public: //保证金率
	///获取合约保证金率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
	virtual int GetMarginRate(const string& nsBrokerID,
		const string& nsAccountID,
		const string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) ;


	///获取合约保证金率, 成功返回0，失败返回-1，查询次数超限返回CustomErrorID
	virtual int GetMarginRate(const string& nsUserID,	
		const string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) ;

	///设置合约保证金率
	virtual void SetMarginRate(PlatformStru_InstrumentMarginRate& outData) ;


	//报盘调用，当某一个用户合约的保证金率查询完成后
	virtual void EndUserQryMargin(const std::string& nsBrokerID,
		const std::string &nAccountID) ;

public: //资金

	//报盘调用，当某一个主帐号资金查询完成
	virtual void QryFundReach(const PlatformStru_TradingAccountInfo& inData,const std::string & nUserID) ;


public: //持仓

	//查询到某一个主帐号的持仓
	virtual void QryPositionReach( PlatformStru_Position& PositionInfo, int ErrID, bool bIsLast) ;


	//报盘调用，当某一个持仓查询完成后
	virtual void EndUserQryPosition(const std::string& nsBrokerID,
		const std::string &nAccountID) ;

public: //持仓

	//查询到某一个主帐号的持仓
	virtual void QryPositionDetailReach( PlatformStru_PositionDetail& PositionInfo, int ErrID, bool bIsLast) ;


	//报盘调用，当某一个持仓明细查询完成后
	virtual void EndUserQryPositionDetail(const std::string& nsBrokerID,
		const std::string &nAccountID) ;

/***********************************涉及子账号的操作******************************************************************************/
/*
//持仓明细
//持仓
//组合持仓明细
//组合持仓
//成交
//成交汇总
//报单
*/

public://报单操作

	//查询报单到来
	virtual CF_ERROR QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
		const int ErrID,const bool bIsLast) ;

	//报盘调用，当某一个账户的报单后
	virtual void EndUserQryOrder(const std::string& nsBrokerID,
		const std::string &nAccountID);

	////添加一个报单
	virtual CF_ERROR PushOrder(PlatformStru_OrderInfo & nOrderInfo,
		bool nbNeedRiseEvent = true) ;
	
	// 报单录入应答
	virtual CF_ERROR PushRspOrderInsert(int nFrontID,
		int nSessionID,
		CThostFtdcInputOrderField *pInputOrder, 
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast);

	///报单录入错误回报
	virtual CF_ERROR PushErrRtnOrderInsert(int nFrontID,
		int nSessionID,
		CThostFtdcInputOrderField *pInputOrder,
		CThostFtdcRspInfoField *pRspInfo);

	///报单操作请求响应
	virtual CF_ERROR PushRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, 
		CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, 
		bool bIsLast);

	///报单操作错误回报
	virtual CF_ERROR PushErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction,
		CThostFtdcRspInfoField *pRspInfo);


	///获取指定报单
	virtual CF_ERROR GetUserOrderByKey(const std::string & nUserID,
		const OrderKey& nOrderKey,
		PlatformStru_OrderInfo& outData) ;

	///获取全部报单
	virtual CF_ERROR GetUserOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////获取全部已触发的报单
	virtual CF_ERROR GetUserTriggerOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////获取已成交报单
	virtual CF_ERROR GetUserTradedOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////获取已撤单和错误报单
	virtual CF_ERROR GetUserCanceledOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////获取未成交报单
	virtual CF_ERROR GetUserWaitOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	///获取未知状态的报单
	virtual CF_ERROR GetUserUnkownOrders(const std::string & nUserID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	//
	///获取指定报单
	virtual CF_ERROR GetAccountOrderByKey(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const OrderKey& nOrderKey,
		PlatformStru_OrderInfo& outData);


	///获取全部报单
	virtual CF_ERROR GetAccountOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////获取全部已触发的报单
	virtual CF_ERROR GetAccountTriggerOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////获取已成交报单
	virtual CF_ERROR GetAccountTradedOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////获取已撤单和错误报单
	virtual CF_ERROR GetAccountCanceledOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	/////获取未成交报单
	virtual CF_ERROR GetAccountWaitOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;

	///获取未知状态的报单
	virtual CF_ERROR GetAccountUnkownOrders(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_OrderInfo>& outData) ;
public: //成交操作

	//查询的成交回报到来
	virtual CF_ERROR QryTradeReach( PlatformStru_TradeInfo& TradeInfo,
		int ErrID,bool bIsLast) ;


	//报盘调用，当某一个账户的成交后
	virtual void EndUserQryTrade(const std::string& nsBrokerID,
		const std::string &nAccountID);

	//新的成交回报到来
	virtual CF_ERROR PushTrade( PlatformStru_TradeInfo& TradeInfo);

	//获取全部成交单，返回数据项的数量
	virtual CF_ERROR  GetUserTradeInfo(const std::string & nUserID,
		std::vector<PlatformStru_TradeInfo>& outData);

	//检查CTP成交里面的报单编号是不是我们的报单编号
	virtual bool CheckTradeOrderSysIDIsOwn(const std::string& nsUserID,
		const std::string& nsOrderSysID) ;

	//获取全部成交单，返回数据项的数量
	virtual CF_ERROR  GetAccountTradeInfo(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		std::vector<PlatformStru_TradeInfo>& outData) ;



	//获取主账户报单成交大于Seq
	virtual CF_ERROR  GetAccountOrderAndTraderGESeq(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo>& nvecTrader) ;


	//获取主账户报单成交大于Seq
	virtual CF_ERROR  GetUserOrderAndTraderGESeq(const std::string & nInvestorID,
		int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo>& nvecTrader) ;

public://持仓汇总操作


	//获取持仓汇总，返回数据项的数量
	virtual CF_ERROR  GetUserPositions(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) ;

	//获取总持仓量
	virtual CF_ERROR GetUserPositionVolume(const std::string & nUserID,
		const std::string & nInstrumentID,
		int & nVolume) ;
	//获取持仓汇总，
	virtual CF_ERROR  GetAccountPositions(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) ;
public: //持仓明细汇总

	//获取持仓明细，返回数据项的数量
	virtual CF_ERROR  GetUserPositionDetail(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) ;

	//获取持仓明细，返回对应的key的数量
	virtual CF_ERROR  GetUserPositionDetail(const std::string & nUserID,
		const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) ;

	//获取持仓明细
	//成功返回CF_ERROR_SUCCESS
	virtual CF_ERROR  GetAccountPositionDetail(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) ;

	//获取持仓明细，返回对应的key的持仓明细
	//成功返回CF_ERROR_SUCCESS
	virtual CF_ERROR  GetAccountPositionDetail(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) ;




public: //组合持仓汇总操作
	//获取组合持仓，返回数据项的数量
	virtual CF_ERROR  GetUserPositionComb(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData) ;

public: //组合持仓明细操作

	//获取组合持仓明细，返回数据项的数量
	virtual CF_ERROR  GetUserPositionCombDetail(const std::string & nUserID,
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetailComb>& outData);


	
public: //资金操作
	//获取交易员资金信息
	virtual CF_ERROR  GetUserFundInfo(const std::string & nUserID,
		PlatformStru_TradingAccountInfo& outData) ;

	//获取交易员初始资金
	virtual CF_ERROR  GetUserInitFund(const std::string & nUserID,
		double& outData) ;


	//获取委托交易帐号资金信息
	virtual CF_ERROR  GetAccountFundInfo(const std::string & nsBrokerID,
		const std::string & nInvestorID,
		PlatformStru_TradingAccountInfo& outData) ;

public: //获取汇总操作


	//获取当前交易日的合约
	virtual CF_ERROR  GetCurrentDayAlInstrumentInfo(
		std::vector<PlatformStru_InstrumentInfo> & outData) ;

	//获取所有委托交易账号的手续费率
	virtual CF_ERROR GetAllCommissionRate(
		std::vector<PlatformStru_InstrumentCommissionRate> & outData) ;

	//获取所有委托交易账号的保证金率
	virtual CF_ERROR GetAllMarginRate(
		std::vector<PlatformStru_InstrumentMarginRate> & outData) ;


	//获取所有的结算价
	virtual CF_ERROR  GetAllSettlementPrice(
		std::map<std::string,PlatformStru_DepthMarketData> & outData) ;

	//获取当前交易日的结算价
	virtual CF_ERROR  GetCurrentDayAllSettlementPrice(
		std::map<std::string,PlatformStru_DepthMarketData> & outData) ;

	//获取所有的成交
	virtual CF_ERROR  GetAllTradeInfo(
		std::map<std::string,std::vector<PlatformStru_TradeInfo>> & outData) ;

	//获取所有的持仓明细
	virtual CF_ERROR  GetAllPositionDetail(
		std::map<std::string,std::vector<PlatformStru_PositionDetail>> & outData) ;

	//获取所有的持仓
	virtual CF_ERROR  GetAllPosition(
		std::map<std::string,std::vector<PlatformStru_Position>> & outData) ;

	//获取所有的资金
	virtual CF_ERROR  GetAllFundInfo(
		std::map<std::string,PlatformStru_TradingAccountInfo> & outData) ;

	//开始结算
	virtual CF_ERROR  StartSettlement(eSettlementLoadOption nOption,
		const std::string & nsDay ,
		bool nbHistorySettlement) ;

	
	//结束结算
	virtual CF_ERROR  EndSettlement(const std::string & nsDay);

	//设置修改后的手续费率和保证金率，并触发成交和持仓和资金的计算
	virtual CF_ERROR  SetModifiedCommissionAndMargin( std::vector<PlatformStru_InstrumentCommissionRate> & inCommssion,
		 std::vector<PlatformStru_InstrumentMarginRate> & inMargin) ;

	//设置修改后的成交，并触发成交和持仓和资金的计算
	virtual CF_ERROR  SeUserNewTrades(const std::string & nUserName,
		const std::vector<PlatformStru_TradeInfo> & inTradeInfo) ;

public:
	virtual CF_ERROR CheckUserInstrumentCommissionAndMaginExist(const std::string & nUserName,
		const std::string & nIstrumentID) ;
	
	//判断是否可开仓
	virtual CF_ERROR  CanOrderOpen(const PlatformStru_OrderInfo & nOrder);

	//判断是否可平今
	virtual CF_ERROR  CanOrderColseToday(const PlatformStru_OrderInfo & nOrder) ;

	//判断是否可平仓
	virtual CF_ERROR  CanOrderColse(const PlatformStru_OrderInfo & nOrder) ;

public:	
	//出入金
	virtual CF_ERROR FundInOut(const sFundInOut& nFundInOut) ;

	//查询历史出入金
	virtual CF_ERROR QryHistroyFundInOut(const sQryFundInOut & nQryFund,
		std::vector<sFundInOut> & nvecFund) ;

    //设置用户初始资金 策略本地调试 
    virtual CF_ERROR SetUserInitFund(const double dInitFund);

public:
	//问询函数
	virtual CF_ERROR ResponseToAsk(const eAskType& neDataType,
		const eAskOperationType& neChangeType,
		const int nllDataID,
		const char * npDataBuf);


public:
	////添加一个CTP报单
	virtual CF_ERROR QryCTPOrder(PlatformStru_OrderInfo & nOrderInfo,const int ErrID,const bool bIsLast) ;

	//添加一个CTP成交
	virtual CF_ERROR QryCTPTrader(PlatformStru_TradeInfo & nTraderInfo,const int ErrID,const bool bIsLast) ;

	////添加一个CTP报单
	virtual CF_ERROR PushCTPOrder(PlatformStru_OrderInfo & nOrderInfo) ;

	//添加一个CTP成交
	virtual CF_ERROR PushCTPTrader(PlatformStru_TradeInfo & nTraderInfo) ;


	////添加客户端下单
	virtual CF_ERROR PushExecuteInOrder(PlatformStru_OrderInfo & nOrderInfo) ;

	//添加一个客户端下单的返回结果
	virtual CF_ERROR PushExecuteOutOrder(PlatformStru_OrderInfo & nOraderInfo) ;

public:
	//获取操作数据库的接口
// 	virtual IInterfaceDBOpr * GetInterfaceDBOpr();

public:
	//查看用户是否确认了结算单
	virtual bool IsUserSettlementConfirmed(const std::string & nUserName,
		std::string & nsConfirmedDate,
		std::string & nsConfirmedTime) ;

	//用户确认了结算单
	//用户确认了结算单
	virtual void SetUserSettlementConfirmed(const std::string & nsUserName,
		const std::string & nsConfirmedDate,
		const std::string & nsConfirmedTime) ;
public:
	//获取初始化时间
	virtual CF_ERROR GetReinitTime(sReinitTime & nsReinitTime) ;

	//获取数据保存时间
	virtual CF_ERROR GetSaveTime(sSaveTime & nsReinitTime) ;

	//设置初始化时间
	virtual CF_ERROR SetReinitTime(const sReinitTime & nsReinitTime) ;

	//获取交易时间
	virtual CF_ERROR GetTradingTime(sTradingTime & nsTradingTime) ;


	//设置交易时间
	virtual CF_ERROR SetTradingTime(const sTradingTime & nsTradingTime) ;


public:
	//报盘查询不成功调用
	virtual void QryFailed(const BrokerAccountKey & nBrokerAccount,eEventType nEvent) ;

	//获取对应用户对应时间段的持仓明细
	virtual CF_ERROR GetPositionDetail_MaxTime(const  std::vector<std::string> nvecAccount, 
		int LowerOpenTime,int UpperOpenTime,
		std::map<std::string, std::vector<PlatformStru_PositionDetail>>& mapDetail) ;




/******************************************************************************************************************/

public: //辅助功能
	virtual void GenerateOrderInfo(const PlatformStru_InputOrder& inputOrder,PlatformStru_OrderInfo & orderInfo);
	virtual void GenerateInputOrder(const PlatformStru_OrderInfo& orderInfo,PlatformStru_InputOrder & inputOrder);
/******************************************************************************************************************/
public://内部接口

	//检测日初数据的持仓是否有过期合约
	bool CheckInitDataHasExpiredInstruementPosition();
	//处理发送查询费率和保证金率
	void SendQryCommissionAndMargin();

	void GenerateOrderInfoFromCTPInputOrder(const CThostFtdcInputOrderField& inputOrder,
		PlatformStru_OrderInfo & orderInfo);

	//获取交易日后，进行的交易初始化动作
	virtual void TradeDataInitAfterGetTradingDay() ;
	
	//没有获取交易日后，进行的交易初始化动作
	virtual void TradeDataInitAfterNoTradingDay() ;

	//检查每个用户是否都做完了动作，切入到交易状态
	virtual bool CheckAllUserFinishedInit();


	//检查每个用户和主服务是否同步完成
	virtual bool CheckAllUserAccountSyn();

	//检查每个用户ctp登陆情况
	virtual bool CheckAllUserFinishedCTP(int & nLoginCount);

	//结算调用，以那一天的数据进行数据库初始化交易数据
	virtual CF_ERROR SettlementInitTradeDataWithTradingDB(eSettlementLoadOption nOption,const std::string & nsDay) ;


	virtual _MapUserTradeData* GetMapUserDataWithOption(eSettlementLoadOption nOption);
	//设置交易数据管理模块状态
	virtual void SetTraderDataStatus(eTradeDataStatus neStatus) ;

	////根据输入单生成报单
	//static void GenerateOrderInfo(const CThostFtdcInputOrderField& inputOrder,
	//	PlatformStru_OrderInfo & orderInfo);

	//将费率对象分派给各个用户的对象
	virtual void SetRatesObjectToAccountRelationUserObject(const std::string & nsBrokerId,
        const std::string & nAccountID,
		const std::string & nUserID);

	//将合约对象分派给各个用户的对象
	virtual void DispathRateToAllUser(_MapUserTradeData & mapUserTradeData);


	//将合约对象分派给各个用户的对象
	virtual void SetPlatformObjectToAllUser(_MapUserTradeData & mapUserTradeData);

	//将合约对象分派给各个主账户的对象
	virtual void SetPlatformObjectToAllAccount();


	//创建空的子对象并加入map
	virtual void CreateEmptyAccountOpAndAddMap(const string & nUserId,_MapUserTradeData & nmapUserData);

	//分派用户的初始数据到交易数据中
	virtual void DispathInitialDataToUserWithAccount(const std::string & nBrokerID, 
		const std::string & nAccountID,
		_MapUserTradeData & nmapUserTradeData,
        const std::string & nUserID);

	//分派用户的初始数据到交易数据中
	virtual void DispathInitialDataToUser(const std::string & nUserId, 
		 const std::string & nsInstruemntDay,
		_MapUserTradeData & nmapUserTradeData);


	//分派今天的出入金给用户对象
	virtual void DispathFundInOut(const std::string & nsDate,_MapUserTradeData & nmapUserTradeData);

	//装载某一天所有用户的的日初数据
	virtual CF_ERROR LoadInitData(PlatformStru_TradingAccountInfo& inData, const std::string & nsDate,const std::string & szUser);

	//从结算数据库里面装置日初的持仓
	virtual CF_ERROR LoadInitialPositionFromDB(const std::string & nUserName,
		const std::string & nsTime,
		vector<PlatformStru_Position> & nVecPosition);

	//从结算数据库里面装置日初的持仓明细
	virtual CF_ERROR LoadInitialPositionDetailFromDB(const std::string & nUserName,
		const std::string & nsTime,
		vector<PlatformStru_PositionDetail> & nVecPositionDetail);

	//从结算数据库里面装置日初的资金
	virtual CF_ERROR LoadInitialFundFromDB(const std::string & nUserName,
		const std::string & nsTime,
		PlatformStru_TradingAccountInfo & nFundIndo);

	////从初始权益表里面表里面转载初始资金
	virtual CF_ERROR LoadInitialInitFundFromDB(const std::string & nUserName,		
		double & nFundIndo);
	

	//初始化用户对象
	virtual CF_ERROR InitUserMapWithEmpty(string szUser,string szBrokerCode,string szTradeAccount,_MapUserTradeData & nmapUserData);
	//全部数据重置
	virtual CF_ERROR ResetAll();

	//盘中数据落地
	virtual CF_ERROR SaveAllTradingData(bool nbInitFirst=false);

	//定时存储
	virtual CF_ERROR TimerSaveTradingData(bool bTimer = true);

	//推送一个事件用户相关的账户为参数
	virtual CF_ERROR PushEventToAccountWithUser(eEventType nEve,
		const std::string & nUserName);


	//测试一个用户是否有持仓
	virtual bool TestUserHasPosition(const std::string & nUserName);

	//盘中新增加一个交易员，其委托交易账号也已查完费率
	virtual CF_ERROR AddTraderUserWhenTrading(const std::string & nUserName,
		const std::string & nsAccountName,
		const std::string & nBrokerCode);
	
	//结算时将所有的未成交，部分成交，待审核的报单全部改为已撤单
	virtual CF_ERROR CancelAllUnkonwnOrder();

	//测试一个用户是否有该成交
	virtual bool TestUserTraderExist(const PlatformStru_TradeInfo & nTraderInfo);


	//从服务盘中新增加一个交易员，其委托交易账号也已查完费率
	virtual CF_ERROR AddInitTraderUser(const std::string & nUserName,
		const std::string & nsAccountName,
		const std::string & nBrokerCode);

private:
	static CReal_SvrTradeData    *m_pObj;
//     IInterfaceDBOpr * mpDBOpr;   //数据库中间层接口
	std::string   msCurrentTradingDay; //年-月-日  
	Ceasymutex mMutexTradingday;

	eTradeDataStatus meTraderDataStatus;
	eExchangeStatus  meExchangeStatus;


	bool mbForceInit; //初始化的过程中强制查询合约和费率
private: //数据对象	
	



	std::string msUserInitDay;  //日初数据是哪一天的
	std::map<std::string,std::vector<PlatformStru_Position>>       mUserInitPosition;       //日初持仓
	std::map<std::string,std::vector<PlatformStru_PositionDetail>> mUserInitPositionDeatil; //日初持仓明细
	std::map<std::string,PlatformStru_TradingAccountInfo>          mUserIniFundInfo;        //日初资金
	std::map<std::string,double>                                   mUserInitInitFund;        //初始资金
	Ceasymutex                                                     m_mutexInitData;     //保护日初数据的变量


	std::string msInstrumentDay;                              //合约和费率数据是哪一天的
	CPlatformDataMgr_Instruments *mpPlatformDataInstrument;   //合约
	eDataReady                   mbInstrumentsReady;          //合约数据准备好了


	CPlatformDataMgr_Quots       *mpPlatformDataQuots;                        //行情
	std::map<std::string,PlatformStru_DepthMarketData> mmapSettlementQuot;    //结算行情
	Ceasymutex mMutextSettlementQuot;


	_MapAccountData              mMapAccountData;             //所有委托交易账号的数据（费率，状态）
	rwlock_t                     mrwLockMapAccount;           //保护委托交易账号数据对象的锁


	
	_MapUserTradeData            mMapUser2TradingData;        //所有的子账户盘中交易数据对象
	bool                         mbDirty;                     //盘中交易数据对象由于结算的修改而变脏
  	rwlock_t                     mrwLockMapUserTrading;       //保护子账户盘中交易数据对象的读写锁 


	//_MapUserTradeData            mMapUser2TradedData;               //所有的子账户结算对象
	//Ceasymutex                   m_mutexMapUserTraded;              //保护子账户结算数据对象的锁 

	//_MapUserTradeData            mMapUser2TradedDataWithCTP;         //所有的子账户从盘中数据库结算数据对象
	//eDataReady                   meUserTradedDataWithCTPReady;       //结算交易数据准备好了
	//Ceasymutex                   m_mutexMapUserTraderWithCTP;        //保护子账户结算数据对象的锁 




	CTradingDayManager           mpTradingDayManager;                //交易日管理     



	//数据库存储的对象
	CBufferDataStore<PlatformStru_TradeInfo> mBufferDataStoreTrade;
	CBufferDataStore<PlatformStru_OrderInfo> mBufferDataStoreOrder;
	CBufferDataStore<PlatformStru_TradeInfo> mBufferDataStoreCTPTrade;
	CBufferDataStore<PlatformStru_OrderInfo> mBufferDataStoreCTPOrder;
	CBufferDataStore<PlatformStru_OrderInfo> mBufferDataStoreExecuteInOrder;
	CBufferDataStore<PlatformStru_OrderInfo> mBufferDataStoreExecuteOutOrder;


	//日志记录
	CWriteLog *m_pWriteLog;


private:
//用户主从服务同步的参数
	volatile int mMasterSocket; //主服务的socket
public:
	void SetMasterSocket(int nVal) {mMasterSocket = nVal;}
	int GetMasterSocket() { return mMasterSocket;}
    bool SlaverInitAfterGetTradingDay(std::string szUser,std::string szBrokerCode,std::string szTradeAccount,CDataEntity* pDataEntity, CMemCache* pCmdCache, double dFund );
	void SetUserSynFinished(const std::string & nsUserName);
	bool GetUserSynFinished(const std::string & nsUserName);
	void SetAccountSynFinished(const BrokerAccountKey & nKey);

public:
// 	bool NeedCache() {return mbNeedCache;}
// 	bool SubscribeCacheChannel(){	return mSubCache.SubscibleRead(&CReal_SvrTradeData::AddCommandCallback);}
	bool CacheErrOrderInsert(const PlatformStru_OrderInfo & lOrder) {return mpCache->PushOrderInputError(lOrder);}
	bool CacheErrOrderAction(const CThostFtdcOrderActionField & lOrderAction) {return mpCache->PushOrderActionError(lOrderAction);}

	static void AddCommandCallback(const std::string & nsCommand);
	void AddCommand(const std::string & nsCommand);
	void DealCommand();
	
public:	
	bool PushStrategyStopOrRun(const std::string & nsName,bool nsStop) {return mpCache->PushStrategyStopOrRun(nsName,nsStop);};

	bool PushTraderUseroffLine(const std::string & nUserName,bool bOffline) {return mpCache->PushTraderUseroffLine(nUserName,bOffline);} ;

	bool PushStrategyInstanceStopOrRun(const std::string & nsUserName,
		const std::string & nsStategyName,
		bool nsStop) 
	{
			return mpCache->PushStrategyInstanceStopOrRun(nsUserName,nsStategyName,nsStop);
	}

	virtual bool GetStategyUserInstanceUpdateTime(const std::string & nsUserName,
		const std::string & nsStategyName,
		time_t & nsUpdateTime) ;
	
private:
 	CMemCache* mpCache;
	bool mbNeedCache;
	std::string mCacheHostIP;
	int mCacheHostPort;
//  	CMemCache mSubCache;
	Ceasymutex mCommandMutex;
	std::queue<std::string> mCommandList;
	Ceasymutex mUpdateTimeMapMutex;
	std::map<std::string ,time_t> mStrategyInstanceUpdateTimeMap;
	

};



