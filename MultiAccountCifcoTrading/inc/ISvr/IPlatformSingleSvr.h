#pragma once
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
using namespace std;
#include "platformID.h"
#include "BusinessData.hpp"
#include "PlatformStruct.h"
enum ServiceStatus
{
    SS_NOLOGIN,            //还未登入
    SS_LOGINING,           //正在登入中
    SS_LOGINFAILED,        //登入失败
    SS_LOGINED,            //登入成功
    SS_LOSTCONN//,           //失去连接，内部会自动重连
    //SS_LOGOUT              //已登出
};
enum UIOrderType
{
	UIOT_STANDARD,
	UIOT_BATCH,
	UIOT_CONDITION,
	UIOT_PROFIT,
	UIOT_HEDGE,
	UIOT_STOPLIMIT,
	UIOT_REPLACEORDER,
	UIOT_SIMULATEMARKET,
	UIOT_OPENCLOSE, 
	UIOT_ACCOUNT
};
class IPlatformSingleSvr
{
public:
	virtual PTYPE GetPlatformType()=0;
   ///启动平台
    virtual int Start()=0;

    ///关闭平台
    virtual int Stop()=0;
    ///关闭平台中的线程
    virtual int StopThread()=0;
	///密码是否是当前密码
	///1：是，其他：不是
	virtual int IsValidPW(const std::string& yourPW)=0;
	virtual int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts=NULL)=0;
    virtual string GetAccount()=0;
	///设置交易前置机地址
    virtual int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)=0;

    ///设置行情前置机地址
    virtual int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)=0;

    ///设置经纪公司参数信息
    virtual int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)=0;
	//设置代理服务器字符串
	virtual int SetProxyConnectStr(const std::string& str)=0;

    ///登入
    virtual int Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd="")=0;

    ///登出
    virtual int Logout()=0;

    ///定制业务数据
    virtual int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false)=0;

    ///取消业务数据的定制
    virtual int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID)=0;

    ///获取底层服务的当前状态
    virtual ServiceStatus GetCurrentStatus(int* pQuotStatus=NULL,int* pTradeStatus=NULL)=0;

    ///获取合约列表
    virtual int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="")=0;
	virtual int GetInstrumentList(std::vector<GroupInfo> &outData)=0;
    ///添加主力合约列表
    virtual int AddMainInstrumentList(std::string instrument)=0;
    ///获取主力合约列表
    virtual int GetMainInstrumentList(std::set<std::string> &outData)=0;
	// 设置合约容差列表
	virtual void SetInstrumentVarietyMap(map<string, string>& inData)=0;
	// 获取合约容差列表
	virtual int GetInstrumentVarietyMap(map<string, string>& outData)=0;
    //设置合约订阅状态
	virtual void SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper=S_STATUS_ADD)=0;
    ///获取指定合约信息
    virtual int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)=0;
    ///设置指定合约信息
    virtual void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)=0;
    ///获取合约的产品类型，失败返回-1
    virtual char GetProductClassType(const std::string& InstrumentID)=0;

    ///获取合约品种代码
    virtual int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="")=0;    

    ///获取合约手续费率
    virtual int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)=0;
    ///获取合约手续费率，如果没有，则触发查询
    virtual int GetCommissionRate_TriggerReq(const std::string& InstrumentID)=0;
    ///设置合约手续费率
    virtual int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)=0;  
    ///获取合约保证金率
    virtual int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)=0;
    ///获取合约保证金率，如果没有，则触发查询
    virtual int GetMarginRate_TriggerReq(const std::string& InstrumentID)=0;
    ///设置合约保证金率
    virtual int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)=0;     
    ///获取合约乘数，成功返回合约乘数，失败返回-1
    virtual int GetInstrumentMultiple(const std::string& InstrumentID)=0;
	///获取指定品种的合约列表,返回合约个数
	virtual int GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData)=0;    
    
    ///获取指定合约行情
    virtual int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)=0;
    ///获取指定合约旧行情
    virtual int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)=0;
    ///获取指定合约的现价，失败返回0.0
    virtual double GetCurPrice(const std::string& InstrumentID)=0;

    //获取交易所交易日期
    virtual string GetTradingDay(const string& ExchangeID)=0;
	virtual bool IsTodayPosition( const char* pOpenData ) = 0;
    ///获取交易所时间
    virtual unsigned long GetExchangeTime(const string& ExchangeID,string& time)=0;
    ///获取全部交易所
    virtual int GetExchangeIDs(vector<string>& outData)=0;
    ///获取交易资金账户信息
	virtual int GetTradingAccountWithdrawQuota(double& fWithdrawQuota)=0;
	virtual int GetTradingAccountAvailable(double& fAvailable)=0;
	virtual int GetTradingAccountID(char* AccountID)=0;
	virtual int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)=0;
	///获取账户资金文本信息
	virtual int GetAccountText(std::string& outData,int language)=0;
    ///设置交易资金账户信息
    virtual void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData)=0;
    ///请求查询投资者结算结果响应
    virtual int GetLastSettlementInfo(std::string& outData)=0;


    ///获取指定合约的持仓明细     add zhifa.gao	
    virtual bool HavePositionDetail(const std::string& strInstrumentID)=0;

    virtual int GetPositions2(const std::string& strInstrument,
                                std::set<long>& setFTID,
                                std::vector<PlatformStru_Position>& outData,
                                long& lastseq)=0;
    virtual int GetPositions3(const std::string& strInstrument,
                                std::vector<PlatformStru_Position>& outData,
                                long& lastseq)=0;
    virtual int GetPositionDetails3(const std::string& strInstrument,
                                      std::set<long>& setFTID,
                                      std::vector<PlatformStru_PositionDetail>& vecValue,
                                      long& lastseq)=0;
	virtual int GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData)=0;

    virtual int GetPositions(std::vector<PlatformStru_Position>& outData)=0;
    virtual int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)=0;
    virtual int GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData)=0;

    //获取持仓记录键值列表,返回持仓记录的数量
    virtual int GetPositionKeySet(std::set<PositionKey> &PositionKeySet)=0;
    //获取指定账号、合约、方向、投保的持仓数据，成功返回0，失败返回-1
    virtual int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData)=0;

    //获取挂单数量
    virtual int  GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag)=0;

    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    virtual int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag)=0;

    ///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
    virtual void SetCurrentPositionContent(int PositionContentMode)=0;


    ///获取全部报单
    virtual int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取全部报单
    virtual int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取指定报单
    virtual bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData)=0;

    ///获取全部已触发的报单
    virtual int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取合约相关的已触发的报单
    virtual int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取指定已触发的报单
    virtual bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///获取已成交报单
    virtual int GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取已成交报单
    virtual int GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取指定已成交报单
    virtual bool GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///获取已撤单和错误报单
    virtual int GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取已撤单和错误报单
    virtual int GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取指定已撤单和错误报单
    virtual bool GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///获取未成交报单
    virtual int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取未成交报单
    virtual int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取指定未成交报单
    virtual bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///获取全部成交单
    virtual int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)=0;
    ///获取指定成交信息
    virtual bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)=0;
    ///获取指定成交信息
    virtual int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)=0;
    ///获取全部成交统计记录
    virtual int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData)=0;
    ///获取指定合约的成交统计记录成功返回0，失败返回-1
    virtual int GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData)=0;
    ///获取成交统计
    virtual int GetAllTradeTotalDatas(vector<TotalInfo>& outData)=0;
	///在成交统计查找参数rawData
	virtual int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData )=0;


	virtual bool IsSettlementInfoConfirmed()=0;
	virtual void GetErrorField(int& ErrorID)=0;
	virtual void GetErrorField(string& ErrMsg)=0;
	virtual void SetReqID(int nOrderReqID)=0; 
	virtual long GetReqID()=0; 
	virtual long GetNextReqID()=0;
	virtual int  GetAccountList(std::vector<string>& AcctList)=0;
	virtual void FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec )=0;

	///交易所ID
	virtual const string ExchangeID2String(const string& ExchangeID) = 0;
	//根据合约组获得交易所ID，如果是非系统合约组则返回空
	virtual const string ExchangeName2ID(const string& name) = 0;
	//获取服务器参数
	virtual SERVER_PARAM_CFG& GetServerParamCfg()=0;
	//判断列表字段在该平台是否有效
	virtual bool IsFieldValid( int nModuleId, int nFieldId ) = 0;
	virtual bool IsModuleValid( int nModuleId )=0;
	//判断是否支持保值
	virtual bool HaveOrderType(UIOrderType type)=0;
	//判断是否支持银期转账
	virtual bool HaveBankFutureTransfer()=0;
	
	// 读取合约的简称表结构，so as "CU,铜,T"
	virtual void GetContractShortNames(string& strShortNames)=0;
	//
	virtual string GetVersionIconString()=0;
	//
	virtual bool SupportPlugins()=0;
	//是否支持多账号
	virtual bool IsMultipleAccounts()=0;
	///获取账号信息
   virtual  int GetTrader(std::vector<PlatformStru_Trader> &outData) =0;
    ///获取交易所信息
   virtual  int GetExchange(std::vector<PlatformStru_Exchange> &outData) =0;
    ///获取合约信息
   virtual  int GetContract(std::vector<PlatformStru_Contract> &outData) =0;
    ///获取商品信息
   virtual  int GetCommodity(std::vector<PlatformStru_Commodity> &outData) =0;
   ///获取报单类型信息
   virtual  int GetOrderType(std::vector<PlatformStru_OrderType> &outData) =0;
   //获取账户持仓
   virtual bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi)=0;
   virtual bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)=0;
   virtual bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi)=0;
   //判断是否支持获取账号信息
   virtual bool HaveGetTrader()=0;
   //判断是否支获取交易所信息
   virtual bool HaveGetExchange()=0;
   //判断是否支持获取合约信息
   virtual bool HaveGetContract()=0;
   //判断是否支持获取商品信息
   virtual bool HaveGetCommodity()=0;
   //判断是否支持获取报单类型信息
   virtual bool HaveGetOrderType()=0;
   //判断是否支持获取持仓信息
   virtual bool HaveGetPosition()=0;
   //判断是否支持获取报告信息
   virtual bool HaveGetReport()=0;
   //判断是否支持获取汇率信息
   virtual bool HaveGetExchangeRate()=0;
	//判断是否支持在登录时候容许修改密码
   virtual bool HaveModifyPwdOnLongon()=0;
	//判断是否支持
	virtual bool HaveShowInstumentComboList()=0;
   //获取登录状态原因
   virtual std::string GetLogonReason()=0;
   //获取Pats连接状态
   virtual void GetLinkStatus(int* pHost,int* pPrice)=0;
   //是否支持金士达配置
   virtual bool IsSunGardSupported() = 0;
   //是否支持
   virtual bool IsHistorySettlementSupported() = 0;
   //是否有资金查询报告
   virtual bool HaveAccountText()=0;
   //是否有资金查询报告
   virtual bool HaveFileCache()=0;
   //合约名和合约ID是否完全一致
   virtual bool ContractNameIsID()=0;
   //保证金是否在合约信息相关结构体中
   virtual bool MarginRatioInInstrumentInfo()=0;
   //获取客户签约银行信息
   virtual int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData)=0;
