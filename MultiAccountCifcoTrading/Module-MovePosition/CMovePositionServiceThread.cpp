#include "StdAfx.h"
#include "CMovePositionServiceThread.h"
#include "wx/file.h"
#include "wx/filename.h"
#include "../inc/Module-Misc/tools_util.h"
#include "CMovePositionDlg.h"
#include "CSwapPositionDlg.h"
#define  MOVEPOS_TIMERID 210

//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

CMovePositionServiceThread* g_poMovePositionServiceThread = NULL;
CMovePositionServiceThread g_oMovePositionServiceThread;

int CMovePositionServiceThread::RspOrderInsertCallBackFunc(const GUIModuleID GID,const AbstractBusinessData& data)
{
	if(!g_poMovePositionServiceThread)
		return 0;

	wxCommandEvent evt(wxEVT_ORDERSERVICE_THREAD_NEW_ORDER, wxID_ANY);
	if(data.BID==BID_RspOrderInsert && data.Size==sizeof(DataRspOrderInsert))
	{
		DataRspOrderInsert* pTmp=new DataRspOrderInsert();
		memcpy(pTmp, &data, sizeof(DataRspOrderInsert));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RtnOrder && data.Size==sizeof(DataRtnOrder))
	{
		DataRtnOrder* pTmp=new DataRtnOrder();
		memcpy(pTmp, &data, sizeof(DataRtnOrder));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RspOrderAction1 && data.Size==sizeof(DataRspOrderAction1))
	{
		DataRspOrderAction1* pTmp=new DataRspOrderAction1();
		memcpy(pTmp, &data, sizeof(DataRspOrderAction1));
		evt.SetClientData(pTmp);
	}
	else if(data.BID==BID_RspOrderAction2 && data.Size==sizeof(DataRspOrderAction2))
	{
		DataRspOrderAction2* pTmp=new DataRspOrderAction2();
		memcpy(pTmp, &data, sizeof(DataRspOrderAction2));
		evt.SetClientData(pTmp);
	}
	else
	{
		return 0;
	}

	//post event to OrderInsertPanel
	g_poMovePositionServiceThread->AddPendingEvent(evt);

	return 0;
}

int CMovePositionServiceThread::TradeCallBackFunc(const GUIModuleID GID,const AbstractBusinessData &data)	  //成交回报
{
	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
		return 0;

	if(!g_poMovePositionServiceThread)
		return 0;

	DataRtnTrade* pTmp=new DataRtnTrade();
	memcpy(pTmp, &data, sizeof(DataRtnTrade));

	//post event to COrderServiceThread
	wxCommandEvent evt(wxEVT_ORDERSERVICE_THREAD_NEW_TRADE, wxID_ANY);
	evt.SetClientData(pTmp);
	
	g_poMovePositionServiceThread->AddPendingEvent(evt);
	return 0;
}

int	CMovePositionServiceThread::sm_nMovePosNum=0;
int	CMovePositionServiceThread::sm_nSwapPosNum=0;
//定时器回调
void   CALLBACK   MovePositionTimerProc(HWND,UINT,UINT_PTR,DWORD)
{
	if(g_poMovePositionServiceThread)
		g_poMovePositionServiceThread->OnTimer();
}

CMovePositionServiceThread* CMovePositionServiceThread::GetInstance()
{
	if(g_poMovePositionServiceThread == NULL) {
		SetTimer(NULL,MOVEPOS_TIMERID,1000,MovePositionTimerProc);	
		g_oMovePositionServiceThread.InitService();
		g_poMovePositionServiceThread = &g_oMovePositionServiceThread;
	}
	return g_poMovePositionServiceThread;
}

void CMovePositionServiceThread::ReleaseInstance()
{
	KillTimer(NULL,MOVEPOS_TIMERID);
}

//BOOL CMovePositionServiceThread::MakeReq(wxString& strInstrumentID, BOOL bDirection, int nOCMode, 
//										 double fPrice, int nVolume, BOOL bIsHedge, BOOL bIsMarketPrice, 
//										 BOOL bSimulate, PlatformStru_InputOrder& req)
//{
//	return MakeReq(strInstrumentID, bDirection, nOCMode, 
//			fPrice, nVolume, bIsHedge, bIsMarketPrice, bSimulate, req.Thost);
//}

BOOL CMovePositionServiceThread::MakeReq(wxString& strInstrumentID, BOOL bDirection, int nOCMode, 
										 double fPrice, int nVolume, BOOL bIsHedge, BOOL bIsMarketPrice, 
										 BOOL bSimulate, PlatformStru_InputOrder& req)
{

	//合约代码
	strncpy(req.InstrumentID, strInstrumentID.c_str(), strInstrumentID.Length());// sizeof(req.InstrumentID)-1);

	//买卖方向
	req.Direction = (bDirection ? THOST_FTDC_D_Buy : THOST_FTDC_D_Sell);

	if(bIsMarketPrice) {

		PlatformStru_DepthMarketData field;
		memset(&field,0,sizeof(field));
		if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strInstrumentID.c_str(), field);

		PlatformStru_InstrumentInfo outData;
		memset(&outData,0,sizeof(outData));

		if(DEFAULT_SVR()) DEFAULT_SVR()->GetInstrumentInfo(req.InstrumentID, outData);
//		if(strcmp(outData.ExchangeID, "SHFE")==0 && bSimulate) { 
		if(NEEDSIMULATEMARKET(outData.ExchangeID) && bSimulate) { 
			
			//上期所不支持市价单，用限价单模拟
			if(req.Direction == THOST_FTDC_D_Buy)
				req.LimitPrice = field.UpperLimitPrice;
			else
				req.LimitPrice = field.LowerLimitPrice;

			//价格类型
			req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
			//有效期类型
			req.TimeCondition=THOST_FTDC_TC_GFD;
		}
		else {
			//价格类型
			req.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
			//价格
			req.LimitPrice=0;
			//有效期类型
			req.TimeCondition=THOST_FTDC_TC_IOC;
		}

	}
	else {
		//价格类型
		req.OrderPriceType=THOST_FTDC_OPT_LimitPrice;
		//限价价格
		req.LimitPrice=fPrice;
		//有效期类型
		req.TimeCondition=THOST_FTDC_TC_GFD;
	}

	//开平标志
	switch(nOCMode) {
	case 0:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Open;
		break;
	case 1: 
		req.CombOffsetFlag[0]=THOST_FTDC_OF_CloseToday;//平今
		break;
	case 2:
		req.CombOffsetFlag[0]=THOST_FTDC_OF_Close;//平仓
		break;
	};

	//组合投机套保标志，只有第一个字节有效
	if(!bIsHedge)
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Speculation;
	else
		req.CombHedgeFlag[0]=THOST_FTDC_HF_Hedge;

	//数量
	req.VolumeTotalOriginal=nVolume;
	//成交量类型
	req.VolumeCondition = THOST_FTDC_VC_AV; 
	//最小成交量
	req.MinVolume=1;
//	//触发条件
//	if(bIsParketOrder)
//		req.ContingentCondition = THOST_FTDC_CC_ParkedOrder;		// 预埋模式
//	else
		req.ContingentCondition = THOST_FTDC_CC_Immediately;		// 立即模式
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;

	return TRUE;

}


BEGIN_EVENT_TABLE(CMovePositionServiceThread, wxEvtHandler)
//    EVT_TIMER(ID_TIMER_RUN, OnTimer)
    EVT_COMMAND(wxID_ANY, wxEVT_ORDERSERVICE_THREAD_NEW_ORDER, OnRcvOrder)
	EVT_COMMAND(wxID_ANY, wxEVT_ORDERSERVICE_THREAD_NEW_TRADE, OnRcvTrade)
END_EVENT_TABLE()
//////////////////////////////////////////////////////////////////////////////////////


CMovePositionServiceThread::CMovePositionServiceThread():
//wxThread(wxTHREAD_DETACHED),
wxEvtHandler() 
{
}

CMovePositionServiceThread::~CMovePositionServiceThread()
{
	g_poMovePositionServiceThread = NULL;

	//SaveDataToFile();

}

CMovePositionServiceThread* CMovePositionServiceThread::CreateThread()
{
    //wxThread* pThread = new COrderServiceThread();
	return NULL;//pThread;
}

BOOL CMovePositionServiceThread::InitService()
{

//	LoadDataFromFile();
	
	//s_poOrderServiceThread = this;

	//定制业务信息
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnTrade, GID_MOVESWAPPOSITION_SERVICETHREAD, CMovePositionServiceThread::TradeCallBackFunc);	//订阅成交回报

	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderInsert, GID_MOVESWAPPOSITION_SERVICETHREAD, CMovePositionServiceThread::RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RtnOrder, GID_MOVESWAPPOSITION_SERVICETHREAD, CMovePositionServiceThread::RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction1, GID_MOVESWAPPOSITION_SERVICETHREAD, CMovePositionServiceThread::RspOrderInsertCallBackFunc);
	if(DEFAULT_SVR()) DEFAULT_SVR()->SubscribeBusinessData(BID_RspOrderAction2, GID_MOVESWAPPOSITION_SERVICETHREAD, CMovePositionServiceThread::RspOrderInsertCallBackFunc);
	return TRUE;

}
void CMovePositionServiceThread::OnInitSubscribe(wxCommandEvent& evt)
{
	InitService();
}

BOOL CMovePositionServiceThread::OrderInsert(POSITIONUNIT& tPosition, BOOL bIsCloseToday)
{
	
	int nRet = 0;

	if(!bIsCloseToday) {

		PlatformStru_InputOrder& req = tPosition.req;

        nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID()):-1;
		tPosition.nRegNum = req.RequestID;
		tPosition.nRegState = conPosRegOrderWaiting;
		if(nRet!=0) {
			tPosition.nRegState = conPosRegOrderFail;
		}
	}
	else {

		PlatformStru_InputOrder& req = tPosition.req_CloseToday;

        nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderInsert(req, DEFAULT_SVR()->GetNextReqID()):-1;
		tPosition.nRegNum_CloseToday = req.RequestID;
		tPosition.nRegState_CloseToday = conPosRegOrderWaiting;
		if(nRet!=0) {
			tPosition.nRegState_CloseToday = conPosRegOrderFail;
		}
	}
	return TRUE;

}

