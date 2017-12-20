#include "StdAfx.h"
#include "TradeSimulation.h"
#include "MyXML.h"
#include "tools_util.h"
#include "FileOpr.h"
#include "tinyxml\tinyxml.h"
#include "..\SvrTradeData\Interface_SvrTradeData.h"
#include "CommonMacro.h"
#pragma comment(lib, "SvrTradeData_Local.lib")

//auto_ptr<CTradeSimulation> CTradeSimulation::m_pObj;
CTradeSimulation* CTradeSimulation::m_pObj;
DWORD				g_idThreadSubcribe=0;
CTradeSimulation::CTradeSimulation(void)
{
	n_nSessionID = 0;
	m_nSeqID = 0;
	m_nOrderID = 0;
	m_nTradeID = 0;

	m_pDataEntity = NULL;
	m_hThreadSubscribe=NULL;
	g_idThreadSubcribe=0;
	mSubCache=NULL;
	m_preDynamicProfit = util::GetDoubleInvalidValue();	
	m_TotalTradingTime=0;
}
CTradeSimulation::~CTradeSimulation(void)
{

}
bool CTradeSimulation::Release()
{

	/*if( m_pDataEntity != NULL ) 
	{
		delete m_pDataEntity;
		m_pDataEntity = NULL;
	}
	if( mSubCache != NULL ) 
	{
		delete mSubCache;
		mSubCache = NULL;
	}*/
	if(m_hThreadSubscribe)
	{

		//发送WM_QUIT通知线程结束
		PostThreadMessage(g_idThreadSubcribe,WM_QUIT,0,0);


		//等待线程退出
		DWORD ExitCode;
		WaitForSingleObject((HANDLE)m_hThreadSubscribe,8000);
		if(GetExitCodeThread((HANDLE)m_hThreadSubscribe,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
			TerminateThread((HANDLE)m_hThreadSubscribe,0);

		CloseHandle(m_hThreadSubscribe);

		m_hThreadSubscribe=NULL;
		g_idThreadSubcribe=0;
	}
	return true;
}
bool CTradeSimulation::init(std::string& nUserID, CDataEntity*	pDataEntity ,int ntradeRule, CMemCache* SubCache)
{
	m_nUserID =nUserID;
	m_nTradeID =ntradeRule;
	m_pDataEntity =pDataEntity;
	mSubCache = SubCache;
	m_nTradeRule = ntradeRule;

	m_pObj=this;

	m_hThreadSubscribe=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadWorkerSubscribe,this,0,&g_idThreadSubcribe);
	
	return true;

}

CTradeSimulation& CTradeSimulation::getObj(void)
{
	//static volatile long running=0;
	//while(InterlockedCompareExchange(&running,1,0)==1) Sleep(0);
	//if(m_pObj.get()==NULL)
	//	m_pObj.reset(new CTradeSimulation());
	//InterlockedExchange(&running,0);
	return *m_pObj;
}
//工作线程
DWORD CTradeSimulation::ThreadWorkerSubscribe(LPVOID pParameter)
{
	if(pParameter==NULL)
		return -1;

	CTradeSimulation* pMe = (CTradeSimulation*)pParameter;
	pMe->SubscribeCacheChannel();

	return 0;
}
void CTradeSimulation::AddCommandCallback(const std::string & nsCommand)
{
	CTradeSimulation::getObj().AddCommand(nsCommand);
	
}
void CTradeSimulation::AddCommand(const std::string & nsCommand)
{
	BOOL bRet = FALSE;
	LockObject();
	if(!nsCommand.empty())
	{
		mCommandList.push(nsCommand);	
	}
	UnlockObject();
    //DealCommand();
}
void CTradeSimulation::DealCommand()
{
	LockObject();
	while(mCommandList.size() != 0)
	{
		std::string lCommandStr = mCommandList.front();
		std::size_t found = std::string::npos;
		if( (found = lCommandStr.find(ORDERINPUT_CHANNEL_NAME)) != std::string::npos)
		{
			PlatformStru_InputOrder input;
			memset(&input, 0, sizeof(input));
			if (OrderInput2String(lCommandStr,input))
			{
				InsertOrder(input);
			}

			
		}
		else if((found = lCommandStr.find(ORDERACTION_CHANNEL_NAME)) != std::string::npos) 
		{
			PlatformStru_InputOrderAction action;
			memset(&action, 0, sizeof(action));
			if (Orderaction2String(lCommandStr,action))
			{
				CancelOrder(action);
			}
			
			
		}
		else  if((found = lCommandStr.find(HEARTBEAT_CHANNEL_NAME)) != std::string::npos) 
		{
			
		}
		mCommandList.pop();
		
	}
	UnlockObject();
	return;
}

bool CTradeSimulation::IsInvalidValue(double fVal)
{
	return util::isInvalidValue(fVal);
}
void CTradeSimulation::GenerateCancelFail(const PlatformStru_InputOrderAction& action, PlatformStru_OrderInfo& order)
{
	strncpy(order.BrokerID, action.Thost.BrokerID,sizeof(order.BrokerID)-1);
	strncpy(order.InvestorID, action.Thost.InvestorID,sizeof(order.InvestorID)-1);
	strncpy(order.InstrumentID, action.Thost.InstrumentID,sizeof(order.InstrumentID)-1);
	strncpy(order.OrderRef, action.Thost.OrderRef,sizeof(order.OrderRef)-1);
	strncpy(order.UserID, action.Thost.InvestorID, sizeof(order.UserID)-1);
	order.LimitPrice = action.Thost.LimitPrice;
	order.RequestID = action.Thost.RequestID;
	order.OrderSubmitStatus = THOST_FTDC_OSS_CancelRejected;
	strcpy(order.StatusMsg,"撤单失败");
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(order.CancelTime, sizeof(order.CancelTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strncpy(order.UpdateTime, order.CancelTime, sizeof(order.UpdateTime));


}
void CTradeSimulation::ConvInput2Order(const PlatformStru_InputOrder &input, PlatformStru_OrderInfo& order)
{
	strncpy(order.BrokerID, input.BrokerID,sizeof(order.BrokerID)-1);
	strncpy(order.InvestorID, input.InvestorID,sizeof(order.InvestorID)-1);
	strncpy(order.InstrumentID, input.InstrumentID,sizeof(order.InstrumentID)-1);
	strncpy(order.OrderRef, input.OrderRef,sizeof(order.OrderRef)-1);
	strncpy(order.UserID, input.InvestorID, sizeof(order.UserID)-1);
	order.OrderPriceType = input.OrderPriceType;
	order.Direction = input.Direction;
	order.CombOffsetFlag[0] = input.CombOffsetFlag[0];
	order.CombHedgeFlag[0] = input.CombHedgeFlag[0];
	order.LimitPrice = input.LimitPrice;
	order.VolumeTotalOriginal = input.VolumeTotalOriginal;
	order.TimeCondition = input.TimeCondition;
	strncpy(order.GTDDate, input.GTDDate, sizeof(order.GTDDate)-1);
	order.VolumeCondition = input.VolumeCondition;
	order.MinVolume = input.MinVolume;
	order.ContingentCondition = input.ContingentCondition;
	order.StopPrice = input.StopPrice;
	order.ForceCloseReason = input.ForceCloseReason;
	order.IsAutoSuspend = input.IsAutoSuspend;
	strncpy(order.BusinessUnit, input.BusinessUnit, sizeof(order.BusinessUnit)-1);
	order.RequestID = input.RequestID;
	//strncpy(order.OrderLocalID, input.OrderLocalID, sizeof(order.OrderLocalID)-1);
	//strncpy(order.ExchangeID, strExchangeID.c_str(), sizeof(order.ExchangeID)-1);
	//strncpy(order.ParticipantID, input.ParticipantID, sizeof(order.ParticipantID)-1);
	//strncpy(order.ClientID, input.ClientID, sizeof(order.ClientID)-1);
	strncpy(order.ExchangeInstID, input.InstrumentID, sizeof(order.ExchangeInstID)-1);
	//strncpy(order.TraderID, input.TraderID, sizeof(order.TraderID)-1);
	//order.InstallID = input.InstallID;
	//order.OrderSubmitStatus = input.OrderSubmitStatus;
	//order.NotifySequence = input.NotifySequence;
	//strncpy(order.TradingDay, input.TradingDay, sizeof(order.TradingDay)-1);
	order.SettlementID = 1;//input.SettlementID;
	//strncpy(order.OrderSysID, input.OrderSysID, sizeof(order.OrderSysID)-1);
	//order.OrderSource = input.OrderSource;
	//order.OrderStatus = input.OrderStatus;
	//order.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	//order.OrderType = input.OrderType;
	order.VolumeTraded = 0;
	//strncpy(order.InsertDate, input.InsertDate, sizeof(order.InsertDate)-1);
	//strncpy(order.InsertTime, input.InsertTime, sizeof(order.InsertTime)-1);
	//strncpy(order.ActiveTime, input.ActiveTime, sizeof(order.ActiveTime)-1);
	//strncpy(order.SuspendTime, input.SuspendTime, sizeof(order.SuspendTime)-1);
	//strncpy(order.UpdateTime, input.UpdateTime, sizeof(order.UpdateTime)-1);
	//strncpy(order.CancelTime, input.CancelTime, sizeof(order.CancelTime)-1);
	//strncpy(order.ActiveTraderID, input.ActiveTraderID, sizeof(order.ActiveTraderID)-1);
	//strncpy(order.ClearingPartID, input.ClearingPartID, sizeof(order.ClearingPartID)-1);
	//order.SequenceNo = input.SequenceNo;
	order.FrontID = 0;//input.FrontID;
	order.SessionID = 0;//input.SessionID;
	//strncpy(order.UserProductInfo, input.UserProductInfo, sizeof(order.UserProductInfo)-1);
	//strncpy(order.StatusMsg, input.StatusMsg, sizeof(order.StatusMsg)-1);
	order.UserForceClose = input.UserForceClose;
	//strncpy(order.ActiveUserID, input.ActiveUserID, sizeof(order.ActiveUserID)-1);
	//order.BrokerOrderSeq = input.BrokerOrderSeq;
	//strncpy(order.RelativeOrderSysID, input.RelativeOrderSysID, sizeof(order.RelativeOrderSysID)-1);

	order.OrderSubmitStatus = THOST_FTDC_OSS_InsertSubmitted;
	order.bInvalid = false;
	order.VolumeTotal = input.VolumeTotalOriginal;
	order.troubleMoney = util::GetDoubleInvalidValue();	
	order.freezeMargin = util::GetDoubleInvalidValue();

}

void CTradeSimulation::ConvOrder2Trade(const PlatformStru_OrderInfo &order, 
								   PlatformStru_TradeInfo& trade)
{
	strncpy(trade.BrokerID, order.BrokerID,sizeof(trade.BrokerID)-1);
	strncpy(trade.InvestorID, order.InvestorID,sizeof(trade.InvestorID)-1);
	strncpy(trade.InstrumentID, order.InstrumentID,sizeof(trade.InstrumentID)-1);
	strncpy(trade.OrderRef, order.OrderRef,sizeof(trade.OrderRef)-1);
	strncpy(trade.UserID, order.UserID,sizeof(trade.UserID)-1);
	strncpy(trade.ExchangeID, order.ExchangeID,sizeof(trade.ExchangeID)-1);
	//strncpy(trade.TradeID, order.TradeID,sizeof(trade.TradeID)-1);
	trade.Direction = order.Direction;
	strncpy(trade.OrderSysID, order.OrderSysID,sizeof(trade.OrderSysID)-1);
	strncpy(trade.ParticipantID, order.ParticipantID,sizeof(trade.ParticipantID)-1);
	strncpy(trade.ClientID, order.ClientID,sizeof(trade.ClientID)-1);
	//trade.TradingRole = order.TradingRole;
	strncpy(trade.ExchangeInstID, order.ExchangeInstID,sizeof(trade.ExchangeInstID)-1);
	trade.OffsetFlag = order.CombOffsetFlag[0];
	trade.HedgeFlag = order.CombHedgeFlag[0];
	//trade.Price = order.Price;
	trade.Volume = order.VolumeTotalOriginal;
	//strncpy(trade.TradeDate, order.InsertDate,sizeof(trade.TradeDate)-1);
	//strncpy(trade.TradeTime, order.TradeTime,sizeof(trade.TradeTime)-1);
	//trade.TradeType = order.TradeType;
	//trade.PriceSource = order.PriceSource;
	strncpy(trade.TraderID, order.TraderID,sizeof(trade.TraderID)-1);
	strncpy(trade.OrderLocalID, order.OrderLocalID,sizeof(trade.OrderLocalID)-1);
	strncpy(trade.ClearingPartID, order.ClearingPartID,sizeof(trade.ClearingPartID)-1);
	//strncpy(trade.BusinessUnit, BusinessUnit,sizeof(trade.BusinessUnit)-1);
	trade.SequenceNo = order.SequenceNo;
	strncpy(trade.TradingDay, order.TradingDay,sizeof(trade.TradingDay)-1);
	trade.SettlementID = order.SettlementID;
	trade.BrokerOrderSeq = order.BrokerOrderSeq;
	//trade.TradeSource = order.TradeSource;

}



//推送报单
void CTradeSimulation::PushOrder( PlatformStru_OrderInfo& order)
{
	
	CInterface_SvrTradeData::getObj().PushOrder(order,true);
	m_pDataEntity->AppendOrder(order);
}
//推送成交
void CTradeSimulation::PushTrade( PlatformStru_TradeInfo& trade)
{
		
	CInterface_SvrTradeData::getObj().PushTrade(trade);
	m_pDataEntity->AppendTrade(trade);

}
void CTradeSimulation::CancelOrderDeal(PlatformStru_OrderInfo & orderInfo,CF_ERROR nErrorCode)
{
	orderInfo.OrderStatus = THOST_FTDC_OST_Canceled;
	orderInfo.OrderSubmitStatus = THOST_FTDC_OSS_InsertRejected;
	orderInfo.ExStatus = PlatformStru_OrderInfo::ExSta_none;
	strcpy(orderInfo.StatusMsg,FormatErrorCode(nErrorCode));
	orderInfo.troubleMoney = util::GetDoubleInvalidValue();	
	orderInfo.freezeMargin = util::GetDoubleInvalidValue();

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(orderInfo.CancelTime, sizeof(orderInfo.CancelTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	strncpy(orderInfo.UpdateTime, orderInfo.CancelTime, sizeof(orderInfo.UpdateTime));
	PushOrder(orderInfo);
}
//下单接口
CF_ERROR CTradeSimulation::InsertOrder(const PlatformStru_InputOrder &input)
{
	TRADE_LOG("模拟成交模块 接收到下单 input.InstrumentID=%s input.LimitPrice=%.4f  input.VolumeTotalOriginal =%d  input.Direction=%c input.CombOffsetFlag=%c input.OrderRef=%s",
		input.InstrumentID,input.LimitPrice,input.VolumeTotalOriginal,input.Direction,input.CombOffsetFlag,input.OrderRef);

	int nErrorCode = CF_ERROR_SUCCESS;
	PlatformStru_OrderInfo order;
	ConvInput2Order(input,  order);

	MarkOrderBase(order);
	PushOrder(order);

	

	if (m_strRspOrderTime.empty())// 无行情时
	{
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		order.OrderStatus = THOST_FTDC_OST_Canceled;
		strncpy(order.StatusMsg, "已撤单报单被拒绝当前状态禁止此项操作", sizeof(order.StatusMsg)-1);
		sprintf_s(order.CancelTime, sizeof(order.CancelTime), "%02d:%02d:%02d", 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		strncpy(order.UpdateTime, order.CancelTime, sizeof(order.UpdateTime));
		PushOrder(order);
		return CF_ERROR_SUCCESS;
	}
	else//有行情
	{
		



		if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
		{

		  nErrorCode =CInterface_SvrTradeData::getObj().CanOrderOpen(order);

		}
		else if(order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday)
		{
			//如果是平今，判断持仓里面的可平今量
			nErrorCode =CInterface_SvrTradeData::getObj().CanOrderColseToday(order);

		}
		else if(order.CombOffsetFlag[0] == THOST_FTDC_OF_CloseYesterday ||
			order.CombOffsetFlag[0] == THOST_FTDC_OF_Close)
		{
			//如果是平仓，判断可平量
			//如果是平今，判断持仓里面的可平今量
			nErrorCode = CInterface_SvrTradeData::getObj().CanOrderColse(order);

		}
		if ( nErrorCode != CF_ERROR_SUCCESS )
		{
			//返回已撤单错误	
			CancelOrderDeal(order,nErrorCode);
			return nErrorCode;
		}
		else 
		{
			//返回下单已接收
			strcpy(order.StatusMsg,FormatErrorCode(CF_ERROR_TRADE_HAS_RECEVIED));			
			MarkOrderSys(order);
			PushOrder(order);
			return CF_ERROR_SUCCESS;
		}
	}

}

//撤单接口
CF_ERROR CTradeSimulation::CancelOrder(const PlatformStru_InputOrderAction& action)
{
	//获取该账号的所有挂单
	std::vector<PlatformStru_OrderInfo> vOrder;
   CInterface_SvrTradeData::getObj().GetUserWaitOrders(m_nUserID, vOrder);

   bool bFindOrder=false;
	for(unsigned int i = 0; i < vOrder.size(); i++)
	{
		if (vOrder[i].OrderSysID != action.Thost.OrderSysID)
		continue;

		bFindOrder=true;

		if(vOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
			vOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeQueueing||
			vOrder[i].OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing||
			vOrder[i].OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing ||
			vOrder[i].OrderStatus == THOST_FTDC_OST_Unknown )
		{
			vOrder[i].OrderStatus = THOST_FTDC_OST_Canceled;
			strcpy(vOrder[i].StatusMsg,"撤单成功");
			SYSTEMTIME sysTime;
			::GetLocalTime(&sysTime);
			sprintf_s(vOrder[i].CancelTime, sizeof(vOrder[i].CancelTime), "%02d:%02d:%02d", 
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			strncpy(vOrder[i].UpdateTime, vOrder[i].CancelTime, sizeof(vOrder[i].UpdateTime));
			PushOrder(vOrder[i]);
		
		}
	}

	if (bFindOrder == false)
	{ 
		PlatformStru_OrderInfo order;
		GenerateCancelFail(action,order);
		
		PushOrder(order);
	}



	return CF_ERROR_SUCCESS;
}

//行情接口
CF_ERROR CTradeSimulation::Quot(const PlatformStru_DepthMarketData& quot)
{
	TRADE_LOG("模拟成交模块 接收到行情 quot.InstrumentID=%s quot.LastPrice=%.4f quot.UpdateTime=%s quot.UpdateMillisec=%d ",
		quot.InstrumentID,quot.LastPrice,quot.UpdateTime,quot.UpdateMillisec);

	m_strRspOrderTime = quot.UpdateTime;
	//交易日改变
	if (m_strTradingDay  != quot.TradingDay)
	{
		m_strTradingDay =quot.TradingDay;

	    m_TotalTradingTime++;
		m_pDataEntity->SetTradingTime(m_TotalTradingTime);

	}
	


	//获取该账号的所有未成交单
	std::vector<PlatformStru_OrderInfo> vOpenOrder;
	CInterface_SvrTradeData::getObj().GetUserWaitOrders(m_nUserID, vOpenOrder);
	std::vector<PlatformStru_OrderInfo>::iterator itop= vOpenOrder.begin();
	int nOpenCnt=0;
	while (itop != vOpenOrder.end())
	{

		nOpenCnt += (*itop).VolumeTotal;
		itop++;
	}

	m_pDataEntity->SetIndiOpenOrder(nOpenCnt);

	// 获取持仓
	std::vector<PlatformStru_Position> vPosition;
	CInterface_SvrTradeData::getObj().GetUserPositions(m_nUserID,"",vPosition);
	std::vector<PlatformStru_Position>::iterator itpo = vPosition.begin();
	int positioncnt=m_pDataEntity->GetMaxPosition();
	while (itpo != vPosition.end())
	{
		if ( positioncnt < (*itpo).Position)
		{
			positioncnt=(*itpo).Position;
		}
		itpo++;
	}
	m_pDataEntity->SetMaxPosition(positioncnt);

	//资金有变动时，记录资金数据
	PlatformStru_TradingAccountInfo data;
	CInterface_SvrTradeData::getObj().GetUserFundInfo(m_nUserID,data);
	if( data.DynamicProfit != m_preDynamicProfit)
	{
		m_preDynamicProfit = data.DynamicProfit;
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		sprintf_s(data.Account, sizeof(data.Account), "%02d:%02d:%02d", 
			sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		m_pDataEntity->AppendTradingAccount(data);



	}
    
	//LockObject();

	//获取该账号的所有未成交报单
	std::vector<PlatformStru_OrderInfo> vOrder;
	CInterface_SvrTradeData::getObj().GetUserWaitOrders(m_nUserID, vOrder);

	for (int k = 0; k < (int)vOrder.size(); k++)
	{
		
		PlatformStru_OrderInfo& order = vOrder[k];

		/*if (order.OrderStatus == THOST_FTDC_OST_Canceled ||order.OrderStatus == THOST_FTDC_OST_AllTraded )
		{
			continue;
		}*/
		if(strcmp(quot.InstrumentID, order.InstrumentID) !=0)
			continue;

		switch(m_nTradeRule)
		{
		case TradeRule_LastPrice://	按最新价触发，触发的全部成交
			{

				if((order.Direction==THOST_FTDC_D_Buy && quot.LastPrice<=order.LimitPrice && (!IsInvalidValue(quot.LastPrice)))
					|| (order.Direction==THOST_FTDC_D_Sell && quot.LastPrice>=order.LimitPrice && (!IsInvalidValue(quot.LastPrice))) )
				{
						PlatformStru_TradeInfo trade;
						ConvOrder2Trade(order, trade);
						MarkTrade(trade, quot,TradeRule_LastPrice);
						order.VolumeTotal = 0;
						order.VolumeTraded = order.VolumeTotalOriginal;
						order.OrderStatus = THOST_FTDC_OST_AllTraded;
						strncpy(order.StatusMsg, "全部成交报单已提交", sizeof(order.StatusMsg)-1);
						SYSTEMTIME sysTime;
						::GetLocalTime(&sysTime);
						sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
							sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
						PushOrder(order);
						PushTrade(trade);	
						
						//bHasTrade = TRUE;
				}
				/*else
				{

					order.VolumeTotal = order.VolumeTotalOriginal;
					order.VolumeTraded = 0;
					order.OrderStatus = THOST_FTDC_OST_NoTradeQueueing;
					strncpy(order.StatusMsg, "未成交还在队列中", sizeof(order.StatusMsg)-1);
					SYSTEMTIME sysTime;
					::GetLocalTime(&sysTime);
					sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
						sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
					PushOrder(order);
				}*/
			
			break;
			}
		case TradeRule_AskBidPrice:
			{
				if((order.Direction==THOST_FTDC_D_Buy && quot.AskPrice1<=order.LimitPrice && (!IsInvalidValue(quot.AskPrice1)))
					|| (order.Direction==THOST_FTDC_D_Sell && quot.BidPrice1>=order.LimitPrice && (!IsInvalidValue(quot.BidPrice1))) )
				{
						PlatformStru_TradeInfo trade;
						ConvOrder2Trade(order, trade);
						MarkTrade(trade, quot,TradeRule_AskBidPrice);
						order.VolumeTotal = 0;
						order.VolumeTraded = order.VolumeTotalOriginal;
						order.OrderStatus = THOST_FTDC_OST_AllTraded;
						strncpy(order.StatusMsg, "全部成交报单已提交", sizeof(order.StatusMsg)-1);
						SYSTEMTIME sysTime;
						::GetLocalTime(&sysTime);
						sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
							sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
						PushOrder(order);
						PushTrade(trade);	

				}
				/*else
				{

					order.VolumeTotal = order.VolumeTotalOriginal;
					order.VolumeTraded = 0;
					order.OrderStatus = THOST_FTDC_OST_NoTradeQueueing;
					strncpy(order.StatusMsg, "未成交还在队列中", sizeof(order.StatusMsg)-1);
					SYSTEMTIME sysTime;
					::GetLocalTime(&sysTime);
					sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
						sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
					PushOrder(order);
				}*/
			
			break;
			}
		case TradeRule_AskBidVolume:
			{
				MAPQUOT::iterator itquot = m_mapQuot.find(quot.InstrumentID);
				if (itquot !=m_mapQuot.end() 
					&&((itquot->second.AskPrice1 == quot.AskPrice1 && itquot->second.AskVolume1 == quot.AskVolume1&& order.Direction==THOST_FTDC_D_Sell) 
					||( order.Direction==THOST_FTDC_D_Buy && itquot->second.BidPrice1 == quot.BidPrice1 &&itquot->second.BidVolume1 == quot.BidVolume1)))
				{
					break;
				}
				if((order.Direction==THOST_FTDC_D_Buy && quot.AskPrice1<=order.LimitPrice && (!IsInvalidValue(quot.AskPrice1)))
					|| (order.Direction==THOST_FTDC_D_Sell && quot.BidPrice1>=order.LimitPrice && (!IsInvalidValue(quot.BidPrice1)))) 
				{
						PlatformStru_TradeInfo trade;
						ConvOrder2Trade(order, trade);
						MarkTrade(trade, quot,TradeRule_AskBidVolume);
						if (trade.Volume < order.VolumeTotalOriginal)
						{

							order.VolumeTotal = order.VolumeTotalOriginal - trade.Volume;
							order.VolumeTraded = trade.Volume;
							order.OrderStatus = THOST_FTDC_OST_PartTradedQueueing;
							strncpy(order.StatusMsg, "部分成交还在队列中", sizeof(order.StatusMsg)-1);
						}
						else
						{
							order.VolumeTotal = 0;
							order.VolumeTraded = order.VolumeTotalOriginal;
							order.OrderStatus = THOST_FTDC_OST_AllTraded;
							strncpy(order.StatusMsg, "全部成交报单已提交", sizeof(order.StatusMsg)-1);
						}
						
						
						SYSTEMTIME sysTime;
						::GetLocalTime(&sysTime);
						sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
							sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
						PushOrder(order);
						PushTrade(trade);	

				}
				//else
				//{

				//	order.VolumeTotal = order.VolumeTotalOriginal;
				//	order.VolumeTraded = 0;
				//	order.OrderStatus = THOST_FTDC_OST_NoTradeQueueing;
				//	strncpy(order.StatusMsg, "未成交还在队列中", sizeof(order.StatusMsg)-1);
				//	SYSTEMTIME sysTime;
				//	::GetLocalTime(&sysTime);
				//	sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
				//		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
				//	PushOrder(order);
				//}

			
			break;
			}
		}
	}


	m_mapQuot[quot.InstrumentID] = quot;


	/*UnlockObject();*/


	return CF_ERROR_SUCCESS;
}


bool CTradeSimulation::MarkOrderBase(PlatformStru_OrderInfo& order)
{
	int nSeqID = GetNextSeqID();

	//sprintf_s(order.OrderLocalID, sizeof(order.OrderLocalID)-1, "%d", nOrderID);
	order.OrderStatus = THOST_FTDC_OST_Unknown;
	order.OrderSubmitStatus = THOST_FTDC_OSS_InsertSubmitted;
	order.NotifySequence = nSeqID;
	strncpy(order.TradingDay, m_strTradingDay.c_str(), sizeof(order.TradingDay)-1);
	//sprintf_s(order.OrderSysID, sizeof(order.OrderSysID)-1, "%d", nOrderID);
	order.OrderSource = THOST_FTDC_OSRC_Participant;
	//order.OrderStatus = THOST_FTDC_OST_Unknown;
	order.OrderType = THOST_FTDC_ORDT_Normal;
	
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(order.InsertDate, sizeof(order.InsertDate), "%04d%02d%02d", 
		sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	sprintf_s(order.InsertTime, sizeof(order.InsertTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	order.SequenceNo = nSeqID;
	strncpy(order.UserProductInfo, "cifco-simu", sizeof(order.UserProductInfo)-1);
	order.BrokerOrderSeq = nSeqID;
	order.ExStatus = PlatformStru_OrderInfo::ExSta_none;

	return true;
}

bool CTradeSimulation::MarkOrderSys(PlatformStru_OrderInfo& order)
{
	int nOrderID = GetNextOrderID();
	SYSTEMTIME sysTime;

	::GetLocalTime(&sysTime);
	sprintf_s(order.OrderLocalID, sizeof(order.OrderLocalID)-1, "%d", nOrderID);
	//order.OrderSubmitStatus = THOST_FTDC_OSS_InsertSubmitted;
	//order.NotifySequence = nOrderID;
	//strncpy(order.TradingDay, m_strTradingDay.c_str(), sizeof(order.TradingDay)-1);
	sprintf_s(order.OrderSysID, sizeof(order.OrderSysID)-1, "%d", nOrderID);
	//order.OrderSource = THOST_FTDC_OSRC_Participant;
	order.OrderStatus = THOST_FTDC_OST_NoTradeQueueing;
	//order.OrderType = THOST_FTDC_ORDT_Normal;
	//sprintf_s(order.InsertDate, sizeof(order.InsertDate), "%04d%02d%02d", 
	//		sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	//sprintf_s(order.InsertTime, sizeof(order.InsertTime), "%02d:%02d:%02d", 
	//		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	sprintf_s(order.UpdateTime, sizeof(order.UpdateTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	//order.SequenceNo = nOrderID;
	//strncpy(order.UserProductInfo, "cifco-simu", sizeof(order.UserProductInfo)-1);
	//order.BrokerOrderSeq = nOrderID;

	return true;
}

bool CTradeSimulation::MarkTrade(PlatformStru_TradeInfo& trade,  const PlatformStru_DepthMarketData& quot,int tradeRule)
{
	int nTradeID = GetNextTradeID();

	sprintf_s(trade.TradeID, sizeof(trade.TradeID)-1, "%d", nTradeID);
	trade.TradingRole = THOST_FTDC_ER_Maker;
	switch(tradeRule)
	{
	case TradeRule_LastPrice:
		{
			trade.Price = quot.LastPrice;
			trade.PriceSource = THOST_FTDC_PSRC_LastPrice;
			break;
		}
	case TradeRule_AskBidPrice:
		{
			if (trade.Direction==THOST_FTDC_D_Buy)
			{
				trade.Price = quot.AskPrice1;
				trade.PriceSource = THOST_FTDC_PSRC_Sell;
			} 
			else
			{
				trade.Price = quot.BidPrice1;
				trade.PriceSource = THOST_FTDC_PSRC_Buy;
			}
			
			break;
		}
	case TradeRule_AskBidVolume:
		{
			if (trade.Direction==THOST_FTDC_D_Buy)
			{
				trade.Price = quot.AskPrice1;
				trade.PriceSource = THOST_FTDC_PSRC_Sell;
				if (quot.AskVolume1 < trade.Volume)
				{
					trade.Volume = quot.AskVolume1;
				}
				
			} 
			else
			{
				trade.Price = quot.BidPrice1;
				trade.PriceSource = THOST_FTDC_PSRC_Buy;
				if (quot.BidVolume1 < trade.Volume)
				{
					trade.Volume = quot.BidVolume1;
				}
			}
			break;
		}
	}

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	sprintf_s(trade.TradeDate, sizeof(trade.TradeDate), "%04d%02d%02d", 
		sysTime.wYear, sysTime.wMonth, sysTime.wDay);
	sprintf_s(trade.TradeTime, sizeof(trade.TradeTime), "%02d:%02d:%02d", 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	
	trade.TradeType = THOST_FTDC_TRDT_Common;
	
	strncpy(trade.BusinessUnit, "1", sizeof(trade.BusinessUnit)-1);
	trade.TradeSource = THOST_FTDC_TSRC_NORMAL;

	return true;
}

bool CTradeSimulation::OrderInput2String(std::string strRedis,PlatformStru_InputOrder& orderInput)
{
	if (strRedis.length() == 0)
	{
		return false;
	}

	TiXmlDocument lDoc;
	lDoc.Parse(strRedis.c_str());
	TiXmlElement * lpElm = lDoc.FirstChildElement(ORDERINPUT_CHANNEL_NAME);
	if(lpElm == NULL)
	{
		//参数错误
		//TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail param error no root element");	
		return false;
	}


	int lUserID = 0, lFrontID = 0, lSessionID = 0;	

	bool lbIsMarket = false;
	std::string lsOrderref,lsBrokerID,lsInvestorId,llsInstrumentID;

	if( TIXML_SUCCESS != lpElm->QueryIntAttribute("FrontID",&lFrontID) || 
		TIXML_SUCCESS != lpElm->QueryIntAttribute("SessionID",&lSessionID) || 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("OrderRef",&lsOrderref)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("BrokerID",&lsBrokerID)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("DealerID",&lsInvestorId)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("InstrumentID",&llsInstrumentID)|| 
		TIXML_SUCCESS != lpElm->QueryDoubleAttribute("Price",&orderInput.LimitPrice) ||
		TIXML_SUCCESS != lpElm->QueryBoolAttribute("IsMarket",&lbIsMarket) ||
		TIXML_SUCCESS != lpElm->QueryIntAttribute("Qty",&orderInput.VolumeTotalOriginal) ||
		TIXML_SUCCESS != lpElm->QueryValueAttribute("OpenClose",&orderInput.CombOffsetFlag[0]) ||
		TIXML_SUCCESS != lpElm->QueryValueAttribute("Hedge",&orderInput.CombHedgeFlag[0]) ||
		TIXML_SUCCESS != lpElm->QueryValueAttribute("Direction",&orderInput.Direction))
	{
		//参数错误，报单失败	
		//TRADE_EXT_LOG("StragegyInputOrderReqDeal Fail param error no root element");	
		return false;
	}

	strcpy(orderInput.BrokerID,lsBrokerID.c_str());
	strcpy(orderInput.InvestorID,lsInvestorId.c_str());
	strcpy(orderInput.InstrumentID,llsInstrumentID.c_str());
	strcpy(orderInput.OrderRef,lsOrderref.c_str());
	orderInput.ContingentCondition = THOST_FTDC_CC_Immediately;
	orderInput.MinVolume = 1;
	orderInput.VolumeCondition = THOST_FTDC_VC_AV; 
	orderInput.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	//组成下单结构体
	if(lbIsMarket)
	{
		//价格类型
		orderInput.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
		//价格
		orderInput.LimitPrice=0;
		//有效期类型
		orderInput.TimeCondition=THOST_FTDC_TC_IOC;
	}
	else
	{
		//价格类型
		orderInput.OrderPriceType=THOST_FTDC_OPT_LimitPrice;

		//有效期类型	
		orderInput.TimeCondition=THOST_FTDC_TC_GFD;
	}
	return true;
}

bool CTradeSimulation::Orderaction2String(std::string strRedis, PlatformStru_InputOrderAction& action)
{
	if (strRedis.length() == 0)
	{
		return false;
	}

	TiXmlDocument lDoc;
	lDoc.Parse(strRedis.c_str());
	TiXmlElement * lpElm = lDoc.FirstChildElement(ORDERACTION_CHANNEL_NAME);
	if(lpElm == NULL)
	{
		//参数错误
		//TRADE_EXT_LOG("StragegyActionOrderReqDeal Fail param error no root element");	
		return false;
	}


	int lUserID = 0, lFrontID = 0, lSessionID = 0;	

	bool lbIsMarket = false;
	std::string lsOrderref,lsBrokerID,lsInvestorId,llsInstrumentID,lsOrderSysId,lsExchangeID;

	if( TIXML_SUCCESS != lpElm->QueryIntAttribute("FrontID",&action.Thost.FrontID) || 
		TIXML_SUCCESS != lpElm->QueryIntAttribute("SessionID",&action.Thost.SessionID) || 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("OrderRef",&lsOrderref)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("BrokerID",&lsBrokerID)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("DealerID",&lsInvestorId)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("InstrumentID",&llsInstrumentID)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("OrderSysID",&lsInvestorId)|| 
		TIXML_SUCCESS != lpElm->QueryStringAttribute("ExchangeID",&llsInstrumentID) )

	{
		//参数错误，报单失败	
		//TRADE_EXT_LOG("StragegyActionOrderReqDeal Fail param error no root element");	
		return false;
	}

	strcpy(action.Thost.BrokerID,lsBrokerID.c_str());
	strcpy(action.Thost.InvestorID,lsInvestorId.c_str());
	strcpy(action.Thost.InstrumentID,llsInstrumentID.c_str());
	strcpy(action.Thost.OrderRef,lsOrderref.c_str());
	strcpy(action.Thost.ExchangeID,lsExchangeID.c_str());
	strcpy(action.Thost.OrderSysID,lsOrderSysId.c_str());
}
void CTradeSimulation::LockObject() 
{
	m_mutex.lock();
}
void CTradeSimulation::UnlockObject() 
{
	m_mutex.unlock();
}
bool CTradeSimulation::SubscribeCacheChannel()
{	
	//return CTradeSimulation::getObj().mSubCache->SubscibleRead(&CTradeSimulation::AddCommandCallback);
	return mSubCache->SubscibleRead(&CTradeSimulation::AddCommandCallback);

	
}