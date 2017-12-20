/***************************************************************************//**
*   @file          PlatFormService.hpp
*   @brief        综合交易平台(CTP)交易接口
*
*   @author     jacky <panxun.li@cithinc.com>
*   @version    0.0.1
*   @date       created on 2010/09/13
********************************************************************************/
#ifndef _PLATFORMSERVICE_HPP_
#define _PLATFORMSERVICE_HPP_

#include <string>
#include <set>
#include <map>
#include <list>
#include <vector>


//#include "ctp/ThostFtdcTraderApi.h"
//#include "ctp/ThostFtdcMdApi.h"

#include "TraderSpi.hpp"
#include "QuotSpi.hpp"
#include "../inc/ISvr/BusinessData.hpp"
#include "../inc/ISvr/PlatformStruct.h"
#include "../inc/Patsystem/PatsystemApi.h"


class CPackageCache;

class zqWriteLog;


namespace Pats
{
	class CDataCenter;
	class CPlatFormParam
	{
	public:
		//下面定义交易所连接状态
		enum CONNSTATUS
		{    
			CONNSTATUS_Disconnected   =      0       ,            //未连接
			CONNSTATUS_Connecting     =      1       ,            //正在连接
			CONNSTATUS_Logining       =      2       ,            //正在登录
			CONNSTATUS_LoginFailure   =      3       ,            //登录失败
			CONNSTATUS_TradeSettlementInfoConfirming     =   4 ,  //正在进行交易结算结果确认
			//CONNSTATUS_TradeSettlementInfoConfirmFailure =   5 ,  //交易结算结果确认失败
			CONNSTATUS_Connected      =      6       ,            //连接登录成功
			CONNSTATUS_Disconnecting  =      7       ,            //正在断开连接
			CONNSTATUS_UnInit         =      8                    //未初始化
		};

	public:
		CPlatFormParam()
		{
			TradeStatus=CONNSTATUS_Disconnected;
			QuotStatus =CONNSTATUS_Disconnected;
			HaveAllInstrumentIDs=false;
			RequestID=0;
		};

	public:
		std::vector<std::string>   TradeFrontAddrs; //类似多个"asp-sim2-front1.financial-trading-platform.com:26205"
		std::vector<std::string>   QuotFrontAddrs;   //类似多个"58.246.49.130:41213"
		std::string                BrokerID;
		std::string                UserID;
		std::string                UserPW;
		std::string                InvestorID;
		std::string                InvestorPW;
		std::string					m_strInvestorNewPWD;
		CONNSTATUS                 TradeStatus;
		CONNSTATUS                 QuotStatus;
		bool                       HaveAllInstrumentIDs;
		int                        RequestID;
		std::string                OneTimePassword;
		std::set<std::string>		Accounts;		// 交易账号列表
	};

	class CPlatFormService
	{
		friend class CTraderSpi;
		friend class CQuotSpi;
		friend class CTradeThread;
		friend class CQuotThread;
		friend class CPatsApiSvr;
		//交易接口
	public:
		///用户口令更新请求
		int ReqUserPasswordUpdate(PlatformStru_UserPasswordUpdate *pUserPasswordUpdate, int nRequestID=0);

		///资金账户口令更新请求
		int ReqTradingAccountPasswordUpdate(PlatformStru_TradingAccountPasswordUpdate *pTradingAccountPasswordUpdate, int nRequestID=0);

		///报单录入请求
		int ReqOrderInsert(PlatformStru_InputOrder *pInputOrder, int nRequestID=0);

		///预埋单录入请求
		int ReqParkedOrderInsert(PlatformStru_ParkedOrder *pParkedOrder, int nRequestID=0);

		///预埋撤单录入请求
		int ReqParkedOrderAction(PlatformStru_ParkedOrderAction *pParkedOrderAction, int nRequestID=0);

		///报单操作请求
		int ReqOrderAction(PlatformStru_InputOrderAction *pInputOrderAction, int nRequestID=0);