BOOL CMovePositionServiceThread::OrderCancel(POSITIONUNIT& tPosition, BOOL bIsCloseToday)
{
	
	int nRet = 0;

	if(!bIsCloseToday) {

		PlatformStru_InputOrderAction& action = tPosition.cancelAction;

        nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderAction(action, DEFAULT_SVR()->GetNextReqID()):-1;
		tPosition.nCancelRegNum = action.Thost.RequestID;
		tPosition.nRegState = conPosRegCancelWaiting;
		if(nRet!=0) {
			tPosition.nRegState = conPosRegCancelFail;
		}
	}
	else {

		PlatformStru_InputOrderAction& action = tPosition.cancelAction_CloseToday;

        nRet = DEFAULT_SVR()?DEFAULT_SVR()->ReqOrderAction(action, DEFAULT_SVR()->GetNextReqID()):-1;
		tPosition.nCancelRegNum_CloseToday = action.Thost.RequestID;
		tPosition.nRegState_CloseToday = conPosRegCancelWaiting;
		if(nRet!=0) {
			tPosition.nRegState_CloseToday = conPosRegCancelFail;
		}
	}
	return TRUE;

}

BOOL CMovePositionServiceThread::DoPositionOrder_ContinueTOut(POSITIONUNIT& tPosition, BOOL bIsCloseToday)
{
	// 以下下单活动在撤单完成后再执行

	string strInputCode(tPosition.strInstrumentID);

	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
    if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strInputCode, field);

	if(!bIsCloseToday) {
		switch(tPosition.nTimeOutAction) {
		case conTOutAction_LastPrice:			// 按最新价浮动改单

			if(tPosition.nRemainVolume>0) {
				if(tPosition.bDirection) {
					if(!util::isInvalidValue(field.LastPrice)) {
						tPosition.req.LimitPrice = field.LastPrice + tPosition.nTimeOutPriceOffset*tPosition.fPriceTick;
						if(tPosition.req.LimitPrice > field.UpperLimitPrice) // && IsValid(field.UpperLimitPrice) && field.UpperLimitPrice!=0.0f
							tPosition.req.LimitPrice = field.UpperLimitPrice;
					}
					else {
						tPosition.req.LimitPrice = field.UpperLimitPrice;
					}
				}
				else {
					if(!util::isInvalidValue(field.LastPrice)) {
						tPosition.req.LimitPrice = field.LastPrice - tPosition.nTimeOutPriceOffset*tPosition.fPriceTick;
						if(tPosition.req.LimitPrice < field.LowerLimitPrice) // && IsValid(field.LowerLimitPrice) && field.LowerLimitPrice!=0.0f
							tPosition.req.LimitPrice = field.LowerLimitPrice;
					}
					else {
						tPosition.req.LimitPrice = field.LowerLimitPrice;
					}
				}
				tPosition.req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
				tPosition.req.TimeCondition = THOST_FTDC_TC_GFD;
				tPosition.req.VolumeTotalOriginal = tPosition.nRemainVolume;;
				return OrderInsert(tPosition);
			}
			else {
				//tPosition.nState = conPositionCompleted;
				tPosition.nRegState = conPosRegOrderCompleted;
				return TRUE;
			}

			break;
		case conTOutAction_BuySellPrice:		// 按买卖价浮动改单

			if(tPosition.nRemainVolume>0) {
				if(tPosition.bDirection) {
					if(!util::isInvalidValue(field.AskPrice1)) {
						tPosition.req.LimitPrice = field.AskPrice1 + tPosition.nTimeOutPriceOffset*tPosition.fPriceTick;
						if(tPosition.req.LimitPrice > field.UpperLimitPrice) // && IsValid(field.UpperLimitPrice) && field.UpperLimitPrice!=0.0f
							tPosition.req.LimitPrice = field.UpperLimitPrice;
					}
					else {
						tPosition.req.LimitPrice = field.UpperLimitPrice;
					}
				}
				else {
					if(!util::isInvalidValue(field.BidPrice1)) {
						tPosition.req.LimitPrice = field.BidPrice1 - tPosition.nTimeOutPriceOffset*tPosition.fPriceTick;
						if(tPosition.req.LimitPrice < field.LowerLimitPrice) // && IsValid(field.LowerLimitPrice) && field.LowerLimitPrice!=0.0f
							tPosition.req.LimitPrice = field.LowerLimitPrice;
					}
					else {
						tPosition.req.LimitPrice = field.LowerLimitPrice;
					}
				}
				tPosition.req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
				tPosition.req.TimeCondition = THOST_FTDC_TC_GFD;
				tPosition.req.VolumeTotalOriginal = tPosition.nRemainVolume;;
				return OrderInsert(tPosition);
			}
			else {
				//tPosition.nState = conPositionCompleted;
				tPosition.nRegState = conPosRegOrderCompleted;
				return TRUE;
			}

			break;
		case conTOutAction_Market:				// 按市价改单

			if(tPosition.nRemainVolume>0) {
				if(!tPosition.bSimulate) {
					tPosition.req.LimitPrice = 0.0f;
					tPosition.req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
					tPosition.req.TimeCondition = THOST_FTDC_TC_IOC;
				}
				else {
					if(tPosition.bDirection) 
						tPosition.req.LimitPrice = field.UpperLimitPrice;
					else 
						tPosition.req.LimitPrice = field.LowerLimitPrice;
					tPosition.req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
					tPosition.req.TimeCondition = THOST_FTDC_TC_GFD;
				}
				tPosition.req.VolumeTotalOriginal = tPosition.nRemainVolume;
				return OrderInsert(tPosition);
			}
			else {
				//tPosition.nState = conPositionCompleted;
				tPosition.nRegState = conPosRegOrderCompleted;
				return TRUE;
			}

			break;
		case conTOutAction_CancelOrder:			// 撤单

			tPosition.nState = conPositionCancel;
			
			return TRUE;
			//return OrderCancel(tPosition);

			break;
		case conTOutAction_Nothing:				// 无动作

			return TRUE;
			break;
		};
	}
	else {
		switch(tPosition.nTimeOutAction) {
		case conTOutAction_LastPrice:			// 按最新价浮动改单

			if(tPosition.nRemainVolume_CloseToday>0) {
				if(tPosition.bDirection) {
					if(!util::isInvalidValue(field.LastPrice)) {
						tPosition.req_CloseToday.LimitPrice = field.LastPrice + tPosition.nTimeOutPriceOffset*tPosition.fPriceTick;
						if(tPosition.req_CloseToday.LimitPrice > field.UpperLimitPrice) // && IsValid(field.UpperLimitPrice) && field.UpperLimitPrice!=0.0f
							tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
					}
					else {
						tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
					}
				}
				else {
					if(!util::isInvalidValue(field.LastPrice)) {
						tPosition.req_CloseToday.LimitPrice = field.LastPrice - tPosition.nTimeOutPriceOffset*tPosition.fPriceTick;
						if(tPosition.req_CloseToday.LimitPrice < field.LowerLimitPrice) // && IsValid(field.LowerLimitPrice) && field.LowerLimitPrice!=0.0f
							tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
					}
					else {
						tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
					}
				}
				tPosition.req_CloseToday.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
				tPosition.req_CloseToday.TimeCondition = THOST_FTDC_TC_GFD;
				tPosition.req_CloseToday.VolumeTotalOriginal = tPosition.nRemainVolume_CloseToday;;
				return OrderInsert(tPosition, TRUE);
			}
			else {
				//tPosition.nState = conPositionCompleted;
				tPosition.nRegState_CloseToday = conPosRegOrderCompleted;
				return TRUE;
			}

			break;
		case conTOutAction_BuySellPrice:		// 按买卖价浮动改单

			if(tPosition.nRemainVolume_CloseToday>0) {
				if(tPosition.bDirection) {
					if(!util::isInvalidValue(field.AskPrice1)) {
						tPosition.req_CloseToday.LimitPrice = field.AskPrice1 + tPosition.nTimeOutPriceOffset*tPosition.fPriceTick;
						if(tPosition.req_CloseToday.LimitPrice > field.UpperLimitPrice) // && IsValid(field.UpperLimitPrice) && field.UpperLimitPrice!=0.0f
							tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
					}
					else {
						tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
					}
				}
				else {
					if(!util::isInvalidValue(field.BidPrice1)) {
						tPosition.req_CloseToday.LimitPrice = field.BidPrice1 - tPosition.nTimeOutPriceOffset*tPosition.fPriceTick;
						if(tPosition.req_CloseToday.LimitPrice < field.LowerLimitPrice) // && IsValid(field.LowerLimitPrice) && field.LowerLimitPrice!=0.0f
							tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
					}
					else {
						tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
					}
				}
				tPosition.req_CloseToday.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
				tPosition.req_CloseToday.TimeCondition = THOST_FTDC_TC_GFD;
				tPosition.req_CloseToday.VolumeTotalOriginal = tPosition.nRemainVolume_CloseToday;;
				return OrderInsert(tPosition, TRUE);
			}
			else {
				//tPosition.nState = conPositionCompleted;
				tPosition.nRegState_CloseToday = conPosRegOrderCompleted;
				return TRUE;
			}

			break;
		case conTOutAction_Market:				// 按市价改单

			if(tPosition.nRemainVolume_CloseToday>0) {
				if(!tPosition.bSimulate) {
					tPosition.req_CloseToday.LimitPrice = 0.0f;
					tPosition.req_CloseToday.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
					tPosition.req_CloseToday.TimeCondition = THOST_FTDC_TC_IOC;
				}
				else {
					if(tPosition.bDirection) 
						tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
					else 
						tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
					tPosition.req_CloseToday.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
					tPosition.req_CloseToday.TimeCondition = THOST_FTDC_TC_GFD;
				}
				tPosition.req_CloseToday.VolumeTotalOriginal = tPosition.nRemainVolume_CloseToday;
				return OrderInsert(tPosition, TRUE);
			}
			else {
				//tPosition.nState = conPositionCompleted;
				tPosition.nRegState_CloseToday = conPosRegOrderCompleted;
				return TRUE;
			}

			break;
		case conTOutAction_CancelOrder:			// 撤单

			tPosition.nState = conPositionCancel;

			return TRUE;
			//return OrderCancel(tPosition);

			break;
		case conTOutAction_Nothing:				// 无动作

			return TRUE;
			break;
		};
	}
	return FALSE;
}

