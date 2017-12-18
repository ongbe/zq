/***************************************************************************//**
*   @file          PlatformCTPTrade.h
*   @brief        综合交易平台(CTP)交易接口
*
********************************************************************************/
#ifndef _PLATFORMCTPTRADE_H_
#define _PLATFORMCTPTRADE_H_

#include <string>
#include <set>
#include <map>
#include <vector>
using std::vector;
using std::string;
using std::map;
using std::set;

#include "VariablePackage.h"
#include "easymutex.h"

#include "PlatformCTP.h"


#pragma managed(push,off)



//交易接口
class CPlatformCTPTrade : public CThostFtdcTraderSpi, public CPlatformCTP
{

	//******************************************************************************************
	//	下面定义查询队列处理类
	//******************************************************************************************
	class CQryQueue
	{
	public:
		//******************************************************************************************
		//	查询队列类里使用的宏
		//******************************************************************************************
		enum eQryID
		{
			QryID_ReqQryNotice=							1001,	//请求查询客户通知
			QryID_ReqQrySettlementInfoConfirm,                  //请求查询结算信息确认
			QryID_ReqQrySettlementInfo,							//请求查询结算信息

			QryID_ReqQryInstrument,                             //查询合约信息
			QryID_ReqQryOrder,                                  //查询报单
			QryID_ReqQryTrade,                                  //查询成交
			QryID_ReqQryInvestorPosition,                       //查询持仓汇总
			QryID_ReqQryInvestorPositionDetail,                 //查询持仓明细
			QryID_ReqQryInvestorPositionCombineDetail,          //请求查询投资者持仓明细(组合)
			QryID_ReqQryTradingAccount,                         //查询资金

			QryID_ReqQryInstrumentCommissionRate,               //查询合约手续费率
			QryID_ReqQryInstrumentMarginRate                    //请求查询合约保证金率
		};


	public:
		CQryQueue(CPlatformCTPTrade& RefTrade);
		~CQryQueue(void);

		void ClearbInQry(void) { m_bInQry=false; }

		//是否到达查询时刻。ctp规定每秒只能有一次查询 
		bool IsQryTime(void);
		bool IsCacheEmpty(void);
		//发送查询请求，返回值表示发送是否成功
		//ReqRlt表示如果进行了查询，查询返回值，<0表示查询失败
		bool SendQryCmd(int& ReqRlt);
		//线程函数
		static unsigned int ThreadProc(void *arg);

	public:
		CPlatformCTPTrade&	m_RefTrade;

		CVariablePackage	m_ReqQueue;							//查询队列。队列按先进先出顺序查询
		bool				m_bInQry;							//是否在查询途中
		unsigned int		m_QryTime;							//查询时间(tickcount)

		bool				m_bNeedExit;                        //线程是否需要退出
		int					m_hThread;                          //线程句柄

		map<string,unsigned int> m_QryCommissionRateTime;		//同一个合约，查询一次后，必须过60s才再次查询
		map<string,int> m_QryCommissionRateCount;
		map<string,unsigned int> m_QryMarginRateTime;
		map<string,int> m_QryMarginRateCount;
	};

public:
    //******************************************************************************************
    //  ConnParam:              连接参数
    //  bPushCache:             是否需要将ctp返回的数据放到m_Cache中，等待其它线程处理(类似于FastTrader处理方式)
    //  bUserComb:              是否使用组合单功能
    //  bCalcuCombQuotOnLegQuot:针对组合单，是否根据单腿合约行情自动生成组合合约行情
    //  Ver_String:             版本号，如"1,6,0,13"
	//	bAutoReqSettlementInfoConfirmed:连接过程中是否查询结算确认信息
    //  bAutoConfirmSettlement: 连接过程中是否自动进行结算确认
	//	bAutoQryInstrumentInfo: 登录后自动查询合约信息
    //******************************************************************************************
    CPlatformCTPTrade(const Stru_CTPConnectionParam& ConnParam=Stru_CTPConnectionParam(),
        bool bPushCache=true,bool bUseComb=true,bool bCalcuCombQuotOnLegQuot=true,
        const string& Ver_String=string(""),
		bool bAutoReqSettlementInfoConfirmed=true,
        bool bAutoConfirmSettlement=true,
		bool bAutoQryInstrumentInfo=true);

    virtual ~CPlatformCTPTrade();

