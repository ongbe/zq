#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
using namespace std;
#include "CmeCore.h"
#include "ISvr\PlatformStruct.h"

#include "../inc/Module-Misc/SimpleWriteLog.h"
#include "DataWorker.h"
#include "../inc/Tools/UniPkg.h"
#include "../inc/tools/client_stru.h"
#include "../inc/tools/client_cmdid.h"
#pragma warning (disable : 4251)
#pragma warning(disable:4996)
#define THOST_FTDC_OST_ERROE  'd'
struct EPCID
{
    int ExchangeID;
    int ProductID;
    int ContractID;
    EPCID(int e=0,int p=0,int c=0)
    {
        ExchangeID=e;
        ProductID=p;
        ContractID=c;
    }
    bool operator <(const EPCID& rhs) const 
    {
        if(ExchangeID!=rhs.ExchangeID) 
            return (ExchangeID<rhs.ExchangeID);
        if(ProductID!=rhs.ProductID) 
            return (ProductID<rhs.ProductID);
        return (ContractID<rhs.ContractID);
    }
};
class PATSYSTEM_API CCmeCoreApi
{
public:
	CCmeCoreApi(void);
	~CCmeCoreApi(void);
public:
	static CCmeCoreApi *CreateFtdcApi(const char *pszFlowPath = "", const bool bIsUsingUdp=false);
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
    int GetTradingAccount(const std::string& strAccount, PlatformStru_TradingAccountInfo& Data);
    ///获取账户资金文本信息
    int GetAccountText(std::string& outData,int language);
    //
    int GetAccountList(std::vector<string>& AcctList);	
    bool GetPositionInfo( const std::string& strAccount, 
        const std::string& strInstrumentID, PlatformStru_Position& field );
    void GetAllContractPosition(std::vector<PlatformStru_Position>& vecPosition);
    void SetNewPwd( const std::string& strNewPwd );
    int GetExchangeIDs(vector<string>& outData);
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
   bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
   bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,PlatformStru_AccountPosition* posi);
   // 获取登录状态原因
   std::string GetLogonReason();
   void GetLinkStatus(int* pHost,int* pPrice);
   double GetExchangeRate(char* currency);
public:
    void	trade_rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//从tcp得到数据后，调用此回调函数进行处理
    void	trade_disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);		//断开tcp连接后，调用此回调函数进行处理
    void	trade_startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);										//开始调用connect()后，调用此函数通知应用层
    void	trade_connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//连接成功后，调用此回调函数进行处理
    void	trade_connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//连接失败后，调用此回调函数进行处理
    void    trade_ontime(void);
    void	quot_rcvdata(char *pData,int DataLen,Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);	//从tcp得到数据后，调用此回调函数进行处理
    void	quot_disconn(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);		//断开tcp连接后，调用此回调函数进行处理
    void	quot_startconnect(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);										//开始调用connect()后，调用此函数通知应用层
    void	quot_connsuccess(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//连接成功后，调用此回调函数进行处理
    void	quot_connfailure(Stru_IPAddress& LocalAddr,Stru_IPAddress& SvrAddr,SOCKET hSocket);									//连接失败后，调用此回调函数进行处理
    void    quot_ontime(void);
public:
    void ParseRcvData(const char *pData,int DataLen,bool bTrader);
    void OnRspUserLogin(const char *pData,int DataLen,bool bTrader);
    void OnRspUserLogout(const char *pData,int DataLen,bool bTrader);
    void OnRspQrySettlementInfoConfirm(const char *pData,int DataLen,bool bTrader);
    void OnRspQrySettlementInfo(const char *pData,int DataLen,bool bTrader);
    void OnRspSettlementInfoConfirm(const char *pData,int DataLen,bool bTrader);
    void OnRspGetCurrencyInfo(const char *pData,int DataLen,bool bTrader);
    void OnCurrencyInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRspGetAllExchangeInfo(const char *pData,int DataLen,bool bTrader);
    void OnExchangeInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRtnExchangeStatus(const char *pData,int DataLen,bool bTrader);
    void OnRspGetExchangeProductGroupInfo(const char *pData,int DataLen,bool bTrader);
    void OnProductGroupInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRspGetExchangeProductInfo(const char *pData,int DataLen,bool bTrader);
    void OnProductInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnContractInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void GetExchangeContractInfo(const char *pData,int DataLen,bool bTrader);
    void OnRspQryInstrument(int RequestID);
    void FillInstrumentInfo(const int ExchangeID,const Stru_Base_ContractInfo& ContractInfo,
        PlatformStru_InstrumentInfo& Instrument);
    void MakeGUIContractID(const Stru_Base_ExchangeInfo& ExchangeInfo,
                                    const Stru_Base_ProductInfo& ProductInfo,
                                    const Stru_Base_ContractInfo& ContractInfo,char* strContract);
    void OnRspQryTradingAccount(const char *pData,int DataLen,bool bTrader);
    void OnRspGetDepositWithdrawInfo(const char *pData,int DataLen,bool bTrader);
    void OnDepositWithdrawInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRspQryTrade(const char *pData,int DataLen,bool bTrader);
    void OnRtnTrade(const char *pData,int DataLen,bool bTrader);
    void OnRtnPositionDetail(const char *pData,int DataLen,bool bTrader);
    void OnRspQryInvestorPositionDetail(const char *pData,int DataLen,bool bTrader);
    void OnRspQryOrder(const char *pData,int DataLen,bool bTrader);
    void OnRspOrderCmd(const char *pData,int DataLen,bool bTrader);
    void OnRspOrderStatusUpdate(const char *pData,int DataLen,bool bTrader);
    void OnRspSubscribeMarketData(const char *pData,int DataLen,bool bTrader);
    void OnRspUnsubscribeMarketData(const char *pData,int DataLen,bool bTrader);
    void OnRspGetMarketDataSnapshot(const char *pData,int DataLen,bool bTrader);
    void OnNewMarketData(const char *pData,int DataLen,bool bTrader);
    void OnRspUserPasswordUpdate(const char *pData,int DataLen,bool bTrader);
    void SendHeartbeat(bool bTrader);
    void OnRspGetExchangeCommissionMarginInfo(const char *pData,int DataLen,bool bTrader);
    void OnCommissionMarginInfoUpdate(const char *pData,int DataLen,bool bTrader);
    void FillDepthMarketData(const Stru_Base_Market_SnapshotData& md,CThostFtdcDepthMarketDataField& field);
    void FillDepthMarketData(const Stru_Client_NewMarketData& md,PlatformStru_DepthMarketData& field);
    void FillOrderInfo(const Stru_Base_OrderInfo& Info,PlatformStru_OrderInfo& Order);
    void FillTradeInfo(const Stru_Base_TradeInfo& src,PlatformStru_TradeInfo& info);
    void FillPositionInfo(const Stru_Base_PositionInfo& src,PlatformStru_Position& pos);
    void FillTradingAccount(const Stru_Base_EquityInfo& src,CThostFtdcTradingAccountField& field);
    void FillTradingAccount(const Stru_Client_DepositWithdrawDynaUpdate& src,CThostFtdcTradingAccountField& field);
    void FillTradingAccount(const Stru_Base_DepositWithdrawDyna& src,CThostFtdcTradingAccountField& field);
    void FillOrderCmdInfo(const PlatformStru_InputOrder& src,Stru_Base_OrderCmdInfo& field);
    void FillOrderCmdInfo(const CThostFtdcInputOrderActionField& src,Stru_Base_OrderCmdInfo& field);
    void FillOrderCmdInfo(const CThostFtdcParkedOrderField& src,Stru_Base_OrderCmdInfo& field);
    void FillOrderCmdInfo(const CThostFtdcParkedOrderActionField& src,Stru_Base_OrderCmdInfo& field);

    void FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcInputOrderField& field);
    void FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcInputOrderActionField& field);
    void FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcParkedOrderField& field);
    void FillOrderCmdInfo(const EnumOrderCmdFlag flag,const Stru_Base_OrderInfo src,CThostFtdcParkedOrderActionField& field);
    void FillRtnOrder(const Stru_Base_OrderInfo& src,PlatformStru_OrderInfo& field);
    void FillCommissionMarginInfo(const Stru_Base_CustomerCommissionInfo& src,PlatformStru_InstrumentCommissionRate& cr);
    void FillCommissionMarginInfo(const Stru_Base_CustomerMarginInfo& src,PlatformStru_InstrumentMarginRate&  mr);
