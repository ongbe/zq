#include "stdafx.h"
#include "Real_SubUserOp.h"
#include "CommonMacro.h"
#include "CommonDataChangeEvent.h"
//#include "..\SvrNotifyAndAsk\Interface_SvrNotifyAndAsk.h"


CSubUserOp::~CSubUserOp()
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

//行情到来引起的计算
bool CSubUserOp::CalulateByNewQuto(const PlatformStru_DepthMarketData & nQutosData,bool nbSettlement)
{
	bool lbRet = false;

	do 
	{
		eUserInitStatus leStatus = GetUserInitStatus();
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
int CSubUserOp::GetMarginRate(const std::string& InstrumentID, 
								 PlatformStru_InstrumentMarginRate& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformDataRates)
		return mpPlatformDataRates->GetMarginRate(InstrumentID,outData);
	else
		return -1;

}

int CSubUserOp::GetCommissionRate(const std::string& InstrumentID, 
									 PlatformStru_InstrumentCommissionRate& outData) 
{
	CSafeLock ll(mMutexData);
	if( mpPlatformDataRates)
		return mpPlatformDataRates->GetCommissionRate(InstrumentID,outData);
	else
		return -1;
}
//查询报单的到来
CF_ERROR  CSubUserOp::QryOrderReach(PlatformStru_OrderInfo& OrderInfo,
									   const int ErrID,
									   const bool bIsLast)
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;
	else if(mbQryOrderLast)
	{
		return CF_ERROR_NOINITED;
	}
	else
	{
		if(bIsLast)		
		{
			mbQryOrderLast = true;

		}
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
			SetUserInitStatus(USER_STATUS_InitQryOrdersReady);

		if(bIsLast )
		{
			UpdateQryWaitOrderPosition();

			//初始化查询完成，根据日初持仓明细、日内成交记录，重新计算成交的平尽量、平仓盈亏、手续费
			if(mpPlatformPositionDetail)			
				mpPlatformPositionDetail->ReCalcuTradeInfo_OnInitQryCompleted();

			if(mpPlatformTraderAccount)
				mpPlatformTraderAccount->UpdateTradingAccount_RspQryOrder_RspQryMarginRate_RtnOrder_RspQryDepthMarketData();
		}
		return CF_ERROR_SUCCESS;
	}

}
CF_ERROR CSubUserOp::PushOrder( PlatformStru_OrderInfo & nOrder)
{
	CSafeLock ll(mMutexData);
	if(GetUserInitStatus() != USER_STATUS_Inited ||
		mpPlatformOrders == NULL)
	{
		TRADE_LOG("PushOrder GetUserInitStatus NotInit ，Discard this order BrokerID:%s UserID %s",
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
CF_ERROR CSubUserOp::GetOrderByKey(const OrderKey &nOrderKey,
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
CF_ERROR  CSubUserOp::GetAllOrders(std::vector<PlatformStru_OrderInfo>& outData)
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
CF_ERROR  CSubUserOp::GetTriggerOrders(std::vector<PlatformStru_OrderInfo>& outData)
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
CF_ERROR  CSubUserOp::GetTradedOrders(std::vector<PlatformStru_OrderInfo>& outData)
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
CF_ERROR  CSubUserOp::GetCanceledOrders(std::vector<PlatformStru_OrderInfo>& outData)
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
CF_ERROR  CSubUserOp::GetWaitOrders(std::vector<PlatformStru_OrderInfo>& outData)
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
CF_ERROR CSubUserOp::GetUnkownOrders(std::vector<PlatformStru_OrderInfo>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformOrders == NULL)
		return CF_ERROR_NOINITED;

	if( 0 != mpPlatformOrders->GetUnknownStatusOrders(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_ORDER;
}

//检查报单编号是否是我们的交易客户端下的
bool CSubUserOp::CheckTradeOrderSysIDIsOwn(const std::string & nsOrderSysId) 
{
	CSafeLock ll(mMutexData);
	//if(GetUserInitStatus() != USER_STATUS_Inited
	//	|| mpPlatformOrders == NULL)
	//	return false;

	PlatformStru_OrderInfo lOrder;
    return mpPlatformOrders->GetOrder2(nsOrderSysId,lOrder);

}  



//查询成交的到来
CF_ERROR CSubUserOp::QryTradeReach(PlatformStru_TradeInfo& nTrader,
									  const int ErrID,
									  const bool bIsLast) 
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformTraders == NULL)
		return CF_ERROR_NOINITED;
	else if(mbQryTradeLast)
	{
		return CF_ERROR_NOINITED;
	}
	else
	{
		if(bIsLast)	
			mbQryTradeLast = true;
		//mpPlatformTraders->QryTradeReach(nTrader,ErrID,bIsLast);

		if(bIsLast)
			SetUserInitStatus(USER_STATUS_InitQryTradesReady);

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

		//更新报单数据
		mpPlatformTraders->NewTradeReady(nTrader,InstrumentInfo);

		if(mpPlatformPositionDetail && 
			mpPlatformPositions && 
			mpPlatformPositionDetailCombs 
			&& mpPlatformPositionsCombs 
			&& mpPlatformTraderAccount)
		{
			//ModifiedPositionDetails保存被成交修改的持仓明细记录
			//std::vector<PlatformStru_PositionDetail> ModifiedPositionDetails;
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

}
CF_ERROR CSubUserOp::PushTrader( PlatformStru_TradeInfo & nTrader)
{
	CSafeLock ll(mMutexData);
	if(GetUserInitStatus() != USER_STATUS_Inited 
		|| mpPlatformTraders == NULL)
	{
		TRADE_LOG("GetUserInitStatus NotInit ，Discard this Trade BrokerID:%s UserID %s",nTrader.BrokerID,nTrader.InvestorID);
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
		//std::vector<PlatformStru_PositionDetail> ModifiedPositionDetails;
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
CF_ERROR CSubUserOp::GetAllTrades(std::vector<PlatformStru_TradeInfo>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraders == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}

	if( mpPlatformTraders->GetAllTrades(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_TRADE;
}


//测试成交是否已存在
bool CSubUserOp::TestTradeInfoExist(const TradeKey &key)
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
CF_ERROR  CSubUserOp::QryPositionReach( 
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
			
			TRADE_LOG("SetUserInitStatus QryPosition Ready ");
			SetUserInitStatus(USER_STATUS_InitQryPositionReady);
		}
		return CF_ERROR_SUCCESS;	
	}
}
//获取持仓汇总，返回数据项的数量
CF_ERROR  CSubUserOp::GetPositions(const std::string & nInstrumentID,
									  std::vector<PlatformStru_Position>& outData,
									  bool bNeedZeroVolumePosition ) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard thiscall");
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
bool CSubUserOp::HasPositions() 
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
//获取持仓汇总，返回数据项的数量
CF_ERROR  CSubUserOp::GetPositionVolume(const std::string & nInstrumentID,
									int& nVolume) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard thiscall");
		return CF_ERROR_NOINITED;
	}

	//int lnRet = -1;
	//if(nInstrumentID.empty())
	//{
	//	lnRet = mpPlatformPositions->;
	//	if(lnRet != 0)
	//		return CF_ERROR_SUCCESS;
	//	else
	//		return CF_ERROR_TRADEDATA_NO_POSITION;	
	//}
	//else
	//{
	//	long lSeq = -1;
	//	lnRet = mpPlatformPositions->GetPositions3(nInstrumentID,outData,lSeq);
	//	if(lnRet == 0)
	//		return CF_ERROR_SUCCESS;
	//	else
	//		return CF_ERROR_TRADEDATA_NO_POSITION;	
	//}
	return CF_ERROR_SUCCESS;
}

//日初查询持仓明细的到来
CF_ERROR  CSubUserOp::QryPositionDetailReach( 
	PlatformStru_PositionDetail& PositionDetailInfo,
	int ErrID,
	bool bIsLast) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;	
	}
	else
	{	
		mpPlatformPositionDetail->QryPositionDetailReach(PositionDetailInfo,ErrID,bIsLast);
		if(bIsLast)
		{
			TRADE_LOG("SetUserInitStatus QryPositionDetail Ready ");
			SetUserInitStatus(USER_STATUS_InitQryPositionDetailReady);
		}
		return CF_ERROR_SUCCESS;	
	}
}
//获取持仓明细，返回数据项的数量
CF_ERROR  CSubUserOp::GetPositionDetail(const std::string & nInstrumentID,
										   std::vector<PlatformStru_PositionDetail>& outData) 
{
	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
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
CF_ERROR   CSubUserOp::GetPositionDetail(const PositionDetailKey & nKey,
									PlatformStru_PositionDetail& outData) 
{
	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}	

	if( mpPlatformPositionDetail->GetPositionDetail(nKey,outData) != false)
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_POSDETAIL;	

}
//获取某个时间段的持仓明细
CF_ERROR  CSubUserOp::GetPositionDetail(int nLowtime,int nUpperTime,
									std::vector<PlatformStru_PositionDetail>& outData) 
{

	CSafeLock ll(mMutexData);
	if( mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}	

	mpPlatformPositionDetail->GetPositionDetailsInOpenTimeScope(nLowtime,nUpperTime,outData);
	return CF_ERROR_SUCCESS;
}


//获取组合持仓，返回数据项的数量
CF_ERROR  CSubUserOp::GetPositionComb(const std::string & nInstrumentID,
										 std::vector<PlatformStru_Position>& outData) 
{
	CSafeLock ll(mMutexData);
	int lnRet = -1;
	if( mpPlatformPositionsCombs == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}
	if(nInstrumentID.empty())
	{
		lnRet = mpPlatformPositionsCombs->GetPositionCombs( outData);
		if(lnRet != 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSITION;	
	}
	else
	{

		lnRet = mpPlatformPositionsCombs->GetPositionCombs2(nInstrumentID,outData);
		if(lnRet == 0)
			return CF_ERROR_SUCCESS;
		else
			return CF_ERROR_TRADEDATA_NO_POSITION;	
	}
}



//获取组合持仓明细，返回数据项的数量
CF_ERROR  CSubUserOp::GetPositionCombDetail(const std::string & nInstrumentID,
											   std::vector<PlatformStru_PositionDetailComb>& outData) 
{
	CSafeLock ll(mMutexData);
	return CF_ERROR_TRADEDATA_NO_POSITION;

	/*if(nInstrumentID.empty())
	{
	lnRet = mpPlatformPositionDetailCombs->GetPositionCombs( outData);
	if(lnRet != 0)
	return CF_ERROR_SUCCESS;
	else
	return CF_ERROR_TRADEDATA_NO_POSITION;	
	}
	else
	{

	lnRet = mpPlatformPositionsCombs->GetPositionCombs2(nInstrumentID,outData);
	if(lnRet == 0)
	return CF_ERROR_SUCCESS;
	else
	return CF_ERROR_TRADEDATA_NO_POSITION;	
	}*/
}

//设置日初的资金
CF_ERROR CSubUserOp::SetQryFundInfo(const PlatformStru_TradingAccountInfo& inData) 
{
	CSafeLock ll(mMutexData);
	if(	mpPlatformTraderAccount == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;	
	}
	else
	{	
		TRADE_LOG("SetUserInitStatus QryFundInfo Ready ");

		//DataRspQryTradingAccount lRspAccount;
		//memset(&lRspAccount,0,sizeof(lRspAccount));
		//lRspAccount.Head.BID = BID_RspQryTradingAccount;
		//lRspAccount.Head.Size = sizeof(inData);
		//memcpy(&lRspAccount.TradingAccountField,&inData,sizeof(inData));
		//lRspAccount.bIsLast = true;
		mpPlatformTraderAccount->SetTradingAccount(inData);

		SetUserInitStatus(USER_STATUS_InitQryFundInfoReady);
		return CF_ERROR_SUCCESS;	
	}

}
//获取交易员资金信息
CF_ERROR  CSubUserOp::GetFundInfo(PlatformStru_TradingAccountInfo& outData) 
{
	CSafeLock ll(mMutexData);

	if(mpPlatformTraderAccount == NULL)
	{
	//	TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}
	if(0 == mpPlatformTraderAccount->GetTradingAccount(outData))
		return CF_ERROR_SUCCESS;
	else
		return CF_ERROR_TRADEDATA_NO_FUND;
}
CF_ERROR  CSubUserOp::UserFundInOut(const sFundInOut& nFundInOut) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraderAccount == NULL /*|| GetUserInitStatus() != USER_STATUS_Inited*/ )
		return CF_ERROR_TRADEDATA_FUNDIN_DENY_NOINIT;


	//这块有风险，需加锁
	//计算静态权益、动态权益、可用资金、可取资金
	PlatformStru_TradingAccountInfo lFundInfo;
	mpPlatformTraderAccount->GetTradingAccount(lFundInfo);

	if(nFundInOut.meInOut == FundIN)
	{
		lFundInfo.Deposit += nFundInOut.mdbVolume;
		mdbFirstFund += nFundInOut.mdbVolume;
	}
	else
	{
		mdbFirstFund -= nFundInOut.mdbVolume;
		lFundInfo.Withdraw += nFundInOut.mdbVolume;
	}

	lFundInfo.StaticProfit = lFundInfo.PreBalance
		- lFundInfo.PreCredit - lFundInfo.PreMortgage
		+ lFundInfo.Mortgage - lFundInfo.Withdraw 
		+ lFundInfo.Deposit;
	lFundInfo.DynamicProfit =  lFundInfo.StaticProfit
		+ lFundInfo.CloseProfit + 
		+ lFundInfo.PositionProfit
		- lFundInfo.Commission;
	lFundInfo.Available =    lFundInfo.DynamicProfit 
		- lFundInfo.CurrMargin 
		- lFundInfo.FrozenMargin 
		- lFundInfo.FrozenCommission 
		- lFundInfo.DeliveryMargin 
		+ lFundInfo.Credit;

	//mpPlatformTraderAccount->SetTradingAccount(lFundInfo);
	//TRADE_LOG("Fund InOut ");

	/* DataRspQryTradingAccount lRspAccount;
	memset(&lRspAccount,0,sizeof(lRspAccount));
	lRspAccount.Head.BID = BID_RspQryTradingAccount;
	lRspAccount.Head.Size = sizeof(lFundInfo);
	memcpy(&lRspAccount.TradingAccountField,&lFundInfo,sizeof(lFundInfo));
	lRspAccount.bIsLast = true;*/
	mpPlatformTraderAccount->SetTradingAccount(lFundInfo);

	return CF_ERROR_SUCCESS;

}

//获取所有的持仓明细
CF_ERROR  CSubUserOp::GetAllPositionDetail(
	std::vector<PlatformStru_PositionDetail> & outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositionDetail == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}


	long lastSq = 0;
	mpPlatformPositionDetail->GetPositionDetails(outData,lastSq);


	return CF_ERROR_SUCCESS;
}

//获取所有的持仓
CF_ERROR  CSubUserOp::GetAllPosition(
										std::vector<PlatformStru_Position>& outData) 
{
	CSafeLock ll(mMutexData);
	if(mpPlatformPositions == NULL)
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}


	mpPlatformPositions->GetPositions(outData);
	return CF_ERROR_SUCCESS;
}

