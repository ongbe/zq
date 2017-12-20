#pragma once
#include "Interface_UserOp.h"
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

class CSubUserOp:public IInterfaceUserOp
{
public:
	CSubUserOp() 
	{
		mpPlatformInstruments = NULL;   //和约	
		mpPlatformQutos = NULL;         //行情
		mpPlatformDataRates = NULL;           //费率

		mpPlatformOrders = NULL;            //报单
		mpPlatformTraders = NULL;           //成交
		mpPlatformTraderTotal = NULL; //成交汇总

		mpPlatformPositionDetailCombs = NULL; //组合持仓汇总
		mpPlatformPositionsCombs = NULL;            //组合持仓
		mpPlatformPositions = NULL;                     //持仓
		mpPlatformPositionDetail = NULL;          //持仓明细

		mpPlatformTraderAccount = NULL;           //资金	
		SetUserCTPStatus(ACCOUNT_STATUS_UnInit);
		meUserInitStatus = USER_STATUS_UnInit;
		meSettlementConfirmedReady = DataReday_NoReady;
		mdbFirstFund = 0.0f;
		mbQryOrderLast = false;
		mbQryTradeLast = false;  

	
		mnLastSaveOrderUpdateSeq = 1;
		mnLastSaveTraderUpdateSeq= 1;
	
		mbaseFTID = mbaseUpdateSeq = 1;
		mbSynFinished = false;
	}
	~CSubUserOp();
public:
	virtual void SetPlatFormInstrument(CPlatformDataMgr_Instruments * npInstrument)
	{
		CSafeLock ls(mMutexData);
		mpPlatformInstruments = npInstrument;	
		SetUserInitStatus(USER_STATUS_InitQryInstrumentsReady);
	}
	virtual void SetPlatFormQutos(CPlatformDataMgr_Quots * npQutos)
	{
		CSafeLock ls(mMutexData);
		mpPlatformQutos = npQutos;
	}
	virtual void SetPlatFormRates(CPlatformDataMgr_Rates * npRates)
	{
		CSafeLock ls(mMutexData);
		SetUserInitStatus(USER_STATUS_InitQryRatesReady);
		mpPlatformDataRates = npRates;
		if(mpPlatformOrders != NULL)
		{			
			return;
		}

		//InterlockedExchange((volatile long*)(&meUserInitStatus),USER_STATUS_InitQryRatesReady);
		SetUserInitStatus(USER_STATUS_InitQryRatesReady);


		if(mpPlatformInstruments  && mpPlatformDataRates && mpPlatformQutos )
		{
			//临时合约
			//mpPlatformQutos = new CPlatformDataMgr_Quots(*mpPlatformInstruments);

			mpPlatformOrders = new CPlatformDataMgr_Orders(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformDataRates,
				*mpPlatformQutos,3,3,true);
			mpPlatformTraders = new CPlatformDataMgr_Trades(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformDataRates);
			mpPlatformTraderTotal = new CPlatformDataMgr_TradeTotals(mbaseFTID,mbaseUpdateSeq,*mpPlatformTraders);


			mpPlatformPositionDetailCombs = new CPlatformDataMgr_PositionDetailCombs(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformQutos,			
				*mpPlatformTraders,
				*mpPlatformOrders);

			mpPlatformPositionDetail = new CPlatformDataMgr_PositionDetails(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformDataRates,
				*mpPlatformQutos,
				*mpPlatformTraders,
				*mpPlatformOrders,
				*mpPlatformPositionDetailCombs,3,0,1,0,0,1);

			mpPlatformPositions = new CPlatformDataMgr_Positions(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformDataRates,
				*mpPlatformQutos,
				*mpPlatformTraders,
				*mpPlatformOrders,
				*mpPlatformPositionDetail);

			mpPlatformPositionsCombs = new CPlatformDataMgr_PositionCombs(mbaseFTID,mbaseUpdateSeq,*mpPlatformInstruments,
				*mpPlatformQutos,
				*mpPlatformTraders,
				*mpPlatformOrders,
				*mpPlatformPositionDetailCombs,
				*mpPlatformPositionDetail);

			mpPlatformTraderAccount = new CPlatformDataMgr_TradingAccount(*mpPlatformInstruments,
				*mpPlatformQutos,
				*mpPlatformTraders,	
				*mpPlatformOrders,	
				*mpPlatformPositionDetailCombs,
				*mpPlatformPositionDetail,
				*mpPlatformPositionsCombs,
				*mpPlatformPositions);
		}
	}

public:
	//行情到来引起的计算
	virtual bool CalulateByNewQuto(const  PlatformStru_DepthMarketData & nQutosData,bool nbSettlement);
public:
	virtual int GetMarginRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentMarginRate& outData) ;

	virtual int GetCommissionRate(const std::string& InstrumentID, 
		PlatformStru_InstrumentCommissionRate& outData) ;

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

	//检查报单编号是否是我们的交易客户端下的
	virtual bool CheckTradeOrderSysIDIsOwn(const std::string & nsOrderSysId) ;
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


	//获取持仓汇总，返回数据项的数量
	virtual CF_ERROR  GetPositionVolume(
		const std::string & nInstrumentID,
		int& nVolume) ;

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

