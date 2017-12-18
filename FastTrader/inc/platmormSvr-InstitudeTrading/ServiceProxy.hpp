#pragma once
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
#include "Module-Misc2/BusinessData.hpp"
//#include "Module-Misc2/PlatformStruct.h"
#include "ISvr/IPlatformSingleSvr.h"
#include "Module-Misc2/GlobalConfigMgr.h"
#include "QryQueue.h"
///////////////////////////////////////////////////////////////////////////
//下面API返回值0表示成功，其他表示失败
class CPlatFormService;
class zqWriteLog;
class CServiceProxy :public IPlatformSingleSvr
{

public:
    ///启动平台
    int Start();

    ///关闭平台
    int Stop();
    //关闭平台中的线程
    int StopThread();
	///密码是否是当前密码
	bool IsValidInvestorPW(const std::string& yourPW);
	bool IsValidUserPW(const std::string& yourPW);
	int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts=NULL);
    string GetAccount();
    ///设置交易前置机地址
    int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr);

    ///设置行情前置机地址
    int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr);

    ///设置经纪公司参数信息
    int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword);
	//设置代理服务器参数
	int SetProxyParam(const Stru_ProxyServerParam& ProxyParam);
    ///登入
    int Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPWD="");

    ///登出
    int Logout();

    ///定制业务数据
    int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

    ///取消业务数据的定制
    int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

    ///获取底层服务的当前状态
    ServiceStatus GetCurrentStatus(int* pQuotStatus=NULL,int* pTradeStatus=NULL);
    ///DataCenter是否已经和底层同步上，可以开始初始化查询了
    bool IsDataCenterReady(void);

    ///获取合约列表
    int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
	int GetInstrumentList(std::vector<InstrumentGroupInfo> &outData);
    ///添加主力合约列表
    void AddMainInstrumentList(std::string instrument);
    ///获取主力合约列表
    int GetMainInstrumentList(std::set<std::string> &outData);
	// 设置合约容差列表
	void SetInstrumentVarietyMap(map<string, string>& inData);
	// 获取合约容差列表
	int GetInstrumentVarietyMap(map<string, string>& outData);
    //设置合约订阅状态
	void SetSubscribeStatus(const std::string& InstrumentID,int GID,SubscribeMarketDataOper oper=eSubscribeMarketData);
	//退订GID对应的全部合约行情
	void UnsubscribeMarketDataOfGID(int GID);
	//检查GID是否订阅了指定合约的行情
	bool IsSubscribeMarketDataOfGID(const string& InstrumentID,int GID);
    ///获取指定合约信息
    int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
	bool GetLegInstrumentID(const string& strComInstrumentID,
                                    string& strLeg1InstrumentID,
                                    string& strLeg2InstrumentID);
    ///设置指定合约信息
    void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///获取合约的产品类型，失败返回-1
    char GetProductClassType(const std::string& InstrumentID);

    ///获取合约品种代码
    int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");    

    ///获取合约手续费率
    int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    ///获取合约手续费率，如果没有，则触发查询
    int GetCommissionRate_TriggerReq(const std::string& InstrumentID);
    ///设置合约手续费率
    int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
    ///获取合约保证金率
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///获取合约保证金率，如果没有，则触发查询
    int GetMarginRate_TriggerReq(const std::string& InstrumentID);
    ///设置合约保证金率
    int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);     
    ///获取合约乘数，成功返回合约乘数，失败返回-1
    int GetInstrumentMultiple(const std::string& InstrumentID);
	///获取指定品种的合约列表,返回合约个数
	int GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData);    
    
    ///获取指定合约行情
    int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///获取指定合约旧行情
    int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
    ///获取指定合约的现价，失败返回无效值
    double GetCurPrice(const std::string& InstrumentID);


    //获取交易所交易日期
    string GetTradingDay(const string& ExchangeID);
	bool IsTodayPosition( const char* pOpenData );
    ///获取交易所时间
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    ///获取全部交易所
    int GetExchangeIDs(vector<string>& outData);
    ///获取交易资金账户信息
	int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
	int GetTradingAccountAvailable(double& fAvailable);
	int GetTradingAccountID(char* AccountID,int rltsize);
    int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData);
	///获取账户资金文本信息
	int GetAccountText(std::string& outData,int language);

    ///请求查询投资者结算结果响应
    int GetLastSettlementInfo(std::string& outData);


    ///获取指定合约的持仓明细     add zhifa.gao	
    bool HavePositionDetail(const std::string& strInstrumentID);
    bool HaveCombPositionDetail();
    void GetDerivedInstrumentID_OnCloseTrade(set<string>& InstrumentIDs);

    int GetPositions2(const std::string& strInstrument,
                        std::set<long>& setFTID,
                        std::vector<PlatformStru_Position>& outData,
                        long& lastseq);
    int GetPositions3(const std::string& strInstrument,
                        std::vector<PlatformStru_Position>& outData,
                        long& lastseq);
    int GetPositionDetails3(const std::string& strInstrument,
                              std::set<long>& setFTID,
                              std::vector<PlatformStru_PositionDetail>& vecValue,
                              long& lastseq);
    int GetPositionCombs2(const std::string& strInstrument,std::vector<PlatformStru_Position>& outData);

	int GetPositions(std::vector<PlatformStru_Position>& outData);
    int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq);
    int GetPositionCombs(std::vector<PlatformStru_Position>& outData);


    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
    int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);

    //获取挂单数量
    int  GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    ///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
    void SetCurrentPositionContent(int PositionContentMode);



    ///获取全部报单
    int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取全部报单
    int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定报单
    bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);
    bool GetOrder2(const string& strOrderSysID,PlatformStru_OrderInfo& outData);

    ///获取全部已触发的报单
    int GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取合约相关的已触发的报单
    int GetTriggerOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已触发的报单
    bool GetTriggerOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);

    ///获取已成交报单
    int GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取已成交报单
    int GetTradedOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已成交报单
    bool GetTradedOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);


    ///获取已撤单和错误报单
    int GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取已撤单和错误报单
    int GetCanceledOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定已撤单和错误报单
    bool GetCanceledOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);


    ///获取未成交报单
    int GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);
    ///获取未成交报单
    int GetWaitOrders3(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定合约相关的未成交报单, 不包含手工审核中的报单，合约是单合约，报单是对应报单，或包含该合约的组合报单
    int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
    ///获取指定未成交报单
    bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);


    ///获取全部成交单
    int GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData);
    ///获取指定成交信息
    bool GetTradeInfo(const TradeKey& tradekey, PlatformStru_TradeInfo& outData);
    ///获取指定成交信息
    int GetTradesOfInstrument(const std::string& strInstrument,std::vector<PlatformStru_TradeInfo>& outData);
    ///获取全部成交统计记录
    int GetAllTradeTotals(std::vector<PlatformStru_TradeTotalInfo>& outData);
    ///获取指定合约的成交统计记录成功返回0，失败返回-1
    int GetTradeTotalOfInstrument(const std::string& strInstrument, std::vector<PlatformStru_TradeTotalInfo>& outData);
    ///获取成交统计
    int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
	///在成交统计查找参数rawData
	int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );

	bool IsSettlementInfoConfirmed();
	void GetErrorField(int& ErrorID);
	void GetErrorField(string& ErrMsg);
    //获取客户签约银行信息
    int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData);

	void SetReqID(int nOrderReqID); 
	long GetReqID(); 
	long GetNextReqID();
	int  GetAccountList(std::vector<string>& AcctList);
	void FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec );

	///交易所ID
	const string ExchangeID2String(const string& ExchangeID);
	///根据合约ID获得交易所ID
	//根据合约组获得交易所ID，如果是非系统合约组则返回空
	const string ExchangeName2ID(const string& name);
    //获取服务器参数
	SERVER_PARAM_CFG&  GetServerParamCfg();

	//判断列表字段在该平台是否有效
	bool IsFieldValid( int nModuleId, int nFieldId );
	bool IsModuleValid( int nModuleId );
	//判断是否支持保值
	bool HaveOrderType(UIOrderType type);
	//判断是否支持银期转账
	bool HaveBankFutureTransfer(){return false;}

    //获取币种信息
    vector<string> GetCurrencys(const string& BankName);

	// 读取合约的简称表结构，so as "CU,铜,t"
	void GetContractShortNames(string& strShortNames);
	//
	string GetVersionIconString();
	//是否支持插件
	bool SupportPlugins();
	//bool IsMultipleAccounts();
    ///获取账号信息
      int GetTrader(std::vector<PlatformStru_Trader> &outData);
    ///获取交易所信息
     int GetExchange(std::vector<PlatformStru_Exchange> &outData);
    ///获取合约信息
     int GetContract(std::vector<PlatformStru_Contract> &outData);
    ///获取商品信息
     int GetCommodity(std::vector<PlatformStru_Commodity> &outData);
	 ///获取报单类型信息
     int GetOrderType(std::vector<PlatformStru_OrderType> &outData);
	 //获取账户持仓
	 bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
	 bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
	 bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   //判断是否支持获取账号信息
    bool HaveGetTrader() {return false;}
   //判断是否支获取交易所信息
    bool HaveGetExchange() {return false;}
   //判断是否支持获取合约信息
    bool HaveGetContract() {return false;}
   //判断是否支持获取商品信息
    bool HaveGetCommodity() {return false;}
	//判断是否支持获取报单类型信息
    bool HaveGetOrderType() {return false;}
	//判断是否支持获取持仓信息
	bool HaveGetPosition() {return false;}
	//判断是否支持获取报告信息
	bool HaveGetReport() {return false;}
	//判断是否支持获取汇率信息
	bool HaveGetExchangeRate(){return false;}
	bool HaveShowInstumentComboList() {return true;};
	//判断是否支持在登录时候容许修改密码
    bool HaveModifyPwdOnLongon(){return false;}
    bool HaveAccountText(){return true;}
       //合约名和合约ID是否完全一致
    bool ContractNameIsID(){return false;}
       //保证金是否在合约信息相关结构体中
    bool MarginRatioInInstrumentInfo(){return false;}