//结算的时候重新计算成交
CF_ERROR  CSubUserOp::RecalculateTrade(const std::vector<PlatformStru_TradeInfo>& nVecTrader,
										  bool bUserInputTrader )
{
	CSafeLock ll(mMutexData);
	if(mpPlatformTraders ==NULL || 
		mpPlatformPositions ==NULL || 
		mpPlatformTraderAccount ==NULL  || 
		mpPlatformPositionDetail==NULL )
	{
		//TRADE_LOG("GetUserInitStatus NotInit ，Discard this call");
		return CF_ERROR_NOINITED;
	}
	std::vector<PlatformStru_TradeInfo> lVecTrader ;
	if(!bUserInputTrader)
	{		
		GetAllTrades(lVecTrader);
	}
	else
	{
		std::vector<PlatformStru_TradeInfo>::const_iterator lIterTrade = nVecTrader.begin();
		for(;lIterTrade != nVecTrader.end();lIterTrade++)
		{
			lVecTrader.push_back(*lIterTrade);
		}
	}

	//if(nVecTrader.size() != 0)
	//{
	//	////清空
	//	//mbQryTradeLast = false;
	//	//mpPlatformPositions->clear();
	//	////mpPlatformTraderAccount->clear();
	//	//mpPlatformTraders->clear();
	//	//mpPlatformPositionDetailCombs->clear();
	//	//mpPlatformPositionDetail->clear();
	//	//mpPlatformPositionsCombs->clear();

	//	//重新加入成交
	//	mbQryOrderLast = false;
	//	std::vector<PlatformStru_TradeInfo>::iterator lIter = lVecTrader.begin();
	//	for(;lIter != lVecTrader.end();lIter++)
	//	{
	//		QryTradeReach(*lIter,0,false);
	//	}
	//	PlatformStru_TradeInfo lTempTrader;
	//	QryTradeReach(lTempTrader,-1,true); //最后一条
	//}
	//重新加入成交
	mbQryOrderLast = false;
	std::vector<PlatformStru_TradeInfo>::iterator lIter = lVecTrader.begin();
	for(;lIter != lVecTrader.end();lIter++)
	{
		QryTradeReach(*lIter,0,false);
	}
	PlatformStru_TradeInfo lTempTrader;
	QryTradeReach(lTempTrader,-1,true); //最后一条
	//按结算价算持仓，持仓明细，和资金
	if(mpPlatformPositionDetail)
		mpPlatformPositionDetail->UpdatePositionDetail_OnSettlement();

	if(mpPlatformPositions)
		mpPlatformPositions->UpdatePosition_OnSettlement();

	/*if(mpPlatformTraderAccount)
		mpPlatformTraderAccount->DynamicUpdateTradingAccount_RtnTrade();*/

	return CF_ERROR_SUCCESS;
}

