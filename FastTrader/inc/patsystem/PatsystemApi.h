#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
using namespace std;
#include "patsystem.h"
#include "patsIntf.h"
#include "ISvr\PlatformStruct.h"

#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "ApiFunc.h"


#pragma warning (disable : 4251)
#pragma warning(disable:4996)
#define THOST_FTDC_OST_ERROE  'd'
typedef CHAR SafeFloatStr[128];
typedef struct _tagPatsPositionItem
{
    _tagPatsPositionItem(Array20 ac,Array10 ex,Array10 cn,Array50 cd)
    {
        memset(this, 0, sizeof(*this));
        strncpy(TraderAccount,ac,20);
        strncpy(ExchangeName,ex,20);
        strncpy(ContractName,cn,20);
        strncpy(ContractDate,cd,20);
    };
    bool operator<(const _tagPatsPositionItem  &b)const 
    {
        int ret;
        ret=strcmp(TraderAccount,b.TraderAccount);
        if(ret!=0) return ret<0;
        ret=strcmp(ExchangeName,b.ExchangeName);
        if(ret!=0) return ret<0;
        ret=strcmp(ContractName,b.ContractName);
        if(ret!=0) return ret<0;
        ret=strcmp(ContractDate,b.ContractDate);
        if(ret!=0) return ret<0;
        return false;
    }
    Array20 TraderAccount;
    Array10 ExchangeName;
    Array10 ContractName;
    Array50 ContractDate;
}PatsPositionItem;
class CPatsystemApi
{
public:
	CPatsystemApi(const char *pszFlowPath);
	~CPatsystemApi(void);
public:
	static CPatsystemApi *CreateFtdcApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	virtual void Release();
	
	///初始化
	///@remark 初始化运行环境,只有调用后,接口才开始工作
	virtual void Init();
	
	///等待接口线程结束运行
	///@return 线程退出代码
	virtual int Join();
	
