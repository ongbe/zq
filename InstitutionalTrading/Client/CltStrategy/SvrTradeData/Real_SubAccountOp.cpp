#include "stdafx.h"
#include "Real_SubAccountOp.h"
#include "CommonMacro.h"
#include "CommonDataChangeEvent.h"
//#include "..\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"


void CSubAccountOp::clear()
{
	if(mpPlatformDataRates)
		mpPlatformDataRates->clear();
	InterlockedExchange((volatile long*)(&meReady),CommissionMargin_NoReady);	
	SetAccountCTPStatus(ACCOUNT_STATUS_UnInit);
}


CSubAccountOp::~CSubAccountOp()
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraderAccount)
		delete mpPlatformTraderAccount;
	if(mpPlatformPositionsCombs)
		delete mpPlatformPositionsCombs;
	if(mpPlatformPositions)
		delete mpPlatformPositions;
	if(mpPlatformPositionDetail)
		delete mpPlatformPositionDetail;
	if(mpPlatformPositionDetailCombs)
		delete mpPlatformPositionDetailCombs;
	if(mpPlatformTraderTotal)
		delete mpPlatformTraderTotal;
	if(mpPlatformTraders)
		delete mpPlatformTraders;
	if(mpPlatformOrders)
		delete mpPlatformOrders;
}

//设置用户的CTP链接状态
CF_ERROR CSubAccountOp::SetAccountCTPStatus(const EnumAccountStatus & nStatus)
{
	InterlockedExchange((volatile long*)(&meCTPStatus),nStatus);
	mMutexTime.lock();		
	time(&mtimeCtp);	
	mMutexTime.unlock();
	return CF_ERROR_SUCCESS;
}

//获取用户的CTP链接状态
//nDiffTime 返回进入该状态的时间
EnumAccountStatus CSubAccountOp::GetAccountCTPStatus(double & nDifftime) 
{
	EnumAccountStatus leStatus = ACCOUNT_STATUS_UnInit;
	InterlockedExchange((volatile long*)(&leStatus),meCTPStatus);
	mMutexTime.lock();
	time_t lTime;
	time(&lTime);	
	nDifftime = difftime(lTime, mtimeCtp);
	mMutexTime.unlock();
	return leStatus;
}


eCommissionMarginReady CSubAccountOp::GetCommissonMarginReady() 
{
	eCommissionMarginReady leStatus = CommissionMargin_NoReady;
	InterlockedExchange((volatile long*)(&leStatus),meReady);
	return leStatus;
}