public:

    /*--------------------------------------------------------------------------------------------------------
               下面业务请求都是异步的，需定制具体业务数据，才能收到请求结果
      ---------------------------------------------------------------------------------------------------------*/
    ///用户口令更新请求
   virtual  int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate &pUserPasswordUpdate, int nRequestID=0)=0;

    ///资金账户口令更新请求
   virtual  int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate &pTradingAccountPasswordUpdate, int nRequestID=0)=0;

    ///报单录入请求
   virtual  int ReqOrderInsert(PlatformStru_InputOrder &pInputOrder, int nRequestID=0)=0;

    ///预埋单录入请求
   virtual  int ReqParkedOrderInsert(PlatformStru_ParkedOrder &pParkedOrder, int nRequestID=0)=0;

    ///预埋撤单录入请求
   virtual  int ReqParkedOrderAction(PlatformStru_ParkedOrderAction &pParkedOrderAction, int nRequestID=0)=0;

    ///报单操作请求
   virtual  int ReqOrderAction(PlatformStru_InputOrderAction &pInputOrderAction, int nRequestID=0)=0;

    ///查询最大报单数量请求
   virtual  int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &pQueryMaxOrderVolume, int nRequestID=0)=0;

    ///投资者结算结果确认
   virtual  int ReqSettlementInfoConfirm() =0;

    ///请求删除预埋单
   virtual  int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &pRemoveParkedOrder, int nRequestID=0) =0;

    ///请求删除预埋撤单
   virtual  int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &pRemoveParkedOrderAction, int nRequestID=0) =0;

    //查询队列是否为空
    virtual bool IsQryQueueEmpty(void)=0;

    ///请求查询报单
   virtual  int ReqQryOrder(PlatformStru_QryOrder &pQryOrder, int nRequestID=0) =0;

    ///请求查询成交
   virtual  int ReqQryTrade(PlatformStru_QryTrade &pQryTrade, int nRequestID=0) =0;

    ///请求查询投资者持仓
   virtual  int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &pQryInvestorPosition, int nRequestID=0) =0;

    ///请求查询资金账户
   virtual  int ReqQryTradingAccount(PlatformStru_QryTradingAccount &pQryTradingAccount, int nRequestID=0) =0;
  // virtual int  GetTradingAccount(PlatformStru_TradingAccountInfo& Data)=0;

    ///请求查询投资者
   virtual  int ReqQryInvestor(PlatformStru_QryInvestor &pQryInvestor, int nRequestID=0) =0;


    ///请求查询合约
   virtual  int ReqQryInstrument(PlatformStru_QryInstrument &pQryInstrument, int nRequestID=0) =0;

    ///请求查询行情
   virtual  int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &pQryDepthMarketData, int nRequestID=0) =0;

    ///请求查询投资者结算结果
   virtual  int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &pQrySettlementInfo, int nRequestID=0) =0;

    ///请求查询保证金监管系统经纪公司资金账户密钥
   virtual  int ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID=0)=0; 

    ///请求查询转帐银行
   virtual  int ReqQryTransferBank(PlatformStru_QryTransferBank &pQryTransferBank, int nRequestID=0) =0;

    ///请求查询投资者持仓明细
   virtual  int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &pQryInvestorPositionDetail, int nRequestID=0) =0;

    ///请求查询客户通知
   virtual  int ReqQryNotice(PlatformStru_QryNotice &pQryNotice, int nRequestID=0) =0;

    ///请求查询结算信息确认
   virtual  int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &pQrySettlementInfoConfirm, int nRequestID=0) =0;

    ///请求查询投资者持仓明细
   virtual  int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &pQryInvestorPositionCombineDetail, int nRequestID=0) =0;

    ///请求查询转帐流水
   virtual  int ReqQryTransferSerial(PlatformStru_QryTransferSerial &pQryTransferSerial, int nRequestID=0) =0;

    ///请求查询签约银行
   virtual  int ReqQryContractBank(PlatformStru_QryContractBank &pQryContractBank, int nRequestID=0) =0;
        
    ///请求查询银期签约关系
	virtual  int ReqQryAccountregister(PlatformStru_QryAccountRegister &pQryAccountregister, int nRequestID=0)=0;

    ///请求查询预埋单
   virtual  int ReqQryParkedOrder(PlatformStru_QryParkedOrder &pQryParkedOrder, int nRequestID=0) =0;

    ///请求查询预埋撤单
   virtual  int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction &pQryParkedOrderAction, int nRequestID=0) =0;


    ///期货发起银行资金转期货请求
   virtual  int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer &pReqTransfer, int nRequestID=0) =0;

    ///期货发起期货资金转银行请求
   virtual  int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer &pReqTransfer, int nRequestID=0) =0;

    ///期货发起查询银行余额请求
   virtual  int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &pReqQueryAccount, int nRequestID=0) =0;
	virtual void SetModuleDataQry(bool bQry)=0;
	virtual bool GetModuleDataQry()=0;
    //重新连接
	virtual void ReConnetServer()=0;
	//获取报告
	virtual int GetReports(map<DWORD, Report>& mapReport)=0;
	//获取货币汇率
	virtual double GetExchangeRate(char* currency)=0;

    virtual void NotifyStartStopInitQry(bool bStart)=0;

	//为插件添加以下接口
	//获取委托单列表FTID
	virtual int GetAllOrderFTIDList(std::vector<long>& vec)=0;
	//获取委托单详情
	virtual int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData)=0;
	virtual int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData)=0;

	//获取持仓列表FTID
	virtual int GetPositionFTIDList(std::vector<long>& vec)=0;
	//获取持仓详情
	virtual int GetPositionInfo(long lFTID, PlatformStru_Position& outData)=0;
	virtual int GetPositionInfo(PositionKey key, PlatformStru_Position& outData)=0;

	//获取组合持仓列表FTID
	virtual int GetPositionCombFTIDList(std::vector<long>& vec)=0;
	//获取组合持仓详情
	virtual int GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData)=0;
	//virtual int GetPositionCombInfo(PositionCombKey key, PlatformStru_PositionComb& outData)=0;
	//获取平台一个合约组最大的合约数量
	virtual int GetGroupMaxContractNum()=0;
	//合约数量合适不需要特别处理
	virtual bool ContractCountIsFit()=0;
	//
	virtual bool GetMACurrentAccount()=0;
	virtual void SetMACurrentAccount(bool bCur)=0;

	virtual bool GetQryFinished()=0;
	virtual void SetQryFinished(bool bFinished)=0;
public:
    static IPlatformSingleSvr* Create(PTYPE type);
	static void Release(IPlatformSingleSvr* pSingleSvr);
};