public: //组合持仓汇总操作
	//获取组合持仓，返回数据项的数量
	virtual CF_ERROR  GetPositionComb(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_Position>& outData) ;

public: //组合持仓明细操作

	//获取组合持仓明细，返回数据项的数量
	virtual CF_ERROR  GetPositionCombDetail(
		const std::string & nInstrumentID,
		std::vector<PlatformStru_PositionDetailComb>& outData) ;

public: //资金操作
	virtual CF_ERROR SetQryFundInfo(const PlatformStru_TradingAccountInfo& inData) ;
	//获取交易员资金信息
	virtual CF_ERROR  GetFundInfo(PlatformStru_TradingAccountInfo& outData) ;
	//出入金
	virtual CF_ERROR UserFundInOut(const sFundInOut& nFundInOut) ;

public://汇总操作

	//获取所有的持仓明细
	virtual CF_ERROR  GetAllPositionDetail(
		std::vector<PlatformStru_PositionDetail> & outData) ;

	//获取所有的持仓
	virtual CF_ERROR  GetAllPosition(
		std::vector<PlatformStru_Position>& outData) ;
public:
	//结算的时候重新计算成交
	virtual CF_ERROR RecalculateTrade(const std::vector<PlatformStru_TradeInfo>& outData,
		bool bUserInputTrader ) ;
public:
	//设置用户的CTP链接状态
	virtual CF_ERROR SetUserCTPStatus(const EnumAccountStatus & nStatus) 
	{
		InterlockedExchange((volatile long*)(&meUserCTPStatus),nStatus);
		mMutexTime.lock();		
		time(&mtimeCtp);	
		mMutexTime.unlock();
		return CF_ERROR_SUCCESS;
	}

	//获取用户的CTP链接状态
	//nDiffTime 返回进入该状态的时间
	virtual EnumAccountStatus GetUserCTPStatus(double & nDifftime) 
	{
		EnumAccountStatus leStatus = ACCOUNT_STATUS_UnInit;
		InterlockedExchange((volatile long*)(&leStatus),meUserCTPStatus);
		mMutexTime.lock();
		time_t lTime;
		time(&lTime);	
		nDifftime = difftime(lTime, mtimeCtp);
		mMutexTime.unlock();
		return leStatus;
	}

	//获取用户初始化状态
	virtual eUserInitStatus GetUserInitStatus()
	{
		eUserInitStatus leStatus = USER_STATUS_UnInit;
		InterlockedExchange((volatile long*)(&leStatus),meUserInitStatus);
		return leStatus;
	}

	virtual void SetUserInitStatus(eUserInitStatus neStatus) 	
	{
		_InterlockedOr((volatile long*)(&meUserInitStatus),neStatus);
	}


	virtual void SetEqualUserInitStatus(eUserInitStatus neStatus) 
	{
		InterlockedExchange((volatile long*)(&meUserInitStatus),neStatus);		
	}
