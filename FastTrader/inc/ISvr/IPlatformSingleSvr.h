#pragma once
#include <string>
#include <set>
#include <map>
#include <vector>
using std::string;
using std::map;
using std::vector;
using std::set;

#include "platformID.h"
#include "Module-Misc2/BusinessData.hpp"

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
	UIOT_PANE_STANDARD,				// 标准下单面板
	UIOT_PANE_BATCH,				// 批量下单面板
	UIOT_PANE_CONDITION,			// 条件单下单面板
	UIOT_PANE_FAKFOK,				// Fak/fok下单面板
	UIOT_PANE_PROFIT,				// 盈损单下单面板
	UIOT_PANE_STOPLIMIT,			// 停止单下单面板
	UIOT_CMD_REPLACEORDER,			// pats改单菜单项
	UIOT_CMD_SIMULATEMARKET,		// 模拟市价功能
	UIOT_CMD_PARKED,				// 预埋单按钮
	UIOT_ITEM_HEDGE,				// 下单板保值单项
	UIOT_ITEM_OPENCLOSE,			// 下单板开平项
	UIOT_ITEM_ACCOUNT,				// 下单板帐号项
	UIOT_ITEM_CONDITION_LOCAL,		// 本地条件单控制项
	UIOT_ITEM_CONDITION_SERVER,		// 服务器条件单控制项
	UIOT_ITEM_PARKED_LOCAL,			// 本地预埋单控制项
	UIOT_ITEM_PARKED_SERVER,		// 服务器预埋单控制项
	UIOT_ITEM_PROFIT_LOCAL,			// 本地预埋单控制项
	UIOT_ITEM_PROFIT_SERVER,		// 服务器预埋单控制项
};
//代理服务器的参数结构
struct Stru_ProxyServerParam
{
    Stru_ProxyServerParam()
    {
        dwProxyIP=0;
        usProxyPort=0;
    };
    Stru_ProxyServerParam(const Stru_ProxyServerParam& r)
    {
        *this=r;
    }
	Stru_ProxyServerParam& operator= (const Stru_ProxyServerParam& r)
    {
        ProxyType=r.ProxyType;
        ProxyUser=r.ProxyUser;
        ProxyPassword=r.ProxyPassword;
        ProxyIP=r.ProxyIP;
        ProxyPort=r.ProxyPort;
        dwProxyIP=r.dwProxyIP;
        usProxyPort=r.usProxyPort;
        return *this;
    };
    void clear(void)
    {
        ProxyType.clear();
        ProxyUser.clear();
        ProxyPassword.clear();
        ProxyIP.clear();
        ProxyPort.clear();
        dwProxyIP=0;
        usProxyPort=0;
    };

    string ProxyType;               //socks4,socks4a,socks5,http
    string ProxyUser;
    string ProxyPassword;
    string ProxyIP;
    string ProxyPort;
    unsigned int dwProxyIP;
    unsigned short usProxyPort;
};
class IPlatformSingleSvr
{
public:
    IPlatformSingleSvr(const string& strAccount,int PlatformID,int MaxPlatformID) : m_Account(strAccount),m_PlatformID(PlatformID),m_MaxPlatformID(MaxPlatformID) {}

   ///启动平台
    virtual int Start()=0;