BOOL CMovePositionServiceThread::DoPositionOrder(POSITIONUNIT& tPosition, BOOL bIsTimeOut)
{

	string strInputCode(tPosition.strInstrumentID);


	PlatformStru_DepthMarketData field;
	ZeroMemory(&field, sizeof(field));
    if(DEFAULT_SVR()) DEFAULT_SVR()->GetQuotInfo(strInputCode, field);

	if(!bIsTimeOut) {
		// 用户手动的活动

		//if(tPosition.nState==conPositionActive) {

			switch(tPosition.nPriceType) {
			case conPriceType_Market:

				if(tPosition.nRemainVolume>0) {
					if(tPosition.nRegState==conPosRegWaitingSend || tPosition.nRegState==conPosRegCancelHasResponsed) {
						if(!tPosition.bSimulate) { 
							tPosition.req.LimitPrice = 0.0f;
							tPosition.req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
							tPosition.req.TimeCondition = THOST_FTDC_TC_IOC;
						}
						else {
							if(tPosition.bDirection) 
								tPosition.req.LimitPrice = field.UpperLimitPrice;
							else 
								tPosition.req.LimitPrice = field.LowerLimitPrice;
							tPosition.req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
							tPosition.req.TimeCondition = THOST_FTDC_TC_GFD;
						}
						tPosition.req.VolumeTotalOriginal = tPosition.nRemainVolume;
						tPosition.bIsAuto = FALSE;
						OrderInsert(tPosition);
					}
					else {
						//tPosition.nState = conPositionCancel;
						tPosition.nRegState = conPosRegCancel;
					}
				}
				else {
					//tPosition.nState = conPositionCompleted;
					tPosition.nRegState = conPosRegOrderCompleted;
				}
					
				if(tPosition.nRemainVolume_CloseToday>0) {
					if(tPosition.nRegState_CloseToday==conPosRegWaitingSend || tPosition.nRegState_CloseToday==conPosRegCancelHasResponsed) {
						if(!tPosition.bSimulate) { 
							tPosition.req_CloseToday.LimitPrice = 0.0f;
							tPosition.req_CloseToday.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
							tPosition.req_CloseToday.TimeCondition = THOST_FTDC_TC_IOC;
						}
						else {
							if(tPosition.bDirection) 
								tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
							else 
								tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
							tPosition.req_CloseToday.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
							tPosition.req_CloseToday.TimeCondition = THOST_FTDC_TC_GFD;
						}
						tPosition.req_CloseToday.VolumeTotalOriginal = tPosition.nRemainVolume_CloseToday;
						tPosition.bIsAuto_CloseToday = FALSE;
						OrderInsert(tPosition, TRUE);
					}
					else {
						//tPosition.nState = conPositionCancel;
						tPosition.nRegState_CloseToday = conPosRegCancel;
					}
				}
				else {
					//tPosition.nState = conPositionCompleted;
					tPosition.nRegState_CloseToday = conPosRegOrderCompleted;
				}
				return TRUE;

				break;
			case conPriceType_LastPrice:

				if(tPosition.nRemainVolume>0) { 
					if(tPosition.nRegState==conPosRegWaitingSend || tPosition.nRegState==conPosRegCancelHasResponsed) {
						if(tPosition.bDirection) {
							if(!util::isInvalidValue(field.LastPrice)) {
								tPosition.req.LimitPrice = field.LastPrice + tPosition.nPriceOffset*tPosition.fPriceTick;
								if(tPosition.req.LimitPrice > field.UpperLimitPrice) // && IsValid(field.UpperLimitPrice) && field.UpperLimitPrice!=0.0f
									tPosition.req.LimitPrice = field.UpperLimitPrice;
							}
							else {
								tPosition.req.LimitPrice = field.UpperLimitPrice;
							}
						}
						else {
							if(!util::isInvalidValue(field.LastPrice)) {
								tPosition.req.LimitPrice = field.LastPrice - tPosition.nPriceOffset*tPosition.fPriceTick;
								if(tPosition.req.LimitPrice < field.LowerLimitPrice) // && IsValid(field.LowerLimitPrice) && field.LowerLimitPrice!=0.0f
									tPosition.req.LimitPrice = field.LowerLimitPrice;
							}
							else {
								tPosition.req.LimitPrice = field.LowerLimitPrice;
							}
						}
						tPosition.req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
						tPosition.req.TimeCondition = THOST_FTDC_TC_GFD;
						tPosition.req.VolumeTotalOriginal = tPosition.nRemainVolume;
						tPosition.bIsAuto = FALSE;
						OrderInsert(tPosition);
					}
					else {
						//tPosition.nState = conPositionCancel;
						tPosition.nRegState = conPosRegCancel;
					}
				}
				else {
					//tPosition.nState = conPositionCompleted;
					tPosition.nRegState = conPosRegOrderCompleted;
				}
				if(tPosition.nRemainVolume_CloseToday>0) { 
					if(tPosition.nRegState_CloseToday==conPosRegWaitingSend || tPosition.nRegState_CloseToday==conPosRegCancelHasResponsed) {
						if(tPosition.bDirection) {
							if(!util::isInvalidValue(field.LastPrice)) {
								tPosition.req_CloseToday.LimitPrice = field.LastPrice + tPosition.nPriceOffset*tPosition.fPriceTick;
								if(tPosition.req_CloseToday.LimitPrice > field.UpperLimitPrice) // && IsValid(field.UpperLimitPrice) && field.UpperLimitPrice!=0.0f
									tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
							}
							else {
								tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
							}
						}
						else {
							if(!util::isInvalidValue(field.LastPrice)) {
								tPosition.req_CloseToday.LimitPrice = field.LastPrice - tPosition.nPriceOffset*tPosition.fPriceTick;
								if(tPosition.req_CloseToday.LimitPrice < field.LowerLimitPrice) // && IsValid(field.LowerLimitPrice) && field.LowerLimitPrice!=0.0f
									tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
							}
							else {
								tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
							}
						}
						tPosition.req_CloseToday.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
						tPosition.req_CloseToday.TimeCondition = THOST_FTDC_TC_GFD;
						tPosition.req_CloseToday.VolumeTotalOriginal = tPosition.nRemainVolume_CloseToday;
						tPosition.bIsAuto_CloseToday = FALSE;
						OrderInsert(tPosition, TRUE);
					}
					else {
						//tPosition.nState = conPositionCancel;
						tPosition.nRegState_CloseToday = conPosRegCancel;
					}
				}
				else {
					//tPosition.nState = conPositionCompleted;
					tPosition.nRegState_CloseToday = conPosRegOrderCompleted;
				}
				return TRUE;

				break;
			case conPriceType_BuySellPrice:

				if(tPosition.nRemainVolume>0) { 
					if(tPosition.nRegState==conPosRegWaitingSend || tPosition.nRegState==conPosRegCancelHasResponsed) {
						if(tPosition.bDirection) {
							if(!util::isInvalidValue(field.AskPrice1)) {
								tPosition.req.LimitPrice = field.AskPrice1 + tPosition.nPriceOffset*tPosition.fPriceTick;
								if(tPosition.req.LimitPrice > field.UpperLimitPrice) // && IsValid(field.UpperLimitPrice) && field.UpperLimitPrice!=0.0f
									tPosition.req.LimitPrice = field.UpperLimitPrice;
							}
							else {
								tPosition.req.LimitPrice = field.UpperLimitPrice;
							}
						}
						else {
							if(!util::isInvalidValue(field.BidPrice1)) {
								tPosition.req.LimitPrice = field.BidPrice1 - tPosition.nPriceOffset*tPosition.fPriceTick;
								if(tPosition.req.LimitPrice < field.LowerLimitPrice) // && IsValid(field.LowerLimitPrice) && field.LowerLimitPrice!=0.0f
									tPosition.req.LimitPrice = field.LowerLimitPrice;
							}
							else {
								tPosition.req.LimitPrice = field.LowerLimitPrice;
							}
						}
						tPosition.req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
						tPosition.req.TimeCondition = THOST_FTDC_TC_GFD;
						tPosition.req.VolumeTotalOriginal = tPosition.nRemainVolume;
						tPosition.bIsAuto = FALSE;
						OrderInsert(tPosition);
					}
					else {
						//tPosition.nState = conPositionCancel;
						tPosition.nRegState = conPosRegCancel;
					}
				}
				else {
					//tPosition.nState = conPositionCompleted;
					tPosition.nRegState = conPosRegOrderCompleted;
				}
				if(tPosition.nRemainVolume_CloseToday>0) { 
					if(tPosition.nRegState_CloseToday==conPosRegWaitingSend || tPosition.nRegState_CloseToday==conPosRegCancelHasResponsed) {
						if(tPosition.bDirection) {
							if(!util::isInvalidValue(field.AskPrice1)) {
								tPosition.req_CloseToday.LimitPrice = field.AskPrice1 + tPosition.nPriceOffset*tPosition.fPriceTick;
								if(tPosition.req_CloseToday.LimitPrice > field.UpperLimitPrice) // && IsValid(field.UpperLimitPrice) && field.UpperLimitPrice!=0.0f
									tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
							}
							else {
								tPosition.req_CloseToday.LimitPrice = field.UpperLimitPrice;
							}
						}
						else {
							if(!util::isInvalidValue(field.BidPrice1)) {
								tPosition.req_CloseToday.LimitPrice = field.BidPrice1 - tPosition.nPriceOffset*tPosition.fPriceTick;
								if(tPosition.req_CloseToday.LimitPrice < field.LowerLimitPrice) // && IsValid(field.LowerLimitPrice) && field.LowerLimitPrice!=0.0f
									tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
							}
							else {
								tPosition.req_CloseToday.LimitPrice = field.LowerLimitPrice;
							}
						}
						tPosition.req_CloseToday.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
						tPosition.req_CloseToday.TimeCondition = THOST_FTDC_TC_GFD;
						tPosition.req_CloseToday.VolumeTotalOriginal = tPosition.nRemainVolume_CloseToday;
						tPosition.bIsAuto_CloseToday = FALSE;
						OrderInsert(tPosition, TRUE);
					}
					else {
						//tPosition.nState = conPositionCancel;
						tPosition.nRegState_CloseToday = conPosRegCancel;
					}
				}
				else {
					//tPosition.nState = conPositionCompleted;
					tPosition.nRegState_CloseToday = conPosRegOrderCompleted;
				}
				return TRUE;

				break;
			case conPriceType_Limit:

				if(tPosition.nRemainVolume>0) { 
					if(tPosition.nRegState==conPosRegWaitingSend || tPosition.nRegState==conPosRegCancelHasResponsed) {
						tPosition.bIsAuto = FALSE;
						OrderInsert(tPosition);
					}
					else {
						//tPosition.nState = conPositionCancel;
						tPosition.nRegState = conPosRegCancel;
					}
				}
				else {
					//tPosition.nState = conPositionCompleted;
					tPosition.nRegState = conPosRegOrderCompleted;
				}
				if(tPosition.nRemainVolume_CloseToday>0) {
					if(tPosition.nRegState_CloseToday==conPosRegWaitingSend || tPosition.nRegState_CloseToday==conPosRegCancelHasResponsed) {
						tPosition.bIsAuto_CloseToday = FALSE;
						OrderInsert(tPosition, TRUE);
					}
					else {
						//tPosition.nState = conPositionCancel;
						tPosition.nRegState_CloseToday = conPosRegCancel;
					}
				}
				else {
					//tPosition.nState = conPositionCompleted;
					tPosition.nRegState_CloseToday = conPosRegOrderCompleted;
				}
				return TRUE;

				break;
			};
		//}
	}
	else {
		// 超时的自动活动
		//if(tPosition.nState==conPositionActive) {

			if(tPosition.nRemainVolume > 0 && tPosition.nRegState==conPosRegOrderHasResponsed) {
				tPosition.bIsAuto = TRUE;
				OrderCancel(tPosition);
			}
			else if(tPosition.nRemainVolume > 0 && 
					(tPosition.nRegState==conPosRegOrderWaiting || tPosition.nRegState==conPosRegOrderFail)) {
				DoPositionOrder_ContinueTOut(tPosition);
			}
			if(tPosition.nRemainVolume_CloseToday > 0 && tPosition.nRegState_CloseToday==conPosRegOrderHasResponsed) {
				tPosition.bIsAuto_CloseToday = TRUE;
				OrderCancel(tPosition, TRUE);
			}
			else if(tPosition.nRemainVolume_CloseToday > 0 && 
					(tPosition.nRegState_CloseToday==conPosRegOrderWaiting || tPosition.nRegState_CloseToday==conPosRegOrderFail)) {
				DoPositionOrder_ContinueTOut(tPosition, TRUE);
			}

		//}
	}

	return FALSE;

}

