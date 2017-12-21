#pragma once

#include "FTTraderSpi.h"
#include "Module-Misc2\\KernelStruct.h"

// 这里管理Spi、Ctp的Api，并把Ctp的Api指针出给Spi
// 实现Ctp的Api初始化和Release过程
// 如有必要，创建查询线程，管理查询队列

//下面定义交易所连接状态
enum CTPCONNSTATUS
{    
	CTPCONNSTATUS_Disconnected   =      0       ,            //未连接
	CTPCONNSTATUS_Connecting     =      1       ,            //正在连接
	CTPCONNSTATUS_Logining       =      2       ,            //正在登录
	CTPCONNSTATUS_LoginFailure   =      3       ,            //登录失败
	CTPCONNSTATUS_TradeSettlementInfoConfirming     =   4 ,  //正在进行交易结算结果确认
	CTPCONNSTATUS_Connected      =      6       ,            //连接登录成功
	CTPCONNSTATUS_Disconnecting  =      7       ,            //正在断开连接
	CTPCONNSTATUS_UnInit         =      8                    //未初始化
};

class CFTTraderApi
{
public:
	CFTTraderApi(void);
	virtual ~CFTTraderApi(void);

	void Init(int nSocket);
	void Init(LPVOID pLinkEntity);
	void Release();


// 这里实现协议中提到的仿CThostFtdcTraderApi的函数
public:
	///用户登录请求
	int ReqUserLogin(CThostFtdcReqUserLoginField *pData, int nRequestID);

	///登出请求
	int ReqUserLogout(CThostFtdcUserLogoutField *pData, int nRequestID);

	///用户口令更新请求
	int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate* pData, int nRequestID);

	///资金账户口令更新请求
	int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate* pData, int nRequestID);

	///报单录入请求
	int ReqOrderInsert(PlatformStru_InputOrder* pData, int nRequestID);

	///预埋单录入请求
	int ReqParkedOrderInsert(PlatformStru_ParkedOrder* pData, int nRequestID);

	///预埋撤单录入请求
	int ReqParkedOrderAction(PlatformStru_ParkedOrderAction* pData, int nRequestID);

	///报单操作请求
	int ReqOrderAction(PlatformStru_InputOrderAction* pData, int nRequestID);

	///查询最大报单数量请求
	int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume* pData, int nRequestID);

	///投资者结算结果确认
	int ReqSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pData, int nRequestID);

	///请求删除预埋单
	int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder* pData, int nRequestID);

	///请求删除预埋撤单
	int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction* pData, int nRequestID);

	///询价录入请求
	int ReqForQuoteInsert(PlatformStru_InputForQuoteField* pData, int nRequestID);

	///执行宣告录入请求
	int ReqExecOrderInsert(PlatformStru_InputExecOrderField* pData, int nRequestID);

	///执行宣告操作请求
	int ReqExecOrderAction(PlatformStru_InputExecOrderActionField* pData, int nRequestID);

	///请求查询报单
	int ReqQryOrder(PlatformStru_QryOrder* pData, int nRequestID);

	///请求查询成交
	int ReqQryTrade(PlatformStru_QryTrade* pData, int nRequestID);

	///请求查询投资者持仓
	int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition* pData, int nRequestID);

	///请求查询资金账户
	int ReqQryTradingAccount(PlatformStru_QryTradingAccount* pData, int nRequestID);

	///请求查询投资者
	int ReqQryInvestor(PlatformStru_QryInvestor* pData, int nRequestID);

	///请求查询合约保证金率
	int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField* pData, int nRequestID);

	///请求查询合约手续费率
	int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField* pData, int nRequestID);

	///请求查询交易所
	int ReqQryExchange(CThostFtdcQryExchangeField* pData, int nRequestID);

	///请求查询产品
	int ReqQryProduct(PlatformStru_QryProduct* pData, int nRequestID);

	///请求查询合约
	int ReqQryInstrument(PlatformStru_QryInstrument* pData, int nRequestID);

	///请求查询投资者结算结果
	int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo* pData, int nRequestID);

	///请求查询投资者持仓明细
	int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail* pData, int nRequestID);

	///请求查询客户通知
	int ReqQryNotice(PlatformStru_QryNotice* pData, int nRequestID);

	///请求查询结算信息确认
	int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm* pData, int nRequestID);

	///请求查询投资者持仓明细
	int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail* pData, int nRequestID);

	///请求查询保证金监管系统经纪公司资金账户密钥
	int ReqQryCFMMCTradingAccountKey(CThostFtdcQryCFMMCTradingAccountKeyField* pData, int nRequestID);

	///请求查询期权交易成本
	int ReqQryOptionInstrTradeCost(CThostFtdcQryOptionInstrTradeCostField* pData, int nRequestID);

	///请求查询期权合约手续费
	int ReqQryOptionInstrCommRate(CThostFtdcQryOptionInstrCommRateField* pData, int nRequestID);

	///请求查询执行宣告
	int ReqQryExecOrder(PlatformStru_QryExecOrderField* pData, int nRequestID);

	///请求查询转帐流水
	int ReqQryTransferSerial(PlatformStru_QryTransferSerial* pData, int nRequestID);

	///请求查询银期签约关系
	int ReqQryAccountregister(PlatformStru_QryAccountRegister* pData, int nRequestID);

	///请求查询签约银行
	int ReqQryContractBank(PlatformStru_QryContractBank* pData, int nRequestID);

	///请求查询预埋单
	int ReqQryParkedOrder(PlatformStru_QryParkedOrder* pData, int nRequestID);

	///请求查询预埋撤单
	int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction* pData, int nRequestID);

	///请求查询交易通知
	int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField* pData, int nRequestID);

	///请求查询经纪公司交易参数
	int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField* pData, int nRequestID);

	///期货发起银行资金转期货请求
	int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer* pData, int nRequestID);

	///期货发起期货资金转银行请求
	int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer* pData, int nRequestID);

	///期货发起查询银行余额请求
	int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount* pData, int nRequestID);

private:
	CTPCONNSTATUS		     m_TradeStatus;	 //与ctp服务器连接状态
	CThostFtdcTraderApi*     m_pTradeApi;    //交易接口对象，由综合交易平台的接口提供
	CFTTraderSpi	         m_TradeSpi;     //交易接口回调对象
};
