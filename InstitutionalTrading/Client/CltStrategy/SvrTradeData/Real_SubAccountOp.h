#pragma once
#include "Interface_AccountOp.h"
#include "PlatformDataMgr_Orders.h"
#include "PlatformDataMgr_Trades.h"
#include "PlatformDataMgr_PositionCombs.h"
#include "PlatformDataMgr_PositionDetailCombs.h"
#include "PlatformDataMgr_Positions.h"
#include "PlatformDataMgr_PositionDetails.h"
#include "PlatformDataMgr_TradeTotals.h"
#include "PlatformDataMgr_TradingAccount.h"
#include "SafeLock.h"
#include "intrin.h"



class CSubAccountOp:public IInterfaceAccountOp
{
public:
	CSubAccountOp() 
	{
		
		mpPlatformDataRates = new CPlatformDataMgr_Rates();           //费率	
		mpPlatformInstruments = NULL;   //和约	
		mpPlatformQutos = NULL;         //行情
	
		mpPlatformOrders = NULL;            //报单
		mpPlatformTraders = NULL;           //成交
		mpPlatformTraderTotal = NULL; //成交汇总

		mpPlatformPositionDetailCombs = NULL; //组合持仓汇总
		mpPlatformPositionsCombs = NULL;            //组合持仓
		mpPlatformPositions = NULL;                     //持仓
		mpPlatformPositionDetail = NULL;          //持仓明细

		mpPlatformTraderAccount = NULL;           //资金	

		meAccountInitStatus = USER_STATUS_UnInit;
		SetAccountCTPStatus(ACCOUNT_STATUS_UnInit);
		ClearAll();
	
		mnLastSaveOrderUpdateSeq = 1;
		mnLastSaveTraderUpdateSeq= 1;
	
		mbaseFTID = mbaseUpdateSeq = 1;
		mbSynFinished = false;
	
	}
	~CSubAccountOp();
public:		
	virtual CPlatformDataMgr_Rates* GetPlatFormRates();
public:

	//设置合约	
	virtual void SetPlatFormInstrument(CPlatformDataMgr_Instruments * npInstrument)
	{
		CSafeLock ls(mMutexData);
		mpPlatformInstruments = npInstrument;	
		SetAccountInitStatus(USER_STATUS_InitQryInstrumentsReady);
	}
	//设置行情
	virtual void SetPlatFormQutos(CPlatformDataMgr_Quots * npQutos)
	{
		CSafeLock ls(mMutexData);
		mpPlatformQutos = npQutos;
	}

public:
	///设置合约保证金率
	virtual void SetMarginRate(const std::string& InstrumentID,
		PlatformStru_InstrumentMarginRate& outData) ;

	virtual int GetMarginRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) ;

	///设置合约手续费率
	virtual void SetCommissionRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;

	virtual int GetCommissionRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;


	virtual eCommissionMarginReady GetCommissonMarginReady();

	virtual void SetCommissionMarginReady(const eCommissionMarginReady & nReady) ;

public://行情操作
	virtual bool CalulateByNewQuto(const PlatformStru_DepthMarketData & nQutosData,bool nbSettlement);

public: //保单操作

	//查询报单的到来
	virtual CF_ERROR QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
		const int ErrID,
		const bool bIsLast);

	//追加一个报单
	virtual CF_ERROR PushOrder( PlatformStru_OrderInfo & nOrder);
	//

	///获取指定报单
	virtual CF_ERROR GetOrderByKey(const OrderKey &nOrderKey,
		PlatformStru_OrderInfo& outData) ;

	///获取全部报单
	virtual CF_ERROR GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////获取全部已触发的报单
	virtual CF_ERROR GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////获取已成交报单
	virtual CF_ERROR GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////获取已撤单和错误报单
	virtual CF_ERROR GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////获取未成交报单
	virtual CF_ERROR GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData);

	/////获取未知状态的报单
	virtual CF_ERROR GetUnkownOrders(std::vector<PlatformStru_OrderInfo>& outData) ;

public:// 成交操作
	//查询成交的到来
	virtual CF_ERROR QryTradeReach(PlatformStru_TradeInfo& OrderInfo,
		const int ErrID,
		const bool bIsLast) ;
	//追加一个成交
	virtual CF_ERROR PushTrader( PlatformStru_TradeInfo & nTrader);

	//获取全部成交单，返回数据项的数量
	virtual CF_ERROR GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData) ;


	//测试成交是否已存在
	virtual bool TestTradeInfoExist(const TradeKey &key) ;
public://持仓汇总操作

	//日初查询持仓的到来
	virtual CF_ERROR  QryPositionReach( 
		PlatformStru_Position& PositionInfo,
		int ErrID,
		bool bIsLast) ;
	//获取持仓汇总，返回数据项的数量
	virtual CF_ERROR  GetPositions(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData,
		bool bNeedZeroVolumePosition = false) ;



	//判断是否有持仓
	virtual bool HasPositions() ;