	///获取当前交易日
	///@retrun 获取到的交易日
	///@remark 只有登录成功后,才能得到正确的交易日
	virtual const char *GetTradingDay();
	
	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(char *pszFrontAddress);
	
	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CPatsQuotSpi *pSpi);
	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CPatsTraderSpi *pSpi);
	//---------------------------------------------------------------------------------------------
	///订阅行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int SubscribeMarketData(char *ppInstrumentID[], int nCount);

	///退订行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int UnSubscribeMarketData(char *ppInstrumentID[], int nCount);
	//---------------------------------------------------------------------------------------------
	///订阅私有流。
	///@param nResumeType 私有流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后私有流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到私有流的数据。
	virtual void SubscribePrivateTopic(THOST_TE_RESUME_TYPE nResumeType) ;
	
	///订阅公共流。
	///@param nResumeType 公共流重传方式  
	///        THOST_TERT_RESTART:从本交易日开始重传
	///        THOST_TERT_RESUME:从上次收到的续传
	///        THOST_TERT_QUICK:只传送登录后公共流的内容
	///@remark 该方法要在Init方法前调用。若不调用则不会收到公共流的数据。
	virtual void SubscribePublicTopic(THOST_TE_RESUME_TYPE nResumeType) ;

	///客户端认证请求
	virtual int ReqAuthenticate(CThostFtdcReqAuthenticateField *pReqAuthenticateField, int nRequestID) ;

	///用户登录请求
	virtual int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) ;
	

	///登出请求
	virtual int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID) ;

	///用户口令更新请求
	virtual int ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, int nRequestID) ;

	///资金账户口令更新请求
	virtual int ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, int nRequestID) ;

	///报单录入请求
	virtual int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID) ;

	///预埋单录入请求
	virtual int ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID) ;

	///预埋撤单录入请求
	virtual int ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID) ;

	///报单操作请求
	virtual int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID) ;

	///查询最大报单数量请求
	virtual int ReqQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, int nRequestID) ;

	///投资者结算结果确认
	virtual int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, int nRequestID) ;

	///请求删除预埋单
	virtual int ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID) ;

	///请求删除预埋撤单
	virtual int ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID) ;

	///请求查询报单
	virtual int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) ;

	///请求查询成交
	virtual int ReqQryTrade(CThostFtdcQryTradeField *pQryTrade, int nRequestID) ;

	///请求查询投资者持仓
	virtual int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID) ;

	///请求查询资金账户
	virtual int ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount, int nRequestID) ;
    int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& Data);
	///获取账户资金文本信息
	int GetAccountText(std::string& outData,int language);

	///请求查询投资者
	virtual int ReqQryInvestor(CThostFtdcQryInvestorField *pQryInvestor, int nRequestID) ;

	///请求查询交易编码
	virtual int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID) ;

	///请求查询合约保证金率
	virtual int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID) ;

	///请求查询合约手续费率
	virtual int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID) ;

	///请求查询交易所
	virtual int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID) ;

	///请求查询合约
	virtual int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument, int nRequestID) ;

	///请求查询行情
	virtual int ReqQryDepthMarketData(CThostFtdcQryDepthMarketDataField *pQryDepthMarketData, int nRequestID) ;

	///请求查询投资者结算结果
	virtual int ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo, int nRequestID) ;

	///请求查询转帐银行
	virtual int ReqQryTransferBank(CThostFtdcQryTransferBankField *pQryTransferBank, int nRequestID) ;

	///请求查询投资者持仓明细
	virtual int ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail, int nRequestID) ;

	///请求查询客户通知
	virtual int ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice, int nRequestID) ;

	///请求查询结算信息确认
	virtual int ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm, int nRequestID) ;

	///请求查询投资者持仓明细
	virtual int ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail, int nRequestID) ;

	///请求查询保证金监管系统经纪公司资金账户密钥
	virtual int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField *pQryCFMMCTradingAccountKey, int nRequestID) ;

	///请求查询仓单折抵信息
	virtual int ReqQryEWarrantOffset(CThostFtdcQryEWarrantOffsetField *pQryEWarrantOffset, int nRequestID) ;

	///请求查询转帐流水
	virtual int ReqQryTransferSerial(CThostFtdcQryTransferSerialField *pQryTransferSerial, int nRequestID) ;

	///请求查询银期签约关系
	virtual int ReqQryAccountregister(CThostFtdcQryAccountregisterField *pQryAccountregister, int nRequestID) ;

	///请求查询签约银行
	virtual int ReqQryContractBank(CThostFtdcQryContractBankField *pQryContractBank, int nRequestID) ;

	///请求查询预埋单
	virtual int ReqQryParkedOrder(CThostFtdcQryParkedOrderField *pQryParkedOrder, int nRequestID) ;

	///请求查询预埋撤单
	virtual int ReqQryParkedOrderAction(CThostFtdcQryParkedOrderActionField *pQryParkedOrderAction, int nRequestID) ;

	///请求查询交易通知
	virtual int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID) ;

	///请求查询经纪公司交易参数
	virtual int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID) ;

	///请求查询经纪公司交易算法
	virtual int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID) ;

	///期货发起银行资金转期货请求
	virtual int ReqFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) ;

	///期货发起期货资金转银行请求
	virtual int ReqFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, int nRequestID) ;

	///期货发起查询银行余额请求
	virtual int ReqQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, int nRequestID) ;
	//---------------------------------------------------------------------------------------------
public:
	void DoQryInstrument();
	void FireLogon();
protected:
	 void CalcPositionCloseProfit(const std::string strAccount,PlatformStru_TradingAccountInfo& info);
	 void DoLogon(BOOL bOK,CThostFtdcReqUserLoginField *pReqUserLoginField=NULL, int nRequestID=0);
	 std::string MakeLongInstrumentID(std::string exchange, std::string name, std::string date);
	 void GetLastUpdateTime( char* pBuf, int nLen, const PriceDetailStruct& priceDetail );
	 std::string GetLogonStatusString(BYTE LogonStatus);
	 void initOrderTypeMap();
public:
	int GetAccountList(std::vector<string>& AcctList);	
	int GetExchangeIDs(vector<string>& outData);
	//设置新密码
	void SetNewPwd( const std::string& strNewPwd );
	//定时器
	friend VOID CALLBACK OnTimerProc(HWND   hwnd,UINT   uMsg,UINT_PTR   idEvent,DWORD   dwTime);
	///获取账号信息
	int GetTrader(std::vector<PlatformStru_Trader> &outData);
    ///获取交易所信息
   int GetExchange(std::vector<PlatformStru_Exchange> &outData);
    ///获取合约信息
   int GetContract(std::vector<PlatformStru_Contract> &outData);
    ///获取商品信息
   int GetCommodity(std::vector<PlatformStru_Commodity> &outData);
   ///获取报单类型信息
   int GetOrderType(std::vector<PlatformStru_OrderType>& outData);
   // 获取报告
   int GetReports(map<DWORD, Report>& mapReport);
   //
   double GetExchangeRate(char* currency);
   bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
   bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);

   bool GetPositionInfo( const std::string& strAccount, const std::string& strInstrumentID, PlatformStru_Position& field );
   void GetAllPositionInfo(std::vector<PlatformStru_Position>& vPosition);
   // 获取登录状态原因
   std::string GetLogonReason();
   //注册回调
   void RegisterCallback(bool reg);
	void GetAllContractPosition(std::vector<PlatformStru_Position>& vecPosition);
