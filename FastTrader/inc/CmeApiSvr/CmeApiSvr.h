#pragma once
#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/ISvr/PlatformStruct.h"
#include "../inc/ISvr/IPlatformSvr.h"
#ifdef _DEBUG
	#pragma comment(lib, "CmeCoreD.lib")
#else
	#pragma comment(lib, "CmeCore.lib")
#endif
class CPlatFormService;
class CCmeApiSvr:public IPlatformSvr
{
public:
	CCmeApiSvr(void);
	~CCmeApiSvr(void);
public:
    ///启动平台
    int Start();

    ///关闭平台
    int Stop();

	///密码是否是当前密码
	///1：是，其他：不是
	int IsValidPW(const std::string& yourPW);
	int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID, set<string>* accounts=NULL);
    ///设置交易前置机地址
    int SetTradeFrontAddr(const std::string& addr);
    int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr);

    ///设置行情前置机地址
    int SetQuotFrontAddr(const std::string& addr);
    int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr);

    ///设置经纪公司参数信息
    int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword);

    ///登入
	int Login(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd = "");

    ///登出
    int Logout();

    ///定制业务数据
    int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

    ///取消业务数据的定制
    int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

    ///获取底层服务的当前状态
    ServiceStatus GetCurrentStatus(int* pQuotStatus=NULL,int* pTradeStatus=NULL);

    ///获取合约列表
    int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
    int GetInstrumentList(std::vector<GroupInfo> &outData);
    ///添加主力合约列表
    int AddMainInstrumentList(std::string instrument);
    ///获取主力合约列表
    int GetMainInstrumentList(std::set<std::string> &outData);
	// 设置合约容差列表
	void SetInstrumentVarietyMap(map<string, string>& inData);
	// 获取合约容差列表
	int GetInstrumentVarietyMap(map<string, string>& outData);
    //设置合约订阅状态
	void SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper=S_STATUS_ADD);
	//更新合约订阅状态
	int UpdateSubscribeStatus();
    ///获取指定合约信息
    int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///设置指定合约信息
    void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
    ///获取合约的产品类型，失败返回-1
    char GetProductClassType(const std::string& InstrumentID);

    ///获取合约品种代码
    int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");    

    ///获取合约手续费率
    int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    ///获取合约手续费率，如果没有，则触发查询
    int GetCommissionRate_TriggerReq(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
    ///设置合约手续费率
    int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
    ///获取合约保证金率
    int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
    ///获取合约保证金率，如果没有，则触发查询
    int GetMarginRate_TriggerReq(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
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
    ///获取指定合约的现价，失败返回0.0
    double GetCurPrice(const std::string& InstrumentID);


    //获取交易所交易日期
    string GetTradingDay();
    ///获取交易所时间
    unsigned long GetExchangeTime(const string& ExchangeID,string& time);
    ///获取全部交易所
    int GetExchangeIDs(vector<string>& outData);
    ///获取交易资金账户信息
	int GetTradingAccountWithdrawQuota(double& fWithdrawQuota);
	int GetTradingAccountAvailable(double& fAvailable);
	int GetTradingAccountID(char* AccountID);
    int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& outData);
	///获取账户资金文本信息
    int GetAccountText(std::string& outData,int language);
    ///设置交易资金账户信息
    void SetTradingAccount(const PlatformStru_TradingAccountInfo& inData);

    ///请求查询投资者结算结果响应
    int GetLastSettlementInfo(std::string& outData);

	///获取所有委托单信息
    int GetAllOrdersEx(std::map <AllOrderKey,  PlatformStru_OrderInfo> &AllOrdersMap);

    ///获取指定合约的持仓明细     add zhifa.gao	
    int GetPositionDetails(std::multimap<std::string, PositionDetailStru> &PositionDetailMap);
    int GetPositionDetails(const std::string& InstrumentID,std::multimap<std::string, PositionDetailStru> &PositionDetailMap);
    int GetPositionDetails(std::list<PositionDetailStru> &pPositionDetailList);
    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeyList(std::list<PositionKey> &PositionKeyList);
    //获取持仓记录键值列表,返回持仓记录的数量
    int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
    //获取指定账号、合约、方向、投保的持仓数据，成功返回0，失败返回-1
	int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PositionStru& PositionData);

    //获取挂单数量
    int  GetWaitOrderVolume(const std::string &strInstrumentID, const char Direction, const char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);

    //获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
    int GetCloseOrderVolume(const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);


    ///获取全部报单
    int GetAllOrders(std::map<OrderKey,  PlatformStru_OrderInfo>& outData);

    ///获取全部成交单
    int GetAllTrades(std::map<TradeKey,  PlatformStru_TradeInfo>& outData);
    ///获取成交统计
    int GetAllTradeTotalDatas(vector<TotalInfo>& outData);
	///在成交统计查找参数rawData
	int FindIndexFromAllTradeTotalDatas(const PlatformStru_TradeInfo& rawData );
	int InitCombRelation();
	bool IsSettlementInfoConfirmed();
	void GetErrorField(int& ErrorID);
	void GetErrorField(string& ErrMsg);

	void SetReqID(int nOrderReqID); 
	long GetReqID(); 
	long GetNextReqID();
	int  GetAccountList(std::vector<string>& AcctList);
	void FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID, std::vector<std::string>& vec );

	///交易所ID
	const string ExchangeID2String(const string& ExchangeID);
	//根据合约组获得交易所ID，如果是非系统合约组则返回空
	const string ExchangeName2ID(const string& name);
	//
	SERVER_PARAM_CFG&  GetServerParamCfg();

	//判断列表字段在该平台是否有效
	bool IsFieldValid( int nModuleId, int nFieldId );
	//获取当前可用模块ID数组
	int  GetModuleValid(vector<int>& ModuleIds);
	bool IsModuleValid( int nModuleId );
	//判断是否支持保值
	bool HaveOrderType(UIOrderType type);
	//判断是否支持银期转账
	bool HaveBankFutureTransfer(){return false;}
	// 读取合约的简称表结构，so as "CU,铜,T"
	void GetContractShortNames(string& strShortNames);
	//
	string GetVersionIconString();
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
   //
   bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
   bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);

   //判断是否支持获取账号信息
    bool HaveGetTrader() {return true;}
   //判断是否支获取交易所信息
    bool HaveGetExchange() {return true;}
   //判断是否支持获取合约信息
    bool HaveGetContract() {return false;}
   //判断是否支持获取商品信息
    bool HaveGetCommodity() {return true;}
	//判断是否支持获取报单类型信息
    bool HaveGetOrderType() {return true;}
	//判断是否支持获取持仓信息
	bool HaveGetPosition(){return true;}
	//判断是否支持获取报告信息
	bool HaveGetReport(){return true;}
	//判断是否支持获取汇率信息
	bool HaveGetExchangeRate(){return true;}
	//判断是否支持在登录时候容许修改密码
	bool HaveModifyPwdOnLongon() {return false;}
	bool HaveShowInstumentComboList() {return false;};
    bool HaveAccountText(){return false;}
       //合约名和合约ID是否完全一致
    bool ContractNameIsID(){return true;}
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

    ///报单录入请求
    int ReqOrderInsert(PlatformStru_InputOrder &pInputOrder, int nRequestID=0);

    ///预埋单录入请求
    int ReqParkedOrderInsert(PlatformStru_ParkedOrder &pParkedOrder, int nRequestID=0);

    ///预埋撤单录入请求
    int ReqParkedOrderAction(PlatformStru_ParkedOrderAction &pParkedOrderAction, int nRequestID=0);

    ///报单操作请求
    int ReqOrderAction(PlatformStru_InputOrderAction &pInputOrderAction, int nRequestID=0);

    ///查询最大报单数量请求
    int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume &pQueryMaxOrderVolume, int nRequestID=0);

    ///投资者结算结果确认
    int ReqSettlementInfoConfirm() ;

    ///请求删除预埋单
    int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder &pRemoveParkedOrder, int nRequestID=0) ;

    ///请求删除预埋撤单
    int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction &pRemoveParkedOrderAction, int nRequestID=0) ;

    ///请求查询报单
    int ReqQryOrder(PlatformStru_QryOrder &pQryOrder, int nRequestID=0) ;

    ///请求查询成交
    int ReqQryTrade(PlatformStru_QryTrade &pQryTrade, int nRequestID=0) ;

    ///请求查询投资者持仓
    int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition &pQryInvestorPosition, int nRequestID=0) ;

    ///请求查询资金账户
    int ReqQryTradingAccount(PlatformStru_QryTradingAccount &pQryTradingAccount, int nRequestID=0) ;
	//int  GetTradingAccount(PlatformStru_TradingAccountInfo& Data);

    ///请求查询投资者
    int ReqQryInvestor(PlatformStru_QryInvestor &pQryInvestor, int nRequestID=0) ;


    ///请求查询合约
    int ReqQryInstrument(PlatformStru_QryInstrument &pQryInstrument, int nRequestID=0) ;

    ///请求查询行情
    int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData &pQryDepthMarketData, int nRequestID=0) ;

    ///请求查询投资者结算结果
    int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo &pQrySettlementInfo, int nRequestID=0) ;

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

    ///订阅行情
    int SubscribeQuot(const std::string &InstrumentID);

    ///取消行情
    int UnSubscribeQuot(const std::string &InstrumentID);
	//
	void _SetInitQryStep(int step){m_qrystep=step;}
	//获取查询进度
	bool GetInitQryStatus(int status){return status<m_qrystep;}
    //重新连接
	void ReConnetServer();
	//获取登录状态原因
	std::string GetLogonReason();
	void GetLinkStatus(int* pHost,int* pPrice);
	//获取报告
	int GetReports(map<DWORD, Report>& mapReport);
	double GetExchangeRate(char* currency);
	bool IsSunGardSupported(){return false;}
	bool IsHistorySettlementSupported(){return false;}
	//获取合约与组合合约的关系表
	int GetCombineRelation(std::multimap<std::string,std::string>& mapCombineRelation){return 0;}
	//获取组合持仓列表
	virtual int GetComboPositions(std::map<pair<std::string, char>, CombinPositionStru>& outData){return 0;}
private:
    bool isLogin;
	int  m_qrystep;
	string	m_strContractShortName;
protected:
	CPlatFormService* m_PlatFormService;
};