BOOL CMovePositionServiceThread::MovePositionDoNow(int nNum)
{
	MAPMOVEPOSITIONITEM it = m_mapMovePosition.find(nNum);
	if(it != m_mapMovePosition.end()) {
		if(it->second.nState != conMovePause) 
			return FALSE;
		it->second.nState = conMoveActive;
		if(it->second.tSrcPos.nState == conPositionPause) {
			it->second.tSrcPos.nState = conPositionActive;
			it->second.tSrcPos.nRemainTime = it->second.nTimeOutInterval;
			DoPositionOrder(it->second.tSrcPos, FALSE);
		}
		if(it->second.tDestPos.nState == conPositionPause && it->second.nActionMode == 1) {
			it->second.tDestPos.nState = conPositionActive;
			it->second.tDestPos.nRemainTime = it->second.nTimeOutInterval;
			DoPositionOrder(it->second.tDestPos, FALSE);
		}
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMovePositionServiceThread::MovePositionRedo(int nNum, MOVEPOSITION& tTmpMove, BOOL& bIsEqual)
{
	// 比较两个MOVEPOSITION是否相同
	MAPMOVEPOSITIONITEM it;
	
	it = m_mapMovePosition.find(nNum);
	if(it==m_mapMovePosition.end())
		return FALSE;
	MOVEPOSITION& tMove = it->second;

	if(it->second.nRetryNum != tTmpMove.nRetryNum || it->second.nTimeOutInterval != tTmpMove.nTimeOutInterval) {
		it->second.nRetryNum = tTmpMove.nRetryNum;
		it->second.nTimeOutInterval = tTmpMove.nTimeOutInterval;

		it->second.tDestPos.nRetryCount = 0;
		it->second.tDestPos.nRemainTime = tTmpMove.nTimeOutInterval;
		it->second.tSrcPos.nRetryCount = 0;
		it->second.tSrcPos.nRemainTime = tTmpMove.nTimeOutInterval;
	}

	BOOL bIsNEQSrc = TRUE;
	
	{
		POSITIONUNIT& tSrcPos = tMove.tSrcPos;
		POSITIONUNIT& tTmpSrcPos = tTmpMove.tSrcPos;
		
		if(tSrcPos.nState == conPositionActive) {
			if(tSrcPos.nPriceType!=tTmpSrcPos.nPriceType || 
					(tSrcPos.nPriceType==tTmpSrcPos.nPriceType && (tSrcPos.nPriceType==conPriceType_LastPrice || tSrcPos.nPriceType==conPriceType_BuySellPrice) && tSrcPos.nPriceOffset!=tTmpSrcPos.nPriceOffset) || 
					(tSrcPos.nPriceType==tTmpSrcPos.nPriceType && tSrcPos.nPriceType==conPriceType_Limit && tSrcPos.fPrice!=tTmpSrcPos.fPrice) ) { 
				bIsNEQSrc = FALSE;
				tSrcPos.nPriceType = tTmpSrcPos.nPriceType;
				tSrcPos.nPriceOffset = tTmpSrcPos.nPriceOffset;
				tSrcPos.fPrice = tTmpSrcPos.fPrice;

				if(tSrcPos.nRemainVolume > 0 && tSrcPos.nRegState==conPosRegOrderHasResponsed) {
					tSrcPos.bIsAuto = FALSE;
					OrderCancel(tSrcPos);
				}
				if(tSrcPos.nRemainVolume_CloseToday > 0 && tSrcPos.nRegState_CloseToday==conPosRegOrderHasResponsed) {
					tSrcPos.bIsAuto_CloseToday = FALSE;
					OrderCancel(tSrcPos, TRUE);
				}
			}
			if(tSrcPos.nTimeOutAction!=tTmpSrcPos.nTimeOutAction || 
					tSrcPos.nTimeOutPriceOffset!=tTmpSrcPos.nTimeOutPriceOffset ) { 
				bIsNEQSrc = FALSE;
				tSrcPos.nTimeOutAction = tTmpSrcPos.nTimeOutAction;
				tSrcPos.nTimeOutPriceOffset = tTmpSrcPos.nTimeOutPriceOffset;
			}
		}
	}

	BOOL bIsNEQDest = TRUE;

	{
		POSITIONUNIT& tDestPos = tMove.tDestPos;
		POSITIONUNIT& tTmpDestPos = tTmpMove.tDestPos;
		
		if(tDestPos.nState == conPositionActive) {
			if(tDestPos.nPriceType!=tTmpDestPos.nPriceType || 
					(tDestPos.nPriceType==tTmpDestPos.nPriceType && (tDestPos.nPriceType==conPriceType_LastPrice || tDestPos.nPriceType==conPriceType_BuySellPrice) && tDestPos.nPriceOffset!=tTmpDestPos.nPriceOffset) || 
					(tDestPos.nPriceType==tTmpDestPos.nPriceType && tDestPos.nPriceType==conPriceType_Limit && tDestPos.fPrice!=tTmpDestPos.fPrice) ) { 
				bIsNEQDest = FALSE;
				tDestPos.nPriceType = tTmpDestPos.nPriceType;
				tDestPos.nPriceOffset = tTmpDestPos.nPriceOffset;
				tDestPos.fPrice = tTmpDestPos.fPrice;

				if(tDestPos.nRemainVolume > 0 && tDestPos.nRegState==conPosRegOrderHasResponsed) {
					tDestPos.bIsAuto = FALSE;
					OrderCancel(tDestPos);
				}
			}
			if(tDestPos.nTimeOutAction!=tTmpDestPos.nTimeOutAction || 
					tDestPos.nTimeOutPriceOffset!=tTmpDestPos.nTimeOutPriceOffset ) { 
				bIsNEQDest = FALSE;
				tDestPos.nTimeOutAction = tTmpDestPos.nTimeOutAction;
				tDestPos.nTimeOutPriceOffset = tTmpDestPos.nTimeOutPriceOffset;
			}
		}
	}

	bIsEqual = (bIsNEQSrc && bIsNEQDest);

	return TRUE;
}

// 某个移仓单是否有改单正在进行中	
BOOL CMovePositionServiceThread::MovePositionCheckAction(int nNum, int& nAction)
{
	MAPMOVEPOSITIONITEM it;

	it = m_mapMovePosition.find(nNum);
	if(it==m_mapMovePosition.end()) {
		nAction = -1;
		return FALSE;
	}
	
	//MOVEPOSITION& tMove = it->second;
	if(it->second.nState == conMovePause) {
		nAction = conActionPause;
		return TRUE;
	}
	if(it->second.nState == conMoveCancel) {
		nAction = conActionCancel;
		return TRUE;
	}
	if(it->second.nState == conMoveCompleted) {
		nAction = conActionCompleted;
		return TRUE;
	}
	if(it->second.tSrcPos.nRegState == conPosRegOrderWaiting || 
			it->second.tSrcPos.nRegState_CloseToday == conPosRegOrderWaiting || 
			it->second.tDestPos.nRegState == conPosRegOrderWaiting) {
		if(!it->second.tSrcPos.bIsAuto)
			nAction = conActionUserChanging;
		else
			nAction = conActionAutoChanging;
		return TRUE;
	}
	nAction = conActionMoving;
	return TRUE;

}

// 执行编号为nNum的仓位互换单
BOOL CMovePositionServiceThread::SwapPositionDoNow(int nNum)
{
	MAPSWAPPOSITIONITEM it = m_mapSwapPosition.find(nNum);
	MAPMOVEPOSITIONITEM itmove1;
	MAPMOVEPOSITIONITEM itmove2;

	if(it != m_mapSwapPosition.end()) {
		if(it->second.nState != conSwapPause) 
			return FALSE;
		it->second.nState = conSwapActive;
		itmove1 = m_mapMovePosition.find(it->second.nMovePos1Num);
		itmove2 = m_mapMovePosition.find(it->second.nMovePos2Num);
		if(itmove1 != m_mapMovePosition.end() && itmove2 != m_mapMovePosition.end()) {
			MovePositionDoNow(itmove1->second.nNum);
			MovePositionDoNow(itmove2->second.nNum);
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else
		return FALSE;
}

// 重新执行编号为nNum的仓位互换单
BOOL CMovePositionServiceThread::SwapPositionRedo(int nNum, MOVEPOSITION& tMove1, MOVEPOSITION& tMove2, BOOL& bIsEqual)
{

	MAPSWAPPOSITIONITEM it = m_mapSwapPosition.find(nNum);
	MAPMOVEPOSITIONITEM itmove1;
	MAPMOVEPOSITIONITEM itmove2;
	BOOL bIsEqual1 = TRUE;
	BOOL bIsEqual2 = TRUE;

	if(it != m_mapSwapPosition.end()) {
		if(it->second.nState != conSwapActive) 
			return FALSE;
		itmove1 = m_mapMovePosition.find(it->second.nMovePos1Num);
		itmove2 = m_mapMovePosition.find(it->second.nMovePos2Num);
		if(itmove1 != m_mapMovePosition.end() && itmove2 != m_mapMovePosition.end()) {
			MovePositionRedo(itmove1->second.nNum, tMove1, bIsEqual1);
			MovePositionRedo(itmove2->second.nNum, tMove2, bIsEqual2);
			bIsEqual = (bIsEqual1 && bIsEqual2);
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else
		return FALSE;

}

// 某个移仓单是否有改单正在进行中	
BOOL CMovePositionServiceThread::SwapPositionCheckAction(int nNum, int& nAction)
{
	//MovePositionCheckAction(int nNum, int& nAction)

	MAPSWAPPOSITIONITEM it = m_mapSwapPosition.find(nNum);
	MAPMOVEPOSITIONITEM itmove1;
	MAPMOVEPOSITIONITEM itmove2;
	int nAction1 = 0;
	int nAction2 = 0;

	if(it != m_mapSwapPosition.end()) {
		if(it->second.nState != conSwapActive) 
			return FALSE;
		itmove1 = m_mapMovePosition.find(it->second.nMovePos1Num);
		itmove2 = m_mapMovePosition.find(it->second.nMovePos2Num);
		if(itmove1 != m_mapMovePosition.end() && itmove2 != m_mapMovePosition.end()) {
			MovePositionCheckAction(itmove1->second.nNum, nAction1);
			MovePositionCheckAction(itmove2->second.nNum, nAction2);
			if(nAction1==conActionUserChanging || nAction2==conActionUserChanging) {
				nAction = conActionUserChanging;
				return TRUE;
			}
			if(nAction1==conActionAutoChanging || nAction2==conActionAutoChanging) {
				nAction = conActionAutoChanging;
				return TRUE;
			}
			if(nAction1==conActionCompleted && nAction2==conActionCompleted) {
				nAction = conActionCompleted;
				return TRUE;
			}
			if(nAction1==conActionCancel || nAction2==conActionCancel) {
				nAction = conActionCancel;
				return TRUE;
			}
			if(nAction1==conActionPause || nAction2==conActionPause) {
				nAction = conActionPause;
				return TRUE;
			}
			nAction = conActionMoving;
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else
		return FALSE;

}

void CMovePositionServiceThread::OnRcvOrder(wxCommandEvent& evt)
{
	AbstractBusinessData* pAbstractBusinessData=static_cast<AbstractBusinessData*>(evt.GetClientData());
	if(!pAbstractBusinessData)
	{
		wxASSERT(pAbstractBusinessData);
		return;
	}
	
	LockObject();

	switch (pAbstractBusinessData->BID) {
		case BID_RtnOrder:
		{
			DataRtnOrder& RawData= *(DataRtnOrder*)(pAbstractBusinessData);

			MAPMOVEPOSITIONITEM it;

			for(it=m_mapMovePosition.begin(); it!=m_mapMovePosition.end(); it++) {
				MOVEPOSITION& tMove = it->second;
				if(tMove.nState != conMoveActive)
					continue;

				{
				POSITIONUNIT& tPos = tMove.tSrcPos;
				if(tPos.nState == conPositionActive) {
					wxString strOrderSysID(RawData.OrderField.OrderSysID);
					switch(RawData.OrderField.OrderStatus) {
					case THOST_FTDC_OST_Canceled:			//下单后，CTP自动撤单
						if(tPos.nRegNum == RawData.OrderField.RequestID || tPos.nCancelRegNum == RawData.OrderField.RequestID 
								|| (strlen(tPos.strOrderSysID)>0 && strOrderSysID.Cmp(tPos.strOrderSysID)==0)) {
							if(tMove.tSrcPos.nRemainVolume > RawData.OrderField.VolumeTotal) {
								tMove.tSrcPos.nRemainVolume = RawData.OrderField.VolumeTotal;
							}
							{
								wxCommandEvent evt(wxEVT_MOVEPOSITION_ORDERLOG);
								if(tMove.pWnd) {
//									DataRtnOrder* pTmp=new DataRtnOrder();
//									memcpy(pTmp, &RawData, sizeof(DataRtnOrder));
									wxString strText;
									strText.Printf("%d", tMove.nNum);
//									evt.SetString(strText.c_str());
//									evt.SetInt(0);
//									evt.SetClientData(pTmp);
//									((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
	                                if(1)
	                                {
		                                DWORD EvtParamID;
		                                std::string EvtParamStr(strText.c_str());
		                                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,(char*)&RawData,sizeof(DataRtnOrder),0))
		                                {
			                                evt.SetInt((int)EvtParamID);
			                                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                                }
	                                }
								}
							}
							if(tPos.nRegState == conPosRegCancelWaiting) {
								tPos.nRegState = conPosRegCancelHasResponsed;
								if(tPos.bIsAuto)
									DoPositionOrder_ContinueTOut(tPos);
								else
									DoPositionOrder(tPos);
							}
							else if(tPos.nRegNum == RawData.OrderField.RequestID) {
								tPos.nRegState = conPosRegOrderFail;
							}
							else {
								//tPos.nState = conPositionCancel;
								//tMove.nState = conMoveCancel;
								continue;
							}
						}
						if(tPos.nRegNum_CloseToday == RawData.OrderField.RequestID || tPos.nCancelRegNum_CloseToday == RawData.OrderField.RequestID 
								|| (strlen(tPos.strOrderSysID_CloseToday)>0 && strOrderSysID.Cmp(tPos.strOrderSysID_CloseToday)==0)) {
							if(tMove.tSrcPos.nRemainVolume_CloseToday > RawData.OrderField.VolumeTotal) {
								tMove.tSrcPos.nRemainVolume_CloseToday = RawData.OrderField.VolumeTotal;
							}
							{
								wxCommandEvent evt(wxEVT_MOVEPOSITION_ORDERLOG);
								if(tMove.pWnd) {
//									DataRtnOrder* pTmp=new DataRtnOrder();
//									memcpy(pTmp, &RawData, sizeof(DataRtnOrder));
									wxString strText;
									strText.Printf("%d", tMove.nNum);
//									evt.SetString(strText.c_str());
//									evt.SetInt(0);
//									evt.SetClientData(pTmp);
//									((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
	                                if(1)
	                                {
		                                DWORD EvtParamID;
		                                std::string EvtParamStr(strText.c_str());
		                                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,(char*)&RawData,sizeof(DataRtnOrder),0))
		                                {
			                                evt.SetInt((int)EvtParamID);
			                                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                                }
	                                }
								}
							}
							if(tPos.nRegState_CloseToday == conPosRegCancelWaiting) {
								tPos.nRegState_CloseToday = conPosRegCancelHasResponsed;
								if(tPos.bIsAuto_CloseToday)
									DoPositionOrder_ContinueTOut(tPos, TRUE);
								else
									DoPositionOrder(tPos);
							}
							else if(tPos.nRegNum_CloseToday == RawData.OrderField.RequestID) {
								tPos.nRegState_CloseToday = conPosRegOrderFail;
							}
							else {
								//tPos.nState = conPositionCancel;
								//tMove.nState = conMoveCancel;
								continue;
							}
						}
						break;
					case THOST_FTDC_OST_NoTradeQueueing:	//下单后，无法立即成交，处于委托状态
					case THOST_FTDC_OST_AllTraded:			//下单后，立即成交
					case THOST_FTDC_OST_PartTradedQueueing:	//下单后，部分成交，处于委托状态
						if(tPos.nRegNum == RawData.OrderField.RequestID) {
							{
								wxCommandEvent evt(wxEVT_MOVEPOSITION_ORDERLOG);
								if(tMove.pWnd) {
//									DataRtnOrder* pTmp=new DataRtnOrder();
//									memcpy(pTmp, &RawData, sizeof(DataRtnOrder));
									wxString strText;
									strText.Printf("%d", tMove.nNum);
//									evt.SetString(strText.c_str());
//									evt.SetInt(0);
//									evt.SetClientData(pTmp);
//									((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
	                                if(1)
	                                {
		                                DWORD EvtParamID;
		                                std::string EvtParamStr(strText.c_str());
		                                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,&RawData,sizeof(DataRtnOrder),0))
		                                {
			                                evt.SetInt((int)EvtParamID);
			                                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                                }
	                                }
								}
							}
							if(tPos.nRegState == conPosRegOrderWaiting || tPos.nRegState == conPosRegWaitingSend) {
								tPos.nRegState = conPosRegOrderHasResponsed;
								strcpy(tPos.strOrderSysID, RawData.OrderField.OrderSysID);

								memcpy(tPos.cancelAction.Thost.OrderSysID, RawData.OrderField.OrderSysID, sizeof(RawData.OrderField.OrderRef));
								tPos.cancelAction.Thost.FrontID = RawData.OrderField.FrontID;
								tPos.cancelAction.Thost.SessionID = RawData.OrderField.SessionID;
								memcpy(tPos.cancelAction.Thost.OrderRef, RawData.OrderField.OrderRef, sizeof(RawData.OrderField.OrderRef));
								memcpy(tPos.cancelAction.Thost.OrderSysID, RawData.OrderField.OrderSysID, sizeof(RawData.OrderField.OrderSysID));
								memcpy(tPos.cancelAction.Thost.ExchangeID, RawData.OrderField.ExchangeID, sizeof(RawData.OrderField.ExchangeID));
								memcpy(tPos.cancelAction.Thost.InstrumentID, RawData.OrderField.InstrumentID, sizeof(RawData.OrderField.InstrumentID));
							}
							else if(tPos.nRegState == conPosRegOrderHasResponsed) {
							}
							else {
								if(tPos.nRegState != conPosRegCancelWaiting) {
									tPos.nState = conPositionCancel;
									tMove.nState = conMoveCancel;
								}
								continue;
							}
						}
						if(tPos.nRegNum_CloseToday == RawData.OrderField.RequestID) {
							{
								wxCommandEvent evt(wxEVT_MOVEPOSITION_ORDERLOG);
								if(tMove.pWnd) {
//									DataRtnOrder* pTmp=new DataRtnOrder();
//									memcpy(pTmp, &RawData, sizeof(DataRtnOrder));
									wxString strText;
									strText.Printf("%d", tMove.nNum);
//									evt.SetString(strText.c_str());
//									evt.SetInt(0);
//									evt.SetClientData(pTmp);
//									((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
	                                if(1)
	                                {
		                                DWORD EvtParamID;
		                                std::string EvtParamStr(strText.c_str());
		                                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,&RawData,sizeof(DataRtnOrder),0))
		                                {
			                                evt.SetInt((int)EvtParamID);
			                                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                                }
	                                }
								}
							}
							if(tPos.nRegState_CloseToday == conPosRegOrderWaiting || tPos.nRegState_CloseToday == conPosRegWaitingSend) {
								tPos.nRegState_CloseToday = conPosRegOrderHasResponsed;
								strcpy(tPos.strOrderSysID_CloseToday, RawData.OrderField.OrderSysID);

								memcpy(tPos.cancelAction_CloseToday.Thost.OrderSysID, RawData.OrderField.OrderSysID, sizeof(RawData.OrderField.OrderRef));
								tPos.cancelAction_CloseToday.Thost.FrontID = RawData.OrderField.FrontID;
								tPos.cancelAction_CloseToday.Thost.SessionID = RawData.OrderField.SessionID;
								memcpy(tPos.cancelAction_CloseToday.Thost.OrderRef, RawData.OrderField.OrderRef, sizeof(RawData.OrderField.OrderRef));
								memcpy(tPos.cancelAction_CloseToday.Thost.OrderSysID, RawData.OrderField.OrderSysID, sizeof(RawData.OrderField.OrderSysID));
								memcpy(tPos.cancelAction_CloseToday.Thost.ExchangeID, RawData.OrderField.ExchangeID, sizeof(RawData.OrderField.ExchangeID));
							}
							else if(tPos.nRegState_CloseToday == conPosRegOrderHasResponsed) {
							}
							else {
								if(tPos.nRegState_CloseToday != conPosRegCancelWaiting) {
									tPos.nState = conPositionCancel;
									tMove.nState = conMoveCancel;
								}
								continue;
							}
						}
						break;
					default:
						break;
					};
				}
				}

				{
				POSITIONUNIT& tPos = tMove.tDestPos;
				if(tPos.nState == conPositionActive) {
					wxString strOrderSysID(RawData.OrderField.OrderSysID);
					switch(RawData.OrderField.OrderStatus) {
					case THOST_FTDC_OST_Canceled:			//下单后，CTP自动撤单
						if(tPos.nCancelRegNum == RawData.OrderField.RequestID || strOrderSysID.Cmp(tPos.strOrderSysID)==0) {
							{
								wxCommandEvent evt(wxEVT_MOVEPOSITION_ORDERLOG);
								if(tMove.pWnd) {
//									DataRtnOrder* pTmp=new DataRtnOrder();
//									memcpy(pTmp, &RawData, sizeof(DataRtnOrder));
									wxString strText;
									strText.Printf("%d", tMove.nNum);
//									evt.SetString(strText.c_str());
//									evt.SetInt(1);
//									evt.SetClientData(pTmp);
//									((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
	                                if(1)
	                                {
		                                DWORD EvtParamID;
		                                std::string EvtParamStr(strText.c_str());
		                                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,&RawData,sizeof(DataRtnOrder),1))
		                                {
			                                evt.SetInt((int)EvtParamID);
			                                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                                }
	                                }
								}
							}
							if(tPos.nRegState == conPosRegCancelWaiting) {
								tPos.nRegState = conPosRegCancelHasResponsed;
								if(tPos.bIsAuto)
									DoPositionOrder_ContinueTOut(tPos);
								else
									DoPositionOrder(tPos);
							}
							else {
								//tPos.nState = conPositionCancel;
								//tMove.nState = conMoveCancel;
								continue;
							}
						}
						break;
					case THOST_FTDC_OST_NoTradeQueueing:	//下单后，无法立即成交，处于委托状态
					case THOST_FTDC_OST_AllTraded:			//下单后，立即成交
					case THOST_FTDC_OST_PartTradedQueueing:	//下单后，部分成交，处于委托状态
						if(tPos.nRegNum == RawData.OrderField.RequestID) {
							{
								wxCommandEvent evt(wxEVT_MOVEPOSITION_ORDERLOG);
								if(tMove.pWnd) {
//									DataRtnOrder* pTmp=new DataRtnOrder();
//									memcpy(pTmp, &RawData, sizeof(DataRtnOrder));
									wxString strText;
									strText.Printf("%d", tMove.nNum);
//									evt.SetString(strText.c_str());
//									evt.SetInt(1);
//									evt.SetClientData(pTmp);
//									((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
	                                if(1)
	                                {
		                                DWORD EvtParamID;
		                                std::string EvtParamStr(strText.c_str());
		                                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,&RawData,sizeof(DataRtnOrder),1))
		                                {
			                                evt.SetInt((int)EvtParamID);
			                                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                                }
	                                }
								}
							}
							if(tPos.nRegState == conPosRegOrderWaiting || tPos.nRegState == conPosRegWaitingSend) {
								tPos.nRegState = conPosRegOrderHasResponsed;
								strcpy(tPos.strOrderSysID, RawData.OrderField.OrderSysID);

								memcpy(tPos.cancelAction.Thost.OrderSysID, RawData.OrderField.OrderSysID, sizeof(RawData.OrderField.OrderRef));
								tPos.cancelAction.Thost.FrontID = RawData.OrderField.FrontID;
								tPos.cancelAction.Thost.SessionID = RawData.OrderField.SessionID;
								memcpy(tPos.cancelAction.Thost.OrderRef, RawData.OrderField.OrderRef, sizeof(RawData.OrderField.OrderRef));
								memcpy(tPos.cancelAction.Thost.OrderSysID, RawData.OrderField.OrderSysID, sizeof(RawData.OrderField.OrderSysID));
								memcpy(tPos.cancelAction.Thost.ExchangeID, RawData.OrderField.ExchangeID, sizeof(RawData.OrderField.ExchangeID));
								memcpy(tPos.cancelAction.Thost.InstrumentID, RawData.OrderField.InstrumentID, sizeof(RawData.OrderField.InstrumentID));
							}
							else if(tPos.nRegState == conPosRegOrderHasResponsed) {
							}
							else {
								if(tPos.nRegState != conPosRegCancelWaiting) {
									tPos.nState = conPositionCancel;
									tMove.nState = conMoveCancel;
								}
								continue;
							}
						}
						break;
					default:
						break;
					};
				}
				}

			}
		}
			break;
		case BID_RspOrderInsert:
		{
			//if(tPos.nCancelRegNum_CloseToday == RawData.OrderField.RequestID || strOrderSysID.Cmp(tPos.strOrderSysID_CloseToday)==0) {
			//			if(tPos.nRegNum == RawData.OrderField.RequestID) {
			//				{
			//					wxCommandEvent evt(wxEVT_MOVEPOSITION_ORDERLOG);
			//					if(tMove.pWnd) {
			//						DataRtnOrder* pTmp=new DataRtnOrder();
			//						memcpy(pTmp, &RawData, sizeof(DataRtnOrder));
			//						evt.SetClientData(pTmp);
			//						((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
			//					}
			//				}




			//DataRspOrderInsert& RawData = *(DataRspOrderInsert*)(pAbstractBusinessData);
			//
			//MAPMOVEPOSITIONITEM it;

			//for(it=m_mapMovePosition.begin(); it!=m_mapMovePosition.end(); it++) {
			//	MOVEPOSITION& tMove = it->second;
			//	
			//	if(tMove.nState != conMoveAction)
			//		continue;
			//	if(tMove.nRegNum == RawData.InputOrderField.RequestID && tMove.nReqState == conPosRegOrderWaiting) {
			//		tMove.nReqState = conPosRegOrderHasResponsed;
			//	}
			//	if(tMove.nRegNum_CloseToday == RawData.InputOrderField.RequestID && tMove.nReqState_CloseToday == conPosRegOrderWaiting) {
			//		tMove.nReqState_CloseToday = conPosRegOrderHasResponsed;
			//	}
			//}
		}
		case BID_RspOrderAction1:
		{
			//if(!m_bWaitingRsp_OrderDelete)
			//	break;
			//DataRspOrderAction1& RawData= *(DataRspOrderAction1*)(pAbstractBusinessData);
			//if(RawData.InputOrderActionField.RequestID!=COrderServiceThread::sm_nReqNum)
			//	break;

			//wxASSERT(RawData.RspInfoField.ErrorID);//ErrorID肯定是非零

			//ShowTradeInfoDlg(wxString("操作失败"), RawData);
			//m_bWaitingRsp_OrderDelete=false;
		}
			break;
		case BID_RspOrderAction2:
		{
			//if(!m_bWaitingRsp_OrderDelete)
			//	break;
			//DataRspOrderAction2& RawData= *(DataRspOrderAction2*)(pAbstractBusinessData);
			//if(RawData.OrderActionField.RequestID!=COrderServiceThread::sm_nReqNum)
			//	break;			

			//ShowTradeInfoDlg(wxString("操作失败"), RawData.OrderActionField);
			//m_bWaitingRsp_OrderDelete=false;
		}
			break;
		default:
			break;
	};

	UnlockObject();
	
	delete (void*)pAbstractBusinessData;
	pAbstractBusinessData=NULL;
	evt.SetClientData(NULL);
}