public:
	virtual void ClearAll();
	//清楚持仓，持仓明细，资金和成交的交易数据
	virtual void ClearTraderAndPosition() ;
public:
	//读取日初数据设置
	virtual CF_ERROR SetInitFund(const double & ndbVal) ;
	//获取初始总权益
	virtual CF_ERROR GetFirstFund(double & ndbVal);

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
	//结算单是否确认
	virtual bool IsSettlementComfirmed(const std::string & nsValidateDate,
		std::string & nsConfirmedDate,
		std::string & nsConfirmedTime) 
	{
		CSafeLock ll(mMutexData);
		eDataReady leDataReady = DataReday_NoReady;
		InterlockedExchange((volatile long*)(&leDataReady),meSettlementConfirmedReady);

		
		nsConfirmedDate = msConfirmDate;
		nsConfirmedTime = msConfirmTime;

		return leDataReady == DataReady_Ready ;
	}

	//设置结算单是否确认
	virtual void SetSettlementConfirmed(bool  nbVal,
		const std::string & nsValidateDate,
		const std::string & nsConfirmedDate,
		const std::string & nsConfirmedTime) 
	{
		CSafeLock ll(mMutexData);
		eDataReady leDataReady = nbVal ? DataReady_Ready : DataReday_NoReady;
		InterlockedExchange((volatile long*)(&meSettlementConfirmedReady),leDataReady);	
		msConfirmDate = nsConfirmedDate;
		msConfirmTime = nsConfirmedTime;
	}

	bool IsSyncFinished() {
		bool lbVal = false;
		CSafeLock ll(mMutexData);
		lbVal = mbSynFinished;
		return lbVal;
	}
	void SetSyncFinished(bool bVal) {
		CSafeLock ll(mMutexData);
		mbSynFinished = bVal;
	}
public:
	//结算时将所有的未成交，部分成交，待审核的报单全部改为已撤单
	virtual void CancelAllUnkonwnOrders() ;


	//查询完成后，更新所有挂单对应的持仓冻结项
	virtual void UpdateQryWaitOrderPosition() ;

private: //引用外部的地址
	CPlatformDataMgr_Instruments *mpPlatformInstruments;   //和约	
	CPlatformDataMgr_Quots       *mpPlatformQutos;         //行情
	CPlatformDataMgr_Rates *mpPlatformDataRates;           //费率

	EnumAccountStatus     meUserCTPStatus;                    //用户关联的账户连接到CTP的状态
	time_t                mtimeCtp;                           //进入该状态的时间
	Ceasymutex			  mMutexTime;                         //保护时间的锁

	eDataReady            meSettlementConfirmedReady;         //结算单是否确认
	std::string           msConfirmDate;                      //结算日
	std::string			  msConfirmTime;                      //结算日期


	eUserInitStatus     meUserInitStatus;                    //用户数据初始化状态
	double              mdbFirstFund;                        //初始总权益，所有该账户出入金的总和
	bool                mbQryOrderLast;                      //是否最后一条报单到了
	bool                mbQryTradeLast;                      //是否最后一条成交到了
private:
	volatile long mbaseFTID;
	volatile long mbaseUpdateSeq;
	CPlatformDataMgr_Orders *mpPlatformOrders;            //报单
	CPlatformDataMgr_Trades *mpPlatformTraders;           //成交
	CPlatformDataMgr_TradeTotals * mpPlatformTraderTotal; //成交汇总

	CPlatformDataMgr_PositionDetailCombs * mpPlatformPositionDetailCombs; //组合持仓汇总
	CPlatformDataMgr_PositionCombs * mpPlatformPositionsCombs;            //组合持仓
	CPlatformDataMgr_Positions * mpPlatformPositions;                     //持仓
	CPlatformDataMgr_PositionDetails * mpPlatformPositionDetail;          //持仓明细

	CPlatformDataMgr_TradingAccount         *mpPlatformTraderAccount;           //资金

	Ceasymutex mMutexData;


private:
	bool mbSynFinished;	
	int mnLastSaveOrderUpdateSeq;
	int mnLastSaveTraderUpdateSeq;
	
};