void CSubAccountOp::SetCommissionMarginReady(const eCommissionMarginReady & nReady) 
{
	//_InterlockedOr((volatile long*)(&meReady),nReady);
	//InterlockedExchange((volatile long*)(&meUserInitStatus),USER_STATUS_InitQryRatesReady);
	SetAccountInitStatus(USER_STATUS_InitQryRatesReady);


	if(mpPlatformInstruments  && mpPlatformDataRates && mpPlatformQutos && mpPlatformOrders == NULL)
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

///设置合约保证金率
void CSubAccountOp::SetMarginRate(const std::string& InstrumentID,
						   PlatformStru_InstrumentMarginRate& outData) 
{
	CSafeLock lLock(mMutexData);
	if(mpPlatformDataRates)
		mpPlatformDataRates->SetMarginRate(InstrumentID,outData);

}

int CSubAccountOp::GetMarginRate(const std::string& InstrumentID, 
						  PlatformStru_InstrumentMarginRate& outData) 
{

	CSafeLock lLock(mMutexData);
	if(mpPlatformDataRates)
		return mpPlatformDataRates->GetMarginRate(InstrumentID,outData);
	
	return -1;
}

///设置合约手续费率
void CSubAccountOp::SetCommissionRate(const std::string& InstrumentID, 
							   PlatformStru_InstrumentCommissionRate& outData) 
{
	CSafeLock lLock(mMutexData);
	if(mpPlatformDataRates)
		mpPlatformDataRates->SetCommissionRate(InstrumentID,outData);
}

int CSubAccountOp::GetCommissionRate(const std::string& InstrumentID, 
							  PlatformStru_InstrumentCommissionRate& outData) 
{
	CSafeLock lLock(mMutexData);
	if(mpPlatformDataRates)
		return mpPlatformDataRates->GetCommissionRate(InstrumentID,outData);

	return -1;
}

CPlatformDataMgr_Rates* CSubAccountOp::GetPlatFormRates()
{
	return mpPlatformDataRates;
}

//行情到来引起的计算
bool CSubAccountOp::CalulateByNewQuto(const PlatformStru_DepthMarketData & nQutosData,bool nbSettlement)
{
	bool lbRet = false;

	do 
	{
		eUserInitStatus leStatus = GetAccountInitStatus();
		if( mpPlatformInstruments)
		{
			PlatformStru_InstrumentInfo InstrumentInfo;		//取合约信息
			if(!mpPlatformInstruments->GetInstrumentInfo(nQutosData.InstrumentID,InstrumentInfo))
				break;

			if(mpPlatformPositionDetail == NULL)
				break;

			if(!mpPlatformPositionDetail->HavePositionDetail(nQutosData.InstrumentID))
				break;

			if(nbSettlement == false)
			{
				if(mpPlatformPositionDetail)
					mpPlatformPositionDetail->UpdatePositionDetail_RtnDepthMarketData(nQutosData,InstrumentInfo);

				if(mpPlatformPositions)
					mpPlatformPositions->UpdatePosition_RtnQuot(nQutosData,InstrumentInfo,true);

				if(mpPlatformTraderAccount)
					mpPlatformTraderAccount->DynamicUpdateTradingAccount_RtnDepthMarketData(nQutosData.InstrumentID);
			}
			else
			{

				if(mpPlatformPositionDetail)
					mpPlatformPositionDetail->UpdatePositionDetail_OnSettlement();

				if(mpPlatformPositions)
					mpPlatformPositions->UpdatePosition_OnSettlement();

				if(mpPlatformTraderAccount)
					mpPlatformTraderAccount->DynamicUpdateTradingAccount_RtnDepthMarketData(nQutosData.InstrumentID);
			}
			lbRet = true;
		}
		break;
	} while (true);

	return lbRet;
}

//查询报单的到来
CF_ERROR  CSubAccountOp::QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
									   const int ErrID,
									   const bool bIsLast)
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	else
	{
		
		mpPlatformOrders->QryOrderReach(OrderInfo,ErrID,bIsLast);
		//if(mpPlatformPositions && mpPlatformTraderAccount)
		//{
		//	//更新单合约持仓冻结和平仓量。
		//	mpPlatformPositions->UpdatePositionFreezeFields_RtnOrder(OrderInfo, OldOrderInfo);

		//	//组合合约报单 : 更新组合合约持仓冻结和平仓量
		//	/*if(bComb)
		//	mpPlatformPositionsCombs->UpdatePositionCombFreezeFields_RtnOrder(nOrder, OldOrderInfo);*/

		//	//更新资金数据
		//	mpPlatformTraderAccount->DynamicUpdateTradingAccount_RtnOrder(OrderInfo, OldOrderInfo);

		//}

		if(bIsLast)
			SetAccountInitStatus(USER_STATUS_InitQryOrdersReady);

		if(bIsLast )
		{
			UpdateQryWaitOrderPosition();

			////初始化查询完成，根据日初持仓明细、日内成交记录，重新计算成交的平尽量、平仓盈亏、手续费
			//if(mpPlatformPositionDetail)			
			//	mpPlatformPositionDetail->ReCalcuTradeInfo_OnInitQryCompleted();

			if(mpPlatformTraderAccount)
				mpPlatformTraderAccount->UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
		}
		return CF_ERROR_SUCCESS;
	}

}
CF_ERROR CSubAccountOp::PushOrder( PlatformStru_OrderInfo & nOrder)
{
	CSafeLock ll(mMutexData);
	if(GetAccountInitStatus() != USER_STATUS_Inited ||
		mpPlatformOrders == NULL)
	{
		TRADE_LOG("PushOrder GetAccountInitStatus NotInit ，Discard this order BrokerID:%s UserID %s",
			nOrder.BrokerID,nOrder.InvestorID);
		return CF_ERROR_NOINITED;
	}



	std::string strInstrumentID(nOrder.InstrumentID);
	PlatformStru_InstrumentInfo InstrumentInfo;
	if(!mpPlatformInstruments->GetInstrumentInfo(strInstrumentID,InstrumentInfo))
		return CF_ERROR_TRADEDATA_NO_INSTRUMENTS;

	PlatformStru_OrderInfo OldOrderInfo;

	//更新报单数据
	mpPlatformOrders->NewOrderReady(nOrder,OldOrderInfo,InstrumentInfo);

	//检查是否是组合合约报单
	//std::string Leg1InstrumentID,Leg2InstrumentID;
	//bool bComb=GlobalFunc::IsCombInstrument(strInstrumentID,Leg1InstrumentID,Leg2InstrumentID);

	////根据需要触发查询费率。如果是组合合约，按分腿查询
	//if(bComb)
	//{
	//	m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(Leg1InstrumentID);
	//	m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(Leg1InstrumentID);
	//	m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(Leg2InstrumentID);
	//	m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(Leg2InstrumentID);
	//}
	//else
	//{
	//	m_PlatFormService->m_rServiceProxy.GetCommissionRate_TriggerReq(strInstrumentID);
	//	m_PlatFormService->m_rServiceProxy.GetMarginRate_TriggerReq(strInstrumentID);
	//}


	//更新单合约持仓冻结和平仓量。
	mpPlatformPositions->UpdatePositionFreezeFields_RtnOrder(nOrder, OldOrderInfo,InstrumentInfo);

	//组合合约报单 : 更新组合合约持仓冻结和平仓量
	/*if(bComb)
	mpPlatformPositionsCombs->UpdatePositionCombFreezeFields_RtnOrder(nOrder, OldOrderInfo);*/

	//更新资金数据
	mpPlatformTraderAccount->UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData( );


	return CF_ERROR_SUCCESS;
}