public: //持仓明细汇总

	//日初查询持仓明细的到来
	virtual CF_ERROR  QryPositionDetailReach( 
		PlatformStru_PositionDetail& PositionDetailInfo,
		int ErrID,
		bool bIsLast) ;
	//获取持仓明细，返回数据项的数量
	virtual CF_ERROR  GetPositionDetail(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetail>& outData) ;

	//获取持仓明细，返回数据项的数量
	virtual CF_ERROR  GetPositionDetail(const PositionDetailKey & nKey,
		PlatformStru_PositionDetail& outData) ;


	//获取某个时间段的持仓明细
	virtual CF_ERROR  GetPositionDetail(int nLowtime,int nUpperTime,
		std::vector<PlatformStru_PositionDetail>& outData) ;



public: //资金操作
	virtual CF_ERROR SetQryFundInfo(const PlatformStru_TradingAccountInfo& inData) ;
	//获取交易员资金信息
	virtual CF_ERROR  GetFundInfo(PlatformStru_TradingAccountInfo& outData) ;


public:

	virtual void SetLoadOrderUpdateSeq(int nSeq) {if(nSeq > mnLastSaveOrderUpdateSeq) mnLastSaveOrderUpdateSeq = nSeq;};
	virtual void SetLoadTraderUpdateSeq(int nSeq){if(nSeq > mnLastSaveTraderUpdateSeq) mnLastSaveTraderUpdateSeq = nSeq;};

	virtual void GetLoadOrderUpdateSeq(int& nSeq) {nSeq = mnLastSaveOrderUpdateSeq;};
	virtual void GetLoadTraderUpdateSeq(int& nSeq){nSeq = mnLastSaveTraderUpdateSeq;};
	

	virtual void GetOrderAndTradeGeSeq( int nOrderSeq,
		int nTraderSeq,
		std::vector<PlatformStru_OrderInfo> & nvecOrder,
		std::vector<PlatformStru_TradeInfo> & nvecTrader) ;

public: 

	//设置用户的CTP链接状态
	virtual CF_ERROR SetAccountCTPStatus(const EnumAccountStatus & nStatus) ;

	//获取用户的CTP链接状态
	//获取用户的CTP链接状态
	//nDiffTime 返回进入该状态的时间
	virtual EnumAccountStatus GetAccountCTPStatus(double & nDifftime) ; 	



	//获取用户初始化状态
	virtual eUserInitStatus GetAccountInitStatus()
	{
		eUserInitStatus leStatus = USER_STATUS_UnInit;
		InterlockedExchange((volatile long*)(&leStatus),meAccountInitStatus);
		return leStatus;
	}

	virtual void SetAccountInitStatus(eUserInitStatus neStatus) 	
	{
		//_InterlockedOr((volatile long*)(&meAccountInitStatus),neStatus);
	}


	virtual void SetEqualAccountInitStatus(eUserInitStatus neStatus) 
	{
		InterlockedExchange((volatile long*)(&meAccountInitStatus),neStatus);		
	}
	virtual void clear() ;

public://汇总操作
	//获取所有的持仓明细
	virtual CF_ERROR  GetAllPositionDetail(
		std::vector<PlatformStru_PositionDetail> & outData) ;

	//获取所有的持仓
	virtual CF_ERROR  GetAllPosition(
		std::vector<PlatformStru_Position>& outData) ;

public:
	//清楚所有交易数据
	virtual void ClearAll() ;

	//清楚持仓，持仓明细，资金和成交的交易数据
	virtual void ClearTraderAndPosition() ;


public:
	//结算时将所有的未成交，部分成交，待审核的报单全部改为已撤单
	virtual void CancelAllUnkonwnOrders() ;

	//查询完成后，更新所有挂单对应的持仓冻结项
	virtual void UpdateQryWaitOrderPosition() ;
public:
	bool IsSyncFinished() {return mbSynFinished;}
	void SetSyncFinished(bool bVal) {mbSynFinished = bVal;}
private: //引用外部的地址	
	CPlatformDataMgr_Instruments *mpPlatformInstruments;   //和约	
	CPlatformDataMgr_Quots       *mpPlatformQutos;         //行情

	CPlatformDataMgr_Rates *mpPlatformDataRates;           //费率

	EnumAccountStatus     meCTPStatus;                    //用户关联的账户连接到CTP的状态
	time_t                mtimeCtp;                           //进入该状态的时间
	Ceasymutex			  mMutexTime;                         //保护时间的锁

	eUserInitStatus meAccountInitStatus;

	eCommissionMarginReady meReady;                       //费率是否准备好了

	CPlatformDataMgr_Orders *mpPlatformOrders;            //报单
	CPlatformDataMgr_Trades *mpPlatformTraders;           //成交
	CPlatformDataMgr_TradeTotals * mpPlatformTraderTotal; //成交汇总

	CPlatformDataMgr_PositionDetailCombs * mpPlatformPositionDetailCombs; //组合持仓汇总
	CPlatformDataMgr_PositionCombs * mpPlatformPositionsCombs;            //组合持仓
	CPlatformDataMgr_Positions * mpPlatformPositions;                     //持仓
	CPlatformDataMgr_PositionDetails * mpPlatformPositionDetail;          //持仓明细

	CPlatformDataMgr_TradingAccount         *mpPlatformTraderAccount;           //资金

	volatile long mbaseFTID;
	volatile long mbaseUpdateSeq;
private:


	Ceasymutex mMutexData;

private:
	bool mbSynFinished;  

	int mnLastSaveOrderUpdateSeq;
	int mnLastSaveTraderUpdateSeq;


};