private:
	HANDLE m_hThread; 
	CPatsQuotSpi *m_pMdSpi;
	CPatsTraderSpi *m_pTraderSpi;
    std::list<TraderAcctStruct> m_AccountList;//账户列表
	std::list<OrderTypeStruct> m_OrderType;//报单类型列表
	//std::list<FillStruct> m_FillList;//成交列表
	std::list<OrderDetailStruct> m_OrderList;//成交列表
	std::list<CommodityStruct> m_Commodity;//商品列表
	TraderStr m_accountName;
	std::map<std::string, ContractStruct> m_mapInstrumentDetail;
	std::map<std::string, PlatformStru_DepthMarketData*> m_mapInstrumentPrices;
	std::map<std::string, char> mapOrderType;
	std::set<std::string> m_SubscribedInstrument;
    std::set<PatsPositionItem> m_PatsPositionItems;
	INT iTimerID;
	PlatformStru_TradingAccountInfo m_FundAccount;
	std::string m_strNewPwd;
	LogonStatusStruct m_logonStatus;
	bool bFisrtReqUserLogin;
	bool bFirstLinkConnected;
public:
	int	m_HostLinkState;
	int	m_PriceLinkState;
	zqWriteLog* m_pWriteLog;
	ClientType m_ClientType;
public:
	//下面是回调函数
	static void WINAPI onHostLinkStateChange(LinkStateStructPtr data);
	static void WINAPI onPriceLinkStateChange(LinkStateStructPtr data);
	static void WINAPI onLogonStatus();
	static void WINAPI onDataDLComplete();
	static void WINAPI onForcedLogout();
	static void WINAPI onContractAdded(ContractUpdStructPtr data);
	static void WINAPI onContractDeleted(ContractUpdStructPtr data);
	static void WINAPI onMessage(MsgIDPtr MsgID);
	static void WINAPI onOrder(OrderUpdStructPtr data);
	static void WINAPI onStatusChange(StatusUpdStructPtr data);
	static void WINAPI onFill(FillUpdStructPtr data);
	static void WINAPI onPriceUpdate(PriceUpdStructPtr data);
	static void WINAPI onTickerUpdate(TickerUpdStructPtr data);
	static void WINAPI onExchangeRate(ExchangeRateUpdStructPtr data);
	static void WINAPI onConnectivityStatus(ConnectivityStatusUpdStructPtr data);
	static void WINAPI onOrderCancelFailure(OrderUpdStructPtr data);
	static void WINAPI onAtBestUpdate(AtBestUpdStructPtr data);
	static void WINAPI onMemoryWarning();
	static void WINAPI onSubscriberDepthUpdate(SubscriberDepthUpdStructPtr data);
	static void WINAPI onDOMUpdate(DOMUpdStructPtr data);
	static void WINAPI onSettlementCallback(SettlementPriceStructPtr data);
	static void WINAPI onStrategyCreateSuccess(StrategyCreateSuccessStructPtr data);
	static void WINAPI onStrategyCreateFailure(StrategyCreateFailureStructPtr data);
	static void WINAPI onGenericPriceUpdate(GenericPriceStructPtr data);
	static void WINAPI onBlankPrice(BlankPriceStructPtr data);
	static void WINAPI onOrderSentFailure(OrderUpdStructPtr data);
	static void WINAPI onOrderQueuedFailure(OrderUpdStructPtr data);
	static void WINAPI onExchangeUpdate( ExchangeUpdStructPtr data);
	static void WINAPI onCommodityUpdate(CommodityUpdStructPtr data);
	static void WINAPI onPurgeCompleted( ExchangeUpdStructPtr data);
	static void WINAPI onOrderTypeUpdate(OrderTypeStructPtr data);
	static void InvalidateDepthMarketData( PlatformStru_DepthMarketData* pDepthMarketData );
};
VOID   CALLBACK   OnTimerProc(HWND   hwnd,UINT   uMsg,UINT_PTR   idEvent,DWORD   dwTime) ;