    //******************************************************************************************
    //  pSpi:                   spi回调处理对象的指针。一般为NULL，使用本对象进行默认处理即可；
    //                          如果实例化的是CPlatformCTPTrade的子类，pMdSpi应该等于子类对象的指针，
    //                          使子类对象可以进行回调处理
    //******************************************************************************************
    virtual bool Login(const CThostFtdcTraderSpi* pSpi=NULL);
    virtual void Logout(void);



///****************************************************************************
/// 下面是ctp指令执行函数。返回零均表示成功，返回其它值表示失败。
///****************************************************************************

public:
	///用户口令更新请求
	int ReqUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate);
	///资金账户口令更新请求
	int ReqTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate);

	///报单录入请求. 返回时pInputOrder中的RequestID、BrokerID、InvestorID、UserID、OrderRef的值被修改为底层确定的值
	int ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder);
	///报单操作请求
	int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction);

	///投资者结算结果确认
	int ReqSettlementInfoConfirm() ;



	///请求查询投资者结算结果
	int ReqQrySettlementInfo(CThostFtdcQrySettlementInfoField *pQrySettlementInfo) ;
	///请求查询结算信息确认
	int ReqQrySettlementInfoConfirm(CThostFtdcQrySettlementInfoConfirmField *pQrySettlementInfoConfirm) ;
	///请求查询客户通知
	int ReqQryNotice(CThostFtdcQryNoticeField *pQryNotice) ;

	///请求查询合约
	int ReqQryInstrument(CThostFtdcQryInstrumentField *pQryInstrument) ;
	///请求查询报单
	int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder) ;
	///请求查询成交
	int ReqQryTrade(CThostFtdcQryTradeField *pQryTrade) ;
	///请求查询投资者持仓
	int ReqQryInvestorPosition(CThostFtdcQryInvestorPositionField *pQryInvestorPosition) ;
	///请求查询投资者持仓明细
	int ReqQryInvestorPositionDetail(CThostFtdcQryInvestorPositionDetailField *pQryInvestorPositionDetail) ;
	///请求查询投资者组合持仓明细
	int ReqQryInvestorPositionCombineDetail(CThostFtdcQryInvestorPositionCombineDetailField *pQryInvestorPositionCombineDetail) ;
	///请求查询资金账户
	int ReqQryTradingAccount(CThostFtdcQryTradingAccountField *pQryTradingAccount) ;

	///请求查询合约保证金率
	int ReqQryInstrumentMarginRate(CThostFtdcQryInstrumentMarginRateField *pQryInstrumentMarginRate) ;
	///请求查询合约手续费率
	int ReqQryInstrumentCommissionRate(CThostFtdcQryInstrumentCommissionRateField *pQryInstrumentCommissionRate) ;





///****************************************************************************
/// 下面是ctp回调函数
///****************************************************************************

protected:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
	virtual void OnFrontDisconnected(int nReason);
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);
#ifdef _NEED_AUTHENTICATE
	///客户端认证响应
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
#endif


	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    ///请求查询结算信息确认响应
	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///用户口令更新请求响应
	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///资金账户口令更新请求响应
	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;





	///请求查询报单响应
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询成交响应
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询投资者响应
	virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询合约手续费率响应
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


	///请求查询投资者结算结果响应
	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///请求查询客户通知响应
	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;


	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) ;

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) ;

	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo) ;

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo) ;

	///合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus) ;

	///交易通知
	virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) ;




///****************************************************************************
/// 下面是成员变量
///****************************************************************************

private:
	string					m_Ver_String;						//版本字符串，类似于"1,6,0,13"
	bool					m_bAutoReqSettlementInfoConfirmed;	//是否自动查询结算确认信息
    bool                    m_bAutoConfirmSettlement;			//是否自动确认结算结果
	bool					m_bAutoQryInstrumentInfo;			//登录后自动查询合约信息

	int				        m_CurOrderRef;						//当前报单引用编号

    CThostFtdcTraderApi*    m_pTradeApi;						//交易接口对象，由综合交易平台的接口提供
	CQryQueue*				m_pQryQueue;						//查询队列处理对象

	bool				    m_bSettlementInfoConfirmed;			//是否已经结算确认

    CThostFtdcRspUserLoginField m_LoginInfo;					//登录信息。登录响应时得到



private:
    CPlatformCTPTrade(const CPlatformCTPTrade&);
	CPlatformCTPTrade& operator= (const CPlatformCTPTrade&);
};


#pragma managed(pop)


#endif