///获取指定报单
CF_ERROR CSubAccountOp::GetOrderByKey(const OrderKey &nOrderKey,
							   PlatformStru_OrderInfo& outData)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( false != mpPlatformOrders->GetOrder(nOrderKey,outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;

}
///获取全部报单
CF_ERROR  CSubAccountOp::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
{

	CSafeLock ll(mMutexData);
	if( mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetAllOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;

}
/////获取全部已触发的报单
CF_ERROR  CSubAccountOp::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetTriggerOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

/////获取已成交报单
CF_ERROR  CSubAccountOp::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetTradedOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

/////获取已撤单和错误报单
CF_ERROR  CSubAccountOp::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	CSafeLock ll(mMutexData);
	if( mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetCanceledOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

/////获取未成交报单
CF_ERROR  CSubAccountOp::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetWaitOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

/////获取未知状态的报单
CF_ERROR CSubAccountOp::GetUnkownOrders(std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetUnknownStatusOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}



//查询成交的到来
CF_ERROR CSubAccountOp::QryTradeReach(PlatformStru_TradeInfo& nTrader,
									  const int ErrID,
									  const bool bIsLast) 
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformTraders == NULL)
		return CF_ERROR_NOINITED;	
	else
	{
		//if(bIsLast)	
		//	mbQryTradeLast = true;
		//mpPlatformTraders->QryTradeReach(nTrader,ErrID,bIsLast);

		if(bIsLast)
			SetAccountInitStatus(USER_STATUS_InitQryTradesReady);

		PlatformStru_TradeInfo lTempTrade;
		TradeKey lTradeKey(nTrader);
		if(mpPlatformTraders->GetTradeInfo(lTradeKey,lTempTrade))
		{
			TRADE_LOG("The same trade key exist ，discard this tradeinfo");
			return CF_ERROR_NOINITED;
		}

		std::string strInstrumentID(nTrader.InstrumentID);
		PlatformStru_InstrumentInfo InstrumentInfo;
		if(!mpPlatformInstruments->GetInstrumentInfo(strInstrumentID,InstrumentInfo))
			return CF_ERROR_TRADEDATA_NO_INSTRUMENTS;

		nTrader.TradeCommission = util::GetDoubleInvalidValue();
		nTrader.CloseProfitByTrade = util::GetDoubleInvalidValue();
		nTrader.CloseProfitByDate = util::GetDoubleInvalidValue();


		//处理查询结果，将结果送到数据集中
		mpPlatformTraders->QryTradeReach(nTrader,ErrID,bIsLast);

		if(bIsLast)
		{
			//成交收全后，计算成交统计
			mpPlatformTraderTotal->QryTradeTotalReach(true);

			//更新资金数据
			mpPlatformTraderAccount->UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
		}

		//更新报单数据
		//mpPlatformTraders->NewTradeReady(nTrader,InstrumentInfo);

		//if(mpPlatformPositionDetail && 
		//	mpPlatformPositions && 
		//	mpPlatformPositionDetailCombs 
		//	&& mpPlatformPositionsCombs 
		//	&& mpPlatformTraderAccount)
		//{
		//	//ModifiedPositionDetails保存被成交修改的持仓明细记录
		//	//std::vector<PlatformStru_PositionDetail> ModifiedPositionDetails;
		//	std::vector<PlatformStru_PositionDetailComb> ModifiedPositionDetailsComb;
		//	map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
		//	map<string,int> ComTradeID2CloseVolume;
		//	//更新持仓明细
		//	mpPlatformPositionDetail->UpdatePositionDetail_RtnTrade(nTrader,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume);

  //          //更新成交记录中的平今量，平仓盈亏，重算手续费。
  //          mpPlatformTraders->UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(
		//		nTrader.FTID,
  //              InstrumentInfo,
  //              nTrader.CloseTodayVolume,
  //              nTrader.CloseProfitByDate,
  //              nTrader.CloseProfitByTrade,
		//		nTrader.TradeCommission);

		//	//更新持仓
		//	mpPlatformPositions->UpdatePosition_RtnTrade(nTrader,ModifiedPositionDetails,InstrumentInfo);

		//	//const map<PositionDetailKey,int>& mapCloseDeltaVolume,
		//	//	const map<string,int>& ComTradeID2CloseVolume,
		//	//	map<PositionDetailCombKey,PlatformStru_PositionDetailComb>&ModifiedPositionDetailCombs
		//	////更新组合持仓明细
		//	//mpPlatformPositionDetailCombs->Update_RtnTrade(nTrader,ModifiedPositionDetails,ModifiedPositionDetailsComb);

		//	////更新组合持仓
		//	//mpPlatformPositionsCombs->Update_RtnTrade(nTrader,ModifiedPositionDetailsComb);


		//	//更新资金
		//	mpPlatformTraderAccount->UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
		//}

		return CF_ERROR_SUCCESS;
	}

}
CF_ERROR CSubAccountOp::PushTrader( PlatformStru_TradeInfo & nTrader)
{
	CSafeLock ll(mMutexData);
	if(GetAccountInitStatus() != USER_STATUS_Inited 
		|| mpPlatformTraders == NULL)
	{
		TRADE_LOG("GetAccountInitStatus NotInit ，Discard this Trade BrokerID:%s UserID %s",nTrader.BrokerID,nTrader.InvestorID);
		return CF_ERROR_NOINITED;
	}
	
	PlatformStru_TradeInfo lTempTrade;
	TradeKey lTradeKey(nTrader);
	if(mpPlatformTraders->GetTradeInfo(lTradeKey,lTempTrade))
	{
		TRADE_LOG("The same trade key exist ，discard this tradeinfo");
		return CF_ERROR_NOINITED;
	}

	std::string strInstrumentID(nTrader.InstrumentID);
	PlatformStru_InstrumentInfo InstrumentInfo;
	if(!mpPlatformInstruments->GetInstrumentInfo(strInstrumentID,InstrumentInfo))
		return CF_ERROR_NOINITED;


	////检测是从别的客户端产生的成交
	//if(false == CheckTradeOrderSysIDIsOwn(nTrader.OrderSysID))
	//	return CF_ERROR_TRADEDATA_TRADE_FROM_OTHER;

	//更新报单数据
	mpPlatformTraders->NewTradeReady(nTrader,InstrumentInfo);

	if(mpPlatformPositionDetail && 
		mpPlatformPositions && 
		mpPlatformPositionDetailCombs 
		&& mpPlatformPositionsCombs 
		&& mpPlatformTraderAccount)
	{
		//ModifiedPositionDetails保存被成交修改的持仓明细记录
		
		std::vector<PlatformStru_PositionDetailComb> ModifiedPositionDetailsComb;

		map<PositionDetailKey,PlatformStru_PositionDetail> ModifiedPositionDetails;
		map<string,int> ComTradeID2CloseVolume;
		//更新持仓明细
		mpPlatformPositionDetail->UpdatePositionDetail_RtnTrade(nTrader,InstrumentInfo,ModifiedPositionDetails,ComTradeID2CloseVolume);

        //更新成交记录中的平今量，平仓盈亏，重算手续费。
        mpPlatformTraders->UpdateTradeCloseTodayVolume_CloseProfit_RecalcuCommission(
			nTrader.FTID,
            InstrumentInfo,
            nTrader.CloseTodayVolume,
            nTrader.CloseProfitByDate,
            nTrader.CloseProfitByTrade,
			nTrader.TradeCommission);


		//更新持仓
		mpPlatformPositions->UpdatePosition_RtnTrade(nTrader,ModifiedPositionDetails,InstrumentInfo);

		////更新组合持仓明细
		//mpPlatformPositionDetailCombs->Update_RtnTrade(nTrader,ModifiedPositionDetails,ModifiedPositionDetailsComb);

		////更新组合持仓
		//mpPlatformPositionsCombs->Update_RtnTrade(nTrader,ModifiedPositionDetailsComb);


		//更新资金
		mpPlatformTraderAccount->UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
	}


	return CF_ERROR_SUCCESS;
}

//获取全部成交单，返回数据项的数量
CF_ERROR CSubAccountOp::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraders == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}

	if( mpPlatformTraders->GetAllTrades(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_TRADE;
}



//测试成交是否已存在
bool CSubAccountOp::TestTradeInfoExist(const TradeKey &key)
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraders == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return false;
	}

	PlatformStru_TradeInfo lTemp;
	return  mpPlatformTraders->GetTradeInfo(key,lTemp);

}
//日初查询持仓的到来
CF_ERROR  CSubAccountOp::QryPositionReach( 
	PlatformStru_Position& PositionInfo,
	int ErrID,
	bool bIsLast) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
		return CF_ERROR_NOINITED;	
	else
	{	
		mpPlatformPositions->QryPositionReach(PositionInfo,ErrID,bIsLast);
	
		if(bIsLast)
		{
			mpPlatformPositions->QryPositionReach(PositionInfo,-1,bIsLast);
			if(mpPlatformTraderAccount)
				mpPlatformTraderAccount->UpdateTradingAccount_RtnTrade_RspQryPosition_RspQryTrade_InitQryCompleted();
			
			TRADE_LOG("SetAccountInitStatus QryPosition Ready ");
			SetAccountInitStatus(USER_STATUS_InitQryPositionReady);
		}
		return CF_ERROR_SUCCESS;	
	}
}
//获取持仓汇总，返回数据项的数量
CF_ERROR  CSubAccountOp::GetPositions(const std::string & nInstrumentID,
									  std::vector<PlatformStru_Position>& outData,
									  bool bNeedZeroVolumePosition ) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard thiscall");
		return CF_ERROR_NOINITED;
	}

	int lnRet = -1;
	if(nInstrumentID.empty())
	{
		if(bNeedZeroVolumePosition)
			lnRet = mpPlatformPositions->GetPositions4(outData);
		else
			lnRet = mpPlatformPositions->GetPositions(outData);
		if(lnRet != 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSITION;	
	}
	else
	{
		long lSeq = -1;
		lnRet = mpPlatformPositions->GetPositions3(nInstrumentID,outData,lSeq);
		if(lnRet == 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSITION;	
	}

}

//判断是否有持仓
bool CSubAccountOp::HasPositions() 
{
	bool lbRet = false;
	if(mpPlatformPositions)
	{
		//lbRet = !mpPlatformPositions->empty();
		std::vector<PlatformStru_Position> lVecPos;
		mpPlatformPositions->GetPositions(lVecPos);

		if(lVecPos.size() == 0)
			lbRet = false;
		else
		{
			//过滤持仓量为0的持仓
			std::vector<PlatformStru_Position>::iterator lItr = lVecPos.begin();
			for(;lItr != lVecPos.end();lItr++)
			{
				if(lItr->Position != 0)
				{
				   lbRet = true;
				   break;
				}
			}
		}
	}
	return lbRet;
	
}
//日初查询持仓明细的到来
CF_ERROR  CSubAccountOp::QryPositionDetailReach( 
	PlatformStru_PositionDetail& PositionDetailInfo,
	int ErrID,
	bool bIsLast) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;	
	}
	else
	{	
		mpPlatformPositionDetail->QryPositionDetailReach(PositionDetailInfo,ErrID,bIsLast);
		if(bIsLast)
		{
			TRADE_LOG("SetAccountInitStatus QryPositionDetail Ready ");
			SetAccountInitStatus(USER_STATUS_InitQryPositionDetailReady);
		}
		return CF_ERROR_SUCCESS;	
	}
}
//获取持仓明细，返回数据项的数量
CF_ERROR  CSubAccountOp::GetPositionDetail(const std::string & nInstrumentID,
										   std::vector<PlatformStru_PositionDetail>& outData) 
{
	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}
	int lnRet = -1;
	long lSeq = -1;
	if(nInstrumentID.empty())
	{
		lnRet = mpPlatformPositionDetail->GetPositionDetails( outData,lSeq);
		if(lnRet != 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSDETAIL;	
	}
	else
	{	
		std::set<long> setFTID;
		lnRet = mpPlatformPositionDetail->GetPositionDetails3(nInstrumentID,setFTID,outData,lSeq);
		if(lnRet == 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSDETAIL;	
	}
}
//获取持仓明细，返回数据项的数量
CF_ERROR   CSubAccountOp::GetPositionDetail(const PositionDetailKey & nKey,
									PlatformStru_PositionDetail& outData) 
{
	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}	

	if( mpPlatformPositionDetail->GetPositionDetail(nKey,outData) != false)
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_POSDETAIL;	

}
//获取某个时间段的持仓明细
CF_ERROR  CSubAccountOp::GetPositionDetail(int nLowtime,int nUpperTime,
									std::vector<PlatformStru_PositionDetail>& outData) 
{

	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}	

	mpPlatformPositionDetail->GetPositionDetailsInOpenTimeScope(nLowtime,nUpperTime,outData);
	return CF_ERROR_SUCCESS;
}