		///查询最大报单数量请求
		int ReqQueryMaxOrderVolume(PlatformStru_QueryMaxOrderVolume *pQueryMaxOrderVolume, int nRequestID=0);

		///投资者结算结果确认
		int ReqSettlementInfoConfirm() ;

		///请求删除预埋单
		int ReqRemoveParkedOrder(PlatformStru_RemoveParkedOrder *pRemoveParkedOrder, int nRequestID=0) ;

		///请求删除预埋撤单
		int ReqRemoveParkedOrderAction(PlatformStru_RemoveParkedOrderAction *pRemoveParkedOrderAction, int nRequestID=0) ;

		///请求查询报单
		int ReqQryOrder(PlatformStru_QryOrder *pQryOrder, int nRequestID=0) ;

		///请求查询成交
		int ReqQryTrade(PlatformStru_QryTrade *pQryTrade, int nRequestID=0) ;

		///请求查询投资者持仓
		int ReqQryInvestorPosition(PlatformStru_QryInvestorPosition *pQryInvestorPosition, int nRequestID=0) ;

		///请求查询资金账户
		int ReqQryTradingAccount(PlatformStru_QryTradingAccount *pQryTradingAccount, int nRequestID=0) ;

		///请求查询投资者
		int ReqQryInvestor(PlatformStru_QryInvestor *pQryInvestor, int nRequestID=0) ;

		///请求查询交易编码
		int ReqQryTradingCode(CThostFtdcQryTradingCodeField *pQryTradingCode, int nRequestID=0) ;

		///请求查询合约保证金率
		int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate, int nRequestID=0) ;