void CMovePositionServiceThread::OnRcvTrade(wxCommandEvent& evt)
{
//	if(data.BID!=BID_RtnTrade || data.Size!=sizeof(DataRtnTrade))
	void* pRawData= (void*)(evt.GetClientData());
	if(!pRawData) {
		wxASSERT(pRawData);
		return;
	}
	DataRtnTrade& RawData = *(DataRtnTrade*)(evt.GetClientData());

	MAPMOVEPOSITIONITEM it;
	string strOrderSysID;
	char strOutText[1024];

	LockObject();

	for(it=m_mapMovePosition.begin(); it!=m_mapMovePosition.end(); it++) {
		MOVEPOSITION& tMove = it->second;
		if(tMove.nState != conMoveActive)
			continue;

		{
		POSITIONUNIT& tPos = tMove.tSrcPos;
		if(tPos.nState == conPositionActive) {
			strOrderSysID = tPos.strOrderSysID;
			if(strOrderSysID.compare(RawData.TradeField.OrderSysID)==0) {
				if(tPos.nRemainVolume == RawData.TradeField.Volume 
						&& (tPos.nRegState == conPosRegCancelWaiting || tPos.nRegState == conPosRegCancelFail)) 
				{
					tPos.nRegState = conPosRegOrderHasResponsed;
				}
				if(tPos.nRegState == conPosRegOrderHasResponsed && tPos.nRemainVolume>0) {
					tPos.nRemainVolume -= RawData.TradeField.Volume;//Price
					if(tPos.nRemainVolume==0) {
						tPos.nRegState = conPosRegOrderCompleted;
						if(tPos.nRemainVolume_CloseToday==0)
							tPos.nState = conPositionCompleted;
					}
					// 这里需要向外部发送wxEVT_MOVEPOSITION_TRADELOG消息
					sprintf(strOutText, "%d,%d,%s,%s,%0.2f,%d,%0.2f,%d,%d", tMove.nNum, 1,
							RawData.TradeField.OrderSysID, RawData.TradeField.TradeID, 
							tPos.req.LimitPrice, tPos.nVolume, 
							RawData.TradeField.Price, 
							RawData.TradeField.Volume, tPos.nRemainVolume+tPos.nRemainVolume_CloseToday);
					wxCommandEvent evt(wxEVT_MOVEPOSITION_TRADELOG);
//					evt.SetString(strOutText);
//					if(tMove.pWnd) {
//						((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
//					}
	                if(1)
	                {
		                DWORD EvtParamID;
		                std::string EvtParamStr(strOutText);
		                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
		                {
			                evt.SetInt((int)EvtParamID);
			                if(tMove.pWnd)
				                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                }
	                }
				}
			}
			strOrderSysID = tPos.strOrderSysID_CloseToday;
			if(strOrderSysID.compare(RawData.TradeField.OrderSysID)==0) {
				if(tPos.nRemainVolume_CloseToday == RawData.TradeField.Volume 
						&& (tPos.nRegState_CloseToday == conPosRegCancelWaiting || tPos.nRegState_CloseToday == conPosRegCancelFail)) 
				{
					tPos.nRegState_CloseToday = conPosRegOrderHasResponsed;
				}
				if(tPos.nRegState_CloseToday == conPosRegOrderHasResponsed && tPos.nRemainVolume_CloseToday>0) {
					tPos.nRemainVolume_CloseToday -= RawData.TradeField.Volume;//Price
					if(tPos.nRemainVolume_CloseToday==0) {
						tPos.nRegState_CloseToday = conPosRegOrderCompleted;
						if(tPos.nRemainVolume==0)
							tPos.nState = conPositionCompleted;
					}
					// 这里需要向外部发送wxEVT_MOVEPOSITION_TRADELOG消息
					sprintf(strOutText, "%d,%d,%s,%s,%0.2f,%d,%0.2f,%d,%d", tMove.nNum, 1,
							RawData.TradeField.OrderSysID, RawData.TradeField.TradeID, 
							tPos.req_CloseToday.LimitPrice, tPos.nVolume, 
							RawData.TradeField.Price, 
							RawData.TradeField.Volume, tPos.nRemainVolume+tPos.nRemainVolume_CloseToday);
					wxCommandEvent evt(wxEVT_MOVEPOSITION_TRADELOG);
//					evt.SetString(strOutText);
//					if(tMove.pWnd) {
//						((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
//					}
	                if(1)
	                {
		                DWORD EvtParamID;
		                std::string EvtParamStr(strOutText);
		                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
		                {
			                evt.SetInt((int)EvtParamID);
			                if(tMove.pWnd)
				                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                }
	                }
				}
			}
		}
		}

		{
		POSITIONUNIT& tPos = tMove.tDestPos;
		if(tPos.nState == conPositionActive) {
			strOrderSysID = tPos.strOrderSysID;
			if(strOrderSysID.compare(RawData.TradeField.OrderSysID)==0) {
				if(tPos.nRemainVolume == RawData.TradeField.Volume 
						&& (tPos.nRegState == conPosRegCancelWaiting || tPos.nRegState == conPosRegCancelFail)) 
				{
					tPos.nRegState = conPosRegOrderHasResponsed;
				}
				if(tPos.nRegState == conPosRegOrderHasResponsed && tPos.nRemainVolume>0) {
					tPos.nRemainVolume -= RawData.TradeField.Volume;//Price
					if(tPos.nRemainVolume==0) {
						tPos.nRegState = conPosRegOrderCompleted;
						tPos.nState = conPositionCompleted;
					}
					// 这里需要向外部发送wxEVT_MOVEPOSITION_TRADELOG消息
					sprintf(strOutText, "%d,%d,%s,%s,%0.2f,%d,%0.2f,%d,%d", tMove.nNum, 2,
							RawData.TradeField.OrderSysID, RawData.TradeField.TradeID, 
							tPos.req.LimitPrice, tPos.nVolume, 
							RawData.TradeField.Price, 
							RawData.TradeField.Volume, tPos.nRemainVolume+tPos.nRemainVolume_CloseToday);
					wxCommandEvent evt(wxEVT_MOVEPOSITION_TRADELOG);
//					evt.SetString(strOutText);
//					if(tMove.pWnd) {
//						((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
//					}
	                if(1)
	                {
		                DWORD EvtParamID;
		                std::string EvtParamStr(strOutText);
		                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
		                {
			                evt.SetInt((int)EvtParamID);
			                if(tMove.pWnd)
				                ((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
		                }
	                }
				}
			}
		}
		}

		if(tMove.tSrcPos.nRemainVolume+tMove.tSrcPos.nRemainVolume_CloseToday==0) {
			if(tMove.nActionMode==0 && tMove.tDestPos.nState==conPositionPause) {
				tMove.tDestPos.nState = conPositionActive;
				tMove.tDestPos.nRemainTime = it->second.nTimeOutInterval;
				DoPositionOrder(tMove.tDestPos, FALSE);
			}
		}
		if(tMove.tSrcPos.nRemainVolume+tMove.tSrcPos.nRemainVolume_CloseToday+tMove.tDestPos.nRemainVolume==0 && 
				tMove.nState == conMoveActive) {
			tMove.nState = conMoveCompleted;
			wxCommandEvent evt(wxEVT_MOVEPOSITION_ACTION);
			evt.SetInt(conActionMoveCompleted);
			if(tMove.pWnd) {
				((wxEvtHandler*)tMove.pWnd)->AddPendingEvent(evt);
			}
		}
	}
	for(MAPSWAPPOSITIONITEM itSwap = m_mapSwapPosition.begin(); itSwap != m_mapSwapPosition.end(); itSwap++) {
		if(!(itSwap->second.nState == conSwapActive))
			continue;
		
		MAPMOVEPOSITIONITEM itMove1;
		MAPMOVEPOSITIONITEM itMove2;
		itMove1 = m_mapMovePosition.find(itSwap->second.nMovePos1Num);
		itMove2 = m_mapMovePosition.find(itSwap->second.nMovePos2Num);
		if(itMove1 != m_mapMovePosition.end() && itMove2 != m_mapMovePosition.end()) {
			if(itMove1->second.nState == conMoveCompleted && itMove2->second.nState == conMoveCompleted) {
				itSwap->second.nState = conSwapCompleted;
				wxCommandEvent evt(wxEVT_MOVEPOSITION_ACTION);
				evt.SetInt(conActionSwapCompleted);
				if(itMove1->second.pWnd) {
					((wxEvtHandler*)itMove1->second.pWnd)->AddPendingEvent(evt);
				}
			}
		}
	}

	UnlockObject();

	delete pRawData;
	pRawData=NULL;
	evt.SetClientData(NULL);
}

BOOL CMovePositionServiceThread::LoadDataFromFile()
{
/*	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";

	FILEHEAD tFileHead={0};

//	if(!wxFile::Exists(wxT(ORDERSERVICEFILE)))
//		return FALSE;
//	string strInvestorID,strBrokerID;
//	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
	wxString strFileName;
	
	strFileName = strAppPath;
	strFileName += wxT(m_strInvestorID.c_str());
	strFileName += wxT(".Dat");
	wxFileName filename(strFileName); 
	if(!filename.FileExists())
		return FALSE;

	wxDateTime dtCurr;
	dtCurr = wxDateTime::Today();
	wxDateTime dtFileModify = filename.GetModificationTime();
	
	DWORD dwCurrDate = dtCurr.GetYear()*10000+dtCurr.GetMonth()*100+dtCurr.GetDay();
	DWORD dwFileModifyDate = dtFileModify.GetYear()*10000+dtFileModify.GetMonth()*100+dtFileModify.GetDay();

	wxFile file;
	DWORD dwPos=0;

	// 如果是隔夜的条件单等数据，则清空文件
	if(dwCurrDate-dwFileModifyDate) {
		file.Create(strFileName, true);
		file.Close();
		return TRUE;
	}

	file.Open(strFileName);
	if(!file.IsOpened())
		return FALSE;

	file.Read(&tFileHead, sizeof(FILEHEAD));
	dwPos+=sizeof(FILEHEAD);

	tFileHead.dwDate;
	tFileHead.dwTime;
	sm_nReqNum = tFileHead.dwOrderReqNum;
	sm_nBatchOrderNum = tFileHead.dwBatchGroupNum;
	sm_nBatchSubOrderNum = tFileHead.dwBatchOrderNum;
	sm_nParkedOrderNum = tFileHead.dwParkedOrderNum;
	sm_nConditionOrderNum = tFileHead.dwCondOrderNum;
	sm_nPositionOrderNum = tFileHead.dwPositionOrderNum;


	PARKEDORDER tParkedOrder={0};

	dwPos = tFileHead.dwParkedOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwParkedCount; i++) {
		file.Read(&tParkedOrder, sizeof(PARKEDORDER));
		m_mapParkedOrder[tParkedOrder.nNum] = tParkedOrder;
	}

	CONDITIONORDER tCondOrder={0};

	dwPos = tFileHead.dwCondActiveOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwCondActiveCount; i++) {
		file.Read(&tCondOrder, sizeof(CONDITIONORDER));
		m_mapConditionOrder[tCondOrder.nNum] = tCondOrder;
	}

	dwPos = tFileHead.dwCondOverdueOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwCondOverdueCount; i++) {
		file.Read(&tCondOrder, sizeof(CONDITIONORDER));
		m_mapConditionOrder_Overdue[tCondOrder.nNum] = tCondOrder;
	}

	POSITIONORDER tPosOrder={0};

	dwPos = tFileHead.dwPositionOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwPositionCount; i++) {
		file.Read(&tPosOrder, sizeof(POSITIONORDER));
		m_mapPositionOrder[tPosOrder.nNum] = tPosOrder;
	}

	dwPos = tFileHead.dwPosOverdueOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwPosOverdueCount; i++) {
		file.Read(&tPosOrder, sizeof(POSITIONORDER));
		m_mapPositionOrder_Overdue[tPosOrder.nNum] = tPosOrder;
	}

	BATCHSUBORDER tBatchSub={0};
	
	dwPos = tFileHead.dwBatchSubOffset;
	file.Seek(dwPos);
	for(int i=0; i<(int)tFileHead.dwBatchSubCount; i++) {
		file.Read(&tBatchSub, sizeof(BATCHSUBORDER));
		m_mapBatchSubOrder[tBatchSub.nNum] = tBatchSub;
	}

	file.Close();
*/
	return TRUE;
}

BOOL CMovePositionServiceThread::SaveDataToFile()
{
 /*   // 创建目录
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";
    if(!wxDirExists(strAppPath.c_str())) 
		wxMkdir(strAppPath);

	FILEHEAD tFileHead={0};

	wxFile file;
	DWORD dwPos=0;
//	string strInvestorID,strBrokerID;
//	if(DEFAULT_SVR()) DEFAULT_SVR()->GetBrokerIDUserID(strInvestorID,strBrokerID);
	wxString strFileName;
	
	strFileName = strAppPath;
	strFileName += wxT(m_strInvestorID.c_str());
	strFileName += wxT(".Dat");
	file.Create(strFileName, true);
	if(!file.IsOpened())
		return FALSE;

	tFileHead.dwDate;
	tFileHead.dwTime;
	tFileHead.dwOrderReqNum = sm_nReqNum;
	tFileHead.dwBatchGroupNum = sm_nBatchOrderNum;
	tFileHead.dwBatchOrderNum = sm_nBatchSubOrderNum;
	tFileHead.dwParkedOrderNum = sm_nParkedOrderNum;
	tFileHead.dwCondOrderNum = sm_nConditionOrderNum;
	tFileHead.dwPositionOrderNum = sm_nPositionOrderNum;

	file.Write(&tFileHead, sizeof(FILEHEAD));
	dwPos+=sizeof(FILEHEAD);

	tFileHead.dwParkedOffset = dwPos;
	for(MAPPARKEDORDERITEM itParked=m_mapParkedOrder.begin(); 
			itParked!=m_mapParkedOrder.end(); ++itParked) {
		file.Write(&(itParked->second), sizeof(PARKEDORDER));
		tFileHead.dwParkedCount++;
		tFileHead.dwParkedSize+=sizeof(PARKEDORDER);
		dwPos+=sizeof(PARKEDORDER);
	}

	tFileHead.dwCondActiveOffset = dwPos;
	for(MAPCONDITIONORDERITEM itCond=m_mapConditionOrder.begin(); 
			itCond!=m_mapConditionOrder.end(); ++itCond) {
		file.Write(&(itCond->second), sizeof(CONDITIONORDER));
		tFileHead.dwCondActiveCount++;
		tFileHead.dwCondActiveSize+=sizeof(CONDITIONORDER);
		dwPos+=sizeof(CONDITIONORDER);
	}

	tFileHead.dwCondOverdueOffset = dwPos;
	for(MAPCONDITIONORDERITEM itCondOver=m_mapConditionOrder_Overdue.begin(); 
			itCondOver!=m_mapConditionOrder_Overdue.end(); ++itCondOver) {
		file.Write(&(itCondOver->second), sizeof(CONDITIONORDER));
		tFileHead.dwCondOverdueCount++;
		tFileHead.dwCondOverdueSize+=sizeof(CONDITIONORDER);
		dwPos+=sizeof(CONDITIONORDER);
	}

	tFileHead.dwPositionOffset = dwPos;
	for(MAPPOSITIONORDERITEM itPos=m_mapPositionOrder.begin(); 
			itPos!=m_mapPositionOrder.end(); ++itPos) {
		file.Write(&(itPos->second), sizeof(POSITIONORDER));
		tFileHead.dwPositionCount++;
		tFileHead.dwPositionSize+=sizeof(POSITIONORDER);
		dwPos+=sizeof(POSITIONORDER);
	}

	tFileHead.dwPosOverdueOffset = dwPos;
	for(MAPPOSITIONORDERITEM itPos=m_mapPositionOrder_Overdue.begin(); 
			itPos!=m_mapPositionOrder_Overdue.end(); ++itPos) {
		file.Write(&(itPos->second), sizeof(POSITIONORDER));
		tFileHead.dwPosOverdueCount++;
		tFileHead.dwPosOverdueSize+=sizeof(POSITIONORDER);
		dwPos+=sizeof(POSITIONORDER);
	}
	
	tFileHead.dwBatchSubOffset = dwPos;
	for(MAPBATCHSUBORDERITEM itSub=m_mapBatchSubOrder.begin(); 
			itSub!=m_mapBatchSubOrder.end(); ++itSub) {
		file.Write(&(itSub->second), sizeof(BATCHSUBORDER));
		tFileHead.dwBatchSubCount++;
		tFileHead.dwBatchSubSize+=sizeof(BATCHSUBORDER);
		dwPos+=sizeof(BATCHSUBORDER);
	}

	file.Seek(0);
	file.Write(&tFileHead, sizeof(FILEHEAD));

	file.Close() ;
*/
	return TRUE;
}

void CMovePositionServiceThread::OnTimer()
{
	
	MAPMOVEPOSITIONITEM moveit;
	MAPSWAPPOSITIONITEM swapit;


	//BOOL bIsValidTimer = TRUE;
	char strOutText[1024];

	LockObject();

	for(moveit = m_mapMovePosition.begin(); moveit != m_mapMovePosition.end(); moveit++) {
		if(!(moveit->second.nState==conMoveActive))
			continue;
		if(moveit->second.bPauseTimer)
			continue;
		if(moveit->second.tSrcPos.nState==conPositionActive) {
			
			if(moveit->second.tSrcPos.nRemainTime<=0) {
				if(moveit->second.tSrcPos.nRetryCount >= moveit->second.nRetryNum) {
					moveit->second.tSrcPos.nState = conPositionCancel;
				}
				else {
					moveit->second.tSrcPos.nRetryCount++;
					moveit->second.tSrcPos.nRemainTime = moveit->second.nTimeOutInterval;
					// 这里执行超时处理功能
					if(moveit->second.tSrcPos.nTimeOutAction != conTOutAction_Nothing) {
						DoPositionOrder(moveit->second.tSrcPos, TRUE);

					}
				}
			}
			// 这里需要向外部发送wxEVT_MOVEPOSITION_TIMER消息
			sprintf(strOutText, "%d,%d,%d,%d,%d,%d", moveit->second.nNum, 1,
					moveit->second.nTimeOutInterval-moveit->second.tSrcPos.nRemainTime, 
					moveit->second.tSrcPos.nRemainTime, moveit->second.tSrcPos.nRetryCount, 
					moveit->second.nRetryNum-moveit->second.tSrcPos.nRetryCount);
			wxCommandEvent evt(wxEVT_MOVEPOSITION_TIMER);
//			evt.SetString(strOutText);
//			if(moveit->second.pWnd) {
//				((wxEvtHandler*)moveit->second.pWnd)->AddPendingEvent(evt);
//			}
            if(1)
            {
                DWORD EvtParamID;
                std::string EvtParamStr(strOutText);
                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                {
	                evt.SetInt((int)EvtParamID);
	                if(moveit->second.pWnd)
		                ((wxEvtHandler*)moveit->second.pWnd)->AddPendingEvent(evt);
                }
            }
			moveit->second.tSrcPos.nRemainTime--;
		}
		if(moveit->second.tDestPos.nState==conPositionActive) {
			
			if(moveit->second.tDestPos.nRemainTime<=0) {
				if(moveit->second.tDestPos.nRetryCount >= moveit->second.nRetryNum) {
					moveit->second.tDestPos.nState = conPositionCancel;
				}
				else {
					moveit->second.tDestPos.nRetryCount++;
					moveit->second.tDestPos.nRemainTime = moveit->second.nTimeOutInterval;
					// 这里执行超时处理功能
					if(moveit->second.tDestPos.nTimeOutAction != conTOutAction_Nothing) {
						DoPositionOrder(moveit->second.tDestPos, TRUE);

					}
				}
			}
			// 这里需要向外部发送wxEVT_MOVEPOSITION_TIMER消息
			sprintf(strOutText, "%d,%d,%d,%d,%d,%d", moveit->second.nNum, 2,
					moveit->second.nTimeOutInterval-moveit->second.tDestPos.nRemainTime, 
					moveit->second.tDestPos.nRemainTime, moveit->second.tDestPos.nRetryCount, 
					moveit->second.nRetryNum-moveit->second.tDestPos.nRetryCount);
			wxCommandEvent evt(wxEVT_MOVEPOSITION_TIMER);
//			evt.SetString(strOutText);
//			if(moveit->second.pWnd) {
//				((wxEvtHandler*)moveit->second.pWnd)->AddPendingEvent(evt);
//			}
            if(1)
            {
                DWORD EvtParamID;
                std::string EvtParamStr(strOutText);
                if(CEventParam::CreateEventParam(EvtParamID,NULL,&EvtParamStr,NULL,0,0))
                {
	                evt.SetInt((int)EvtParamID);
	                if(moveit->second.pWnd)
		                ((wxEvtHandler*)moveit->second.pWnd)->AddPendingEvent(evt);
                }
            }
			moveit->second.tDestPos.nRemainTime--;
		}
	}

	for(moveit = m_mapMovePosition.begin(); moveit != m_mapMovePosition.end(); moveit++) {
		if(!(moveit->second.nState==conMoveActive))
			continue;
		if(moveit->second.tSrcPos.nState==conPositionCancel || 
				moveit->second.tDestPos.nState==conPositionCancel) {
			moveit->second.tSrcPos.nState = conPositionCancel;
			moveit->second.tDestPos.nState = conPositionCancel;
			moveit->second.nState = conMoveCancel;
			if(moveit->second.nRelateMode==0) {
				wxCommandEvent evt(wxEVT_MOVEPOSITION_ACTION);
				evt.SetInt(conActionCancel_TimeOut);
				if(moveit->second.pWnd) {
					((wxEvtHandler*)moveit->second.pWnd)->AddPendingEvent(evt);
				}
			}
		}
	}
	for(MAPSWAPPOSITIONITEM itSwap = m_mapSwapPosition.begin(); itSwap != m_mapSwapPosition.end(); itSwap++) {
		if(!(itSwap->second.nState == conSwapActive))
			continue;
		
		MAPMOVEPOSITIONITEM itMove1;
		MAPMOVEPOSITIONITEM itMove2;
		itMove1 = m_mapMovePosition.find(itSwap->second.nMovePos1Num);
		itMove2 = m_mapMovePosition.find(itSwap->second.nMovePos2Num);
		if(itMove1 != m_mapMovePosition.end() && itMove2 != m_mapMovePosition.end()) {
			if(itMove1->second.nState == conMoveCancel || itMove2->second.nState == conMoveCancel) {
				itSwap->second.nState = conSwapCancel;
				itMove1->second.nState = conMoveCancel;
				itMove2->second.nState = conMoveCancel;
				wxCommandEvent evt(wxEVT_MOVEPOSITION_ACTION);
				evt.SetInt(conActionCancel_TimeOut);
				if(itMove1->second.pWnd) {
					((wxEvtHandler*)itMove1->second.pWnd)->AddPendingEvent(evt);
				}
			}
		}
	}

	UnlockObject();

}