    ///关闭平台
    virtual int Stop()=0;
    ///关闭平台中的线程
    virtual int StopThread()=0;
	///密码是否是当前密码
	virtual bool IsValidInvestorPW(const std::string& yourPW)=0;
	virtual bool IsValidUserPW(const std::string& yourPW)=0;
	virtual int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts=NULL)=0;
    virtual string GetAccount()=0;
    ///设置交易前置机地址
    virtual int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr)=0;

    ///设置行情前置机地址
    virtual int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr)=0;

    ///设置经纪公司参数信息
    virtual int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword)=0;
	//设置代理服务器参数
	virtual int SetProxyParam(const Stru_ProxyServerParam& ProxyParam)=0;
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
    ///DataCenter是否已经和底层同步上，可以开始初始化查询了
    virtual bool IsDataCenterReady(void)=0;

    ///获取合约列表
    virtual int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="")=0;
    virtual int GetInstrumentList(vector<string> &outData,string ExchangeID="")=0;
	virtual int GetInstrumentList(std::vector<InstrumentGroupInfo> &outData)=0;
    ///添加主力合约列表
    virtual void AddMainInstrumentList(std::string instrument)=0;
    ///获取主力合约列表
    virtual int GetMainInstrumentList(std::set<std::string> &outData)=0;
	// 设置合约容差列表
	virtual void SetInstrumentVarietyMap(map<string, string>& inData)=0;
	// 获取合约容差列表
	virtual int GetInstrumentVarietyMap(map<string, string>& outData)=0;
    //设置合约订阅状态
	virtual void SetSubscribeStatus(const std::string& InstrumentID,int GID,SubscribeMarketDataOper oper=eSubscribeMarketData)=0;
	//退订GID对应的全部合约行情
	virtual void UnsubscribeMarketDataOfGID(int GID)=0;
	//检查GID是否订阅了指定合约的行情
	virtual bool IsSubscribeMarketDataOfGID(const string& InstrumentID,int GID)=0;
    ///获取指定合约信息
    virtual int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData)=0;
	virtual bool GetLegInstrumentID(const string& strComInstrumentID,
                                    string& strLeg1InstrumentID,
                                    string& strLeg2InstrumentID)=0;
    ///设置指定合约信息
    virtual void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData,bool bLast)=0;
    ///获取合约的产品类型，失败返回-1
    virtual char GetProductClassType(const std::string& InstrumentID)=0;
    ///获取合约的ProductID
    virtual string GetProductID(const string& strInstrumentID)=0;
    virtual string GetExchangeID(const string& strInstrumentID)=0;

    ///设置指定品种信息
    virtual void SetProductInfo(const string& ProductID, PlatformStru_ProductInfo& outData)=0;
    ///获取品种列表
    virtual int GetProductList(vector<string> &outData,const string& ExchangeID="")=0;

    ///获取指定交易所的所有品种-合约
    virtual int GetProductID_InstrumentIDsByExchangeID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="")=0;    
    ///获取全部交易所的品种合约ID vector<pair<ExchangeID,vector<pair<ProductID,vector<InstrumentID>>>>>
    virtual void GetExchangeID_ProductID_InstrumentIDs(vector<pair<string,vector<pair<string,vector<string>>>>>& outData)=0;
    ///获取全部交易所的品种信息 vector<pair<ExchangeID,vector<PlatformStru_ProductInfo>>>
    virtual void GetExchangeID_ProductInfos(vector<pair<string,vector<PlatformStru_ProductInfo>>>& outData)=0;
    virtual bool GetProductInfo(const string& strInstrumentID,PlatformStru_ProductInfo& outData)=0;
    virtual bool GetProductInfo2(const string& strProductID,PlatformStru_ProductInfo& outData)=0;
    //获取指定交易所的期权标的物期货品种
    virtual int GetOptionProductIDsByExchangeID(const string& ExchangeID,vector<string>& outProductIDs)=0;
    //获取指定标的物期货合约对应的期权合约ID，返回目标期权合约的数量。UnderlyingInstrumentID为空则返回全部期权合约
    virtual int GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outOptionInstrumentIDs)=0;
    //获取指定标的物期货合约对应的看涨期权合约ID数组和看跌期权合约ID数组。UnderlyingInstrumentID为空则返回全部期权合约
    virtual void GetOptionInstrumentIDsByUnderlyingInstrumentID(const string& UnderlyingInstrumentID,vector<string>& outCallOptionInstrumentIDs,vector<string>& outPutOptionInstrumentIDs)=0;
    //获取所有标的物期货合约
    virtual int GetAllOptionUnderlyingInstrumentIDs(vector<string>& outUnderlyingInstrumentIDs)=0;
    virtual int GetAllOptionUnderlyingInstrumentIDs(set<string>& outUnderlyingInstrumentIDs)=0;
    //判断是否是期权标的物期货合约
    virtual bool IsUnderlyingInstrumentID(const string& InstrumentID)=0;

    ///获取合约手续费率
    virtual int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)=0;
    virtual bool IsCommissionRateExist(const string& InstrumentID)=0;
    ///获取合约手续费率，如果没有，则触发查询
    virtual int GetCommissionRate_TriggerReq(const std::string& InstrumentID)=0;
    ///设置合约手续费率
    virtual int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData)=0;  
    ///获取合约保证金率
    virtual int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)=0;
    virtual bool IsMarginRateExist(const string& InstrumentID)=0;
    ///获取合约保证金率，如果没有，则触发查询
    virtual int GetMarginRate_TriggerReq(const std::string& InstrumentID)=0;
    ///设置合约保证金率
    virtual int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData)=0;     
    ///获取合约乘数，成功返回合约乘数，失败返回-1
    virtual int GetInstrumentMultiple(const std::string& InstrumentID)=0;
	///获取指定品种的合约列表,返回合约个数
	virtual int GetInstrumentListByProductID(const string& ProductID,set<string> &outData)=0;    
	virtual int GetInstrumentListByProductID(const string& ProductID,vector<string> &outData)=0;    
    
    ///获取指定合约行情。0表示成功
    virtual int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)=0;
    ///获取指定合约旧行情
    virtual int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData)=0;
    ///获取指定合约的现价，失败返回无效值
    virtual double GetCurPrice(const std::string& InstrumentID)=0;

    //获取交易所交易日期
    virtual string GetTradingDay()=0;
	virtual bool IsTodayPosition( const char* pOpenData ) = 0;
    ///获取交易所时间
    virtual unsigned long GetExchangeTime(const string& ExchangeID,string& time)=0;
    ///获取交易所日内交易时段, 返回交易时段起始时间对的数组，如{pair("0915","1130"),pair("1300","1515")}等
    virtual vector<pair<string,string>> GetTradingTimespan(const string& ExchangeID)=0;
    ///获取合约日内交易时段, 返回交易时段起始时间对的数组，如{pair("0915","1130"),pair("1300","1515")}等
    virtual vector<pair<string,string>> GetTradingTimespan2(const string& InstrumentID)=0;
    ///获取全部交易所
    virtual int GetExchangeIDs(vector<string>& outData)=0;
    ///获取交易资金账户信息
	virtual int GetTradingAccountWithdrawQuota(double& fWithdrawQuota)=0;
	virtual int GetTradingAccountAvailable(double& fAvailable)=0;
	virtual int GetTradingAccountID(char* AccountID,int rltsize)=0;
	virtual int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData)=0;
	///获取账户资金文本信息
	virtual int GetAccountText(std::string& outData,int language)=0;
    ///请求查询投资者结算结果响应
    virtual int GetLastSettlementInfo(std::string& outData)=0;


    ///获取指定合约的持仓明细     add zhifa.gao	
    virtual bool HavePositionDetail(const std::string& strInstrumentID)=0;
    virtual bool HaveCombPositionDetail()=0;
    virtual void GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs)=0;


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
	virtual int GetPositionCombs2(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData)=0;

    virtual int GetPositions(std::vector<PlatformStru_Position>& outData)=0;
    virtual int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq)=0;
    virtual int GetPositionCombs(std::vector<PlatformStru_Position>& outData)=0;

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
    virtual bool GetOrder2(const string& strOrderSysID,PlatformStru_OrderInfo& outData)=0;

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
    virtual int GetWaitOrders3(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取指定合约相关的未成交报单, 不包含手工审核中的报单，合约是单合约，报单是对应报单，或包含该合约的组合报单
    virtual int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData)=0;
    ///获取指定未成交报单
    virtual bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData)=0;

    ///获取全部成交单
    virtual int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData)=0;
    ///获取指定成交信息
    virtual bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData)=0;
    ///获取指定成交信息
    virtual int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData)=0;
    virtual int GetTradeBriefsOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfoBrief>& outData)=0;
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
	
    //获取币种信息
    virtual vector<string> GetCurrencys(const string& BankName,bool bAddUSD=true,bool bAddCNY=true)=0;
    virtual vector<string> GetCurrencys(bool bAddUSD=true,bool bAddCNY=true)=0;

	// 读取合约的简称表结构，so as "CU,铜,T"
	virtual void GetContractShortNames(string& strShortNames)=0;
	//
	virtual string GetVersionIconString()=0;
	//
	virtual bool SupportPlugins()=0;
	//virtual bool IsMultipleAccounts()=0;
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

    ///报单录入请求. Ctp: 返回时pInputOrder中的RrequestID、BrokerID、InvestorID、UserID、OrderRef的值被修改为底层确定的值
   virtual  int ReqOrderInsert(PlatformStru_InputOrder &pInputOrder, int nRequestID=0)=0;

    ///预埋单录入请求. Ctp: 返回时pInputOrder中的RrequestID、BrokerID、InvestorID、UserID、OrderRef的值被修改为底层确定的值
   virtual  int ReqParkedOrderInsert(PlatformStru_ParkedOrder &pParkedOrder, int nRequestID=0)=0;

    ///预埋撤单录入请求
   virtual  int ReqParkedOrderAction(PlatformStru_ParkedOrderAction &pParkedOrderAction, int nRequestID=0)=0;

   ///报单操作请求(含撤单)
   virtual  int ReqOrderAction(PlatformStru_InputOrderAction &pInputOrderAction, int nRequestID=0)=0;
   ///报单操作请求(撤单)
   virtual  int ReqCancelOrder(const OrderKey& orderkey, int nRequestID=0)=0;

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

   //针对ctp有效：
    //清除查询请求队列中指定类型的查询请求，用于TCP重连后的重查前的处理
    //ReqCmdID:1:资金查询；2:成交查询；4:报单查询；8:持仓明细查询；16:持仓查询；32:组合持仓明细查询
    //ReqCmdID可以或处理
    virtual void ClearReqInQryQueue(int ReqCmdID)=0;

    ///请求查询投资者
   virtual  int ReqQryInvestor(PlatformStru_QryInvestor &pQryInvestor, int nRequestID=0) =0;


    ///请求查询品种
   virtual  int ReqQryProduct(PlatformStru_QryProduct &pQryProduct, int nRequestID=0) =0;

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

    ///请求查询汇率
    virtual int ReqQryExchangeRate(PlatformStru_QryExchangeRate &QryExchangeRate, int nRequestID=0)=0;

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

    ///订阅询价通知，OptionInstrumentIDs为空则订阅全部期权合约的询价通知
   virtual int  SubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs) = 0;
    ///退订询价通知，OptionInstrumentIDs为空则退订全部合约的询价通知
   virtual int UnSubscribeForQuoteRsp(const vector<string>& OptionInstrumentIDs) = 0;
    ///请求询价录入
    virtual int ReqForQuoteInsert(PlatformStru_InputForQuoteField& InputForQuote, int nRequestID)=0;
    ///执行宣告录入请求
	virtual int ReqExecOrderInsert(PlatformStru_InputExecOrderField& InputExecOrder, int nRequestID)=0;
    ///执行宣告操作请求
	virtual int ReqExecOrderAction(PlatformStru_InputExecOrderActionField& InputExecOrderAction, int nRequestID)=0;
    ///请求查询执行宣告
    virtual int ReqQryExecOrder(PlatformStru_QryExecOrderField& QryExecOrder, int nRequestID)=0;

   //重新连接
	virtual void ReConnetServer()=0;
	//获取报告
	virtual int GetReports(map<DWORD, CTPReport>& mapReport)=0;
	//获取货币汇率
	virtual double GetExchangeRate(char* currency)=0;

    virtual void NotifyInitQryStart()=0;
    virtual void NotifyInitQrySucc()=0;
    virtual void NotifyInitQryFail()=0;
    virtual bool IsInitQrySucc()=0;

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
	virtual int GetPositionCombInfo(long lFTID, PlatformStru_Position& outData)=0;


    //获取交易连接的FrontID和SessionID。这两个值在交易登录时由ctp返回
    virtual const PlatformStru_LoginInfo& GetTradingLoginInfo()=0;

	//获取平台一个合约组最大的合约数量
	virtual int GetGroupMaxContractNum()=0;
	//合约数量合适不需要特别处理
	virtual bool ContractCountIsFit()=0;

	virtual int  GetPlatformType()=0;
	//virtual bool GetQryFinished()=0;
	//virtual void SetQryFinished(bool bFinished)=0;

public:
    static IPlatformSingleSvr* Create(const string& strAccount,ePLATFORMTYPE type,int PlatformID=0,int MaxPlatformID=1);
	static void Release(IPlatformSingleSvr* pSingleSvr);

    int GetPlatformID(void) { return m_PlatformID; }

public:
    string      m_Account;

    //每个底层平台有一个唯一的PlatformID，创建时由PlatformSvrMgr设置
    const int   m_PlatformID;
    const int   m_MaxPlatformID;

    //服务即将停止。停止服务时，先设置该标志，然后发送wxEVT_MA_PLAT_DELETE事件，再停止服务。业务层在处理wxEVT_MA_PLAT_DELETE事件时获取已登录服务，要排除该服务
    bool        m_bWillStop;
};