		///请求查询合约手续费率
		int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate, int nRequestID=0) ;

		///请求查询交易所
		int ReqQryExchange(CThostFtdcQryExchangeField *pQryExchange, int nRequestID=0) ;

		///请求查询合约
		int ReqQryInstrument(PlatformStru_QryInstrument *pQryInstrument, int nRequestID=0) ;

		///请求查询行情
		int ReqQryDepthMarketData(PlatformStru_QryDepthMarketData *pQryDepthMarketData, int nRequestID=0) ;

		///请求查询投资者结算结果
		int ReqQrySettlementInfo(PlatformStru_QrySettlementInfo *pQrySettlementInfo, int nRequestID=0) ;

		///请求查询保证金监管系统经纪公司资金账户密钥
		int ReqQryCFMMCTradingAccountKey(PlatformStru_QryCFMMCTradingAccountKey *pQryCFMMCTradingAccountKey, int nRequestID=0) ;

		///请求查询转帐银行
		int ReqQryTransferBank(PlatformStru_QryTransferBank *pQryTransferBank, int nRequestID=0) ;

		///请求查询投资者持仓明细
		int ReqQryInvestorPositionDetail(PlatformStru_QryInvestorPositionDetail *pQryInvestorPositionDetail, int nRequestID=0) ;

		///请求查询客户通知
		int ReqQryNotice(PlatformStru_QryNotice *pQryNotice, int nRequestID=0) ;

		///请求查询结算信息确认
		int ReqQrySettlementInfoConfirm(PlatformStru_QrySettlementInfoConfirm *pQrySettlementInfoConfirm, int nRequestID=0) ;

		///请求查询投资者持仓明细
		int ReqQryInvestorPositionCombineDetail(PlatformStru_QryInvestorPositionCombineDetail *pQryInvestorPositionCombineDetail, int nRequestID=0) ;

		///请求查询转帐流水
		int ReqQryTransferSerial(PlatformStru_QryTransferSerial *pQryTransferSerial, int nRequestID=0) ;

		///请求查询签约银行
		int ReqQryContractBank(PlatformStru_QryContractBank *pQryContractBank, int nRequestID=0) ;

		///请求查询银期签约关系
		int ReqQryAccountregister(PlatformStru_QryAccountRegister *pQryAccountregister, int nRequestID=0);

		///请求查询预埋单
		int ReqQryParkedOrder(PlatformStru_QryParkedOrder *pQryParkedOrder, int nRequestID=0) ;

		///请求查询预埋撤单
		int ReqQryParkedOrderAction(PlatformStru_QryParkedOrderAction *pQryParkedOrderAction, int nRequestID=0) ;

		///请求查询交易通知
		int ReqQryTradingNotice(CThostFtdcQryTradingNoticeField *pQryTradingNotice, int nRequestID=0) ;

		///请求查询经纪公司交易参数
		int ReqQryBrokerTradingParams(CThostFtdcQryBrokerTradingParamsField *pQryBrokerTradingParams, int nRequestID=0) ;

		///请求查询经纪公司交易算法
		int ReqQryBrokerTradingAlgos(CThostFtdcQryBrokerTradingAlgosField *pQryBrokerTradingAlgos, int nRequestID=0) ;

		///期货发起银行资金转期货请求
		int ReqFromBankToFutureByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID=0) ;

		///期货发起期货资金转银行请求
		int ReqFromFutureToBankByFuture(PlatformStru_ReqTransfer *pReqTransfer, int nRequestID=0) ;

		///期货发起查询银行余额请求
		int ReqQueryBankAccountMoneyByFuture(PlatformStru_ReqQueryAccount *pReqQueryAccount, int nRequestID=0) ;

		//行情接口
	public:
		///向综合交易平台定制行情
		int SubscribeMarketData(const std::string& InstrumentID);

		///向综合交易平台取消行情定制
		int UnSubscribeMarketData(const std::string& InstrumentID);

		//控制管理接口    
	public:
		int LoginTrade(const std::string& InvestorID, const std::string& InvestorPW, const std::string& InvestorNewPwd);
		int LogoutTrade();
		int LoginQuot(const std::string& InvestorID, const std::string& InvestorPW);
		int LogoutQuot();
		void StopThreads(void);
		///密码是否是当前密码
		///1：是，其他：不是
		int IsValidPW(const std::string& yourPW);
		int GetBrokerIDUserID(std::string& UserID, std::string& BrokerID,set<string>* accounts=NULL);
		int SetTradeFrontAddrs(const std::vector<std::string>& TradeAddr);
		int SetQuotFrontAddrs(const std::vector<std::string>& QuotAddr);
		int SetParticipantInfo(const std::string& BrokerID, const std::string& UserID, const std::string& UserPW, const std::string& OneTimePassword);   

		//自定义接口
	public:
		///定制业务数据
		int SubscribeBusinessData(BusinessID BID, GUIModuleID GID, GuiCallBackFunc callback, bool sendCurrentInfo=false);

		///取消业务数据的定制
		int UnSubscribeBusinessData(BusinessID BID, GUIModuleID GID);

		///获取合约列表
		int GetInstrumentList(std::set<std::string> &outData,std::string ExchangeID="");
		int GetInstrumentList(std::vector<GroupInfo> &outData);
		///添加主力合约列表
		int AddMainInstrumentList(std::string instrument);
		///获取主力合约列表
		int GetMainInstrumentList(std::set<std::string> &outData);
		///设置合约容差列表
		void SetInstrumentVarietyMap(map<string, string>& inData);
		// 获取合约容差列表
		int GetInstrumentVarietyMap(map<string, string>& outData);
		///获取指定品种的合约列表,返回合约个数
		int GetInstrumentListByProductID(const std::string& ProductID,std::set<std::string> &outData);
		///获取指定合约信息
		int GetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
		///设置指定合约信息
		void SetInstrumentInfo(const std::string& InstrumentID, PlatformStru_InstrumentInfo& outData);
		///获取合约的产品类型，失败返回-1
		char GetProductClassType(const std::string& InstrumentID);
		//设置合约订阅状态
		void SetSubscribeStatus(const std::string& InstrumentID,int status,S_Status oper=S_STATUS_ADD);
		///获取合约品种代码
		int GetProductID(std::map<std::string, std::set<std::string> >& outData,std::string ExchangeID="");    

		///获取合约手续费率
		int GetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);
		///设置合约手续费率
		int SetCommissionRate(const std::string& InstrumentID, PlatformStru_InstrumentCommissionRate& outData);  
		///设置正在查询手续费率的合约
		void SetReqCommissionRateInstrumentID(const std::string& InstrumentID);
		///获取合约保证金率
		int GetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);
		///获取合约保证金率
		int SetMarginRate(const std::string& InstrumentID, PlatformStru_InstrumentMarginRate& outData);  
		///获取合约乘数，成功返回合约乘数，失败返回-1
		int GetInstrumentMultiple(const std::string& InstrumentID);


		///获取指定合约行情
		int GetQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
		///获取指定合约旧行情
		int GetOldQuotInfo(const std::string& InstrumentID, PlatformStru_DepthMarketData& outData);
		///获取指定合约的现价，失败返回0.0
		double GetCurPrice(const std::string& InstrumentID);


		bool HavePositionDetail(const std::string& strInstrumentID);

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
		int GetPositionCombs2(const std::string& strTradeID, std::string& strInstrument,std::vector<PlatformStru_PositionComb>& outData);

		int GetPositions(std::vector<PlatformStru_Position>& outData);
		int GetPositionDetails(std::vector<PlatformStru_PositionDetail>& outData,long& lastseq);
		int GetPositionCombs(std::vector<PlatformStru_PositionComb>& outData);


		//获取持仓记录键值列表,返回持仓记录的数量
		int GetPositionKeySet(std::set<PositionKey> &PositionKeySet);
		//获取指定合约、方向、投保的持仓数据，成功返回0，失败返回-1
		int GetPositionData(const std::string& strAccount, const std::string& InstrumentID,const char Direction,const char HedgeFlag,PlatformStru_Position& PositionData);

		///设置当前显示的持仓内容。1:持仓；2:持仓明细；3:组合持仓
		void SetCurrentPositionContent(int PositionContentMode);

		string GetTradingDay(const string& ExchangeID);
		bool IsTodayPosition( const char* pOpenData );

		///获取交易所时间
		unsigned long GetExchangeTime(const string& ExchangeID,string& time);
		///获取全部交易所
		int GetExchangeIDs(vector<string>& outData);
		///获取全部报单
		int GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);
		///获取全部报单
		int GetAllOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///获取指定报单
		bool GetOrder(const OrderKey& inOrderKey,PlatformStru_OrderInfo& outData);

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
		int GetWaitOrders2(const std::string& strInstrument,std::vector<PlatformStru_OrderInfo>& outData);
		///获取指定未成交报单
		bool GetWaitOrder(const OrderKey& orderkey,PlatformStru_OrderInfo& outData);


		int GetWaitOrderVolume(const std::string& strAccount, const std::string &strInstrumentID, const char Direction, char CloseType,const char HedgeFlag=THOST_FTDC_HF_Speculation);
		//获取指定合约指定方向的平仓挂单的平仓量，返回0表示成功，<0表示失败。成功时CloseVolume返回平仓单(含强平和本地强平)的手数，CloseTodayVolume返回平今单手数，CloseYesterdayVolume表示平昨单手数
		int GetCloseOrderVolume(const std::string& strAccount,const std::string &strInstrumentID, const char& Direction,int& CloseVolume,int& CloseTodayVolume,int& CloseYesterdayVolume,const char HedgeFlag=THOST_FTDC_HF_Speculation);

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
		void GetMarketDataInstrumentID(std::set<std::string>& setMarketDataInstrumentID);
		void SetOrderRef(int orderref){m_CurOrderRef=orderref;}
		//获取客户签约银行信息
		int GetContractBank(std::map<std::string,  PlatformStru_ContractBank>& outData);

		CPlatFormParam::CONNSTATUS GetTradeStatus(void);


		CPlatFormParam::CONNSTATUS GetQuotStatus(void);


		std::string GetTradeBrokerID(void);


		std::string GetTradeInvestorID(void);

		//
		bool IsSettlementInfoConfirmed();
		//
		void GetErrorField(int& ErrorID){ErrorID=m_ErrorInfoField.ErrorID;}
		void GetErrorField(string& ErrMsg){ErrMsg=m_ErrorInfoField.ErrorMsg;}
		void SetReqID(int nOrderReqID) {
#ifndef __PLATFORM_IB_
			m_nOrderReqID = nOrderReqID;
#endif
		};

		long GetReqID() {
#ifdef __PLATFORM_IB_
			if(m_pTradeApi != NULL)
				return m_pTradeApi->GetOrderID();
			else
				return -1;
#else
			return m_nOrderReqID;
#endif
		};

		long GetNextReqID() {
#ifdef __PLATFORM_IB_
			if(m_pTradeApi != NULL)
				return m_pTradeApi->GetNextOrderID();
			else
				return -1;
#else
			return ++m_nOrderReqID;
#endif
		};
		SERVER_PARAM_CFG&  GetServerParamCfg();
		void SetModuleDataQry(bool bQry);
		bool GetModuleDataQry();

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
		int GetPositionCombInfo(long lFTID, PlatformStru_PositionComb& outData);

	private:
		CPlatFormService(CPatsApiSvr& rServiceProxy);
		~CPlatFormService();    


		///用户登录请求
		int ReqUserLogin(CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID=0);	

		///登出请求
		int ReqUserLogout(CThostFtdcUserLogoutField *pUserLogout, int nRequestID=0);

		zqWriteLog*              m_pWriteLog;

		CPatsystemApi*			m_pTradeApi;    //交易接口对象，由综合交易平台的接口提供
		CTraderSpi*             m_pTradeSpi;    //交易接口回调对象
		CPatsystemApi*			m_pQuotApi;     //行情接口对象，由综合交易平台的接口提供
		CQuotSpi*               m_pQuotSpi;     //行情接口回调对象

		CPackageCache*          m_PCache;       //CTraderSpi与CQuotSpi将交易所的返回的数据保存到此cache中，
		//CDataCenter读取此cache
		CDataCenter*            m_pDataCenter;  //数据中心，负责读取m_PCache中的业务数据，并根据定制信息
		//调用对应的回调函数
		int				 m_CurOrderRef;//当前报单引用编号
		std::set<std::string> m_MarketDataInstrumentID;
		enum PLATFORM_STATUS
		{
			PLATFORM_STATUS_UNINIT,
			PLATFORM_STATUS_INITED
		};
		int                     m_status;       //平台的当前状态
		CPlatFormParam          m_PlatformParam;//平台的连接
		bool					m_bSettlementInfoConfirmed;
		CThostFtdcRspInfoField  m_ErrorInfoField;

		int						m_nOrderReqID;
		std::map<std::string, int> m_SubscribeMap;       //订阅行情的合约列表和订阅状态map

		CRITICAL_SECTION m_CS;