protected:
    void ParseAddress(char *pszFrontAddress,Stru_IPAddress& TradeFrontAddr,Stru_IPAddress& QuotFrontAddr);
    CTradeDataWorker*   m_pTradeDataWorker;
    CQuotDataWorker*    m_pQuotDataWorker;
    Stru_IPAddress      m_TradeFrontAddr;
    Stru_IPAddress      m_QuotFrontAddr;
    CPatsQuotSpi*       m_pMdSpi;
    CPatsTraderSpi*     m_pTraderSpi;
private:
    map<int,Stru_Base_CurrencyInfo>                 m_CurrencyInfos;//<CurrencyID,Stru_Base_CurrencyInfo>
    map<int,Stru_Base_ExchangeInfo>::const_iterator m_QueryExchangeInfoIterator;//<ExhangeID,Stru_Base_ExchangeInfo>  
    map<int,Stru_Base_ExchangeInfo>                 m_ExchangeInfos;//<ExhangeID,Stru_Base_ExchangeInfo>  
    map<int,map<int,Stru_Base_ProductGroupInfo>>     m_ProductGroupInfos;//<ExhangeID,<ProductGroupID,Stru_Base_ProductGroupInfo>>
    map<int,map<int,Stru_Base_ProductInfo>>         m_ProductInfos;//<ExhangeID,<ProductID,Stru_Base_ProductInfo>>
    map<int,map<int,Stru_Base_ContractInfo>>         m_ContractInfos;//<ExhangeID,<ContractID,Stru_Base_ContractInfo>>
    vector<Stru_Base_SettlementReport>               m_SettlementReportInfos;//
    map<string,EPCID>                                m_ContractName2ID;//<ContractGUIName,ContractID>
    map<int,string>                                  m_ID2ContractName;//<ContractID,ContractGUIName>
    map<int,EnumOrderCmdFlag>                       m_RequestID2OrderCmdFlag;//<RequestID,OrderCmdFlag>
    int                                           m_TradeOverTimeCount,m_QuotOverTimeCount;//超时次数
    CThostFtdcTradingAccountField                  m_TradingAccount;//当前资金信息
    set<int>                                       m_ReqCommissionMarginInfo;//<ExchangeID>
/////////////////////////////////////////////////////////////////////////////////////////////////////
    int SubscribeUnsubscribeID;
    map<int,string>                                 m_SubscribeUnsubscribeMarketMap;
};