//清楚持仓，持仓明细，资金和成交的交易数据
void CSubUserOp::ClearTraderAndPosition()
{
	CSafeLock ll(mMutexData);
	TRADE_LOG("Clear all subaccount TraderAndPosition");

	mbQryOrderLast = false;
	mbQryTradeLast = false;

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

void  CSubUserOp::ClearAll()
{
	CSafeLock ll(mMutexData);
	TRADE_LOG("Clear all subaccount info");
	SetUserCTPStatus(ACCOUNT_STATUS_UnInit);
	SetEqualUserInitStatus(USER_STATUS_UnInit);
	mbQryOrderLast = false;
	mbQryTradeLast = false;

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
CF_ERROR CSubUserOp::SetInitFund(const double & ndbVal) 
{
	CSafeLock lLock(mMutexData);
	mdbFirstFund = ndbVal;
	return CF_ERROR_SUCCESS;
}
//获取初始总权益
CF_ERROR CSubUserOp::GetFirstFund(double & ndbVal)
{
	CSafeLock lLock(mMutexData);
	ndbVal = mdbFirstFund;
	return CF_ERROR_SUCCESS;

}

//结算时将所有的未成交，部分成交，待审核的报单全部改为已撤单
void CSubUserOp::CancelAllUnkonwnOrders()
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
void CSubUserOp::UpdateQryWaitOrderPosition()
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



void CSubUserOp::GetOrderAndTradeGeSeq( int nOrderSeq,
										  int nTraderSeq,
										  std::vector<PlatformStru_OrderInfo> & nvecOrder,
										  std::vector<PlatformStru_TradeInfo> & nvecTrader) 
{

	CSafeLock lLock(mMutexData);
	if(mpPlatformTraders && mpPlatformOrders)
	{
		mpPlatformTraders->GetAll_GE_UpdateSeq(nTraderSeq,nvecTrader);
		mpPlatformOrders->GetAll_GE_UpdateSeq(nOrderSeq,nvecOrder);
	}
	return;
}