public:
	CPatsApiSvr& m_rServiceProxy;
	public:
		//pats独有
		int  GetAccountList(std::vector<string>& AcctList);
		void FillQuotData( const PlatformStru_DepthMarketData& rawQuot, QuotFiled fieldID,
			std::vector<std::string>& vec );
		int GetTrader(std::vector<PlatformStru_Trader> &outData) ;
		int GetExchange(std::vector<PlatformStru_Exchange> &outData);
		int GetContract(std::vector<PlatformStru_Contract> &outData);
		int GetCommodity(std::vector<PlatformStru_Commodity> &outData);
		int GetOrderType(std::vector<PlatformStru_OrderType> &outData);
		bool GetTotalPosition(std::string account,PlatformStru_AccountPosition* posi);
		bool GetOpenPosition(PlatformStru_Contract InstrumentID,std::string account,
			PlatformStru_AccountPosition* posi);
		bool GetContractPosition(PlatformStru_Contract InstrumentID,std::string account,
			PlatformStru_AccountPosition* posi);
		std::string GetLogonReason();
		void GetLinkStatus(int * pHost,int * pPrice);
		int GetReports(map<DWORD, Report>& mapReport);
		double GetExchangeRate(char* currency);
		void GetAllPosition(std::vector<PlatformStru_Position>& vecPosition);
	};
}
#endif