public:

    /*--------------------------------------------------------------------------------------------------------
               下面业务请求都是异步的，需定制具体业务数据，才能收到请求结果
      ---------------------------------------------------------------------------------------------------------*/
    ///用户口令更新请求
    int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate &pUserPasswordUpdate, int nRequestID=0);

    ///资金账户口令更新请求
    int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate &pTradingAccountPasswordUpdate, int nRequestID=0);

    ///报单录入请求. 返回时pInputOrder中的RrequestID、BrokerID、InvestorID、UserID、OrderRef的值被修改为底层确定的值
    int ReqOrderInsert(PlatformStru_InputOrder &pInputOrder, int nRequestID=0);

    ///预埋单录入请求. 返回时pInputOrder中的RrequestID、BrokerID、InvestorID、UserID、OrderRef的值被修改为底层确定的值
    int ReqParkedOrderInsert(PlatformStru_ParkedOrder &pParkedOrder, int nRequestID=0);

    ///预埋撤单录入请求
    int ReqParkedOrderAction(PlatformStru_ParkedOrderAction &pParkedOrderAction, int nRequestID=0);

	///报单操作请求(含撤单)
    int ReqOrderAction(PlatformStru_InputOrderAction &pInputOrderAction, int nRequestID=0);
	///报单操作请求(撤单)
	virtual  int ReqCancelOrder(const OrderKey& orderkey, int nRequestID=0);

    ///查询最大报单数量请求
    int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &pQueryMaxOrderVolume, int nRequestID=0);

    ///投资者结算结果确认
    int ReqSettlementInfoConfirm() ;

    ///请求删除预埋单
    int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &pRemoveParkedOrder, int nRequestID=0) ;

    ///请求删除预埋撤单
    int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &pRemoveParkedOrderAction, int nRequestID=0) ;

    //查询队列是否为空
    bool IsQryQueueEmpty(void);


    ///请求查询报单
    int ReqQryOrder(PlatformStru_QryOrder &pQryOrder, int nRequestID=0) ;

    ///请求查询成交
    int ReqQryTrade(PlatformStru_QryTrade &pQryTrade, int nRequestID=0) ;

    ///请求查询投资者持仓
    int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &pQryInvestorPosition, int nRequestID=0) ;

    ///请求查询资金账户
    int ReqQryTradingAccount(PlatformStru_QryTradingAccount &pQryTradingAccount, int nRequestID=0) ;

    //清除查询请求队列中指定类型的查询请求，用于TCP重连后的重查前的处理
    //ReqCmdID:1:资金查询；2:成交查询；4:报单查询；8:持仓明细查询；16:持仓查询；32:组合持仓明细查询
    //ReqCmdID可以或处理
    void ClearReqInQryQueue(int ReqCmdID);

    ///请求查询投资者
    int ReqQryInvestor(PlatformStru_QryInvestor &pQryInvestor, int nRequestID=0) ;


    ///请求查询合约
    int ReqQryInstrument(PlatformStru_QryInstrument &pQryInstrument, int nRequestID=0) ;

    ///请求查询行情
    int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &pQryDepthMarketData, int nRequestID=0) ;

    ///请求查询投资者结算结果
    int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &pQrySettlementInfo, int nRequestID=0) ;

    ///请求查询保证金监管系统经纪公司资金账户密钥
    int ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey &QryCFMMCTradingAccountKey, int nRequestID=0); 

    ///请求查询转帐银行
    int ReqQryTransferBank(PlatformStru_QryTransferBank &pQryTransferBank, int nRequestID=0) ;

    ///请求查询投资者持仓明细
    int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail &pQryInvestorPositionDetail, int nRequestID=0) ;

    ///请求查询客户通知
    int ReqQryNotice(PlatformStru_QryNotice &pQryNotice, int nRequestID=0) ;

    ///请求查询结算信息确认
    int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm &pQrySettlementInfoConfirm, int nRequestID=0) ;

    ///请求查询投资者持仓明细
    int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail &pQryInvestorPositionCombineDetail, int nRequestID=0) ;

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


    ///期货发起银行资金转期货请求
    int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer &pReqTransfer, int nRequestID=0) ;

    ///期货发起期货资金转银行请求
    int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer &pReqTransfer, int nRequestID=0) ;

    ///期货发起查询银行余额请求
    int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount &pReqQueryAccount, int nRequestID=0) ;

	//void SetModuleDataQry(bool bQry);
    //bool GetModuleDataQry();
    //重新连接
	void ReConnetServer();
	//获取登录状态原因
	std::string GetLogonReason();
	void GetLinkStatus(int * pHost,int * pPrice);
	//获取报告
	int GetReports(map<DWORD, CTPReport>& mapReport);
	double GetExchangeRate(char* currency);
	bool IsSunGardSupported(){return true;}
	bool IsHistorySettlementSupported(){return true;}
	//CTP支持文件保存合约，手续费，保证金
	bool HaveFileCache(){return true;}
    //通知: 初始化查询开始/成功/失败
    void NotifyInitQryStart();
    void NotifyInitQrySucc();
    void NotifyInitQryFail();
    bool IsInitQrySucc();


	//为插件添加以下接口
	//获取委托单列表FTID
	int GetAllOrderFTIDList(std::vector<long>& vec);
	//获取委托单详情
	int GetAllOrderInfo(long lFTID, PlatformStru_OrderInfo& outData);
	int GetAllOrderInfo(OrderKey key, PlatformStru_OrderInfo& outData);

	//获取持仓列表FTID
	int GetPositionFTIDList(std::vector<long>& vec);
	//获取持仓详情
	int GetPositionInfo(long lFTID, PlatformStru_Position& outData);
	int GetPositionInfo(PositionKey key, PlatformStru_Position& outData);

	//获取组合持仓列表FTID
	int GetPositionCombFTIDList(std::vector<long>& vec);
	//获取组合持仓详情
	int GetPositionCombInfo(long lFTID, PlatformStru_Position& outData);
	//int GetPositionCombInfo(PositionCombDetailKey2 key, PlatformStru_PositionCombDetail2& outData);

    //获取交易连接的FrontID和SessionID。这两个值在交易登录时由ctp返回
    void GetTradingLoginInfo(int& FrontID,int& SessionID,string& UserID);

	int GetGroupMaxContractNum();
	//合约数量合适不需要特别处理
	bool ContractCountIsFit();

	//bool IsCurrentAccount();
	//void SetCurrentAccount(bool bCurrent);

	int  GetPlatformType();
	//bool GetQryFinished();
	//void SetQryFinished(bool bFinished);

public:
 //   static CServiceProxy& GetSingleInstance();
	//static void Release();
    ~CServiceProxy();

    CServiceProxy(const string& strAccount,int PlatformID,int MaxPlatformID);
private:
    //static CServiceProxy* s_mInstance;
    bool m_isLogin;
	bool  m_QryFinished;
	int  m_qrystep;
    zqWriteLog*           m_pWriteLog;
    CQryQueue* m_pQryQueue;
	//bool m_bCurrentAccount;

public:
    bool m_bInitQryStart;                            //正在进行初始化查询
    bool m_bInitQrySucc;                             //初始化查询成功
    bool m_bInitQryFail;                             //初始化查询失败

protected:
	CPlatFormService* m_PlatFormService;
};