//设置日初的资金
CF_ERROR CSubAccountOp::SetQryFundInfo(const PlatformStru_TradingAccountInfo& inData) 
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformTraderAccount == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;	
	}
	else
	{	
		TRADE_LOG("SetAccountInitStatus QryFundInfo Ready ");

		//DataRspQryTradingAccount lRspAccount;
		//memset(&lRspAccount,0,sizeof(lRspAccount));
		//lRspAccount.Head.BID = BID_RspQryTradingAccount;
		//lRspAccount.Head.Size = sizeof(inData);
		//memcpy(&lRspAccount.TradingAccountField,&inData,sizeof(inData));
		//lRspAccount.bIsLast = true;
		mpPlatformTraderAccount->SetTradingAccount(inData);

		SetAccountInitStatus(USER_STATUS_InitQryFundInfoReady);
		return CF_ERROR_SUCCESS;	
	}

}
//获取主帐号资金信息
CF_ERROR  CSubAccountOp::GetFundInfo(PlatformStru_TradingAccountInfo& outData) 
{
	CSafeLock ll(mMutexData);

	if(mpPlatformTraderAccount == NULL)
	{
	//	TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}
	if(0 == mpPlatformTraderAccount->GetTradingAccount(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_FUND;
}


//获取所有的持仓明细
CF_ERROR  CSubAccountOp::GetAllPositionDetail(
	std::vector<PlatformStru_PositionDetail> & outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}


	long lastSq = 0;
	mpPlatformPositionDetail->GetPositionDetails(outData,lastSq);


	return CF_ERROR_SUCCESS;
}

//获取所有的持仓
CF_ERROR  CSubAccountOp::GetAllPosition(
										std::vector<PlatformStru_Position>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
	{
		//TRADE_LOG("GetAccountInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}


	mpPlatformPositions->GetPositions(outData);
	return CF_ERROR_SUCCESS;
}


//清楚持仓，持仓明细，资金和成交的交易数据
void CSubAccountOp::ClearTraderAndPosition()
{
	CSafeLock ll(mMutexData);
	TRADE_LOG("Clear all subaccount TraderAndPosition");

	
	
	if(mpPlatformTraderAccount)
		mpPlatformTraderAccount->clear();
	if(mpPlatformPositionsCombs)
		mpPlatformPositionsCombs->clear();
	if(mpPlatformPositions)
		mpPlatformPositions->clear();
	if(mpPlatformPositionDetail)
		mpPlatformPositionDetail->clear();
	if(mpPlatformPositionDetailCombs)
		mpPlatformPositionDetailCombs->clear();
	if(mpPlatformTraderTotal)
		mpPlatformTraderTotal->clear();
	if(mpPlatformTraders)
		mpPlatformTraders->clear();

}

void  CSubAccountOp::ClearAll()
{
	CSafeLock ll(mMutexData);
	TRADE_LOG("Clear all subaccount info");
	InterlockedExchange((volatile long*)(&meReady),CommissionMargin_NoReady);	
	SetAccountCTPStatus(ACCOUNT_STATUS_UnInit);
	SetEqualAccountInitStatus(USER_STATUS_UnInit);


	mnLastSaveOrderUpdateSeq = 1;
	mnLastSaveTraderUpdateSeq= 1;

	mbaseFTID = mbaseUpdateSeq = 1;
	mbSynFinished = false;
	if(mpPlatformTraderAccount)
		mpPlatformTraderAccount->clear();
	if(mpPlatformPositionsCombs)
		mpPlatformPositionsCombs->clear();
	if(mpPlatformPositions)
		mpPlatformPositions->clear();
	if(mpPlatformPositionDetail)
		mpPlatformPositionDetail->clear();
	if(mpPlatformPositionDetailCombs)
		mpPlatformPositionDetailCombs->clear();
	if(mpPlatformTraderTotal)
		mpPlatformTraderTotal->clear();
	if(mpPlatformTraders)
		mpPlatformTraders->clear();
	if(mpPlatformOrders)
		mpPlatformOrders->clear();


}


//结算时将所有的未成交，部分成交，待审核的报单全部改为已撤单
void CSubAccountOp::CancelAllUnkonwnOrders()
{
	std::vector<PlatformStru_OrderInfo> lvecOrder;
	GetAllOrders(lvecOrder);
	for(unsigned int i = 0; i < lvecOrder.size(); i++)
	{
		if(lvecOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
			lvecOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeQueueing||
			lvecOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing||
			lvecOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing ||
			lvecOrder[i].OrderStatus == THOST_FTDC_OST_Unknown )
		{
			lvecOrder[i].OrderStatus = THOST_FTDC_OST_Canceled;
			strcpy(lvecOrder[i].StatusMsg,"服务器撤单");
			PushOrder(lvecOrder[i]);
		}
	}
	return;
}



//查询完成后，更新所有挂单对应的持仓冻结项
void CSubAccountOp::UpdateQryWaitOrderPosition()
{
	std::vector<PlatformStru_OrderInfo> lvecOrder;
	GetWaitOrders(lvecOrder);
	for(unsigned int i = 0; i < lvecOrder.size(); i++)
	{
		if(mpPlatformPositions)
		{
			PlatformStru_OrderInfo lOrder(true); //构造一个无效的Order
			std::string strInstrumentID(lvecOrder[i].InstrumentID);
			PlatformStru_InstrumentInfo InstrumentInfo;
			if(mpPlatformInstruments->GetInstrumentInfo(strInstrumentID,InstrumentInfo))
			{
				mpPlatformPositions->UpdatePositionFreezeFields_RtnOrder(lvecOrder[i],lOrder,InstrumentInfo);
			}
		}
	}
}

void CSubAccountOp::GetOrderAndTradeGeSeq( int nOrderSeq,
								   int nTraderSeq,
								   std::vector<PlatformStru_OrderInfo> & nvecOrder,
								   std::vector<PlatformStru_TradeInfo> & nvecTrader) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraders && mpPlatformOrders)
	{
		mpPlatformTraders->GetAll_GE_UpdateSeq(nTraderSeq,nvecTrader);
		mpPlatformOrders->GetAll_GE_UpdateSeq(nOrderSeq,nvecOrder);
	}
	return;
}